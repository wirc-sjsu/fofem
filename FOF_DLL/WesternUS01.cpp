
/*****************************************************************************
* Name: WesternUS01.cpp
* Desc: 
* Date: sept 24 2018
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include <cmath>
#include "BMsoil.h"


//%--------------------------------------------------------------------------
//%                           WesternUS01.m                            
//%
//%               Initialize soil physical and model constants                     
//%--------------------------------------------------------------------------

//%__________________________________________________________________________
//%  nhalfhr = length of burn in one-half hour units [half-hours]
//%  nmin    = length of burn in minutes [minutes]

double longfire; 
double nhrs;
extern double nmin; 
extern double Qabs, vapori, vaporf, tempaf;

double tm, td; 

double a1td; 

/******************************************************
* Name: asinh
* Desc: calc the arc sin hyperbolic 
*       This function is not in math.h 
*******************************************************/
double  _asinh(double x)
{  
  double d; 
  d =  xlog(x + sqrt((x*x) + 1));
  return d; 
}

/***************************************************************
* Name: WesternUS01
* 
****************************************************************/
int WesternUS01 (d_BMI *bmi)
{
double a,b,c,d; 

longfire = 1;
nhrs = 60;
nmin = 60*nhrs;

nmin = bmi->d_SimTime; 

//% Input model temporal and spatial discretization parameters 

   if ( !Model_Param(bmi) )
      return 0; 

//%__________________________________________________________________________
//% Variables to be used with the model's initial and boundary conditions 
//%
//%  Qabs   = Energy input from heater [W/m^2]
//%  vapori = inital atmospheric vapor density [Pa]
//%  vaporf = final atmospheric vapor density [Pa]
//%  tempaf = final atmospheric temperature [C]

Qabs    = 47000;
vapori  =  1000;
vaporf  = 15000;
tempaf  =   500;


//%__________________________________________________________________________
//%   Parameters for the original BFD curve = Forcing function for UBC  
//%
//%   tm = time (hrs) from inception of the fire at which 
//%        the maximum forcing occurs 
//%
//%   td = duration or length of the fire (hrs) - Strictly speaking td is 
//%        the difference between the two times at which the forcng = 0.01 - 
//%        force(t1) = force(t2) = 0.01 and t1 < tm < t2 and td = t2-t1 - 

tm     = 9.25;
td     = 24;

// Test..................
tm = bmi->f_MaxWatTim; 
   td = bmi->f_BurnTime; 

// a1td   = 2*log(10) / (asinh(0.5*td/tm)) ^2;

   a1td = 2.0 * xlog(10) /  xpow ( (_asinh(0.5*td/tm)), 2 );  

//%__________________________________________________________________________
//% Variables for Campbell's soil thermal conductivity model
//% But new model only uses zlamm - Can be adjusted if necessary - 
//% 2 <= zlamm (W/m/K) <= 14 is to be expected

extern double zlamm, xw0, ga, gc, q0t; 
double q0; 

zlamm   = 4.42;  /* thermal conductivity of mineral fraction */
xw0     = 0.09;   /* Water Content for liquid recirculation BM said not currently used anymore */
ga      = 0.123;  /* Vries-shape */
q0      = 3;      /* cop-power reciculation function BM said not used */

gc      = 1.0-2.0*ga;      
q0t     = q0/(303*303);
 
//%__________________________________________________________________________
//% Variables to be used with the initial soil conditions  
//% 
//% thetar = residual soil moisture 
//% [numf1 numf2] = thetar T_K and psin dependence term 
//% thetai = initial volumetric soil moisture [m^3/m^3] excluding thetar
//% psini  = inital valus of normalized soil water potential [ - ]
//%          psini must be consistent with thetai-thetar and must 
//%          be computed beforehand
//% tempai = initial atmospheric temperature [C]
//% den0   = soil bulk density [gm/cm^3]
//% parden = soil particle density [g/cm^3]
//% dg     = mean particle size [m]

extern double thetai, tempai,den0, parden, dg0, dg; 

thetai    = bmi->f_Moist;      // 0.07;   /* Soil Moisture */
     
  tempai = bmi->f_AmbAirTmp ;  /* Change 3/23/19  Get from GUI */
// tempai    = 19.0;           /* starting soil temperature */ 

den0      = bmi->f_SoiBulDen;  // 1.30;   /* Soil Bulk density */
parden    = bmi->f_SoiParDen;  // 2.35;   /* Soil Particle density */
dg0       = 20;
dg        = dg0*1e-06;

extern double  thetar;
extern double  numf1, numf2;
extern int THETAR; 
double psini;
int i; 
double a0CS, aCS[7], bCS[7], wCS;
double  a0[6] = { 3.722, -1.724,  0.4272,  -0.04202, 0, 0, };
double  b0[6] = {-7.484,  1.294, -0.07394, -0.01343, 0, 0 };

double a19[6] = {-2.243, -1.713,  -0.2822,  0.072,   0.04757, 0};
double b19[6] = { 2.247, -0.7484, -0.7179, -0.2635, -0.02794, 0}; 
 
double a34[6] =  { 2.646, -1.548,  0.5911, -0.1395, 0.01572, 0};
double b34[6] =  {-1.687,  1.028, -0.4124,  0.101, -0.01162, 0}; 


  if ( THETAR == 1 ) {
    thetar  = 0.01;
    thetai  = thetai-thetar;
//% LOW  thetai  = 0.120-thetar;
//% HIGH thetai  = 0.190-thetar;
    psini   = 2.14841e-03;
    numf1   = 1.25;
    numf2   = 1/3; }
  else{
    thetar  = 0;
    if ( thetai <= 0.19 ) {
      a0CS    = -2.383;
      for ( i = 1; i <= 6; i++ ) { 
        aCS[i] = a0[i-1];  
        bCS[i] = b0[i-1];}  
       wCS =  7.614;   }

    else if ( thetai > 0.19 && thetai <=0.34 ) {
      a0CS    = -10.88;
      for ( i = 1; i <= 6; i++ ) { 
        aCS[i] = a19[i-1];  
        bCS[i] = b19[i-1];}  
        wCS    =  12.06; }

    else if ( thetai > 0.34  && thetai <= 0.43) {
       a0CS = -13.55 ;
       for ( i = 1; i <= 6; i++ ) { 
        aCS[i] = a34[i-1];  
        bCS[i] = b34[i-1];} 
       wCS = 16.34; }
    else {
       strcpy (bmi->cr_ErrMes, " ***** initial value of theta exceeds 0.43 *****");
       return 0; }  

    psini   = PSINi(thetai,a0CS,aCS,bCS,wCS); 

//% p       = [0 0 0 0 0 0 4.786 -0.8029 -39.35 -0.0002096];
//% psini   = PSINi_poly(thetai,p);
//% psini   = 1.449242e-03;
//% LOW  psini   = 8.863218e-03;
//% HIGH psini   = 5.65627e-04;
    numf1   = 0;
    numf2   = 0;

  } /* else of THETAR == 1*/



//%__________________________________________________________________________
//% Parameters for temperature effects on the water retention curve
   PsinTmpnT (tempai); 

//%__________________________________________________________________________
// % KInc = Intrinsic Hydraulic Conductivity [m^2] 
extern double KInc;
double Bear; 

Bear = 0.000617; 
KInc = Bear*dg*dg;

//%__________________________________________________________________________
//% Variable and parameters for the film flow term of 
//% the hydraulic conductivity

extern double Rwfac, Fwfac;
double facrw, facMx; 

facrw = 0.04*dg0*dg0*dg0;
facMx = 1000;
if ( facrw >= facMx )
   facrw  = facMx;

double fcrw;
extern double eps0; 
extern double kBoltz,elec; 

fcrw  = xsqrt(eps0/dg)*kBoltz/elec;
Rwfac = facrw*xsqrt(2)*e_pi*e_pi*fcrw*fcrw*fcrw/Bear;

extern double psi0;
Fwfac = dg*psi0;


//%__________________________________________________________________________
//% Variables to be used with the Brooks-Corey, vanGenuchten-Assouline and  
//%   VanGenuchten-Nielsen models of hydraulic conductivity

extern double bcexp;
double  brcr;  

brcr  = 6;
bcexp = 2*brcr+3;


extern double  EA, MA; 
extern int DRYSV,NOVAK; 

if (DRYSV == 1) {
   EA = 2.40;  
   MA = 0.20;   }

if (NOVAK == 1) {
   EA = 2.25;
   MA = 0.20; }   

extern  double MV, MVe, MVx;
MV    = 0.20;
MVe   = 2.0/MV;
MVx   = 2.0 + 0.5*MV;


//%__________________________________________________________________________
//% Variables to be used with the surface diffusion model  
extern double thetam0, dsurf0;
thetam0 = 0.02;
dsurf0  = 1e-10;

//%__________________________________________________________________________
//% Variables to be used with the Shiozawa-Campbell water rentention curve  
//%   
extern double thetah, alpha4, mexp;
extern double thetan; 
double alphah,thetap,zlnpsi0 ;
extern double grav; 

thetah  =  0.1414;
alphah  = -2.189*psi0/grav;
alpha4  =  alphah*alphah*alphah*alphah;
mexp    = -0.3608;
thetap  =  0.3508;
zlnpsi0 =  13.8155106;
thetan  =  thetap/zlnpsi0;

//%__________________________________________________________________________
//% Variables to be used with the Groenevelt-Grant water rentention curve 
//%   and the hydraulic conductivity 
//%   Note: NGb is NG for the hydraulic conductivity function GGHYB = 1
//%     and NGm is NG for the hydraulic conductivity function GGHYD = 1

extern double thetaGf, k1G, NG, kGp0, NGb, NGm;

//% ++++ GGWRC ++++ 
//% ++++ thetaGf = 1-thetar;
thetaGf = 1;
k1G     = 1.742;
NG      = 0.3413;
//% k1G     = 4;
//% NG      = 4/9;
kGp0    = k1G*grav/abs(psi0);
NGb     = 9*NG;
NGm     = 3.5*NG;

//%__________________________________________________________________________
//% Variables to be used with the Fredlund-Ying water rentention curve  

extern  double mur, almur, mui, Nexp, Mexp;

mur     = 323.6246e-06;
almur   = 1.0/(log(1.0+1.0/mur));
mui     = 2.94985251e-06;
Nexp    = 3.04;
Mexp    = 0.3099;


/*---------------------------------------------------------*/
double aFY0[6] = { 4.416, -2.553,   0.5845, -0.02512, -0.007538, 0};
double bFY0[6] = {-5.94,   0.09777, 0.4812, -0.1467,   0.01235,  0}; 

double  aFY19[6] =  {-12.33, 7.267, 2.82,   0, 0, 0,};
double  bFY19[6] =  {28.24,  13.0,  0.5732, 0, 0, 0}; 

double  aFY34[6] =  {-6.752,  3.19,  1.839,  0.05515, 0, 0};
double  bFY34[6] =  {-9.459, -5.258, 0.2943, 0.2463,  0, 0}; 


extern double psiniFY; 
double a0FY,wFY;
double aFY[7],bFY[7]; 

  if ( THETAR == 1 )
    psiniFY = 1.583416e-03;
  else {
    if ( thetai <= 0.19 ) {
      a0FY    = -2.415;
      for ( i = 1; i <= 6; i++ ) {
        aFY[i] = aFY0[i-1];
        bFY[i] = bFY0[i-1];}
      wFY     =  5.173; }
    else if (thetai > 0.19 && thetai <=0.34) {
      a0FY    =  -29.97;
      for ( i = 1; i <= 6; i++ ) {
        aFY[i] = aFY19[i-1];
        bFY[i] = bFY19[i-1];}
      wFY     =   3.245; }
    else if (thetai > 0.34 && thetai <= 0.43) {
      a0FY    = -19.04;
      for ( i = 1; i <= 6; i++ ) {
        aFY[i] = aFY34[i-1];
        bFY[i] = bFY34[i-1];}
      wFY     =  5.532; }
    else {
      strcpy (bmi->cr_ErrMes," ***** initial value of theta exceeds 0.43 *****");    
    }  // end

    psiniFY = PSINi(thetai*parden/(parden-den0),a0FY,aFY,bFY,wFY);     
    //% p       = [0 0 0 0 0 0 -14.16 18.66 -25.19 0.002464];
    //% psiniFY = PSINi_poly(thetai*parden/(parden-den0),p);
    //% psiniFY = 1.248083e-03;
    //% LOW  psiniFY = 6.272000e-03;
    // % HIGH psiniFY = 5.39777e-04;
   }  /* end - if THETAR else */


//%__________________________________________________________________________
//% Variables to be used with the hydraulic function constructed from the 
//% Fredlund-Xing water retention curve -- See calhydauWRC.m  

extern double HYD0, HYD1, HYD2, HYDP;

HYD0    = -448;
HYD1    =  414;
HYD2    =  0.004946;
HYDP    =  0.005;


//%__________________________________________________________________________
//% Water acivity parameters for the Type V adsorption isotherm (Cherblanc)
//%   using my modified Kelvin Equation

extern double AWA, BWA;

AWA    = 0.8;
BWA    = 2;

//%__________________________________________________________________________
//% Water acivity parameters for the Cherblanc adsorption isotherm 

double w0Cden, AWC, BWC;

w0Cden =  1.0/(den0*0.0028); 
AWC    =  2.9;
BWC    = -1.9;

//%__________________________________________________________________________
//% Vapor Source Term Coefficient [Svcoef] and parameters for Awa model 

extern double Svcoef, NB, CB, NAWX, Swc[],  Awax;

if (DRYSV == 1)
   Svcoef = 0.3;

if (NOVAK == 1 )
   Svcoef = 0.3;


extern double nlevel ;

NB     = 50;
CB     = 0.008;
NAWX   = 8;


/*-------------------------------------------------*/
// Swc    = (0.001/0.45)*ones(1,nlevel);
// aux1   = Swc.*(1-Swc);
// aux3   = CB*nthroot(aux1,NAWX);
// aux4c  = aux3.*(1-2*Swc)./(NAWX*aux1);

//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
extern double Swc1[], aux1[], aux3[]; 
extern double aux4c[]; 

// Swc    = (0.001/0.45)*ones(1,nlevel);
  for ( i = 1; i<= nlevel; i++ )
     Swc[i] = (0.001/0.45) * 1.0;

// aux1   = Swc.*(1-Swc);
  for ( i = 1; i <= nlevel; i++ ) 
    Swc1[i] = 1.0 - Swc[i];

  Vec_Mult (Swc, Swc1, aux1, nlevel); 

// aux3   = CB*nthroot(aux1,NAWX);
//double a,b,c;  /* pow() wants doubles - ?  */
  for ( i = 1; i <= nlevel; i++ ) { 
     b = aux1[i];
     c = NAWX; 
     a = xpow (b,1.0/c);
     aux3[i] = a * CB; }

// aux4c  = aux3.*(1-2*Swc)./(NAWX*aux1);
  for ( i = 1; i <= nlevel; i++ ) { 
    aux4c[i] = aux3[i] * (1.0-2.0 * Swc[i]) / (NAWX * aux1[i]);
  }

/*------------------------------------------------------------*/

//SwQQ   = 0:0.001:1;
//aux1   = SwQQ.*(1-SwQQ);
//aux3   = CB*nthroot(aux1,NAWX);
//if BRUSS == 1
//aux2   = (1-SwQQ).^(NB-1);
//Awax   = max(aux1.*aux2+aux3);
//else
//Awax   = max(aux3);
//end
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//SwQQ   = 0:0.001:1;
//double d; 
extern double SwQQ[], SwQQ1[];
extern int BRUSS;  
extern double aux2[]; 
  d = 0;
  for ( i = 1; i <= eC_SwQQ; i++ ) {
    SwQQ[i] = d;
    SwQQ1[i] = (1.0 - SwQQ[i]);
    if ( SwQQ1[i] < 0 ) 
      SwQQ1[i] = 0; 
    d += 0.001;  }

//aux1   = SwQQ.*(1-SwQQ); 
  Vec_Mult (SwQQ, SwQQ1, aux1, eC_SwQQ); 


//aux3   = CB*nthroot(aux1,NAWX);
  for ( i = 1; i <= eC_SwQQ; i++ ) 
    aux3[i] =  CB * nthroot (aux1[i],NAWX);


  if ( BRUSS == 1 ) {
    //aux2   = (1-SwQQ).^(NB-1);
    for ( i = 1; i <= eC_SwQQ; i++ ) 
      aux2[i] = xpow ( (1.0 - SwQQ[i]), (NB-1.0) );

    //Awax   = max(aux1.*aux2+aux3);
    Awax = 0; 
    for ( i = 1; i <= eC_SwQQ; i++ ) {
      d = aux1[i] * aux2[i] + aux3[i];
      if ( d > Awax ) 
        Awax = d;  }
    } /* if BRUSS */

    else {   /* BRUSS == 0 */
      Awax = 0; 
      for ( i = 1; i <= eC_SwQQ; i++ ) {
        if ( aux3[i] > Awax ) 
         Awax = aux3[i]; 
      } /* for */ 
    } /* else */


//%__________________________________________________________________________
//% Vapor Source Term Condensation coefficient (actemp)  

extern double actemp, tempoi;
extern double rgas, temp0; 
if ( DRYSV == 1 )
   actemp = 10000/rgas;
if ( NOVAK == 1 )
   actemp = 10000/rgas;

tempoi = 1.0/(tempai+temp0);


//%__________________________________________________________________________
//% Variable and parameters for the radiation-driven term of  
//% the thermal conductivity: Rp [m] = mean pore radius of the soil  

extern double stefRad,  stefbol;
double Rp;
Rp      = 10e-06;
stefRad = 3.8*stefbol*Rp;

//%__________________________________________________________________________
//% Variables to be used with the surface latent and sensible heat fluxes 
//% for the surface boundary condition 
//%
//%  CH = Transfer coefficient: surface sensible heat flux [m/s]
//%  CE = Transfer coefficient: surface latent heat flux [m/s]
//%  Htrans = Scaled CH = CH*(rho_a*(p_a/p_00)*C_pa)*temp0 [W/m^2] 
extern double CH, Htrans,presr,CE; 

CH     = 0.032;
Htrans = 1.29*presr*1010*CH*temp0;
// % CE      = 0.61*CH/0.71;
CE     = 0.0010;
if ( NOVAK == 1 ) 
  CE     = 0.0005;

//%__________________________________________________________________________
//%  Advective component of soil surface evaporation: Preferred model
extern double CU, CUNR; 
//%  Crank-Nicolson
CU      = 0.125;
//%  Newton-Raphson
CUNR    = 0;


  return 1; 
}


