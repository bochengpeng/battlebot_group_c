#include <Adafruit_NeoPixel.h>
#include "variables.h"
#include "functions.h"

void setup(){

  //ultrasound sensor pins
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //motion control pins
  pinMode(turnLeftPin, INPUT);
  pinMode(turnRightPin, INPUT);
  pinMode(rightForwardPin, INPUT);
  pinMode(leftForwardPin, INPUT);

  //lineSensor pins
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  pinMode(14, INPUT);
  pinMode(15, INPUT);
  pinMode(16, INPUT);
  pinMode(17, INPUT);
  pinMode(18, INPUT);
  pinMode(19, INPUT);

  //other
  pinMode(neoPixelPin, INPUT);
  pinMode(buttonOnePin, INPUT);

  LEDs.begin();

}

void loop(){

  start();
  if(hasStarted){
   // getSensorDistance(); do not re comment until the timer has been fixed
    getLineSensorValues();
   mainControl();
  }
}