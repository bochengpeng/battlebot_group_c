#include "variables.h"
#include "functions.h"
#include <Adafruit_NeoPixel.h>

void getSensorDistance(){

digitalWrite(triggerPin, LOW);  //turn off trigger
	delay(20);  //wait
	digitalWrite(triggerPin, HIGH); //turn on trigger 
	delay(100) ; //wait
	digitalWrite(triggerPin, LOW); //turn off trigger  
	sensorTime = pulseIn(echoPin, HIGH);  //get time
  	sensorDistance = (sensorTime*.0343)/2; //calculate distance
}
