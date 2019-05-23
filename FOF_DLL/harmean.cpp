#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

/***********************************************************
* The Matlab code when calling this functions multiplies the
* incoming array by a coefficient, but doesn't change that array
* so here I send it in and create a temp array 
* to work with. 
* NOTE: hmean MUST be nlev + 1 in size - see code below
*       aray is nlev in size 
*  ->  'array' is a reserverd word in C++
************************************************************/
void harmean(double *hmean, double co, double *A,  double nlev )
{
int n; 
double aray [eC_MaxLev]; 
    
   for ( n = 1; n <= nlev; n++ ) 
      aray[n] = A[n] * co; 

   hmean[1] = aray[1];
   for ( n = 2; n <= nlev; n++ ) 
     hmean[n] = 2.0 * aray[n] * aray[n-1] / (aray[n] + aray[n-1]);
    
 //  for ( n = 1; n <= nlev; n++ ) 
     hmean[(int)nlev+1] = aray[(int)nlev]; 
  
   
}


#ifdef wowowo
%--------------------------------------------------------------------------
function [ hmean ] = harmean( array,nlev )
%--------------------------------------------------------------------------
%
% Calculate the harmonic mean of the input array

hmean(1)      = array(1);

n             = 2:nlev;
hmean(n)      = 2*array(n).*array(n-1)./(array(n)+array(n-1));

hmean(nlev+1) = array(nlev);

end
#endif 