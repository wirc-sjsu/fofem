
#include "string.h"
#include "math.h"
#include "BMSoil.h"

double gradNv [eC_MaxLev]; 

/********************************************************************
*
* Note-1: The Matlab code (at end of file) does this differently 
*         but the end result it to 0 out the array 
*******************************************************************/ 
void  caldelHMV( double *u, double *delu, double *advdif, double *adtcon, 
                 double *uu1, double *Lambdau, double delz, double delvel, 
                 double *rhov, double bcra, double rh0, double CE,
                 double *vdiff, double nlev, double nl )
{
int i,n; 


   gradNv[1] = -CE * rh0 *(rhov[1] - bcra) / (vdiff[1] * rhov[1]);

  for ( n = 2; n <= nl; n++ ) {
    gradNv[n] = ( rhov[n+1] - rhov[n-1] ) / (2.0 * delz * rhov[n] );
  } 
  gradNv[(int)nlev] = 0; 


  for ( i = 1; i <= nlev; i++ ) 
    u[i] = -uu1[i] * gradNv[i];

/* See Note-1 above */ 
  for ( i = 1; i <= nlev; i++ ) { 
    delu[i] = 0;
    advdif[i] = 0; }

  for ( i = 1; i <= nlev; i++ )  
    adtcon[i] = -0.25 * delvel * Lambdau[i] * gradNv[i];


} 

#ifdef wowo
%--------------------------------------------------------------------------
function [ u delu advdif adtcon ] = caldelHMV( uu1,Lambdau,delz,delvel, ... 
                         rhov,bcra,rh0,CE,vdiff,nlev,nl )
%--------------------------------------------------------------------------
%__________________________________________________________________________
% Calculate the density driven advective velocity associated with  
%   volitalization of liquid water = u [m/s]

gradNv(1)    = -CE*rh0*(rhov(1)-bcra)/(vdiff(1)*rhov(1));
% gradNv(1)  = -CE*(rhov(1)-bcra)/(vdiff(1)*rhov(1));
% gradNv(1)  = (-3*rhov(1)+4*rhov(2)-rhov(3))/(2*delz*rhov(1));
n            =  2:nl;
gradNv(n)    = (rhov(n+1)-rhov(n-1))./(2*delz*rhov(n));
gradNv(nlev) =  0;

u            = -uu1.*gradNv;

nn           = 1:nlev;
delu(nn)     = 0;

%__________________________________________________________________________
% Calculate the normalized advective diffusivity terms 
% for the vapor and heat transport equations

% ++ advdif  =  difcoef*deldifu;
advdif(nn)   =  0;
adtcon       = -0.25*delvel*Lambdau.*gradNv;

end  
#endif 