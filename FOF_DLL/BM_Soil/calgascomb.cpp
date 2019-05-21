#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double pidv, pivd, mvd4;

/*********************************************************/
void calgascomb(double  mois[], double vapor[], double dry[], double mvapor[], 
                double mdry[], double xhiv[], double nlevel )
{
int i; 
double mrat,mrt2,PhiD,PhiV;
   for ( i = 1; i <= nlevel; i++ ) {
     if ( mvapor[i] != 0 ) 
       mrat = mvd4 * xsqrt(mdry[i] / mvapor[i]);
     else 
       mrat = xZero ("mvapor"); 

     mrt2 = mrat * mrat;

     PhiD = pidv * (1.0+2.0 * mrat + mrt2);
     PhiV = pivd * (1.0+2.0 / mrat + 1.0 / mrt2);
     mois[i] = (1.0-xhiv[i])*dry[i]/(1.0-xhiv[i]+xhiv[i]*PhiD)+xhiv[i]*vapor[i]/(xhiv[i]+(1.0-xhiv[i])*PhiV);

   }  /* for */ 

}


#ifdef wowow
%--------------------------------------------------------------------------
function [ mois ] = calgascomb( vapor,dry,mvapor,mdry,xhiv )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Rule for combining dry air and water vapor properties to determine  
%      the thermal conductivity and dynamic viscosity of moist air 

global pidv pivd mvd4

mrat = mvd4*sqrt(mdry./mvapor);
mrt2 = mrat.*mrat;
PhiD = pidv*(1+2*mrat+mrt2);
PhiV = pivd*(1+2./mrat+1./mrt2);

mois = (1-xhiv).*dry./(1-xhiv+xhiv.*PhiD)+xhiv.*vapor./(xhiv+(1-xhiv).*PhiV);

end

#endif 