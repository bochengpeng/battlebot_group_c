//LEDs NeoPixels
#include <Adafruit_NeoPixel.h> //import NeoPixel Library

#define PIXEL_PIN 13 //LEDs PIN
#define NUM_PIXELS 4 //Number of LEDs;

// Create an instance of the Adafruit_NeoPixel class
Adafruit_NeoPixel pixels(NUM_PIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

// NeoPixel Colors
#define COLOR_TEAL    pixels.Color(0, 128, 128)
#define COLOR_YELLOW  pixels.Color(255, 255, 0)
#define COLOR_BLUE   pixels.Color(0, 0, 255)
#define COLOR_PURPLE pixels.Color(255, 0, 255)
#define COLOR_RED pixels.Color(0, 255, 0)
#define COLOR_PINK pixels.Color(55, 3, 196)
#define COLOR_GREEN pixels.Color(5, 247, 235)
#define COLOR_LIGHTGREEN pixels.Color(231, 247, 2)

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

//Line Sensors
const int numSensors = 8; // Number of line sensors
const int sensorPins[numSensors] = {A0, A1, A2, A3, A4, A5, A6, A7}; // Analog input pins for the line sensors

//Gripper 
const int gripperPin = 10;
const int minPulseWidth = 910;
const int  maxPulseWidth = 1900;

//Start and End Point
const long minDistance = 30;
const int blackThreshold = 800;
bool startingPoint = true;
bool endGame = false;
bool onSquareStart = false;
bool gameStart = false;
bool start = false;

//Left and Right wheel Count
volatile int countL = 0;
volatile int countR = 0;

/*   _   _   _____    ___    ____    ___  __  __  _____   _         _   _   ____    _____ 
 | \ | | | ____|  / _ \  |  _ \  |_ _| \ \/ / | ____| | |       | | | | / ___|  | ____|
 |  \| | |  _|   | | | | | |_) |  | |   \  /  |  _|   | |       | | | | \___ \  |  _|  
 | |\  | | |___  | |_| | |  __/   | |   /  \  | |___  | |___    | |_| |  ___) | | |___ 
 |_| \_| |_____|  \___/  |_|     |___| /_/\_\ |_____| |_____|    \___/  |____/  |_____|
                                                                                        */

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

/*   __  __  _____     _______ __  __ _____ _   _ _____    ____ ___  _   _ _____ ____   ___  _     
 |  \/  |/ _ \ \   / | ____|  \/  | ____| \ | |_   _|  / ___/ _ \| \ | |_   _|  _ \ / _ \| |    
 | |\/| | | | \ \ / /|  _| | |\/| |  _| |  \| | | |   | |  | | | |  \| | | | | |_) | | | | |    
 | |  | | |_| |\ V / | |___| |  | | |___| |\  | | |   | |__| |_| | |\  | | | |  _ <| |_| | |___ 
 |_|  |_|\___/  \_/  |_____|_|  |_|_____|_| \_| |_|    \____\___/|_| \_| |_| |_| \_\\___/|_____|
                                                                                                */

//Rotation count
void ISR_L(){
  countL++;
}

void ISR_R(){
  countR++;
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
    setColor(COLOR_BLUE);
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
    beforeTurningPixel();
  }
}

void beforeTurningLeft() {

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
    beforeTurningLeftPixel();

    if (isBlack(sensorPins[0])) {
      beforeTurning();
      turnRight();
      break;
    }
  }
}

void beforeTurningAround() {

  countL = 0;
  countR = 0;

  while (countL < 18 && countR < 18) {
    
    int rightSpeed = 220;
    int leftSpeed = 235;

    digitalWrite(MOTOR_A1_PIN, LOW);
    analogWrite(MOTOR_A2_PIN, leftSpeed);

    digitalWrite(MOTOR_B2_PIN, LOW);
    analogWrite(MOTOR_B1_PIN, rightSpeed);
    turnAroundPixel();
  }
}

void turnLeftAround() {

  countL = 0;
  countR = 0;

  while (countR < 19 && countL < 19) {
    
    int rightSpeed = 235;
    int leftSpeed = 245;

    analogWrite(MOTOR_A1_PIN, leftSpeed);
    digitalWrite(MOTOR_A2_PIN, LOW);

    digitalWrite(MOTOR_B2_PIN, LOW);
    analogWrite(MOTOR_B1_PIN, rightSpeed);
    turnLeftPixel(); 
  }
}

