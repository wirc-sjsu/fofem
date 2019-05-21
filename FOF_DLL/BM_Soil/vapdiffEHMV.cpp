
#include "string.h"
#include "math.h"
#include "BMSoil.h"

extern double  h2omol;

/*************************************************************************/
void vapdiffEHMV (double *enh, double *vdiff, double *lv, 
                  double *diffs, double *xa, double *poros, 
                  double *fw,  double *ka, double *hv, double nlevel )
{
int i; 
double xatau;

   for ( i = 1; i <= nlevel; i++ ) {
     xatau = 0.66 * xpow(xa[i],4.0) / xpow (poros[i],3.0);
     enh[i] = (1.0 + 2.0 * fw[i]) * ka[i];
     vdiff[i] = enh[i] * xatau * diffs[i];
     lv[i] = hv[i] / h2omol;
   } /* for i */

}



#ifdef wowow
%--------------------------------------------------------------------------
function [ enh vdiff lv  ] = vapdiffEHMV( diffs,xa,poros,fw,ka,hv )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Determine the Vertical Profile of 
%     ENHANCED soil vapor diffusivity ---- (vdiff) [m^2/s]
%     heat of vaporization   -------------   (lv)  [J/kg] 
% Note Enhancement factor    -------------   (enh) [  -  ]
%
%     DuPlessis model of xatau = (eta-theta)*tau  = 1-(1-xa).^(2/3);
% Moldrup et al model of xatau = (eta-theta)*tau  = 0.66*xa^4/eta^3;


xatau = 0.66*(xa.^4)./poros.^3;
enh   = (1+2*fw).*ka;
vdiff = enh.*xatau.*diffs;

global h2omol

lv = hv/h2omol;

end
#endif 