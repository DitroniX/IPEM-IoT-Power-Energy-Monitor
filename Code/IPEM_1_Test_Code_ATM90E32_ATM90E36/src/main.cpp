/*
  Dave Williams, DitroniX 2019-2023 (ditronix.net)
  IPEM-1 ESP32 ATM90E32 ATM90E36 IoT Power Energy Monitoring Energy Monitor v1.0
  Features include ESP32 IPEM ESP32 ATM90E32 ATM90E36 16bit ADC EEPROM 3Phase 3+1 CT-Clamps Current Voltage Frequency Power Factor GPIO I2C OLED SMPS D1 USB
  PCA 1.2302-20x - Test Code Firmware v1 - Development Code - WORK-IN-PROGRESS - 22nd March 2023

  The purpose of this test code is to cycle through the various main functions of the board, as shown below, as part of board bring up testing.

  Simplified Board Bring Up Test - ATM90E3x Test Routines (Output sent to the Serial Print - ONLY ON BOOT! Press RESET or HOLD USR Button to REFRESH)

  Base register formulation based on the excellent ground work from Tisham Dhar, whatnick | ATM90E32 ATM90E36 Energy Monitor Core | E32/E36 Registers/Code Merged and Updated by Date Williams

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
*/

// Libraries
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <driver/adc.h>
#include <ATM90E3x.h>
#include <IPEM_Hardware.h>
#include <Domoticz.h>

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES ****************

// Constants
const int LoopDelay = 1;                        // Loop Delay in Seconds
boolean EnableBasicLoop = false;                // Set to true to display loop readings and displaying only one per reset cycle.
boolean EnableDisplayBoardConfiguration = true; // Set to true to display board software configuration Information

// **************** FUNCTIONS AND ROUTINES ****************

