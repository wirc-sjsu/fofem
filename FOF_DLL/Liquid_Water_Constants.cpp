#include <stdio.h>
#include <math.h> 
#include "BMSoil.h"

extern double  pres0;
 
double  TempR, Tempa, Tempb, Rvp0, RTp0;

double nw[7]; 
double nw_Init[7] = {0, 0, 4, 5, 7, 8, 9 };

double mw[7];
double mw_Init[7] = {0, 1, 2, 3, 4, 5, 6};

double aw[7];
double aw_Init[7] = {0, 1.93763157e-02,  6.74458446e+03, -2.22521604e+05,  1.00231247e+08, 
                -1.63552118e+09,  8.32299658e+09};
 
double bw[7]; 
double bw_Init[7] = {0, 5.78545292e-03, -1.53195665e-02,  3.11337859e-02, -4.23546241e-02, 
                 3.38713507e-02, -1.19946761e-02};   


/*-----------------------------------------------------*/   
double  TempSi;   
#define e_TempSi  (double ) 1.0 / 300.0

#define LWC 1e-06
double aim[5];
double aim_Init[5] =  {0, 280.68*LWC, 511.45*LWC, 61.131*LWC, 0.45903*LWC};

double bim[5];
double bim_Init[5] =  {0, -1.9, -7.7, -19.6, -40}; 

//% ---------------- Thermal Conductivity = zlamw [W/m/K] -------------------    
double cim[5]; 
double cim_Init[5] = {0, 1.663, -1.7781, 1.1567, -0.432115}; 
double dim[5]; 
double dim_Init[5] = {0, -1.15, -3.4, -6, -7.6};       


/****************************************************************
* Init in here because we can't do some of that stuff like
*   "Rv / pres0"  globally in C
****************************************************************/
void  Liquid_Water_Constants()
{
int i,n;
double Rv; 

  for ( i = 0; i < 7; i++ ) { 
    nw[i] = nw_Init[i];
    mw[i] = mw_Init[i];
    aw[i] = aw_Init[i];
    bw[i] = bw_Init[i]; }

  TempSi = e_TempSi; 

  for ( i = 0; i < 5; i++ ) { 
    aim[i] = aim_Init[i];
    bim[i] = bim_Init[i];
    cim[i] = cim_Init[i];
    dim[i]  = dim_Init[i]; } 


   TempR =  10.0;
   Tempa = 593.0;
   Tempb = 232.0;
   Rv    = 461.51805;
   Rvp0  = Rv / pres0;
   RTp0  = Rvp0 * TempR;
}


#ifdef wowowoww
%--------------------------------------------------------------------------
%                      Liquid_Water_Constants.m                           
%--------------------------------------------------------------------------
%__________________________________________________________________________
% Coefficients for temperature dependency of liquid water properties 

% ------------------------ Density = denw [kg/m^3] ------------------------ 

global TempR Tempa Tempb Rvp0 RTp0 nw mw aw bw

TempR =  10;
Tempa = 593;
Tempb = 232;
Rv    = 461.51805;
Rvp0  = Rv/pres0;
RTp0  = Rvp0*TempR;
nw    = [0 4 5 7 8 9];
mw    = [1 2 3 4 5 6];
aw    = [ 1.93763157e-02  6.74458446e+03 -2.22521604e+05  1.00231247e+08... 
         -1.63552118e+09  8.32299658e+09]; 
bw    = [ 5.78545292e-03 -1.53195665e-02  3.11337859e-02 -4.23546241e-02... 
          3.38713507e-02 -1.19946761e-02];      
     
% -------------------- Dynamic Viscosity = muw [Pa s] ---------------------    
     
global TempSi aim bim
     
TempSi = 1/300;
aim    = 1e-06*[280.68 511.45 61.131 0.45903];
bim    = [-1.9 -7.7 -19.6 -40]; 

% ---------------- Thermal Conductivity = zlamw [W/m/K] -------------------    
     
global cim dim

cim = [1.663 -1.7781 1.1567 -0.432115]; 
dim = [-1.15 -3.4 -6 -7.6];       

%__________________________________________________________________________
% High temperature default values for liquid water properties 
% To be assigned when tempk > 383.15 K 
% global denwx denix muwx zlmwx 
% [denwx denix muwx dmTmx zlmwx] = caldwxHMV(383.15);
#endif