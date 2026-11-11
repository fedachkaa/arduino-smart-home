#pragma once
#include <Arduino.h>
#include "SoilMoistureSensor.h"

class SoilAlarm {
public:
    explicit SoilAlarm(SoilMoistureSensor& sensor);

    void update();
    bool isActive() const;

private:
    SoilMoistureSensor& _sensor;
    bool _active = false;

    static constexpr int DRY_ON_THRESHOLD = 800;
    static constexpr int DRY_OFF_THRESHOLD = 850;
};
