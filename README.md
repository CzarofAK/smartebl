# Smart EBL - Intelligent Electroblock for Motorhomes

[![ESPHome](https://img.shields.io/badge/ESPHome-2025.12-blue.svg)](https://esphome.io/)
[![ESP32](https://img.shields.io/badge/ESP32-WROVER--IE-green.svg)](https://www.espressif.com/)
[![Home Assistant](https://img.shields.io/badge/Home%20Assistant-Compatible-brightgreen.svg)](https://www.home-assistant.io/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

**Smart Electroblock** is a modern, open-source central electrical control unit for motorhomes and campervans. It replaces legacy OEM electroblocks from manufacturers like CBE, Schaudt, Nordelectronica, Arsiliicii, and Campoli with a smart, connected solution.

**Hardware Design:** Ingenieurburo Jan Morawek, Munich

---

## Features

- **70+ Home Assistant Entities** - Complete visibility and control of your RV electrical system
- **Truma Heater Integration** - Full control of CP Plus heating via LIN bus
- **16 Fuse Monitoring Channels** - Real-time fuse status with blown detection
- **3 Tank Sensors** - Fresh, grey, and black water level monitoring
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
| I2C | - | Internal | 4x ADS7830 ADCs |

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

### Sensors (~35)

| Category | Entities | Description |
|----------|----------|-------------|
| Battery | 2 | Main battery, starter battery voltage |
| Fuses | 16 | F1-F16 voltage monitoring |
| Outputs | 5 | Light, 12V, AUX, Pump, EisEx output voltage |
| Tanks | 3 | Fresh, grey, black water levels |
| Inputs | 4 | Duocontrol, shore power, analog inputs |
| Truma | 5 | Room/water temp, heating mode, status, errors |
| System | 3 | WiFi signal, ESP32 temp, uptime |

### Binary Sensors (~25)

| Category | Entities | Description |
|----------|----------|-------------|
| Fuse Status | 16 | F1-F16 OK/blown detection |
| Truma | 7 | Connected, heater active, gas/electric mode, errors |
| Inputs | 5 | D+ signal, shore power, buttons |

### Controls (~15)

| Type | Entities | Description |
|------|----------|-------------|
| Switches | 4 | 12V group, AUX group, Pump, EisEx, CAN termination |
| Climate | 2 | Truma room heating, water heating |
| Number | 3 | Truma target temps, electric power level |
| Select | 2 | Truma energy mix, fan mode |

---

## Installation

### Prerequisites

- Python 3.9+
- ESPHome 2023.4+
- USB-C cable for initial flash

### Quick Start

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
ota:
  - platform: esphome
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

## GPIO Mapping

| GPIO | Function | Notes |
|------|----------|-------|
| GPIO0 | Mode Button | Pull-up, active low (boot) |
| GPIO2 | CAN Termination | Software switchable |
| GPIO4 | CAN RX | SN65HVD234 |
| GPIO5 | CAN TX | SN65HVD234 |
| GPIO12 | LIN RX | TJA1021T (Truma) |
| GPIO13 | LIN TX | TJA1021T (Truma) |
| GPIO14 | 12V Relay ON | via ULN2803A |
| GPIO15 | EisEx Output | via BTS6143D |
| GPIO16 | Nextion RX | UART1 |
| GPIO17 | Nextion TX | UART1 |
| GPIO21 | I2C SDA | ADS7830 ADCs |
| GPIO22 | I2C SCL | ADS7830 ADCs |
| GPIO25 | AUX Relay OFF | via ULN2803A |
| GPIO26 | AUX Relay ON | via ULN2803A |
| GPIO27 | 12V Relay OFF | via ULN2803A |
| GPIO32 | Pump Relay ON | Bistable |
| GPIO33 | Pump Relay OFF | Bistable |
| GPIO34 | D+ Signal | Input only |
| GPIO35 | Shore Power | Input only |
| GPIO36 | Pump ON Button | Input only |
| GPIO39 | Pump OFF Button | Input only |

### I2C ADC Addresses

| Address | Function |
|---------|----------|
| 0x48 | Fuse group 1 (Battery, Starter, F4-F8, F1) |
| 0x49 | Fuse group 2 (F2-F3, F9-F14) |
| 0x4A | Fuse group 3 (F15-F16, outputs) |
| 0x4B | Tanks and inputs (water levels, Duocontrol) |

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
├── basics.yaml             # Common settings (git-ignored)
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
  - entity: sensor.smart_ebl_battery_voltage
  - entity: sensor.smart_ebl_starter_battery_voltage
  - entity: binary_sensor.smart_ebl_shore_power
  - entity: switch.smart_ebl_12v_group
  - entity: switch.smart_ebl_pump
  - entity: climate.smart_ebl_truma_room_heating
  - entity: sensor.smart_ebl_fresh_water_tank
```

### Automation Example

```yaml
# Turn off pump when fresh water is empty
automation:
  - alias: "Low Water Warning"
    trigger:
      - platform: numeric_state
        entity_id: sensor.smart_ebl_fresh_water_tank
        below: 10
    action:
      - service: switch.turn_off
        entity_id: switch.smart_ebl_pump
      - service: notify.mobile_app
        data:
          message: "Fresh water tank is low!"
```

---

## Build Information

| Metric | Value |
|--------|-------|
| RAM Usage | ~11% (36KB / 328KB) |
| Flash Usage | ~13% (1.07MB / 8MB) |
| Compile Time | ~7-8 minutes |
| Entities | 70+ |

---

## Planned Features

- [ ] RV-C protocol handlers
- [ ] Nextion display UI pages
- [ ] Tank calibration wizard
- [ ] CI-BUS (CIVD) support
- [ ] Bluetooth proxy for BLE sensors
- [ ] Energy consumption tracking

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
