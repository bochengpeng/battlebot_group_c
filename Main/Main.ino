//LEDs NeoPixels
#include <Adafruit_NeoPixel.h> //import NeoPixel Library
#define PIXEL_PIN 13 //LEDs PIN
#define NUM_PIXELS 4 //Number of LEDs;
// Create an instance of the Adafruit_NeoPixel class
Adafruit_NeoPixel pixels(NUM_PIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

// NeoPixel colors
#define COLOR_TEAL    pixels.Color(0, 128, 128)
#define COLOR_YELLOW  pixels.Color(255, 255, 0)
#define COLOR_BLUE   pixels.Color(0, 0, 255)
#define COLOR_PURPLE pixels.Color(255, 0, 255)
#define COLOR_RED pixels.Color(0, 255, 0)
#define COLOR_PINK pixels.Color(55, 3, 196)
#define COLOR_GREEN pixels.Color(5, 247, 235)
#define COLOR_LIGHTGREEN pixels.Color(231, 247, 2)

//Gripper Pin
#define GRIPPER_PIN 10 // Pin connected to the servo signal wire
//UltraSonic Sensor Pin
#define TRIGPIN 7
#define ECHOPIN 8

//Motors
#define MOTOR_A1_PIN 6 // Motor A input 6
#define MOTOR_A2_PIN 9 // Motor A input 9
#define MOTOR_B1_PIN 11 // Motor A input 11
#define MOTOR_B2_PIN 5 // Motor A input 5

// Rotation sensors
#define ROTATION_SENSOR_LEFT 2
#define ROTATION_SENSOR_RIGHT 3
volatile int countL = 0;
volatile int countR = 0;

//Line Sensors
const int numSensors = 8; // Number of line sensors
const int sensorPins[numSensors] = {A0, A1, A2, A3, A4, A5, A6, A7}; // Analog input pins for the line sensors
const int threshold = 800; // Threshold value for black line detection

//Start and End Point
const long minDistance = 30;
int lineCount = 0;
bool startingPoint = true;
int endTimer = 0;
bool endGame = false;
bool lineMaze = false;

//Use of NeoPixels
void setColor(uint32_t color) {
  for (int i = 0; i < NUM_PIXELS; i++) {
    pixels.setPixelColor(i, color); // Set color for each pixel
  }
  pixels.show(); // Update pixels
}

void goStraightPixel(){
   setColor(COLOR_BLUE);
}

void beforeTurningPixel(){
   setColor(COLOR_PINK);
}

void beforeTurningLeftPixel(){
  setColor(COLOR_GREEN);
}

void turnLeftPixel(){
  setColor(COLOR_PURPLE);
}

void turnRightPixel(){
  setColor(COLOR_YELLOW);
}

void toLeftAdjustPixel(){
  setColor(COLOR_BLUE);
}

void toRightAdjustPixel(){
  setColor(COLOR_BLUE);
}

void turnAroundPixel(){
  setColor(COLOR_TEAL);
}

void moveStopPixel(){
  setColor(COLOR_RED);
}

void goBackwardsPixel(){
  setColor(COLOR_LIGHTGREEN);
}

//Rotation count
void ISR_L(){
  countL++;
}

void ISR_R(){
  countR++;
}

bool deadEndFlag = false;
int consecutiveDeadEndCount = 0;

// read sensor
bool isBlack(int sensorPin) {
    return analogRead(sensorPin) > threshold; // when sensor value > 800 means on black
}

void goStraight() {
  
    int rightSpeed = 220;
    int leftSpeed = 235;
    
    //left wheel anticlockwise
    digitalWrite(MOTOR_A1_PIN, LOW);
    analogWrite(MOTOR_A2_PIN, leftSpeed);
    
    // right wheel anticlockwise
    digitalWrite(MOTOR_B2_PIN, LOW);
    analogWrite(MOTOR_B1_PIN, rightSpeed);
}

void goBackwards(){
  
    int rightSpeed = 220;
    int leftSpeed = 235;
    
    //left wheel anticlockwise
    digitalWrite(MOTOR_A1_PIN, HIGH);
    analogWrite(MOTOR_A2_PIN, leftSpeed);
    
    // right wheel anticlockwise
    digitalWrite(MOTOR_B2_PIN, HIGH);
    analogWrite(MOTOR_B1_PIN, rightSpeed);
}

void turnLeft() {
  
    countL=0;
    countR=0;

    while(countR < 19 && countL < 19) {
      int rightSpeed = 235; 
      int leftSpeed = 245; 
           
      analogWrite(MOTOR_A1_PIN, leftSpeed);
      digitalWrite(MOTOR_A2_PIN, LOW);
      
      digitalWrite(MOTOR_B2_PIN, LOW);
      analogWrite(MOTOR_B1_PIN, rightSpeed);
    } 
}

void turnRight() {

    countL=0;
    countR=0;
    
    while((!isBlack(sensorPins[3]) || !isBlack(sensorPins[4]))||(countL < 32)) {
      
      int rightSpeed = 220; 
      int leftSpeed = 240;  

      digitalWrite(MOTOR_A1_PIN, LOW);
      analogWrite(MOTOR_A2_PIN, leftSpeed);
    
      digitalWrite(MOTOR_B1_PIN, LOW);
      analogWrite(MOTOR_B2_PIN, LOW);
    }  
}

void beforeTurning() {

  countL=0;
  countR=0;
  
  while(countL < 6 && countR < 6){
    int rightSpeed = 220;
    int leftSpeed = 235;
    
    //left wheel anticlockwise
    digitalWrite(MOTOR_A1_PIN, LOW);
    analogWrite(MOTOR_A2_PIN, leftSpeed);
    
    // right wheel anticlockwise
    digitalWrite(MOTOR_B2_PIN, LOW);
    analogWrite(MOTOR_B1_PIN, rightSpeed);
  }
}

void beforeTurningLeft() {

  countL=0;
  countR=0;
  
  while(countL < 18 && countR < 18){
    int rightSpeed = 220;
    int leftSpeed = 235;
    
    //left wheel anticlockwise
    digitalWrite(MOTOR_A1_PIN, LOW);
    analogWrite(MOTOR_A2_PIN, leftSpeed);
    
    // right wheel anticlockwise
    digitalWrite(MOTOR_B2_PIN, LOW);
    analogWrite(MOTOR_B1_PIN, rightSpeed);
  }
}

void toLeftAdjust() {
    
    int rightSpeed = 230; 
    int leftSpeed = 150; 
    
    
    digitalWrite(MOTOR_A1_PIN, LOW);
    analogWrite(MOTOR_A2_PIN, leftSpeed);
    
    digitalWrite(MOTOR_B2_PIN, LOW);
    analogWrite(MOTOR_B1_PIN, rightSpeed);
}

void toRightAdjust() {
    
    int rightSpeed = 150; 
    int leftSpeed = 245; 
    
    
    digitalWrite(MOTOR_A1_PIN, LOW);
    analogWrite(MOTOR_A2_PIN, leftSpeed);
    
    digitalWrite(MOTOR_B2_PIN, LOW);
    analogWrite(MOTOR_B1_PIN, rightSpeed);
}

void turnAround() {

    countL=0;
    countR=0;

    while((!isBlack(sensorPins[3]) || !isBlack(sensorPins[4]))||(countL < 38 && countR < 38)) {
        digitalWrite(MOTOR_A1_PIN, LOW);
        digitalWrite(MOTOR_A2_PIN, HIGH);
        digitalWrite(MOTOR_B1_PIN, LOW);
        digitalWrite(MOTOR_B2_PIN, HIGH);
    }
    
}

void moveStop() {
    digitalWrite(MOTOR_A1_PIN, LOW);
    digitalWrite(MOTOR_A2_PIN, LOW);
    digitalWrite(MOTOR_B1_PIN, LOW);
    digitalWrite(MOTOR_B2_PIN, LOW);
}

void checkSensors(bool sensorValues[]) {
    for (int i = 0; i < numSensors; ++i) {
        sensorValues[i] = isBlack(sensorPins[i]);
    }
}

//For Starting and End point
void closeGripper() {
    digitalWrite(GRIPPER_PIN, HIGH); 
    delayMicroseconds(1000); // Pulse width for neutral position (0 degrees)
    digitalWrite(GRIPPER_PIN, LOW); 
}

void openGripper() {
    digitalWrite(GRIPPER_PIN, HIGH); 
    delayMicroseconds(1500); // Pulse width for opened position (90 degrees)
    digitalWrite(GRIPPER_PIN, LOW); 
}

bool checkLines(){
    // At starting point, check if any sensor detects a black line
    for (int i = 0; i < numSensors; i++) {
        if (analogRead(sensorPins[i]) > threshold) {
            return true; // Return true if a black line is detected by any sensor
        }
    }
    return false; // Return false if no black line is detected by any sensor
}

void lineCountAndGrabing(){
  if(lineCount < 4){
      openGripper();
      goStraight();
    // Move straight until the fourth black line is detected
    bool lineDetected = checkLines();
    if (lineDetected) {
        lineCount++;
        delay(200); 
    }
  }

      // When the fourth black line is detected, stop, close the gripper, and then turn left
  if(lineCount == 4){
        moveStop();
        closeGripper();
        delay(1000); // Delay for gripper to close
        goStraight();
        delay(150);
        turnLeft();
        delay(50);
        startingPoint = false;
        lineCount++;
   }
}

bool checkEndCondition(){
  
   bool endIndication = isBlack(sensorPins[2]) > 900 && isBlack(sensorPins[3]) > 900 && isBlack(sensorPins[4])> 900 && isBlack(sensorPins[5])> 900;
   
   if(endIndication){
    if(millis() - endTimer >= 300 && endIndication){
        return true;
      }
    }

  return false;
}

void finalDrop(){
      moveStop();
      openGripper();
      delay(1500);
      goBackwards();
      goBackwardsPixel();
      delay(2000);
      moveStop();
      moveStopPixel();
}

long startTrigger() {
    long duration, distance;
    digitalWrite(TRIGPIN, LOW);
    delayMicroseconds(1);
    digitalWrite(TRIGPIN, HIGH);
    delayMicroseconds(1);
    digitalWrite(TRIGPIN, LOW);
    duration = pulseIn(ECHOPIN, HIGH);
    distance = (duration * 0.0343) / 2;  // Calculate distance in cm

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    delay(500);  // Delay for stability

    return distance;
}

// For Line Maze Logic
void maze() {

    // Array to store sensor values
    bool sensorValues[numSensors];
    checkSensors(sensorValues); 
    bool deadEnd = !isBlack(sensorPins[0]) && !isBlack(sensorPins[1]) && !isBlack(sensorPins[2]) && !isBlack(sensorPins[3]) && !isBlack(sensorPins[4]) && !isBlack(sensorPins[5]) && !isBlack(sensorPins[6]) && !isBlack(sensorPins[7]);
    bool right = isBlack(sensorPins[0]); // A0,A1, the first two at the right, black
    bool straight = !isBlack(sensorPins[2]) && !isBlack(sensorPins[5]);
    bool adjustRight =  isBlack(sensorPins[2]);
    bool adjustLeft = isBlack(sensorPins[5]);
    bool left = isBlack(sensorPins[7]); // A6,A7 the first two at the left, black
    bool rightCheck = isBlack(sensorPins[2]) && isBlack(sensorPins[3]);
    bool leftCheck = isBlack(sensorPins[5]) && isBlack(sensorPins[6]);
    bool tCross = isBlack(sensorPins[0]) && isBlack(sensorPins[1]) && isBlack(sensorPins[2]) && isBlack(sensorPins[3]) && isBlack(sensorPins[4]) && isBlack(sensorPins[5]) && isBlack(sensorPins[6]) && isBlack(sensorPins[7]);

    
    if (straight) {
       goStraight();
       goStraightPixel();
       Serial.println("Straight");
       
    } else if (adjustRight && !adjustLeft) {
          toRightAdjust();
          toRightAdjustPixel();
          Serial.println("ADRight");
    } else if (adjustLeft && !adjustRight) {
      toLeftAdjust();
      toLeftAdjustPixel();
      Serial.println("ADLeft");
    }

    if (tCross || right) {
        beforeTurningPixel();
        beforeTurning();
        turnRight();
        turnRightPixel();
        Serial.println("Right");
    } else if (left && !right) {
        beforeTurningLeftPixel();
        beforeTurningLeft();
        deadEndFlag = true;
        consecutiveDeadEndCount = 1;
        Serial.println("BeforeLeft");
        
          if (straight) {
              goStraight();
              goStraightPixel();
              deadEndFlag = false;
              consecutiveDeadEndCount = 0;
              Serial.println("NOPStraight");
          }
    }

    if (deadEnd) {
        if (!deadEndFlag) {
            beforeTurningLeftPixel();
            beforeTurningLeft();
            moveStop();
            moveStopPixel();
            delay(500);
            turnLeft();
            turnLeftPixel();
            moveStop();
            moveStopPixel();
            delay(1000);
            Serial.println("LEFT");
            deadEndFlag = true;
            consecutiveDeadEndCount++;
        } else {
            moveStop();
            moveStopPixel();
            delay(500);
            turnLeft();
            turnLeftPixel();
            moveStop();
            moveStopPixel();
            delay(1000);
            Serial.println("LEFT");
            
            consecutiveDeadEndCount++;
            if (consecutiveDeadEndCount >= 2) {
                deadEndFlag = false;
                consecutiveDeadEndCount = 0;
            }
        }
    }

    if (checkEndCondition()) {
       endGame = true;
    }
}

void setup() {

    //Gripper
    pinMode(GRIPPER_PIN, OUTPUT);

    //Motors
    pinMode(MOTOR_A1_PIN, OUTPUT);
    pinMode(MOTOR_A2_PIN, OUTPUT);
    pinMode(MOTOR_B1_PIN, OUTPUT);
    pinMode(MOTOR_B2_PIN, OUTPUT);

    Serial.begin(9600);
    //UltraSonic Sensor
    pinMode(TRIGPIN, OUTPUT);
    pinMode(ECHOPIN, INPUT);
    
    //Line Sensors
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
    pinMode(A6, INPUT);
    pinMode(A7, INPUT);

    //Rotation Sensros
    pinMode(ROTATION_SENSOR_LEFT,INPUT_PULLUP);
    pinMode(ROTATION_SENSOR_RIGHT,INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(ROTATION_SENSOR_LEFT), ISR_L, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ROTATION_SENSOR_RIGHT), ISR_R, CHANGE);

    //NeoPixel strip
    pixels.begin(); 
    
}


void loop() {

    if(startingPoint == true && endGame == false){
      delay(80);//time for removing hands
      lineCountAndGrabing();
    }
    
    if(startingPoint == false && endGame == false){
       maze();
    }

    if(startingPoint == false && endGame == true){
      finalDrop();
    }

//Logic when UltraSonic sensor used:
//    if(startTrigger() <= minDistance){
//       delay(1500);
//       lineMaze = true;
//    }
//
//    if(lineMaze == true){
//      if(startingPoint == true && endGame == false){
//        delay(80);//time for removing hands
//        lineCountAndGrabing();
//      }
//      
//      if(startingPoint == false && endGame == false){
//         maze();
//      }
//  
//      if(startingPoint == false && endGame == true){
//        finalDrop();
//      }     
//   } 

}
