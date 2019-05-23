
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

void calpsinProf (double psin[], double psini, double nlev)
{
int i; 
   for ( i = 1; i <= nlev; i++)
       psin[i] = psini; 
} 

#ifdef wowo
%------------------------------------------------------------
function [ psin ] = calpsinProf( psini,nlev )
%------------------------------------------------------------

%____________________________________________________________
% Assign the initial value for psin [ - ]

psin(1:nlev) = psini;

end

#endif 