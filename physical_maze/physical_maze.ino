//       \_\
//      (_)
//     _) #
//    ( )...()
//    || | |I|
//    || | |()/ ESTRIPPER
//    /\(_)
//   -"""""""-""-_
//   -,,,,,,,,- ,,-MJP
#include <Adafruit_NeoPixel.h>
#include "CuteBuzzerSounds.h"

//Motor pins
#define MOTOR_B1 10 //left backward pin
#define MOTOR_B2 9 //left forward pin
#define MOTOR_A1 6 //right forward pin
#define MOTOR_A2 5 //right backward pin
#define MOTOR_R1 2 //right encoder
#define MOTOR_R2 3 //left encoder

//Motor speeds
#define LEFT_MOTOR_SPEED 255
#define RIGHT_MOTOR_SPEED 255

//Wheel rotation counters
volatile int countR = 0;
volatile int countL = 0;

//Servo pins
#define ULTRA_SERVO 11 //servo for the ultrasonic sensor
#define GRIPPER_SERVO 4 //servo for the gripper

//Ultrasonic sensor pins
#define TRIG_PIN_FRONT 12
#define ECHO_PIN_FRONT 13
#define TRIG_PIN_SIDE 7
#define ECHO_PIN_SIDE 8

//Variables for distance calculation on ultrasonic sensors
long durationFront;
int distanceCmFront;
long durationSide;
int distanceCmSide;

//Variables for sequences
bool startSequence = true;
bool mazeSequence = false;
bool endSequence = false;

//NeoPixel 
#define PIN 4
#define NUMPIXELS 4
#define BRIGHTNES_LEVEL 20
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//Line sensor
const int sensorPins[] = {A1, A2, A3, A4, A5, A6};
int sensor_A1, sensor_A2, sensor_A3, sensor_A4, sensor_A5, sensor_A6;
int BlackLimit;

//Buzzer pin
#define BUZZER 4

int calibrationCheck = 0;
void setup() {
  pixels.begin();

  pinMode(MOTOR_B1, OUTPUT);
  pinMode(MOTOR_B2, OUTPUT);
  pinMode(MOTOR_A1, OUTPUT);
  pinMode(MOTOR_A2, OUTPUT);

  pinMode(TRIG_PIN_FRONT, OUTPUT);
  pinMode(ECHO_PIN_FRONT, INPUT);
  pinMode(TRIG_PIN_SIDE, OUTPUT);
  pinMode(ECHO_PIN_SIDE, INPUT);

  pinMode(MOTOR_R1, INPUT_PULLUP);//rotation sensor
  pinMode(MOTOR_R2, INPUT_PULLUP);//rotation sensor
  attachInterrupt(digitalPinToInterrupt(MOTOR_R1), updateRightRotations, CHANGE);
  attachInterrupt(digitalPinToInterrupt(MOTOR_R2), updateLeftRotations, CHANGE);

  pinMode(ULTRA_SERVO , OUTPUT);

  for (int i = 0; i < 6; i++) {
    pinMode(sensorPins[i], INPUT);
  }
  
  Serial.begin(9600);

}

void loop() {
  
  distanceCmFront = getUltrasonicCm(TRIG_PIN_FRONT , ECHO_PIN_FRONT);
  if(startSequence)
  {
   distanceCmFront = getUltrasonicCm(TRIG_PIN_FRONT , ECHO_PIN_FRONT);
   delay(50);
   Serial.println(distanceCmFront);
   if(distanceCmFront < 25)
   {
    startSequenceCode();
   }
  }
  else if(mazeSequence)
  {
    mazeSequenceCode();
  }
  else stopRobot();
}

void updateRightRotations(){
  noInterrupts();
  countR++;
  interrupts();
}


void updateLeftRotations(){
  noInterrupts();
  countL++;
  interrupts();
}

void resetRotations(){
  countR = 0;
  countL = 0;
}