//*************************************************************************
//*************************************************************************
//*************************************************************************
//*************************************************************************




#ifdef WWWW
%--------------------------------------------------------------------------
%                           WesternUS01.m                            
%
%               Initialize soil physical and model constants                     
%--------------------------------------------------------------------------

%__________________________________________________________________________
%  nhalfhr = length of burn in one-half hour units [half-hours]
%  nmin    = length of burn in minutes [minutes]

longfire = 1;

nhrs = 60;
nmin = 60*nhrs;

%__________________________________________________________________________
% Input model temporal and spatial discretization parameters 

Model_Param

%__________________________________________________________________________
% Variables to be used with the model's initial and boundary conditions 
%
%  Qabs   = Energy input from heater [W/m^2]
%  vapori = inital atmospheric vapor density [Pa]
%  vaporf = final atmospheric vapor density [Pa]
%  tempaf = final atmospheric temperature [C]

Qabs    = 47000;
vapori  =  1000;
vaporf  = 15000;
tempaf  =   500;

%__________________________________________________________________________
%   Parameters for the original BFD curve = Forcing function for UBC  
%
%   tm = time (hrs) from inception of the fire at which 
%        the maximum forcing occurs 
%
%   td = duration or length of the fire (hrs) - Strictly speaking td is 
%        the difference between the two times at which the forcng = 0.01 - 
%        force(t1) = force(t2) = 0.01 and t1 < tm < t2 and td = t2-t1 - 

