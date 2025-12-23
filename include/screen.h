#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>

class Screen
{
public:
    void init();
    TFT_eSPI tft;
};