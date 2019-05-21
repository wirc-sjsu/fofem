
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"


extern double  tempcr, tempci;

extern double Qim0, Qim[], Zi[];

/***********************************************************************/
void calepssurfHMV (double *sdialc, double *ddiaT, double *stensn, double *dstensdT, 
                    double *tempk, double *tempki, double *TempR, double nlev )
{
int i,N; 
double tauc, tauc1,tauc31;
double term1, term2,term3, term4, aux;  
   
   N = 0; 
   for ( i = 1; i <= nlev; i++ ) {
    if ( tempk[i] >= tempcr ) { 
      sdialc[i] = 0;
      ddiaT[i] = 0; 
      stensn[i] = 0; 
      dstensdT[i] = 0; 
      N++; 
      continue; }

    tauc   = 1.0 - TempR[i];
    tauc1  = xnthroot(tauc,3.0);
    tauc31 = tauc * tauc1;

    term1     = (Qim[1]+Qim[2]*tauc1) * tauc1 + (Qim[3]+Qim[6]*tauc)*tauc;
    term2     =  Qim[4]+Qim[5]*tauc1 + Qim[7]*tauc+Qim[8]*tauc31;
    sdialc[i] =  Qim0*(1.0 + term1 + term2 * tauc31);
    term3     = (2.0*Qim[2]+3.0*Qim[3]*tauc1) * tauc1 + (4.0*Qim[4]+7.0*Qim[7]*tauc)*tauc;
    term4     =  5.0*Qim[5]+6.0*Qim[6]*tauc1 + 8.0*Qim[8]*tauc;
    ddiaT[i]  = -Qim0*(Qim[1] + term3 + term4*tauc31)*tempci;

    aux       =  Zi[1] * xpow (tauc, Zi[3]);   // ? ? 
    stensn[i]   =  aux*(1.0+Zi[2]*tauc);
    dstensdT[i] = -Zi[3]*stensn[i]*tempki[i] - Zi[2]*aux*tempci;
  }

 // if ( N > 0 ) 
 //   LMS_Mess ( "Logic - Bad Compare - zero filled arrays",  "Function:  calepssurfHMV()"); 
  
}


#ifdef wowow
%--------------------------------------------------------------------------
function [ sdialc ddiaT stensn dstensdT ] = ... 
                               calepssurfHMV( tempk,tempki,TempR,nlev )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Calculate the Vertical Profiles of liquid water thermophysical properties:
%     static dialectric constant --------------------- ( sdialc ) [  -  ] 
%     surface tension -------------------------------- ( stensn ) [ N/m ] 
% And their first derivatives wrt tempk
              
%__________________________________________________________________________
% Calculate values when tempk < tempcr [K]

global tempcr tempci

lgl    = tempk >= tempcr;
tauc   = 1 - TempR(~lgl);
tauc1  = nthroot(tauc,3);
tauc31 = tauc.*tauc1;

%__________________________________________________________________________
% Static Diaelctric constant

global Qim0 Qim
       
term1        = (Qim(1)+Qim(2)*tauc1).*tauc1 + (Qim(3)+Qim(6)*tauc).*tauc;
term2        =  Qim(4)+Qim(5)*tauc1 + Qim(7)*tauc+Qim(8)*tauc31;
sdialc(~lgl) =  Qim0*(1 + term1 + term2.*tauc31);
term3        = (2*Qim(2)+3*Qim(3)*tauc1).*tauc1 + (4*Qim(4)+7*Qim(7)*tauc).*tauc;
term4        =  5*Qim(5)+6*Qim(6)*tauc1 + 8*Qim(8)*tauc;
ddiaT(~lgl)  = -Qim0*(Qim(1) + term3 + term4.*tauc31)*tempci;

%__________________________________________________________________________
% Surface Tension 

global Zi 

aux            =  Zi(1)*tauc.^Zi(3);
stensn(~lgl)   =  aux.*(1+Zi(2)*tauc);
dstensdT(~lgl) = -Zi(3)*stensn(~lgl).*tempki(~lgl) - Zi(2)*aux*tempci;

end
#endif 