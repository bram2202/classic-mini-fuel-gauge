#include <Arduino.h>
#include "screen.h"

void Screen::init()
{
    tft.init();

    // Set screen orientation and clear to black
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);
}
