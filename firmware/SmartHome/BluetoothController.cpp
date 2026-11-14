#include "BluetoothController.h"
#include "LightController.h"
#include "WindowController.h"

BluetoothController::BluetoothController(
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
) : _fan(fan),
    _lightController(lightController),
    _windowController(windowController),
    _yellowLed(yellowLed),
    _relay(relay),
    _lightSensor(lightSensor),
    _gasSensor(gasSensor),
    _soilSensor(soilSensor),
    _rainSensor(rainSensor),
    _alarmController(alarmController),
    _doorController(doorController),
    _motionSensor(motionSensor) {}

void BluetoothController::update() {
    if (_parseState == ParseState::READING_NUMBER &&
        millis() - _lastNumericByteAt >= NUMERIC_COMMAND_TIMEOUT) {
        resetNumericParser();
    }

    while (Serial.available() > 0) {
        char value = Serial.read();
        processByte(value);
    }
}

void BluetoothController::setRainState(bool isRaining) {
    _isRaining = isRaining;
}

void BluetoothController::handleCommand(char command) {
    switch (command) {
        case '?':
            sendStatus();
            break;
       case 'r':
            _fan.on();
            break;

        case 's':
            _fan.off();
            break;

        case '1':
            _fan.setSpeed(100);
            break;

        case '2':
            _fan.setSpeed(180);
            break;

        case '3':
            _fan.setSpeed(255);
            break;

        // White LED
        case 'a':
            _lightController.turnOn();
            break;

        case 'b':
            _lightController.turnOff();
            break;

        case 'A':
            _lightController.setMode(ControlMode::AUTO);
            break;

        // Window
        case 'n':
            _windowController.open();
            break;

        case 'o':
            _windowController.close();
            break;

        case 'N':
            _windowController.setMode(ControlMode::AUTO);
            break;

        // Yellow LED
        case 'p':
            _yellowLed.on();
            break;

        case 'q':
            _yellowLed.off();
            break;

       case 'P':
            if (_yellowLed.isOn()) {
                _yellowLed.off();
            } else {
                _yellowLed.on();
            }
            break;

        // Relay
        case 'c':
            _relay.on();
            break;

        case 'd':
            _relay.off();
            break;

        case 'h':
            sendSensorReading("LIGHT", _lightSensor.read());
            break;

        case 'i':
            sendSensorReading("GAS", _gasSensor.read());
            break;

        case 'j':
            sendSensorReading("SOIL", _soilSensor.read());
            break;

        case 'k':
            sendSensorReading("RAIN", _rainSensor.read());
            break;
        
        // Buzzer melodies
        case 'e':
            _alarmController.playMelody(MelodyPlayer::Melody::HAPPY_BIRTHDAY);
            break;

        case 'f':
            _alarmController.playMelody(MelodyPlayer::Melody::ODE_TO_JOY);
            break;

        case 'g':
            _alarmController.stopMelody();
            break;

        case 'l':
            _doorController.open();
            break;

        case 'm':
            _doorController.close();
            break;

        case 'T':
            _doorController.setMode(DoorController::Mode::AUTO);
            break;

        default:
            break;
    }
}

void BluetoothController::processByte(char value) {
    if (_parseState == ParseState::WAITING_FOR_COMMAND) {
        if (value == 'v' || value == 't' ||
            value == 'w' || value == 'u') {

            _numericCommand = value;
            _numericValue = 0;
            _numericDigits = 0;
            _numericInvalid = false;
            _lastNumericByteAt = millis();

            _parseState = ParseState::READING_NUMBER;
            return;
        }

        handleCommand(value);
        return;
    }

    if (_parseState == ParseState::READING_NUMBER) {
        if (value == '#') {
            if (!_numericInvalid && _numericDigits > 0) {
                handleNumericCommand(_numericCommand, _numericValue);
            }

            resetNumericParser();
            return;
        }

        if (value >= '0' && value <= '9') {
            _lastNumericByteAt = millis();

            if (_numericDigits >= 3) {
                _numericInvalid = true;
                return;
            }

            ++_numericDigits;
            _numericValue = _numericValue * 10 + (value - '0');

            if (_numericValue > 255) {
                _numericInvalid = true;
            }

            return;
        }

        // Invalid character: discard the incomplete numeric command.
        resetNumericParser();

        // Process the character as a possible new command.
        processByte(value);
    }
}

