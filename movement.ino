int L1 = 10;
int L2 = 9;
int R1 = 6;
int R2 = 5;
int counter = 0;
int motorspeed1 = 234;
int motorspeed2 = 255;
int check = 0;

// Define pins for the ultrasonic sensor
const int trigPin = 2; // Trigger pin
const int echoPin = 3; // Echo pin

// Define variables for distance calculation
long duration = 1000;
int distance_cm = 100;

void setup() {

  pinMode(L1 , OUTPUT);
  pinMode(L2 , OUTPUT);
  pinMode(R1 , OUTPUT);
  pinMode(R2 , OUTPUT);

  // Set trigPin as an output and echoPin as an input
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.begin(9600);

}

void moveForward(int motorspeed1 , int motorspeed2) {
  // Motor 1
  analogWrite(L1, 0);
  analogWrite(L2, motorspeed1);
  // Motor 2
  analogWrite(R1, motorspeed2);
  analogWrite(R2, 0);
}

void moveBackward(int motorspeed1 , int motorspeed2) {
  // Motor 1
  analogWrite(L1, motorspeed1);
  analogWrite(L2, 0);
  // Motor 2
  analogWrite(R1, 0);
  analogWrite(R2, motorspeed2);
}

void turnLeft(int motorspeed1 , int motorspeed2) {
  // Motor 1
  analogWrite(L1, motorspeed1);
  analogWrite(L2, 0);
  // Motor 2
  analogWrite(R1, motorspeed2);
  analogWrite(R2, 0);
}

void turnRight(int motorspeed1 , int motorspeed2) {
  // Motor 1
  analogWrite(L1, 0);
  analogWrite(L2, motorspeed1);
  // Motor 2
  analogWrite(R1, 0);
  analogWrite(R2, motorspeed2);
}

void loop() {
  // Clear the trigger pin before sending the signal
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Send a 10 microsecond pulse to the trigger pin
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the duration of the pulse on the echo pin
  duration = pulseIn(echoPin, HIGH);

  // Calculate distance in centimeters
  distance_cm = duration * 0.034 / 2;
  //initialize the distance_cm variable when u start the robot to not make it turn right instatly
  if(check == 0)
  {
    distance_cm = 11;
    check = 1;
  }
  
  // Print the distance to the serial monitor
  Serial.print("Distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  // Check if the object is within 10cm
  if (distance_cm <= 10) {
    Serial.println("Object detected at 5cm or less!");
    // Turn right
    turnRight(motorspeed1 , motorspeed2);
    delay(450);
  } else {
    // Move forward if no obstacle detected
    moveForward(motorspeed1 , motorspeed2);
  }
}
