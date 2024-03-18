const int motorA1 = 6;  
const int motorA2 = 9;  
const int motorB1 = 3;  
const int motorB2 = 5;  
 
const int sensorPins[] = {A0, A1, A2, A3, A4, A5, A6, A7}; 
const int middleSensorPin1 = A2;
const int middleSensorPin2 = A3;
const int middleSensorPin3 = A4;
const int middleSensorPin4 = A5;


const int baseSpeedLeft = 240; 
const int baseSpeedRight = 230; 

void setup() {
  // Set motor control pins as OUTPUT
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  
  // Set line sensor pins as INPUT
  for (int i = 0; i < 8; i++) {
    pinMode(sensorPins[i], INPUT);
  }  

  // Start serial communication
  Serial.begin(9600);
}

void loop() {
  int middleSensor1Value = analogRead(middleSensorPin1);
  int middleSensor2Value = analogRead(middleSensorPin2);
  int middleSensor3Value = analogRead(middleSensorPin3);
  int middleSensor4Value = analogRead(middleSensorPin4);

  // Calculate an error value based on sensor readings
  int error = (middleSensor2Value + middleSensor3Value) - (middleSensor1Value + middleSensor4Value);

  // Adjust motor speeds based on the error
  int leftSpeed = baseSpeedLeft + error;
  int rightSpeed = baseSpeedRight - error;

  // Ensure motor speeds are within limits
  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

  digitalWrite(motorA1, LOW);
  digitalWrite(motorB2, LOW);

  analogWrite(motorA2, leftSpeed);
  analogWrite(motorB1, rightSpeed);

  Serial.print("Error: ");
  Serial.print(error);
  Serial.print("M1: ");
  Serial.print(middleSensor1Value);
  Serial.print("\tM2: ");
  Serial.print(middleSensor2Value);
  Serial.print("\tM3: ");
  Serial.print(middleSensor3Value);
  Serial.print("\tM4: ");
  Serial.print(middleSensor4Value);
  Serial.print("\tLeft Speed: ");
  Serial.print(leftSpeed);
  Serial.print("\tRight Speed: ");
  Serial.println(rightSpeed);
  

  delay(100);
}
