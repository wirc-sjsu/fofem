#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double  tempcr, tempci;

#ifdef wowow
extern double XIM[]; 
extern double denair0;
// 
extern double global;
extern double  zlamm, xw0, ga, gc, q0t; 
extern double h2omol, psi0; 
extern double pres; 
#endif 
extern double zlamm, ga,gc;
extern double pres; 
extern double tempcr, tempci; 
extern double XIM[]; 
extern double q0t,xw0,denair0,ga,gc,psi0,h2omol; 

/**********************************************************/
void calconHMVl( double tcon[], double xa[], double fw[], double hv[], double dhvdT[], double ka[], 
double TempR[], double temR[], double tempk[], double tempki[], 
double diff[], double zlamda[], double zlamw[], double ss[],
double xm[], double poros[], double theta[], double psin[], double nlev )
{
int i; 
double lgl, X, X2, XA, XB;
double tm,xkm,q,xqfac,zlamv,zlama,zlamf; 
double ta,tw,xka,xkw; 

   for ( i = 1; i <= nlev; i++ ) {

     if ( tempk[i] <= tempcr ) {
        X  = 1.0 - TempR[i];
        X2 = X * X;
        XA = xsqrt(xsqrt(xsqrt(X*X2)));
        XB = X2 * xsqrt(xsqrt(X));
        hv[i] =  XIM[1] *(temR[i] - 1) + XIM[2] * XA - XIM[3] * XB;
        dhvdT[i] = -XIM[1] * temR[i] * tempki[i] - (XIM[4] * XA - XIM[5] * XB) * tempci / X; }
     else {
        hv[i] = 0; 
        dhvdT[i] = 0; }

/* Set defatuls in case */ 
     xa[i] = poros[i];
     tm = zlamm / zlamda[i] - 1.0;     
     xkm = xm[i] * (2.0 / (1.0 + tm * ga ) + 1.0 / (1.0+tm*gc))/3.0;
     tcon[i] = (xkm * zlamm +xa[i] * zlamda[i]) / (xkm+xa[i]);
     fw[i] = 0 ;
     ka[i] = 1;

     if ( theta[i]  <= 0.01 ) 
       continue; 
     xa[i] = poros[i] - theta[i];
     q  = q0t * tempk[i] * tempk[i];
     xqfac = xpow (  (theta[i] / xw0),-q );
     fw[i]  = 1.0 / (1 + xqfac);

     zlamv = denair0 * hv[i] * diff[i] * ss[i] * fw[i] / pres;
     zlama = zlamda[i] + zlamv;

     zlamf = zlama + fw[i] * (zlamw[i] - zlama);
     ta = zlama / zlamf - 1.0;
     tw = zlamw[i] / zlamf - 1.0;
     tm = zlamm / zlamf - 1.0; 
     ka[i] = (2.0 / (1.0 + ta * ga) + 1.0 / (1.0 + ta * gc)) / 3.0;
     xka = xa[i] * ka[i];
     xkw = theta[i] * (2.0 / (1.0 + tw * ga) + 1.0 / (1.0 + tw * gc) ) / 3.0;
     xkm = xm[i] * (2.0 / (1.0 + tm * ga) + 1.0 / (1.0 + tm * gc)) / 3.0;
     tcon[i] = (xkm * zlamm + xka * zlama + xkw * zlamw[i]) / (xkm + xka + xkw);
     hv[i] = hv[i] - psi0 * h2omol * psin[i];  
  }  /* for i */ 

}

#ifdef wowoow
%--------------------------------------------------------------------------
function [ tcon xa fw hv dhvdT ka ] = calconHMVl( TempR,temR,tempk,tempki,... 
                      diff,zlamda,zlamw,ss,xm,poros,theta,psin,nlev )
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

%--------------------------------------------------------------------------
%                             Dry Soil
%------------------------------------------------------------------------

xa         = poros;
tm         = zlamm./zlamda - 1;     
xkm        = xm.*(2./(1+tm.*ga)+1./(1+tm*gc))/3;
tcon       = (xkm.*zlamm+xa.*zlamda)./(xkm+xa);
fw(1:nlev) = 0 ;
ka(1:nlev) = 1;

%--------------------------------------------------------------------------
%                             Moist Soil
%--------------------------------------------------------------------------
      
LGL        = theta <= 0.01;

xa(~LGL)   = poros(~LGL)-theta(~LGL);
q          = q0t*tempk(~LGL).*tempk(~LGL);
xqfac      = (theta(~LGL)/xw0).^(-q);
fw(~LGL)   = 1./(1+xqfac);
zlamv      = denair0*hv(~LGL).*diff(~LGL).*ss(~LGL).*fw(~LGL)/pres;
zlama      = zlamda(~LGL) + zlamv;
zlamf      = zlama + fw(~LGL).*(zlamw(~LGL)-zlama);
ta         = zlama./zlamf - 1;
tw         = zlamw(~LGL)./zlamf - 1;
tm(~LGL)   = zlamm./zlamf - 1;    
ka(~LGL)   = (2./(1+ta.*ga)+1./(1+ta.*gc))/3;
xka        = xa(~LGL).*ka(~LGL);
xkw        = theta(~LGL).*(2./(1+tw.*ga)+1./(1+tw.*gc))/3;
xkm(~LGL)  = xm(~LGL).*(2./(1+tm(~LGL).*ga)+1./(1+tm(~LGL).*gc))/3;
tcon(~LGL) = (xkm(~LGL).*zlamm+xka.*zlama+xkw.*zlamw(~LGL))./ ... 
             (xkm(~LGL)+xka+xkw);

% ***************** Reassign Hv for Lv = Lv(psin) = Hv/Mw *****************

hv = hv-psi0*h2omol*psin;
% global h2opsir 
% psic = -4.67e+06;
% hv   = hv - h2omol*psic*(1-exp(h2opsir*tempki.*psin));

% logicl      = psin >= 1; 
% hv(~logicl) = hv(~logicl)-psi0*h2omol*psin(~logicl);

end

#endif 