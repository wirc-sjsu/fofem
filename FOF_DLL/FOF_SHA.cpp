/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: soi_sha.c
* Desc: Soil Heat Array
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include  "fof_sgv.h"
#include  "fof_sh.h"
#include  "fof_sha.h"

extern char gcr_SoiErr[];

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/* NOTICE that we declare 'eC_Lay + 1' the 0 Layer is not used to store      */
/*  any heat values, see notes in the soi_sha.h file                         */
float     rr_SHA [eC_Lay+1] [eC_Tim+1];
int   gi_SHA_TimX;     /* running index to store temps */
int   gi_SHA_Inc;      /* # seconds between temp recordings */

float  gf_DufDepRem;   /* Remaining duff depth after soil sim burn */

/*-----------------------------------------------------------------
* Name: SHA_SetInc  SHA_GetInc
* Desc: Set and get the rr_SHA[] time increment, the time increment
*       is the number of seconds each element in the array represents
*       which is how often the soil simulations is runs each cycle
*       and stores the temps in the rr_SHA[]
*******************************************************************/
void SHA_SetInc (int i)
{
  gi_SHA_Inc = i;
}
int SHA_GetInc()
{
  return gi_SHA_Inc;
}

/*-----------------------------------------------------------------------------*/
/* Time, Percent, Watt Array, */
/*  setup up to test the new Graph 3 soil graph */
#define eC_TP  100000
int   gi_Tim [eC_TP];    /* Running time in seconds */
float gf_PC  [eC_TP];    /* percent of heat/watts reaching soil */
float gf_Wts [eC_TP];    /* Watts ( with percent already applied to it) */

void SHA_TP_Init()
{
int i;
    for ( i = 0; i < eC_TP; i++ ) {
     gi_Tim [i] = -1; 
     gf_PC  [i] = -1; 
     gf_Wts [i] = -1;  }
}

/******************************************************
* Name: SHA_TP_Put
* Desc: save the time (seconds) and watts(w/m2) that
*       the Campbell model used for the Duff and non-Duff
*       models,
*
*       See Notes below for SHA_TP_Get()
*
*******************************************************/
int SHA_TP_Put (int i_Tim, float f_PC, float f_Wts)
{
int i;
  for ( i = 0; i < eC_TP; i++ ) {
     if ( gi_Tim[i] >= 0 )
        continue;
     gi_Tim [i] = i_Tim;   /* Running time in seconds */
     gf_PC  [i] = f_PC;
     gf_Wts [i] = f_Wts;
     return 1; 
  }
  return 0; 
}

/**************************************************************
* Name: SHA_TP_Get 
* Desc: Get FOFEM fire intensity
*       Watts were loaded when FOFEM ran the Campbell model 
*       see SE_Mngr_Array() or SD_Mngr_New()
*   In: iX....index into the watts array
*       ai_Tim....time in seconds
*       af_Wts....watts per meter square, 
*                 NOTE: the amount of heat reduction has been
*                 allied to this by the above mentioned 
*                 So this is the heat/watts that actualy
*                 reaches the soil 
***************************************************************/
int SHA_TP_Get (int iX, int *ai_Tim,  float *af_Wts)
{
int i;
   if ( iX >= eC_TP )
      return 0;
   *ai_Tim = gi_Tim[iX];  /* Running time in seconds */
//  *af_PC  = gf_PC[iX]; 
   *af_Wts = gf_Wts[iX];
   return 1; 
}

/************************************************************
* Name: SHA_TP_GetByTime
* Desc: Get the data for the specified time (seconds)
*   In: i_Sec.....time in seconds of info to retreive
*  Out: ai_Tim....burnup time where info is found
*       af_PC.....percent of heat the reaches soil
*       af_Wts....Watts 
* Note: The arrays searched are create when Burnup runs
*       b
*************************************************************/
int SHA_TP_GetByTime (int i_Sec, int *ai_Tim, float *af_PC, float *af_Wts)
{
int i;

 *ai_Tim = 0;
 *af_PC = 0; 
 *af_Wts = 0;
 
   for ( i = 0; i < 1000000; i++ ){
     if ( gi_Tim[i] < 0 )
       return 0; 
     if (  gi_Tim[i] < i_Sec ) 
       continue; 
     *ai_Tim = gi_Tim[i];  
     *af_PC  = gf_PC[i]; 
     *af_Wts = gf_Wts[i];
      return 1; 
   }

  return 0; 
}



/*************************************************************************
* Name: SHA_DufDepRem
* Desc: get and set the Remaining (post fire) duff depth,
*       this is the amount that is calculated in the new SD_Mngr_Dam() 
*       note - that function name may be changed after testing 
**************************************************************************/
float SHA_DufDepRem ()
{
   return gf_DufDepRem;
}

