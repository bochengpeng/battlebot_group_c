
const int motorA1 = 6;  // Motor A input 1
const int motorA2 = 9;  // Motor A input 2
const int motorB1 = 3;  // Motor B input 1
const int motorB2 = 5;  // Motor B input 2

const int numSensors = 8; // Number of line sensors
const int sensorPins[numSensors] = {A0, A1, A2, A3, A4, A5, A6, A7}; // Analog input pins for the line sensors
const int middleSensorPin1 = A2; // Analog pin for the middle-left sensor
const int middleSensorPin2 = A3;
const int middleSensorPin3 = A4;
const int middleSensorPin4 = A5; // Analog pin for the middle-right sensor


// read sensor
bool isBlack(int sensorPin) {
    return analogRead(sensorPin) > 550; // when sensor value > 550 means on black
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
    int leftSpeed = 180;
    
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    analogWrite(motorA1, leftSpeed);
    
    int rightSpeed = 210;
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    analogWrite(motorA1, rightSpeed);
}

void toRightAdjust() {
    int leftSpeed = 210;
    
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    analogWrite(motorA1, leftSpeed);
    
    int rightSpeed = 180;
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
    bool left = isBlack(sensorPins[0]) && isBlack(sensorPins[1]); // A0,A1 black
    bool straight = isBlack(middleSensorPin2) && isBlack(middleSensorPin3); // A3, A4 Black;
    bool rightTitled = isBlack(middleSensorPin1) && isBlack(middleSensorPin2); //A2,A3, Black;
    bool leftTitled = isBlack(middleSensorPin3) && isBlack(middleSensorPin4); //A4,A5 Black;   
    bool right = isBlack(sensorPins[6]) && isBlack(sensorPins[7]); // A6,A7 black

    // read sensors to control movement
    if (straight && !left && !right) {
        // when the two middle sensors detect black, move forwarde
        goStraight();
    } else if (!left && right) {
        turnRight();
    } else if (left && !right) {
        turnLeft();
    } else if (rightTitled) {
        toLeftAdjust();
    } else if (leftTitled) {
        toRightAdjust();
    }
  }
