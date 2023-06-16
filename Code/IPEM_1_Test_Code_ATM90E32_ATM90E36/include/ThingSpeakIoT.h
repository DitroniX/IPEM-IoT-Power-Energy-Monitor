/*
  Dave Williams, DitroniX 2019-2023 (ditronix.net)
  IPEM-1 ESP32 ATM90E32 ATM90E36 IoT Power Energy Monitoring Energy Monitor v1.0

  Full header information in main.cpp.

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and also github.com/DitroniX
*/

// Example Publish (play data), can be found on https://thingspeak.com/channels/2179490

// Libraries
#include "ThingSpeak.h" // https://github.com/mathworks/thingspeak-arduino/blob/master/README.md

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES ****************

// ThingSpeak Server info.  Setup with your ThingSpeak Configration
unsigned long myChannelNumber1 = 0; // ThingSpeak Channel ID 1
const char *myWriteAPIKey1 = "";    // ThingSpeak Write API Key for Channel ID 1
unsigned long myChannelNumber2 = 0; // ThingSpeak Channel ID 2
const char *yWriteAPIKey2 = "";     // ThingSpeak Write API Key for Channel ID 2
unsigned long myChannelNumber3 = 0; // ThingSpeak Channel ID 3
const char *myWriteAPIKey3 = "";    // ThingSpeak Write API Key for Channel ID 3
unsigned long myChannelNumber4 = 0; // ThingSpeak Channel ID 4
const char *myWriteAPIKey4 = "";    // ThingSpeak Write API Key for Channel ID 4

// Enable Publishing
boolean EnableThingSpeak = false; // Change to true, to enable Loop reading and sending data to ThingSpeak.  Default false.

// Initialise ThingSpeak
void InitialiseThingSpeak()
{
  if (EnableThingSpeak == true && WiFi.status() == WL_CONNECTED)
  {
    ThingSpeak.begin(wlan_client); // Initialize ThingSpeak #################################################################
    Serial.println("Initialised ThingSpeak");
  }

} // void InitialiseThingSpeak()

// Publish a Multi-Field Formatted ThingSpeak Message
void SetThingSpeakField(unsigned long ChannelID, float SensorValue, int dPlaces = 1, int dWidth = 5)
{
  // Green LED
  digitalWrite(LED_Green, LOW);

  // Structure ThingSpeak Payload
  char MQTTPayload[10];
  dtostrf(SensorValue, dWidth, dPlaces, MQTTPayload);

  // Update Serial Monitor
  Serial.print("Sending ThingSpeak Message: ");
  Serial.print(ChannelID);
  Serial.print("\t");
  Serial.println(MQTTPayload);

  ThingSpeak.setField(ChannelID, MQTTPayload);

  // Green LED
  digitalWrite(LED_Green, HIGH);

} // PublishThingSpeakMessage

// Publish ThingSpeak Values and Channels
void PublishThingSpeakValues()
{

  // ms publish every 5 sec

  // Examples to Set the Fields with Values [fields 1-8 available, variable, Decimal Places]
  SetThingSpeakField(1, LineVoltage1, 1);
  SetThingSpeakField(2, ActivePowerCT1, 1);
  SetThingSpeakField(3, ActivePowerCT2, 1);
  SetThingSpeakField(4, ActivePowerCT3), 2;
  SetThingSpeakField(5, CalculatedTotalPower, 1);
  SetThingSpeakField(6, TotalPowerFactor, 2);
  SetThingSpeakField(7, LineFrequency, 2);
  SetThingSpeakField(8, PCBTemperature), 1;

  /* Examples of Available Variables
    LineVoltage1, LineVoltage2, LineVoltage3, LineVoltageTotal, LineVoltageAverage
    LineCurrentCT1, LineCurrentCT2, LineCurrentCT3, LineCurrentCT4, LineCurrentCTN, LineCurrentTotal
    CalculatedPowerCT1, CalculatedPowerCT2, CalculatedPowerCT3, CalculatedPowerCT4, CalculatedPowerCTN, CalculatedTotalPower
    ActivePowerCT1, ActivePowerCT2, ActivePowerCT3, TotalActivePower, CalculatedTotalActivePower
    ActivePowerImportCT1, ActivePowerImportCT2, ActivePowerImportCT3, TotalActivePowerImport
    ActivePowerExportCT1, ActivePowerExportCT2, ActivePowerExportCT3, TotalActivePowerExport
    ReactivePowerCT1, ReactivePowerCT2, ReactivePowerCT3, TotalReactivePower, CalculatedTotalReactivePower
    ApparentPowerCT1, ApparentPowerCT2, ApparentPowerCT3, TotalApparentPower, CalculatedTotalApparentPower
    TotalActiveFundPower, TotalActiveHarPower
    PowerFactorCT1, PowerFactorCT2, PowerFactorCT3, TotalPowerFactor
    PhaseAngleCT1, PhaseAngleCT2, PhaseAngleCT3
    ChipTemperature, LineFrequency
    DCVoltage, PCBTemperature
    PWMLocalPower, PWMRemotePower, PWMPowerOutput, PWMPowerPercentage
    DACLocalPower, DACRemotePower, DACPowerPercentage, DACPowerOutput

    *Control
    EnablePWMLocal, EnablePWMRemote, EnablePWMTestOutput
    EnableDACLocal, EnableDACRemote, EnableDACTestOutput
  */

  // NTP Time and Pre-Format Example Message for Channel Status Updates
  timeClient.update();
  ThingSpeak.setStatus(HostName + " Last Test Publish: " + timeClient.getFormattedTime());

  // Publish a Multi-Field Formatted ThingSpeak Channel.  Return Codes
  int x = ThingSpeak.writeFields(myChannelNumber1, myWriteAPIKey1);

  if (x == 200) // OK / Success
  {
    Serial.println("ThingSpeak Channel " + String(myChannelNumber1) + " Successfully Updated");
  }
  else if (x == 404) //	Incorrect API key (or invalid ThingSpeak server address)
  {
    // Red LED
    digitalWrite(LED_Red, LOW);

    Serial.println("ThingSpeak Channel " + String(myChannelNumber1) + " Not Updated. Key Error (" + String(x) + ")");

    // Update OLED
    oled.clear();
    OLEDPrint("Error 404", 2, 0);
    OLEDPrint("ThingSpeak", 2, 2);
    oled.update();
    delay(1000);

    // Red LED
    digitalWrite(LED_Red, HIGH);
  }
  else if (x == -401) //	Point was not inserted (most probable cause is the rate limit of once every 15 seconds)
  {
    // Red LED
    digitalWrite(LED_Red, LOW);

    Serial.println("ThingSpeak Channel " + String(myChannelNumber1) + " Not Updated. Rate Error (" + String(x) + ")");

    // Update OLED
    oled.clear();
    OLEDPrint("Error 401", 2, 0);
    OLEDPrint("ThingSpeak", 2, 2);
    oled.update();
    delay(1000);

    // Red LED
    digitalWrite(LED_Red, HIGH);
  }
  else
  {
    // Red LED
    digitalWrite(LED_Red, LOW);

    Serial.println("ThingSpeak Channel " + String(myChannelNumber1) + " Not Updated. Other Error (" + String(x) + ")");

    // Update OLED
    oled.clear();
    OLEDPrint("Error ?", 2, 0);
    OLEDPrint("ThingSpeak", 2, 2);
    oled.update();
    delay(1000);

    // Red LED
    digitalWrite(LED_Red, HIGH);
  }

  Serial.println("");

} // PublishThingSpeakValues
