
#include <stdio.h> 
#include <math.h> 
#include "BMSoil.h"


/***************************************************************************/
double Him[5];
double Him_Init[5] = { 0, 1.67752, 2.20462, 0.6366564, -0.241605};

double vil[11]; 
double vil_Init[11] = { 0, -4.747782033,  0.04799919289, -0.1931588954,  1.46072834,
                           -4.000075762,  5.576209858,    2.570488297,  -0.8670521019, 
                            0.166642939, -0.0139663462};

extern  double h2omol;

void Water_Vapor_Constants ()
{
int i,n;
double f;

  for ( i = 0; i < 5; i++ ) 
    Him[i] = Him_Init[i]; 

  for ( i = 0; i < 11; i++ ) 
    vil[i] = vil_Init[i]; 


   f = 1e+04;
   n = ARRAY_SIZE (Him); 
   for ( i = 0; i < n; i++ ) 
     Him[i] = Him[i] * f; 
   
   f = 1e+06;
   n = ARRAY_SIZE (vil);
   for ( i = 0; i < n; i++ ) 
     vil[i] = vil[i] * f;  

   for ( i = 0; i < n; i++ ) 
     vil[i] = vil[i] / h2omol;  

}


#ifdef wowoow
%--------------------------------------------------------------------------
%                        Water_Vapor_Constants.m                           
%--------------------------------------------------------------------------

%__________________________________________________________________________
%           Coefficients for temperature dependency of water vapor  
% -------------------- Dynamic Viscosity = Him -------------------
% -------------------- Specific Heat     = vil -------------------


global Him 
global vil

Him = 1e+04*[1.67752 2.20462 0.6366564 -0.241605];

vil = 1e+06*[-4.747782033  0.04799919289 -0.1931588954  1.46072834   ...
             -4.000075762  5.576209858    2.570488297  -0.8670521019 ... 
              0.166642939 -0.0139663462]/h2omol;
#endif 