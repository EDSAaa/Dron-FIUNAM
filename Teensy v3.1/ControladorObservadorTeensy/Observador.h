//Variables de las velocidades angulares
float om1=0;
float om2=0;
float om3=0;
float om4=0;

//Variables para los valores del estado auxiliar anterior y actual
float zauxB=0;
float zauxN=0;
float vzNow=0; //Velocidad actual

float observador(float zr, int16_t ax, int16_t ay, int16_t az, int16_t al, int16_t gx, int16_t gy, int16_t gz, float *o1, float *o2, float *o3, float *o4, float *zauxBeforeP)
{
  //Estados de equilibrio (x-X)
  
  //Ángulos para el observador en radianes
  angx = ax/10.0*pi/180;
  angy = -ay/10.0*pi/180;
  angz = -az*pi/180;
  
  z = al/100.0-zr;

  //Velocidades angulares en rad/s
  gyrox = gx/4.096*pi/180;
  gyroy = -gy/4.096*pi/180;
  gyroz = -gz/4.096*pi/180;

  //Omegas en el punto de equilibrio
  om1=*o1;
  om2=*o2;
  om3=*o3;
  om4=*o4;
  zauxB=*zauxBeforeP; //Variable z auxiliar anterior
  zauxN = 0.00007314*om1 + 0.00007314*om2 + 0.00007314*om3 + 0.00007314*om4 - 1.813*z + 0.8187*zauxB; //Se calcula zauxN con el espacio de estados Azd*z+Bzd*[y;u]
  vzNow =10*z + zauxB; ///Se calcula la velocidad en z de equilibrio observada con el espacio de estados Czd*z+Dzd*[y;u]
  
  //Actualización de la variable zauxBefore
  *zauxBeforeP = zauxN;
  
  return vzNow;
}

