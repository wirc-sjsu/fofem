#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

void calSw(double *Sw, int *LGLth,  double *theta, double *poros, double nlev )
{
int i; 
  for ( i = 1; i <= nlev; i++ ) {
     Sw[i] = 0;
     if ( theta[i] < 0 ) {
       LGLth[i] = 1 ;
       continue; }
     LGLth[i] = 0; 
     if ( poros[i] != 0 ) 
        Sw[i] = theta[i] / poros[i];
     else 
        Sw[i] = xZero ("calSw()"); 
  } /* for i */ 

}


#ifdef wowow
%--------------------------------------------------------------------------
function [ Sw LGLth ] = calSw( theta,poros,nlev )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Calculate soil water satturation amount -- (Sw)   

Sw(1:nlev) = 0;
LGLth      = theta < 0;
Sw(~LGLth) = theta(~LGLth)./poros(~LGLth);

end
#endif 