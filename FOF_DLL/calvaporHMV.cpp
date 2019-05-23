#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double h2omol, rgas; 

void calvaporHMV (double *vapres, double *rhov, double *tempk, double nlevel )
{
int i;

   for ( i = 1; i <= nlevel; i++ ) 
     vapres[i] = rgas * rhov[i] * tempk[i] / h2omol;

}



#ifdef wowowo
%--------------------------------------------------------------------------
function [ vapres ] = calvaporHMV( rhov,tempk )
%--------------------------------------------------------------------------

%_______________________________________________________________________
% Determine the Vertical Profile of soil vapor pressure (vapres) [Pa]

global h2omol rgas 

vapres = rgas*rhov.*tempk/h2omol;

end
#endif 

