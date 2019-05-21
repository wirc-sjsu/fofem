/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: bat_mai.c
* Desc: Upper level functions for Consumed, Emissions and Mortality
*
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
#include <windows.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <float.h>

#include "Wnd_Mai.h" 
#include "Wnd_Util.h"

#include "wnd_def.h" 
#include  "fof_sgv.h"
#include "bat_mai.h"
#include "fof_ci.h"
#include "fof_co.h"
#include "fof_co2.h"
#include "fof_cm.h"
#include "fof_sh.h"
#include "fof_iss.h"
#include "fof_mrt.h"
#include "bat_sif.h"
#include "fof_util.h"
#include "bat_bif.h"
#include "bat_bof.h"
#include "cdf_util.h"
#include "wnd_def.h" 
#include "fof_nes.h" 

#include "..\fof_dll\Brn_FileSw.h"

#define   e_NoSoilType "NA"

char cr_ErrMes[3000];
char  gcr_tmp[2000];

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
int   Show_Soil (d_SO *a_SO);
int  _ChkSoil (d_SI *a_SI, char cr_ErrMes[]);
int  _ChkMoistCond (d_SI *a_SI, char cr_ErrMes[]);

int   _AskUser (void);
int    Bat_CovGrp (d_CI *a_CI, char cr_ErrMes[]);
int   _ChkCM (float f, char cr[], char cr_ErrMes[]);
int   _ChkConMiss (d_CI *a_CI, char cr_ErrMes[], char cr_1kFmt[]);
int   _ChkLim1000 (d_CI *a_CI, char cr_ErrMes[], char cr_1kFmt[]);
void  _ErrMessage (char cr_StdId[], char cr_ErrMes[], int *aiN_Errs,FILE *fh_Err);
void  _SoilNAMess (char cr_StdId[], char cr_NoSoilType[]);
int   _MortErrMes (d_SIF *a_SIF, char cr_ErrMes[], int *aiN_Err, FILE *fh_Err);
void  _MortBadFile (int iN_Rec, FILE *fh_Err);
int   _MortChkMiss (d_SIF *a_SIF, char cr_ErrMes[]);
int    Chk_SIF (d_SIF *a_SIF, d_MI *a_MI, char cr[]);
void _xError_Window  (char A[], char B[], FILE *fh_Err);
int  Get_BrnParFil (char cr_BrnParFN[], d_CI *a_CI, char cr_ErrMes[]);
int _FindSw (FILE *fh, float *af, char cr_Switch[],  char cr_ErrMes[]);
void _SetBrnParm (d_CI *a_CI, d_CI *a);


 // char gcr_tmp[5000];
 void ErrBat_Window (char A[], char B[]); 
int _FindSw (FILE *fh, float *af, char cr_Switch[], char cr_ErrMes[]);
int  Get_BrnParFil (char cr_BrnParFN[], d_CI *a_CI, char cr_ErrMes[]);
void _ErrMessage (char cr_StdId[], char cr_ErrMes[], int *aiN_Errs, FILE *fh_Err);
void  SRM_SoiSimFail (d_SI *a_SI, char cr_StdId[], char cr_Mess[]);
int _ChkLogDis (char cr[], char cr_ErrMes[]) ;

int SAF_WeightDist (d_CI *a_CI, char cr_WD[], float f_Load, float f_pc); 


void AppendPFN (char cr_Path[], char cr_FN[], char cr_Out[]);


int _SetEmisGroup (d_CI *a_CI, char cr_EmiFla[], char cr_EmiSmo[], char cr_EmiDuf[], char cr_ErrMes[]);



