#include "SD.h"
#define SD_ChipSelectPin 10
#include "TMRpcm.h"
#include "SPI.h"
#include <Adafruit_NeoPixel.h>

#define TRIG_PIN 5
#define ECHO_PIN 4
#define LED_PIN 6
#define SPEAKER_PIN 9  // Connect the speaker to this digital pin
#define LED_COUNT 24

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
TMRpcm tmrpcm;
const char *currentSong = "";

void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
    ;
  }

  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_ChipSelectPin))
  {
    Serial.println("SD fail");
    return;
  }
  Serial.println("Initialization done.");

  tmrpcm.speakerPin = SPEAKER_PIN;

  strip.begin();
  strip.show();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop()
{
  long duration, distance;

  // Trigger ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the echo pulse
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate distance in cm
  distance = (duration * 0.0343) / 2;

  // Perform actions based on distance
  if (!tmrpcm.isPlaying()) // Check if no song is currently playing
  {
    if (distance >= 0 && distance <= 5)
    {
      playAudio("abcd.wav");
      illuminateRainbow(3);
      currentSong = "abcd.wav";
    }
    else if (distance > 5 && distance <= 10)
    {
      playAudio("efgh.wav");
      illuminateRainbow(6);
      currentSong = "efgh.wav";
    }
    else if (distance > 10 && distance <= 15)
    {
      playAudio("ijkl.wav");
      illuminateRainbow(9);
      currentSong = "ijkl.wav";
    }
    // ... (other distance conditions)
  }

  // Write distance to SD card
  File myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile)
  {
    myFile.print("Distance: ");
    myFile.print(distance);
    myFile.println(" cm");
    myFile.close();
  }

  // Gradually turn off the LEDs if no sound is detected
  if (!tmrpcm.isPlaying()) // Check if no sound is playing
  {
    turnOffLEDsOneByOne();
  }
  else
  {
    // Update the LED strip
    strip.show();
  }

  delay(1000); // Add a delay to avoid rapid readings
}

void playAudio(const char *filename)
{
  if (tmrpcm.isPlaying())
  {
    tmrpcm.stopPlayback();
  }
  tmrpcm.play(filename);
}

void illuminateRainbow(int numLEDs)
{
  // Create a rainbow gradient effect on the specified number of LEDs
  for (int i = 0; i < numLEDs; i++)
  {
    int hue = map(i, 0, numLEDs - 1, 0, 255);
    strip.setPixelColor(i, strip.ColorHSV(hue));
    strip.show();
    delay(50); // Adjust the delay for the desired speed
  }
}

void turnOffLEDsOneByOne()
{
  for (int i = strip.numPixels() - 1; i >= 0; i--)
  {
    strip.setPixelColor(i, strip.Color(0, 0, 0)); // Turn off the LED
    strip.show();
    delay(50); // Adjust the delay for the desired speed
  }
}
