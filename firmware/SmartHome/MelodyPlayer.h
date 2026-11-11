#pragma once
#include <Arduino.h>
#include "Buzzer.h"

struct MelodyNote {
    uint16_t frequency;
    uint16_t duration;
};

class MelodyPlayer {
public:
    enum class Melody : uint8_t {
        HAPPY_BIRTHDAY,
        ODE_TO_JOY,
        DOORBELL
    };

    explicit MelodyPlayer(Buzzer& buzzer);

    void play(Melody melody);
    void stop();
    void update();

    bool isPlaying() const;

private:
    struct Note {
        uint16_t frequency;
        uint16_t duration;
    };

    Buzzer& _buzzer;
    const MelodyNote* _notes = nullptr;
    uint8_t _noteCount = 0;
    uint8_t _noteIndex = 0;
    unsigned long _noteStartedAt = 0;
    bool _playing = false;

    void startNote(unsigned long now);
};
