#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double temp0;

void  caltempkHMV( double *tempk, double *tempki, double *tempr, double *temp, double nlevel)
{
int i; 
double f; 

  for ( i = 1; i <= nlevel; i++ )  
    tempk [i] = temp[i] + temp0;
  for ( i = 1; i <= nlevel; i++ )  
    tempki[i] = 1.0 / tempk[i];
  for ( i = 1; i <= nlevel; i++ )  
    tempr [i] = temp0 * tempki[i];
  
}



#ifdef wowo
%--------------------------------------------------------------------------
function [ tempk tempki tempr] = caltempkHMV( temp )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Calculate temperature in degrees K (tempk)
% Calcualte the reciprocal of tempk (tempki)
% Calculate the reduced temperature (tempr)

global temp0

tempk  = temp + temp0;

tempki = 1./tempk;

tempr  = temp0*tempki;

end
#endif 