tm     = 9.25;
td     = 24;
a1td   = 2*log(10)/(asinh(0.5*td/tm))^2;

%__________________________________________________________________________
% Variables for Campbell's soil thermal conductivity model
% But new model only uses zlamm - Can be adjusted if necessary - 
% 2 <= zlamm (W/m/K) <= 14 is to be expected

global zlamm xw0 ga gc q0t 

zlamm   = 4.42;  
xw0     = 0.09;
ga      = 0.123;
q0      = 3;

gc      = 1.0-2.0*ga;      
q0t     = q0/(303*303);
 
%__________________________________________________________________________
% Variables to be used with the initial soil conditions  
% 
% thetar = residual soil moisture 
% [numf1 numf2] = thetar T_K and psin dependence term 
% thetai = initial volumetric soil moisture [m^3/m^3] excluding thetar
% psini  = inital valus of normalized soil water potential [ - ]
%          psini must be consistent with thetai-thetar and must 
%          be computed beforehand
% tempai = initial atmospheric temperature [C]
% den0   = soil bulk density [gm/cm^3]
% parden = soil particle density [g/cm^3]
% dg     = mean particle size [m]

thetai    = 0.07;
tempai    = 19.0;
den0      = 1.30;
parden    = 2.35;
dg0       = 20;
dg        = dg0*1e-06;

