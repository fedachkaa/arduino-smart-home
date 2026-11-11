#pragma once
#include <Arduino.h>

class MotionSensor {
public:
    explicit MotionSensor(uint8_t pin);

    void begin();
    bool isMotionDetected() const;

private:
    uint8_t _pin;
};
