#include <Adafruit_NeoPixel.h>
#include "variables.h"
#include "functions.h"

void setup(){

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buttonOnePin, INPUT);
  pinMode(turnLeftPin, INPUT);
  pinMode(turnRightPin, INPUT);
  pinMode(rightForwardPin, INPUT);
  pinMode(leftForwardPin, INPUT);

  pinMode(12, INPUT);
  pinMode(13, INPUT);
  pinMode(14, INPUT);
  pinMode(15, INPUT);
  pinMode(16, INPUT);
  pinMode(17, INPUT);
  pinMode(18, INPUT);
  pinMode(19, INPUT);

  pinMode(neoPixelPin, INPUT);

}

void loop(){

  start();
  if(hasStarted){
   // getSensorDistance(); do not re comment until the timer has been fixed
    getLineSensorValues();
    mainControl();
  }
}