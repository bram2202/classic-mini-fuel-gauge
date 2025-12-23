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
    tft.fillRect(80, 290, 80, 30, TFT_DARKGREY);
    tft.drawString(String(buf), 80, 290, 4);
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

void Screen::addDataPoint(float value)
{
    if (dataPointCount < 100)
    {
        dataPoints[dataPointCount++] = value;
    }
    else
    {
        // Shift data points to make room for new value
        for (int i = 1; i < 100; ++i)
        {
            dataPoints[i - 1] = dataPoints[i];
        }
        dataPoints[99] = value;
    }

    drawGraph();
}

void Screen::drawGraph()
{
    // Define graph box at the top of the screen
    int screenW = tft.width();
    int screenH = tft.height();
    (void)screenH; // currently unused

    const int boxX = 0;
    const int boxY = 0;
    const int boxW = screenW;
    const int boxH = 60; // top strip height

    // Background black and white outline
    tft.fillRect(boxX, boxY, boxW, boxH, TFT_BLACK);
    tft.drawRect(boxX, boxY, boxW, boxH, TFT_WHITE);

    // Draw each data point as a vertical red line
    int count = dataPointCount;
    if (count > 100)
        count = 100;

    // Avoid division by zero
    if (count <= 0)
        return;

    // Horizontal spacing across the box from left to right
    // Use fixed 100 columns so indices map consistently as new data arrives
    const int columns = 100;
    const int left = boxX + 1;
    const int right = boxX + boxW - 2;
    const int usableW = max(1, right - left + 1);

    for (int i = 0; i < count; ++i)
    {
        // Map index to x-position across the box
        int x = left + (i * usableW) / columns;

        // Scale value (assumed 0..100) to height within the box
        float v = dataPoints[i];
        if (v < 0.0f)
            v = 0.0f;
        if (v > 100.0f)
            v = 100.0f;

        int h = (int)(v / 100.0f * (boxH - 4));
        if (h < 1)
            h = 1;

        int bottomY = boxY + boxH - 2;
        int topY = bottomY - h + 1;
        if (topY < boxY + 1)
            topY = boxY + 1;

        // Draw the vertical red line
        tft.drawLine(x, bottomY, x, topY, TFT_RED);
    }
}