/*
  Dave Williams, DitroniX 2019-2023 (ditronix.net)
  IPEM-1 ESP32 ATM90E32 ATM90E36 IoT Power Energy Monitoring Energy Monitor v1.0

  Full header information in main.cpp.

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and also github.com/DitroniX
*/

// Libraries
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES ****************

// WiFi.  Setup with your Wireless Information
const char *ssid = "xxxx";       // WiFi Network SSID - Case Sensitive
const char *password = "xxxx";   // WiFi Network password - Case Sensitive
WiFiClient client;               // Initialize the client library
String HostNameHeader = "IPEM-"; // Hostname Prefix

// Domoticz Server info.  Setup with your Domoticz IP and Port
const char *domoticz_server = "0.0.0.0"; // Domoticz Server IP Address (Typically a Fixed Local Address)
int port = 8080;                         // Domoticz Network Port (Default)
boolean EnableDomoticz = false;          // Change to true to enable read Loop and sending data to Domoticz.

// NTP Time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Domoticz Hardware Device Indexes

// Set these values to the Domoticz Devices Indexes (IDX).  If Zero, then entry is ignored. Device needs to be created in Domoticz.
int idxLineVoltage1 = 0;       // LineVoltage1 - Urms - Line 1 Voltage RMS
int idxLineVoltage2 = 0;       // LineVoltage2 - Urms - Line 2 Voltage RMS
int idxLineVoltage3 = 0;       // LineVoltage3 - Urms - Line 3 Voltage RMS
int idxLineVoltageAverage = 0; // LineVoltageAverage - Urms - Line Average Voltage RMS (V1+V2+V3/3)
int idxLineVoltageTotal = 0;   // LineVoltageTotal - Urms - Line Voltage Total RMS (V1+0+V3)

int idxLineCurrentCT1 = 0;   // LineCurrentCT1 - Irms - Line 1 Current RMS
int idxLineCurrentCT2 = 0;   // LineCurrentCT2 - Irms - Line 2 Current RMS
int idxLineCurrentCT3 = 0;   // LineCurrentCT3 - Irms - Line 3 Current RMS
int idxLineCurrentCT4 = 0;   // LineCurrentCT4 - Irms - Line 4 Current RMS (ESP)
int idxLineCurrentCTN = 0;   // LineCurrentCTN - Irms - Line N Current RMS
int idxLineCurrentTotal = 0; // LineCurrentTotal - Irms - Line Total Current RMS (CT1+CT2+CT3+(CT4))

int idxCalculatedPowerCT1 = 0;   // Calculated Power V*A Watts RMS
int idxCalculatedPowerCT2 = 0;   // Calculated Power V*A Watts RMS
int idxCalculatedPowerCT3 = 0;   // Calculated Power V*A Watts RMS
int idxCalculatedPowerCT4 = 0;   // Calculated Power V*A Watts RMS (ESP)
int idxCalculatedTotalPower = 0; // CalculatedTotalPower

int idxActivePowerCT1 = 0;   // ActivePowerCT1 - Pmean - Line Mean Active Power
int idxActivePowerCT2 = 0;   // ActivePowerCT2 - Pmean - Line Mean Active Power
int idxActivePowerCT3 = 0;   // ActivePowerCT3 - Pmean - Line Mean Active Power
int idxTotalActivePower = 0; // TotalActivePower - Pmean - Line Mean Active Power

int idxActivePowerImportCT1 = 0;   // ActivePowerCT1 - Pmean - Line Mean Active Power Import
int idxActivePowerImportCT2 = 0;   // ActivePowerCT2 - Pmean - Line Mean Active Power Import
int idxActivePowerImportCT3 = 0;   // ActivePowerCT3 - Pmean - Line Mean Active Power Import
int idxTotalActivePowerImport = 0; // TotalActivePower - Pmean - Line Mean Active Power Import

int idxActivePowerExportCT1 = 0;   // ActivePowerCT1 - Pmean - Line Mean Active Power Export
int idxActivePowerExportCT2 = 0;   // ActivePowerCT2 - Pmean - Line Mean Active Power Export
int idxActivePowerExportCT3 = 0;   // ActivePowerCT3 - Pmean - Line Mean Active Power Export
int idxTotalActivePowerExport = 0; // TotalActivePower - Pmean - Line Mean Active Power Export

