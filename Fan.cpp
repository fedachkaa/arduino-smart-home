#include "Fan.h"

Fan::Fan(uint8_t pinINA, uint8_t pinINB)
    : _pinINA(pinINA),
      _pinINB(pinINB) {}

void Fan::begin() {
    pinMode(_pinINA, OUTPUT);
    pinMode(_pinINB, OUTPUT);

    off();
}

void Fan::on() {
    setSpeed(255);
}

void Fan::off() {
    setSpeed(0);
}

void Fan::setSpeed(uint8_t speed) {
    _speed = speed;

    digitalWrite(_pinINA, LOW);
    analogWrite(_pinINB, _speed);
}

uint8_t Fan::getSpeed() const {
    return _speed;
}

bool Fan::isRunning() const {
    return _speed > 0;
}
