const int motorA1 = 6;  // Motor A input 1
const int motorA2 = 9;  // Motor A input 2
const int motorB1 = 11;  // Motor B input 1
const int motorB2 = 5;  // Motor B input 2

// Rotation sensors

#define ROTATION_SENSOR_LEFT 2
#define ROTATION_SENSOR_RIGHT 3

const int numSensors = 8; // Number of line sensors
const int sensorPins[numSensors] = {A0, A1, A2, A3, A4, A5, A6, A7}; // Analog input pins for the line sensors


volatile int countL = 0;
volatile int countR = 0;


void ISR_L() {
  countL++;
}


void ISR_R() {
  countR++;
}


// read sensor
bool isBlack(int sensorPin) {
  return analogRead(sensorPin) > 800; // when sensor value > 800 means on black
}


void goStraight() {


  int rightSpeed = 220;
  int leftSpeed = 235;

  //left wheel anticlockwise
  digitalWrite(motorA1, LOW);
  analogWrite(motorA2, leftSpeed);

  // right wheel anticlockwise
  digitalWrite(motorB2, LOW);
  analogWrite(motorB1, rightSpeed);


}

void beforeTurning() {

  countL = 0;
  countR = 0;

  while (countL < 6 && countR < 6) {
    int rightSpeed = 220;
    int leftSpeed = 235;

    //left wheel anticlockwise
    digitalWrite(motorA1, LOW);
    analogWrite(motorA2, leftSpeed);

    // right wheel anticlockwise
    digitalWrite(motorB2, LOW);
    analogWrite(motorB1, rightSpeed);
  }
}

void beforeTurningLeft() {

  countL = 0;
  countR = 0;

  while (countL < 6 && countR < 6) {
    int rightSpeed = 220;
    int leftSpeed = 235;

    //left wheel anticlockwise
    digitalWrite(motorA1, LOW);
    analogWrite(motorA2, leftSpeed);

    // right wheel anticlockwise
    digitalWrite(motorB2, LOW);
    analogWrite(motorB1, rightSpeed);

    if (isBlack(sensorPins[0])) {
      beforeTurning();
      turnRight();
      break;
    }
  }
}

void beforeTurningAround() {

  countL = 0;
  countR = 0;

  while (countL < 18 && countR < 18) {
    int rightSpeed = 220;
    int leftSpeed = 235;

    //left wheel anticlockwise
    digitalWrite(motorA1, LOW);
    analogWrite(motorA2, leftSpeed);

    // right wheel anticlockwise
    digitalWrite(motorB2, LOW);
    analogWrite(motorB1, rightSpeed);
  }
}

void turnLeftAround() {

  countL = 0;
  countR = 0;

  while (countR < 19 && countL < 19) {
    int rightSpeed = 235;
    int leftSpeed = 245;

    analogWrite(motorA1, leftSpeed);
    digitalWrite(motorA2, LOW);

    digitalWrite(motorB2, LOW);
    analogWrite(motorB1, rightSpeed);
  }

}

void turnLeftAround2() {

  countL = 0;
  countR = 0;

  while ((!isBlack(sensorPins[3]) || !isBlack(sensorPins[4])) || (countR < 19 && countL < 19)) {
    int rightSpeed = 210;
    int leftSpeed = 210;

    analogWrite(motorA1, leftSpeed);
    digitalWrite(motorA2, LOW);

    digitalWrite(motorB2, LOW);
    analogWrite(motorB1, rightSpeed);
  }

}

void turnLeft() {

  countL = 0;
  countR = 0;

  while ((!isBlack(sensorPins[3]) || !isBlack(sensorPins[4])) || (countR < 19)) {
    int rightSpeed = 210;
    int leftSpeed = 210;

    analogWrite(motorA1, LOW);
    digitalWrite(motorA2, LOW);

    digitalWrite(motorB2, LOW);
    analogWrite(motorB1, rightSpeed);
  }

}

void toLeftAdjust() {
  int rightSpeed = 230;
  int leftSpeed = 150;


  digitalWrite(motorA1, LOW);
  analogWrite(motorA2, leftSpeed);

  digitalWrite(motorB2, LOW);
  analogWrite(motorB1, rightSpeed);
}

void toRightAdjust() {
  int rightSpeed = 150;
  int leftSpeed = 245;


  digitalWrite(motorA1, LOW);
  analogWrite(motorA2, leftSpeed);

  digitalWrite(motorB2, LOW);
  analogWrite(motorB1, rightSpeed);
}

