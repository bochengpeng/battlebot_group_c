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

void setup() {
  pixels.begin(); // Initialize NeoPixel strip
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
}
