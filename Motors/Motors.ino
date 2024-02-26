// Motor control pins
const int motorPin1 = 2; // Replace with your actual pin numbers
const int motorPin2 = 3;
const int enablePin = 5;

void setup() {
  // Set motor control pins as outputs
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);

  // Initialize serial communication (optional)
  Serial.begin(9600);
}

void loop() {
  // Move the motor forward
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, HIGH);
  
  // Adjust the motor speed (0-255)
  analogWrite(enablePin, 220);

  // Optional: Print a message to the serial monitor
  Serial.println("Motor is moving forward");

  delay(2000); // Move forward for 2 seconds

  // Stop the motor
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);

  // Optional: Print a message to the serial monitor
  Serial.println("Motor stopped");

  delay(1000); // Pause for 1 second

  // Move the motor backward
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);

  // Adjust the motor speed (0-255)
  analogWrite(enablePin, 200);

  // Optional: Print a message to the serial monitor
  Serial.println("Motor is moving backward");

  delay(2000); // Move backward for 2 seconds

  // Stop the motor
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);

  // Optional: Print a message to the serial monitor
  Serial.println("Motor stopped");

  delay(1000); // Pause for 1 second
}
