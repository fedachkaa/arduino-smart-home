#include "RainSensor.h"

RainSensor::RainSensor(uint8_t pin) : _pin(pin) {}

void RainSensor::begin() {
    pinMode(_pin, INPUT);
}

int RainSensor::read() const {
    return analogRead(_pin);
}

bool RainSensor::isRaining(int threshold) const {
    return read() > threshold;
}