/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: ConEmi_Mngr
* Desc: Do the Consumed, Emissions and/or Soil
*       Reads input file, do calcs and do output records.
* Change: 10-30-05 overhauled to use my CDF_* functions, which are much
*          better for error checking, detecting missing inputs, etc
* Note-1: I put this checks in here, The Soil Heating code checks them too
*         but I wanted to check them up front for some reason.
* Note-2: Normal input has 1000 hour load as a total so we need to distribute
*          it using the skew code into it's size classes, don't have to do this
*          when the input comes in as individual classes, there is a size
*          class switch in the input file to tell how file is formated
* Note-3: Change 11-6-05, a check has been put in the soil code to catch
*          the infinite loop bug, and a Popup Wnd Message is displayed
*          here when it happens. This bug first got noticed when DL was
*          running his batch files.
*
* Note-4: ARB - California Air Resourses Board
*         I added 2 switches that can be used at the top of a standard batch
*         input file for the ARB
*         1. #BurnUpFile filnam.txt
*            Specifies the name of a file that holds specific burnup inputs
*            like wind speed, sigma, etc  these are most of the same inputs
*            that are in a burnup input file
*         2. #Emission-File Prefix - the presence of this switch will cause
*             an output of a time emission file for each plot in the batch
*             input file.
*             The emission files are named after the plot id names and using
*             the 'PreFix in the switch argument.
* 
* Note-5 When batch is run from the Command Line the Emission Factor Group .csv file
*        will need to get read in and loaded, normally this is done when the GUI 
*        is being initialized so the GUI batch doesn't have to do it.
*
*    In: cr_InFN.....Input File name & Path
*        cr_OutFN....Output File name & Path
*        iS_Soil.....1 = do soil heating, else set to 0
*        iS_CL.......1 = put Heading Column Labels to output file
*        cr_Mode....."GUI" or "CommandLine" tells who's call this function
*   Out: *iN.........number of output records created, ( plots processed )
*        *ai_SoilErr.number of soil errors
* Ret: 0 = No Errors, else number of errors
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  BAT_CESExec (char cr_InFN[], char cr_OutFN[], int *iN, int iS_Soil,
                  int *ai_SoilErr, int iS_CL, FILE *fh_Err, char cr_Mode[])
{
int i, iN_Errs, iN_Cnt;
float f; 
char cr_StdId[eC_StdId], cr_1kFmt[30], cr[1000];
char  cr_BrnParFN[300], cr_EmisSw[30];
char cr_ErrMes[1000], cr_FN[1000];
char cr_EmiFla[100], cr_EmiSmo[100], cr_EmiDuf[100];
d_CI s_CI;
d_CI s_CIBrnParm;  /* in case we use a Burnup Parameter file */
d_CO s_CO;
d_SI s_SI;
d_SO s_SO;

  *iN = 0;
  iN_Errs = 0;
  *ai_SoilErr  = 0;
  iN_Cnt = 0;

  CI_Init (&s_CI); 

  if ( !BIF_Open (cr_InFN) ) {
      sprintf (gcr_tmp, "Can't Open Input File:\n%s", cr_InFN);
    if ( fh_Err != NULL )
      fprintf (fh_Err,"%s\n",gcr_tmp);
    else
      ErrBat_Window (gcr_tmp, "BAT_CESExec()");
    return 1; }

  if ( !BOF_Open (cr_OutFN) ) {
    sprintf (gcr_tmp, "Can't Open Output File:\n%s", cr_OutFN);
    if ( fh_Err != NULL )
      fprintf (fh_Err,"%s\n",gcr_tmp);
    else
      ErrBat_Window (gcr_tmp, "BAT_CESExec()");
    BIF_Close ();
    return 1; }

  if ( iS_CL == 1 )
     BOF_PutHeading (iS_Soil);          /* Put heading labels to output file */

/* Batch input file switches at top of file - if any */
  if ( !BIF_Switch (cr_1kFmt,iS_Soil,gcr_tmp)){ /* see if any, what swtch in input fil */
    if ( fh_Err != NULL )
      fprintf (fh_Err,"%s\n",gcr_tmp);
    else
      ErrBat_Window (gcr_tmp, "BAT_CESExec()");
    iN_Errs = 1;
    goto Close; }

/*........................................................*/
/* Set Note-4 above, check for ARB switches               */
  if ( !BIF_Switch2 (cr_BrnParFN, cr_EmisSw, gcr_tmp)) {
    if ( fh_Err != NULL )
      fprintf (fh_Err,"%s\n",gcr_tmp);
    else
      ErrBat_Window (gcr_tmp, "BAT_CESExec()");
    iN_Errs = 1;
    goto Close; }

/*........................................................*/
/* Get the Emission Group Numbers from input batch file */
/*  they will get used below */
   if ( !BIF_SwitchEG (cr_EmiFla, cr_EmiSmo, cr_EmiDuf, gcr_tmp)) {
    if ( fh_Err != NULL )
      fprintf (fh_Err,"%s\n",gcr_tmp);
    else
      ErrBat_Window (gcr_tmp, "BAT_CESExec()");
    iN_Errs = 1;
    goto Close; }

    if ( !stricmp (cr_Mode, "GUI" ) ) 
       goto GUI;

/* We're running from the Command File, so we need to load the Emission Factor Group .csv file */
/*  See Note-5 above   */ 
    if ( !NES_Read ("", gcr_tmp)) {  /* input file and load table */
       sprintf (cr, "\n %s - file needs to be in the same folder that you are running your batch from.", e_EmiFN); 
       strcat (gcr_tmp, cr); 
       if ( fh_Err != NULL )
         fprintf (fh_Err,"%s\n",gcr_tmp);
       else
          ErrBat_Window (gcr_tmp, "BAT_CESExec()");
        iN_Errs = 1;
        goto Close;  }

GUI:
/* if there was a Burnup Parameter File Name switch in the Batch input file */
  if ( strcmp(cr_BrnParFN,"") ) {

    AppendPFN (cr_InFN, cr_BrnParFN, cr_FN);

    if ( !Get_BrnParFil (cr_FN,&s_CIBrnParm,gcr_tmp)){  /* read the Brnup Parm file in */
      if ( fh_Err != NULL )
        fprintf (fh_Err,"%s\n",gcr_tmp);
      else
        ErrBat_Window (gcr_tmp, "BAT_CESExec()");
      iN_Errs = 1;
      goto Close; }  }

  while (1) {                           /* Do each record                    */

    CO_Init (&s_CO);

    iN_Cnt++;                           /* Count records read in             */
    i= BIF_Get (&s_CI,&s_SI,cr_StdId,cr_ErrMes,iS_Soil, cr_1kFmt);
    if ( i == 0 )                       /* End of file                       */
       break;
    if ( i == -1 ) {                    /* Serious Error, beat it            */
      if ( fh_Err != NULL )
        fprintf (fh_Err,"%s\n",cr_ErrMes);
      else
        ErrBat_Window ("Input file does not appear to be properly formated", "BAT_CESExec()");
      iN_Errs++;
      goto Close; }

    if ( strcmp (cr_ErrMes,"") ) {      /* Bad field format type errors      */
      if ( iN_Cnt == 1 )
        strcat (cr_ErrMes,"\n NOTE: The problem could be caused by column name headings on first line of input file\n");
       _ErrMessage (cr_StdId,cr_ErrMes,&iN_Errs,fh_Err);
      continue; }

/*  We have to do this here, setting group numbers and factors because the s_CI */
/*   gets initialized for each new plot, see in BIG_Get(...) above */ 
    if ( ! _SetEmisGroup (&s_CI, cr_EmiFla, cr_EmiSmo, cr_EmiDuf, cr_ErrMes) ){
      _ErrMessage (cr_StdId, cr_ErrMes, &iN_Errs,fh_Err);
      continue; }

    if ( !_ChkConMiss(&s_CI,cr_ErrMes,cr_1kFmt)){ /* Check of missing vals   */
      _ErrMessage (cr_StdId, cr_ErrMes, &iN_Errs,fh_Err);
      continue; }

    if ( !_ChkLim1000 (&s_CI,cr_ErrMes,cr_1kFmt)){/* Chk Lim on 1000 hour    */
      _ErrMessage (cr_StdId, cr_ErrMes, &iN_Errs,fh_Err);
      continue; }

    if ( !Bat_CovGrp (&s_CI,cr_ErrMes)){          /* Chk Cnvt Cov Grp Codes   */
      _ErrMessage (cr_StdId, cr_ErrMes, &iN_Errs,fh_Err);
      continue; }

    if ( !strcmp (cr_1kFmt, "")) {           /* Distrib 1000hr Note-2 above  */
//      if ( !WD_SetWeightDist (s_CI.f_DW1000, s_CI.f_pcRot, s_CI.cr_WD, &s_CI, cr_ErrMes))
       if ( !_ChkLogDis(s_CI.cr_WD,cr_ErrMes)) {
         _ErrMessage (cr_StdId, cr_ErrMes, &iN_Errs,fh_Err);
         continue; }
        f = s_CI.f_pcRot / 100.0; 
        SAF_WeightDist (&s_CI, s_CI.cr_WD, s_CI.f_DW1000, f); 
     }
    
/* If we have an emission output file switch, this will output put and  */
/*  emission file for each individual plot */
/* NOTE - note that '@' that put on the front of the file name below */
/*  that signals Burnup to output the emission file with flaming and smolder */
/*  emissions */
    if ( stricmp (cr_EmisSw,"")) {
      AppendPFN (cr_OutFN, cr_EmisSw, cr_FN);
      sprintf (s_CI.cr_EmiFN, "@%s-%s.emi",cr_FN,cr_StdId);
    }

/* if we have a Burnup Parameter file in use, then move the various  */
/*  Burnup Parameters we read in into the s_CI for use */
/*  these are the specific Burnup inputs like ignition temp, wind speed */
   if ( strcmp ( cr_BrnParFN, "") )
     _SetBrnParm (&s_CI,&s_CIBrnParm);

    i = CM_Mngr (&s_CI,&s_CO,cr_ErrMes);    /* Run Calculations             */
    if ( i == 0 ) {
        _ErrMessage (cr_StdId, cr_ErrMes, &iN_Errs,fh_Err);
        continue; }

    if ( i == 2 )
       printf (" Warning - Model Predicts No Ignition, Stand Id: %s \n", cr_StdId);

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* Soil Heating, if the switch is on do it                                    */
     if ( iS_Soil == 1 ) {                   /* Soil Switch is On            */

       SO_Init (&s_SO);                      /* init soil output struct      */

       if ( !stricmp (s_SI.cr_SoilType,e_NoSoilType)){ /* Soil Type 'NA'    */
         _SoilNAMess (cr_StdId,e_NoSoilType);
         goto Put; }                                  /* skip doing soil   */

       if ( !_ChkSoil (&s_SI,cr_ErrMes)){    /* check soil type, See Note-1  */
        _ErrMessage (cr_StdId, cr_ErrMes, ai_SoilErr,fh_Err);
         continue; }

       if ( !_ChkMoistCond (&s_SI,cr_ErrMes)){ /* Chk Moist Cond, wet,dry... */
        _ErrMessage (cr_StdId, cr_ErrMes, ai_SoilErr,fh_Err);
        continue; }

       s_SI.f_DufDepPre = s_CO.f_DufDepPre;  /* Get Pre & Post fire duff     */
       s_SI.f_DufDepPos = s_CO.f_DufDepPos;  /*  from Consumerd Calc         */
       strcpy (s_SI.cr_BrnIg,s_CO.cr_BrnIg); /* Tells if Burnup Ignited       */

/* Get the fire intesity array from CO struct into the Soil input Struct */
/* Both arrays are defined to be the same size */ 
float fr_Fi[eC_SGV]; 
    for ( i = 0; i < eC_SGV; i++ ) {
         fr_Fi[i] = s_CO.sr_SGV[i].f_Inten;} 


   if ( !SH_Mngr (&s_SI,&s_SO,s_CO.fr_SFI,s_CO.fr_SFIhs, "",cr_ErrMes)){
         if ( !strcmp (cr_ErrMes,e_SoiSimFail)){ /* See Note-3 above         */
           sprintf (cr,"Stand: %s, Output record will not contain soil info",cr_StdId);
           SRM_SoiSimFail (&s_SI,cr_StdId,cr);   /* Soil Sim Failed Message  */
           goto Put; }
         else {
          _ErrMessage (cr_StdId, cr_ErrMes, ai_SoilErr,fh_Err);
          continue; } }
      }

Put:
     BOF_Put (cr_StdId, &s_CO, iS_Soil, &s_SO);
    *iN = *iN + 1;

   }

Close:
    BIF_Close();
    BOF_Close();
    return iN_Errs;
}



