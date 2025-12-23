#include <Arduino.h>
#include "screen.h"
#include "graph.h"
#include "gauge.h"
#include "senderInput.h"

Screen screen;
Graph graph;
Gauge gauge;
SenderInput senderInput;

void setup()
{
  Serial.begin(115200);

  screen.init();
  gauge.init();

  // GPIO34, Rs empty ~15Ω, Rs full ~270Ω
  senderInput.init(34, 15.0f, 270.0f);
}

void loop()
{
  // Get value from sender
  float percent = senderInput.readPercent();

  // Update screen
  gauge.updateFuelLevel(percent);
  graph.addDataPoint(percent);

  delay(100);
}