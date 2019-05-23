#include <stdio.h>
#include <string.h>
#include <math.h> 
#include <float.h> 
#include <setjmp.h> 
\
#include "BMsoil.h"
//%--------------------------------------------------------------------------
//%                             SolveHMV.m                                                        
//%
//%                 Crank-Nicolson approach for solving the         
//%         soil heat flow equation with Neumann Boundary Conditions
//%
//%--------------------------------------------------------------------------
//%__________________________________________________________________________
//% Declare Ai, Bi, Ci, Ap, Bp, Cp, Ri, di, and xi as cells
//% These cells are to be used in the Generalized Thomas algorithm
// NOTE - Not implementing these for now, unless we do Thomas - see above 
// Ai = cell(nlevel,1);
// Bi = cell(nlevel,1);
// Ci = cell(nlevel,1);
// Ap = cell(nlevel,1);
// Bp = cell(nlevel,1);
// Cp = cell(nlevel,1);
// Ri = cell(nlevel,1);
// di = cell(nlevel,1);
// xi = cell(nlevel,1);
// %__________________________________________________________________________
// % Solve the heat flow equation using either the Crank-Nicolson 
// %     finite difference scheme or the Newton-Raphson method  

extern int CRANK ;
extern double nsteps; 
extern double temp[];
extern double nlevel; 
double jstep; 
extern double temp1[]; 
extern double denw[]; 
extern double psin[]; 
extern double denlv[];
extern double Gama[];
extern double ktcon[];
extern double dKHdT[];
extern double thpsin[];

extern double  gf_DivErr; 

/******************************************************************
* Name: BMI_Init
* Desc: Init the input struct for BM simulation
******************************************************************/
void BMI_Init (d_BMI *b)
{
   strcpy (b->cr_FirTyp,""); 
   strcpy ( b->cr_TemMoi,"");        
   strcpy ( b->cr_ErrMes,""); 
}

/********************************************************************
* Name: SolveHMV
* Desc: loops on each time step 
*  Ret: 1 ok, 
*       0 error occured 
*      -1 Exception cause Long Jump to occured - probably cause by
*          a bad math function - see xexp(), xpow(), etc  
*********************************************************************/  
int  SolveHMV (char  cr_ErrMes[])
{
int i,j ;
double f,g;
unsigned int current;

   strcpy (cr_ErrMes,""); 

/* Will jump directly back to here if we hit a math error, see my functions  */
/*  xexp(), xpow(), etc. . . */  
//extern jmp_buf g_env; 
//extern char gcr_JumpMess[]; 
//extern char gcr_FuncName[]; 
  i = 0; 
//  i = setjmp(g_env);
//  if ( i > 0 ) {
//    sprintf (cr_ErrMes, "JumpMess = %s, Func: %s\n",gcr_JumpMess,gcr_FuncName); 
 //   return -1;  } 

  if ( CRANK == 1 ) {
    for ( jstep = 1; jstep <= nsteps; jstep++ )  { // for jstep = 1:nsteps;

      CrankNicolson((int)jstep, cr_ErrMes);             // CrankNicolson
    } /* for i */ 
  }  /* if CRANK == 1 */        
    
          
Ret:
  return 1; 
}


// NOTE for now, not implementing NewtonRaphsonHMV
// else 
//   nsteps = 800;
//   nSolve(nsteps) = zeros; 
//   for jstep = 1:nsteps;
//       jstep  
//   NewtonRaphsonHMV
//   end
// end



#ifdef sssss
%nsteps = 800;
%csteps = 240;
%for jstep = 1:csteps;
%    jstep
%    CrankNicolson
%end
%    nSolve(nsteps) = zeros; 
%for jstep = csteps+1:nsteps;
%    jstep  
%NewtonRaphsonHMV
%end

%__________________________________________________________________________
% Calculate dtemp/dz for each time step 
% Calculate dpsin/dz for each time step
% Calculate the profile of heat and vapor fluxes (W/m^2)

% dtempdz = calgrad2(tempfeN,delz,nlevel,nsteps);
% dpsindz = calgrad2(psinf,delz,nlevel,nsteps);
% drhovdz = calgrad2(rhovf,delz,nlevel,nsteps);

% fluxGeN = -tconf.*dtempdz;
#endif 