#include <Arduino.h>
#include "senderInput.h"

void SenderInput::init(int fuelPin, float rsEmpty, float rsFull)
{
    _pin = fuelPin;
    _rsEmpty = rsEmpty;
    _rsFull = rsFull;
}

float SenderInput::readPercent(int samples)
{
    samples = max(1, samples);
    unsigned long sum = 0;

    // Take multiple samples and average
    for (int i = 0; i < samples; ++i)
    {
        sum += analogRead(_pin);
    }
    float raw = (float)sum / (float)samples;

    // Known hardware
    const float VREF = 3.3f;       // ESP32 ADC reference (approx.)
    const float ADC_MAX = 4095.0f; // 12-bit ADC
    const float R_FIXED = 220.0f;  // Fixed resistor in ohms

    // Convert ADC reading to voltage
    float vOut = (raw / ADC_MAX) * VREF;
    // Prevent division by zero and out-of-range artifacts
    vOut = constrain(vOut, 0.0f, VREF - 0.001f);

    // Voltage divider: 3.3V ── Rfixed ──┬── ADC
    //                                 |
    //                              Rsender
    //                                 |
    //                                GND
    // Rsender = (Vout * Rfixed) / (Vref - Vout)
    float rSender = (vOut * R_FIXED) / (VREF - vOut);

    // Map ohms to percentage (linear interpolation, then clamp)
    float percent = (_rsEmpty - rSender) / (_rsEmpty - _rsFull) * 100.0f;
    float value = constrain(percent, 0.0f, 100.0f);

    Serial.println("ADC: " + String(raw));
    Serial.println(" V: " + String(vOut, 3));
    Serial.println(" Rs: " + String(rSender, 1) + " Ω");
    Serial.println(" Fuel: " + String(value) + "%");

    return value;
}