// Return Register Values from the ATM90E32, or ATM90E36, - and Display
void DisplayRegisters(boolean DisplayFull = true)
{

  if (DisplayFull == false) // Display Expanded Information
  {
    Serial.println("\nDisplay Output Set to Minimised");
  }

  // Display Board Configuration
  if (EnableDisplayBoardConfiguration == true && DisplayFull == true)
    DisplayBoardConfiguration();

  // **************** VOLTAGE ****************

  // Get Mains Voltages
  LineVoltage1 = NoiseFilterSquelch(eic.GetLineVoltage1(), 1);
  LineVoltage2 = NoiseFilterSquelch(eic.GetLineVoltage2(), 1);
  LineVoltage3 = NoiseFilterSquelch(eic.GetLineVoltage3(), 1);
  LineVoltageAverage = 0;
  LineVoltageTotal = 0;

  if (DisplayFull == true) // Display Expanded Information
  {
// Calculate Voltage
#if ATM_SINGLEVOLTAGE == true && ATM_SINGLEVOLTAGE == true
    Serial.println("* SOFTWARE CONFIGURATION ERROR *\n* You cannot have ATM_SINGLEVOLTAGE and ATM_SPLITPHASE, both Enabled.\n");
#else
#if ATM_SPLITPHASE == true
    LineVoltageTotal = LineVoltage1 + LineVoltage3; // Split Single Phase configured, so only 120V per leg
#else
    LineVoltageAverage = (LineVoltage1 + LineVoltage2 + LineVoltage3) / 3; // Voltage should be 110V, or 220-240, at the AC transformer
#endif
#endif

    // Mains RMS Voltage
    PrintUnderline("Mains RMS Voltage");

#if ATM_SINGLEVOLTAGE == false

    Serial.println("Voltage 1: " + String(LineVoltage1) + " V");

#if ATM_SPLITPHASE == false
    Serial.println("Voltage 2: " + String(LineVoltage2) + " V");
#endif

    Serial.println("Voltage 3: " + String(LineVoltage3) + " V");

#if ATM_SPLITPHASE == true
    PrintSeparator("Total Split Voltage: " + String(LineVoltageTotal) + " V (V1~X~V3)");
#else
    PrintSeparator("Average Voltage: " + String(LineVoltageAverage) + " V (V1~V2~V3)");
#endif

#else
    PrintSeparator("Voltage 1: " + String(LineVoltage1) + " V (Single Line Voltage Selected)");
#endif
  }

  // **************** CURRENT ****************

  // Get Total Current.  Ignore CT2 if Split Phase
  LineCurrentCT1 = NoiseFilterSquelch(eic.GetLineCurrentCT1());
#if ATM_SPLITPHASE == falseelse
  LineCurrentCT2 = NoiseFilterSquelch(eic.GetLineCurrentCT2());
#else
  LineCurrentCT2 = 0;
#endif
  LineCurrentCT3 = NoiseFilterSquelch(eic.GetLineCurrentCT3());

// Using ESP32 ADC for CT4 Current Input
#if ATM90DEVICE == ATM90E32_DEVICE || CT4_CONFIG == CT4_ESP
  ReadCT4Current();
#if CT4_ENABLED == true
  LineCurrentTotal = NoiseFilterSquelch(LineCurrentCT1 + LineCurrentCT2 + LineCurrentCT3 + LineCurrentCT4);
#else
  LineCurrentTotal = NoiseFilterSquelch(LineCurrentCT1 + LineCurrentCT2 + LineCurrentCT3);
#endif
#endif

// E36 Using I4N Input for Current
#if ATM90DEVICE == ATM90E36_DEVICE && CT4_CONFIG == CT4_ATM
  LineCurrentCTN = NoiseFilterSquelch(eic.GetLineCurrentCTN());
  LineCurrentTotal = NoiseFilterSquelch(LineCurrentCT1 + LineCurrentCT2 + LineCurrentCT3);
#endif

  if (DisplayFull == true) // Display Expanded Information
  {
    Serial.println("");

    // Mains RMS Current
    PrintUnderline("Mains RMS Current");
    Serial.println("Current CT1: " + String(LineCurrentCT1) + " A");

#if ATM_SPLITPHASE == false
    Serial.println("Current CT2: " + String(LineCurrentCT2) + " A");
#endif

    Serial.println("Current CT3: " + String(LineCurrentCT3) + " A");

#if ATM90DEVICE == ATM90E32_DEVICE && CT4_ENABLED == true || CT4_CONFIG == CT4_ESP && CT4_ENABLED == true
    Serial.println("Current CT4: " + String(LineCurrentCT4) + " A (ESP)");
#endif

#if ATM90DEVICE == ATM90E36_DEVICE && CT4_CONFIG == CT4_ATM
    Serial.println("Current CTN: " + String(LineCurrentCTN) + " A");
#endif

#if ATM_SPLITPHASE == true
    Serial.println("Actual Total Current: " + String(LineCurrentTotal) + " A (CT1~X~CT3)");
#else
#if CT4_ENABLED == true
    PrintSeparator("Actual Total Current: " + String(LineCurrentTotal) + " A (CT1~CT2~CT3~CT4)");
#else
    PrintSeparator("Actual Total Current: " + String(LineCurrentTotal) + " A (CT1~CT2~CT3)");
#endif
#endif

    Serial.println("");
  }

  // **************** POWER ****************

  // Calculated Power CT2ased on V*I
#if ATM_SINGLEVOLTAGE == true // Single Mains Voltage for all Current Measurements
  CalculatedPowerCT1 = LineVoltage1 * LineCurrentCT1;
  CalculatedPowerCT2 = LineVoltage1 * LineCurrentCT2;
  CalculatedPowerCT3 = LineVoltage1 * LineCurrentCT3;
  CalculatedPowerCT4 = LineVoltage1 * LineCurrentCT4;
  if (DisplayFull == true) // Display Expanded Information
  {
    PrintUnderline("Calculated RMS Power");
    Serial.println("Power V1*I1: " + String(CalculatedPowerCT1) + " W");
    Serial.println("Power V1*I2: " + String(CalculatedPowerCT2) + " W");
    Serial.println("Power V1*I3: " + String(CalculatedPowerCT3) + " W");
    Serial.println("Power V1*I4: " + String(CalculatedPowerCT4) + " W");
  }
#else // Seperate Mains Voltages for Current Measurements
  CalculatedPowerCT1 = LineVoltage1 * LineCurrentCT1;
  CalculatedPowerCT2 = LineVoltage2 * LineCurrentCT2;
  CalculatedPowerCT3 = LineVoltage3 * LineCurrentCT3;
  CalculatedPowerCT4 = LineVoltage1 * LineCurrentCT4; // ESP32 ADC using V1
  if (DisplayFull == true)                            // Display Expanded Information
  {
    PrintUnderline("Calculated RMS Power");
    Serial.println("Power V1*I1: " + String(CalculatedPowerCT1) + " W");
    Serial.println("Power V2*I2: " + String(CalculatedPowerCT2) + " W");
    Serial.println("Power V3*I3: " + String(CalculatedPowerCT3) + " W");
  }
#if CT4_ENABLED == true
  if (DisplayFull == true) // Display Expanded Information
  {
    Serial.println("Power V1*I4: " + String(CalculatedPowerCT4) + " W");
  }
#endif
#endif

  if (DisplayFull == true) // Display Expanded Information
  {
// Calculate Total Watts
#if ATM_SPLITPHASE == true // USA 120+120
    CalculatedTotalPower = (LineVoltage1 * LineCurrentCT1) + (LineVoltage3 * LineCurrentCT3);
    PrintSeparator("Calculated Total Power: " + String(CalculatedTotalPower) + " W (CT1~X~CT3)");
#else // World
#if CT4_CONFIG == CT4_ESP && CT4_ENABLED == true
    CalculatedTotalPower = (LineVoltage1 * LineCurrentCT1) + (LineVoltage2 * LineCurrentCT2) + (LineVoltage3 * LineCurrentCT3) + (LineVoltage1 * LineCurrentCT4);
    PrintSeparator("Calculated Total Power: " + String(CalculatedTotalPower) + " W (CT1~CT2~CT3~CT4)");
#else
    CalculatedTotalPower = (LineVoltage1 * LineCurrentCT1) + (LineVoltage2 * LineCurrentCT2) + (LineVoltage3 * LineCurrentCT3);
    PrintSeparator("Calculated Total Power: " + String(CalculatedTotalPower) + " W (CT1~CT2~CT3)");
#endif
#endif

    Serial.println("");
  }
  // Active Power.  Extra Import/Export values included to provide added detail for Home Automation logging
  String ActiveFlag = "";
  if (DisplayFull == true) // Display Expanded Information
  {
    PrintUnderline("Active Power (Absorbed or Used by the Load)");
  }
  // Active Power CT1
  ActivePowerCT1 = NoiseFilterSquelch(eic.GetActivePowerCT1(), 0.2);
  ActivePowerImportCT1 = 0;
  ActivePowerExportCT1 = 0;
  ActiveFlag = "";

  if (ActivePowerCT1 > 0)
  {
    ActivePowerImportCT1 = ActivePowerCT1;
    ActiveFlag = "(Import)";
  }

  if (ActivePowerCT1 < 0)
  {
    ActivePowerExportCT1 = -ActivePowerCT1;
    ActiveFlag = "(Export)";
  }
  if (DisplayFull == true) // Display Expanded Information
  {
    Serial.println("Active Power CT1: " + String(ActivePowerCT1) + " W \t" + ActiveFlag);
  }

  // Active Power CT2
  ActivePowerCT2 = NoiseFilterSquelch(eic.GetActivePowerCT2(), 0.2);
  ActivePowerImportCT2 = 0;
  ActivePowerExportCT2 = 0;
  ActiveFlag = "";

  if (ActivePowerCT2 > 0)
  {
    ActivePowerImportCT2 = ActivePowerCT2;
    ActiveFlag = "(Import)";
  }

  if (ActivePowerCT2 < 0)
  {
    ActivePowerExportCT2 = -ActivePowerCT2;
    ActiveFlag = "(Export)";
  }
  if (DisplayFull == true) // Display Expanded Information
  {
    Serial.println("Active Power CT2: " + String(ActivePowerCT2) + " W \t" + ActiveFlag);
  }

  // Active Power CT3
  ActivePowerCT3 = NoiseFilterSquelch(eic.GetActivePowerCT3(), 0.2);
  ActivePowerImportCT3 = 0;
  ActivePowerExportCT3 = 0;
  ActiveFlag = "";

  if (ActivePowerCT3 > 0)
  {
    ActivePowerImportCT3 = ActivePowerCT3;
    ActiveFlag = "(Import)";
  }

  if (ActivePowerCT3 < 0)
  {
    ActivePowerExportCT3 = -ActivePowerCT3;
    ActiveFlag = "(Export)";
  }
  if (DisplayFull == true) // Display Expanded Information
  {
    Serial.println("Active Power CT3: " + String(ActivePowerCT3) + " W \t" + ActiveFlag);
  }

  // Total Active Power
  TotalActivePower = NoiseFilterSquelch(eic.GetTotalActivePower(), 0.2);
  ActiveFlag = "";

  if (TotalActivePower > 0)
  {
    TotalActivePowerImport = TotalActivePower;
    ActiveFlag = "(Import)";
  }

  if (TotalActivePower < 0)
  {
    TotalActivePowerExport = -TotalActivePower;
    ActiveFlag = "(Export)";
  }
  if (DisplayFull == true) // Display Expanded Information
  {
    PrintSeparator("Total Active Power: " + String(TotalActivePower) + " W \t" + ActiveFlag);
    Serial.println("");
  }

  // Re-Active Power
  ReactivePowerCT1 = NoiseFilterSquelch(eic.GetReactivePowerCT1());
  ReactivePowerCT2 = NoiseFilterSquelch(eic.GetReactivePowerCT2());
  ReactivePowerCT3 = NoiseFilterSquelch(eic.GetReactivePowerCT3());
  TotalReactivePower = NoiseFilterSquelch(eic.GetTotalReactivePower());

  if (DisplayFull == true) // Display Expanded Information
  {
    PrintUnderline("Re-Active Power");
    Serial.println("ReActive Power CT1: " + String(ReactivePowerCT1) + " VAR");
    Serial.println("ReActive Power CT2: " + String(ReactivePowerCT2) + " VAR");
    Serial.println("ReActive Power CT3: " + String(ReactivePowerCT3) + " VAR");
    PrintSeparator("Total ReActive Power: " + String(TotalReactivePower) + " VAR (CT1~CT2~CT3)");
    Serial.println("");
  }

  // Apparent Power Flowing - Voltage and Current (V * I)
  ApparentPowerCT1 = NoiseFilterSquelch(eic.GetApparentPowerCT1());
  ApparentPowerCT2 = NoiseFilterSquelch(eic.GetApparentPowerCT2());
  ApparentPowerCT3 = NoiseFilterSquelch(eic.GetApparentPowerCT3());
  TotalApparentPower = NoiseFilterSquelch(eic.GetTotalApparentPower());

  if (DisplayFull == true) // Display Expanded Information
  {
    PrintUnderline("Apparent Power (Total Amount of Power Flowing from Source to Load)");
    Serial.println("Apparent Power CT1: " + String(ApparentPowerCT1) + " VA");
    Serial.println("Apparent Power CT2: " + String(ApparentPowerCT2) + " VA");
    Serial.println("Apparent Power CT3: " + String(ApparentPowerCT3) + " VA");
    PrintSeparator("Total Apparent Power: " + String(TotalApparentPower) + " VA (CT1~CT2~CT3)");
    Serial.println("");
  }

#if ATM90DEVICE == ATM90E32_DEVICE
  if (DisplayFull == true) // Display Expanded Information
  {
    PrintUnderline("Other Power Information");
  }

  // Total Fundamental Power
  TotalActiveFundPower = NoiseFilterSquelch(eic.GetTotalActiveFundPower());
  if (DisplayFull == true) // Display Expanded Information
  {
    Serial.println("Total Fundamental Power: " + String(TotalActiveFundPower) + " pH(t)\n");
  }

  // Total Harmonic Power
  TotalActiveHarPower = NoiseFilterSquelch(eic.GetTotalActiveHarPower());
  if (DisplayFull == true) // Display Expanded Information
  {
    Serial.println("Total Harmonic Power: " + String(TotalActiveHarPower) + " ");
  }

#endif

  Serial.println("");

  // **************** OTHER ****************

  // Power Factor
  PowerFactorCT1 = NoiseFilterSquelch(eic.GetPowerFactorCT1(), 0.01, false);
  PowerFactorCT2 = NoiseFilterSquelch(eic.GetPowerFactorCT2(), 0.01, false);
  PowerFactorCT3 = NoiseFilterSquelch(eic.GetPowerFactorCT3(), 0.01, false);
  TotalPowerFactor = NoiseFilterSquelch(eic.GetTotalPowerFactor(), 0, false);
  if (DisplayFull == true) // Display Expanded Information
  {
    PrintUnderline("Power Factor");
    Serial.println("Power Factor CT1: " + String(PowerFactorCT1) + " PF");
    Serial.println("Power Factor CT2: " + String(PowerFactorCT2) + " PF");
    Serial.println("Power Factor CT3: " + String(PowerFactorCT3) + " PF");
    PrintSeparator("Total Power Factor: " + String(TotalPowerFactor) + " PF");
    Serial.println("");
  }

  // Phase Angle
  PhaseAngleCT1 = NoiseFilterSquelch(eic.GetPhaseCT1(), 0, true, 180);
  PhaseAngleCT2 = NoiseFilterSquelch(eic.GetPhaseCT2(), 0, true, 180);
  PhaseAngleCT3 = NoiseFilterSquelch(eic.GetPhaseCT3(), 0, true, 180);
  if (DisplayFull == true) // Display Expanded Information
  {
    PrintUnderline("Phase Angle");
    Serial.println("Phase Angle CT1: " + String(PhaseAngleCT1));
    Serial.println("Phase Angle CT2: " + String(PhaseAngleCT2));
    Serial.println("Phase Angle CT3: " + String(PhaseAngleCT3));
    Serial.println("");
  }
  if (DisplayFull == true) // Display Expanded Information
  {
    PrintUnderline("Other Information");
  }

  // Chip Temperature
  ChipTemperature = NoiseFilterSquelch(eic.GetTemperature());
  if (DisplayFull == true) // Display Expanded Information
  {
    Serial.println("Chip Temp: " + String(ChipTemperature) + " °C");
    Serial.println("");
  }

  // Line Frequency
  LineFrequency = NoiseFilterSquelch(eic.GetFrequency());
  if (DisplayFull == true) // Display Expanded Information
  {
    Serial.println("Frequency: " + String(LineFrequency) + " Hz");
    Serial.println("");
  }

  // Read PCB NTC Temperature
  ReadPCBTemperature(); // Read PCB NTC Temperature

  Serial.println("");

  // Read PCB DCV_IN Voltage
  ReadADCVoltage(); // Read AC>DC Input Voltage
}

