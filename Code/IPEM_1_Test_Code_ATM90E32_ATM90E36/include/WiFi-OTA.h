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

   WebServer URL Commands.  An example is http://ipaddress/pwm-on  e.g. http:192.168.0.5/pwm-on
   * /pwm-local - This is Enable PWM to take use Local Power reading from specified variable and output.
   * /pwm-remote - This is Enable PWM to take use Remote Power reading from specified variable and output.
   * /pwm-on - This is Enable PWM to take use Local Power reading from specified variable and output.
   * /pwm-off - This is Turn Off PWM Output
   * /pwm-test - This will turn on a test PWM Loop and output
   * /dac-local - This is Enable DAC to take use Local Power reading from specified variable and output.
   * /dac-remote - This is Enable DAC to take use Remote Power reading from specified variable and output.
   * /dac-on - This is Enable DAC to take use Local Power reading from specified  variable and output.
   * /dac-off - This is Turn Off DAC Output
   * /dac-test - This will turn on a test DAC Loop and output

   To setup the Push OTA and Reflash The Board:
    * Once flashed, Open Web Page using IP Address in Web Browser (PC, Tablet, Phone).
    * Upload a BIN file. Compiled from PlatformIO. In Folder IPEM_1_Test_Code_ATM90E32_ATM90E36\.pio\build\wemos_d1_mini32\
*/

// Libraries
#include <WiFi.h>
#include <WiFiMulti.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <AsyncTCP.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ElegantOTA.h>
#include <ATM90E3x.h>

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES ****************

// WiFi.  Setup with your Wireless Information.  Add more as needed.
const char *ssid1 = "";     // WiFi Network SSID - Case Sensitive
const char *password1 = ""; // WiFi Network password - Case Sensitive
const char *ssid2 = "";     // WiFi Network SSID - Case Sensitive
const char *password2 = ""; // WiFi Network password - Case Sensitive
const char *ssid3 = "";     // WiFi Network SSID - Case Sensitive
const char *password3 = ""; // WiFi Network password - Case Sensitive

// WiFi. Force Disable for Testing.  !!!!! BEWARE Outside of Local Developmet Do NOT Disable as OTA will NOT work !!!!!
const boolean DisableWiFi = false; // Force Disable WiFi for Local USB Development and Testing Only.  You can leave SSID/IP etc populated.

// Set your Static IP address and Gateway - Alternatively leave at (0, 0, 0, 0)
IPAddress local_IP(0, 0, 0, 0);     // Leave at (0, 0, 0, 0) if DHCP required
IPAddress gateway(0, 0, 0, 0);      // Set to your Router IP = Gateway
IPAddress subnet(255, 255, 255, 0); // Should not need to change subnet from default (255, 255, 255, 0)

// Optional DNS Defaults.
IPAddress primaryDNS(0, 0, 0, 0);   // Defaults to your above Gateway IP if left as (0, 0, 0, 0)
IPAddress secondaryDNS(8, 8, 4, 4); // For Google Public DNS use for Primary or Secondary (8,8,8,8) and/or (8,8,4,4)

// WiFi Other
WiFiClient wlan_client;                   // Initialize the Client Library  / Client Instance
WiFiMulti wifiMulti;                      // Multiple WiFi Options - Auto Scanning
String HostNameHeader = AppAcronym + "-"; // Hostname Prefix
String HostName;                          // Hostname
// String RSSILevel;                         // WiFi RSSI Level Information
const uint32_t connectTimeoutMs = 10000; // WiFi connect timeout per AP. Increase when connecting takes longer.

// NTP Time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);
// United Kingdom (London, Belfast)
// TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, 60};        // British Summer Time
// TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 2, 0};         // Standard Time
// Timezone UK(BST, GMT);

// Web Server.
WebServer server(80);
String page_header, page_css, page_title, page_board, page_wifi, page_detail, page_updater, page_footer;

// ######### FUNCTIONS #########

// Return Some Meaningful Information From RSSI
String RSSI_Info(int RSSI_Value)
{
    switch (-RSSI_Value) // Inverted dBm Level ;)
    {
        {
        case 0 ... 30:
            return "Signal Very Strong";
            break;
        case 31 ... 50:
            return "Signal Excellent";
            break;
        case 51 ... 60:
            return "Signal Healthy";
            break;
        case 61 ... 70:
            return "Signal Very Good";
            break;
        case 71 ... 80:
            return "Signal Good";
            break;
        case 81 ... 90:
            return "Signal Poor - Try Moving Position";
            break;
        case 91 ... 100:
            return "Signal Very Low! - Move Position";
            break;
        default:
            return "No Signal :(";
            break;
        }
    }

} // RSSI_Info

// Wifi Information
void DisplayWiFiConfiguration()
{

    // RSSIInformation();

    PrintUnderline("Connection Details:");
    Serial.println("WiFi SSID \t " + String(WiFi.SSID()) + " (Wifi Station Mode)");

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
    Serial.println("WiFi RSSI \t " + String(WiFi.RSSI()) + " dBm (" + RSSI_Info(WiFi.RSSI()) + ")");

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
    }
} // InitialiseStaticIP

