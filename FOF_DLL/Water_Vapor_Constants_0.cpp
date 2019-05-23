//%--------------------------------------------------------------------------
//%                      Water_Vapor_Constants_0.m                           
//%--------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>
#include <string.h> 
#include "BMSoil.h"

double Backward_Sat (double pres );
void Crit_H2O_Vapor_nSTP();

//%__________________________________________________________________________
//%  Coefficients for calculating saturated vapor pressure = esat [Pa]  
double prescr;
#define e_prescr (double) 22.064e+06

double ai [7];
double ai_Init[7] = { 0,  -7.85951783, 1.84408259, -11.7866497, 22.6807411, -15.9618719, 1.80122502 };  

//%__________________________________________________________________________
//%  Coefficients for calculating saturated vapor density = csat [kg/m^3]  
double dencr = 322.0;
#define e_dencr (double) 322.0

double ci[7];
double ci_Init[7] = { 0,  -2.03150240, -2.68302940, -5.38626492, -17.2991605, -44.7586581, -63.92010630 };   

//%__________________________________________________________________________
//% High temperature default values for saturated vapor properties 
//% To be assigned whenever tempk > tcutof = the cutoff temperature 
/********************************************************
*
*********************************************************/
void  Water_Vapor_Constants_0() 
{
int i; 
 prescr = e_prescr; 
 dencr = e_dencr; 

 for ( i = 0; i < 7; i++ ) { 
    ai[i] = ai_Init[i];
    ci[i] = ci_Init[i]; } 


  Crit_H2O_Vapor_nSTP();
}

/*****************************************************************/
/*  file Crit_H20_Vapor_Constants_nSTP.m */
double  tempcr, tempci, tempcf;
double esatx, ssx, csatx;

extern double pres; 

void Crit_H2O_Vapor_nSTP()
{
  tempcr = 647.096;
  tempci = 1.0 / tempcr;

  tempcf = Backward_Sat(pres);

// [esatx ssx csatx ~] = densatmaxHMV(tempcf);
double WTF;
  densatmaxHMV (tempcf, &esatx, &ssx, &csatx, &WTF); 

}


// %--------------------------------------------------------------------------
// function [ tempsat ] = Backward_Sat( pres )
// %--------------------------------------------------------------------------
double Backward_Sat (double pres )
{
double bet2,bet,E,F,G,D,A, tempsat;

double nib[] = { 0, 
                0.11670521452767e+04, -0.72421316703206e+06, -0.17073846940092e+02, 
                0.12020824702470e+05, -0.32325550322333e+07,  0.14915108613530e+02, 
               -0.48232657361591e+04,  0.40511340542057e+06, -0.23855557567849,
                0.65017534844798e+03};  
  
  bet2 = xsqrt(1e-06*pres); 
  bet  = xsqrt(bet2);    
  E  = bet2 + nib[3] * bet + nib[6];
  F  = nib[1] * bet2 + nib[4] * bet + nib[7]; 
  G  = nib[2] * bet2 + nib[5] * bet + nib[8];
  D  = 2 * G / (-F - xsqrt(F*F-4*E*G));
  A  = nib[10] + D;

  tempsat = 0.5*(A-xsqrt(A*A-4*( nib[9] + nib[10] * D)));


 return tempsat; 
}


