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

### ESP32 GPIO Mapping (from schematic analysis)

| GPIO | Function | Notes |
|------|----------|-------|
| GPIO0 | BOOT / Mode Button | Pull-up, active low |
| GPIO1 | TXD0 | USB programming (CP2102N) |
| GPIO3 | RXD0 | USB programming (CP2102N) |
| GPIO4 | CAN RX | esp32_can (SN65HVD234) |
| GPIO5 | CAN TX | esp32_can (SN65HVD234) |
| GPIO16 | UART1 RX | LIN bus / Nextion display |
| GPIO17 | UART1 TX | LIN bus / Nextion display |
| GPIO21 | I2C SDA | ADCs + MCP23017 |
| GPIO22 | I2C SCL | ADCs + MCP23017 |
| GPIO34 | D+ Signal | Input only |
| GPIO35 | Shore Power | Input only |
| GPIO36 | Input (VP) | Input only |
| GPIO39 | Input (VN) | Input only |

### MCP23017 IO Expander (U17) - Address 0x20

All relay control is via MCP23017 -> ULN2803A (U8) -> Relay coils.

**Port A (GPA0-GPA7) - Relay Control:**
| Pin | Signal | Function |
|-----|--------|----------|
| GPA0 | 12V_ON | K3 bistable relay ON coil (12V group) |
| GPA1 | 12V_OFF | K3 bistable relay OFF coil |
| GPA2 | AUX_ON | K7 bistable relay ON coil (AUX group) |
| GPA3 | AUX_OFF | K7 bistable relay OFF coil |
| GPA4 | Light_ON | K8 bistable relay ON coil (Light group) |
| GPA5 | Light_OFF | K8 bistable relay OFF coil |
| GPA6 | Pump_ON | K4 bistable relay ON coil (Water pump) |
| GPA7 | Pump_OFF | K4 bistable relay OFF coil |

**Port B (GPB0-GPB7) - Additional Outputs:**
| Pin | Signal | Function |
|-----|--------|----------|
| GPB0 | EisEx | EisEx heater (via BTS6143D) |
| GPB1 | D+_Relay | D+ programmable output relay |
| GPB2 | Fridge | Fridge control relay (K5/K6) |
| GPB3-GPB7 | Reserved | Future expansion |

### I2C Bus Devices

| Address | Device | Function |
|---------|--------|----------|
| 0x20 | MCP23017 (U17) | IO Expander - Relay control |
| 0x48 | ADS7830 (U4) | Fuse monitoring group 1 |
| 0x49 | ADS7830 (U5) | Fuse monitoring group 2 |
| 0x4A | ADS7830 (U6) | Input monitoring |
| 0x4B | ADS7830 (U7) | Output monitoring |

### Bistable Relay Control
Bistable (latching) relays require a pulse to toggle state. Controlled via MCP23017:
```yaml
# Example: Water pump relay via MCP23017
- platform: gpio
  id: relay_pump_on_coil
  pin:
    mcp23xxx: io_expander
    number: 6  # GPA6
    mode: OUTPUT
  internal: true
  on_turn_on:
    - delay: 100ms
    - switch.turn_off: relay_pump_on_coil

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
