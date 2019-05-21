#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"


void caluHMV(double *u, double *delu, double *advdif, double *adtcon,
             double *xa, double difcoef, double delz, double *sourcev, double *rhov,
             double *rhcpa, double NLEV, double NL )
{
int i,n,nl,nlev;
double f,  adjt;
   
   nl = (int)NL;       /* for loop below wants these as int */
   nlev = (int)NLEV; 

   adjt = 2.0;

   for ( i = 1; i <= nlev; i++ ) {
     f = xa[i] * rhov[i];
     if ( f != 0 ) 
        delu[i] = adjt * delz * sourcev[i] / f ;
     else
        delu[i] = 0;

     if ( delu[i] < 0 ) 
        delu[i] = 0; 
   } /* for i */


   for ( i = 1; i <= nlev; i++ )  
      u[i] = 0; 
 
   for ( n = 1; n <= nl; n += 2 ) { 
      u[nl-n] = u[nlev+1-n] + delu[nlev-n];
      u[nlev-n] = 0.5 * (u[nl-n] + u[nlev+1-n]); }


   for ( i = 1; i <= nlev; i++ ) {
     advdif[i] = 0.5 * delz * difcoef * xa[i] * u[i];
     adtcon[i] = rhcpa[i]*advdif[i];  }



}

#ifdef wowow
%--------------------------------------------------------------------------
function [ u delu advdif adtcon ] = ... 
           caluHMV( xa,difcoef,delz,sourcev,rhov,rhcpa,nlev,nl )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Calculate the advective velocity associated with volitalization  
%   of liquid water = u [m/s]

adjt = 2;

delu = adjt*delz*sourcev./(xa.*rhov);

for n        = 1:nlev
  if delu(n) < 0;
     delu(n) = 0;
  end
end

u(1:nlev)   = 0;
for n       = 1:2:nl; 
  u(nl-n)   = u(nlev+1-n) + delu(nlev-n);
  u(nlev-n) = 0.5*(u(nl-n)+ u(nlev+1-n));
end

%__________________________________________________________________________
% Calculate the normalized advective diffusivity terms 
% for the vapor and heat transport equations

advdif = 0.5*delz*difcoef*xa.*u;
adtcon = rhcpa.*advdif;
% adtcon = 0*adtcon;
% advdif = 0**advdif;

end  
#endif 