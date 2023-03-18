/*
  Dave Williams, DitroniX 2019-2023 (ditronix.net)
  IPEM-1 ESP32 ATM90E32 ATM90E36 IoT Power Energy Monitoring Energy Monitor v1.0
  Features include ESP32 IPEM ESP32 ATM90E32 ATM90E36 16bit ADC EEPROM 3Phase 3+1 CT-Clamps Current Voltage Frequency Power Factor GPIO I2C OLED SMPS D1 USB
  PCA 1.2302-101 - Test Code Firmware v1 - 18th February 2023

  The purpose of this test code is to cycle through the various main functions of the board, as shown below, as part of board bring up testing.
  Simplified Board Bring Up Test - DCV_IN Voltage Sensor - PCA DC Supply (Output sent to the Serial Print)

  Remember!
  Set the BOARD to ESP32, 'WEMOS D1 MINI ESP32' DEV Module (or similar).
  You may set the BAUD rate to 921600 to speed up flashing.
  The SDK does NOT need external power to flash.  It will take Power from the USB 5V.
  
  Note: In the default state, upon first power up and during reset, the Green LED may be partially lit. Once programmed and the GPIO defined, the Green LED will go off after power up.

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and github.com/DitroniX
*/

// Libraries
#include <driver/adc.h>

// ****************  VARIABLES / DEFINES / STATIC ****************

// Constants
const int LoopDelay = 1; // Loop Delay in Seconds

// Variables
float ADC_Voltage;
float ADC_Constant;

// **************** OUTPUTS ****************
#define LED_Red 2 // Red LED
#define LED_Green 4 // Green LED

// **************** INPUTS ****************
#define DCV_IN 36 //GPIO 36 (Analog VP / ADC 1 CH0)

void setup() {

  // Stabalise
  delay(250);

  // Initialise UART:
  Serial.begin(115200, SERIAL_8N1);  //115200
  while (!Serial);
  Serial.println("");

  // LEDs
  pinMode(LED_Red, OUTPUT);
  pinMode(LED_Green, OUTPUT);

  // LEDs Default Off State
  digitalWrite(LED_Red, HIGH);
  digitalWrite(LED_Green, HIGH);

}

void loop() {

  // ESP32 ADC 12-Bit SAR (Successive Approximation Register)
  // Conversion resolution 0 - 4095 (4096)
  // You may need to calibrate as needed.

  ADC_Constant = 31.340; // Adjust as needed for calibration of VDC_IN.
  ADC_Voltage  = (analogRead(DCV_IN) * ADC_Constant) / 4095;

  // Update the display
  Serial.print(ADC_Voltage);
  Serial.println(" V DCV_IN");

  // Heatbeat LED
  digitalWrite(LED_Red, LOW);
  delay(50);
  digitalWrite(LED_Red, HIGH);

  // Loop Delay
  delay(LoopDelay * 1000);

}
