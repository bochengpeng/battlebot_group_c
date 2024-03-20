int L1 = 10;
int L2 = 9;
int R1 = 6;
int R2 = 5;
int counter = 0;
int motorspeed1 = 234;
int motorspeed2 = 255;
int check = 1;
int ultraServo = 11; // servo for the ultrasonic sensor

int rightRotation = 2; //R1
int leftRotation = 3; //R2
volatile int rotationRight = 0;
volatile int rotationLeft = 0;

// Define pins for front the ultrasonic sensor
const int trigPinFront = 12; // Trigger pin Front
const int echoPinFront = 13; // Echo pin Front
const int trigPinSide = 7; // Trigger pin Side
const int echoPinSide = 8; // Echo pin Side

// Define variables for distance calculation
long durationFront;
long durationSide;

int distanceCmFront;
int distanceCmSide;

void setup() {

  pinMode(L1 , OUTPUT);
  pinMode(L2 , OUTPUT);
  pinMode(R1 , OUTPUT);
  pinMode(R2 , OUTPUT);

  // Set trigPin as an output and echoPin as an input
  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);

  pinMode(trigPinSide, OUTPUT);
  pinMode(echoPinSide, INPUT);

  pinMode(rightRotation, INPUT);//rotation sensor
  pinMode(leftRotation, INPUT);//rotation sensor

  attachInterrupt(digitalPinToInterrupt(rightRotation),updateRightRotations,CHANGE);
  attachInterrupt(digitalPinToInterrupt(leftRotation) ,updateLeftRotations,CHANGE);


  pinMode(ultraServo , OUTPUT);
  Serial.begin(9600);

}



void stopRobot()
{
  digitalWrite(L1, LOW);
  digitalWrite(L2, LOW);
  digitalWrite(R1, LOW);
  digitalWrite(R2, LOW);
}

void updateRightRotations(){
  noInterrupts();
  rotationRight++;
  interrupts();
  return;
}

void updateLeftRotations(){
  noInterrupts();
  rotationLeft++;
  interrupts();
  return;
}

void moveForwardInRotations (int rotations){

  if(distanceCmSide > 8)
  {
    analogWrite(L1, 0);
    analogWrite(L2, motorspeed1);
    analogWrite(R1, 180);
    analogWrite(R2, 0);
  }
  else if(distanceCmSide < 7)
  {
    analogWrite(L1, 0);
    analogWrite(L2, 180);
    analogWrite(R1, motorspeed2);
    analogWrite(R2, 0);
  }
  else if(rotationRight < rotations && rotationLeft < rotations){
  // Motor 1
  analogWrite(L1, 0);
  analogWrite(L2, motorspeed1);
  // Motor 2
  analogWrite(R1, motorspeed2);
  analogWrite(R2, 0);
  delay(10);
  }
  else
  {
    resetRotations();
    stopRobot();
  }
}
 
void resetRotations(){
  rotationRight = 0;
  rotationLeft = 0;
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

int getUltrasonicCm(int trigPin , int echoPin)
{
  long duration;
  int distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.034) / 2;

  return distance;
}

void rotateUltraServoLeft()
{
  for(int i = 0 ; i<=5 ; i++)
  {
    digitalWrite(ultraServo , HIGH);
    delayMicroseconds(2400);
    digitalWrite(ultraServo , LOW);
    delayMicroseconds(17600);
  }
}

void rotateUltraServo(int angle)
{
  for(int i = 0 ;  i<=10 ; i++)
  {
    int pulseWidth = map(angle, 90 , -90 , 600 , 2400);
    digitalWrite(ultraServo , HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(ultraServo , LOW);
    delay(20);
  }
}

int getDistanceLeft()
{
  stopRobot();
  rotateUltraServoLeft();
  return getUltrasonicCm(trigPinFront , echoPinFront);
}

void loop() {
  rotateUltraServo(-90);
  rotateUltraServo(0);
  
  distanceCmFront = getUltrasonicCm(trigPinFront , echoPinFront);
  distanceCmSide = getUltrasonicCm(trigPinSide , echoPinSide);
  
  Serial.println(distanceCmFront);
  if(distanceCmSide > 30)
  {
    moveForward(motorspeed1 , motorspeed2);
    delay(450);
    turnRight(motorspeed1 , motorspeed2);
    delay(450);
    moveForward(motorspeed1 , motorspeed2);
    delay(350);
  }
  else if(distanceCmFront < 11 && distanceCmSide < 30)
  {
    turnLeft(motorspeed1 , motorspeed2);
    delay(450);
    moveForward(motorspeed1 , motorspeed2);
    delay(350);
  }
  else 
  {
    Serial.println("Stuck in forward");
    moveForwardInRotations(20);
  }

}
