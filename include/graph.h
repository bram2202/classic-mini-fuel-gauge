#pragma once

#include <Arduino.h>

class Graph
{
public:
    void addDataPoint(float value);

private:
    float dataPoints[100];
    int dataPointCount;
    void drawGraph();
};