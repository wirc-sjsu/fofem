#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

//%------------------------------------------------------------------
//%                         PsinTmpnT.m                            
//%
//%                  Initialize model constants         
//%------------------------------------------------------------------

//%__________________________________________________________________
//% The function used in this model is 
//%     PsiT = psiTx - (psiTx-1)*exp(-apsiT*(temp-tempai))

double  psiTx, apsiT, apsiTa;

extern double temp0;


void PsinTmpnT (double tempai)
{
psiTx  = 1.6;
apsiT  = 3e-03;
apsiTa = xlog(psiTx)  *  (tempai+temp0);
}