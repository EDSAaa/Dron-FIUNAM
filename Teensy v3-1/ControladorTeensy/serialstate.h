#include <Arduino.h>

struct XYAngle {
  int16_t angleX;
  int16_t angleY;
  int16_t heading;
};

struct Altitude {
  int16_t value;
  int16_t vel;
};


struct IMUValues {
  int16_t accX;
  int16_t accY;
  int16_t accZ;

  int16_t gyroX;
  int16_t gyroY;
  int16_t gyroZ;

};


static enum _serial_state {
  IDLE,
  HEADER_START,
  HEADER_M,
  HEADER_ARROW,
  HEADER_SIZE,
  HEADER_CMD,
} 
c_state;




