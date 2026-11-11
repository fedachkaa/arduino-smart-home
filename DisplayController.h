#pragma once

#include <Arduino.h>
#include "Display.h"
#include "MorsePassword.h"
#include "DoorController.h"

class DisplayController {
public:
    DisplayController(
        Display& display,
        MorsePassword& password,
        DoorController& doorController
    );

    void begin();
    void update();

private:
    Display& _display;
    MorsePassword& _password;
    DoorController& _doorController;

    String _lastInput;
    DoorController::State _lastDoorState =
        DoorController::State::CLOSED;

    bool _showingResult = false;
    unsigned long _resultShownAt = 0;

    static constexpr unsigned long RESULT_DURATION = 1500;

    void showDoorState(DoorController::State state);
    void showPasswordInput();
};
