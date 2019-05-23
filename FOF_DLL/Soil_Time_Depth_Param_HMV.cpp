
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

extern double tempki  [eC_MaxLev]; 

extern double nlevel; 
extern double den0, thetai, thetar, parden, tempai;
extern double psini;  
extern int GGWRC,CSWRC,FYWRC; 
extern int TMPWRC; 
extern double thetaGf; 


extern double psiniFY;

double deltheta;
double psin    [eC_MaxLev]; 
double Rcoef   [eC_MaxLev]; 
double dRcdT   [eC_MaxLev]; 
double dRcdp   [eC_MaxLev];       
double thetaG  [eC_MaxLev];       
double thpsin  [eC_MaxLev]; 
double thpsini [eC_MaxLev]; 
double thtemp  [eC_MaxLev];            
double tempk   [eC_MaxLev]; 
double tempr   [eC_MaxLev]; 
double rhov    [eC_MaxLev]; 
double den     [eC_MaxLev];
double xm    [eC_MaxLev];
double poros [eC_MaxLev];
double temp  [eC_MaxLev];
double theta [eC_MaxLev];
double csd   [eC_MaxLev];      
double vapres[eC_MaxLev];      
double theto [eC_MaxLev];
double tempo [eC_MaxLev];
double rhovo [eC_MaxLev];


/*********************************************************************
* Name: Soil_Time_Depth_Param_HMV
*
*********************************************************************/
int Soil_Time_Depth_Param_HMV(d_BMI *bmi)
{
int i; 
  strcpy (bmi->cr_ErrMes,""); 

// % Input soil initial conditions and related model parameters
  if ( !Soil_Model_Data_Files_HMV(bmi))
     return 0;  

//%__________________________________________________________________________
//% Calculate the soil profiles of soil density (den),
//%   mineral fraction (xm), total porosity (poros), 
//%   initial soil temperature (temp), initial soil moisture (theta),  
//%   and the volumetric specific heat as a funciton of density (cpd)
//[den xm poros temp theta csd] = calprofP(den0,tempai,thetai+thetar,parden,nlevel);
 
  calprofP (den, xm, poros, temp, theta, csd,              /* Return values */
             den0, tempai, thetai+thetar, parden, nlevel);  /* Ingoing */ 

//%__________________________________________________________________________
//% Calculate the initial profile of the normalized soil water 
//%   potential (psin) and the derivative dtheta/dpsi (thpsin), 
//%   and its inverse (thpsini)  
//%   from an appropriate Water Retention Curve 
// [Rcoef dRcdT dRcdp] = calConCoef2(psini,temp);   

   calConCoef2(Rcoef, dRcdT, dRcdp, psini, temp, nlevel );

   if ( GGWRC == 1 ) 
      printf ("GGWRC == 1 Not Implemented "); 

   else if ( CSWRC == 1 ) {
      calpsinProf(psin,psini,nlevel); 

      if  ( TMPWRC == 0 ) 
        calthetaCSr (theta, thpsin, thpsini, thtemp,   Rcoef, dRcdT, dRcdp, psin,   nlevel);       /* ingoing */ 
      else 
         printf (" calthetaCSnT() - Not Implemented \n"); 
      
      for ( i = 1; i <= nlevel; i++ ) 
        thetaG[i] = thetaGf * poros[i];
   }  /* else if CSWRC */ 

   else if ( FYWRC == 1 )  {
     calpsinProf (psin, psiniFY, nlevel);
  } /* else */ 

  if ( TMPWRC == 0 ) {
    calthetaFYr(  theta, thpsin, thpsini, thtemp,  Rcoef,dRcdT,dRcdp, psin, poros, nlevel );
    for ( i = 1; i <= nlevel; i++ )
      thetaG[i] = thetaGf * poros[i];
  } /* if TMPWRC */ 

  deltheta = thetai + thetar - theta[1]; 


//%__________________________________________________________________________
//% Calculate Kelvin temperature (tempk), its inverse (tempki), and the 
//%          reduced temperature (tempr)
//% Assign initial profiles of vapor density (rhov) and pressure (vapres) 
//%   Assume the initial profile of rhov is at equilibrium, therefore 
//%   rhov = rhoveq = rh*csat according to the source term (sourcev) 

// [tempk tempki tempr] = caltempkHMV(temp);


  caltempkHMV(tempk, tempki, tempr, temp, nlevel);

  rhoveqHMV(rhov, tempk, psin, nlevel);

  calvaporHMV (vapres, rhov, tempk, nlevel );

//%__________________________________________________________________________
//% Initialize the old variables from the previous time step
   for ( i = 1; i <= nlevel; i++ ) { 
     theto[i] = theta[i];
     tempo[i] = temp[i];
     rhovo[i] = rhov[i];  }

   return 1;
}


