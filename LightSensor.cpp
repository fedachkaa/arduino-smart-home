#include "LightSensor.h"

LightSensor::LightSensor(uint8_t pin) : _pin(pin) {}

void LightSensor::begin() {
    pinMode(_pin, INPUT);
}

int LightSensor::read() const {
    return analogRead(_pin);
}

bool LightSensor::isDark(int threshold) const {
    return read() < threshold;
}
