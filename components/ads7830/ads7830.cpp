#include "ads7830.h"
#include "esphome/core/log.h"

namespace esphome {
namespace ads7830 {

static const char *TAG = "ads7830";

// Single-ended mode: SD=1 (0x80), PD=01 (0x04) = internal ref OFF, ADC ON
static const uint8_t CMD_BASE = 0x84;

// Channel mapping for single-ended mode (non-linear per datasheet)
// The channel bits C2:C1:C0 map to physical channels as follows:
static const uint8_t CHANNEL_MAP[8] = {
    0x00,  // CH0: C2C1C0 = 000
    0x40,  // CH1: C2C1C0 = 100
    0x10,  // CH2: C2C1C0 = 001
    0x50,  // CH3: C2C1C0 = 101
    0x20,  // CH4: C2C1C0 = 010
    0x60,  // CH5: C2C1C0 = 110
    0x30,  // CH6: C2C1C0 = 011
    0x70,  // CH7: C2C1C0 = 111
};

void ADS7830Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up ADS7830 at address 0x%02X...", this->address_);

  // Test communication by reading channel 0
  uint8_t cmd = this->get_command_byte_(0);
  uint8_t value;

  if (this->write(&cmd, 1) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "I2C write failed - check wiring and address");
    this->mark_failed();
    return;
  }

  if (this->read(&value, 1) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "I2C read failed");
    this->mark_failed();
    return;
  }

  ESP_LOGCONFIG(TAG, "ADS7830 initialized (test read: %u)", value);
}

void ADS7830Component::dump_config() {
  ESP_LOGCONFIG(TAG, "ADS7830:");
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "  Communication failed!");
  }
}

uint8_t ADS7830Component::get_command_byte_(uint8_t channel) {
  if (channel > 7) {
    ESP_LOGW(TAG, "Invalid channel %u, using 0", channel);
    channel = 0;
  }
  return CMD_BASE | CHANNEL_MAP[channel];
}

uint8_t ADS7830Component::read_channel_raw(uint8_t channel) {
  uint8_t cmd = this->get_command_byte_(channel);
  uint8_t value = 0;

  if (this->write(&cmd, 1) != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Write failed for channel %u", channel);
    return 0;
  }

  if (this->read(&value, 1) != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Read failed for channel %u", channel);
    return 0;
  }

  return value;
}

float ADS7830Component::read_channel_voltage(uint8_t channel, float vref) {
  uint8_t raw = this->read_channel_raw(channel);
  return (static_cast<float>(raw) / 255.0f) * vref;
}

// Sensor implementation

void ADS7830Sensor::update() {
  if (this->parent_ == nullptr) {
    ESP_LOGW(TAG, "No parent component set");
    return;
  }

  float voltage = this->parent_->read_channel_voltage(this->channel_, this->vref_);
  ESP_LOGD(TAG, "CH%u: %.3f V", this->channel_, voltage);
  this->publish_state(voltage);
}

void ADS7830Sensor::dump_config() {
  LOG_SENSOR("", "ADS7830 Sensor", this);
  ESP_LOGCONFIG(TAG, "  Channel: %u", this->channel_);
  ESP_LOGCONFIG(TAG, "  Reference: %.2f V", this->vref_);
}

}  // namespace ads7830
}  // namespace esphome
