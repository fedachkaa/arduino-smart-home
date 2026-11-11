#pragma once

#include <Arduino.h>

// Digital pins
constexpr uint8_t PIN_PIR = 2;
constexpr uint8_t PIN_BUZZER = 3;
constexpr uint8_t PIN_BUTTON_1 = 4;
constexpr uint8_t PIN_YELLOW_LED = 5;

// Fan - actual wiring
constexpr uint8_t PIN_FAN_INA = 7;
constexpr uint8_t PIN_FAN_INB = 6;

constexpr uint8_t PIN_BUTTON_2 = 8;
constexpr uint8_t PIN_DOOR_SERVO = 9;
constexpr uint8_t PIN_WINDOW_SERVO = 10;
constexpr uint8_t PIN_GAS_DIGITAL = 11;
constexpr uint8_t PIN_RELAY = 12;
constexpr uint8_t PIN_WHITE_LED = 13;

// Analog pins
constexpr uint8_t PIN_GAS_ANALOG = A0;
constexpr uint8_t PIN_LIGHT = A1;
constexpr uint8_t PIN_SOIL = A2;
constexpr uint8_t PIN_RAIN = A3;

// LCD1602: SDA = A4, SCL = A5
// Bluetooth: UART RX = D0, TX = D1
