/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_suf    Soil input user file
* Desc: functions for doing a soil user input file
*
* Date:  4/7/05
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
#include <windows.h>
#include <windowsx.h>
#include <winuser.h>
#include <winbase.h>
#include <commctrl.h>

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "Wnd_Mai.h" 
#include "Wnd_Def.h" 
#include "Wnd_Util.h"

#include  "fof_sgv.h"
#include  "fof_util.h"
#include  "fof_gen.h"
#include  "fof_suf.h"
#include  "fof_sh.h"
#include  "fof_sha.h"
// #include  "fof_suf2.h"
#include  "fof_se.h"
#include  "fof_sd.h"
#include  "fof_sh2.h"
#include  "fof_ci.h"
#include  "fof_co.h"


// #include  "fof_srm.h"
// #include  "win_gra.h"
// #include  "win_mai.h"

#include "Unit_Abv.h" 

/* Signitures for user input files                                           */
#define e_DuffSig   "Duff-Model"
#define e_NonDuffSig    "NonDuff-Model"

// extern char gcr_ErrMes[];

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
// int   SUF_ChkSet (d_USI ar_USI[], char cr_Sw[], float f_Val, char cr[]);
int   SUF_ReadFil (d_SE *a_SE, d_SD *a_SD, char cr_Type[], char cr_InFN[], char cr_Err[]);
// int   SUF_SwAll (d_USI ar_SID[], char cr[] );
int   SUF_isDufExp (char cr_FN[], char  cr_Type[], char cr_Err[]);
int SUR_ReadNonDuff (d_SE *a_SE, char cr_InFN[], char cr_Err[]);