void SHA_DufDepRem_Set (float f)
{
  gf_DufDepRem = f; 
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SHA_Init
* Desc: Init the entire Soil Heating Array (2 dimension)
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
void  SHA_Init ()
{
int i,j;
   for ( i = 0; i <= eC_Lay; i++ )
     for ( j = 0; j < eC_Tim; j++ )
       rr_SHA[i][j] = e_SHA_Init;
  gi_SHA_TimX = 0; 
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SHA_Init_0
* Desc: Init the entire Soil Heating Array with 0s
*       I'm doing this for a Burnup NO Ignite condition, not sure if
*       I'll really use this or not.
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
void  SHA_Init_0 ()
{
int i,j;

   SHA_Init(); 
   for ( i = 0; i <= eC_Lay; i++ )
     for ( j = 0; j < 60; j++ )
       rr_SHA[i][j] = 21.0;
 
  gi_SHA_TimX = 0; 
  SHA_SetInc (60);
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SHA_Largest
* Desc: Get Largest Heat Value stored in the table
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
float  SHA_Largest ()
{
int i,j;
float  r;
   r = 0;
   for ( i = 0; i <= eC_Lay; i++ ) {
     for ( j = 0; j < eC_Tim; j++ ) {
       if ( rr_SHA[i][j] > r )
         r = rr_SHA[i][j]; }}
   return r;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SHA_isLay
* Desc: Go thru a layer and see if any of the values are set to a non-init
*        value which means there has been a heat value set into layer
*        and the layer is not empty.
*        A row across the SHA table represents a layer.
*   In: i_Lay......layer
*  Ret: 0 layer is empty,   1 layer not empty
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int SHA_isLay (int i_Lay)
{
int i;
  for ( i = 0; i < eC_Tim; i++ ) {
    if ( rr_SHA [i_Lay] [i] != e_SHA_Init )
    return 1;
  }
 return 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SHA_Put
* Desc: Put a value into the array
* Note-1 Array size is declared as eC_Tim + 1, so make sure there is  
*        and init value in the last position of the array
*  Ret: true = array can't hold any more, 
*       false = didn't overflow array 
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
bool  SHA_Put (int i_Lay, float r_Val)
{
   if ( gi_SHA_TimX >= eC_Tim ) {
     rr_SHA[i_Lay][eC_Tim] = e_SHA_Init; 
     return true; }
 
   rr_SHA[i_Lay][gi_SHA_TimX] = r_Val; 
 
   return false;
}
/***************************************************************
*
*
**************************************************************/
void SHA_TimX ()
{
  gi_SHA_TimX++; 
}
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SHA_Get
* Desc: Get the Heat Temp values from the specified location in the
*        array,
*   In: i_Lay.....Layer index (row), note--> NO 0 Layer
*       iX........next index
*  Ret: Heat Temp,
*       e_SHA_Init...is returned if there is no value, this happens depending
*                    on time increments, and also at end of each row
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
float  SHA_Get (int i_Lay, int iX)
{
   if ( i_Lay > eC_Lay || i_Lay < 1 ) {
     return 0; }
   if ( iX >= eC_Tim || iX < 0 ) {
     return e_SHA_Init ; }   /* was 0 */
   return rr_SHA[i_Lay][iX];
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SHA_LayerDeg
* Desc: Find deepest layer that has a specified degree
*  Ret: the layer.....Note 1 is the first layer,
*                     Layer 0 in the rr_SHA[][] is not used
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int  SHA_LayerDeg  (float f_Deg)
{
int i,j,i_Lay;
float  r;
   i_Lay = -1;
   r = (float) f_Deg;
   for ( i = 0; i <= eC_Lay; i++ ) {         /* look at each layer           */
     for ( j = 0; j < eC_Tim; j++ ) {        /*  each time increment         */
       if ( rr_SHA[i][j] >= r )              /* check heat                   */
          i_Lay = i; }}                      /* save the layer               */

   return i_Lay;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SHA_MaxTmpLay
* Desc: Find the maximum temperature in a specified layer
* NOTE: Remember the 0 row of the 2 dimensional array is not used
*       so don't send in 0,
*   In: i_Lay......see note above, 0 is NOT used
*  Ret: temperature, -1 if no
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
float  SHA_MaxTmpLay (int i_Lay)
{
int   j;
float  r;
float f;
   r = -1;

   for ( j = 0; j < eC_Tim; j++ ) {
     if ( rr_SHA[i_Lay] [j] >= r )
       r = rr_SHA [i_Lay] [j];
   }

   f = (float) r;
   return f;
}


/**********************************************************
* Name: ToMinutes
* Desc: convert seconds to minutes, 
*       example: 90 seconds = 1.5 minutes, 75 sec = 1.25 
**********************************************************/
float ToMinutes (int i_Sec)
{
float f,g;
    f = i_Sec; 
    g = f / 60.0;
    return g; 
}
