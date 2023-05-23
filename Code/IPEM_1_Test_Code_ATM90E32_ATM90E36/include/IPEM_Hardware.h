/*
  Dave Williams, DitroniX 2019-2023 (ditronix.net)
  IPEM-1 ESP32 ATM90E32 ATM90E36 IoT Power Energy Monitoring Energy Monitor v1.0

  Full header information in main.cpp.

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and also github.com/DitroniX
*/

// Libraries
#include <SPI.h>
#include <Wire.h>
#include <driver/adc.h>
#include <EmonLib.h>
#include <IPEM_EEPROM.h>
#include <GyverOLED.h>
#include <ATM90E3x.h>

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// App
String AppVersion = "230523";
String AppName = "DitroniX IPEM-1 ATM90E32 ATM90E36 IoT Power Energy Monitor Board - Development Code";

// App USER
String LocationName = "House"; // Enter Location of Device such as House, Solar etc.  Used for Serial Monitor and OLED.

// Variables
float TemperatureC;      // Temperature
float TemperatureF;      // Temperature
int VoltageSensorRaw;    // ADC Raw Voltage Value
float VoltageCalculated; // Calculated Voltage Value
int VoltagePercentage;   // Voltage Percentage
boolean OLED_Enabled;    // Auto Enabled if OLED Detected on I2C Scan
int OLEDCount;           // OLED Information Counter

// Variables ATM
float LineVoltage1, LineVoltage2, LineVoltage3, LineVoltageTotal, LineVoltageAverage;
float LineCurrentCT1, LineCurrentCT2, LineCurrentCT3, LineCurrentCT4, LineCurrentCTN, LineCurrentTotal;
float CalculatedPowerCT1, CalculatedPowerCT2, CalculatedPowerCT3, CalculatedPowerCT4, CalculatedPowerCTN, CalculatedTotalPower;
float ActivePowerCT1, ActivePowerCT2, ActivePowerCT3, TotalActivePower, CalculatedTotalActivePower;
float ActivePowerImportCT1, ActivePowerImportCT2, ActivePowerImportCT3, TotalActivePowerImport;
float ActivePowerExportCT1, ActivePowerExportCT2, ActivePowerExportCT3, TotalActivePowerExport;
float ReactivePowerCT1, ReactivePowerCT2, ReactivePowerCT3, TotalReactivePower, CalculatedTotalReactivePower;
float ApparentPowerCT1, ApparentPowerCT2, ApparentPowerCT3, TotalApparentPower, CalculatedTotalApparentPower;
float TotalActiveFundPower, TotalActiveHarPower;
float PowerFactorCT1, PowerFactorCT2, PowerFactorCT3, TotalPowerFactor;
float PhaseAngleCT1, PhaseAngleCT2, PhaseAngleCT3;
float ChipTemperature, LineFrequency;

// Variables PCB
float DCVoltage;      // DCV Input Voltage
float PCBTemperature; // PCB NTC Temperature

// Constants USER
int VoltageRawFactor = 0;                // ADC Raw Adjustment for 2048 @ 1.65V Default 0
float VoltageFactor = 39;                // Adjust as needed for Voltage Calibration. Default 39
const int AverageSamples = 25;           // Average Multi-Samples.  Default 25
const int AverageDelay = 20;             // Average Multi-Sample Delay. Default 20
boolean EnableAveraging = true;          // Set to true to enable averaging (ESP32 DCV).  Default true
boolean DisableHardwareTest = false;     // Set to true to speed up booting.  Default false
boolean EnableNoiseFilterSquelch = true; // This realtes to NoiseFilterSquelch Threshold.  false returns raw values.  Default true.

// Constants Application
uint64_t chipid = ESP.getEfuseMac();     // Get ChipID (essentially the MAC address)

// OLED Instance. You will need to select your OLED Display.   Uncomment/Comment as needed.
GyverOLED<SSD1306_128x32, OLED_BUFFER> oled; // 0.6"
// GyverOLED<SSH1106_128x64> oled; // 1.1"
// GyverOLED<SSD1306_128x32, OLED_NO_BUFFER> oled;
// GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
// GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;

// Create an Energy Monitor Library Instance (Used ONLY for CT4)
EnergyMonitor emon1;

