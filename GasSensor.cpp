#include "GasSensor.h"

GasSensor::GasSensor(uint8_t pin) : _pin(pin) {}

void GasSensor::begin() {
    pinMode(_pin, INPUT);
}

int GasSensor::read() const {
    return analogRead(_pin);
}
