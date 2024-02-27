#include "variables.h"
#include "functions.h"
#include <Adafruit_NeoPixel.h>

void getSensorDistance(){

digitalWrite(triggerPin, LOW);  //turn off trigger
	delay(20);  //wait
	digitalWrite(trigPin, HIGH); //turn on trigger 
	delay(100) ; //wait
	digitalWrite(trigPin, LOW); //turn off trigger  
  duration = pulseIn(echoPin, HIGH);  //get time
  distance = (duration*.0343)/2; //calculate distance
}
