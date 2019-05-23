

#include "string.h"
#include "math.h"
#include "BMSoil.h"

double temdR [eC_MaxLev]; 
double temRi [eC_MaxLev]; 
double mud1  [eC_MaxLev]; 
double mud2  [eC_MaxLev]; 

extern double Jim[];
extern double tempdi; 

/*********************************************************
*
*********************************************************/
void caldryvis (double mud[], double tempk[], double nlevel) 
{
int i; 
double f; 

 for ( i = 1; i <= nlevel; i++ ) {
   temdR[i] = tempk[i] * tempdi;
   if ( temdR[i] == 0 ) 
     temRi[i] = xZero("temdR"); 
   else
     temRi[i] = 1.0 / temdR[i];
   mud1[i]  = Jim[1] * temdR[i] + Jim[2] * xsqrt(temdR[i]) + Jim[3];
   mud2[i]  = (Jim[4] + (Jim[5] + (Jim[6] + Jim[7] * temRi[i]) * temRi[i] ) * temRi[i]) * temRi[i];
   mud[i]   = mud1[i] + mud2[i];
 }    

}


#ifdef wowowo
%--------------------------------------------------------------------------
function [ mud ] = caldryvis( tempk )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Determine the Vertical Profiles of pore space gas phase property: 
%           dynamic viscosity dry air -------- (mud) [Pa s] 

global tempdi
global Jim

temdR = tempk*tempdi;
temRi = 1./temdR;
mud1  = Jim(1)*temdR + Jim(2)*sqrt(temdR) + Jim(3);
mud2  = (Jim(4)+(Jim(5)+(Jim(6)+Jim(7)*temRi).*temRi).*temRi).*temRi;
mud   = mud1 + mud2;

end

#endif 