#pragma once
#include <Arduino.h>
#include <Servo.h>

class Door {
public:
    explicit Door(uint8_t pin);

    void begin(int initialAngle);
    void open();
    void close();
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

    static constexpr int OPEN_ANGLE = 50;
    static constexpr int CLOSED_ANGLE = 0;
    static constexpr unsigned long MOVE_INTERVAL = 30;
};