# Smart EBL Project

Smart Electroblock - Central electrical control unit for motorhomes/campervans.
Replaces OEM electroblocks from CBE, Schaudt, Nordelectronica, Arsiliicii, Campoli, etc.

**Author:** Ingenieurbüro Jan Morawek, Munich

## Key Documents

- `Smart EBL V1.0 Datenblatt.pdf` - Hardware datasheet (German)
- `E-Block.pdf` - KiCad schematic

## Hardware Overview

### MCU
- **ESP32-WROVER-IE-N16R8**
  - 16 MB Flash
  - 8 MB PSRAM
  - WiFi 802.11b/g/n (up to 150 Mbps)
  - Bluetooth 4.2 BR/EDR + BLE
- **USB Programming:** CP2102N via USB-C (automatic boot mode)

### Display
- **Nextion NX8048P070-011C**
  - 7" capacitive touch
  - 800x480 resolution, 65536 colors
  - Connected via RJ45 (RS232 + LIN + power)
  - Alternative: 16-pin flat cable for Campoli retrofits

### Power Supply
- **Input:** 4x 16mm² lever terminals, each up to 60A, total max 80A
- **Internal:** TPS54231DR DC-DC for 3.3V, AMS1117-5.0 for 5V
- **Protection:** Reverse polarity and overvoltage protected up to 28V (electronics only, not relays)
- **Reference:** REF3033 (3.3V precision reference for ADC)

## Communication Interfaces

### CAN Bus
- **Transceiver:** SN65HVD234
- **Connectors:** 2x Mini-Fit (active parallel)
- **Standard:** RV-C, ISO 11898-2
- **Termination:** Software switchable (via GPIO)
- **Signals:** CAN-H, CAN-L, CAN_TERM, TXD_CAN1, RXD_CAN1, CAN_1_STB

### LIN Bus
- **Transceiver:** TJA1021T
- **Connector:** RJ12 (Truma standard)
- **Standard:** ISO 17987 / LIN 2.x / SAE J2602
- **Mode:** Master/Slave capable
- **Protection:** ESDLIN1524BJ

### Serial (Nextion Display)
- **Connector:** RJ45 (shielded)
- **Signals:** RS232 TX/RX, LIN, 12V power

## I2C Bus

### ADC Monitoring (4x ADS7830)
8-bit, 8-channel ADCs for voltage monitoring:

| Address | Function |
|---------|----------|
| 0x48 (1001000) | Fuse monitoring group 1 |
| 0x49 (1001001) | Fuse monitoring group 2 |
| 0x4A (1001010) | Fuse monitoring group 3 |
| 0x4B (1001011) | Tank sensors, inputs |

Monitors: all fuses, outputs, starter battery, tank sensors, transition resistances.

## Outputs

### Switching Groups (Bistable Relays - HFE10)
Self-latching, manual override on relay, direct control inputs available.

| Group | Fuses | Rating | Connector |
|-------|-------|--------|-----------|
| Light | F4-F5 | 2x 30A | 6mm² lever |
| 12V | F14-F16 | 3x 30A | 6mm² lever |
| AUX | F11-F13 | 3x 30A | 6mm² lever |

### Permanent Outputs
| Fuses | Rating | Connector |
|-------|--------|-----------|
| F6-F8 | 3x 30A | 6mm² lever |

### Fridge (Kühlschrank)
| Fuse | Function | Rating |
|------|----------|--------|
| F9 | Permanent | 30A |
| F10 | D+ controlled or programmable | 30A |
| - | Control voltage | 5A (4mm²) |

### Special Outputs
| Output | Fuse | Rating | Notes |
|--------|------|--------|-------|
| Contour light | F2 | 10A | Controlled by base vehicle |
| Awning light | F3 | 10A | Disabled via D+ |
| Water pump | F1 | 10A | Bistable relay (HF3F-L) + BTS6143D |
| EisEx heater | Multifuse | 2A | Via BTS6143D smart switch |
| D+ out (vehicle) | Multifuse | 2A | Activated by vehicle D+ |
| D+ out (prog.) | Multifuse | 2A | Freely programmable |

### Relay Driver
- **ULN2803A** - 8-channel Darlington driver for relay coils

## Inputs

### Voltage Sensing (up to 16V)
| Input | Fuse | Function |
|-------|------|----------|
| Duocontrol | - | Gas system status |
| Shore power | - | AC charger detection |
| Starter battery | Multifuse 0.5A | Vehicle battery monitoring |

### Digital Inputs
| Input | Count | Type |
|-------|-------|------|
| D+ (positive) | 2 | Voltage sense |
| D+ (negative) | 2 | Voltage sense |
| Contour light (pos/neg) | 2 | Voltage sense |
| Relay control | 8 | Active low (ground switching) |

