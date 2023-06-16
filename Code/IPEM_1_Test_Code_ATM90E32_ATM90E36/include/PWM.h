/*
  Dave Williams, DitroniX 2019-2023 (ditronix.net)
  IPEM-1 ESP32 ATM90E32 ATM90E36 IoT Power Energy Monitoring Energy Monitor v1.0

  Full header information in main.cpp.

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and also github.com/DitroniX
*/

// NOTE: THIS CODE IS UNDER DEVELOPMENT. DAVE WILLIAMS JUNE 2023

// Libraries
#include <Arduino.h>

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

#define PWM_GPIO 17 // Define PWM GPIO (Default  GPIO 17 = Existing USR_GP17_TX_PWM)

// Variables USER and External (Power Up Default)
boolean EnablePWMLocal = false;  // Enable/TurnOn PWM Local. Default(false) Soft Switch for Local Direct Control /PWM-On /PWM-Off
boolean EnablePWMRemote = false; // Enable/Allow PWM Remote Power Control. Default(false) Soft Switch for Remote Control /PWM-Remote

// Development and testing Use.  EnablePWMTestOutput will over-ride EnablePWMLocal and EnablePWMRemote
boolean EnablePWMTestOutput = false; // Enable/TurnOn Continuous PWM Cycle Test.  Will Bypass Actual Readings. Default(false)

// Variables PWM Internal
int PWM_DutyCycle;
float PWM_LastBasePower;
int PWM_MaximumResolution; // 8bit = 256-1 | 10bit = 1024-1 | 12bit = 4096-1 | 14bit = 16384-1 | 16bit = 65536-1.
float PWM_ScalingFactor;   // PWM Scaling Power Factor Based on Rounded Maximum Power (i.e. PWM_MaximumPowerLimit / PWM_MaximumResolution)

// Variables PWM External
int PWMPowerOutput;       // PWM Power Output Value
int PWMPowerPercentage;   // PWM Power Output Percentage
float PWMRemotePower = 0; // Remote Default Base Power Level (Default 0)
float PWMLocalPower = 0;  // Local Default Base Power Level (Default 0)

// Constants Power
float PWM_MaximumPowerLimit = 4000; // Maximum CT Power Range (in Watts) 0 to x (Default 4000). Upper Limit of Measured Power for Scaling
float PWM_MinimumDutyCycle = 0;     // PWM Minimum Duty Cycle (Default 0)
int PWM_MinimumPowerThreshold = 5;  // Minimum Power Threshold (in Watts), Before Outputting PWM.  Also used for Noise and Jitter Control. (Default 5)
int PWM_FrequencyHz = 1000;         // PWM Default Frequency (Default 1000).  PWM Frequency Range 10Hz to 40MHz.
int PWM_ResolutionBit = 14;         // PWM bit Range 1-16 bits (Default 14)
int PWM_Channel = 0;                // PWM Channels 0-15 (Default 0)

// **************** FUNCTIONS AND ROUTINES ****************

// Initialise PWM
void InitialisePWM()
{
    ledcSetup(PWM_Channel, PWM_FrequencyHz, PWM_ResolutionBit);
    ledcAttachPin(PWM_GPIO, PWM_Channel);
    PWM_MaximumResolution = (int)(pow(2, PWM_ResolutionBit) - 1); // 8bit = 256-1 | 10bit = 1024-1 | 12bit = 4096-1 | 14bit = 16384-1 | 16bit = 65536-1.
    PWM_ScalingFactor = PWM_MaximumPowerLimit / PWM_MaximumResolution;
    ledcWrite(PWM_Channel, 0);

    // PWM Integration Status
    if (EnablePWMLocal == false && EnablePWMRemote == false && EnablePWMTestOutput == false)
    {
        Serial.println("PWM Output is Disabled\n");
    }
    else
    {
        if (EnablePWMLocal == true)
            Serial.println("PWM Output is Enabled from PWMLocal");

        if (EnablePWMRemote == true)
            Serial.println("PWM Output is Enabled from PWMRemote");

        if (EnablePWMTestOutput == true)
            Serial.println("PWM Output is Enabled from PWM Test Output");

        // Configuration Info
        Serial.printf("PWM Maximum Power Limit:\t%0f\n", PWM_MaximumPowerLimit);
        Serial.printf("PWM Resolution Bit:\t\t%d\n", PWM_ResolutionBit);
        Serial.printf("PWM Maximum Resolution:\t\t%d\n", PWM_MaximumResolution);
        Serial.printf("PWM Scaling Factor:\t\t%f\n\n", PWM_ScalingFactor);
    }

} // InitialisePWM

