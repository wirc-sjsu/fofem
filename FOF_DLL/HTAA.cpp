/******************************************************************
* Name: HTA.cpp   Heat Time Arrays
* Desc: 
*
******************************************************************/
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "BMSoil.h" 
#include "HTAA.h"   /* Store Temps for graphing */ 



float fr_Heat  [e_BM_Lay+1][eC_HTA+1];
float fr_Moist [e_BM_Lay+1][eC_HTA+1];  /* Moisture */ 
float fr_Time  [e_BM_Lay+1][eC_HTA+1];

float fr_psin [e_BM_Lay+1][eC_HTA+1];  /* Soil Water Potential */


int gi_PutIx = 0; 
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
   gi_PutIx = 0; 
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
* Desc: store massman simulation values 
*       Surface Layer is index 1 of array 
*  Ret: 1 ok,  0 array is full 
****************************************************/
int HTA_Put (int i_Lay, float temp, float moist, float psin, float t) 
{
int i,j;
float f;
j = 0;  

   
   for ( i = gi_PutIx; i < eC_HTA; i++ ) { 

      f = fr_Heat[i_Lay][i]; 
      if ( f != e_HTA_Init )
         continue;  
      fr_Heat[i_Lay][i] = temp;
      fr_Moist[i_Lay][i] = moist; 
      fr_psin [i_Lay][i] = psin; 
      fr_Time[i_Lay][i] = t;       /* time minutes */ 
      gi_PutIx = i; 
      return 1; 
    }
   return 0; 
}

/*************************************************
*
* Surface layer is array index 1
**************************************************/
int HTA_Get (int i_Lay, int iX, float *af_Heat, float *af_Moist, float *af_psin, float *af_Time)
{
   *af_Heat = 0;
   *af_Time = 0; 
   if ( iX >= eC_HTA )
     return 0; 
   if ( fr_Heat[i_Lay][iX] == e_HTA_Init )
     return 0; 
   *af_Heat = fr_Heat[i_Lay][iX];    /* temp (C) */
   *af_Moist = fr_Moist[i_Lay][iX];
   *af_Time = fr_Time[i_Lay][iX];    /* Time minutes */ 
   *af_psin = fr_psin [i_Lay][iX];   
    return 1; 
} 

/******************************************************
*
*
*******************************************************/
int HTA_Count ()
{
int i,j;
float f;
j = 0;  
   for ( i = 0; i < eC_HTA; i++ ) { 
      f = fr_Heat[1][i]; 
      if ( f != e_HTA_Init )
         continue; 
      break; 
   }
   return i;
}