int SUR_ReadDuff (d_SD *a_SD, char cr_InFN[], char cr_Err[]);
float _FindSwArg (FILE *fh, char cr_Switch[],  char cr_Err[]);
int   _FindSw    (FILE *fh, char cr_Switch[], char cr_Line[], char cr_Err[]);


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SUF_Exec
* Desc: Run soil heating using an input file supplied by user.
*       The input file is the ascii file that the Duff Simm or Exp Heat
*       use.
*       Loads Soil Heating Array, for report and graphing
* NOTE: The main menu has options for creating sample input files so you
*        create them and run this option.
*
* NOTE: the input file has the settings in it to determine which layer 
*        information the simulation will create and also how long
*        it will run.
* Note-1: Need to fill in the duff head efficency (amount of heat that
*         make it to soil) This amount when running FOFEM GUI is found
*         in the Settings Window, but not in the soil input file so 
*         we use 100 as a default amount. 
*    In: cr_PthFN.....Path and file name of user input soil file
*  Ret: 1 OK,  0 Error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int Wnd_Mai:: SUF_Exec ()
{
int i;
float f; 
char cr_PthFN[500], cr_Err[300], cr_ErrMes[500];
char cr_Type[50], cr_FN[100];
String ^ Str; 
d_SE s_SE;
d_SD s_SD;
d_SO s_SO;
d_CO s_CO;
d_SI s_SI;
float frWL[3300], frHS[44444];

System::Windows::Forms::OpenFileDialog FD;

// #define e_DuFil  "duf files (*.duf)|*.duf|All files (*.*)|*.*"
// #define e_ExFil  "exp files (*.exp)|*.exp|All files (*.*)|*.*"
//  orig #define e_SFil  "duff files (*.duf)|*.duf|non-duff/exp file (*.exp)|*.exp|All files (*.*)|*.*"

#define e_SFil  "Duff, Non-Duff Files (*.duf, *.exp)|*.duf;*.exp"

  strcpy (cr_ErrMes,""); 
  strcpy (cr_PthFN,""); 
 
/* Pop Up n OprnFileDialog to get a file name */
/* Use the Path & File Name from the Project File TextBox */
  FD.Title = "FOFEM - Run Soil Input File"; 
  FD.InitialDirectory = this->Prj_GetPath();  
  FD.FileName = ""; 
  FD.Filter = e_SFil;
  i = (int) FD.ShowDialog(); 
  if ( i != 1 )
    return 0; 

  Str = FD.FileName;  /* Get Path + File Name from DialogBox */ 
 _StrToChar (Str,cr_PthFN);

/* Determine if input file is a Duff or Zero Duff (Exp) Type                 */
   if ( !SUF_isDufExp (cr_PthFN,cr_Type,cr_Err)){
     sprintf (cr_ErrMes, "%s\n File Name: %s - SUF)Exec*(", cr_Err, cr_PthFN);
     return 0; }

   SHA_Init ();                                /* Init the Soil Heat Temp Array*/

   if ( !strcmp (cr_Type,e_DuffSig) ) {        /* Duff input selected by user */
     if ( !SUR_ReadDuff (&s_SD, cr_PthFN, cr_Err) )
       goto A;
     if ( s_SD.f_DufDepPre <= 0 ) {            /* Make sure there was duff specified */
       strcpy (cr_Err,"The soil input file contains and invalid duff amount.");
       goto A; }
     for ( i = 0; i <= e_mplus1; i++ ) { 
       this->soil_file->FR[i] = s_SD.rr_z[i];  /* Save for Graph legend */
       s_SD.rr_z[i] = s_SD.rr_z[i] * 10.0; }   /* Model wants in kilometers */
     SD_Mngr_New(&s_SD,"",cr_Err);            /* Duff simulation              */
     this->soil_file->f_SoilMoist = s_SD.r_startwc * 100.0; /* to display on Graph */ 
     this->soil_file->f_DufMoi = s_SD.f_DufMoi;
     f = s_SD.f_DufConPer / 100.0 ;   /* Percent Consumed */
     f = s_SD.f_DufDepPre * f;            /* depth consumed  */
     this->soil_file->f_DufDepPos = s_SD.f_DufDepPre - f;  /* Remaining/post fire depth */   
   }

/*.......................................................*/
/*  Non-Duff                                             */
   if ( !strcmp (cr_Type, e_NonDuffSig) ) {        /* Non-Duff input selected by user */
     if ( !SUR_ReadNonDuff (&s_SE, cr_PthFN, cr_Err) )
       goto A;
     for ( i = 0; i <= e_mplus1; i++ ) { 
       this->soil_file->FR[i] = s_SE.rr_z[i];  /* Save for Graph legend */
       s_SE.rr_z[i] = s_SE.rr_z[i] * 10.0; }   /* Model wants in kilometers */
     SE_Mngr_Array (&s_SE, s_SE.frWL, s_SE.frHS, s_SE.f_fiInt, "" ,cr_Err);  
     this->soil_file->f_DufDepPos = 0; 
     this->soil_file->f_SoilMoist = s_SE.r_startwc * 100.0; /* to display on Graph */ 
   }

A:
   if ( strcmp (cr_Err,"" )) {                /* IF error message */
     sprintf (cr_ErrMes, "SUF_Exec() %s", cr_Err); 
     myMessageBox(cr_ErrMes); 
     return 0; }

  this->soil_file->Graph_File("Init");
  this->soil_file->UpdateWatts(); 
  this->soil_file->Show(); 

  return 1;
}


