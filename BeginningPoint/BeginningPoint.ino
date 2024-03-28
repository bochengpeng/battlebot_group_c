#define MOTOR_A1_PIN 6
#define MOTOR_A2_PIN 9
#define MOTOR_B1_PIN 11
#define MOTOR_B2_PIN 5

#define GRIPPER_PIN 10 // Pin connected to the servo signal wire

// Rotation sensors
#define ROTATION_SENSOR_LEFT 2
#define ROTATION_SENSOR_RIGHT 3

volatile int countL = 0;
volatile int countR = 0;

const int numSensors = 8;
const int sensorPins[numSensors] = {A0, A1, A2, A3, A4, A5, A6, A7};
const int threshold = 800; // Threshold value for black line detection

int lineCount = 0;
bool startingPoint = true;

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
    return analogRead(sensorPin) > 800; // when sensor value > 800 means on black
}


void setup() {

    pinMode(MOTOR_A1_PIN, OUTPUT);
    pinMode(MOTOR_A2_PIN, OUTPUT);
    pinMode(MOTOR_B1_PIN, OUTPUT);
    pinMode(MOTOR_B2_PIN, OUTPUT);
    pinMode(GRIPPER_PIN, OUTPUT);
    
    Serial.begin(9600);

    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
    pinMode(A6, INPUT);
    pinMode(A7, INPUT);

    pinMode(ROTATION_SENSOR_LEFT,INPUT_PULLUP);
    pinMode(ROTATION_SENSOR_RIGHT,INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(ROTATION_SENSOR_LEFT), ISR_L, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ROTATION_SENSOR_RIGHT), ISR_R, CHANGE);
}


bool checkLines(){
    // Check if any sensor detects a black line
    for (int i = 0; i < numSensors; i++) {
        if (analogRead(sensorPins[i]) > threshold) {
            return true; // Return true if a black line is detected by any sensor
        }
    }
    return false; // Return false if no black line is detected by any sensor
}

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

void turnLeft() {

    countL=0;
    countR=0;
    
    while((!isBlack(sensorPins[3]) || !isBlack(sensorPins[4]))||(countR < 32)) {
      
        int rightSpeed = 240; 
        int leftSpeed = 240;  

        digitalWrite(MOTOR_A1_PIN, LOW);
        analogWrite(MOTOR_A2_PIN, LOW);
        
        digitalWrite(MOTOR_B2_PIN, LOW);
        analogWrite(MOTOR_B1_PIN, rightSpeed);
    }   
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
        delay(1500); // Delay for gripper to close
        goStraight();
        delay(150);
        turnLeft();
        delay(50);
        startingPoint = false;
        lineCount++;
   }
}

void loop(){

    
    if(startingPoint){
      delay(80);//time for removing hands
      lineCountAndGrabing();
    }
}
