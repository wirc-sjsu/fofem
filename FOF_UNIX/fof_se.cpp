/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_se.c
* Desc: Soil Exp Simulation
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include  "fof_sgv.h"
#include "fof_sh.h"
#include "fof_sh2.h"
#include "fof_se.h"
#include "fof_se2.h"
#include "fof_ansi.h"
#include "fof_sha.h"
#include "fof_soi.h"

#include "fof_co.h" 

void  Watt_Init (float f_Max);
float Watt_Governor (float f_Kw);

bool _Done (float rr[], float r_starttemp, int i_ClockSec, float f_FI );
float ToMinutes (int i_Sec);

/**********************************************************************
* Name: SE_Mngr_Array
* Desc: Non-Soil Heat simulation
*       New implemantaion that uses the burnup fire intensity array
*
* Note-2: Stop simulation when surface temp comes back to starting 
*         temp, but sometimes the surface temp is back to start 
*         temp but lower layers aren't so let run at least 60 minutes
*   In: a_SE....soil inputs struct
*       fr_FI...array of wood liter fire intensity - Kw/m2
*       fr_FIhs...array of herb shrub fire intensity - Kw/m2
*       i_frInc...tells the second increment for the fire intensity arrays
*                 for instance Burnup's arrays are always 15 seconds apart
*                 However this number could be anything when a user soil input 
*                 file is in use, 
*       cr_TmpFN.."" or a file name to create the output soil points file
*
***********************************************************************/
int   SE_Mngr_Array (d_SE *a_SE, float fr_FI[],  float fr_FIhs[], int i_frInc,  char cr_TmpFN[], char cr_ErrMes[])
{
int  i,j, i_success, ii, i_dt; 
float f, f_fi, f_FI;
float f_SoilHsEff, f_SoilWlEff; 

bool b_Done = false ; 
float  r_Rabs;
float rr_node [e_mplus1+1], rr_z [e_mplus1+1], rr_w [e_mplus1+1];
float rr_t [e_mplus1+1];
FILE  *fh_Out;
int i_ClockSec;

  strcpy (cr_ErrMes,"");

/* Fire Intensity efficiencies - testing  */
  f_SoilWlEff = a_SE->f_SoilWlEff / 100.0;  /* Get it to decimal percent */  
  f_SoilHsEff = a_SE->f_SoilHsEff / 100.0; 
  if ( f_SoilWlEff < 0 )                   /* Use a default valeu */
    f_SoilWlEff = 0.15;
  if ( f_SoilHsEff < 0 ) 
    f_SoilHsEff = 0.10;

  if ( !strcmp (cr_TmpFN,"") ) {             /* Tmp output File Name         */
     fh_Out = NULL;                          /*  Not doing                   */
     goto A; }

  fh_Out = SI_fopen (cr_TmpFN,"w");
  if ( fh_Out == NULL ) {
    sprintf (cr_ErrMes, "Can't open output file: %s\n", cr_TmpFN);
    return 0;  }
  TmpFil_Heading (fh_Out); 

A:
  SHA_SetInc (a_SE->i_dt);                      /* How often we'll save temps */ 
  i_ClockSec = 0; 
  r_Rabs = 5.67e-8 * sqr ( sqr (a_SE->r_starttemp + 273.0)); /* ambient Radiant heat at surface */
 
  soiltemp_initconsts(a_SE->r_bd,
                       a_SE->r_pd,
                       a_SE->r_ls,
                       a_SE->r_ga,
                       a_SE->r_xwo,
                       a_SE->r_cop,
                       a_SE->r_xo,
                       a_SE->rr_z);
 
  soiltemp_getdepths(rr_z);

  for ( i = 0; i <= e_mplus1; i++ ) {
    rr_w[i] = a_SE->r_startwc;
    rr_t[i] = a_SE->r_starttemp; }

  soiltemp_initprofile(rr_w,rr_t);

  SHA_TP_Init();                         /* Init for saving fire intensity */
  SHA_DufDepRem_Set (0);                 /* There is no duff */
 
/*-----------------------------------------------------------------------*/
  while ( 1 ) {

     f_fi = f_FI = _Get_FirInt (i_ClockSec, fr_FI, i_frInc); /* Wood Liter Intensity */
     if ( f_fi < 0 ) 
       f_fi = 0; 
     f_fi = f_fi * 1000.0;        /* Kilowatts to watts */
     f_fi = f_fi * f_SoilWlEff;   /* percent of intensity that goes down into soil */ 
 
     f  = _Get_FirInt (i_ClockSec, fr_FIhs, i_frInc); /* Herb Shrub intensity */
     if ( f < 0 ) 
        f = 0; 
     f = f * 1000.0;                     /* Kilowatts to watts */
     f = f * f_SoilHsEff;                /* percent of intensity that goes down into soil */ 

     f_FI = f_fi + f;                    /* save for checking in a bit */
     f_fi = f_fi + f + r_Rabs;           /* add ambient radient heat at surface */
 
     SHA_TP_Put (i_ClockSec,100,f_fi);   /* Save fire intensity for graphing */
     i_dt = a_SE->i_dt;
/*................................................. */
     while (1) {
       if ( !soiltemp_step(f_fi, i_dt ,&i_success)){ /* simulate a time step  } */
         sprintf (cr_ErrMes, "%s NonDuff Model %s\n Time (min): %d  Watts: %4.0f",e_SoilErr, e_SoiSimFail, (i_ClockSec / 60), f_fi);         /* Unable to do Soil Sim   */
         goto Close; }                             /*  under thes coditions   */

       if ( !i_success ) {                         /* if not successful then} */
         i_dt = i_dt / 2;
         soiltemp_initprofile (rr_w,rr_t);}       /* cut time step in half } */
       if ( i_success )
         break;  }
/*.................................................*/
 
     soiltemp_getwater (rr_w);
     soiltemp_gettemps (rr_t);

     if (fh_Out != NULL ) {                          /* tmp file if requested   */
       j = i_ClockSec + i_dt; 
       SI_fprintfVal (fh_Out, "%6.1f", ToMinutes (j) );
     }


     for ( i = 1; i <= e_mplus1; i++ ) {           /* each layer */
       b_Done = SHA_Put (i, rr_t[i]);              /* save temp */
       if ( a_SE->rr_node[i] == 1  ) {
         if (fh_Out != NULL )
           SI_fprintfVal (fh_Out,"%6.1f", rr_t[i]); }  
     } /* for i */
     SHA_TimX();                                   /* Increment SHA array index */

     if (fh_Out != NULL )
       SI_fprintfRet (fh_Out, "\n");

     b_Done = _Done (rr_t, a_SE->r_starttemp, i_ClockSec, f_FI);
     if ( b_Done == true ) 
       break; 

     i_ClockSec += i_dt; 

  } /* while end */

Close:
  if (fh_Out != NULL ) {
    SI_fclose (fh_Out);
    fh_Out = NULL; }

  if ( strcmp (cr_ErrMes,"") )          /* Error Occured                     */
    return 0;

  return 1;
}
#ifdef wowow
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
#endif 

