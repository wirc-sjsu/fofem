//%--------------------------------------------------------------------------
//%                           BoundaryIR_T1.m
//%
//%   Set the upper boundary condition for controling IR cooling
//%
//%   To be used with Boundary_BFD.m
//%
//%--------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double nsteps; 
extern double forIR[]; 

void BoundaryIR_T1 (double  force[])
{
int j; 

 for ( j = 1; j <= nsteps; j++ )    // forIR = force;
    forIR[j] = force[j]; 


}