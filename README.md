# Traffic System Control Project

The purpose of this project is/was to replicate a traditional traffic control system on a smaller scale using distance sensors and a microcontroller. The general logic is that the signals will assume a `default` light status and (a) sensor(s) dictate which light should change and when.

## Material List:
- Arduino UNO R3
- 2x Adafruit VL53L0X Time of Flight Sensors (ToFS)
- 1x Adafruit TCA9548A I2C Multiplexer (TI2C)
- Standard Breadboard
- 2x 220 OHM Resistor
- 2x Traffic Light LED Modules (TLED)
- 
## Resources Used

1. https://docs.arduino.cc/language-reference/en/functions/communication/wire/
2. https://docs.arduino.cc/learn/communication/wire/
3. https://learn.adafruit.com/adafruit-tca9548a-1-to-8-i2c-multiplexer-breakout/arduino-wiring-and-test
4. https://learn.adafruit.com/adafruit-vl53l0x-micro-lidar-distance-sensor-breakout/arduino-code
   4a. https://docs.arduino.cc/libraries/vl53l0x/

## AI Disclosure

Due to my lack of experience in low-memory programming, Artificial Intelligence (AI) has been used regarding optimization of written code. All the code has been written by me, Jesus Cardenas. AI has been used a tool for consultation rather than creation.

## Other Notes

The VL53L0X libraries take up ~25% of dynamic memory per instance so memory efficiency could be further improved.
- Project is scalable in theory, but the above needs to be addressed first assuming the same ToFS is used.
