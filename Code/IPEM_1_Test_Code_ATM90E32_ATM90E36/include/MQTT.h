/*
  Dave Williams, DitroniX 2019-2023 (ditronix.net)
  IPEM-1 ESP32 ATM90E32 ATM90E36 IoT Power Energy Monitoring Energy Monitor v1.0

  Full header information in main.cpp.

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and also github.com/DitroniX
*/

// Libraries
#include <Arduino.h>
#include <PubSubClient.h> // https://pubsubclient.knolleary.net/api

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES ****************

// MQTT Server info.  Setup with your MQTT Configration
const char *MQTTServer = "0.0.0.0"; // MQTT Server IP Address (Typically a Fixed Local Address)
int MQTTPort = 1883;                // MQTT Network Port (Default 1883)
const char *MQTTUser = "";          // MQTT User - if applicable
const char *MQTTPassword = "";      // MQTT Password - if applicable
String MQTTTopicMain = "ipem";      // MQTT Default Topic. Exclude the trailing /

// Enable Publishing
boolean EnableMQTT = true; // Change to true, to enable Loop reading and sending data to MQTT.  Default false.
boolean EnableMQTTCallBack = true; // Under Development

// MQTT Object / Client Instance
PubSubClient mqtt_client(wlan_client);

// ######### FUNCTIONS #########

// Callback for Receiving MQTT Messages
void MQTTCallback(char *topic, byte *message, unsigned int length)
{
    Serial.print("MQTT Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)message[i]);
        messageTemp += (char)message[i];
    }
    Serial.println();
} // MQTTCallback

// Publish a Formatted MQTT Message
void PublishMQTTMessage(String SensorName, float SensorValue, int dPlaces = 1, int dWidth = 5, int dQOS = 0)
{
    if (mqtt_client.connect("wlan_client", MQTTUser, MQTTPassword))
    // if (mqtt_client.connected)
    {

        // Green LED
        digitalWrite(LED_Green, LOW);

        // Format Sensor Message to MQTTTopic
        String SensorMessage = MQTTTopicMain + "/" + SensorName;
        int str_len = SensorMessage.length() + 1;
        char MQTTTopic[str_len];
        SensorMessage.toCharArray(MQTTTopic, str_len);

        // Structure MQTT Payload
        char MQTTPayload[10];
        dtostrf(SensorValue, dWidth, dPlaces, MQTTPayload);

        // Update Serial Monitor
        Serial.print("Sending MQTT Message: ");
        Serial.print(MQTTTopic);
        Serial.print("\t");
        Serial.println(MQTTPayload);

        // Publish
        mqtt_client.publish(MQTTTopic, MQTTPayload);
        // mqtt_client.publish("ipem/test/1", 0);

        // Green LED
        digitalWrite(LED_Green, HIGH);
    }
    else
    {
        // Red LED
        digitalWrite(LED_Red, LOW);

        Serial.println("WiFi or MQTT Broker Not Connected");

        // Update OLED
        oled.clear();
        OLEDPrint("Error", 2, 0);
        OLEDPrint("MQTT", 2, 2);
        oled.update();
        delay(1000);

        // Red LED
        digitalWrite(LED_Red, HIGH);

        // Stabalise for slow Access Points
        InitialiseWiFi();
    }
} // PublishMQTTMessage

// Initialise MQTT
void InitialiseMQTT()
{
    if (EnableMQTT == true && WiFi.status() == WL_CONNECTED)
    {
        // Initialise MQTT Connection and Instance
        mqtt_client.setServer(MQTTServer, MQTTPort);

        // if (EnableMQTTCallBack == true)
        //     mqtt_client.setCallback(MQTTCallback);

        Serial.println("Initialised MQTT");

        // mqtt_client.subscribe("Light/Switch");
        // mqtt_client.setCallback(MQTTCallback);
    }
} // InitialiseMQTT

// Publish MQTT Values
void PublishMQTTValues()
{
    // Test Message Example.
    // PublishMQTTMessage("LineVoltage1", LineVoltage1);
    // PublishMQTTMessage("ActivePowerCT1", ActivePowerCT1, 0);
    // PublishMQTTMessage("ActivePowerCT2", ActivePowerCT2, 0);
    // PublishMQTTMessage("ActivePowerCT3", ActivePowerCT3, 0);
    // PublishMQTTMessage("LineFrequency", LineFrequency, 2);

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

    Serial.println("");

} // PublishMQTTValues

// while (!client.connected())
// {
//     Serial.println("Connecting to MQTT...");
//     if (client.connect("ESP32Client", mqttUser, mqttPassword))
//     {
//         Serial.println("connected " + String(client.state()));
//         client.subscribe(topic, "XXXX");
//     }
//     else
//     {
//         Serial.print("failed with state ");
//         Serial.print(client.state());
//         delay(2000);
//     }
