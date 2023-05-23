The **IPEM** Test and Development Firmware has evolved a great deal over a short period and this Wiki page will hopefully provide some insight to it's features and options.

Rather than use the Arduino IDE, the IPEM firmware has been developed using Visual Studio Code (VSCode) and PlatformIO.  This integrating increased flexibility, ease of use and also allows for the correct libraries to be automatically installed, so making for a better overall user experience.

Originally, the purpose of '[IPEM_1_Test_Code_ATM90E32_ATM90E36](https://github.com/DitroniX/IPEM-IoT-Power-Energy-Monitor/tree/main/Code/IPEM_1_Test_Code_ATM90E32_ATM90E36)' was as a piece of test code / firmware, which cycled through the various main functions of the board, as part of board bring up testing.

As time moved on, the single code base, expanded to include various functions, which applies to all variations of IPEM.  This makes it easier to maintain and configure, irrespective of board version.

You can use this firmware for both:
* Testing (modify as needed to suit your integration requirements)
* Final Application (just configure and flash)


**IPEM Board Overview**

The main purpose of the code, is to provide an array of functionality, such as:

* Initial IPEM Board Bring Up and Hardware Test 
* Integration of functions and parameters for both ATM90E32 and ATM90E36
* Configuration of CT Clamp Inputs
	- CT1, 2 and 3 to ATM90E32 or ATM90E32
	- CT4 to ATM90E36 or ESP32 ADC
	
* Mains voltage calculations, using either:
	- Single AC Input for all three main Power readings
	- Three Phase.  Separate AC Input for all three main Power readings
	* Split Phase. Dual AC Voltage  Input for Split Voltage US Power reading
	
* Fast Setup and Installation
	
* Comprehensive [List of Variables](https://github.com/DitroniX/IPEM-IoT-Power-Energy-Monitor/wiki/IPEM-Variables-and-Functions), which ultimately translate into available readings, which can be displayed or published to Home Automation.  Domoticz, MQTT etc..  
* Integrated Domoticz Publishing (this will be expanded)
* Web Server
	- Home Page (with basic information)
	- Push OTA (Remote flashing over HTTP)
	
* OLED Display Driver
	- Directly plug in an I2C SSD1306 Display
	* Runtime Monitoring Information
	* Diagnostics
	

**Hardware Configuration**

The default hardware and firmware settings configuration is based on:

* Current Clamps.  Default example settings are based on the [YDHC SCT-013-000 100A/50mA](https://ditronix.net/shop/yhdc-current-transformer-sct-013-000-100a-50ma/).

* AC Voltage.  Inputs from a [Greenbrook DAT01A (TC TR7) Transformer](https://www.greenbrook.co.uk/dat01a), set to 12V AC.

**Firmware Logic**

Various software logic and switches has been included within the code, this automatically enables, and disables, various functions depending on options set.  For the time being, please do not alter these, unless you understand the implications.  

*Please keep to the below 'Firmware Configuration' settings.*


**Calibration**

Based on the above hardware configuration, calibration should be absolutely minimal, to not required.  All boards are tested using this configuration, with a fixed AC voltage source and fixed resistive loads, to three CT Clamp.

An example Serial Monitor output, can be seen at [here](https://github.com/DitroniX/IPEM-IoT-Power-Energy-Monitor/wiki/IPEM-Serial-Monitor-Example).


**Quick Flashing and Initial Test**

From the 'box', the firmware have been thoroughly tested and should open, and compile, without any issues.  

*NB. All boards will have this firmware installed by default, as is is used for production testing too.*

1) Download the '[Source Code](https://github.com/DitroniX/IPEM-IoT-Power-Energy-Monitor/tree/main/Code/IPEM_1_Test_Code_ATM90E32_ATM90E36)'
2) Open the folder from PlatformIO
3) Connect an IPEM board to the USB Port
4) Click Upload.

