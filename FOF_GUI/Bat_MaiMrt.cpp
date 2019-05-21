/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Bat_MaiMrt.cpp    New Mortality Batch - 5/4/2018
* Desc: New Mortality Batch - uses new batch input file that has FFI tree 
*        inputs for doing all three mortality equation types
* Date: 5/4/2018
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
#include "fof_pf2.h" 
#include "fof_smt.h"
#include "fof_duf.h" 
#include "fof_hsf.h"
#include "fof_iss.h" 
#include "fof_bcm.h"
#include "fof_ffi.h"
#include "fof_mis.h"

#include "fof_pst.h" 

#include "fof_ansi.h"


extern d_CDF  s_CDFTree[]; 
extern d_FFItre s_ft; 

d_CDF *a; 
FILE *fh_Tre;
FILE *fh_Out; 

char gcr_ErrMes[3000];
char gcr_temp[3000];

void _mError_Window  (char A[], char B[], FILE *fh_Err);
int Get_TreRec (d_MIS *MIS, char cr_Plot[], char cr_Err[], float f_FS, char cr_FS[]);
int _MortErrMes ( d_MIS *a, char cr_Plot[], char cr_ErrMes[], int *aiN_Err, FILE *fh_Err);
int _FlaSco_SetDef (d_MIS *a, float f_FS, char cr_FS[]);

int MRT_Switch_SC (FILE *fh, int i_SC,  d_PST  *pst, char cr_ErrMes[]);

int _FloToChar (char cr[], char fmt[], float f);
int Bat_Salvage (FILE *fh_Tre, char cr_InFN[], float f_FS, char cr_FS[], int iS_CL, char cr_ErrMes[], char cr_SalvFN[]);

