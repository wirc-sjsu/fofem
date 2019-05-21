/*******************************************************
* Name: calVsourceGBRa1.cpp
*       Updated 10/27/17
* NOTE: this replaces the previous version, see the old
*       code at end of this file 
********************************************************/
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern  double Svcoef, h2omol, rgas, Awax; 

double HKflux [eC_MaxLev];
double dHKfdT [eC_MaxLev];
double dsigdT [eC_MaxLev];
double dsigdp [eC_MaxLev];

/***************************************************************
* New Version 10/27/17
*****************************************************************/
void  calVsourceGNRa1( double *sourcevx, double *dSvdTx, double *dSvdpx, double *dSvdrx, double *Srhov, 
                      double *Awa, double *dAwadt, double *dAwadp, double *thtemp, double *rhove, double *rhov, double *Concoef, 
                      double *dCondT, double *dCondp, double *drevdT, double *drevdp, double *tempk, double *tempki, double nlevel )
{
int i; 

    for ( i = 1; i <= nlevel; i++ ) {

     Srhov[i]    = Awax * Concoef[i] * rhov[i];  // Srhov    = Awax*Concoef.*rhov;
                                       
     HKflux[i]   = Svcoef * xsqrt(rgas * tempk[i] / h2omol);  // HKflux   = Svcoef*sqrt(rgas*tempk/h2omol);

     sourcevx[i] = HKflux[i] * (Awa[i]*rhove[i]-Srhov[i]);    // sourcevx = HKflux.*(Awa.*rhove-Srhov);  

     dHKfdT[i]   = 0.5 * tempki[i] * HKflux[i];         // dHKfdT   = 0.5*tempki.*HKflux;

     dsigdT[i]   =  dHKfdT[i] * Awa[i] * rhove[i] + HKflux[i] * Awa[i] * drevdT[i] + dAwadt[i] * thtemp[i] * HKflux[i];   // dsigdT   =  dHKfdT.*Awa.*rhove+HKflux.*Awa.*drevdT+dAwadt.*thtemp.*HKflux;

     dsigdp[i]   =  HKflux[i] * Awa[i] * drevdp[i] + HKflux[i] * rhove[i] * dAwadp[i];      // dsigdp   =  HKflux.*Awa.*drevdp + HKflux.*rhove.*dAwadp;

     dSvdTx[i]   =  dsigdT[i] - dHKfdT[i] * Srhov[i] - HKflux[i] * dCondT[i] * rhov[i];               // dSvdTx   =  dsigdT - dHKfdT.*Srhov - HKflux.*dCondT.*rhov;

     dSvdpx[i]   =  dsigdp[i] - HKflux[i] * dCondp[i] * rhov[i];         // dSvdpx   =  dsigdp - HKflux.*dCondp.*rhov;

     dSvdrx[i]   = -Concoef[i] * HKflux[i];   // dSvdrx   = -Concoef.*HKflux;

  } 


}

#ifdef wowowowowo
%--------------------------------------------------------------------------
function [ sourcevx,dSvdTx,dSvdpx,dSvdrx,Srhov ] = ... 
           calVsourceGNRa1( Awa,dAwadt,dAwadp,thtemp,rhove,rhov,Concoef, ... 
                            dCondT,dCondp,drevdT,drevdp,tempk,tempki )
%--------------------------------------------------------------------------

%__________________________________________________________________________
%               Massman's New 2017 model for the source term
%
% Determine the Vertical Profiles of the vapor source term --- (sourcev)
%  and associated first derivatives wrt temperature ---------- (dSvdTx)  
%                                   wrt psin ----------------- (dSvdpx) 
%                               and wrt rhov ----------------- (dSvdrx)                                   

global Svcoef Awax
global h2omol rgas 

Srhov    = Awax*Concoef.*rhov;

HKflux   = Svcoef*sqrt(rgas*tempk/h2omol);
sourcevx = HKflux.*(Awa.*rhove-Srhov); 

dHKfdT   = 0.5*tempki.*HKflux;

dsigdT   =  dHKfdT.*Awa.*rhove+HKflux.*Awa.*drevdT+dAwadt.*thtemp.*HKflux;
dsigdp   =  HKflux.*Awa.*drevdp + HKflux.*rhove.*dAwadp;

dSvdTx   =  dsigdT - dHKfdT.*Srhov - HKflux.*dCondT.*rhov;
dSvdpx   =  dsigdp - HKflux.*dCondp.*rhov;
dSvdrx   = -Concoef.*HKflux;

end
#endif 











//#####################################################################################
//
//    This is old C and Matlab code 
// 
void  calVsourceGNRa_Old ( double *sourcevx, double *dSvdTx, double *dSvdpx, double *dSvdrx, double *Srhov, 
                      double *Awa, double *dAwadt, double *dAwadp, double *thtemp, double *rhove, double *rhov, double *Concoef, 
                      double *dCondT, double *dCondp, double *drevdT, double *drevdp, double *tempk, double *tempki, double nlevel )
{
int i; 
double HKflux,alphav,dalpdT,dalpdp,dsigdT,  dsigdp; 



    for ( i = 1; i <= nlevel; i++ ) {

      Srhov[i] = Concoef[i] * rhov[i];

      HKflux   = Svcoef * xsqrt(rgas*tempk[i] / h2omol);
      alphav   = Awa[i] * HKflux;

      dalpdT   = 0.5 * tempki[i] * alphav + dAwadt[i] * thtemp[i] * HKflux;
      dalpdp   = dAwadp[i] * HKflux;

      sourcevx[i] = alphav * (rhove[i] - Srhov[i]); 

      dsigdT   =  dalpdT * rhove[i] + alphav * drevdT[i];
      dsigdp   =  dalpdp * rhove[i] + alphav * drevdp[i];

     dSvdTx[i]   =  dsigdT - dalpdT * Srhov[i] - alphav * dCondT[i] * rhov[i];
     dSvdpx[i]   =  dsigdp - dalpdp * Srhov[i] - alphav * dCondp[i] * rhov[i];
     dSvdrx[i]   = -Concoef[i] * alphav;

    }

}


#ifdef wowo
%--------------------------------------------------------------------------
function [ sourcevx dSvdTx dSvdpx dSvdrx Srhov ] = ... 
           calVsourceGNRa( Awa,dAwadt,dAwadp,thtemp,rhove,rhov,Concoef, ... 
                           dCondT,dCondp,drevdT,drevdp,tempk,tempki )
%--------------------------------------------------------------------------

%__________________________________________________________________________
%               Massman's New model for the source term
%
% Determine the Vertical Profiles of the vapor source term --- (sourcev)
%  and associated first derivatives wrt temperature ---------- (dSvdTx)  
%                                   wrt psin ----------------- (dSvdpx) 
%                               and wrt rhov ----------------- (dSvdrx)                                   

global Svcoef 
global h2omol rgas 

Srhov    = Concoef.*rhov;

HKflux   = Svcoef*sqrt(rgas*tempk/h2omol);
alphav   = Awa.*HKflux;

dalpdT   = 0.5*tempki.*alphav + dAwadt.*thtemp.*HKflux;
dalpdp   = dAwadp.*HKflux;

sourcevx = alphav.*(rhove-Srhov); 

dsigdT   =  dalpdT.*rhove+alphav.*drevdT;
dsigdp   =  dalpdp.*rhove+alphav.*drevdp;

dSvdTx   =  dsigdT - dalpdT.*Srhov - alphav.*dCondT.*rhov;
dSvdpx   =  dsigdp - dalpdp.*Srhov - alphav.*dCondp.*rhov;
dSvdrx   = -Concoef.*alphav;

end

#endif 