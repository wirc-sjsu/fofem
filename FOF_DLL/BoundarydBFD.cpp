/****************************************************************
* Name: BoundarydBFD.cpp
* Desc:
* Date: 9/20/2018





   Latest Massman Boundry - not implemented yet




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

extern double expp[eC_nsteps]; 
extern double expt[]; 

extern double ttme[]; 

extern double force[];

/****************************************************************
%--------------------------------------------------------------------------
%                        BoundarydBFD.m
%
%   Set the upper boundary condition for Observed Surface Temperature
%   using the original BFD curve 
%
%   tm = time (hrs) from inception of the fire at which 
%        the maximum forcing occurs 
%
%   td = duration or length of the fire (hrs) - Strictly speaking td is 
%        the difference between the two times at which the forcng = 0.01 - 
%        force(t1) = force(t2) = 0.01 and t1 < tm < t2 and td = t2-t1 - 
%
%   These are ambient surface: 
%
%   incoming radiation = bcQ [W/m^2] ; vapor pressure = bcva [Pa] 
%          temperature = bcta  [C]   ; vapor density  = bcra [kg/m^3]                        
%   downward atmospheric shortwave radiation term = eta4 = eps*T^4 [K^4]   
%
%--------------------------------------------------------------------------
*******************************************************************/

void BoundarydBFD (d_BMI *bmi)
{
int i,js,nn; 
double d;

  for ( i = 0; i <= eC_nsteps; i++ ) {
    expp[i]  = 0; 
    expt[i]= 0; 
    bcQ[i]= 0; 
    bcva[i]= 0; 
    bcta[i]= 0; 
    bcra[i]= 0; 
    forIR[i]= 0; 
  }

// js        = 1:nsteps;
// ttme      = (js-1)*delt/3600;
   for ( js = 1; js <= nsteps; js++ )    
     ttme[js] = (js-1) * delt / 3600.0;

// force(nn) = exp(-a1td*(abs(log(ttme(nn)/tm))).^2);
extern double a1td,tm,ttme[];
   force[1] = 0;
 

  for ( nn = 2; nn <= nsteps; nn++ ) {

//     d = exp ( -a1td * (abs(   log(ttme[nn] / tm)   )  ) );  // .^2);
//     force[nn] = d *d ;  
// 11/1/18 - I was doing the above wrong, Bill gave me the correct code below 
d = (abs(log(ttme[nn]/tm)));
force[nn] = exp(-a1td*d*d);


}





  BoundaryIR_T1(force);  

  for ( js = 2; js <= nsteps; js++ )     
    bcQ[js] =  bmi->f_Qabs * force[js];  

  for ( js = 2; js <= nsteps; js++ )  
    bcva[js] = vapori + (vaporf-vapori)*force[js];

  for ( js = 2; js <= nsteps; js++ )  
    bcta[js]      = tempai + (tempaf-tempai)*force[js];

  for ( js = 2; js <= nsteps; js++ )  
    bcra[js]  = bcva[js] * h2omol / (rgas * (bcta[js] + temp0));

 
   caleta4(eta4,bcta,bcva,nsteps);    // eta4      = caleta4(bcta,bcva);


}

/*-----------------------------------------------------------*/
#ifdef eeieie


% BoundaryIR










#endif 



/*---------------------------------------------------------*/
#ifdef AAAAAA
                      Original Matlab code 

%--------------------------------------------------------------------------
%                        BoundarydBFD.m
%
%   Set the upper boundary condition for Observed Surface Temperature
%   using the original BFD curve 
%
%   tm = time (hrs) from inception of the fire at which 
%        the maximum forcing occurs 
%
%   td = duration or length of the fire (hrs) - Strictly speaking td is 
%        the difference between the two times at which the forcng = 0.01 - 
%        force(t1) = force(t2) = 0.01 and t1 < tm < t2 and td = t2-t1 - 
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

force(1)  = 0;
nn        = 2:nsteps;
force(nn) = exp(-a1td*(abs(log(ttme(nn)/tm))).^2);

% BoundaryIR
  BoundaryIR_T1 

bcQ       = Qabs*force;

bcva      = vapori + (vaporf-vapori)*force;

bcta      = tempai + (tempaf-tempai)*force;

bcra      = bcva*h2omol./(rgas*(bcta+temp0));

eta4      = caleta4(bcta,bcva);
#endif 