#define e_BadMFile "The input file does Not appear to be properly formated"

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: BAT_MortExec     NEW Mortality Batch  5/4/2018
* Desc: Do the batch mortality.
*       Reads input file, does calcs and puts out one record per stand.
* Note-1: Read the first line in the file which is the column headings
*         we get an error back but don't care, just need to get past 
*         the first line of file. 
*   In: cr_InFN..Input File Name.
*       cr_OutFN..Output File Name
*       iS_CL.... 1 = Put Heading column labels to output file
*  Out: Number of records and stands processed
*       ai_PFI...the number of Post Fire Injury records processed
*       cr_SalvFN...Salvage Report File name if created, else ""
*  Ret: 0 or number of errors.
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  BAT_MortExec (char cr_InFN[], char cr_OutFN[], int *aiN_Std, int iS_CL, FILE *fh_Err, int *ai_PFI,
                   float f_FS, char cr_FS[], char cr_SalvFN[])
{
int i, iS, iN_Err, iN_Std, iN_Rec, iS_PFI; 

float f, f_Prob, fN_Kill, fN_Live, fN_Den, f_CC,f_kill,f_live; 
float  fN_BasTot, fN_BasLiv, fN_BasKil; 
float  fN_CCPre, fN_CCPost, fN_CCDiff;
float  f_CCPre, f_CCPost, f_CCDiff; 
float  fN_AvgDBHKil, fN_AvgKil; 
float  fN_Den4, fN_Kill4; 
char cr_PltA[300], cr_PltB[300]; 
char   cr_PthFN[1000], cr_ErrMes[3000], cr[1000]; 
d_MIS MIS; 
d_MO MO;
String ^Str;
 
  iN_Std = iN_Err = iN_Rec = 0;
  *aiN_Std = 0;
  iS_PFI = 0; 
  *ai_PFI = 0; 

  strcpy (cr_SalvFN,""); /* Salvage Report FN */ 

  Str = System::Windows::Forms::Application::StartupPath;
  Str = Str + "\\" +  e_FOF_Spp ;
 _StrToChar (Str,cr_PthFN);        /* fof_spp.dat file is in same folder as .exe */ 

/* Open the Species Master Equation File */
  MRT_LoadSpe ( cr_PthFN,"", cr_ErrMes);     /* Load Master Specis list file  */
  if ( strcmp (gcr_ErrMes,"") ) {
    fprintf (fh_Err, "ERROR - %s \n",gcr_ErrMes);
    return 1; }

/* Open FFI Tree input file  .tre   */
  fh_Tre = fopen (cr_InFN, "r" );
  if ( fh_Tre == NULL ) {
    fprintf (fh_Err, "Error Opening Input File: %s \n", cr_InFN);
    return 1; }

/* Describes fields in FFI tree input file and */
  a = &s_CDFTree[0]; 

/* Open Output file */ 
  fh_Out = fopen (cr_OutFN, "w" );    
  if ( fh_Out == NULL ) {
    fprintf (fh_Err, "Error Opening Output File: %s \n", cr_OutFN);
    fclose (fh_Tre); 
    return 1; }

  fprintf (fh_Out, "If Flame/Scorch Height is missing in the TRE file FOFEM will use the UI setting shown below:\n"); 
  fprintf (fh_Out, "Flame/Scorch,feet\n"); 
  if ( !stricmp (cr_FS, "S" ) ) 
     fprintf (fh_Out,"Scorch Height,%4.1f\n\n",f_FS); 
  else 
     fprintf (fh_Out,"Flame Length,%4.1f\n\n",f_FS); 
  
  if ( iS_CL == 1 ) {                       /* Heading Col labels to         */
    fprintf (fh_Out, "Plot, Density,Density,Density,BAPre,BAPost,BAKld,CanCov,CanCov,CanCov,DBHKldAvg, MortAvg4, MortAvg\n");
    fprintf (fh_Out, "Id,PrefirePre,Postfire,Killed,sq/ft,sq/ft,sq/ft,Prefire,Postfire,Diff,(inch),percent,percent\n"); }

/* Get The FIRST record (non-comment '#') - Note the first row in the file MUST be the column headings */
/*  if should come back with an error, if not we report an error  */
 i = Get_TreRec (&MIS, cr_PltB, cr, f_FS,cr_FS); 
  if ( !strstr (cr_PltB, e_PlotId) ) {  
    fprintf (fh_Err, "The first row of the input file does not appear to contain the\n");
    fprintf (fh_Err, "reguired column headings, starting with '%s' in the first column \n", e_PlotId);
    iN_Err++;
    goto CloseFile; 
   return 0;  }

/* Get the first data record in the file */
 iS = Get_TreRec (&MIS, cr_PltB, cr_ErrMes,f_FS,cr_FS); /* Prime pump with first input record */   
 strcpy (cr_PltA, cr_PltB); 

 fN_Kill = fN_Live = fN_Den = 0;
 fN_BasTot = fN_BasLiv = fN_BasKil = 0; 
 fN_CCPre = fN_CCPost = fN_CCDiff = 0;
 fN_AvgDBHKil = fN_AvgKil = 0; 
 fN_Den4 = fN_Kill4 = 0; 

/*---------------------------------------------------------*/
  while (1) {

     if ( strcmp (cr_PltA,cr_PltB) ){   /* Plot changed - Do Totals for the Plot*/
       fprintf (fh_Out, "%s,%4.0f,%4.0f,%4.0f,", cr_PltA, fN_Den,fN_Live,fN_Kill); 
       fprintf (fh_Out, "%4.0f,%4.0f,%4.0f,", fN_BasTot,fN_BasLiv,fN_BasKil); 
       f_CCPre = CC_Overlap (fN_CCPre);
       f_CCPost = CC_Overlap (fN_CCPost);
       f_CCDiff = f_CCPre - f_CCPost; 
       fprintf (fh_Out, "%4.0f,%4.0f,%4.0f,", f_CCPre, f_CCPost, f_CCDiff); 

       f = 0; 
       if (  fN_AvgKil > 0 ) 
         f = fN_AvgDBHKil / fN_AvgKil  ;  
       fprintf (fh_Out, "%4.1f,",f);

       f = 0; 
       if ( fN_Den4 > 0 ) 
          f = (fN_Kill4 / fN_Den4) * 100.0 ;   /* Percent of trees kill over 4 inch dbh */ 
       fprintf (fh_Out, "%4.0f,",f);

       f = 0; 
       if ( fN_Den > 0 ) 
         f = (fN_Kill / fN_Den) * 100.0;
       fprintf (fh_Out,"%4.0f\n",f); 

       /* Clear totals */
       fN_Kill = fN_Live = fN_Den = 0;
       fN_BasTot = fN_BasLiv = fN_BasKil = 0; 
       fN_CCPre = fN_CCPost = fN_CCDiff = 0;
       fN_AvgDBHKil = fN_AvgKil = 0; 
       fN_Den4 = fN_Kill4 = 0; 
       strcpy (cr_PltA,cr_PltB); 

       *aiN_Std = *aiN_Std + 1; 
        
       if ( iS == 0 )                   /* End of file was hit */ 
         goto Close;    
     } /* if - Plot Changed */ 

     if ( iS < 0 ) {   /* Record had errors */
       _MortErrMes(&MIS,cr_PltB,cr_ErrMes, &iN_Err, fh_Err); 
       goto GetRec; }

/* Accumulate totals...............................*/
     f_Prob = MRT_CalcMngr (&MIS,&MO,cr_ErrMes);  /* Mortality percent */
     if ( strcmp (cr_ErrMes, "" ) ){ 
      _MortErrMes(&MIS,cr_PltB,cr_ErrMes, &iN_Err, fh_Err); 
       goto GetRec; }

     f_kill = f_Prob * MIS.f_Den;
     f_live = MIS.f_Den - f_kill;  
     fN_Live += f_live; 
     fN_Kill += f_kill;
     fN_Den += MIS.f_Den; 
     if ( MIS.f_DBH >= 4.0 ) {
        fN_Den4 += MIS.f_Den;
        fN_Kill4 +=  f_kill;   }

     fN_BasTot  += (Basal_Area (MIS.f_DBH, MIS.f_Den) / 144.0);    
     fN_BasLiv  += (Basal_Area (MIS.f_DBH, f_live) / 144.0);    
     fN_BasKil  += (Basal_Area (MIS.f_DBH, f_kill) / 144.0);    

     f_CC = SMT_CalcCrnCov (MIS.cr_Spe, MIS.f_DBH, MIS.f_Hgt);
     f_CCPre  = f_CC *  MIS.f_Den;
     f_CCPost = f_CC *  f_live; 
     f_CCDiff = f_CCPre - f_CCPost; 
     fN_CCPre += f_CCPre; 
     fN_CCPost += f_CCPost;
     fN_CCDiff += f_CCDiff;

     fN_AvgDBHKil += MIS.f_DBH * f_kill;
     fN_AvgKil += f_kill; 

GetRec:
     iS = Get_TreRec (&MIS,cr_PltB,cr_ErrMes,f_FS,cr_FS); 

  }    /* while (1) */ 

Close:


   Bat_Salvage (fh_Tre, cr_InFN,  f_FS,  cr_FS, iS_CL, cr_ErrMes, cr_SalvFN);
   if ( strcmp (cr_ErrMes,"" )) {
     iN_Err++;
     fprintf (fh_Err, "%s\n",cr_ErrMes); }
   if ( iS_CL != 1 )             /* if not doing Command line batch */
      myMessageBox (cr_ErrMes); 

CloseFile:
   fclose (fh_Tre); 
   fclose (fh_Out); 


   return (int) iN_Err; 
}


