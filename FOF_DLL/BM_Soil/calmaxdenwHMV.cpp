#include <stdio.h>
#include <math.h> 
#include "BMSoil.h"

//global dencr tempcr BW 

extern double dencr;   /* See - Water_Vapor_Constants_0.cpp */
extern double tempcr;  /* See - Water_Vapor_Constants_0.cpp */
extern double BW[];    /* Liquid_Water_Constants_2.cpp */

void calmaxdenwHMV (double TMax, double *denwx, double *denwix, double *denwNx, double *denwdTx )
{
double TempR; 
double tauc,tauc1,tauc2,denwa; 
double denwN, denw, denwi,denwTa,denwTN,denwdT;

  TempR  = TMax / tempcr;
  tauc   = 1.0 - TempR;
  tauc1  = xnthroot(tauc,3);
  tauc2  = tauc1 * tauc1;
  denwa  = ( BW[1] + (BW[4] + BW[5] * xpow((double)tauc,9.0) )* xpow((double)tauc,5.0) ) * tauc1;
  denwN  = denwa + ( BW[2] + (BW[3] + BW[6] * xpow ((double)tauc,35.0) ) * tauc ) * tauc2;
  denw   = dencr * ( 1.0 + denwN );
  denwi  = 1.0 / denw; 

  denwTa =  ( BW[1] + (16.0 * BW[4] + 43.0 * BW[5] * xpow ((double)tauc,9.0) ) * xpow((double)tauc,5.0) ) / (3.0 * tauc2);
  denwTN =  denwTa + ( 2.0 * BW[2]+ (5.0 * BW[3] + 110.0 * BW[6] * xpow((double)tauc,35.0) ) * tauc) / ( 3.0 *tauc1 );
  denwdT = -dencr * denwTN / tempcr;

 *denwx = denw; 
 *denwix = denwi; 
 *denwNx = denwN; 
 *denwdTx = denwdT; 

}

#ifdef XXXXXXXXXX
%--------------------------------------------------------------------------
function [ denw denwi denwN denwdT ] = calmaxdenwHMV( TMax )
%--------------------------------------------------------------------------

global dencr tempcr BW 

TempR  = TMax/tempcr;

tauc   = 1-TempR;
tauc1  = nthroot(tauc,3);
tauc2  = tauc1*tauc1;
denwa  = (BW(1)+(BW(4)+BW(5)*tauc^9)*tauc^5)*tauc1;
denwN  = denwa+(BW(2)+(BW(3)+BW(6)*tauc^35)*tauc)*tauc2;
denw   = dencr*(1+denwN);
denwi  = 1/denw;

denwTa =  (BW(1)+(16*BW(4)+43*BW(5)*tauc^9)*tauc^5)/(3*tauc2);
denwTN =  denwTa+(2*BW(2)+(5*BW(3)+110*BW(6)*tauc^35)*tauc)/(3*tauc1);
denwdT = -dencr*denwTN/tempcr;

end
#endif 