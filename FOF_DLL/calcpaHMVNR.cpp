#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double  rgas;
extern double  pres;
extern double  drymol;
extern double vil[], dil[];



/*********************************************************************/
void calcpaHMVNR (double *rhcpv, double *rhcpa, double *drhcaT, double *drhcar, 
                  double *tempki, double *tempr, double *rhov, double nlevel )
{
int i; 
double sqtrm, sqqtr, sqtri, sqqti;


double dcpvT, drcvT,rhodry,cpd, dcpdT, rhcpd, drhyT;

   for ( i = 1; i <= nlevel; i++ ) {

     sqtrm = xsqrt(tempr[i]);
     sqqtr = xsqrt(sqtrm);
     sqtri = 1.0 / sqtrm;
     sqqti = 1.0 / sqqtr;

     calcp0HMV(&drhcar[i], &dcpvT, vil, sqtrm,sqqtr,sqtri,sqqti,tempr[i]);

     rhcpv[i] = rhov[i] * drhcar[i];
     drcvT = rhov[i] * dcpvT;



     rhodry = drymol * pres * tempki[i] / rgas;
     calcp0HMV(&cpd, &dcpdT, dil,sqtrm,sqqtr,sqtri,sqqti,tempr[i]);
     rhcpd =  rhodry * cpd;
     drhyT = -rhcpd * tempki[i] + rhodry * dcpdT;


     rhcpa[i]  = rhcpv[i] + rhcpd;
     drhcaT[i] = drcvT + drhyT;

   }


} 

#ifdef wowo
%--------------------------------------------------------------------------
function [ rhcpv rhcpa drhcaT drhcar ] = calcpaHMVNR( tempki,tempr,rhov )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Determine specific heat of air (cpa) and water vapor (drcar) -- [J/kg/K]
%   and the volumetric specific heat (rhcpa = rho_a*cpa) -------- [J/m^3/K]
%
% Also find the first derivatives wrt temp (drhcaT) and rhov (drhcar) 
% NOTE (a) that drhcar is identical the specific heat of water vapor (cpv) 
% NOTE (b) that drcar is also identical to drhcvr = the first derivative of 
%          rhov*cpv wrt rhov       

global rgas
global pres
global drymol
global vil dil

sqtrm = xsqrt(tempr);
sqqtr = sqrt(sqtrm);
sqtri = 1./sqtrm;
sqqti = 1./sqqtr;

%--------------------------------------------------------------------------
%                             Water Vapor
%--------------------------------------------------------------------------

[drhcar dcpvT] = calcp0HMV(vil,sqtrm,sqqtr,sqtri,sqqti,tempr);
rhcpv          = rhov.*drhcar;
drcvT          = rhov.*dcpvT;

%--------------------------------------------------------------------------
%                               Dry Air
%--------------------------------------------------------------------------

rhodry      = drymol*pres*tempki/rgas;

[cpd dcpdT] =  calcp0HMV(dil,sqtrm,sqqtr,sqtri,sqqti,tempr);
rhcpd       =  rhodry.*cpd;
drhyT       = -rhcpd.*tempki + rhodry.*dcpdT;

%--------------------------------------------------------------------------
%                           Moist Air = Sum 
%--------------------------------------------------------------------------

rhcpa  = rhcpv + rhcpd;

drhcaT = drcvT + drhyT;
     
end
#endif 