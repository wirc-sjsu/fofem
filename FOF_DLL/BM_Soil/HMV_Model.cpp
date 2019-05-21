/*%--------------------------------------------------------------------------
%--------------------------------------------------------------------------
%
%                        Fire and Soil-HMV_model.m
%
%              The Fire & Soil Heat-Moisture-Vapor Model              
%                        Linearized Crank-Nicolson 
%                      [Version 1.0 -- February 2012] 
%                      
%--------------------------------------------------------------------------
%-------------------------------------------------------------------------*/
#include <windows.h> 
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h> 
#include <setjmp.h> 

#include "BMSoil.h"
#include "SCN_Arrays.h" 
#include "HTAA.h" 

extern double tempki[]; 
extern double nmin; 
extern double nlevels;

//extern d_BMI s_BMI; 


/****************************************************************************************
* Name: main - 
* Desc: this is actually the HMV_Model.m in Matlab starting point 
*   In: bmi - inputs
*       cr_Mode ... "Boundry" - return after doing the Boundry function
*
****************************************************************************************/
int HMV_Model(d_BMI *bmi, char cr_Mode[])
// int main()
{
int i; 
double f,g,h; 
int r, c; 
double  fr[4];
char cr_ErrMes[1000]; 



//%__________________________________________________________________________
//% Choose models of physical processes  
   Model_Switch_HMV(); 

//%__________________________________________________________________________
//% Asign general physical constants and related model paramters 
   Physical_Constants_HMV();

//%__________________________________________________________________________
//% Input soil parameters 
//% Assign spatial grid scale and time step
//% Initialze vertical profiles of soil bulk density, mineral fraction, 
//% total porosity, soil moisture, soil temperature, and the 
//% normalized soil water potential      

  if ( !Soil_Time_Depth_Param_HMV(bmi)) {
    return 0; }

// Added 12/2/7
//   BoundaryLHB17dBFD (bmi); 
 BoundarydBFD (bmi);

//% Input Upper Boundary Conditions = UBC
//   BoundaryUBFD(bmi); 
//  BoundaryU(bmi);

  if ( !stricmp (cr_Mode, "Boundry") )
     return 1; 


Skip:
   i =  SolveHMV(cr_ErrMes);


   return i; 

}

