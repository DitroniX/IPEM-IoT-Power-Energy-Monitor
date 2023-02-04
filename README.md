**IPEM ESP32 ATM90E32 ATM90E36 IoT Power Energy Monitor SDK**

------------

**This is a NEW board and is in Beta (Testing during February 2023).  Expected final production boards, early March 2023**

Four versions of this board are planned.  The board layout is designed to be a flexible dual purpose.
- ESP32 WROOM, with Internal Antenna, or UF.L connector for external antenna
- ATM90E32 or ATM90E36

------------

IPEM 'Beta Test PCB'
![Display-Type-B](https://ditronix.net/wp-content/uploads/2023/02/IPEM-ESP32-ATM90E3x-SDK-1.2302-101-PCB-Top-Copper-3D.png?raw=true)

IPEM 'Beta Test PCB'
![Display-Type-B](https://ditronix.net/wp-content/uploads/2023/02/IPEM-ESP32-ATM90E3x-SDK-1.2302-101-PCB-Bottom-Copper.png?raw=true)

 The **IPEM IoT Power Energy Monitor** board main features:
 - **Compact** and **Flexible Design** SDK Board
	 - All connections are **low voltage**, for **safe operation**
	 - **Easy to interface**
 - *Optional* **Espressif** **ESP32** **WROOM**
	 - **ESP32-WROOM-32E-N4** 
		 - Integral Antenna Wireless and Bluetooth  
	 - **ESP32-WROOM-32UE-N4** 
		 - Ext Antenna - U.FL Socket 
		 - Wireless and Bluetooth
 - *Optional* **ATM90E** Series
	 - **ATM90E32** Energy Monitor 
		 - 3 x Phases Line CT Clamp Input  
			 -  Example YHDC SCT013000
		 - 3 x Voltage Inputs (AC RMS)
		 - Power Modes Settings
	 - **ATM90E36** Energy Monitor 
		 - 3 x Phases Line CT Clamp Input 
			 -  Example YHDC SCT013000
		 - 3 x Voltage Inputs (AC RMS) 
		 - 1 x Phase Neutral CT Clamp
		 - DMA Mode (Logging via SPI)
		 - Power Modes Settings
 - **24C64** EEPROM 
	 - Parameter Settings
	 - Logging
 - **OLED I2C**
	 - I2C Interface
	 - OLED I2C Connector
		 - Configurable Power Pins
 - **AC Low Voltage** Input (for Safety)
	 - Power safely derived from a SELV / Wall AC/AC Power Supply 
		 - 8 to 12V AC RMS
		 - Examples GreenBrook DAT01A or TLC TC TR7
 - Onboard **3V3 DC SMPS** Power Supply
	 - Power taken from AC Input
 - Arduino **Wemos D1 Compatible** USB Interface
	 - On-board Micro USB Socket
	 - Flashing and Programming
	 - Example wemos_d1_mini32
	 - Can also used to power the board
	 - Debugging
 - On Board **NTC** (Temperature) 
 - **USER GPIO/UART**
	 - 2 GPIO for User (GP16 and GP17)
	 - UART Interfacing (UART 2)
 - **Reset Button** 
 - User **Programmable Button** 
 - **RGB Status LED**
	 - User Configurable
 - **Power LED**
	 - On 3V3 
 - **ATM CT LEDs**
	 - CF1 - Active 
	 - CF2 - ReActive
	 - CF3 - Fundamental
	 - CF4 - Harmonic
 - PCB designed to fit into an BMC enclosure 
	 - Also allows for the display to be included, or not. 
 - **Size** 70 mm x 53 mm
