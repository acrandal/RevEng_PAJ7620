/*
  Example Script: paj7620_proximity.ino
  Package: RevEng_PAJ7620

  Author       : Tianrui Bai <tbai9@wisc.edu>
  Modified Time: April 2025

  Description: This demo put PAJ7620 sensor into proximity mode and read object status and proximity value from sensor.
  getProximityDistance will return 0, 32~255 as value. 
  Sensor will return 32 when object as far as sensor can detect,
  and it will return 255 when object as closest distance it can detect, although object is 5cm away from sensor.

  License: Same as package under MIT License (MIT)
*/

// Includes enum definition of GES_* return values from readGesture()
#include "RevEng_PAJ7620.h"

// Create gesture sensor driver object
RevEng_PAJ7620 sensor = RevEng_PAJ7620();


// ******************************************************************
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);     // Configure LED for output

  Serial.begin(115200);
  if( !sensor.begin() )             // return value of 1 == success
  {
    Serial.print("PAJ7620 init error -- device not found -- halting");
    while(true) {}
  }

  sensor.setProximityMode();

  Serial.println("PAJ7620U2 init: OK.");
  Serial.println("Place an object in front of the sensor to see it's IR brightness (0..255) and size (0..900)");
  delay(2000);
}


// ******************************************************************
void loop()
{
  Serial.println("**************************************");
  bool approach = false;
  int distance = 0;
  approach = sensor.getProximityApproach(); //Detect any object in view
  distance = sensor.getProximityDistance(); //Read object distance from sensor
  Serial.print("Object Status: ");
  Serial.println(approach ? "Detected Object" : "No Object");
  Serial.print(" Object Distance: ");
  Serial.println(distance);

  delay(100);
}
