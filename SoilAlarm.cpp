#include "SoilAlarm.h"

SoilAlarm::SoilAlarm(SoilMoistureSensor& sensor)
    : _sensor(sensor) {}

void SoilAlarm::update() {
    int value = _sensor.read();

    if (value == 0) {
        _active = false;
        return;
    }

    if (!_active && value < DRY_ON_THRESHOLD) {
        _active = true;
    } else if (_active && value >= DRY_OFF_THRESHOLD) {
        _active = false;
    }
}

bool SoilAlarm::isActive() const {
    return _active;
}
