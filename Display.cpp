#include "Display.h"

Display::Display(uint8_t address)
    : _lcd(address, COLUMNS, ROWS) {}

void Display::begin() {
    _lcd.init();
    _lcd.backlight();
    clear();
}

void Display::showMessage(
    const char* firstLine,
    const char* secondLine
) {
    clear();

    _lcd.setCursor(0, 0);
    _lcd.print(firstLine);

    _lcd.setCursor(0, 1);
    _lcd.print(secondLine);
}

void Display::clear() {
    _lcd.clear();
}
