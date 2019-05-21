/*****************************************************************************
* Name: Quincy1G.cpp
* Desc: --->>  BM's updated version, received on 8-22-2017
*
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

#define Quincy 

#ifdef Quincy
#define e_Qabs    31000.0
#define e_vapori  1000.0
#define e_vaporf  5000.0
#define e_tempaf  400.0
#define e_tauh    300.0  
#elif Manitou
#define e_Qabs =  16000.0
#define e_vapori  300.0
#define e_vaporf  35000.0
#define e_tempaf  320.0
#define e_tauh    300.0  
#else 
#define e_Qabs -1.0 
#endif 

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
double  CUNR; 
double Swc  [eC_MaxLev];
double Swc1 [eC_MaxLev];
double aux4c[eC_MaxLev];


double SwQQ [eC_SwQQ + 1 ];
double SwQQ1[eC_SwQQ + 1 ];
double aux1 [eC_SwQQ + 1 ]; 
double aux2 [eC_SwQQ + 1 ];
double aux3 [eC_SwQQ + 1 ];
double Awax; 

extern int THETAR, GGWRC ; 
extern int GGHYB, GGHYD;
extern int BRUSS; 
double HYD0, HYD1, HYD2, HYDP;
double q0;
double  dg0,dg; 

/*------------------------------------------------------*/
int Quincy1G(d_BMI *bmi)
{
double d, d_Max;
float f; 

  strcpy (bmi->cr_ErrMes,""); 

//%  nmin = length of lab heating experiment [minutes]
  if ( bmi->d_Minutes > 0 ) 
     nmin = bmi->d_Minutes; 
//  else
//     nmin = e_Minutes;

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

  Qabs   =  e_Qabs;       //  31000.0;
  vapori =  e_vapori;     //  1000;
  vaporf =  e_vaporf;     //  5000;
  tempaf =  e_tempaf;     //  400;
  tauh   =  e_tauh;        // 300.0;  

//%__________________________________________________________________________
//% Variables for Campbell's soil thermal conductivity model
//  global zlamm xw0 ga gc q0t    

  zlamm   = 15;     //  orig 12.0; 
  xw0     = 0.091;
  ga      = 0.090;
  q0      = 3.01;

  gc      = 1.0 - 2.0 * ga;      
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

 if  ( THETAR == 1 ) {
   thetar  = 0.01;
//   psini   = xxxxxxe-01; 
   strcpy (bmi->cr_ErrMes,"Quincy1G() fix this - psini = xxxxxxe-01 "); 
   return 0;  }   
 else {
   thetar  = 0;
   psini   = e_SoiWatPot;   /* normalized soil water potential  */
   numf1   = 0;
   numf2   = 0;
 }  // end


  thetai    = 0.142-thetar;
  tempai    = 21.3;
  den0      = 1.56;
  parden    = 2.82;
  dg0       = 180;
  dg        = dg0*1e-06;

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
brcr  = 3.75;
bcexp = 2.0*brcr+3.0;

extern int DRYSV; 
if (DRYSV == 1 ) {
  EA = 2.25;
  MA = 0.27; }
  
extern int NOVAK;
if ( NOVAK == 1 ) { 
   EA = 2.25;
   MA = 0.26; }

// global MV MVe MVx
MV    = 0.25;
MVe   = 2.0/MV;
MVx   = 2.0 + 0.5 * MV;


//----------------
// AAA
//--------------------


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

thetah  =  0.23;
alphah  = -0.5*psi0/grav;
alpha4  =  alphah*alphah*alphah*alphah;
mexp    = -1.0/3.0;
w1      =  0.061;
thetap  =  0.19;
zlnpsi0 =  13.8155106;
thetan  =  thetap/zlnpsi0;

//%__________________________________________________________________________
//% Variables to be used with the hydraulic function constructed from the 
//% Shiozawa-Campbell WRC -- See calhydauWRC.m & calhydauWRCt.m 
// I don't see where these get used anywhere else

HYD0    = -55.6;
HYD1    = 16.0;
HYD2    = 3.0/40.0;
HYDP    =  1.0 / 6.0 ;
 
//%__________________________________________________________________________
//% Variables to be used with the Groenevelt-Grant water rentention curve 
//%   and the hydraulic conductivity 
//%   Note: NGb is NG for the hydraulic conductivity function GGHYB = 1
//%     and NGm is NG for the hydraulic conductivity function GGHYD = 1
//global thetaGf k1G NG kGp0 NGb NGm
//% ++++ GGWRC ++++ 
//% ++++ thetaGf = 1-thetar;
thetaGf = 1.0;
k1G     = 1.8;
NG      = 0.45;
kGp0    = k1G*grav/abs(psi0);
NGb     = 3.1;
NGm     = 1.4;

//%__________________________________________________________________________
//% Variables to be used with the Fredlund-Ying water rentention curve  
// global mur almur mui Nexp Mexp

mur     =  360e-06;
almur   = 1.0/(xlog(1.0+1.0/mur));
mui     = 11e-06;
Nexp    = 1.0;
Mexp    = 1.0;

  if ( THETAR == 1 ) {
     psiniFY = 0;   // not sure what this means in .m --> XXXXXXXe-05;
     strcpy (bmi->cr_ErrMes,"Quincy1G() fix this, XXXXXXe-05"); 
     return 0; 
  }
  else
    psiniFY = 1.868114e-04;
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

extern int DRYSV;
 if ( DRYSV == 1 ) 
   Svcoef = 0.50;
 if ( NOVAK == 1 ) 
   Svcoef = 0.50; 

NB     = 40.0;
CB     = 0.003;
NAWX   = 8.0;

//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
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

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//SwQQ   = 0:0.001:1;
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
// global actemp tempoi
extern int MASSO, CRANK; 
extern double rgas,temp0; 

actemp = 40000.0 / rgas;      
if (CRANK != 1)
   actemp = 30000.0/rgas;
    
tempoi = 1.0/(tempai+temp0);


//%__________________________________________________________________________
//% Variable and parameters for the radiation-driven term of  
//% the thermal conductivity: Rp [m] = mean pore radius of the soil  
// global stefRad
double Rp; 
extern double stefbol; 
Rp      = 70e-06;       // orig 1e-03;
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
Htrans = 1.29*presr*1010.0*CH*temp0;
CE     = 0.0010;

if ( NOVAK == 1 ) 
  CE     = 0.0005;





//%__________________________________________________________________________
//%  Advective component of soil surface evaporation: Preferred model
//%  Crank-Nicolson
CU      = 0.125;

//%  Newton-Raphson
CUNR    = 0;

   return 1;   
} 