/**********************************************************************
* Name: SUR_ReadNonDuff
* Desc: *
**********************************************************************/
int SUR_ReadNonDuff (d_SE *a_SE, char cr_InFN[], char cr_Err[])
{
int i; 
float f; 
char cr_ErrMes[2000], cr_Line[3000], *a, cr[300];
FILE *fh;

   fh = fopen (cr_InFN,"r");                 /* Open the Input file          */
   if ( fh == NULL ) {
       sprintf (cr_Err, "Error Opening File: %s ", cr_InFN);
       return 0; }

   if ( !_FindSw (fh,e_NonDuffSig, cr_Line, cr_ErrMes)) {   /* make sure it's a duff soil input file  */
      sprintf  (cr_Err, "Invalid Input File Type - %s",cr_ErrMes ); 
      fclose (fh); 
      return 0; }

   a_SE->r_bd        =  _FindSwArg (fh, "bulk-density",    cr_Err);  if ( strcmp (cr_Err,"")) return 0;
   a_SE->r_pd        =  _FindSwArg (fh, "particle-density",cr_Err);  if ( strcmp (cr_Err,"")) return 0;
   a_SE->r_xo        =  _FindSwArg (fh, "extrap-water",    cr_Err);  if ( strcmp (cr_Err,"")) return 0;
   a_SE->r_ls        =  _FindSwArg (fh, "thermal-conduct", cr_Err);  if ( strcmp (cr_Err,"")) return 0;
   a_SE->r_ga        =  _FindSwArg (fh, "Vries-shape",     cr_Err);  if ( strcmp (cr_Err,"")) return 0;
   a_SE->r_xwo       =  _FindSwArg (fh, "water-content",   cr_Err);  if ( strcmp (cr_Err,"")) return 0;
   a_SE->r_cop       =  _FindSwArg (fh, "cop-power",       cr_Err);  if ( strcmp (cr_Err,"")) return 0;
   a_SE->i_dt        =  _FindSwArg (fh, "time-step",       cr_Err);  if ( strcmp (cr_Err,"")) return 0;
   a_SE->r_startwc   =  _FindSwArg (fh, "start-water",     cr_Err);  if ( strcmp (cr_Err,"")) return 0;
   a_SE->r_starttemp =  _FindSwArg (fh, "start-temp",      cr_Err);  if ( strcmp (cr_Err,"")) return 0;

/* Deal with the soil depth layer info */   
  for ( i = 0; i <= e_mplus1; i++ ) {  /* Init the arrays to hold info */
      a_SE->rr_z[i] = 0;               /* layer depths */
      a_SE->rr_node[i] = 1; }          /* Display layer 1 = yes */

   if ( !_FindSw (fh, "layers", cr_Line,  cr_ErrMes)) {
     sprintf (cr_Err, "No soil layers specifed in file - %s", cr_ErrMes); 
     fclose (fh); 
     return 0; }

   sscanf (cr_Line, "%s %f %f %f %f %f %f %f %f %f %f %f %f %f",
          cr,  
          &a_SE->rr_z[2],
          &a_SE->rr_z[3],
          &a_SE->rr_z[4],
          &a_SE->rr_z[5],
          &a_SE->rr_z[6],
          &a_SE->rr_z[7],
          &a_SE->rr_z[8],
          &a_SE->rr_z[9],
          &a_SE->rr_z[10],
          &a_SE->rr_z[11],
          &a_SE->rr_z[12],
          &a_SE->rr_z[13],
          &a_SE->rr_z[14]);

/* check layers depths read in, should catch missing or out of order depths */
   for ( i = 2; i <= e_mplus1; i++ ) { 
     if ( a_SE->rr_z[i] == 0 )
       break;
    if ( a_SE->rr_z[i] < a_SE->rr_z[i-1] ) {
      strcpy (cr_Err, "Invalid or out of order layer depth specified");
      fclose (fh); 
      return 0; }
   }  /* for */


   a_SE->f_SoilHsEff = _FindSwArg (fh, "efficiency-HS", cr_Err);  if ( strcmp (cr_Err,"")) return 0;
   a_SE->f_SoilWlEff = _FindSwArg (fh, "efficiency-WL", cr_Err);  if ( strcmp (cr_Err,"")) return 0;
   a_SE->f_fiInt     = _FindSwArg (fh, "fi-interval",   cr_Err);  if ( strcmp (cr_Err,"")) return 0;  

   for ( i = 0; i < e_FI; i++ )
      a_SE->frWL[i] = a_SE->frHS[i] = -1;  
 
  _FindSw (fh, "fire-intensity", cr_Line,  cr_Err); 
   for ( i = 0; i < e_FI; i++ ) {
      a = fgets (cr_Line, 500, fh );
      if ( a == NULL ) 
        break;
      sscanf (cr_Line,"%f %f", &a_SE->frWL[i], &a_SE->frHS[i] );
   }

   fclose (fh);
   return 1; 
}

