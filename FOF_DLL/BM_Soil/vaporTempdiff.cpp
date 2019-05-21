#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

void vaporTempdiff  (double *DiffT, double *vdiff, double *tempki, double nlevel )
{
int i; 
   for ( i = 1; i <= nlevel; i++ ) {
      DiffT[i] = vdiff[i] * tempki[i];
   }

}


#ifdef wowow
%--------------------------------------------------------------------------
function [ DiffT ] = vaporTempdiff( vdiff,tempki )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Determine the vertical profile of soil vapor diffusivity  
%           associated with temp gradient ---- (KvT) [kg/m/s/K]

DiffT = vdiff.*tempki;

end
#endif 