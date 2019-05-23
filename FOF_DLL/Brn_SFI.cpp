/*******************************************************************************
* Name: Brn_SFI.cpp               Soil Fire Intensity
* Desc: Save the fire intensity for soil model 
*       
********************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include "Brn_SFI.h"

float xBRN_Intensity (float f_Con, float f_Seconds);

/* Max Tons that can be consumed in a minute */
#define e_MaxLoa60   10.0 
#define e_MaxLoa15   e_MaxLoa60 / 4.0 
float  gf_Box;

/**********************************************************************/
/* Soil Heating Fire Intensity Array                                  */
/* Store fire intensity to be used by soil heating model              */
/* each position of the array is 15 seconds                           */
/* These fire intensity values are used to do the non-duff soil       */
/* so we only store the fire intensity for wood liter herb shrub      */
#define eC_SFI 6000   /* Soil Heating fire intensity array */
float  fr_SFI[eC_SFI + 1 ];
int    iX_SFI;

float  fr_SFIhs[eC_SFI + 1 ];


/******************************************************
* Name:
* Desc:
*
*******************************************************/
int HSBI_Next (float f_LWfi, double *af_Con, double *af_FI)
{
float f_Con, f_FI;

  f_FI = 0; 
  f_Con = 0; 
  *af_Con = 0; 
  *af_FI = 0; 
  if ( gf_Box <= 0 ) 
    goto Put; 

  if ( gf_Box < e_MaxLoa15 ) {
    f_Con = gf_Box;
    gf_Box = 0; }
  else {
    f_Con = e_MaxLoa15;
    gf_Box = gf_Box - f_Con; }

  f_FI = xBRN_Intensity(f_Con,15.0); 

Put: 
  fr_SFIhs[iX_SFI] = f_FI;  /* Herb Shrub fire intensity */
  fr_SFI[iX_SFI] = f_LWfi; 
  iX_SFI++;   
  if ( iX_SFI > eC_SFI )
    iX_SFI = eC_SFI; 

  *af_Con = f_Con; 
  *af_FI = f_FI; 
  return 1; 
}


/********************************************************
* Name: 
* Desc: Calculate the Herb/Shrub fire intensity for the first
*        minute, put into first 15 second elements of the 
*        array. 
*       We need to ramp up the intensity in the first minute
*       For the first minute we cannot consume more than one 
*       half of the specifed maximum consumed amount that is
*       used consuming in minutes, 2,3...
*
* Note-1: We can consume entire herb/shrub in the first minute
*         so divy it accordingly 
*   In: f_HS...Total Herb Shrub load 
*       f_LWfi..Litter Wood fire intensity, 
*  Out: af_Con...amount of Herb Shrub consumed
*       af_FI....fire intenstiy from Herb Shrub
********************************************************/
int  HSBI_FirstMinute (float f_HS, float f_LWfi, double *af_Con, double *af_FI)
{
int i; 
float f,g,x, fi, joule, f_TotCon; 
double A,B,C,D, a,b,c,d;


  for ( i = 0; i <= eC_SFI; i++ ) {  /* Init fire intsity arrays */
    fr_SFI[i] = -1.0;               
    fr_SFIhs[i] = 1.0; }

  x = e_MaxLoa60 / 2.0;      
  if ( f_HS < x ) {         /* See Note-1 above */
    D = f_HS *  0.40;
    C = f_HS *  0.30;
    B = f_HS *  0.20; 
    A = f_HS * 0.10; 
    goto FirInt; }

  f = e_MaxLoa15;      
  D = f * 1.0; 
  C = f * 0.54;
  B = f * 0.34;
  g = D + C + B ;
  A = x - g; 


FirInt:
   fr_SFIhs[0] = xBRN_Intensity(A,15.0); 
   fr_SFIhs[1] = xBRN_Intensity(B,15.0);
   fr_SFIhs[2] = xBRN_Intensity(C,15.0); 
   fr_SFIhs[3] = xBRN_Intensity(D,15.0);
   iX_SFI = 4; 

   f_TotCon = A + B + C + D; 

   gf_Box = f_HS - f_TotCon;
   if ( gf_Box < 0 ) 
      gf_Box = 0;  

/* fire intensity for entire load for 1 minute*/
   fi = xBRN_Intensity(f_TotCon,60.0); 

   *af_Con = f_TotCon; 
   *af_FI = fi;
 

/*................................................*/
/* Store the Litter Wood fire intensity           */
/* fire intenstiy = 1 watt = 1 joule per second   */
/* Ramp up intensity for first minute,            */
/*  take to total jouls, divide into 1/4 minutes  */
   f = f_LWfi * 60.0; 
   a = f * 0.10;
   b = f * 0.20;
   c = f * 0.30;
   d = f * 0.40; 

   fr_SFI[0] = a / 15.0; 
   fr_SFI[1] = b / 15.0; 
   fr_SFI[2] = c / 15.0 ; 
   fr_SFI[3] = d / 15.0; 

   return 1; 
}

/*****************************************************************
*
*
******************************************************************/

float xBRN_Intensity (float f_Con, float f_Seconds)
{
float a,b,c,d,e,f,x;
   a = 8000;
   b = 2000;
   c = 43560;

 //  d = 60;
   d = f_Seconds; 

   e = 11.35;
   f = a * b / c / d * e;
   f = f * f_Con;
   return f;
}