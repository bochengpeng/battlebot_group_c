#define TRIGPIN 7
#define ECHOPIN 8
#define GRIPPER_PIN 10 
const long minDistance = 30;

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

long startTrigger() {
    long duration, distance;
    digitalWrite(TRIGPIN, LOW);
    delayMicroseconds(1);
    digitalWrite(TRIGPIN, HIGH);
    delayMicroseconds(1);
    digitalWrite(TRIGPIN, LOW);
    duration = pulseIn(ECHOPIN, HIGH);
    distance = (duration * 0.0343) / 2;  // Calculate distance in cm

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    delay(500);  // Delay for stability

    return distance;
}


void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(gripperPin, OUTPUT);

}

void loop() {

    bool lineMaze = false;

    if(startTrigger() <= minDistance){
       delay(1500);
       lineMaze = true;
    }

    if(lineMaze == true){
      if(startingPoint == true && endGame == false){
        delay(80);//time for removing hands
        lineCountAndGrabing();
      }
      
      if(startingPoint == false && endGame == false){
         maze();
      }
  
      if(startingPoint == false && endGame == true){
        finalDrop();
      }     
   } 
