#include <Arduino.h>

//Accionar motores
#include <Servo.h>

//Constantes para los motores
#define MAXPWM 1900 
#define MINESC 1000  
#define MINPWM 1350
#define PINMOTOR1 2  
#define PINMOTOR2 3
#define PINMOTOR3 4
#define PINMOTOR4 5  
#define pi 3.141592
#define OMEGAEQ 1211.85

//Objetos de la clase Servo para los motores
Servo Mot1;
Servo Mot2;
Servo Mot3;
Servo Mot4;

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
    delay(30);
  }

  Serial2.write((byte *)"Arranqué",8);
}

void emergencyStop(void)
{     
  Mot1.writeMicroseconds(MINESC);
  Mot2.writeMicroseconds(MINESC);
  Mot3.writeMicroseconds(MINESC);
  Mot4.writeMicroseconds(MINESC);
  delay(50);
}


void control(float zr, int16_t ax, int16_t ay, int16_t az, int16_t al, float vz, int16_t gx, int16_t gy, int16_t gz, float *o1P, float *o2P, float *o3P, float *o4P)
{
  //Ángulos para el controlador en radianes
  angx = ax/10.0*pi/180;
  angy = -ay/10.0*pi/180;
  angz = -az*pi/180;
  
  z = al/100.0; //Altura en m
  //velz = vz/100.0; //Velocidad en z medida (vz está en cm/s)
  
  //Velocidad angular en rad/s
  gyrox = gx/4.096*pi/180;
  gyroy = -gy/4.096*pi/180;
  gyroz = -gz/4.096*pi/180;
  velz = vz; //Velocidad en z observada (vz está en m/s)

  //ECUACIONES DEL CONTROLADOR (Cálculo de las omegas del sistema trasladado al punto de equilibrio)
  omega1 = 91.14*angx - 91.27*angy + 14.3*angz + 46.14*gyrox - 46.25*gyroy + 46.78*gyroz - 66.22*velz - 15.64*z + 15.64*zr;
  omega2 = 15.64*zr - 91.27*angy - 14.3*angz - 46.14*gyrox - 46.25*gyroy - 46.78*gyroz - 66.22*velz - 15.64*z - 91.14*angx;
  omega3 = 91.27*angy - 91.14*angx + 14.3*angz - 46.14*gyrox + 46.25*gyroy + 46.78*gyroz - 66.22*velz - 15.64*z + 15.64*zr;
  omega4 = 91.14*angx + 91.27*angy - 14.3*angz + 46.14*gyrox + 46.25*gyroy - 46.78*gyroz - 66.22*velz - 15.64*z + 15.64*zr;

  //Se actualizan el dato almacenado en las direcciones de memoria o, mejor dicho, se asignan las omegas de equilibrio (omega1eq, omega2eq, omega3eq y omega4eq)
  *o1P = omega1;
  *o2P = omega2;
  *o1P = omega3;
  *o2P = omega4;

  //Cálculo de las omegas del sistema
  omega1 += OMEGAEQ;
  omega2 += OMEGAEQ;
  omega3 += OMEGAEQ;
  omega4 += OMEGAEQ;

  //Cálculo de los tiempos en alto para cada motor
  pwmM1 = constrain((int)((0.0002730)*omega1*omega1 + 1004), MINPWM, MAXPWM);
  pwmM2 = constrain((int)((0.0002730)*omega2*omega2 + 1004), MINPWM, MAXPWM);
  pwmM3 = constrain((int)((0.0002730)*omega3*omega3 + 1004), MINPWM, MAXPWM);
  pwmM4 = constrain((int)((0.0002730)*omega4*omega4 + 1004), MINPWM, MAXPWM);
  
  //Serial.println("Motor1: " + (String)pwmM1 + "\tMotor2: " + (String)pwmM2 + "\tMotor3: " + (String)pwmM3 + "\tMotor4: " + (String)pwmM4 + "\tzref: " + (String)zr + "\tz: " + (String)z + "\tz: " + (String)velz);

  //Se envían las señales a cada ESC, y consecuentemente a cada motor
  Mot1.writeMicroseconds(pwmM1);
  Mot2.writeMicroseconds(pwmM2);
  Mot3.writeMicroseconds(pwmM3);
  Mot4.writeMicroseconds(pwmM4);

}


