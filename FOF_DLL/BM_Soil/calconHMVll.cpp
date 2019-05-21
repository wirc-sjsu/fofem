/********************************************************************************
* Name: calconHMV11.cpp
* Desc: This is BM's updated version of calconHMV1.cpp
*       BM gave me this approx Aug 24 2017, said it replaces
*       calconHMV1() 
*********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double  tempcr, tempci;
extern double zlamm, ga,gc;
extern double pres; 
extern double tempcr, tempci; 
extern double XIM[]; 
extern double q0t,xw0,denair0,ga,gc,psi0,h2omol; 


/***********************************************************************************

* Note-1: _XA was XA but for some reason XA wouldn't display in the debugger
*          correctly
*
************************************************************************************/

void calconHMVll  (
   double tcon[], double xa[], double fw[], double hv[], double dhvdT[], double ka[], double zlamv[],
   double TempR[], double temR[], double tempk[], double tempki[], double temp[], 
   double diff[], double zlamda[], double zlamw[], double ss[],
   double xm[], double poros[], double theta[], double psin[], double nlev )
{
int i; 
double   X[eC_MaxLev], X2[eC_MaxLev];
double  _XA[eC_MaxLev], XB[eC_MaxLev];    /* See Note-1 */ 
double  xkm[eC_MaxLev] ,xqfac[eC_MaxLev], zlama[eC_MaxLev], zlamf[eC_MaxLev]; 
double  ta[eC_MaxLev], tw[eC_MaxLev], xka[eC_MaxLev], xkw[eC_MaxLev]; 

double zlamT[eC_MaxLev], tm[eC_MaxLev],q[eC_MaxLev];


   for ( i = 1; i <= nlev; i++ ) {

     if ( tempk[i] <= tempcr ) {
        X[i]  = 1.0 - TempR[i];
        X2[i] = X[i] * X[i];
       _XA[i] = xsqrt(xsqrt(xsqrt(X[i]*X2[i])));
        XB[i] = X2[i] * xsqrt(xsqrt(X[i]));
        hv[i] =  XIM[1] *(temR[i] - 1) + XIM[2] * _XA[i] - XIM[3] * XB[i];
        dhvdT[i] = -XIM[1] * temR[i] * tempki[i] - (XIM[4] * _XA[i] - XIM[5] * XB[i]) * tempci / X[i];
        zlamT[i] = zlamm * (8.0 * exp (-0.008 * (temp[i]-27) ) + 3.0) / 11.0 ; }
     else {
        hv[i] = 0; 
        dhvdT[i] = 0; }

/* Set defatuls in case */ 
     xa[i] = poros[i];
     tm[i] = zlamT[i] /  zlamda[i] - 1.0;     
     xkm[i] = xm[i] * (2.0 / (1.0 + tm[i] * ga ) + 1.0 / (1.0 + tm[i] * gc))/3.0;
     tcon[i] = (xkm[i] * zlamT[i] + xa[i] * zlamda[i]) / (xkm[i] + xa[i]);
     fw[i] = 0 ;
     ka[i] = 1;
     zlamv[i] = 0; 
    
     if ( theta[i]  <= 0.01 ) 
       continue; 
     xa[i] = poros[i] - theta[i];
     q[i] = q0t * tempk[i] * tempk[i];
     xqfac[i] = xpow (  (theta[i] / xw0),-q[i] );
     fw[i]  = 1.0 / (1 + xqfac[i]);
     zlamv[i] = denair0 * hv[i] * diff[i] * ss[i] * fw[i] / pres;
     zlama[i] = zlamda[i];
     zlamf[i] = zlama[i] + fw[i] * (zlamw[i] - zlama[i]);
     ta[i] = zlama[i] / zlamf[i] - 1.0;
     tw[i] = zlamw[i] / zlamf[i] - 1.0;
     tm[i] = zlamT[i] / zlamf[i] - 1.0; 
     ka[i] = (2.0 / (1.0 + ta[i] * ga) + 1.0 / (1.0 + ta[i] * gc)) / 3.0;
     xka[i] = xa[i] * ka[i];

     xkw[i] = theta[i] * (2.0 / (1.0 + tw[i] * ga) + 1.0 / (1.0 + tw[i] * gc) ) / 3.0;
     xkm[i] = xm[i] * (2.0 / (1.0 + tm[i] * ga) + 1.0 / (1.0 + tm[i] * gc)) / 3.0;
     tcon[i] = (xkm[i] * zlamT[i] + xka[i] * zlama[i] + xkw[i] * zlamw[i]) / (xkm[i] + xka[i] + xkw[i]);
     hv[i] = hv[i] - psi0 * h2omol * psin[i]; 


#ifdef wowowo
  
 
 
#endif 
  }  /* for i */ 

}

