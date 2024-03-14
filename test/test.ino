const int motorA1 = 6;  // Motor A input 1
const int motorA2 = 9;  // Motor A input 2
const int motorB1 = 3;  // Motor B input 1
const int motorB2 = 5;  // Motor B input 2

const int numSensors = 8; // Number of line sensors
const int sensorPins[numSensors] = {A0, A1, A2, A3, A4, A5, A6, A7}; // Analog input pins for the line sensors



// read sensor
bool isBlack(int sensorPin) {
    return analogRead(sensorPin) > 900; // when sensor value > 800 means on black
}


void goStraight() {
    int speed = 180;
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
    int speed = 160;
    
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
    
    int rightSpeed = 120;
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    analogWrite(motorA1, rightSpeed);
}

void toRightAdjust() {
    int leftSpeed = 120;
    
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    analogWrite(motorA1, leftSpeed);
    
    int rightSpeed = 100;
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    analogWrite(motorA1, rightSpeed);
}

void turnRight() {
    int speed = 160;
    
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, HIGH);
    analogWrite(motorA2, speed);
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, HIGH);
    analogWrite(motorA2, speed);
}

void turnAround() {
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, HIGH);
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, HIGH);
}

void setup() {
    pinMode(motorA1, OUTPUT);
    pinMode(motorA2, OUTPUT);
    pinMode(motorB1, OUTPUT);
    pinMode(motorB2, OUTPUT);

    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
    pinMode(A6, INPUT);
    pinMode(A7, INPUT);
    
}

void loop() {
    bool deadEnd = !isBlack(sensorPins[0]) && !isBlack(sensorPins[1]) && !isBlack(sensorPins[2]) && !isBlack(sensorPins[3]) && !isBlack(sensorPins[4]) && !isBlack(sensorPins[5]) && !isBlack(sensorPins[6]) && !isBlack(sensorPins[7]);
    bool right = isBlack(sensorPins[0]) && isBlack(sensorPins[1]); // A0,A1, the first two at the right, black
    bool straight = isBlack(sensorPins[3]) && isBlack(sensorPins[4]); // A3, A4 Black;
    int deviation = abs(analogRead(sensorPins[3]) - analogRead(sensorPins[4]));//the absolute value between A3 and A4 
    bool left = isBlack(sensorPins[6]) && isBlack(sensorPins[7]); // A6,A7 the first two at the left, black
    bool rightCheck = isBlack(sensorPins[2]) && isBlack(sensorPins[3]);
    bool leftCheck = isBlack(sensorPins[4]) && isBlack(sensorPins[5]);
    
    // read sensors to control movement
    if (straight && deviation < 200) {
        // when the two middle sensors detect black, move forwarde
        goStraight();
    } else if ((left && leftCheck || left && straight) && !right) {
        turnLeft();
    } else if (deviation > 200) {
      if(analogRead(sensorPins[3]) > analogRead(sensorPins[4])){
        toRightAdjust();
      } else {
        toLeftAdjust();
      }
    } else if (right && left) {
        turnRight();
    } else if (right && (straight || rightCheck)) {
        turnRight();
    } else if (deadEnd) {
        turnAround();
    }
}
