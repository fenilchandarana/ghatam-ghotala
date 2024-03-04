#include <Adafruit_NeoPixel.h>

#define PIN_LED_STRIP 6
#define NUM_LEDS 42  // Change this to match the number of LEDs on your strip

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN_LED_STRIP, NEO_GRB + NEO_KHZ800);

const int soundSensorPin = A0;

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  int soundSensorValue = analogRead(soundSensorPin);

  // Map the analog reading to the number of LEDs
  int numLedsToLight = map(soundSensorValue, 0, 300, 0, NUM_LEDS);

  // Ensure the number of LEDs to light is within the valid range
  numLedsToLight = constrain(numLedsToLight, 0, NUM_LEDS);

  // Display the mapped value and light up the corresponding number of LEDs
  Serial.print("Analog Reading: ");
  Serial.print(soundSensorValue);
  Serial.print(", Number of LEDs to Light: ");
  Serial.println(numLedsToLight);

  colorWipe(strip.Color(255, 0, 0), numLedsToLight, 50);  // Red color, adjust as needed
}

// Fill the first numLeds LEDs with the specified color
void colorWipe(uint32_t color, int numLeds, int wait) {
  for (int i = 0; i < numLeds; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
  delay(wait);
  // Turn off the remaining LEDs
  for (int i = numLeds; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
}
