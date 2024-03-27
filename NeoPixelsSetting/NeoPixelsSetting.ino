#include <Adafruit_NeoPixel.h>

#define PIN        13
#define NUM_PIXELS 4

// Create an instance of the Adafruit_NeoPixel class
Adafruit_NeoPixel pixels(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

// NeoPixel colors
#define COLOR_TEAL    pixels.Color(0, 128, 128)
#define COLOR_YELLOW  pixels.Color(255, 255, 0)
#define COLOR_BLUE   pixels.Color(0, 0, 255)
#define COLOR_PURPLE pixels.Color(255, 0, 255)
#define COLOR_RED pixels.Color(0, 255, 0)

#define gripperPin 10 // Pin connected to the servo signal wire
// Define servo pulse width constants
#define minPulseWidth 910 // Minimum pulse width in microseconds
#define maxPulseWidth 2000 // Maximum pulse width in microseconds

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

void grabCone() {
  gripOpen(); // Open the gripper
  delay(1000); // Wait for a duration to simulate grabbing the cone
  gripClose(); // Close the gripper
}

void setup() {
  pixels.begin(); // Initialize NeoPixel strip
  pinMode(gripperPin, OUTPUT);
}

void setColor(uint32_t color) {
  for (int i = 0; i < NUM_PIXELS; i++) {
    pixels.setPixelColor(i, color); // Set color for each pixel
  }
  pixels.show(); // Update pixels
}

void loop() {
  // Example: Set color to purple
  setColor(COLOR_PURPLE);
  delay(1000); // Delay for demonstration
  setColor(COLOR_YELLOW);
  delay(1000); 
  setColor(COLOR_BLUE);
  delay(1000); 
  setColor(COLOR_TEAL);
  delay(1000); 
  setColor(COLOR_RED);
  delay(1000); 
   grabCone();
  delay(2000);
}
