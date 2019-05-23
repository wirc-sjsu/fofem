#include <stdio.h>
#include <math.h> 
#include "BMSoil.h"

double HIM[6];
double HIM_Init[6] = {0, 1.67752, 2.20462, 0.6366564, -0.241605, 0};

double LIM[6];
double LIM_Init[6] = {0, 2.443221, 13.23095, 6.770357, -3.454586, 0.4096266};

double XIM[6];
double XIM_Init[6] = {0, 13405.538, 54188.028, 58822.461, (0.375*54188.028), (2.25*58822.461) };

/************************************************/
void Water_Vapor_Constants_2()
{
int i,n; 

  for ( i = 0; i < 6; i++ ) {
      HIM[i] = HIM_Init[i];
      LIM[i] = LIM_Init[i];
      XIM[i] = XIM_Init[i]; }

 //  n = ARRAY_SIZE(HIM); 
   for ( i = 1; i < 100; i++){
     if ( HIM[i] == 0 ) 
        break; 
     HIM[i] = HIM[i] * 1e+04;
   }
}


#ifdef wowowo
%--------------------------------------------------------------------------
%                      Water_Vapor_Constants_2.m                           
%--------------------------------------------------------------------------
%__________________________________________________________________________
%           Coefficients for temperature dependency of water vapor  
% -------------------- Dynamic Viscosity = HIM -------------------
% ----------------- Thermal Conductivity = LIM ------------------- 
% -------------- Latent Heat of Enthalpy = XIM ------------------- 

global HIM 
global LIM
global XIM

HIM = 1e+04*[1.67752 2.20462 0.6366564 -0.241605];

LIM = [2.443221 13.23095 6.770357 -3.454586 0.4096266];

XIM = [13405.538 54188.028 58822.461 0.375*54188.028 2.25*58822.461];

#endif 

/******************************************************************/
