#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double  h2opsir; 


void calrhev(double *rh, double *rhove, double *drevdp, double *drevdT,
             double *tempki, double *csat, double *dcsatT, double *psin, double nlevel)
{
int i; 

  for ( i = 1; i <= nlevel; i++ ) {
// if ( i == 19 ) 
 //  printf ("%f %f,  ", psin[i], psin[i+1] );
    rh[i] = xexp ( h2opsir * tempki[i] * psin[i] );
    rhove[i]  = csat[i] * rh[i];
    drevdp[i] = h2opsir * tempki[i] * rhove[i];
    drevdT[i] = -psin[i] * tempki[i] * drevdp[i] + rh[i] * dcsatT[i];
  }
}

#ifdef wowo
%--------------------------------------------------------------------------
function [ rh rhove drevdp drevdT ] = calrhev( tempki,csat,dcsatT,psin )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Calculate profile of relative humidity from the Kelvin Eqn -- (rh)
%  and the equilibrium vapor density -------------------------- (rhove)  
% Calculate the first derivatives of rhove wrt to psin -------- (drevdp)
%  and temperature -------------------------------------------- (drevdT)

global h2opsir 

rh     = exp(h2opsir*tempki.*psin);
rhove  = csat.*rh;

drevdp =  h2opsir*tempki.*rhove;
drevdT = -psin.*tempki.*drevdp + rh.*dcsatT;

end

#endif 