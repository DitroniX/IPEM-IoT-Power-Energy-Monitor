/*
  Dave Williams, DitroniX 2019-2023 (ditronix.net)
  IPEM-1 ESP32 ATM90E32 ATM90E36 IoT Power Energy Monitoring Energy Monitor v1.0
  Features include ESP32 IPEM ESP32 ATM90E32 ATM90E36 16bit ADC EEPROM 3Phase 3+1 CT-Clamps Current Voltage Frequency Power Factor GPIO I2C OLED SMPS D1 USB
  PCA 1.2302-201 - Test Code Firmware v1 - 18th February 2023

  The purpose of this test code is to cycle through the various main functions of the board, as shown below, as part of board bring up testing.
  
  Simplified Board Bring Up Test - GPIO - User Button Test

  Remember!
  Set the BOARD to ESP32, 'WEMOS D1 MINI ESP32' DEV Module (or similar).
  You may set the BAUD rate to 921600 to speed up flashing.
  The SDK does NOT need external power to flash.  It will take Power from the USB 5V.
  
  Note: In the default state, upon first power up and during reset, the Green LED may be partially lit. Once programmed and the GPIO defined, the Green LED will go off after power up.

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and github.com/DitroniX
*/

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// **************** INPUTS ****************
#define User_Button 26  // GPIO 26 (DAC2 ADC2_CH9)

// **************** OUTPUTS ****************
#define LED_Red 2    // Red LED
#define LED_Green 4  // Green LED
#define LED_Blue 15  // Blue LED

// **************** SETUP ****************
void setup() {

  // Stabalise
  delay(250);

  // Initialise UART
  Serial.begin(115200, SERIAL_8N1);  // 115200
  while (!Serial)
    ;
  Serial.println("");

  //  Configures the specified LED GPIO as outputs
  pinMode(LED_Red, OUTPUT);
  pinMode(LED_Green, OUTPUT);
  pinMode(LED_Blue, OUTPUT);

  // LEDs Default Off State
  digitalWrite(LED_Red, HIGH);
  digitalWrite(LED_Green, HIGH);
  digitalWrite(LED_Blue, HIGH);

  // Configure GPIO Inputs
  pinMode(User_Button, INPUT_PULLUP);
}

// **************** LOOP ****************
void loop() {

  // Heatbeat Red
  digitalWrite(LED_Red, LOW);
  delay(1000);
  digitalWrite(LED_Red, HIGH);
  delay(100);

  // Check for Button Press
  if (digitalRead(User_Button) == LOW) {

    Serial.println("Button Pressed");

    // Blue
    digitalWrite(LED_Blue, LOW);
    delay(1000);
    digitalWrite(LED_Blue, HIGH);
    delay(100);
  }
}