/**************************************************************************
* Name: _SetEmisGroup
* Desc: set the Emis Group Numbers and their respective emission factors
*        into the CI (consumed input struct) - they will get passed into
*        burnup for use.
* Note-1: Critical Fire Intensity point, where flame turns to smolder
*  Ret: 1 ok, else 0 probably an invalid Group number 
***************************************************************************/
int _SetEmisGroup (d_CI *a_CI, char cr_EmiFla[], char cr_EmiSmo[], char cr_EmiDuf[], char cr_ErrMes[])
{
int i;

  strcpy (cr_ErrMes,"");

  a_CI->f_CriInt = NES_Get_CriFirInt();   /* See Note-1 above */

/* Flaming................ */
  strcpy (a_CI->cr_EmiFla,cr_EmiFla); 
  if ( !stricmp (cr_EmiFla,""))                    /* if none use default */   
     strcpy (a_CI->cr_EmiFla,e_DefFlaGrp); 
  i = NES_Get_MajFactor(a_CI->cr_EmiFla,
                        &a_CI->f_fCO, 
                        &a_CI->f_fCO2,
                        &a_CI->f_fCH4, 
                        &a_CI->f_fPM25, 
                        &a_CI->f_fPM10, 
                        &a_CI->f_fNOX, 
                        &a_CI->f_fSO2);
  if ( i == 0 ) {         /* Grp Num not found - this would be a logic error */
	sprintf (cr_ErrMes, "Input File Switch has invalid argument value: %s %s ", e_IF_Flame, cr_EmiFla); 
    return 0; }

/* Smolde ..................... */
  strcpy (a_CI->cr_EmiSmo,cr_EmiSmo); 
  if ( !stricmp (a_CI->cr_EmiSmo,""))  
     strcpy (a_CI->cr_EmiSmo,e_DefSmoGrp);     /* if none use default */
  i = NES_Get_MajFactor(a_CI->cr_EmiSmo,
                        &a_CI->f_sCO, 
                        &a_CI->f_sCO2,
                        &a_CI->f_sCH4, 
                        &a_CI->f_sPM25, 
                        &a_CI->f_sPM10, 
                        &a_CI->f_sNOX, 
                        &a_CI->f_sSO2);
  if ( i == 0 ) {         /* Grp Num not found - this would be a logic error */
	sprintf (cr_ErrMes,  "Input File Switch has invalid argument value: %s %s ", e_IF_Smold, cr_EmiSmo); 
    return 0; }

/* Duff ..................... */
  strcpy (a_CI->cr_EmiDuf,cr_EmiDuf); 
  if ( !stricmp (a_CI->cr_EmiDuf,""))  
     strcpy (a_CI->cr_EmiDuf,e_DefDufGrp);            /* if now use default */
  i = NES_Get_MajFactor(a_CI->cr_EmiDuf,
                        &a_CI->f_dCO, 
                        &a_CI->f_dCO2,
                        &a_CI->f_dCH4, 
                        &a_CI->f_dPM25, 
                        &a_CI->f_dPM10, 
                        &a_CI->f_dNOX, 
                        &a_CI->f_dSO2);
  if ( i == 0 ) {         /* Grp Num not found - this would be a logic error */
	sprintf (cr_ErrMes,  "Input File Switch has invalid argument value: %s %s ", e_IF_Duff, cr_EmiDuf); 
    return 0; }

  return 1; 

}
 
