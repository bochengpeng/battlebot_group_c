
void getSensorDistance(){

    digitalWrite(triggerPin, HIGH); //turn on trigger
    if(millis() - previousSonicTime >= 100){
      digitalWrite(triggerPin, LOW); //turn off trigger
    }
    previousSonicTime = millis();
    sensorTime = pulseIn(echoPin, HIGH);  //get time
    if(sensorTime > 0.00) {sensorDistance = (sensorTime*.0343)/2;} //calculate distance
}

void getLineSensorValues(){

	if(lineTimer <= millis()){
		for(int j = 0; j < sizeof(lineSensorPins); j++){
			lineSensorValues[j] = analogRead(lineSensorPins[j]);
		}
		lineTimer = 50 + millis();
 	}
}

void mainControl(){

  if(lineSensorValues[3] > 900){goForward();}
	else if(lineSensorValues[4] > 900){goForward();}
	else if(lineSensorValues[0] > 900){turnLeft(90);}
	else if(lineSensorValues[1] > 900){turnLeft(60);}
	else if(lineSensorValues[2] > 900){turnLeft(30);}
	else if(lineSensorValues[5] > 900){turnRight(30);}
	else if(lineSensorValues[6] > 900){turnRight(60);}
	else if(lineSensorValues[7] > 900){turnRight(90);}
  //else {stop();}
}

void goForward(){

  if(millis() - previousTime >= interval){
    //neoPixelControl(1);
    analogWrite(leftForwardPin, 224);
    analogWrite(rightForwardPin, 224);
    digitalWrite(turnLeftPin, 0);
    digitalWrite(turnRightPin, 0);
  }
  previousTime = millis();
}

void turnRight(int turnAmount){

  if(millis() - previousTime >= interval){
    //neoPixelControl(2);
    analogWrite(leftForwardPin, 255);
    analogWrite(rightForwardPin, 0);
    analogWrite(turnRightPin, turnAmount);
    analogWrite(turnLeftPin, 0);
  }
  previousTime = millis();
}

void turnLeft(int turnAmount){
	
  if(millis() - previousTime >= interval){
    //neoPixelControl(2);
    analogWrite(rightForwardPin, 255);
    analogWrite(leftForwardPin, 0);
    analogWrite(turnLeftPin, turnAmount);
    analogWrite(turnRightPin, 0);
  }
  previousTime = millis();
}

void stop(){
  //neoPixelControl(0);
  analogWrite(rightForwardPin, 0);
  analogWrite(leftForwardPin, 0);
	analogWrite(turnLeftPin, 0);
	analogWrite(turnRightPin, 0);
}

bool start(){

  if(sensorDistance > 20 && (analogRead(leftForwardPin == 0 ) || analogRead(rightForwardPin == 0)))
  {
    return true;
  }
  else return false;
}

void neoPixelControl(int state){

	switch (state)
	{
	case 1:
    for (int i = 0; i < 4; i++) {
    	LEDs.setPixelColor(i, 28, 252, 3);
    }
		break;
	
	case 2: 
    for (int i = 0; i < 4; i++) {
    	LEDs.setPixelColor(i, 252, 219 ,3);
    }
		break;

	default:
    for (int i = 0; i < 4; i++) {
    	LEDs.setPixelColor(i, 252, 3, 3);
    }
		break;
	}
  LEDs.show();
}