#include <Adafruit_NeoPixel.h>


//pins
const int neoPixelPin = 3;
const int rightForwardPin = 5; 
const int leftForwardPin = 6; 
const int triggerPin = 8;
const int echoPin = 9;
const int turnLeftPin = 10; 
const int turnRightPin = 11;
const int lineSensorPins[] = {A0, A1, A2, A3, A4, A5, A6, A7}; 
const int gripperPin = 4;

//ultrasonic sensor
float sensorDistance = 0;
float sensorTime = 0;

//timing
unsigned long lineTimer = 0;
unsigned long previousTime = 0;
unsigned long endTimer = 0;
unsigned long avoidTimer = 0;
int interval = 10;
unsigned long neoPixelTimer = 0;

//lineSensor
int lineSensorValues[8];

//start and end
bool hasStarted = false;
bool hasEnded = false;

Adafruit_NeoPixel LEDs(4, neoPixelPin, NEO_RGB + NEO_KHZ800);


void setup(){
  

  //ultrasonic sensor pins
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
  pinMode(neoPixelPin, INPUT);
  pinMode(gripperPin, OUTPUT);
  digitalWrite(gripperPin, LOW);
  putDownObject();
  LEDs.begin();
  beforeStartNeoPixel();
  
}

void loop(){

  getSensorDistance();
  start();
  end();
  if(hasStarted)
  {
    avoidObject();
    getLineSensorValues();
    mainControl();
  }
}

//TODO:
//add comments
//make neopixel cooler

void getSensorDistance()
{
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(triggerPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  sensorTime = pulseIn(echoPin, HIGH);
  // Calculating the distance
  sensorDistance = sensorTime * 0.034 / 2;
}

void getLineSensorValues()
{

	if(lineTimer <= millis())
  {
		for(int j = 0; j < sizeof(lineSensorPins); j++)
    {
			lineSensorValues[j] = analogRead(lineSensorPins[j]);
      
		}
		lineTimer = 5 + millis();
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


void pickUpObject()
{
  for(int i = 0; i < 15; i++)
  {
    digitalWrite(gripperPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(gripperPin, LOW);
  }
}

void putDownObject()
{
  for(int i = 0; i < 15; i++)
  {
   digitalWrite(gripperPin, HIGH);
   delayMicroseconds(1700);
   digitalWrite(gripperPin, LOW);
  }
}

void goForward(){
  if(millis() - previousTime >= interval)
  {
    goForwardNeoPixel();
    analogWrite(leftForwardPin, 224);
    analogWrite(rightForwardPin, 224);
    digitalWrite(turnLeftPin, 0);
    digitalWrite(turnRightPin, 0);
  }
  previousTime = millis();
}

void turnRight(int turnAmount)
{

  if(millis() - previousTime >= interval)
  {
    turnRightNeoPixel();
    analogWrite(leftForwardPin, 255);
    analogWrite(rightForwardPin, 0);
    analogWrite(turnRightPin, turnAmount);
    analogWrite(turnLeftPin, 0);
  }
  previousTime = millis();
}

void turnLeft(int turnAmount)
{
	
  if(millis() - previousTime >= interval)
  {
    turnLeftNeoPixel();
    analogWrite(rightForwardPin, 255);
    analogWrite(leftForwardPin, 0);
    analogWrite(turnLeftPin, turnAmount);
    analogWrite(turnRightPin, 0);
  }
  previousTime = millis();
}

void stop()
{
  analogWrite(rightForwardPin, 0);
  analogWrite(leftForwardPin, 0);
	analogWrite(turnLeftPin, 0);
	analogWrite(turnRightPin, 0);
}

void start()
{

  if(hasEnded == false && sensorDistance > 20 && hasStarted == false)
  {
    startNeoPixel();
    hasStarted = true;
    goForward();
    delay(1000);
    pickUpObject();
    turnLeft(255);
    delay(700);
    goForward();
  } 
}

void end()
{

  if(lineSensorValues[0] > 900 && lineSensorValues[3] > 900 && lineSensorValues[7] > 900)
  {
    getLineSensorValues();
    if(millis() - endTimer >= 300)
    {
      getLineSensorValues();
      if(lineSensorValues[0] > 900 && lineSensorValues[3] > 900 && lineSensorValues[7] > 900)
      {
        endNeoPixel();
        hasEnded = true;
        hasStarted = false;
        stop();
        putDownObject();
      } 
    }
    endTimer = millis();
  }
}

void avoidObject()
{

  getSensorDistance();
  if(sensorDistance <= 28)
  {

    delay(500);
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
    delay(200);

  mainControl();
  }
}

void beforeStartNeoPixel()
{

  LEDs.setPixelColor(0, 250, 5, 5);
  LEDs.setPixelColor(1, 250, 5, 5);
  LEDs.setPixelColor(2, 250, 5, 5);
  LEDs.setPixelColor(3, 250, 5, 5);
  LEDs.show();
 
}

void startNeoPixel()
{

  LEDs.setPixelColor(0, 250, 251, 252);
  LEDs.setPixelColor(1, 250, 251, 252);
  LEDs.setPixelColor(2, 255, 3, 196);
  LEDs.setPixelColor(3, 255, 3, 196);
  LEDs.show();

}

void turnRightNeoPixel()
{

  LEDs.setPixelColor(0, 250, 251, 252);
  LEDs.setPixelColor(1, 255, 3, 196);
  LEDs.setPixelColor(2, 255, 3, 196);
  LEDs.setPixelColor(3, 250, 251, 252);
  LEDs.show();

}

void turnLeftNeoPixel()
{

  LEDs.setPixelColor(0, 255, 3, 196);
  LEDs.setPixelColor(1, 250, 251, 252);
  LEDs.setPixelColor(2, 250, 251, 252);
  LEDs.setPixelColor(3, 255, 3, 196);
  LEDs.show();

}

void goForwardNeoPixel()
{
  //chnage this to be cooler
  LEDs.setPixelColor(0, 250, 251, 252);
  LEDs.setPixelColor(1, 250, 251, 252);
  LEDs.setPixelColor(2, 255, 3, 196);
  LEDs.setPixelColor(3, 255, 3, 196);
  LEDs.show();

}

void endNeoPixel()
{

  LEDs.setPixelColor(0, 255, 3, 196);
  LEDs.setPixelColor(1, 255, 3, 196);
  LEDs.setPixelColor(2, 255, 3, 196);
  LEDs.setPixelColor(3, 255, 3, 196);
  LEDs.show();

}