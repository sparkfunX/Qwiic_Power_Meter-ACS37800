/*
  Production Test code for the Qwiic Power Meter ACS37800
  By: Paul Clark
  SparkFun Electronics
  Date: March 19th, 2021
  License: please see LICENSE.md for details

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/17873

  LED_BUILTIN will light up if the ACS37800 is detected and the registers read correctly

*/

#include "SparkFun_ACS37800_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_ACS37800
#include <Wire.h>

ACS37800 mySensor; //Create an object of the ACS37800 class

// Global copies of the register contents
// EEPROM registers are 0B-0F. Shadow registers are 1B-1F. Volatile registers are 20-30.
ACS37800_REGISTER_0B_t reg1B;
ACS37800_REGISTER_0D_t reg1D;

void setup()
{
  Serial.begin(115200);
  Serial.println(F("ACS37800 Production Test"));

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  delay(250); // Try every 0.25 seconds

  Wire.begin();

  Wire.beginTransmission(0x60); // Detect ACS37800 on address 0x60
  if (Wire.endTransmission() != 0)
  {
    digitalWrite(LED_BUILTIN, LOW);
    Wire.end();
    Serial.println(F("Error: nothing detected on address 0x60"));
    return;
  }
  
  if (mySensor.begin(0x60) == false) //Initialize sensor using I2C address 0x60
  {
    digitalWrite(LED_BUILTIN, LOW);
    Wire.end();
    Serial.println(F("Error: .begin failed"));
    return;
  }

  // Read the shadow registers
  mySensor.readRegister(&reg1B.data.all, ACS37800_REGISTER_SHADOW_1B);
  mySensor.readRegister(&reg1D.data.all, ACS37800_REGISTER_SHADOW_1D);

  // Check the register contents
  if (reg1B.data.bits.crs_sns != 0x5)
  {
    digitalWrite(LED_BUILTIN, LOW);
    Wire.end();
    Serial.println(F("Error: invalid register value (crs_sns)"));
    return;
  }

  if (reg1D.data.bits.fault != 0x46)
  {
    digitalWrite(LED_BUILTIN, LOW);
    Wire.end();
    Serial.println(F("Error: invalid register value (fault)"));
    return;
  }

  // All good!
  digitalWrite(LED_BUILTIN, HIGH);
  Wire.end();
  Serial.println(F("ACS37800 detected! Test passed!"));
}
