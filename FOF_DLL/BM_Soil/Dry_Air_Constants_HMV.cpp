
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

double  drymol;
double  tempdi;  

double dil[20];

// global double Jim = 6.1609e-06 * [0.128517 2.60661 -1 -0.709661 0.662534 -0.197846 0.00770147];
// Kim = 25.9778e-03*[0.239503 0.00649768 1 -1.92615 2.00383 -1.07553 0.229414];
#define e_Jim 6.1609e-06
double Jim[10];
double Jim_Init[10] = { 0, 0.128517, 2.60661, -1.0, -0.709661, 0.662534, -0.197846, 0.00770147, 0 };

#define e_Kim 25.9778e-03
double Kim[10];   
double Kim_Init[10] = { 0, 0.239503, 0.00649768, 1.0, -1.92615, 2.00383, -1.07553, 0.229414, 0 };

#define e_nil 1e+06
double nil[20]; 
double nil_Init[20] =  { 0, 2.475830346,   -0.02815239891,   0.1116401165, -0.8147644187,
                            2.185120405,   -2.978031305,    -1.308008001,   0.430594851, 
                           -0.08082302563,  0.006622545214, 0 };  

#define e_oil 1e+05
double oil[20];
double oil_Init[20] = { 0,  6.408242565, -0.01599937045, 0.09984801256, -1.280873444,
                            4.186599156, -6.720142804,   -3.799977202,   1.378691801, 
                           -0.2806954185, 0.02459943097, 0 };  

double pidv, pivd, mvd4;

/********************************************************************************/
void Dry_Air_Constants_HMV()
{
int i; 

  drymol = 0.0289586;
  tempdi = 1.0 / 132.5;  


   for ( i = 1; i < 100; i++ ) {
      if ( Jim_Init[i] == 0 ) 
         break;
      Jim[i] = Jim_Init[i] * e_Jim;  }

  for ( i = 1; i < 100; i++ ) {
      if ( Kim_Init[i] == 0 ) 
         break;
      Kim[i] = Kim_Init[i] * e_Kim;  }

// nil ....................................
  for ( i = 1; i < 100; i++ ) {
     if ( nil_Init[i] == 0 ) 
       break; 
     nil[i] = nil_Init[i] * e_nil; 
     nil[i] = nil[i] * (0.79/drymol);
   }

// oil ...................................
 for ( i = 1; i < 100; i++ ) {
     if ( oil_Init[i] == 0 ) 
       break; 
     oil[i] = oil_Init[i] * e_oil; 
     oil[i] = oil[i] * (0.21/drymol);
     dil[i] = nil[i] + oil[i];
   }


double mdmv, mvmd;  
extern double h2omol; 

mdmv = drymol/h2omol;
mvmd = h2omol/drymol;
pidv = 0.25*xsqrt(2.0/(1.0+mdmv));
pivd = 0.25*xsqrt(2.0/(1.0+mvmd));
mvd4 = 1.0/xsqrt(xsqrt(mdmv));



} 

#ifdef XXXXXXXX

%--------------------------------------------------------------------------
%                        Dry_Air_Constants_HMV.m                           
%--------------------------------------------------------------------------                      

%__________________________________________________________________________
%                       Physical Constants of dry air  

global drymol tempdi 

drymol = 0.0289586;
tempdi = 1/132.5;          
          
%__________________________________________________________________________
%           Coefficients for temperature dependencies of dry air             
% ------------------ Dynamic Viscosity    = Jim ---------------------------
% ------------------ Thermal Conductivity = Kim ---------------------------
% ------------------ Specific Heat        = dil ---------------------------

global Jim Kim
global dil

Jim = 6.1609e-06*[0.128517 2.60661 -1 -0.709661 0.662534 -0.197846 0.00770147];
Kim = 25.9778e-03*[0.239503 0.00649768 1 -1.92615 2.00383 -1.07553 0.229414];
            
nil   = 1e+06*[ 2.475830346  -0.02815239891  0.1116401165 -0.8147644187 ...
                2.185120405  -2.978031305   -1.308008001   0.430594851  ... 
               -0.08082302563 0.006622545214]*0.79/drymol;   
           
oil   = 1e+05*[ 6.408242565 -0.01599937045  0.09984801256 -1.280873444 ...
                4.186599156 -6.720142804   -3.799977202    1.378691801 ... 
               -0.2806954185 0.02459943097]*0.21/drymol;           
            
dil   = nil + oil;          

%__________________________________________________________________________
%            Constants involving ratios of molecular masses

global pidv pivd mvd4

mdmv = drymol/h2omol;
mvmd = h2omol/drymol;
pidv = 0.25*sqrt(2/(1+mdmv));
pivd = 0.25*sqrt(2/(1+mvmd));
mvd4 = 1/sqrt(sqrt(mdmv));

#endif 