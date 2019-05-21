#include <stdio.h>
#include <math.h> 
#include "BMSoil.h"
 
extern  double tempcr;
extern  double HIM[];   /* See Water_Vapor_Constants_2.cpp */ 
extern  double LIM[];    /* See Water_Vapor_Constants_2.cpp */ 

/*****************************************************************************************************
*
* Note-1: It's neater to just do calcs on local variables and then send all this
*         stuff out of here at the end 
*****************************************************************************************/
void  calmaxmulaHMV  (double TMax, double *muvx, double *muvTx, double *lamvx, double *lavTx, double *temRx )
{
int i; 
double Numr; 
double A1, A2, A3, A4, A5;
double muvD, NmuT; 
double muv, muvT, lamv, lavT, temR; //  double *lamvx, double *lavTx, double *temRx
double thcD, NlaT; 

  Numr = xsqrt (TMax / tempcr);
  temR = tempcr / TMax; 

// %__________________________________________________________________________
// % Dynamic Viscosity

  muvD = Vmult5 ( HIM[1], HIM[2], HIM[3], HIM[4], 0, 0, temR); 
  muv  = Numr / muvD;
  NmuT = Vmult5 (0, HIM[2], 2.0*HIM[3], 3.0*HIM[4], 0,0, temR); 
  muvT = (0.5 + NmuT / muvD) * muv / TMax;
 
//%__________________________________________________________________________
//% Thermal Conductivity
  thcD = Vmult5 (LIM[1], LIM[2], LIM[3], LIM[4], LIM[5], 0, temR); 
  lamv = Numr / thcD;
  NlaT = Vmult5(0, LIM[2], 2*LIM[3], 3*LIM[4], 4*LIM[5], 0, temR);
  lavT = ( 0.5 + NlaT / thcD ) * lamv / TMax;

  *muvx   = muv;
  *muvTx  = muvT; 
  *lamvx  = lamv; 
  *lavTx  = lavT; 
  *temRx  = temR;

 }

#ifdef wowwwwwww
%--------------------------------------------------------------------------
function [ muv muvT lamv lavT temR ] = calmaxmulaHMV( TMax )
%--------------------------------------------------------------------------
%__________________________________________________________________________
% Calculate the Vertical Profiles of water vapor thermophysical properties:
%     dynamic viscosity --------------------- (   mu  ) [  Pa s  ] 
%     thermal conductivity ------------------ ( zlamw ) [ W/m/K  ]
% and their first derivatives wrt temperature: muvT and lavT

global tempcr

Numr = sqrt(TMax/tempcr);
temR = tempcr/TMax; 

%__________________________________________________________________________
% Dynamic Viscosity

global HIM

muvD = Vmult5([HIM(1:4) 0 0],temR);
muv  = Numr/muvD;
NmuT = Vmult5([0 HIM(2) 2*HIM(3) 3*HIM(4) 0 0],temR);
muvT = (0.5+NmuT/muvD)*muv/TMax;

%__________________________________________________________________________
% Thermal Conductivity

global LIM

thcD = Vmult5([LIM(1:5) 0],temR);
lamv = Numr/thcD;
NlaT = Vmult5([0 LIM(2) 2*LIM(3) 3*LIM(4) 4*LIM(5) 0],temR);
lavT = (0.5+NlaT/thcD)*lamv/TMax;

end
#endif 