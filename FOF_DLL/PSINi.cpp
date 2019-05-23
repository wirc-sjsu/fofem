#include <stdio.h>
#include <string.h>
#include <math.h> 

double PSINi (double theta, double a0,double a[], double b[], double w)
{
double tw,Fourier; 
double psin;

 tw      = theta*w;

 Fourier =  a0 + a[1]*cos(tw)+b[1]*sin(tw) + a[2]*cos(2.0*tw)+b[2]*sin(2.0*tw) +   
      a[3]*cos(3.0*tw)+b[3]*sin(3.0*tw) + a[4]*cos(4.0*tw)+b[4]*sin(4.0*tw) + 
      a[5]*cos(5.0*tw)+b[5]*sin(5.0*tw) + a[6]*cos(6.0*tw)+b[6]*sin(6.0*tw);

  psin = exp (Fourier); 

  return psin ; 
}


#ifdef WWWW
%--------------------------------------------------------------------------
function [ psin ] = PSINi( theta,a0,a,b,w )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% 
%  Subroutine uses an empirical fit to the inverse of the WRC to estimate 
%  the initial value of the psin (psini) from the inital value of soil 
%  moisture (thetai)  
%  NOTE: This function was derived on the basis of a fit to the WRC with 
%  0 <= theta <= 0.30, meaning it may not be as precise for values of 
%  theta > 0.30;

tw      = theta*w;

Fourier = a0 + a(1)*cos(tw)+b(1)*sin(tw) + a(2)*cos(2*tw)+b(2)*sin(2*tw) + ...
           a(3)*cos(3*tw)+b(3)*sin(3*tw) + a(4)*cos(4*tw)+b(4)*sin(4*tw) + ...
           a(5)*cos(5*tw)+b(5)*sin(5*tw) + a(6)*cos(6*tw)+b(6)*sin(6*tw);
psin    = exp(Fourier);

end

#endif 