// CT4 Energy Monitor Library Configuration
// Initialize EmonLib (111.1 = EmonCalibration value, adjust as needed)
// NB.  EmonCalibration set to 260 for low current bring-up testing < 1.5A with Burden NOT connected.  Update Burden and Values as required.
float EmonCalibration = 260; // Used for bring-up EMON calibration value.  To be calibrated.  Default 1.
float EmonCalcIrms = 7400;   // Calculate EMON Irms. Default 1480
float EmonThreshold = 0.2;   // Used to squelch low values. Default 0.2

// **************** ATM90Ex CALIBRATION SETTINGS GUIDE ****************
// LineFreq = 389 for 50 Hz (World)  4485 for (North America)
// VoltageGain = Depends on Transformer used for Voltage Isolation.  Below values are based on Greenbrook DAT01A (TC TR7) Transformer, set to 12V AC.  https://www.tlc-direct.co.uk/Products/TCTR7.html
// CurrentGainCTx = Depends on CT Clamps Used.  You can sue different on each channel is needed.  Below based on SCT-013-000 100A/50mA

#if ATM90DEVICE == ATM90E32_DEVICE

// The below related to calibration of Voltage inputs (V1, V2 and V3), also Current Clamp inputs (CT1, CT2 and CT3 only).

// Mains Frequency
unsigned short LineFreq = 389; // MMode0 0x33 _lineFreq | Default 389

// Voltage and Current - Overall Gain
// PMPGA 0x17              DDV3V2V1I4I3I2I1  // D = DPGA       4/3/2/1 = V1-4 = I1-4
unsigned short PGAGain = 0b0101010101111111; // PMPGA 0x17  | DPGA Gain = 2 and PGA Gain = 1

// Voltage.
// This is calculated based on the Bell Transformer DAT01 on 12V setting @ ~19V RMS.  Need to allow for +/- ~ 1% Tolerance.
// Calculations: Base value for 240V is 38800.  To increase/decrease change by Approx. ~100 per 1.0 V RMS.
// Calculations: Base value for 120V is 20200.  To increase/decrease change by Approx. ~100 per 1.0 V RMS.
#if ATM_SINGLEVOLTAGE == true
unsigned short VoltageGain1 = 38800;        // uGain = UgainA | 0x61	0x0002 40500 20000 42620 (10000 = ~60V)
unsigned short VoltageGain2 = VoltageGain1; // Duplicate V1 Values to V2 and V3.
unsigned short VoltageGain3 = VoltageGain1; // Duplicate V1 Values to V2 and V3.
#else
unsigned short VoltageGain1 = 38800; // uGain = UgainA | 38800 Default Starting Value
unsigned short VoltageGain2 = 38800; // uGain = UgainB | 38800 Default Starting Value
unsigned short VoltageGain3 = 38800; // uGain = UgainC | 38800 Default Starting Value
#endif

// Current
// This is calculated based on the YDH? 100A/50mA  Need to allow for +/- ~ 1% Tolerance.  Trim values are fine!
// Calculations: Base value for CT100/50 Mid Range 38500.  To increase/decrease change by Approx. ~500 per 0.01A RMS
unsigned short CurrentGainCT1 = 38500; // IgainA 0x62	| CT100/50 Mid Range 38500
unsigned short CurrentGainCT2 = 38500; // IgainA 0x62	| CT100/50 Mid Range 38500
unsigned short CurrentGainCT3 = 38500; // IgainA 0x62	| CT100/50 Mid Range 38500
#endif

#if ATM90DEVICE == ATM90E36_DEVICE

// The below related to calibration of Voltage inputs (V1, V2 and V3), also Current Clamp inputs (CT1, CT2 and CT3 only).

// Mains Frequency
unsigned short LineFreq = 389; // MMode0 0x33 _lineFreq | 389

// Voltage and Current - Overall Gain
// PMPGA 0x17              DDV3V2V1I4I3I2I1  // D = DPGA       4/3/2/1 = V1-4 = I1-4
unsigned short PGAGain = 0b0101010101010101; // PMPGA 0x17  | DPGA Gain = 2 and PGA Gain = 1