/*****************************************************************
* Name: Bat_Salvage
* Desc: Create an output Salvage file
*       Report is only create if a least one switch is found 
*        in the file.
*       There could be 1,2,3 or NO switches in the file, 
*        #SalDia-1, #SalDia-2, #SalDia-3
*        #SalDia  1   6   0.7     1 >=  < 6 inches, 0.7 prob mort
*
* NOTE: DO THIS function after the the main batch is done when 
*        input tree file has been opened and error checked 
*   In: fh_Tre....Opened input tree file handle
*       cr_InFN...input tree file name, use this to make Salvage
*                 output file name
*       f_FS......Flame Length or Scorch height
*       cr_FS....."F" or "S"
*       NOTE......use GUI Batch Wnd inputs
*                 or -1 with Command Line Batch 
*       iS_CL.....1 to put column headings in output file 
*       cr_SalvFN...Salvage output report file name
*  Ret: 1........file created
*       0........no file create (no switches found in file
*      -1........switches found to be in error 
******************************************************************/
int Bat_Salvage (FILE *fh_Tre, char cr_InFN[], float f_FS, char cr_FS[], int iS_CL, char cr_ErrMes[], char cr_SalvFN[])
{
int i, j, iS, iN;
float f_Prob;  
char cr[3000], cr_Plt[200], A[10];

char DBH[20],Hgt[20],CR[20],CrnDam[20],CKR[20], BolCha[20];

FILE *fh_Sal; 
d_PST pst; 
d_MIS MIS;
d_MO  MO;

   strcpy (cr_ErrMes,""); 
   strcpy (cr_SalvFN,""); 

   PST_Init ();   /* table to hold siz cla info */
   iN = 0;

/* Get any Size Class switchs from input Tree file */
  for ( i = 0; i < 3; i++ ) {
      j =  MRT_Switch_SC (fh_Tre, i+1, &pst, cr);
      if ( j < 0 ) {            /* Error */
        myMessageBox (cr); 
        return -1;  }
      if ( j == 0 )             /* not found */ 
        continue;   
      PST_Put (i, pst.low, pst.up, pst.cut); /* Save info */
      iN++;
   } /* for i */ 

   if ( iN == 0 )             /* No switches found, so no report */
     return 0; 

   if ( !PST_Check () ) {
     sprintf (cr_ErrMes, "Missing/Invalid input tree file switch: %s \n", e_SC); 
     return -1; }

/* Open the output Salvage file */
   _MakeFileName (cr_InFN, cr_SalvFN, e_SalvExt);
   fh_Sal = fopen (cr_SalvFN, "w"); 
   if ( fh_Sal == NULL ) {
     sprintf (cr_ErrMes, "Error opening Batch Salvage File: \n%s\n",cr_SalvFN); 
     return 0; } 

  fseek (fh_Tre,0L,SEEK_SET);                /* Set to begin of file         */

/* Get The FIRST record (non-comment '#') - Note the first row in the file MUST be the column headings */
/*  if should come back with an error, if not we report an error  */
/*  should have already been checked, but we still need to get past it (read in) */
 i = Get_TreRec (&MIS, cr_Plt, cr,f_FS,cr_FS); 
  if ( !strstr (cr_Plt, e_PlotId) ) {  
    strcpy (cr_ErrMes, "The first row of the input file does not appear to contain the\n");
    sprintf (cr, "reguired column headings, starting with '%s' in the first column \n",e_PlotId);
    strcat (cr_ErrMes,cr); 
    return 0;  }

/* Put column heads - use same col names as used on the Salvage GUI report */
  if ( iS_CL == 1 ) {
   fprintf (fh_Sal, "Plot,Species,DBH,Height,Crn,Crn,CKR,Btle,Bole,Mort,Cut,Status\n");
   fprintf (fh_Sal, "Id,Code,(inch),(feet),Rat,Dam%c,(0-4),Dam,Char,Prob,Off,(Live/Dead)\n", '%');
  }


  while (1) {
     iS = Get_TreRec (&MIS, cr_Plt, cr, f_FS,cr_FS);   
     if ( iS == 0 ) 
       break; 

    _FloToChar (DBH,    "%4.1f", MIS.f_DBH);
    _FloToChar (Hgt,    "%4.0f", MIS.f_Hgt);   
    _FloToChar (CR,     "%4.0f", MIS.f_CR);     
    _FloToChar (CrnDam, "%4.0f", MIS.f_CrnDam);
    _FloToChar (CKR,    "%4.0f", MIS.f_CKR);   
    _FloToChar (BolCha, "%4.0f", MIS.f_BolCha);

     fprintf (fh_Sal, "%s,%s,%s,%s,%s,%s,%s,%s,%s,",
              cr_Plt, MIS.cr_Spe, 
              DBH,  Hgt, CR,
              CrnDam, CKR, MIS.cr_BeeDam,
              BolCha);
 
     MIS.f_ProbCutOff = PST_GetProb(MIS.f_DBH); /* Get Prob Cut-off for this Siz Cla */

     f_Prob = MRT_CalcMngr  (&MIS, &MO, cr);    /* get Prob Mort */  
     fprintf (fh_Sal, "%4.2f,",f_Prob); 

     if (MIS.f_ProbCutOff < 0 ) {               /* No Size Class for this dbh */
       fprintf (fh_Sal, ",\n");               /* so leave columns blank */
       continue; }

     if ( f_Prob >= MIS.f_ProbCutOff )          
        strcpy (A,"K");                        /* Tree is killed */ 
     else 
        strcpy (A,"L"); 

     fprintf (fh_Sal, "%4.2f,%s\n", MIS.f_ProbCutOff,A); 

  } /* while */ 

  fclose (fh_Sal); 

  return 1; 
}

