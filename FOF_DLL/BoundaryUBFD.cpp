/****************************************************************
*
*
*****************************************************************/
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"


extern double js    [];
extern double bcQ   [];
extern double bcva  []; 
extern double bcta  []; 
extern double bcra  []; 
extern double forIR  [];
extern double eta4  [];

extern double  h2omol, rgas, temp0; 
extern double Qabs; 
extern double vaporf,vapori; 
extern double tempaf, tempai; 
extern double delt,tauh,nsteps; 

double expp[eC_nsteps]; 
 extern double expt[]; 


/**********************************************************
*
***********************************************************/
void BoundaryUBFD (d_BMI *bmi)
{
int js; 
double d;

  for ( js = 0; js <= eC_nsteps; js++ ) {
    expp[js]  = 0; 
    expt[js]= 0; 
    bcQ[js]= 0; 
    bcva[js]= 0; 
    bcta[js]= 0; 
    bcra[js]= 0; 
    forIR[js]= 0; 
  }

bcQ[1]    = 0;
bcva[1]   = vapori;
bcta[1]   = tempai;
bcra[1]   = bcva[1] * h2omol / (rgas * ( bcta[1]+temp0));
forIR[1]  = 0;
double tQmax;

  tQmax  = ( bmi->f_BurnTime * 60.0 );  /* User entered burn time in minutes get it to seconds */
  tQmax  = tQmax / 2.0;                 /* and half it */ 
  for ( js = 2; js <= nsteps; js++ ) {
    d = js - 1 ;
    expp[js]  = xlog ( d * delt / tQmax );       // expp(js)  = log((js-1)*delt/tQmax);
  }


  for ( js = 2; js <= nsteps; js++ )
    expt[js]  = exp (-expp[js] * expp[js] / 0.7);   //  expt(js)  = exp(-expp(js).*expp(js)/0.7);

  for ( js = 2; js <= nsteps; js++ )                //  bcQ(js)   = Qabs*expt(js);
    bcQ[js]   =  bmi->f_Qabs * expt[js];      //  Qabs * expt[js];

  for ( js = 2; js <= nsteps; js++ )  
    bcva[js]  = vapori + (vaporf - vapori) * expt[js];   // bcva(js) = vapori + (vaporf-vapori)*expt(js);

  for ( js = 2; js <= nsteps; js++ )  
    bcta[js] = tempai + (tempaf-tempai) * expt[js];         //  bcta(js) = tempai + (tempaf-tempai)*expt(js);

  for ( js = 2; js <= nsteps; js++ )  
    bcra[js] = bcva[js] * h2omol / ( rgas * (bcta[js]+temp0) );     //  bcra(js)  = bcva(js)*h2omol./(rgas*(bcta(js)+temp0));

  caleta4(eta4,bcta,bcva,nsteps);

  for ( js = 2; js <= nsteps; js++ )  
    forIR[js] = expt[js];

 }

#ifdef oowooo
%--------------------------------------------------------------------------
%                          BoundaryUBFD.m
%
%   Set the upper boundary condition for Campbell's laboratory tests     
%   These are ambient surface: 
%
%   incoming radiation = bcQ [W/m^2] ; vapor pressure = bcva [Pa] 
%          temperature = bcta  [C]   ; vapor density  = bcra [kg/m^3]                        
%   downward atmospheric shortwave radiation term = eta4 = eps*T^4 [K^4]   
%
%--------------------------------------------------------------------------

bcQ(1)    = 0;
bcva(1)   = vapori;
bcta(1)   = tempai;
bcra(1)   = bcva(1)*h2omol./(rgas*(bcta(1)+temp0));
forIR(1)  = 0;

tQmax     = 1000;

js        = 2:nsteps;
expp(js)  = log((js-1)*delt/tQmax);
expt(js)  = exp(-expp(js).*expp(js)/0.7);

bcQ(js)   = Qabs*expt(js);

bcva(js)  = vapori + (vaporf-vapori)*expt(js);

bcta(js)  = tempai + (tempaf-tempai)*expt(js);

bcra(js)  = bcva(js)*h2omol./(rgas*(bcta(js)+temp0));

eta4      = caleta4(bcta,bcva);

forIR(js) = expt(js);

#endif 