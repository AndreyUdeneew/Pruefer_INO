#include <Wire.h>
#include "MAX30105.h"
//#include "Pulse.h"
#include <avr/pgmspace.h>
#include <EEPROM.h>
#include <avr/sleep.h>
int tmpAddress = 0x4A; //Slave Addresses set
int ResolutionBits = 12;   //Resolution set
MAX30105 particleSensor;
#define debug Serial

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(115200);  // start serial for output
  SetResolution();
  // Initialize sensor
  if (particleSensor.begin() == false)
  {
    debug.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  particleSensor.setup(); //Configure sensor. Use 6.4mA for LED drive
}

void loop()
{
  getTemperature();
//  Serial.print(", ");
    colorReatio();
//  Serial.print(particleSensor.getRed());
//  Serial.print(", ");
//  Serial.println(particleSensor.getIR());
  delay(10);
}

float colorReatio()
{
  float R2IR = ((float)particleSensor.getIR()/(float)particleSensor.getRed())*1000;
  Serial.println(R2IR);
}

float getTemperature()
{
  Wire.requestFrom(tmpAddress,2);
  byte MSB = Wire.read();
  byte LSB = Wire.read();
  int TemperatureSum = ((MSB << 8) | LSB) >> 4;
  float celsius = TemperatureSum*0.0625;
//  Serial.print("Celsius: ");
  Serial.println(celsius);
}

void SetResolution(){
  if (ResolutionBits < 9 || ResolutionBits > 12) exit;
  Wire.beginTransmission(tmpAddress);
  Wire.write(B00000001); //addresses the configuration register
  Wire.write((ResolutionBits-9) << 5); //writes the resolution bits
  Wire.endTransmission();
  Wire.beginTransmission(tmpAddress); //resets to reading the temperature
  Wire.write((byte)0x00);
  Wire.endTransmission();
}
