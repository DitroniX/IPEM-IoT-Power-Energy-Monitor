/*
  Dave Williams, DitroniX 2019-2023 (ditronix.net)
  IPEM-1 ESP32 ATM90E32 ATM90E36 IoT Power Energy Monitoring Energy Monitor v1.0
  Features include ESP32 IPEM ESP32 ATM90E32 ATM90E36 16bit ADC EEPROM 3Phase 3+1 CT-Clamps Current Voltage Frequency Power Factor GPIO I2C OLED SMPS D1 USB
  PCA 1.2302-201 - Test Code Firmware v1 - 18th February 2023

  The purpose of this test code is to cycle through the various main functions of the board, as shown below, as part of board bring up testing.
  
  Simplified Board Bring Up Test - Wifi Scan (Output to Serial Print)

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
#include <WiFi.h>

// ****************  VARIABLES / DEFINES / STATIC ****************

// Constants
const int LoopDelay = 1; // Loop Delay in Seconds

// **************** OUTPUTS ****************
#define LED_Red 2 // Red LED
#define LED_Green 4 // Green LED

WiFiClient client; // Initialize the client library

void setup() {

  // Stabalise
  delay(250);

  // Initialize UART:
  Serial.begin(115200, SERIAL_8N1);  //115200
  while (!Serial);

  //  Configures the specified LED GPIO as outputs
  pinMode(LED_Red, OUTPUT);
  pinMode(LED_Green, OUTPUT);

  // LED Default Off State
  digitalWrite(LED_Red, HIGH);
  digitalWrite(LED_Green, HIGH);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
}

void loop() {

  Serial.println("ESP32 WiFi MAC \t " + WiFi.macAddress());
  Serial.println("Scanning for Access Points or Routers.  Please wait....");
  delay(100);

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.print("Scan Complete.  ");
  if (n == 0) {
    Serial.println("No Access Points or Routers Found.");
  } else {
    Serial.print(n);
    Serial.println(" Access Points or Routers Found.");
    Serial.println("");

    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": \t");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(100);
    }
  }

  Serial.println("");

  // Heatbeat LED
  digitalWrite(LED_Red, LOW);
  delay(50);
  digitalWrite(LED_Red, HIGH);

  delay(LoopDelay * 1000);
}
