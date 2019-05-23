
#include <stdio.h>
#include <math.h> 
#include "BMSoil.h"


extern double dencr;
extern double Hi[]; 
extern double Li[];


void calmaxmulaWHMV (double tempk, double denwN, double temR, double muv, double muvT, double lamv, double lavT, double denwi, double denwdT, 
                     double *_muw, double *_muwT, double *_dmTm, double *_lamw, double  *_lawT  ) 
{
double T0,T1,T2,D2,T3,T4,T5;
double temN,mua,mu1, mub; 
double muw, muwT, dmTm, lamw, lawT;
double TT0,TT1,TT2,TT3,TT4,TT5,TTX,TTA,TTB;
double L0,L1,L2,L3,L4;
double lwa, lw1, lwb;
double LT0,LT1,LT2,LT3,LT4,LTX,LTA,LTB;

T0   =  Vmult5 ( Hi[1], Hi[5], Hi[10], Hi[15], Hi[17], 0, denwN);
T1   =  Vmult5 ( Hi[2], Hi[6], Hi[11], Hi[16], 0, 0, denwN);
T2   =  Vmult5 ( Hi[3], Hi[7], Hi[12], 0, 0, 0, denwN);
D2   =  denwN * denwN;
T3   =  Hi[8] * denwN + Vmult5(Hi[4], Hi[13], Hi[18], Hi[20], 0, 0, D2);
T4   =  (Hi[14] + Hi[19] * denwN * D2) * D2;
T5   =  Hi[9] * denwN + Hi[21] * D2 * D2 * D2;
temN =  temR - 1.0;
mua  =  T0 + (T1 + (T2 + (T3 + (T4 + T5 * temN) * temN) * temN) * temN) * temN;
mu1  =  xexp ( (1.0 + denwN) * mua );
muw  =  muv * mu1;

TT0  =  Vmult5 ( Hi[5],  2.0*Hi[10], 3.0*Hi[15], 4.0*Hi[17], 0, 0, denwN);
TT1  =  Vmult5 ( Hi[6], 2.0*Hi[11], 3.0*Hi[16], 0, 0, 0, denwN);
TT2  =  Vmult5 ( Hi[7], 2.0*Hi[12], 0, 0, 0, 0, denwN);
TT3  =  Hi[8] + ( 2.0*Hi[13] + (4.0*Hi[18] + 6.0*Hi[20] * D2) * D2) * denwN;
TT4  = ( 2.0*Hi[14] + 5.0*Hi[19] * denwN * D2) * denwN;
TT5  =  Hi[9] + 6.0*Hi[21] * denwN * D2 * D2;
TTX  =  TT0 + (TT1+(TT2+(TT3+(TT4+TT5*temN)*temN)*temN)*temN)*temN;
TTA  =  denwdT * (mua+TTX*(1.0+denwN))/dencr; 
mub  =  T1+(2*T2+(3*T3+(4*T4+5*T5*temN)*temN)*temN)*temN;
TTB  =  mub * (1+denwN)*temR/tempk;
muwT = muvT * mu1 + (TTA - TTB) * muw;

dmTm = muwT / muw - denwdT * denwi;

L0   = Vmult5 (Li[1], Li[2] ,Li[3], Li[4],  Li[5], Li[6], denwN);
L1   = Vmult5 (Li[7], Li[8], Li[9], Li[10],Li[11],Li[12], denwN);
L2   = Vmult5 (Li[13],Li[14],Li[15],Li[16],Li[17],Li[18], denwN);
L3   = Vmult5 (Li[19],Li[20],Li[21],Li[22],    0,     0,  denwN); 
L4   = Vmult5 (Li[23],Li[24],Li[25],Li[26],Li[27],Li[28], denwN);
lwa  = L0 + ( L1+(L2+(L3+L4*temN)*temN)*temN ) * temN;
lw1  = xexp ((1.0 + denwN) * lwa);
lamw = lamv * lw1;

LT0  = Vmult5 (  Li[2],  2.0*Li[3],  3.0*Li[4],  4.0*Li[5],  5.0*Li[6],  0, denwN);
LT1  = Vmult5 (  Li[8],  2.0*Li[9],  3.0*Li[10], 4.0*Li[11], 5.0*Li[12], 0, denwN);
LT2  = Vmult5 (  Li[14], 2.0*Li[15], 3.0*Li[16], 4.0*Li[17], 5.0*Li[18], 0, denwN);
LT3  = Vmult5 (  Li[20], 2.0*Li[21], 3.0*Li[22],          0,          0, 0, denwN);
LT4  = Vmult5 (  Li[24], 2.0*Li[25], 3.0*Li[26], 4.0*Li[27], 5.0*Li[28], 0, denwN);
LTX  = LT0 + ( LT1+(LT2+(LT3+LT4*temN)*temN)*temN)*temN;
LTA  = denwdT * ( lwa + LTX * (1.0 + denwN) ) / dencr; 

lwb  = L1+(2*L2+(3*L3+4*L4*temN)*temN)*temN;

LTB  = lwb*(1.0+denwN)*temR/tempk;

lawT = lavT*lw1 + (LTA-LTB)*lamw;

/* Send everything out now */ 
  *_muw  = muw; 
  *_muwT = muwT; 
  *_dmTm = dmTm;
  *_lamw = lamw; 
  *_lawT = lawT; 
}