global thetar
global numf1 numf2

if THETAR == 1
  thetar  = 0.01;
  thetai  = thetai-thetar;
% LOW  thetai  = 0.120-thetar;
% HIGH thetai  = 0.190-thetar;
  psini   = 2.14841e-03;
  numf1   = 1.25;
  numf2   = 1/3;
else
  thetar  = 0;
if thetai <= 0.19
  a0CS    = -2.383;
  aCS     =  [ 3.722 -1.724  0.4272  -0.04202 0 0];
  bCS     =  [-7.484  1.294 -0.07394 -0.01343 0 0]; 
  wCS     =  7.614;
elseif (thetai > 0.19) && (thetai <=0.34)
  a0CS    = -10.88;
  aCS     =  [-2.243 -1.713  -0.2822  0.072   0.04757 0];
  bCS     =  [ 2.247 -0.7484 -0.7179 -0.2635 -0.02794 0]; 
  wCS     =  12.06;
elseif (thetai > 0.34) && (thetai <= 0.43)
  a0CS    = -13.55 ;
  aCS     =  [ 2.646 -1.548  0.5911 -0.1395 0.01572 0];
  bCS     =  [-1.687  1.028 -0.4124  0.101 -0.01162 0]; 
  wCS     =  16.34;
else
  error(' ***** initial value of theta exceeds 0.43 *****')    
