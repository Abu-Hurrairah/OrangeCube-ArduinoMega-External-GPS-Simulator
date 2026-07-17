/*
  Orange Cube Plus External GPS Simulator using Arduino Mega
  --------------------------------------------------------
  Purpose:
  Sends MAVLink GPS_INPUT messages from Arduino Mega to Pixhawk Orange Cube Plus.

  Wiring:
  Pixhawk TELEM2 TX -> Arduino Mega RX1 / Pin 19
  Pixhawk TELEM2 RX -> Arduino Mega TX1 / Pin 18
  Pixhawk GND       -> Arduino Mega GND

  Arduino Serial:
  Serial  = USB debug monitor at 9600
  Serial1 = Pixhawk TELEM2 at 57600

  Mission Planner suggested parameters:
  GPS_TYPE        = 14  // MAVLink GPS
  SERIAL2_BAUD    = 57  // 57600
  SERIAL2_PROTOCOL= 2   // MAVLink

  Requires MAVLink C headers/library available to Arduino IDE.
*/

#include <mavlink.h>

#define PIXHAWK_SERIAL Serial1
#define DEBUG_SERIAL   Serial

const uint8_t SYSTEM_ID = 42;      // Arduino simulated system ID
const uint8_t COMPONENT_ID = 220;  // MAV_COMP_ID_GPS is commonly 220

// Islamabad example coordinates, scaled by 1e7
int32_t sim_lat = 335600920;
int32_t sim_lon = 731968180;
float sim_alt_m = 50.0f;

uint32_t last_send_ms = 0;
const uint32_t SEND_INTERVAL_MS = 200; // 5 Hz GPS update

void sendGpsInput()
{
  mavlink_message_t msg;
  uint8_t buffer[MAVLINK_MAX_PACKET_LEN];

  uint64_t time_usec = (uint64_t)millis() * 1000ULL;
  uint8_t gps_id = 0;

  // Ignore velocity and accuracy fields if not needed.
  // Keep lat, lon, alt, fix_type, satellites, hdop/vdop active.
  uint16_t ignore_flags = 0;

  uint32_t time_week_ms = 0;
  uint16_t time_week = 0;
  uint8_t fix_type = 3; // 3D fix

  float hdop = 0.8f;
  float vdop = 1.0f;
  float vn = 0.0f;
  float ve = 0.0f;
  float vd = 0.0f;
  float speed_accuracy = 0.2f;
  float horiz_accuracy = 1.0f;
  float vert_accuracy = 1.5f;
  uint8_t satellites_visible = 12;
  uint16_t yaw = 0; // cdeg, 0 if unavailable

  mavlink_msg_gps_input_pack(
    SYSTEM_ID,
    COMPONENT_ID,
    &msg,
    time_usec,
    gps_id,
    ignore_flags,
    time_week_ms,
    time_week,
    fix_type,
    sim_lat,
    sim_lon,
    sim_alt_m,
    hdop,
    vdop,
    vn,
    ve,
    vd,
    speed_accuracy,
    horiz_accuracy,
    vert_accuracy,
    satellites_visible,
    yaw
  );

  uint16_t len = mavlink_msg_to_send_buffer(buffer, &msg);
  PIXHAWK_SERIAL.write(buffer, len);

  DEBUG_SERIAL.print("GPS_INPUT sent | lat: ");
  DEBUG_SERIAL.print(sim_lat / 10000000.0, 7);
  DEBUG_SERIAL.print(" lon: ");
  DEBUG_SERIAL.print(sim_lon / 10000000.0, 7);
  DEBUG_SERIAL.print(" alt: ");
  DEBUG_SERIAL.print(sim_alt_m);
  DEBUG_SERIAL.println(" m");
}

void setup()
{
  DEBUG_SERIAL.begin(9600);
  PIXHAWK_SERIAL.begin(57600);

  DEBUG_SERIAL.println("Orange Cube Plus External GPS Simulator");
  DEBUG_SERIAL.println("Sending MAVLink GPS_INPUT on Serial1...");
}

void loop()
{
  uint32_t now = millis();

  if (now - last_send_ms >= SEND_INTERVAL_MS) {
    last_send_ms = now;
    sendGpsInput();
  }
}
