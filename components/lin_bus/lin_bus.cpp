#include "lin_bus.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace lin_bus {

static const char *const TAG = "lin_bus";

// Timeout for receiving a complete frame (ms)
static const uint32_t RX_TIMEOUT_MS = 50;

void LINBusComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up LIN Bus...");

  // Configure sleep pin if set (TJA1021T ~SLP pin)
  if (this->sleep_pin_ >= 0) {
    pinMode(this->sleep_pin_, OUTPUT);
    digitalWrite(this->sleep_pin_, HIGH);  // Active low, so HIGH = not sleeping
  }

  // Configure wake pin if set (TJA1021T ~WAKE pin)
  if (this->wake_pin_ >= 0) {
    pinMode(this->wake_pin_, OUTPUT);
    digitalWrite(this->wake_pin_, HIGH);  // Active low
  }

  this->is_awake_ = true;
  this->last_activity_ = millis();

  ESP_LOGCONFIG(TAG, "LIN Bus initialized as %s", this->is_master_ ? "MASTER" : "SLAVE");
}

void LINBusComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "LIN Bus:");
  ESP_LOGCONFIG(TAG, "  Mode: %s", this->is_master_ ? "Master" : "Slave");
  ESP_LOGCONFIG(TAG, "  Node Address: 0x%02X", this->node_address_);
  if (this->sleep_pin_ >= 0) {
    ESP_LOGCONFIG(TAG, "  Sleep Pin: GPIO%d", this->sleep_pin_);
  }
  if (this->wake_pin_ >= 0) {
    ESP_LOGCONFIG(TAG, "  Wake Pin: GPIO%d", this->wake_pin_);
  }
}

void LINBusComponent::loop() {
  // Read any available data
  while (this->available()) {
    uint8_t byte;
    if (this->read_byte(&byte)) {
      this->rx_buffer_.push_back(byte);
      this->rx_timeout_ = millis() + RX_TIMEOUT_MS;
      this->last_activity_ = millis();
    }
  }

  // Check for timeout and process buffer
  if (!this->rx_buffer_.empty() && millis() > this->rx_timeout_) {
    this->process_received_data();
    this->rx_buffer_.clear();
  }
}

void LINBusComponent::send_break() {
  // To send a break, we need to send a low signal for at least 13 bit times
  // At 19200 baud, that's about 677us minimum
  // We achieve this by temporarily changing baud rate or using a GPIO

  // Method: Send 0x00 at a lower baud rate to create break
  // For a proper break, we'd need to go even lower, but 0x00 at current rate
  // followed by sync can work for many devices

  // Alternative: Use UART break function if available
  // For ESP32, we can use uart_set_line_inverse or manual GPIO control

  ESP_LOGV(TAG, "Sending break signal");

  // Simple approach: just send 0x00 which gives us 9 low bits (start + 8 data)
  // followed by a high bit (stop). Not a perfect break but works for many devices.
  this->write_byte(0x00);
  this->flush();

  // Wait for break to complete
  delayMicroseconds(LIN_BREAK_DURATION_US);
}

uint8_t LINBusComponent::calculate_pid(uint8_t id) {
  // ID must be 0-63 (6 bits)
  id &= 0x3F;

  // Calculate parity bits P0 and P1
  // P0 = ID0 XOR ID1 XOR ID2 XOR ID4
  // P1 = NOT(ID1 XOR ID3 XOR ID4 XOR ID5)
  uint8_t p0 = ((id >> 0) ^ (id >> 1) ^ (id >> 2) ^ (id >> 4)) & 0x01;
  uint8_t p1 = (~((id >> 1) ^ (id >> 3) ^ (id >> 4) ^ (id >> 5))) & 0x01;

  return id | (p0 << 6) | (p1 << 7);
}

uint8_t LINBusComponent::calculate_checksum(uint8_t pid, const std::vector<uint8_t> &data,
                                            LINChecksumType type) {
  uint16_t sum = 0;

  // Enhanced checksum includes PID
  if (type == LINChecksumType::ENHANCED) {
    sum = pid;
  }

  // Add all data bytes
  for (uint8_t byte : data) {
    sum += byte;
    // Handle carry
    if (sum > 255) {
      sum = (sum & 0xFF) + 1;
    }
  }

  // Invert the result
  return ~(sum & 0xFF);
}

void LINBusComponent::send_header(uint8_t id) {
  if (!this->is_master_) {
    ESP_LOGW(TAG, "Only master can send headers!");
    return;
  }

  ESP_LOGD(TAG, "Sending header for ID: 0x%02X", id);

  // 1. Send break
  this->send_break();

  // 2. Send sync byte
  this->write_byte(LIN_SYNC_BYTE);

  // 3. Send protected ID
  uint8_t pid = this->calculate_pid(id);
  this->write_byte(pid);

  this->flush();
  this->last_activity_ = millis();
}

void LINBusComponent::send_response(const std::vector<uint8_t> &data,
                                    LINChecksumType checksum_type) {
  if (data.empty() || data.size() > 8) {
    ESP_LOGW(TAG, "Invalid data length: %d (must be 1-8)", data.size());
    return;
  }

  ESP_LOGD(TAG, "Sending response with %d bytes", data.size());

  // Send data bytes
  for (uint8_t byte : data) {
    this->write_byte(byte);
  }

  // Note: For proper checksum calculation in response-only mode,
  // we'd need to know the PID. For now, use classic checksum.
  uint8_t checksum = this->calculate_checksum(0, data, LINChecksumType::CLASSIC);
  this->write_byte(checksum);

  this->flush();
  this->last_activity_ = millis();
}

