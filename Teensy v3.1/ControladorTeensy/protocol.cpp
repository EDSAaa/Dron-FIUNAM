#include <Arduino.h>
#include "protocol.h"

Protocol::Protocol() 
{

}

/*
We have to combine two uint8 to a single int16
 */
int16_t readInt16(uint8_t inBuf[], int index) {
  return  (inBuf[index*2+1] << 8) | inBuf[index*2];
}

uint16_t readuInt16FromIndex(uint8_t inBuf[], int index) {
  return  (inBuf[index+1] << 8) | inBuf[index];
}

uint32_t readUInt32FromIndex(uint8_t inBuf[], int index) {
  uint32_t int1 = ((unsigned int)inBuf[index+1] << 8) | inBuf[index];
  uint32_t int2 =  ((unsigned int)inBuf[index+3] << 8) | inBuf[index+2];
  uint32_t result = (int2 << 16) | int1; 
  return result;
}

void Protocol::send_msp(uint8_t opcode, uint8_t * data, uint8_t n_bytes) {

  uint8_t checksum = 0;

  Serial1.write((byte *)"$M<", 3);
  Serial1.write(n_bytes);
  checksum ^= n_bytes;

  Serial1.write(opcode);
  checksum ^= opcode;

  Serial1.write(checksum);
}




XYAngle Protocol::evalAtt(uint8_t inBuf[]) {

  int16_t angx = readInt16(inBuf,0);
  int16_t angy = readInt16(inBuf,1);
  int16_t angle = readInt16(inBuf,2);  

  XYAngle ang = { 
    angx, angy, angle  };
  return ang;
}

Altitude Protocol::evalAlt(uint8_t inBuf[]) {

  uint32_t value = readUInt32FromIndex(inBuf,0);
  int16_t vel = readuInt16FromIndex(inBuf,4);
  
  Altitude alt = { 
    value, vel  };
  return alt;
}


IMUValues Protocol::evalIMU(uint8_t inBuf[]) {

  IMUValues result = { 
    readInt16(inBuf,0), readInt16(inBuf,1), readInt16(inBuf,2), readInt16(inBuf,3), readInt16(inBuf,4), readInt16(inBuf,5)        };
  return result;

}






