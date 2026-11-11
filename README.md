# Arduino Smart Home

A modular DIY smart-home prototype built with an Arduino-compatible Keyestudio UNO board. It combines sensor-driven automation, physical controls, alarms, and bidirectional Bluetooth Low Energy (BLE) communication.

The firmware is written in C++ for Arduino and separates hardware devices, automation controllers, and communication logic into dedicated classes. An Android companion app is planned but **not included** in this repository.

## Features

- **Lighting:** automatic white LED control based on ambient light and PIR motion; manual override; PWM brightness control for a separate yellow LED.
- **Window:** servo-operated window with automatic rain response or manual angle control (0–130°).
- **Door:** servo-operated door with Morse-code password entry, PIR-based automatic behavior, and manual Bluetooth controls. A long press on the confirm button triggers a doorbell melody.
- **Ventilation:** fan on/off and adjustable speed.
- **Relay:** remote on/off control.
- **Safety:** gas and soil-moisture alarms with a buzzer; gas alarm takes priority.
- **LCD:** 16×2 I²C display for local feedback.
- **BLE:** command input, full state snapshot, and change-driven state notifications.
- **Non-blocking operation:** controllers use periodic `update()` calls for state changes and timing.

## Hardware

The project uses a Keyestudio UNO-compatible board and the components listed below. Pin assignments come from `Pins.h`.

| Component | Pin(s) |
| --- | --- |
| PIR motion sensor | D2 |
| Buzzer | D3 |
| Morse input button | D4 |
| Yellow PWM LED | D5 |
| Fan driver inputs | D7 (INA), D6 (INB) |
| Confirm / doorbell button | D8 |
| Door servo | D9 |
| Window servo | D10 |
| Gas digital signal (defined, not used by current firmware) | D11 |
| Relay | D12 |
| White LED | D13 |
| Gas analog sensor | A0 |
| Light sensor | A1 |
| Soil-moisture sensor | A2 |
| Rain sensor | A3 |
| LCD1602 I²C | A4 (SDA), A5 (SCL) |
| HM-10-compatible BLE module | Hardware UART: D0 (RX), D1 (TX) |

> **Wiring caution:** D0/D1 are shared with the board's USB serial interface. Disconnect or isolate the BLE module as appropriate when uploading sketches, and ensure the BLE module's RX input uses a safe logic level for its specific breakout board.

## Project structure

| Files | Responsibility |
| --- | --- |
| `SmartHome.ino`, `Pins.h` | Hardware setup, component wiring, main update loop |
| `Led`, `Fan`, `Relay`, `Door`, `Window`, `Buzzer`, `Button`, `Display` | Hardware abstractions |
| `LightSensor`, `MotionSensor`, `RainSensor`, `GasSensor`, `SoilMoistureSensor` | Sensor access |
| `LightController`, `WindowController`, `DoorController`, `AlarmController`, `DisplayController` | Device behavior and automation |
| `GasAlarm`, `SoilAlarm`, `MelodyPlayer`, `MorsePassword` | Alarm, sound, and input logic |
| `BluetoothController` | Incoming commands and outgoing serial messages |
| `EventController` | State-change detection and automatic BLE notifications |
| `ControlMode.h`, `Debug.h` | Shared mode definition and optional debug logging |

The main loop processes Bluetooth commands and buttons, updates the device controllers and alarms, emits change events, and refreshes the LCD.

## Bluetooth interface

The firmware communicates with an HM-10-compatible BLE module using the board's hardware UART at **9600 baud**. The module used for development exposes service `FFE0` and characteristic `FFE1`; check your module's actual GATT configuration.

### Commands

Commands are case-sensitive. Single-character commands do **not** require a terminator. Numeric commands end with `#`.

