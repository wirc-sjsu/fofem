#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

/************************************************************************/
//%--------------------------------------------------------------------------
//%                          Physical_Constants_HMV.m                           
//%
//%                   Initialize physical and model constants                  
//%--------------------------------------------------------------------------
//%__________________________________________________________________________
//% Physical constants

extern  double pres0;
extern  double temp0, denair0, diff0, diffH0;
extern  double h2omol, psi0, rgas, temp00;
extern  double h2opsir; 
extern  double stefbol;
extern  double grav;

double csoil0, coefcs1;
double cwater, coefwt1, coefwt2;
double clight, eps0, pi, elec, kBoltz; 
double pres,  presr,  presi; 


extern char soilmoddat[];


/********************************************************************************
*
*
*********************************************************************************/
void Physical_Constants_HMV()
{
   pi = e_pi;
   temp0   = 273.15;
   temp00  = 373.15;
   pres0   = 101325;
   denair0 = 44.65; 
   diff0   = 2.12e-05;
   diffH0  = 1.39e-05;

   rgas    = 8.314;
   h2omol  = 0.01801;
   psi0    = -1e+06;
   h2opsir = h2omol * psi0 / rgas;

   stefbol = 5.67e-08;

   grav    = 9.81;

   clight  = 299792458;
   eps0    = 1e+07 / ( 4.0 * pi * clight * clight );
   elec    = 1.60217733e-19;
   kBoltz  = 1.308568e-23;

//%__________________________________________________________________________
//% pres = ambient background pressure [Pa] 
// global double pres,  presr,  presi; 
   pres  = 92000;    

   if ( !stricmp (soilmoddat,"Manitou4") ) 
      pres = 73800.0;
       
   presr = pres/pres0;
   presi = pres0/pres;  

//%__________________________________________________________________________
//%           Coefficients for temperature dependency of water vapor  
   Water_Vapor_Constants_0(); 
   Water_Vapor_Constants ();
   Water_Vapor_Constants_2(); 

//%__________________________________________________________________________
//% Coefficients for temperature dependency of liquid water properties 
   Liquid_Water_Constants();
   Liquid_Water_Constants_2(); 

//%__________________________________________________________________________
//% Coefficients for temperature dependency of specific heat of  
//% dry soil (csoil0 coefcs1) and water (cwater coefwt1 coefwt2) 
// Declared this global above...
// global  csoil0, coefcs1;
// global  cwater, coefwt1, coefwt2;

csoil0  =  0.870e+06;
coefcs1 =  0.250e+04;
cwater  =  4.216e+06;
coefwt1 = -0.160e+04;
coefwt2 =  0.170e+02;

//%__________________________________________________________________________
//%                       Physical Constants of dry air  

   Dry_Air_Constants_HMV(); 


/************************************************************************/



}