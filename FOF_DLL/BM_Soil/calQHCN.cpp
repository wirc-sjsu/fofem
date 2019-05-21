
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

void calQHCN (double *QH, double bcQ, double bct, double bcr,
              double emis0, double CHp, double CEp, double IR, double IRl )
{
   *QH = emis0 * bcQ + CHp * bct + CEp * bcr + IR + IRl;
}


#ifdef wowowo
%--------------------------------------------------------------------------
function [ QH ] = calQHCN( bcQ,bct,bcr,emis0,CHp,CEp,IR,IRl )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Calculate the model-dervied surface energy term -- QH [W/m^2]

QH = emis0*bcQ + CHp*bct + CEp*bcr + IR + IRl;

end

#endif 