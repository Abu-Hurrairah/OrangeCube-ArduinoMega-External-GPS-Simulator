/*
  Optional MAVLink SET_POSITION_TARGET_GLOBAL_INT Demo
  ---------------------------------------------------
  This sketch sends a movement/position target command.

  IMPORTANT:
  This is NOT a GPS simulator. It is included because some early experiments
  use SET_POSITION_TARGET_GLOBAL_INT for movement command testing.

  For external GPS simulation, use:
  Arduino_Mega_GPS_INPUT_Simulator.ino
*/

#include <mavlink.h>

#define PIXHAWK_SERIAL Serial1
#define DEBUG_SERIAL   Serial

void setup()
{
  PIXHAWK_SERIAL.begin(57600);
  DEBUG_SERIAL.begin(9600);
  DEBUG_SERIAL.println("Starting SET_POSITION_TARGET_GLOBAL_INT demo...");
}

void loop()
{
  int32_t target_lat = 335600920;
  int32_t target_lon = 731968180;
  float target_alt = 50.0f;
  uint16_t yaw = 0;

  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];

  mavlink_msg_set_position_target_global_int_pack(
    1,
    200,
    &msg,
    millis(),
    1,
    0,
    MAV_FRAME_GLOBAL_RELATIVE_ALT_INT,
    0b0000111111111000,
    target_lat,
    target_lon,
    target_alt,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    yaw * 100.0f,
    0.0f
  );

  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PIXHAWK_SERIAL.write(buf, len);

  DEBUG_SERIAL.println("Position target command sent.");
  delay(1000);
}
