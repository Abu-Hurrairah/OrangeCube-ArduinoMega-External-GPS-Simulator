# Mission Planner Parameter Checklist

Use this checklist before testing Arduino Mega GPS simulation with Pixhawk Orange Cube Plus.

## Required

| Parameter | Value | Notes |
|---|---:|---|
| `GPS_TYPE` | `14` | MAVLink GPS input |
| `SERIAL2_BAUD` | `57` | 57600 baud |
| `SERIAL2_PROTOCOL` | `2` | MAVLink |

## Wiring

```text
Pixhawk TELEM2 TX → Arduino Mega RX1 / Pin 19
Pixhawk TELEM2 RX → Arduino Mega TX1 / Pin 18
Pixhawk GND       → Arduino Mega GND
```

## Verification

- Reboot Pixhawk after writing parameters.
- Open Mission Planner → DATA screen.
- Confirm GPS data appears.
- Confirm map position updates.