int idxReactivePowerCT1 = 0;   // ReactivePowerCT1 - Pmean - Line Mean Active Power
int idxReactivePowerCT2 = 0;   // ReactivePowerCT2 - Pmean - Line Mean Active Power
int idxReactivePowerCT3 = 0;   // ReactivePowerCT3 - Pmean - Line Mean Active Power
int idxTotalReactivePower = 0; // ReTotalActivePower - Pmean - Line Mean Active Power

int idxApparentPowerCT1 = 0;   // ApparentPowerCT1 - Pmean - Line Mean Apparent Power
int idxApparentPowerCT2 = 0;   // ApparentPowerCT2 - Pmean - Line Mean Apparent Power
int idxApparentPowerCT3 = 0;   // ApparentPowerCT3 - Pmean - Line Mean Apparent Power
int idxTotalApparentPower = 0; // TotalApparentPower - Pmean - Line Mean Apparent Power

int idxTotalActiveFundPower = 0; // TotalActiveFundPower
int idxTotalActiveHarPower = 0;  // TotalActiveHarPower

int idxPowerFactorCT1 = 0;   // PowerFactorCT1 - PowerF - Line Power Factor
int idxPowerFactorCT2 = 0;   // PowerFactorCT2 - PowerF - Line Power Factor
int idxPowerFactorCT3 = 0;   // PowerFactorCT3 - PowerF - Line Power Factor
int idxTotalPowerFactor = 0; // TotalPowerFactor - PowerF - Line Power Factor

int idxPhaseAngleCT1 = 0; // PhaseAngleCT1 - Phase Angle
int idxPhaseAngleCT2 = 0; // PhaseAngleCT2 - Phase Angle
int idxPhaseAngleCT3 = 0; // PhaseAngleCT3 - Phase Angle

int idxChipTemperature = 0; // ChipTemperature ATM90E3x Internal Chip Temperature °C
int idxLineFrequency = 0;   // LineFrequency - Line Voltage Frequency

int idxDCVoltage = 0;      // PCB DC Input (Derived from AC)
int idxPCBTemperature = 0; // PCB NTC °C

// Set this value to the Domoticz Device Group Index (IDX) - Note: Currently Unused Virtual Device.
int idxDomoticzBase = 0; // If Zero, then entry is ignored.  Group device needs to be created in Domoticz. WIP.

// ######### FUNCTIONS #########

// Domoticz Integration
void DomoticzIntegration()
{
    if (EnableDomoticz == true)
    {
        Serial.println("Domoticz Enabled - Register Values Will be Published");
    }
    else
    {
        Serial.println("Domoticz Publishing Disabled");
    }
} // DomoticzIntegration

// Initialise WiFi
void InitialiseWiFi()
{
    // Connect or reconnect to WiFi
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Attempting to connect to " + String(ssid) + "\n");

        // Force Hostname
        String Hostname = HostNameHeader;
        Hostname.concat(WiFi.macAddress().substring(WiFi.macAddress().length() - 5, WiFi.macAddress().length()));
        Hostname.replace(":", "");
        WiFi.setHostname(Hostname.c_str());

        // Wifi Initialisation
        WiFi.begin(ssid, password);

        // Stabalise for slow Access Points
        delay(2000);

        // Wifi Settings
        WiFi.mode(WIFI_STA);
        WiFi.setAutoReconnect(true);
        WiFi.persistent(true);

        // Stabalise for slow Access Points
        delay(1000);

        // Wifi Information
        PrintUnderline("Connection Details:");
        Serial.println("WiFi SSID \t " + String(ssid) + "(Wifi Station Mode)");
        Serial.printf("WiFi IP \t %s\n", WiFi.localIP().toString().c_str());
        Serial.printf("WiFi GW \t %s\n", WiFi.gatewayIP().toString().c_str());
        Serial.printf("WiFi MASK \t %s\n", WiFi.subnetMask().toString().c_str());
        Serial.println("WiFi MAC \t " + WiFi.macAddress());
        Serial.printf("WiFi Hostname \t %s\n", WiFi.getHostname());
        Serial.println("WiFi RSSI \t " + String(WiFi.RSSI()));
        Serial.println("");

        timeClient.begin();
    }
} // InitialiseWiFi

