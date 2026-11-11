#include "MotionSensor.h"

MotionSensor::MotionSensor(uint8_t pin) : _pin(pin) {}

void MotionSensor::begin() {
    pinMode(_pin, INPUT);
}

bool MotionSensor::isMotionDetected() const {
    return digitalRead(_pin) == HIGH;
}
