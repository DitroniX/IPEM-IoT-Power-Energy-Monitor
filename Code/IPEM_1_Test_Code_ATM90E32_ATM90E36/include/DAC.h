/*
  Dave Williams, DitroniX 2019-2023 (ditronix.net)
  IPEM-1 ESP32 ATM90E32 ATM90E36 IoT Power Energy Monitoring Energy Monitor v1.0

  Full header information in main.cpp.

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and also github.com/DitroniX
*/

// NOTE: THIS CODE IS UNDER DEVELOPMENT. DAVE WILLIAMS JUNE 2023

// Note: This function needs to use one of the two DAC outputs, both of which are already assigned by default.  WO on GP25 is the least needed, so we will repurpose this GPIO.
// To repurpose, you will need to CUT a track from GP25 to the ATM WO.  Details will be provided in the Wiki once the testing is complete.
// DAC Output Voltage is NOT Calibrated, although.  Basic DAC Output Voltage Test @ 1.5 = 1.4979V

// Libraries
#include <Arduino.h>
#include <DacESP32.h>

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// Not Directly possible on IPEM 1.2304.203P without a board mod.  Effectively, Cut a Track and Bypass WO Input.
#define DAC_GPIO GPIO_NUM_25 // Define DAC GPIO (Default GPIO_NUM_25 = GP25)
DacESP32 dacoutput(DAC_GPIO);

// Variables USER and External (Power Up Default)
boolean EnableDACLocal = false;  // Enable/TurnOn DAC Local. Default(false) Soft Switch for Local Direct Control /DAC-On /DAC-Off
boolean EnableDACRemote = false; // Enable/Allow DAC Remote Power Control. Default(false) Soft Switch for Remote Control /DAC-Remote

// EnableDACTestOutput will over-ride EnableDACLocal and EnableDACRemote
boolean EnableDACTestOutput = false; // Enable/TurnOn Continuous DAC Cycle Test.  Will Bypass Actual Readings. Default(false)

// Variables DAC Internal
float DAC_ScaledVoltage;
float DAC_LastValue;
int DAC_WritePower;

int DAC_MaximumResolution; // DAC Maximum 8-bit argument (Default 255)  Note: This is the only option when using ESP32 DAC
float DAC_ScalingFactor;   // DAC Scaling Power Factor Based on Rounded Maximum Power (i.e. DAC_MaximumPowerLimit / Resolution of DAC_MaximumDutyCycle)

// Variables DAC External
float DACPowerOutput;     // Last Value Used For DAC
int DACPowerPercentage;   // Base Power Percentage
float DACLocalPower = 0;  // Base Local Power Level (Power Up Default 0)
float DACRemotePower = 0; // Base Remote Power Level (Power Up Default 0)

// Constants Power
float DAC_MaximumPowerLimit = 4000; // Maximum CT Power Range 0 to x. Upper Limit of Measured Power for Scaling (Default 4000)
int DAC_MinimumPowerLimit = 0;      // DAC Minimum 8-bit argument (Default 0)
int DAC_MinimumPowerThreshold = 5;  // Minimum Power Threshold (in Watts), Before Outputting to DAC.  Also used for Noise and Jitter Control. (Default 5)
int DAC_ResolutionBit = 8;          // DAC Resolution bit (Default 8).   Note: This is the only option when using ESP32 DAC
float DAC_MinimumVoltage = 0;       // (Default 0)
float DAC_MaximumVoltage = 3.3;     // Default is 3.3 but can be scaled if using external driver control. i.e. DAC into a transistor.

// **************** FUNCTIONS AND ROUTINES ****************

// Initialise DAC
void InitialiseDAC()
{
    DAC_MaximumResolution = (int)(pow(2, DAC_ResolutionBit) - 1);   // 8bit = 256-1 | 10bit = 1024-1 | 12bit = 4096-1 | 14bit = 16384-1 | 16bit = 65536-1.
    DAC_ScalingFactor = DAC_MaximumVoltage / DAC_MaximumPowerLimit; // E.g. 3.3 / 4000

    // DAC Integration Status
    if (EnableDACLocal == false && EnableDACRemote == false && EnableDACTestOutput == false)
    {
        Serial.println("DAC Output is Disabled\n");
    }
    else
    {
        if (EnableDACLocal == true)
            Serial.println("DAC Output is Enabled from DACLocal");

        if (EnableDACRemote == true)
            Serial.println("DAC Output is Enabled from DACRemote");

        if (EnableDACTestOutput == true)
            Serial.println("DAC Output is Enabled from DAC Test Output");

        // Configuration Info
        Serial.printf("DAC Maximum Power Limit:\t%0f\n", DAC_MaximumPowerLimit);
        Serial.printf("DAC Resolution Bit:\t\t%d\n", DAC_ResolutionBit);
        Serial.printf("DAC Maximum Resolution:\t\t%d\n", DAC_MaximumResolution);
        Serial.printf("DAC Scaling Factor:\t\t%f\n\n", DAC_ScalingFactor);
    }
} // InitialiseDAC

