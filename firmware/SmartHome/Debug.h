#pragma once
#include <Arduino.h>

#define DEBUG_ENABLED 0

#if DEBUG_ENABLED
    #define DEBUG_PRINT(x) Serial.print(x)
    #define DEBUG_PRINTLN(x) Serial.println(x)
#else
    #define DEBUG_PRINT(x) do {} while (0)
    #define DEBUG_PRINTLN(x) do {} while (0)
#endif