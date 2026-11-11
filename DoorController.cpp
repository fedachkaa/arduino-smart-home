#include "DoorController.h"

DoorController::DoorController(
    Door& door,
    MorsePassword& password,
    MotionSensor& motionSensor
) : _door(door),
    _password(password),
    _motionSensor(motionSensor) {}

void DoorController::begin() {
    _mode = Mode::AUTO;
    _state = State::CLOSED;
    _door.close();
}

void DoorController::changeState(State newState) {
    _state = newState;

    switch (_state) {
        case State::CLOSED:
            DEBUG_PRINTLN("Door: CLOSED");
            break;

        case State::OPENING:
            DEBUG_PRINTLN("Door: OPENING");
            break;

        case State::OPEN:
            _openedAt = millis();
            DEBUG_PRINTLN("Door: OPEN");
            break;

        case State::CLOSING:
            DEBUG_PRINTLN("Door: CLOSING");
            break;
    }
}

void DoorController::open() {
    _mode = Mode::MANUAL;
    _door.open();
    changeState(State::OPENING);
}

void DoorController::close() {
    _mode = Mode::MANUAL;
    _door.close();
    changeState(State::CLOSING);
}

void DoorController::setAngle(int angle) {
    _mode = Mode::MANUAL;

    angle = constrain(angle, 0, 180);
    _door.setAngle(angle);

    if (angle > _door.getCurrentAngle()) {
        changeState(State::OPENING);
    } else if (angle < _door.getCurrentAngle()) {
        changeState(State::CLOSING);
    } else {
        changeState(angle == 0 ? State::CLOSED : State::OPEN);
    }
}

void DoorController::setMode(Mode mode) {
    if (_mode == mode) {
        return;
    }

    _mode = mode;

    if (_mode == Mode::AUTO) {
        _door.close();
        changeState(State::CLOSING);
    }
}

void DoorController::update() {
    _door.update();

    if (_mode == Mode::MANUAL) {
        if (_door.isMoving()) {
            return;
        }

        if (_state == State::OPENING ||
            _state == State::CLOSING) {
            changeState(
                _door.getCurrentAngle() == 0
                    ? State::CLOSED
                    : State::OPEN
            );
        }

        return;
    }

    switch (_state) {
        case State::CLOSED:
            if (_password.wasAccepted()) {
                _door.open();
                changeState(State::OPENING);
            }
            break;

        case State::OPENING:
            if (!_door.isMoving()) {
                changeState(State::OPEN);
            }
            break;

        case State::OPEN:
            if (_motionSensor.isMotionDetected()) {
                _openedAt = millis();
            } else if (millis() - _openedAt >= OPEN_DURATION) {
                _door.close();
                changeState(State::CLOSING);
            }
            break;

        case State::CLOSING:
            if (!_door.isMoving()) {
                changeState(State::CLOSED);
            }
            break;
    }
}

DoorController::State DoorController::getState() const {
    return _state;
}

DoorController::Mode DoorController::getMode() const {
    return _mode;
}
