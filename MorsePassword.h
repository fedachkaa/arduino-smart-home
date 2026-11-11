#pragma once

#include <Arduino.h>
#include "Button.h"
#include "Debug.h"

class MorsePassword {
public:
    MorsePassword(Button& inputButton, Button& confirmButton);

    enum class Result {
        NONE,
        ACCEPTED,
        REJECTED
    };

    void update();
    bool wasAccepted() const;
    Result getResult() const;
    const String& getInput() const;
    bool wasDoorbellRequested() const;


private:
    Button& _inputButton;
    Button& _confirmButton;

    String _input;
    Result _result = Result::NONE;
    unsigned long _pressStartTime = 0;
    bool _accepted = false;
    bool _doorbellRequested = false;

    static constexpr unsigned long DASH_THRESHOLD = 500;
    static constexpr size_t MAX_LENGTH = 6;

    void addSymbol(char symbol);
    void checkPassword();
};
