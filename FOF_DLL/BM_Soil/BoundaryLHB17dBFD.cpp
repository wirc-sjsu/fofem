/********************************************************************************
* Name: BoundaryLHB17dBFD.m
* Desc: Added 12/2/17
* 
*
********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

double force  [eC_nsteps]; 
double ttme  [eC_nsteps]; 

extern double js    [];
extern double bcQ   [];
extern double bcva  []; 
extern double bcta  []; 
extern double bcra  []; 
extern double forIR  [];
extern double eta4  [];
extern double delt,nsteps; 
extern double vaporf,vapori; 
extern double tempaf, tempai; 
extern double  h2omol, rgas, temp0; 
double _CalcA1 (double t_m, double t_d);
double  asinh(double x);

double  asinh(double x);

/*******************************************************************
* Name: BoundaryLHB17dBFD
* Note-1: to make sure that there will always be zeros on the
*         end of these arrays 
*  In: bmi......inputs from GUI
********************************************************************/
void  BoundaryLHB17dBFD ( d_BMI *bmi) 
{
int js,nn;
double tm, td, altd, l, d; 
double a[3];

/* See Note-1 above */
   for ( js = 0; js < eC_nsteps; js++ ) {
     bcQ[js] = 0; 
     bcva[js] = 0;     
     bcta[js] = 0;     
     bcra[js] = 0;     
     eta4[js] = 0; }


// js        = 1:nsteps;
// ttme      = (js-1)*delt/3600;
   for ( js = 1; js <= nsteps; js++ )     // ttme   = (js-1)*delt/3600;
     ttme[js] = (js-1) * delt / 3600.0;

/* GUI inputs tm = time(hr) when max watts are applied, td = burn time (hr) */
   tm = bmi->f_MaxWatTim;           // tm        = 0.6;
   td = bmi->f_BurnTime;           // td        = 17.834 

   altd = 2.0 * xlog(10) /  xpow ( (asinh(0.5*td/tm)), 2 );   // a1td      = 2*log(10)/(asinh(0.5*td/tm))^2;

   a[1] = altd;                            // a  = [a1td tm];
   a[2] = tm; 

    force [1] = 0;                         // force(1)  = 0;
    for ( nn = 2; nn <= nsteps; nn++ ) {   // nn  = 2:nsteps;
      l = xlog(ttme[nn] / a[2] );          // force(nn) = exp(-a(1)*( log(ttme(nn)/a(2))  ) .^2); 
      l = l * l; 
      d = -a[1] * l; 
    
      force[nn] = xexp (d);  
   }                         

   BoundaryIR();   // BoundaryIR
 
// bcQ       = Qabs*force;
// bcva      = vapori + (vaporf-vapori)*force;
// bcta      = tempai + (tempaf-tempai)*force;
// bcra      = bcva*h2omol./(rgas*(bcta+temp0));
// eta4      = caleta4(bcta,bcva);


   for ( nn = 1; nn <= nsteps; nn++ ) {
      bcQ[nn] = bmi->f_Qabs * force[nn];       // bcQ = Qabs*force;

      bcva[nn] = vapori + (vaporf-vapori)*force[nn];              // bcva      = vapori + (vaporf-vapori)*force;

      bcta[nn] = tempai + (tempaf-tempai)*force[nn];                            // bcta      = tempai + (tempaf-tempai)*force;

      bcra[nn] = bcva[nn] * h2omol / ( rgas * (bcta[nn] + temp0 ));   // bcra      = bcva*h2omol./(rgas*(bcta+temp0));

   } /* for */ 

   caleta4(eta4,bcta,bcva,nsteps);    // eta4      = caleta4(bcta,bcva);
}


/******************************************************
* Name: asinh
* Desc: calc the arc sin hyperbolic 
*       This function is not in math.h 
*******************************************************/
double  asinh(double x)
{  
  double d; 
  d =  xlog(x + sqrt((x*x) + 1));
  return d; 
}



/************************************************************************************/
#ifdef wowowowowo

%--------------------------------------------------------------------------
%                     BoundaryLHB17dBFD.m
%
%   Set the upper boundary condition for Observed Surface Temperature
%   using the modified BFD curve 
%
%   These are ambient surface: 
%
%   incoming radiation = bcQ [W/m^2] ; vapor pressure = bcva [Pa] 
%          temperature = bcta  [C]   ; vapor density  = bcra [kg/m^3]                        
%   downward atmospheric shortwave radiation term = eta4 = eps*T^4 [K^4]   
%
%--------------------------------------------------------------------------

js        = 1:nsteps;
ttme      = (js-1)*delt/3600;

tm        = 0.6;
td        = 17.834;
a1td      = 2*log(10)/(asinh(0.5*td/tm))^2;

a         = [a1td tm];

force(1)  = 0;
nn        = 2:nsteps;
force(nn) = exp(-a(1)*(log(ttme(nn)/a(2))).^2);

BoundaryIR
 
bcQ       = Qabs*force;

bcva      = vapori + (vaporf-vapori)*force;

bcta      = tempai + (tempaf-tempai)*force;

bcra      = bcva*h2omol./(rgas*(bcta+temp0));

eta4      = caleta4(bcta,bcva);

#endif 