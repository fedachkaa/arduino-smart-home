#include "Relay.h"

Relay::Relay(uint8_t pin) : _pin(pin) {}

void Relay::begin() {
    digitalWrite(_pin, !ACTIVE_LEVEL);
    pinMode(_pin, OUTPUT);
    _isOn = false;
}

void Relay::on() {
    digitalWrite(_pin, ACTIVE_LEVEL);
    _isOn = true;
}

void Relay::off() {
    digitalWrite(_pin, !ACTIVE_LEVEL);
    _isOn = false;
}

bool Relay::isOn() const {
    return _isOn;
}