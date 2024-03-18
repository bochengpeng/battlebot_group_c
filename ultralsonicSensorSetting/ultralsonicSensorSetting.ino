const int trigPin = 7;
const int echoPin = 8;
const int gripperPin = 10;
const long maxDistance = 40;
const long minDistance = 11;

void closeGripper() {
    digitalWrite(gripperPin, HIGH); // Start the pulse
    delayMicroseconds(960); // Pulse width for neutral position (0 degrees)
    digitalWrite(gripperPin, LOW); // End the pulse
}

void openGripper() {
    digitalWrite(gripperPin, HIGH); // Start the pulse
    delayMicroseconds(1700); // Pulse width for opened position (90 degrees)
    digitalWrite(gripperPin, LOW); // End the pulse
}

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(gripperPin, OUTPUT);

}

void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(1);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.0343) / 2;  // Calculate distance in cm
    if (distance <= maxDistance) {
      openGripper();  // Open gripper
      if (distance <= minDistance){
        closeGripper(); // Close gripper 
      }  
     }  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(1000);  // Delay for stability
  
}
