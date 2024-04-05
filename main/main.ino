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
const int rightReversePin = 13;
const int leftReversePin = 12;

//ultrasonic sensor
float sensorDistance = 0;
float sensorTime = 0;

//timing
unsigned long lineTimer = 0;
unsigned long previousTime = 0;
unsigned long endTimer = 0;
unsigned long previousSonicTime = 0;
int interval = 50; //how often the movement controls should be executed

//lineSensor
int lineSensorValues[8];

//start and end
bool hasStarted = false;
bool hasEnded = false;
bool theEnd = false;

//NeoPixel
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

  //other
  pinMode(neoPixelPin, INPUT);
  pinMode(gripperPin, OUTPUT);

  //inital setup
  putDownObject();
  LEDs.begin();
  beforeStartNeoPixel();
  
}

void loop(){

  getSensorDistance();
  start();
  end();
  if(hasStarted == true)
  {
    avoidObject();
    getLineSensorValues();
    mainControl();
  }
  
}

/* 
   _____                               
  / ____|                              
 | (___   ___ _ __  ___  ___  _ __ ___ 
  \___ \ / _ \ '_ \/ __|/ _ \| '__/ __|
  ____) |  __/ | | \__ \ (_) | |  \__ \
 |_____/ \___|_| |_|___/\___/|_|  |___/

*/ 


void getSensorDistance()
{
  if(hasEnded == false)
  {
    if(millis() - previousSonicTime >= 250) //runs every 250 milliseconds
    {
      sensorDistance = 0;
      digitalWrite(triggerPin, LOW); //make sure the trigger is turned off
      delayMicroseconds(3);
      digitalWrite(triggerPin, HIGH); //turn on trigger
      delayMicroseconds(9);
      digitalWrite(triggerPin, LOW); //turn off trigger
      sensorTime = pulseIn(echoPin, HIGH);  //get time
      sensorDistance = (sensorTime*.0343)/2; //calculate distance, 0.343 is the speed of sound, which is divided by 2
      Serial.println(sensorDistance);
      previousSonicTime = millis();
    }
  }
}

void getLineSensorValues()
{

	if(millis() - lineTimer >= 40) //runs every 40 milliseconds
  {
		for(int j = 0; j < 8; j++)
    {
			lineSensorValues[j] = analogRead(lineSensorPins[j]);
		}
		lineTimer = millis();
	}
}

/*
   _____      _                       
  / ____|    (_)                      
 | |  __ _ __ _ _ __  _ __   ___ _ __ 
 | | |_ | '__| | '_ \| '_ \ / _ \ '__|
 | |__| | |  | | |_) | |_) |  __/ |   
  \_____|_|  |_| .__/| .__/ \___|_|   
               | |   | |              
               |_|   |_|              
*/


void pickUpObject()
{
  for(int i = 0; i < 15; i++) //we have to execute this 15 times to guarantee reliability
  {
    digitalWrite(gripperPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(gripperPin, LOW);
  }
}

void putDownObject()
{
  for(int i = 0; i < 15; i++) //same here
  {
   digitalWrite(gripperPin, HIGH);
   delayMicroseconds(1700);
   digitalWrite(gripperPin, LOW);
  }
}

/*
  __  __       _   _                               _             _     
 |  \/  |     | | (_)                             | |           | |    
 | \  / | ___ | |_ _  ___  _ __     ___ ___  _ __ | |_ _ __ ___ | |___ 
 | |\/| |/ _ \| __| |/ _ \| '_ \   / __/ _ \| '_ \| __| '__/ _ \| / __|
 | |  | | (_) | |_| | (_) | | | | | (_| (_) | | | | |_| | | (_) | \__ \
 |_|  |_|\___/ \__|_|\___/|_| |_|  \___\___/|_| |_|\__|_|  \___/|_|___/
                                                                                                                                              
*/


void mainControl(){

  //main control logic based on the lineSensorValues

  if(lineSensorValues[3] > 900){goForward();}
	else if(lineSensorValues[4] > 900){goForward();}
	else if(lineSensorValues[0] > 900){turnLeft(50);}
	else if(lineSensorValues[1] > 900){turnLeft(40);}
	else if(lineSensorValues[2] > 900){turnLeft(30);}
	else if(lineSensorValues[5] > 900){turnRight(30);}
	else if(lineSensorValues[6] > 900){turnRight(40);}
	else if(lineSensorValues[7] > 900){turnRight(50);}
  
}

void goForward(){
  if(millis() - previousTime >= interval)
  {
    goForwardNeoPixel();
    analogWrite(leftForwardPin, 224);
    analogWrite(rightForwardPin, 224);
    digitalWrite(turnLeftPin, 0);
    digitalWrite(turnRightPin, 0);
    previousTime = millis();
  }
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
    previousTime = millis();
  }
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
    previousTime = millis();
  }
}

