/************************************************************
* Name: calthetaFYr.cpp
* Desc: This is BM's version - Aug 22 2017
  There's a 'if' conditional call for catthetaFYr in cranknicholson.m but it doesn't 
  get called so i didn't convert it  . . . 
    Didn't convert this yet because it doesn't get called
************************************************************/
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include <new> 
#include "BMsoil.h"


extern double  mur, almur, mui, Nexp, Mexp;
extern double gthetar, thetar;


/************************************************************
%__________________________________________________________________________
% Determine Vertical Profile of soil 
%      volumetric moisture ---------------------- (theta) [ m^3/m^3 ]
%  
****************************************************************/
int calthetaFYr( double *theta, double *thpsin,  double *thpsini, double *thtemp,
                 double *rfac, double *drfacT, double *drfacp, double *psin, double *poros,
                 double nlevel )
{
int i; 
double d; 
double  term1 [eC_MaxLev], term2 [eC_MaxLev], term3 [eC_MaxLev]; 
double  term4 [eC_MaxLev], term5 [eC_MaxLev], term6 [eC_MaxLev]; 
double  aux0 [eC_MaxLev], aux1 [eC_MaxLev], theta0 [eC_MaxLev]; 

// term1   = 1-log(1+psin/mur)*almur;
 for ( i = 1; i <= nlevel; i++ ) {
   term1[i] = 1.0 - xlog (1.0 + psin[i] / mur) * almur; 

// aux0    = exp(1)+(psin/mui).^Nexp;
   aux0[i]    = exp(1.0) + xpow ( ( psin[i] / mui), Nexp );

// aux1    = log(aux0);
   aux1[i]    = xlog(aux0[i]);

// term2   = aux1.^Mexp;
   term2[i] = xpow (aux1[i],Mexp) ;

// theta0  = (poros-rfac*thetar).*term1./term2;
   theta0[i]  = (poros[i] - rfac[i] * thetar) * term1[i] / term2[i];

// theta   = theta0 + rfac*thetar;
   theta[i]   = theta0[i] + rfac[i] * thetar;
 

//%__________________________________________________________________________
//% Also calculate \partial theta/\partial psin --- (thpsin)  [ - ] 
//%                             and its inverse --- (thpsini) [ - ] 

   term3[i] = -(poros[i] - rfac[i]*thetar)*almur / ((mur+psin[i]) * term2[i]);
   term4[i] = -Mexp * theta0[i] / aux1[i];
   term5[i] =  Nexp *    xpow ( (psin[i] / mui), (Nexp-1))   / (aux0[i] * mui); 
   term6[i]   =  thetar * (1.0 - term1[i] / term2[i]);
   thpsin[i]  =  term3[i] + term4[i] *term5[i] + drfacp[i] * term6[i];
   thpsini[i] =  1.0 / thpsin[i];
   thtemp[i]  = drfacT[i] * term6[i];


} /* for i */ 

   return 1; 

}

#ifdef NNNNN
%--------------------------------------------------------------------------
function [ theta thpsin thpsini thtemp ] = ... 
                        calthetaFYr( rfac,drfacT,drfacp,psin,poros )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Determine Vertical Profile of soil 
%      volumetric moisture ---------------------- (theta) [ m^3/m^3 ]
%      from the normalized soil moiture tension   (psin)  [    -    ] 

global mur almur mui Nexp Mexp
global thetar

term1   = 1-log(1+psin/mur)*almur;
aux0    = exp(1)+(psin/mui).^Nexp;
aux1    = log(aux0);
term2   = aux1.^Mexp;
theta0  = (poros-rfac*thetar).*term1./term2;
theta   = theta0 + rfac*thetar;

%__________________________________________________________________________
% Also calculate \partial theta/\partial psin --- (thpsin)  [ - ] 
%                             and its inverse --- (thpsini) [ - ] 

term3   = -(poros-rfac*thetar)*almur./((mur+psin).*term2);
term4   = -Mexp*theta0./aux1;
term5   =  Nexp.*(psin/mui).^(Nexp-1)./(aux0*mui); 
term6   =  thetar.*(1-term1./term2);
thpsin  =  term3 + term4.*term5 + drfacp.*term6;

thpsini = 1./thpsin;
thtemp  = drfacT.*term6;


#endif 