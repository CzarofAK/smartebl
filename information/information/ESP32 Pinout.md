| GPIO Pin | Label     | What           | Direction     | Function to be Implemented | Comment                                          | OLD |
|----------|-----------|----------------|---------------|----------------------------|--------------------------------------------------|-----|
| EN       | EN        | MCU Reset      | Input         | none                       |                                                  |     |
| GPIO0    | IO0       | BOOT           | Input         | none                       | Flash mode automatically set when USB plugged in |     |
| GPIO1    | TXD0      | USB Programmer | Bidirectional | none                       |                                                  |     |
| GPIO12   | TXD1      | LIN            | Bidirectional | Inetbox/Truma              |                                                  |     |
| GPIO13   | RXD1      | LIN            | Bidirectional | Inetbox/Truma              |                                                  |     |
| GPIO14   | RXD_CAN1  | CAN bus        | Bidirectional |                            |                                                  |     |
| GPIO15   | TXD_CAN1  | CAN bus        | Bidirectional |                            |                                                  |     |
| GPIO18   | TXD2      | UART           | Bidirectional | Display                    | Connects to TRS3221 (Outputs RS232)              |     |
| GPIO19   | RXD2      | UART           | Bidirectional | Display                    | Connects to TRS3221 (Outputs RS232)              |     |
| GPIO21   | SDA       | I2C            | Bidirectional | all I2C sensors            | ADS7830 → ADC, MCP23017_ML → Relays              |     |
| GPIO22   | SCL       | I2C            | Bidirectional | all I2C sensors            | ADS7830 → ADC, MCP23017_ML → Relays              |     |
| GPIO23   | CAN_TERM  | CAN bus        | Bidirectional |                            |                                                  |     |
| GPIO25   | LIN_MS    | LIN            | Bidirectional | Inetbox/Truma              |                                                  |     |
| GPIO26   | LIN_WAKE  | LIN            | Bidirectional | Inetbox/Truma              |                                                  |     |
| GPIO27   | LIN_SLP   | LIN            | Bidirectional | Inetbox/Truma              |                                                  |     |
| GPIO3    | RXD0      | USB Programmer | Bidirectional | none                       |                                                  |     |
| GPIO32   | CAN_1_STB | CAN bus        | Bidirectional |                            |                                                  |     |
| GPIO33   | IO_RESET  | Data           | Bidirectional |                            | For Relay Driver MCP23017_ML (Pin14)             |     |
| GPIO34   | -         | SW1 Mode       | Input         | SW1 onboard                | Function to be defined                           |     |
| GPIO35   | GPIO35    |                |               |                            | Not Connected                                    |     |
| GPIO36   | INT_A     | Data           | Bidirectional |                            | For Relay Driver MCP23017_ML (Pin16)             |     |
| GPIO39   | INT_B     | Data           | Bidirectional |                            | For Relay Driver MCP23017_ML (Pin15)             |     |