// DAC Output Voltage Based on BasePower PreScaled to BaseValue
void DAC_WriteVoltage(float InputBasePower)
{
    if (DACPowerOutput != InputBasePower)
    {
        // Store Last Reading
        DACPowerOutput = InputBasePower;

        // Set Max and Min Limits (Top and Tail)
        if (DACPowerOutput >= DAC_MaximumPowerLimit)
            DACPowerOutput = DAC_MaximumPowerLimit;

        if (DACPowerOutput <= DAC_MinimumPowerLimit)
            DACPowerOutput = DAC_MinimumPowerLimit;

        // Calculate DAC Duty Cycle
        DAC_ScaledVoltage = (InputBasePower * DAC_ScalingFactor);

        // Set Max Duty Cycle limit
        if (DAC_ScaledVoltage >= DAC_MaximumVoltage)
            DAC_ScaledVoltage = DAC_MaximumVoltage;

        // Set Min Duty Cycle limit.  If Zero then Ignore Lower Limit as this is DAC Off.
        if (DAC_ScaledVoltage <= DAC_MinimumVoltage)
            DAC_ScaledVoltage = 0;

        // Update DACLastValue to DAC Port. Example: Vout = 100/255 * 3.3V = ~1.3V
        dacoutput.outputVoltage(DAC_ScaledVoltage);

        // Display Value
        if (DACPowerOutput > 0)
        {
            Serial.print("Updated DAC Power: " + String(InputBasePower) + "\t\t");
            Serial.print("Voltage: " + String(DAC_ScaledVoltage) + "\t\t");
            DACPowerPercentage = map(DACPowerOutput, DAC_MinimumPowerLimit, DAC_MaximumPowerLimit, 0, 100); // Scaled example 0-4kW = 0-100% = 255
            Serial.println(String(DACPowerPercentage) + " %");
        }
        else
        {
            Serial.println("Updated DAC Output \tDAC Off");
        }
    }
} // DAC_WriteVoltage

// Output DAC Voltage Proportionally Based on Input Power.  Approx Values: ?=1kW, ?=2kW, ?=3kW and ?=4kW
void DAC_Power(float InputPower = 0)
{
    // Check Maximum Limits.  0 to Ignore Limit.
    if (InputPower >= DAC_MaximumPowerLimit)
        InputPower = DAC_MaximumPowerLimit;

    // Check Valid Limits / Calculate
    if (InputPower > DAC_MinimumPowerThreshold && InputPower > 0)
    { // Local Power Control via CT
        DAC_WriteVoltage(InputPower);
    }
    else
    { // Turn DAC Off if Zero or Below DAC_MinimumPowerThreshold
        DAC_WriteVoltage(0);
    }

    // Configuration Conflict Issue - Turn Both Off
    if (EnableDACLocal == true && EnableDACRemote == true)
    {
        Serial.print("DAC Configuration Issue - Forcing Off");
        DAC_WriteVoltage(0);
        EnableDACLocal = false;
        EnableDACRemote = false;
    }

} // DAC_Power

// Output DAC - Test Use Only.  Modes 0 = Dynamic Output based on CT Power. 1 = SineWave.  2 =Fixed Test Voltage for Calibration etc. 
void DAC_Test(int Test = 0, float DAC_Value = 0)
{
    dacoutput.outputVoltage((float)0);
    int DAC_StepDelayms = 500; // Inter Step Delay in milli seconds
    float DAC_Increment = 0.1; // Loop Step Rate

    if (Test == 0)
    { // DAC_Test Slow Stepped Voltages

        for (float dacv = 0; dacv <= DAC_MaximumVoltage; dacv = dacv + DAC_Increment)
        {
            Serial.printf("DAC Voltage Output Test:\t%f\t", dacv);
            DACPowerPercentage = map(dacv, DAC_MinimumVoltage, DAC_MaximumVoltage, 0, 100); // Scaled example 0-4kW = 0-100% = 255
            Serial.println(String(DACPowerPercentage) + " %");
            dacoutput.outputVoltage((float)dacv);
            delay(DAC_StepDelayms);
        }

        for (float dacv = DAC_MaximumVoltage; dacv >= DAC_MinimumVoltage; dacv = dacv - DAC_Increment)
        {
            Serial.printf("DAC Voltage Output Test:\t%f\t", dacv);
            DACPowerPercentage = map(dacv, DAC_MinimumVoltage, DAC_MaximumVoltage, 0, 100); // Scaled example 0-4kW = 0-100% = 255
            Serial.println(String(DACPowerPercentage) + " %");
            dacoutput.outputVoltage((float)dacv);
            delay(DAC_StepDelayms);
        }
    }

    if (Test == 1)
    { // DAC Test Sinewave
        Serial.printf("DAC Voltage Output Test:\tSineWave");
        dacoutput.outputCW(DAC_Value); // 1, 10, 50 Hz etc
    }

    if (Test == 2)
    { // DAC Test Fixed Voltage
        Serial.printf("DAC Voltage Output Test:\t%f\t", DAC_Value);
        DACPowerPercentage = map(DAC_Value, DAC_MinimumVoltage, DAC_MaximumVoltage, 0, 100); // Scaled example 0-4kW = 0-100% = 255
        Serial.println(String(DACPowerPercentage) + " %");
        dacoutput.outputVoltage((float)DAC_Value);
    }
}