#ifdef XXXXXXXX
%--------------------------------------------------------------------------
function [ muw muwT dmTm lamw lawT ] = ... 
           calmaxmulaWHMV( tempk,denwN,temR,muv,muvT,lamv,lavT,denwi,denwdT )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Determine the Vertical Profiles of liquid water properties: 
%   dynamic viscosity, thermal conductivity, and their first derivatives, 
%   all are functions of temperature: 
%   ---------------------------------- (  muw ) [  Pa s  ] 
%   ---------------------------------- ( dmTm ) [  K^-1  ] 
%   ---------------------------------- ( lamw ) [ W/m/K  ] 

%--------------------------------------------------------------------------
%           Dynamic Viscosity of Liquid Water: muw = muw(tempk)             
%--------------------------------------------------------------------------

global dencr
global Hi

T0   =  Vmult5([Hi(1) Hi(5) Hi(10) Hi(15) Hi(17) 0],denwN);
T1   =  Vmult5([Hi(2) Hi(6) Hi(11) Hi(16) 0 0],denwN);
T2   =  Vmult5([Hi(3) Hi(7) Hi(12) 0 0 0],denwN);
D2   =  denwN*denwN;
T3   =  Hi(8)*denwN + Vmult5([Hi(4) Hi(13) Hi(18) Hi(20) 0 0],D2);
T4   = (Hi(14)+Hi(19)*denwN*D2)*D2;
T5   =  Hi(9)*denwN+Hi(21)*D2*D2*D2;
temN =  temR-1;
mua  =  T0+(T1+(T2+(T3+(T4+T5*temN)*temN)*temN)*temN)*temN;
mu1  =  exp((1+denwN)*mua);
muw  =  muv*mu1;

TT0  =  Vmult5([Hi(5) 2*Hi(10) 3*Hi(15) 4*Hi(17) 0 0],denwN);
TT1  =  Vmult5([Hi(6) 2*Hi(11) 3*Hi(16) 0 0 0],denwN);
TT2  =  Vmult5([Hi(7) 2*Hi(12) 0 0 0 0],denwN);
TT3  =  Hi(8) + (2*Hi(13)+(4*Hi(18)+6*Hi(20)*D2)*D2)*denwN;
TT4  = (2*Hi(14)+5*Hi(19)*denwN*D2)*denwN;
TT5  =  Hi(9)+6*Hi(21)*denwN*D2*D2;
TTX  =  TT0+(TT1+(TT2+(TT3+(TT4+TT5*temN)*temN)*temN)*temN)*temN;
TTA  =  denwdT.*(mua+TTX*(1+denwN))/dencr; 
mub  =  T1+(2*T2+(3*T3+(4*T4+5*T5.*temN)*temN)*temN)*temN;
TTB  =  mub*(1+denwN)*temR/tempk;

muwT = muvT*mu1 + (TTA-TTB)*muw;

%--------------------------------------------------------------------------
%           Total dK_H/dT term, including the water density term             
%--------------------------------------------------------------------------
          
dmTm = muwT/muw-denwdT*denwi;

%--------------------------------------------------------------------------
%        Thermal Conductivity of Liquid Water: zlamw = zlamw(tempk)             
%--------------------------------------------------------------------------

global Li

L0   = Vmult5(Li(1:6),denwN);
L1   = Vmult5(Li(7:12),denwN);
L2   = Vmult5(Li(13:18),denwN);
L3   = Vmult5([Li(19:22) 0 0],denwN); 
L4   = Vmult5(Li(23:28),denwN);
lwa  = L0+(L1+(L2+(L3+L4*temN)*temN)*temN)*temN;
lw1  = exp((1+denwN)*lwa);
lamw = lamv*lw1;

LT0  = Vmult5([Li(2) 2*Li(3) 3*Li(4) 4*Li(5) 5*Li(6) 0],denwN);
LT1  = Vmult5([Li(8) 2*Li(9) 3*Li(10) 4*Li(11) 5*Li(12) 0],denwN);
LT2  = Vmult5([Li(14) 2*Li(15) 3*Li(16) 4*Li(17) 5*Li(18) 0],denwN);
LT3  = Vmult5([Li(20) 2*Li(21) 3*Li(22) 0 0 0],denwN);
LT4  = Vmult5([Li(24) 2*Li(25) 3*Li(26) 4*Li(27) 5*Li(28) 0],denwN);
LTX  = LT0+(LT1+(LT2+(LT3+LT4*temN)*temN)*temN)*temN;
LTA  = denwdT*(lwa+LTX*(1+denwN))/dencr; 
lwb  = L1+(2*L2+(3*L3+4*L4*temN)*temN)*temN;
LTB  = lwb*(1+denwN)*temR/tempk;

lawT = lavT*lw1 + (LTA-LTB)*lamw;

end

#endif 