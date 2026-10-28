#include <Wire.h>
#include "Adafruit_VL53L0X.h"
#include <Arduino.h>

#define TCAADDR 0x70

// External Devices
Adafruit_VL53L0X sensorOne = Adafruit_VL53L0X();
Adafruit_VL53L0X sensorTwo = Adafruit_VL53L0X();
VL53L0X_RangingMeasurementData_t measure;

//First Traffic LED
const int grnPinOne = 2;
const int ylwPinOne = 3;
const int redPinOne = 4;

//Second Traffic LED
const int grnPinTwo = 5;
const int ylwPinTwo = 6;
const int redPinTwo = 7;

// Machine State
bool firstSignal, secSignal, greenOn;

// Generic Variables
const int thresholdNum = 250;
const int lockTimer = 10000;
int sensOneDist;
int sensTwoDist;

const uint8_t CH_ONE = 2;
const uint8_t CH_TWO = 7;

//User-Created Functions
bool tcaselect(uint8_t i) { //Based off Adafruit's TCA Documentation
  if (i > 7) return false;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  uint8_t err = Wire.endTransmission();
  if (Wire.endTransmission()) {
    Serial.print("TCA error "); Serial.print(err);
    Serial.print(" on ch "); Serial.println(i);
  }
  return err == 0;
}

void activateSecondSignal() //Switching from one state to another
{
  digitalWrite(grnPinOne, LOW);
  delay(3000);
  digitalWrite(ylwPinOne, HIGH);
  delay(3000);
  digitalWrite(ylwPinOne, LOW);
  delay(1500);
  digitalWrite(redPinOne, HIGH);
  digitalWrite(grnPinTwo, HIGH);

  firstSignal = false;
  secSignal = true;
  greenOn = false;
}

void activateFirstSignal()
{
  digitalWrite(grnPinTwo, LOW);
  delay(3000);
  digitalWrite(ylwPinTwo, HIGH);
  delay(3000);
  digitalWrite(ylwPinTwo, LOW);
  delay(1500);
  digitalWrite(redPinTwo, HIGH);
  digitalWrite(grnPinOne, HIGH);

  firstSignal = true;
  secSignal = false;
  greenOn = false;
}

void setup() {

  while (!Serial);
  delay(1000);

  //Beginning the two objects
  tcaselect(CH_ONE);
  sensorOne.begin();
  sensorOne.rangingTest(&measure, false);

  tcaselect(CH_TWO);
  sensorTwo.begin();
  sensorTwo.rangingTest(&measure, false);

  //Interval of Information Throughput
  Serial.begin(115200);

  //Putting the LED Pins on OUTPUT Mode
  pinMode(grnPinOne, OUTPUT);
  pinMode(grnPinTwo, OUTPUT);
  pinMode(ylwPinOne, OUTPUT);
  pinMode(ylwPinTwo, OUTPUT);
  pinMode(redPinOne, OUTPUT);
  pinMode(redPinTwo, OUTPUT);

  //Initializing Variables
  firstSignal = true;
  secSignal = false;
}

void loop() {
  // put your main code here, to run repeatedly:

  
  

  if (firstSignal)
  {
    tcaselect(CH_ONE);
    while (sensOneDist > thresholdNum)
    {
      if (!greenOn)
      {
        digitalWrite(grnPinOne, HIGH);
        greenOn = true;
      }
      sensOneDist = measure.RangeMilliMeter;
      Serial.print("SensOneDist: "); Serial.print(sensOneDist);
      delay(1000);
    }
    activateSecondSignal();
  }
  if (secSignal)
  {
    tcaselect(CH_TWO);
    while (sensTwoDist < thresholdNum)
    {
      if (!greenOn)
      {
        digitalWrite(grnPinTwo, HIGH);
        greenOn = true;
      }
      sensTwoDist = measure.RangeMilliMeter;
      delay(1000);
    }
    activateSecondSignal();
  }
}
