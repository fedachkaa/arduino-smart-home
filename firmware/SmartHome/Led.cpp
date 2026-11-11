#include "Led.h"

Led::Led(uint8_t pin) : _pin(pin) {}

void Led::begin() {
    pinMode(_pin, OUTPUT);
    off();
}

void Led::on() {
    digitalWrite(_pin, HIGH);
    _brightness = 255;
}

void Led::off() {
    digitalWrite(_pin, LOW);
    _brightness = 0;
}

void Led::setBrightness(uint8_t brightness) {
    analogWrite(_pin, brightness);
    _brightness = brightness;
}

bool Led::isOn() const {
    return _brightness > 0;
}

uint8_t Led::getBrightness() const {
    return _brightness;
}