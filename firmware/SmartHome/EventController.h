#pragma once

#include <Arduino.h>
#include "GasAlarm.h"
#include "BluetoothController.h"
#include "SoilAlarm.h"
#include "DoorController.h"
#include "LightController.h"
#include "WindowController.h"
#include "Fan.h"
#include "Relay.h"
#include "Led.h"
#include "MotionSensor.h"

class EventController {
public:
    EventController(
        GasAlarm& gasAlarm,
        SoilAlarm& soilAlarm,
        RainSensor& rainSensor,
        DoorController& doorController,
        LightController& lightController,
        WindowController& windowController,
        Fan& fan,
        Relay& relay,
        Led& yellowLed,
        MotionSensor& motionSensor,
        BluetoothController& bluetoothController
    );

    void update();

private:
    GasAlarm& _gasAlarm;
    BluetoothController& _bluetoothController;
    RainSensor& _rainSensor;
    SoilAlarm& _soilAlarm;
    DoorController& _doorController;
    LightController& _lightController;
    WindowController& _windowController;
    Fan& _fan;
    Relay& _relay;
    Led& _yellowLed;
    MotionSensor& _motionSensor;

    bool _previousSoilState = false;
    DoorController::State _previousDoorState = DoorController::State::CLOSED;
    bool _previousGasState = false;
    bool _previousRainState = false;
    bool _initialized = false;

    bool _pendingRainState = false;
    unsigned long _rainStateChangedAt = 0;

    bool _previousLightState = false;
    uint8_t _previousYellowBrightness = 0;
    bool _previousFanState = false;
    int _previousFanSpeed = 0;
    bool _previousRelayState = false;

    int _previousWindowAngle = 0;

    ControlMode _previousLightMode = ControlMode::AUTO;
    ControlMode _previousWindowMode = ControlMode::AUTO;
    DoorController::Mode _previousDoorMode = DoorController::Mode::AUTO;

    bool _previousMotionState = false;
    bool _pendingMotionState = false;
    bool _isMotionReportingReady = false;
    unsigned long _motionStabilizationStartedAt = 0;
    unsigned long _motionStateChangedAt = 0;

    static constexpr unsigned long RAIN_DEBOUNCE_DELAY = 500;
    static constexpr int RAIN_ON_THRESHOLD = 850;
    static constexpr int RAIN_OFF_THRESHOLD = 750;
    static constexpr unsigned long MOTION_STABILIZATION_DELAY = 30000;
    static constexpr unsigned long MOTION_DEBOUNCE_DELAY = 100;
};
