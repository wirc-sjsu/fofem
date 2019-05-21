/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fofs_sd.c
* Desc: Soil Duff Simulation
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include  "fof_sgv.h"

#include "fof_sh.h"
#include "fof_sh2.h"
#include "fof_sd.h"
#include "fof_sd2.h"
#include "fof_ansi.h"
#include "fof_sha.h"
#include "fof_soi.h"
#include "bur_brn.h"
#include "fof_util.h"
#include "Unit_Abv.h" 


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* This is percent of heat that makes it thru the duff during fire           */
/* see code where this get set, this is global so we can save it and make    */
/* it available to other parts of program via function defined herein        */
float  gf_Heatpc = 0;

bool _Done (float rr[], float r_starttemp, int i_ClockSec, int  i_BurnTime );

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SD_Mngr_New
* Desc: Soil Duff Simulation. This simulation is done when there is Duff
*        prefire Duff present
* NOTE: output from the soil simulation gets returned indirectly thru
*        the SHA_* table and functions
* Note-2: percentage gets saved on every time step so what we end up with
*         is the percentage on the last time step. After the simulation FOFEM
*         will check this looking for a really small number. A really small 
*         number would indicate that very little heat actually reached the soil 
*   In: a_SD.....see the SD_Init() function,
*  Ret: 1 OK
*       0 Error - Couldn't open file, OR
*                Soil Simulation was unable to be done with the set
*                condtions, see notes in fof_soi.c soiltemp_step ()
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int  SD_Mngr_New (d_SD *a_SD, char cr_TmpFN[], char cr_ErrMes[])
{
int     i, j, i_dt, i_success, i_ClockSec, i_BurnTime; 
bool    b_Done; 
float   f_Sec, f_SurfTemp,  pc, slice, DufDep, f_PreAmt;
float   f, g, f_Watts, f_DufLoaPre, f_DufConPer, f_DuffMoist;
double   d_Duf_CPTS, d_Watts, d_BurnTime, d_kW, d_Secs, d_AmtSec;
float  r_Rabs, r_Rabsub;
float  rr_z[e_mplus1+1],  rr_w[e_mplus1+1], rr_t[e_mplus1+1];
FILE  *fh_Out;

  strcpy (cr_ErrMes,"");
  b_Done = false; 
  SHA_TP_Init();                             /* to Save Time, percent and watts */

  if ( !strcmp (cr_TmpFN,"") ) {             /* Tmp output File Name         */
     fh_Out = NULL;                          /*  Not doing                   */
     goto A; }

  fh_Out = SI_fopen (cr_TmpFN,"w");            /* Open the Soil Points file   */
  if ( fh_Out == NULL ) {
    sprintf (cr_ErrMes, "%s Can't open output file:  %s\n", e_SoilErr, cr_TmpFN);
    return 0; }

  TmpFil_Heading (fh_Out);

A:
  r_Rabs = 5.67e-8 * sqr ( sqr (a_SD->r_starttemp + 273.0) ); /* ambient radiant heat (c) */

  soiltemp_initconsts (a_SD->r_bd,   /* Bulk Density - soil */
                       a_SD->r_pd,   /* Particle Density - soil */
                       a_SD->r_ls,   /* Thermal conductivity */
                       a_SD->r_ga,   /* de Vries shpe fac */
                       a_SD->r_xwo,  /* water content liq recirc */
                       a_SD->r_cop,  /* power recirculation */
                       a_SD->r_xo,   /* extrapolated water */
                       a_SD->rr_z);  /* layers array */
  
  soiltemp_getdepths (rr_z);



  for ( i = 0; i <= e_mplus1; i++ ) {
    rr_w[i] = a_SD->r_startwc;
    rr_t[i] = a_SD->r_starttemp;  }

  soiltemp_initprofile (rr_w,rr_t);
    
/*------------------------------------------------------*/  
  f_DufConPer = a_SD->f_DufConPer;
  f_DufLoaPre = TPA_To_KiSq(a_SD->f_DufLoaPre);
  f_DuffMoist = a_SD->f_DufMoi / 100.0 ; 

  DuffBurn (f_DufLoaPre,           /* --> wdf, Duff Weight */
            f_DuffMoist,          /* --> dfm, Duff Moisture, as decimal percent */
            &d_kW,                /* <-- duff fire intensity - kW kilowatts */
            &d_Secs,              /* <-- Time in Seconds it takes to burn */
            f_DufConPer,    /* --> f_DufConPerCent, Duff consumed as percent (whole number) of starting load */
            &d_AmtSec);           /* <-- Load Amount consumed per second */
 
  pc = d_AmtSec / f_DufLoaPre;     /* percent duff consumed each second */
  slice = pc * a_SD->f_DufDepPre;  /* depth/thickness comsumed each second */
  i_BurnTime = d_Secs; 
  f_Watts = d_kW * 1000;           /* Kilowatts to Watts */  

  f = a_SD->f_DufDepPre * ( f_DufConPer / 100.0 ) ; /* Depth Consumed */
  g = a_SD->f_DufDepPre - f;      /* Depth remaining */       
  SHA_DufDepRem_Set (g);          /* remaining duff depth */

   SHA_SetInc (a_SD->i_dt);       /* i_dt is # of seconds in a time step */
   i_ClockSec = 0; 

/*-------------------------------------------------------------------*/
  while ( 1 ) {
    if ( i_ClockSec < i_BurnTime ) {         /* Duff still burning */
      f_Sec = i_ClockSec ;                /* seconds fire has been burning */
      g =  slice * f_Sec;                /* inches burned */ 
      DufDep = a_SD->f_DufDepPre - g;    /* Remaining inches of duff */
      pc = SD_HeatAdj (DufDep);          /* % heat that goes thru the duff */  
      f = f_Watts * pc; 
      gf_Heatpc = pc;  }                  /* See Note-2 above */
    else {                                 /* Fire is out */
      f = 0;   }


/* test...we put a duff efficency textbox on the GUI, */
    f = f * ( a_SD->f_EffFF * 0.01 ); 
/* test...*/


    r_Rabsub = r_Rabs + f; 
    i_dt = a_SD->i_dt;
    i_success = 0;

    SHA_TP_Put (i_ClockSec,pc,r_Rabsub);    /* Save these */

/*.......................................................*/
    while (1) { 
      if ( !soiltemp_step(r_Rabsub,i_dt,&i_success)){ /* simulate a time step  } */
        sprintf (cr_ErrMes, "%s Duff Model %s",e_SoilErr, e_SoiSimFail);    /* Unable to do Soil Sim   */
        goto Close; }                                                       /*  under thes coditions   */
      if ( !i_success ) {                       /* if not successful then} */
         i_dt = i_dt / 2;                       /* cut time in half and */
         soiltemp_initprofile (rr_w,rr_t);}     /* try again  */
      if ( i_success )
         break;  
     }  /* while */
/*......................................................*/

    soiltemp_getwater (rr_w);
    soiltemp_gettemps (rr_t);

    if ( fh_Out != NULL ) {                        /* if Soil Pts file open         */
      j = i_ClockSec + i_dt; 
      SI_fprintfVal (fh_Out,"%6.1f", ToMinutes (j));       /* Put out Soil Pts file        */
    }

    for ( i = 1; i <= e_mplus1; i++ ) {
      if ( a_SD->rr_node[i] == 1  ) {
        b_Done = SHA_Put (i, rr_t[i]);
        if ( fh_Out != NULL )                    /* if Soil Pts file open         */
          SI_fprintfVal (fh_Out,"%6.1f", rr_t[i]); } }
     SHA_TimX(); 

     if ( fh_Out != NULL )                    /* if Soil Pts file open         */
      SI_fprintfRet (fh_Out,"\n");

     b_Done = _Done (rr_t, a_SD->r_starttemp, i_ClockSec, i_BurnTime );
     if ( b_Done == true ) 
       break; 

     i_ClockSec += a_SD->i_dt;

 }  /* while r_time */


Close:
  if ( fh_Out != NULL ) {                  /* if Soil Pts file open         */
    SI_fclose (fh_Out);
    fh_Out = NULL; }

  if ( strcmp (cr_ErrMes,"") )          /* Error Occured                     */
    return 0;

  return 1;
}

