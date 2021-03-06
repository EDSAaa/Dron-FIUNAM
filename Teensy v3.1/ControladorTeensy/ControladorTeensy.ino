//Cabecera del controlador
#include "Controlador.h"

//Cabecera del protocolo de comunicación
#include "protocol.h"

//Definiciones de la Máquina de estados
#ifndef serialstate_h
#define serialstate_h
#include "serialstate.h"
#endif

//Variables para el tiempo de muestreo y de los datos adquiridos
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

//Variable auxiliar y referencia de altura
int aux=0;
float zRef=0;

//Variables para los valores de los sensores
XYAngle angles;
Altitude alt;
IMUValues imu;

void setup()  
{
  Serial.begin(BAUD); //Monitor Serial PC
  Serial1.begin(BAUD); //Multiwii
  Serial2.begin(BAUD);  //telemetría

  //Declaración de los pines de los motores
  Mot1.attach(PINMOTOR1);
  Mot2.attach(PINMOTOR2);
  Mot3.attach(PINMOTOR3);
  Mot4.attach(PINMOTOR4);

  //Inicialización
  escCalibration();
  delay(10000);
  inicialization();
  startMotors();
}

void loop() //Bucle principal
{

  ti=millis(); //Se verifica el tiempo inicial

  //Se revisa si llegó información por el puerto serial 2
  if(Serial2.available()>0)
  {
    aux = Serial2.read();
  }

  //Solicitud de los datos a MultiWii con el MSP
  uint8_t  datad = 0;
  uint8_t  *data = & datad;
  p.send_msp( MSP_ATTITUDE  ,data, 0);
  readData();
  p.send_msp( MSP_ALTITUDE  ,data, 0);
  readData();
  p.send_msp( MSP_RAW_IMU  ,data, 0);
  readData();

  //Se verifica si llegó la señal de paro
  if(aux==255)
  {
    emergencyStop();
    inicialization();
    startMotors();
    delay(2000);
  }
  else if (aux!=254) //Si no llegó paro, ejecuta el controlador
  {
    zRef=(float)(aux/100.00);  //Zref en metros
    control(zRef,angles.angleX,angles.angleY,angles.heading,alt.value,alt.vel,imu.gyroX,imu.gyroY,imu.gyroZ);
    tf=millis(); //Se verifica el tiempo final
    delay(Ts-(tf-ti)); //Se asegura que el periodo de muestreo sea constante
  } 
}

void readData() {

  delayMicroseconds(3000);

  //Máquina de estados para solicitar los datos a MultiWii
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
        }
        if (commandMW == MSP_ALTITUDE) {
          alt = p.evalAlt(inBuf);
        }
        if (commandMW == MSP_RAW_IMU) {
          imu = p.evalIMU(inBuf);
        } 
      } 
      
      c_state = IDLE;
    }

  }
}


void inicialization(void)
{
  //Se espera a que se reciba un 254 para arrancar los motores
  while(inicializationAux!=254)
  {
    uint8_t  datad = 0;
    uint8_t  *data = & datad;
    p.send_msp( MSP_ATTITUDE  ,data, 0);
    readData();
    p.send_msp( MSP_ALTITUDE  ,data, 0);
    readData();
    p.send_msp( MSP_RAW_IMU  ,data, 0);
    readData();
    delay(Ts);
    
    if(Serial2.available()>0)
    {
      inicializationAux=Serial2.read();
    }
  }
  
  inicializationAux=0;
  Serial2.write((byte *)"INICIO",6);
  delay(1000);
}

