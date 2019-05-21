#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern int jstep ; 

void  caldiseq ( double diseq[],  double rhove[], double rhov[], double nlevel )
{
int i;
  
    for ( i = 1; i <= nlevel; i++ ) {
      diseq[i] = (rhove[i] - rhov[i]) / rhove[i];
    }
}


#ifdef wowow
%--------------------------------------------------------------------------
function [ diseq ] = caldiseq( rhove,rhov )
%--------------------------------------------------------------------------

diseq = (rhove-rhov)./rhove;

end
#endif 