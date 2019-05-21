
// New Update file 10-18-17
//  see Model_Param-Old.cpp

#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

double nlevel; 
double delt; 
double nsteps; 
double nl; 
double difcoef; 
double delz ;
double delvel; 
double z [eC_MaxLev]; 
double time [eC_nsteps]; 

extern int CRANK;
extern double nmin; 

void linspace (double z[], double x1, double x2, double n );

double ndpthT; 
double difcoeF; 
extern double difcoef; 

double Enconv;
double Rconv;
double Convcr;

extern int ECONVG; 

double depth; 



/***************************************************************
%--------------------------------------------------------------------------
%                              Model_Param.m                                                                
%
%                       Initialize model parameters         
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Set up depth and time arrays and parameters for the model 
% simulation of coupled soil heat, moisture, and vapor flow
%        depth, delz, and z [m] 
%        delt and time [s] 
%        nmin = number of minutes of simulation


* Note-1: make sure we don't exceed the array sizes that are 
*         used to hold time step info in simulation
****************************************************************/
int  Model_Param(d_BMI *bmi)
{
int i; 

double f; 

/* NOTE NOTE - bmi has the GUI inputs for Massman model */
/* I was using the delt (time step) from the GUI, took */
/* it off the GUI - just plug the value staight in now here*/
  strcpy (bmi->cr_ErrMes,""); 

  delz    = e_delz;
  depth   =  e_Depth;
  nlevel  = ceil(depth/delz + 1);

   linspace(z, 0,depth,nlevel);


// % Crank-Nicolson
   delt   = e_delt;
// % delt    = 0.6;
   ndpthT = 0;


#ifdef  XXXXXXXXXXX
When/if we do Manitou, will need to figure out what/where
 some of these variables below are 
  if  ( stricmp( soilmoddat,"Manitou04")) {
    delt   = 1.5;
    delz   = 0.002;
    depth  = 0.600;
    nlevel = ceil(depth / delz + 1);
//    z      = linspace(0,depth,nlevel);
    Delt = 120;
    savetm = Delt / delt;
    indxsv = 0;
    depthT = 0;
    ndpthT = 0;
    if ( VARPROF == 1 ) {
      depthT = 0.15;
      ndpthT = ceil(depthT/delz + 1);
    } //end
  }  // end
#endif 

// % Newton-Raphson 
  if ( CRANK != 1 ) {
     delt    = 0.6;
  } // end

/* NOTE see BMSoil.h for eC_nsteps   */
  nsteps  = nmin * 60.0 / delt + 1.0;
  if ( nsteps > eC_nsteps ) {     /* See Note-1 above */  
    sprintf (bmi->cr_ErrMes,"Maximum number of time steps (nsteps) exceeded");    
    return 0; }

//  time    = delt * linspace (0,nsteps-1,nsteps);
 linspace (time, 0,nsteps-1,nsteps);
 for ( i = 1; i <= nsteps; i++ ){
     time[i] = time[i] * delt; 
     f = time[i]; // test to see value 
 } 

// %__________________________________________________________________________
// % Set numerical algorithm parameters
// % Newton-Raphson 
  difcoeF = delt / (delz * delz);
// % Crank-Nicolson
  difcoef = 0.5 * difcoeF;

  delvel  = delt / delz;
  nl = nlevel - 1;

//%__________________________________________________________________________
//% Set convergence criteria for Newton-Raphson Method
//% Enconv [W/m2] for convergence using global energy change criteria
//% Rconv [ - ] for convergence using global proportional change criteria

  Enconv = 10; 
  Rconv  = 0.005;


  Convcr = Enconv;
  if  ( ECONVG != 1 ) {
    Convcr = Rconv;
  } // end

   return 1; 
}

/*-----------------------------------------------------------------
* Name: linspace
* Desc: Mimics a Matlab built in function
*       Generate linearly spaced vector
*       google ->  "Matlab linspace" to get to Matlab's definition
*   In: z....... array to load
*       x1...start vector
*       x2...end vecot
*       n....array entries
*-----------------------------------------------------------------*/
void linspace (double z[], double x1, double x2, double n )
{
int i; 
double f, g, tot;
/* This how Matlab defines it */ 
  g = x2 - x1; 
  f  = g / ( n - 1.0); 
  tot = 0;  
  for ( i = 1; i <= n; i++ ) { 
     z[i] = tot;
     tot = tot + f; }
}




#ifdef XXXXX
%--------------------------------------------------------------------------
%                              Model_Param.m                                                                
%
%                       Initialize model parameters         
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Set up depth and time arrays and parameters for the model 
% simulation of coupled soil heat, moisture, and vapor flow
%        depth, delz, and z [m] 
%        delt and time [s] 
%        nmin = number of minutes of simulation

delz    = 0.001;
depth   = 0.200;
nlevel  = ceil(depth/delz + 1);
z       = linspace(0,depth,nlevel);

% Crank-Nicolson
delt   = 1.2;
% delt    = 0.6;
ndpthT = 0;
if strcmp(soilmoddat,'Manitou04')
delt   = 1.5;
delz   = 0.002;
depth  = 0.600;
nlevel = ceil(depth/delz + 1);
z      = linspace(0,depth,nlevel);
Delt = 120;
savetm = Delt/delt;
indxsv = 0;
depthT = 0;
ndpthT = 0;
if VARPROF == 1
depthT = 0.15;
ndpthT = ceil(depthT/delz + 1);
end
end
% Newton-Raphson 
if CRANK ~= 1
delt    = 0.6;
end

nsteps  = nmin*60/delt + 1;
time    = delt*linspace(0,nsteps-1,nsteps);

%__________________________________________________________________________
% Set numerical algorithm parameters

% Newton-Raphson 
difcoeF = delt/(delz*delz);
% Crank-Nicolson
difcoef = 0.5*difcoeF;

delvel  = delt/delz;
nl      = nlevel-1;

%__________________________________________________________________________
% Set convergence criteria for Newton-Raphson Method
% Enconv [W/m2] for convergence using global energy change criteria
% Rconv [ - ] for convergence using global proportional change criteria

Enconv = 10; 
Rconv  = 0.005;

Convcr = Enconv;
if ECONVG ~= 1;
Convcr = Rconv;
end


#endif 