/********************************************************************
* Name: _Done
* Desc: Determine if its time to stop the simulation
*   In: rr[]....  current heat at each layer
*       r_starttemp...starting temp, probably 21.0 (C)
*       i_ClockSec....current time in seconds 
*       f_FI.........fire intensity
*  Ret: true - time to stop
*
*********************************************************************/
bool _Done (float rr[], float r_starttemp, int i_ClockSec, float f_FI )
{
int i; 
float f_temp;

   if ( i_ClockSec < ( 60 * 20 ))  /* Just picking some minimum number of */
     return false;                /* minutes to run */

   if ( f_FI > 0 )       /* Still getting fire intensity from fire */
     return false;

   f_temp = r_starttemp + 0.5;   /* an arbitrary temp above start temp */

/* see if top 5 layers have all come back to starting temp */   
   for ( i = 1; i <=5; i++ ) {
     if ( rr[i] > f_temp )         /* if surface layer is above start temp */
       return false ;              /*  we're not done yet */
   }
   
   return true;
}


/**************************************************
* Name _Get_FirInt
* Desc: get the fire intensity from Burnup array given
*       the time in seconds, starting time is 0
*       Burnup array is in 15 second increments, but
*       a user soil input file could use anything
* NOTE  End of array is marks with -1  

*   In: i_ClockSec....time of simulation in seconds 
*       fr.......the array
**************************************************/
float _Get_FirInt (int i_ClockSec,  float fr[], int i_frInc)
{
int i; 
float f;  
    i = i_ClockSec / i_frInc ;      /* burnup array is in 15 second increments */ 
 
/* change 3-25-16 Ticket-66, big fuel loads caused array to fill, better check here */
    if ( i >= eC_sfi )
      return -1.0; 

    f = fr[i];
    return f; 
} 

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SE_Init
* Desc: Load the SE (soil exp) input struct. Most inputs comes from
*        the sr_SE table based on the soil type.
* Note-1: radiant heat coming from burnup is in Kilowatts per Meter Sq.
*         so we multiply by 1000 to gets watts sq.