void turnRight() {

  countL = 0;
  countR = 0;

  while ((!isBlack(sensorPins[3]) || !isBlack(sensorPins[4])) || (countL < 19)) {

    int rightSpeed = 220;
    int leftSpeed = 240;

    digitalWrite(motorA1, LOW);
    analogWrite(motorA2, leftSpeed);

    digitalWrite(motorB1, LOW);
    analogWrite(motorB2, LOW);
  }

}

void turnAround() {

  countL = 0;
  countR = 0;

  while (countL < 37 && countR < 40) {
    
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, HIGH);
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, HIGH);
  }

}

void moveStop() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
}

void setup() {
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  Serial.begin(9600);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);

  pinMode(ROTATION_SENSOR_LEFT, INPUT_PULLUP);
  pinMode(ROTATION_SENSOR_RIGHT, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ROTATION_SENSOR_LEFT), ISR_L, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ROTATION_SENSOR_RIGHT), ISR_R, CHANGE);

}

void loop() {

  maze();

}

void checkSensors(bool sensorValues[]) {
  for (int i = 0; i < numSensors; ++i) {
    sensorValues[i] = isBlack(sensorPins[i]);
  }
}

void maze() {

  // Array to store sensor values
  bool sensorValues[numSensors];

  checkSensors(sensorValues);

  bool deadEnd = !isBlack(sensorPins[0]) && !isBlack(sensorPins[1]) && !isBlack(sensorPins[2]) && !isBlack(sensorPins[3]) && !isBlack(sensorPins[4]) && !isBlack(sensorPins[5]) && !isBlack(sensorPins[6]) && !isBlack(sensorPins[7]);
  bool right = isBlack(sensorPins[0]); // A0,A1, the first two at the right, black
  bool straight = isBlack(sensorPins[3]) && isBlack(sensorPins[4]);
  bool adjustRight =  isBlack(sensorPins[2]) || (!isBlack(sensorPins[2]) && isBlack(sensorPins[3]) && !isBlack(sensorPins[4]));
  bool adjustLeft = isBlack(sensorPins[5]) || (!isBlack(sensorPins[5]) && isBlack(sensorPins[4]) && !isBlack(sensorPins[3]));
  bool left = isBlack(sensorPins[7]); // A6,A7 the first two at the left, black
  bool rightCheck = isBlack(sensorPins[2]) && isBlack(sensorPins[3]);
  bool leftCheck = isBlack(sensorPins[5]) && isBlack(sensorPins[6]);
  bool tCross = isBlack(sensorPins[0]) && isBlack(sensorPins[1]) && isBlack(sensorPins[2]) && isBlack(sensorPins[3]) && isBlack(sensorPins[4]) && isBlack(sensorPins[5]) && isBlack(sensorPins[6]) && isBlack(sensorPins[7]);


  if (straight) {
    goStraight();
    Serial.println("Straight");

  }

  else if (adjustRight && !adjustLeft) {
    toRightAdjust();
    Serial.println("ADRight");
  } else if (adjustLeft && !adjustRight) {
    toLeftAdjust();
    Serial.println("ADLeft");
  }

  if (tCross || right) {
    beforeTurning();
    turnRight();
    Serial.println("Right");
  } else if (left && !right) {
    beforeTurningLeft();
    Serial.println("BeforeLeft");
    checkSensors(sensorValues);
    bool straight = !isBlack(sensorPins[3]) && !isBlack(sensorPins[4]);
    
    if (straight) {
      turnLeft();
      moveStop();
      Serial.println("LeftTurn");
    }
  }

  if (deadEnd) {

      beforeTurningAround();
      moveStop();
      delay(500);
      turnAround();
      moveStop();
      delay(1000);
      Serial.println("TurnAround");
      
      checkSensors(sensorValues);
      bool adjustRightAfterTurn = isBlack(sensorPins[0]) || isBlack(sensorPins[1]) || isBlack(sensorPins[2]);
      bool adjustRight =  isBlack(sensorPins[2]) || (!isBlack(sensorPins[2]) && isBlack(sensorPins[3]) && !isBlack(sensorPins[4]));
      bool adjustLeft = isBlack(sensorPins[5]) || (!isBlack(sensorPins[5]) && isBlack(sensorPins[4]) && !isBlack(sensorPins[3]));
      bool adjustLeftAfterTurn = isBlack(sensorPins[7]) || isBlack(sensorPins[6]) || isBlack(sensorPins[5]);

      if (adjustRightAfterTurn && !adjustLeft) {
          while (!isBlack(sensorPins[3]) && !isBlack(sensorPins[4])) {
            toRightAdjust();
          }
      } else if (adjustLeftAfterTurn && !adjustRight) {
          while (!isBlack(sensorPins[3]) && !isBlack(sensorPins[4])) {
            toLeftAdjust();
          }
      }
      
    }
    
}
