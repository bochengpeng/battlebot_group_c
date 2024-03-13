#include <Servo.h>
Servo myservo;

int M_R1 = 6;
int M_R2 = 5;
int M_L1 = 10;
int M_L2 = 9;
int S_Servo = 4;
int motorspeed1 = 255;
int motorspeed2 = 243;
int check = 0;

// Define pins for the ultrasonic sensor
const int trigPin = 2; // Trigger pin
const int echoPin = 3; // Echo pin
// Define variables for distance calculation
long duration;
int distance_cm;


void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(M_R1 , OUTPUT);
  pinMode(M_R2 , OUTPUT);
  pinMode(M_L1 , OUTPUT);
  pinMode(M_L2 , OUTPUT);
  myservo.attach(S_Servo);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
}

void stationary(){
  analogWrite(M_R1 , 0);
  analogWrite(M_R2 , 0);
  analogWrite(M_L1 , 0);
  analogWrite(M_L2 , 0);
  delay(500);
}

void forward(int num1 , int num2){
  digitalWrite(M_R1 , num1);
  digitalWrite(M_R2 , 0);
  digitalWrite(M_L1 , 0);
  digitalWrite(M_L2 , 200);
}

void backward(int num1 , int num2){
  analogWrite(M_L1 , 0);
  analogWrite(M_L2 , num1);
  delay(40);
  analogWrite(M_R1 , 0);
  analogWrite(M_R2 , num1);
  analogWrite(M_L2 , num2);
  analogWrite(M_L1 , 0);
  delay(5000);
  analogWrite(M_R1 , 0);
  analogWrite(M_R2 , 0);
  analogWrite(M_L1 , 0);
  analogWrite(M_L2 , 0);
  delay(1000);
}

void turnRight(int num1 , int num2){
  analogWrite(M_R1 , 0);
  analogWrite(M_R2 , num1);
  analogWrite(M_L1 , num2);
  analogWrite(M_L2 , 0);
  delay(500);
  analogWrite(M_R1 , num1);
  analogWrite(M_R2 , 0);
  analogWrite(M_L1 , num2);
  analogWrite(M_L2 , 0);
  delay(200);
  analogWrite(M_R1 , 0);
  analogWrite(M_R2 , num1);
  analogWrite(M_L1 , num2);
  analogWrite(M_L2 , 0);
  delay(500);
  stationary();
}

void turnLeft(int num1 , int num2){
  analogWrite(M_R1 , num1);
  analogWrite(M_R2 , 0);
  analogWrite(M_L1 , 0);
  analogWrite(M_L2 , num2);
  delay(215);
  analogWrite(M_R1 , num1);
  analogWrite(M_R2 , 0);
  analogWrite(M_L1 , num2);
  analogWrite(M_L2 , 0);
  delay(200);
  analogWrite(M_R1 , num1);
  analogWrite(M_R2 , 0);
  analogWrite(M_L1 , 0);
  analogWrite(M_L2 , num2);
  delay(265);
  stationary();
}

void turnS_ServoRight(){
  myservo.write(180);
  delay(1000);
}

void turnS_ServoForward(){
  myservo.write(90);
  delay(1000);
}

void turnS_ServoLeft(){
  myservo.write(0);
  delay(1000);
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

  Serial.print("Distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");


  // Check if the object is within 5cm
  if (distance_cm <= 5) {
    turnRight(motorspeed1 , motorspeed2);
  }
  else forward(motorspeed1 , motorspeed2);
  // Wait for a short delay before taking another measurement
  delay(1000);
  
}
