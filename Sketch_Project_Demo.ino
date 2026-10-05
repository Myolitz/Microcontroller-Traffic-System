#include <Wire.h>
#include "Adafruit_VL53L0X.h"

#define MULTIPLEXER 0x70

//Both Sensors
Adafruit_VL53L0X sensorOne = Adafruit_VL53L0X();
Adafruit_VL53L0X sensorTwo = Adafruit_VL53L0X();

//First Traffic LED
const int grnPinOne = A2;
const int ylwPinOne = A4;
const int redPinOne = A6;

//Second Traffic LED
const int grnPinTwo = A3;
const int ylwPinTwo = A5;
const int redPinTwo = A7;

// Machine State
bool firstSignal;
bool secSignal;

// Generic Variables
const int thresholdNum = 500;
const int lockTimer = 10000;
int sensOneDist;
int sensTwoDist;


//User-Created Functions

/*
  Used for debugging, based on Adafruit's Official Multiplex Documentation
*/
void tcaseselect(uint8_t i)
{
  if (i > 7)
  {
    return;
  }
  
  Wire.beginTransmission(MULTIPLEXER);
  Wire.write(1 << i);
  Wire.endTransmission();
}

void activateSecondSignal() //Switching from one state to another
{
  digitalWrite(grnPinOne, LOW);
  delay(10);
  digitalWrite(ylwPinOne, HIGH);
  delay(10);
  digitalWrite(ylwPinOne, LOW);
  delay(10);
  digitalWrite(redPinOne, HIGH);
  digitalWrite(grnPinTwo, HIGH);

  firstSignal = false;
  secondSignal = true;
}

void setup() {
  // put your setup code here, to run once:

  //Interval of Information Throughput
  Serial.begin(115200);

  //Beginning the Wire Library for I2C information reading.
  Wire.begin();

  //Putting the LED Pins on OUTPUT Mode
  PinMode(grnPinOne, OUTPUT);
  PinMode(grnPinTwo, OUTPUT);
  PinMode(ylwPinOne, OUTPUT);
  PinMode(ylwPinTwo, OUTPUT);
  PinMode(redPinOne, OUTPUT);
  PinMode(redPinTwo, OUTPUT);

  //Initializing Variables
  firstSignal = true;
  secondSignal = false;


}

void loop() {
  // put your main code here, to run repeatedly:

  if (firstSignal)
  {
    while (sensOneDist < thresholdNum)
    {
      sensOneDist = sensorOne.readRange();
    }
  }

}