void turnLeft() {

  countL = 0;
  countR = 0;

  while ((!isBlack(sensorPins[3]) || !isBlack(sensorPins[4])) || (countR < 19)) {
    
    int rightSpeed = 210;
    int leftSpeed = 210;

    analogWrite(MOTOR_A1_PIN, LOW);
    digitalWrite(MOTOR_A2_PIN, LOW);

    digitalWrite(MOTOR_B2_PIN, LOW);
    analogWrite(MOTOR_B1_PIN, rightSpeed);
    turnLeftPixel();
  }
}

void toLeftAdjust() {
  
  int rightSpeed = 230;
  int leftSpeed = 150;

  digitalWrite(MOTOR_A1_PIN, LOW);
  analogWrite(MOTOR_A2_PIN, leftSpeed);

  digitalWrite(MOTOR_B2_PIN, LOW);
  analogWrite(MOTOR_B1_PIN, rightSpeed);
  toLeftAdjustPixel();
}

void toRightAdjust() {
  
  int rightSpeed = 150;
  int leftSpeed = 245;

  digitalWrite(MOTOR_A1_PIN, LOW);
  analogWrite(MOTOR_A2_PIN, leftSpeed);

  digitalWrite(MOTOR_B2_PIN, LOW);
  analogWrite(MOTOR_B1_PIN, rightSpeed);
  toRightAdjustPixel();
}

void turnRight() {

  countL = 0;
  countR = 0;

  while ((!isBlack(sensorPins[3]) || !isBlack(sensorPins[4])) || (countL < 19)) {

    int rightSpeed = 220;
    int leftSpeed = 240;

    digitalWrite(MOTOR_A1_PIN, LOW);
    analogWrite(MOTOR_A2_PIN, leftSpeed);

    digitalWrite(MOTOR_B1_PIN, LOW);
    analogWrite(MOTOR_B2_PIN, LOW);
    turnRightPixel();
  }
}

void turnAround() {

  countL = 0;
  countR = 0;

  while (countL < 37 && countR < 40) {
    
    digitalWrite(MOTOR_A1_PIN, LOW);
    digitalWrite(MOTOR_A2_PIN, HIGH);
    digitalWrite(MOTOR_B1_PIN, LOW);
    digitalWrite(MOTOR_B2_PIN, HIGH);
    turnAroundPixel();
  }
}

void moveStop() {
  digitalWrite(MOTOR_A1_PIN, LOW);
  digitalWrite(MOTOR_A2_PIN, LOW);
  digitalWrite(MOTOR_B1_PIN, LOW);
  digitalWrite(MOTOR_B2_PIN, LOW);
  moveStopPixel();
}


void goBackwards(){
  
    int rightSpeed = 220;
    int leftSpeed = 235;
    
    digitalWrite(MOTOR_A1_PIN, HIGH);
    analogWrite(MOTOR_A2_PIN, leftSpeed);
    
    digitalWrite(MOTOR_B2_PIN, HIGH);
    analogWrite(MOTOR_B1_PIN, rightSpeed);
    goBackwardsPixel();
}


void finalDrop(){
      moveStop();
      gripOpen();
      delay(1500);
      goBackwards();
      goBackwardsPixel();
      delay(2000);
      moveStop();
      moveStopPixel();
}

/*   ____                                
 / ___|  ___ _ __  ___  ___  _ __ ___ 
 \___ \ / _ | '_ \/ __|/ _ \| '__/ __|
  ___) |  __| | | \__ | (_) | |  \__ \
 |____/ \___|_| |_|___/\___/|_|  |___/
                                       */

// Check sensor value if it is on black lines
bool isBlack(int sensorPin) {
    return analogRead(sensorPin) > blackThreshold; // when sensor value > 800 means on black lines
} 

void checkSensors(bool sensorValues[]) {
    for (int i = 0; i < numSensors; ++i) {
        sensorValues[i] = isBlack(sensorPins[i]); //check if any sensor detects a black lin
    }
}

bool checkLines(){
    // At starting point, check if any sensor detects a black line
    for (int i = 0; i < numSensors; i++) {
        if (analogRead(sensorPins[i]) > blackThreshold) {
            return true; // Return true if a black line is detected by any sensor
        }
    }
    
    return false; // Return false if no black line is detected by any sensor
}