* Note-3: Need to check this, if a 0 ever did come in it will cause a
*          divide by 0 later in the simulation.
*   In: a_EID_......a EID_ to load
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int  SE_Init (d_SI *a_SI, d_SE *a_SE, char cr_ErrMes[])
{
int i,  i_time;
float   f_Heat;

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/* Find entry in table, based on soil type                                   */
   for ( i = 0;  i < 100; i++ ) {            /* find match in EID Tbl   */
     if ( !strcmp (sr_SE[i].cr_Name,"")) {   /* Hit end and didn't find */
       sprintf (cr_ErrMes,"%s Invalid Soil Type: %s ", e_SoilErr, a_SI->cr_SoilType);
       return 0; }
     if ( !strcmp (sr_SE[i].cr_Name,a_SI->cr_SoilType)){ /* Copy out                */
       memcpy (a_SE,&sr_SE[i],sizeof(d_SE));
       break; } }

   if ( a_SI->f_SoilMoist > e_SMV_Max || a_SI->f_SoilMoist < e_SMV_Min ){
      sprintf (cr_ErrMes,"%s Moisture %6.2f is out of range (%3.0f to %3.0f)", e_SoilErr,a_SI->f_SoilMoist, e_SMV_Min, e_SMV_Max);
      return 0; }

   a_SE->r_startwc = a_SI->f_SoilMoist / 100;         /* get it in decimal           */

/* Init the Layer and Display arrays....                                     */
   SH_Init_LayDis (a_SE->rr_z, a_SE->rr_node);


/* Fire Intensity Efficeny */
  a_SE->f_SoilWlEff = a_SI->f_SoilWlEff;   /* Wood Litter */
  a_SE->f_SoilHsEff = a_SI->f_SoilHsEff;   /* Herb Shrub */


   return 1;
}
/*#################################################################################
##################################################################################*/

//-------------------------------------------------------------------------
//
//   the two functions below get put in the SE_Mngr_Array () 
//
// I was testing to see how much watts the model could handle before it
//  went into an infitie loop, looks like about 164, but that might depend
//  on the other inputs, like soil moisture, soil type, I didn't get into 
//  testing that far,
//  See below the Watt_Governor(), that would limit watts 
// 100 150 160 ok
// 170 180 to much 
//  f = 164.0 * 1000.0;   /* Kw to watts */ 
//     Watt_Init ( f ) ; 
//
//  Current thinking is to not fix the problem here but in Burnup that is
//   creating the extreme amounts, so we're working on changing Burnup and
//   how it handle the herb and shrub
//
//-------------------------------------------------------------------------

 //     f_fi = Watt_Governor (f_fi); /* only allow so much fi per time step */

/********************************************************************
*  Store watts and return back a threshold/maximum number of watts
*  The soil model can only handle up to a certain amount of watts 
*   per time step. So this function is setup to store the excess
*   watts and return the on subsequent calls, but never return more than 
*   the threshold amount
*  gf_FI stores/accumulates any fire intensity (watts)  
********************************************************************/
float gf_Watts = 0; 
float gf_Thrhld = 0; 