void startSequenceCode()
{      

      rotateGripperServo(-45);
      delay(300);
      rotateGripperServo(45);
      delay(300);
      rotateGripperServo(-45);
      delay(300);
      rotateGripperServo(45);
      delay(300);
      
      //Calibration for black limit
      int blackLimit[3];
      int currentIndex = 0;
      int color = getAverageLightLevel();
      int lineCounter = 0;
    
      while(lineCounter < 4) {
        stopRobot();
        delay(100);
        int curentColor = getAverageLightLevel();
        moveForward(1);
        if (curentColor < color - 300) {
          stopRobot();
          color = getAverageLightLevel();
          delay(100);
        }
        else if (curentColor > color + 300) {
          stopRobot();
          color = getAverageLightLevel();
          delay(100);
          blackLimit[currentIndex] = curentColor;
          currentIndex++;
          lineCounter++;
        }
      }
      BlackLimit = getAverageBlackLimit(blackLimit);
      
      stopRobot();
      rotateGripperServo(-45);
      moveForward(10);
      stopRobot();
      turnLeft(39);
      stopRobot();
      moveForward(40);
      stopRobot();
  
      while(sensor_A1 > BlackLimit || sensor_A2 > BlackLimit || sensor_A3 > BlackLimit || sensor_A4 > BlackLimit || sensor_A5 > BlackLimit || sensor_A6 > BlackLimit)
      {
        read();
        if(sensor_A1 > BlackLimit)
        {
          rotateRight(4);
          moveForward(1);
        }
        else if(sensor_A2 > BlackLimit)
        {
          rotateRight(2);
          moveForward(1);
        }
        else if(sensor_A6 > BlackLimit)
        {
          rotateLeft(4);
          moveForward(1);
        }
        else if(sensor_A5 > BlackLimit)
        {
          rotateLeft(2);
          moveForward(1);
        }
        else moveForward(1);
      }
      
      startSequence = false;
      mazeSequence = true;
}

void mazeSequenceCode()
{
  read();
  distanceCmFront = getUltrasonicCm(TRIG_PIN_FRONT , ECHO_PIN_FRONT);
  distanceCmSide = getUltrasonicCm(TRIG_PIN_SIDE , ECHO_PIN_SIDE);
  Serial.println(distanceCmSide);

  if(sensor_A1 > BlackLimit || sensor_A2 > BlackLimit || sensor_A3 > BlackLimit || sensor_A4 > BlackLimit || sensor_A5 > BlackLimit || sensor_A6 > BlackLimit)
  {
    mazeSequence = false;
    endSequence = true;
  }
  else if(distanceCmSide > 35)
  {
    stopRobot();
    rightTurnSequence();
  }
  else if(distanceCmFront < 12)
  {
    stopRobot();
    distanceCmFront = getUltrasonicCm(TRIG_PIN_FRONT , ECHO_PIN_FRONT);
    delay(50);
    
    if(distanceCmFront < 12)
    {
      rotateUltraServo(90);
      delay(100);
      distanceCmFront = getUltrasonicCm(TRIG_PIN_FRONT , ECHO_PIN_FRONT);
      distanceCmSide = getUltrasonicCm(TRIG_PIN_SIDE , ECHO_PIN_SIDE); 
      
      if(distanceCmFront > 30)
      {
        leftTurnSequence(); 
      }
      else
      { 
         rotateRight(25);
         stopRobot();
         moveBackward(10);
         stopRobot();
         turnRight(37);
         stopRobot();
         moveBackward(35);
         stopRobot();
      }    
      rotateUltraServo(0);
     }
     else moveForwardWithAdjusting();
  }
  else
  {
    moveForwardWithAdjusting();
  }
}

void endSequenceCode()
{ 
  if(sensor_A1 > BlackLimit && sensor_A2 > BlackLimit && sensor_A3 > BlackLimit && sensor_A4 > BlackLimit && sensor_A5 > BlackLimit && sensor_A6 > BlackLimit)
  {
     rotateGripperServo(45);
      delay(300);
     moveBackward(50);
     endSequence = false;
  }
  else
      {
        read();
        if(sensor_A1 > BlackLimit)
        {
          rotateRight(4);
          moveForward(1);
        }
        else if(sensor_A2 > BlackLimit)
        {
          rotateRight(2);
          moveForward(1);
        }
        else if(sensor_A6 > BlackLimit)
        {
          rotateLeft(4);
          moveForward(1);
        }
        else if(sensor_A5 > BlackLimit)
        {
          rotateLeft(2);
          moveForward(1);
        }
        else moveForward(1);
      }
}

int getUltrasonicCm(int trigPin , int echoPin)
{
  long duration;
  int distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.034) / 2;

  return distance;
}

