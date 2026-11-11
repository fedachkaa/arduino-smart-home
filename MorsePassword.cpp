#include "MorsePassword.h"

MorsePassword::MorsePassword(
    Button& inputButton,
    Button& confirmButton
) : _inputButton(inputButton),
    _confirmButton(confirmButton) {}

void MorsePassword::update() {
    _accepted = false;
    _result = Result::NONE;
    _doorbellRequested = false;

    if (_inputButton.wasPressed()) {
        _pressStartTime = millis();
    }

    if (_inputButton.wasReleased()) {
        unsigned long duration = millis() - _pressStartTime;

        char symbol = duration < DASH_THRESHOLD ? '.' : '-';
        addSymbol(symbol);
    }

    if (_confirmButton.wasLongPressed()) {
        _doorbellRequested = true;
    }

    if (_confirmButton.wasReleased() && !_confirmButton.wasLongHold()) {
        checkPassword();
    }
}

void MorsePassword::addSymbol(char symbol) {
    if (_input.length() >= MAX_LENGTH) {
        DEBUG_PRINTLN("Password is too long");
        _input = "";
        return;
    }

    _input += symbol;

    DEBUG_PRINT("Password input: ");
    DEBUG_PRINTLN(_input);
}

void MorsePassword::checkPassword() {
    if (_input == ".--.-.") {
        _accepted = true;
        _result = Result::ACCEPTED;

        DEBUG_PRINTLN("ACCESS GRANTED");
    } else {
        _result = Result::REJECTED;

        DEBUG_PRINTLN("ACCESS DENIED");
    }

    _input = "";
}

bool MorsePassword::wasAccepted() const {
    return _accepted;
}

MorsePassword::Result MorsePassword::getResult() const {
    return _result;
}

const String& MorsePassword::getInput() const {
    return _input;
}

bool MorsePassword::wasDoorbellRequested() const {
    return _doorbellRequested;
}
