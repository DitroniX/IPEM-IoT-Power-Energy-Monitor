
Further details on this code options can be found in our Wiki
https://github.com/DitroniX/IPEM-IoT-Power-Energy-Monitor/wiki/IPEM-ESP32-ATM90E32-ATM90E36-Test-Code

  PCA 1.2302-20x - Test Code Firmware v1 - **Development Code - WORK-IN-PROGRESS**
  
    > FIRMWARE CONFIGURATION INFO WIKI > https://github.com/DitroniX/IPEM-IoT-Power-Energy-Monitor/wiki/Firmware-Configuration-and-Flashing

  This software has expanded way past it's original bring-up task and is now quite comprehensive.  It can now be, as is, used for both bring-up, final use and publishing. Else, simply use the bits you like!

  A number of software switches are used, throughout.  The defaults are listed below.  You should be able to just compile this current version to an IPEM ATM90E32 and see some valid results in the serial monitor / OLED.

  Board Bring Up Test - ATM90E3x Test Routines (Output sent to the Serial Print - ONLY ON BOOT! Press RESET or HOLD USR Button to REFRESH)

  Base 90E32/36 register formulation based on the excellent ground work from Tisham Dhar, whatnick | ATM90E32 ATM90E36 Energy Monitor Core
  VSCode base, E32/E36 Registers/Code Merged, Updated, Software Logic/Routines, Bring Up Firmware, Comprehensive Functions, OTA and Domoticz Integration by Date Williams

  **CONFIGURATION** (Setup for bring-up testing of the board)

  This version of firmware code is by default, configured for:
  * ATM90E32 (ATM90DEVICE ATM90E32_DEVICE)
  * CT4 Configured to ESP32 ADC (CT4_CONFIG CT4_ESP)
  * CT4 Input - Enabled (CT4_ENABLED true)
  * PWM Output on Default GPIO Port GP17 (EnablePWMLocal false EnablePWMRemote false EnablePWMTestOutput false)
  * DAC Output. Port TBD (EnableDACLocal false EnableDACRemote false EnableDACTestOutput false)
  * Multi-Voltage Input (ATM_SINGLEVOLTAGE true)
  * Split-Phase USA - Disabled (ATM_SPLITPHASE false)
  * Hardware Test - Enabled (DisableHardwareTest false)
  * Display of Board Configuration (EnableDisplayBoardConfiguration true)
  * Domoticz Publishing - Disabled (EnableDomoticz false)
  * MQTT Publishing - Disabled (EnableMQTT false)
  * Loop Refreshing Terminal Output (EnableBasicLoop false) - Display Info ONCE uppon Reset.
  * Value Outputs are filtered through a Sofware Noise Filter / Comparator / Squelch (EnableNoiseFilterSquelch true)
  * When Publising to Domoticz - Mute Detailed Output to Serial (Loop)
  * OLED 0.6" Display SSD1306 128x32.  (EnableOLEDLoop true)
  * Board Location and Firmware Version to OLED and Serial Monitor
  * IP Address Defaults to DHCP.  Static IP Address Configuration in WiFi-OTA.h

  **CALIBRATION** (This should be minimal - based on the below)

  This version of firmware has been setup for ATM90E32 and CT4 to ESP32 ADC.
  * Current Clamp default example settings are based on SCT-013-000 100A/50mA.
  * Voltages default example settings are based on Single/Three Phase Voltage Inputs from a Greenbrook DAT01A (TC TR7) Transformer, set to 12V AC.
  * Current Clamps CT1, CT2 and CT3 are connected to the ATM90E3x. Calibration requirements should be minimal and board ready to use.
  * AC Voltage Inputs V1, V2 and V3 are connected to the ATM90E3x. Calibration requirements should be minimal and board ready to use.
  * Current Clamp CT4 is connected to the ESP32 ADC1 CH7. Software EmonLib calibratrion may be required.
  * You can update ATM90Ex calibration (if required), in IPEM_Hardware.h
  * You can update board ATM Configurations in ATM90E3x.h
  * You can update CT4 calibration in iPEM_Hardware.h - See EmonLib

  **WiFi and OTA Updates**

  * Setup WiFi
  * Configure Multi-WiFi SSID/PWD
  * Setup Optional Static IP address and Gateway (DHCP or Static)
  * Setup Preferred Hostname
  * Setup Serial Device over IP (Used for OTA)
  * Display WiFI Signal Meter
  * Web Server Information Page and Push OTA Updater
  * Web Listner
  * PWM Switches added: /pwm-local, /pwm-remote, /pwm-on, /pwm-off, /pwm-test
  * DAC Switches added: /dac-local, /dac-remote, /dac-on, /dac-off, /dac-test
  * Seperate switches dac-remote and pwm-remote, allow you to remotely send a fixed value to pwm / dac output.  Ideal for scenes etc.  Remote turns off or over-rides CT for pwm/dac.

  **DOMOTICZ**

  * Setup connection to Domoticz Home Automation
  * Configure Required Values to Publish to Domoticz Hardware Device Indexes

  **MQTT**

  * Setup connection to MQTT Broker / Home Automation
  * Configure Required Values to Publish to MQTT Broker

  **ThingSpeak**
  * Example Publish (play data), can be found on https://thingspeak.com/channels/2179490
  * Setup easy connection to ThingSpeak Cloud Base Server / Home Automation.  FREE Cloud Account.  View on Phone/Web.
  * Configure Required Values to Publish to ThingSpeak

  **PWM Duty Cycle Output**

  * PWM (On Default GPIO 17), which will track Power Usage (from Default Variable ActivePowerExportCT1)
  * Output Updated on Each DisplayRegisters Loop (Only if Value Changed)
  * WebServer /pwm-local, /pwm-remote, /pwm-on, /pwm-off, /pwm-test
  * Ability to Enable and Disable PWM Local Readings
  * Ability to Enable and Disable PWM Remote Readings
  * Ability to Enable and Disable PWM Test Mode
  * Ability to Set Fixed Power from Remote Value, or Leave to Dynamic CT Power Values (WIP)
  * Continuous PWM Cycle Test Mode Loop

  **DAC Voltage Output**

  * Fixed Voltage Output from DAC, tracking Power Usage (from Default Variable ActivePowerExportCT1)
  * Output Updated on Each DisplayRegisters Loop (Only if Value Changed)
  * WebServer /dac-local, /dac-remote, /dac-on, /dac-off, /dac-test
  * Ability to Enable and Disable DAC Local Readings
  * Ability to Enable and Disable DAC Remote Readings
  * Ability to Enable and Disable DAC Test Mode
  * Abilty to Set Fixed DAC Output from Remote Value, or Leave to Dynamic CT Power Values (WIP)
  * DAC Sinewave Test Mode Loop (Default 10 Hz)
  * DAC Stepped Voltages Test Mode Loop (Slow Staircase)

  Enjoy!  Dave Williams, DitroniX.net


  **Remember!**
  * Set the BOARD to ESP32, 'WEMOS D1 MINI ESP32' DEV Module (or similar).
  * You may set the BAUD rate to 921600 to speed up flashing.
  * The SDK does NOT need external power to flash.  It will take Power from the USB 5V.
  * You can safely connect both USB Power and PSU AC In (Makes development much easier)

  **Note**: In the default state, upon first power up and during reset, the Green LED may be partially lit. Once programmed and the GPIO defined, the Green LED will go off after power up.

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and also github.com/DitroniX
