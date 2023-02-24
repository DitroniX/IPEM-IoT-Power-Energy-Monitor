**IPEM ESP32 ATM90E32 ATM90E36 IoT Power Energy Monitor SDK**

------------

**This is a NEW board which has now completed Beta testing and final changes for production has been finallised.**

Four fully tested versions of this board are still planned. 

The board layout is designed to be a flexible dual purpose.
- ESP32 WROOM, with Internal Antenna, or UF.L connector for external antenna
- ATM90E32 or ATM90E36

The first production run will be for ATM90E32, both ESP32 variants.

**CT Clamp Inputs**
The forth CT input on the ATM90E32 board, which is included for the ATM90E36, now remains on the board and goes to the ESP32 ADC.  So both 
board variants will have FOUR CT inputs. 

- **ATM90E32**
 - 3 x CT Line inputs to the ATM chip.
 - 1 x CT input to the ESP32 ADC.

- **ATM90E36**
 - 3 x CT Line inputs to the ATM chip.
 - 1 x CT inputs to the ATM chip.

------------

IPEM 'Production PCB
![Display-Type-B](https://ditronix.net/wp-content/uploads/2023/02/IPEM-ESP32-ATM90E32-ATM90E36-IoT-Power-Energy-Monitor-Board-Connections-1-scaled.jpg?raw=true)

ATM90E32
![Display-Type-B](https://ditronix.net/wp-content/uploads/2023/02/IPEM-ESP32-ATM90E32-IoT-Power-Energy-Monitor-Board-Solder-Links-scaled.jpg?raw=true)

ATM90E36
![Display-Type-B](https://ditronix.net/wp-content/uploads/2023/02/IPEM-ESP32-ATM90E36-IoT-Power-Energy-Monitor-Board-Solder-Links-scaled.jpg?raw=true)

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
		 - Auto DMA selection opton via GP12
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
