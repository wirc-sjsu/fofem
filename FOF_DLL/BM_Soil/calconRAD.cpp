#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double stefRad; 

/**********************************************************************/
void calconRAD (double *tcon, double *tconR,  double *poros, double *theta, double *tempk, double *tcon1, double nlevel )
{
int i; 
double Rndex; 

  for ( i = 1; i <= nlevel; i++ ) {
    Rndex = 1.0 + theta[i] / (3.0 * poros[i]);
    tconR[i] = stefRad * Rndex * Rndex * tempk[i] * tempk[i] * tempk[i];
    tcon[i] = tconR[i] + tcon1[i]; }
}


#ifdef wowow
%--------------------------------------------------------------------------
function [ tcon tconR ] = calconRAD( poros,theta,tempk,tcon1 )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Pore space radiation soil thermal conductivity term -- (tconR) [W/m/K]

global stefRad

Rndex = 1 + theta./(3*poros);

tconR = stefRad*Rndex.*Rndex.*tempk.*tempk.*tempk;

tcon  = tconR + tcon1;

end
#endif 