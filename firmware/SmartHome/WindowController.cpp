#include "WindowController.h"

WindowController::WindowController(
    Window& window,
    RainSensor& rainSensor
) : _window(window),
    _rainSensor(rainSensor) {}

void WindowController::update() {
    if (_mode == ControlMode::AUTO) {
        bool raining = _rainSensor.isRaining(RAIN_THRESHOLD);

        _window.setAngle(
            raining ? CLOSED_ANGLE : OPEN_ANGLE
        );
    }

    _window.update();
}

void WindowController::setMode(ControlMode mode) {
    _mode = mode;
}

ControlMode WindowController::getMode() const {
    return _mode;
}

void WindowController::open() {
    _mode = ControlMode::MANUAL;
    _window.setAngle(OPEN_ANGLE);
}

void WindowController::close() {
    _mode = ControlMode::MANUAL;
    _window.setAngle(CLOSED_ANGLE);
}

void WindowController::setAngle(int angle) {
    _mode = ControlMode::MANUAL;
    _window.setAngle(constrain(angle, CLOSED_ANGLE, OPEN_ANGLE));
}

int WindowController::getCurrentAngle() const {
    return _window.getCurrentAngle();
}

bool WindowController::isMoving() const {
    return _window.isMoving();
}