/************************************************************************
*
*************************************************************************/
void _SetBrnParm (d_CI *a_CI, d_CI *a)
{
  a_CI->f_SURat_Lit       = a->f_SURat_Lit;
  a_CI->f_SURat_DW1       = a->f_SURat_DW1;
  a_CI->f_SURat_DW10      = a->f_SURat_DW10;
  a_CI->f_SURat_DW100     = a->f_SURat_DW100;
  a_CI->f_SURat_DWk_3_6   = a->f_SURat_DWk_3_6;
  a_CI->f_SURat_DWk_6_9   = a->f_SURat_DWk_6_9;
  a_CI->f_SURat_DWk_9_20  = a->f_SURat_DWk_9_20;
  a_CI->f_SURat_DWk_20    = a->f_SURat_DWk_20;

  a_CI->l_MAX_TIMES    = a->l_MAX_TIMES;
  a_CI->f_INTENSITY    = a->f_INTENSITY;
  a_CI->f_IG_TIME      = a->f_IG_TIME  ;
  a_CI->f_WINDSPEED    = a->f_WINDSPEED;
  a_CI->f_DEPTH        = a->f_DEPTH    ;
  a_CI->f_AMBIENT_TEMP = a->f_AMBIENT_TEMP;
  a_CI->f_R0           = a->f_R0 ;
  a_CI->f_DR           = a->f_DR ;
  a_CI->f_TIMESTEP     = a->f_TIMESTEP;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _SoilNAMess
* Desc: Message when a NA (not available) soil type code is found in batch
*        input file, tell user no soil info will go to output file
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  _SoilNAMess (char cr_StdId[], char cr_NoSoilType[])
{
   printf ("Warning - Stand: %s No soil output information created for stand\n",cr_StdId);
   printf ("          due to %s soil type code found in input record.\n",cr_NoSoilType);

}
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _ErrMessage
* Desc: Do a common error message
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void _ErrMessage (char cr_StdId[], char cr_ErrMes[], int *aiN_Errs, FILE *fh_Err)
{

 if ( fh_Err == NULL ) {
   printf ("\n ERROR - Invalid Input, Stand Id: %s \n    %s \n", cr_StdId,cr_ErrMes);
   printf ("    No Output will be created for this stand.\n"); }
 else {
   fprintf (fh_Err, "\n ERROR - Invalid Input, Stand Id: %s \n    %s \n", cr_StdId,cr_ErrMes);
   fprintf (fh_Err, "    No Output will be created for this stand.\n"); }


   *aiN_Errs = *aiN_Errs + 1;
}



/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: BAT_MortExec
* Desc: Do the batch mortality.
*       Reads input file, does calcs and puts out one record per stand.
*   In: cr_InFN..Input File Name.
*       cr_OutFN..Output File Name
*       iS_CL.... 1 = Put Heading column labels to output file
*  Out: Number of records and stands processed
*  Ret: 0 or number of errors.
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  BAT_MortExec (char cr_InFN[], char cr_OutFN[], int *aiN_Std, int iS_CL, FILE *fh_Err)
{
int i, iN_Err, iN_Std, iN_Rec;
char   cr_CurStd[30], cr_PthFN[1000];
d_SIF s_SIF;
d_MI s_MI;
d_MO s_MO;
String ^Str; 

   iN_Std = iN_Err = iN_Rec = 0;
   *aiN_Std = 0;

 
  Str = System::Windows::Forms::Application::StartupPath;
  Str = Str + "\\" +  e_FOF_Spp ;
  _StrToChar (Str,cr_PthFN);        /* fof_spp.dat file is in same folder as .exe */ 


   MRT_LoadSpe ( cr_PthFN,"", cr_ErrMes);     /* Load Master Specis list file  */
   if ( strcmp (cr_ErrMes,"") ) {
     if ( fh_Err == NULL )
       printf ("ERROR - %s \n",cr_ErrMes);
     else
       fprintf (fh_Err, "ERROR - %s \n",cr_ErrMes);
     return 1; }

   if ( !SIF_Open (cr_InFN) ) {
     sprintf (cr_ErrMes, "Error Opening Input File: %s \n", cr_InFN);
     _xError_Window (cr_ErrMes,"Mortality()",fh_Err);
     return 1; }

   if ( SIF_isPPCP () )
     strcpy (s_MI.cr_CP,"Y");
   else
     strcpy (s_MI.cr_CP,"N");

   if ( !BOF_Open (cr_OutFN)){
     sprintf (cr_ErrMes, "Error Opening Output File: %s \n", cr_OutFN);
     _xError_Window (cr_ErrMes,"Mortality()",fh_Err);
     SIF_Close();
     return 1; }

   if ( iS_CL == 1 )                        /* Heading Col labels to         */
     BOF_MortHeading ();                    /*  output file                  */


First:
   i = SIF_Next (&s_SIF,cr_ErrMes);        /* Get 1st input record         */

   iN_Rec++;
   if ( i == 0 ) {
     _xError_Window ("No records found in input file","Input File Error",fh_Err);
     goto Close; }

   if ( i == -1 ) {
     _MortBadFile (iN_Rec,fh_Err);
     iN_Err++;
     goto Close; }
   if ( !_MortErrMes (&s_SIF,cr_ErrMes,&iN_Err,fh_Err))/* Chk for error message and disp */
      goto First;

   strcpy (cr_CurStd,s_SIF.cr_StdId);
   MO_Init(&s_MO);                           /* Init output struct           */

/*...........................................................................*/
   while (1) {                               /* Each input record            */

     if ( strcmp(cr_CurStd,s_SIF.cr_StdId)){ /* Stand Id Changed             */
        BOF_PutMort(&s_MO, cr_CurStd);       /* do output stand record       */
        MO_Init(&s_MO);                      /* Init output struct           */
        strcpy (cr_CurStd,s_SIF.cr_StdId);   /* set to do next Stand         */
        iN_Std++;
        if ( !strcmp (cr_CurStd,e_EOF))      /* if last record was read      */
           break; }

     _MortChkMiss (&s_SIF, cr_ErrMes);       /* Chk missing vals/codes       */
     if ( !_MortErrMes (&s_SIF,cr_ErrMes,&iN_Err,fh_Err)) /* Chk error messe & disp */
       goto NextRec;

     Chk_SIF (&s_SIF, &s_MI, cr_ErrMes);     /* Check input values           */
     if ( !_MortErrMes (&s_SIF,cr_ErrMes,&iN_Err,fh_Err)) /* Chk error messe & disp */
       goto NextRec;

     MRT_Calc (&s_MI, &s_MO,cr_ErrMes);       /* Calc & accum, Mort,Basal,etc */
     if ( !_MortErrMes (&s_SIF,cr_ErrMes,&iN_Err,fh_Err)) /* Chk error messe & disp */
       goto NextRec;

NextRec:
     i = SIF_Next (&s_SIF,cr_ErrMes);        /* Get next input Record        */
     iN_Rec++;
     if ( i == 0 ) {
       strcpy (s_SIF.cr_StdId,e_EOF);        /* Hit end of file              */
       continue; }
     if ( i == -1 ) {                        /* Error in input file, miss etc*/
       _MortBadFile (iN_Rec,fh_Err);
       iN_Err++;
       goto Close;}
     if ( !_MortErrMes (&s_SIF,cr_ErrMes,&iN_Err,fh_Err)) /* Chk for error message and disp */
         goto NextRec;
   }  /* while */

Close:
  SIF_Close ();                              /* Close input and output recs  */
  BOF_Close ();

  *aiN_Std = iN_Std;                         /* Number of Stands Processed   */
  return iN_Err;
}



/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _xError_Window
* Desc:
*   In:
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void _xError_Window  (char A[], char B[], FILE *fh_Err)
{
char cr[3000]; 
   sprintf ( cr, "%s\n%s",A,B);
   if ( fh_Err == NULL )
     myMessageBox (cr); //  Error_Window (A,B);
   else
     fprintf (fh_Err,"%s\n",A);
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _MortChkMiss
* Desc:
*
*   In:
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int _MortChkMiss (d_SIF *a_SIF, char cr_ErrMes[])
{

  strcpy (cr_ErrMes,"");

  if ( !strcmp (a_SIF->cr_StdId,"")) {
    strcpy (cr_ErrMes,"Missing Stand Id");
    return 0; }
  if ( !strcmp (a_SIF->cr_Spe,"")) {
    strcpy (cr_ErrMes,"Missing Tree Species Code");
    return 0; }
  if ( !strcmp (a_SIF->cr_FS,"")) {
    strcpy (cr_ErrMes,"Missing Scorch/Flame Code");
    return 0; }

  if ( !_ChkCM (a_SIF->f_Den, "Density",       cr_ErrMes)) return 0;
  if ( !_ChkCM (a_SIF->f_DBH, "DBH",           cr_ErrMes)) return 0;
  if ( !_ChkCM (a_SIF->f_Hei, "Tree Height",   cr_ErrMes)) return 0;
  if ( !_ChkCM (a_SIF->f_CroRat, "Crown Ratio",cr_ErrMes)) return 0;
  if ( !_ChkCM (a_SIF->f_Sch, "Scorch-Height Flame-Length ",cr_ErrMes)) return 0;

  return 1;

}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _MortErrMes
* Desc: Check for and error message in the string, if so print it out
*        and count the error
*   In: see below
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int _MortErrMes (d_SIF *a_SIF, char cr_ErrMes[], int *aiN_Err, FILE *fh_Err)
{
  if ( !strcmp (cr_ErrMes,"" ))
    return 1;

  if ( fh_Err == NULL )
    printf  ("ERROR - Stand: %s Species: %s \n %s, Record will not be processed\n", a_SIF->cr_StdId,  a_SIF->cr_Spe, cr_ErrMes );
  else
    fprintf  (fh_Err, "ERROR - Stand: %s Species: %s \n %s, Record will not be processed\n", a_SIF->cr_StdId,  a_SIF->cr_Spe, cr_ErrMes );

  *aiN_Err = *aiN_Err + 1;
  return 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _MortBadFile
* Desc: Message used when the input file is detected to be not
*        properly formated
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  _MortBadFile (int iN_Rec, FILE *fh_Err)
{

#define e_BadMFil "The input file does Not appear to be properly formated"
  sprintf (gcr_tmp, "%s\nApproximate record/line number in error: %d",e_BadMFil,iN_Rec);
  _xError_Window (gcr_tmp, "Input File Error",fh_Err);
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Chk_SIF
* Desc: Check the inputs from the Mortality Species Input File and
*       Copy them into the Mortality Calc Input Struct MI
*  Out: cr.....error messages or ""
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int Chk_SIF (d_SIF *a_SIF, d_MI *a_MI, char cr[])
{

  strcpy (cr,"");

  if ( a_SIF->f_Den < 1 || a_SIF->f_Den > 10000 ) {
    sprintf (cr,"Density:%5.0f is out of Range ( 1 -> 10000 )",a_SIF->f_Den);
    return 0;  }

  if ( a_SIF->f_DBH < 1 || a_SIF->f_DBH > 999 ) {
    sprintf (cr,"DBH:%5.0f is out of Range ( 1 -> 999 )",a_SIF->f_DBH);
    return 0;  }

  if ( a_SIF->f_Hei < 1 || a_SIF->f_Hei > 300 ) {
    sprintf (cr,"Tree Height:%5.0f is out of Range ( 1 -> 300 )",a_SIF->f_Hei);
    return 0;  }

  if ( a_SIF->f_CroRat < 1 || a_SIF->f_CroRat > 10 ) {
    sprintf (cr,"Crown Ratio: %2.0f is out of Range ( 1 -> 10 )",a_SIF->f_CroRat);
    return 0;  }


/* Flame Length - Scorch Height .............................................*/
/* First check, as case insens, then set to proper case code                 */
  if ( !stricmp (a_SIF->cr_FS,e_Scorch)){
    strcpy (a_SIF->cr_FS,e_Scorch);
    if (a_SIF->f_Sch < e_ScoLow || a_SIF->f_Sch > e_ScoHig ){
       sprintf (cr,"Scorch Height %4.2f is out of %4.0f -> %4.0f range ",a_SIF->f_Sch, e_ScoLow,e_ScoHig);
       return 0; }}
  else if ( !stricmp (a_SIF->cr_FS,e_Flame)){
    strcpy (a_SIF->cr_FS,e_Flame);
    if (a_SIF->f_Sch < e_FlaLow || a_SIF->f_Sch > e_FlaHig ){
       sprintf (cr,"Flame Length %4.2f is out of %4.0f -> %4.0f range ", a_SIF->f_Sch, e_FlaLow,e_FlaHig);
       return 0; }}
  else {
      sprintf (cr,"Invalid Flame/Scorch Code (%s) Valid Codes -> %s or %s",a_SIF->cr_FS,e_Flame,e_Scorch);
      return 0; }

/* Fire Severity............                                                 */
  if ( stricmp (a_SIF->cr_FirSev,e_Low) && stricmp (a_SIF->cr_FirSev,"")) {
    sprintf (cr,"Invalid Fire Severity Code (%s), Use  %s or blank (\"\")",a_SIF->cr_FirSev, e_Low);
    return 0;  }

/*...........................................................................*/
/* Deal with PINPAL Coastal Plain - IF we have it                            */

  a_MI->f_RelHum = e_RHNonen;          /* assume we won't have any          */

  if (!strcmp (a_MI->cr_CP,"N"))       /* Don't have to deal with it        */
      goto CPY;

  if ( a_SIF->f_RelHum == -1 ) {        /* No Relative Humid in input file   */
    goto CPY; }

  if ( a_SIF->f_RelHum < e_RHLow || a_SIF->f_RelHum > e_RHHigh ) {
    sprintf (cr,"Invalid Relative Humidity, use %3.0f ->  %3.0f, or leave blank", e_RHLow, e_RHHigh);
    return 0; }

  a_MI->f_RelHum =a_SIF->f_RelHum;


/* Copy fields the Mortality Input Struct..................                  */
CPY:
  strcpy (a_MI->cr_Spe, a_SIF->cr_Spe);
  strcpy (a_MI->cr_FS,a_SIF->cr_FS);
  strcpy (a_MI->cr_FirSev, a_SIF->cr_FirSev);

  a_MI->f_Den = a_SIF->f_Den;
  a_MI->f_DBH = a_SIF->f_DBH;
  a_MI->f_Hgt = a_SIF->f_Hei;
  a_MI->f_FS  = a_SIF->f_Sch;
  a_MI->f_CroRat = a_SIF->f_CroRat;
  return 1;

}


#ifdef wowowow

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name:
* Desc:
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int Show_Soil (d_SO *a_SO)
{
int i;
   printf ("----------------------------------------------------------------\n");
   printf (" Duff Depth.....: Pre-Fire: %6.2f cm.,  Post-Fire: %6.2f cm.\n\n",
            a_SO->f_cDufPre,a_SO->f_cDufPost);

   printf ("                               Soil Layer Maximum Temperature\n");
   printf ("                         ( measurements are in centimeters and Celsius )\n\n");
   printf ("Depth     0    1    2    3    4    5    6    7    8    9   10   11   12   13\n");

   printf ("Temp.  ");
   for (i = 0; i < e_Layers; i++ )
     printf (" %3d ", a_SO->ir_Temp[i]);
   printf ("\n");
   printf ("Time   ");
   for (i = 0; i < e_Layers; i++ )
     printf (" %3d ", a_SO->ir_Time[i]);
   printf ("\n");

   if ( a_SO->i_LayMaxDeg1 == e_LayNone )
     printf ("Max Depth Having %d degrees: - None - \n", e_Max1);
   else
     printf ("Max Depth Having %d degrees: %d\n", e_Max1, a_SO->i_LayMaxDeg1);

   if ( a_SO->i_LayMaxDeg2 == e_LayNone )
     printf ("Max Depth Having %d degrees: - None - \n", e_Max2);
   else
     printf ("Max Depth Having %d degrees: %d\n", e_Max2, a_SO->i_LayMaxDeg2);

   printf ("Model: %s \n", a_SO->cr_Model);

   return 1;
}
#endif

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _ChkLim1000
* Desc: Need to check the 1000 hour load limit when it comes in as a total
*        load. Want to do this before it gets split into size classes.
*        If the 1000 hour does come in from batch file as size classes
*         already, then it gets limited checked later in consmed calc mngr.
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int _ChkLim1000 (d_CI *a_CI, char cr_ErrMes[], char cr_1kFmt[])
{

  if ( !strcmp (cr_1kFmt, e_IF_1kSC))   /* load came in as size classes      */
    return 1;                           /* Don't care about chk now          */

  if ( a_CI->f_DW1000  < e_CI_LoadLimLow || a_CI->f_DW1000 > e_CI_LoadLimUp ) {
     sprintf (cr_ErrMes,"Down Wood 1000 Hour load (%6.2f) is out limits ( %4.0f -> %4.0f)",
              a_CI->f_DW1000 , e_CI_LoadLimLow,e_CI_LoadLimUp);
     return 0;   }
  return 1;
}



/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _ChkConMissing
* Desc: Check for missing values coming in, limits and invalid format floats
*        get checked else where
*       JUST check for missing values here, make sure whatever we need at
*        least has something there
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int _ChkConMiss (d_CI *a_CI, char cr_ErrMes[], char cr_1kFmt[])
{

  strcpy (cr_ErrMes,"");

  if ( !_ChkCM (a_CI->f_Lit,  "Litter",            cr_ErrMes)) return 0;
  if ( !_ChkCM (a_CI->f_DW1,  "Down Wood 1 hour",  cr_ErrMes)) return 0;
  if ( !_ChkCM (a_CI->f_DW10, "Down Wood 10 hour", cr_ErrMes)) return 0;
  if ( !_ChkCM (a_CI->f_MoistDW10,"Down Wood 10 hour Moisture",cr_ErrMes)) return 0;
  if ( !_ChkCM (a_CI->f_DW100,"Down Wood 100 hour",cr_ErrMes)) return 0;

  if ( !_ChkCM (a_CI->f_CroFol,   "Crown Foliage",             cr_ErrMes)) return 0;
  if ( !_ChkCM (a_CI->f_CroBra,   "Crown Branch",              cr_ErrMes)) return 0;
  if ( !_ChkCM (a_CI->f_Pc_CroBrn,"Crown Branch Percent Burn", cr_ErrMes)) return 0;
  if ( !_ChkCM (a_CI->f_Herb,     "Herbaceous",                cr_ErrMes)) return 0;
  if ( !_ChkCM (a_CI->f_Shrub,    "Shrub",                     cr_ErrMes)) return 0;

  if ( !_ChkCM (a_CI->f_MoistDW1000, "Down Wood 1000 Hour Moisture", cr_ErrMes)) return 0;

  if ( !_ChkCM (a_CI->f_Duff,     "Duff Load",  cr_ErrMes)) return 0;
  if ( !_ChkCM (a_CI->f_DufDep,   "Duff Depth", cr_ErrMes)) return 0;
  if ( !_ChkCM (a_CI->f_MoistDuff,"Duff Moisture", cr_ErrMes)) return 0;


  if ( !strcmp (cr_1kFmt, e_IF_1kSC)){
    if ( !_ChkCM (a_CI->f_Snd_DW3,  "Down Wood  3 inch Sound", cr_ErrMes)) return 0;
    if ( !_ChkCM (a_CI->f_Snd_DW6,  "Down Wood  6 inch Sound", cr_ErrMes)) return 0;
    if ( !_ChkCM (a_CI->f_Snd_DW9,  "Down Wood  9 inch Sound", cr_ErrMes)) return 0;
    if ( !_ChkCM (a_CI->f_Snd_DW20, "Down Wood 20 inch Sound", cr_ErrMes)) return 0;

    if ( !_ChkCM (a_CI->f_Rot_DW3,  "Down Wood  3 inch Rotten", cr_ErrMes)) return 0;
    if ( !_ChkCM (a_CI->f_Rot_DW6,  "Down Wood  6 inch Rotten", cr_ErrMes)) return 0;
    if ( !_ChkCM (a_CI->f_Rot_DW9,  "Down Wood  9 inch Rotten", cr_ErrMes)) return 0;
    if ( !_ChkCM (a_CI->f_Rot_DW20, "Down Wood 20 inch Rotten", cr_ErrMes)) return 0;  }
  else {
    if ( !strcmp (a_CI->cr_WD,"") ) {
      strcpy (cr_ErrMes, "Missing 1000 Hour Weight Distribution Code");
      return 0; }
    if ( !_ChkCM (a_CI->f_DW1000, "Down Wood 1000 Hour", cr_ErrMes)) return 0;
    if ( !_ChkCM (a_CI->f_pcRot,  "Down Wood 1000 Hour Rotten Percent", cr_ErrMes)) return 0;
  }

  return 1;

}

int _ChkCM (float f, char cr[], char cr_ErrMes[])
{
  if ( f == e_Flo_Init ) {
    sprintf (cr_ErrMes,"%s value is missing in input file.",cr);
    return 0; }
  return 1;
}





/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _ChkSoil
* Desc: Check the soil type, we check is as case-insensitive and then
*        assign the official define which has the proper case for later
*        on in the program when it gets used.
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int _ChkSoil (d_SI *a_SI, char cr_ErrMes[])
{
  if ( !strcmp (a_SI->cr_SoilType, "" )) {
     sprintf (cr_ErrMes,"Missing Soil Type, Use %s for None Available, or a valid code.",e_NoSoilType);
     return 0; }

  if      ( !stricmp (a_SI->cr_SoilType, e_LoaSke))     strcpy (a_SI->cr_SoilType, e_LoaSke);
  else if ( !stricmp (a_SI->cr_SoilType, e_FinSil))     strcpy (a_SI->cr_SoilType, e_FinSil);
  else if ( !stricmp (a_SI->cr_SoilType, e_Fin   ))     strcpy (a_SI->cr_SoilType, e_Fin   );
  else if ( !stricmp (a_SI->cr_SoilType, e_CoaSil))     strcpy (a_SI->cr_SoilType, e_CoaSil);
  else if ( !stricmp (a_SI->cr_SoilType, e_CoaLoa))     strcpy (a_SI->cr_SoilType, e_CoaLoa);
  else if ( !stricmp (a_SI->cr_SoilType, e_NoSoilType)) strcpy (a_SI->cr_SoilType, e_NoSoilType);
  else {
     sprintf (cr_ErrMes,"Invalid Soil Type: %s (Valid: %s, %s, %s, %s, %s, or %s)",
     a_SI->cr_SoilType, e_LoaSke,e_FinSil,e_Fin,e_CoaSil,e_CoaLoa,e_NoSoilType);
     return 0; }

  return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _ChkMoistCond
* Desc: Check for Missing Soil Moisture and Moisture Condition
*       Moisture Condition can come in as any case(upper/lower) and be sent
*        to soil model code and be ok
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int _ChkMoistCond (d_SI *a_SI, char cr_ErrMes[])
{
  if ( a_SI->f_SoilMoist == e_Flo_Init ) {
    strcpy (cr_ErrMes,"Missing Soil Moisture.");
    return 0; }

  if ( !stricmp (a_SI->cr_MoistCond,e_VeryDry )) goto OK;
  if ( !stricmp (a_SI->cr_MoistCond,e_Dry     )) goto OK;
  if ( !stricmp (a_SI->cr_MoistCond,e_Moderate)) goto OK;
  if ( !stricmp (a_SI->cr_MoistCond,e_Wet     )) goto OK;
  sprintf (cr_ErrMes,"Invalid/Missing Moisture Condition: %s (Valid: %s, %s, %s, %s)",
     a_SI->cr_MoistCond, e_VeryDry,e_Dry,e_Moderate,e_Wet);
  return 0;
OK:
  return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _AskUser
* Desc: I give the user a warning if there is an error on the first
*        record of the input file.
*       This is a way to try and catch the user if they select a weird
*        input file by mistake.
*       An error on the first line could just be some invalid data in a
*        field of a valid file, so the user could/may want to continue.
*        But and error on the first line could mean the file is completely
*        of the wrong kind.
*  Ret:
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int _AskUser ()
{
int i;

#define e_AUM "An error has occured on the first record of the input file.\n" \
   "Please check the following: \n" \
   "  - Are you using a valid input file ? \n" \
   "  - Is the input file properly formated ? \n" \
   "  - Is the input file format appropriate for\n" \
   "    the Model Type button you have selected ? \n" \
   "  \n" 

//   " Do you want to continue processing the file ? \n"

//   i = MessageBox (ghDlgBat, e_AUM, "FOFEM - Batch - Input File?", MB_YESNO);

 //  if ( i == IDYES )
 //    return 1;
// FIX 
   ErrBat_Window ("Error", e_AUM); 
   return 0;

}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: BIF_CovGrp
* Desc: Check and Set Cover Group codes.
*       This will check for either type Cover Group codes, the short or
*       long codes, ex:  "GG" or "GrassGroup", short codes are the same
*       codes that are used in the fof_saf.dat, fof_nvcs.dat, fof_fcc.dat
*       files, the long codes are the codes used internally in the program
*       when determining forumlas.
*       If a short code is read in from the input file we catch it here
*       and convert it to the long code. I wanted to allow both codes
*       to get used.
* NOTE: Alot of other input codes get checked later in code, even the
*        Cover Groups, I left other Cover Group checks in place, not sure
*         but it might be needed else where.
*        I had to insert this check here because the short codes are in
*        the fof_cvt.h file which doesn't go with the ANSI package.
*        So we just catch and convert this right up front.
*   In:  a_CI....input data
*  Out:  cr_ErrMes
*  Ret: 1 ok, 0 error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int Bat_CovGrp (d_CI *a_CI, char cr_ErrMes[])
{

  strcpy (cr_ErrMes,"");
  if ( !stricmp (a_CI->cr_CoverGroup,"" )) return 1;

  if ( !stricmp (a_CI->cr_CoverGroup,e_ShrubGroup )) return 1;
  if ( !stricmp (a_CI->cr_CoverGroup,e_SageBrush  )) return 1;
  if ( !stricmp (a_CI->cr_CoverGroup,e_GrassGroup )) return 1;
  if ( !stricmp (a_CI->cr_CoverGroup,e_BalBRWSpr  )) return 1;
  if ( !stricmp (a_CI->cr_CoverGroup,e_Ponderosa  )) return 1;
  if ( !stricmp (a_CI->cr_CoverGroup,e_Pocosin    )) return 1;
  if ( !stricmp (a_CI->cr_CoverGroup,e_WhiPinHem  )) return 1;
  if ( !stricmp (a_CI->cr_CoverGroup,e_RedJacPin  )) return 1;
  if ( !stricmp (a_CI->cr_CoverGroup,e_CoastPlain )) return 1;


  if ( !stricmp (a_CI->cr_CoverGroup, e_CVT_ShrubGroup )) {
       strcpy (a_CI->cr_CoverGroup, e_ShrubGroup);
       return 1; }

  if ( !stricmp (a_CI->cr_CoverGroup, e_CVT_SageBrush  )) {
       strcpy (a_CI->cr_CoverGroup, e_SageBrush);
       return 1; }

  if ( !stricmp (a_CI->cr_CoverGroup, e_CVT_GrassGroup )) {
       strcpy (a_CI->cr_CoverGroup, e_GrassGroup);
       return 1; }

  if ( !stricmp (a_CI->cr_CoverGroup, e_CVT_BalBRWSpr  )) {
       strcpy (a_CI->cr_CoverGroup, e_BalBRWSpr);
       return 1; }

  if ( !stricmp (a_CI->cr_CoverGroup, e_CVT_Ponderosa  )) {
       strcpy (a_CI->cr_CoverGroup, e_Ponderosa);
       return 1; }

  if ( !stricmp (a_CI->cr_CoverGroup, e_CVT_Pocosin  )) {
       strcpy (a_CI->cr_CoverGroup, e_Pocosin);
       return 1; }

  if ( !stricmp (a_CI->cr_CoverGroup, e_CVT_WhiPinHem  )) {
       strcpy (a_CI->cr_CoverGroup, e_WhiPinHem);
       return 1; }

  if ( !stricmp (a_CI->cr_CoverGroup, e_CVT_RedJacPin  )) {
       strcpy (a_CI->cr_CoverGroup, e_RedJacPin);
       return 1; }

  if ( !stricmp (a_CI->cr_CoverGroup, e_CVT_CoastPlain )) {
       strcpy (a_CI->cr_CoverGroup, e_CoastPlain);
       return 1; }

  sprintf (cr_ErrMes, "Invalid Cover Group Code --> \"%s\"\n", a_CI->cr_CoverGroup);
  strcat (cr_ErrMes, " See main menu Help 'Batch Processing' for list of valid codes.");
  return 0;
}




/************************************************************************
*
* ----------------------------------------------------------------
* Sample file
*  # maximum number iterations burnup does, default 3000
*  #MAX_TIMES 3000
*  #  intensity of the igniting surface fire,
*  #  kW/m2 sq m, 40.0 -> 1.0e5, burnup var - fi  (float)
*  #INTENSITY 50.0
*  #  residence time of the ignition surface fire, seconds
*  #  default = 60.0, fofem's burnup input file uses 30.0
*  #  burnup var = ti,   limits 10.0 -> 200.0
*  #IG_TIME 60
*  # windspeed at top of fuelbed meters/second
*  # burnup var = u, default 0, limits 0.0 -> 5.0,
*  #WINDSPEED 3.0
*  # fuel depth, meters,
*  # burnup var = d, defualt 0.3, limits 0.1 -> 5.0
*  #DEPTH  0.3
*  #  ambient air temperature, degrees Celcus
*  #  burnup var = tamb, default 27,
*  # if ( tamb-273 < tam1 || tamb-273 > tam2)
*  # const double tam1 = -40.0,  tam2 = 40.0;
*  #AMBIENT_TEMP  27.0
*  # fire environment minumum dimension paramter
*  # default 1.83
*  #R0  1.83
*  # fire environment increment temp parater
*  # default 0.40
*  #DR  0.4
*  # time step for integration of burning rates.
*  # TIMESTEP * MAX_TIMES gives max simulation period
*  #TIMESTEP 10
*
*  # Sigma - Surface area to volume ratio
*  #SURat_Lit         8200.0
*  #SURat_DW1         1480.0
*  #SURat_DW10         394.0
*  #SURat_DW100        105.0
*  #SURat_DWk_3_6       39.4
*  #SURat_DWk_6_9       21.9
*  #SURat_DWk_9_20      12.7
*  #SURat_DWk_20        5.91
*
*
************************************************************************/
int  Get_BrnParFil (char cr_BrnParFN[], d_CI *a_CI, char cr_ErrMes[])
{
float f;
FILE *fh;

  strcpy (cr_ErrMes,"");
  fh = fopen (cr_BrnParFN, "r");
  if ( fh == NULL ) {
    sprintf (cr_ErrMes, "Error - Can't open Burnup Parameter file...\n%s\n",cr_BrnParFN);
    return 0; }

  if ( !_FindSw (fh, &f, e_swpMAX_TIMES,    cr_ErrMes)) return 0;
  a_CI->l_MAX_TIMES = (long) f;

  if (! _FindSw (fh, &a_CI->f_INTENSITY,    e_swpINTENSITY,         cr_ErrMes)) return 0;
  if (! _FindSw (fh, &a_CI->f_IG_TIME,      e_swpIG_TIME,           cr_ErrMes)) return 0;
  if (! _FindSw (fh, &a_CI->f_WINDSPEED,    e_swpWINDSPEED,         cr_ErrMes)) return 0;
  if (! _FindSw (fh, &a_CI->f_DEPTH,        e_swpDEPTH,             cr_ErrMes)) return 0;
  if (! _FindSw (fh, &a_CI->f_AMBIENT_TEMP, e_swpAMBIENT_TEMP,      cr_ErrMes)) return 0;
  if (! _FindSw (fh, &a_CI->f_R0,           e_swpR0,                cr_ErrMes)) return 0;
  if (! _FindSw (fh, &a_CI->f_DR,           e_swpDR,                cr_ErrMes)) return 0;
  if (! _FindSw (fh, &a_CI->f_TIMESTEP,     e_swpTIMESTEP,          cr_ErrMes)) return 0;

  if (! _FindSw (fh, &a_CI->f_SURat_Lit     , "#SURat_Lit",       cr_ErrMes)) return 0;
  if (! _FindSw (fh, &a_CI->f_SURat_DW1     , "#SURat_DW1",       cr_ErrMes)) return 0;
  if (! _FindSw (fh, &a_CI->f_SURat_DW10    , "#SURat_DW10",      cr_ErrMes)) return 0;
  if (! _FindSw (fh, &a_CI->f_SURat_DW100   , "#SURat_DW100",     cr_ErrMes)) return 0;
  if (! _FindSw (fh, &a_CI->f_SURat_DWk_3_6 , "#SURat_DWk_3_6",   cr_ErrMes)) return 0;
  if (! _FindSw (fh, &a_CI->f_SURat_DWk_6_9 , "#SURat_DWk_6_9",   cr_ErrMes)) return 0;
  if (! _FindSw (fh, &a_CI->f_SURat_DWk_9_20, "#SURat_DWk_9_20",  cr_ErrMes)) return 0;
  if (! _FindSw (fh, &a_CI->f_SURat_DWk_20  , "#SURat_DWk_20",    cr_ErrMes)) return 0;

  fclose (fh);
  return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _FindSw
* Desc: Look for a swithc in the Burnup Parameter File
*   In: cr_Switch.....switch to look for
*  Out:   *af...........switch argument
*  Ret: 1 Ok,    0 Not Found or bad argument
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int _FindSw (FILE *fh, float *af, char cr_Switch[], char cr_ErrMes[])
{
char  *a, cr[300], cr1[300], cr2[300];
char c;

   strcpy (cr_ErrMes,"");

   fseek (fh,0L,SEEK_SET);
   while (1) {
      a = fgets ( cr, 200, fh );         /* Read a line from file        */
      if ( a == NULL )                       /* End of File                  */
         break;
      if ( !strcmp (cr,"") )                 /* Empty Line                   */
         continue;

      strcpy (cr1,"");                /* Need to do this or any       */
      strcpy (cr2,"");                /* blank args get junk in them  */

      sscanf (cr, "%s %s", cr1, cr2);

      if ( stricmp (cr1,cr_Switch) )
        continue;


//    MessageBox (NULL,  cr1, cr2, MB_YESNO);

      c = Get_NumTyp (cr2);
      if ( c == 'X' ) {
        sprintf (cr_ErrMes, "Burnup Parameter file has bad switch argument: %s %s\n", cr1, cr2);
        fclose (fh);
        return 0; }

      *af = atof(cr2);

      return 1;
   } /* while */


   sprintf (cr_ErrMes, "Burnup Parameter file, switch not found %s\n",cr_Switch);

   fclose (fh);
   return 0;
}




/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Bat_MakSamCES
* Desc: Make a sample input batch file for Consumed Emissions or Soil
*       The soil is the same as the Consumed Emissions with a few addtional
*        fields added to the end.
*   In: cr_FN.......Path and File Name
*       iS_Soil.....1 = do the Soil File
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  Bat_MakSamCES (char cr_FN[], int iS_Soil)
{
FILE *fh;

  fh = fopen (cr_FN,"w");
  if ( fh == NULL )
     return 0;

    fprintf (fh,"# \n");
    fprintf (fh,"#    This is a sample input file for the FOFEM6 Batch Program\n");
    if ( iS_Soil != 1 ) 
      fprintf (fh,"#    Consumed - Emissions\n");
    else 
      fprintf (fh,"#  Soil\n");

 //  originally -->   fprintf (fh,"#    Consumed - Emissions - Soil\n");


    fprintf (fh,"# \n");
    fprintf (fh,"# Lines starting with '#' are comment lines or switches (see 1000 hour notes below)\n");
    fprintf (fh,"# See program help section for more details.\n");
    fprintf (fh,"# \n");
    fprintf (fh,"# \n");

    fprintf (fh,"# Distr. - 1000 hour fuel distribution into classes\n");
    fprintf (fh,"#  \"%s\", \"%s\", \"%s\", \"%s\", \"%s\" \n", e_Right,e_Left,e_Even,e_EndWD,e_Center);
    fprintf (fh,"# \n");

    fprintf (fh,"# \n");
    fprintf (fh,"# DufMet - (duff moisture method) - default/blank \"\" for Entire \n");
    fprintf (fh,"#  \"%s\", \"%s\", \"%s\" \n", e_Entire, e_Lower, e_NFDR);
    fprintf (fh,"# \n");

    fprintf (fh,"# Region:\n");
    fprintf (fh,"# \"%s\", \"%s\", \"%s\", \"%s\" \n", e_CI_SouthEast,e_CI_InteriorWest,e_CI_PacificWest, e_CI_NorthEast);
    fprintf (fh,"# \n");

    fprintf (fh,"# Cover Group:\n");
    fprintf (fh,"# \"%s\", \"%s\", \"%s\", \"%s\" \n",e_GrassGroup,e_SageBrush,e_ShrubGroup,e_Pocosin);
    fprintf (fh,"# \"%s\", \"%s\", \"%s\", \"%s\" \n",e_Ponderosa, e_WhiPinHem,e_RedJacPin, e_BalBRWSpr);
    fprintf (fh,"#   \"\" = no cover group\n");
    fprintf (fh,"# \n");
    fprintf (fh,"#  The following cover group codes from the input cover fuel files (.dat)\n");
    fprintf (fh,"#  can also be used. \n");
    fprintf (fh,"# \"%s\", \"%s\", \"%s\", \"%s\" \n",e_CVT_GrassGroup,e_CVT_SageBrush,e_CVT_ShrubGroup,e_CVT_Pocosin);
    fprintf (fh,"# \"%s\", \"%s\", \"%s\", \"%s\" \n",e_CVT_Ponderosa, e_CVT_WhiPinHem,e_CVT_RedJacPin, e_CVT_BalBRWSpr);
    fprintf (fh,"# \n");

#ifdef TAKEOUT
    if ( iS_Soil != 1 ) {
      fprintf (fh,"# Longleaf Pine Coastal Plain Cover Group\n");
      fprintf (fh,"# \"%s\", \"%s\",\n", e_CoastPlain, e_CVT_CoastPlain);
      fprintf (fh,"#   Please see %s instructions below \n",e_IF_CPLP );
      fprintf (fh,"# \n"); }
#endif 

    fprintf (fh,"# Season:\n");
    fprintf (fh,"# \"%s\", \"%s\", \"%s\", \"%s\" \n", e_Winter,e_Spring,e_Summer,e_Fall);
    fprintf (fh,"# \n");

    fprintf (fh,"# Fuel Category:\n");
    fprintf (fh,"# \"%s\", \"%s\"\n", e_Natural, e_Slash);
    fprintf (fh,"# \n");

  if ( iS_Soil == 1 ) {
    fprintf (fh,"# Soil Mositure Condtions:\n");
    fprintf (fh,"#  \"%s\", \"%s\", \"%s\", \"%s\" \n", e_VeryDry, e_Dry, e_Moderate,e_Wet);
    fprintf (fh,"# \n");
    fprintf (fh,"# Soil Types:\n");
    fprintf (fh,"#  \"%s\", \"%s\", \"%s\", \"%s\" , \"%s\" \n",e_LoaSke, e_FinSil, e_Fin, e_CoaSil, e_CoaLoa);
    fprintf (fh,"# \n");  }

    fprintf (fh,"# \n");
    fprintf (fh,"# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  \n");
    fprintf (fh,"# This switch (optional) causes an emission file to be created for each plot\n");
    fprintf (fh,"# You must supply an argument ('Wow') after the switch which will be used as a prefix to\n");
    fprintf (fh,"# form output file names.\n");
    fprintf (fh,"# The emission files will go into the same folder as the other output files\n"); 
    fprintf (fh,"# %s Wow \n",  e_IF_EmisFile);
    fprintf (fh,"# \n\n");
    fprintf (fh,"# This switch (optional) specifies the name of a burnup paramter file.\n");
    fprintf (fh,"# Specify just the burnup file name (no file path) and place the file in the same\n"); 
    fprintf (fh,"# folder as the stand input file (this file)\n");  
    fprintf (fh,"# %s Burnup.txt\n", e_IF_BurnUpFN);
    fprintf (fh,"# \n");

#ifdef THAKEOUT
    fprintf (fh,"# Emission Factor Group Number Switches\n"); 
    fprintf (fh,"# These switches are optional, default values will be used if not found.\n"); 
    fprintf (fh,"# Refer to the FOFEM GUI for valid Group Numbers\n"); 
    fprintf (fh,"# %s %s\n", e_IF_Flame,  e_DefFlaGrp );  
    fprintf (fh,"# %s %s\n", e_IF_Smold,  e_DefSmoGrp ); 
    fprintf (fh,"# %s %s\n", e_IF_Duff,   e_DefDufGrp ); 
    fprintf (fh,"# \n");
#endif 

    fprintf (fh,"# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  \n");
    fprintf (fh,"# 1000 hour fuel loads can be enter in total or in size classes.\n");
    fprintf (fh,"# The stand example data in this file is setup as 1000 hour total.\n");
    fprintf (fh,"# To do size classes: \n");
    fprintf (fh,"# NOTE -> You MUST enter the following switch '%s' staring in column 1 at the top of the file.\n",  e_IF_1kSC );
    fprintf (fh,"# Remove the 4 columns of 1000 hour data in main example and insert \n");
    fprintf (fh,"#  the following 9 columns of data. \n");
    fprintf (fh,"# \n");
    fprintf (fh,"#  .......Sound......   ......Rotten......    1000Hr\n");
    fprintf (fh,"#   3    6    9    20    3    6    9    20    Moist\n");
    fprintf (fh,"#  0.5, 1.2, 2.3, 3.5,  0.2, 0.8, 1.1, 1.4,    50,\n");
    fprintf (fh,"# \n");
    fprintf (fh,"#  Size classes defined in inches: 3->5.99, 6->8.99, 9->19.99, 20-> \n");
    fprintf (fh,"# \n");


#ifdef TAKEOUT
    if ( iS_Soil != 1 ) {
       fprintf (fh,"# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  \n");
       fprintf (fh,"# Southeast Longleaf Pine Coastal Plain cover types\n");
       fprintf (fh,"#  (identified by Cover Group code - see above) \n");
       fprintf (fh,"# Enter the following switch '%s' staring in column 1 at the top of the file.\n", e_IF_CPLP   );
       fprintf (fh,"# The switch identifies the file as having a standard format (see Sample Data below) \n");
       fprintf (fh,"# with the inclusion of a litter moisture column immediately following the \n");
       fprintf (fh,"# the litter load column. \n");
       fprintf (fh,"# \n"); }
#endif 


    fprintf (fh,"# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  \n");
    fprintf (fh,"# \n");
    fprintf (fh,"#      Sample Data......\n");
    fprintf (fh,"# \n");

  if ( iS_Soil == 1 ) {
    fprintf (fh,"# Stand         1   ...10... 100  ......1000 Hr.......      ....Duff......                 ...Crown......                   Cover                   Fuel       Soil              Soil   Moist\n");
    fprintf (fh,"#  Id      Lit  Hr  Hr Moist  Hr  Load Moist  %cRot Distr.    Load Moist Dep Method Herb Shrub  Foli Brnch %cBrn     Region       Group        Season     Category   Type              Moist  Cond\n",'%','%');
    fprintf (fh,"\"stand-1\", .5,  1,   1,  16,   1,   1,   30,   50, \"Even\",     1,    75,  1, \"\",  1,   1,    1,    1,   50, \"PacificWest\", \"GrassGroup\", \"Spring\",  \"Natural\", \"Loamy-Skeletal\",   10,  \"VeryDry\"  \n");
    fprintf (fh,"\"stand-2\",  1,  1,   1,  16,   1,   1,   30,   50, \"Right\",    1,    75,  1, \"\",  1,   1,    1,    1,   50, \"PacificWest\", \"REDJACPIN\",  \"Spring\",  \"Natural\", \"Fine-Silt\",        15,  \"Dry\"      \n");
    fprintf (fh,"\"stand-3\",  1,  1.3, 1,  16,   1,   1,   30,   50, \"Left\",     1,    75,  1, \"\",  1,   1,    1,    1,   50, \"PacificWest\", \"Ponderosa\",  \"Fall\",    \"Natural\",   \"Fine\",             20,  \"Moderate\" \n");
    fprintf (fh,"\"stand-4\",  1,  1,   1,  16,   1,   1,   30,   50, \"End\",      1,    75,  1, \"\",  1,   1,    1,    1,   50, \"PacificWest\", \"REDJACPIN\",  \"Spring\",  \"Natural\", \"Coarse-Silt\",      25,  \"Wet\"      \n");
    fprintf (fh,"\"stand-5\",  1,  1,   1,  16,   1,   1,   30,   50, \"Center\",   1,    75,  1, \"\",  1,   1,    1,    1,   50, \"PacificWest\", \"\",           \"Spring\",  \"Natural\", \"Coarse-Loamy\",     10,  \"Wet\"      \n");   }
  else {
    fprintf (fh,"# Stand         1   ...10... 100  ......1000 Hr.......       ....Duff......                ...Crown......                   Cover                   Fuel\n");
    fprintf (fh,"#  Id      Lit  Hr  Hr Moist  Hr  Load Moist  %cRot Distr.    Load Moist Dep  Herb Shrub  Foli Brnch %cBrn     Region       Group        Season     Category\n",'%','%');
    fprintf (fh,"\"stand-1\", .5,  1,   1,  16,   1,   1,   30,   50, \"Even\",     1,    75,  1, \"\",  1,   1,    1,    1,   50, \"PacificWest\", \"GrassGroup\", \"Spring\",  \"Natural\"\n");
    fprintf (fh,"\"stand-2\",  1,  1,   1,  16,   1,   1,   30,   50, \"Right\",    1,    75,  1, \"\",  1,   1,    1,    1,   50, \"PacificWest\", \"REDJACPIN\",  \"Spring\",  \"Natural\"\n");
    fprintf (fh,"\"stand-3\",  1,  1.3, 1,  16,   1,   1,   30,   50, \"Left\",     1,    75,  1, \"\",  1,   1,    1,    1,   50, \"PacificWest\", \"Ponderosa\",  \"Fall\",    \"Natural\"  \n");
    fprintf (fh,"\"stand-4\",  1,  1,   1,  16,   1,   1,   30,   50, \"End\",      1,    75,  1, \"\",  1,   1,    1,    1,   50, \"PacificWest\", \"REDJACPIN\",  \"Spring\",  \"Natural\"\n");
    fprintf (fh,"\"stand-5\",  1,  1,   1,  16,   1,   1,   30,   50, \"Center\",   1,    75,  1, \"\",  1,   1,    1,    1,   50, \"PacificWest\", \"\",           \"Spring\",  \"Natural\"\n"); }



  fclose (fh);
  return 1;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Bat_MakSamMort
* Desc: Make a sample input batch file for Mortality
*   In: cr_FN.......Path and File Name
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  Bat_MakSamMort (char cr_FN[])
{
FILE *fh;

  fh = fopen (cr_FN,"w");
  if ( fh == NULL )
     return 0;

   fprintf (fh,"# \n");
   fprintf (fh,"#  FOFEM Mortality Batch Input file\n");
   fprintf (fh,"#\n");
   fprintf (fh,"#  Column\n");
   fprintf (fh,"#    1  Stand Id (double quotes optional) \n");
   fprintf (fh,"#    2  Tree Species Code (must be in double quotes) \n");
   fprintf (fh,"#    3  Stand Density (trees per acre)\n");
   fprintf (fh,"#    4  Diameter Breast Height (inches) \n");
   fprintf (fh,"#    5  Tree Height (feet) \n");
   fprintf (fh,"#    6  Crown Ratio (1 to 10)\n");
   fprintf (fh,"#    7  Flame Length or Scorch Height (feet)\n");
   fprintf (fh,"#    8  Specify what col 7 represents \"F\" (flame)\n");
   fprintf (fh,"#        or \"S\" (scorch). If blank \"\", default is Flame \"F\"\n");
   fprintf (fh,"#    9  Fire Severity, \"L\" for low or leave blank \"\"\n");
   fprintf (fh,"#\n");
   fprintf (fh,"# Std         Std         Tree   Crown  Flame   Fl-S  Fire \n");
   fprintf (fh,"# Id   Spe   Density DBH  Height Ratio  Scorch  Code  Sev  \n");
   fprintf (fh,"\n");
   fprintf (fh," 1101, \"ABCO\", 10,   15,  50.0,    5,    6,    \"F\",   \"\"\n");
   fprintf (fh," 1101, \"ABCO\", 10,   30,  50.0,    5,    6,    \"F\",   \"\"\n");
   fprintf (fh," 1101, \"ABCO\", 10,   45,  50.0,    5,    6,    \"F\",   \"\"\n");
   fprintf (fh," 1102, \"ABCO\", 10,   15,  50.0,    5,    4,    \"F\",   \"\"\n");
   fprintf (fh," 1102, \"ABCO\", 10,   30,  50.0,    5,    4,    \"F\",   \"\"\n");
   fprintf (fh," 1102, \"ABCO\", 10,   45,  50.0,    5,    4,    \"F\",   \"\"\n");
   fprintf (fh," 1103, \"ABCO\", 10,   15,  50.0,    5,    4,    \"F\",   \"\"\n");
   fprintf (fh," 1103, \"ABCO\", 10,   30,  50.0,    5,    4,    \"F\",   \"\"\n");
   fprintf (fh," 1103, \"ABCO\", 10,   45,  50.0,    5,    4,    \"F\",   \"\"\n");

  fclose (fh);
  return 1;
}

/*****************************************************************
*  Stub for now
******************************************************************/
void ErrBat_Window (char A[], char B[])
{
char C[2000];
   sprintf (C, "%s\n%s\n",A,B); 
}

/****************************************************************
*
*
***************************************************************/
int _ChkLogDis (char cr[], char cr_ErrMes[]) 
{
  
  if ( stricmp (cr,e_Even   )) return 1;
  if ( stricmp (cr,e_Right  )) return 1;
  if ( stricmp (cr,e_Left   )) return 1;
 // if ( stricmp (cr,e_End    )) return 1;
  if ( stricmp (cr, e_EndWD )) return 1; 
  if ( stricmp (cr,e_Center )) return 1;

  sprintf (cr_ErrMes, "Invalid Log Distribution Code: %s ",cr);
  return 0;

}

/*****************************************************************
* Name 
* Desc: Append cr_FN (file name) to cr_Path[].
*  In: cr_Path...Path and a file name, 
*      cr_FN....file name to append (might have a path on front)
* Out: cr_Out...new path file name
******************************************************************/
void AppendPFN (char cr_Path[], char cr_FN[], char cr_Out[])
{
String ^ Str;
String ^ Path;
String ^ FN; 

  strcpy (cr_Out,""); 

/* Just get the file, strip off any path that might be there*/
  Str = _CharToStr(cr_FN);
  FN = System::IO::Path::GetFileName(Str); 
   
/* Get just the path from here */
  Path = _CharToStr(cr_Path);
  Path = System::IO::Path::GetDirectoryName(Path); 
 
/* Now append */
  if ( Path == "" ) 
     Str = FN ;
  else 
     Str = Path + "\\" + FN ;


 _StrToChar (Str,cr_Out); 
  
}
