
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

//ultrasound sensor
float sensorDistance = 0;
float sensorTime = 0;

//timing
unsigned long timer = 0;
unsigned long previousTime = 0;
int interval = 200;

//motor values
const int leftForwardSpeed = 255;
const int rightForwardSpeed = 250;

//lineSensor
int lineSensorValues[8];

//button
int buttonOneState;
bool hasStarted = false;

Adafruit_NeoPixel LEDs(4, neoPixelPin, NEO_RGB + NEO_KHZ800);
