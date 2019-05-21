/******************************************************************
* Name: HTA.cpp   Heat Time Arrays
* Desc: 
*
******************************************************************/
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "fof_sh.h"

#define e_HTA_Init  -999
#define eC_HTA 150000    //    5000
#define e_BM_Lay 21 

float fr_Heat  [e_BM_Lay+1][eC_HTA+1];
float fr_Moist [e_BM_Lay+1][eC_HTA+1];  /* Moisture */ 
float fr_Time  [e_BM_Lay+1][eC_HTA+1];

float fr_psin [e_BM_Lay+1][eC_HTA+1];  /* Soil Water Potential */

/*****************************************************
* Return the number of layer 
******************************************************/
int HTA_Layers ()
{
  return e_BM_Lay;
}
/**************************************************
*
**************************************************/
void HTA_Init ()
{
int i,j;
 
  for ( i = 0; i <= e_BM_Lay; i++ ){
    for ( j = 0; j <= eC_HTA; j++ ){
      fr_Heat [i][j] = e_HTA_Init;
      fr_Moist[i][j] = e_HTA_Init;
      fr_Time [i][j] = e_HTA_Init;
      fr_psin [i][j] = e_HTA_Init; 
  }}
}

/*****************************************************
* Name: HTA_Put
* Desc: Store Massman simulation values
*   In:
****************************************************/
void HTA_Put (int i_Lay, float temp, float moist, float psin, float t) 
{
int i;
float f; 
   for ( i = 0; i < eC_HTA; i++ ) { 
      f = fr_Heat[i_Lay][i]; 
      if ( f != e_HTA_Init )
         continue;  
      fr_Heat[i_Lay][i] = temp;    /* Celsisu */
      fr_Moist[i_Lay][i] = moist; 
      fr_psin [i_Lay][i] = psin; 
      fr_Time[i_Lay][i] = t;     /* Minutes */
      break; 
    }
}

/*************************************************
* Name: HTA_Get
* Desc: Retreive stored Massman simulation values
**************************************************/
int HTA_Get (int i_Lay, int iX, float *af_Heat, float *af_Moist, float *af_psin, float *af_Time)
{
   *af_Heat = 0;
   *af_Time = 0; 
   if ( iX >= eC_HTA )
     return 0; 
   if ( fr_Heat[i_Lay][iX] == e_HTA_Init )
     return 0; 
   *af_Heat = fr_Heat[i_Lay][iX];    /* Celsius */
   *af_Moist = fr_Moist[i_Lay][iX];
   *af_Time = fr_Time[i_Lay][iX];    /* Minutes */ 
   *af_psin = fr_psin [i_Lay][iX];
    return 1; 

} 

/*****************************************************
* Name:
* Desc: 
*   In:
****************************************************/
int  HTA_Count () 
{
int i;
float f; 
   for ( i = 0; i < eC_HTA; i++ ) { 
     if ( fr_Heat[1][i] == e_HTA_Init )
       break;
   } 
  return i;
 }