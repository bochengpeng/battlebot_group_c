// Define motor control pins
const int motorA1 = 6;  // Motor A input 1
const int motorA2 = 9;  // Motor A input 2
const int motorB1 = 3;  // Motor B input 1
const int motorB2 = 5;  // Motor B input 2

// Define line sensor pins
const int numSensors = 8; // Number of line sensors
const int sensorPins[numSensors] = {A0, A1, A2, A3, A4, A5, A6, A7}; // Analog input pins for the line sensors

// Define motor speeds
const int baseSpeed = 800; // Base motor speed
const int turnSpeed = 500; // Motor speed for turning

// Array to store previous sensor values
int previousSensorValues[numSensors];

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
  // Read sensor values
  int sensorValues[numSensors];
  for (int i = 0; i < numSensors; i++) {
    sensorValues[i] = analogRead(sensorPins[i]);
  }

  // Print sensor values to serial monitor
  for (int i = 0; i < numSensors; i++) {
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(sensorValues[i]);
  }

  // Call a function to determine the motor speeds based on sensor readings
  lineFollowingAlgorithm(sensorValues);
}

// Function to implement a basic line-following algorithm
void lineFollowingAlgorithm(int sensorValues[]) {
  // Calculate error by comparing current sensor values with previous values
  int error = 0;
  for (int i = 0; i < numSensors; i++) {
    error += (i * sensorValues[i]);
  }

  // Make adjustments based on the error
  int leftSpeed = baseSpeed + (error * 0.1);
  int rightSpeed = baseSpeed - (error * 0.1);

  // Adjust speeds for turning
  if (sensorValues[0] < 800) {
    // Turn left
    leftSpeed = -turnSpeed;
    rightSpeed = turnSpeed;
  } else if (sensorValues[numSensors - 1] < 800) {
    // Turn right
    leftSpeed = turnSpeed;
    rightSpeed = -turnSpeed;
  }

   // Check if all sensor values indicate black
  bool allBlack = true;
  for (int i = 0; i < numSensors; i++) {
    if (sensorValues[i] < 700) {  // Adjust this threshold based on your sensor characteristics
      allBlack = false;
      break;
    }
  }

  // If all sensors detect black, move the robot forward
  if (allBlack) {
    move(baseSpeed, baseSpeed);
  } else {
    // Stop the motors if any sensor detects a non-black surface
    move(0, 0);
  }  
}




// Function to control motor movement
void move(int leftSpeed, int rightSpeed) {
  // Motor A
  if (leftSpeed >= 0) {
    digitalWrite(motorA2, LOW);
    analogWrite(motorA1, leftSpeed);
  } else {
    digitalWrite(motorA1, HIGH);
    analogWrite(motorA2, -leftSpeed);
  }

  // Motor B
  if (rightSpeed >= 0) {
    digitalWrite(motorB1, HIGH);
    analogWrite(motorB2, rightSpeed);
  } else {
    digitalWrite(motorB1, LOW);
    analogWrite(motorB2, -rightSpeed);
  }
}
