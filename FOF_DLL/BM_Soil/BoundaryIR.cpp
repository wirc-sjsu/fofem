/********************************************************************************
* Name: BoundaryIR.cpp
* Desc: Added 12/2/17
* 
*
********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double nsteps;

extern double force []; 
extern double forIR []; 

/*--------------------------------------------------------------------*/
void BoundaryIR()
{
int j, jend;
double jmax; 

   jend  = nsteps+1;                     // jend  = nsteps+1;
   for ( j = 1; j <= nsteps; j++ ) {     // for j = 1:nsteps
     if ( force[j] < 0 ) {               //      if force(j) < 0
       jend = j;                         // jend     = j;
       break; }
   }  /* for j */ 

   if ( jend < nsteps ) {                 //  if jend < nsteps
      for ( j = jend; j <= nsteps; j++ )  //  force(jend:nsteps) = 0;
        force [j] = 0;                
   } /* if */ 


   for ( j = 1; j <= nsteps; j++ ) {       // for j = 1:nsteps
     if ( (force[j+1] - force[j]) < 0 ) {  // if force(j+1)-force(j) < 0
        jmax = j;
        break; }
    } /* for */ 

  for ( j = 1; j <= nsteps; j++ )    // forIR = force;
    forIR[j] = force[j]; 
   
  for ( j = jmax; j <= nsteps; j++ )    // forIR(jmax:nsteps) = 1;
    forIR[j] = 1.0; 


}



#ifdef ZZZZZZZZZZ
%--------------------------------------------------------------------------
%                           BoundaryIR.m
%
%   Set the upper boundary condition for controling IR cooling
%
%   To be used with Boundary___.m
%
%--------------------------------------------------------------------------

jend  = nsteps+1;
for j = 1:nsteps
if force(j) < 0
   jend     = j;
   break
end
end
if jend < nsteps
force(jend:nsteps) = 0;
end

for j = 1:nsteps
if force(j+1)-force(j) < 0
   jmax = j;
   break
end
end

forIR = force;
forIR(jmax:nsteps) = 1;

#endif

