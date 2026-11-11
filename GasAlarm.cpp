#include "GasAlarm.h"

GasAlarm::GasAlarm(GasSensor& sensor)
    : _sensor(sensor) {}

void GasAlarm::update() {
    int value = _sensor.read();

    if (!_active && value > ALARM_ON_THRESHOLD) {
        _active = true;
    } else if (_active && value < ALARM_OFF_THRESHOLD) {
        _active = false;
    }
}

bool GasAlarm::isActive() const {
    return _active;
}
