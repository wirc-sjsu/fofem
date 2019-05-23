#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double tempcr; 
extern double HIM[];
extern double LIM[]; 

double Numr [eC_MaxLev];
double muvD [eC_MaxLev];
double NmuT [eC_MaxLev];
double thcD [eC_MaxLev];
double NlaT [eC_MaxLev];

/*****************************************************************************/
void  calmulaHMV(double TempR[], double temR[], double muv[], double muvT[], double lamv[], double lavT[],
            double tempk[], double tempki[], double nlevel )
{
int i;

  for ( i = 1; i <= nlevel; i++ ) {
    if ( tempcr != 0 ) 
       TempR[i] = tempk[i] / tempcr;
    else 
      TempR[i] = xZero("tempcr");
    Numr[i]  = xsqrt(TempR[i]);
    if ( TempR[i] != 0 ) 
      temR[i]  = 1.0 / TempR[i]; 
    else 
      temR[i] = xZero("TempR"); 
   } 

  for ( i = 1; i <= nlevel; i++ ) {
    muvD[i] = Vmult5( HIM[1], HIM[2], HIM[3], HIM[4], 0, 0, temR[i]);
    if ( muvD[i] != 0 ) 
       muv[i] = Numr[i] / muvD[i];
    else 
       muv[i] = xZero ("muvD");  
    NmuT[i] = Vmult5(0, HIM[2], 2.0*HIM[3], 3.0*HIM[4], 0, 0, temR[i]);
    if ( muvD != 0 ) 
       muvT[i] = (0.5 + NmuT[i] / muvD[i]) * muv[i] * tempki[i];
    else 
       muvT[i] = xZero ("muvD");   
    }

  for ( i = 1; i <= nlevel; i++ ) {
    thcD[i] = Vmult5(LIM[1], LIM[2], LIM[3], LIM[4], LIM[5], 0, temR[i]);
    if ( thcD[i] != 0 ) 
      lamv[i] = Numr[i] / thcD[i];
    else 
      lamv[i] = xZero("thcD"); 
    NlaT[i] = Vmult5(0, LIM[2], 2.0*LIM[3], 3.0*LIM[4], 4.0*LIM[5], 0, temR[i]);
    if ( thcD[i] != 0 ) 
      lavT[i] = (0.5 + NlaT[i] / thcD[i]) * lamv[i] * tempki[i];
    else 
       lavT[i] = xZero("thcD"); 
   }

}

#ifdef wowowow
%--------------------------------------------------------------------------
function [ TempR temR muv muvT lamv lavT ] = calmulaHMV( tempk,tempki )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Calculate the Vertical Profiles of water vapor thermophysical properties:
%     dynamic viscosity --------------------- (   mu  ) [  Pa s  ] 
%     thermal conductivity ------------------ ( zlamw ) [ W/m/K  ]
% and their first derivatives wrt temperature: muvT and lavT

global tempcr

TempR = tempk/tempcr;
Numr  = sqrt(TempR);
temR  = 1./TempR; 

%__________________________________________________________________________
% Dynamic Viscosity

global HIM

muvD = Vmult5([HIM(1:4) 0 0],temR);
muv  = Numr./muvD;
NmuT = Vmult5([0 HIM(2) 2*HIM(3) 3*HIM(4) 0 0],temR);
muvT = (0.5+NmuT./muvD).*muv.*tempki;

%__________________________________________________________________________
% Thermal Conductivity

global LIM

thcD = Vmult5([LIM(1:5) 0],temR);
lamv = Numr./thcD;
NlaT = Vmult5([0 LIM(2) 2*LIM(3) 3*LIM(4) 4*LIM(5) 0],temR);
lavT = (0.5+NlaT./thcD).*lamv.*tempki;

end


#endif 