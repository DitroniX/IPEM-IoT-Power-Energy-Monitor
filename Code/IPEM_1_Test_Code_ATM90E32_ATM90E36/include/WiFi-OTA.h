/*
  Dave Williams, DitroniX 2019-2023 (ditronix.net)
  IPEM-1 ESP32 ATM90E32 ATM90E36 IoT Power Energy Monitoring Energy Monitor v1.0

  Full header information in main.cpp.

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and also github.com/DitroniX

  Note: Adding an SSID will Automatically ENABLE WiFi attempt to Log-In.

   Preamble.  This WiFi-OTA will:
    * Configure WiFi Parameters (e.g. ssid etc.)
    * Configure Optional Static IP Address (leave 0,0,0,0 for DHCP settings)
    * Connect to WiFi
    * Create a Network Port in Serial Devices
    * Display WiFI detail on OLED
    * Display Web Information Page (Put IP in Browser)
    * Display Push OTA Page (Remote Uploading of BIN Files)

   To setup the Push OTA and Reflash The Board:
    * Once flashed, Open Web Page using IP Address in Web Browser (PC, Tablet, Phone).
    * Upload a BIN file. Compiled from PlatformIO. In Folder IPEM_1_Test_Code_ATM90E32_ATM90E36\.pio\build\wemos_d1_mini32\
*/

// Libraries
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <AsyncTCP.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ElegantOTA.h>

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES ****************

// WiFi.  Setup with your Wireless Information.
const char *ssid = "";     // WiFi Network SSID - Case Sensitive
const char *password = ""; // WiFi Network password - Case Sensitive

// WiFi Other
WiFiClient client;               // Initialize the client library
String HostNameHeader = "IPEM-"; // Hostname Prefix
String HostName;                 // Hostname
String RSSILevel;                // WiFi RSSI Level Information

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

// Web Server.
WebServer server(80);
String page_header, page_css, page_title, page_wifi, page_updater, page_footer;

// ######### FUNCTIONS #########

// Return some meaningful info from RSSI
void RSSIInformation()
{

    RSSILevel = "";

    switch (-WiFi.RSSI()) // Inverted dBm Level ;)
    {
    case 0 ... 50:
        RSSILevel = "Signal Excellent";
        break;
    case 51 ... 60:
        RSSILevel = "Signal Healthy";
        break;
    case 61 ... 70:
        RSSILevel = "Signal Very Good";
        break;
    case 71 ... 80:
        RSSILevel = "Signal Good";
        break;
    case 81 ... 90:
        RSSILevel = "Signal Low - Try Moving Position";
        break;
    case 91 ... 100:
        RSSILevel = "Signal Very Low! - Move Position";
        break;
    default:
        RSSILevel = "No Signal :(";
        break;
    }
} // RSSIInformation

// Wifi Information
void DisplayWiFiConfiguration()
{

    RSSIInformation();

    PrintUnderline("Connection Details:");
    Serial.println("WiFi SSID \t " + String(ssid) + " (Wifi Station Mode)");

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
    Serial.println("WiFi RSSI \t " + String(WiFi.RSSI()) + " dBm (" + RSSILevel + ")");

    Serial.println("");
} // DisplayWiFiConfiguration

// Force Hostname
void ForceHostName(void)
{
    HostName = HostNameHeader;
    HostName.concat(WiFi.macAddress().substring(WiFi.macAddress().length() - 5, WiFi.macAddress().length()));
    HostName.replace(":", "");
    WiFi.setHostname(HostName.c_str());
    HostName = WiFi.getHostname();
} // ForceHostName

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

        ForceHostName();

        // Wifi Initialisation
        WiFi.begin(ssid, password);

        // Stabalise for Slow Access Points
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }

        Serial.println("\n\nConnected to " + String(ssid) + "\n");

        // Wifi Settings
        WiFi.mode(WIFI_STA);
        WiFi.setAutoReconnect(true);
        WiFi.persistent(true);

        // Wifi Information
        DisplayWiFiConfiguration();

        // Time Server
        timeClient.begin();
    }
} // InitialiseWiFi

void WebServerPageContent(void)
{
    // This section is made up of HTML/CSS Strings, which the ESP32 Web Server will hopefully deliver the page content.
    page_header = "<!DOCTYPE HTML><html><head><title>IPEM v" + AppVersion + " ESP32 Home</title><meta name='viewport' content='width=device-width, initial-scale=1'></head>";
    page_css = "<style>h1 {text-align: center;}p {text-align: center;}div {text-align: center;}body {font-family: Arial, Helvetica, sans-serif;}</style>";
    page_title = "<body><h1>Welcome to the IPEM - Local Web Portal</h1><p><strong>" + AppName + "</strong><br>(Firmware Version " + AppVersion + ")</p>";
    page_wifi = "<br><p>Board Hostname:<strong> " + HostName + "</strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Wi-Fi SSID:<strong> " + ssid + "</strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Wi-Fi RSSI:<strong> " + String(WiFi.RSSI()) + " dBm (" + RSSILevel + ")" + "</strong></p><br>";
    page_updater = "<br><p><strong>To Update the IPEM Firmware via Push OTA - <a href='http://localhost/update' target='_blank'>Click Here</a></strong><br></p>";
    page_footer = "<br><hr><p><small>Dave Williams&nbsp;&nbsp;| &nbsp;&nbsp;DitroniX&nbsp;| &nbsp;<a href='http://ditronix.net' target='_blank'>DitroniX.net</a>&nbsp;| &nbsp;<a href='https://github.com/DitroniX' target='_blank'>github.com/DitroniX</a>&nbsp;| &nbsp;<a href='https://www.hackster.io/ditronix' target='_blank'>hackster.io/DitroniX</a></small></p><p><a href='https://www.buymeacoffee.com/DitroniX' target='_blank'>Buy Me A Coffee</a></P></body></html>";
} // WebServerPageContent

// WebServer Pages 200 (and 400).
void WebServerRoot()
{
    page_updater.replace("localhost", WiFi.localIP().toString().c_str());
    server.send(200, "text/html", page_header + page_css + page_title + page_wifi + page_updater + page_footer);
} // WebServerRoot

// Initialise WebServer
void InitialiseWebServer()
{
    Serial.println("Initialise WebServer");

    WebServerPageContent();

    // Display Web Page Upon Client Request
    server.on("", WebServerRoot);

    // Display Web Page Upon Invalid URL Client Request.  Faster than redirecting a 404 error.
    server.onNotFound(WebServerRoot);

    // Ignore SerialMonitor _handleRequest().  Future Options.

    ElegantOTA.begin(&server); // Start ElegantOTA
    server.begin();
    Serial.println("HTTP server started");
} // InitialiseWebServer

// Display Web Server Page for OTA
void CheckOTA()
{
    server.handleClient();
} // CheckOTA