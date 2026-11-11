#pragma once

#include "ControlMode.h"
#include "Led.h"
#include "LightSensor.h"
#include "MotionSensor.h"

class LightController {
public:
    LightController(
        Led& led,
        LightSensor& lightSensor,
        MotionSensor& motionSensor
    );

    void update();
    void setMode(ControlMode mode);
    ControlMode getMode() const;

    void turnOn();
    void turnOff();

    bool isOn() const;

private:
    Led& _led;
    LightSensor& _lightSensor;
    MotionSensor& _motionSensor;

    ControlMode _mode = ControlMode::AUTO;

    static constexpr int DARKNESS_THRESHOLD = 300;
};