/********************************************************
*
*
********************************************************/
int _FloToChar (char cr[], char fmt[], float f)
{
   strcpy (cr,""); 
   if ( f < 0 ) 
     return 0;
   sprintf (cr,fmt,f); 
   return 1; 
} 




/*********************************************************
* Name: _FlaSco_SetDef
* Desc: Set the flame scorch default values that the user
*       entered on the Batch Window. 
*       We only want to set these values for the GUI
*       batch, NOT when doing the Command line batch. 
*       In the case of the GUI Batch it's ok if the fields
*       are missing, but for the Command line the user is 
*       required to enter them in the input file. 
********************************************************/ 
int _FlaSco_SetDef (d_MIS *a, float f_FS, char cr_FS[])
{
/* "" means the Command Line is run, so don't set defaults */
   if ( !strcmp (cr_FS,"" ) )
     return 0;

/* For the GUI Batch, if either the numeric value */
/* for the "F"  "S" code is missing - we set defaults */
   if ( a->f_FS < 0 || !strcmp(a->cr_FS,"" ) ) {
     a->f_FS = f_FS;
     strcpy (a->cr_FS,cr_FS); 
     return 1; }   

   return 0; 
}

/************************************************
* Name: GetRec
* Desc: get the next Tree input record
*   In: f_FS.....Flame Lenght - Scorch Height
*       cr_FS...."F", "S" 
*       NOTE: for .... 
*             > GUI batch use the value on the Batch Wnd
*             Command Line Batch, send in -1.0, in this
*             case the value Must be in the input record
*  Ret: 1 ok
*       0 End of File
*      -1 Error 
************************************************/
int Get_TreRec (d_MIS *MIS, char cr_Plot[], char cr_Err[], float f_FS, char cr_FS[])
{
int i; 

  strcpy (cr_Err,""); 
  strcpy (cr_Plot,"EOF"); 

 Top:
 i = CDF_Get (a, fh_Tre, cr_Err); 
  if ( i == 0 ) 
     return 0;  /* End of File */
  if ( i < 0 ) 
     return -1;  /* Error, see cr_Err */

  if ( !stricmp (s_ft.cr_Status,"D") )     /* Skip Dead Trees */ 
     goto Top; 

  if ( s_ft.f_DBH > 0 && s_ft.f_DBH < 0.2 )  /* Skip Seedling - See Note-1 above */
     goto Top; 

  sprintf (cr_Plot, "%s-%s-%s", s_ft.cr_Plot,s_ft.cr_MonSta,s_ft.cr_MonOrd);

  strcpy (MIS->cr_Spe,s_ft.cr_Spe);
  strcpy (MIS->cr_EquTyp,s_ft.cr_EquTyp);

  MIS->f_Den = s_ft.f_Den;
  MIS->f_DBH = s_ft.f_DBH;
  MIS->f_Hgt = s_ft.f_Hgt;
  MIS->f_CR = _CrownRatio (s_ft.i_CrnRat); /* convert 0->100 to 0->10 */

  strcpy (MIS->cr_FS,s_ft.cr_FS);      /* "S" Scorch Height, "F" - Flame Len */
  MIS->f_FS = s_ft.f_FlaSco;  
 _FlaSco_SetDef (MIS, f_FS, cr_FS); 

  strcpy (MIS->cr_FirSev,s_ft.cr_FirSev);  /* Fire Severity "Low" or "" */ 

  MIS->f_CrnDam = s_ft.f_CrnScoPer;         /* Crown Damage Percent */
  MIS->f_CKR = s_ft.f_CKR;                  /* Cambium Kill Ratio */
  strcpy (MIS->cr_BeeDam, s_ft.cr_BeeDam);  /* Beetle Damage */
  _SetYesNo (MIS->cr_BeeDam); /* Chk - Convert Ex; "Yes" -> "Y" */

  MIS->f_BolCha = s_ft.f_BolChaHgt;       /* Bole Char height */  

/* Check inputs */
  i = MIS_ChkRec(MIS,-1,cr_Err); 
  if ( i == 0 ) /* error */ 
    return -1;


/* Check Flame Length - Scorch Height */ 
  if ( !ChkFlaSco (MIS->f_FS, MIS->cr_FS, cr_Err) )
    return -1; 

 return 1;
} 

