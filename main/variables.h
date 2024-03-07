//pins
const int buttonOnePin = 2;
const int neoPixelPin = 3;
const int rightForwardPin = 5; 
const int leftForwardPin = 6; 
const int triggerPin = 8;
const int echoPin = 9;
const int turnLeftPin = 10; 
const int turnRightPin = 11;
const int lineSensorPins[] = {12, 13, 14, 15, 16, 17, 18, 19}; 
float sensorDistance = 0;
float sensorTime = 0;
long timer = 0;
const int leftForwardSpeed = 255;
const int rightForwardSpeed = 250;
int lineSensorValues[8];
int buttonOneState;
bool hasStarted = false;
