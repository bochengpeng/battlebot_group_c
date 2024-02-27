//pins
const int buttonOnePin = 2;
const int neoPixelPin = 3;
const int RWFPin = 5; //right wheel forward
const int LWFPin = 6; //left wheel forward
const int triggerPin = 8;
const int echoPin = 9;
const int TLPin = 10; //turn left pin
const int TRPin = 11; //turn right pin
const int lineSensorPins[] = {12, 13, 14, 15, 16, 17, 18, 19}; 
bool hasItStarted = false;
float sensorDistance = 0;
float sensorTime = 0;
