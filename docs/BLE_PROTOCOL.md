# Smart Home BLE protocol

The HM-10-compatible module exposes the Arduino hardware UART at 9600 baud. The development module uses service `FFE0` and characteristic `FFE1`; clients must discover and verify the actual module configuration.

## Framing

Every firmware response is UTF-8/ASCII text terminated by line feed (`\n`). Arduino `Serial.println()` emits `\r\n`, so clients should remove a trailing `\r` from each completed line.

BLE notifications are transport fragments, not protocol records. A client must append notification bytes to a connection-scoped buffer, split only on `\n`, process every complete line, and retain the incomplete suffix for the next notification. The buffer must be cleared when the connection ends.

## Commands

Single-character commands are sent without a terminator. Numeric actuator commands consist of a command character, one to three decimal digits, and `#`.

| Command | Response or action |
| --- | --- |
| `?` | Full status snapshot ending in `STATUS:END:OK` |
| `h` | `LIGHT:<0-1023>` |
| `i` | `GAS:<0-1023>` |
| `j` | `SOIL:<0-1023>` |
| `k` | `RAIN:<0-1023>` |

All actuator commands remain as documented in the project README.

## Records

Sensor readings have two fields:

```text
SENSOR:VALUE
```

Status and event records have three fields:

```text
COMPONENT:FIELD:VALUE
```

The field count disambiguates `LIGHT:<reading>` from `LIGHT:STATE:<state>` and `RAIN:<reading>` from `RAIN:STATE:<state>`.

Motion uses:

```text
MOTION:STATE:ON
MOTION:STATE:OFF
```

The `?` snapshot always includes the current PIR input before `STATUS:END:OK`. Unsolicited motion events report debounced state changes after the firmware's 30-second PIR startup stabilization window. Identical stable states are not repeated.

## Compatibility

Commands are backward-compatible, but the four sensor responses intentionally change from an anonymous decimal line to an identified record. Updated clients should route identified readings by label and may optionally retain legacy decimal parsing only while exactly one sequential legacy request is pending. A legacy timeout or late numeric reply must never be assigned to a newer request.

Status/event parsing remains compatible with the existing three-field records. Unknown well-formed records should be ignored so newer firmware can add components without breaking older clients.
