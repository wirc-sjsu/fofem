#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double pres;

/**************************************************************/
void calxhiv1(double xhiv1[], double dxhivdT[], double dxhivdr[],   
              double vapres[], double tempki[], double rhov[], double nlevel )
{
int i; 
double aux; 
  for ( i = 1; i <= nlevel; i++ ) {
    xhiv1[i]   = abs(vapres[i]) / (pres + abs(vapres[i]));
    aux     = xhiv1[i] * (1.0 - xhiv1[i]);
    dxhivdT[i] = aux * tempki[i];
    if ( rhov[i] != 0 ) 
       dxhivdr[i] = aux / rhov[i];
    else 
       dxhivdr[i] = xZero ("rhov"); 
  }
}



#ifdef wowowo
%--------------------------------------------------------------------------
function [ xhiv1 dxhivdT dxhivdr ] = calxhiv1( vapres,tempki,rhov )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Calculate the vapor molar mixing ratio 

global pres

xhiv1   = abs(vapres)./(pres+abs(vapres));

aux     = xhiv1.*(1-xhiv1);

dxhivdT = aux.*tempki;
dxhivdr = aux./rhov;

% xhiv1 = vapres/pres;
% LGL = xhiv1 <= 1;
% xhiv1(~LGL)  = 1;

end
#endif 