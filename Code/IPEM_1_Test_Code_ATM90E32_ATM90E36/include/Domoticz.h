/*
  Dave Williams, DitroniX 2019-2023 (ditronix.net)
  IPEM-1 ESP32 ATM90E32 ATM90E36 IoT Power Energy Monitoring Energy Monitor v1.0

  Full header information in main.cpp.

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and also github.com/DitroniX
*/

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES ****************

// Domoticz Server info.  Setup with your Domoticz IP and Port
const char *DomoticzServer = "0.0.0.0"; // Domoticz Server IP Address (Typically a Fixed Local Address)
int DomoticzPort = 8080;                // Domoticz Network Port (Default 8080)
const char *Domoticz_User = "";         // Domoticz User - if applicable  (Not currently implimented in the connection)
const char *Domoticz_Password = "";     // Domoticz Password - if applicable  (Not currently implimented in the connection)

// Enable Publishing
boolean EnableDomoticz = true; // Change to true, to enable Loop reading and sending data to Domoticz.  Default false.

// Domoticz Hardware Device Indexes

// Set these values to the Domoticz Devices Indexes (IDX).  If Zero, then entry is ignored. A device first needs to be created in Domoticz, then use related IDX here.
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

int idxActivePowerCT1 = 0;             // ActivePowerCT1 - Pmean - Line Mean Active Power
int idxActivePowerCT2 = 0;             // ActivePowerCT2 - Pmean - Line Mean Active Power
int idxActivePowerCT3 = 0;             // ActivePowerCT3 - Pmean - Line Mean Active Power
int idxTotalActivePower = 0;           // TotalActivePower - Pmean - Line Mean Active Power
int idxCalculatedTotalActivePower = 0; // CalculatedTotalActivePower - Pmean - Line Mean Active Power

int idxActivePowerImportCT1 = 0;   // ActivePowerCT1 - Pmean - Line Mean Active Power Import
int idxActivePowerImportCT2 = 0;   // ActivePowerCT2 - Pmean - Line Mean Active Power Import
int idxActivePowerImportCT3 = 0;   // ActivePowerCT3 - Pmean - Line Mean Active Power Import
int idxTotalActivePowerImport = 0; // TotalActivePower - Pmean - Line Mean Active Power Import

int idxActivePowerExportCT1 = 0;   // ActivePowerCT1 - Pmean - Line Mean Active Power Export
int idxActivePowerExportCT2 = 0;   // ActivePowerCT2 - Pmean - Line Mean Active Power Export
int idxActivePowerExportCT3 = 0;   // ActivePowerCT3 - Pmean - Line Mean Active Power Export
int idxTotalActivePowerExport = 0; // TotalActivePower - Pmean - Line Mean Active Power Export

int idxReactivePowerCT1 = 0;             // ReactivePowerCT1 - Pmean - Line Mean Active Power
int idxReactivePowerCT2 = 0;             // ReactivePowerCT2 - Pmean - Line Mean Active Power
int idxReactivePowerCT3 = 0;             // ReactivePowerCT3 - Pmean - Line Mean Active Power
int idxTotalReactivePower = 0;           // TotalReactivePower - Pmean - Line Mean Active Power
int idxCalculatedTotalReactivePower = 0; // CalculatedTotalReactivePower - Pmean - Line Mean Active Power

int idxApparentPowerCT1 = 0;             // ApparentPowerCT1 - Pmean - Line Mean Apparent Power
int idxApparentPowerCT2 = 0;             // ApparentPowerCT2 - Pmean - Line Mean Apparent Power
int idxApparentPowerCT3 = 0;             // ApparentPowerCT3 - Pmean - Line Mean Apparent Power
int idxTotalApparentPower = 0;           // TotalApparentPower - Pmean - Line Mean Apparent Power
int idxCalculatedTotalApparentPower = 0; // CalculatedTotalApparentPower - Pmean - Line Mean Apparent Power

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

int idxEnablePWMLocal = 120;     // PWM Output Local (Turn Switch On/Off)
int idxEnablePWMRemote = 0;    // PWM Output Remote (Turn Switch On/Off)
int idxPWMPowerOutput = 0;     // PWM Last Factored Power
int idxPWMPowerPercentage = 0; // PWM Percentage

int idxEnableDACLocal = 124;     // DAC Output Local (Turn Switch On/Off)
int idxEnableDACRemote = 0;    // DAC Output Remote (Turn Switch On/Off)
int idxDACPowerOutput = 0;     // DAC Last Factored Power
int idxDACPowerPercentage = 0; // DAC Percentage

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

