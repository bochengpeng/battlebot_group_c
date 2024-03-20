const int motorA1 = 6;  // Motor A input 1
const int motorA2 = 9;  // Motor A input 2
const int motorB1 = 11;  // Motor B input 1
const int motorB2 = 5;  // Motor B input 2

// Rotation sensors

const int sensorR1 = 2;
const int sensorR2 = 3;

const int numSensors = 8; // Number of line sensors
const int sensorPins[numSensors] = {A0, A1, A2, A3, A4, A5, A6, A7}; // Analog input pins for the line sensors



// read sensor
bool isBlack(int sensorPin) {
    return analogRead(sensorPin) > 800; // when sensor value > 800 means on black
}


void goStraight() {
    int rightSpeed = 220;
    int leftSpeed = 235;
    
    //left wheel anticlockwise
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, HIGH);
    analogWrite(motorA2, leftSpeed);
    // right wheel anticlockwise
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    analogWrite(motorB1, rightSpeed);
}

void turnLeft() {
    int rightSpeed = 190;
    int leftSpeed = 200;
    
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    analogWrite(motorA1, leftSpeed);
    
   
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    analogWrite(motorB1, rightSpeed);

}

void toLeftAdjust() {
    int rightSpeed = 205;
    int leftSpeed = 280;
    
    //left wheel anticlockwise
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, HIGH);
    analogWrite(motorA2, leftSpeed);
    // right wheel anticlockwise
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    analogWrite(motorB1, rightSpeed);
}

void toRightAdjust() {
    int rightSpeed = 140;
    int leftSpeed = 220;
    
    //left wheel anticlockwise
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, HIGH);
    analogWrite(motorA2, leftSpeed);
    // right wheel anticlockwise
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    analogWrite(motorB1, rightSpeed);
}

void turnRight() {
    int rightSpeed = 190;
    int leftSpeed = 200;
    
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, HIGH);
    analogWrite(motorA2, leftSpeed);
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, HIGH);
    analogWrite(motorB2, rightSpeed);
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

    Serial.begin(9600);

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

    // Read sensor values
    int sensorValues[numSensors];
    for (int i = 0; i < numSensors; i++) {
      sensorValues[i] = analogRead(sensorPins[i]);
    }
    
    bool deadEnd = !isBlack(sensorPins[0]) && !isBlack(sensorPins[1]) && !isBlack(sensorPins[2]) && !isBlack(sensorPins[3]) && !isBlack(sensorPins[4]) && !isBlack(sensorPins[5]) && !isBlack(sensorPins[6]) && !isBlack(sensorPins[7]);
    bool right = isBlack(sensorPins[0]) && isBlack(sensorPins[1]); // A0,A1, the first two at the right, black
    bool straight = isBlack(sensorPins[3]) && isBlack(sensorPins[4]); // A3, A4 Black;
    int deviation = abs(analogRead(sensorPins[3]) - analogRead(sensorPins[4]));//the absolute value between A3 and A4 
    bool left = isBlack(sensorPins[6]) && isBlack(sensorPins[7]); // A6,A7 the first two at the left, black
    bool rightCheck = isBlack(sensorPins[2]) && isBlack(sensorPins[3]);
    bool leftCheck = isBlack(sensorPins[5]) && isBlack(sensorPins[6]);
    
    // read sensors to control movement
    if (!isBlack(sensorPins[0]) && !isBlack(sensorPins[1]) && !isBlack(sensorPins[2]) && isBlack(sensorPins[3]) && isBlack(sensorPins[4]) && !isBlack(sensorPins[5]) && !isBlack(sensorPins[6]) && !isBlack(sensorPins[7]) && deviation < 200) {
        // when the two middle sensors detect black, move forwarde
        goStraight();

    } else if (deviation > 200) {
      if(analogRead(sensorPins[3]) > analogRead(sensorPins[4])) {
        toRightAdjust();
      } else {
        toLeftAdjust();
      }
    } else if (isBlack(sensorPins[0]) && isBlack(sensorPins[1]) && isBlack(sensorPins[2]) && isBlack(sensorPins[3]) && isBlack(sensorPins[4]) && isBlack(sensorPins[5]) && isBlack(sensorPins[6]) && isBlack(sensorPins[7])) {
        turnRight();
    }
}
