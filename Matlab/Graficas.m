%Generación de Gráficas

Linewidth=2;

%Estados z
figura=figure(1);
plot(Estados_z(:,1),Estados_z(:,2), 'r','linewidth',Linewidth);
hold on;
plot(Estados_z(:,1),Estados_z(:,3), 'b','linewidth',Linewidth);
grid on;
title('Posición y velocidad vertical','FontName','Arial','FontSize', 18);
xlabel('Tiempo [s]');
%ylabel('Desplazamiento [m]');
legend1=legend('z [m]','z_p [m/s]','Location','best');
set(legend1,'FontSize',16);
nombre=strcat('Estz_NL');
saveas(figura,nombre,'jpg');

%Ángulos
figura=figure(2);
plot(Angulos(:,1),Angulos(:,2), 'g','linewidth',Linewidth);
hold on;
plot(Angulos(:,1),Angulos(:,3), 'k','linewidth',Linewidth);
hold on;
plot(Angulos(:,1),Angulos(:,4), 'c','linewidth',Linewidth);
grid on;
title('Ángulos de Navegación','FontName','Arial','FontSize', 18);
xlabel('Tiempo [s]');
ylabel('Ángulo [°]');
legend2=legend('\phi [°]','\theta [°]','\psi [°]','Location','best');
set(legend2,'FontSize',16);
nombre=strcat('Angulos_NL');
saveas(figura,nombre,'jpg');

%Velocidades angulares
figura=figure(3);
plot(VelAng(:,1),VelAng(:,2), 'g','linewidth',Linewidth);
hold on;
plot(VelAng(:,1),VelAng(:,3), 'k','linewidth',Linewidth);
hold on;
plot(VelAng(:,1),VelAng(:,4), 'c','linewidth',Linewidth);
grid on;
title('Velocidades Angulares','FontName','Arial','FontSize', 18);
xlabel('Tiempo [s]');
ylabel('Velocidad Angular [rad/s]');
legend3=legend('\phi_p [rad/s]','\theta_p [rad/s]','\psi_p [rad/s]','Location','best');
set(legend3,'FontSize',16);
nombre=strcat('VelAng_NL');
saveas(figura,nombre,'jpg');

%Plano XY CI
figura=figure(4);
plot(XY_CI(:,1),XY_CI(:,2), 'r','linewidth',Linewidth);
hold on;
plot(XY_CI(:,1),XY_CI(:,3), 'b','linewidth',Linewidth);
hold on;
plot(XY_CI(:,1),XY_CI(:,4), 'g','linewidth',Linewidth);
hold on;
plot(XY_CI(:,1),XY_CI(:,5), 'c','linewidth',Linewidth);
grid on;
title('Posición y Velocidad en el plano XY','FontName','Arial','FontSize', 18);
xlabel('Tiempo [s]');
%ylabel('Velocidad Angular [rad/s]');
legend4=legend('x [m]','y [m]','x_p [m/s]','y_p [m/s]','Location','best');
set(legend4,'FontSize',16);
nombre=strcat('XY_CI_NL');
saveas(figura,nombre,'jpg');

%Omega
figura=figure(5);
plot(Omega(:,1),Omega(:,2), 'r','linewidth',Linewidth);
hold on;
plot(Omega(:,1),Omega(:,3), 'b','linewidth',Linewidth);
hold on;
plot(Omega(:,1),Omega(:,4), 'c','linewidth',Linewidth);
hold on;
plot(Omega(:,1),Omega(:,5), 'g','linewidth',Linewidth);
grid on;
title('Señal de Control (\omega)','FontName','Arial','FontSize', 18);
xlabel('Tiempo [s]');
ylabel('Velocidad Angular [rad/s]');
legend5=legend('\omega_1 [rad/s]','\omega_2 [rad/s]','\omega_3 [rad/s]','\omega_4 [rad/s]','Location','best');
set(legend5,'FontSize',16);
nombre=strcat('Omega_NL');
saveas(figura,nombre,'jpg');

