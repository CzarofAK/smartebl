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

## Hardware Architecture

### Relay Control via MCP23017 IO Expander

All relay control is handled via the **MCP23017 (U17)** I2C IO expander at address **0x20**, driving the **ULN2803A (U8)** Darlington array which powers the relay coils.

```
ESP32 GPIO21/22 (I2C) --> MCP23017 (0x20) --> ULN2803A --> Relay Coils
```

### MCP23017 Pin Mapping

**Port B (pins 8-15) - Relay Coils via ULN2803A:**
| Pin | ESPHome# | Signal | Function |
|-----|----------|--------|----------|
| GPB0 | 8 | P_off | K4 Pump OFF coil |
| GPB1 | 9 | P_on | K4 Pump ON coil |
| GPB2 | 10 | L_off | K8 Light OFF coil |
| GPB3 | 11 | L_on | K8 Light ON coil |
| GPB4 | 12 | A_off | K7 AUX OFF coil |
| GPB5 | 13 | A_on | K7 AUX ON coil |
| GPB6 | 14 | 12_off | K3 12V OFF coil |
| GPB7 | 15 | 12_on | K3 12V ON coil |

**Port A (pins 0-5) - Additional Outputs:**
| Pin | ESPHome# | Signal | Function |
|-----|----------|--------|----------|
| GPA0 | 0 | EisX | EisEx heater |
| GPA1 | 1 | D+2 | D+ relay |
| GPA2 | 2 | F-CTRL | Fridge relay |
| GPA3-5 | 3-5 | T1/T2/T3_DC | Tank DC mode |

### ESP32 GPIO Mapping

| GPIO | Label | Function | Notes |
|------|-------|----------|-------|
| GPIO12 | TXD1 | LIN TX | Truma (TJA1021T) |
| GPIO13 | RXD1 | LIN RX | Truma (TJA1021T) |
| GPIO14 | RXD_CAN1 | CAN RX | SN65HVD234 |
| GPIO15 | TXD_CAN1 | CAN TX | SN65HVD234 |
| GPIO18 | TXD2 | Display TX | TRS3221 RS-232 |
| GPIO19 | RXD2 | Display RX | TRS3221 RS-232 |
| GPIO21 | SDA | I2C Data | MCP23017 + ADCs |
| GPIO22 | SCL | I2C Clock | MCP23017 + ADCs |
| GPIO23 | CAN_TERM | CAN Term | Software switchable |
| GPIO25-27 | LIN_* | LIN Control | MS/WAKE/SLP |
| GPIO32 | CAN_1_STB | CAN Standby | |
| GPIO33 | IO_RESET | MCP23017 Reset | |
| GPIO36 | INT_A | MCP23017 INT A | |
| GPIO39 | INT_B | MCP23017 INT B | |

### I2C Bus Devices

| Address | Device | Function |
|---------|--------|----------|
| 0x20 | MCP23017 (U17) | IO Expander - Relay control |
| 0x48 | ADS7830 (U4) | Fuse monitoring group 1 |
| 0x49 | ADS7830 (U5) | Fuse monitoring group 2 |
| 0x4A | ADS7830 (U6) | Input monitoring |
| 0x4B | ADS7830 (U7) | Output monitoring |

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
