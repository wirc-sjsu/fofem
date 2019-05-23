
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"


// %--------------------------------------------------------------------------
// function [ den xm poros temp theta csd ] = ... 
//                   calprofP( den0,tempi,thetai,pard,nlev )    
// %--------------------------------------------------------------------------

// %__________________________________________________________________________
// % Determine Vertical Profiles of soil 
// %      bulk density ------------------------- (den)   [g/cm^3] or [Mg/m^3]
// %      volumetric mineral fraction ---------- (xm)    [ - ]
// %      total porosity ----------------------- (poros) [ - ]
// %      temperature -------------------------- (temp)  [ C ]
// %      volumetric moisture ------------------ (theta) [ m^3/m^3]
// %      volumetric specific heat for dry soil  
// %        with no temperature dependency ----- (cpd)   [ J/m^3/K ]

// %__________________________________________________________________________

void calprofP (double *den, double *xm, double *poros, double *temp, double *theta, double *csd, 
                double den0, double tempi, double thetai, double pard, double nlev)
{
int i,j; 

// % Initial profiles of soil bulk density, temperature, and moisture
// NOTE: I think 'np' 'den(np) is just a way of init'ing the arrays without using a loop
//         np gets assigned = [ 1 2 3 4 . . . ] which then are used as indexing values..  
//np             = 1:nlev;
//den(np)        = den0;
//temp(np)       = tempi;
//theta(np)      = thetai;

  for ( i = 1; i <= nlev; i++ ) { 
     den[i] = den0; 
     temp[i] = tempi; 
     theta[i] = thetai; }


// %__________________________________________________________________________
// % Next calculate the mineral fraction and the porosity 
// % Also make sure that the soil moisture does not exceed saturation
// Note-1   MY NOTES below
// The Matlab code uses an array of bool to make value assignments based 
//  on the <= comparision, here we can just make the compare and assignment
//  while we loop. 
//  I set up the logicl array to mimick MatLab but realized I don't need it
//  but left it in place for now in case i need to review this again.
//int *logicl = new int[(int)nlev+1];
 for ( i = 1; i <= nlev; i++ ) { 
    xm [i] = den[i] / pard;
    poros[i] = 1.0 - xm[i];
    if ( theta[i] <= poros[i] )   //logicl         = theta <= poros;
 //     logicl[i] = 1;
       j = 0;  // Just do something 
    else {
  //    logicl[i] = 0;
      theta[i] = poros[i];   /* NOTE NOTE - See Note-1 above */
    }
/* Question - what does this do ? - I didn't implement this but what's in theta match against */
/*  what Matlab had - see above where we assign theta[] */  
   // theta(~logicl) = poros(~logicl);

 }  /* for end */ 


// %__________________________________________________________________________
// % Finally calculate the profile of soil's dry volumetric specific heat 
extern double csoil0; 
   for ( i = 1; i <= nlev; i++ ) 
     csd[i] = csoil0 * den[i]; 

}


#ifdef wowowowo
%--------------------------------------------------------------------------
function [ den xm poros temp theta csd ] = ... 
                  calprofP( den0,tempi,thetai,pard,nlev )    
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Determine Vertical Profiles of soil 
%      bulk density ------------------------- (den)   [g/cm^3] or [Mg/m^3]
%      volumetric mineral fraction ---------- (xm)    [ - ]
%      total porosity ----------------------- (poros) [ - ]
%      temperature -------------------------- (temp)  [ C ]
%      volumetric moisture ------------------ (theta) [ m^3/m^3]
%      volumetric specific heat for dry soil  
%        with no temperature dependency ----- (cpd)   [ J/m^3/K ]

%__________________________________________________________________________
% Initial profiles of soil bulk density, temperature, and moisture

np             = 1:nlev;
den(np)        = den0;
temp(np)       = tempi;
theta(np)      = thetai;

%__________________________________________________________________________
% Next calculate the mineral fraction and the porosity 
% Also make sure that the soil moisture does not exceed saturation

xm             = den/pard;
poros          = 1 - xm;
logicl         = theta <= poros;
theta(~logicl) = poros(~logicl);

%__________________________________________________________________________
% Finally calculate the profile of soil's dry volumetric specific heat 

global csoil0

csd = csoil0*den;    

end

#endif 