/**********************************************************************
* Name: SUR_ReadDuff
* Desc: Read in the Soil Duff User file, 
*       For an example of the user input file format see the
*       function that outputs one to file. 
* NOTE: the file switches don't have to be in any particular 
*       order on this version we did, like they did in the original
*       file. 
**********************************************************************/
int SUR_ReadDuff (d_SD *a_SD, char cr_InFN[], char cr_Err[])
{
int i; 
float f; 
char cr_ErrMes[2000], cr_Line[3000], *a, cr[300];
FILE *fh;

   fh = fopen (cr_InFN,"r");                 /* Open the Input file          */
   if ( fh == NULL ) {
       sprintf (cr_Err, "Error Opening File: %s ", cr_InFN);
       return 0; }

   if ( !_FindSw (fh, e_DuffSig, cr_Line, cr_ErrMes)) {   /* make sure it's a duff soil input file  */
      sprintf  (cr_Err, "Invalid Input File Type - %s",cr_ErrMes ); 
      fclose (fh); 
      return 0; }

   a_SD->f_DufLoaPre =  _FindSwArg (fh, "duff-weight"       , cr_Err);  if ( strcmp (cr_Err,"")) return 0;
   a_SD->f_DufDepPre =  _FindSwArg (fh, "duff-depth"        , cr_Err);  if ( strcmp (cr_Err,"")) return 0;
   a_SD->f_DufConPer =  _FindSwArg (fh, "duff-consumed"     , cr_Err);  if ( strcmp (cr_Err,"")) return 0;
   a_SD->f_DufMoi    =  _FindSwArg (fh, "duff-moisture"     , cr_Err);  if ( strcmp (cr_Err,"")) return 0;
   a_SD->r_bd        =  _FindSwArg (fh, "bulk-density"      , cr_Err);  if ( strcmp (cr_Err,"")) return 0;
   a_SD->r_pd        =  _FindSwArg (fh, "particle-density"  , cr_Err);  if ( strcmp (cr_Err,"")) return 0;
   a_SD->r_xo        =  _FindSwArg (fh, "extrap-water"      , cr_Err);  if ( strcmp (cr_Err,"")) return 0;
   a_SD->r_ls        =  _FindSwArg (fh, "thermal-conduct"   , cr_Err);  if ( strcmp (cr_Err,"")) return 0;
   a_SD->r_ga        =  _FindSwArg (fh, "Vries-shape"       , cr_Err);  if ( strcmp (cr_Err,"")) return 0;
   a_SD->r_xwo       =  _FindSwArg (fh, "water-content"     , cr_Err);  if ( strcmp (cr_Err,"")) return 0;
   a_SD->r_cop       =  _FindSwArg (fh, "cop-power"         , cr_Err);  if ( strcmp (cr_Err,"")) return 0;
   a_SD->i_dt        =  _FindSwArg (fh, "time-step"         , cr_Err);  if ( strcmp (cr_Err,"")) return 0;
   a_SD->r_startwc   =  _FindSwArg (fh, "start-water"       , cr_Err);  if ( strcmp (cr_Err,"")) return 0;
   a_SD->r_starttemp =  _FindSwArg (fh, "start-temp"        , cr_Err);  if ( strcmp (cr_Err,"")) return 0;

   a_SD->f_EffFF     =  _FindSwArg (fh, "efficiency-duff"   , cr_Err);  if( strcmp (cr_Err,"")) return 0;


/* Deal with the soil depth layer info */   
  for ( i = 0; i <= e_mplus1; i++ ) {  /* Init the arrays to hold info */
      a_SD->rr_z[i] = 0;               /* layer depths */
      a_SD->rr_node[i] = 1; }          /* Display layer 1 = yes */

   if ( !_FindSw (fh, "layers", cr_Line,  cr_ErrMes)) {
     sprintf (cr_Err, "No soil layers specifed in file - %s", cr_ErrMes); 
     fclose (fh); 
     return 0; }

   sscanf (cr_Line, "%s %f %f %f %f %f %f %f %f %f %f %f %f %f",
          cr,  
          &a_SD->rr_z[2],
          &a_SD->rr_z[3],
          &a_SD->rr_z[4],
          &a_SD->rr_z[5],
          &a_SD->rr_z[6],
          &a_SD->rr_z[7],
          &a_SD->rr_z[8],
          &a_SD->rr_z[9],
          &a_SD->rr_z[10],
          &a_SD->rr_z[11],
          &a_SD->rr_z[12],
          &a_SD->rr_z[13],
          &a_SD->rr_z[14]);

/* check layers depths read in, should catch missing or out of order depths */
   for ( i = 2; i <= e_mplus1; i++ ) { 
     if ( a_SD->rr_z[i] == 0 )
       break;
    if ( a_SD->rr_z[i] < a_SD->rr_z[i-1] ) {
      strcpy (cr_Err, "Invalid or out of order layer depth specified");
      fclose (fh); 
      return 0; }
   }  /* for */



   fclose (fh);
   return 1; 

}

