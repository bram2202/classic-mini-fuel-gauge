#include <Arduino.h>
#include "screen.h"
#include "graph.h"

extern Screen screen;

float dataPoints[100];
int dataPointCount = 0;

void Graph::addDataPoint(float value)
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

void Graph::drawGraph()
{
    // Define graph box at the top of the screen
    int screenW = screen.tft.width();
    int screenH = screen.tft.height();
    (void)screenH; // currently unused

    const int boxX = 0;
    const int boxY = 0;
    const int boxW = screenW;
    const int boxH = 60; // top strip height

    // Background black and white outline
    screen.tft.fillRect(boxX, boxY, boxW, boxH, TFT_BLACK);
    screen.tft.drawRect(boxX, boxY, boxW, boxH, TFT_WHITE);

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
        screen.tft.drawLine(x, bottomY, x, topY, TFT_RED);
    }
}