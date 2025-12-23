#pragma once

#include <Arduino.h>

class Gauge
{
public:
    void init();
    void updateFuelLevel(float percent);

private:
    void drawGaugeFace();
    void drawNeedle(float percent);
    void drawLabels();
    float percentToAngle(float percent);

    int SCREEN_W = 240;
    int SCREEN_H = 320;

    int GAUGE_CX = 120;              // Gauge center X
    int GAUGE_CY = 180;              // Gauge center Y
    int GAUGE_R = 90;                // Radius
    float GAUGE_ROT_OFFSET = -90.0f; // degrees, CCW

    float START_ANGLE = 225.0f; // Empty
    float SWEEP_ANGLE = 270.0f; // Degrees

    float lastAngle = NAN;
};