// Voltage.
// This is calculated based on the DAT01 on 12V setting @ ~19V RMS.  Need to allow for +/- ~ 1% Tolerance.
// Calculations: Base value for 240V is 20200.  To increase/decrease change by Approx. ~100 per 1.0 V RMS.
// Calculations: Base value for 120V is 9700.  To increase/decrease change by Approx. ~100 per 1.0 V RMS.
#if ATM_SINGLEVOLTAGE == true
unsigned short VoltageGain1 = 20200;        // uGain = UgainA | 20200 Default Starting Value
unsigned short VoltageGain2 = VoltageGain1; // Duplicate V1 Values to V2 and V3.
unsigned short VoltageGain3 = VoltageGain1; // Duplicate V1 Values to V2 and V3.
#else
unsigned short VoltageGain1 = 20200; // uGain = UgainA | 20200 Default Starting Value
unsigned short VoltageGain2 = 20200; // uGain = UgainB | 20200 Default Starting Value
unsigned short VoltageGain3 = 20200; // uGain = UgainC | 20200 Default Starting Value
#endif

// Current
// This is calculated based on the YDH? 100A/50mA  Need to allow for +/- ~ 1% Tolerance.  Trim values are fine!
// Calculations: Base value for CT100/50 Mid Range 33500.  To increase/decrease change by Approx. ~500 per 0.01A RMS
unsigned short CurrentGainCT1 = 33500; // IgainA 0x62
unsigned short CurrentGainCT2 = 33500; // IgainA 0x65
unsigned short CurrentGainCT3 = 33500; // IgainA 0x6A
unsigned short CurrentGainCTN = 33500; // IgainA 0x6E
#endif

// **************** EMONLIB CT4 ****************
#if CT4_CONFIG == CT4_ESP || ATM90DEVICE == ATM90E32_DEVICE

#ifdef ADC_BITS     // If ADC_BITS is already defined as 10 (Depending on the EmonLib used)
#undef ADC_BITS     // Un-Define
#define ADC_BITS 12 // Redefined to increase ESP32 ADC Resolution
#endif
// #define ADC_COUNTS (1 << ADC_BITS)
#define emonTxV3 // Force the library to use 3v3 as virtual Supply Voltage

#endif

// **************** Initialize ATM90E32 or ATM90E36 IC Class ****************
ATM90E3x eic{}; //

// **************** ESP32 INPUTS ****************
#define ATM_WO 25      // GPIO 25 (DAC1 ADC2_CH8)
#define ATM_CF1 27     // GPIO 27 (Digital ADC 2 CH7)
#define ATM_CF2 32     // GPIO 32 (Digital ADC 1 CH4)
#define ATM_CF3 33     // GPIO 33 (Digital ADC 1 CH5)
#define ATM_CF4 34     // GPIO 34 (Digital ADC 1 CH6)
#define ATM_IRQ0 13    // GPIO 13 (GPIO)
#define ATM_IRQ1 14    // GPIO 14 (GPIO)
#define CT4_IN 35      // GPIO 35 (Digital ADC 1 CH7)
#define DCV_IN 36      // GPIO 36 (Analog VP / ADC 1 CH0)
#define NTC_IN 39      // GPIO 39/VN (Analog ADC 1 CH3)
#define User_Button 26 // GPIO 26 (DAC2 ADC2_CH9)

// **************** ESP32 OUTPUTS ****************
#define GP12_DMA_CTRL 12 // GP12 and Auto DMA Control (MUST be LOW on ESP32 Boot)
#define LED_Red 2        // Red LED
#define LED_Green 4      // Green LED
#define LED_Blue 15      // Blue LED

// **************** ESP32 GPIO or UART2 ****************
#define USR_GP16_RX 16 // GPIO 16 (Digital TTL_RXD)
#define USR_GP17_TX 17 // GPIO 17 (Digital TTL_TXD)

// Define I2C (Expansion Port)
#define I2C_SDA 21
#define I2C_SCL 22

/*
  References to ESP32 SPI Expansion Port to ATM90E3x
  * CS = 5
  * MISO = 19
  * MOSI = 23
  * SCK = 18
 */

// **************** FUNCTIONS AND ROUTINES ****************

