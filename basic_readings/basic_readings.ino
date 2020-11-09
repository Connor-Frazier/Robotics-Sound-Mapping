#include "MeAuriga.h"
#include <SoftwareSerial.h>
#include <Wire.h>

MeLightSensor lightSensor(2);
MeSoundSensor mySound(5);
MeUltrasonicSensor ultraSensor(9);
MeLineFollower lineFinder(10);

int light = 0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{

  String temp = Serial.readString();

  if (temp.equals("Go")) {
    // Line state is 3 when both sensors are over a line
    // 2 if sensor 2 is over one, 1 if sesnor 1 is over one
    int lineState = lineFinder.readSensors();
    if (lineState == 3) {
      Serial.println("Sensor 1 and 2 on line");
    } else {
      Serial.print("Line state ");
      Serial.println(lineState);
    }
  
    // Distance to closest object or 400cm if nothing in range
    Serial.print("Ultrasonic sensor distance: ");
    Serial.print(ultraSensor.distanceCm());
    Serial.println(" cm");
  
    // Sound reading catches ambient sound
    Serial.print("Sound reading: ");
    Serial.println(mySound.strength());
  
    // Light reading from the top of the arduino
    Serial.print("Light reading: ");
    light = lightSensor.read();
    Serial.println(light);
  
  
    Serial.println("=============================");   
  }
 
}
