#include "Pins.h"
#include "Led.h"
#include "LightSensor.h"
#include "MotionSensor.h"
#include "RainSensor.h"
#include "Window.h"
#include "GasSensor.h"
#include "Buzzer.h"
#include "GasAlarm.h"
#include "SoilMoistureSensor.h"
#include "SoilAlarm.h"
#include "AlarmController.h"
#include "Fan.h"
#include "Door.h"
#include "Button.h"
#include "MorsePassword.h"
#include "DoorController.h"
#include "Display.h"
#include "DisplayController.h"
#include "BluetoothController.h"
#include "LightController.h"
#include "WindowController.h"
#include "Relay.h"
#include "MelodyPlayer.h"
#include "EventController.h"

constexpr int DARKNESS_THRESHOLD = 300;
constexpr int RAIN_THRESHOLD = 800;

constexpr int WINDOW_OPEN_ANGLE = 130;
constexpr int WINDOW_CLOSED_ANGLE = 0;

Led whiteLed(PIN_WHITE_LED);
Led yellowLed(PIN_YELLOW_LED);
LightSensor lightSensor(PIN_LIGHT);
MotionSensor motionSensor(PIN_PIR);
RainSensor rainSensor(PIN_RAIN);
Window window(PIN_WINDOW_SERVO);
GasSensor gasSensor(PIN_GAS_ANALOG);
Buzzer buzzer(PIN_BUZZER);
MelodyPlayer melodyPlayer(buzzer);
SoilMoistureSensor soilSensor(PIN_SOIL);
GasAlarm gasAlarm(gasSensor);
SoilAlarm soilAlarm(soilSensor);
AlarmController alarmController(gasAlarm, soilAlarm, buzzer, melodyPlayer);
Fan fan(PIN_FAN_INA, PIN_FAN_INB);
Door door(PIN_DOOR_SERVO);
Button morseButton(PIN_BUTTON_1);
Button confirmButton(PIN_BUTTON_2);
MorsePassword morsePassword(morseButton, confirmButton);
DoorController doorController(door, morsePassword, motionSensor);
Display display(0x27);
Relay relay(PIN_RELAY);
DisplayController displayController(display, morsePassword, doorController);
LightController lightController(whiteLed, lightSensor, motionSensor);
WindowController windowController(window, rainSensor);
BluetoothController bluetoothController(
    fan, 
    lightController, 
    windowController, 
    yellowLed, 
    relay, 
    lightSensor, 
    gasSensor, 
    soilSensor, 
    rainSensor, 
    alarmController, 
    doorController
);
EventController eventController(
    gasAlarm,
    soilAlarm,
    rainSensor,
    doorController,
    lightController,
    windowController,
    fan,
    relay,
    yellowLed,
    bluetoothController
);

void setup() {
    Serial.begin(9600);

    whiteLed.begin();
    yellowLed.begin();
    lightSensor.begin();
    motionSensor.begin();
    rainSensor.begin();
    window.begin(WINDOW_OPEN_ANGLE);
    gasSensor.begin();
    soilSensor.begin();
    buzzer.begin();
    fan.begin();
    door.begin(0);
    morseButton.begin();
    confirmButton.begin();
    doorController.begin();
    display.begin();
    displayController.begin();
    relay.begin();
}

void loop() {
    // Bluetooth commands
    bluetoothController.update();

    // Input
    morseButton.update();
    confirmButton.update();
    morsePassword.update();

    if (morsePassword.wasDoorbellRequested()) {
        alarmController.playMelody(MelodyPlayer::Melody::DOORBELL);
    }

    // Automatic / manual control
    lightController.update();
    windowController.update();
    doorController.update();

    // Alarms
    alarmController.update();

    // BLE events
    eventController.update();

    // LCD
    displayController.update();
}