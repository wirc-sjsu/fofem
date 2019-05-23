#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

double  T0   [eC_MaxLev];
double  T1   [eC_MaxLev];
double  T2   [eC_MaxLev];
double  T3   [eC_MaxLev];
double  T4   [eC_MaxLev];
double  T5   [eC_MaxLev];
double  D2   [eC_MaxLev];
double  temN [eC_MaxLev];
double  mua  [eC_MaxLev];
double  mu1  [eC_MaxLev];
double  TT0  [eC_MaxLev];
double  TT1  [eC_MaxLev];
double  TT2  [eC_MaxLev];
double  TT3  [eC_MaxLev];
double  TT4  [eC_MaxLev];
double  TT5  [eC_MaxLev];
double  TTX  [eC_MaxLev];
double  TTA  [eC_MaxLev];
double  mub  [eC_MaxLev];
double  TTB  [eC_MaxLev];
double  L0   [eC_MaxLev];
double  L1   [eC_MaxLev];
double  L2   [eC_MaxLev];
double  L3   [eC_MaxLev];
double  L4   [eC_MaxLev];
double  lwa  [eC_MaxLev];
double  lw1  [eC_MaxLev];
double  LT0  [eC_MaxLev];
double  LT1  [eC_MaxLev];
double  LT2  [eC_MaxLev];
double  LT3  [eC_MaxLev];
double  LT4  [eC_MaxLev];
double  LTX  [eC_MaxLev];
double  LTA  [eC_MaxLev];
double  lwb  [eC_MaxLev];
double  LTB  [eC_MaxLev];