void stop()
{
  analogWrite(rightForwardPin, 0);
  analogWrite(leftForwardPin, 0);
	analogWrite(turnLeftPin, 0);
	analogWrite(turnRightPin, 0);
  analogWrite(rightReversePin, 0);
  analogWrite(leftReversePin, 0);
}

void reverse()
{
  digitalWrite(rightReversePin, HIGH);
  digitalWrite(leftReversePin, HIGH);
	analogWrite(turnLeftPin, 0);
	analogWrite(turnRightPin, 0);
}

void avoidObject()
{
  getSensorDistance();
  if(sensorDistance <= 17)
  {
    //turn left 90 degrees ->
    stop();
    delay(200);
    turnLeft(255);
    delay(700); //we can use delays here since we're taking "manual" control and there is no line

    //go forward a bit |
    goForward();
    delay(250);

    //turn right <-
    turnRight(255);
    delay(700);

    //go forward |
    goForward();
    delay(500);

    //turn right ->
    turnRight(255);
    delay(300);

    //go forward | and pray we find the line
    goForward();
    delay(300);

  }
}

/*
   _____ _             _                     _   ______           _ 
  / ____| |           | |                   | | |  ____|         | |
 | (___ | |_ __ _ _ __| |_    __ _ _ __   __| | | |__   _ __   __| |
  \___ \| __/ _` | '__| __|  / _` | '_ \ / _` | |  __| | '_ \ / _` |
  ____) | || (_| | |  | |_  | (_| | | | | (_| | | |____| | | | (_| |
 |_____/ \__\__,_|_|   \__|  \__,_|_| |_|\__,_| |______|_| |_|\__,_|
                                                                                                                                        
*/

void start()
{

  if(hasEnded == false && sensorDistance > 20 && hasStarted == false)
  {
    //inital starting sequence, we can take manual control of the bot here
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

  /* the main logic here is that if we see a potential black box we set
     theEnd to true, get the line sensor values, BUT we don't end the race just yet,
     after 200 milliseconds we check if the potential black box is still there, if not then set
     theEnd to be false, however if it is still there then we start the ending sequence
  */

  if(lineSensorValues[0] > 900 &&
     lineSensorValues[1] > 900 && 
     lineSensorValues[2] > 900 &&
     lineSensorValues[3] > 900 &&
     lineSensorValues[4] > 900 &&
     lineSensorValues[5] > 900 &&
     lineSensorValues[6] > 900 &&
     lineSensorValues[7] > 900 && 
     hasEnded == false)
  {
    getLineSensorValues();
    if(millis() - endTimer >= 200)
    {
      if(lineSensorValues[0] > 900 &&
         lineSensorValues[1] > 900 && 
         lineSensorValues[2] > 900 &&
         lineSensorValues[3] > 900 &&
         lineSensorValues[4] > 900 &&
         lineSensorValues[5] > 900 &&
         lineSensorValues[6] > 900 &&
         lineSensorValues[7] > 900 && 
         theEnd == true)
      {
        endNeoPixel();
        hasEnded = true;
        hasStarted = false;
        stop();
        putDownObject();
        delay(500);
        reverse();
        delay(1000);
        stop();
      }
      else
      {
        theEnd = false;
      }
      endTimer = millis();
    }
    theEnd = true;
  }
  else
  {
    theEnd = false;
  }
}

/*
  _   _            _____ _          _     
 | \ | |          |  __ (_)        | |    
 |  \| | ___  ___ | |__) |__  _____| |___ 
 | . ` |/ _ \/ _ \|  ___/ \ \/ / _ \ / __|
 | |\  |  __/ (_) | |   | |>  <  __/ \__ \
 |_| \_|\___|\___/|_|   |_/_/\_\___|_|___/
                                                                                    
*/

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