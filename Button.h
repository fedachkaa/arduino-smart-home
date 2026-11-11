#pragma once
#include <Arduino.h>

class Button {
public:
    explicit Button(uint8_t pin);

    void begin();
    void update();

    bool isPressed() const;
    bool wasPressed() const;
    bool wasReleased() const;
    bool wasLongPressed() const;
    bool wasLongHold() const;

private:
    uint8_t _pin;

    bool _stableState = HIGH;
    bool _lastReading = HIGH;
    bool _pressedEvent = false;
    bool _releasedEvent = false;

    unsigned long _lastDebounceTime = 0;

    static constexpr unsigned long DEBOUNCE_DELAY = 30;

    unsigned long _pressStartTime = 0;
    bool _longPressEvent = false;
    bool _longPressHandled = false;

    static constexpr unsigned long LONG_PRESS_DURATION = 2000;
};
