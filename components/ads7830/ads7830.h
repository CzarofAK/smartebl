#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace ads7830 {

// ADS7830 Command byte format:
// Bit 7: Single-ended/Differential (1 = single-ended)
// Bit 6-4: Channel select (see datasheet)
// Bit 3-2: Power-down selection
// Bit 1-0: Don't care

// Single-ended channel selection (SD=1)
static const uint8_t ADS7830_CMD_SD = 0x80;  // Single-ended mode
static const uint8_t ADS7830_CMD_PD_NONE = 0x00;  // Power down between conversions
static const uint8_t ADS7830_CMD_PD_REF_OFF = 0x04;  // Internal ref OFF, ADC ON
static const uint8_t ADS7830_CMD_PD_REF_ON = 0x08;  // Internal ref ON, ADC OFF
static const uint8_t ADS7830_CMD_PD_BOTH_ON = 0x0C;  // Internal ref ON, ADC ON

// Channel mapping for single-ended mode
static const uint8_t ADS7830_CHANNEL_MAP[8] = {
    0x00,  // CH0: 0000
    0x40,  // CH1: 0100
    0x10,  // CH2: 0001
    0x50,  // CH3: 0101
    0x20,  // CH4: 0010
    0x60,  // CH5: 0110
    0x30,  // CH6: 0011
    0x70,  // CH7: 0111
};

class ADS7830Component : public Component, public i2c::I2CDevice {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  /// Read a single channel (0-7), returns raw 8-bit value
  uint8_t read_channel_raw(uint8_t channel);

  /// Read a single channel (0-7), returns voltage based on reference
  float read_channel_voltage(uint8_t channel, float reference_voltage);

 protected:
  bool is_ready_{false};
};

class ADS7830Sensor : public sensor::Sensor, public PollingComponent {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;

  void set_parent(ADS7830Component *parent) { this->parent_ = parent; }
  void set_channel(uint8_t channel) { this->channel_ = channel; }
  void set_reference_voltage(float reference_voltage) { this->reference_voltage_ = reference_voltage; }

 protected:
  ADS7830Component *parent_{nullptr};
  uint8_t channel_{0};
  float reference_voltage_{3.3f};
};

}  // namespace ads7830
}  // namespace esphome
