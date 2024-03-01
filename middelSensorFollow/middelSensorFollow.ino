// Define motor control pins
const int motorA1 = 6;  // Motor A input 1
const int motorA2 = 9;  // Motor A input 2
const int motorB1 = 3;  // Motor B input 1
const int motorB2 = 5;  // Motor B input 2

// Define line sensor pins
const int numSensors = 8; // Number of line sensors
const int sensorPins[numSensors] = {A0, A1, A2, A3, A4, A5, A6, A7}; // Analog input pins for the line sensors
const int middleSensorPin1 = A2; // Analog pin for the middle-left sensor
const int middleSensorPin2 = A3;
const int middleSensorPin3 = A4;
const int middleSensorPin4 = A5; // Analog pin for the middle-right sensor

// Define motor speeds
const int baseSpeed = 255; // Base motor speed

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
  bool moveForward;
  if (middleSensor2Value > 500 && middleSensor3Value > 500) moveForward = true;
  else moveForward = false;
  bool adjustRight = (middleSensor2Value < 500);
  bool adjustLeft = (middleSensor3Value < 500);

  // Move the robot
  move(baseSpeed, baseSpeed, moveForward, adjustRight, adjustLeft);

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
void move(int leftSpeed, int rightSpeed, bool forward, bool adjustRight, bool adjustLeft) {
  // Adjust motor speeds based on direction adjustment
  if (moveForward = true) {

    digitalWrite(motorA1, LOW);
    digitalWrite(motorB2, LOW);

    analogWrite(motorB1, baseSpeed);
    analogWrite(motorB2, baseSpeed);

  
    // Adjust the motor speed (0-255)
    analogWrite(enablePin, 220);

    // Optional: Print a message to the serial monitor
    Serial.println("Motor is moving forward");

    delay(2000); // Move forward for 2 seconds

    if (adjustRight) {
      rightSpeed = baseSpeed + 100; // Increase right motor speed
    } else if (adjustLeft) {
      leftSpeed = baseSpeed + 100; // Increase left motor speed
    }
    
  }
}