void BluetoothController::handleNumericCommand(
    char command,
    uint16_t value
) {
    switch (command) {
        case 'v':
            _yellowLed.setBrightness(static_cast<uint8_t>(value));
            break;

        case 't':
            if (value <= 180) {
                _doorController.setAngle(static_cast<int>(value));
            }
            break;

        case 'w':
            _fan.setSpeed(static_cast<uint8_t>(value));
            break;

        case 'u':
            if (value <= 130) {
                _windowController.setAngle(static_cast<int>(value));
            }
            break;

        default:
            break;
    }
}

void BluetoothController::resetNumericParser() {
    _parseState = ParseState::WAITING_FOR_COMMAND;
    _numericCommand = '\0';
    _numericValue = 0;
    _numericDigits = 0;
    _numericInvalid = false;
    _lastNumericByteAt = 0;
}

void BluetoothController::sendSensorReading(
    const char* sensor,
    int value
) {
    Serial.print(sensor);
    Serial.print(':');
    Serial.println(value);
}

void BluetoothController::sendEvent(
    const char* component,
    const char* event,
    const char* value
) {
    Serial.print(component);
    Serial.print(':');
    Serial.print(event);
    Serial.print(':');
    Serial.println(value);
}

void BluetoothController::sendEvent(
    const char* component,
    const char* event,
    int value
) {
    char buffer[12];
    snprintf(buffer, sizeof(buffer), "%d", value);
    sendEvent(component, event, buffer);
}

void BluetoothController::sendStatus() {
    // Fan
    sendEvent("FAN", "STATE", _fan.isRunning() ? "ON" : "OFF");
    sendEvent("FAN", "SPEED", _fan.getSpeed());

    // Relay
    sendEvent("RELAY", "STATE", _relay.isOn() ? "ON" : "OFF"); 

    // White LED mode
    sendEvent("LIGHT", "MODE", _lightController.getMode() == ControlMode::AUTO ? "AUTO" : "MANUAL");
    sendEvent("LIGHT", "STATE", _lightController.isOn() ? "ON" : "OFF");

    // Window mode
    sendEvent("WINDOW", "MODE", _windowController.getMode() == ControlMode::AUTO ? "AUTO" : "MANUAL");

    // Yellow LED
    sendEvent("YELLOW_LED", "STATE", _yellowLed.isOn() ? "ON" : "OFF");
    sendEvent("YELLOW_LED", "BRIGHTNESS", static_cast<int>(_yellowLed.getBrightness()));

    // Door
    const char* doorState = "UNKNOWN";

    switch (_doorController.getState()) {
        case DoorController::State::CLOSED:
            doorState = "CLOSED";
            break;

        case DoorController::State::OPENING:
            doorState = "OPENING";
            break;

        case DoorController::State::OPEN:
            doorState = "OPEN";
            break;

        case DoorController::State::CLOSING:
            doorState = "CLOSING";
            break;
    }

    sendEvent("DOOR", "STATE", doorState);
    sendEvent("DOOR", "MODE", _doorController.getMode() == DoorController::Mode::AUTO ? "AUTO" : "MANUAL");

    // Window angle
    sendEvent("WINDOW", "ANGLE", _windowController.getCurrentAngle());

    // Alarms
    sendEvent("ALARM", "GAS", _alarmController.isGasAlarmActive() ? "ON" : "OFF");
    sendEvent("ALARM", "SOIL", _alarmController.isSoilAlarmActive() ? "ON" : "OFF");

    // Rain
    sendEvent("RAIN", "STATE", _isRaining ? "ON" : "OFF");

    sendEvent("MOTION", "STATE", _motionSensor.isMotionDetected() ? "ON" : "OFF");

    sendEvent("STATUS", "END", "OK");
}
