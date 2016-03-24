//Programa para probar y calibrar los motores brushless

#include <stdlib.h> 
#include <Servo.h> 

#define PINMOTOR1 2  
#define PINMOTOR2 3
#define PINMOTOR3 5
#define PINMOTOR4 6
#define PINMOTORAUX 7
#define MAXIMOPWM 2000   
#define MINIMOPWM 1000    
#define  BAUD 57600   
  
Servo Mot1;
Servo Mot2;
Servo Mot3;
Servo Mot4;

int tiempoAlto1=0;
int tiempoAlto2=0;
int tiempoAlto3=0;
int tiempoAlto4=0;
int tiempoCiclo=10;
int timeWithoutConection=0;

int LeerDato(void);  //Prototipo de la función LeerDato
void CalibrarESC(void); //Prototipo de la función CalibrarESC
void PararMotores(void); //Prototipo de la función PararMotores

void setup() 
{
  
  Serial.begin(BAUD);
  Mot1.attach(PINMOTOR1);
  Mot2.attach(PINMOTOR2);
  Mot3.attach(PINMOTOR3);
  Mot4.attach(PINMOTOR4);
  CalibrarESC();
  delay(500);
  
}

void loop() 
{

  if(Serial.available()>=16)
  {
    
    tiempoAlto1 = LeerDato();
    tiempoAlto2 = LeerDato();
    tiempoAlto3 = LeerDato();
    tiempoAlto4 = LeerDato();

     
    Mot1.writeMicroseconds(tiempoAlto1);
    Mot2.writeMicroseconds(tiempoAlto2);
    Mot3.writeMicroseconds(tiempoAlto3);
    Mot4.writeMicroseconds(tiempoAlto4);

    
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

void CalibrarESC(void)
{
  Mot1.writeMicroseconds(MAXIMOPWM);
  Mot2.writeMicroseconds(MAXIMOPWM);
  Mot3.writeMicroseconds(MAXIMOPWM);
  Mot4.writeMicroseconds(MAXIMOPWM);
  delay(3000);
  Mot1.writeMicroseconds(MINIMOPWM);
  Mot2.writeMicroseconds(MINIMOPWM);
  Mot3.writeMicroseconds(MINIMOPWM);
  Mot4.writeMicroseconds(MINIMOPWM);
  delay(2000);
}

void PararMotores(void)
{
  Mot1.writeMicroseconds(MINIMOPWM);
  Mot2.writeMicroseconds(MINIMOPWM);
  Mot3.writeMicroseconds(MINIMOPWM);
  Mot4.writeMicroseconds(MINIMOPWM);
}
