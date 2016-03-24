//Programa para probar el dron como RC

#include <stdlib.h> 
#include <Servo.h> 

#define PINTHROTTLE 2  
#define PINROLL 3
#define PINPITCH 4
#define PINYAW 5
#define PINAUX1 6
#define PINAUX2 7
#define MAXIMOPWM 2000   
#define MINIMOPWM 1000    
#define  BAUD 57600   
  
Servo Throttle;
Servo Roll;
Servo Pitch;
Servo Yaw;
Servo CanalAux1;
Servo CanalAux2;

int throttle=0;
int roll=0;
int pitch=0;
int yaw=0;
int aux1=0;
int aux2=0;
int tiempoCiclo=10;
int timeWithoutConection=0;

int LeerDato(void);  //Prototipo de la función LeerDato
void PararMotores(void); //Prototipo de la función PararMotores

void setup() 
{
  
  Serial.begin(BAUD);
  Throttle.attach(PINTHROTTLE); 
  Roll.attach(PINROLL);
  Pitch.attach(PINPITCH);
  Yaw.attach(PINYAW);
  CanalAux1.attach(PINAUX1);
  CanalAux2.attach(PINAUX2);
  
  delay(500);
  
}

void loop() 
{

  if(Serial.available()>=24)
  {
    
    throttle = LeerDato();
    roll = LeerDato();
    pitch = LeerDato();
    yaw = LeerDato();
    aux1=LeerDato();
    aux2=LeerDato();

     
    Throttle.writeMicroseconds(throttle);
    Roll.writeMicroseconds(roll);
    Pitch.writeMicroseconds(pitch);
    Yaw.writeMicroseconds(yaw);
    CanalAux1.writeMicroseconds(aux1);
    CanalAux2.writeMicroseconds(aux2);

    
    timeWithoutConection=0;
  }
  else
  {
    Serial.write("Crap");
    timeWithoutConection+=tiempoCiclo; //Si no se recibió ningún dato, incrementa el tiempo sin conexión
  }
  
  //Por seguridad, si no se reciben datos en 1 segundos, se decrementa la velocidad hasta apagar los motores
 
  if(timeWithoutConection>=1000)
  {
    PararMotores();
  }


  delay(tiempoCiclo);
  
}

//Función LeerDato
int LeerDato(void)
{
  
  String cadena="";
  char auxiliar;
  
  for(int i=0;i<4;i++)
  {
      auxiliar=(char)Serial.read();
      cadena+=auxiliar;
  }
  
  for(int i=0;i<4;i++)
  {
    Serial.write(cadena[i]);
  }
  return cadena.toInt();
    
}//Fin de la función LeerDato



//Función PararMotores
void PararMotores(void)
{
  while(throttle>1100)
  {
    throttle-=50;
    Throttle.writeMicroseconds(throttle);
   
    delay(50);
  }
  
  throttle=1000;  
  Throttle.writeMicroseconds(throttle);
  
}
