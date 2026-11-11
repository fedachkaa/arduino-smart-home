#include "DisplayController.h"

DisplayController::DisplayController(
    Display& display,
    MorsePassword& password,
    DoorController& doorController
) : _display(display),
    _password(password),
    _doorController(doorController) {}

void DisplayController::begin() {
    _lastInput = "";
    _lastDoorState = _doorController.getState();

    _display.showMessage("ENTER PASSWORD", "");
}

void DisplayController::update() {
    unsigned long now = millis();

    MorsePassword::Result result = _password.getResult();

    if (result != MorsePassword::Result::NONE) {
        _showingResult = true;
        _resultShownAt = now;

        if (result == MorsePassword::Result::ACCEPTED) {
            _display.showMessage(
                "ACCESS GRANTED",
                "Door opening..."
            );
        } else {
            _display.showMessage(
                "ACCESS DENIED",
                "Try again"
            );
        }

        return;
    }

    if (_showingResult) {
        if (now - _resultShownAt < RESULT_DURATION) {
            return;
        }

        _showingResult = false;
        _lastInput = "";
        _lastDoorState = _doorController.getState();

        if (_lastDoorState == DoorController::State::CLOSED) {
            _display.showMessage("ENTER PASSWORD", "");
        } else {
            showDoorState(_lastDoorState);
        }

        return;
    }

    DoorController::State state = _doorController.getState();

    if (state != _lastDoorState) {
        _lastDoorState = state;
        showDoorState(state);
        return;
    }

    if (state == DoorController::State::CLOSED &&
        _password.getInput() != _lastInput) {
        _lastInput = _password.getInput();
        showPasswordInput();
    }
}

void DisplayController::showDoorState(
    DoorController::State state
) {
    switch (state) {
        case DoorController::State::CLOSED:
            _display.showMessage("ENTER PASSWORD", "");
            break;

        case DoorController::State::OPENING:
            _display.showMessage("DOOR OPENING", "");
            break;

        case DoorController::State::OPEN:
            _display.showMessage("DOOR OPEN", "");
            break;

        case DoorController::State::CLOSING:
            _display.showMessage("DOOR CLOSING", "");
            break;
    }
}

void DisplayController::showPasswordInput() {
    _display.showMessage(
        "PASSWORD:",
        _password.getInput().c_str()
    );
}
