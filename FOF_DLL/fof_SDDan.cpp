/**************************************************************************
* Name: FOF_SDDan.cpp
* Desc: Soil Simulation for Duff model using DuffBurn() and Dan's 
*       equation
**************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include  "fof_sgv.h"

#include "fof_sh.h"
#include "fof_sh2.h"

#include "fof_sd.h"


//  #include "fof_sd2.h"

#include "fof_ansi.h"

#include "fof_sha.h"
#include "fof_soi.h"

#include "bur_brn.h"

#include "fof_util.h"


extern d_SD  sr_SD[];

//  #include "fof_se.h"
//  #include "fof_se2.h"

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SD_Mngr_Dan
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
int  SD_Mngr_Dan (d_SD *a_SD, char cr_TmpFN[], char cr_ErrMes[])
{
int   i;
int   i_midburn, i_burntime, i_stoptime, i_duffdensity, i_dt;
REAL  r_duffdepth, r_bd, r_pd, r_xo, r_ls, r_ga;
REAL  r_xwo, r_cop, r_starttemp, r_startwc, r_duffheat;

REAL  r_Rabs, r_Qo, r_qm, r_time, r_Rabsub, r_simdt;
REAL  rr_z[e_mplus1+1],  rr_w[e_mplus1+1], rr_t[e_mplus1+1];
int   i_success, i_Tim;
FILE  *fh_Out;

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
    
  if ( !stricmp (a_SD->cr_DoIt, "Yes")) /* heat is coming from DuffBurn() function  */
    r_Qo = r_duffheat;
  else 
    r_Qo = r_duffheat * r_duffdepth * i_duffdensity;
 
 r_qm = 0.57 * r_Qo / ( i_burntime * 60 );  /* max. rate of heat production; W/m2} */
  r_time = 0;

  while ( r_time < i_stoptime ) {

    r_Rabsub = r_Rabs + r_qm * exp ( -sqr ( (i_midburn - r_time ) / i_burntime));
    r_simdt = i_dt;
    i_success = 0;

//-----------------------------------
// send to Visual Studio Output Window during Debug
#define BUF_SIZE 400
   wchar_t wcsbuf[BUF_SIZE];
   swprintf(wcsbuf, BUF_SIZE, L"%f\n", r_Rabsub);
//   OutputDebugString(wcsbuf);
//------------------------------------


    while (1) {
      if ( !soiltemp_step(r_Rabsub,i_dt,&i_success,r_time)){ /* simulate a time step  } */
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

/**********************************************************************
* Name: SE_Mngr_Array
* Desc: Non-Soil Heat simulation
* NOTE: This one is setup to use the Burnup Fire Intensity array
*
*
* Note-1: Take a look at the SE_Init() - that shows how we were using
*         the Burnup fire intensity in the original non-duff model 
*         --> radiant heat coming from burnup is in Kilowatts per Meter Sq.
*         so we multiply by 1000 to gets watts sq.
*
*
***********************************************************************/
#ifdef eoeoeo
int   SE_Mngr_Array (d_SE *a_SE, float fr_FI[], char cr_TmpFN[], char cr_ErrMes[])
{
int  i, i_success, i_Tim;
int  i_starttime, i_burntime, i_cooltime, i_heatconst, i_coolconst,i_dt;
REAL r_bd,r_pd,r_xo,r_ls,r_ga,r_xwo,r_cop,r_startwc,r_starttemp,r_maxRabs;
REAL r_Rabs, r_time, r_exh, r_exc, r_Rabsub, r_simdt;
REAL rr_node [e_mplus1+1], rr_z [e_mplus1+1], rr_w [e_mplus1+1];
REAL rr_t [e_mplus1+1];
FILE  *fh_Out;

int iX, iS_End ; 
float f_fi; 

  strcpy (cr_ErrMes,"");

  if ( !strcmp (cr_TmpFN,"") ) {             /* Tmp output File Name         */
     fh_Out = NULL;                          /*  Not doing                   */
     goto A; }

  fh_Out = SI_fopen (cr_TmpFN,"w");
  if ( fh_Out == NULL ) {
    sprintf (cr_ErrMes, "Can't open output file: %s\n", cr_TmpFN);
    return 0;  }

  TmpFil_Heading (fh_Out); 

A:
    i_starttime = a_SE->i_starttime;         /* see note-1 above             */
    i_burntime  = a_SE->i_burntime;
    i_cooltime  = a_SE->i_cooltime;
    i_heatconst = a_SE->i_heatconst;
    i_coolconst = a_SE->i_coolconst;
    i_dt        = a_SE->i_dt;      /* Time Step */
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

// NOTE NOTE
//   this is still a work in progress
//   a lot of the orginal code is still in here which isn't doing much
//   if anythink, I'll need to clean this up as we see what's what

  iS_End = -1; 
  while ( r_time < i_cooltime ) {
     iX = r_time; 

     f_fi = _Get_FirInt (r_time, fr_FI, 1500);
     f_fi = f_fi * 1000.0;       /* See Note-1 above */
     f_fi = f_fi * 0.25;          /* percent of intensity that goes down into soil */ 
     
     f_fi = f_fi + r_Rabs;

    if ( r_time < i_starttime ) {
      r_exh =0; r_exc = 1; }

    if ( r_time > i_starttime &&  r_time < i_burntime)               /* during the burn */
      r_exh = ( 1 - exp ( -(r_time - i_starttime) / i_heatconst));   /* exp. heating and */

    if ( r_time > i_burntime)                      /* cooling during */
      r_exc = exp(-(r_time - i_burntime) /i_coolconst);    /*and after the burn  */

    r_Rabsub = r_maxRabs * r_exh * r_exc + r_Rabs;    /* set abs. rad. at up. bnd */

    r_simdt = i_dt;

    while (1) {
 
     if ( !soiltemp_step(f_fi, i_dt,&i_success,r_time)){ /* simulate a time step  } */

//     if ( !soiltemp_step(r_Rabsub, i_dt,&i_success,r_time)){ /* simulate a time step  } */
        sprintf (cr_ErrMes, "%s Non-Duff Model %s",e_SoilErr, e_SoiSimFail);         /* Unable to do Soil Sim   */
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
