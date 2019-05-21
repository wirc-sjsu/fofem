#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double  KInc, psi0, grav, EA, MA, thetar;
extern double jstep; 
extern char gcr_FuncName[];

/******************************************************************************/
void calhydrauVA(double *Kn, double *KH, double *dKHdT, double *dKHdt, double *dKHdp,
                 double *Rcoef, double *muw, double *dmTm, double *denw, double *Sw,
                 double *poros, double *thpsin, double nlevel )
{
int i, N; 
double Sww, x1, x2, x3, x4, t1,KRa, LGL,auxx; 
char cr[1000]; 

   strcpy (gcr_FuncName,"calhydrauVA"); 
   N = 0; 

   for ( i = 1; i <= nlevel; i++ ) {
     Sww   = Sw[i] - Rcoef[i] * thetar / poros[i];
     if ( Sww > 0 ) 
       LGL = 1; 
     else {
       LGL = 0;
       N++; } 
     Sww   = LGL * Sww;

     x1 = xpow (Sww, (1.0/MA));
     x2 = xpow ((1.0 - x1), MA);
     t1 = xpow ((1.0- x2), EA);
     auxx  = KInc * denw[i] / muw[i];
     KRa   = auxx * t1;

     Kn[i] = KRa * psi0;
     KH[i] = KRa * grav;


     dKHdT[i] = -KH[i] * dmTm[i];
     x3 =  xpow (Sww, (1.0 / MA - 1.0));
     x4 =  xpow ( (1.0 - x2), (EA-1.0));
     dKHdt[i] =  EA * grav * auxx * x2 * x3 * x4 / ((1.0-x1) * poros[i]);
     dKHdp[i] = thpsin[i] * dKHdt[i];
   } 


  strcpy (gcr_FuncName,"calhydrauVA"); 
}



#ifdef wowow
%--------------------------------------------------------------------------
function [ Kn KH dKHdT dKHdt dKHdp ] = ... 
           calhydrauVA( Rcoef,muw,dmTm,denw,Sw,poros,thpsin )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% VanGenuchten-Assouline model for soil hydraulic conductivity
% Determine the Vertical Profiles of Kn and KH

global KInc 
global psi0 
global grav
global EA MA
global thetar

Sww   = Sw-Rcoef*thetar./poros;
LGL   = Sww > 0;
Sww   = LGL.*Sww;

x1    = Sww.^(1/MA);
x2    = (1-x1).^MA;
t1    = (1-x2).^EA;
auxx  = KInc*denw./muw;
KRa   = auxx.*t1;

Kn    = KRa*psi0;
KH    = KRa*grav;

%__________________________________________________________________________
% Determine the Vertical Profiles of dKH/dT and dKH/dpsin

dKHdT = -KH.*dmTm;
x3    =  Sww.^(1/MA-1);
x4    =  (1-x2).^(EA-1);
dKHdt =  EA*grav*auxx.*x2.*x3.*x4./((1-x1).*poros);
dKHdp =  thpsin.*dKHdt;

end

#endif 