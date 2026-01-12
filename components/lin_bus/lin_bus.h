#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include <vector>

namespace esphome {
namespace lin_bus {

// LIN protocol constants
static const uint8_t LIN_SYNC_BYTE = 0x55;
static const uint8_t LIN_BREAK_DURATION_US = 730;  // Minimum 13 bit times at 19200 baud
static const uint32_t LIN_BAUD_RATE = 19200;

// LIN frame types
enum class LINFrameType : uint8_t {
  UNCONDITIONAL = 0,
  EVENT_TRIGGERED = 1,
  SPORADIC = 2,
  DIAGNOSTIC = 3,
};

// LIN checksum types
enum class LINChecksumType : uint8_t {
  CLASSIC = 0,    // LIN 1.x: checksum over data only
  ENHANCED = 1,   // LIN 2.x: checksum over PID + data
};

// LIN frame structure
struct LINFrame {
  uint8_t id;                    // Frame ID (0-63)
  uint8_t pid;                   // Protected ID (ID with parity bits)
  std::vector<uint8_t> data;     // Data bytes (0-8)
  uint8_t checksum;              // Checksum byte
  LINChecksumType checksum_type;
};

// Callback type for received frames
using LINFrameCallback = std::function<void(const LINFrame &)>;

class LINBusComponent : public Component, public uart::UARTDevice {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::BUS; }

  // Configuration
  void set_sleep_pin(int pin) { this->sleep_pin_ = pin; }
  void set_wake_pin(int pin) { this->wake_pin_ = pin; }
  void set_node_address(uint8_t address) { this->node_address_ = address; }
  void set_is_master(bool is_master) { this->is_master_ = is_master; }

  // LIN operations
  void send_frame(uint8_t id, const std::vector<uint8_t> &data,
                  LINChecksumType checksum_type = LINChecksumType::ENHANCED);
  void send_header(uint8_t id);
  void send_response(const std::vector<uint8_t> &data,
                     LINChecksumType checksum_type = LINChecksumType::ENHANCED);

  // Wake/Sleep control
  void wake_up();
  void go_to_sleep();

  // Register callback for received frames
  void register_frame_callback(uint8_t id, LINFrameCallback callback);

  // Truma-specific commands (common in motorhome applications)
  void truma_request_status();

 protected:
  // Send break signal
  void send_break();

  // Calculate protected ID (add parity bits)
  uint8_t calculate_pid(uint8_t id);

  // Calculate checksum
  uint8_t calculate_checksum(uint8_t pid, const std::vector<uint8_t> &data,
                             LINChecksumType type);

  // Parse received data
  void process_received_data();

  // Configuration
  int sleep_pin_{-1};
  int wake_pin_{-1};
  uint8_t node_address_{0x00};
  bool is_master_{true};

  // State
  bool is_awake_{false};
  uint32_t last_activity_{0};

  // Receive buffer
  std::vector<uint8_t> rx_buffer_;
  uint32_t rx_timeout_{0};

  // Frame callbacks
  std::map<uint8_t, LINFrameCallback> frame_callbacks_;
};

}  // namespace lin_bus
}  // namespace esphome
