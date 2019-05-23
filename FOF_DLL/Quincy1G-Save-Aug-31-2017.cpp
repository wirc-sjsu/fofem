#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

double nmin; 
double  zlamm, xw0, ga, gc, q0t; 
double thetar;
double numf1, numf2;
double KInc; 
double Rwfac, Fwfac;
double  bcexp; 
double  EA, MA;
double MV, MVe, MVx;
double  thetam0, dsurf0;
double  thetah, alpha4, mexp;
double  thetan; 
double thetaGf, k1G, NG, kGp0, NGb, NGm;
double  mur, almur, mui, Nexp, Mexp;
double psiniFY;
double  AWA, BWA;
double w0Cden, AWC, BWC;
double Svcoef, NB, CB, NAWX;
double actemp, tempoi;
double stefRad;
double  tauh;
double Qabs; 
double vaporf, vapori; 
double  tempaf, tempai;
double  thetai,  den0;
double parden;
double  psini;
double CH,CE,Htrans;
double CU;

double Swc  [eC_MaxLev];
double aux1 [eC_MaxLev]; 
double Swc1 [eC_MaxLev];
double aux3 [eC_MaxLev];
double aux4c[eC_MaxLev];

// int nthroot (double *A, double n, double P);

/*------------------------------------------------------*/
int Quincy1G(d_BMI *bmi)
{
//%  nmin = length of lab heating experiment [minutes]
  if ( bmi->d_Minutes > 0 ) 
     nmin = bmi->d_Minutes; 
  else
     nmin = e_Minutes;

//%__________________________________________________________________________
//% Input model temporal and spatial discretization parameters 

   if ( !Model_Param(bmi) )
      return 0;                     /* Error */  

//%__________________________________________________________________________
//% Variables to be used with the model's initial and boundary conditions 
//%
//%  Qabs   = Energy input from heater [W/m^2]
//%  vapori = inital atmospheric vapor density [Pa]
//%  vaporf = final atmospheric vapor density [Pa]
//%  tempaf = final atmospheric temperature [C]
//%  tauh   = time constant of the heater [s]

  Qabs = 29400.0;

extern int THETAR, GGWRC ; 
extern int GGHYB, GGHYD;
extern int FYWRC; 

if ( THETAR == 1 && GGWRC == 0 )
  Qabs   = 32000.0 ;

if ( GGHYB == 1 || GGHYD == 1 )
  Qabs   = 33000;

if ( FYWRC == 1 )
  Qabs   = 44000;

/* User GUI input value */ 
if ( bmi->f_Qabs > 0 ) 
  Qabs = bmi->f_Qabs; 


  vapori = 1000;
  vaporf = 5000;
  tempaf = 600;
   tauh   = 300;


//%__________________________________________________________________________
//% Variables for Campbell's soil thermal conductivity model
//  global zlamm xw0 ga gc q0t    

zlamm   = 2.61; 
xw0     = 0.091;
ga      = 0.090;
double q0      = 3.01;

gc      = 1.0-2.0*ga;      
q0t     = q0/(303.0*303.0);
 
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
// global thetar
// global numf1 numf2
double  dg0,dg;  




 if  ( THETAR == 1 ) {
   thetar  = 0.03;
   psini   = 7.261885e-05;
   numf1   = 1.25;
   numf2   = 1.0/3.0;
   if ( GGWRC == 1 ) {
    numf1   = 0.30;
    numf2   = 0; }
 }   //  end

 else {
   thetar  = 0;
   psini   = 5.79629e-05;
   numf1   = 0;
   numf2   = 0;
 }  // end

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* Code added to use GUI moisture input */ 
double d; 
   if ( bmi->f_Moist > 0 )     /* make something is there */ 
     d = bmi->f_Moist;
   else 
     d = 0.142;                /* Original defalut value */ 
   thetai = d - thetar; 
// thetai    = 0.142-thetar;   /* Original code */ 

/* Normalized Soil Water Potential - REPLACE what was assigned above */ 
if ( bmi->d_SoiWatPot > 0 ) 
  psini = bmi->d_SoiWatPot;

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-*/

tempai    = 21.3;
den0      = 1.56;
parden    = 2.82;
dg0       = 180.0;
dg        = dg0*1e-06;


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* Soil Bulk & Particle density from GUI */
  if ( bmi->f_SoiBulDen > 0 ) 
    den0 = bmi->f_SoiBulDen; 
  if ( bmi->f_SoiParDen > 0 ) 
     parden = bmi->f_SoiParDen; 
/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-*/





//%__________________________________________________________________________
//% Parameters for temperature effects on the water retention curve

 PsinTmpnT (tempai);

//%__________________________________________________________________________
//% KInc = Intrinsic Hydraulic Conductivity [m^2] 

double Bear = 0.000617; 
      KInc = Bear*dg*dg;


//%__________________________________________________________________________
//% Variable and parameters for the film flow term of 
//% the hydraulic conductivity

extern double eps0,kBoltz,elec,pi,psi0; 
double   facMx,fcrw,facrw;

facrw = 0.04*dg0*dg0*dg0;
facMx = 1000;
if (facrw >= facMx ) {
   facrw  = facMx;
  } // end
fcrw  = xsqrt(eps0/dg) * kBoltz/elec;
Rwfac = facrw*xsqrt(2.0)*pi*pi*fcrw*fcrw*fcrw/Bear;

Fwfac = dg*psi0;

//%__________________________________________________________________________
//% Variables to be used with the Brooks-Corey, vanGenuchten-Assouline and  
//%   VanGenuchten-Nielsen models of hydraulic conductivity

//global bcexp 
double brcr; 
brcr  = 2.95;
bcexp = 2.0*brcr+3.0;

//global EA MA
EA = 1.80;
MA = 0.26;

extern int NOVAK;
if ( NOVAK == 1 ) 
   EA = 1.90;

// global MV MVe MVx
MV    = 0.30;
MVe   = 2.0/MV;
MVx   = 2.0 + 0.5*MV;

//%__________________________________________________________________________
//% Variables to be used with the surface diffusion model  
//global thetam0 dsurf0

thetam0 = 0.02;
dsurf0  = 1e-10;

//%__________________________________________________________________________
//% Variables to be used with the Shiozawa-Campbell water rentention curve  
//%   
//% thetah = 0.25*den0 [m3/m3]          
//% alphah = alpha*psi0/grav [-] = nondimensional coefficient
//% mexp   = exponent [-]
//% w1     = extrapolated water content [kg/kg] 
//% thetap = 0.75*w1*den0 [m3/m3] = Campbell's theta1

//global thetah alpha4 mexp
//global thetan 
double  alphah, w1,thetap,zlnpsi0; 
extern double grav; 

thetah  =  0.25*den0;
alphah  = -0.5*psi0/grav;
alpha4  =  alphah*alphah*alphah*alphah;
mexp    = -1.0/3.0;
w1      =  0.061;
thetap  =  0.75*w1*den0;
zlnpsi0 =  13.8155106;
thetan  =  thetap/zlnpsi0;

//%__________________________________________________________________________
//% Variables to be used with the Groenevelt-Grant water rentention curve 
//%   and the hydraulic conductivity 
//%   Note: NGb is NG for the hydraulic conductivity function GGHYB = 1
//%     and NGm is NG for the hydraulic conductivity function GGHYD = 1
//global thetaGf k1G NG kGp0 NGb NGm
//% ++++ GGWRC ++++ 
//% ++++ thetaGf = 1-thetar;
thetaGf = 0.9999;
k1G     = 3.0;
NG      = 0.44;
kGp0    = k1G*grav/abs(psi0);
NGb     = 0.225;
// % NGb     = 0.250;
NGm     = 0.180;

//%__________________________________________________________________________
//% Variables to be used with the Fredlund-Ying water rentention curve  
// global mur almur mui Nexp Mexp

mur     = 9e-06;
almur   = 1.0/(xlog(1.0+1.0/mur));
mui     = 30e-06;
Nexp    = 3.0;
Mexp    = 0.90;

  if ( THETAR == 1 )
     psiniFY = 6.346158e-05;
  else
// %2-40-10-.7 psiniFY = 5.548941e-05;
// %2-40-04-.7 psiniFY = 8.560533e-05;
// %5-50-05-.9 psiniFY = 8.158466e-05;
    psiniFY = 7.265405e-05;
// end

//%__________________________________________________________________________
//% Water acivity parameters for the Type V adsorption isotherm (Cherblanc)
//%   using my modified Kelvin Equation
// global AWA BWA
AWA    = 0.8;
BWA    = 2.0;

//%__________________________________________________________________________
//% Water acivity parameters for the Cherblanc adsorption isotherm 
//global w0Cden AWC BWC

w0Cden =  1.0/(den0*0.0028); 
AWC    =  2.9;
BWC    = -1.9;

//%__________________________________________________________________________
//% Vapor Source Term Coefficient [Svcoef] and parameters for Awa model 
//global Svcoef NB CB NAWX Swc aux4c
extern double nlevel; 
int i; 
Svcoef = 0.86;
NB     = 40.0;
CB     = 0.003;
NAWX   = 8.0;

// Swc    = (0.001/0.45)*ones(1,nlevel);

  for ( i = 1; i<= nlevel; i++ )
     Swc[i] = (0.001/0.45) * 1.0;

// aux1   = Swc.*(1-Swc);
 
  for ( i = 1; i <= nlevel; i++ ) 
    Swc1[i] = 1.0 - Swc[i];

  Vec_Mult (Swc, Swc1, aux1, nlevel); 

// aux3   = CB*nthroot(aux1,NAWX);
double a,b,c;  /* pow() wants doubles - ?  */

  for ( i = 1; i <= nlevel; i++ ) { 
     b = aux1[i];
     c = NAWX; 
     a = xpow (b,1.0/c);
     aux3[i] = a * CB; }

// aux4c  = aux3.*(1-2*Swc)./(NAWX*aux1);

  for ( i = 1; i <= nlevel; i++ ) { 
    aux4c[i] = aux3[i] * (1.0-2.0 * Swc[i]) / (NAWX * aux1[i]);
  }

//%__________________________________________________________________________
//% Vapor Source Term Condensation coefficient (actemp)  
// global actemp tempoi
extern int MASSO, CRANK; 
extern double rgas,temp0; 

actemp = 30000/rgas;
if  (MASSO == 1)
   actemp = 39000.0/rgas;
       
if (CRANK != 1)
   actemp = 30000.0/rgas;
    
tempoi = 1.0/(tempai+temp0);


//%__________________________________________________________________________
//% Variable and parameters for the radiation-driven term of  
//% the thermal conductivity: Rp [m] = mean pore radius of the soil  
// global stefRad
double Rp; 
extern double stefbol; 
Rp      = 1e-03;
stefRad = 3.8*stefbol*Rp;


//%__________________________________________________________________________
//% Variables to be used with the surface latent and sensible heat fluxes 
//% for the surface boundary condition 
//%
//%  CH = Transfer coefficient: surface sensible heat flux [m/s]
//%  CE = Transfer coefficient: surface latent heat flux [m/s]
//%  Htrans = Scaled CH = CH*(rho_a*(p_a/p_00)*C_pa)*temp0 [W/m^2] 
extern double presr; 


CH     = 0.032;
Htrans = 1.29*presr*1010*CH*temp0;
CE     = 0.0013;

if  ( GGHYB == 1 || NOVAK == 1 || GGHYD == 1 || GGWRC == 1 )
  CE     = 0.0005;

//%__________________________________________________________________________
//%  Advective component of soil surface evaporation: Preferred model
//%  Crank-Nicolson
double  CUNR; 
CU      = 0;
//%  Newton-Raphson
CUNR    = 0;

   return 1;   
} 



