#pragma once
#include <Arduino.h>
#include "GasSensor.h"

class GasAlarm {
public:
    explicit GasAlarm(GasSensor& sensor);

    void update();
    bool isActive() const;

private:
    GasSensor& _sensor;
    bool _active = false;

    static constexpr int ALARM_ON_THRESHOLD = 700;
    static constexpr int ALARM_OFF_THRESHOLD = 650;
};
