#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double prescr, ai[];
extern double tempcr, tempci, tempcf;

extern double presr;
extern double  dencr,  ci[]; 

extern double  esatx, ssx, csatx; 

extern double  esatx, ssx, csatx;
 
/*******************************************************************/
void   densatHMV(double esat[], double ss[], double csat[], double dcsatdT[],
                 double tempk[],  double tempki[], double nlevel )
{
int i;
double tempR,tR2,tR3,tRq,tR6,tRt,tRs;
double  term1, term2,lnprt;
double  term3, term4,terma,termb,termc,termd; 

   for ( i = 1; i <= nlevel; i++ ) {

     tempR = 1.0 - tempk[i] * tempci;
     tR2   =  tempR * tempR;
     tR3   = tempR * tR2;
     tRq   = xsqrt(tempR);
     term1 = (ai[1]+ai[2]*tRq) * tempR + (ai[3]+ai[4]*tRq)*tR3;
     term2 = (ai[5]+ai[6] * tRq * tR3) * tR3 * tempR;
     lnprt = tempcr * (term1+term2) * tempki[i];
     term3 = lnprt+(ai[1]+1.5*ai[2]*tRq)+(3.0*ai[3]+3.5*ai[4]*tRq)*tR2;
     term4 = (4.0*ai[5]+7.5*ai[6]*tRq*tR3)*tR3;

     esat[i] =  prescr * xexp(lnprt);
     ss[i]   = -esat[i] * (term3+term4) * tempki[i];
 

     tR6   = tR3*tR3;
     tRt   = xnthroot(tempR,3.0);
     tRs   = xsqrt(tRt);
     terma = (ci[1]+ci[2]*tRt+ci[3]*tempR) * tRt+(ci[4]+ci[5]*tRs*tR3)*tR3; 
     termb = (ci[1]+2.0*ci[2]*tRt+4.0*ci[3]*tempR)*tRt / (3.0 * tempR);
     termc = (18.0*ci[4]+37.0*ci[5]*tR3*tRs)*tR2 / 6.0;
     termd = 71.0*ci[6]*tR6*tR3*tR2 / (6.0*tRs);

     csat[i]    =  presr * dencr * xexp (terma+ci[6]*tR6*tR6 / tRs);
     dcsatdT[i] = -csat[i] * (termb+termc+termd) * tempci;

     if  ( tempk[i] <= tempcf ) 
       continue; 
 
     esat[i]    =  esatx;
     csat[i]    =  csatx * tempcf * tempki[i];
     ss[i]      =  ssx;
     dcsatdT[i] = -csat[i] * tempki[i];


   }  /* for i */


}



#ifdef  sfdsdfsd
%--------------------------------------------------------------------------
function [ esat ss csat dcsatdT ] = densatHMV( tempk,tempki )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Determine Vertical Profiles of soil 
%    saturation vapor pressure ---------------------- (esat)    [Pa] 
%    slope of the saturation vapor pressure curve --- (ss)      [Pa/K] 
%    saturation vapor density ----------------------- (csat)    [kg m^-3] 
%    first derivative wrt temperature of csat ------- (dcsatdT) [kg m^-3/K]

%__________________________________________________________________________
% Use Wagner and Pruess (2002) for esat and desatdT (ss)

global prescr ai
global tempcr tempci tempcf

tempR = 1-tempk*tempci;
tR2   = tempR.*tempR;
tR3   = tempR.*tR2;
tRq   = sqrt(tempR);
term1 = (ai(1)+ai(2)*tRq).*tempR + (ai(3)+ai(4)*tRq).*tR3;
term2 = (ai(5)+ai(6)*tRq.*tR3).*tR3.*tempR;
lnprt = tempcr*(term1+term2).*tempki;
term3 = lnprt+(ai(1)+1.5*ai(2)*tRq)+(3*ai(3)+3.5*ai(4)*tRq).*tR2;
term4 = (4*ai(5)+7.5*ai(6)*tRq.*tR3).*tR3;

esat  =  prescr*exp(lnprt);
ss    = -esat.*(term3+term4).*tempki;

%__________________________________________________________________________
% Use Wagner and Pruess (2002) for csat and dsatdT

global presr
global dencr ci

tR6   = tR3.*tR3;
tRt   = nthroot(tempR,3);
tRs   = sqrt(tRt);
terma = (ci(1)+ci(2)*tRt+ci(3)*tempR).*tRt+(ci(4)+ci(5)*tRs.*tR3).*tR3; 
termb = (ci(1)+2*ci(2)*tRt+4*ci(3)*tempR).*tRt./(3*tempR);
termc = (18*ci(4)+37*ci(5)*tR3.*tRs).*tR2/6;
termd = 71*ci(6)*tR6.*tR3.*tR2./(6*tRs);

csat    =  presr*dencr*exp(terma+ci(6)*tR6.*tR6./tRs);
dcsatdT = -csat.*(termb+termc+termd)*tempci;

%__________________________________________________________________________
% Calculate values when tempk > cutoff temperature = tempcf

LGL = tempk <= tempcf;

%__________________________________________________________________________
% Assign default values for these saturated vapor properties

global esatx ssx csatx 

esat(~LGL)    =  esatx;
csat(~LGL)    =  csatx*tempcf*tempki(~LGL);
ss(~LGL)      =  ssx;
dcsatdT(~LGL) = -csat(~LGL).*tempki(~LGL);

% +++ LGL2 = tempk <= tempcf-40;
% +++ ss(~LGL2) = ssx;

end

#endif 