#ifdef wowowo
%--------------------------------------------------------------------------
%                                Quincy1G.m                            
%
%               Initialize soil physical and model constants                     
%--------------------------------------------------------------------------

%__________________________________________________________________________
%  nmin = length of lab heating experiment [minutes]

nmin = 90;

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
%  tauh   = time constant of the heater [s]

Qabs   = 29400;
if THETAR == 1 && GGWRC == 0;
Qabs   = 32000;
end
if GGHYB == 1 || GGHYD == 1;
Qabs   = 33000;
end
if FYWRC == 1;
Qabs   = 44000;
end
vapori = 1000;
vaporf = 5000;
tempaf = 600;
tauh   = 300;

%__________________________________________________________________________
% Variables for Campbell's soil thermal conductivity model

global zlamm xw0 ga gc q0t 

zlamm   = 2.61; 
xw0     = 0.091;
ga      = 0.090;
q0      = 3.01;

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

global thetar
global numf1 numf2

if THETAR == 1;
%-thetar  = 0.02;
%-psini   = 6.66160e-05;
  thetar  = 0.03;
  psini   = 7.261885e-05;
% numf1   = 0.30;
% numf2   = 0.25;
  numf1   = 1.25;
  numf2   = 1/3;
  if GGWRC == 1;
  numf1   = 0.30;
  numf2   = 0;
  end
