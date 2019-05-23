#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double NB, CB, NAWX, Swc[], aux4c[];

void  calAwa( double Awa[], double dAwadt[], double dAwadp[],
              double Sw[],  double por[],    double thpsin[],
              int LGL[], double nlevel )
{
int i; 
double aux1, aux2, aux3, aux4; 
   for ( i = 1; i <= nlevel; i++ ) {
     aux1       = Sw[i] * (1.0 - Sw[i]);
     aux2       = xpow( (1.0 - Sw[i]), (NB - 1.0) );
     aux3       = CB * xnthroot(aux1,NAWX);
     aux4       = aux4c[i];
     if ( Swc[i] <= Sw[i] )         // lgl = Swc > Sw;
       aux4 = aux3 * (1.0 - 2.0 * Sw[i]) / (NAWX * aux1);
     if ( LGL[i] == 1 ) 
        aux4 = aux4;

     Awa[i] = aux1 * aux2 + aux3;
     dAwadt[i] = ((1.0 - (NB+1) * Sw[i]) * aux2 + aux4) / por[i];
     dAwadp[i] = dAwadt[i] * thpsin[i];
   } 
}

#ifdef owowo
%--------------------------------------------------------------------------
function [ Awa dAwadt dAwadp ] = calAwa( Sw,por,thpsin,LGL )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Calculate profile of the volume specific source area, also known  
%  as the volume-normalized air water interfacial area ----------- (Awa)  
%  and its first derivative wrt psin ----------------------------- (dAwadp) 

global NB CB NAWX Swc aux4c

aux1       = Sw.*(1-Sw);
aux2       = (1-Sw).^(NB-1);
aux3       = CB*nthroot(aux1,NAWX);
aux4       = aux4c;
lgl        = Swc > Sw;
aux4(~lgl) = aux3(~lgl).*(1-2*Sw(~lgl))./(NAWX*aux1(~lgl));
aux4       = ~LGL.*aux4;

Awa        = aux1.*aux2+aux3;
dAwadt     = ((1-(NB+1)*Sw).*aux2+aux4)./por;
dAwadp     = dAwadt.*thpsin;

end
#endif 