#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"


extern double KInc; 
extern double psi0;
extern double grav; 

extern double HYD0, HYD1, HYD2, HYDP; 

/************************************************************************************
*
*
************************************************************************************/
int  calhydrauWRC (double Kn[], double KH[], double dKHdT[], double dKHdt[], double dKHdp[],
               double psin[], double muw[], double dmTm[], double denw[], double nlev) 
{
int i;
double a,p, d; 
double auxx[eC_MaxLev];
double lpsin[eC_MaxLev];
double aux0[eC_MaxLev];
double aux[eC_MaxLev];
double KRa[eC_MaxLev];

  for ( i = 1; i <= nlev; i++ ) 
    auxx[i] = KInc * denw[i] / muw[i];

  for ( i = 1; i <= nlev; i++ ) 
    lpsin[i] = xlog(psin[i]);

 //   aux0[i]  = HYD2 / (abs(lpsin)).^HYDP;
  for ( i = 1; i <= nlev; i++ ) { 
    a = abs (lpsin[i]);  
    d = xpow ( a, HYDP );  
    p = HYD2 / d; 
    aux0[i] = p;  } 

//  aux   = HYD0 + HYD1*lpsin + aux0;
 for ( i = 1; i <= nlev; i++ )  
   aux[i] = HYD0 + HYD1 * lpsin[i] + aux0[i];

// KRa   = auxx.*exp(aux);
 for ( i = 1; i <= nlev; i++ )  
   KRa[i] = auxx[i] * xexp(aux[i]);


 for ( i = 1; i <= nlev; i++ ) {
   Kn[i]    = KRa[i] * psi0;
   KH[i]    = KRa[i] * grav; } 


 for ( i = 1; i <= nlev; i++ ) { 
   dKHdT[i] = -KH[i] * dmTm[i];
   dKHdt[i] =  0 * dKHdT[i];
// dKHdp =  KH.*((HYD1+HYDP*aux0./abs(lpsin))./psin);  
   dKHdp[i] =  KH[i] *(( HYD1 + HYDP * aux0[i] / abs(lpsin[i])) / psin[i]);
  }



   return 1; 

} 



#ifdef woowo
%--------------------------------------------------------------------------
function [ Kn,KH,dKHdT,dKHdt,dKHdp ] = calhydrauWRC( psin,muw,dmTm,denw )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Approximation for Fredlund-Xing model for soil hydraulic conductivity
% Determine the Vertical Profiles of Kn and KH

global KInc 
global psi0 
global grav
global HYD0 HYD1 HYD2 HYDP 

auxx  = KInc*denw./muw;
lpsin = log(psin);
aux0  = HYD2./(abs(lpsin)).^HYDP;
aux   = HYD0 + HYD1*lpsin + aux0;
KRa   = auxx.*exp(aux);

Kn    = KRa*psi0;
KH    = KRa*grav;

%__________________________________________________________________________
% Determine the Vertical Profiles of dKH/dT and dKH/dpsin

dKHdT = -KH.*dmTm;
dKHdt =  0*dKHdT;
dKHdp =  KH.*((HYD1+HYDP*aux0./abs(lpsin))./psin);

end

#endif 