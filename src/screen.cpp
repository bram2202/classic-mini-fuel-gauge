#include <Arduino.h>
#include "screen.h"

void Screen::init()
{
    tft.init();

    // Set screen orientation and clear to black
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);

    // Draw fuel gauge outline
    drawGaugeFace();
}

void Screen::updateFuelLevel(float percent)
{
    // draw needle
    drawNeedle(percent);

    // show percentage text using Font 4
    char buf[16];
    snprintf(buf, sizeof(buf), "%3.0f%%", percent);
    tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
    tft.setTextDatum(TL_DATUM);

    // clear a small area and redraw text
    tft.fillRect(10, 10, 80, 30, TFT_DARKGREY);
    tft.drawString(String(buf), 10, 10, 4);
}

// Private methods

void Screen::drawNeedle(float percent)
{
    // Redraw labels E and F
    drawLabels();

    // Get angle for percent
    float angle = percentToAngle(percent);

    // Erase old needle
    if (!isnan(lastAngle))
    {
        int ex = GAUGE_CX + cos(lastAngle) * (GAUGE_R - 10);
        int ey = GAUGE_CY + sin(lastAngle) * (GAUGE_R - 10);

        // erase both thickness lines drawn previously
        tft.drawLine(GAUGE_CX, GAUGE_CY, ex, ey, TFT_BLACK);
        tft.drawLine(GAUGE_CX + 1, GAUGE_CY, ex + 1, ey, TFT_BLACK);

        // restore hub (it may get overdrawn)
        tft.fillCircle(GAUGE_CX, GAUGE_CY, 3, TFT_LIGHTGREY);
    }

    // Draw New needle
    int nx = GAUGE_CX + cos(angle) * (GAUGE_R - 10);
    int ny = GAUGE_CY + sin(angle) * (GAUGE_R - 10);

    tft.drawLine(GAUGE_CX, GAUGE_CY, nx, ny, TFT_RED);
    tft.drawLine(GAUGE_CX + 1, GAUGE_CY, nx + 1, ny, TFT_RED); // thickness

    // Redraw Hub
    tft.fillCircle(GAUGE_CX, GAUGE_CY, 3, TFT_LIGHTGREY);

    lastAngle = angle;
}

void Screen::drawLabels()
{
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("E",
                   GAUGE_CX + cos(percentToAngle(0)) * (GAUGE_R - 18),
                   GAUGE_CY + sin(percentToAngle(0)) * (GAUGE_R - 18),
                   4);

    tft.drawString("F",
                   GAUGE_CX + cos(percentToAngle(100)) * (GAUGE_R - 18),
                   GAUGE_CY + sin(percentToAngle(100)) * (GAUGE_R - 18),
                   4);
}

/**
 * Draw the outline of the fuel gauge, including tick marks and labels.
 */
void Screen::drawGaugeFace()
{
    // Outer ring
    tft.drawCircle(GAUGE_CX, GAUGE_CY, GAUGE_R, TFT_LIGHTGREY);

    // Tick marks
    for (int i = 0; i <= 100; i += 10)
    {
        float a = percentToAngle(i);

        int x1 = GAUGE_CX + cos(a) * (GAUGE_R - 6);
        int y1 = GAUGE_CY + sin(a) * (GAUGE_R - 6);
        int x2 = GAUGE_CX + cos(a) * GAUGE_R;
        int y2 = GAUGE_CY + sin(a) * GAUGE_R;

        tft.drawLine(x1, y1, x2, y2, TFT_LIGHTGREY);
    }

    // Minor ticks
    for (int i = 0; i <= 100; i += 5)
    {
        float a = percentToAngle(i);

        int x1 = GAUGE_CX + cos(a) * (GAUGE_R - 3);
        int y1 = GAUGE_CY + sin(a) * (GAUGE_R - 3);
        int x2 = GAUGE_CX + cos(a) * GAUGE_R;
        int y2 = GAUGE_CY + sin(a) * GAUGE_R;

        tft.drawPixel(x2, y2, TFT_DARKGREY);
    }

    // Center hub
    tft.fillCircle(GAUGE_CX, GAUGE_CY, 4, TFT_LIGHTGREY);
}

/**
 * Convert a percentage (0-100) to an angle in radians for the gauge.
 */
float Screen::percentToAngle(float percent)
{
    percent = constrain(percent, 0.0f, 100.0f);
    float angleDeg =
        START_ANGLE +
        (percent * SWEEP_ANGLE / 100.0f) +
        GAUGE_ROT_OFFSET;

    return angleDeg * DEG_TO_RAD;
}
