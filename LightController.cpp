#include "LightController.h"

LightController::LightController(
    Led& led,
    LightSensor& lightSensor,
    MotionSensor& motionSensor
) : _led(led),
    _lightSensor(lightSensor),
    _motionSensor(motionSensor) {}

void LightController::update() {
    if (_mode == ControlMode::MANUAL) {
        return;
    }

    bool shouldTurnOn =
        _lightSensor.isDark(DARKNESS_THRESHOLD) &&
        _motionSensor.isMotionDetected();

    if (shouldTurnOn) {
        _led.on();
    } else {
        _led.off();
    }
}

void LightController::setMode(ControlMode mode) {
    _mode = mode;
}

ControlMode LightController::getMode() const {
    return _mode;
}

void LightController::turnOn() {
    _mode = ControlMode::MANUAL;
    _led.on();
}

void LightController::turnOff() {
    _mode = ControlMode::MANUAL;
    _led.off();
}

bool LightController::isOn() const {
    return _led.isOn();
}
