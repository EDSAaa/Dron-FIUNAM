#include <SD.h>
#include "protocol.h"

#ifndef serialstate_h
#define serialstate_h
#include "serialstate.h"
#endif

#ifndef cardlogger_h
#define cardlogger_h
#include "cardlogger.h"
#endif

#define pi 3.141592
#define INBUF_SIZE 64


static uint8_t inBuf[INBUF_SIZE];
static uint8_t checksum;
static uint8_t commandMW;
static uint8_t offset;
static uint8_t dataSize;

static Protocol p;
static CardLogger logger;
int8_t c,n;  

//global variable for altitude
float current_altitude;

void setup()  
{

  Serial2.begin(57600);  //telemetría
  Serial1.begin(57600); //Multiwii
  //logger.init();
  delay(5000);

}

void loop() // run over and over
{

  uint8_t  datad = 0;
  uint8_t  *data = & datad;

  // If you dont need a certain log just comment out the two lines for sending and reading
  p.send_msp( MSP_ATTITUDE  ,data, 0);
  readData();
  p.send_msp( MSP_ALTITUDE  ,data, 0);
  readData();
  p.send_msp( MSP_RAW_IMU  ,data, 0);
  readData();

  //p.send_msp( MSP_RC  ,data, 0);
  //readData();
  //p.send_msp( MSP_MOTOR ,data, 0);
  //readData(); 
  //p.send_msp( MSP_RAW_GPS  ,data, 0);
  //readData();
  
  //Compare altitude for servo deployment
  //When altitude reaches 25,000 m , send a message to the multiwii for opening the servo bay.
  /*
  if current_altitide => 25000
    data = [ 1000, 1000, 1000, 1000, 2000, 1000, 1000, 1000 ];
    p.send_msp( MSP_SET_RAW_RC  ,data, 0);
  
  */
  //delay(50);
}

void readData() {

  delayMicroseconds(3000);

  while (Serial1.available()) {

    byte c = Serial1.read();
    Serial2.write(c);

    if (c_state == IDLE) {
      c_state = (c=='$') ? HEADER_START : IDLE;
    } 
    else if (c_state == HEADER_START) {
      c_state = (c=='M') ? HEADER_M : IDLE;
    } 
    else if (c_state == HEADER_M) {
      c_state = (c=='>') ? HEADER_ARROW : IDLE;
    } 
    else if (c_state == HEADER_ARROW) {

      if (c > INBUF_SIZE) {  // now we are expecting the payload size
        c_state = IDLE;

      } 
      else {
        dataSize = c;
        offset = 0;
        checksum = 0;
        checksum ^= c;
        c_state = HEADER_SIZE;  
      }
    } 
    else if (c_state == HEADER_SIZE) {
      commandMW = c;
      checksum ^= c;
      c_state = HEADER_CMD;
    } 
    else if (c_state == HEADER_CMD && offset < dataSize) {
      checksum ^= c;
      inBuf[offset++] = c;
    } 
    else if (c_state == HEADER_CMD && offset >= dataSize) {

      if (checksum == c) {
        if (commandMW == MSP_ATTITUDE) {
          XYAngle result = p.evalAtt(inBuf);
          //Serial.print("Ang x: " + (String)(float)(result.angleX/10.0) + "\tAng y: " + (String)(float)(result.angleY/10.0) + "\tAng z: " + (String)(float)result.heading);
          //Serial2.write(result.angleX);
          //Serial2.write(result.angleY);
          //Serial2.write(result.heading);

           //logger.logXYAngle(result);
        }
        if (commandMW == MSP_ALTITUDE) {
          Altitude result = p.evalAlt(inBuf);
          //Serial.print("\tAlt: "+(String)(float)(result.value/100.0)+"\tVelZ: "+(String)(float)(result.vel));
          //logger.logAltitude(result);
          //get altitude and save it as a global variable
          //current_altitude = result;
        }
        if (commandMW == MSP_RAW_IMU) {
          IMUValues result = p.evalIMU(inBuf);
           //Serial.println("\tVelAngX: "+(String)(float)(result.gyroX/4.096*pi/180)+"\tVelAngY: "+(String)(float)(result.gyroY/4.096*pi/180 )+"\tVelAngZ: "+(String)(float)(result.gyroZ/4.096*pi/180 ));
          //logger.logIMU(result);
        }
        if (commandMW == MSP_RC) {
          //RCInput result = p.evalRC(inBuf);
          //logger.logRC(result);
        }
        if (commandMW == MSP_RAW_GPS) {
          //GPSValues result = p.evalGPS(inBuf);
          //logger.logGPS(result);
        }

      } 

      c_state = IDLE;
    }

  }
}



