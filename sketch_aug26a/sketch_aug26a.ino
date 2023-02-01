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
  SetResolution();      // thermal sensor
  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST))
  {
    debug.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
    //Setup to sense up to 18 inches, max LED brightness
  byte ledBrightness = 0xFF; //Options: 0=Off to 255=50mA
  byte sampleAverage = 1; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 3200; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 16384; //Options: 2048, 4096, 8192, 16384

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
//  particleSensor.setup(); //Configure sensor. Use 6.4mA for LED drive
  pinMode(A0, OUTPUT);
}

void loop()
{
  getTemperature();
//  Serial.print(", ");
    colorReatio();
//  Serial.print(particleSensor.getRed());
//  Serial.print(", ");
//  Serial.println(particleSensor.getIR());
    digitalWrite(A0, HIGH);
    digitalWrite(A0, LOW);
//  delay(20);
}

float colorReatio()
{
  float R2IR = ((float)particleSensor.getRed()/(float)particleSensor.getIR())*1000;
//uint16_t R2IR = particleSensor.getRed();
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