#ifdef XXXXXX
%--------------------------------------------------------------------------
%
%                       Soil_Time_Depth_Param_HMV.m                         
%
%--------------------------------------------------------------------------

%__________________________________________________________________________
% Input soil initial conditions and related model parameters

Soil_Model_Data_Files_HMV

%__________________________________________________________________________
% Calculate the soil profiles of soil density (den),
%   mineral fraction (xm), total porosity (poros), 
%   initial soil temperature (temp), initial soil moisture (theta),  
%   and the volumetric specific heat as a funciton of density (cpd)

[den xm poros temp theta csd] = ... 
     calprofP(den0,tempai,thetai+thetar,parden,nlevel);
 
%__________________________________________________________________________
% Calculate the initial profile of the normalized soil water 
%   potential (psin) and the derivative dtheta/dpsi (thpsin), 
%   and its inverse (thpsini)  
%   from an appropriate Water Retention Curve 

[Rcoef dRcdT dRcdp] = calConCoef2(psini,temp);    

if GGWRC == 1;
% *** Groenevelt-Grant Water Retention Curve ------------------------------
  if TMPWRC == 0;
%----[psin thpsin thpsini thetaG thtemp]  = calpsinGG(theta,poros,nlevel);
     [psin thpsin thpsini thetaG thtemp] = ... 
                  calpsinGGr(theta,poros,Rcoef,dRcdT,dRcdp);         
  else
     [psin thpsin thpsini thetaG thtemp psiT] = ...
                  calpsinGGnT(theta,poros,temp,tempai);
%----[psin thpsin thpsini thetaG thtemp psiT] = ... 
%----             calpsinGGnTa(theta,poros,tempki);         
  end

elseif CSWRC == 1;
% *** Campbell-Shiozawa Water Retention Curve -----------------------------
  psin = calpsinProf(psini,nlevel);   
  if TMPWRC == 0;
%----[theta thpsin thpsini thtemp] = calthetaCS(psin,nlevel);
     [theta thpsin thpsini thtemp] = ... 
                   calthetaCSr(Rcoef,dRcdT,dRcdp,psin);
  else
     [theta thpsin thpsini thtemp psiT] = calthetaCSnT(psin,temp,tempai);
  end
  thetaG = thetaGf*poros;

else FYWRC = 1;
% *** Fredlund-Ying Water Retention Curve ---------------------------------
  psin   = calpsinProf(psiniFY,nlevel);
  if TMPWRC == 0;
     [theta thpsin thpsini thtemp] = ... 
                   calthetaFYr(Rcoef,dRcdT,dRcdp,psin,poros);
  else  
     [theta thpsin thpsini thtemp] = calthetaFY(psin,poros,nlevel);
  end
  thetaG = thetaGf*poros;
end

deltheta = thetai+thetar-theta(1)

%__________________________________________________________________________
% Calculate Kelvin temperature (tempk), its inverse (tempki), and the 
%          reduced temperature (tempr)
% Assign initial profiles of vapor density (rhov) and pressure (vapres) 
%   Assume the initial profile of rhov is at equilibrium, therefore 
%   rhov = rhoveq = rh*csat according to the source term (sourcev) 

[tempk tempki tempr] = caltempkHMV(temp);
rhov                 = rhoveqHMV(tempk,psin);
vapres               = calvaporHMV(rhov,tempk); 

%__________________________________________________________________________
% Initialize the old variables from the previous time step

theto = theta;
tempo = temp;
rhovo = rhov;

#endif 