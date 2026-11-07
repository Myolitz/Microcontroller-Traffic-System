// Author: Jesus Cardenas
// Class: ETEE 3376 01
// Semester: Fall 2026
// Professor: Dr. Basith

#include <Wire.h>
#include "Adafruit_VL53L0X.h"
#include <Arduino.h>

#define TCAADDR 0x70

/*============================================

GLOBAL VARIABLES BELOW

============================================*/

// Distance Sensors
Adafruit_VL53L0X sensorOne = Adafruit_VL53L0X();
Adafruit_VL53L0X sensorTwo = Adafruit_VL53L0X();

// Multiplexer Channels
const uint8_t channelOne = 7;
const uint8_t channelTwo = 2;

// LED Pins
const int pinList[] = {2, 3, 4, 8, 9, 10};
const int sensOnePins[] = 
{
  2,  // GRN
  3,  // YLW
  4   // RED
};

const int sensTwoPins[] =
{
  10,   // GRN
  9,    // YLW
  8     // RED
};

// Machine State
int signalState; // 0: Sensor 1 (CH 7) is relevant, 1; Sensor 2 (CH 2) is relevant.

// Generic Variables
const int thresholdNum = 50; // If sensor < thresholdNum -> do something
const int lockTimer = 10000; // 10 Second Lockout

// Enables/Disables Debug Mode
bool debug = false;


/*============================================

USER FUNCTIONS BELOW

============================================*/

void firstInitial()
{
  digitalWrite(sensOnePins[0], HIGH);
  digitalWrite(sensTwoPins[2], HIGH);
}

bool tcaselect(uint8_t i) { //Based off Adafruit's TCA Documentation
  if (i > 7) return false;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  uint8_t err = Wire.endTransmission();
  if (err != 0) {
    Serial.print("TCA error "); Serial.print(err);
    Serial.print(" on ch "); Serial.println(i);
  }
  return err == 0;
}

/*============================================

`SETUP` AND `LOOP` BELOW

============================================*/

void setup() {

  Serial.begin(115200);
  while (!Serial);
  Wire.begin();

  //Debug Message
  Serial.println("Initiating New Run");

  // Selecting TCA channel and initializing the sensor
  Serial.println(tcaselect(channelOne) ? "ch 7 select OK" : "ch 7 select FAIL");
  Serial.println(sensorOne.begin() ? "Sensor 1 OK" : "Sensor 1 failed");

  // Repeat for second Sensor
  Serial.println(tcaselect(channelTwo) ? "ch 7 select OK" : "ch 7 select FAIL");
  Serial.println(sensorTwo.begin() ? "Sensor 2 OK" : "Sensor 2 failed");

  //Putting the LED Pins on OUTPUT Mode
  for (int i : pinList)
  {
    pinMode(i, OUTPUT);
  }

  //Setting Signal State
  signalState = 0;
  
  firstInitial(); // Sets up the circuit to its "default" state  
}

void loop() {
  if (debug) // Goes through simple light sequence.
  {
    digitalWrite(sensOnePins[0], HIGH);
    digitalWrite(sensTwoPins[0], HIGH);

    delay(lockTimer / 2);

    digitalWrite(sensOnePins[1], HIGH);
    digitalWrite(sensTwoPins[1], HIGH);

    delay(lockTimer / 2);

    digitalWrite(sensTwoPins[2], HIGH);
    digitalWrite(sensOnePins[2], HIGH);
  }
  else 
  {
    if (signalState != 1)
    {
      int sensTwoDist = 10000;
      tcaselect(channelTwo);
      sensorTwo.startRangeContinuous();
      while (sensTwoDist == -1 || sensTwoDist == 10000) // Means theres an "invalid" distance measurement or 10000 means default state
      {
        sensTwoDist = sensorTwo.readRange();

        //Debug Printing
        Serial.print("Sensor Two Distance: "); Serial.println(sensTwoDist);
        delay(1000);

        if (sensTwoDist < thresholdNum)
        {
          sensorTwo.stopRangeContinuous(); // Stop the readings

          Serial.print("Threshold Reached! Switching Signals"); // Debug Print
          delay(10000);

          //Second Signal Method
        }
        else { continue; }
      }
    }
    else 
    {
      int sensOneDist = 10000;
      tcaselect(channelOne);
      sensorOne.startRangeContinuous();
    }
  }
}
