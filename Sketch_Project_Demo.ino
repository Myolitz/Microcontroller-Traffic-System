#include <Wire.h>
#include "Adafruit_VL53L0X.h"

//Both
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




}

void loop() {
  // put your main code here, to run repeatedly:

}