end
  psini   = PSINi(thetai,a0CS,aCS,bCS,wCS); 
% p       = [0 0 0 0 0 0 4.786 -0.8029 -39.35 -0.0002096];
% psini   = PSINi_poly(thetai,p);
% psini   = 1.449242e-03;
% LOW  psini   = 8.863218e-03;
% HIGH psini   = 5.65627e-04;
  numf1   = 0;
  numf2   = 0;
end

%__________________________________________________________________________
% Parameters for temperature effects on the water retention curve

PsinTmpnT

%__________________________________________________________________________
% KInc = Intrinsic Hydraulic Conductivity [m^2] 

global KInc

Bear = 0.000617; 
KInc = Bear*dg*dg;

%__________________________________________________________________________
% Variable and parameters for the film flow term of 
% the hydraulic conductivity

global Rwfac Fwfac

facrw = 0.04*dg0*dg0*dg0;
facMx = 1000;
if facrw >= facMx
   facrw  = facMx;
end
fcrw  = sqrt(eps0/dg)*kBoltz/elec;
Rwfac = facrw*sqrt(2)*pi*pi*fcrw*fcrw*fcrw/Bear;

Fwfac = dg*psi0;

%__________________________________________________________________________
% Variables to be used with the Brooks-Corey, vanGenuchten-Assouline and  
%   VanGenuchten-Nielsen models of hydraulic conductivity