// Publish to Domoticz - Single Values
void PublishDomoticz(int Sensor_Index, float Sensor_Value, String Sensor_Name = "")
{

    if (Sensor_Index > 0)
    {
        if (client.connect(domoticz_server, port))
        {
            Serial.print("Sending Message to Domoticz #");
            Serial.print(Sensor_Index);
            Serial.print(" ");
            Serial.print(Sensor_Value);
            Serial.print(" \t");
            Serial.println(Sensor_Name);

            client.print("GET /json.htm?type=command&param=udevice&idx=");
            client.print(Sensor_Index);

            client.print("&svalue=");
            client.print(Sensor_Value);

            client.println(" HTTP/1.1");
            client.print("Host: ");
            client.print(domoticz_server);
            client.print(":");

            client.println(port);
            client.println("User-Agent: Arduino-ethernet");
            client.println("Connection: close");
            client.println();

            client.stop();
        }
        else
        {
            Serial.println("WiFi or Domoticz Server Not Connected");

            // Stabalise for slow Access Points
            delay(2000);
            InitialiseWiFi();
        }
    }
} // PublishDomoticz

// Publish to Domoticz - Listed Values
void PublishDomoticzValues()
{

    if (WiFi.status() == WL_CONNECTED)
    {

        // Voltage LineVoltage1, LineVoltage2, LineVoltage3, LineVoltageTotal, LineVoltageAverage
        if (idxLineVoltage1 > 0)
        {
            PublishDomoticz(idxLineVoltage1, LineVoltage1, "LineVoltage1");
        }

        if (idxLineVoltage2 > 0)
        {
            PublishDomoticz(idxLineVoltage2, LineVoltage2, "LineVoltage2");
        }

        if (idxLineVoltage3 > 0)
        {
            PublishDomoticz(idxLineVoltage3, LineVoltage3, "LineVoltage3");
        }

        if (idxLineVoltageTotal > 0)
        {
            PublishDomoticz(idxLineVoltageTotal, LineVoltageTotal, "LineVoltageTotal");
        }

        if (idxLineVoltageAverage > 0)
        {
            PublishDomoticz(idxLineVoltageAverage, LineVoltageAverage, "LineVoltageAverage");
        }

        // Current LineCurrentCT1, LineCurrentCT2, LineCurrentCT3, LineCurrentCT4, LineCurrentCTN, LineCurrentTotal
        if (idxLineCurrentCT1 > 0)
        {
            PublishDomoticz(idxLineCurrentCT1, LineCurrentCT1, "LineCurrentCT1");
        }

        if (idxLineCurrentCT2 > 0)
        {
            PublishDomoticz(idxLineCurrentCT2, LineCurrentCT2, "LineCurrentCT2");
        }

        if (idxLineCurrentCT3 > 0)
        {
            PublishDomoticz(idxLineCurrentCT3, LineCurrentCT3, "LineCurrentCT3");
        }
        if (idxLineCurrentCT4 > 0)
        {
            PublishDomoticz(idxLineCurrentCT4, LineCurrentCT4, "LineCurrentCT4");
        }
        if (idxLineCurrentCTN > 0)
        {
            PublishDomoticz(idxLineCurrentCTN, LineCurrentCTN, "LineCurrentCTN");
        }
        if (idxLineCurrentTotal > 0)
        {
            PublishDomoticz(idxLineCurrentTotal, LineCurrentTotal, "LineCurrentTotal");
        }

        // Calculated Power CalculatedPowerCT1, CalculatedPowerCT2, CalculatedPowerCT3, CalculatedPowerCT4, CalculatedTotalPower
        if (idxCalculatedPowerCT1 > 0)
        {
            PublishDomoticz(idxCalculatedPowerCT1, CalculatedPowerCT1, "CalculatedPowerCT1");
        }
        if (idxCalculatedPowerCT2 > 0)
        {
            PublishDomoticz(idxCalculatedPowerCT2, CalculatedPowerCT2, "CalculatedPowerCT2");
        }
        if (idxCalculatedPowerCT3 > 0)
        {
            PublishDomoticz(idxCalculatedPowerCT3, CalculatedPowerCT3, "CalculatedPowerCT3");
        }
        if (idxCalculatedPowerCT4 > 0)
        {
            PublishDomoticz(idxCalculatedPowerCT4, CalculatedPowerCT4, "CalculatedPowerCT4");
        }
        if (idxCalculatedTotalPower > 0)
        {
            PublishDomoticz(idxCalculatedTotalPower, CalculatedTotalPower, "CalculatedTotalPower");
        }

        // Active Power ActivePowerCT1, ActivePowerCT2, ActivePowerCT3, TotalActivePower
        if (idxActivePowerCT1 > 0)
        {
            PublishDomoticz(idxActivePowerCT1, ActivePowerCT1, "ActivePowerCT1");
        }
        if (idxActivePowerCT2 > 0)
        {
            PublishDomoticz(idxActivePowerCT2, ActivePowerCT2, "ActivePowerCT2");
        }
        if (idxActivePowerCT3 > 0)
        {
            PublishDomoticz(idxActivePowerCT3, ActivePowerCT3, "ActivePowerCT3");
        }
        if (idxTotalActivePower > 0)
        {
            PublishDomoticz(idxTotalActivePower, TotalActivePower, "TotalActivePower");
        }

        // Active Power Import ActivePowerImportCT1, ActivePowerImportCT2, ActivePowerImportCT3, TotalActivePowerImport
        if (idxActivePowerImportCT1 > 0)
        {
            PublishDomoticz(idxActivePowerImportCT1, ActivePowerImportCT1, "ActivePowerImportCT1");
        }
        if (idxActivePowerImportCT2 > 0)
        {
            PublishDomoticz(idxActivePowerImportCT2, ActivePowerImportCT2, "ActivePowerImportCT2");
        }
        if (idxActivePowerImportCT3 > 0)
        {
            PublishDomoticz(idxActivePowerImportCT3, ActivePowerImportCT3, "ActivePowerImportCT3");
        }
        if (idxTotalActivePowerImport > 0)
        {
            PublishDomoticz(idxTotalActivePowerImport, TotalActivePowerImport, "TotalActivePowerImport");
        }

        // Active Power Export ActivePowerExportCT1, ActivePowerExportCT2, ActivePowerExportCT3, TotalActivePowerExport
        if (idxActivePowerExportCT1 > 0)
        {
            PublishDomoticz(idxActivePowerExportCT1, ActivePowerExportCT1, "ActivePowerExportCT1");
        }
        if (idxActivePowerExportCT2 > 0)
        {
            PublishDomoticz(idxActivePowerExportCT2, ActivePowerExportCT2, "ActivePowerExportCT2");
        }
        if (idxActivePowerExportCT3 > 0)
        {
            PublishDomoticz(idxActivePowerExportCT3, ActivePowerExportCT3, "ActivePowerExportCT3");
        }
        if (idxTotalActivePowerExport > 0)
        {
            PublishDomoticz(idxTotalActivePowerExport, TotalActivePowerExport, "TotalActivePowerExport");
        }

        // Reactive Power ReactivePowerCT1, ReactivePowerCT2, ReactivePowerCT3, TotalReactivePower
        if (idxReactivePowerCT1 > 0)
        {
            PublishDomoticz(idxReactivePowerCT1, ReactivePowerCT1, "ReactivePowerCT1");
        }
        if (idxReactivePowerCT2 > 0)
        {
            PublishDomoticz(idxReactivePowerCT2, ReactivePowerCT2, "ReactivePowerCT2");
        }
        if (idxReactivePowerCT3 > 0)
        {
            PublishDomoticz(idxReactivePowerCT3, ReactivePowerCT3, "ReactivePowerCT3");
        }
        if (idxTotalReactivePower > 0)
        {
            PublishDomoticz(idxTotalReactivePower, TotalReactivePower, "TotalReactivePower");
        }

        // Apparent Popwer ApparentPowerCT1, ApparentPowerCT2, ApparentPowerCT3, TotalApparentPower
        if (idxApparentPowerCT1 > 0)
        {
            PublishDomoticz(idxApparentPowerCT1, ApparentPowerCT1, "ApparentPowerCT1");
        }
        if (idxApparentPowerCT2 > 0)
        {
            PublishDomoticz(idxApparentPowerCT2, ApparentPowerCT2, "ApparentPowerCT2");
        }
        if (idxApparentPowerCT3 > 0)
        {
            PublishDomoticz(idxApparentPowerCT3, ApparentPowerCT3, "ApparentPowerCT3");
        }
        if (idxTotalApparentPower > 0)
        {
            PublishDomoticz(idxTotalApparentPower, TotalApparentPower, "TotalApparentPower");
        }

        // ActivePower TotalActiveFundPower, TotalActiveHarPower
        if (idxTotalActiveFundPower > 0)
        {
            PublishDomoticz(idxTotalActiveFundPower, TotalActiveFundPower, "TotalActiveFundPower");
        }
        if (idxTotalActiveHarPower > 0)
        {
            PublishDomoticz(idxTotalActiveHarPower, TotalActiveHarPower, "TotalActiveHarPower");
        }

        // Power Factor PowerFactorCT1, PowerFactorCT2, PowerFactorCT3, TotalPowerFactor
        if (idxPowerFactorCT1 > 0)
        {
            PublishDomoticz(idxPowerFactorCT1, PowerFactorCT1, "PowerFactorCT1");
        }
        if (idxPowerFactorCT2 > 0)
        {
            PublishDomoticz(idxPowerFactorCT2, PowerFactorCT2, "PowerFactorCT2");
        }
        if (idxPowerFactorCT3 > 0)
        {
            PublishDomoticz(idxPowerFactorCT3, PowerFactorCT3, "PowerFactorCT3");
        }
        if (idxTotalPowerFactor > 0)
        {
            PublishDomoticz(idxTotalPowerFactor, TotalPowerFactor, "TotalPowerFactor");
        }

        // Phase Angle PhaseAngleCT1, PhaseAngleCT2, PhaseAngleCT3
        if (idxPhaseAngleCT1 > 0)
        {
            PublishDomoticz(idxPhaseAngleCT1, PhaseAngleCT1, "PhaseAngleCT1");
        }
        if (idxPhaseAngleCT2 > 0)
        {
            PublishDomoticz(idxPhaseAngleCT2, PhaseAngleCT2, "PhaseAngleCT2");
        }
        if (idxPhaseAngleCT3 > 0)
        {
            PublishDomoticz(idxPhaseAngleCT3, PhaseAngleCT3, "PhaseAngleCT3");
        }
        // Other ChipTemperature, LineFrequency, DCVoltage, PCBTemperature
        if (idxChipTemperature > 0)
        {
            PublishDomoticz(idxChipTemperature, ChipTemperature, "ChipTemperature");
        }
        if (idxLineFrequency > 0)
        {
            PublishDomoticz(idxLineFrequency, LineFrequency, "PhaLineFrequencyseAngleC");
        }
        if (idxDCVoltage > 0)
        {
            PublishDomoticz(idxDCVoltage, DCVoltage, "DCVoltage");
        }
        if (idxPCBTemperature > 0)
        {
            PublishDomoticz(idxPCBTemperature, PCBTemperature, "PCBTemperature");
        }

        // NTP Time
        timeClient.update();
        Serial.println("\n" + timeClient.getFormattedTime() + "> Published to Domoticz");
    }
    else
    {
        InitialiseWiFi();
    }
} // PublishDomoticzValues