PlatformIO should automatically install any libraries not already installed and then connect to the IPEM board.  Once compiled (Click Upload in Status Bar), it should flash the board.

Once flashed, Open the 'Serial Monitor' (Click Serial Monitor in Status Bar), and you should see the default output from the IPEM board.  Press RESET to refresh the Serial Monitor Info.

NB.  You can also press (for 1 second), the USER BUTTON to refresh the information displayed.

**Firmware Configuration**

Now you have proven the flashing and test concept, we can now expand to the options in the source files.

* **main.cpp** {src folder}	*For the most part, these can stay as defaults.*
	* **LoopDelay**  *(Loop Delay in Seconds.  Default 1)*
	* **EnableBasicLoop** *(Display looped readings in the Serial Monitor, or leave for displaying only once per reset cycl using default of false.)*
	* **EnableDisplayBoardConfiguration**  *(Display board software configuration Information if DisplayFull is true.)*
	* **EnableOLEDLoop** *(Enables OLED Display.  Over-ride via I2C Scan.)*
	
* **ATM90E3x.cpp** {src folder} *Nothing to update.*

* **IPEM_Hardware.h** {include folder}
	* **App USER**
		* Change 'LocationName', to something more descriptive, such as Home, Solar, Workshop etc.
	* **Constants USER**
		* These can be left as the defaults
	* **OLED Instance**
		* Uncomment your OLED Display.  Default is the 0.6"
		* If no OLED connected, can be left as the defaults 
	* **CT4 Energy Monitor Library Configuration**
		* These can be left as the defaults
		* Update at a later time, if using CT4 into ESP32
		
* **ATM90E3x.h** {include folder} *For the most part, these can stay as defaults.*
	* **ATM90E3x device**
		* These can be left as the defaults
		* Only need to update, if using an ATM90E36		
	* **DEFINE Current Clamp Input CT4**
		* These can be left as the defaults
		* Possibly, only may need to update if using an ATM90E36		
	* **CT4 Software Configuration**
		* These can be left as the defaults
		* Possibly, only need to update if using ESP32/CT4
	* **DEFINE VOLTAGE REFERENCE**
		* Can be left as the default true
		* Change to False, only if using separate AC voltage Inputs.	
	* **DEFINE PHASE TYPE**
		* Can be left as the default false
		* Change to true, only if using split phase AC Voltage. e.g. USA					

* **WiFi-OTA.h** {include folder} *Update with your Wi-Fi Settings*
	* **Wireless Information**
		* Enter your Router/AP SSID and Password
	* **Static IP**
		* If you are using DHCP, then leave as defaults
		* If wanting a Static IP, first find a suitable address and update as needed.
			- The board will automatically set this IP upload flashing.
	* **DNS**
		* For the most parts, leave as default.
* **Domoticz.h** {include folder} *Update with your Domoticz Server Settings*
	* **Server IP**
		- Domoticz Server IP Address (Typically a Fixed Local Address)
	* **EnableDomoticz**
		- Set to true, once you have setup the Device Indexes.
	* **Domoticz Hardware Device Indexes**
		- Simply change index('s) from 0 to a 'device index number', which you want to publish.
		- A Variable list can be found [here](https://github.com/DitroniX/IPEM-IoT-Power-Energy-Monitor/wiki/Domoticz-Variables)
						
**Web Server and Push OTA**			

The Web Server will assume the IP, either via DHCP or Static, which ever you setup.

To find the IP address, it will be displayed on both the Serial Monitor and OLED Display, upon pressing board Reset.			

You have TWO pages on the Web Server.

* 'Home Page' which displays basic info on the board configuration
* 'update' which will display the ElegantOTA page
	- Simply compile your code
	- Choose *firmware.bin* file
		+ Found in IPEM_1_Test_Code_ATM90E32_ATM90E36 - DW Domoticz - OTA\.pio\build\wemos_d1_mini32\
	- ElegantOTA will then upload to the IPEM and auto reboot.
		