// Display BIN from Var
void DisplayBIN16(int var)
{
  for (unsigned int i = 0x8000; i; i >>= 1)
  {
    Serial.write(var & i ? '1' : '0');
  }
  Serial.print(" ");
} // DisplayBIN16

// Display Hex from Var
void DisplayHEX(unsigned long var, unsigned char numChars)
{
  unsigned long mask = 0x0000000F;
  mask = mask << 4 * (numChars - 1);

  for (unsigned int i = numChars; i > 0; --i)
  {
    Serial.print(((var & mask) >> (i - 1) * 4), HEX);
    mask = mask >> 4;
  }
  Serial.print(" ");
} // DisplayHEX

// Return Rounded Value to 2 decimals and Remove +/- Noise Threshold.  Default 0.02
float NoiseFilterSquelch(float Value, float Threshold = 0.02, boolean AllowNegative = true, float ValueMax = 0)
{
  if (EnableNoiseFilterSquelch == true) // If false, output is raw value
  {
    Value = roundf(Value * 100) / 100; // Round to 2 Decimal Places

    if (Value >= -Threshold && Value <= Threshold) // Window Comparator / Squelch.  Default 0.2
      Value = 0;

    if (AllowNegative == false && Value < 0) // Negative Value Filter.  Default true
      Value = 0;

    if (ValueMax != 0 && Value > ValueMax) // Maximum Filter.  Default 0
      Value = 0;
  }

  return Value;
} // NoiseFilterSquelch

// Map float values to percentages
float MapValues(float x, float DC_Min, float DC_Max, float Percentage_Min, float Percentage_Max)
{
  return (x - DC_Min) * (Percentage_Max - Percentage_Min) / (DC_Max - DC_Min) + Percentage_Min;
}

// Calculate Average Value and Reduce Jitter
int CalculateADCAverage(int SensorChannel)
{
  int AverageRAW = 0;
  if (EnableAveraging == true)
  {

    for (int i = 0; i < AverageSamples; i++)
    {
      AverageRAW = AverageRAW + analogRead(SensorChannel);
      delay(AverageDelay);
    }
    AverageRAW = AverageRAW / AverageSamples;
    if (AverageRAW < 2)
      AverageRAW = 0;
  }
  else
  {
    AverageRAW = analogRead(SensorChannel);
  }
  return AverageRAW;
} // CalculateADCAverage

// Rough and Ready Underline Text.
void PrintUnderline(String sText)
{
  int count = 0;
  Serial.println(sText);

  while (count <= sText.length())
  {
    Serial.print("─");
    count++;
  }
  Serial.println("");
} // PrintUnderline

// Equally Rough and Ready Dash Separator
void PrintSeparator(String sText)
{
  int count = 0;

  while (count <= (sText.length() / 2) + 1)
  {
    Serial.print("- ");
    count++;
  }
  Serial.println("");

  Serial.println(sText);
} // PrintSeparator

void ConfigureBoard()
{

  // Configure GPIO Inputs
  pinMode(ATM_CF1, INPUT);
  pinMode(ATM_CF2, INPUT);
  pinMode(ATM_CF3, INPUT);
  pinMode(ATM_CF4, INPUT);
  pinMode(ATM_IRQ0, INPUT);
  pinMode(ATM_IRQ1, INPUT);
  pinMode(ATM_WO, INPUT);
  pinMode(User_Button, INPUT_PULLUP);

  // Configure GPIO Outputs
  pinMode(GP12_DMA_CTRL, OUTPUT);
  pinMode(LED_Red, OUTPUT);
  pinMode(LED_Green, OUTPUT);
  pinMode(LED_Blue, OUTPUT);

  // LEDs Default Off State
  digitalWrite(LED_Red, HIGH);
  digitalWrite(LED_Green, HIGH);
  digitalWrite(LED_Blue, HIGH);

  // Initialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);

  // Initialize EEPROM
  InitializeEEPROM();

  // OLED
  oled.init();
  oled.clear();
  oled.setCursor(17, 0);
  oled.setScale(5);
  oled.print("IPEM");
  oled.update();

// Initialize ADC and EmonLib
#if CT4_CONFIG == CT4_ESP || ATM90DEVICE == ATM90E32_DEVICE
  // adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);
  analogReadResolution(ADC_BITS);
  emon1.current(CT4_IN, EmonCalibration);
#endif
} // ConfigureBoard

