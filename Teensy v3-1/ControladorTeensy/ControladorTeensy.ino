//Cabecera del controlador
#include "Controlador.h"

//Cabecera del protocolo de comunicación
#include "protocol.h"

//Máquina de estados
#ifndef serialstate_h
#define serialstate_h
#include "serialstate.h"
#endif



#define Ts 20
#define INBUF_SIZE 64
#define  BAUD 57600   

//Variables para la máquina de estados
static uint8_t inBuf[INBUF_SIZE];
static uint8_t checksum;
static uint8_t commandMW;
static uint8_t offset;
static uint8_t dataSize;

static Protocol p;

int8_t c,n;  

//Tiempos inicial y final
int ti=0;
int tf=0;

int aux=0;
float zRef=0;

XYAngle angles;
Altitude alt;
IMUValues imu;

void setup()  
{
  pinMode(led,OUTPUT);
  Serial2.begin(BAUD);  //telemetría
  Serial1.begin(BAUD); //Multiwii

  Mot1.attach(PINMOTOR1);
  Mot2.attach(PINMOTOR2);
  Mot3.attach(PINMOTOR3);
  Mot4.attach(PINMOTOR4);
  
  escCalibration();
  delay(1000);
  inicialization();
  //Serial2.write((byte *)"Ya calibre",10);
  startMotors();
  delay(2000);
}

void loop() // run over and over
{

  ti=millis();
 
  if(aux==255)
  {
    emergencyStop();
    inicialization();
    startMotors();
    delay(2000);
  }
  else 
  {
    if(Serial2.available()>0)
    {
      aux = Serial2.read();
    }
    
    uint8_t  datad = 0;
    uint8_t  *data = & datad;
    p.send_msp( MSP_ATTITUDE  ,data, 0);
    readData();
    p.send_msp( MSP_ALTITUDE  ,data, 0);
    readData();
    p.send_msp( MSP_RAW_IMU  ,data, 0);
    readData();
    zRef=(float)(aux/100.00);  
    control(zRef,angles.angleX,angles.angleY,angles.heading,alt.value,alt.vel,imu.gyroX,imu.gyroY,imu.gyroZ);
    aux=0;
    tf=millis();
    delay(Ts-(tf-ti));
  }
  
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
          angles = p.evalAtt(inBuf);
          //Serial.print("Ang x: " + (String)(float)(result.angleX/10.0) + "\tAng y: " + (String)(float)(result.angleY/10.0) + "\tAng z: " + (String)(float)result.heading);
          //Serial2.write(result.angleX);
          //Serial2.write(result.angleY);
          //Serial2.write(result.heading);

          
        }
        if (commandMW == MSP_ALTITUDE) {
          alt = p.evalAlt(inBuf);
          //Serial.print("\tAlt: "+(String)(float)(result.value/100.0)+"\tVelZ: "+(String)(float)(result.vel));
          
        }
        if (commandMW == MSP_RAW_IMU) {
          imu = p.evalIMU(inBuf);
           //Serial.println("\tVelAngX: "+(String)(float)(result.gyroX/4.096*pi/180)+"\tVelAngY: "+(String)(float)(result.gyroY/4.096*pi/180 )+"\tVelAngZ: "+(String)(float)(result.gyroZ/4.096*pi/180 ));
          
        }
        
      } 

      c_state = IDLE;
    }

  }
}



