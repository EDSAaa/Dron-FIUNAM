clear all;
clc;

k=7.7e-7;
b=6.1e-8;
g= 9.78; 
m=0.4419;
d=(0.125/sqrt(2));
Ixx=1694.39e-6;
Iyy=1708.46e-6;
Izz=2948.248e-6;

Ts=0.02;

%sistema Lineal

Am=[
    0, 0, 0, 0, 1, 0, 0, 0;
    0, 0, 0, 0, 0, 1, 0, 0;
    0, 0, 0, 0, 0, 0, 1, 0;
    0, 0, 0, 0, 0, 0, 0, 1;
    0, 0, 0, 0, 0, 0, 0, 0;
    0, 0, 0, 0, 0, 0, 0, 0;
    0, 0, 0, 0, 0, 0, 0, 0;
    0, 0, 0, 0, 0, 0, 0, 0
   ];

Bm=[
    0, 0, 0, 0;
    0, 0, 0, 0;
    0, 0, 0, 0;
    0, 0, 0, 0;
    0.00412812, 0.00412812, 0.00412812, 0.00412812;
    -0.0951609, 0.0951609, 0.0951609, -0.0951609;
    0.0943772, 0.0943772, -0.0943772, -0.0943772;
    -0.0490176, 0.0490176, -0.0490176, 0.0490176
   ];

Cm=eye(8);

Dm=zeros(8,4);

CIm=[0,0,0,0,0,0,0,0]';


%Diseño del Controlador por Retroalimentación de Estados

% pd=[-6.1,-5.3,-5.2,-3+i,-3-i,-5.7,-6,-5]
%  
% K=place(Am,Bm,pd)

%Diseño de un controlador LQR discreto

qd=10000;
md=1;
Q=diag([qd,5*qd,5*qd,qd,qd,qd,qd,qd]);    %Matriz de ponderacióm del error
M=diag([md,md,md,md]);            %Matriz de ponderación del controlador

[K,S,pd]=lqrd(Am,Bm,Q,M,Ts);

zeq=1;
omegaeq=1184.55729
X=[zeq,0,0,0,0,0,0,0]'
U=[omegaeq,omegaeq,omegaeq,omegaeq]'
R=K*X+U


K0=pinv(Cm*inv(-Am+Bm*K)*Bm)

r=[zeq,0,0,0,0,0,0,0]'


%Diseño del Observador

Cr = [1, 0, 0, 0, 0, 0, 0, 0;
      0, 1, 0, 0, 0, 0, 0, 0;
      0, 0, 1, 0, 0, 0, 0, 0;
      0, 0, 0, 1, 0, 0, 0, 0; 
      0, 0, 0, 0, 0, 1, 0, 0;
      0, 0, 0, 0, 0, 0, 1, 0;
      0, 0, 0, 0, 0, 0, 0, 1];
  
Dr=zeros(7,4);
pdo=5*real(pd);
L=place(Am',Cr',pdo)';

'Obsevador de Orden Reducido'

f=7 %Número de estados medibles
n=8 %Orden del sistema
T=[1, 0, 0, 0, 0, 0, 0, 0;
   0, 1, 0, 0, 0, 0, 0, 0;
   0, 0, 1, 0, 0, 0, 0, 0;
   0, 0, 0, 1, 0, 0, 0, 0; 
   0, 0, 0, 0, 0, 1, 0, 0;
   0, 0, 0, 0, 0, 0, 1, 0;
   0, 0, 0, 0, 0, 0, 0, 1;
   0, 0, 0, 0, 1, 0, 0, 0]
Ab=T*Am*inv(T)
Bb=T*Bm
Cb=Cr*inv(T)

Ab11=Ab(1:f,1:f)
Ab12=Ab(1:f,f+1:n)
Ab21=Ab(f+1,1:f)
Ab22=Ab(f+1,f+1:n)

B1=Bb(1:f,:)
B2=Bb(f+1,:)

pdoor=[-10]
Lor=(place(Ab22',Ab12',pdoor))'

Az=Ab22-Lor*Ab12
Bz=[(Ab22-Lor*Ab12)*Lor+Ab21-Lor*Ab11,B2-Lor*B1]
Cz=[1]
Dz=[Lor,zeros(1,4)]

%Observador de orden reducido  discreto
SS_OOR=ss(Az,Bz,Cz,Dz);
SS_OORd=c2d(SS_OOR,Ts,'zoh');

Azd=SS_OORd.a;
Bzd=SS_OORd.b;
Czd=SS_OORd.c;
Dzd=SS_OORd.d;

pOORd=eig(Azd)

% Producto matricial K0*r
syms zr
rsim=[zr;0;0;0;0;0;0;0]
K0r=vpa(K0*rsim,4)

% Producto matricial K*x
syms z angx angy angz velz gyrox gyroy gyroz 
x=[z; angx; angy; angz; velz; gyrox; gyroy; gyroz]
Kx=vpa(K*x,4)

'Matriz K0r-Kx'

vpa(K0r-Kx,4)