// Publish to Domoticz - Single String Values
void PublishDomoticzString(int Sensor_Index, float Sensor_Value, String Sensor_Name = "")
{

    if (Sensor_Index > 0)
    {
        if (wlan_client.connect(DomoticzServer, DomoticzPort))
        {
            // Green LED
            digitalWrite(LED_Green, LOW);

            Serial.print("Sending Message to Domoticz #");
            Serial.print(Sensor_Index);
            Serial.print(" ");
            Serial.print(Sensor_Value);
            Serial.print(" \t");
            Serial.println(Sensor_Name);

            wlan_client.print("GET /json.htm?type=command&param=udevice&idx=");
            wlan_client.print(Sensor_Index);

            wlan_client.print("&svalue=");
            wlan_client.print(Sensor_Value);

            wlan_client.println(" HTTP/1.1");
            wlan_client.print("Host: ");
            wlan_client.print(DomoticzServer);
            wlan_client.print(":");

            wlan_client.println(DomoticzPort);
            wlan_client.println("User-Agent: Arduino-ethernet");
            wlan_client.println("Connection: close");
            wlan_client.println();

            wlan_client.stop();

            // Green LED
            digitalWrite(LED_Green, HIGH);
        }
        else
        {
            // Red LED
            digitalWrite(LED_Red, LOW);

            Serial.println("WiFi or Domoticz Server Not Connected");

            // Update OLED
            oled.clear();
            OLEDPrint("Error", 2, 0);
            OLEDPrint("Domoticz", 2, 2);
            oled.update();
            delay(1000);

            // Red LED
            digitalWrite(LED_Red, HIGH);

            // Stabalise for slow Access Points
            InitialiseWiFi();
        }
    }
} // PublishDomoticz

// Publish to Domoticz - Single Numeric Values
void PublishDomoticzNumeric(int Sensor_Index, float Sensor_Value, String Sensor_Name = "")
{

    if (Sensor_Index > 0)
    {
        if (wlan_client.connect(DomoticzServer, DomoticzPort))
        {
            // Green LED
            digitalWrite(LED_Green, LOW);

            Serial.print("Sending Message to Domoticz #");
            Serial.print(Sensor_Index);
            Serial.print(" ");
            Serial.print(Sensor_Value);
            Serial.print(" \t");
            Serial.println(Sensor_Name);

            wlan_client.print("GET /json.htm?type=command&param=udevice&idx=");
            wlan_client.print(Sensor_Index);

            wlan_client.print("&nvalue=");
            wlan_client.print(Sensor_Value);

            wlan_client.println(" HTTP/1.1");
            wlan_client.print("Host: ");
            wlan_client.print(DomoticzServer);
            wlan_client.print(":");

            wlan_client.println(DomoticzPort);
            wlan_client.println("User-Agent: Arduino-ethernet");
            wlan_client.println("Connection: close");
            wlan_client.println();

            wlan_client.stop();

            // Green LED
            digitalWrite(LED_Green, HIGH);
        }
        else
        {
            // Red LED
            digitalWrite(LED_Red, LOW);

            Serial.println("WiFi or Domoticz Server Not Connected");

            // Update OLED
            oled.clear();
            OLEDPrint("Error", 2, 0);
            OLEDPrint("Domoticz", 2, 2);
            oled.update();
            delay(1000);

            // Red LED
            digitalWrite(LED_Red, HIGH);

            // Stabalise for slow Access Points
            InitialiseWiFi();
        }
    }
} // PublishDomoticzNumeric

