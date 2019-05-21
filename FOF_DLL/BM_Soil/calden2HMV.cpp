#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double TMax, denwx, denix, denwNx;
extern double dencr; 
extern double BW[]; 
extern double tempcr; 

double tauc   [eC_MaxLev]; 
double tauc1  [eC_MaxLev]; 
double tauc2  [eC_MaxLev]; 
double denwa  [eC_MaxLev]; 
double denwTa [eC_MaxLev]; 
double denwTN [eC_MaxLev]; 

/******************************************************************************
* Name: calden2HMV
* Desc: 
******************************************************************************/
void calden2HMV(bool *LGL, double *denw, double *denwi, double *denwdT, double *denwN,
                double *TempR, double *tempk, double nlev, int i_Step )
{
int i; 
double  f,g; 

/* These will be default return values if the calc below isn't performed */
   for ( i = 1; i <= nlev; i++ ) {
     denw[i]   = denwx;
     denwi[i]  = denix;
     denwN[i]  = denwNx;
     denwdT[i] = 0;
     tauc[i] = tauc1[i] = tauc2[i] = denwa[i] = denwTa[i] = denwTN[i] = 0; 
   }


   for ( i = 1; i <= nlev; i++ ) { 
      if ( tempk[i] >= TMax )
         LGL[i] = 1;
      else 
         LGL[i] = 0;  } 
 
   for ( i = 1; i <= nlev; i++ ) {  
     if ( tempk[i] >= TMax ) 
       continue; 

     tauc[i] = 1.0 - TempR[i]; 
     tauc1 [i] = xnthroot(tauc[i],3.0);
     tauc2 [i] = tauc1[i] * tauc1[i];
     denwa [i] = ( BW[1]+(BW[4]+BW[5] * xpow (tauc[i],9.0) ) * xpow (tauc[i],5.0)) * tauc1[i];

     f = xpow (tauc[i],35.0); /* NOTE - seem to need to be done on a separate line */ 
     denwN[i] = denwa[i] + ( BW[2]+(BW[3]+BW[6] * f ) * tauc[i] ) * tauc2[i];

     denw[i]  = dencr * ( 1.0 + denwN[i] );
     denwi[i] = 1.0 / denw[i];  

     f = xpow (tauc[i],9.0); 
     g = xpow (tauc[i],5.0); 
     denwTa[i] = (BW[1]+(16.0*BW[4]+43.0*BW[5] *  f ) * g) / ( 3.0 * tauc2[i]);
     f = xpow (tauc[i],35.0);
     denwTN[i] = denwTa[i] + (2.0 * BW[2] + ( 5.0 * BW[3] + 110.0 * BW[6] * f ) * tauc[i] ) / (3.0 * tauc1[i]);
    
     denwdT[i] = -dencr * denwTN[i] / tempcr;

  }  /* for */ 


       
}

#ifdef wowowo
%--------------------------------------------------------------------------
function [ LGL denw denwi denwdT denwN ] = calden2HMV( TempR,tempk,nlev )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Determine the Vertical Profiles of liquid water: 
%           density    -------------------------- ( denw ) [  kg/m^3  ]       
%           d(denw)/dT -------------------------- (denwdT) [kg/(m^3 K)] 

%__________________________________________________________________________
% Assign default values for these liquid water properties

global TMax denwx denix denwNx

denw(1:nlev)   = denwx;
denwi(1:nlev)  = denix;
denwN(1:nlev)  = denwNx;
denwdT(1:nlev) = 0;


%__________________________________________________________________________
% Calculate values when tempk < TMax [K]

LGL = tempk >= TMax;

%--------------------------------------------------------------------------
%          Density of Liquid Water: denw   = denw(tempk)   
%            The first derivative:  denwdT = denwdT(tempk) 
%--------------------------------------------------------------------------

global dencr tempcr BW

tauc        = 1-TempR(~LGL);
tauc1       = nthroot(tauc,3);
tauc2       = tauc1.*tauc1;
denwa       = (BW(1)+(BW(4)+BW(5)*tauc.^9).*tauc.^5).*tauc1;
denwN(~LGL) = denwa+(BW(2)+(BW(3)+BW(6)*tauc.^35).*tauc).*tauc2;
denw(~LGL)  = dencr*(1+denwN(~LGL));
denwi(~LGL) = 1./denw(~LGL);

denwTa = (BW(1)+(16*BW(4)+43*BW(5)*tauc.^9).*tauc.^5)./(3*tauc2);
denwTN = denwTa+(2*BW(2)+(5*BW(3)+110*BW(6)*tauc.^35).*tauc)./(3*tauc1);
denwdT(~LGL) = -dencr*denwTN/tempcr;

end

#endif 