//Starting Point Logic
void startUp(){

    //sensors used as the indication to trigger time-recording when on black areas before pick up cone
    bool grabIndication = isBlack(sensorPins[3]) && isBlack(sensorPins[4]) && isBlack(sensorPins[5]) && isBlack(sensorPins[6]);
    static unsigned long startTime = 0;
    int durationThreshold = 100;

    //if not on black square
    if (onSquareStart == false) {
      gripOpen();  
      goStraight();
      delay(200);    
      moveStop();
      onSquareStart = true;
    }

     //if on black square 
    if (onSquareStart = true && grabIndication) {
       if (startTime == 0) {
            startTime = millis(); // record time
            goStraight();
        } else{
            // count duration
            unsigned long duration = millis() - startTime;
            // up to the duration
            if (duration >= durationThreshold) { 
                gripClose(); 
                delay(1000);     
                turnLeft();      
                delay(20);
                goStraight();
                startingPoint = false; 
                gameStart = true;
            }
        }
    }
}

//check if the robot meets the endpoint
void checkEndCondition(){
  
    //sensors used as the indication to trigger time-recording when on black areas before release the cone
    bool endIndication = isBlack(sensorPins[2]) && isBlack(sensorPins[3]) > 800 && isBlack(sensorPins[4]) > 800 && isBlack(sensorPins[5]) > 800 && isBlack(sensorPins[6]) && isBlack(sensorPins[7]) > 800;
    static unsigned long startTime = 0;
    int durationThreshold = 100;
    
    if (endGame == false && endIndication) {
        if (startTime == 0) {
            startTime = millis(); // record time
        } else {
            // count duration
            unsigned long duration = millis() - startTime;
            if (duration >= durationThreshold) {
                endGame = true;
                gameStart = false;
            }
        }
    }
}

/* ULTRASONIC SENSORS USE AT STARTING POINT */

//when ultrasonic sensor detects return value lower than minDistance, start up robot
void startTrigger() {
  
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

    if(distance < minDistance){
      start = true;
    }
}

/*    ____ ____  ___ ____  ____  _____ ____  
  / ___|  _ \|_ _|  _ \|  _ \| ____|  _ \ 
 | |  _| |_) || || |_) | |_) |  _| | |_) |
 | |_| |  _ < | ||  __/|  __/| |___|  _ < 
  \____|_| \_\___|_|   |_|   |_____|_| \_\
                                          */

// Function to set servo angle 
void setServoAngle(int angle, int gripperPin){
  int pulseWidth = map(angle, 0, 180, minPulseWidth, maxPulseWidth);
  digitalWrite(gripperPin, HIGH); // Start the pulse
  delayMicroseconds(pulseWidth); // Wait for the pulse width
  digitalWrite(gripperPin, LOW); // End the pulse
  delay(20); // wait for servo to settle
}

void gripOpen(){
  setServoAngle(90,gripperPin);
}

void gripClose(){
  setServoAngle(2,gripperPin);
}


/*  _     ___ _   _ _____   __  __    _    __________  
 | |   |_ _| \ | | ____| |  \/  |  / \  |__  | ____| 
 | |    | ||  \| |  _|   | |\/| | / _ \   / /|  _|   
 | |___ | || |\  | |___  | |  | |/ ___ \ / /_| |___  
 |_____|___|_| \_|_____| |_|  |_/_/   \_/____|_____| 
                                                      */