/****************************************************************
* Name: _FindSwArg
* Desc: Find switch in file and get its argument;
* NOTE:  file will get CLOSED here if error found 
****************************************************************/
float _FindSwArg (FILE *fh, char cr_Switch[], char cr_Err[])
{
int i;
float f; 
char  A[3000], *a,  cr[1000];
   strcpy (cr_Err,""); 
   fseek(fh,0,0);                            /* Begin of file */
   for ( i = 0; i < 10000; i++ ) {           /* Read each switch in Input    */
      a = fgets (A, 300, fh );
      if ( a == NULL ) {                      /* EOF, shouldn't get here yet     */
        sprintf  (cr_Err, "Switch Not Found (%s) ", cr_Switch); 
        fclose (fh); 
        return -1; }

      strcpy (cr,"");
      f = -1;
      sscanf (A, "%s %f", cr, &f ); 
      if ( stricmp (cr,cr_Switch))
        continue;
      break;
 
  }  /* for */
  if ( f < 0 ) {
    sprintf  (cr_Err, "Invalid Argument Found for - %s ", cr_Switch); 
    fclose (fh); 
    return -1; }
  
   return f; 
}

/************************************************************
* Name _FindSw
* Desc: Find a switch in file, don't look for an argument
************************************************************/
int  _FindSw (FILE *fh, char cr_Switch[], char A[], char cr_Err[])
{
int i;
float f; 
char *a,  cr[1000];
   strcpy (cr_Err,""); 
   fseek(fh,0,0);                            /* Begin of file */
   for ( i = 0; i < 10000; i++ ) {           /* Read each switch in Input    */
      a = fgets (A, 300, fh );
      if ( a == NULL ) {                      /* EOF, shouldn't get here yet     */
        sprintf  (cr_Err, "Switch Not Found (%s) ", cr_Switch); 
        return 0; }
      sscanf (A, "%s", cr); 
      if ( stricmp (cr,cr_Switch))            /* Not it - keep looking */
        continue;
      break;                                  /* Found it */
  }  /* for */
  
 return 1;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SUF_isDufExp
* Desc: See if a 'User' Soil input file is a Duff-Model or Non-Duff Model
*       Do this by looking for the Id Signiture
*   In: file name
*  Out: cr_Type...e_DuffSig, e_ExpSib or "Error"
*       cr_Err.....any error message
*  Ret: 1 OK    0 Error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  SUF_isDufExp (char cr_FN[], char  cr_Type[], char cr_Err[])
{
int i;
char  cr_Line[300], cr_Sw[300], *a;
FILE *fh;
   strcpy (cr_Err,"");
   strcpy (cr_Type,"Error");
   fh = fopen (cr_FN,"r");                 /* Open the Input file          */
   if ( fh == NULL ) {
      sprintf (cr_Err, "Error Opening File: %s ", cr_FN);
      return 0; }

   for ( i = 0; i < 10000; i++ ) {           /* Read each switch in Input    */
     a = fgets (cr_Line, 299, fh );
     if ( a == NULL ) {                      /* EOF, should get here yet     */
        fclose (fh);
        break;}
     sscanf (cr_Line, "%s", cr_Sw);
     if ( !stricmp (cr_Sw, e_DuffSig)) {
        strcpy (cr_Type, e_DuffSig);
        return 1; }
     if ( !stricmp (cr_Sw, e_NonDuffSig)) {
        strcpy (cr_Type, e_NonDuffSig);
        return 1; }
   }
  sprintf(cr_Err, "No '%s' or '%s' Identifier found in file", e_DuffSig,e_NonDuffSig);
  return 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SUF_Sample_Exp
* Desc: Create a sample 'user' type Exp Heat input file.
* Note-1: We output the fire intensity for this non-duff file even
*         though there may be a duff amount presentt in the GUI inputs
*         so these fire intensity values won't be a true reflection, but
*         need to put something out. 
*   In: cr_FN...File Name
*  Ret: 1 OK,  else 0
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int SUF_Sample_Exp (char cr_FN[], d_FLI *a_FLI, d_CO *a_CO)
{
int i; 
char cr_ErrMes[2000]; 
FILE *fh;
d_SI  s_SI;
d_SE  s_SE; 

  fh = fopen (cr_FN,"w");
  if ( fh == NULL )
    return 0;

  strcpy (s_SI.cr_SoilType, a_FLI->cr_SoilType);
  SE_Init(&s_SI, &s_SE, cr_ErrMes); 

//  fprintf (fh,"# Zero Duff Model - Example Input File \n");
//  fprintf (fh,"# Use this file type when there is an absence of duff\n");
//  fprintf (fh,"# Lines that start with a # are comments. \n");
//  fprintf (fh,"# Warning: extreme or missing values in this file can \n");
//  fprintf (fh, "# cause unexpected results. \n"); 
// fprintf (fh,"# These inputs represent the current GUI settings.\n");
//  fprintf (fh,"# However the fire intensity values below may not be\n");
//  fprintf (fh,"# totally accurate if there is duff present on the plot\n");   
//  fprintf (fh,"# Values represent soil type = %s \n", s_SI.cr_SoilType);

// New text from Ticket-95 
  fprintf (fh,"# Zero Duff Model - Example Input File\n"); 
  fprintf (fh,"# Use this file type when there is an absence of duff\n");
  fprintf (fh,"# Lines that start with a # are comments.\n"); 
  fprintf (fh,"# Warning: extreme or missing values in this file can\n"); 
  fprintf (fh,"# cause unexpected results.\n"); 
  fprintf (fh,"# These inputs represent the current GUI settings.\n");
  fprintf (fh,"# Fire intensity is simulated using GUI settings.\n"); 
  fprintf (fh,"# Soil characteristics represent %s\n", s_SI.cr_SoilType);
 
  fprintf (fh,"\n"); 

  fprintf (fh,"%s  \n", e_NonDuffSig);
  fprintf (fh,"bulk-density           %4.0f   soil bulk density - (%s) \n",                  s_SE.r_bd, e_GrCuM);
  fprintf (fh,"particle-density       %4.0f   soil particle density - (%s) \n",              s_SE.r_pd, e_GrCuM );
  fprintf (fh,"extrap-water           %4.2f   extrapolated water cont. at -1 (%s) \n",       s_SE.r_xo, e_JoulKg);
  fprintf (fh,"thermal-conduct        %4.2f   thermal conductivity of mineral fraction\n",   s_SE.r_ls);
  fprintf (fh,"Vries-shape            %4.3f   de Vries shape factor\n",                      s_SE.r_ga);
  fprintf (fh,"water-content          %4.3f   water content for liquid recirculation\n",     s_SE.r_xwo);
  fprintf (fh,"cop-power              %4.2f   power for recirculation function\n",           s_SE.r_cop);
  fprintf (fh,"time-step              %d      time step - (%s) \n",                          s_SE.i_dt, e_Second);
  fprintf (fh,"start-water            %4.2f   starting soil water content, Wet 0.25, Mod 0.15, Dry 0.10, V-Dry 0.05,  m3/m3\n", a_FLI->f_MoistSoil / 100.0);
  fprintf (fh,"start-temp             %4.1f   starting soil temperatue (%s) \n",             s_SE.r_starttemp, e_Celsius);

  fprintf (fh,"\n");
  fprintf (fh,"# Specifiy exactly 13 depths - (%s)\n", e_Centimeter); 
  fprintf (fh,"layers   1  2  3 4  5  6  7  8  9  10  11  12  13\n"); 
  fprintf (fh,"\n"); 
 
  fprintf (fh, "efficiency-WL  %4.0f    percent of Wood and Litter fire intensity applied to soil surface \n", a_FLI->f_SoilWlEff);
  fprintf (fh, "efficiency-HS  %4.0f    percent of Herbaceous and Shrub fire intensity applied to soil surface\n", a_FLI->f_SoilHsEff); 
  fprintf (fh, "fi-interval       15    time between fire intensity values listed in arrays below - (%s) \n", e_Second); 
  fprintf (fh, "\n"); 
  fprintf (fh, "# Fire intensity (%s) arrays for Wood/Litter and Herb/Shurb \n", e_KwSqM);
  fprintf (fh, "# specify total fire intensity (above efficiency percents will be applied to these values) \n");  
  fprintf (fh, "# values must start on the line immediately after 'fire-intensity' \n");
  fprintf (fh, "fire-intensity \n");

/* See Note-1 above*/
  for ( i = 0; i < 200000; i++ ) {
    if ( a_CO->fr_SFI[i] < 0 ) 
      break;
    fprintf (fh, "  %4.0f   %4.0f \n", a_CO->fr_SFI[i], a_CO->fr_SFIhs[i] ); 
   }

/* Just in case the arrays above were empty, we'll put something out */
  if ( a_CO->fr_SFI[0] < 0 ) {
    fprintf (fh, "  45   70 \n" ); 
    fprintf (fh, "  50   81 \n" ); 
    fprintf (fh, "  55   82 \n" ); 
    fprintf (fh, "  70   83 \n" ); 
    fprintf (fh, "  85   84 \n" ); 
    fprintf (fh, " 150   85 \n" ); 
    fprintf (fh, "  50    0 \n" ); 
    fprintf (fh, "  50    0 \n" ); 
    fprintf (fh, "   0    0 \n" ); 
    fprintf (fh, "   0    0 \n" ); }


  fclose (fh);
  return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SUF_Sample_Duff
* Desc: Create a sample 'user' type duff sim input file.
* Note-1: Better make sure that there was some duff coming in from the
*         GUI, it could be 0.  We still need to make the sample file
*         so stick something in. 
*   In: cr_FN...File Name
*       a_FLI...GUI inputs
*       f_DufConPer....calculated percent of duff consumed
*  Ret: 1 OK,  else 0
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int SUF_Sample_Duff (char cr_FN[], d_FLI *a_FLI, float f_DufConPer)
{
int i; 
float f, f_Duff, f_DuffDepth; 
char cr_ErrMes[3000];
FILE *fh;
d_SD  s_SD;
d_SI  s_SI; 

  fh = fopen (cr_FN,"w");
  if ( fh == NULL )
    return 0;

  strcpy (s_SI.cr_SoilType, a_FLI->cr_SoilType);
  SD_Init(&s_SD, &s_SI, cr_ErrMes); 

/* See Note-1 above */
  if ( a_FLI->f_Duff <= 0 || a_FLI->f_DuffDepth <= 0 ) { 
    f_Duff = 5.0;
    f_DuffDepth = 0.5; }
  else {
    f_Duff = a_FLI->f_Duff;
    f_DuffDepth = a_FLI->f_DuffDepth; }

//  fprintf (fh,"# Duff Model - Example Inputs File \n");
//  fprintf (fh,"# Use this file type when duff is present \n");
//  fprintf (fh,"# Lines that start with a # are comments. \n");
//  fprintf (fh,"# Warning: extreme or missing values in this file can \n");
//  fprintf (fh,"# cause unexpected results. \n"); 
//  fprintf (fh,"# These inputs represent the current GUI settings. \n"); 
//  fprintf (fh,"# However if no duff is present a fictitious amount will be used.\n");
//  fprintf (fh,"# And the soil type = %s \n", s_SI.cr_SoilType);

// New Text Ticket-95 */
  fprintf (fh,"# Duff Model - Example Input File\n"); 
  fprintf (fh,"# Use this file type when duff is present\n"); 
  fprintf (fh,"# Lines that start with a # are comments.\n"); 
  fprintf (fh,"# Warning: extreme or missing values in this file can\n"); 
  fprintf (fh,"# cause unexpected results.\n"); 
  fprintf (fh,"# These inputs represent the current GUI settings.\n"); 
  fprintf (fh,"# Soil characteristics represent %s\n",s_SI.cr_SoilType);

  fprintf (fh,"\n"); 
  fprintf (fh,"%s          \n", e_DuffSig);
  fprintf (fh,"duff-weight            %4.1f   pre-burn weight - (%s) \n",                    f_Duff, e_TonAc);
  fprintf (fh,"duff-depth             %4.2f   duffdepth - (%s) \n",                          f_DuffDepth, e_Inch );
  fprintf (fh,"duff-consumed          %4.1f   percent consumed, whole integer 0 -> 100 \n",  f_DufConPer );
  fprintf (fh,"duff-moisture          %4.1f   very dry 20, dry 40, moderate 75, wet 130 \n", a_FLI->f_MoistDuff);
  fprintf (fh,"bulk-density           %4.0f   soil bulk density - (%s) \n",                  s_SD.r_bd, e_GrCuM);
  fprintf (fh,"particle-density       %4.0f   soil particle density - (%s) \n",              s_SD.r_pd, e_GrCuM );
  fprintf (fh,"extrap-water           %4.2f   extrapolated water cont. at -1 (%s) \n",       s_SD.r_xo, e_JoulKg);
  fprintf (fh,"thermal-conduct        %4.2f   thermal conductivity of mineral fraction\n",   s_SD.r_ls);
  fprintf (fh,"Vries-shape            %4.3f   de Vries shape factor\n",                      s_SD.r_ga);
  fprintf (fh,"water-content          %4.3f   water content for liquid recirculation\n",     s_SD.r_xwo);
  fprintf (fh,"cop-power              %4.2f   power for recirculation function\n",           s_SD.r_cop);
  fprintf (fh,"time-step              %d      time step - (%s) \n",                          s_SD.i_dt, e_Second);
  fprintf (fh,"start-water            %4.2f   starting soil water content, Wet 0.25, Mod 0.15, Dry 0.10, V-Dry 0.05,  m3/m3\n", a_FLI->f_MoistSoil / 100.0);
  fprintf (fh,"start-temp             %4.1f   starting soil temperatue (%s) \n",             s_SD.r_starttemp, e_Celsius);
  fprintf (fh,"\n");
  fprintf (fh, "efficiency-duff       %4.0f  percent of Duff fire intensity applied to soil surface \n", a_FLI->f_SoilDuffEff);
  fprintf (fh,"\n");


  fprintf (fh,"# Specifiy exactly 13 depths (%s)\n", e_Centimeter); 
  fprintf (fh,"layers   1 2 3 4 5 6 7 8 9 10 11 12 13\n"); 
  fclose (fh);
  return 1;
}
