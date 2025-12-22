#pragma once

#include <Arduino.h>

class SenderInput
{
public:
    SenderInput() = default;
    void init(int fuelPin, float rsEmpty, float rsFull);
    float readPercent(int samples = 1);

private:
    int _pin = -1;
    float _rsEmpty = -1;
    float _rsFull = -1;
};