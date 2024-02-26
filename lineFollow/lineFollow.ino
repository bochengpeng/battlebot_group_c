// Define motor control pins
const int motorA1 = 6;  // Motor A input 1
const int motorA2 = 9;  // Motor A input 2
const int motorB1 = 3;  // Motor B input 1
const int motorB2 = 5;  // Motor B input 2

// Define line sensor pins
const int numSensors = 8; // Number of line sensors
const int sensorPins[numSensors] = {A0, A1, A2, A3, A4, A5, A6, A7}; // Analog input pins for the line sensors

// Define motor speeds
const int baseSpeed = 500; // Base motor speed
const int maxSpeedDifference = 50; // Maximum speed difference between motors

void setup() {
  // Set motor control pins as OUTPUT
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  // Set line sensor pins as INPUT
  for (int i = 0; i < numSensors; i++) {
    pinMode(sensorPins[i], INPUT);
  }
}

void loop() {
  // Read line sensor values
  int sensorValues[numSensors];
  for (int i = 0; i < numSensors; i++) {
    sensorValues[i] = analogRead(sensorPins[i]);
  }

  // Calculate error (difference between sensor readings and desired line position)
  int total = 0;
  int weightedSum = 0;
  for (int i = 0; i < numSensors; i++) {
    total += sensorValues[i];
    weightedSum += i * sensorValues[i];
  }
  int error = (weightedSum / total) - (numSensors - 1) / 2;

  // Adjust motor speeds based on error
  int leftSpeed = baseSpeed + error * 5; // Adjust left motor speed based on error
  int rightSpeed = baseSpeed - error * 5; // Adjust right motor speed based on error

  // Limit the speed difference between motors
  leftSpeed = constrain(leftSpeed, 0, baseSpeed + maxSpeedDifference);
  rightSpeed = constrain(rightSpeed, 0, baseSpeed + maxSpeedDifference);

  // Move the robot
  move(leftSpeed, rightSpeed);
}

void move(int leftSpeed, int rightSpeed) {
  // Motor A
  if (leftSpeed >= 0) {
    digitalWrite(motorA1, LOW);
    analogWrite(motorA2, leftSpeed);
  } else {
    digitalWrite(motorA1, HIGH);
    analogWrite(motorA2, -leftSpeed);
  }

  // Motor B
  if (rightSpeed >= 0) {
    digitalWrite(motorB2, LOW);
    analogWrite(motorB1, rightSpeed);
  } else {
    digitalWrite(motorB2, HIGH);
    analogWrite(motorB1, -rightSpeed);
  }
}
