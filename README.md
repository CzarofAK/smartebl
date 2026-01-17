# Smart EBL - Intelligent Electroblock for Motorhomes

[![ESPHome](https://img.shields.io/badge/ESPHome-2025.12-blue.svg)](https://esphome.io/)
[![ESP32](https://img.shields.io/badge/ESP32-WROVER--IE-green.svg)](https://www.espressif.com/)
[![Home Assistant](https://img.shields.io/badge/Home%20Assistant-Compatible-brightgreen.svg)](https://www.home-assistant.io/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

**Smart Electroblock** is a modern, open-source central electrical control unit for motorhomes and campervans. It replaces legacy OEM electroblocks from manufacturers like CBE, Schaudt, Nordelectronica, Arsiliicii, and Campoli with a smart, connected solution.

**Hardware Design:** Ingenieurburo Jan Morawek, Munich

---

## Features

- **80+ Home Assistant Entities** - Complete visibility and control of your RV electrical system
- **Truma Heater Integration** - Full control of CP Plus heating via LIN bus
- **16 Fuse Monitoring Channels** - Real-time fuse status with blown detection
- **3 Tank Sensors** - Voltage-based water level monitoring (Tank 1, 2, 3)
- **Bistable Relay Control** - Energy-efficient latching relays for main switching groups
- **CAN Bus (RV-C)** - Ready for RV-C protocol integration
- **Nextion Touch Display** - 7" capacitive touch interface
- **OTA Updates** - Wireless firmware updates
- **Web Interface** - Standalone configuration and monitoring

---

## Hardware Specifications

### MCU
| Specification | Value |
|---------------|-------|
| Chip | ESP32-WROVER-IE-N16R8 |
| Flash | 16 MB |
| PSRAM | 8 MB |
| WiFi | 802.11b/g/n (up to 150 Mbps) |
| Bluetooth | 4.2 BR/EDR + BLE |
| USB Programming | CP2102N via USB-C (auto boot) |

### Power Supply
| Specification | Value |
|---------------|-------|
| Input Terminals | 4x 16mm lever terminals |
| Max Current | 60A per terminal, 80A total |
| Protection | Reverse polarity, overvoltage (up to 28V) |
| Internal Rails | 3.3V (TPS54231DR), 5V (AMS1117) |
| ADC Reference | REF3033 (3.3V precision) |

### Communication Interfaces

| Interface | Chip | Connector | Protocol |
|-----------|------|-----------|----------|
| CAN Bus | SN65HVD234 | 2x Mini-Fit | RV-C / ISO 11898-2 |
| LIN Bus | TJA1021T | RJ12 | Truma / ISO 17987 |
| Display | - | RJ45 | Nextion Serial |
| I2C | MCP23017, 4x ADS7830 | Internal | IO Expander + ADCs |

### Switching Groups

| Group | Fuses | Rating | Relay Type |
|-------|-------|--------|------------|
| Light | F4-F5 | 2x 30A | Bistable HFE10 |
| 12V | F14-F16 | 3x 30A | Bistable HFE10 |
| AUX | F11-F13 | 3x 30A | Bistable HFE10 |
| Pump | F1 | 10A | Bistable HF3F-L |
| Permanent | F6-F8 | 3x 30A | Direct |
| Fridge | F9-F10 | 2x 30A | Direct + D+ |

---

## Entity Overview

### Sensors (~40)

| Category | Entities | Description |
|----------|----------|-------------|
| Battery | 2 | Leisure battery, starter battery voltage |
| Fuses | 16 | F1-F16 voltage monitoring |
| Outputs | 8 | Light, Light L, 12V, AUX, Pump, Contour Light output voltage |
| Tanks | 3 | Tank 1, Tank 2, Tank 3 levels |
| Inputs | 5 | Duocontrol, shore power, D+ signal, F10 ABS, 3.3V bus |
| Truma | 3 | Room/water temp, error code |
| System | 3 | WiFi signal, ESP32 temp, uptime |

### Binary Sensors (~20)

| Category | Entities | Description |
|----------|----------|-------------|
| Fuse Status | 16 | F1-F16 OK/blown detection |
| Truma | 2 | CP Plus connected, has error |
| Inputs | 2 | D+ active, shore power connected |

### Controls (~20)

| Type | Entities | Description |
|------|----------|-------------|
| Switches | 10 | Water Pump, Light Group, 12V Group, AUX Group, EisEx, D+2 Relay, Fridge, Tank 1-3 DC control |
| Climate | 1 | Truma room heater |
| Number | 2 | Truma target room/water temp |
| Select | 2 | Truma energy mix, fan mode |

---

## Installation

### Prerequisites

- Python 3.9+
- ESPHome 2023.4+
- USB-C cable for initial flash

### Quick Start

To install it on your SmartEBL, you simply have to create a new device in you HomeAssistant ESPhome GUI and past the [smart-ebl.yaml](https://github.com/CzarofAK/smartebl/blob/main/smart-ebl.yaml) content.
Make sure you got the ["basic.yaml"](https://github.com/CzarofAK/smartebl/blob/main/basic.yaml.example) and ["secrets.yaml"](https://github.com/CzarofAK/smartebl/blob/main/secrets.yaml.example) in right spot and updated with your personal credentials. Examples can be found below or in the code above.

```bash
# Clone the repository
git clone https://github.com/CzarofAK/smartebl.git
cd smartebl

# Install ESPHome
pip install esphome

# Create secrets file
cp secrets.yaml.example secrets.yaml
# Edit secrets.yaml with your WiFi credentials and API keys

# Create basics.yaml for common settings
# (contains wifi, ota, logger, web_server config)

# Compile firmware
esphome compile smart-ebl.yaml

# Flash via USB (first time)
esphome upload smart-ebl.yaml

# Flash OTA (subsequent updates)
esphome upload smart-ebl.yaml --device smart-ebl.local
```

### secrets.yaml Template

```yaml
# WiFi credentials
wifi_ssid: "your-wifi-ssid"
wifi_password: "your-wifi-password"

# Home Assistant API encryption key
# Generate with: openssl rand -base64 32
api_encryption_key: "your-32-byte-base64-key"

# OTA update password
password: "your-ota-password"
```

### basics.yaml Template

```yaml
api:
  encryption:
    key: !secret api_encryption_key

ota:
  - platform: esphome
    id: ota_esphome
    password: !secret password

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password
  ap:
    ssid: ${name}
    password: "fallback-password"

captive_portal:

web_server:
  port: 80

logger:
  level: DEBUG

time:
  - platform: homeassistant
    id: homeassistant_time
```

---

## Hardware Reference

### ESP32 GPIO Pinout

| GPIO | Label | Function | Direction | Notes |
|------|-------|----------|-----------|-------|
| EN | EN | MCU Reset | Input | - |
| GPIO0 | IO0 | BOOT | Input | Auto set when USB plugged in |
| GPIO1 | TXD0 | USB Programmer | Bidir | CP2102N |
| GPIO3 | RXD0 | USB Programmer | Bidir | CP2102N |
| GPIO12 | TXD1 | LIN TX | Bidir | Truma iNetbox (TJA1021T) |
| GPIO13 | RXD1 | LIN RX | Bidir | Truma iNetbox (TJA1021T) |
| GPIO14 | RXD_CAN1 | CAN RX | Bidir | SN65HVD234 |
| GPIO15 | TXD_CAN1 | CAN TX | Bidir | SN65HVD234 |
| GPIO18 | TXD2 | Display TX | Bidir | Via TRS3221 RS-232 |
| GPIO19 | RXD2 | Display RX | Bidir | Via TRS3221 RS-232 |
| GPIO21 | SDA | I2C Data | Bidir | ADS7830 ADCs + MCP23017 |
| GPIO22 | SCL | I2C Clock | Bidir | ADS7830 ADCs + MCP23017 |
| GPIO23 | CAN_TERM | CAN Termination | Output | Software switchable |
| GPIO25 | LIN_MS | LIN Master/Slave | Output | TJA1021T control |
| GPIO26 | LIN_WAKE | LIN Wake | Output | TJA1021T control |
| GPIO27 | LIN_SLP | LIN Sleep | Output | TJA1021T control |
| GPIO32 | CAN_1_STB | CAN Standby | Output | SN65HVD234 control |
| GPIO33 | IO_RESET | MCP23017 Reset | - | External 10k pull-up (not configured in firmware) |
| GPIO34 | - | SW1 Mode | Input | Onboard button, function TBD |
| GPIO35 | GPIO35 | - | - | Not connected |
| GPIO36 | INT_A | MCP23017 INT A | Input | Pin 16 on MCP23017 |
| GPIO39 | INT_B | MCP23017 INT B | Input | Pin 15 on MCP23017 |

> **Note:** GPIO33 (IO_RESET) is not configured as an output in firmware. The external 10k pull-up resistor keeps the MCP23017 out of reset. Configuring GPIO33 as any output type causes a brief LOW pulse during initialization that resets the MCP23017.

---

### I2C Bus Overview

All ADCs and the IO expander share the I2C bus at 100kHz.

#### Device Addresses

| Address | Binary | Device | Function |
|---------|--------|--------|----------|
| 0x20 | 0100000 | MCP23017 (U17) | IO Expander - Relay control |
| 0x48 | 1001000 | ADS7830 (U4) | Fuse monitoring F1-F8 |
| 0x49 | 1001001 | ADS7830 (U5) | Fuse monitoring F9-F16 |
| 0x4A | 1001010 | ADS7830 (U6) | Tank sensors, inputs |
| 0x4B | 1001011 | ADS7830 (U7) | Output monitoring, battery |

#### ADC 0x48 (U4) - Fuse Group 1

| Channel | Label | Function | Power Source |
|---------|-------|----------|--------------|
| CH0 | F8 | Permanent power fuse | +BATT |
| CH1 | F7 | Permanent power fuse | +BATT |
| CH2 | F6 | Permanent power fuse | +BATT |
| CH3 | F5 | Light output fuse | K3 Light relay |
| CH4 | F4 | Light output fuse | K3 Light relay |
| CH5 | F3 | Awning light fuse | +BATT (disabled by D+) |
| CH6 | F2 | Contour light fuse | +BATT (switched by Contour_Light) |
| CH7 | F1 | Pump fuse | +BATT |

#### ADC 0x49 (U5) - Fuse Group 2

| Channel | Label | Function | Power Source |
|---------|-------|----------|--------------|
| CH0 | F9 | Fridge fuse | +BATT |
| CH1 | F10 | ABS Fridge fuse | +BATT (via K6 relay) |
| CH2 | F11 | AUX output fuse | K8 Aux relay |
| CH3 | F12 | AUX output fuse | K8 Aux relay |
| CH4 | F13 | AUX output fuse | K8 Aux relay |
| CH5 | F14 | 12V switch fuse | K7 12V relay |
| CH6 | F15 | 12V switch fuse | K7 12V relay |
| CH7 | F16 | 12V switch fuse | K7 12V relay |

#### ADC 0x4A (U6) - Inputs

| Channel | Label | Function | Notes |
|---------|-------|----------|-------|
| CH0 | Tank1 | Tank 1 level | Voltage-based sensor |
| CH1 | Tank2 | Tank 2 level | Voltage-based sensor |
| CH2 | Tank3 | Tank 3 level | Voltage-based sensor |
| CH3 | +3V3 | 3.3V bus monitoring | Via protection circuit |
| CH4 | ACdet | AC charger detection | Shore power status |
| CH5 | DuoC | DuoControl status | Gas system monitoring |
| CH6 | D+ | D+ signal status | K5 relay and D+ status |
| CH7 | F10_ABS | F10 ABS status | K6 relay and ABS status |

#### ADC 0x4B (U7) - Outputs

| Channel | Label | Function | Notes |
|---------|-------|----------|-------|
| CH0 | +BATT | Leisure battery | Main battery voltage |
| CH1 | VCAR | Starter battery | Vehicle battery voltage |
| CH2 | V_Light | Light output | K3 relay status |
| CH3 | V_12V_User | 12V user output | K7 relay status |
| CH4 | V_12V_Aux | 12V AUX output | K8 relay status |
| CH5 | PUMP | Pump output | K4 relay status |
| CH6 | V_L_out | Awning light output | K1 relay status |
| CH7 | C_L_out | Contour light output | K2 relay status |

---

### MCP23017 IO Expander (0x20)

All relay control is handled via the MCP23017 driving the ULN2803A Darlington array.

```
ESP32 GPIO21/22 (I2C) --> MCP23017 (0x20) --> ULN2803A (U8) --> Relay Coils
```

#### Port B (GPB0-GPB7) - Bistable Relay Coils

| Pin | ESPHome# | Signal | Function | Notes |
|-----|----------|--------|----------|-------|
| GPB0 | 8 | P_off | K4 Pump OFF coil | 100ms pulse to turn off |
| GPB1 | 9 | P_on | K4 Pump ON coil | 100ms pulse to turn on |
| GPB2 | 10 | L_off | K8 Light OFF coil | 100ms pulse to turn off |
| GPB3 | 11 | L_on | K8 Light ON coil | 100ms pulse to turn on |
| GPB4 | 12 | A_off | K7 AUX OFF coil | 100ms pulse to turn off |
| GPB5 | 13 | A_on | K7 AUX ON coil | 100ms pulse to turn on |
| GPB6 | 14 | 12_off | K3 12V OFF coil | 100ms pulse to turn off |
| GPB7 | 15 | 12_on | K3 12V ON coil | 100ms pulse to turn on |

#### Port A (GPA0-GPA5) - Additional Outputs

| Pin | ESPHome# | Signal | Function | Fuse | Notes |
|-----|----------|--------|----------|------|-------|
| GPA0 | 0 | EisX | EisEx heater | F18 | Via BTS6143D |
| GPA1 | 1 | D+2 | D+ programmable output | F23 | Toggle D+2 relay |
| GPA2 | 2 | F-CTRL | Fridge control relay K6 | - | Parallel to D+ for ABS recognition |
| GPA3 | 3 | T1_DC | Tank 1 DC mode | F22 | Inhibit readings while driving |
| GPA4 | 4 | T2_DC | Tank 2 DC mode | F22 | Inhibit readings while driving |
| GPA5 | 5 | T3_DC | Tank 3 DC mode | F22 | Inhibit readings while driving |

---

### ADC Voltage Calibration

The ADS7830 ADCs use voltage dividers to measure 12V signals. Calibration multipliers:

| Measurement | Multiplier | Notes |
|-------------|------------|-------|
| 12V signals | 6.62 | Battery, fuses, outputs |
| 3.3V bus | 1.60 | Protection circuit causes voltage drop |
| Tank levels | 0.392 | Scale 0-255 to 0-100% |

---

### PCB Connectors

#### Power Input Terminals (JM1-JM4)

| Connector | Description | Function | Notes |
|-----------|-------------|----------|-------|
| JM1 | IN2 | RV Battery input | +BATT, GND |
| JM2 | IN1 | AC Charger input | +BATT, GND |
| JM3 | IN4 | Booster input | +BATT, GND |
| JM4 | IN3 | Solar input | +BATT, GND |

#### Output Connectors (J1-J4)

| ID | Description | Pin | Function | Fuse | Notes |
|----|-------------|-----|----------|------|-------|
| J1 | ODLT | 1 | Awning Light output | F3 | Switched by D+ |
| J2 | COLT | 1 | Contour Light output | F2 | Switched by base vehicle |
| J3 | PMP | 1 | Water Pump output | F1 | ESP32 or switch controlled |
| J4 | Multi | 1 | EisEx output | F18 | - |
| J4 | Multi | 2 | DuoControl input | - | Voltage measurement |
| J4 | Multi | 3 | D+2 output | F23 | Programmable |
| J4 | Multi | 4 | D+ Relais output | F17 | Switched by D+ signal |

#### Input Connectors (J5-J8)

| ID | Description | Pin | Function | Notes |
|----|-------------|-----|----------|-------|
| J5 | CAR-BAT | 1 | Starter battery input | Fuse F20 |
| J5 | AC-DET | 2 | AC Shore power detection | - |
| J6 | D+/Contour | 1-2 | D+ Negative/Positive input | Controls awning light |
| J6 | D+/Contour | 3-4 | Contour Negative/Positive input | Controls contour light |
| J7 | Relay Ctrl | 1-2 | 12V relay ON/OFF inputs | Ground switching |
| J7 | Relay Ctrl | 3-4 | AUX relay ON/OFF inputs | Ground switching |
| J8 | Relay Ctrl | 1-2 | Light relay ON/OFF inputs | Ground switching |
| J8 | Relay Ctrl | 3-4 | Pump relay ON/OFF inputs | Ground switching |

#### Communication Connectors (J9-J14)

| ID | Type | Function | Signals |
|----|------|----------|---------|
| J9 | RJ45 | Display Panel | UART TX/RX, LIN, +BATT (F21), GND |
| J10 | RJ12 | LIN Bus | LIN, GND |
| J11 | 16-pin | Panel (Campoli retrofit) | Tanks, LIN, UART, +BATT |
| J12 | USB-C | Programming | USB data |
| J13 | Mini-Fit | CAN Bus #1 | CAN-H, CAN-L, GND, +BATT (F24) |
| J14 | Mini-Fit | CAN Bus #2 | CAN-H, CAN-L, GND, +BATT (F24) |

#### Tank Sensor Connectors (J15-J17)

| ID | Pin 1 | Pin 2 | Pin 3 | Pin 4 |
|----|-------|-------|-------|-------|
| J15 | GND | Tank 1 signal | +5V | +12V (F22) |
| J16 | GND | Tank 2 signal | +5V | +12V (F22) |
| J17 | GND | Tank 3 signal | +5V | +12V (F22) |

#### Fridge Control (J18)

| Pin | Function | Notes |
|-----|----------|-------|
| 1 | +BATT | Direct power, Fuse F9 |
| 2 | GND | - |

#### Switched Output Connectors (JP1-JP13)

| Group | Connectors | Fuses | Relay | Function |
|-------|------------|-------|-------|----------|
| Light | JP1, JP2 | F4, F5 | K8 | Switched light outputs |
| Permanent | JP3-JP5 | F6-F8 | Direct | Always-on outputs |
| Fridge | JP6 | F10 | Direct | Compressor fridge |
| Fridge | JP7 | F10 | K6 | Absorber fridge (D+ controlled) |
| AUX | JP8-JP10 | F11-F13 | K7 | Switched AUX outputs |
| 12V | JP11-JP13 | F14-F16 | K3 | Switched 12V outputs |

---

## Truma Integration

The Smart EBL integrates with Truma CP Plus heaters via LIN bus, emulating a Truma iNet Box.

### Supported Features

- **Room Heating** - Temperature control and monitoring
- **Water Heating** - Eco/Hot/Boost modes
- **Energy Mix** - Gas, electric, or mixed operation
- **Fan Control** - Ventilation and boost modes
- **Status Monitoring** - Operating status, errors, timers

### Hardware Connection

Connect the Smart EBL to the Truma CP Plus using an RJ12 cable to the LIN bus port.

| Pin | Signal |
|-----|--------|
| 1 | +12V |
| 2 | LIN |
| 3 | GND |

### External Component

Uses [skrebber/esphome-truma_inetbox](https://github.com/skrebber/esphome-truma_inetbox) - a fork with fixes for newer ESPHome versions.

---

## Project Structure

```
smartebl/
├── smart-ebl.yaml          # Main ESPHome configuration
├── secrets.yaml            # Credentials (git-ignored)
├── secrets.yaml.example    # Credentials template
├── .basics.yaml            # Common settings (git-ignored)
├── components/             # Custom ESPHome components
│   ├── ads7830/            # 8-bit I2C ADC driver
│   │   ├── __init__.py
│   │   ├── sensor.py
│   │   ├── ads7830.h
│   │   └── ads7830.cpp
│   └── lin_bus/            # LIN bus driver
│       ├── __init__.py
│       ├── lin_bus.h
│       └── lin_bus.cpp
├── information/            # Hardware documentation
│   ├── ESP32 Pinout        # GPIO pin assignments
│   ├── I2C overview        # ADC and MCP23017 channel mapping
│   └── PCB connectors      # Connector pinout reference
├── CLAUDE.md               # Development notes
└── README.md               # This file
```

---

## Custom Components

### ADS7830 - 8-bit I2C ADC

Custom ESPHome component for the Texas Instruments ADS7830 8-channel ADC.

```yaml
ads7830:
  - id: adc_fuses_1
    address: 0x48

sensor:
  - platform: ads7830
    ads7830_id: adc_fuses_1
    channel: 0
    name: "Battery Voltage"
    filters:
      - calibrate_linear:
          - 0.0 -> 0.0
          - 3.3 -> 16.0
```

### LIN Bus Driver

Generic LIN bus master/slave implementation for the TJA1021T transceiver.

```yaml
uart:
  - id: uart_lin
    tx_pin: GPIO13
    rx_pin: GPIO12
    baud_rate: 9600
    stop_bits: 2
```

---

## Home Assistant Integration

The Smart EBL automatically integrates with Home Assistant via the native ESPHome API.

### Dashboard Example

```yaml
type: entities
title: Smart EBL
entities:
  - entity: sensor.smart_ebl_leisure_battery_voltage
  - entity: sensor.smart_ebl_starter_battery_voltage
  - entity: binary_sensor.smart_ebl_shore_power_connected
  - entity: switch.smart_ebl_12v_group
  - entity: switch.smart_ebl_water_pump
  - entity: climate.smart_ebl_truma_heater
  - entity: sensor.smart_ebl_tank_1_level
```

### Automation Example

```yaml
# Turn off pump when tank 1 (fresh water) is empty
automation:
  - alias: "Low Water Warning"
    trigger:
      - platform: numeric_state
        entity_id: sensor.smart_ebl_tank_1_level
        below: 10
    action:
      - service: switch.turn_off
        entity_id: switch.smart_ebl_water_pump
      - service: notify.mobile_app
        data:
          message: "Tank 1 water level is low!"
```

---

## Build Information

| Metric | Value |
|--------|-------|
| RAM Usage | ~11% (36KB / 328KB) |
| Flash Usage | ~13% (1.07MB / 8MB) |
| Compile Time | ~7-8 minutes |
| Entities | 80+ |

---

## Planned Features

- [ ] RV-C protocol handlers
- [ ] Nextion display UI pages
- [ ] Tank calibration wizard
- [ ] CI-BUS (CIVD) support
- [ ] Bluetooth proxy for BLE sensors

---

## Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## Acknowledgments

- [ESPHome](https://esphome.io/) - Amazing firmware framework
- [Home Assistant](https://www.home-assistant.io/) - Home automation platform
- [Daniel Fett](https://github.com/danielfett/inetbox.py) - Original Truma protocol decode
- [Fabian Schmidt](https://github.com/Fabian-Schmidt/esphome-truma_inetbox) - ESPHome Truma component
- [skrebber](https://github.com/skrebber/esphome-truma_inetbox) - Truma component fixes

---

## Support

- **Issues:** [GitHub Issues](https://github.com/CzarofAK/smartebl/issues)
- **Discussions:** [GitHub Discussions](https://github.com/CzarofAK/smartebl/discussions)

---

*Made with care for the RV community*
