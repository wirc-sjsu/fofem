#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double actemp, tempoi, h2opsir;

void  calConCoef5 (double *Concoef, double *dCondT, double *dCondp,
                   double *psin, double *tempki,double nlevel )
{
int i; 
double Cf1; 

  for ( i = 1; i <= nlevel; i++ ) {
    Cf1 = actemp - h2opsir * psin[i];

    Concoef[i] = xexp(Cf1 * (tempki[i] - tempoi) );
    dCondT[i] = -Cf1 * tempki[i] * tempki[i] * Concoef[i];
    dCondp[i] = -h2opsir * (tempki[i] - tempoi) * Concoef[i];
  }

}

#ifdef wowow
%--------------------------------------------------------------------------
function [ Concoef dCondT dCondp ] = calConCoef5( psin,tempki )
%--------------------------------------------------------------------------

%__________________________________________________________________________
%        Massman Original Model of Condensation Coefficient [ Concoef ]
%        Reduces rate of condensation as temperature increases
%        Also compute first derivatives [ dCondT dCondp ]

global actemp tempoi 
global h2opsir 

Cf1     = actemp-h2opsir*psin;

Concoef =  exp(Cf1.*(tempki-tempoi));
dCondT  = -Cf1.*tempki.*tempki.*Concoef;
dCondp  = -h2opsir*(tempki-tempoi).*Concoef;

end

#endif 