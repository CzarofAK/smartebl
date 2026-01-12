#include "ads7830.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace ads7830 {

static const char *const TAG = "ads7830";

void ADS7830Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up ADS7830...");

  // Test communication by reading channel 0
  uint8_t cmd = ADS7830_CMD_SD | ADS7830_CHANNEL_MAP[0] | ADS7830_CMD_PD_BOTH_ON;
  uint8_t value;

  if (this->write(&cmd, 1) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "Communication with ADS7830 failed!");
    this->mark_failed();
    return;
  }

  delay(1);  // Wait for conversion

  if (this->read(&value, 1) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "Failed to read from ADS7830!");
    this->mark_failed();
    return;
  }

  this->is_ready_ = true;
  ESP_LOGCONFIG(TAG, "ADS7830 initialized successfully");
}

void ADS7830Component::dump_config() {
  ESP_LOGCONFIG(TAG, "ADS7830:");
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with ADS7830 failed!");
  }
}

uint8_t ADS7830Component::read_channel_raw(uint8_t channel) {
  if (channel > 7) {
    ESP_LOGE(TAG, "Invalid channel: %d (must be 0-7)", channel);
    return 0;
  }

  uint8_t cmd = ADS7830_CMD_SD | ADS7830_CHANNEL_MAP[channel] | ADS7830_CMD_PD_BOTH_ON;
  uint8_t value = 0;

  if (this->write(&cmd, 1) != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Failed to write command to ADS7830");
    return 0;
  }

  delay(1);  // Wait for conversion (typically 2us, but add margin)

  if (this->read(&value, 1) != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Failed to read from ADS7830");
    return 0;
  }

  return value;
}

float ADS7830Component::read_channel_voltage(uint8_t channel, float reference_voltage) {
  uint8_t raw = this->read_channel_raw(channel);
  return (static_cast<float>(raw) / 255.0f) * reference_voltage;
}

// ADS7830Sensor implementation

void ADS7830Sensor::setup() {
  ESP_LOGCONFIG(TAG, "Setting up ADS7830 Sensor for channel %d...", this->channel_);
}

void ADS7830Sensor::update() {
  if (this->parent_ == nullptr) {
    ESP_LOGW(TAG, "Parent ADS7830 component not set!");
    return;
  }

  float voltage = this->parent_->read_channel_voltage(this->channel_, this->reference_voltage_);
  ESP_LOGD(TAG, "Channel %d: %.3f V", this->channel_, voltage);
  this->publish_state(voltage);
}

void ADS7830Sensor::dump_config() {
  LOG_SENSOR("", "ADS7830 Sensor", this);
  ESP_LOGCONFIG(TAG, "  Channel: %d", this->channel_);
  ESP_LOGCONFIG(TAG, "  Reference Voltage: %.2f V", this->reference_voltage_);
}

}  // namespace ads7830
}  // namespace esphome