/********************************************************************
* Name: _Done
* Desc: Determine if its time to stop the simulation
*  Ret: true - time to stop
*********************************************************************/
bool _Done (float rr[], float r_starttemp, int i_ClockSec, int  i_BurnTime )
{
int i; 
float f_temp;
   if ( i_ClockSec <= i_BurnTime )   /* Need to be at least past end of burn */
     return false;

   f_temp = r_starttemp + 0.5;   /* an arbitrary temp above start temp */

/* see if top 5 layers have all come back to starting temp */   
   for ( i = 1; i <=5; i++ ) {
     if ( rr[i] > f_temp )         /* if surface layer is above start temp */
       return false ;              /*  we're not done yet */
   }
   
   return true;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SD_Init
* Desc: Most of the init values come from the sr_SE table, selected
*        based on the Soil Type name.
*       Other inputs come from user in the s_SI struct
* Note-1: We'll be running Duff Sim because we had some duff but the fire
*         didn't consume any of it so we fake a small amount to create a
*         little bit of heat
* Note-2: Duff heat efficency percent 0->100 that comes from the Settings
*          Window
*   In: a_SI......soil input structure
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int  SD_Init (d_SD *a_SD,  d_SI *a_SI,  char cr_ErrMes[])
{
int i;
float  r, r_Post, r_Pre, r_Con;
double d_Watts, d_TotJoules;  
double d_BurnTime, d_MidBurn; 
  
 gf_Heatpc = 0;                                /* Heat percent            */

/* Use Soil Type, get Duff Sim param record from table                       */
   for ( i = 0;  i < 100; i++ ) {                /* find match in table     */
     if ( !strcmp (sr_SD[i].cr_Name,"")) {       /* Hit end and didn't find */
       sprintf (cr_ErrMes,"%s Invalid Soil Type: %s ",e_SoilErr, a_SI->cr_SoilType);
       return 0; }
     if ( !strcmp (sr_SD[i].cr_Name,a_SI->cr_SoilType)){ /* Copy out                */
       memcpy (a_SD,&sr_SD[i],sizeof(d_SD));
       break; } }

/* Check Soil Moisture and set starting water content                        */
   if ( a_SI->f_SoilMoist > e_SMV_Max || a_SI->f_SoilMoist < e_SMV_Min ){
      sprintf (cr_ErrMes,"%s Moisture %6.2f is out of range (%3.0f to %3.0f)", e_SoilErr, a_SI->f_SoilMoist, e_SMV_Min, e_SMV_Max);
      return 0; }
   a_SD->r_startwc = a_SI->f_SoilMoist / 100;    /* get it in decimal       */

/* Init the Layer and Display arrays....                                     */
   SH_Init_LayDis (a_SD->rr_z, a_SD->rr_node);

   a_SD->f_DufDepPre = a_SI->f_DufDepPre;
   a_SD->f_DufLoaPre = a_SI->f_DufLoaPre;
   a_SD->f_DufMoi = a_SI->f_DufMoi;
   a_SD->f_DufConPer = a_SI->f_DufConPer;

   if ( a_SI->f_SoilDuffEff < 0 || a_SI->f_SoilDuffEff > 100.0 ) 
     a_SD->f_EffFF = 100.0;
   else 
     a_SD->f_EffFF = a_SI->f_SoilDuffEff;    /* See Note-2 above */
 
   return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SD_Heatpercent
* Desc: Return the percent of heat that makes it thru post duff depth
*  Ret: percent
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
float  SD_Heatpercent()
{
   return gf_Heatpc;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SD_HeatAdj
* Desc: Calculate the percent of heat that passes thru a duff layer.
*       DJ ran a regression to get this formula,
*       We use this to determine how much heat passes thru the remaining
*        duff which is acting as an insulator during the fire.
*       when post duff depth is zero than all the heat (100 percent)
*       goes into the soil. Once the post duff depth layer gets to
*       around 6 the percent is 0 meaning no heat is getting thru.
* NOTE: you send in the post duff depth in INCHES
*   In: r_Post.....post duff depth in INCHES
*  Ret: percent of heat
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
float SD_HeatAdj (float r_Post)
{
float r, x, y0,a,b,c,d;

   x = InchtoMeter (r_Post);                 /* Inches to Meter              */
   x = x * 100;

   y0 = -1.6996;
    a = 32.7652;
    b = 7.4601;
    c = 68.9349;
    d = 0.6077;
   r = y0 + (a * exp(-b*x)) + (c * exp(-d*x));
   if ( r < 0 )                              /* this will go negative        */
     r = 0;                                  /* when x is around 6 >         */
   if ( r > 100 )                            /* this shouldn't happen        */
     r = 100;                                /* but better check             */
   r = r * 0.01;                             /* get to decimal percent       */
   return r;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _ChkSoiDuf
* Desc: Check the Pre and Post fire depth.
*   In: see below
*  Ret: 1 ok, 0 error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int SD_ChkSoiDuf (float f_DufDepPre, float f_DufDepPos, char cr_ErrMes[])
{
float f_Con;
char cr[300];

  if ( f_DufDepPre < 0 ) {
    sprintf (cr_ErrMes, "%s Prefire Duff Depth is Negative: %f\n", e_SoilErr, f_DufDepPre);
    return 0; }

  if ( f_DufDepPos < 0 ) {
    sprintf (cr_ErrMes, "%s Postfire Duff Depth is Negative: %f\n", e_SoilErr, f_DufDepPos);
    return 0; }

  if ( f_DufDepPos > f_DufDepPre ) {
    sprintf (cr_ErrMes, "%s Postfire duff depth (%f) is greater than prefire depth (%f).\n",  e_SoilErr, f_DufDepPos,f_DufDepPre);
    return 0; }

  f_Con = f_DufDepPre - f_DufDepPos;         /* Consumed Duff Depth          */

  if ( f_Con > e_DufDepLim ) {
    sprintf (cr_ErrMes, "%s Consumed duff depth (%f) exceeds limit (%f). \n", e_SoilErr,  f_Con,e_DufDepLim);
    sprintf (cr, "Consumed duff depth is the difference of prefire and postfire, %f - %f", e_SoilErr,  f_DufDepPre,f_DufDepPos);         /* Consumed Duff Depth          */
    strcat  (cr_ErrMes,cr);
    return 0; }

  if ( f_Con == 0 ) {
    sprintf (cr_ErrMes,"%s There has been NO duff reduction under these conditions.\n", e_SoilErr );
    strcat (cr_ErrMes,"Therefore, the effect on soil heating will be minimal.");
    return 1;}

  return 1;
}


/********************************************************************
* Name: TmpFil_Heading
* Desc: put a heading in the tmp soil points ouput file
********************************************************************/
void  TmpFil_Heading (FILE *fh_Out)
{
  fprintf (fh_Out,"Column 1     Time in minutes\n");
  fprintf (fh_Out,"Column 2     Surface layer temp (%s) \n", e_Celsius);
  fprintf (fh_Out,"Column 3->15 Subsequent layers, 1->2, 2->3, ... (%s)  \n", e_Centimeter);
  fprintf (fh_Out,"\n");
  fprintf (fh_Out,"  Time  Surf  1-Cm  2-Cm  3-Cm  4-Cm  5-Cm  6-Cm  7-Cm  8-Cm  9-Cm  10-Cm 11-Cm 12-Cm 13-Cm\n");

}


#ifdef OLD_CODE

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SD_Mngr
* Desc: Soil Duff Simulation. This simulation is done when there is Duff
*        prefire Duff present
* NOTE: output from the soil simulation gets returned indirectly thru
*        the SHA_* table and functions
* Note-1: The orginal soil code read this inputs from an input file, so
*          I left alot of the variables in place and just made assignments
*          to them from the inputs I send in now.
*   In: a_SD.....see the SD_Init() function,
*  Ret: 1 OK
*       0 Error - Couldn't open file, OR
*                Soil Simulation was unable to be done with the set
*                condtions, see notes in fof_soi.c soiltemp_step ()
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/

int  SD_Mngr_Old (d_SD *a_SD, char cr_TmpFN[], char cr_ErrMes[])
{
int   i;
int   i_midburn, i_burntime, i_stoptime, i_duffdensity, i_dt;
float  r_duffdepth, r_bd, r_pd, r_xo, r_ls, r_ga;
float  r_xwo, r_cop, r_starttemp, r_startwc, r_duffheat;

float  r_Rabs, r_Qo, r_qm, r_time, r_Rabsub, r_simdt;
float  rr_z[e_mplus1+1],  rr_w[e_mplus1+1], rr_t[e_mplus1+1];
int   i_success, i_Tim;
FILE  *fh_Out;
float f; 

/*----------------------------------------------------*/
//   if ( !stricmp (a_SD->cr_Dan, "Yes") ) {
 //    i =  SD_Mngr_Dan (a_SD, cr_TmpFN, cr_ErrMes);
//     return i; }
/*----------------------------------------------------*/


  SHA_TP_Init();                             /* to Save Time, percent and watts */

  strcpy (cr_ErrMes,"");

  if ( !strcmp (cr_TmpFN,"") ) {             /* Tmp output File Name         */
     fh_Out = NULL;                          /*  Not doing                   */
     goto A; }

  fh_Out = SI_fopen (cr_TmpFN,"w");            /* Open the Soil Points file   */
  if ( fh_Out == NULL ) {
    sprintf (cr_ErrMes, "%s Can't open output file:  %s\n", e_SoilErr, cr_TmpFN);
    return 0; }

  TmpFil_Heading (fh_Out);

A:
  i_midburn     =  a_SD->i_midburn;          /* See Note-1 above             */
  i_burntime    =  a_SD->i_burntime;
  i_stoptime    =  a_SD->i_stoptime;
  i_duffdensity =  a_SD->i_duffdensity;
  i_dt          =  a_SD->i_dt;
  r_duffdepth   =  a_SD->r_ConDufDep;
  r_bd          =  a_SD->r_bd;
  r_pd          =  a_SD->r_pd;
  r_xo          =  a_SD->r_xo;
  r_ls          =  a_SD->r_ls;
  r_ga          =  a_SD->r_ga;
  r_xwo         =  a_SD->r_xwo;
  r_cop         =  a_SD->r_cop;
  r_starttemp   =  a_SD->r_starttemp;
  r_startwc     =  a_SD->r_startwc;
  r_duffheat    =  a_SD->r_duffheat;


  r_Rabs = 5.67e-8 * sqr ( sqr (r_starttemp + 273.0) );
  soiltemp_initconsts (r_bd, r_pd, r_ls, r_ga, r_xwo, r_cop, r_xo,
                       a_SD->rr_z);
  soiltemp_getdepths (rr_z);

  for ( i = 0; i <= e_mplus1; i++ ) {
    rr_w[i] = r_startwc;
    rr_t[i] = r_starttemp;  }

  soiltemp_initprofile (rr_w,rr_t);
    
//  if ( !stricmp (a_SD->cr_DoIt, "Yes")) /* heat is coming from DuffBurn() function  */
//    r_Qo = r_duffheat;
//  else 
    r_Qo = r_duffheat * r_duffdepth * i_duffdensity;
 
 r_qm = 0.57 * r_Qo / ( i_burntime * 60 );  /* max. rate of heat production; W/m2} */
  r_time = 0;

float f_Test = 0; 

  while ( r_time < i_stoptime ) {
    f = exp ( -sqr ( (i_midburn - r_time ) / i_burntime));
    r_Rabsub = r_Rabs + r_qm * exp ( -sqr ( (i_midburn - r_time ) / i_burntime));
    r_simdt = i_dt;
    i_success = 0;


   SHA_TP_Put (r_time,0,r_Rabsub);    /* Save these */

//-----------------------------------
// send to Visual Studio Output Window during Debug
#define BUF_SIZE 400
   wchar_t wcsbuf[BUF_SIZE];
   swprintf(wcsbuf, BUF_SIZE, L"%f\n", r_Rabsub);
//   OutputDebugString(wcsbuf);
//------------------------------------

    while (1) {
f_Test += r_Rabsub;
int ii; 
      ii = (int) r_simdt; /* put this fix in even though we probably won't use this function anymore */
      if ( !soiltemp_step(r_Rabsub,ii,&i_success,r_time)){ /* simulate a time step  } */
// original       if ( !soiltemp_step(r_Rabsub,i_dt,&i_success,r_time)){ /* simulate a time step  } */
        sprintf (cr_ErrMes, "%s Duff Model %s",e_SoilErr, e_SoiSimFail);         /* Unable to do Soil Sim   */
        goto Close; }                             /*  under thes coditions   */

      if ( !i_success ) {                         /* if not successful then} */
         r_simdt = r_simdt / 2;
         soiltemp_initprofile (rr_w,rr_t);}       /* cut time step in half } */
      if ( i_success )
         break;  }

    r_time = r_time + r_simdt / 60;
    soiltemp_getwater (rr_w);
    soiltemp_gettemps (rr_t);

    if ( fh_Out != NULL )                    /* if Soil Pts file open         */
      SI_fprintfVal (fh_Out,"%6.1f",r_time);         /* Put out Soil Pts file        */

    for ( i = 1; i <= e_mplus1; i++ ) {
      i_Tim = r_time;
      if ( a_SD->rr_node[i] == 1  ) {
        SHA_Put (i,i_Tim, rr_t[i]);
        if ( fh_Out != NULL )                    /* if Soil Pts file open         */
          SI_fprintfVal (fh_Out,"%6.1f", rr_t[i]);
       }
    }

    if ( fh_Out != NULL )                    /* if Soil Pts file open         */
      SI_fprintfRet (fh_Out,"\n");

 }  /* while end */

Close:
  if ( fh_Out != NULL ) {                  /* if Soil Pts file open         */
    SI_fclose (fh_Out);
    fh_Out = NULL; }

  if ( strcmp (cr_ErrMes,"") )          /* Error Occured                     */
    return 0;

  return 1;
}
#endif werwerwe


#ifdef wow
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SD_Disp
* Desc:
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
void SD_Disp (d_SD *a_SD)
{
int i;
  printf ("midburn     %d\n", a_SD->i_midburn);        /* midburn 720  1680 900 1100        1000  900  */
  printf ("burntime    %d\n", a_SD->i_burntime);       /* burntime 240  180  300  300        400  400  */
  printf ("stoptime    %d\n", a_SD->i_stoptime);       /* stoptime 1200 2400 1800 1800      1500  1500 */
  printf ("ConDufDep   %f\n", a_SD->r_ConDufDep);      /* consumed duffdepth 0.08 0.06 0.05 0.04 0.07  */
  printf ("duffheat    %f\n", a_SD->r_duffheat);       /* duff heat content - J/m3                     */
  printf ("duffdensity %d\n", a_SD->i_duffdensity);    /* duff density - kg/m3                         */
  printf ("bd          %f\n", a_SD->r_bd);             /* soil bulk density - g/m3                     */
  printf ("pd          %f\n", a_SD->r_pd);             /* soil particle density - g/m3                 */
  printf ("xo          %f\n", a_SD->r_xo);             /* extrapolated water cont. at -1 J/kg          */
  printf ("ls          %f\n", a_SD->r_ls);             /* thermal conductivity of mineral fraction     */
  printf ("ga          %f\n", a_SD->r_ga);             /* de Vries shape factor                        */
  printf ("xwo         %f\n", a_SD->r_xwo);            /* water content for liquid recirculation       */
  printf ("cop         %f\n", a_SD->r_cop);            /* power for recirculation function             */
  printf ("dt          %d\n", a_SD->i_dt);             /* time step - s                                */
  printf ("startwc     %f\n", a_SD->r_startwc);        /* starting soil water content - m3/m3          */
  printf ("starttemp   %f\n", a_SD->r_starttemp);      /* starting soil temperatue - C                 */

  for ( i = 0; i < e_mplus1+1; i++ )
    printf ("%d:  %f - %f \n", i, a_SD->rr_z[i], a_SD->rr_node[i]);

}
#endif

