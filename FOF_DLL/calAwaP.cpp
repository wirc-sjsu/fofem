#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double CB, CB, NAWX, Swc[], aux4c[];


void  calAwaP( double Awa[], double dAwadt[], double dAwadp[],
              double Sw[],  double por[],    double thpsin[],
              int LGL[], double nlevel )
{
int i; 
double aux1; 

   for ( i = 1; i <= nlevel; i++ ) {
     aux1   = Sw[i] * (1.0 - Sw[i]);
     Awa[i] = CB * nthroot(aux1,NAWX);
     dAwadt[i] = aux4c[i];

     if ( Swc[i] <= Sw[i] )    //   lgl  = Swc > Sw;
        dAwadt[i] = Awa[i] * ( 1.0 - 2.0 * Sw[i]) / (NAWX * aux1);

     if ( LGL[i] == 0 ) 
        dAwadt[i] = dAwadt[i] / por[i];

     dAwadp[i] = dAwadt[i] * thpsin[i];

   } /* for i */ 

}

#ifdef wowowow
%--------------------------------------------------------------------------
function [ Awa dAwadt dAwadp ] = calAwaP( Sw,por,thpsin,LGL )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Calculate profile of the volume specific source area, also known  
%  as the volume-normalized air water interfacial area ----------- (Awa)  
%  and its first derivative wrt psin ----------------------------- (dAwadp) 
% NOTE: This is the parabolic form of the Awa function

global CB NAWX Swc aux4c

aux1         = Sw.*(1-Sw);
Awa          = CB*nthroot(aux1,NAWX);
dAwadt       = aux4c;
lgl          = Swc > Sw;
dAwadt(~lgl) = Awa(~lgl).*(1-2*Sw(~lgl))./(NAWX*aux1(~lgl));

dAwadt       = ~LGL.*dAwadt./por;
dAwadp       = dAwadt.*thpsin;

end

#endif 