float Watt_Governor (float f_Watts)
{
float f; 
    gf_Watts += f_Watts;   /* Add watts to the store */

    if ( gf_Watts >= gf_Thrhld ) {     /* if have more watts than threshold amt */
      gf_Watts = gf_Watts - gf_Thrhld;    /* take them out of store */
      return gf_Thrhld; }            /* and return them */
    
/* if less watts in store than a threshold amt return whats in store */
    f = gf_Watts;                      
    gf_Watts = 0; 
    return f;

} 
/**********************************************************
* Init and set the threshold, 
***********************************************************/
void  Watt_Init (float f_Max)
{
  gf_Watts = 0; 
  gf_Thrhld = f_Max;
}


#ifdef wow
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SE_Disp
* Desc:
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void SE_Disp (d_SE *a_SE)
{
int i;

  printf ("starttime; %d \n",a_SE->i_starttime);   /* starttime-time from simulation start to fire on - min */
  printf ("burntime;  %d \n",a_SE->i_burntime);    /* burntime-time from sim start to fire off - min  */
  printf ("cooltime;  %d \n",a_SE->i_cooltime);    /* cooltime-time from start to end of simulation   */
  printf ("maxRabs;   %f \n",a_SE->r_maxRabs);     /* maxrabs-maximum radiant heat input to surface   */
  printf ("heatconst; %d \n",a_SE->i_heatconst);   /* heatconst-time constant for heating, minutes    */
  printf ("coolconst; %d \n",a_SE->i_coolconst);   /* coolconst-time constant for cooling, minutes    */
  printf ("bd;        %f \n",a_SE->r_bd);          /* bd-soil bulk density - g/m3                     */
  printf ("pd;        %f \n",a_SE->r_pd);          /* pd-soil particle density - g/m3                 */
  printf ("xo;        %f \n",a_SE->r_xo);          /* xo-extrapolated water cont. at -1 J/kg          */
  printf ("ls;        %f \n",a_SE->r_ls);          /* ls-thermal conductivity of mineral fraction     */
  printf ("ga;        %f \n",a_SE->r_ga);          /* ga-de Vries shape factor                        */
  printf ("xwo;       %f \n",a_SE->r_xwo);         /* xwo-water content for liquid recirculation      */
  printf ("cop;       %f \n",a_SE->r_cop);         /* cop-power for recirculation function            */
  printf ("dt;        %d \n",a_SE->i_dt);          /* dt-time step - s                                */
  printf ("startwc;   %f \n",a_SE->r_startwc);     /* startwc-starting soil water content - m3/m3     */
  printf ("starttemp; %f \n",a_SE->r_starttemp);   /* starttemp-starting soil temperatue - C          */

  for ( i = 0; i < e_mplus1+1; i++ )
    printf ("%d:  %f - %f \n", i, a_SE->rr_z[i], a_SE->rr_node[i]);

}

#endif