### Tank Sensors (3x)
- **Compatible:** CBE, Votronic, Supersense, Campoli
- **Input range:** 0-3.3V
- **Modes:** Voltage measurement or resistance measurement (switchable)
- **Calibration:** Built-in function for any tank shape

## Key Components (from Schematic)

| Ref | Part | Function |
|-----|------|----------|
| U1 | BTS6143D | Smart high-side switch (EisEx/pump) |
| U2 | AMS1117-5.0 | 5V LDO regulator |
| U3 | REF3033 | 3.3V precision reference |
| U4-U7 | ADS7830 | 8-ch 8-bit I2C ADC |
| U8 | ULN2803A | Relay driver (8ch Darlington) |
| U9 | TPS54231DR | 3.3V DC-DC converter |
| U10 | SN65HVD234 | CAN transceiver |
| U11 | CP2102N | USB-UART bridge |
| U14 | TJA1021T | LIN transceiver |
| U15 | ESP32-WROVER-IE-N16R8 | Main MCU |
| U17 | MCP23017 | I2C 16-bit IO expander (relay control) |

### Relays
| Ref | Part | Type | Function |
|-----|------|------|----------|
| K1, K2 | TRS-L-12VDC-S-Z | Monostable | Light outputs |
| K3, K7, K8 | HFE10 | Bistable | Switching groups |
| K4 | HF3F-L | Bistable | Pump relay |
| K5, K6 | TRS-L-12VDC-S-Z | Monostable | Aux outputs |

## Planned Protocols
- MQTT (slave mode)
- Home Assistant integration
- ESPHome
- ESP-NOW
- CI-BUS (CIVD standard)
- RV-C (RVIA standard)

## Certifications (Planned)
- CE (Regulation EC 765/2008)
- ECE R10 (EMC)
- CI-BUS (CIVD)
- RV-C (RVIA)

## Firmware Architecture

### Framework: ESPHome
The firmware is based on **ESPHome** for easy customization and Home Assistant integration.

**Benefits:**
- YAML-based configuration
- Native Home Assistant integration
- OTA updates
- Web interface
- Large community

### Project Structure
```
smartebl/
├── smart-ebl.yaml          # Main ESPHome configuration
├── secrets.yaml            # WiFi/API credentials (not in git)
├── secrets.yaml.example    # Template for secrets
├── components/             # Custom ESPHome components
│   ├── ads7830/           # 8-bit I2C ADC driver
│   │   ├── __init__.py
│   │   ├── sensor.py
│   │   ├── ads7830.h
│   │   └── ads7830.cpp
│   └── lin_bus/           # LIN bus driver (Truma compatible)
│       ├── __init__.py
│       ├── lin_bus.h
│       └── lin_bus.cpp
├── Smart EBL V1.0 Datenblatt.pdf
└── E-Block.pdf
```

### Custom Components

| Component | Purpose | Status |
|-----------|---------|--------|
| `ads7830` | 8-bit 8-channel I2C ADC | Implemented |
| `lin_bus` | LIN bus master/slave (TJA1021T) | Implemented |

### Native ESPHome Components Used
- `nextion` - Display communication
- `esp32_can` - CAN bus (RV-C)
- `i2c` - ADC communication
- `uart` - Serial interfaces

### Building & Flashing
```bash
# Install ESPHome
pip install esphome

# Create secrets.yaml from template
cp secrets.yaml.example secrets.yaml
# Edit secrets.yaml with your credentials

# Compile
esphome compile smart-ebl.yaml

# Flash via USB
esphome upload smart-ebl.yaml

# Flash OTA (after initial USB flash)
esphome upload smart-ebl.yaml --device smart-ebl.local
```

## Development Notes

### ESP32 GPIO Mapping (from information/ESP32 Pinout)

| GPIO | Label | Function | Notes |
|------|-------|----------|-------|
| GPIO0 | IO0 | BOOT | Flash mode auto set when USB plugged in |
| GPIO1 | TXD0 | USB Programmer | CP2102N |
| GPIO3 | RXD0 | USB Programmer | CP2102N |
| GPIO12 | TXD1 | LIN TX | Truma iNetbox (TJA1021T) |
| GPIO13 | RXD1 | LIN RX | Truma iNetbox (TJA1021T) |
| GPIO14 | RXD_CAN1 | CAN RX | SN65HVD234 |
| GPIO15 | TXD_CAN1 | CAN TX | SN65HVD234 |
| GPIO18 | TXD2 | Display TX | Via TRS3221 RS-232 |
| GPIO19 | RXD2 | Display RX | Via TRS3221 RS-232 |
| GPIO21 | SDA | I2C Data | ADS7830 ADCs + MCP23017 |
| GPIO22 | SCL | I2C Clock | ADS7830 ADCs + MCP23017 |
| GPIO23 | CAN_TERM | CAN Termination | Software switchable |
| GPIO25 | LIN_MS | LIN Master/Slave | TJA1021T control |
| GPIO26 | LIN_WAKE | LIN Wake | TJA1021T control |
| GPIO27 | LIN_SLP | LIN Sleep | TJA1021T control |
| GPIO32 | CAN_1_STB | CAN Standby | SN65HVD234 control |
| GPIO33 | IO_RESET | MCP23017 Reset | Pin 14 on MCP23017 |
| GPIO34 | - | SW1 Mode Button | Function TBD |
| GPIO35 | GPIO35 | Not Connected | - |
| GPIO36 | INT_A | MCP23017 INT A | Pin 16 on MCP23017 |
| GPIO39 | INT_B | MCP23017 INT B | Pin 15 on MCP23017 |

