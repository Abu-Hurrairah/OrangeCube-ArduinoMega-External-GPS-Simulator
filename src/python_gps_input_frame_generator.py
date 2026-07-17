"""
Generate a MAVLink v2 GPS_INPUT frame without using the MAVLink library.

This is useful for understanding what the Arduino is sending and for testing
serial GPS input from a PC.
"""

from __future__ import annotations

import struct

MAVLINK_V2_MAGIC = 0xFD
GPS_INPUT_MSG_ID = 232
GPS_INPUT_CRC_EXTRA = 151


def x25_accumulate(byte: int, crc: int) -> int:
    tmp = byte ^ (crc & 0xFF)
    tmp ^= (tmp << 4) & 0xFF
    return ((crc >> 8) ^ (tmp << 8) ^ (tmp << 3) ^ (tmp >> 4)) & 0xFFFF


def x25_crc(data: bytes, crc_extra: int) -> int:
    crc = 0xFFFF
    for b in data:
        crc = x25_accumulate(b, crc)
    crc = x25_accumulate(crc_extra, crc)
    return crc


def build_gps_input_frame(
    lat: int = 335600920,
    lon: int = 731968180,
    alt: float = 50.0,
    fix_type: int = 3,
    satellites_visible: int = 12,
    seq: int = 0,
    sys_id: int = 42,
    comp_id: int = 220,
) -> bytes:
    """Build a MAVLink v2 GPS_INPUT packet."""

    time_usec = 0
    gps_id = 0
    ignore_flags = 0
    time_week_ms = 0
    time_week = 0
    hdop = 0.8
    vdop = 1.0
    vn = ve = vd = 0.0
    speed_accuracy = 0.2
    horiz_accuracy = 1.0
    vert_accuracy = 1.5
    yaw = 0

    # GPS_INPUT payload layout from common.xml
    payload = struct.pack(
        "<QIHBBBBii ffffffffH".replace(" ", ""),
        time_usec,
        time_week_ms,
        ignore_flags,
        gps_id,
        fix_type,
        satellites_visible,
        time_week,
        lat,
        lon,
        alt,
        hdop,
        vdop,
        vn,
        ve,
        vd,
        speed_accuracy,
        horiz_accuracy,
        vert_accuracy,
        yaw,
    )

    payload_len = len(payload)
    msg_id_bytes = GPS_INPUT_MSG_ID.to_bytes(3, "little")

    header_without_magic = bytes([
        payload_len,
        0,  # incompat flags
        0,  # compat flags
        seq & 0xFF,
        sys_id & 0xFF,
        comp_id & 0xFF,
    ]) + msg_id_bytes

    crc = x25_crc(header_without_magic + payload, GPS_INPUT_CRC_EXTRA)
    checksum = struct.pack("<H", crc)

    return bytes([MAVLINK_V2_MAGIC]) + header_without_magic + payload + checksum


if __name__ == "__main__":
    frame = build_gps_input_frame()
    print("MAVLink GPS_INPUT frame:")
    print(frame.hex(" ").upper())
    print(f"Length: {len(frame)} bytes")