#ifdef wowowo

Bills Update version - 


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
vapori =  1000;
vaporf =  5000;
% tempaf =   600;
tempaf =   400;
tauh   =   100;

%__________________________________________________________________________
% Variables for Campbell's soil thermal conductivity model
% NOTE: As of 05/2017 the only parameter being used to compute  
%       soil thermal conducitivy is zlamm, which is now an 
%       adjustable parameter 

global zlamm xw0 ga gc q0t

zlamm   = 8;
% zlamm   = 2.61; 
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

brcr  = 3.5;
bcexp = 2*brcr+3;

global EA MA

if DRYSV == 1
   EA = 2.40;
   MA = 0.30;
%  EA = 2.25;
%  MA = 0.30;
end
if NOVAK == 1
   EA = 2.25;
   MA = 0.25;   
end

global MV MVe MVx

MV    = 0.26;
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
% Variables to be used with the hydraulic function constructed from the 
% Shiozawa-Campbell WRC -- See calhydauWRC.m & calhydauWRCt.m 

global HYD0 HYD1 HYD2 HYDP

HYD0    = -26.5;
HYD1    = -2.283;
HYD2    = -1.388;
HYDP    =  1/3;

% global HYD1 HYD2 HYD3 HYD4 HYDS

% HYD1    = -7.978;
% HYD2    =  2.048;
% HYD3    = -21.09;
% HYD4    =  13.6;
% HYDS    =  0;

%__________________________________________________________________________
% Variables to be used with the Groenevelt-Grant water rentention curve 
%   and the hydraulic conductivity 
%   Note: NGb is NG for the hydraulic conductivity function GGHYB = 1
%     and NGm is NG for the hydraulic conductivity function GGHYD = 1

global thetaGf k1G NG kGp0 NGb NGm

% ++++ GGWRC ++++ 
% ++++ thetaGf = 1-thetar;
thetaGf = 1;
k1G     = 1.8;
NG      = 0.55;
kGp0    = k1G*grav/abs(psi0);
NGb     = 0.30;
NGm     = 0.30;

%__________________________________________________________________________
% Variables to be used with the Fredlund-Ying water rentention curve  

global mur almur mui Nexp Mexp

mur     = 360e-06;
almur   = 1/(log(1+1/mur));
mui     = 11e-06;
Nexp    = 6;
Mexp    = 0.60;
if THETAR == 1;
  psiniFY = XXXXXXXe-05;
else
psiniFY = 3.320097e-05;
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

if DRYSV == 1
   Svcoef = 0.75;
end
if NOVAK == 1
   Svcoef = 1;
end
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

% actemp = 0.90;
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
% CE     = 0.0013;
CE     = 0.0010;
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