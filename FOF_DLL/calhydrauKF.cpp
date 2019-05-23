#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double  tempcr;
extern double  Rwfac, Fwfac;
extern double  KInc; 
extern double psi0; 
extern double grav;



/******************************************************************************/
void calhydrauKF(
                double *Knf, double *KHf, double *dKnfdT, double *dKnfdp, double *dKHfdT, double *dKHfdp,
                double *por, double *tempk, double *tempki, double *psin,
                double *denw, double *muw, double *dmTm, double *sdialc, double *ddiaT, 
                double *stensn, double *dstensdT, double nlev )
{
int i;

  for ( i = 1; i <= nlev; i++ ) {
    Knf[i]    = 0;
    KHf[i]    = 0;
    dKnfdT[i] = 0;
    dKHfdT[i] = 0;
    dKnfdp[i] = 0;
    dKHfdp[i] = 0; }

double Rw1, KF0, KF1, T01, T02; 
double KRf,KFA,KFB,dKRdT,dKRdp,  KRa;

  for ( i = 1; i <= nlev; i++ ) {
    if ( tempk[i] >= tempcr ) 
      continue; 

    Rw1 = xsqrt(sdialc[i]) * tempk[i];

    KF0 = 2.0 * stensn[i] - Fwfac * denw[i] * psin[i];
    KF1 = xsqrt(KF0);
    T01 = Rw1 / KF1;
    T02 = T01 * T01;

    KRf = Rwfac * T01 * T02 * por[i];
    KFA = 3.0 * KRf / (2.0 * KF0);
    KFB = 0.5 * ddiaT[i] / sdialc[i] + tempki[i];
    dKRdT = 3.0 * KRf * KFB - 2.0 * dstensdT[i]  * KFA;
    dKRdp = Fwfac * denw[i] * KFA;

    KRa = KInc * denw[i] / muw[i];
    Knf[i] = KRa * KRf * psi0;
    KHf[i] = KRa * KRf * grav;

    dKnfdT[i] = -Knf[i] * dmTm[i] + KRa * dKRdT * psi0;
    dKHfdT[i] = -KHf[i] * dmTm[i] + KRa * dKRdT * grav;

    dKnfdp[i] = KRa * dKRdp * psi0;
    dKHfdp[i] = KRa * dKRdp * grav;
 } /* For i */ 

}




#ifdef wiwiww

%--------------------------------------------------------------------------
function [ Knf KHf dKnfdT dKnfdp dKHfdT dKHfdp ] = ... 
           calhydrauKF( por,tempk,tempki,psin,denw,muw,dmTm,sdialc,ddiaT,... 
                        stensn,dstensdT,nlev )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Zhang (2011) model for relative hydraulic conductivity for film flow

global tempcr
global Rwfac Fwfac
global KInc 
global psi0 
global grav

n           = 1:nlev;
Knf(n)      = 0;
KHf(n)      = 0;
dKnfdT(n)   = 0;
dKHfdT(n)   = 0;
dKnfdp(n)   = 0;
dKHfdp(n)   = 0;

%__________________________________________________________________________
% Calculate values when tempk < tempcr [K]

lgl          = tempk >= tempcr;

Rw1          = sqrt(sdialc(~lgl)).*tempk(~lgl);
KF0          = 2*stensn(~lgl) - Fwfac*denw(~lgl).*psin(~lgl);
KF1          = sqrt(KF0);
T01          = Rw1./KF1;
T02          = T01.*T01;

KRf          = Rwfac*T01.*T02.*por(~lgl);
KFA          = 3*KRf./(2*KF0);
KFB          = 0.5*ddiaT(~lgl)./sdialc(~lgl) + tempki(~lgl);
dKRdT        = 3*KRf.*KFB - 2*dstensdT(~lgl).*KFA;
dKRdp        = Fwfac*denw(~lgl).*KFA;

KRa          = KInc*denw(~lgl)./muw(~lgl);
Knf(~lgl)    = KRa.*KRf*psi0;
KHf(~lgl)    = KRa.*KRf*grav;

dKnfdT(~lgl) = -Knf(~lgl).*dmTm(~lgl) + KRa.*dKRdT*psi0;
dKHfdT(~lgl) = -KHf(~lgl).*dmTm(~lgl) + KRa.*dKRdT*grav;

dKnfdp(~lgl) = KRa.*dKRdp*psi0;
dKHfdp(~lgl) = KRa.*dKRdp*grav;

end

#endif 