### Display Connection (TRS3221 RS-232 Transceiver)
The Nextion display connects via RS-232 levels through the TRS3221 (U18):
- **TXD_PANEL**: ESP32 GPIO18 (TXD2) -> TRS3221 -> Nextion RX
- **RXD_PANEL**: Nextion TX -> TRS3221 -> ESP32 GPIO19 (RXD2)
- **Connector**: RJ45 (includes RS232, LIN, and 12V power)

### MCP23017 IO Expander (U17) - Address 0x20

All relay control is via MCP23017 -> ULN2803A (U8) -> Relay coils.

**Port B (GPB0-GPB7) - Relay Coil Control via ULN2803A:**
| Pin | ESPHome# | Signal | Function |
|-----|----------|--------|----------|
| GPB0 | 8 | P_off | K4 bistable relay OFF coil (Water pump) |
| GPB1 | 9 | P_on | K4 bistable relay ON coil (Water pump) |
| GPB2 | 10 | L_off | K8 bistable relay OFF coil (Light group) |
| GPB3 | 11 | L_on | K8 bistable relay ON coil (Light group) |
| GPB4 | 12 | A_off | K7 bistable relay OFF coil (AUX group) |
| GPB5 | 13 | A_on | K7 bistable relay ON coil (AUX group) |
| GPB6 | 14 | 12_off | K3 bistable relay OFF coil (12V group) |
| GPB7 | 15 | 12_on | K3 bistable relay ON coil (12V group) |

**Port A (GPA0-GPA5) - Additional Outputs:**
| Pin | ESPHome# | Signal | Function |
|-----|----------|--------|----------|
| GPA0 | 0 | EisX | EisEx heater (via BTS6143D) |
| GPA1 | 1 | D+2 | D+ programmable output relay |
| GPA2 | 2 | F-CTRL | Fridge control relay K6 |
| GPA3 | 3 | T1_DC | Tank 1 DC mode (inhibit while moving) |
| GPA4 | 4 | T2_DC | Tank 2 DC mode (inhibit while moving) |
| GPA5 | 5 | T3_DC | Tank 3 DC mode (inhibit while moving) |

### I2C Bus Devices

| Address | Device | Function |
|---------|--------|----------|
| 0x20 | MCP23017 (U17) | IO Expander - Relay control |
| 0x48 | ADS7830 (U4) | Fuse monitoring group 1 |
| 0x49 | ADS7830 (U5) | Fuse monitoring group 2 |
| 0x4A | ADS7830 (U6) | Input monitoring |
| 0x4B | ADS7830 (U7) | Output monitoring |

### Bistable Relay Control
Bistable (latching) relays require a pulse to toggle state. Controlled via MCP23017 Port B:
```yaml
# Example: Water pump relay via MCP23017
- platform: gpio
  id: relay_pump_on_coil
  pin:
    mcp23xxx: io_expander
    number: 9  # GPB1 = P_on
    mode: OUTPUT
  internal: true
  on_turn_on:
    - delay: 100ms
    - switch.turn_off: relay_pump_on_coil

- platform: gpio
  id: relay_pump_off_coil
  pin:
    mcp23xxx: io_expander
    number: 8  # GPB0 = P_off
    mode: OUTPUT
  internal: true
  on_turn_on:
    - delay: 100ms
    - switch.turn_off: relay_pump_off_coil

# Template switch for user control
- platform: template
  name: "Water Pump"
  turn_on_action:
    - switch.turn_on: relay_pump_on_coil
  turn_off_action:
    - switch.turn_on: relay_pump_off_coil
```

## Session Notes

### 2025-01-16
- **CRITICAL FIX:** Discovered relay control uses MCP23017 IO expander (U17), not direct ESP32 GPIO
- Parsed KiCad schematic (E-Block.kicad_sch) using kiutils Python library
- Completely rewrote smart-ebl.yaml with correct MCP23017-based relay control
- Updated hardware documentation with accurate pin mappings
- Signal chain: ESP32 -> I2C -> MCP23017 (0x20) -> ULN2803A (U8) -> Relay coils

### 2025-01-12
- Project initialized with ESPHome framework
- Created custom components: ads7830, lin_bus
- Main configuration in smart-ebl.yaml
