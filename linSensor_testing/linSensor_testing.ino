// Define line sensor pins
const int numSensors = 8; // Number of line sensors
const int sensorPins[numSensors] = {A0, A1, A2, A3, A4, A5, A6, A7}; // Analog input pins for the line sensors

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

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

  // Add a short delay before reading again
  delay(500);  // Adjust delay as needed
}
