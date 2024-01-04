/*
connections:
For SD card module
CS = 10
SCK = 13
MOSI = 11
MISO = 12
VCC = 5V
GND = GND

For speaker
+ = 9
- = GND
*/

#include "SD.h"
#define SD_ChipSelectPin 10
#include "TMRpcm.h"
#include "SPI.h"

TMRpcm tmrpcm;

void setup()
{
tmrpcm.speakerPin=9;
Serial.begin(9600);
if(!SD.begin(SD_ChipSelectPin))
{
  Serial.println("SD fail");
  return;
}
tmrpcm.setVolume(6);
tmrpcm.play("abcd.wav");

}

void loop() {
  // put your main code here, to run repeatedly:

}