// PWM Output Based on Input Base Power (With Defaults).
void PWM_Write(float InputBasePower)
{
    if (PWM_LastBasePower != InputBasePower)
    {
        // Store Last Reading and Calculate PWM_LastBasePower
        PWM_LastBasePower = InputBasePower;

        // Calculate PWM Duty Cycle
        PWM_DutyCycle = (InputBasePower / PWM_ScalingFactor);

        // Set Max Duty Cycle limit
        if (PWM_DutyCycle >= PWM_MaximumResolution)
            PWM_DutyCycle = PWM_MaximumResolution;

        // Set Min Duty Cycle limit.  If Zero then Ignore Lower Limit as this is PWM Off.
        if (PWM_DutyCycle <= PWM_MinimumDutyCycle && PWM_DutyCycle > 0)
            PWM_DutyCycle = 0;

        // Update PWM Port
        ledcWrite(PWM_Channel, PWM_DutyCycle);

        // Display Value
        if (PWM_DutyCycle > 0)
        {
            Serial.print("Updated PWM Power: " + String(PWM_LastBasePower) + "\t\t");
            Serial.print("Duty Cycle: " + String(PWM_DutyCycle) + "\t\t");
            PWMPowerPercentage = map(PWM_DutyCycle, PWM_MinimumDutyCycle, PWM_MaximumResolution, 0, 100); // Scaled example 0-4kW = 0-100% = 1023
            Serial.println(String(PWMPowerPercentage) + " %");
        }
        else
        {
            Serial.println("Updated PWM Power Duty Cycle: \tPWM Off");
        }
    }

} // PWM_Write

// Output PWM Value Based on Input Power. DutyCycle Approx Values: 256=1kW, 512=2kW, 769=3kW and 1023=4kW
void PWM_Power(float InputBasePower = 0)
{
    // Check Maximum Limits.  0 to Ignore Limit.
    if (InputBasePower >= PWM_MaximumPowerLimit)
        InputBasePower = PWM_MaximumPowerLimit;

    // Check Valid Limits / Calculate.  Also ensure no Negative Powers
    if (InputBasePower >= PWM_MinimumPowerThreshold)
    {
        PWM_Write(InputBasePower);
    }
    else
    { // Turn PWM Off if Zero or Below PWM_MinimumPowerThreshold
        PWM_Write(0);
    }

    // Configuration Conflict Issue - Turn Both Off
    if (EnablePWMLocal == true && EnablePWMRemote == true)
    {
        Serial.print("PWM Configuration Issue - Forcing Off");
        PWM_Write(0);
        EnablePWMLocal = false;
        EnablePWMRemote = false;
        EnablePWMTestOutput = false;
    }

} // PWM_Power

// Output Full PWM Duty / Power Cycle - Test Use Only
void PWM_Test()
{
    int BasePower = 0;

    /* Increasing an LED brightness with PWM */
    for (BasePower = 0; BasePower <= PWM_MaximumPowerLimit; BasePower++)
    {
        PWM_Write(BasePower);
    }
    /* Decreasing an LED brightness with PWM */
    for (BasePower = PWM_MaximumPowerLimit; BasePower >= 0; BasePower--)
    {
        PWM_Write(BasePower);
    }
} // PWM_Test
