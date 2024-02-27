#include "variables.h"
#include "functions.h"


void setup(){
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buttonOnePin, INPUT);
  pinMode(TLPin, INPUT);
  pinMode(TRPin, INPUT);
  pinMode(RWFPin, INPUT);
  pinMode(LWFPin, INPUT);

}

void loop(){

getSensorDistance();

}