extern  double muwx,  zlmwx; 
extern double dencr;
extern double Hi[];
extern double Li[];
extern char gcr_FuncName[]; 
/****************************************************************************
*
*
***************************************************************************/
void  calmulaWHMV( 
    double muw[], double muwT[], double dmTm[], double lamw[], double lawT[], 
    double tempki[], bool LGL[], double denwN[], double temR[], 
    double muv[], double muvT[], double lamv[], double lavT[],
    double denwi[], double denwdT[], double nlev )
{
int i;

    strcpy (gcr_FuncName,"calmulaWHMV()"); 

/* Set defaults incase LGL condition isn't meet and no calc is done */
  for ( i = 1; i <= nlev; i++ ) {
    muw [i] = muwx;
    muwT[i] = 0;
    dmTm[i] = 0;
    lamw[i] = zlmwx;
    lawT[i] = 0; }

  for ( i = 1; i <= nlev; i++ ) {
    if ( LGL[i] == true ) 
      continue; 
    T0[i] = Vmult5 (Hi[1], Hi[5], Hi[10], Hi[15], Hi[17], 0, denwN[i] );
    T1[i] = Vmult5 (Hi[2], Hi[6], Hi[11], Hi[16],      0, 0, denwN[i] );
    T2[i] = Vmult5 (Hi[3], Hi[7], Hi[12],      0,      0, 0, denwN[i] );
    D2[i] = denwN[i] * denwN[i];
    T3[i] = Hi[8] * denwN[i] + Vmult5 (Hi[4], Hi[13], Hi[18], Hi[20], 0, 0, D2[i]);
    T4[i] = (Hi[14]+Hi[19]*denwN[i]*D2[i])*D2[i];
    T5[i] = Hi[9]*denwN[i]+Hi[21]*D2[i]*D2[i]*D2[i];
    temN[i] =  temR[i] -1.0;
    mua[i]  =  T0[i]+(T1[i]+(T2[i]+(T3[i]+(T4[i]+T5[i]*temN[i])*temN[i])*temN[i])*temN[i])*temN[i];

// crash 
    mu1[i]  =  xexp(  (1.0+denwN[i] ) * mua[i] );  

    muw[i] = muv[i]*mu1[i];  } /* for */

  for ( i = 1; i <= nlev; i++ ) {
    if ( LGL[i] == true ) 
      continue; 

    TT0[i] =  Vmult5(Hi[5], 2.0*Hi[10], 3.0*Hi[15], 4.0*Hi[17], 0, 0, denwN[i]);
    TT1[i] =  Vmult5(Hi[6], 2.0*Hi[11], 3.0*Hi[16],          0, 0, 0, denwN[i]);
    TT2[i] =  Vmult5(Hi[7], 2.0*Hi[12],          0,          0, 0, 0, denwN[i]);
    TT3[i] =  Hi[8] + (2.0*Hi[13]+(4.0*Hi[18]+6.0*Hi[20]*D2[i])*D2[i])*denwN[i];
    TT4[i] = (2.0*Hi[14]+5.0*Hi[19]*denwN[i]*D2[i])*denwN[i];
    TT5[i] =  Hi[9]+6.0*Hi[21]*denwN[i]*D2[i]*D2[i];
    TTX[i]  = TT0[i]+(TT1[i]+(TT2[i]+(TT3[i]+(TT4[i]+TT5[i]*temN[i])*temN[i])*temN[i])*temN[i])*temN[i];
    TTA[i]  = denwdT[i]*(mua[i]+TTX[i]*(1.0+denwN[i])) / dencr; 
    mub[i]  =  T1[i]+(2.0*T2[i]+(3.0*T3[i]+(4.0*T4[i]+5.0*T5[i]*temN[i])*temN[i])*temN[i])*temN[i];
    TTB[i]  =  mub[i]*(1.0+denwN[i])*temR[i]*tempki[i];
    muwT[i] = muvT[i]*mu1[i] + (TTA[i]-TTB[i])*muw[i]; }

//%--------------------------------------------------------------------------
//%           Total dK_H/dT term, including the water density term             
//%--------------------------------------------------------------------------
  for ( i = 1; i <= nlev; i++ )  {
     if ( LGL[i] == true ) 
      continue; 
    dmTm[i] = muwT[i] / muw[i] - denwdT[i] * denwi[i]; }

//%--------------------------------------------------------------------------
//%        Thermal Conductivity of Liquid Water: zlamw = zlamw(tempk)             
//%--------------------------------------------------------------------------

  for ( i = 1; i <= nlev; i++ )   {
    if ( LGL[i] == true ) 
      continue; 
    L0[i] = Vmult5(Li[1], Li[2], Li[3], Li[4], Li[5], Li[6], denwN[i]);
    L1[i] = Vmult5(Li[7], Li[8], Li[9], Li[10],Li[11],Li[12], denwN[i]);
    L2[i] = Vmult5(Li[13],Li[14],Li[15],Li[16],Li[17],Li[18], denwN[i]);
    L3[i] = Vmult5(Li[19],Li[20],Li[21],Li[22], 0, 0, denwN[i]); 
    L4[i] = Vmult5(Li[23],Li[24],Li[25],Li[26],Li[27],Li[28], denwN[i]);
    lwa[i] = L0[i]+(L1[i]+(L2[i]+(L3[i]+L4[i]*temN[i])*temN[i])*temN[i])*temN[i];
    lw1[i] = xexp((1.0+denwN[i])*lwa[i]);
    lamw[i] = lamv[i]*lw1[i]; }

  for ( i = 1; i <= nlev; i++ )   {
    if ( LGL[i] == true ) 
      continue; 
    LT0[i] = Vmult5 (Li[2], 2.0*Li[3], 3.0*Li[4], 4.0*Li[5], 5.0*Li[6], 0, denwN[i]);
    LT1[i] = Vmult5 (Li[8], 2.0*Li[9], 3.0*Li[10], 4.0*Li[11], 5.0*Li[12], 0, denwN[i]);
    LT2[i] = Vmult5 (Li[14], 2.0*Li[15], 3.0*Li[16], 4.0*Li[17], 5.0*Li[18], 0, denwN[i]);
    LT3[i] = Vmult5 (Li[20], 2.0*Li[21], 3.0*Li[22], 0, 0, 0,denwN[i]);
    LT4[i] = Vmult5 (Li[24], 2.0*Li[25], 3.0*Li[26], 4.0*Li[27], 5.0*Li[28], 0,denwN[i]);
    LTX[i] = LT0[i]+(LT1[i]+(LT2[i]+(LT3[i]+LT4[i]*temN[i])*temN[i])*temN[i])*temN[i];
    LTA[i] = denwdT[i]*(lwa[i]+LTX[i]*(1.0+denwN[i])) / dencr; 
    lwb[i] = L1[i]+(2.0*L2[i]+(3.0*L3[i]+4.0*L4[i]*temN[i])*temN[i])*temN[i];
    LTB[i] = lwb[i]*(1.0+denwN[i])*temR[i]*tempki[i];
    lawT[i] = lavT[i]*lw1[i] + (LTA[i]-LTB[i])*lamw[i]; }


    strcpy (gcr_FuncName,""); 
}

#ifdef wowowo
%--------------------------------------------------------------------------
function [ muw muwT dmTm lamw lawT ] = ... 
  calmulaWHMV( tempki,LGL,denwN,temR,muv,muvT,lamv,lavT,denwi,denwdT,nlev )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Determine the Vertical Profiles of liquid water properties: 
%   dynamic viscosity, thermal conductivity, and their first derivatives, 
%   all are functions of temperature: 
%   ---------------------------------- (  muw ) [  Pa s  ] 
%   ---------------------------------- ( dmTm ) [  K^-1  ] 
%   ---------------------------------- ( lamw ) [ W/m/K  ] 

global muwx zlmwx 

muw(1:nlev)  = muwx;
muwT(1:nlev) = 0;
dmTm(1:nlev) = 0;
lamw(1:nlev) = zlmwx;
lawT(1:nlev) = 0;

