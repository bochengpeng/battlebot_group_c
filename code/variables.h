//pins
const int buttonOnePin = 2;
const int neoPixelPin = 3;
const int rightForwardPin = 5; //right wheel forward
const int leftForwardPin = 6; //left wheel forward
const int triggerPin = 8;
const int echoPin = 9;
const int turnLeftPin = 10; //turn left pin
const int turnRightPin = 11; //turn right pin
const int lineSensorPins[] = {12, 13, 14, 15, 16, 17, 18, 19}; 
bool hasItStarted = false;
float sensorDistance = 0;
float sensorTime = 0;
const int leftForwardSpeed = 255;
const int rightForwardSpeed = 250;
