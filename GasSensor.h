#pragma once
#include <Arduino.h>

class GasSensor {
public:
    explicit GasSensor(uint8_t pin);

    void begin();
    int read() const;

private:
    uint8_t _pin;
};