global bcexp 

brcr  = 6;
bcexp = 2*brcr+3;

global EA MA

if DRYSV == 1
   EA = 2.40;  
   MA = 0.20;   
end
if NOVAK == 1
   EA = 2.25;
   MA = 0.20;    
end

global MV MVe MVx

MV    = 0.20;
MVe   = 2/MV;
MVx   = 2 + 0.5*MV;

%__________________________________________________________________________
% Variables to be used with the surface diffusion model  

global thetam0 dsurf0

thetam0 = 0.02;
dsurf0  = 1e-10;

%__________________________________________________________________________
% Variables to be used with the Shiozawa-Campbell water rentention curve  
%   

global thetah alpha4 mexp
global thetan 

thetah  =  0.1414;
alphah  = -2.189*psi0/grav;
alpha4  =  alphah*alphah*alphah*alphah;
mexp    = -0.3608;
thetap  =  0.3508;
zlnpsi0 =  13.8155106;
thetan  =  thetap/zlnpsi0;

%__________________________________________________________________________
% Variables to be used with the Groenevelt-Grant water rentention curve 
%   and the hydraulic conductivity 
%   Note: NGb is NG for the hydraulic conductivity function GGHYB = 1
%     and NGm is NG for the hydraulic conductivity function GGHYD = 1