#ifdef wowoow
%--------------------------------------------------------------------------
function [ tcon xa fw hv dhvdT ka zlamv ] = calconHMV11( TempR,temR,tempk,... 
                tempki,temp,diff,zlamda,zlamw,ss,xm,poros,theta,psin,nlev )
%--------------------------------------------------------------------------

% NOTE - This is Campbell's model for soil thermal conductivity

%__________________________________________________________________________
% Determine the Vertical Profile of 
%      soil thermal conductivity ------------ (tcon) [W/m/K]
%      air filled porosity ------------------ (xa)   [ - ]
%      empircal weighting factor ------------ (fw)   [ - ]
%      latent heat of vaporization ---------- (hv)   [J/mol]
%      weighting factor for deVries model 
%            of soil thermal conductivity --- (ka)   [ - ]    

global tempcr tempci
global XIM
global denair0
global zlamm xw0 ga gc q0t
global h2omol psi0 
global pres 

lgl   = tempk <= tempcr;
X     = 1-TempR;
X2    = lgl.*X.*X;
XA    = sqrt(sqrt(sqrt(X.*X2)));
XB    = X2.*sqrt(sqrt(X));

hv    =  XIM(1)*lgl.*(temR - 1) + XIM(2)*XA - XIM(3)*XB;
dhvdT = -XIM(1)*lgl.*temR.*tempki - (XIM(4)*XA - XIM(5)*XB)*tempci./X;

zlamT = zlamm*(8*exp(-0.008*(temp-27))+3)/11;

%--------------------------------------------------------------------------
%                             Dry Soil
%------------------------------------------------------------------------

xa            = poros;
tm            = zlamT./zlamda - 1;     
xkm           = xm.*(2./(1+tm.*ga)+1./(1+tm*gc))/3;
tcon          = (xkm.*zlamT+xa.*zlamda)./(xkm+xa);
fw(1:nlev)    = 0;
ka(1:nlev)    = 1;
zlamv(1:nlev) = 0;

%--------------------------------------------------------------------------
%                             Moist Soil
%--------------------------------------------------------------------------
      
LGL         = theta <= 0.01;

xa(~LGL)    = poros(~LGL)-theta(~LGL);
q           = q0t*tempk(~LGL).*tempk(~LGL);
xqfac       = (theta(~LGL)/xw0).^(-q);
fw(~LGL)    = 1./(1+xqfac);
zlamv(~LGL) = denair0*hv(~LGL).*diff(~LGL).*ss(~LGL).*fw(~LGL)/pres;
%--------------------------------------------------------------------------
%REMOVE zlamv wjm0417: zlama = zlamda(~LGL) + zlamv;
%--------------------------------------------------------------------------
zlama       = zlamda(~LGL);
zlamf       = zlama + fw(~LGL).*(zlamw(~LGL)-zlama);
ta          = zlama./zlamf - 1;
tw          = zlamw(~LGL)./zlamf - 1;
tm(~LGL)    = zlamT(~LGL)./zlamf - 1;    
ka(~LGL)    = (2./(1+ta.*ga)+1./(1+ta.*gc))/3;
xka         = xa(~LGL).*ka(~LGL);
xkw         = theta(~LGL).*(2./(1+tw.*ga)+1./(1+tw.*gc))/3;
xkm(~LGL)   = xm(~LGL).*(2./(1+tm(~LGL).*ga)+1./(1+tm(~LGL).*gc))/3;
tcon(~LGL)  = (xkm(~LGL).*zlamT(~LGL)+xka.*zlama+xkw.*zlamw(~LGL))./ ... 
              (xkm(~LGL)+xka+xkw);

% ***************** Reassign Hv for Lv = Lv(psin) = Hv/Mw *****************

hv = hv-psi0*h2omol*psin;

end

#endif 