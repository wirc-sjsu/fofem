/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: bur_but.c
* Desc: Some burnup misc/utility kind of functions
*
* Date: 3/17/05
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
#ifdef ANSI
#define WINAPI
#else
#include <windows.h>
#endif


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <malloc.h>

#include  "fof_sgv.h"
#include "fof_gen.h"
#include "fof_ci.h"
#include "fof_co.h"
#include "bur_brn.h"
#include "bur_but.h"
#include "fof_util.h"
#include "fof_bcm.h"

#include "..\fof_dll\Unit_Abv.h"
#include "..\fof_dll\Brn_FileSw.h" 

// #include "fof_wd.h"
// #include "win_ccwf.h"

extern double  wdry  [];
extern double  htval [];
extern double  fmois [];
extern double  dendry[];
extern double  sigma [];
extern double  cheat [];
extern double  condry[];
extern double  tpig  [];
extern double  tchar [];
extern double  ash   [];

extern long number;

extern float gf_CriInt;

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: But_RBIF
* Desc: Run Burnup from an Input file.
*       Burnup can run usng an input file, everything needed to run
*        Burnup is in the file, loads, etc. FOFEM has an option that
*        will create a sample file, you can look at.
* Note-1: We only run the Burnup input file using Default Emission Factors
*         If we wanted to run with the Expanded Emission Factors that would 
*         require having to load the Expanded Emis Factors into burnup 
*   In: cr_FN........Input file name
*       cr_AmtFN.....A file name here will cause burnup to save the
*                    Remaining or Post load file
*       cr_EmisFN....A file name here will cause burnup to save the
*                    Emissons output file
*   Out: cr_Err......Error/Warning Message
*   Ret: 1 = OK....see code below for other return values
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int But_RBIF (char cr_FN[], char cr_AmtFN[], char cr_EmisFN[], char cr_Err[])
{
int i;
char cr[400];

   i = But_RunBrnFil (cr_FN, cr);
   if ( i == 0 ) {                           /* had an error                 */
     strcpy (cr_Err, "Error - Reading Burnup input parameter file.\n\n");
     strcat (cr_Err,cr);                     /* append rest of detail err mes*/
     strcat (cr_Err, "\n Check for:\n");
     strcat (cr_Err, "  - all required switches\n");
     strcat (cr_Err, "  - valid switch arguments\n");
     strcat (cr_Err, "  - switches in proper order\n");
     return 0; }

/* Tell burnup to use the Default Emissions factors - See Note-1 above */
 //  gf_CriInt = -1.0; 

   i = BRN_Run(cr_AmtFN,                     /* Post Load output file        */
               cr_EmisFN,                    /* Emission output file         */
               -1,                           /* % duf consumed -1            */
               0,0,0,                        /* Consumed, Herb,Shr,Bra,Fol  */
               cr_Err);

   if ( i == 0 ) {                           /*  Error                       */
     return 0; }
   if ( i == 2 )                             /* Burnup couldn't ignite       */
     return 2;
   if ( i == -1 )                            /* Burnup did a div by 0 err    */
     return 3;                               /* 'Summary' func atmpt div 0   */

   return 1;                                 /* Everything is OK             */
}



