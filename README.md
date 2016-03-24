## Dron-FIUNAM ##


This is a repository that contains the source code of a project developed at UNAM in Mexico, which was a thesis for obtain a degree. 

The purpose of the project was the design, construction and control of an aerial vehicle kind of dron, which could be a system for a laboratory of Automatic Control. At the beginning, the team couldn't decide between the different sort of existing vehicles, but finally was chosen a quadcopter in *X* configuration.

The dron has the components that are listed below:

- Flight Controller (FC) All In One Pro V.2
- Teensy V3.1
- (4) Gemfan 5x4 propellers
- (4) Baby Beast 2150 [KV] Brushless Motors
- (4) X Afro ESC 12 [A]
- LiPo Battery Turnigy Nanotech 3S 45~90C 1300 [mAh]
- 3D printed frame class 250

The operation of the quadcopter is as follows. The FC was programmed with a release of MultiWii, only for use it like a Data Acquisition Module. And on the other hand Teensy has implemented the MultiWii Serial Protocol (MSP) to request all the sensor data to the FC. Simultaneously, this microcontroller calculates a State Feedback Controller and sends the PWM signals to the brushless motors.

The State Feedback controller only consider 8 states (which can be measured with the FC): *z*, *roll*, *pitch*, *yaw*, and its derivatives with respect to time. Thats why the controller only can only adjust the height, but not the *xy* position.
 
This repository includes both the programs that were made for obtain the mathematical model and those that were made for controlling the quadcopter.

### LabVIEW ###

This folder contains the interface that allows the operator manipulate the desired *z*, and watch the graphics of the system variables behavior.

### Mathematica ###

This folder has the file that calculates the mathematical modelling of a quadcopter in *X* configuration using Euler - Lagrange Equations. This includes the Equations of Motion and the State Space Model in their nonlinear form, and also includes the Linear State Space Model.

Because all the model was obtained symbolically, you can use this program to obtain a model for any *X* quadcopter, only changing the numeric values with the parameters of your device.

### Matlab ###

There are two files.

- One is a .m script that contains the numeric parameters of the dron. In this file was designed a LQR Controller and a Reduced Order Observer.
- The other is a Simulink file, which has the necessary blocks for simulating the response of the system (model linear and non linear) with  the controller and observer implemented. 

### Teensy v3.1 ###

Here are two programs written in Wiring (C for Arduino). 

- One program contains the implementation of MultiWii Serial Protocol, whose purpose is allow Tensy request the values of the sensors to the Flight Controller. And with those vales it calculates a State Feedback Controller.
- The other has the same Controller, but additionally it includes a Reduced Order Observer that estimates the value of *dz/dt*.  
