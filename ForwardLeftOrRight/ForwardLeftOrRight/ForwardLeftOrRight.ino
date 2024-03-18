const int motorA1 = 6;  // Motor A input 1
const int motorA2 = 9;  // Motor A input 2
const int motorB1 = 3;  // Motor B input 1
const int motorB2 = 5;  // Motor B input 2

// 定义机器人的旋转时间（毫秒）
const int rotateTime = 2000; // 1秒

void turnRight() {
    int speed = 200;
    // 左侧轮子逆时针旋转
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, HIGH);
    analogWrite(motorA2, speed);
    // 右侧轮子顺时针旋转
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, HIGH);
    analogWrite(motorA2, speed);
}

void turnLeft() {
    int speed = 200;
    // 左侧轮子顺时针旋转
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    analogWrite(motorA1, speed);
    // 右侧轮子逆时针旋转
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    analogWrite(motorA1, speed);
}

void moveForward() {
    int speed = 255;
    // 左侧轮子逆时针旋转
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, HIGH);
    analogWrite(motorA2, speed);
    // 右侧轮子顺时针旋转
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    analogWrite(motorB1, speed);
}

void setup() {
    // 初始化引脚和其他设置
    pinMode(motorA1, OUTPUT);
    pinMode(motorA2, OUTPUT);
    pinMode(motorB1, OUTPUT);
    pinMode(motorB2, OUTPUT);
}

void loop() {
  turnLeft();
}
