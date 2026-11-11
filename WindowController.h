#pragma once

#include "ControlMode.h"
#include "Window.h"
#include "RainSensor.h"

class WindowController {
public:
    WindowController(Window& window, RainSensor& rainSensor);

    void update();
    void setMode(ControlMode mode);
    ControlMode getMode() const;

    void open();
    void close();
    void setAngle(int angle);
    int getCurrentAngle() const;
    bool isMoving() const;

private:
    Window& _window;
    RainSensor& _rainSensor;

    ControlMode _mode = ControlMode::AUTO;

    static constexpr int RAIN_THRESHOLD = 800;
    static constexpr int OPEN_ANGLE = 130;
    static constexpr int CLOSED_ANGLE = 0;
};