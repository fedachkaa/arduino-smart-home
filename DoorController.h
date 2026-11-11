#pragma once

#include <Arduino.h>
#include "Door.h"
#include "MorsePassword.h"
#include "MotionSensor.h"
#include "Debug.h"

class DoorController {
public:
    enum class State {
        CLOSED,
        OPENING,
        OPEN,
        CLOSING
    };

    enum class Mode {
        AUTO,
        MANUAL
    };

    DoorController(Door& door, MorsePassword& password, MotionSensor& motionSensor);

    void begin();
    void update();

    void open();
    void close();
    void setAngle(int angle);
    void setMode(Mode mode);

    State getState() const;
    Mode getMode() const;

private:
    Door& _door;
    MorsePassword& _password;
    MotionSensor& _motionSensor;

    State _state = State::CLOSED;
    Mode _mode = Mode::AUTO;

    unsigned long _openedAt = 0;

    static constexpr unsigned long OPEN_DURATION = 5000;

    void changeState(State newState);
};
