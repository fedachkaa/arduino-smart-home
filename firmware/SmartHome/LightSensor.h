#pragma once
#include <Arduino.h>

class LightSensor {
public:
    explicit LightSensor(uint8_t pin);

    void begin();
    int read() const;
    bool isDark(int threshold) const;

private:
    uint8_t _pin;
};
