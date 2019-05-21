
#include <stdio.h>
#include <string.h>
#include <math.h> 

#include "BMsoil.h"


//%--------------------------------------------------------------------------
//%
//%                       Model_Switch_HMV.m                         
//%
//%--------------------------------------------------------------------------

//%__________________________________________________________________________
//% Choose method of solution: CRANK == 1 implies Crank-Nicholson
//%                  otherwise CRANK == 0 implies Newton-Raphson
//%                  if Crank-Nicolson Choose temperature dependent WRC 
//%                  NO: TMPWRC == 0 ---- YES: TMPWRC  == 1 (YES) 
//%                  if Newton-Raphson Choose Convergence Criteria
//%                  ECONVG == 1: total energy change is <= Enconv [W/m2]
//%                  ECONVG ~= 1: total proportional change is <= Rconv [ - ] 
//%                  RCONVG == 1: Separate solution for vapor density 

int CRANK  = 1;
int TMPWRC = 0;
int ECONVG = 1;
int RCONVG = 0;

//%__________________________________________________________________________
//% Choose soil thermal monductivty model wo/w additional radiation term 

int CONRD = 1;

//%__________________________________________________________________________
//% Choose Enhancement of Evaporation or Not 

int ENHEV = 1;

//%__________________________________________________________________________
//% Choose Hydraulic Function 

int GGHYD = 0;
int GGHYB = 0;
int VNHYD = 0;
int BCHYD = 0;
int VAHYD = 1;

int WRCHY = 0;  // added 10-20-17

//%__________________________________________________________________________
//% Include surface diffusion of liquid water Kd as part of Kn

int SURFD = 1;

//%__________________________________________________________________________
//% Choose Water Activity Model: KELVN == 1 is the Kelvin Eqn 
//%        or my modification of the Kelvin Eqn == Type V Adsorption Isotherm        

int KELVN = 1;

//%__________________________________________________________________________
//% Choose Volume-averaged Soil Water (Evaporative) Surface Area Model [AWA]
//% BRUSS == 1 is my version of the Brusseau Model for AWA
//%            or a simpler parabolic function for AWA 
// original 1,   0 gives a little smoother moisture cruve 
int BRUSS = 0;

//%__________________________________________________________________________
//% Choose Evaporation Source Term Model

int MASSM = 0;
int NOVAK = 0;
int MASSO = 0;
int DRYSV = 1;

//%__________________________________________________________________________
//% Choose Form of Induced Advective Term SORCE == 1 => du/dz \propto S_v 
//%        or du/dz \propto d\theta/dt 

int SORCE = 1;

//%__________________________________________________________________________
//% Choose Water Retention Curve 

int GGWRC = 0;
int CSWRC = 0;   // change, was 0
int FYWRC = 1;   // change, was 1

//%__________________________________________________________________________
//% Choose to include residual moisture term [ thetar = thetar(psin,TK) ]  

int THETAR = 0;

//%__________________________________________________________________________
//% Choose to include hydraulic conductivity associated with film flow

int FILMFL = 0;

//%__________________________________________________________________________
//% Choose to mass mixing ratio formulation for diffusive flux

int MIXDIF = 0;

//%__________________________________________________________________________
//% Choose Data Set to Model 

char soilmoddat[] = "QUINCY1";

int VARPROF = 0;  // added 10-20-17

//%__________________________________________________________________________
//% Choose Model run for plot comparisons 

int Compare = 1;

char ComModel[100]; 

void   Model_Switch_HMV ()
{

  if ( !stricmp ("QUINCY1",soilmoddat ) )
      strcpy (ComModel,"CSVA00R4"); 

  if ( !stricmp ("QUINCYD", soilmoddat ) )   
    strcpy (ComModel,"QDCSVA00");

  if ( !stricmp ( "PBAIRG", soilmoddat ) )   
     strcpy (ComModel,"PBACSVA10"); 

  if ( !stricmp ("PBWETG", soilmoddat ) )   
      strcpy (ComModel,"PBWCSVA10a"); 
  
  if ( !stricmp ("BOULDERCW", soilmoddat ) )   
     strcpy (ComModel,"BCWCSVA00R0"); 

  if ( !stricmp ("Manitou04", soilmoddat ) )    
     strcpy (ComModel,"M04cbase");   

}