#include <stdio.h>
#include <math.h> 
#include "BMSoil.h"

double BW[7]; 
double BW_Init[7] = {0, 1.99274064, 1.09965342, -0.510839303, -1.75493479, -45.5170352, -6.74694450e+05 };

double Hi[22]; 
double Hi_Init[22] = {0,  0.520094, 0.0850895, -1.08374,  -0.289555,  0.222531,    0.999115,    1.88797, 
                      1.26613, 0.120573,  -0.281378, -0.906851, -0.772479,   -0.489837,   -0.25704,
                     0.161913, 0.257399,  -0.0325372, 0.0698452, 0.00872102, -0.00435673, -0.000593264 };
double  Li[29]; 
double Li_Init[29] = {0,    1.60397357,   -0.646013523,   0.111443906,  0.102997357, -0.0504123634, 
                     0.00609859258,     2.33771842,   -2.78843778,   1.53616167, -0.463045512,
                      0.0832827019, -0.00719201245,    2.19650529,   -4.54580785, 3.55777244, 
                       -1.40944978,    0.275418278, -0.0205938816,   -1.21051378, 1.60812989, 
                      -0.621178141,   0.0716373224,    -2.7203370,    4.57586331, -3.18369245, 
                         1.1168348,    -0.19268305,   0.012913842 };

double Zi[4]; 
double Zi_Init[4] = {0,  0.2358, -0.625, 1.256 }; 

#define e_Qim0 5.36058
double Qim0;  

double Qim[9];  
double Qim_Init[9]  = {0,  2.725384249466, 1.090337041668,  21.45259836736, -47.12759581194, 
                           4.346002813555, 237.5561886971, -417.7353077397,  249.3834003133 };

double  denwx, denix, denwNx, muwx, zlmwx, sdialx, ddiaTx;

#define e_TMax 383.15
double TMax;

/****************************************************************/
void Liquid_Water_Constants_2()
{
int i; 
double muvx, muvTx, lamvx, lavTx, temRx; 
double denwdTx;
double dmTmx, muwTx,lawTx ; 

  for ( i = 0; i < 7; i++ ) 
    BW[i] = BW_Init[i];

  for ( i = 0; i < 22; i++ ) 
    Hi[i] = Hi_Init[i]; 

  for ( i = 0; i < 29; i++ ) 
    Li[i] = Li_Init[i]; 

  for ( i = 0; i < 4; i++ ) 
    Zi[i] = Zi_Init[i]; 

  for ( i = 0; i < 9; i++ )
    Qim[i] = Qim_Init[i];  

  Qim0 = e_Qim0;  
  TMax = e_TMax; 


  calmaxmulaHMV  (TMax, &muvx, &muvTx, &lamvx, &lavTx, &temRx );
  calmaxdenwHMV (TMax, &denwx, &denix, &denwNx, &denwdTx );
  calmaxmulaWHMV(TMax,denwNx,temRx,muvx,muvTx,lamvx,lavTx,denix,denwdTx, &muwx, &muwTx, &dmTmx, &zlmwx, &lawTx ); 
}


#ifdef woowowowo
%--------------------------------------------------------------------------
%                  Liquid_Water_Constants_2.m                           
%--------------------------------------------------------------------------
%__________________________________________________________________________
% Coefficients for temperature dependency of liquid water properties 

% ------------------------ Density = denw [kg/m^3] ------------------------ 

global BW

BW = [ 1.99274064 1.09965342 -0.510839303 -1.75493479 -45.5170352 ...
      -6.74694450e+05 ];
     
% -------------------- Dynamic Viscosity = muw [Pa s] ---------------------

global Hi

Hi = [ 0.520094 0.0850895 -1.08374 -0.289555 0.222531 0.999115 1.88797 ...
       1.26613 0.120573 -0.281378 -0.906851 -0.772479 -0.489837 -0.25704 ...
       0.161913 0.257399 -0.0325372 0.0698452 0.00872102 -0.00435673 ...
      -0.000593264 ];

% ---------------- Thermal Conductivity = zlamw [W/m/K] -------------------    
     
global Li

Li = [ 1.60397357 -0.646013523 0.111443906 0.102997357 -0.0504123634 ... 
       0.00609859258 2.33771842 -2.78843778 1.53616167 -0.463045512 ...
       0.0832827019 -0.00719201245 2.19650529 -4.54580785 3.55777244 ... 
      -1.40944978 0.275418278 -0.0205938816 -1.21051378 1.60812989 ... 
      -0.621178141 0.0716373224 -2.7203370 4.57586331 -3.18369245 ... 
       1.1168348 -0.19268305 0.012913842 ];

% ----------------------- Surface Tension [N/m] ---------------------------

global Zi

Zi = [ 0.2358 -0.625 1.256 ]; 

%--------------------- Static Dialectric Constant [-] ---------------------

global Qim0 Qim

Qim0 = 5.36058; 
Qim  = [ 2.725384249466 1.090337041668  21.45259836736 -47.12759581194 ... 
         4.346002813555 237.5561886971 -417.7353077397  249.3834003133 ];

%__________________________________________________________________________
% High temperature default values for liquid water properties 
% To be assigned when tempk > TMax [K]

global TMax denwx denix denwNx muwx zlmwx sdialx ddiaTx

TMax  = 383.15;
% TMax  = 450;

[muvx muvTx lamvx lavTx temRx] = calmaxmulaHMV(TMax);
[denwx denix denwNx denwdTx]   = calmaxdenwHMV(TMax);
[muwx muwTx dmTmx zlmwx lawTx] = ... 
      calmaxmulaWHMV(TMax,denwNx,temRx,muvx,muvTx,lamvx,lavTx,denix,denwdTx); 
% [sdialx ddiaTx]                = calmaxdialHMV(TMax);  
#endif 