#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double temp0; 

void calcp0HMV (double *cp0, double *dcpT,
                double x10[], double sqtrm, double sqqtr,
                double sqtri, double sqqti, double tempr )
{
int i; 
double cp1,cp2,cp3,cp4;
double dcp1, dcp2, dcp3;

    cp1 = (x10[2] * sqtrm + x10[3] * sqqtr) * tempr;
    cp2 = ((x10[4] * sqqtr + x10[5]) * sqtrm + x10[6] * sqqtr);
    cp3 = (x10[7] * sqqti + (x10[8] + x10[9] * sqqti) * sqtri);
    cp4 = x10[1] + x10[10] / tempr;
    *cp0 = cp1 + cp2 + cp3 + cp4;


    dcp1 = -(1.5*x10[2] * sqtrm + 1.25 * x10[3] * sqqtr) * tempr;
    dcp2 = -((0.75*x10[4] * sqqtr + 0.5 * x10[5]) * sqtrm + 0.25 * x10[6] * sqqtr );
    dcp3 =  (0.25*x10[7] * sqqti + (0.5 * x10[8] + 0.75 * x10[9] * sqqti) * sqtri);
    *dcpT = ((dcp1+dcp2+dcp3) * tempr + x10[10]) / temp0;
 
}







#ifdef wowow

void calcp0HMV (double *cp0, double *dcpT,
                double *x10, double *sqtrm, double *sqqtr,
                double *sqtri, double *sqqti, double *tempr, double nlevel )
{
int i; 
double cp1,cp2,cp3,cp4;
double dcp1, dcp2, dcp3;

  for ( i = 1; i <= nlevel; i++ ) {
    cp1 = (x10[2] * sqtrm[i] + x10[3] * sqqtr[i]) * tempr[i];
    cp2 = ((x10[4] * sqqtr[i] + x10[5]) * sqtrm[i] + x10[6]* sqqtr[i]);
    cp3 = (x10[7] * sqqti[i] + (x10[8] + x10[9] * sqqti[i]) * sqtri[i]);
    cp4 = x10[1] + x10[10] / tempr[i];
    cp0[i] = cp1 + cp2 + cp3 + cp4;


    dcp1 = -(1.5*x10[2] * sqtrm[i] + 1.25 * x10[3] * sqqtr[i]) * tempr[i];
    dcp2 = -((0.75*x10[4] * sqqtr[i] + 0.5 * x10[5]) * sqtrm[i] + 0.25 * x10[6] * sqqtr[i] );
    dcp3 =  (0.25*x10[7] * sqqti[i] + (0.5 * x10[8] + 0.75 * x10[9] * sqqti[i]) * sqtri[i]);
    dcpT = ((dcp1+dcp2+dcp3) * tempr[i] + x10[10]) / temp0[i];
  } /* for */ 
}




%--------------------------------------------------------------------------
function [ cp0 dcpT ] = calcp0HMV(x10,sqtrm,sqqtr,sqtri,sqqti,tempr )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Generic function for computaion of specific heat as a funtion of
%   temperature and its first derivative wrt temperature 

global temp0

cp1  = (x10(2)*sqtrm+x10(3)*sqqtr).*tempr;
cp2  = ((x10(4)*sqqtr+x10(5)).*sqtrm+x10(6)*sqqtr);
cp3  = (x10(7)*sqqti+(x10(8)+x10(9)*sqqti).*sqtri);
cp4  = x10(1)+x10(10)./tempr;

cp0  = cp1 + cp2 + cp3 + cp4;

dcp1 = -(1.5*x10(2)*sqtrm+1.25*x10(3)*sqqtr).*tempr;
dcp2 = -((0.75*x10(4)*sqqtr+0.5*x10(5)).*sqtrm+0.25*x10(6)*sqqtr);
dcp3 =  (0.25*x10(7)*sqqti+(0.5*x10(8)+0.75*x10(9)*sqqti).*sqtri);

dcpT = ((dcp1+dcp2+dcp3).*tempr+x10(10))/temp0;

end

#endif 