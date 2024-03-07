
void getSensorDistance(){
	digitalWrite(triggerPin, LOW);  //turn off trigger
	//delay(20); change this to millis  
	digitalWrite(triggerPin, HIGH); //turn on trigger 
	//delay(100); also this
	digitalWrite(triggerPin, LOW); //turn off trigger  
	sensorTime = pulseIn(echoPin, HIGH);  //get time
  sensorDistance = (sensorTime*.0343)/2; //calculate distance
}

void getLineSensorValues(){

	if(timer <= millis()){
		for(int j = 0; j < sizeof(lineSensorPins); j++){
			lineSensorValues[j] = analogRead(lineSensorPins[j]);
		}
		timer = 100 + millis();
 	}
}

void mainControl(){

	if(lineSensorValues[0] > 900){turnLeft(45);}
	else if(lineSensorValues[1] > 900){turnLeft(30);}
	else if(lineSensorValues[2] > 900){turnLeft(15);}
	else if(lineSensorValues[3] > 900){goForward();}
	else if(lineSensorValues[4] > 900){goForward();}
	else if(lineSensorValues[5] > 900){turnRight(15);}
	else if(lineSensorValues[6] > 900){turnRight(30);}
	else if(lineSensorValues[7] > 900){turnRight(45);}
  else {stop();}
}

void goForward(){

  if(millis() - previousTime >= interval){
	neoPixelControl(1);
	analogWrite(leftForwardPin, leftForwardSpeed);
	analogWrite(rightForwardPin, rightForwardSpeed);
	digitalWrite(turnLeftPin, 0);
	digitalWrite(turnRightPin, 0);
  }
  previousTime = millis();
}

void turnRight(int turnAmount){

  if(millis() - previousTime >= interval){
	neoPixelControl(2);
	analogWrite(leftForwardPin, 200);
	analogWrite(rightForwardPin, 122);
	analogWrite(turnRightPin, turnAmount);
	analogWrite(turnRightPin, 0);
  }
  previousTime = millis();
}

void turnLeft(int turnAmount){
	
  if(millis() - previousTime >= interval){
	neoPixelControl(2);
	analogWrite(rightForwardPin, 195);
	analogWrite(leftForwardPin, 127);
	analogWrite(turnLeftPin, turnAmount);
	analogWrite(turnLeftPin, 0);
  }
  previousTime = millis();
}

void stop(){
  neoPixelControl(0);
  analogWrite(rightForwardPin, 0);
  analogWrite(leftForwardPin, 0);
	analogWrite(turnLeftPin, 0);
	analogWrite(turnRightPin, 0);
}

void start(){

	buttonOneState = digitalRead(buttonOnePin);
	if(buttonOneState == LOW){hasStarted = true;}

}

void neoPixelControl(int state){

	Adafruit_NeoPixel LEDs(4, neoPixelPin, NEO_RGB + NEO_KHZ800);
  LEDs.clear();
  LEDs.begin();
	switch (state)
	{
	case 1:
    for (int i = 0; i < 4; i++) {
    	LEDs.setPixelColor(i, 28, 252, 3);
    }
    LEDs.show();
		break;
	
	case 2: 
    for (int i = 0; i < 4; i++) {
    	LEDs.setPixelColor(i, 252, 219 ,3);
    }
    LEDs.show();
		break;

	default:
    for (int i = 0; i < 4; i++) {
    	LEDs.setPixelColor(i, 252, 3, 3);
    }
    LEDs.show();
		break;
	}
}