#pragma once
#include <Arduino.h>

class Led {
public:
    explicit Led(uint8_t pin);

    void begin();
    void on();
    void off();

    void setBrightness(uint8_t brightness);
    bool isOn() const;
    uint8_t getBrightness() const;

private:
    uint8_t _pin;
    uint8_t _brightness = 0;
};