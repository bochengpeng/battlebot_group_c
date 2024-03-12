
const int motorA1 = 6;  
const int motorA2 = 9;  
const int motorB1 = 3;  
const int motorB2 = 5;  

const int numSensors = 8; // Number of line sensors
const int sensorPins[numSensors] = {A0, A1, A2, A3, A4, A5, A6, A7}; 

const int lineThreshold = 700;

int heavyTurnAdjustment = 255;
int hardTurnAdjustment = 175;
int strongTurnAdjustment = 125;
int weakAdjustment = 25;


const int baseSpeed = 255;
const int startSpeed = 200;


void setup() {
    pinMode(motorA1, OUTPUT);
    pinMode(motorA2, OUTPUT);
    pinMode(motorB1, OUTPUT);
    pinMode(motorB2, OUTPUT);

    Serial.begin(9600);

    calibrate();  // Run calibration 
}


void loop() {

    int sensorValues[numSensors];

    for (int i = 0; i < numSensors; i++) {
        sensorValues[i] = analogRead(sensorPins[i]);
    }

    // Moving 
    moveRobot(sensorValues);

    
    // Print sensor values
    for (int i = 0; i < numSensors; i++) {
        Serial.print("S[");
        Serial.print(i);
        Serial.print("]: ");
        Serial.print(sensorValues[i]);
        Serial.print(" ");
    }
    Serial.println();
    delay(100);  // Adjust delay as needed
  }

void calibrate() {
    Serial.println("Calibration Phase");
    delay(2000);  // Wait for 2 seconds

    // Calibrate black and white thresholds
    calibrateSensorValues();

    Serial.println("Calibration Complete");
}


void moveRobot(int sensorValues[]) {
    bool straight = isBlack(sensorValues[3]) && isBlack(sensorValues[4]);
    bool left = isBlack(sensorValues[0]) && isBlack(sensorValues[1]);
    bool right = isBlack(sensorValues[6]) && isBlack(sensorValues[7]);
    bool rightTitled = isBlack(sensorValues[4]) && isBlack(sensorValues[5]);
    bool leftTitled = isBlack(sensorValues[2]) && isBlack(sensorValues[3]);

    if (straight) {
        goStraight();
        Serial.println("Straight");
    } else if (right && !left) {
        turnRight();
        Serial.println("Right");
    } else if (left && !right) {
        turnLeft();
        Serial.println("Left");
    } else if (rightTitled) {
        toLeftAdjust();
        Serial.println("Adjust Left");
    } else if (leftTitled) {
        toRightAdjust();
        Serial.println("Adjust Right");
    } else {
        stopMotors();
    }
}


void calibrateSensorValues() {
    int sensorValues[numSensors];

    // Initialize thresholds to the initial values
    blackThreshold = analogRead(sensorPins[0]);
    whiteThreshold = analogRead(sensorPins[0]);

    for (int i = 0; i < 100; i++) {
        // Read sensor values and update thresholds
        for (int j = 0; j < numSensors; j++) {
            sensorValues[j] = analogRead(sensorPins[j]);

            if (sensorValues[j] > blackThreshold) {
                blackThreshold = sensorValues[j];
            }

            if (sensorValues[j] < whiteThreshold) {
                whiteThreshold = sensorValues[j];
            }
        }

        delay(10);
    }

    Serial.print("Black Threshold: ");
    Serial.println(blackThreshold);
    Serial.print("White Threshold: ");
    Serial.println(whiteThreshold);
}
  

bool isBlack(int sensorValues[]) {
    // Adjust this threshold as needed
    int thresholdSum = 0;
    for (int i = 0; i < numSensors; i++) {
        thresholdSum += blackThreshold;
    }
    int averageThreshold = thresholdSum / numSensors;

    for (int i = 0; i < numSensors; i++) {
        if (sensorValues[i] > averageThreshold) {
            return true;
        }
    }
    return false;
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

void stopMotors() {
    // Stop the motors
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, LOW);
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, LOW);
}
