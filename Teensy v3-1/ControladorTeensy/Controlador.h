#include <Arduino.h>

//Accionar motores
#include <Servo.h>

#define MAXPWM 1900 
#define MINESC 1000  
#define MINPWM 1350
#define PINMOTOR1 2  
#define PINMOTOR2 3
#define PINMOTOR3 4
#define PINMOTOR4 5  
#define pi 3.141592
#define OMEGAEQ 1184.55729

//Objetos de la clase Servo para los motores
Servo Mot1;
Servo Mot2;
Servo Mot3;
Servo Mot4;

int led=13;
//String inicializationAux="";
//char init;
int inicializationAux=0;

//Variables para el controlador
float angx = 0;
float angy = 0;
float angz = 0;
float z = 0;
float velz = 0;
float gyrox = 0;
float gyroy = 0;
float gyroz = 0;
float omega1 = 0; 
float omega2 = 0; 
float omega3 = 0; 
float omega4 = 0;
int pwmM1=0;
int pwmM2=0;
int pwmM3=0;
int pwmM4=0;

void blink2(void)
{
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}



void escCalibration(void)
{
  Mot1.writeMicroseconds(MAXPWM);
  Mot2.writeMicroseconds(MAXPWM);
  Mot3.writeMicroseconds(MAXPWM);
  Mot4.writeMicroseconds(MAXPWM);
  delay(3000);
  Mot1.writeMicroseconds(MINESC);
  Mot2.writeMicroseconds(MINESC);
  Mot3.writeMicroseconds(MINESC);
  Mot4.writeMicroseconds(MINESC);
  delay(2000);
}

void startMotors(void)
{
  for(int i=MINESC;i<=MINPWM;i=i+10)
  {
    Mot1.writeMicroseconds(i);
    Mot2.writeMicroseconds(i);
    Mot3.writeMicroseconds(i);
    Mot4.writeMicroseconds(i);
    delay(200);
  }
}

void emergencyStop(void)
{     
  Mot1.writeMicroseconds(MINESC);
  Mot2.writeMicroseconds(MINESC);
  Mot3.writeMicroseconds(MINESC);
  Mot4.writeMicroseconds(MINESC);
  delay(50);
}


void control(float zr, int16_t ax, int16_t ay, int16_t az, int16_t al, int16_t vz, int16_t gx, int16_t gy, int16_t gz)
{
  angx = ax/10.0;
  angy = ay/10.0;
  angz = az;
  z = al/100.0;
  velz = vz/100.0;
  gyrox = gx/4.096*pi/180;
  gyroy = gy/4.096*pi/180;
  gyroz = gz/4.096*pi/180;
  
  omega1 = 91.5*angx - 91.62*angy + 45.04*angz + 46.42*gyrox - 46.52*gyroy + 49.88*gyroz - 91.59*velz - 49.24*z + 49.24*zr;
  omega2 = 49.24*zr - 91.62*angy - 45.04*angz - 46.42*gyrox - 46.52*gyroy - 49.88*gyroz - 91.59*velz - 49.24*z - 91.5*angx;
  omega3 = 91.62*angy - 91.5*angx + 45.04*angz - 46.42*gyrox + 46.52*gyroy + 49.88*gyroz - 91.59*velz - 49.24*z + 49.24*zr;
  omega4 = 91.5*angx + 91.62*angy - 45.04*angz + 46.42*gyrox + 46.52*gyroy - 49.88*gyroz - 91.59*velz - 49.24*z + 49.24*zr;
  
  omega1 += OMEGAEQ;
  omega2 += OMEGAEQ;
  omega3 += OMEGAEQ;
  omega4 += OMEGAEQ;
  
  pwmM1 = constrain((int)((0.0002730)*omega1*omega1 + 1004), MINPWM, MAXPWM);
  pwmM2 = constrain((int)((0.0002730)*omega2*omega2 + 1004), MINPWM, MAXPWM);
  pwmM3 = constrain((int)((0.0002730)*omega3*omega3 + 1004), MINPWM, MAXPWM);
  pwmM4 = constrain((int)((0.0002730)*omega4*omega4 + 1004), MINPWM, MAXPWM);
  
  Mot1.writeMicroseconds(pwmM1);
  Mot2.writeMicroseconds(pwmM2);
  Mot3.writeMicroseconds(pwmM3);
  Mot4.writeMicroseconds(pwmM4);
}

void inicialization(void)
{
  while(inicializationAux!=254)
  {
    if(Serial2.available()>0)
    {
      /*if(init=='R')
      {
        inicializationAux="";
      }*/
      inicializationAux=Serial2.read();
      Serial2.write(inicializationAux);

      //inicializationAux+=init;

      //Serial2.write((byte *)"Crap",4);
    }
    Serial2.write((byte *)"Ya",2);
    delay(1000);
   
  }
}