void maze() {

  // Array to store sensor values
  bool sensorValues[numSensors];
   checkSensors(sensorValues);
  bool deadEnd = !isBlack(sensorPins[0]) && !isBlack(sensorPins[1]) && !isBlack(sensorPins[2]) && !isBlack(sensorPins[3]) && !isBlack(sensorPins[4]) && !isBlack(sensorPins[5]) && !isBlack(sensorPins[6]) && !isBlack(sensorPins[7]);
  //dead-end indication when all sensors on white areas
  
  bool right = isBlack(sensorPins[0]); // sensor A0, the first one at the right on black areas
  bool straight = isBlack(sensorPins[3]) && isBlack(sensorPins[4]); //sensor A3, sensor A4 on black areas
  bool straightTurn = !isBlack(sensorPins[3]) && !isBlack(sensorPins[4]);//sensor A3, sensor A4 on white areas  
  bool adjustRight =  isBlack(sensorPins[2]) || (!isBlack(sensorPins[2]) && isBlack(sensorPins[3]) && !isBlack(sensorPins[4]));
  //sensor A2 on black areas or sensor A2, A4 on white areas at the same time with sensor A3 on black areas
  
  bool adjustLeft = isBlack(sensorPins[5]) || (!isBlack(sensorPins[5]) && isBlack(sensorPins[4]) && !isBlack(sensorPins[3]));
  //sensor A5 on black areas or sensor A5, A4 on white areas at the same time with sensor A3 on black areas
  
  bool adjustRightAfterTurn = isBlack(sensorPins[0]) || isBlack(sensorPins[1]) || isBlack(sensorPins[2]); //sensor A0, A1, A2 on the black areas
  bool adjustLeftAfterTurn = isBlack(sensorPins[7]) || isBlack(sensorPins[6]) || isBlack(sensorPins[5]);//sensor A7, A6, A5 on the black areas 
  bool left = isBlack(sensorPins[7]); // sensor A7 at the left on black area
  bool rightCheck = isBlack(sensorPins[2]) && isBlack(sensorPins[3]); //sensor A2 and A3 on black areas
  bool leftCheck = isBlack(sensorPins[5]) && isBlack(sensorPins[6]);//sensor A5 and A6 on black areas
  bool tCross = isBlack(sensorPins[0]) && isBlack(sensorPins[1]) && isBlack(sensorPins[2]) && isBlack(sensorPins[3]) && isBlack(sensorPins[4]) && isBlack(sensorPins[5]) && isBlack(sensorPins[6]) && isBlack(sensorPins[7]);
  //when at T intersection all sensors are on the black line

  if (straight) {
    goStraight();
    Serial.println("Straight");
  } else if (adjustRight && !adjustLeft) {
    toRightAdjust();
    Serial.println("ADRight");
  } else if (adjustLeft && !adjustRight) {
    toLeftAdjust();
    Serial.println("ADLeft");
  }

  if (tCross || right) {
    beforeTurning();
    turnRight();
    Serial.println("Right");
  } else if (left && !right) { 
    beforeTurningLeft();
    Serial.println("BeforeLeft");
    checkSensors(sensorValues); 
    if (straightTurn) {   
      turnLeft();
      moveStop();
      Serial.println("LeftTurn");
    }
  }

  if (deadEnd) {
      beforeTurningAround();
      moveStop();
      delay(500);
      turnAround();
      moveStop();
      delay(1000);
      Serial.println("TurnAround");

      if (adjustRightAfterTurn && !adjustLeft) {
          while (!isBlack(sensorPins[3]) && !isBlack(sensorPins[4])) {
            toRightAdjust();
          }
      } else if (adjustLeftAfterTurn && !adjustRight) {
          while (!isBlack(sensorPins[3]) && !isBlack(sensorPins[4])) {
            toLeftAdjust();
          }
      } 
    }
}


void setup() {

    //Gripper
    pinMode(gripperPin,OUTPUT); 
    
    //initialises the gripper
    for (int i = 0; i < 4; i++)
    {
      gripOpen();
    }
    gripOpen();

    //Motors
    pinMode(MOTOR_A1_PIN, OUTPUT);
    pinMode(MOTOR_A2_PIN, OUTPUT);
    pinMode(MOTOR_B1_PIN, OUTPUT);
    pinMode(MOTOR_B2_PIN, OUTPUT);

    Serial.begin(9600);
    
    //UltraSonic Sensors
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

    //Rotation Sensors
    pinMode(ROTATION_SENSOR_LEFT,INPUT_PULLUP);
    pinMode(ROTATION_SENSOR_RIGHT,INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(ROTATION_SENSOR_LEFT), ISR_L, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ROTATION_SENSOR_RIGHT), ISR_R, CHANGE);

    //NeoPixel Strips
    pixels.begin();     
}


void loop() {
  
        if(startingPoint == true && endGame == false && gameStart == false){ 
            startTrigger();    
            if(start == true){
              startUp();
            }            
        }
        
        if(startingPoint == false && endGame == false && gameStart == true){
            maze();
            checkEndCondition();
        }
    
        if(startingPoint == false && endGame == true && gameStart == false){
            finalDrop();
        }
}
