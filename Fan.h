#pragma once
#include <Arduino.h>

class Fan {
public:
    Fan(uint8_t pinINA, uint8_t pinINB);

    void begin();
    void on();
    void off();
    void setSpeed(uint8_t speed);

    uint8_t getSpeed() const;
    bool isRunning() const;

private:
    uint8_t _pinINA;
    uint8_t _pinINB;
    uint8_t _speed = 0;
};
