#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double temp0; 
extern double *bcva; 
 
/***********************************************************
*
* In: bcta, bcva
* Out: bceta4
**********************************************************/
void  caleta4 (double *bceta4, double *bcta, double *bcva, double N) 
{

int i,n;
double f,g; 

   n = N; 
double *bct0 = new double [n+1];
  for ( i = 1; i <=n; i++ ) 
     bct0[i] = bcta[i] + temp0;

double *bct02 = new double [n+1];
  for ( i = 1; i <=n; i++ ) 
    bct02[i] = bct0[i] * bct0[i];    //  bct02  = bct0.*bct0;

double *emisa = new double [n+1];
  for ( i = 1; i <=n; i++ ) {
    g = 1.0 / 7.0; 
    f = xpow ((bcva[i] / bct0[i]), g);
    emisa[i]  = 0.6423 * f;
  }


 for ( i = 1; i <=n; i++ ) 
   bceta4[i] = emisa[i] * bct02[i] * bct02[i];


   delete bct0; 
   delete bct02;
   delete emisa; 
}


#ifdef  wowowo
%------------------------------------------------------------------------
function [ bceta4 ] = caleta4( bcta,bcva )
%------------------------------------------------------------------------

%________________________________________________________________________
% Calculate downward atmospheric shortwave radiation 
% for the surface energy balance boundary condition

global temp0

bct0   = bcta+temp0;
bct02  = bct0.*bct0;
emisa  = 0.6423*(bcva./bct0).^(1/7);
bceta4 = emisa.*bct02.*bct02;

end

#endif 