//Right turn related functions
void turnRight(int r) {
  resetRotations();

  while(r > countL){
    if(countL <= 2)
    {
      analogWrite(MOTOR_B1, 0);
      analogWrite(MOTOR_B2, LEFT_MOTOR_SPEED- 25);
      analogWrite(MOTOR_A1, 0);
      analogWrite(MOTOR_A2, 0);
    }
    else{
      analogWrite(MOTOR_B1, 0);
      analogWrite(MOTOR_B2, LEFT_MOTOR_SPEED- 45);
      analogWrite(MOTOR_A1, 0);
      analogWrite(MOTOR_A2, 0); 
    }
   } 
}

void rightTurnSequence()
{
  setPixlsBlue();
  stopRobot();
  moveForward(15);
  stopRobot();
  turnRight(36);
  stopRobot();
  moveForward(10);
  stopRobot();
}

//Left turn related functions
void turnLeft(int r) {
  resetRotations();

  while(r > countR){
    Serial.println(countR);
    analogWrite(MOTOR_B1, 0);
    analogWrite(MOTOR_B2, 0);
    analogWrite(MOTOR_A1, RIGHT_MOTOR_SPEED - 10);
    analogWrite(MOTOR_A2, 0);
  }
}

void leftTurnSequence()
{
  rotateUltraServo(0);
  turnLeft(30);
  stopRobot();
  moveForward(10);
  stopRobot();
}

//Forward related functions
void moveForwardWithAdjusting()
{
      distanceCmSide = getUltrasonicCm(TRIG_PIN_SIDE , ECHO_PIN_SIDE);
      if(distanceCmSide > 10)
      {
        setPixlsRed();
        analogWrite(MOTOR_B1, 0);
        analogWrite(MOTOR_B2, LEFT_MOTOR_SPEED);
        analogWrite(MOTOR_A1, 120);
        analogWrite(MOTOR_A2, 0);
      }
      else if(distanceCmSide >= 9)
      {
        setPixlsRed();
        analogWrite(MOTOR_B1, 0);
        analogWrite(MOTOR_B2, LEFT_MOTOR_SPEED);
        analogWrite(MOTOR_A1, 180);
        analogWrite(MOTOR_A2, 0);
      }
      else if(distanceCmSide < 5)
      {
        setPixlsYellow();
        analogWrite(MOTOR_B1, 0);
        analogWrite(MOTOR_B2, 120);
        analogWrite(MOTOR_A1, RIGHT_MOTOR_SPEED);
        analogWrite(MOTOR_A2, 0);
      }
      else if(distanceCmSide <= 7){
        setPixlsYellow();
        analogWrite(MOTOR_B1, 0);
        analogWrite(MOTOR_B2, 180);
        analogWrite(MOTOR_A1, RIGHT_MOTOR_SPEED);
        analogWrite(MOTOR_A2, 0);
      }
      else
      {
        setPixlsBlue();
        analogWrite(MOTOR_B1, 0);
        analogWrite(MOTOR_B2, LEFT_MOTOR_SPEED - 20);
        analogWrite(MOTOR_A1, RIGHT_MOTOR_SPEED);
        analogWrite(MOTOR_A2, 0);
      }
}

void moveForward(int rot)
{
    resetRotations();
    while(countL < rot)
    {
       analogWrite(MOTOR_B1, 0);
       analogWrite(MOTOR_B2, LEFT_MOTOR_SPEED - 14);
       analogWrite(MOTOR_A1, RIGHT_MOTOR_SPEED);
       analogWrite(MOTOR_A2, 0);
    }
}

void moveForwardSlow(int rot)
{
        analogWrite(MOTOR_A1, RIGHT_MOTOR_SPEED - 30);
       analogWrite(MOTOR_A2, 0);
       analogWrite(MOTOR_B1, 0);
       analogWrite(MOTOR_B2, LEFT_MOTOR_SPEED - 50);
}

//Backward related functions
void moveBackward(int rot)
{
      resetRotations();
    while(countL < rot)
    {
       analogWrite(MOTOR_B1, LEFT_MOTOR_SPEED - 20);
       analogWrite(MOTOR_B2, 0);
       analogWrite(MOTOR_A1, 0);
       analogWrite(MOTOR_A2, RIGHT_MOTOR_SPEED);
    }
}