%Tiempo en alto
figura=figure(6);
plot(Talto(:,1),Talto(:,2), 'r','linewidth',Linewidth);
hold on;
plot(Talto(:,1),Talto(:,3), 'b','linewidth',Linewidth);
hold on;
plot(Talto(:,1),Talto(:,4), 'c','linewidth',Linewidth);
hold on;
plot(Talto(:,1),Talto(:,5), 'g','linewidth',Linewidth);
grid on;
title('Señal de Control (Tiempo en alto)','FontName','Arial','FontSize', 18);
xlabel('Tiempo [s]');
ylabel('Tiempo en alto [\mu s]');
legend6=legend('T_1 [\mu s]','T_2 [\mu s]','T_3 [\mu s]','T_4 [\mu s]','Location','best');
set(legend6,'FontSize',16);
nombre=strcat('Talto_NL');
saveas(figura,nombre,'jpg');

% Porcentaje de PWM
figura=figure(7);

hold on;
grid on;

plot(Talto(:,1),(Talto(:,2)-1000)*100/1000, 'r','linewidth',Linewidth);
plot(Talto(:,1),(Talto(:,3)-1000)*100/1000, 'b','linewidth',Linewidth);
plot(Talto(:,1),(Talto(:,4)-1000)*100/1000, 'c','linewidth',Linewidth);
plot(Talto(:,1),(Talto(:,5)-1000)*100/1000, 'g','linewidth',Linewidth);
title('Señal de Control (Porcentaje de PWM)','FontName','Arial','FontSize', 18);
xlabel('Tiempo [s]');
ylabel('PWM [%]');
legend7=legend('PWM1 [%]','PWM2 [%]','PWM3 [%]','PWM4 [%]','Location','best');
set(legend7,'FontSize',16);
nombre=strcat('PorPWM_NL');
saveas(figura,nombre,'jpg');





%Error de observación
% figura=figure(7);
% plot(ErrorZp(:,1),ErrorZp(:,2), 'r','linewidth',Linewidth);
% hold on;
% plot(ErrorZp(:,1),ErrorZp(:,3), 'b','linewidth',Linewidth);
% hold on;
% plot(ErrorZp(:,1),ErrorZp(:,4), 'k','linewidth',Linewidth);
% grid on;
% title('Error de observación de la velocidad en z','FontName','Arial','FontSize', 18);
% xlabel('Tiempo [s]');
% ylabel('Velocidad [m/s]');
% legend7=legend('z_p [m/s]','z_p(obs) [m/s]','Error de observación en z_p [m/s]','Location','SouthEast');
% set(legend7,'FontSize',16);
% nombre=strcat('ErrorZp_NL');
% saveas(figura,nombre,'jpg');

% %Plano XY CI=0
% figura=figure(8);
% plot(XY_CI(:,1),XY_CI(:,2), 'r','linewidth',Linewidth);
% hold on;
% plot(XY_CI(:,1),XY_CI(:,3), 'b','linewidth',Linewidth);
% hold on;
% plot(XY_CI(:,1),XY_CI(:,4), 'g','linewidth',Linewidth);
% hold on;
% plot(XY_CI(:,1),XY_CI(:,5), 'c','linewidth',Linewidth);
% grid on;
% title('Posición y Velocidad en el plano XY (CI=0)','FontName','Arial','FontSize', 18);
% xlabel('Tiempo [s]');
% %ylabel('Velocidad Angular [rad/s]');
% legend8=legend('x [m]','y [m]','x_p [m/s]','y_p [m/s]','Location','SouthWest');
% set(legend8,'FontSize',16);
% nombre=strcat('XY_SCI_NL');
% saveas(figura,nombre,'jpg');

%Estados z Lineal vs No lineal
figura=figure(9);
plot(Z_L_NL(:,1),Z_L_NL(:,2), 'r','linewidth',Linewidth);
hold on;
plot(Z_L_NL(:,1),Z_L_NL(:,3), 'b','linewidth',Linewidth);
grid on;
title('Posición vertical (Lineal vs No Lineal)','FontName','Arial','FontSize', 18);
xlabel('Tiempo [s]');
ylabel('Posición [m]');
legend9=legend('z (No Lineal) [m]','z (Lineal) [m]','Location','best');
set(legend9,'FontSize',16);
nombre=strcat('z_L_NL');
saveas(figura,nombre,'jpg');
