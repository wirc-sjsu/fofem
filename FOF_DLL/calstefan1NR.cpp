#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double pres; 

void calstefan1NR(double stefan[], double stefpT[], double stefpr[],      
                  double vapres[], double tempki[], double rhov[], double nlevel )
{
int i;
double vpN;

   for ( i = 1; i <= nlevel; i++ ) {
     vpN = vapres[i] / pres;
     stefan[i] = 1.0 + vpN;
     stefpT[i] = vpN * tempki[i];
     stefpr[i] = vpN / rhov[i];  } 
}


#ifdef wowow
%--------------------------------------------------------------------------
function [ stefan stefpT stefpr ] = calstefan1NR( vapres,tempki,rhov )
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Newest version of the Stefan factor - (WJM 8/2014)    
% Also calculate the first derivatives of the Stefan factor  
%      wrt temperature (stefpT) and vapor density (stefpr)                                          

global pres 

vpN    = vapres/pres;

stefan = 1+vpN;
stefpT = vpN.*tempki;
stefpr = vpN./rhov;

end

#endif 