#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"


extern double coefcs1;
extern double cwater, coefwt1, coefwt2;

/*********************************************************************************
*
* NOTE: the call is sending in a NULL for *csi, see notes where this
*       function gets called 
*********************************************************************************/
void calcsHMV(double cs[], double csi[], double csd[], double den[], double temp[], double theta[], double nlevel )
{
int i; 
double cw, f, tem; 

   for ( i = 1; i <= nlevel; i++ ) 
     cs[i] = csd[i] + den[i] * coefcs1 * temp[i];


/* See the MatLab code at end of this file, it looks pretty different */
/* and confusing but it translates to this, the one condition  */
/* just leaves the existing cs[i] in place, for the other condition it does a calc */
  for ( i = 1; i <= nlevel; i++ ) {
    if  ( theta[i] <= 0.001 )
      cs[i] = cs[i]; 
    else {
      cw = cwater + (coefwt1 + coefwt2 * temp[i] ) * temp[i];
      cs[i] = cs[i] + theta[i] * cw; 

    } /* else */  

  } /* for i */

  if ( csi == NULL ) 
    return ; 
  for ( i = 1; i <= nlevel; i++ ){
    if ( cs[i] == 0 ) 
       csi[i] = xZero("cs"); 
    else 
       csi[i] = 1.0 / cs[i];
  } 
}

#ifdef wowoow
%--------------------------------------------------------------------------
function [ cs csi ] = calcsHMV( csd,den,temp,theta )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Determine the Vertical Profile of soil volumetric specific heat 
%      as a function of soil moisture, temperature,  
%      and bulk density ---------- (cs) [ J/m^3/K ]
%

global coefcs1
global cwater coefwt1 coefwt2

%--------------------------------------------------------------------------
%                                 Dry Soil    
%--------------------------------------------------------------------------

cs = csd + den*coefcs1.*temp;

%--------------------------------------------------------------------------
%                                 Moist Soil    
%--------------------------------------------------------------------------

logicl      = theta <= 0.001;

cw          = cwater + (coefwt1 + coefwt2*temp(~logicl)).*temp(~logicl);
cs(~logicl) = cs(~logicl) + theta(~logicl).*cw;

%--------------------------------------------------------------------------
%                       Calculate the inverse of cs                    
%--------------------------------------------------------------------------

csi = 1./cs;

end
#endif 