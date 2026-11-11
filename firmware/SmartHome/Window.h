#pragma once
#include <Arduino.h>
#include <Servo.h>

class Window {
public:
    explicit Window(uint8_t pin);

    void begin(int initialAngle);
    void setAngle(int angle);
    void update();

    bool isMoving() const;
    int getCurrentAngle() const;

private:
    Servo _servo;

    uint8_t _pin;
    int _currentAngle = 90;
    int _targetAngle = 90;

    unsigned long _lastMoveTime = 0;

    static constexpr unsigned long MOVE_INTERVAL = 30;
};
