
#include "string.h"
#include "math.h"
#include "BMSoil.h"

extern double KInc, EA, MA, thetar;

void  calDENVEL(double *uu1, double *deldifu, double *Lambdau,
                double *Rcoef, double *Sw, double *por, double *theta,
                double *mug, double *vapres, double *rhcpa, double nlevel )
{
int i; 
double f, Sww, x0, x1,x2,x3;

   for ( i = 1; i <= nlevel; i++ ) {
     Sww = Sw[i] - Rcoef[i] * thetar / por[i];
     if  ( Sww > 0 ) 
       Sww = 1.0*Sww;
     else 
       Sww = 0; 

     x0 = 1.0 - Sww;
     x1 = xpow (x0, (1.0/MA));
     x2 = xpow ((1-x1), MA);
     x3 = xpow ((1-x2), EA);
     if ( mug[i] != 0 )       /* Make sure we don't divide by 0 */ 
        uu1[i] = KInc * x3 * abs(vapres[i]) / mug[i];
     else
        uu1[i]; 

     deldifu[i] = (por[i] - theta[i]) *uu1[i];
     Lambdau[i] = rhcpa[i] * deldifu[i];
   }  /* for i */ 

}





#ifdef wowow
%--------------------------------------------------------------------------
function [ uu1 deldifu Lambdau ] = ... 
                       calDENVEL( Rcoef,Sw,por,theta,mug,vapres,rhcpa )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% VanGenuchten-Assouline model for soil gas phase permeability and the 
% parameters associated with density driven advective velocity

global KInc 
global EA MA
global thetar

Sww     = Sw-Rcoef*thetar./por;
LGL     = Sww > 0;
Sww     = LGL.*Sww;

x0      = 1-Sww;
x1      = x0.^(1/MA);
x2      = (1-x1).^MA;
x3      = (1-x2).^EA;

uu1     = KInc*x3.*abs(vapres)./mug;
deldifu = (por-theta).*uu1;
Lambdau = rhcpa.*deldifu;

end

#endif 