%-thetar  = 0.032;
%-psini   = 7.401147e-05;
else
  thetar  = 0;
  psini   = 5.79629e-05;
  numf1   = 0;
  numf2   = 0;
end
thetai    = 0.142-thetar;
tempai    = 21.3;
den0      = 1.56;
parden    = 2.82;
dg0       = 180;
dg        = dg0*1e-06;

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
if facrw >= facMx;
   facrw  = facMx;
end
fcrw  = sqrt(eps0/dg)*kBoltz/elec;
Rwfac = facrw*sqrt(2)*pi*pi*fcrw*fcrw*fcrw/Bear;

Fwfac = dg*psi0;

%__________________________________________________________________________
% Variables to be used with the Brooks-Corey, vanGenuchten-Assouline and  
%   VanGenuchten-Nielsen models of hydraulic conductivity

global bcexp 

brcr  = 2.95;
bcexp = 2*brcr+3;

global EA MA

% thetar EA = 2.25;
% thetar MA = 0.20;
EA = 1.80;
MA = 0.26;
if NOVAK == 1;
   EA = 1.90;
end

global MV MVe MVx

MV    = 0.30;
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
% thetah = 0.25*den0 [m3/m3]          
% alphah = alpha*psi0/grav [-] = nondimensional coefficient
% mexp   = exponent [-]
% w1     = extrapolated water content [kg/kg] 
% thetap = 0.75*w1*den0 [m3/m3] = Campbell's theta1