#ifdef OLDCODE

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SE_Mngr
* Desc: Exp Heat, this run when there is no Duff Depth
*       This will put out and output file and load the Heat Array use
*        to make graph.
* NOTE: output from the soil simulation gets returned indirectly thru
*        the SHA_* table and functions
* Note-1: The orginal soil code read this inputs from an input file, so
*          I left alot of the variables in place and just made assignments
*          to them from the inputs I send in now.
*   In: a_SE......see the SE_Init()
*                 also the the comments in the fof_sh.h in the d_SI struct
* Ret: 1 OK
*      0 Error - Couldn't open file, OR
*                Soil Simulation was unable to be done with the set
*                condtions, see notes in fof_soi.c soiltemp_step ()
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int   SE_Mngr (d_SE *a_SE, char cr_TmpFN[], char cr_ErrMes[])
{
int  i, i_success, i_Tim, ii; 
int  i_starttime, i_burntime, i_cooltime, i_heatconst, i_coolconst,i_dt;
float r_bd,r_pd,r_xo,r_ls,r_ga,r_xwo,r_cop,r_startwc,r_starttemp,r_maxRabs;
float r_Rabs, r_time, r_exh, r_exc, r_Rabsub, r_simdt;
float rr_node [e_mplus1+1], rr_z [e_mplus1+1], rr_w [e_mplus1+1];
float rr_t [e_mplus1+1];
FILE  *fh_Out;

  strcpy (cr_ErrMes,"");

  if ( !strcmp (cr_TmpFN,"") ) {             /* Tmp output File Name         */
     fh_Out = NULL;                          /*  Not doing                   */
     goto A; }

  fh_Out = SI_fopen (cr_TmpFN,"w");
  if ( fh_Out == NULL ) {
    sprintf (cr_ErrMes, "Can't open output file: %s\n", cr_TmpFN);
    return 0;  }

A:
    i_starttime = a_SE->i_starttime;         /* see note-1 above             */
    i_burntime  = a_SE->i_burntime;
    i_cooltime  = a_SE->i_cooltime;
    i_heatconst = a_SE->i_heatconst;
    i_coolconst = a_SE->i_coolconst;
    i_dt        = a_SE->i_dt;
    r_bd        = a_SE->r_bd;
    r_pd        = a_SE->r_pd;
    r_xo        = a_SE->r_xo;
    r_ls        = a_SE->r_ls;
    r_ga        = a_SE->r_ga;
    r_xwo       = a_SE->r_xwo;
    r_cop       = a_SE->r_cop;
    r_startwc   = a_SE->r_startwc;
    r_starttemp = a_SE->r_starttemp;
    r_maxRabs   = a_SE->r_maxRabs;

   r_Rabs = 5.67e-8 * sqr ( sqr (r_starttemp + 273.0));
   soiltemp_initconsts(r_bd,r_pd,r_ls,r_ga,r_xwo,r_cop,r_xo,a_SE->rr_z);
   soiltemp_getdepths(rr_z);

   for ( i = 0; i <= e_mplus1; i++ ) {
     rr_w[i] = r_startwc;
     rr_t[i] = r_starttemp; }

   soiltemp_initprofile(rr_w,rr_t);
   r_time = 0;


  while ( r_time < i_cooltime ) {

    if ( r_time < i_starttime ) {
      r_exh =0; r_exc = 1; }

    if ( r_time > i_starttime &&  r_time < i_burntime)               /* during the burn */
      r_exh = ( 1 - exp ( -(r_time - i_starttime) / i_heatconst));   /* exp. heating and */

    if ( r_time > i_burntime)                      /* cooling during */
      r_exc = exp(-(r_time - i_burntime) /i_coolconst);    /*and after the burn  */

    r_Rabsub = r_maxRabs * r_exh * r_exc + r_Rabs;    /* set abs. rad. at up. bnd */

    r_simdt = i_dt;

    while (1) {
      ii = (int) r_simdt; /* 4-2-15- fix infinit loop bug, put this in even though we don't use this func anymore, we use the _Array one */
      if ( !soiltemp_step(r_Rabsub, ii,&i_success,r_time)){ /* simulate a time step  } */

 // original      if ( !soiltemp_step(r_Rabsub, i_dt,&i_success,r_time)){ /* simulate a time step  } */
        sprintf (cr_ErrMes, "Soil Non-Duff Model %s",e_SoiSimFail);         /* Unable to do Soil Sim   */
        goto Close; }                             /*  under thes coditions   */


      if ( !i_success ) {                         /* if not successful then} */
         r_simdt = r_simdt / 2;
         soiltemp_initprofile (rr_w,rr_t);}       /* cut time step in half } */
      if ( i_success )
         break;  }

    r_time = r_time + r_simdt / 60;
    soiltemp_getwater (rr_w);
    soiltemp_gettemps (rr_t);

    if (fh_Out != NULL )                          /* tmp file if requested   */
       SI_fprintfVal (fh_Out, "%6.1f",r_time);
    for ( i = 1; i <= e_mplus1; i++ ) {           /* save heat & time        */
       i_Tim = r_time;
       SHA_Put (i,i_Tim, rr_t[i]);
       if ( a_SE->rr_node[i] == 1  ) {
         if (fh_Out != NULL )
           SI_fprintfVal (fh_Out,"%6.1f", rr_t[i]); }  }

    if (fh_Out != NULL )
      SI_fprintfRet (fh_Out, "\n");

  } /* while end */

Close:
  if (fh_Out != NULL ) {
    SI_fclose (fh_Out);
    fh_Out = NULL; }

  if ( strcmp (cr_ErrMes,"") )          /* Error Occured                     */
    return 0;

  return 1;
}
#endif 

/* I put this here because I was getting an 'unexpected end of file' error */
/*  when building the FOF_ANSI project */ 
int iiiii; 