/************************************************************
* Name: calthetaCSr.cpp
* Desc: This is BM's updated version Aug 22 2017
************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h> 
#include <new> 
#include "BMsoil.h"

extern double thetah, alpha4, mexp;
extern double thetan;
extern double thetar; 

double  apsi4  [eC_MaxLev];
double  thznr  [eC_MaxLev]; 
double  thznrm [eC_MaxLev]; 
double  thetax [eC_MaxLev]; 
double  term1  [eC_MaxLev]; 
double  term2  [eC_MaxLev]; 

void _XXX (double psin[], double term2[], double thpsin[], double thpsini[], double thtemp[], double nlevel);

/*****************************************************************************
*
*
******************************************************************************/
void calthetaCSr(double theta[], double thpsin[], double thpsini[], double thtemp[], 
                 double rfac[],  double drfacT[], double drfacp[],  double *psin, 
                 double nlevel )
{
int i;
double f; 

/* NOTE: could probably do all of these in one for loop, but one at a time mimicks */
/*       Matlab better incase doing things in a certain order might matter  */ 
  for ( i = 1; i <= nlevel; i++ )
    apsi4[i]  =  alpha4 * psin[i] * psin[i] * psin[i] * psin[i];

  for ( i = 1; i <= nlevel; i++ ) 
    thznr[i] =  1.0 + apsi4[i];

  for ( i = 1; i <= nlevel; i++ ) 
    thznrm[i] = xpow (thznr[i], mexp);

  for ( i = 1; i <= nlevel; i++ ) 
    thetax[i] = (thetah - rfac[i] * thetar) * thznrm[i];

//looks like this blows on jstep 1414 psin[19 20  have  negative, might be 
 // fuckin that log () 
// see what happens in Matlab, try setting a negative value in Matlab

  for ( i = 1; i <= nlevel; i++ ) {
//    if ( psin[i] < 0 ) 
//       printf ("psin < 0 \n"); 
    f = xlog (psin[i]); 
    theta[i]  = -thetan * xlog(psin[i]) + thetax[i] + rfac[i] * thetar;
  } /* for */ 

//%__________________________________________________________________________
//% Also calculate \partial theta/\partial psin --- (thpsin)  [ - ] 
//%                             and its inverse --- (thpsini) [ - ] 
  calparx (term1, thetax, apsi4, thznr, psin, nlevel);

  for ( i = 1; i <= nlevel; i++ ) 
    term2[i] = thetar * drfacp[i] * (1.0 - thznrm[i]);

  for ( i = 1; i <= nlevel; i++ ) 
    thpsin[i] = term1[i] + term2[i];

  for ( i = 1; i <= nlevel; i++ ) 
    thpsini[i] = 1.0 / thpsin[i];

  for ( i = 1; i <= nlevel; i++ ) 
    thtemp[i] = thetar * drfacT[i];

}


#ifdef wowo
%--------------------------------------------------------------------------
function [ theta thpsin thpsini thtemp ] = ... 
                        calthetaCSr( rfac,drfacT,drfacp,psin )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Determine Vertical Profile of soil 
%      volumetric moisture ---------------------- (theta) [ m^3/m^3 ]
%      from the normalized soil moiture tension   (psin)  [    -    ] 

global thetah alpha4 mexp
global thetan
global thetar 

apsi4  =  alpha4*psin.*psin.*psin.*psin;
thznr  =  1+apsi4;
thznrm =  thznr.^mexp;
thetax =  (thetah-rfac*thetar).*thznrm;

theta  = -thetan*log(psin) + thetax + rfac*thetar;

%__________________________________________________________________________
% Also calculate \partial theta/\partial psin --- (thpsin)  [ - ] 
%                             and its inverse --- (thpsini) [ - ] 

term1   = calparx(thetax,apsi4,thznr,psin);
term2   = thetar*drfacp.*(1-thznrm); 
thpsin  = term1 + term2;

thpsini = 1./thpsin;
thtemp  = thetar*drfacT;

end

#endif 