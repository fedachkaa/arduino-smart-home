
#include "Buzzer.h"

Buzzer::Buzzer(uint8_t pin) : _pin(pin) {}

void Buzzer::begin() {
    pinMode(_pin, OUTPUT);
    stop();
}

void Buzzer::play(unsigned int frequency) {
    tone(_pin, frequency);
}

void Buzzer::stop() {
    noTone(_pin);
}
