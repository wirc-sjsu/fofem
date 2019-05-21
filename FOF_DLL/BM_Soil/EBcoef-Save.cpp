#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"


extern double  stefbol;

/*************************************************************************************
*
*
***************************************************************************************/
void  EBcoef(double *_emis0, double *_IR, double *_CHp, double *_RH, double *_IRl, double *_CEp, double *_RE, 
             double theta0,double bct, double eta[], double Htrans, double tmp, double tmpk,
             double tmpki, double CE, double rh0, double CU, double u0, double lv0, double xa0 )
{
double emstef,t03,t04,IRx,Ea;
double emis0, IR, CHp, RH, IRl, CEp, RE;
   
  emis0 = 0.93 + 4.0 * theta0 * theta0 * (9.0 - 20.0 * theta0 ) / 27.0; 

  emstef = emis0 * stefbol;
  t03 = tmpk * tmpk * tmpk;
  t04 = t03 * tmpk;

  if ( (tmp - bct)  >= 0.1 ) { 
    CHp = Htrans * tmpki;
    IR = -emstef * t04; }
  else {
    CHp = 0;
    IR = -emstef * t04; }

  IRx = 4.0 * emstef * t03;
  RH = IRx + CHp;
  IRl = IRx * tmp;

  Ea  = rh0 * CE;
  CEp = lv0 * Ea; 
  RE  = CEp + lv0 * CU * u0;

 *_emis0  = emis0;
 *_IR     = IR; 
 *_CHp    = CHp;
 *_RH     = RH; 
 *_IRl    = IRl;
 *_CEp    = CEp;
 *_RE     = RE;

}


#ifdef wowow
%--------------------------------------------------------------------------
function [ emis0 IR CHp RH IRl CEp RE ] = ... 
         EBcoef( theta0,bct,eta,Htrans,tmp,tmpk,tmpki,CE,rh0,CU,u0,lv0,xa0 )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Calculate the soil surface emissivity -- A function of soil moisture  

emis0 = 0.93 + 4*theta0*theta0*(9-20*theta0)/27; 

%__________________________________________________________________________
% Calculate the soil surface Infrared Radiation loss ----- IR  [W/m^2]  
% Calculate the Convective Heat Flux transfer coeffient -- CHp [rho_aCpC_H] 
% Calculate the Convective Heat Flux coefficient --------- RH  [W/m^2/K] 
% Calculate the linearized infrared term ----------------- IRl [W/m^2]  

global stefbol

emstef      = emis0*stefbol;
t03         = tmpk*tmpk*tmpk;
t04         = t03*tmpk;

if tmp-bct >= 0.1
    CHp     = Htrans*tmpki;
    IR      = -emstef*t04;
%---IR      = emstef*(eta-t04);
else
    CHp     = 0;
    IR      = -emstef*t04;
end

IRx         = 4*emstef*t03;
RH          = IRx + CHp;
IRl         = IRx*tmp;

%__________________________________________________________________________
% Calcuate the surface Evporation transfer coefficent --- CEp [L_v rh_s CE] 
% Calcuate the surface evporation coefficent ------------ RE  [W-m/kg] 

Ea  = rh0*CE;
CEp = lv0*Ea; 
RE  = CEp + lv0*CU*u0;
% RE  = CEp + lv0*CU*xa0*u0;

end
#endif 