// It is possible to post a Group of Values to a single Virtual Sensor and graph it (i.e. Voltage, Current, Wattage).
// This will require some coding in Domoticz (possible plugin/sensor type).  Feedback welcomed!.
// Publish to Domoticz EXAMPLE - Batch or Group Values Example to Virtual Sensor.  Update as needed.  Future WIP Option.
void PublishDomoticzATM(int Sensor_Index)
{
    if (Sensor_Index > 0)
    {
        if (client.connect(domoticz_server, port))
        {
            Serial.print("Sending ATM Group Message to Domoticz #");
            Serial.print(Sensor_Index);

            client.print("GET /json.htm?type=command&param=udevice&idx=");
            client.print(Sensor_Index);

            // // Potential values to select from and batch post to a single sensor
            // client.print("&svalue=");
            // client.print(String(LineVoltage));
            // client.print(";");
            // client.print(String(LineCurrent));
            // client.print(";0;");
            // client.print(String(LineFrequency));
            // client.print(";0;");
            // client.print(String(ActivePower));
            // client.print(";0;");
            // client.print(String(ImportEnergy));
            // client.print(";0;");
            // client.print(String(ExportEnergy));
            // client.print(";0;");
            // client.print(String(PowerFactor));
            // client.print(";0;");
            // client.print(String(DCVoltage));
            // client.print(";0;");
            // client.print(String(PCBTemperature));
            // client.print(";0");

            client.println(" HTTP/1.1");
            client.print("Host: ");
            client.print(domoticz_server);
            client.print(":");

            client.println(port);
            client.println("User-Agent: Arduino-ethernet");
            client.println("Connection: close");
            client.println();

            client.stop();
        }
        else
        {
            Serial.println("Not Connected");
            InitialiseWiFi();
        }
    }
} // PublishDomoticzATM
