#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <Adafruit_NeoPixel.h>

static const uint8_t PIN_MP3_TX = 2; // Connects to module's RX
static const uint8_t PIN_MP3_RX = 3; // Connects to module's TX
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);

DFRobotDFPlayerMini player;

const int trigPin = 4; // Trigger pin
const int echoPin = 5; // Echo pin

const int numLeds = 24; // Number of LEDs in the strip
const int ledPin = 6;   // Pin connected to the data input of the WS2812 strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLeds, ledPin, NEO_GRB + NEO_KHZ800);

int lastAudioTrack = -1;
unsigned long lastChangeTime = 0; // Variable to store the time of the last distance change

// Debounce delay in milliseconds
const unsigned long debounceDelay = 500;

void setup() {
  Serial.begin(9600);
  softwareSerial.begin(9600);

  if (player.begin(softwareSerial)) {
    Serial.println("OK");
    player.volume(20);
    strip.begin();
    strip.show();
  } else {
    Serial.println("Connecting to DFPlayer Mini failed!");
  }
}

void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  if (distance >= 0 && distance <= 48) { // Check if distance is within valid range
    int audioTrack = map(distance, 0, 48, 8, 1); // Map distance to reversed audio track

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
        for (int i = 0; i < numLeds; i++) {
          int hue = map(i, 0, numLeds, 0, 255);
          strip.setPixelColor(i, strip.ColorHSV(hue, 255, 255)); // Rainbow gradient
        }
        strip.show();

        lastAudioTrack = audioTrack; // Update lastAudioTrack
        lastChangeTime = millis();   // Update the last change time
      }
    }

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print(" cm, Audio Track: ");
    Serial.println(lastAudioTrack);
  }

  delay(10);
}
