#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double diff0, diffH0, presi;

/*****************************************************************************************/
void  caldiffHMVNRa( double diffa[], double diffH[], double diffs[], double difsdT[], double difsdr[], 
                     double  xhiv[], double dxhivdT[], double dxhivdr[], double tempki[], double tempr[], 
                     double stef[], double stefpT[], double stefpr[], double nlevel)                      
{
int i; 
double aux00, auxdif, aux11, diff, aux22;
double dT1, difaT, difHT, dT2;

      for ( i = 1; i <= nlevel; i++ ) {
        aux00  = presi / (tempr[i] * tempr[i]);
        auxdif = xsqrt(xsqrt(tempr[i]));
        diffa[i]  = diff0 * auxdif * aux00;
        diffH[i]  = diffH0 * aux00 / auxdif;
        aux11  = 1.0 / (xhiv[i] * diffa[i] + (1.0 - xhiv[i]) * diffH[i]);
        diff   = diffa[i] * diffH[i] * aux11;
        diffs[i]  = stef[i] * diff;

        dT1 = 4.0 * tempki[i] * diffs[i] + stefpT[i] * diff;
        difaT  = (1.75 * tempki[i] + stefpT[i]) * diffa[i];
        difHT  = (2.25 * tempki[i] + stefpT[i]) * diffH[i];
        dT2    = (xhiv[i] * difaT + (1.0 - xhiv[i]) * difHT) * aux11 * diffs[i]; 

        aux22  = diffs[i] * aux11 * (diffa[i] - diffH[i]);

        difsdT[i] = dT1 - dT2 - aux22 * dxhivdT[i];
        difsdr[i] = stefpr[i] * diff - aux22 * dxhivdr[i];

   } /* for i */ 
}


#ifdef wowow
%--------------------------------------------------------------------------
function [ diffa diffH diffs difsdT difsdr ] = ... 
         caldiffHMVNRa( xhiv,dxhivdT,dxhivdr,tempki,tempr,stef,stefpT,stefpr )                      
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Determine the diffusivity of water vapor in air ---- (diff)  [m^2/s]                       
%  Also pass tempr to the soil thermal conductivity subroutine
% Calculate the vapor diffusivity as modified by the Stefan factor and 
%  its first derivatives wrt temperature (difsdT) and rhov (difsdr)

global diff0 diffH0
global presi

aux00  = presi./(tempr.*tempr);
auxdif = sqrt(sqrt(tempr));
diffa  = diff0*auxdif.*aux00;
diffH  = diffH0*aux00./auxdif;
aux11  = 1./(xhiv.*diffa+(1-xhiv).*diffH);
diff   = diffa.*diffH.*aux11;
diffs  = stef.*diff;

dT1    = 4*tempki.*diffs + stefpT.*diff;
difaT  = (1.75*tempki + stefpT).*diffa;
difHT  = (2.25*tempki + stefpT).*diffH;
dT2    = (xhiv.*difaT + (1-xhiv).*difHT).*aux11.*diffs; 

aux22  = diffs.*aux11.*(diffa-diffH);

difsdT = dT1 - dT2 - aux22.*dxhivdT;
difsdr = stefpr.*diff - aux22.*dxhivdr;

end
#endif 