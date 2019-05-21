#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double tempdi;
extern double  Kim[]; 

void  calcondry( double zlamda[], double zlamdT[], double tempk[], double tempki[], double nlevel )
{ 
int i; 
double temdR,temRi,zla1,zla2,zla3,zla4; 

  for ( i = 1; i <= nlevel; i++ ) {
     temdR  = tempk[i] * tempdi;
     if ( temdR != 0 ) 
       temRi  = 1.0 / temdR;
     else
       temRi = xZero ("temdR"); 
     zla1   = Kim[1] * temdR + Kim[2] * xsqrt(temdR) + Kim[3];
     zla2   = Kim[4]+(Kim[5]+(Kim[6]+Kim[7]*temRi)*temRi)*temRi;

     zla3   = Kim[1]*tempdi + 0.5*Kim[2]*xsqrt(temdR)*tempki[i];
     zla4   = Kim[4]+(2.0*Kim[5]+(3.0*Kim[6]+4.0*Kim[7]*temRi)*temRi)*temRi;

     zlamda[i] = zla1 + zla2 * temRi;
     if ( zlamdT != NULL )
        zlamdT[i] = zla3 - zla4 * temRi * tempki[i];
  }  /* for */ 


}

#ifdef wowow
%--------------------------------------------------------------------------
function [ zlamda zlamdT ] = calcondry( tempk,tempki )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Determine the Vertical Profiles of pore space gas phase properties: 
%             thermal conductivity of dry air   -------- (zlamda) [W/M/K]  
%         d(thermal conductivity of dry air)/dT -------- (zlamdT) [W/M/K^2]  

global tempdi
global Kim

temdR  = tempk*tempdi;
temRi  = 1./temdR;
zla1   = Kim(1)*temdR + Kim(2)*sqrt(temdR) + Kim(3);
zla2   = Kim(4)+(Kim(5)+(Kim(6)+Kim(7)*temRi).*temRi).*temRi;
zla3   = Kim(1)*tempdi + 0.5*Kim(2)*sqrt(temdR).*tempki;
zla4   = Kim(4)+(2*Kim(5)+(3*Kim(6)+4*Kim(7)*temRi).*temRi).*temRi;

zlamda = zla1 + zla2.*temRi;
zlamdT = zla3 - zla4.*temRi.*tempki;

end

#endif 



