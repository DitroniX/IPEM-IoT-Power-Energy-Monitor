
Further details on this code options can be found in our Wiki
https://github.com/DitroniX/IPEM-IoT-Power-Energy-Monitor/wiki/IPEM-ESP32-ATM90E32-ATM90E36-Test-Code

**For Installation, Flashing and Configuration information click [here](https://github.com/DitroniX/IPEM-IoT-Power-Energy-Monitor/wiki/Firmware-Configuration-and-Flashing)**

  PCA 1.2302-20x - Test Code Firmware v1 - **Development Code - WORK-IN-PROGRESS**
  
  **CONFIGURATION (Setup for bring-up testing of the board)**

  This version of firmware code is by default, configured for:
  * ATM90E32 (ATM90DEVICE ATM90E32_DEVICE)
  * CT4 Configured to ESP32 ADC (CT4_CONFIG CT4_ESP)
  * CT4 Input - Enabled (CT4_ENABLED true)
  * CT4 Isolated from all Formulas - Disabled (CT4_ISOLATED false)
  * Multi-Voltage Input (ATM_SINGLEVOLTAGE true)
  * Split-Phase USA - Disabled (ATM_SPLITPHASE false)
  * Hardware Test - Enabled (DisableHardwareTest false)
  * Display of Board Configuration (EnableDisplayBoardConfiguration true)
  * Domoticz Publishing - Disabled (EnableDomoticz false)
  * Loop Refreshing Terminal Output (EnableBasicLoop false) - Display Info ONCE uppon Reset.
  * Value Outputs are filtered through a Sofware Noise Filter / Comparator / Squelch (EnableNoiseFilterSquelch true)
  * When Publising to Domoticz - Mute Detailed Output to Serial (Loop)
  * OLED 0.6" Display SSD1306 128x32.  (EnableOLEDLoop true)

**IPEM with OLED**
![Display-Type-B](https://ditronix.net/wp-content/uploads/2023/05/IPEM-with-OLED-Display.jpg?raw=true)

  CALIBRATION (This should be minimal - based on the below)

  This version of firmware has been setup for ATM90E32 and CT4 to ESP32 ADC.
  * Current Clamp default example settings are based on SCT-013-000 100A/50mA.
  * Voltages default example settings are based on Single/Three Phase Voltage Inputs from a Greenbrook DAT01A (TC TR7) Transformer, set to 12V AC.
  * Current Clamps CT1, CT2 and CT3 are connected to the ATM90E3x. Calibration requirements should be minimal and board ready to use.
  * AC Voltage Inputs V1, V2 and V3 are connected to the ATM90E3x. Calibration requirements should be minimal and board ready to use.
  * Current Clamp CT4 is connected to the ESP32 ADC1 CH7. Software EmonLib calibratrion may be required.
  * You can update ATM90Ex calibration (if required), in IPEM_Hardware.h
  * You can update board ATM Configurations in ATM90E3x.h
  * You can update CT4 calibration in iPEM_Hardware.h - See EmonLib

  WiFi and OTA Updates

  * Setup WiFi
  * Setup Optional Static IP address and Gateway (DHCP or Static)
  * Setup Hostname
  * Setup Serial Device over IP (Used for OTA)

**IPEM with Integrated Push OTA Example**
![Display-Type-B](https://ditronix.net/wp-content/uploads/2023/05/IPEM-Push-OTA-Web-Home-Page.png?raw=true)
![Display-Type-B](https://ditronix.net/wp-content/uploads/2023/05/IPEM-Push-OTA-Web-Upload-Page.png?raw=true)

  DOMOTICZ

  * Setup connection to Domoticz Home Automation
  * Configure Required Values to Pubish to Domoticz Hardware Device Indexes

  *Enjoy!  Dave Williams*

  **Remember!**
  * Set the BOARD to ESP32, 'WEMOS D1 MINI ESP32' DEV Module (or similar).
  * You may set the BAUD rate to 921600 to speed up flashing.
  * The SDK does NOT need external power to flash.  It will take Power from the USB 5V.
  * You can safely connect both USB Power and PSU AC In (Makes development much easier)

  Note: In the default state, upon first power up and during reset, the Green LED may be partially lit. Once programmed and the GPIO defined, the Green LED will go off after power up.

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and also github.com/DitroniX
