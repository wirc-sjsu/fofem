
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double  temp0;
extern double  actemp, tempoi; 
extern double  numf1, numf2;

// double tempki  [eC_MaxLev]; 
 extern double  tempki[];


//%----------------------------------------------------------------------
//function [ Rcoef dRcdT dRcdp ] = calConCoef2( psin,temp )
//%--------------------------------------------------------------------------
//%__________________________________________________________________________
//%        Massman Residual Soil Moisture Model [ Rcoef ]
//%        Reduces residual soil moisture as psin and tempk increase
//%        Also computes first derivatives [ dRcdT dRcdp ]

void calConCoef2(double *Rcoef, double *dRcdT, double *dRcdp, double psin, double temp[], double nlevel )
{
int i; 
double Cf1;

  for ( i = 1; i <= nlevel; i++ )   
    tempki[i] = 1.0 / (temp[i] + temp0);

  Cf1    = numf1*actemp*(1-numf2*psin);

  for ( i = 1; i <= nlevel; i++ )
    Rcoef[i] = xexp (Cf1 * (tempki[i] - tempoi));   // Rcoef  =  exp(Cf1.*(tempki-tempoi));

  for ( i = 1; i <= nlevel; i++ )
    dRcdT[i] = -Cf1 * tempki[i] * tempki[i] * Rcoef[i];       // dRcdT  = -Cf1.*tempki.*tempki.*Rcoef;

  for ( i = 1; i <= nlevel; i++ )
    dRcdp[i] = -numf1 * numf2 * (tempki[i] - tempoi) * Rcoef[i];   // dRcdp  = -numf1*numf2*(tempki-tempoi).*Rcoef;

 
}

/*****************************************************************************
*
*
*****************************************************************************/

void calConCoef2_Ary(double Rcoef[], double dRcdT[], double dRcdp[], double psin[], double temp[], double nlevel )
{
int i; 

double *_tempki = new double [nlevel+1]; 
double *Cf1    = new double [nlevel+1]; 

 for ( i = 1; i <= nlevel; i++ ) {  
    _tempki[i] = 1.0 / (temp[i] + temp0);
    Cf1[i]    =  numf1 * actemp * ( 1.0 - numf2 * psin[i]);
  }

/* Could probably do this all in one loop */ 

 for ( i = 1; i <= nlevel; i++ )
   Rcoef[i] = xexp (Cf1[i] * (_tempki[i] - tempoi));   // Rcoef  =  exp(Cf1.*(tempki-tempoi));

 for ( i = 1; i <= nlevel; i++ )
   dRcdT[i] = -Cf1[i] * _tempki[i] * _tempki[i] * Rcoef[i];       // dRcdT  = -Cf1.*tempki.*tempki.*Rcoef;

 for ( i = 1; i <= nlevel; i++ )
   dRcdp[i] = -numf1 * numf2 * (_tempki[i] - tempoi) * Rcoef[i];   // dRcdp  = -numf1*numf2*(tempki-tempoi).*Rcoef;

  delete _tempki;
  delete Cf1;  
}


#ifdef wowow
%--------------------------------------------------------------------------
function [ Rcoef dRcdT dRcdp ] = calConCoef2( psin,temp )
%--------------------------------------------------------------------------

%__________________________________________________________________________
%        Massman Residual Soil Moisture Model [ Rcoef ]
%        Reduces residual soil moisture as psin and tempk increase
%        Also computes first derivatives [ dRcdT dRcdp ]

global temp0
global actemp tempoi 
global numf1 numf2

tempki = 1./(temp+temp0);

Cf1    = numf1*actemp*(1-numf2*psin);

Rcoef  =  exp(Cf1.*(tempki-tempoi));
dRcdT  = -Cf1.*tempki.*tempki.*Rcoef;
dRcdp  = -numf1*numf2*(tempki-tempoi).*Rcoef;

end
#endif 