// Display Board Configuration
void DisplayBoardConfiguration()
{

  // Header
  Serial.println("Register Status and Diagnostic Report");
  Serial.println("IPEM-1 ATM90E32 ATM90E36 IoT Power Energy Monitor");
  Serial.printf("ESP32 Serial ID: %04X", (uint16_t)(chipid >> 32));
  Serial.printf("%08X", (uint32_t)chipid);
  Serial.print("   Firmware Version: ");
  Serial.print(AppVersion);
  Serial.print("   Location: ");
  Serial.println(LocationName);  
  Serial.println("");

  // Stabalise
  delay(100);

// Display Hardware Configuration for ATM90E32 ATM90E36
#if ATM90DEVICE == ATM90E32_DEVICE
  PrintUnderline("Hardware Configuration for ATM90E32");
#endif

#if ATM90DEVICE == ATM90E36_DEVICE
  PrintUnderline("Hardware Configuration for ATM90E36");
#endif

  // ATM Status Values
#if CT4_CONFIG == CT4_ATM && ATM90DEVICE == ATM90E32_DEVICE || CT4_ENABLED == false && ATM90DEVICE == ATM90E32_DEVICE || CT4_ATM == true && CT4_ENABLED == true
  Serial.println("* SOFTWARE CONFIGURATION ERROR *\n* You cannot have an ATM90E32 and I4N Input Selected (Change CT4_CONFIG) *\n");
#endif

#if ATM_SINGLEVOLTAGE == true
  Serial.println("AC Voltage Inputs:\tSingle Input V1 will be used for Current Phase Calculations");
#else
#if ATM_SPLITPHASE == false // World Single, Three Phase or 3 x Single Phases
  Serial.println("AC Voltage Inputs:\tMulti Separated V1 V2 V3 - 3 Phase, or 3 x Single Phase Configuration.");
#else                       // USA Split Phase 120+120
  Serial.println("AC Voltage Inputs:\tMulti Dual V1 and V3 - USA Configuration");
#endif
#endif

#if CT4_ISOLATED == true
  Serial.println("CT4 ISOLATED!.  CT4 Input is Full Isolated and will not be included in Current Calculations");
#endif

#if ATM_SPLITPHASE == true
#if ATM_SINGLEVOLTAGE == false
#if CT4_CONFIG == CT4_ESP && ATM90DEVICE == ATM90E32_DEVICE || CT4_CONFIG == CT4_ESP
  // USA 120+120
  Serial.println("Split AC Voltage:\tDual Input V1 and V3 Enabled - USA Configuration");
  Serial.println("CT Current Clamps:\tPhase Configured for 1 and 3 + 1 Phase (ESP32)");
#else
  // USA 120+120
  Serial.println("Split AC Voltage:\tDual Input V1 + V3 Enabled - USA Configuration");
  Serial.println("CT Current Clamps:\tPhase Configured for 1 and 3 + 1 Phase (I4N)");
#endif
#else
  // Cannot have Split AC Voltage input with Single Voltage Input Selected
  Serial.println("Split AC Voltage:\tConfiguration Error (Check ATM_SINGLEVOLTAGE)");
#endif
#else
  // World
#if CT4_CONFIG == CT4_ESP && CT4_ISOLATED == false && ATM90DEVICE == ATM90E32_DEVICE || CT4_CONFIG == CT4_ESP && CT4_ISOLATED == false
  Serial.println("Split AC Voltage:\tDual Split-Phase (V1-x-V3) Disabled");
  Serial.println("CT Current Clamps:\tConfigured for 1, 2, 3 Phase + 1 Phase (ESP32)");
#else
  Serial.println("Split AC Voltage:\tDual  or Split Voltage Input Disabled");

#if CT4_ISOLATED == false
  Serial.println("CT Current Clamps:\tConfigured for 1, 2, 3 Phase + 1 Phase (I4N)");
#else
  Serial.println("CT Current Clamps:\tConfigured for 1, 2, 3 Phase");
#endif

#endif

#endif

#if CT4_CONFIG == CT4_ESP && CT4_ISOLATED == false || ATM90DEVICE == ATM90E32_DEVICE && CT4_CONFIG == CT4_ESP && CT4_ISOLATED == false
  Serial.println("CT4 Current Input:\tConfigured for ESP32 ADC");
#else
#if CT4_ISOLATED == false
  Serial.print("CT4 Current Input:\tConfigured for I4N on the ATM90E");
  Serial.println(ATM90DEVICE);
#endif
#endif

  Serial.println("");

  // Registers
  String SysStatus0_Value = String(eic.GetSysStatus0(), HEX); // EMMState0
  SysStatus0_Value.toUpperCase();
  String SysStatus1_Value = String(eic.GetSysStatus1(), HEX); // EMMState1
  SysStatus1_Value.toUpperCase();
  String MeterStatus0_Value = String(eic.GetMeterStatus0(), HEX); // EMMIntState0
  MeterStatus0_Value.toUpperCase();
  String MeterStatus1_Value = String(eic.GetMeterStatus1(), HEX); // EMMIntState1
  MeterStatus1_Value.toUpperCase();

  Serial.println("[Status Values]");
  Serial.println("System Status: \t\tS0:0x" + SysStatus0_Value + "\t\tS1:0x" + SysStatus1_Value);
  Serial.println("Meter Status:  \t\tE0:0x" + MeterStatus0_Value + "\t\tE1:0x" + MeterStatus1_Value);

  // Stabalise
  delay(100);

  // if true the MCU is not getting data from the energy meter
  unsigned short sys0 = eic.GetSysStatus0(); // EMMState0
  if (sys0 == 65535 || sys0 == 0)
  {
    Serial.println("");
    Serial.println("*********************************************************************");
    Serial.print("* Error: Not Correct Data from ATM90E");
    Serial.print(ATM90DEVICE);
    Serial.println(" Energy Meter - Check Device *");
    Serial.println("*********************************************************************");
  }

  Serial.println("");
} // DisplayBoardConfiguration