global thetah alpha4 mexp
global thetan 

thetah  =  0.25*den0;
alphah  = -0.5*psi0/grav;
alpha4  =  alphah*alphah*alphah*alphah;
mexp    = -1/3;
w1      =  0.061;
thetap  =  0.75*w1*den0;
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
thetaGf = 0.9999;
k1G     = 3;
NG      = 0.44;
kGp0    = k1G*grav/abs(psi0);
NGb     = 0.225;
% NGb     = 0.250;
NGm     = 0.180;

%__________________________________________________________________________
% Variables to be used with the Fredlund-Ying water rentention curve  

global mur almur mui Nexp Mexp

mur     = 9e-06;
almur   = 1/(log(1+1/mur));
mui     = 30e-06;
Nexp    = 3;
Mexp    = 0.90;
if THETAR == 1;
  psiniFY = 6.346158e-05;
else
%2-40-10-.7 psiniFY = 5.548941e-05;
%2-40-04-.7 psiniFY = 8.560533e-05;
%5-50-05-.9 psiniFY = 8.158466e-05;
psiniFY = 7.265405e-05;
end

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

global Svcoef NB CB NAWX Swc aux4c

Svcoef = 0.86;
NB     = 40;
CB     = 0.003;
NAWX   = 8;
Swc    = (0.001/0.45)*ones(1,nlevel);
aux1   = Swc.*(1-Swc);
aux3   = CB*nthroot(aux1,NAWX);
aux4c  = aux3.*(1-2*Swc)./(NAWX*aux1);

%__________________________________________________________________________
% Vapor Source Term Condensation coefficient (actemp)  

global actemp tempoi

actemp = 30000/rgas;
if MASSO == 1;
actemp = 39000/rgas;
end       
if CRANK ~= 1;
actemp = 30000/rgas;
end    
tempoi = 1/(tempai+temp0);

%__________________________________________________________________________
% Variable and parameters for the radiation-driven term of  
% the thermal conductivity: Rp [m] = mean pore radius of the soil  

global stefRad

% Rp      = 4e-03;
Rp      = 1e-03;
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
CE     = 0.0013;
if GGHYB == 1 || NOVAK == 1 || GGHYD == 1 || GGWRC == 1;
% CE     = 0.0009;
CE     = 0.0005;
end

%CE      = 0.61*CH/0.71;

%__________________________________________________________________________
%  Advective component of soil surface evaporation: Preferred model

%  Crank-Nicolson
CU      = 0;
%  Newton-Raphson
CUNR    = 0;

#endif