// Publish to Domoticz - Listed Values
void PublishDomoticzValues()
{

    if (wlan_client.connect(DomoticzServer, DomoticzPort))
    {

        // Voltage LineVoltage1, LineVoltage2, LineVoltage3, LineVoltageTotal, LineVoltageAverage
        if (idxLineVoltage1 > 0)
        {
            PublishDomoticzString(idxLineVoltage1, LineVoltage1, "LineVoltage1");
        }

        if (idxLineVoltage2 > 0)
        {
            PublishDomoticzString(idxLineVoltage2, LineVoltage2, "LineVoltage2");
        }

        if (idxLineVoltage3 > 0)
        {
            PublishDomoticzString(idxLineVoltage3, LineVoltage3, "LineVoltage3");
        }

        if (idxLineVoltageTotal > 0)
        {
            PublishDomoticzString(idxLineVoltageTotal, LineVoltageTotal, "LineVoltageTotal");
        }

        if (idxLineVoltageAverage > 0)
        {
            PublishDomoticzString(idxLineVoltageAverage, LineVoltageAverage, "LineVoltageAverage");
        }

        // Current LineCurrentCT1, LineCurrentCT2, LineCurrentCT3, LineCurrentCT4, LineCurrentCTN, LineCurrentTotal
        if (idxLineCurrentCT1 > 0)
        {
            PublishDomoticzString(idxLineCurrentCT1, LineCurrentCT1, "LineCurrentCT1");
        }

        if (idxLineCurrentCT2 > 0)
        {
            PublishDomoticzString(idxLineCurrentCT2, LineCurrentCT2, "LineCurrentCT2");
        }

        if (idxLineCurrentCT3 > 0)
        {
            PublishDomoticzString(idxLineCurrentCT3, LineCurrentCT3, "LineCurrentCT3");
        }
        if (idxLineCurrentCT4 > 0)
        {
            PublishDomoticzString(idxLineCurrentCT4, LineCurrentCT4, "LineCurrentCT4");
        }
        if (idxLineCurrentCTN > 0)
        {
            PublishDomoticzString(idxLineCurrentCTN, LineCurrentCTN, "LineCurrentCTN");
        }
        if (idxLineCurrentTotal > 0)
        {
            PublishDomoticzString(idxLineCurrentTotal, LineCurrentTotal, "LineCurrentTotal");
        }

        // Calculated Power CalculatedPowerCT1, CalculatedPowerCT2, CalculatedPowerCT3, CalculatedPowerCT4, CalculatedTotalPower
        if (idxCalculatedPowerCT1 > 0)
        {
            PublishDomoticzString(idxCalculatedPowerCT1, CalculatedPowerCT1, "CalculatedPowerCT1");
        }
        if (idxCalculatedPowerCT2 > 0)
        {
            PublishDomoticzString(idxCalculatedPowerCT2, CalculatedPowerCT2, "CalculatedPowerCT2");
        }
        if (idxCalculatedPowerCT3 > 0)
        {
            PublishDomoticzString(idxCalculatedPowerCT3, CalculatedPowerCT3, "CalculatedPowerCT3");
        }
        if (idxCalculatedPowerCT4 > 0)
        {
            PublishDomoticzString(idxCalculatedPowerCT4, CalculatedPowerCT4, "CalculatedPowerCT4");
        }
        if (idxCalculatedTotalPower > 0)
        {
            PublishDomoticzString(idxCalculatedTotalPower, CalculatedTotalPower, "CalculatedTotalPower");
        }

        // Active Power ActivePowerCT1, ActivePowerCT2, ActivePowerCT3, TotalActivePower
        if (idxActivePowerCT1 > 0)
        {
            PublishDomoticzString(idxActivePowerCT1, ActivePowerCT1, "ActivePowerCT1");
        }
        if (idxActivePowerCT2 > 0)
        {
            PublishDomoticzString(idxActivePowerCT2, ActivePowerCT2, "ActivePowerCT2");
        }
        if (idxActivePowerCT3 > 0)
        {
            PublishDomoticzString(idxActivePowerCT3, ActivePowerCT3, "ActivePowerCT3");
        }
        if (idxTotalActivePower > 0)
        {
            PublishDomoticzString(idxTotalActivePower, TotalActivePower, "TotalActivePower");
        }
        if (idxCalculatedTotalActivePower > 0)
        {
            PublishDomoticzString(idxCalculatedTotalActivePower, CalculatedTotalActivePower, "CalculatedTotalActivePower");
        }

        // Active Power Import ActivePowerImportCT1, ActivePowerImportCT2, ActivePowerImportCT3, TotalActivePowerImport
        if (idxActivePowerImportCT1 > 0)
        {
            PublishDomoticzString(idxActivePowerImportCT1, ActivePowerImportCT1, "ActivePowerImportCT1");
        }
        if (idxActivePowerImportCT2 > 0)
        {
            PublishDomoticzString(idxActivePowerImportCT2, ActivePowerImportCT2, "ActivePowerImportCT2");
        }
        if (idxActivePowerImportCT3 > 0)
        {
            PublishDomoticzString(idxActivePowerImportCT3, ActivePowerImportCT3, "ActivePowerImportCT3");
        }
        if (idxTotalActivePowerImport > 0)
        {
            PublishDomoticzString(idxTotalActivePowerImport, TotalActivePowerImport, "TotalActivePowerImport");
        }

        // Active Power Export ActivePowerExportCT1, ActivePowerExportCT2, ActivePowerExportCT3, TotalActivePowerExport
        if (idxActivePowerExportCT1 > 0)
        {
            PublishDomoticzString(idxActivePowerExportCT1, ActivePowerExportCT1, "ActivePowerExportCT1");
        }
        if (idxActivePowerExportCT2 > 0)
        {
            PublishDomoticzString(idxActivePowerExportCT2, ActivePowerExportCT2, "ActivePowerExportCT2");
        }
        if (idxActivePowerExportCT3 > 0)
        {
            PublishDomoticzString(idxActivePowerExportCT3, ActivePowerExportCT3, "ActivePowerExportCT3");
        }
        if (idxTotalActivePowerExport > 0)
        {
            PublishDomoticzString(idxTotalActivePowerExport, TotalActivePowerExport, "TotalActivePowerExport");
        }

        // Reactive Power ReactivePowerCT1, ReactivePowerCT2, ReactivePowerCT3, TotalReactivePower
        if (idxReactivePowerCT1 > 0)
        {
            PublishDomoticzString(idxReactivePowerCT1, ReactivePowerCT1, "ReactivePowerCT1");
        }
        if (idxReactivePowerCT2 > 0)
        {
            PublishDomoticzString(idxReactivePowerCT2, ReactivePowerCT2, "ReactivePowerCT2");
        }
        if (idxReactivePowerCT3 > 0)
        {
            PublishDomoticzString(idxReactivePowerCT3, ReactivePowerCT3, "ReactivePowerCT3");
        }
        if (idxTotalReactivePower > 0)
        {
            PublishDomoticzString(idxTotalReactivePower, TotalReactivePower, "TotalReactivePower");
        }
        if (idxCalculatedTotalReactivePower > 0)
        {
            PublishDomoticzString(idxCalculatedTotalReactivePower, CalculatedTotalReactivePower, "CalculatedTotalReactivePower");
        }

        // Apparent Popwer ApparentPowerCT1, ApparentPowerCT2, ApparentPowerCT3, TotalApparentPower
        if (idxApparentPowerCT1 > 0)
        {
            PublishDomoticzString(idxApparentPowerCT1, ApparentPowerCT1, "ApparentPowerCT1");
        }
        if (idxApparentPowerCT2 > 0)
        {
            PublishDomoticzString(idxApparentPowerCT2, ApparentPowerCT2, "ApparentPowerCT2");
        }
        if (idxApparentPowerCT3 > 0)
        {
            PublishDomoticzString(idxApparentPowerCT3, ApparentPowerCT3, "ApparentPowerCT3");
        }
        if (idxTotalApparentPower > 0)
        {
            PublishDomoticzString(idxTotalApparentPower, TotalApparentPower, "TotalApparentPower");
        }
        if (idxCalculatedTotalApparentPower > 0)
        {
            PublishDomoticzString(idxCalculatedTotalApparentPower, CalculatedTotalApparentPower, "CalculatedTotalApparentPower");
        }

        // ActivePower TotalActiveFundPower, TotalActiveHarPower
        if (idxTotalActiveFundPower > 0)
        {
            PublishDomoticzString(idxTotalActiveFundPower, TotalActiveFundPower, "TotalActiveFundPower");
        }
        if (idxTotalActiveHarPower > 0)
        {
            PublishDomoticzString(idxTotalActiveHarPower, TotalActiveHarPower, "TotalActiveHarPower");
        }

        // Power Factor PowerFactorCT1, PowerFactorCT2, PowerFactorCT3, TotalPowerFactor
        if (idxPowerFactorCT1 > 0)
        {
            PublishDomoticzString(idxPowerFactorCT1, PowerFactorCT1, "PowerFactorCT1");
        }
        if (idxPowerFactorCT2 > 0)
        {
            PublishDomoticzString(idxPowerFactorCT2, PowerFactorCT2, "PowerFactorCT2");
        }
        if (idxPowerFactorCT3 > 0)
        {
            PublishDomoticzString(idxPowerFactorCT3, PowerFactorCT3, "PowerFactorCT3");
        }
        if (idxTotalPowerFactor > 0)
        {
            PublishDomoticzString(idxTotalPowerFactor, TotalPowerFactor, "TotalPowerFactor");
        }

        // Phase Angle PhaseAngleCT1, PhaseAngleCT2, PhaseAngleCT3
        if (idxPhaseAngleCT1 > 0)
        {
            PublishDomoticzString(idxPhaseAngleCT1, PhaseAngleCT1, "PhaseAngleCT1");
        }
        if (idxPhaseAngleCT2 > 0)
        {
            PublishDomoticzString(idxPhaseAngleCT2, PhaseAngleCT2, "PhaseAngleCT2");
        }
        if (idxPhaseAngleCT3 > 0)
        {
            PublishDomoticzString(idxPhaseAngleCT3, PhaseAngleCT3, "PhaseAngleCT3");
        }
        // Other ChipTemperature, LineFrequency, DCVoltage, PCBTemperature
        if (idxChipTemperature > 0)
        {
            PublishDomoticzString(idxChipTemperature, ChipTemperature, "ChipTemperature");
        }
        if (idxLineFrequency > 0)
        {
            PublishDomoticzString(idxLineFrequency, LineFrequency, "PhaLineFrequencyseAngleC");
        }
        if (idxDCVoltage > 0)
        {
            PublishDomoticzString(idxDCVoltage, DCVoltage, "DCVoltage");
        }
        if (idxPCBTemperature > 0)
        {
            PublishDomoticzString(idxPCBTemperature, PCBTemperature, "PCBTemperature");
        }

        // Switch States
        if (idxEnablePWMLocal > 0)
        {
            PublishDomoticzNumeric(idxEnablePWMLocal, EnablePWMLocal, "EnablePWMLocal");
        }
        if (idxEnableDACLocal > 0)
        {
            PublishDomoticzNumeric(idxEnableDACLocal, EnableDACLocal, "EnableDACLocal");
        }

        // PWM
        if (idxPWMPowerOutput > 0)
        {
            PublishDomoticzString(idxPWMPowerOutput, PWMPowerOutput, "PWMPowerOutput");
        }
        if (idxPWMPowerPercentage > 0)
        {
            PublishDomoticzString(idxPWMPowerPercentage, PWMPowerPercentage, "PWMPowerPercentage");
        }

        // DAC
        if (idxDACPowerOutput > 0)
        {
            PublishDomoticzString(idxDACPowerOutput, DACPowerOutput, "DACPowerOutput");
        }
        if (idxDACPowerPercentage > 0)
        {
            PublishDomoticzString(idxDACPowerPercentage, DACPowerPercentage, "DACPowerPercentage");
        }

        // NTP Time
        timeClient.update();
        Serial.println(timeClient.getFormattedTime() + "> Published to Domoticz\n");
    }
    else
    {
        // Red LED
        digitalWrite(LED_Red, LOW);

        Serial.println("WiFi or Domoticz Server Not Connected");

        // Update OLED
        oled.clear();
        OLEDPrint("Error", 2, 0);
        OLEDPrint("Domoticz", 2, 2);
        oled.update();
        delay(1000);

        // Red LED
        digitalWrite(LED_Red, HIGH);

        // Stabalise for slow Access Points
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
        if (wlan_client.connect(DomoticzServer, DomoticzPort))
        {
            // Green LED
            digitalWrite(LED_Green, LOW);

            Serial.print("Sending ATM Group Message to Domoticz #");
            Serial.print(Sensor_Index);

            wlan_client.print("GET /json.htm?type=command&param=udevice&idx=");
            wlan_client.print(Sensor_Index);

            // // Potential values to select from and batch post to a single sensor
            // wlan_client.print("&svalue=");
            // wlan_client.print(String(LineVoltage));
            // wlan_client.print(";");
            // wlan_client.print(String(LineCurrent));
            // wlan_client.print(";0;");
            // wlan_client.print(String(LineFrequency));
            // wlan_client.print(";0;");
            // wlan_client.print(String(ActivePower));
            // wlan_client.print(";0;");
            // wlan_client.print(String(ImportEnergy));
            // wlan_client.print(";0;");
            // wlan_client.print(String(ExportEnergy));
            // wlan_client.print(";0;");
            // wlan_client.print(String(PowerFactor));
            // wlan_client.print(";0;");
            // wlan_client.print(String(DCVoltage));
            // wlan_client.print(";0;");
            // wlan_client.print(String(PCBTemperature));
            // wlan_client.print(";0");

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

            wlan_client.println(" HTTP/1.1");
            wlan_client.print("Host: ");
            wlan_client.print(DomoticzServer);
            wlan_client.print(":");

            wlan_client.println(DomoticzPort);
            wlan_client.println("User-Agent: Arduino-ethernet");
            wlan_client.println("Connection: close");
            wlan_client.println();

            wlan_client.stop();

            // Green LED
            digitalWrite(LED_Green, HIGH);
        }
        else
        {
            // Red LED
            digitalWrite(LED_Red, LOW);

            Serial.println("WiFi or Domoticz Server Not Connected");

            // Update OLED
            oled.clear();
            OLEDPrint("Error", 2, 0);
            OLEDPrint("Domoticz", 2, 2);
            oled.update();
            delay(1000);

            // Red LED
            digitalWrite(LED_Red, HIGH);

            // Stabalise for slow Access Points
            InitialiseWiFi();
        }
    }
} // PublishDomoticzATM