/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: BRN_ReadInFil
* Desc: Read the input burnup input file.
* Note: The design of the input file was not of my making so gotta deal
*         with it as is. It has those 11 switchs on the front then a file
*         name (that doesn't appear to have any reason for being there)
*         then any number of lines with the fuel load info.
*         So I:
*          - get the 11 switches at the top of the file.
*          - look for that bogus file name and get past it
*             I wrote code so if it's not there that's ok
*          - get the fuel loading lines
*   In: cr_FN......File Name
*  Out: cr_Err.....Error Message
*  Ret: 1 OK,    0 Error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int But_RunBrnFil (char cr_FN[], char cr_Err[])
{
int    i, i_Ret, iN;
long   v, w;
#define eC_Line 250
char   cr_Line[eC_Line], cr1[100], cr2[100], *eof, c_Type;
double d_a, d_b, d_c, d_d, d_e, d_f, d_g, d_h, d_x, d_y;
FILE   *fh_In;
d_CFF  s_CFF;

long    l_ntimes = 3000;       /* max number of iterations                   */
long    l_NumFuelCats = 8;     /* number of fuel categories this time        */
double  d_fi = 150.0;          /* initializing fire intensity (kW/m2/s)      */
double  d_ti = 30.0;           /* duration of initial intensity pulse  (s)   */
double  d_WinSpe = 0.0;
double  d_FueDep = 0.523;      /* fuelbed depth (m)                          */
double  d_tamb = 27.0;         /* ambient temperature (C)                    */
double  d_r0   = 1.8;          /* minimum value of mixing parameter          */
double  d_dr   = 0.4;          /* maximum addition to mixing parameter       */
double  d_dt   = 10;           /* timestep                                   */
double  d_wdf  = 11.0;         /* duff weight (kg/m2)                        */
double  d_dfm  = 1.62;         /* duff moisture content (fraction)           */


   strcpy (cr_Err,"");
   fh_In = fopen (cr_FN, "r");               /* Open Input File              */
   if (  fh_In == NULL) {
      sprintf(cr_Err,"Can't Open Burnup Tmp Input file: %s \n", cr_FN);
      return 0; }

   CFF_Init (&s_CFF);                        /* to check for switches        */
   iN = 0;

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* Get the 11 switches/args at top of file                                   */
   i_Ret = 0;
   while ( 1 ) {
     if ( CFF_ChkAll (&s_CFF) )              /* see if have all switches yet */
       break;                                /*  OK gottem' all              */
     eof = fgets ( cr_Line, 200, fh_In  );   /* Read a line from file        */
     if ( eof == NULL )  {                   /* shouldn't hit EOF            */
       Bur_Error (cr_Err,"Missing Switch(s)","");
       i_Ret = 0;
       goto X; }

     Blk_End_Line (cr_Line,eC_Line);         /* get rid of end line chars    */
     if ( isBlank(cr_Line) )                 /* skip blank lines             */
       continue;
     strcpy (cr1,"");
     strcpy (cr2,"");
     sscanf (cr_Line, "%s %s", cr1,cr2);     /* get switch & argument        */
     if ( !strcmp (cr1,"") )                 /* Empty line                   */
        continue;
     if ( cr1[0] == '#' )                    /* Skip any comments lines      */
       continue;

 //    bstrupr (cr1);                          /* get string to upper case     */

     if ( !BRN_ChkSwi (cr1) ) {              /* check for valid switch       */
        Bur_Error (cr_Err,"Invalid Switch.\n",cr_Line);
        i_Ret = 0;
        goto X; }

     if ( !strcmp (cr2,"") ) {               /* must have an arg             */
        Bur_Error (cr_Err,"Switch Has Missing Argument.\n",cr_Line);
        i_Ret = 0;
        goto X; }

     c_Type = Get_NumTyp (cr2);              /* 2cd arg must be number       */
     if ( c_Type == 'X' ) {
        Bur_Error (cr_Err,"Invalid Argument.\n",cr_Line);
        i_Ret = 0;
        goto X; }

     if (!stricmp(cr1, e_swMAX_TIMES )) {
          l_ntimes = atoi(cr2);
          s_CFF.i_MAX_TIMES++;
          continue; }
     if (_stristr(cr1, e_swINTENSITY)) {
         d_fi = atof(cr2);
         s_CFF.i_INTENSITY++;
         continue;}
     if (_stristr(cr1, e_swIG_TIME      )){ d_ti     = atof(cr2); s_CFF.i_IG_TIME++;      continue;}
     if (_stristr(cr1, e_swWINDSPEED    )){ d_WinSpe = atof(cr2); s_CFF.i_WINDSPEED++;    continue;}
     if (_stristr(cr1, e_swDEPTH        )){ d_FueDep = atof(cr2); s_CFF.i_DEPTH++;        continue;}
     if (_stristr(cr1, e_swAMBIENT_TEMP )){ d_tamb   = atof(cr2); s_CFF.i_AMBIENT_TEMP++; continue;}
     if (_stristr(cr1, e_swR0           )){ d_r0     = atof(cr2); s_CFF.i_r0++;           continue; }
     if (_stristr(cr1, e_swDR           )){ d_dr     = atof(cr2); s_CFF.i_dr++;           continue; }
     if (_stristr(cr1, e_swTIMESTEP     )){ d_dt     = atof(cr2); s_CFF.i_TIMESTEP++;     continue; }
     if (_stristr(cr1, e_swDUFF_LOAD    )){ d_wdf    = atof(cr2); s_CFF.i_DUFF_LOAD++;    continue; }
     if (_stristr(cr1, e_swDUFF_MOIST   )){ d_dfm    = atof(cr2); s_CFF.i_DUFF_MOIST++;   continue;}
     Bur_Error(cr_Err,"Invalid Switch",cr_Line);
     i_Ret = 0;
     goto X;  }

     BRN_SetFireDat (l_ntimes,d_fi,d_ti,d_WinSpe,d_FueDep,d_tamb,d_r0,d_dr,d_dt,d_wdf,d_dfm);

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* Look for bogus file name, that's in the file                              */
#ifdef TAKEOUT
   while ( 1 ) {
     eof = fgets ( cr_Line, 200, fh_In  );   /* Read a line from file        */
     if ( eof == NULL ) {                    /* End of File                  */
       Bur_Error(cr_Err,"No fuel loading data found in file.","");
       i_Ret = 0;
       goto X; }

     Blk_End_Line (cr_Line,eC_Line);         /* get rid of end line chars    */
     if ( isBlank(cr_Line) )                 /* skip blank lines             */
       continue;

     strcpy (cr1,"");
     sscanf (cr_Line, "%s", cr1);
     if ( cr1[0] == '#' )                    /* Skip any comments lines      */
       continue;
     if ( !strcmp (cr1,"") )                 /* skip empty lines             */
        continue;

     c_Type = Get_NumTyp (cr1);              /* get number type              */
     if ( c_Type == 'I' )                    /* integer                      */
       goto Num;                             /* go do fuel loading line      */
     if ( c_Type == 'F' )                    /* float                        */
       goto Num;                             /* go do fuel loading line      */
     if ( c_Type == 'X' )                    /* must be the bogus file name  */
       break;                                /* time to go do fuel lines     */
     strcpy (cr_Err,"Logic Error - BRN_ReadInFil");
     i_Ret = 0;
     goto X;
   }
#endif 

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* read in the fuel loading lines                                            */
   while ( 1 ) {
     eof = fgets ( cr_Line, 200, fh_In  );   /* Read a line from file        */
     if ( eof == NULL )                      /* End of File                  */
       break;

     Blk_End_Line (cr_Line,eC_Line);         /* get rid of end line chars    */
     if ( isBlank(cr_Line) )                 /* skip blank lines             */
       continue;

     strcpy (cr1,"");
     sscanf (cr_Line, "%s", cr1);
     if ( cr1[0] == '#' )                    /* Skip any comments lines      */
       continue;
     if ( !strcmp (cr1,"") )
       continue;                             /* skip empty lines             */

Num:
     i = Bur_ChkArgs (cr_Line);              /* make sure they're all nums   */
     if ( i > 0 ) {
       sprintf (cr1,"argument %d is not numberic or there is not 12 numbers on line.\n",i);
       Bur_Error (cr_Err, cr1,cr_Line);
       i_Ret = 0;
       goto X; }

     sscanf (cr_Line, "%ld %ld %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
              &v,&w,&d_a,&d_b,&d_c,&d_d,&d_e,&d_f,&d_g,&d_h,&d_x,&d_y);

     if ( d_a <= 0.0)                   /* if no fuel Load                   */
        continue;
     wdry  [iN] = d_a;                  /* load into arrays that burnup uses */
     htval [iN] = d_b*1000.0;           /*  to do its calcs                  */
     fmois [iN] = d_c;
     dendry[iN] = d_d;
     sigma [iN] = d_e;
     cheat [iN] = d_f;
     condry[iN] = d_g;
     tpig  [iN] = d_h + e_tpig_adj;
     tchar [iN] = d_x + e_tchar_adj;
     ash   [iN] = d_y;
     iN++;
     number = iN;                       /* number of fuel loads in arrays    */

     if ( iN == MAXNO ) {
       Bur_Error (cr_Err, "Too Many fuel loads in burnup input file.\n","");
       i_Ret = 0;
       goto X; }
   }

   if ( iN == 0 ) {
     Bur_Error (cr_Err, "No fuel load data found in input file \n","");
     i_Ret = 0;
     goto X; }

   if ( !BRN_CheckData(cr_Err) ) {           /* Check input data to burnup   */
     i_Ret = 0;
     goto X; }                               /* Error                        */

   i_Ret = 1;                                /* OK no errors                 */


X:
   fclose (fh_In);
   return i_Ret;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: But_CreInpFil
* Desc: Create a Sample input file for burnup for the FOFEM option....
*         "Create Sampe Burnup Input File"
*       Here we get and use the current Screen settings, fuel loads, etc
*       to build an input file that can be used for Burnup, using FOFEM's
*       "Run Burnup from Input File"
* Change Note: This function got revamped when I was putting in the
*              FCCS stuff, see Old_But_CreInpFil()
*              I noticed I didn't need to care here about FCCS or other
*              because the 3+ loads were already set in the CI by functions
*              that were calling this one.
*
* Note-2: This will make sure that there is always at least something
*           in one of the Down Wood loads. So we put a small amount in 1 hr.
*           Burnup flips out if for example you send in only litter,
*           Also, it will let burnup run with no other down wood loads
*           so it can deal with Duff and other Loads that
*           are needed for emissions stuff.
*           The small amount will not show up any where in consumed loads
*            and basically come back looking like 0.
*   In: a_CI......Loading and info from main window
*       cr_FN.....Input File Name
*  Ret: 1 OK,  0 Error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  But_CreInpFil (d_CI *a_CI, char cr_FN[], char cr_ErrMes[])
{
int   iX_Bur;
float f, f_Load, f_MoistDW10, f_Moi, f_MoiRot, f_MoiSnd;
FILE   *fh;

   strcpy (cr_ErrMes,""); 
   iX_Bur = 1;

   fh = fopen (cr_FN, "w");                  /* open file                    */
   if ( fh == NULL ) {                       /* Error                        */
     strcpy  (cr_ErrMes, "Can't Open Temp Burnup File - Bur_CreInFil");
     return 0; }

/*...........................................................................*/
    fprintf (fh, "%-15s   %d.0\n",           e_swMAX_TIMES,    e_MAX_TIMES   );
    fprintf (fh, "%-15s   %7.2f   (%s) \n",  e_swINTENSITY,    e_INTENSITY,    e_KwSqM   );
    fprintf (fh, "%-15s   %7.2f   (%s) \n",  e_swIG_TIME,      e_IG_TIME,      e_Second     );
    fprintf (fh, "%-15s   %7.2f   (%s) \n",  e_swWINDSPEED,    e_WINDSPEED,    e_MetSec   );
    fprintf (fh, "%-15s   %7.2f   (%s) \n",  e_swDEPTH,        e_DEPTH,        e_Meter       );
    fprintf (fh, "%-15s   %7.2f   (%s) \n",  e_swAMBIENT_TEMP, e_AMBIENT_TEMP, e_Celsius);
    fprintf (fh, "%-15s   %7.2f \n",         e_swR0,e_R0);
    fprintf (fh, "%-15s   %7.2f \n",         e_swDR,e_DR);
    fprintf (fh, "%-15s   %7.2f   (%s) \n",  e_swTIMESTEP,     e_TIMESTEP, e_Second    );

    f = TPA_To_KiSq (a_CI->f_Duff);
    fprintf (fh, "%-15s     %5.2f   (%s) \n", e_swDUFF_LOAD, f, e_KgSqM );

    f = a_CI->f_MoistDuff / 100;
    fprintf (fh, "%-15s     %5.2f", e_swDUFF_MOIST, f);

    fprintf (fh, "\n");
//    fprintf (fh, "dummy_file.nam\n");
 
   f_MoistDW10    = a_CI->f_MoistDW10 / (float) 100;

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* Litter                                                                    */
    f_Load   = TPA_To_KiSq (a_CI->f_Lit);
    if ( f_Load != 0 ){
      f_Moi = BCM_DW10M_Adj ("DW1", f_MoistDW10);
      fprintf (fh, e_SoundFmt, iX_Bur++,f_Load, f_Moi, e_SURat_Lit);   }

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* DW 0->1/4 - 1 Hr                                                          */
    f_Load   = TPA_To_KiSq (a_CI->f_DW1);
    if ( f_Load == 0 )                       /* See Note-2 above             */
      f_Load = 0.0000001;
    if ( f_Load != 0 ){
      f_Moi = BCM_DW10M_Adj ("DW1", f_MoistDW10);
      fprintf (fh, e_SoundFmt, iX_Bur++,f_Load, f_Moi, e_SURat_DW1); }

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* DW 1/4->1 - 10 Hr                                                         */
    f_Load   = TPA_To_KiSq (a_CI->f_DW10);
    if ( f_Load != 0 ){
      f_Moi = BCM_DW10M_Adj ("DW10", f_MoistDW10);
      fprintf (fh, e_SoundFmt, iX_Bur++,f_Load,f_Moi, e_SURat_DW10); }

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* DW 1->3 100 Hr                                                            */
    f_Load   = TPA_To_KiSq (a_CI->f_DW100);
    if ( f_Load != 0 ){
      f_Moi = BCM_DW10M_Adj ("DW100", f_MoistDW10);
      fprintf (fh, e_SoundFmt, iX_Bur++,  f_Load, f_Moi, e_SURat_DW100); }

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* Distribute 3+ load across the classes                                     */
/* 1st get the need Sound & Rotten Moistures                                 */

    f_MoiSnd = a_CI->f_MoistDW1000 / (float) 100;
    f_MoiRot = BCM_DW1k_MoiRot(f_MoiSnd);


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/*  3+ Sound and Rotten                                                      */
/*  Note: these loads have been already distributed by sound & rotten        */
/*        by previous caller of this function,                               */

/* DW 3 -> 6 ................................................................*/
    f_Load   = TPA_To_KiSq (a_CI->f_Snd_DW3);
    if ( f_Load != 0 )
      fprintf (fh, e_SoundFmt, iX_Bur++, f_Load, f_MoiSnd, e_SURat_DWk_3_6);

    f_Load   = TPA_To_KiSq (a_CI->f_Rot_DW3);
    if ( f_Load != 0 )
      fprintf (fh, e_RottenFmt, iX_Bur++,f_Load, f_MoiRot, e_SURat_DWk_3_6);

/* DW 6 -> 9 ................................................................*/
    f_Load   = TPA_To_KiSq (a_CI->f_Snd_DW6);
    if ( f_Load != 0 )
      fprintf (fh, e_SoundFmt, iX_Bur++,f_Load, f_MoiSnd, e_SURat_DWk_6_9);

    f_Load   = TPA_To_KiSq (a_CI->f_Rot_DW6);
    if ( f_Load != 0 )
      fprintf (fh, e_RottenFmt, iX_Bur++, f_Load, f_MoiRot, e_SURat_DWk_6_9);

/* DW 9 -> 20 ..............................................................*/
    f_Load   = TPA_To_KiSq (a_CI->f_Snd_DW9);
    if ( f_Load != 0 )
      fprintf (fh, e_SoundFmt, iX_Bur++,f_Load, f_MoiSnd, e_SURat_DWk_9_20);

    f_Load   = TPA_To_KiSq (a_CI->f_Rot_DW9);
    if ( f_Load != 0 )
      fprintf (fh, e_RottenFmt,iX_Bur++,  f_Load, f_MoiRot, e_SURat_DWk_9_20);

/* DW 20 -> ................................................................*/
    f_Load   = TPA_To_KiSq (a_CI->f_Snd_DW20);
    if ( f_Load != 0 )
      fprintf (fh, e_SoundFmt, iX_Bur++, f_Load, f_MoiSnd, e_SURat_DWk_20);

    f_Load   = TPA_To_KiSq (a_CI->f_Rot_DW20);
    if ( f_Load != 0 )
      fprintf (fh, e_RottenFmt, iX_Bur++, f_Load, f_MoiRot, e_SURat_DWk_20);

  fclose (fh);
  return 1;
}

#ifdef wowowo
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: But_RemoveTmp
* Desc: Removie the 3 temp files, input, 'heat' and 'amount'
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  But_RemoveTmp ()
{
  t_DeleteFile (e_BurTmpIn_FN);
  t_DeleteFile (e_BrnTmpAmt);
  t_DeleteFile (e_BrnTmpHea);
}

#endif 