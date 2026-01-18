| ID   | Description | Pin | Label            | Direction    | Switched by    | Bus           | Function                            | Fuse |
|------|-------------|-----|------------------|--------------|----------------|---------------|-------------------------------------|------|
| J1   | ODLT        | 1   | V_L_out          | Output       | J6 Pin 1/2     | D+_Relais     | Awning Light                        | F3   |
| J1   | ODLT        | 2   | GND              | -            | -              | GND           | -                                   | -    |
| J2   | COLT        | 1   | C_L_out          | Output       | J6 Pin 3/4     | Contour_Light | Countour Light                      | F2   |
| J2   | COLT        | 2   | GND              | -            | -              | GND           | -                                   | -    |
| J3   | PMP         | 1   | PUMP             | Output       | ESP32 or SW3/4 | PUMP          | Waterpump                           | F1   |
| J3   | PMP         | 2   | GND              | Output       | -              | GND           | -                                   | -    |
| J4   | EIS-EX      | 1   | EisX             | Output       | ESP32          | EisX          | Eis-Ex                              | F18  |
| J4   | DUO-CTRL    | 2   | DuoC             | Input        | -              | DuoC          | DuoControl voltage measurement      | -    |
| J4   | D2+-OUT     | 3   | D+2              | Output       | ESP32          | D+2           | D2+ Output                          | F23  |
| J4   | D+-OUT      | 4   | D+_Relais        | Output       | J6 Pin 1/2     | D+_Relais     | D+ Output                           | F17  |
| J5   | CAR-BAT     | 1   | VCAR             | Input        | -              | VCAR          | Starter Battery voltage measurement | F20  |
| J5   | AC-DET      | 2   | ACdet            | Input        | -              | ACdet         | AC Shore power voltage measurement  | -    |
| J5   | GND         | 3   | GND              | -            | -              | GND           | -                                   | -    |
| J5   | GND         | 4   | GND              | -            | -              | GND           | -                                   | -    |
| J6   | D+-NEG      | 1   | D+_Negative      | Input        | -              | D+_Relais     | D+ Negative input                   | -    |
| J6   | D+-POS      | 2   | D+_Positive      | Input        | -              | D+_Relais     | D+ Positive input                   | -    |
| J6   | CON-NEG     | 3   | Contour_Negative | Input        | -              | Contour_Light | Contour Negative input              | -    |
| J6   | CON-POS     | 4   | Contour_Positive | Input        | -              | Contour_Light | Contour Positive input              | -    |
| J7   | 12V-ON      | 1   | 12V_on           | Input        | -              | 12V_on        | Switches 12V relay on               | -    |
| J7   | 12V-OFF     | 2   | 12V_off          | Input        | -              | 12V_off       | Switches 12V relay off              | -    |
| J7   | AUX-ON      | 3   | Aux_on           | Input        | -              | Aux_on        | Switches Aux relay on               | -    |
| J7   | AUX-OFF     | 4   | Aux_off          | Input        | -              | Aux_off       | Switches Aux relay off              | -    |
| J8   | LGT-ON      | 1   | Light_on         | Input        | -              | Light_on      | Switches Light relay on             | -    |
| J8   | LGT-OFF     | 2   | Light_off        | Input        | -              | Light_off     | Switches Light relay off            | -    |
| J8   | PMP-ON      | 3   | Pump_on          | Input        | -              | Pump_on       | Switches Pump relay on              | -    |
| J8   | PMP-OFF     | 4   | Pump_off         | Input        | -              | Pump_off      | Switches Pump relay off             | -    |
| J9   | RJ45        | 1   | RXD_PANEL        | Input        | -              | RXD_PANEL     | Nextion UART connection             |      |
| J9   | RJ45        | 2   | TXD_PANEL        | Output       | -              | TXD_PANEL     | Nextion UART connection             |      |
| J9   | RJ45        | 3   | LIN              | Bidrectional | -              | LIN           | LIN connection to Display Board     |      |
| J9   | RJ45        | 4   | +BATT            | Output       | -              | +BATT         | Power supply for Display Board      | F21  |
| J9   | RJ45        | 5   | +BATT            | Output       | -              | +BATT         | Power supply for Display Board      | F21  |
| J9   | RJ45        | 6   | NC               | -            | -              | -             | -                                   | -    |
| J9   | RJ45        | 7   | GND              | -            | -              | GND           | -                                   | -    |
| J9   | RJ45        | 8   | GND              | -            | -              | GND           | -                                   | -    |
| J10  | RJ12        | 1   | NC               | -            | -              | -             | -                                   | -    |
| J10  | RJ12        | 2   | NC               | -            | -              | -             | -                                   | -    |
| J10  | RJ12        | 3   | LIN              | Bidrectional | -              | LIN           | LIN Bus                             | -    |
| J10  | RJ12        | 4   | NC               | -            | -              | -             | -                                   | -    |
| J10  | RJ12        | 5   | GND              | -            | -              | GND           | -                                   | -    |
| J10  | RJ12        | 6   | NC               | -            | -              | -             | -                                   | -    |
| J11  | Panel       | 1   | GND              | -            | -              | GND           | -                                   | -    |
| J11  | Panel       | 2   | GND              | -            | -              | GND           | -                                   | -    |
| J11  | Panel       | 3   | GND              | -            | -              | GND           | -                                   | -    |
| J11  | Panel       | 4   | Tank1            | Input        | -              | Tank1         | Tank1 voltage measurement           | -    |
| J11  | Panel       | 5   | GND              | -            | -              | GND           | -                                   | -    |
| J11  | Panel       | 6   | Tank2            | Input        | -              | Tank2         | Tank2 voltage measurement           | -    |
| J11  | Panel       | 7   | GND              | -            | -              | GND           | -                                   | -    |
| J11  | Panel       | 8   | Tank3            | Input        | -              | Tank3         | Tank3 voltage measurement           | -    |
| J11  | Panel       | 9   | GND              | -            | -              | GND           | -                                   | -    |
| J11  | Panel       | 10  | GND              | -            | -              | GND           | -                                   | -    |
| J11  | Panel       | 11  | LIN              | Bidrectional | -              | LIN           | LIN connection to Display Board     | -    |
| J11  | Panel       | 12  | NC               | -            | -              | -             | -                                   | -    |
| J11  | Panel       | 13  | TXD_PANEL        | Output       | -              | TXD_PANEL     | -                                   | -    |
| J11  | Panel       | 14  | RXD_PANEL        | Input        | -              | RXD_PANEL     | -                                   | -    |
| J11  | Panel       | 15  | +BATT            | Output       | -              | +BATT         | Power supply for Display Board      | F21  |
| J11  | Panel       | 16  | +BATT            | Output       | -              | +BATT         | Power supply for Display Board      | F21  |
| J12  | USB         | -   | USB              | Bidrectional | -              | -             | Programming port                    | -    |
| J13  | Jack        | 1   | CAN H            | Bidrectional | -              | CAN H         | Can Bus                             | -    |
| J13  | Jack        | 2   | CAN L            | Bidrectional | -              | CAN L         | Can Bus                             | -    |
| J13  | Jack        | 3   | GND              | -            | -              | GND           | -                                   | -    |
| J13  | Jack        | 4   | +BATT            | Output       | -              | +BATT         | Power for CAN bus                   | F24  |
| J14  | Jack        | 1   | CAN H            | Bidrectional | -              | CAN H         | Can Bus                             | -    |
| J14  | Jack        | 2   | CAN L            | Bidrectional | -              | CAN L         | Can Bus                             | -    |
| J14  | Jack        | 3   | GND              | -            | -              | GND           | -                                   | -    |
| J14  | Jack        | 4   | +BATT            | Output       | -              | +BATT         | Power for CAN bus                   | F24  |
| J15  | GND         | 1   | GND              | -            | -              | GND           | -                                   | -    |
| J15  | Signal1     | 2   | Tank1            | Input        | -              | Tank1         | Tank 1 measurement                  | -    |
| J15  | +5V         | 3   | +5V              | Output       | -              | +5V           | Tank 1 5V power supply              | -    |
| J15  | +12V        | 4   | +12V             | Output       | -              | +12V          | Tank 1 12V power supply             | F22  |
| J16  | GND         | 1   | GND              | -            | -              | GND           | -                                   | -    |
| J16  | Signal2     | 2   | Tank2            | Input        | -              | Tank2         | Tank 2 measurement                  | -    |
| J16  | +5V         | 3   | +5V              | Output       | -              | +5V           | Tank 2 5V power supply              | -    |
| J16  | +12V        | 4   | +12V             | Output       | -              | +12V          | Tank 2 12V power supply             | F22  |
| J17  | GND         | 1   | GND              | -            | -              | GND           | -                                   | -    |
| J17  | Signal2     | 2   | Tank3            | Input        | -              | Tank3         | Tank 3 measurement                  | -    |
| J17  | +5V         | 3   | +5V              | Output       | -              | +5V           | Tank 3 5V power supply              | -    |
| J17  | +12V        | 4   | +12V             | Output       | -              | +12V          | Tank 3 12V power supply             | F22  |
| J18  | Ctrl        | 1   | +BATT            | Output       | -              | +BATT         | Fridge output, direct +BATT         | F9   |
| J18  | Ctrl        | 2   | GND              | -            | -              | GND           | -                                   | -    |
| JP1  | LGT1        | 1   | V_Light          | Output       | Light relay    | V_Light       | Light power out                     | F4   |
| JP1  | LGT1        | 2   | GND              | -            | -              | GND           | -                                   | -    |
| JP2  | LGT2        | 1   | V_Light          | Output       | Light relay    | V_Light       | Light power out                     | F5   |
| JP2  | LGT2        | 2   | GND              | -            | -              | GND           | -                                   | -    |
| JP3  | OUT1        | 1   | +BATT            | Output       | -              | +BATT         | Permanent power out                 | F6   |
| JP3  | OUT1        | 2   | GND              | -            | -              | GND           | -                                   | -    |
| JP4  | OUT2        | 1   | +BATT            | Output       | -              | +BATT         | Permanent power out                 | F7   |
| JP4  | OUT2        | 2   | GND              | -            | -              | GND           | -                                   | -    |
| JP5  | OUT3        | 1   | +BATT            | Output       | -              | +BATT         | Permanent power out                 | F8   |
| JP5  | OUT3        | 2   | GND              | -            | -              | GND           | -                                   | -    |
| JP6  | Comp        | 1   | +BATT            | Output       | -              | +BATT         | Compressor Fridge out               | F10  |
| JP6  | Comp        | 2   | GND              | -            | -              | GND           | -                                   | -    |
| JP7  | Abso        | 1   | +BATT            | Output       | Fridge relay   | Fridge        | Absorber Fridge out                 | F10  |
| JP7  | Abso        | 2   | GND              | -            | -              | GND           | -                                   | -    |
| JP8  | AUX1        | 1   | V_12V_Aux        | Output       | Aux relay      | V_12V_Aux     | Aux power out                       | F11  |
| JP8  | AUX1        | 2   | GND              | -            | -              | GND           | -                                   | -    |
| JP9  | AUX2        | 1   | V_12V_Aux        | Output       | Aux relay      | V_12V_Aux     | Aux power out                       | F12  |
| JP9  | AUX2        | 2   | GND              | -            | -              | GND           | -                                   | -    |
| JP10 | AUX3        | 1   | V_12V_Aux        | Output       | Aux relay      | V_12V_Aux     | Aux power out                       | F13  |
| JP10 | AUX3        | 2   | GND              | -            | -              | GND           | -                                   | -    |
| JP11 | 12V1        | 1   | V_12V_User       | Output       | 12V relay      | V_12V_User    | 12V power out                       | F14  |
| JP11 | 12V1        | 2   | GND              | -            | -              | GND           | -                                   | -    |
| JP12 | 12V2        | 1   | V_12V_User       | Output       | 12V relay      | V_12V_User    | 12V power out                       | F15  |
| JP12 | 12V2        | 2   | GND              | -            | -              | GND           | -                                   | -    |
| JP13 | 12V3        | 1   | V_12V_User       | Output       | 12V relay      | V_12V_User    | 12V power out                       | F16  |
| JP13 | 12V3        | 2   | GND              | -            | -              | GND           | -                                   | -    |
| JM1  | IN2         | 1   | RV-Battery       | Input        | -              | +BATT         | RV Battery in                       | -    |
| JM1  | IN2         | 2   | GND              | -            | -              | GND           | -                                   | -    |
| JM2  | IN1         | 1   | AC-Charger       | Input        | -              | +BATT         | AC Charger in                       | -    |
| JM2  | IN1         | 2   | GND              | -            | -              | GND           | -                                   | -    |
| JM3  | IN4         | 1   | Booster          | Input        | -              | +BATT         | Booster in                          | -    |
| JM3  | IN4         | 2   | GND              | -            | -              | GND           | -                                   | -    |
| JM4  | IN3         | 1   | Solar-In         | Input        | -              | +BATT         | Solar in                            | -    |
| JM4  | IN3         | 2   | GND              | -            | -              | GND           | -                                   | -    |
