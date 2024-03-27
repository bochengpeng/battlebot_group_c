
const int gripperPin = 10; // Pin connected to the servo signal wire
// Define servo pulse width constants
const int minPulseWidth = 910; // Minimum pulse width in microseconds
const int maxPulseWidth = 2000; // Maximum pulse width in microseconds

// Function to set servo angle 
void setServoAngle(int angle, int servoPin)
{
  int pulseWidth = map(angle, 0, 180, minPulseWidth, maxPulseWidth);
  digitalWrite(servoPin, HIGH); // Start the pulse
  delayMicroseconds(pulseWidth); // Wait for the pulse width
  digitalWrite(servoPin, LOW); // End the pulse
  delay(20); // wait for servo to settle
}

void gripOpen()
{
  setServoAngle(90, gripperPin);
}

void gripClose()
{
  setServoAngle(0, gripperPin);
}


void setup() {
  pinMode(gripperPin, OUTPUT);

}

void loop() {

gripOpen();
delay(500);
gripClose();
delay(2000);

}
