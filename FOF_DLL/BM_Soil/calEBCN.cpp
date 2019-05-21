#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"


void calEBCN (double *_Radiant, double *_Convec, double *_LE, double *_G0, 
              double bcQ,   double bct, double bcr,
              double emis0, double CHp, double CEp,
              double RE,    double IR,  double rhov0,
              double tmp )
{
double Radiant,Convec,LE,G0;     

  Radiant = emis0 * bcQ;
  Convec  = CHp * (tmp - bct);
  LE      = RE * rhov0 - CEp * bcr;
  G0      = Radiant + IR - Convec - LE;

  *_Radiant =  Radiant;
  *_Convec  =  Convec; 
  *_LE      =  LE;     
  *_G0      =  G0;     
}

#ifdef wowow
%--------------------------------------------------------------------------
function [ Radiant Convec LE G0 ] = ... 
           calEBCN( bcQ,bct,bcr,emis0,CHp,CEp,RE,IR,rhov0,tmp )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Calculate Soil Surface Energy Balance terms: Incident Radiation,
%   Convective Heat Flux, the Evaporation Rate, and the Soil Heat Flux 

Radiant = emis0*bcQ;
Convec  = CHp*(tmp-bct);
LE      = RE*rhov0 - CEp*bcr;
G0      = Radiant + IR - Convec - LE;

end
#endif 