// Test RGB LEDs
void TestRGB()
{

  Serial.println("Testing RGB LED");
  delay(100);

  // Red
  digitalWrite(LED_Red, LOW);
  delay(500);
  digitalWrite(LED_Red, HIGH);
  delay(100);

  // Green
  digitalWrite(LED_Green, LOW);
  delay(500);
  digitalWrite(LED_Green, HIGH);
  delay(100);

  // Blue
  digitalWrite(LED_Blue, LOW);
  delay(500);
  digitalWrite(LED_Blue, HIGH);
  delay(100);

  // White
  digitalWrite(LED_Red, LOW);
  digitalWrite(LED_Green, LOW);
  digitalWrite(LED_Blue, LOW);
  delay(500);
  digitalWrite(LED_Red, HIGH);
  digitalWrite(LED_Green, HIGH);
  digitalWrite(LED_Blue, HIGH);
} // TestRGB

// I2C Bus Scanner
void ScanI2CBus()
{

  byte error, address;
  int nDevices;

  Serial.println("Scanning I2C Bus for Devices ...");

  nDevices = 0;
  for (address = 1; address < 127; address++)
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
      Serial.print(" * I2C device found at address Decimal ");
      if (address < 16)
      {
        Serial.print("0");
      }
      Serial.print(address);
      Serial.print(" = Hex 0x");
      Serial.print(address, HEX);

      switch (address)
      {
      case 0x18 ... 0x1F:
        Serial.print(" MCP9808 Temerature Sensor");
        break;
      case 0x20 ... 0x27:
        Serial.print(" GPIO Expander");
        break;
      case 0x28 ... 0x2B:
        Serial.print(" Digital Pot");
        break;
      case 0x38:
        Serial.print(" Humidity/Temperature Sensor");
        break;
      case 0x40 ... 0x47:
        Serial.print(" Humidity/Temperature Sensor");
        break;
      case 0x48 ... 0x4B:
        Serial.print(" ADS1115, ADS7828 or Sensor");
        break;
      case 0x4C ... 0x4F:
        Serial.print(" Sensor");
        break;
      case 0x3C ... 0x3D:
        Serial.print(" OLED");
        OLED_Enabled = true;
        break;
      case 0x50 ... 0x5F:
        Serial.print(" EEPROM");
        break;
      case 0x76 ... 0x77:
        Serial.print(" BMP/BME Temerature/Humidity/Barometric");
        break;
      }
      Serial.println();

      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print(" * Unknown error at address Decimal ");
      if (address < 16)
      {
        Serial.print("0");
      }
      Serial.print(address);
      Serial.print(" = Hex 0x");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
  {
    Serial.println(" * No I2C devices found. Possible Hardware Issue?");
  }
  else
  {
    Serial.println(" * I2C Bus Scan Complete\n");
  }
} // ScanI2CBus

