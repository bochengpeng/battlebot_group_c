//#include <Adafruit_NeoPixel.h>
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
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);

  //other
  pinMode(neoPixelPin, INPUT);
  pinMode(buttonOnePin, INPUT);

  //LEDs.begin();
  Serial.begin(9600);

}

void loop(){

  getSensorDistance();
  if(start()){
    avoidObject();
    getLineSensorValues();
    mainControl();
  }

  /*
  TODO: (order from most important to least)

  calibrate the avoid object function
  program the gripper
  create a fucnction to detect the end of the track and shut down
  make the sonic sensor better
  Figure out if NeoPixels can be fixed

  */

}