%--------------------------------------------------------------------------
%           Dynamic Viscosity of Liquid Water: muw = muw(tempk)             
%--------------------------------------------------------------------------

global dencr
global Hi

T0   =  Vmult5([Hi(1) Hi(5) Hi(10) Hi(15) Hi(17) 0],denwN(~LGL));
T1   =  Vmult5([Hi(2) Hi(6) Hi(11) Hi(16) 0 0],denwN(~LGL));
T2   =  Vmult5([Hi(3) Hi(7) Hi(12) 0 0 0],denwN(~LGL));
D2   =  denwN(~LGL).*denwN(~LGL);
T3   =  Hi(8)*denwN(~LGL) + Vmult5([Hi(4) Hi(13) Hi(18) Hi(20) 0 0],D2);
T4   = (Hi(14)+Hi(19)*denwN(~LGL).*D2).*D2;
T5   =  Hi(9)*denwN(~LGL)+Hi(21)*D2.*D2.*D2;
temN =  temR(~LGL)-1;
mua  =  T0+(T1+(T2+(T3+(T4+T5.*temN).*temN).*temN).*temN).*temN;
mu1  =  exp((1+denwN(~LGL)).*mua);
muw(~LGL) = muv(~LGL).*mu1;

TT0  =  Vmult5([Hi(5) 2*Hi(10) 3*Hi(15) 4*Hi(17) 0 0],denwN(~LGL));
TT1  =  Vmult5([Hi(6) 2*Hi(11) 3*Hi(16) 0 0 0],denwN(~LGL));
TT2  =  Vmult5([Hi(7) 2*Hi(12) 0 0 0 0],denwN(~LGL));
TT3  =  Hi(8) + (2*Hi(13)+(4*Hi(18)+6*Hi(20)*D2).*D2).*denwN(~LGL);
TT4  = (2*Hi(14)+5*Hi(19)*denwN(~LGL).*D2).*denwN(~LGL);
TT5  =  Hi(9)+6*Hi(21)*denwN(~LGL).*D2.*D2;
TTX  =  TT0+(TT1+(TT2+(TT3+(TT4+TT5.*temN).*temN).*temN).*temN).*temN;
TTA  =  denwdT(~LGL).*(mua+TTX.*(1+denwN(~LGL)))/dencr; 
mub  =  T1+(2*T2+(3*T3+(4*T4+5*T5.*temN).*temN).*temN).*temN;
TTB  =  mub.*(1+denwN(~LGL)).*temR(~LGL).*tempki(~LGL);
muwT(~LGL) = muvT(~LGL).*mu1 + (TTA-TTB).*muw(~LGL);

%--------------------------------------------------------------------------
%           Total dK_H/dT term, including the water density term             
%--------------------------------------------------------------------------
          
dmTm(~LGL) = muwT(~LGL)./muw(~LGL)-denwdT(~LGL).*denwi(~LGL);

%--------------------------------------------------------------------------
%        Thermal Conductivity of Liquid Water: zlamw = zlamw(tempk)             
%--------------------------------------------------------------------------

global Li

L0   = Vmult5(Li(1:6),denwN(~LGL));
L1   = Vmult5(Li(7:12),denwN(~LGL));
L2   = Vmult5(Li(13:18),denwN(~LGL));
L3   = Vmult5([Li(19:22) 0 0],denwN(~LGL)); 
L4   = Vmult5(Li(23:28),denwN(~LGL));
lwa  = L0+(L1+(L2+(L3+L4.*temN).*temN).*temN).*temN;
lw1  = exp((1+denwN(~LGL)).*lwa);
lamw(~LGL) = lamv(~LGL).*lw1;

LT0  = Vmult5([Li(2) 2*Li(3) 3*Li(4) 4*Li(5) 5*Li(6) 0],denwN(~LGL));
LT1  = Vmult5([Li(8) 2*Li(9) 3*Li(10) 4*Li(11) 5*Li(12) 0],denwN(~LGL));
LT2  = Vmult5([Li(14) 2*Li(15) 3*Li(16) 4*Li(17) 5*Li(18) 0],denwN(~LGL));
LT3  = Vmult5([Li(20) 2*Li(21) 3*Li(22) 0 0 0],denwN(~LGL));
LT4  = Vmult5([Li(24) 2*Li(25) 3*Li(26) 4*Li(27) 5*Li(28) 0],denwN(~LGL));
LTX  = LT0+(LT1+(LT2+(LT3+LT4.*temN).*temN).*temN).*temN;
LTA  = denwdT(~LGL).*(lwa+LTX.*(1+denwN(~LGL)))/dencr; 
lwb  = L1+(2*L2+(3*L3+4*L4.*temN).*temN).*temN;
LTB  = lwb.*(1+denwN(~LGL)).*temR(~LGL).*tempki(~LGL);

lawT(~LGL) = lavT(~LGL).*lw1 + (LTA-LTB).*lamw(~LGL);

end

#endif 

