#include "EventController.h"

EventController::EventController(
    GasAlarm& gasAlarm,
    SoilAlarm& soilAlarm,
    RainSensor& rainSensor,
    DoorController& doorController,
    LightController& lightController,
    WindowController& windowController,
    Fan& fan,
    Relay& relay,
    Led& yellowLed,
    BluetoothController& bluetoothController
) : _gasAlarm(gasAlarm),
    _soilAlarm(soilAlarm),
    _rainSensor(rainSensor),
    _doorController(doorController),
    _lightController(lightController),
    _windowController(windowController),
    _fan(fan),
    _relay(relay),
    _yellowLed(yellowLed),
    _bluetoothController(bluetoothController) {}

void EventController::update() {
    unsigned long now = millis();

    bool gasActive = _gasAlarm.isActive();
    bool soilActive = _soilAlarm.isActive();
    DoorController::State doorState = _doorController.getState();
    int rainValue = _rainSensor.read();
    bool lightOn = _lightController.isOn();
    uint8_t yellowBrightness = _yellowLed.getBrightness();
    bool fanRunning = _fan.isRunning();
    int fanSpeed = _fan.getSpeed();
    bool relayOn = _relay.isOn();
    int windowAngle = _windowController.getCurrentAngle();
    ControlMode lightMode = _lightController.getMode();
    ControlMode windowMode = _windowController.getMode();
    DoorController::Mode doorMode = _doorController.getMode();

    bool rainActive = _previousRainState
        ? rainValue > RAIN_OFF_THRESHOLD
        : rainValue > RAIN_ON_THRESHOLD;

      if (!_initialized) {
        _previousGasState = gasActive;
        _previousSoilState = soilActive;
        _previousDoorState = doorState;

        _previousRainState = rainActive;
        _bluetoothController.setRainState(_previousRainState);
        _pendingRainState = rainActive;
        _rainStateChangedAt = now;

        _previousLightState = lightOn;
        _previousYellowBrightness = yellowBrightness;
        _previousFanState = fanRunning;
        _previousFanSpeed = fanSpeed;
        _previousRelayState = relayOn;

        _previousWindowAngle = windowAngle;

        _previousLightMode = lightMode;
        _previousWindowMode = windowMode;
        _previousDoorMode = doorMode;

        _initialized = true;
        return;
    }

    // Gas events
    if (gasActive != _previousGasState) {
        _bluetoothController.sendEvent(
            "ALARM",
            "GAS",
            gasActive ? "ON" : "OFF"
        );

        _previousGasState = gasActive;
    }

    if (soilActive != _previousSoilState) {
        _bluetoothController.sendEvent(
            "ALARM",
            "SOIL",
            soilActive ? "ON" : "OFF"
        );

        _previousSoilState = soilActive;
    }

    if (doorState != _previousDoorState) {
        switch (doorState) {
            case DoorController::State::OPEN:
                _bluetoothController.sendEvent(
                    "DOOR", "STATE", "OPEN"
                );
                break;

            case DoorController::State::CLOSED:
                _bluetoothController.sendEvent(
                    "DOOR", "STATE", "CLOSED"
                );
                break;

            case DoorController::State::OPENING:
            case DoorController::State::CLOSING:
                break;
        }

        _previousDoorState = doorState;
    }

    // Rain debounce
    if (rainActive != _pendingRainState) {
        _pendingRainState = rainActive;
        _rainStateChangedAt = now;
    }

    if (_pendingRainState != _previousRainState &&
        now - _rainStateChangedAt >= RAIN_DEBOUNCE_DELAY) {

        _previousRainState = _pendingRainState;
        _bluetoothController.setRainState(_previousRainState);

        _bluetoothController.sendEvent(
            "RAIN",
            "STATE",
            _previousRainState ? "ON" : "OFF"
        );
    }

    // White LED
    if (lightOn != _previousLightState) {
        _bluetoothController.sendEvent("LIGHT", "STATE", lightOn ? "ON" : "OFF");
        _previousLightState = lightOn;
    }

    // Yellow LED
    if (yellowBrightness != _previousYellowBrightness) {
        _bluetoothController.sendEvent("YELLOW_LED", "STATE", yellowBrightness > 0 ? "ON" : "OFF");
        _bluetoothController.sendEvent("YELLOW_LED", "BRIGHTNESS", (int) yellowBrightness);
        _previousYellowBrightness = yellowBrightness;
    }

    // Fan
    if (fanRunning != _previousFanState) {
        _bluetoothController.sendEvent("FAN", "STATE", fanRunning ? "ON" : "OFF");
        _previousFanState = fanRunning;
    }

    if (fanSpeed != _previousFanSpeed) {
        _bluetoothController.sendEvent("FAN", "SPEED", fanSpeed);
        _previousFanSpeed = fanSpeed;
    }

    // Relay
    if (relayOn != _previousRelayState) {
        _bluetoothController.sendEvent("RELAY", "STATE", relayOn ? "ON" : "OFF");
        _previousRelayState = relayOn;
    }

    // Window
    if (!_windowController.isMoving() && windowAngle != _previousWindowAngle) {
        _bluetoothController.sendEvent("WINDOW", "ANGLE", windowAngle);
        _previousWindowAngle = windowAngle;
    }

    // Light mode
    if (lightMode != _previousLightMode) {
        _bluetoothController.sendEvent(
            "LIGHT",
            "MODE",
            lightMode == ControlMode::AUTO ? "AUTO" : "MANUAL"
        );

        _previousLightMode = lightMode;
    }

    // Window mode
    if (windowMode != _previousWindowMode) {
        _bluetoothController.sendEvent(
            "WINDOW",
            "MODE",
            windowMode == ControlMode::AUTO ? "AUTO" : "MANUAL"
        );

        _previousWindowMode = windowMode;
    }

    // Door mode
    if (doorMode != _previousDoorMode) {
        _bluetoothController.sendEvent(
            "DOOR",
            "MODE",
            doorMode == DoorController::Mode::AUTO ? "AUTO" : "MANUAL"
        );

        _previousDoorMode = doorMode;
    }
}