global thetaGf k1G NG kGp0 NGb NGm

% ++++ GGWRC ++++ 
% ++++ thetaGf = 1-thetar;
thetaGf = 1;
k1G     = 1.742;
NG      = 0.3413;
% k1G     = 4;
% NG      = 4/9;
kGp0    = k1G*grav/abs(psi0);
NGb     = 9*NG;
NGm     = 3.5*NG;

%__________________________________________________________________________
% Variables to be used with the Fredlund-Ying water rentention curve  

global mur almur mui Nexp Mexp

mur     = 323.6246e-06;
almur   = 1/(log(1+1/mur));
mui     = 2.94985251e-06;
Nexp    = 3.04;
Mexp    = 0.3099;
if THETAR == 1
  psiniFY = 1.583416e-03;
else
if thetai <= 0.19
  a0FY    = -2.415;
  aFY     =  [ 4.416 -2.553   0.5845 -0.02512 -0.007538 0];
  bFY     =  [-5.94   0.09777 0.4812 -0.1467   0.01235  0]; 
  wFY     =  5.173;
elseif (thetai > 0.19) && (thetai <=0.34)
  a0FY    =  -29.97;
  aFY     =   [-12.33 7.267 2.82   0 0 0];
  bFY     =   [28.24  13    0.5732 0 0 0]; 
  wFY     =   3.245;
