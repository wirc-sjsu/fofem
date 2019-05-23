#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double  tempci;
extern double  dencr;
extern double  ci[];   /* 1x6 ? ? */ 
extern double  h2opsir;

double *tempR;
double *tR2;
double *tR3;  
double *tR6;  
double *tRt;  
double *tRs; 
double *terma;
double *rhoveq;

extern double tempci; 


/***********************************************************************/
void rhoveqHMV (double *rhoveq, double *tempk, double *psin, double nlevel)
{
int i;
double f; 
  tempR = new double [nlevel+1]; 
  tR2 = new double [nlevel+1]; 
  tR3 = new double [nlevel+1];  
  tR6 = new double [nlevel+1];  
  tRt = new double [nlevel+1];  
  tRs = new double [nlevel+1];  
  terma = new double [nlevel+1]; 

  for ( i = 1; i <= nlevel; i++ ) {
    tempR[i] = 1.0 - tempk[i] * tempci;
    tR2[i] = tempR[i] * tempR[i]; 
    tR3[i] = tempR[i] * tR2[i];
    tR6[i] = tR3[i] * tR3[i];
    tRt[i] = xnthroot (tempR[i],3.0);
    tRs[i] = xsqrt(tRt[i]);
    terma[i]  = (ci[1] + ci[2] * tRt[i] + ci[3] * tempR[i]) * 
                tRt[i] + (ci[4] + ci[5] * tRs[i] * tR3[i]) * tR3[i]; 
    rhoveq[i] = dencr * xexp (h2opsir * psin[i] / tempk[i] + 
                terma[i] + ci[6] * tR6[i] * tR6[i] / tRs[i]);
 }


 delete  tempR;
 delete tR2;
 delete tR3;
 delete tR6;
 delete tRt;
  delete tRs;
  delete terma;
}



#ifdef wow
%--------------------------------------------------------------------------
function [ rhoveq ] = rhoveqHMV( tempk,psin )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Calculate the initial equilibrium vapor density rhoveq [kg/m^3]
% Use Wagner and Pruess (2002) for csat and the Kelvin Equation for rh

global tempci
global dencr ci
global h2opsir

tempR  = 1-tempk*tempci;
tR2    = tempR.*tempR;
tR3    = tempR.*tR2;
tR6    = tR3.*tR3;
tRt    = nthroot(tempR,3);
tRs    = sqrt(tRt);
terma  = (ci(1)+ci(2)*tRt+ci(3)*tempR).*tRt+(ci(4)+ci(5)*tRs.*tR3).*tR3; 

rhoveq = dencr*exp(h2opsir.*psin./tempk+terma+ci(6)*tR6.*tR6./tRs);

end
#endif 
