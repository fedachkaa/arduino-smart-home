#include "MelodyPlayer.h"

// Frequencies in Hz, durations in milliseconds.
// 0 Hz represents a rest.
namespace {
    constexpr MelodyNote BIRTHDAY[] = {
        {392, 200}, {392, 200}, {440, 400},
        {392, 400}, {523, 400}, {494, 800},
        {392, 200}, {392, 200}, {440, 400},
        {392, 400}, {587, 400}, {523, 800},
        {392, 200}, {392, 200}, {784, 400},
        {659, 400}, {523, 400}, {494, 400},
        {440, 800},
        {698, 200}, {698, 200}, {659, 400},
        {523, 400}, {587, 400}, {523, 800}
    };

    constexpr MelodyNote ODE[] = {
        {330, 300}, {330, 300}, {349, 300},
        {392, 300}, {392, 300}, {349, 300},
        {330, 300}, {294, 300}, {262, 300},
        {262, 300}, {294, 300}, {330, 300},
        {330, 450}, {294, 150}, {294, 600},

        {330, 300}, {330, 300}, {349, 300},
        {392, 300}, {392, 300}, {349, 300},
        {330, 300}, {294, 300}, {262, 300},
        {262, 300}, {294, 300}, {330, 300},
        {294, 450}, {262, 150}, {262, 600}
    };

    constexpr MelodyNote DOORBELL[] = {
        {659, 300},  // E5
        {0, 100},    // Pause
        {523, 500}   // C5
    };
}

MelodyPlayer::MelodyPlayer(Buzzer& buzzer)
    : _buzzer(buzzer) {}

void MelodyPlayer::play(Melody melody) {
    stop();

    switch (melody) {
        case Melody::HAPPY_BIRTHDAY:
            _notes = BIRTHDAY;
            _noteCount = sizeof(BIRTHDAY) / sizeof(BIRTHDAY[0]);
            break;

        case Melody::ODE_TO_JOY:
            _notes = ODE;
            _noteCount = sizeof(ODE) / sizeof(ODE[0]);
            break;

        case Melody::DOORBELL:
            _notes = DOORBELL;
            _noteCount = sizeof(DOORBELL) / sizeof(DOORBELL[0]);
            break;
    }

    _noteIndex = 0;
    _playing = true;
    startNote(millis());
}

void MelodyPlayer::startNote(unsigned long now) {
    _noteStartedAt = now;

    const MelodyNote& note = _notes[_noteIndex];

    if (note.frequency == 0) {
        _buzzer.stop();
    } else {
        _buzzer.play(note.frequency);
    }
}

void MelodyPlayer::update() {
    if (!_playing) {
        return;
    }

    unsigned long now = millis();

    if (now - _noteStartedAt <
        _notes[_noteIndex].duration) {
        return;
    }

    ++_noteIndex;

    if (_noteIndex >= _noteCount) {
        stop();
        return;
    }

    startNote(now);
}

void MelodyPlayer::stop() {
    if (_playing) {
        _buzzer.stop();
    }

    _playing = false;
    _notes = nullptr;
    _noteCount = 0;
    _noteIndex = 0;
}

bool MelodyPlayer::isPlaying() const {
    return _playing;
}
