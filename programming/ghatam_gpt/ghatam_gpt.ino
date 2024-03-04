#include <Adafruit_NeoPixel.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#define PIN_LED_STRIP 6
#define NUM_LEDS 42  // Change this to match the number of LEDs on your strip

#define PIN_MP3_TX 2  // Connects to module's RX
#define PIN_MP3_RX 3  // Connects to module's TX
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN_LED_STRIP, NEO_GRB + NEO_KHZ800);
DFRobotDFPlayerMini player;

const int trigPin = 4;  // Trigger pin
const int echoPin = 5;  // Echo pin
const int soundSensorPin = A0;

int lastAudioTrack = -1;
unsigned long lastChangeTime = 0;  // Variable to store the time of the last distance change

// Debounce delay in milliseconds
const unsigned long debounceDelay = 500;

void setup() {
  Serial.begin(9600);
  softwareSerial.begin(9600);
  player.begin(softwareSerial);

  player.volume(20);

  strip.begin();
  strip.show();  // Initialize all pixels to 'off'
}

void loop() {
  // Measure distance using the ultrasonic sensor
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  if (distance >= 0 && distance <= 48) {  // Check if distance is within valid range
    int audioTrack = map(distance, 0, 48, 8, 1);  // Map distance to reversed audio track

    if (audioTrack != lastAudioTrack) {
      // Check if enough time has passed since the last change
      if (millis() - lastChangeTime > debounceDelay) {
        // Play a new audio track only if it's different from the last one and debounce time has passed
        player.play(audioTrack);
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.print(" cm, Playing Audio Track: ");
        Serial.println(audioTrack);

        // Rainbow gradient for LEDs
        for (int i = 0; i < NUM_LEDS; i++) {
          int hue = map(i, 0, NUM_LEDS, 0, 255);
          strip.setPixelColor(i, strip.ColorHSV(hue, 255, 255));  // Rainbow gradient
        }
        strip.show();

        lastAudioTrack = audioTrack;  // Update lastAudioTrack
        lastChangeTime = millis();    // Update the last change time
      }
    }

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print(" cm, Audio Track: ");
    Serial.println(lastAudioTrack);
  }

  // Sound sensor and LED logic
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

  colorWipe(strip.Color(0, 0, 255), numLedsToLight, 50);  // Blue color, adjust as needed

  delay(10);
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