// Initialise WiFi
void InitialiseWiFi()
{

    // Connect or reconnect to WiFi
    if (WiFi.status() != WL_CONNECTED && strlen(ssid1))
    {

        // Green LED
        digitalWrite(LED_Green, LOW);

        // App and Starting...
        oled.clear();
        OLEDPrint(AppAcronym, 2, 0);
        OLEDPrint("Cnx Wi-Fi ", 2, 2);
        oled.update();

        // Wifi Settings
        WiFi.mode(WIFI_STA);
        WiFi.setAutoReconnect(true);
        WiFi.persistent(true);

        // Multi-WiFi Connections.  Add more as needed.
        if (strlen(ssid1))
            wifiMulti.addAP(ssid1, password1);
        if (strlen(ssid2))
            wifiMulti.addAP(ssid2, password2);
        if (strlen(ssid3))
            wifiMulti.addAP(ssid3, password3);

        Serial.println("Scanning WiFi Networks... Please wait...");
        int n = WiFi.scanNetworks();

        if (n == 0)
        {
            Serial.println("No Wi-FI Networks Found");
        }
        else
        {
            Serial.print(n);
            Serial.println(" x Wi-Fi Networks Found");
            for (int i = 0; i < n; ++i)
            {
                // Print SSID and RSSI for Each Network Found
                Serial.print(" ");
                Serial.print(i + 1);
                Serial.print(": ");
                Serial.printf("%25s", WiFi.SSID(i).c_str());
                Serial.print(" (");
                Serial.print(WiFi.RSSI(i));
                Serial.print(" dBm " + RSSI_Info(WiFi.RSSI(i)) + ")");
                Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
                delay(10);
            }
        }

        // Connect to Wi-Fi using wifiMulti (connects to the SSID with strongest connection)
        Serial.println("\nConnecting WiFi...");

        while (wifiMulti.run() != WL_CONNECTED)
        { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
            delay(500);
            Serial.print('.');
        }

        // Green LED
        digitalWrite(LED_Green, HIGH);

        // if (wifiMulti.run() == WL_CONNECTED)
        {
            // // Stabalise for Slow Access Points
            // while (WiFi.status() != WL_CONNECTED)
            // {
            //     delay(500);
            //     Serial.print(".");
            // }

            Serial.println("Connected to " + String(WiFi.SSID()) + "\n");

            delay(500);

            // Force Local IP and Make Static
            InitialiseStaticIP();

            ForceHostName();

            // Wifi Information
            DisplayWiFiConfiguration();

            // Time Server
            timeClient.begin();
            // }
        }
    }
} // InitialiseWiFi

void WebServerPageContent(void)
{
    // This section is made up of HTML/CSS Strings, which the ESP32 Web Server will hopefully deliver the page content.
    page_header = "<!DOCTYPE HTML><html><head><title>" + AppAcronym + " v" + AppVersion + " ESP32 Home</title><meta name='viewport' content='width=device-width, initial-scale=1'></head>";
    page_css = "<style>h1 {text-align: center;}p {text-align: center;}div {text-align: center;}body {font-family: Arial, Helvetica, sans-serif;}</style>";
    page_title = "<body><h1>Welcome to the " + AppAcronym + " - Local Web Portal</h1><p><strong>" + AppName + "</strong><br>(Firmware Version " + AppVersion + ")</p>";
    page_board = "<br><p>Board Location:<strong> " + LocationName + "</strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Device:<strong> ATM90E" + ATM90DEVICE + "</strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;MAC Address: <strong>" + String(WiFi.macAddress()) + "</strong></p>";
    page_wifi = "<p>Board Hostname:<strong> " + HostName + "</strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Wi-Fi SSID:<strong> " + String(WiFi.SSID()) + "</strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Wi-Fi RSSI:<strong> " + String(WiFi.RSSI()) + " dBm (" + RSSI_Info(WiFi.RSSI()) + ")" + "</strong></p><br>";
    page_detail = "";
    page_updater = "<br><p><strong>To Update the " + AppAcronym + " Firmware via Push OTA - <a href='http://localhost/update' target='_blank'>Click Here</a></strong><br></p><p><small>(Load compiled <i>firmware.bin</i> file - located in<strong> 'IPEM_1_Test_Code_ATM90E32_ATM90E36 > .pio > build > wemos_d1_mini32'</small></strong>)</p><br>";
    page_footer = "<br><hr><p><small>Dave Williams&nbsp;&nbsp; | &nbsp;&nbsp;DitroniX&nbsp; | &nbsp;<a href='http://ditronix.net' target='_blank'>DitroniX.net</a>&nbsp; | &nbsp;<a href='https://github.com/DitroniX' target='_blank'>github.com/DitroniX</a>&nbsp; | &nbsp;<a href='https://www.hackster.io/ditronix' target='_blank'>hackster.io/DitroniX</a></small></p><p><a href='https://www.buymeacoffee.com/DitroniX' target='_blank'>Buy Me A Coffee</a></P></body></html>";
} // WebServerPageContent