//Servo related functions
void rotateUltraServo(int angle)
{
  for(int i = 0 ;  i<5 ; i++)
  {
    int pulseWidth = map(angle, -90 , 90 , 600 , 2400);
    digitalWrite(ULTRA_SERVO , HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(ULTRA_SERVO , LOW);
    delay(20);
  }
}

void rotateGripperServo(int angle)
{
  for(int i = 0 ;  i<5 ; i++)
  {
    int pulseWidth = map(angle, -90 , 90 , 600 , 2400);
    digitalWrite(GRIPPER_SERVO , HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(GRIPPER_SERVO , LOW);
    delay(20);
  }
}

//stop robot function
void stopRobot()
{
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, LOW);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, LOW);
}

//Rotate related functions
void rotateLeft(int r) {
  resetRotations();

  while(r > countL){
    analogWrite(MOTOR_B1, LEFT_MOTOR_SPEED);
    analogWrite(MOTOR_B2, 0);
    analogWrite(MOTOR_A1, 0);
    analogWrite(MOTOR_A2, 0);
  }
}

void rotateRight(int r) {
  resetRotations();

  while(r > countR){
    analogWrite(MOTOR_B1, 0);
    analogWrite(MOTOR_B2, 0);
    analogWrite(MOTOR_A1, 0);
    analogWrite(MOTOR_A2, RIGHT_MOTOR_SPEED - 25);
  }
}

//Lights
void setPixlsRed() {
  pixels.setPixelColor(0, pixels.Color(0, BRIGHTNES_LEVEL, 0));
  pixels.setPixelColor(1, pixels.Color(0, BRIGHTNES_LEVEL, 0));
  pixels.setPixelColor(2, pixels.Color(0, BRIGHTNES_LEVEL, 0));
  pixels.setPixelColor(3, pixels.Color(0, BRIGHTNES_LEVEL, 0));
  pixels.show();
}

void setPixlsGreen() {
  pixels.setPixelColor(0, pixels.Color(BRIGHTNES_LEVEL, 0, 0));
  pixels.setPixelColor(1, pixels.Color(BRIGHTNES_LEVEL, 0, 0));
  pixels.setPixelColor(2, pixels.Color(BRIGHTNES_LEVEL, 0, 0));
  pixels.setPixelColor(3, pixels.Color(BRIGHTNES_LEVEL, 0, 0));
  pixels.show();
}

void setPixlsYellow() {
  pixels.setPixelColor(0, pixels.Color(BRIGHTNES_LEVEL, BRIGHTNES_LEVEL, 0));
  pixels.setPixelColor(1, pixels.Color(BRIGHTNES_LEVEL, BRIGHTNES_LEVEL, 0));
  pixels.setPixelColor(2, pixels.Color(BRIGHTNES_LEVEL, BRIGHTNES_LEVEL, 0));
  pixels.setPixelColor(3, pixels.Color(BRIGHTNES_LEVEL, BRIGHTNES_LEVEL, 0));
  pixels.show();
}

void setPixlsBlue() {
  pixels.setPixelColor(0, pixels.Color(0, 0, BRIGHTNES_LEVEL));
  pixels.setPixelColor(1, pixels.Color(0, 0, BRIGHTNES_LEVEL));
  pixels.setPixelColor(2, pixels.Color(0, 0, BRIGHTNES_LEVEL));
  pixels.setPixelColor(3, pixels.Color(0, 0, BRIGHTNES_LEVEL));
  pixels.show();
}

//Line sensor functions

 //Read values from sensor
void read() {
  sensor_A1 = analogRead(A1);
  sensor_A2 = analogRead(A2);
  sensor_A3 = analogRead(A3);
  sensor_A4 = analogRead(A4);
  sensor_A5 = analogRead(A5);
  sensor_A6 = analogRead(A6);
}

int getAverageLightLevel() {
  read();
  return (sensor_A1 + sensor_A2 + sensor_A3 + sensor_A4 + sensor_A5 + sensor_A6) / 6;
}

// Calculate black limit from array of light levels 
int getAverageBlackLimit(int* array) {
  int res = 0;
  for (int i = 0; i < 3; i++) {
    res += array[i];
  }
  return res / 3;
}

void happyTime()
{
  rotateGripperServo(-45);
  delay(200);
  rotateGripperServo(45);
  delay(200);
  rotateGripperServo(-45);
  delay(200);
  rotateGripperServo(45);
  delay(200);
}
