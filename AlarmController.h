#pragma once
#include <Arduino.h>
#include "GasAlarm.h"
#include "SoilAlarm.h"
#include "Buzzer.h"
#include "MelodyPlayer.h"

class AlarmController {
public:
    enum class AlarmType {
        NONE,
        SOIL,
        GAS
    };

    AlarmController(
        GasAlarm& gasAlarm,
        SoilAlarm& soilAlarm,
        Buzzer& buzzer,
        MelodyPlayer& melodyPlayer
    );

    void update();
    AlarmType getActiveAlarm() const;
    void playMelody(MelodyPlayer::Melody melody);
    void stopMelody();
    bool isGasAlarmActive() const;
    bool isSoilAlarmActive() const;

private:
    GasAlarm& _gasAlarm;
    SoilAlarm& _soilAlarm;
    Buzzer& _buzzer;
    MelodyPlayer& _melodyPlayer;
    
    AlarmType _currentAlarm = AlarmType::NONE;
    bool _soundOn = false;
    unsigned long _lastToggleTime = 0;

    static constexpr unsigned int GAS_FREQUENCY = 440;
    static constexpr unsigned int SOIL_FREQUENCY = 880;

    static constexpr unsigned long GAS_INTERVAL = 300;
    static constexpr unsigned long SOIL_INTERVAL = 5000;
    static constexpr unsigned long SOIL_BEEP_DURATION = 200;

    AlarmType determineAlarm() const;
    void changeAlarm(AlarmType alarm, unsigned long now);
};
