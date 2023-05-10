/*
  Dave Williams, DitroniX 2019-2023 (ditronix.net)
  IPEM-1 ESP32 ATM90E32 ATM90E36 IoT Power Energy Monitoring Energy Monitor v1.0

  Full header information in main.cpp.

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and also github.com/DitroniX

   Preamble.  This WiFi-OTA will:
    * Configure WiFi Parameters (e.g. ssid etc.)
    * Configure Optional Static IP Address (leave 0,0,0,0 for DHCP settings)
    * Connect to WiFi
    * Create a Network Port in Serial Devices

   To setup the serial devices network port:
        Once flashed, open platformio.ini and set the curent IP address or mDNS_NAME.local

    Should you wish to go back to USB Serial Flashing, simply comment out upload_port in platformio.ini
*/

// Libraries
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES ****************

// WiFi.  Setup with your Wireless Information
const char *ssid = "";           // WiFi Network SSID - Case Sensitive
const char *password = "";       // WiFi Network password - Case Sensitive

// WiFi Other
WiFiClient client;               // Initialize the client library
String HostNameHeader = "IPEM-"; // Hostname Prefix

// Set your Static IP address and Gateway - Alternatively leave at (0, 0, 0, 0)
IPAddress local_IP(0, 0, 0, 0);     // Leave at (0, 0, 0, 0) if DHCP required
IPAddress gateway(0, 0, 0, 0);      // Set to your Router IP = Gateway
IPAddress subnet(255, 255, 255, 0); // Should not need to change subnet from default (255, 255, 255, 0)

// Optional DNS Defaults.
IPAddress primaryDNS(0, 0, 0, 0);   // Defaults to your above Gateway IP if left as (0, 0, 0, 0)
IPAddress secondaryDNS(8, 8, 4, 4); // For Google Public DNS use for Primary or Secondary (8,8,8,8) and/or (8,8,4,4)

// NTP Time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// ######### FUNCTIONS #########

// OTA
void InitialiseOTA()
{
    if (WiFi.status() == WL_CONNECTED && strlen(ssid))
    {
        ArduinoOTA.begin();
        Serial.println("OTA Serial Devices IP Port Initialised");
    }
    else
    {
        Serial.println("WiFI/OTA Not Enabled");
    }
} // OTA

// Check OTA for Updates
void CheckOTA()
{
    ArduinoOTA.handle();
} // CheckOTA

// Wifi Information
void DisplayWiFiConfiguration()
{
    PrintUnderline("Connection Details:");
    Serial.println("WiFi SSID \t " + String(ssid) + "(Wifi Station Mode)");

    if (local_IP.toString() == WiFi.localIP().toString().c_str())
    {
        Serial.printf("WiFi IP \t %s (Static)\n", WiFi.localIP().toString().c_str());
    }
    else
    {
        Serial.printf("WiFi IP \t %s (DHCP)\n", WiFi.localIP().toString().c_str());
    }

    Serial.printf("WiFi MASK \t %s\n", WiFi.subnetMask().toString().c_str());
    Serial.printf("WiFi GW \t %s\n", WiFi.gatewayIP().toString().c_str());
    Serial.printf("WiFi DNS 1 \t %s\n", WiFi.dnsIP().toString().c_str());
    Serial.printf("WiFi DNS 2 \t %s\n", WiFi.dnsIP(1).toString().c_str());
    Serial.println("WiFi MAC \t " + WiFi.macAddress());
    Serial.printf("WiFi Hostname \t %s\n", WiFi.getHostname());
    Serial.println("WiFi RSSI \t " + String(WiFi.RSSI()));
    Serial.println("");
} // DisplayWiFiConfiguration

// Configure Static IP address (if not 0.0.0.0)
void InitialiseStaticIP()

{
    if (local_IP.toString() != "0.0.0.0")
    {
        // Serial.printf("WiFI Current IP\t \t %s\n", WiFi.localIP().toString().c_str());

        if (primaryDNS.toString() != "0.0.0.0")
        {
            if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
                ;
            {

                Serial.println("STA Static IP Failed to configure");
            }
        }
        else // Use Gateway IP for DNS 1
        {
            if (!WiFi.config(local_IP, gateway, subnet, gateway, secondaryDNS))
            {
                Serial.println("STA Static IP Failed to configure");
            }
        }

        // Stabalise for slow Access Points
        delay(100);

        // Serial.printf("WiFI Static IP\t \t %s\n", WiFi.localIP().toString().c_str());
    }
} // InitialiseStaticIP

// Initialise WiFi
void InitialiseWiFi()
{

    // Connect or reconnect to WiFi
    if (WiFi.status() != WL_CONNECTED && strlen(ssid))
    {
        Serial.println("Attempting to connect to " + String(ssid) + "\n");

        // Force Local IP and Make Static
        InitialiseStaticIP();

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
        DisplayWiFiConfiguration();

        // Time Server
        timeClient.begin();
    }
} // InitialiseWiFi