elseif (thetai > 0.34) && (thetai <= 0.43)
  a0FY    = -19.04;
  aFY     =  [-6.752  3.19  1.839  0.05515 0 0];
  bFY     =  [-9.459 -5.258 0.2943 0.2463  0 0]; 
  wFY     =  5.532;
else
  error(' ***** initial value of theta exceeds 0.43 *****')    
end
  psiniFY = PSINi(thetai*parden/(parden-den0),a0FY,aFY,bFY,wFY);     
% p       = [0 0 0 0 0 0 -14.16 18.66 -25.19 0.002464];
% psiniFY = PSINi_poly(thetai*parden/(parden-den0),p);
% psiniFY = 1.248083e-03;
% LOW  psiniFY = 6.272000e-03;
% HIGH psiniFY = 5.39777e-04;
end

%__________________________________________________________________________
% Variables to be used with the hydraulic function constructed from the 
% Fredlund-Xing water retention curve -- See calhydauWRC.m  

global HYD0 HYD1 HYD2 HYDP

HYD0    = -448;
HYD1    =  414;
HYD2    =  0.004946;
HYDP    =  0.005;

%__________________________________________________________________________
% Water acivity parameters for the Type V adsorption isotherm (Cherblanc)
%   using my modified Kelvin Equation

global AWA BWA

AWA    = 0.8;
BWA    = 2;

%__________________________________________________________________________
% Water acivity parameters for the Cherblanc adsorption isotherm 

global w0Cden AWC BWC

w0Cden =  1/(den0*0.0028); 
AWC    =  2.9;
BWC    = -1.9;

%__________________________________________________________________________
% Vapor Source Term Coefficient [Svcoef] and parameters for Awa model 

global Svcoef NB CB NAWX Swc aux4c Awax

if DRYSV == 1
   Svcoef = 0.3;
end
if NOVAK == 1
   Svcoef = 0.3;
end
NB     = 50;
CB     = 0.008;
NAWX   = 8;
Swc    = (0.001/0.45)*ones(1,nlevel);
aux1   = Swc.*(1-Swc);
aux3   = CB*nthroot(aux1,NAWX);
aux4c  = aux3.*(1-2*Swc)./(NAWX*aux1);

SwQQ   = 0:0.001:1;
aux1   = SwQQ.*(1-SwQQ);
aux3   = CB*nthroot(aux1,NAWX);
if BRUSS == 1
aux2   = (1-SwQQ).^(NB-1);
Awax   = max(aux1.*aux2+aux3);
else
Awax   = max(aux3);
end

%__________________________________________________________________________
% Vapor Source Term Condensation coefficient (actemp)  

global actemp tempoi

if DRYSV == 1
   actemp = 10000/rgas;
end
if NOVAK == 1
   actemp = 10000/rgas;
end
tempoi = 1/(tempai+temp0);

%__________________________________________________________________________
% Variable and parameters for the radiation-driven term of  
% the thermal conductivity: Rp [m] = mean pore radius of the soil  

global stefRad

Rp      = 10e-06;
stefRad = 3.8*stefbol*Rp;
 
%__________________________________________________________________________
% Variables to be used with the surface latent and sensible heat fluxes 
% for the surface boundary condition 
%
%  CH = Transfer coefficient: surface sensible heat flux [m/s]
%  CE = Transfer coefficient: surface latent heat flux [m/s]
%  Htrans = Scaled CH = CH*(rho_a*(p_a/p_00)*C_pa)*temp0 [W/m^2] 

CH     = 0.032;
Htrans = 1.29*presr*1010*CH*temp0;
% CE      = 0.61*CH/0.71;
CE     = 0.0010;
if NOVAK == 1 
CE     = 0.0005;
end

%__________________________________________________________________________
%  Advective component of soil surface evaporation: Preferred model

%  Crank-Nicolson
CU      = 0.125;
%  Newton-Raphson
CUNR    = 0;
#endif 