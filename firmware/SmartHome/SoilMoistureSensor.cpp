#include "SoilMoistureSensor.h"

SoilMoistureSensor::SoilMoistureSensor(uint8_t pin)
    : _pin(pin) {}

void SoilMoistureSensor::begin() {
    pinMode(_pin, INPUT);
}

int SoilMoistureSensor::read() const {
    return analogRead(_pin);
}

bool SoilMoistureSensor::isDry(int threshold) const {
    int value = read();

    return value > 0 && value < threshold;
}
