#pragma once

#include <Arduino.h>
#include "Fan.h"
#include "LightController.h"
#include "WindowController.h"
#include "Led.h"
#include "Relay.h"
#include "LightSensor.h"
#include "GasSensor.h"
#include "SoilMoistureSensor.h"
#include "RainSensor.h"
#include "AlarmController.h"
#include "DoorController.h"
#include "MotionSensor.h"

class BluetoothController {
public:
    BluetoothController(
        Fan& fan,
        LightController& lightController,
        WindowController& windowController,
        Led& yellowLed,
        Relay& relay,
        LightSensor& lightSensor,
        GasSensor& gasSensor,
        SoilMoistureSensor& soilSensor,
        RainSensor& rainSensor,
        AlarmController& alarmController,
        DoorController& doorController,
        MotionSensor& motionSensor
    );

    void update();
    void setRainState(bool isRaining);
    void sendStatus();
    void sendEvent(
        const char* component,
        const char* event,
        const char* value
    );

    void sendEvent(
        const char* component,
        const char* event,
        int value
    );

private:
    Fan& _fan;
    LightController& _lightController;
    WindowController& _windowController;
    Led& _yellowLed;
    Relay& _relay;
    LightSensor& _lightSensor;
    GasSensor& _gasSensor;
    SoilMoistureSensor& _soilSensor;
    RainSensor& _rainSensor;
    AlarmController& _alarmController;
    DoorController& _doorController;
    MotionSensor& _motionSensor;

    bool _isRaining = false;

    void handleCommand(char command);
    void sendSensorReading(const char* sensor, int value);

    enum class ParseState {
        WAITING_FOR_COMMAND,
        READING_NUMBER
    };

    ParseState _parseState = ParseState::WAITING_FOR_COMMAND;

    char _numericCommand = '\0';
    uint16_t _numericValue = 0;
    uint8_t _numericDigits = 0;
    bool _numericInvalid = false;
    unsigned long _lastNumericByteAt = 0;

    static constexpr unsigned long NUMERIC_COMMAND_TIMEOUT = 1000;

    void processByte(char value);
    void handleNumericCommand(char command, uint16_t value);
    void resetNumericParser();
};
