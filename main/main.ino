//#include <Adafruit_NeoPixel.h>
//#include "variables.h"
//#include "functions.h"

//pins
const int buttonOnePin = 2;
//const int neoPixelPin = 3;
const int rightForwardPin = 5; 
const int leftForwardPin = 6; 
const int triggerPin = 8;
const int echoPin = 9;
const int turnLeftPin = 10; 
const int turnRightPin = 11;
const int lineSensorPins[] = {A0, A1, A2, A3, A4, A5, A6, A7}; 
const int gripperPin = 3;

//ultrasonic sensor
float sensorDistance = 0;
float sensorTime = 0;

//timing
unsigned long lineTimer = 0;
unsigned long previousTime = 0;
int interval = 75;
unsigned long previousSonicTime = 0;

//motor values
const int leftForwardSpeed = 255;
const int rightForwardSpeed = 250;

//lineSensor
int lineSensorValues[8];
int count = 0;

//start and end
bool hasStarted = false;
bool hasEnded = false;

//Adafruit_NeoPixel LEDs(4, neoPixelPin, NEO_RGB + NEO_KHZ800);


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
  Serial.begin(9600);

  //other
  //pinMode(neoPixelPin, INPUT);
  //LEDs.begin();
  pinMode(gripperPin, OUTPUT);
  
}

void loop(){

  getSensorDistance();
  start();
  end();
  if(hasStarted){
    //avoidObject();
    getLineSensorValues();
    mainControl();
  }
}
  /*
  TODO: (order from most important to least)

  create a fucnction to detect the end of the track and shut down
  make the sonic sensor better
  figue out if we can use multiple files
  Figure out if NeoPixels can be fixed

  */

void getSensorDistance(){
  //Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(triggerPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  sensorTime = pulseIn(echoPin, HIGH);
  // Calculating the distance
  sensorDistance = sensorTime * 0.034 / 2;
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
}


void pickUpObject(int angle) {
  if(hasStarted == true)
  {
    digitalWrite(gripperPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(gripperPin, LOW);
  }
}

void putDownObject(){
  if(hasEnded == true){
   digitalWrite(gripperPin, HIGH);
   delayMicroseconds(2000);
   digitalWrite(gripperPin, LOW);
  }
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

void start(){

  if(hasEnded == false && sensorDistance > 20 && hasStarted == false){
    hasStarted = true;
    goForward();
    delay(700);
    pickUpObject(90);
    delay(300);
    turnLeft(255);
    delay(700);
    goForward();
  } 
}

void end(){

  count = 0;
  for (int i = 0; i < 8; i++) {
    if(lineSensorValues[i] > 900)
    {
      count++;
    }
  }

  if(count == 8)
  {
    hasEnded = true;
    hasStarted = false;
    stop();
    putDownObject();
  }
}

void avoidObject(){

  if(sensorDistance < 25){

    //turn right 90 degrees ->
    turnRight(255);
    delay(700); //we can use delays here since we're taking "manual" control and there is no line

    //go forward a bit |
    goForward();
    delay(500);

    //turn left <-
    turnLeft(255);
    delay(700);

    //go forward |
    goForward();
    delay(1000);

    //turn left <-
    turnLeft(255);
    delay(300);

    //go forward | and pray we find the line
    goForward();
  }
}

/*
void neoPixelControl(int state){


  if(state == 1){
    for (int i = 0; i < 4; i++) {
    	LEDs.setPixelColor(i, 28, 252, 3);
    }
  }

  if(state == 2)
    for (int i = 0; i < 4; i++) {
    	LEDs.setPixelColor(i, 252, 219 ,3);
    }
  }

  if(state == 0)
    for (int i = 0; i < 4; i++) {
    	LEDs.setPixelColor(i, 252, 3, 3);
    }
	
  LEDs.show();
}
*/