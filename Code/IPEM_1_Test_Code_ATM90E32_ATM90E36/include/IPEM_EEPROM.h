/*
  Dave Williams, DitroniX 2019-2023 (ditronix.net)
  IPEM-1 ESP32 ATM90E32 ATM90E36 IoT Power Energy Monitoring Energy Monitor v1.0

  Full header information in main.cpp.

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and also github.com/DitroniX
*/

// Libraries
#include <serialEEPROM.h> // Reference https://github.com/argandas/serialEEPROM

// EEPROM AT24C64 64K (8192 x 8) 32-byte page writes
serialEEPROM extEEPROM(0x50, 8192, 32); // Address, Size, PageSize

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// **************** FUNCTIONS AND ROUTINES ****************

// Read 8bit Value
byte readEEPROM(unsigned int addEEPROM)
{
  uint8_t valEEPROM = 0x00;
  valEEPROM = extEEPROM.read(addEEPROM);
  return valEEPROM;
}

// Write 8bit Value
void WriteEEPROM(unsigned int addEEPROM, uint8_t valEEPROM)
{
  extEEPROM.write(addEEPROM, valEEPROM);
}

// Read 16bit Value
uint16_t readEEPROM16(unsigned int addEEPROM)
{
  uint16_t valEEPROM = 0x0000;
  valEEPROM = (extEEPROM.read(addEEPROM + 1) << 8);
  valEEPROM |= extEEPROM.read(addEEPROM);
  return valEEPROM;
}

// Write 16bit Value
void WriteEEPROM16(unsigned int addEEPROM, uint16_t valEEPROM)
{
  extEEPROM.write(addEEPROM, valEEPROM);
  extEEPROM.write(addEEPROM + 1, valEEPROM >> 8);
}

// Clear EEPROM (Only if unlocked and fully clear, if Validation not correct or corruption)
void ClearEEPROM() // Size 0x2000 8192
{
  if (extEEPROM.read(1) != 0x99) // Check Locked Status - Future Use
  {
    if (extEEPROM.read(0) != 0x20 || extEEPROM.read(8192) != 0x20) // Check Validation(Future Use) and End Byte
    {
      Serial.println("\nPreparing New EEPROM.  Full Formatting...  Please Wait...");

      for (int i = 0; i < 8192; i++)
      {
        extEEPROM.write(i, 0x00);
      }
    }
    else
    {
      Serial.println("\nFormatting (Smart Clearing) EEPROM.  Checking, Please Wait... ");
      for (int i = 1; i < 8192; i++)
      {
        if (extEEPROM.read(i) != 0x00) // Only Clear Used Bytes
        {
          Serial.print(extEEPROM.read(i), HEX);
          extEEPROM.write(i, 0x00);
          Serial.print("+");
        }
      }
    }

    WriteEEPROM(0x00, 0x20); // Write Status Byte
    {
      for (int i = 1; i < 8192; i++)
      {
        if (extEEPROM.read(i) != 0x00) // Only Clear Used Bytes
        {
          Serial.print(extEEPROM.read(i), HEX);
        }
      }
    }
    Serial.println("EEPROM Cleared and Ready");
  }

  else
    Serial.println("EEPROM Locked");
}

// Initialize EEPROM - Address 0x0000 = EEPROM Status
void InitializeEEPROM()
{
  /* Initialize the I2C interface and EEPROM */
  Wire.begin();

  // Format New EEPROM
  if (extEEPROM.read(0) != 0x20)
    ClearEEPROM();

  Serial.print("EEPROM Check: ");
  Serial.print(readEEPROM16(0x00), HEX);

  // EEPROM Validation
  if (readEEPROM16(0x00) == 0x00)
  {
    Serial.print("\t(Formatted OK)");
  }
  if (readEEPROM16(0x00) == 0xFF)
  {
    Serial.print("\t(Unformatted)");
  }
  if (readEEPROM16(0x00) == 0x20)
  {
    Serial.print("\t(Validation OK)");
  }
  if (readEEPROM16(0x01) == 0x99)
  {
    Serial.print("\t(Locked)");
  }

  Serial.println("\n");
}
