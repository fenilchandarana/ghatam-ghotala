#include <Adafruit_NeoPixel.h>

#define PIN            6  // Pin where the data line is connected to the Arduino
#define NUMPIXELS      20 // Number of pixels in the LED strip

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const int soundSensorPin = A0; // Analog pin for the sound sensor

void setup() {
  Serial.begin(9600); // Initialize serial communication
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  int soundValue = analogRead(soundSensorPin);

  // Output sound value to serial monitor
//  Serial.print("Sound Value: ");
  Serial.println(soundValue);
delay(1);

  // Map the soundValue to the corresponding LED range
  int ledCount = map(soundValue, 0, 300, 0, NUMPIXELS);

  // Ensure the ledCount is within bounds
  ledCount = constrain(ledCount, 0, NUMPIXELS);

  // Gradually turn on the LEDs based on the LED count
  for (int i = 0; i < ledCount; i++) {
    // Calculate rainbow color based on LED index
    uint32_t color = Wheel(map(i, 0, NUMPIXELS, 30, 150) % 255);
    strip.setPixelColor(i, color);
  }

  // Gradually turn off the LEDs if no sound is detected
  if (soundValue < 50) { // Adjust the threshold as needed
    for (int i = NUMPIXELS - 1; i >= 0; i--) {
      strip.setPixelColor(i, strip.Color(0, 0, 0)); // Turn off the LED
      strip.show();
      delay(0); // Adjust the delay for the desired speed
    }
  } else {
    // Update the LED strip
    strip.show();
  }

  delay(100); // Add a small delay for stability
}

// Function to generate rainbow color gradient
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
