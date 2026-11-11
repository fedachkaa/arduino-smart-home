#include "Button.h"

Button::Button(uint8_t pin) : _pin(pin) {}

void Button::begin() {
    pinMode(_pin, INPUT_PULLUP);

    _stableState = digitalRead(_pin);
    _lastReading = _stableState;
}

void Button::update() {
    _pressedEvent = false;
    _releasedEvent = false;
    _longPressEvent = false;

    bool reading = digitalRead(_pin);

    if (reading != _lastReading) {
        _lastDebounceTime = millis();
    }

    if (millis() - _lastDebounceTime >= DEBOUNCE_DELAY) {
        if (reading != _stableState) {
            _stableState = reading;

            if (_stableState == LOW) {
                _pressedEvent = true;
                _pressStartTime = millis();
                _longPressHandled = false;
            } else {
                _releasedEvent = true;
            }
        }
    }

    if (isPressed() &&
        !_longPressHandled &&
        millis() - _pressStartTime >= LONG_PRESS_DURATION) {
        _longPressEvent = true;
        _longPressHandled = true;
    }

    _lastReading = reading;
}

bool Button::isPressed() const {
    return _stableState == LOW;
}

bool Button::wasPressed() const {
    return _pressedEvent;
}

bool Button::wasReleased() const {
    return _releasedEvent;
}

bool Button::wasLongPressed() const {
    return _longPressEvent;
}

bool Button::wasLongHold() const {
    return _longPressHandled;
}
