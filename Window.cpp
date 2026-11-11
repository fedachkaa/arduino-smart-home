#include "Window.h"

Window::Window(uint8_t pin) : _pin(pin) {}

void Window::begin(int initialAngle) {
    initialAngle = constrain(initialAngle, 0, 180);

    _currentAngle = initialAngle;
    _targetAngle = initialAngle;

    _servo.write(initialAngle);
    _servo.attach(_pin);

    _lastMoveTime = millis();
}

void Window::setAngle(int angle) {
    _targetAngle = constrain(angle, 0, 180);
}

void Window::update() {
    if (_currentAngle == _targetAngle) {
        return;
    }

    unsigned long currentTime = millis();

    if (currentTime - _lastMoveTime < MOVE_INTERVAL) {
        return;
    }

    _lastMoveTime = currentTime;

    if (_currentAngle < _targetAngle) {
        _currentAngle++;
    } else {
        _currentAngle--;
    }

    _servo.write(_currentAngle);
}

bool Window::isMoving() const {
    return _currentAngle != _targetAngle;
}

int Window::getCurrentAngle() const {
    return _currentAngle;
}