// WebServer Pages 200 (and 400).
void WebServerRoot()
{
    page_updater.replace("localhost", WiFi.localIP().toString().c_str());
    server.send(200, "text/html", page_header + page_css + page_title + page_board + page_wifi + page_detail + page_updater + page_footer);
} // WebServerRoot

// Initialise WebServer
void InitialiseWebServer()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Initialise WebServer");

        WebServerPageContent();

        // Web Server Listener
        server.on("/pwm-local", []() // Use Local CT Clamp to Derive PWM
                  { 
            Serial.println("Receiving Message: PWM Turn On\n"); 
            PWM_Write(0); // Turn Output Off
            EnablePWMLocal = true;
            EnablePWMRemote = false;
            EnablePWMTestOutput = false;   
            delay(100); WebServerRoot(); });

        server.on("/pwm-on", []() // Use Local CT Clamp to Derive PWM
                  { 
            Serial.println("Receiving Message: PWM Turn On\n"); 
            PWM_Write(0); // Turn Output Off
            EnablePWMLocal = true;
            EnablePWMRemote = false;
            EnablePWMTestOutput = false;   
            delay(100); WebServerRoot(); });

        server.on("/pwm-remote", []() // Use Remote Value to Derive PWM
                  { 
            Serial.println("Receiving Message: PWM Turn On Remote Power\n"); 
            PWM_Write(0); // Turn Output Off
            EnablePWMLocal = false;
            EnablePWMRemote = true;
            EnablePWMTestOutput = false;   
            delay(100); WebServerRoot(); });

        server.on("/pwm-off", []() // Zero and then Disable PWM Output
                  { 
            Serial.println("Receiving Message: PWM Turn Off\n"); 
            PWM_Write(0); // Turn Output Off
            EnablePWMLocal = false;
            EnablePWMRemote = false;
            EnablePWMTestOutput = false;   
            delay(100); WebServerRoot(); });

        server.on("/pwm-test", []() // Use to Enable PWM Test Output
                  { 
            Serial.println("Receiving Message: PWM Turn Off\n"); 
            PWM_Write(0); // Turn Output Off
            EnablePWMLocal = false;
            EnablePWMRemote = false;
            EnablePWMTestOutput = true;            
            delay(100); WebServerRoot(); });

        server.on("/dac-local", []() // Use Local CT Clamp to Derive DAC Output Voltage
                  { 
            Serial.println("Receiving Message: DAC Turn On\n"); 
            DAC_WriteVoltage(0); // Turn Output Off
            EnableDACLocal = true;
            EnableDACRemote = false;
            EnableDACTestOutput = false;
            delay(100); WebServerRoot(); });

        server.on("/dac-on", []() // Use Local CT Clamp to Derive DAC Output Voltage
                  { 
            Serial.println("Receiving Message: DAC Turn On\n"); 
            DAC_WriteVoltage(0); // Turn Output Off
            EnableDACLocal = true;
            EnableDACRemote = false;
            EnableDACTestOutput = false;
            delay(100); WebServerRoot(); });

        server.on("/dac-remote", []() // Use Remote Value to Derive DAC Output Voltage
                  { 
            Serial.println("Receiving Message: DAC Turn On Remote Power\n"); 
            DAC_WriteVoltage(0); // Turn Output Off
            EnableDACLocal = false;
            EnableDACRemote = true;
            EnableDACTestOutput = false;
            delay(100); WebServerRoot(); });

        server.on("/dac-test", []() // Use to Enable DAC Test Output
                  { 
            Serial.println("Receiving Message: DAC Turn On Remote Power\n"); 
            DAC_WriteVoltage(0); // Turn Output Off
            EnableDACLocal = false;
            EnableDACRemote = false;
            EnableDACTestOutput = true;
            delay(100); WebServerRoot(); });

        server.on("/dac-off", []() // Zero and then Disable DAC Output Voltage
                  { 
            Serial.println("Receiving Message: DAC Turn Off\n"); 
            DAC_WriteVoltage(0);  // Turn Output Off
            EnableDACLocal = false;
            EnableDACRemote = false;
            EnableDACTestOutput = false;
            delay(100); WebServerRoot(); });

        // Display Web Page Upon Client Request
        server.on("", WebServerRoot);

        // Display Web Page Upon Invalid URL Client Request.  Faster than redirecting a 404 error.
        server.onNotFound(WebServerRoot);

        // Ignore SerialMonitor _handleRequest().  Future Options.

        ElegantOTA.begin(&server); // Start ElegantOTA
        server.begin();
        Serial.println("HTTP Web Server Started");
        Serial.println("Webpage can be found http://" + WiFi.localIP().toString() + "\n");
    }
} // InitialiseWebServer

// Display Web Server Page for Information and OTA
void CheckOTA()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        WebServerPageContent(); // Refresh Content.  NB.Temporary Solution.
        server.handleClient();
    }

} // CheckOTA