/*********************************************************************
* Name: _ChkFlaSco
* Desc: Check flame/scorch value and code 
**********************************************************************/
int  ChkFlaSco (float f_FS, char cr_FS[], char cr_Err[])
{
  if ( !stricmp (cr_FS,e_Scorch)){
    if (f_FS < e_ScoLow || f_FS > e_ScoHig ){
       sprintf (cr_Err,"Invalid/Missing - Scorch Height %4.2f is out of %4.0f -> %4.0f range ", f_FS, e_ScoLow,e_ScoHig);
       return 0; }}
  else if ( !stricmp (cr_FS,e_Flame)){
    if (f_FS < e_FlaLow || f_FS > e_FlaHig ){
       sprintf (cr_Err,"Invalid/Missing - Flame Length %4.2f is out of %4.0f -> %4.0f range ", f_FS, e_FlaLow,e_FlaHig);
       return 0; }}
  else {
      sprintf (cr_Err,"Invalid/Missing Flame/Scorch Code (%s) Valid Codes -> %s or %s",cr_FS,e_Flame,e_Scorch);
      return 0; }

  return 1;  /* ok */ 
} 



/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _MortErrMes
* Desc: Check for and error message in the string, if so print it out
*        and count the error
*   In: see below
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int _MortErrMes ( d_MIS *a, char cr_Plot[], char cr_ErrMes[], int *aiN_Err, FILE *fh_Err)
{
  if ( !strcmp (cr_ErrMes,"" ))
    return 1;

  if ( fh_Err == NULL )
    printf  ("ERROR - Plot: %s, Species: %s \n %s, Record will not be processed\n\n", cr_Plot, a->cr_Spe, cr_ErrMes );
  else
    fprintf  (fh_Err, "ERROR - Plot: %s, Species: %s \n %s, Record will not be processed\n\n", cr_Plot,  a->cr_Spe, cr_ErrMes );

  *aiN_Err = *aiN_Err + 1;
  return 0;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _mError_Window
* Desc:
*   In:
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void _mError_Window  (char A[], char B[], FILE *fh_Err)
{
char cr[3000]; 
   sprintf ( cr, "%s\n%s",A,B);
   if ( fh_Err == NULL )
     myMessageBox (cr); //  Error_Window (A,B);
   else
     fprintf (fh_Err,"%s\n",A);
}



/****************************************************************************
* Name: MRT_Switch_SC
* Desc: Look for the Size Class switches in the input mortality tree
*        input file  
* Note-1: skip any really long lines, they won't be the switch and they
*          might screw up sscanf ()
*  Ret: 1  switch found
*       0  not found
*      -1  Switch found in error, message in cr_ErrMes 
****************************************************************************/
int MRT_Switch_SC (FILE *fh, int i_SC,  d_PST  *pst, char cr_ErrMes[])
{
int i; 
char  *a,  A[1000], B[100], C[100], D[100];
char   cr_Sw[100], cr_IS[3000]; 

  strcpy (cr_ErrMes,"");
  sprintf (cr_Sw, "%s-%d",e_SC,i_SC);  

   fseek (fh,0L,SEEK_SET);    

  while (1) {                              /* Look thru file for switch      */
    a = fgets (cr_IS, 1000, fh);
    if ( a == NULL ) {                       /* End of File, no switch found   */
      sprintf (cr_ErrMes,"Missing batch input file switch %s1\n - Ex: %s1 1  6  0.5 \n <=low  <up   cutoff (kill >= )\n",e_SC,e_SC);   
      return 0;  }  

    if ( isBlankCR (cr_IS) )               /* Empty Line                     */
       continue;

    strcpy (A,"");
    strcpy (B,"");
    strcpy (C,"");
    strcpy (D,"");

    i = strlen (cr_IS);       /* See Note-1 above */
    if ( i > 100 )
      continue; 
    sscanf (cr_IS,"%s %s %s %s",A,B,C,D);

    if ( !xstrcmpi (A, cr_Sw))   /* Look for Size Class  switch  */
      break;                 /* Not it */
 
   } /* while */ 

/* we'll check for valid values later */  
   if ( !strcmp (D,"" )) 
      goto Err;
   pst->cut = atof (D); 

   if ( !strcmp (C,"" ) )
       goto Err;
   pst->up = atof (C); 

   if ( !strcmp (B,"" ) )
       goto Err;
   pst->low = atof (B); 

   return 1;  /* Switch Found */  


Err:
   sprintf (cr_ErrMes,"Invalid batch input file switch %s1\n - Ex: %s1 1  6  0.5 \n <=low  <up   cutoff (kill >= )\n",e_SC,e_SC);   
   return -1; 
}