// **************** SETUP ****************
void setup()
{

  // Stabalise
  delay(250);

  // Initialise UART
  Serial.begin(115200, SERIAL_8N1); // 115200
  while (!Serial)
    ;
  Serial.println("");

  // Application Info
  Serial.println("");
  Serial.println(AppName);
  Serial.println("");

  // Domoticz Integration
  if (EnableDomoticz == true)
  {
    Serial.println("Domoticz Enabled - Register Values Will be Published");
  }
  else
  {
    Serial.println("Domoticz Publishing Disabled");
  }

  // IPEM Board Port Configuration
  ConfigureBoard();

  // Hardware Tests
  if (DisableHardwareTest == false)
  {
    TestRGB();    // Cycle RGB LED
    ScanI2CBus(); // Scan I2C Bus and Report Devices
  }

  // Check DCV_IN
  CheckDCVINVoltage();

  // ****************  Initialise the ATM90E3x & Pass related calibrations to its library ****************

// LineFreq = lineFreq, PGAGain = pgagain, VoltageGainX = ugain, CurrentGainCT1 = igainA, CurrentGainCT2 = igainB, CurrentGainCT3 = igainC, CurrentGainCTN = igainN
#if ATM90DEVICE == ATM90E32_DEVICE && ATM_SPLITPHASE == true // ???
// the 2nd (B) current channel is not used with the split phase meter
#if ATM_SINGLEVOLTAGE == true
  eic.begin(LineFreq, PGAGain, VoltageGain1, 0, 0, CurrentGainCT1, 0, CurrentGainCT2);
#else
  eic.begin(LineFreq, PGAGain, VoltageGain1, VoltageGain2, VoltageGain3, CurrentGainCT1, 0, CurrentGainCT2);
#endif

#endif

#if ATM90DEVICE == ATM90E32_DEVICE && ATM_SPLITPHASE == false
#if ATM_SINGLEVOLTAGE == true
  eic.begin(LineFreq, PGAGain, VoltageGain1, 0, 0, CurrentGainCT1, CurrentGainCT2, CurrentGainCT3);
#else
  eic.begin(LineFreq, PGAGain, VoltageGain1, VoltageGain2, VoltageGain3, CurrentGainCT1, CurrentGainCT2, CurrentGainCT3);
#endif
#endif

#if ATM90DEVICE == ATM90E36_DEVICE && ATM_SPLITPHASE == false
#if ATM_SINGLEVOLTAGE == true
  eic.begin(LineFreq, PGAGain, VoltageGain1, 0, 0, CurrentGainCT1, CurrentGainCT2, CurrentGainCT3, CurrentGainCTN);
#else
  eic.begin(LineFreq, PGAGain, VoltageGain1, VoltageGain2, VoltageGain3, CurrentGainCT1, CurrentGainCT2, CurrentGainCT3, CurrentGainCTN);
#endif
#endif

  Serial.println("IPEM Hardware Setup and Power-Up Test - Board Initialized");

  delay(1000);

  Serial.println("");

  DisplayRegisters(); // Display once
} // Setup

// **************** LOOP ****************
void loop()
{

  // Simple test for Loop to bypass EnableBasicLoop if User Button Pressed for ~ 1 Second
  if (EnableBasicLoop == true || digitalRead(User_Button) == LOW)
  {
    DisplayRegisters();
    Serial.println("- - - / / - - -\n");
  }

  // Publish Values to Domoticz (Set WiFi and Indexes)
  if (EnableDomoticz == true)
  {
    DisplayRegisters(false); // Refresh Values and Display.  Default false = Mute Expanded Info to Serial
    PublishDomoticzValues(); // Publish Values to Domoticz
  }

  // Heatbeat LED
  digitalWrite(LED_Blue, LOW);
  delay(50);
  digitalWrite(LED_Blue, HIGH);

  // Loop Delay
  delay(LoopDelay * 1000);
} // loop