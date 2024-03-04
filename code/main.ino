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

}

void loop(){

getSensorDistance();

}