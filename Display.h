#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class Display {
public:
    explicit Display(uint8_t address);

    void begin();
    void showMessage(
        const char* firstLine,
        const char* secondLine = ""
    );

    void clear();

private:
    LiquidCrystal_I2C _lcd;

    static constexpr uint8_t COLUMNS = 16;
    static constexpr uint8_t ROWS = 2;
};