| Command | Action |
| --- | --- |
| `?` | Send full status snapshot |
| `a` / `b` / `A` | White light on / off / AUTO mode |
| `p` / `q` / `P` | Yellow LED on / off / toggle |
| `v0#` … `v255#` | Set yellow LED brightness |
| `r` / `s` | Fan on / off |
| `1` / `2` / `3` | Fan speed presets: 100 / 180 / 255 |
| `w0#` … `w255#` | Set fan speed |
| `n` / `o` / `N` | Window open / close / AUTO mode |
| `u0#` … `u130#` | Set window angle |
| `l` / `m` / `T` | Door open / close / AUTO mode |
| `t0#` … `t180#` | Set door angle |
| `c` / `d` | Relay on / off |
| `e` / `f` / `g` | Play Happy Birthday / Ode to Joy / stop melody |
| `h` / `i` / `j` / `k` | Read light / gas / soil / rain sensor (raw numeric response) |

The numeric-command parser rejects invalid values, supports up to three digits, and times out incomplete commands after one second.

### Status snapshot

Send `?` to request the current state. The response consists of newline-terminated `COMPONENT:FIELD:VALUE` records and ends with `STATUS:END:OK`.

Example (illustrative values):

```text
FAN:STATE:OFF
FAN:SPEED:0
RELAY:STATE:OFF
LIGHT:MODE:AUTO
LIGHT:STATE:ON
WINDOW:MODE:AUTO
YELLOW_LED:STATE:OFF
YELLOW_LED:BRIGHTNESS:0
DOOR:STATE:CLOSED
DOOR:MODE:AUTO
WINDOW:ANGLE:130
ALARM:GAS:OFF
ALARM:SOIL:OFF
RAIN:STATE:OFF
STATUS:END:OK
```

### Automatic events

`EventController` emits changes using the same `COMPONENT:FIELD:VALUE` format, including:

- Light state and AUTO/MANUAL mode
- Yellow LED state and brightness
- Fan state and speed
- Relay state
- Window mode and angle (angle emitted after movement completes)
- Door state and mode
- Gas and soil alarm state
- Debounced rain state

**Client implementation note:** BLE notifications can split or combine serial text. Buffer incoming data and parse complete lines ending in `\n`; do not assume one notification equals one record. The four raw sensor-read commands currently return plain numbers rather than structured records.

## Getting started

1. Install the Arduino IDE and select the correct UNO-compatible board and serial port.
2. Install the required **LiquidCrystal_I2C** library and any board-specific dependencies used by your setup; the sketch also uses the standard Arduino and Servo/Wire facilities.
3. Connect the components according to `Pins.h` and verify power and logic levels, especially for servos, the fan, and the BLE module.
4. Open `SmartHome/SmartHome.ino` in Arduino IDE, compile, and upload to the board.
5. Connect to the BLE module using a BLE GATT client (for example, LightBlue), enable notifications, send `?`, and verify the snapshot.
6. Test each actuator and sensor before running all components together.

The `Debug.h` macros default to `DEBUG_ENABLED 0` to avoid mixing diagnostic text with the BLE protocol on the shared serial port.

## Automation notes

- White light AUTO mode uses a light threshold and motion detection.
- Window AUTO mode responds to the rain sensor; the rain **event** uses separate hysteresis and debounce thresholds, so an actuator response and a rain notification may not occur at exactly the same instant.
- Gas and soil alarms use thresholds and hysteresis; soil reading `0` is treated as disconnected/inactive in the current implementation.
- Morse-code password: `.--.-.`. Treat this as a demonstration access mechanism, **not** as production-grade physical security.

## Build size (Arduino UNO)

Latest reported Arduino IDE compilation:

| Resource | Used | Capacity |
| --- | ---: | ---: |
| Program flash | 18,114 bytes (56%) | 32,256 bytes |
| Global SRAM | 1,256 bytes (61%) | 2,048 bytes |

These figures are from a reported build and may change with code or library versions. Runtime stack and heap also use SRAM.

## Roadmap

- [x] Modular Arduino firmware
- [x] Sensor-based automation and local controls
- [x] BLE command handling and status snapshots
- [x] Automatic BLE state-change events
- [ ] Android companion app (Kotlin + Jetpack Compose)
- [ ] BLE device discovery, connection, and live dashboard
- [ ] Mobile controls, sensor readings, and alarm presentation

## Safety and limitations

This is a hobby/educational prototype. It is **not** a certified gas detector, fire protection system, security lock, or unattended safety controller. Use suitable power supplies and drivers for motors and servos, and do not connect mains voltage to a breadboard or relay wiring without appropriate electrical expertise and protection.
