// Define motor control pins
const int motorA1 = 6;  // Motor A input 1
const int motorA2 = 9;  // Motor A input 2
const int motorB1 = 3;  // Motor B input 1
const int motorB2 = 5;  // Motor B input 2

// Define line sensor pins
const int numSensors = 8; // Number of line sensors
const int sensorPins[numSensors] = {A0, A1, A2, A3, A4, A5, A6, A7}; // Analog input pins for the line sensors
const int middleSensorPin1 = A2;
const int middleSensorPin2 = A3;
const int middleSensorPin3 = A4;
const int middleSensorPin4 = A5;

// Define motor speeds
const int baseSpeedLeft = 240; // Base motor speed
const int baseSpeedRight = 230; // Base motor speed


bool moveForward;
bool adjustRight;
bool adjustLeft;

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

  // Start serial communication
  Serial.begin(9600);
}

void loop() {
  
  // Read line sensor values
  
  int middleSensor1Value = analogRead(middleSensorPin1);
  int middleSensor2Value = analogRead(middleSensorPin2);
  int middleSensor3Value = analogRead(middleSensorPin3);
  int middleSensor4Value = analogRead(middleSensorPin4);

  // Determine direction based on sensor readings
  if (middleSensor2Value > 550 && middleSensor3Value > 550) {
    moveForward = true;
  }
  else if (middleSensor2Value < 450 && middleSensor3Value > 550) {
    adjustRight = true;
  }
  else if (middleSensor2Value < 450 && middleSensor3Value > 550) {
    adjustLeft = true;
  }
  else {
    adjustLeft = false;
    adjustRight = false;
    moveForward = false;
  }
  

  // Move the robot
  move();
 
  //   Print sensor values for testing
  Serial.print("M1: ");
  Serial.print(middleSensor1Value);
  Serial.print("\tM2: ");
  Serial.print(middleSensor2Value);
  Serial.print("\tM3: ");
  Serial.print(middleSensor3Value);
  Serial.print("\tM4: ");
  Serial.println(middleSensor4Value);
  delay(100); // Delay for stability
 
}

// Function to move the robot
void move() {
  int leftSpeed, rightSpeed;
  // Adjust motor speeds based on direction adjustment
  if (moveForward) {
     rightSpeed = baseSpeedRight;
    leftSpeed = baseSpeedLeft;
     
    digitalWrite(motorA1, LOW);
    digitalWrite(motorB2, LOW);

    analogWrite(motorA2, leftSpeed);
    analogWrite(motorB1, rightSpeed);

    Serial.println("Forward");

  }
  else if (adjustRight) {
      rightSpeed = baseSpeedRight + 20; // Increase right motor speed
      Serial.println("R");
  }
  else if (adjustLeft) {
      leftSpeed = baseSpeedLeft + 20; // Increase left motor speed
      Serial.println("L");
    }
}