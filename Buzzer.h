#pragma once
#include <Arduino.h>

class Buzzer {
public:
    explicit Buzzer(uint8_t pin);

    void begin();
    void play(unsigned int frequency);
    void stop();

private:
    uint8_t _pin;
};
