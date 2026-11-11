#include "AlarmController.h"

AlarmController::AlarmController(
    GasAlarm& gasAlarm,
    SoilAlarm& soilAlarm,
    Buzzer& buzzer,
    MelodyPlayer& melodyPlayer
) : _gasAlarm(gasAlarm),
    _soilAlarm(soilAlarm),
    _buzzer(buzzer),
    _melodyPlayer(melodyPlayer) {}

AlarmController::AlarmType AlarmController::determineAlarm() const {
    if (_gasAlarm.isActive()) {
        return AlarmType::GAS;
    }

    if (_soilAlarm.isActive()) {
        return AlarmType::SOIL;
    }

    return AlarmType::NONE;
}

void AlarmController::changeAlarm(
    AlarmType alarm,
    unsigned long now
) {
    _melodyPlayer.stop();
    _buzzer.stop();
    _soundOn = false;
    _currentAlarm = alarm;
    _lastToggleTime = now;

    if (alarm == AlarmType::GAS) {
        _buzzer.play(GAS_FREQUENCY);
        _soundOn = true;
    } else if (alarm == AlarmType::SOIL) {
        _buzzer.play(SOIL_FREQUENCY);
        _soundOn = true;
    }
}

void AlarmController::update() {
    _gasAlarm.update();
    _soilAlarm.update();

    unsigned long now = millis();
    AlarmType nextAlarm = determineAlarm();

    if (nextAlarm != _currentAlarm) {
        changeAlarm(nextAlarm, now);
    }

    if (_currentAlarm == AlarmType::NONE) {
        _melodyPlayer.update();
        return;
    }

    if (_currentAlarm == AlarmType::GAS) {
        if (now - _lastToggleTime < GAS_INTERVAL) {
            return;
        }

        _lastToggleTime = now;
        _soundOn = !_soundOn;

        if (_soundOn) {
            _buzzer.play(GAS_FREQUENCY);
        } else {
            _buzzer.stop();
        }

        return;
    }

    // Soil: 200 ms sound, then 4800 ms silence.
    unsigned long elapsed = now - _lastToggleTime;

    if (_soundOn && elapsed >= SOIL_BEEP_DURATION) {
        _buzzer.stop();
        _soundOn = false;
    } else if (!_soundOn && elapsed >= SOIL_INTERVAL) {
        _buzzer.play(SOIL_FREQUENCY);
        _soundOn = true;
        _lastToggleTime = now;
    }
}

AlarmController::AlarmType AlarmController::getActiveAlarm() const {
    return _currentAlarm;
}


void AlarmController::playMelody(
    MelodyPlayer::Melody melody
) {
    if (_currentAlarm != AlarmType::NONE ||
        _gasAlarm.isActive() ||
        _soilAlarm.isActive()) {
        return;
    }

    _melodyPlayer.play(melody);
}

void AlarmController::stopMelody() {
    _melodyPlayer.stop();
}

bool AlarmController::isGasAlarmActive() const {
    return _gasAlarm.isActive();
}

bool AlarmController::isSoilAlarmActive() const {
    return _soilAlarm.isActive();
}