void LINBusComponent::send_frame(uint8_t id, const std::vector<uint8_t> &data,
                                 LINChecksumType checksum_type) {
  if (!this->is_master_) {
    ESP_LOGW(TAG, "Only master can send full frames!");
    return;
  }

  if (data.empty() || data.size() > 8) {
    ESP_LOGW(TAG, "Invalid data length: %d (must be 1-8)", data.size());
    return;
  }

  ESP_LOGD(TAG, "Sending frame ID: 0x%02X with %d bytes", id, data.size());

  // 1. Send break
  this->send_break();

  // 2. Send sync byte
  this->write_byte(LIN_SYNC_BYTE);

  // 3. Send protected ID
  uint8_t pid = this->calculate_pid(id);
  this->write_byte(pid);

  // 4. Send data bytes
  for (uint8_t byte : data) {
    this->write_byte(byte);
  }

  // 5. Send checksum
  uint8_t checksum = this->calculate_checksum(pid, data, checksum_type);
  this->write_byte(checksum);

  this->flush();
  this->last_activity_ = millis();
}

void LINBusComponent::wake_up() {
  if (this->is_awake_) {
    return;
  }

  ESP_LOGD(TAG, "Waking up LIN bus");

  // Assert wake pin if available
  if (this->wake_pin_ >= 0) {
    digitalWrite(this->wake_pin_, LOW);
    delayMicroseconds(250);  // Wake pulse
    digitalWrite(this->wake_pin_, HIGH);
  }

  // Deassert sleep pin
  if (this->sleep_pin_ >= 0) {
    digitalWrite(this->sleep_pin_, HIGH);
  }

  // Send wake-up signal (dominant pulse)
  this->write_byte(0x80);
  this->flush();

  delay(100);  // Wait for slaves to wake up
  this->is_awake_ = true;
}

void LINBusComponent::go_to_sleep() {
  ESP_LOGD(TAG, "Putting LIN bus to sleep");

  // Send sleep command (master request frame with ID 0x3C)
  if (this->is_master_) {
    std::vector<uint8_t> sleep_data = {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    this->send_frame(0x3C, sleep_data, LINChecksumType::CLASSIC);
  }

  delay(10);

  // Assert sleep pin
  if (this->sleep_pin_ >= 0) {
    digitalWrite(this->sleep_pin_, LOW);
  }

  this->is_awake_ = false;
}

void LINBusComponent::register_frame_callback(uint8_t id, LINFrameCallback callback) {
  this->frame_callbacks_[id] = callback;
}

void LINBusComponent::process_received_data() {
  if (this->rx_buffer_.size() < 2) {
    return;  // Not enough data
  }

  ESP_LOGV(TAG, "Processing %d received bytes", this->rx_buffer_.size());

  // Look for sync byte
  size_t sync_pos = 0;
  bool found_sync = false;
  for (size_t i = 0; i < this->rx_buffer_.size(); i++) {
    if (this->rx_buffer_[i] == LIN_SYNC_BYTE) {
      sync_pos = i;
      found_sync = true;
      break;
    }
  }

  if (!found_sync || sync_pos + 1 >= this->rx_buffer_.size()) {
    ESP_LOGV(TAG, "No valid sync byte found");
    return;
  }

  // Get PID
  uint8_t pid = this->rx_buffer_[sync_pos + 1];
  uint8_t id = pid & 0x3F;

  // Verify PID parity
  if (this->calculate_pid(id) != pid) {
    ESP_LOGW(TAG, "PID parity error: received 0x%02X, expected 0x%02X", pid, this->calculate_pid(id));
    return;
  }

  // Extract data (everything after PID except last byte which is checksum)
  size_t data_start = sync_pos + 2;
  if (data_start >= this->rx_buffer_.size()) {
    ESP_LOGV(TAG, "No data in frame");
    return;
  }

  size_t data_len = this->rx_buffer_.size() - data_start - 1;  // -1 for checksum
  if (data_len > 8) {
    data_len = 8;  // Max 8 data bytes
  }

  std::vector<uint8_t> data;
  for (size_t i = 0; i < data_len; i++) {
    data.push_back(this->rx_buffer_[data_start + i]);
  }

  uint8_t received_checksum = this->rx_buffer_.back();

  // Create frame structure
  LINFrame frame;
  frame.id = id;
  frame.pid = pid;
  frame.data = data;
  frame.checksum = received_checksum;
  frame.checksum_type = LINChecksumType::ENHANCED;

  // Verify checksum (try both types)
  uint8_t calc_enhanced = this->calculate_checksum(pid, data, LINChecksumType::ENHANCED);
  uint8_t calc_classic = this->calculate_checksum(pid, data, LINChecksumType::CLASSIC);

  if (received_checksum != calc_enhanced && received_checksum != calc_classic) {
    ESP_LOGW(TAG, "Checksum error for ID 0x%02X: received 0x%02X, expected 0x%02X or 0x%02X",
             id, received_checksum, calc_enhanced, calc_classic);
    return;
  }

  ESP_LOGD(TAG, "Received valid frame ID: 0x%02X with %d bytes", id, data.size());

  // Call registered callback
  auto it = this->frame_callbacks_.find(id);
  if (it != this->frame_callbacks_.end()) {
    it->second(frame);
  }
}

void LINBusComponent::truma_request_status() {
  // Truma heaters typically respond to specific frame IDs
  // Common Truma LIN IDs:
  // 0x20: Heater status request
  // 0x21: Heater status response
  // 0x22: Temperature setpoint

  ESP_LOGD(TAG, "Requesting Truma heater status");
  this->send_header(0x20);

  // The slave (Truma heater) will respond with data
}

}  // namespace lin_bus
}  // namespace esphome
