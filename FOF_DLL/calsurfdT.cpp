#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double thetam0, dsurf0;

/******************************************************************
*
* Note: i flipped the condition from the Matlab to make it work
*       with an "if" instead of the logical way Matlab does it
******************************************************************/
void  calsurfdT (double *Kd, double *Km, 
                 double *tempr, double *theta, double *Sw,
                 double *por, double *thpsin, double *thtemp, double nlevel )
{
int i; 
double aux,exp1,surfdif; 

  for ( i = 1; i <= nlevel; i++ ) {
    aux = por[i] / thetam0;
    exp1 = xsqrt(xsqrt(Sw[i] * aux));
    if (  theta[i] < thetam0 )    // See Note above - orig theta[i] >= thetam0 ) 
       exp1 = Sw[i] * por[i];
    surfdif = dsurf0 * xexp(-2.0 * exp1 * tempr[i]);
    Kd[i] = surfdif * thpsin[i];
    Km[i] = surfdif * thtemp[i]; }


}
#ifdef wowow
%--------------------------------------------------------------------------
function [ Kd Km ] = calsurfdT( tempr,theta,Sw,por,thpsin,thtemp )
%--------------------------------------------------------------------------
% 
%__________________________________________________________________________
% Determine the surface diffusivity of liquid water, Kd ---  [m^2/s]
% Determine the surface diffusivity of liquid water, Km --- [m^2/s/K]

global thetam0 dsurf0

aux        = por/thetam0;

exp1       = sqrt(sqrt(Sw.*aux));
lgl        = theta >= thetam0;
exp1(~lgl) = Sw(~lgl).*por(~lgl);
surfdif    = dsurf0*exp(-2*exp1.*tempr);

Kd         = surfdif.*thpsin;
Km         = surfdif.*thtemp;

end
#endif 