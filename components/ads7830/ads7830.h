#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace ads7830 {

/// ADS7830 8-bit 8-channel I2C ADC
///
/// Command byte format (single-ended mode):
/// Bit 7 (SD):     1 = Single-ended
/// Bits 6-4 (C2-C0): Channel select (non-linear mapping!)
/// Bits 3-2 (PD1-PD0): Power-down selection
/// Bits 1-0:       Don't care
///
/// Channel mapping for single-ended mode (from datasheet):
/// CH0=0x00, CH1=0x40, CH2=0x10, CH3=0x50
/// CH4=0x20, CH5=0x60, CH6=0x30, CH7=0x70

class ADS7830Component : public Component, public i2c::I2CDevice {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  /// Read a single channel (0-7), returns raw 8-bit value
  uint8_t read_channel_raw(uint8_t channel);

  /// Read a single channel (0-7), returns voltage
  float read_channel_voltage(uint8_t channel, float vref);

 protected:
  /// Build command byte for given channel
  uint8_t get_command_byte_(uint8_t channel);
};

class ADS7830Sensor : public sensor::Sensor, public PollingComponent {
 public:
  void update() override;
  void dump_config() override;

  void set_parent(ADS7830Component *parent) { parent_ = parent; }
  void set_channel(uint8_t channel) { channel_ = channel; }
  void set_reference_voltage(float vref) { vref_ = vref; }

 protected:
  ADS7830Component *parent_{nullptr};
  uint8_t channel_{0};
  float vref_{3.3f};
};

}  // namespace ads7830
}  // namespace esphome
