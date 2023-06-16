The below example Serial ouput is based on DisplayRegisters with a false argument. Refresh Values and Display. Default false = Mute Expanded Info to Serial

Only some Indexes have a value, as defined in Domoticz Devices Setup. Example below

		82	Dummy	00082082	1	iVoltage1	General	Voltage	246.31 V	-	2023-03-18 14:16:14
		83	Dummy	00082083	1	iCT1	General	Current	0.03 A	-	-	2023-03-18 14:16:14
		84	Dummy	00082084	1	iCT2	General	Current	0 A	-	-	   	2023-03-18 14:16:14
		85	Dummy	00082085	1	iCT3	General	Current	0 A	-	-	   	2023-03-18 14:16:14
		86	Dummy	82086	1	iCPwr1	Usage	Electric	7.39 Watt	-	-	   	2023-03-18 14:16:14
		87	Dummy	82087	1	iCPwr2	Usage	Electric	0 Watt	-	-	   	2023-03-18 14:16:14
		88	Dummy	82088	1	iCPwr3	Usage	Electric	0 Watt	-	-	   	2023-03-18 14:16:14
		89	Dummy	82089	1	iAPwr1	Usage	Electric	5.17 Watt	-	-	   	2023-03-18 14:16:14
		90	Dummy	82090	1	iAPwr2	Usage	Electric	0 Watt	-	-	   	2023-03-18 14:16:14
		91	Dummy	82091	1	iAPwr3	Usage	Electric	0 Watt	-	-	   	2023-03-18 14:16:14
		92	Dummy	82092	1	iImpPwr1	Usage	Electric	0 Watt	-	-	   	2023-03-18 13:10:35
		93	Dummy	82093	1	iImpPwr2	Usage	Electric	0 Watt	-	-	   	2023-03-18 13:10:40
		94	Dummy	82094	1	iImpPwr3	Usage	Electric	0 Watt	-	-	   	2023-03-18 13:10:45
		95	Dummy	82095	1	iExpPwr1	Usage	Electric	0 Watt	-	-	   	2023-03-18 13:10:54
		96	Dummy	82096	1	iExpPwr2	Usage	Electric	0 Watt	-	-	   	2023-03-18 13:10:59
		97	Dummy	82097	1	iExpPwr3	Usage	Electric	0 Watt	-	-	   	2023-03-18 13:11:04
		98	Dummy	00082098	1	iPF1	General	Custom Sensor	0 PF	-	-	2023-03-18 13:18:58
		99	Dummy	00082099	1	iPF2	General	Custom Sensor	0 PF	-	-	2023-03-18 13:19:05
		100	Dummy	00082100	1	iPF3	General	Custom Sensor	0 PF	-	-	2023-03-18 13:19:12

This translated to the Index setting example within the IPEM Domoticz code.

		/ Set these values to the Domoticz Devices Indexes (IDX).  If Zero, then entry is ignored. Device needs to be created in Domoticz.
		int idxLineVoltage1 = 82;      // LineVoltage1 - Urms - Line 1 Voltage RMS
		int idxLineVoltage2 = 0;       // LineVoltage2 - Urms - Line 2 Voltage RMS
		int idxLineVoltage3 = 0;       // LineVoltage3 - Urms - Line 3 Voltage RMS
		int idxLineVoltageAverage = 0; // LineVoltageAverage - Urms - Line Average Voltage RMS (V1+V2+V3/3)
		int idxLineVoltageTotal = 0;   // LineVoltageTotal - Urms - Line Voltage Total RMS (V1+0+V3)

		int idxLineCurrentCT1 = 83;  // LineCurrentCT1 - Irms - Line 1 Current RMS
		int idxLineCurrentCT2 = 84;  // LineCurrentCT2 - Irms - Line 2 Current RMS
		int idxLineCurrentCT3 = 85;  // LineCurrentCT3 - Irms - Line 3 Current RMS
		int idxLineCurrentCT4 = 0;   // LineCurrentCT4 - Irms - Line 4 Current RMS (ESP)
		int idxLineCurrentCTN = 0;   // LineCurrentCTN - Irms - Line N Current RMS
		int idxLineCurrentTotal = 0; // LineCurrentTotal - Irms - Line Total Current RMS (CT1+CT2+CT3+(CT4))

===============

Display Output Set to Minimised

PCB Temperature Sensor (NTC_IN VN):     33.00 ºC

ADC Raw: 2609 > ADC Adjusted: 2609 > Calculated: 24.85 V

Sending Message to Domoticz #82 245.45  LineVoltage1
Sending Message to Domoticz #83 0.03    LineCurrentCT1
Sending Message to Domoticz #84 0.00    LineCurrentCT2
Sending Message to Domoticz #85 0.00    LineCurrentCT3
Sending Message to Domoticz #86 7.36    CalculatedPowerCT1
Sending Message to Domoticz #87 0.00    CalculatedPowerCT2
Sending Message to Domoticz #88 0.00    CalculatedPowerCT3
Sending Message to Domoticz #89 5.02    ActivePowerCT1
Sending Message to Domoticz #90 0.00    ActivePowerCT2
Sending Message to Domoticz #91 0.00    ActivePowerCT3

14:16:07> Published to Domoticz