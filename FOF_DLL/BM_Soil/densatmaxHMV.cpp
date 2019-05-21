/************************************************************************
*
*  file:    densatmaxHMV.m
*
*************************************************************************/
#include <stdio.h>
#include <string.h>
#include <math.h> 

#include "BMSoil.h"

//%--------------------------------------------------------------------------
// function [ esat ss csat dcsatdT ] = densatmaxHMV( tempk )
// %--------------------------------------------------------------------------
//%__________________________________________________________________________
//% Determine Vertical Profiles of soil 
//%    saturation vapor pressure ---------------------- (esat)    [Pa] 
//%    slope of the saturation vapor pressure curve --- (ss)      [Pa/K] 
//%    saturation vapor density ----------------------- (csat)    [kg m^-3] 
//%    first derivative wrt temperature of csat ------- (dcsatdT) [kg m^-3/K]
//%__________________________________________________________________________
//% Use Wagner and Pruess (2002) for esat and desatdT (ss)

extern double prescr, ai[];
extern double tempcr, tempci;  

extern double presr;
extern double dencr, ci[];

/**************************************************************************
* Name: 
*
***************************************************************************/
void  densatmaxHMV( double tempk, double *esat, double *ss, double *csat, double  *dcsatdT)
{
double tempR, tR2, tR3, tRq, term1, term2, lnprt, term3, term4; 

  tempR = 1.0 - tempk * tempci;
  tR2   = tempR*tempR;
  tR3   = tempR*tR2;
  tRq   = xsqrt(tempR);
  term1 = (ai[1] + ai[2] * tRq) * tempR + (ai[3] + ai[4] * tRq) * tR3;
  term2 = (ai[5]+ai[6]*tRq*tR3)*tR3*tempR;
  lnprt = tempcr*(term1+term2)/tempk;
  term3 = lnprt+(ai[1]+1.5*ai[2]*tRq)+(3.0*ai[3]+3.5*ai[4]*tRq)*tR2;
  term4 = (4.0*ai[5]+7.5*ai[6]*tRq*tR3)*tR3;

double Xesat; // I added this to help clearify syntax 
  *esat =  Xesat = prescr * xexp(lnprt);
  *ss   = -Xesat * (term3+term4) / tempk;

//%__________________________________________________________________________
//% Use Wagner and Pruess (2002) for csat and dsatdT

double tR6, tRt, tRs, terma, termb, termc, termd;

  tR6   = tR3 * tR3;
  tRt   = xnthroot(tempR,3);
  tRs   = xsqrt(tRt);
  terma = (ci[1] + ci[2] * tRt + ci[3] * tempR) * tRt + (ci[4] + ci[5] * tRs * tR3) * tR3; 
  termb = (ci[1]+2.0*ci[2]*tRt+4.0*ci[3]*tempR)*tRt/(3.0*tempR);

 // termc = (18*ci[4]+37*ci[5]*tR3.*tRs)*tR2/6;  NOTE the period "tR3." ? ?
  termc = (18.0*ci[4]+37.0*ci[5]*tR3*tRs)*tR2/6.0;
  termd = 71.0*ci[6]*tR6*tR3*tR2/(6.0*tRs);

double Xcsat;
  *csat = Xcsat =  presr * dencr * xexp(terma+ci[6]*tR6*tR6/tRs);
  *dcsatdT = -Xcsat*(termb+termc+termd)*tempci;
}