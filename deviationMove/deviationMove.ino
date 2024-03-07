
const int motorA1 = 6;  // Motor A input 1
const int motorA2 = 9;  // Motor A input 2
const int motorB1 = 3;  // Motor B input 1
const int motorB2 = 5;  // Motor B input 2

const int numSensors = 8; // Number of line sensors
const int sensorPins[numSensors] = {A0, A1, A2, A3, A4, A5, A6, A7}; // Analog input pins for the line sensors
const int sensor3 = A2; // Analog pin for the middle-left sensor
const int sensor4 = A3;
const int sensor5 = A4;
const int sensor6 = A5; // Analog pin for the middle-right sensor
const int deviationThreshold = 420;


// read sensor
bool isBlack(int sensorPin) {
    return analogRead(sensorPin) > 950; // when sensor value > 800 means on black
}

void goStraight() {
    int speed = 255;
    //left wheel anticlockwise
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, HIGH);
    analogWrite(motorA2, speed);
    // right wheel anticlockwise
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    analogWrite(motorB1, speed);
}

void turnLeft() {
    int speed = 200;
    
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    analogWrite(motorA1, speed);
   
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    analogWrite(motorA1, speed);
}

void toLeftAdjust() {
    int leftSpeed = 100;
    
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    analogWrite(motorA1, leftSpeed);
    
    int rightSpeed = 130;
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    analogWrite(motorA1, rightSpeed);
}

void toRightAdjust() {
    int leftSpeed = 130;
    
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    analogWrite(motorA1, leftSpeed);
    
    int rightSpeed = 100;
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    analogWrite(motorA1, rightSpeed);
}

void turnRight() {
    int speed = 200;
    
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, HIGH);
    analogWrite(motorA2, speed);
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, HIGH);
    analogWrite(motorA2, speed);
}


void setup() {
    pinMode(motorA1, OUTPUT);
    pinMode(motorA2, OUTPUT);
    pinMode(motorB1, OUTPUT);
    pinMode(motorB2, OUTPUT);
}

void loop() {
    bool right = isBlack(sensorPins[0]) && isBlack(sensorPins[1]); // A0,A1, the first two at the right, black
    bool straight = isBlack(sensor4) && isBlack(sensor5); // A3, A4 Black;
    int deviation = abs(analogRead(sensor4) - analogRead(sensor5));//the absolute value between A3 and A4 
    bool left = isBlack(sensorPins[6]) && isBlack(sensorPins[7]); // A6,A7 the first two at the left, black
    // read sensors to control movement
    if (straight && deviation < 470) {
        // when the two middle sensors detect black, move forwarde
        goStraight();
    } else if (!left && right) {
        turnRight();
    } else if (left && !right) {
        turnLeft();
    } else if (deviation > 470) {
      if(analogRead(sensor4) > analogRead(sensor5)){
        toRightAdjust();
      } else {
        toLeftAdjust();
      }
     } 
  }
