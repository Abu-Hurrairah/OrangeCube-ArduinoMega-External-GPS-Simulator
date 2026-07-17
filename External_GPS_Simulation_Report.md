# Simulating an External GPS with Arduino Mega for Pixhawk Orange Cube Plus

## 1. Project Overview

This project demonstrates how an Arduino Mega can be used to simulate an external GPS source for a Pixhawk Orange Cube Plus flight controller. The Arduino sends MAVLink GPS data through a UART connection, allowing the Pixhawk to receive and process GPS-like information in Mission Planner.

The setup is useful for academic testing, telemetry experiments, MAVLink learning, and controlled GPS simulation without relying on a physical GPS module during every test.

## 2. Objective

The objective is to transmit simulated GPS information from an Arduino Mega to a Pixhawk Orange Cube Plus. The Pixhawk is configured to accept GPS data through MAVLink, and Mission Planner is used to verify that the simulated GPS data is received.

## 3. Hardware Components

- Arduino Mega
- Pixhawk Orange Cube Plus
- Mission Planner
- Jumper wires or TELEM cable
- USB cable for Arduino programming
- Optional GPS module for reference testing

## 4. Communication Architecture

The Arduino Mega communicates with the Pixhawk using UART serial communication.

```text
Arduino Mega Serial1 → Pixhawk TELEM2 → MAVLink GPS data → Mission Planner
```

Arduino Mega uses:

- `Serial` at 9600 baud for debugging.
- `Serial1` at 57600 baud for Pixhawk communication.

## 5. Wiring

| Pixhawk TELEM2 | Arduino Mega | Description |
|---|---|---|
| TX | RX1 / Pin 19 | Pixhawk data received by Arduino |
| RX | TX1 / Pin 18 | Arduino data sent to Pixhawk |
| GND | GND | Common reference ground |

The UART connection must be crossed: TX connects to RX, and RX connects to TX.

## 6. Mission Planner Parameters

The Pixhawk must be configured to accept MAVLink GPS input.

Recommended parameter setup:

| Parameter | Value | Description |
|---|---:|---|
| `GPS_TYPE` | `14` | MAVLink GPS input |
| `SERIAL2_BAUD` | `57` | 57600 baud |
| `SERIAL2_PROTOCOL` | `2` | MAVLink protocol |

After changing parameters, reboot the Pixhawk to apply the configuration.

## 7. MAVLink Message Selection

The original workflow describes GPS data simulation using GPS-related MAVLink messages. For sending GPS data into ArduPilot/Pixhawk, this project uses `GPS_INPUT`, because it is designed for external GPS input.

`GPS_RAW_INT` is commonly used by the autopilot to report GPS data outward, while `GPS_INPUT` is better suited for feeding GPS data into the flight controller.

## 8. Arduino Implementation

The Arduino sketch builds a MAVLink `GPS_INPUT` message and sends it repeatedly to the Pixhawk. The simulated values include:

- Latitude
- Longitude
- Altitude
- GPS fix type
- Satellite count
- HDOP / VDOP
- Accuracy values

The Arduino Mega is suitable because it has multiple hardware serial ports, allowing one serial connection for debugging and another for Pixhawk communication.

## 9. Verification

Mission Planner is used for verification. A successful test should show:

- GPS data being recognized.
- Position updating on the map.
- A simulated fix type such as 3D GPS.
- Stable serial communication with no repeated dropouts.

## 10. Troubleshooting

Common issues include:

- TX/RX wiring not crossed correctly.
- Missing common ground.
- Wrong baud rate.
- Pixhawk not rebooted after parameter update.
- GPS_TYPE not set to MAVLink GPS input.
- Wrong MAVLink message type used for GPS input.

## 11. Safety Note

This project should be used only in controlled test environments. Simulated GPS data can affect navigation and vehicle state estimation. It should not be used in real flight unless the entire system has been validated safely.
