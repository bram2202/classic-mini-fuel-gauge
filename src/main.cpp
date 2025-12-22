#include <Arduino.h>
#include <TFT_eSPI.h>
#include "screen.h"
#include "senderInput.h"

Screen screen;
SenderInput senderInput;

void setup()
{
  Serial.begin(115200);

  // Initialize screen and joystick
  screen.init();
  senderInput.init(34, 15.0f, 270.0f); // GPIO34, Rs empty ~15Ω, Rs full ~270Ω
}

void loop()
{
  // Get value from sender
  float percent = senderInput.readPercent();

  // Update screen
  screen.updateFuelLevel(percent);

  delay(100);
}