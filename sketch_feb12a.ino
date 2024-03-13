#include <Servo.h>
Servo myservo;

int M_R1 = 6;
int M_R2 = 9;
int M_L1 = 10;
int M_L2 = 11;
int S_Servo = 4;
int motorspeed1 = 255;
int motorspeed2 = 243;
int check = 0;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(M_R1 , INPUT);
  pinMode(M_R2 , INPUT);
  pinMode(M_L1 , INPUT);
  pinMode(M_L2 , INPUT);
  myservo.attach(S_Servo);
}

void stationary(){
  analogWrite(M_R1 , 0);
  analogWrite(M_R2 , 0);
  analogWrite(M_L1 , 0);
  analogWrite(M_L2 , 0);
  delay(500);
}

void forward(int num1 , int num2){
  analogWrite(M_R1 , 255);
  analogWrite(M_R2 , 0);
  analogWrite(M_L1 , 243);
  analogWrite(M_L2 , 0);
  delay(2000);
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
  delay(215);
  analogWrite(M_R1 , num1);
  analogWrite(M_R2 , 0);
  analogWrite(M_L1 , num2);
  analogWrite(M_L2 , 0);
  delay(200);
  analogWrite(M_R1 , 0);
  analogWrite(M_R2 , num1);
  analogWrite(M_L1 , num2);
  analogWrite(M_L2 , 0);
  delay(265);
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
  // put your main code here, to run repeatedly
  forward(motorspeed1 , motorspeed2);
  delay(5000);
  backward(motorspeed1 , motorspeed2);
  delay(5000);
  turnS_ServoRight();
  turnS_ServoForward();
  turnS_ServoLeft();

  turnS_ServoRight();
}
