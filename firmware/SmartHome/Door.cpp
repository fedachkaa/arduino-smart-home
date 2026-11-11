#include "Door.h"

Door::Door(uint8_t pin) : _pin(pin) {}

void Door::begin(int initialAngle) {
    initialAngle = constrain(initialAngle, 0, 180);

    _currentAngle = initialAngle;
    _targetAngle = initialAngle;

    _servo.write(initialAngle);
    _servo.attach(_pin);

    _lastMoveTime = millis();
}

void Door::setAngle(int angle) {
    _targetAngle = constrain(angle, 0, 180);
}

void Door::update() {
    if (_currentAngle == _targetAngle) {
        return;
    }

    unsigned long now = millis();

    if (now - _lastMoveTime < MOVE_INTERVAL) {
        return;
    }

    _lastMoveTime = now;

    if (_currentAngle < _targetAngle) {
        _currentAngle++;
    } else {
        _currentAngle--;
    }

    _servo.write(_currentAngle);
}

bool Door::isMoving() const {
    return _currentAngle != _targetAngle;
}

int Door::getCurrentAngle() const {
    return _currentAngle;
}

void Door::open() {
    setAngle(OPEN_ANGLE);
}

void Door::close() {
    setAngle(CLOSED_ANGLE);
}
