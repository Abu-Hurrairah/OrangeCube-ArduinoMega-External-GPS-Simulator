"""Send MAVLink GPS_INPUT frames from a PC to Pixhawk over serial."""

from __future__ import annotations

import argparse
import time
import serial
from python_gps_input_frame_generator import build_gps_input_frame


def main() -> None:
    parser = argparse.ArgumentParser(description="Send MAVLink GPS_INPUT frames over serial.")
    parser.add_argument("--port", required=True, help="Serial port, e.g. COM6 or /dev/ttyUSB0")
    parser.add_argument("--baud", type=int, default=57600)
    parser.add_argument("--rate", type=float, default=5.0, help="Messages per second")
    parser.add_argument("--lat", type=int, default=335600920, help="Latitude scaled by 1e7")
    parser.add_argument("--lon", type=int, default=731968180, help="Longitude scaled by 1e7")
    parser.add_argument("--alt", type=float, default=50.0, help="Altitude in meters")
    args = parser.parse_args()

    delay = 1.0 / args.rate
    seq = 0

    with serial.Serial(args.port, args.baud, timeout=1) as ser:
        print(f"Sending GPS_INPUT frames on {args.port} at {args.baud} baud...")
        while True:
            frame = build_gps_input_frame(lat=args.lat, lon=args.lon, alt=args.alt, seq=seq)
            ser.write(frame)
            print(f"sent seq={seq} lat={args.lat / 1e7:.7f} lon={args.lon / 1e7:.7f} alt={args.alt}m")
            seq = (seq + 1) % 256
            time.sleep(delay)


if __name__ == "__main__":
    main()
