Further details on this code options can be found https://github.com/DitroniX/IPEM-IoT-Power-Energy-Monitor/wiki/IPEM-ESP32-ATM90E32-ATM90E36-Test-Code

  PCA 1.2302-20x - Test Code Firmware v1 - Development Code - WORK-IN-PROGRESS
  
  CONFIGURATION (Setup for bring-up testing of the board)

  This version of firmware code is configured for:
  * ATM90E32 (ATM90DEVICE ATM90E32_DEVICE)
  * CT4 Configured to ESP32 ADC (CT4_CONFIG CT4_ESP)
  * CT4 Input Enabled (CT4_ENABLED true)
  * Multi-Voltage Input (ATM_SINGLEVOLTAGE false)
  * Split-Phase USA Disabled (ATM_SPLITPHASE false)
  * Hardware Test Enabled (DisableHardwareTest false)
  * Display of Board Configuration (EnableDisplayBoardConfiguration true)
  * Domoticz Publishing Disabled (EnableDomoticz false)
  * Loop Refreshing Terminal Output (EnableBasicLoop false) - Display Info ONCE uppon Reset.
  * Value Outputs are filtered through a Sofware Noise Filter / Comparator / Squelch (EnableNoiseFilterSquelch true)
  * When Publising to Domoticz - Mute Detailed Output to Serial (Loop)

  CALIBRATION (This should be minimal - based on the below)

  This version of firmware has been setup for ATM90E32 and CT4 to ESP32 ADC.
  * Current Clamp default example settings are based on SCT-013-000 100A/50mA.
  * Voltages default example settings are based on Single/Three Phase Voltage Inputs from a Greenbrook DAT01A (TC TR7) Transformer, set to 12V AC.
  * You can update calibration in IPEM_Hardware.h
  * You can update board ATM Configurations in ATM90E3x.h

  Enjoy!  Dave Williams

  Remember!
  * Set the BOARD to ESP32, 'WEMOS D1 MINI ESP32' DEV Module (or similar).
  * You may set the BAUD rate to 921600 to speed up flashing.
  * The SDK does NOT need external power to flash.  It will take Power from the USB 5V.
  * You can safely connect both USB Power and PSU AC In (Makes development much easier)

  Note: In the default state, upon first power up and during reset, the Green LED may be partially lit. Once programmed and the GPIO defined, the Green LED will go off after power up.

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and also github.com/DitroniX
