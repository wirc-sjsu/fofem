
#include <stdio.h>
#include <math.h> 
#include "BMSoil.h"


#ifdef XXXXX
%--------------------------------------------------------------------------
function [ Xa ] = Vmult5( a,B )
%--------------------------------------------------------------------------

Xa = a(1)+(a(2)+(a(3)+(a(4)+(a(5)+a(6)*B).*B).*B).*B).*B;  



 A1 =   (a[5)]]+a(6)*B
 A2 =   (a(4) +  (a(5)+a(6)*B)          .*B)
 A3 =   (a(3) +  (a(4)+(a(5)+a(6)*B).*B)  .*B)
 A4 =  (a(2 )+  (a(3)+(a(4)+(a(5)+a(6)*B).*B).*B).*B)
 A5 =   a(1) +  (a(2)+(a(3)+(a(4)+(a(5)+a(6)*B).*B).*B).*B).*B

end
#endif 

/************************************************************
*
*
* Note-1: Matlab arrays start with [1], C++ are [0]
*************************************************************/
double Vmult5 (double a1, double a2, double a3, double a4, double a5, double a6, double temR)
{
int i; 
double f, g; 
double a[10];
double B;
  
 B = temR; 

 a[1] = a1;    /* See Note-1 above */
 a[2] = a2;
 a[3] = a3;
 a[4] = a4;
 a[5] = a5;
 a[6] = a6;

// works 
  for ( i = 6; i >= 1; i-- ) {
     f = a[i-1] +  a[i] * B ;
     a[i-1] = f; 
  }

  g = a[1];

  return g; 

// also works  -
//  for ( i = 5; i >= 1; i-- ) {
//     f = a[i] + a[i+1] * B;
 //    a[i] = f; 
 // }




}