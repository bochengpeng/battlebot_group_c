int L1 = 10;
int L2 = 9;
int R1 = 6;
int R2 = 5;
int counter = 0;
int motorspeed1 = 255;
int motorspeed2 = 255;
int fixServo = 0;
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

  pinMode(rightRotation, INPUT_PULLUP);//rotation sensor
  pinMode(leftRotation, INPUT_PULLUP);//rotation sensor

  attachInterrupt(digitalPinToInterrupt(rightRotation),updateRightRotations,CHANGE);
  attachInterrupt(digitalPinToInterrupt(leftRotation) ,updateLeftRotations,CHANGE);


  pinMode(ultraServo , OUTPUT);
  digitalWrite(ultraServo, LOW);
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
}

void updateLeftRotations(){
  noInterrupts();
  rotationLeft++;
  interrupts();
}

void resetRotations(){
  rotationRight = 0;
  rotationLeft = 0;
}

void moveForwardInRotations (int rotations){
  resetRotations();
  while(rotationRight < rotations){
     // Motor 1
     analogWrite(L1, 0);
     analogWrite(L2, motorspeed1);
     // Motor 2
     analogWrite(R1, motorspeed2);
     analogWrite(R2, 0);
    }
  stopRobot();
}


void moveForwardInRotationsWithAdjusting (int rotations){
  resetRotations();
//  while(rotationRight < rotations){
    distanceCmSide = getUltrasonicCm(trigPinSide , echoPinSide);
    if(distanceCmSide > 10)
    {
      analogWrite(L1, 0);
      analogWrite(L2, motorspeed1 - 10);
      analogWrite(R1, 200);
      analogWrite(R2, 0);
    }
    else if(distanceCmSide >= 9)
    {
      analogWrite(L1, 0);
      analogWrite(L2, motorspeed1 - 10);
      analogWrite(R1, 230);
      analogWrite(R2, 0);
    }
    else if(distanceCmSide < 6)
    {
      analogWrite(L1, 0);
      analogWrite(L2, 100);
      analogWrite(R1, motorspeed2);
      analogWrite(R2, 0);
    }
    else{
      // Motor 1
      analogWrite(L1, 0);
      analogWrite(L2, motorspeed1 - 10);
      // Motor 2
      analogWrite(R1, motorspeed2);
      analogWrite(R2, 0);
    }
//  }
//  stopRobot();
}

void moveBackwardInRotationsWithAdjusting (int rotations){
    resetRotations();
    distanceCmSide = getUltrasonicCm(trigPinSide , echoPinSide);
    if(distanceCmSide > 10)
    {
      analogWrite(L1, motorspeed1);
      analogWrite(L2, 0);
      analogWrite(R1, 0);
      analogWrite(R2, 200);
    }
    else if(distanceCmSide >= 9)
    {
      analogWrite(L1, motorspeed1);
      analogWrite(L2, 0);
      analogWrite(R1, 0);
      analogWrite(R2, 230);
    }
    else if(distanceCmSide < 6)
    {
      analogWrite(L1, 100);
      analogWrite(L2, 0);
      analogWrite(R1, 0);
      analogWrite(R2, motorspeed2);
    }
    else{
      // Motor 1
      analogWrite(L1, motorspeed1);
      analogWrite(L2, 0);
      // Motor 2
      analogWrite(R1, 0);
      analogWrite(R2, motorspeed2);
    }
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

void turnLeft(int motorspeed1 , int motorspeed2, int r) {
  resetRotations();

  while(r > rotationRight){
    // Motor 1
    analogWrite(L1, 0);
    analogWrite(L2, 0);
    // Motor 2
    analogWrite(R1, motorspeed2 - 15);
    analogWrite(R2, 0);
  }
  stopRobot();
}

void turnLeftReversed(int motorspeed1 , int motorspeed2, int r) {
  resetRotations();

  while(r > rotationRight){
    // Motor 1
    analogWrite(L1, 0);
    analogWrite(L2, 0);
    // Motor 2
    analogWrite(R1, 0);
    analogWrite(R2, motorspeed2 - 15);
  }
  stopRobot();
}

void turnRight(int motorspeed1 , int motorspeed2, int r) {
  resetRotations();

  while(r > rotationLeft){
    // Motor 1
    analogWrite(L1, 0);
    analogWrite(L2, motorspeed1- 15);
    // Motor 2
    analogWrite(R1, 0);
    analogWrite(R2, 0);
   } 
}

void turnRightReversed(int motorspeed1 , int motorspeed2, int r) {
  resetRotations();

  while(r > rotationLeft){
    // Motor 1
    analogWrite(L1, motorspeed1- 15);
    analogWrite(L2, 0);
    // Motor 2
    analogWrite(R1, 0);
    analogWrite(R2, 0);
   } 
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

void rotateUltraServo(int angle)
{
  for(int i = 0 ;  i<5 ; i++)
  {
    int pulseWidth = map(angle, -90 , 90 , 600 , 2400);
    digitalWrite(ultraServo , HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(ultraServo , LOW);
    delay(20);
  }
}

void loop() {
  distanceCmFront = getUltrasonicCm(trigPinFront , echoPinFront);
  distanceCmSide = getUltrasonicCm(trigPinSide , echoPinSide);
//  Serial.print("Side = ");
//  Serial.println(distanceCmSide);
//    Serial.print("Front = ");
//    Serial.println(distanceCmFront);

  if(distanceCmSide > 30)
  {
    Serial.println("I wanna turn rightt");
    stopRobot();
    moveForwardInRotations(20);
    stopRobot();
    turnRight(motorspeed1 , motorspeed2 , 35);
    stopRobot();
    delay(500);
    moveForwardInRotations(10);
    stopRobot();
  }
  else if(distanceCmFront < 12)
  {
    stopRobot();
    rotateUltraServo(90);
    distanceCmFront = getUltrasonicCm(trigPinFront , echoPinFront);

    if(distanceCmFront > 30)
    {
      Serial.println("I wanna turn left");
      rotateUltraServo(0);
      turnLeft(motorspeed1 , motorspeed2 , 30);
      stopRobot();
      delay(500);
      moveForwardInRotations(10);
      stopRobot();
    }
    else{
      moveForwardInRotations(10);
      stopRobot();
      turnRightReversed(motorspeed1 , motorspeed2 , 15);
      stopRobot();
      moveBackward(motorspeed1 , motorspeed2);
      delay(250);
      turnRightReversed(motorspeed1 , motorspeed2 , 15);
      stopRobot();
      moveBackward(motorspeed1 , motorspeed2);
      delay(200);
      stopRobot();
      turnLeft(motorspeed1 , motorspeed2 , 35);
      stopRobot();
      moveBackward(motorspeed1 , motorspeed2);
      delay(300);
      rotateUltraServo(0);
      stopRobot();
    }
  }
  else
  {
    moveForwardInRotationsWithAdjusting(5);
  }
}
