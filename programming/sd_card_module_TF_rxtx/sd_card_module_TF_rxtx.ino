/*
connections:
For SD card TF module
TX = 2
Rx = 3
VCC = 5V
GND = GND
SPK_1 = + of speaker
SPK_2 = - of speaker

For speaker
+ = SPK_1
- = SPK_2
*/

#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// Use pins 2 and 3 to communicate with DFPlayer Mini
static const uint8_t PIN_MP3_TX = 3; // Connects to module's RX 
static const uint8_t PIN_MP3_RX = 2; // Connects to module's TX 
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);

// Create the Player object
DFRobotDFPlayerMini player;

void setup() {

  // Init USB serial port for debugging
  Serial.begin(9600);
  // Init serial port for DFPlayer Mini
  softwareSerial.begin(9600);

  // Start communication with DFPlayer Mini
  if (player.begin(softwareSerial)) {
   Serial.println("OK");

    // Set volume to maximum (0 to 30).
    player.volume(20);
    player.play(2);
 
  } else {
    Serial.println("Connecting to DFPlayer Mini failed!");
  }
}

void loop() {

   }
