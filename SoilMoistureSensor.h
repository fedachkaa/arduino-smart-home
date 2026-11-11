#pragma once
#include <Arduino.h>

class SoilMoistureSensor {
public:
    explicit SoilMoistureSensor(uint8_t pin);

    void begin();
    int read() const;
    bool isDry(int threshold) const;

private:
    uint8_t _pin;
};
