
const int gripperPin = 10; // Pin connected to the servo signal wire
const long maxDistance = 200; 

void closeGripper() {
    digitalWrite(gripperPin, HIGH); // Start the pulse
    delayMicroseconds(1000); // Pulse width for neutral position (0 degrees)
    digitalWrite(gripperPin, LOW); // End the pulse
}

void openGripper() {
    digitalWrite(gripperPin, HIGH); // Start the pulse
    delayMicroseconds(1500); // Pulse width for opened position (90 degrees)
    digitalWrite(gripperPin, LOW); // End the pulse
}


void setup() {
  pinMode(gripperPin, OUTPUT);

}

void loop() {

  closeGripper();
  delay(2000);
  openGripper();
  delay(2000);
  closeGripper();

}