// Read PCB NTC Thermister on VN
void ReadPCBTemperature()
{
  int Vo;
  float R1 = 10000; // Based on 10K
  float logR2, R2, T;
  // float Tf;
  float tCal = 1.16; // Tweak for Calibration
  float C1 = 1.009249522e-03, C2 = 2.378405444e-04, C3 = 2.019202697e-07;

  Vo = CalculateADCAverage(NTC_IN);
  R2 = R1 * (4095.0 / (float)Vo - tCal);
  logR2 = log(R2);
  T = (1.0 / (C1 + C2 * logR2 + C3 * logR2 * logR2 * logR2));
  TemperatureC = T - 273.15;                        // Centigrade
  TemperatureF = (TemperatureC * 9.0) / 3.3 + 32.0; // Fahrenheit

  // NTC ReadTemperature
  yield();
  Serial.print("PCB Temperature Sensor (NTC_IN VN):\t");
  Serial.print(TemperatureC);
  Serial.println(" ºC");

  PCBTemperature = TemperatureC;
} // ReadPCBTemperature

// Read DCV_IN via VP
void ReadADCVoltage()
{
  // Raw ADC Voltage Output to VoltageSensorRaw
  VoltageSensorRaw = CalculateADCAverage(DCV_IN);
  Serial.print("ADC Raw: ");
  Serial.print(VoltageSensorRaw);
  Serial.print(" > ");

  // Adjust Raw ADC Voltage
  VoltageSensorRaw = VoltageSensorRaw + VoltageRawFactor;
  Serial.print("ADC Adjusted: ");
  Serial.print(VoltageSensorRaw);
  Serial.print(" > ");

  // Calculated ADC Voltage Output
  VoltageCalculated = ((VoltageFactor / 4095.0) * VoltageSensorRaw);
  Serial.print("Calculated: ");
  Serial.print(VoltageCalculated);
  Serial.println(" V");

  DCVoltage = VoltageCalculated;
  Serial.println();
} // ReadADCVoltage

// Check DCV_IN DC Voltage
void CheckDCVINVoltage()
{
  // Raw ADC Voltage Output to VoltageSensorRaw
  VoltageSensorRaw = CalculateADCAverage(DCV_IN);
  VoltageSensorRaw = VoltageSensorRaw + VoltageRawFactor;
  VoltageCalculated = ((VoltageFactor / 4095.0) * VoltageSensorRaw);

  // DCV_IN ReadADCVoltage
  yield();
  if (VoltageCalculated < 5)
    Serial.print("* IPEM Board appears to be only USB Powered.\n* Not all ATM functions will work in this mode\n");
  if (VoltageCalculated > 5)
    Serial.print("AC/DC Voltage Input Detected");
  if (VoltageCalculated > 30)
  {
    Serial.print("* WARNING: Please Check Input Voltage.  Too High! Value ");
    Serial.println(VoltageCalculated);
  }

  DCVoltage = VoltageCalculated;
  Serial.println();
} // CheckDCVINVoltage

// Read CT4 Current (Calculated for Low Current Use around < 1.7A - NO Current Flow Polarity).
// Update Values as required in above Energy Monitor Library Configuration.
void ReadCT4Current()
{
  LineCurrentCT4 = emon1.calcIrms(EmonCalcIrms); // Calculate Irms only.
  LineCurrentCT4 = LineCurrentCT4 / 1000;

  // CT4 Squelch
  LineCurrentCT4 = NoiseFilterSquelch(LineCurrentCT4, EmonThreshold);

  CalculatedPowerCT4 = LineCurrentCT4 * LineVoltage1; // Use Voltage Inpput 1 to calculate Power for CT4
} // ReadCT4Current
