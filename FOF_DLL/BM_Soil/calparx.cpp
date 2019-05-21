#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double thetan, mexp; 

void calparx(double *thpx, double *thetax, double *apsi4, double *thznr, double *psiX, double nlevel )
{
int i;
  for ( i = 1; i <= nlevel; i++ ) 
    thpx[i] = (-thetan + 4.0 * mexp * thetax[i] * apsi4[i] / thznr[i]) / psiX[i];
}


#ifdef wooww
%------------------------------------------------------------
function [ thpx ]  = calparx( thetax,apsi4,thznr,psiX )
%------------------------------------------------------------

%____________________________________________________________________
% Calculate the partial derivative \partial theta/\partial psiX
%   where psiX = psin or psiT          

global thetan mexp

thpx = (-thetan + 4*mexp*thetax.*apsi4./thznr)./psiX;

end
#endif 