// Author: Jesus Cardenas
// Class: ETEE 3376
// Semester: Fall 2026
// Professor: Dr. Basith

#include <Wire.h>
#include "Adafruit_VL53L0X.h"
#include <Arduino.h>
#include <TM1637Display.h>

#define TCAADDR 0x70
#define CLK 13
#define DIO 12

/*============================================

GLOBAL VARIABLES BELOW

============================================*/

// Distance Sensos
Adafruit_VL53L0X sensorOne = Adafruit_VL53L0X();

//Timer Display
TM1637Display display(CLK, DIO);

// TCA Channel
const uint8_t sensorChannel = 2;

// LED Pins
const uint8_t ledPins[] = {2, 3, 4, 8, 9, 10};
const uint8_t signalOnePins[] =
{
  2, // GRN
  3, // YLW
  4  // RED
};

const uint8_t signalTwoPins[] =
{
  10, // GRN
  9,  // YLW
  8   // RED
};

// Threshold Nums
const uint8_t thresholdNum = 100; // 100 mm
const uint16_t lightTimer = 5000; // 5 Sec
const uint16_t lockTimer = 10000; // 10 Sec
int sensDist;

// LED Debug
bool debug = false;

/*============================================

USER FUNCTIONS BELOW

============================================*/

void firstInitial()
{
  digitalWrite(signalOnePins[0], HIGH);
  digitalWrite(signalTwoPins[2], HIGH);
}

void timerCountdown(int startTime)
{
   for (int i = (startTime / 1000); i >= 0; i--) {
    display.showNumberDec(i, true); // Simply shows
    delay(1000);
  }

  // Countdown finished: flash 0 a few times, then stop
  for (int j = 0; j < 2; j++) {
    display.clear();
    delay(300);
    display.showNumberDec(0);
    delay(300);
  }
}

void firstSignal()
{

  digitalWrite(signalTwoPins[0], LOW); // GRN 2 LOW
  digitalWrite(signalTwoPins[1], HIGH); // YLW 2 HIGH

  timerCountdown(lightTimer);

  digitalWrite(signalTwoPins[1], LOW); // YLW 2 LOW
  digitalWrite(signalTwoPins[2], HIGH); // RED 2 HIGH

  timerCountdown(lightTimer);

  digitalWrite(signalOnePins[2], LOW); // RED 1 LOW
  digitalWrite(signalOnePins[0], HIGH); // GRN 1 HIGH

  timerCountdown(lockTimer); // Locking the System before it can be triggered again
}

void secondSignal()
{
  
  digitalWrite(signalOnePins[0], LOW); // GRN 1 LOW
  digitalWrite(signalOnePins[1], HIGH); // YLW 1 HIGH

  timerCountdown(lightTimer);

  digitalWrite(signalOnePins[1], LOW); // YLW 1 LOW
  digitalWrite(signalOnePins[2], HIGH); // RED 1 HIGH

  timerCountdown(lightTimer);

  digitalWrite(signalTwoPins[2], LOW); // RED 2 LOW
  digitalWrite(signalTwoPins[0], HIGH); // GRN 2 HIGH

  timerCountdown(lockTimer); // Locks the system for 10 seconds

  firstSignal(); // Reverts to `Default`
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

  // More Debug
  Serial.println("Initializing New Run");

  // Selecting the channel and Initializing the Sensor
  tcaselect(sensorChannel);
  sensorOne.begin();
  sensorOne.startRangeContinuous();

  // Setting Pin Modes
  for (int i : ledPins)
  {
    pinMode(i, OUTPUT); 
  }

  display.setBrightness(5);


  sensDist = 10000;

  firstInitial();
}

void loop() {
  
  display.clear();

  if (debug) // Goes through all LED digital pins and ensures they're working
  {
    for (int i : ledPins)
    {
      digitalWrite(i , HIGH);
      delay(lightTimer);
    }
  }
  else
  {
    tcaselect(sensorChannel);

    while ((sensDist != 8190 && sensDist > thresholdNum))
    {
      int sensDist = sensorOne.readRange();

      // Debug Prints
      // Serial.print("Sensor Distance is: "); Serial.println(sensDist);
      delay(1000);

      if (sensDist < thresholdNum)
      {
        Serial.println("Threshold Reached! Swapping Signals!");
        sensDist = 10000;

        secondSignal();
      }


    }
  }
}
