
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

void  calcsHMVnT (double denwnT[], double csx[], double csxi[],
                  double cs[], double denw[], double lv[], double thtemp[], double nlevel )
{
int i; 
   for ( i = 1; i <= nlevel; i++ ) {
     denwnT[i] = denw[i] * thtemp[i];
     csx[i] = cs[i] - lv[i] * denwnT[i];
     csxi[i] = 1.0 / csx[i];
   }

}


#ifdef wowow
%--------------------------------------------------------------------------
function [ denwnT csx csxi ] = calcsHMVnT( cs,denw,lv,thtemp )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Determine corrections associated with temperature dependent WRC 
%
% Determine the Vertical Profiles of
%   denw*(d\theta/dTempK) ------------------------ (denwpT) [  K^-1   ]
%   the corrected soil volumetric specific -------  (csx)   [ J/m^3/K ]
%   and its inverse ------------------------------  (csxi)  [  Km^3/J ]

denwnT = denw.*thtemp;
csx    = cs - lv.*denwnT;
csxi   = 1./csx;

end

#endif 