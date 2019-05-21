

/***********************************************************************
* Name: Wnd_Prj.cpp
* Desc: 
*
************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <math.h> 

#include "Wnd_Mai.h" 
#include "Wnd_Util.h"
#include "Wnd_Def.h"
#include "fof_util.h"
#include "fof_ci.h"
#include "CVT_CVT.h"
#include "fof_fwf2.h"
#include "fof_sgv.h"
#include "fof_co.h" 
#include "fof_co2.h" 
#include "fof_cm.h"
#include "Fuel_Frm.h" 
#include "fof_sh.h" 
#include "fof_sd.h" 
#include "fof_mrt.h"
#include "fof_pf2.h" 

using namespace System::IO;

// #define e_PrjOFD  "prj files (*.fcp)|*.fcp|prj files (*.prj)|*.prj|All files (*.*)|*.*"

#define e_PrjOFD  "prj files (*.prj)|*.prj|All files (*.*)|*.*"

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/*                        Project Switch Argument Struct                     */
/* a Project's line is read in and parsed into here                          */
typedef struct {
char  cr1[1000];            /* Project Switch goes here                        */
char  cr2[1000];            /* 1st Argument                                    */
char  cr3[1000];
char  cr4[1000];
char  cr5[1000];
char  cr6[1000];
} d_Arg;

/*------------------------------------------------------------------------------*/
/* Project Switches */
#define e_PrjRegion     "Region"   

#define e_PrjFFI_File   "FFI_File" 

#define e_PrjFFI_FileMort "FFI_FileMort"  
#define e_PrjFFI_LoadAll  "FFI_LoadAll"

#define e_PrjFuelClass "FuelClass"
#define e_PrjFuelCat   "FuelCat"
#define e_PrjLogDist   "LogDist"
#define e_PrjSeason    "Season"
#define e_PrjCoverType "CoverType"
#define e_PrjMoisture  "Moisture"
#define e_PrjSoilType  "SoilType"

#define e_PrjTPA_Litter        "Litter"
#define e_PrjTPA_1Hr        "1Hr"
#define e_PrjTPA_10Hr       "10Hr"
#define e_PrjTPA_100Hr      "100Hr"
#define e_PrjTPA_1kHr       "1kHr"
#define e_PrjTPA_Duff       "Duff"
#define e_PrjTPA_Herb       "Herb"
#define e_PrjTPA_Shrub      "Shrub"
#define e_PrjTPA_Foliage    "Foliage"
#define e_PrjTPA_Branch     "Branch"

#define e_PrjWood_Moist_10Hr  "Wood_Moist_10Hr"
#define e_PrjWood_Moist_1kHr   "Wood_Moist_1kHr"
#define e_PrjDuff_Moist        "Duff_Moist"
#define e_PrjSoil_Moisture     "Soil_Moisture"

#define e_PrjRotten         "Rotten"
#define e_PrjDuff_Depth     "Duff_Depth"
#define e_PrjBurn_Crown     "Burn_Crown"

#define e_PrjDuff_Code      "Duff_Code"
#define e_PrjFlaSco_LenHei  "FlaSco_LenHei"
#define e_PrjFlame_Length   "Flame_Length"
#define e_PrjScorch_Height  "Scorch_Height"

// #define e_PrjPostFileInjury "PostFileInjury"
// #define e_PrjRelHumidity    "RelHumidity"

#define e_PrjTotal_Name     "Total_Name"
#define e_PrjTotal_Yes      "Total_Yes"
#define e_PrjTotal_No       "Total_No"
       
/* Mortality Switches */
#define e_PrjFlaLen         "FlameLength"
#define e_PrjScoHgt         "ScorchHeight" 
#define e_PrjLenHgt         "LenHgt" 
#define e_PrjSpe            "Species" 
#define e_PrjSpePI         "SpeciesPI"   /* Post Fire Injury */ 

#define e_LowFirSev       "LowFireSeverity"    /* Checkbox */

#define e_PrjMortality "Mortality"                  /* Do Reg Mortality */
#define e_PrjMortPFI   "Mortality-Postfire-Injury"  /* Post Fire Injury */

#define e_PrjPFICutoff "ProbCutoff" 
#define e_PrjSalRep   "SalvageReport"  /* Salvage Report Checkbox */ 

#define e_PrjEmiFla  "EmisFlame"
#define e_PrjEmiSmo  "EmisSmold"
#define e_PrjEmiDuf  "EmisDuff" 

#define e_PrjEmiMet  "EmisMethod"

#define e_PrjBatFS   "BatchFlaSco"   /* Batch Wnd - Flame Scorch settings */ 

#define e_BeeDamBlk  "x"  /* Beetle Damage field is blank */

int Prj_FindSw (FILE *fh, char cr_Switch[], d_Arg  *a_Arg, char cr_Line[]);
int PST_Get (int ix, float *low, float *up, float *cut);
void _PutProb (TextBox ^ tb, char cr[]);

/**************************************************************************
* Name: Prj_OpenStr
* Desc: Open and load a project .prj file
* NOTE: This function uses a String for the Path File Name for pussy callers
*        who can't handle char[]
**************************************************************************/
int Wnd_Mai::Prj_OpenStr(String ^ s_FN)
{
int i; 
char cr[500];
   
    _StrToChar (s_FN,cr); 
    i = Prj_Open (cr);  /* Call the real deal */ 
   return i; 
}

/*******************************************************************************************
* Name: Prj_Open
* Desc: Open Project file and load the setting into the GUI/Form1 the main window
*      
******************************************************************************************/
int Wnd_Mai::Prj_Open (char cr_inFN[])
{
bool b; 
int i, i_Pre, iS_Err; 
float f; 
char A[100], B[100], cr_Line[1000]; 
char cr_Spe[100], cr_Pri[100], cr_Min[100], cr_Max[100]; 
FILE *fh;
char cr_FN[500], *adr, cr[1000], cr_ErrMes[500]; 
char  cr_Den[20], cr_DBH[20], cr_Hgt[20], cr_CR[20];
char cr_Sco[50],  cr_CKR[50], cr_Btl[50];

String ^Str;
String ^s_FilNam; 
d_Arg s_Arg; 
System::Windows::Forms::OpenFileDialog FD;
  iS_Err = 0; 

  if ( stricmp (cr_inFN,"") ) {
    s_FilNam = _CharToStr( cr_inFN);	
    strcpy (cr_FN,cr_inFN); 
    goto Open; }

/* Put up an OpenFileDialog Box, and use the Path that is in the Project File */
/*  TextBox  */
  FD.Title = "FOFEM Open Project File"; 
  FD.InitialDirectory = this->Prj_GetPath();  
  FD.FileName = ""; 
  FD.Filter = e_PrjOFD;
  i = (int) FD.ShowDialog(); 
  if ( i != 1 )           /* User Canceled out out Dialog Box */
    return 0; 
  s_FilNam = FD.FileName;  /* Get Path + File Name */
 _StrToChar (s_FilNam,cr_FN); 

Open:

   fh = fopen (cr_FN, "r");
   if ( fh == NULL ) {
     sprintf (cr,"Can't Find/Open Project file %s \n", cr_FN); 
     myMessageBox (cr);
     return 0; }

/*.................................................*/
  this->sS_FFI_Prj = "No";
  this->s_FFI_PthFN = "";

  if ( Prj_FindSw (fh, e_PrjFFI_File, &s_Arg, cr) ) { 
    this->s_FFI_PthFN = _CharToStr (s_Arg.cr2);
    this->sS_FFI_Prj = "Yes";} 

  Prj_SetComboBox (fh, e_PrjFuelClass, this->_cbFuelClass, 0);
  this->sS_FFI_Prj = "No";
     
/*.................................................*/

/* ComboBoxes */
  Prj_SetComboBox (fh, e_PrjRegion,    this->_cbRegion,    0);

  Prj_SetComboBox (fh, e_PrjCoverType, this->_cbCoverType, 1);
  Prj_SetComboBox (fh, e_PrjFuelCat,   this->_cbFuelCat,   0);
  Prj_SetComboBox (fh, e_PrjSeason,    this->_cbSeason,    0);
  Prj_SetComboBox (fh, e_PrjMoisture,  this->_cbMoisture,  0);
  Prj_SetComboBox (fh, e_PrjLogDist,   this->_cbLogDist,   0);


/* Emissions, we keep and set the New Method settings even when user has Old set */ 
  Prj_SetComboBox (fh, e_PrjEmiFla, this->_cbEmiFla, 1);   /* New Emission settings */
  Prj_SetComboBox (fh, e_PrjEmiSmo, this->_cbEmiSmo, 1);
  Prj_SetComboBox (fh, e_PrjEmiDuf, this->_cbEmiDuf, 1);

/* Emission Method - Old or New, start off assuming Old, we'll set as Old if Switch arg  */
/*  is not New or incase no switch is found */   
  this->set_frm->_rb_EmiOld->Checked = true;         /* Setting Wnd Emis Radio But as Old */ 
  this->groupBox3->Visible = false;                  /* To hide Main Wnd Emis ComboBoxs */
  Prj_FindSw (fh, e_PrjEmiMet, &s_Arg, cr);          /* Get Method Switch */
  if ( !stricmp (s_Arg.cr2,"New") ){                 /* Method is New, else assume Old */ 
    this->set_frm->_rb_EmiNew->Checked = true;       /* Setting Wnd radio button */ 
    this->groupBox3->Visible = true; }                                      /* To show comboboxes */

/* Fuel Load TextBoxes */
  Prj_SetFuelBox (fh, e_PrjTPA_Duff,    this->_txDuff);
  Prj_SetFuelBox (fh, e_PrjTPA_Litter,  this->_txLitter   );
  Prj_SetFuelBox (fh, e_PrjTPA_1Hr,     this->_tx1Hr   );
  Prj_SetFuelBox (fh, e_PrjTPA_10Hr,    this->_tx10Hr  );
  Prj_SetFuelBox (fh, e_PrjTPA_100Hr,   this->_tx100Hr );
  Prj_SetFuelBox (fh, e_PrjTPA_1kHr,    this->_tx1kHr  );
  Prj_SetFuelBox (fh, e_PrjTPA_Herb,    this->_txHerb  );
  Prj_SetFuelBox (fh, e_PrjTPA_Shrub,   this->_txShrub );
  Prj_SetFuelBox (fh, e_PrjTPA_Foliage, this->_txFoliage);
  Prj_SetFuelBox (fh, e_PrjTPA_Branch,  this->_txBranch);


/* Moistures */
   Prj_SetTextBox (fh, e_PrjWood_Moist_10Hr, this->_txMoist10Hr, 0);
   Prj_SetTextBox (fh, e_PrjWood_Moist_1kHr , this->_txMoist1kHr,0);
   Prj_SetTextBox (fh, e_PrjDuff_Moist   ,    this->_txMoistDuff,0);
   Prj_SetTextBox (fh, e_PrjSoil_Moisture,    this->_txMoistSoil,0);

   Prj_SetTextBox (fh, e_PrjRotten    ,  this->_txRotLog,2);
   Prj_SetTextBox (fh, e_PrjDuff_Depth,  this->_txDuffDepth,1);
   Prj_SetTextBox (fh, e_PrjBurn_Crown,  this->_txFolBraCon,0);

/* Make sure both Mortality and Post Fire Mortality tree datagrids */
/*  are cleared before we start */
   this->_dgMort->Rows->Clear();


/* Mortality......................................*/
/* NOTE: "Regular" refers to the original or usual way mortality */
/*       is done in FOFEM, verses the Postfire Injury type that */
/*       was added in later */ 
/* There are some separate GUI controls for each type ,like the */
/* the species datagrids */ 

char cr_CrnDam[20], cr_EquTyp[30], cr_BeeDam[20],  cr_BolCha[20];
    for ( i = 0; i < 100; i++ ) {
     sprintf (cr, "%s-%d", e_PrjSpe, i+1);
     if ( !Prj_FindSw (fh, cr, &s_Arg, cr_Line))
       break;
      sscanf (cr_Line,"%s %s %s %s %s %s %s %s %s %s %s", A, cr_Spe, cr_EquTyp, cr_Den, cr_DBH, cr_Hgt, cr_CR,
                                           cr_CrnDam, cr_CKR, cr_BeeDam, cr_BolCha);
      if ( !stricmp (cr_BeeDam,e_BeeDamBlk) ) 
        strcpy (cr_BeeDam,""); 
      Put_SpeRec(cr_Spe, cr_EquTyp, cr_Den, cr_DBH, cr_Hgt, cr_CR,  cr_CrnDam, cr_CKR, cr_BeeDam, cr_BolCha);
   } /* for */ 




/*------------------------------------------------------------*/
/* Probability Cutoff Size Classes - 3 size classes */ 
char a1[20],a2[20],a3[20],a4[20],a5[20],a6[20],a7[20],a8[20],a9[20];
    if ( !Prj_FindSw (fh, e_PrjPFICutoff, &s_Arg, cr_Line))
       goto NoProb;

   sscanf (cr_Line,"%s %s %s %s %s %s %s %s %s %s", A, a1,a2,a3,a4,a5,a6,a7,a8,a9);
   _PutProb(this->_txSC1A,a1);   /* low limit dbh */  
   _PutProb(this->_txSC1B,a2);   /*  upper */
   _PutProb(this->_txSC1C,a3);   /* prob cutoff */ 
 
   _PutProb(this->_txSC2A,a4); 
   _PutProb(this->_txSC2B,a5); 
   _PutProb(this->_txSC2C,a6); 

   _PutProb(this->_txSC3A,a7); 
   _PutProb(this->_txSC3B,a8); 
   _PutProb(this->_txSC3C,a9); 

NoProb: 

/* ------------------------------------------------- */
/* Salvage Report Checkbox */ 
   if ( Prj_FindSw (fh, e_PrjSalRep, &s_Arg, cr_Line) )
     this->_cb_SalRep->Checked = true;
   else 
     this->_cb_SalRep->Checked = false; 


/* Load regular FlameLen/ScorHgt textbox and the Postfire Injury Cutoff textbox */
//   Prj_SetTextBox (fh, e_PrjPFICutoff , this->_txProMorCut,1);   /* Prob Mort Cutoff value */
   i = Prj_SetTextBox (fh, e_PrjLenHgt, this->_txFlaSco,0);          /* Flame Lenght / Scorch Height */
   if ( i == 0 ) 
     this->_txFlaSco->Text = ""; 

/* Flame Lenght - Scorch Height Radio Buttons used on for Regular Mortality */
   if ( Prj_FindSw (fh, e_PrjFlaLen, &s_Arg, cr_Line) )
     this->_rbFlameLen->Checked = true;
   if ( Prj_FindSw (fh, e_PrjScoHgt, &s_Arg, cr_Line) )
     this->_rbScorchHeight->Checked = true;



/* Low Fire Severity Checkbox */ 
   this->_chLowFireSev->Checked = false; 
   if ( Prj_FindSw (fh, e_LowFirSev, &s_Arg, cr_Line) ) 
     this->_chLowFireSev->Checked = true;   

/* Show/Hide Regular and Postfire Injury GUI controls accordingly */       
//   this->Mort_PFIGUI();                 /* show the Mort Regular controls */

/* And load the Master Species dropdown list accordingly */ 
   this->Mort_LoadSpeMstLst();          /* Load Species Master List */



/* Mortality FFI Trees.........................*/
  this->_cbFFI_Plots->Enabled = false;            /* will go active if we load a file below */
  if ( Prj_FindSw (fh, e_PrjFFI_FileMort, &s_Arg, cr_Line) ) { /* Get FFI File Name if there */
     strcpy (cr,s_Arg.cr2);
    _ArgBlank (cr);                                     /* remove fill chars from Path File Name */
    this->FFI_LoadPlotNames(cr);                        /* Load Plot Names & Tree Grid */
    if ( Prj_FindSw (fh,e_PrjFFI_LoadAll, &s_Arg, cr_Line)) /* Load error tree records CheckBox */
      this->_cbFFI_LoadAll->Checked = true; 
 }


/*..............................................*/
  Str = _CharToStr(cr_FN); 
  this->_txProFilNam->Text = Str; 

  fclose (fh); 
  return 1; 
}

/**************************************************
* Name _PutProb
* Desc: Put a Probability Cutoff Size Class number
*        to its textbox, but check for blank indicator
*       -1's were used in project file to signify
*       blanks 
***************************************************/
void _PutProb (TextBox ^ tb, char cr[])
{
   if ( !stricmp (cr,"-1") || 
        !stricmp (cr,"-1.0") || 
        !stricmp (cr,"-1.00") ) {
     _PutTextBox(tb,""); 
     return ; }
  _PutTextBox(tb,cr); 
}

/**********************************************************
* Name:
*
**********************************************************/
int Wnd_Mai::Prj_FFI_File (FILE *fh)
{
char cr[1000];
d_Arg s_Arg;

  if ( !Prj_FindSw (fh, e_PrjFFI_File, &s_Arg, cr) ) 
    return 0; 
  this->s_FFI_PthFN = _CharToStr (s_Arg.cr2);
  return 1; 
}

/*****************************************************************************************
* Name: Prj_SetTextBox
* Desc: Find the switch and put the argument - a float to a textbox
*   In: cr_PrjSw...the project switch
*       tb.........text box
*       i_Per......decimal percision to show in the text 
*  Ret: 
******************************************************************************************/
int Wnd_Mai::Prj_SetTextBox (FILE *fh, char cr_PrjSw[], TextBox ^ tb, int i_Per)
{
float f; 
char cr[1000];
String ^ s_Adj;
d_Arg s_Arg;

  if ( !Prj_FindSw (fh, cr_PrjSw, &s_Arg, cr) ) 
    return 0; 
  f = atof (s_Arg.cr2); 
  FloatToTextBox(tb,f,i_Per);
  return 1;
}

/*****************************************************************************************
* Name:
* Desc:
*  Ret:
******************************************************************************************/
int Wnd_Mai::Prj_SetFuelBox (FILE *fh, char cr_PrjSw[], TextBox ^ tb)
{
int i;
float f; 
char cr[1000];
String ^ s_Adj;
d_Arg s_Arg;

  if ( !Prj_FindSw (fh, cr_PrjSw, &s_Arg, cr) ) 
    return 0; 

  s_Adj = _CharToStr(s_Arg.cr3);
  this->Set_FueBoxCol (s_Adj, tb->Name); 
 
  f = atof (s_Arg.cr2); 
  FloatToTextBox(tb,f,2);

  return 1; 
}


/*****************************************************************************************
* Name:
* Desc:
*  Ret:
******************************************************************************************/
int Wnd_Mai::Prj_SetComboBox (FILE *fh, char cr_PrjSw[], ComboBox ^ cb, int iS_Fill)
{
int i;
char cr[1000];
String ^ Str;
d_Arg s_Arg;

  if ( Prj_FindSw (fh, cr_PrjSw, &s_Arg, cr) ) {
    if ( iS_Fill == 1 )
      _ArgBlank(s_Arg.cr2);

    Str = _CharToStr(s_Arg.cr2);

    i = (int) cb->FindString(Str);
    cb->SelectedIndex = i;
    return 1;}

  return 0;
}

/*****************************************************************************************
* Name: Prj_SaveMngr
* Desc: Manage the saving of a project file
*   In: cr_Mode......."Save" - just saves that file
*                     "New" - save the file and put the file name into
*                             GUI project file name textbox
*  Ret: 1 ok, 0 = error 
******************************************************************************************/
int Wnd_Mai::Prj_SaveMngr (char cr_Mode[])
{
int i;
char cr_PrjFN[500], cr_ErrMes[500], cr[1000]; 
String ^Str;
String ^Str_FN; 

  i = this->Prj_Save(cr_PrjFN, cr_Mode);
  if ( i == 0 )                  /* Project file wasn't saved */ 
    return 0; 

/* IF the Main Form's Project File textbox is empty, put the new project file name in it*/ 
  Str = this->_txProFilNam->Text->ToString();    /* Prg Fil Nam textbox */
  if ( Str == "" || !stricmp (cr_Mode,"New") ) {
    Str_FN = _CharToStr(cr_PrjFN); 
    this->_txProFilNam->Text = Str_FN; }         /* put new prj file name in it */

  return 1; 
}

/*******************************************************************************************
* Name: Save_Prj
* Desc: Save the screen settings into a .prj Project file
*       This function will popup a System type Save File Dialog box 
*   In: cr_Mode.."Save" or "New" -- see code below
*               
*  Out: cr_PrjFN.......Saved Project file name
*  Ret: 1 ok, else 0
******************************************************************************************/
int Wnd_Mai::Prj_Save(char cr_PrjFN[], char cr_Mode[])
{
int i,j,iN;
float f; 
String ^Str;
char *adr;
char cr_FN[500], cr[500]; 
char cr_Spe[50], cr_Pri[20], cr_Min[50], cr_Max[50];  
FILE  *fh; 
d_FLI s_FLI; 
//d_ISS ISS; 
char  cr_ErrMes[1000]; 
System::Windows::Forms::SaveFileDialog FD;

   strcpy (cr_PrjFN,""); 

/* Check the Mortality Species Grid before we get started */
/* incase there are some error */
   i = this->MIS_Load(cr_ErrMes);
   if ( i == 0 ) {
     myMessageBox (cr_ErrMes);
     return 0; }

/* Load and Check the Probability Cutoff Size Class Table */
/* do it here to catch any errors before we start */
   i = this->Load_Probability(cr_ErrMes);
   if ( i == 0 ) {
      myMessageBox (cr_ErrMes); 
      return 0; }

/* Pop Up a SaveFileDialog to get a file name */
/* Use the Path & File Name from the Project File TextBox */
  if ( !stricmp (cr_Mode,"New") ) {       
    FD.Title = "FOFEM New Project File";   /* Put title on Pop up wnd */
    FD.FileName = "New-Project"; }         /* giv user a default 'new' file name */
  else {
    FD.Title = "FOFEM Save Project File";    /* Title for Save Pop wnd */
    FD.FileName = this->Prj_GetFileName();}  /* user current name in Prj Wnd txtbox */

  FD.InitialDirectory = this->Prj_GetPath();  /* Fil Path from Prj Wnd Textbox */
  FD.Filter = e_PrjOFD;
  i = (int) FD.ShowDialog(); 
  if ( i != 1 )
    return 0; 

  Str = FD.FileName;  /* Get Path + File Name from DialogBox */ 
 _StrToChar (Str,cr_FN); 
  fh = fopen (cr_FN, "w");
  if ( fh == NULL ) {
    sprintf (cr,"Can't open project file: %s \n", cr_FN); 
    myMessageBox (cr);
    return 0; }

  strcpy (cr_PrjFN, cr_FN);  /* Pass this back out to caller */

  i = Get_Inputs(&s_FLI);
  if ( i == 0 ) {
     myMessageBox(s_FLI.cr_ErrMes);
     return 0; }

/* Combo Box Settings */
   fprintf (fh, "#%s %s\n",  e_PrjRegion,    s_FLI.cr_Region); 
   fprintf (fh, "#%s %s\n",  e_PrjFuelClass, s_FLI.cr_FuelClass);
   fprintf (fh, "#%s %s\n",  e_PrjFuelCat,   s_FLI.cr_FuelCat);
   fprintf (fh, "#%s %s\n",  e_PrjLogDist,   s_FLI.cr_LogDist);
   fprintf (fh, "#%s %s\n",  e_PrjSeason,    s_FLI.cr_Season);


/* Emission Factor Groups */
   strcpy (cr, s_FLI.cr_EmisFlame);
   _ArgFill(cr); 
   fprintf (fh, "#%s %s\n",  e_PrjEmiFla, cr);

   strcpy (cr,s_FLI.cr_EmisSmold);
   _ArgFill(cr);
   fprintf (fh, "#%s %s\n",  e_PrjEmiSmo, cr);

   strcpy (cr,s_FLI.cr_EmisDuff);
   _ArgFill(cr);
   fprintf (fh, "#%s %s\n",  e_PrjEmiDuf, cr);

   fprintf (fh, "#%s %s\n", e_PrjEmiMet,s_FLI.cr_EmisMethod);    /* Emission Method - Old or New */

   strcpy (cr, s_FLI.cr_CoverType );
   _ArgFill(cr);   /* replace any embedded blank chars */
   fprintf (fh, "#%s %s\n",  e_PrjCoverType, cr);

/*....................................................*/
  if ( !stricmp (s_FLI.cr_FuelClass, e_FFI ) ) { 
      _StrToChar (this->s_FFI_PthFN,cr);
      _ArgFill (cr);
     fprintf (fh,"#%s %s\n", e_PrjFFI_File, cr);   
  }
/*....................................................*/


   fprintf (fh, "#%s %s\n",  e_PrjMoisture,  s_FLI.cr_Moisture);
   fprintf (fh, "#%s %s\n",  e_PrjSoilType,  s_FLI.cr_SoilType);
   
/* Fuel TextBoxs */
   fprintf (fh, "#%s %6.2f %s \n", e_PrjTPA_Litter , s_FLI.f_Litter ,   Prj_GetAdj (this->_txLitter ) );
   fprintf (fh, "#%s %6.2f %s\n",  e_PrjTPA_1Hr    , s_FLI.f_1Hr    ,   Prj_GetAdj (this->_tx1Hr    ) );
   fprintf (fh, "#%s %6.2f %s\n",  e_PrjTPA_10Hr   , s_FLI.f_10Hr   ,   Prj_GetAdj (this->_tx10Hr   ) );
   fprintf (fh, "#%s %6.2f %s\n",  e_PrjTPA_100Hr  , s_FLI.f_100Hr  ,   Prj_GetAdj (this->_tx100Hr  ) );
   fprintf (fh, "#%s %6.2f %s\n",  e_PrjTPA_1kHr   , s_FLI.f_1kHr   ,   Prj_GetAdj (this->_tx1kHr   ) );
   fprintf (fh, "#%s %6.2f %s\n",  e_PrjTPA_Duff   , s_FLI.f_Duff   ,   Prj_GetAdj (this->_txDuff   ) );
   fprintf (fh, "#%s %6.2f %s\n",  e_PrjTPA_Herb   , s_FLI.f_Herb   ,   Prj_GetAdj (this->_txHerb   ) );
   fprintf (fh, "#%s %6.2f %s\n",  e_PrjTPA_Shrub  , s_FLI.f_Shrub  ,   Prj_GetAdj (this->_txShrub  ) );
   fprintf (fh, "#%s %6.2f %s\n",  e_PrjTPA_Foliage, s_FLI.f_Foliage,   Prj_GetAdj (this->_txFoliage) );
   fprintf (fh, "#%s %6.2f %s\n",  e_PrjTPA_Branch , s_FLI.f_Branch ,   Prj_GetAdj (this->_txBranch ) );

   fprintf (fh, "#%s %6.2f\n", e_PrjDuff_Depth,  s_FLI.f_DuffDepth);

   fprintf (fh, "#%s %6.2f\n", e_PrjDuff_Moist,      s_FLI.f_MoistDuff);
   fprintf (fh, "#%s %6.2f\n", e_PrjWood_Moist_10Hr,s_FLI.f_Moist10Hr);
   fprintf (fh, "#%s %6.2f\n", e_PrjWood_Moist_1kHr, s_FLI.f_Moist1kHr);
   fprintf (fh, "#%s %6.2f\n", e_PrjSoil_Moisture,   s_FLI.f_MoistSoil);

   fprintf (fh, "#%s %6.2f\n", e_PrjRotten, s_FLI.f_PerRot);
   fprintf (fh, "#%s %6.2f\n", e_PrjBurn_Crown, s_FLI.f_PerFB);

/*--------------------------------------------------------*/
/*              Tree Mortality                            */


/* Regular Mortality Controls .. . . */
   if ( this->_chLowFireSev->Checked )       /* Low Fire Severity Checkbox */ 
     fprintf (fh, "#%s \n", e_LowFirSev); 

   if ( this->_rbScorchHeight->Checked )
     fprintf (fh, "#%s \n", e_PrjScoHgt);
 
   if ( this->_rbFlameLen->Checked )
     fprintf (fh, "#%s \n", e_PrjFlaLen);  

   if ( _GetTextBox (this->_txFlaSco,cr) )
     fprintf (fh, "#%s %s\n", e_PrjLenHgt, cr); 

   j = this->_dgMort->Rows->Count;

/* New Mortality Grid   */
/* Note - we check for errors above */
d_MIS s_MIS; 
   this->MIS_Load(cr_ErrMes);   
   for ( i = 0; i < 1000; i++ ) {
     if ( !MIS_Get (i, &s_MIS) )
       break; 
     if ( !strcmp (s_MIS.cr_BeeDam,"" ) )
        strcpy (s_MIS.cr_BeeDam,"x"); 
     fprintf (fh, "#%s-%d %s %s %2.1f %2.1f %2.1f %2.0f %2.1f %2.0f %s %2.1f \n", e_PrjSpe, i+1, 
                                     s_MIS.cr_Spe, s_MIS.cr_EquTyp,
                                     s_MIS.f_Den,  s_MIS.f_DBH,
                                     s_MIS.f_Hgt,  s_MIS.f_CR,
                                     s_MIS.f_CrnDam, s_MIS.f_CKR,
                                     s_MIS.cr_BeeDam, s_MIS.f_BolCha );
   }  

/*-------------------------------------------------*/
/*  Probability Cut Off Size Classes */
/*  PST table was at top of this function */  
float low, up, cut; 
   fprintf (fh,"#%s ", e_PrjPFICutoff ); 
   for ( i = 0; i < 3; i++ ) {
     PST_Get (i, &low, &up, &cut);
     fprintf (fh, "%4.1f %4.1f %4.2f   ", low, up, cut); }
   fprintf (fh, "\n"); 

  
/* ------------------------------------------------ */
/* Salvage Report Checkbox, put switch in if box is checked */ 
   if ( this->_cb_SalRep->Checked == true ) 
     fprintf (fh, "#%s\n",e_PrjSalRep); 


/*-------------------------------------------------- */
/* Mortality - FFI Input File, if in use   */

   if ( !strcmp (this->cr_FFI_FN,""))
      goto NoFFITree;
   strcpy (cr,this->cr_FFI_FN);
  _ArgFill (cr);
   fprintf (fh,"#%s %s\n", e_PrjFFI_FileMort, cr);  
   if ( this->_cbFFI_LoadAll->Checked == true ) 
     fprintf (fh, "#%s \n",e_PrjFFI_LoadAll);

NoFFITree:

/*---------------------------------------------------*/
/* Batch Wnd - Flame/Scorch Settings */
#ifdef BAATTCCH 
  started doing this but it can't be done like this
  because the Batch Wnd could be closed, you'd have 
  to save the Batch Wnd settings while it's open and 
  also deal with if it doesn't get opened
char cr_fs[10],cr_FS[10];

  _GetTextBox (this->bat_frm->_txStands,cr_fs);

  if ( this->bat_frm->_rbScoHei->Checked )
    strcpy (cr_FS,"S");
  else 
    strcpy (cr_FS,"F"); 

  fprintf (fh, "#%s %s %s\n", e_PrjBatFS, cr_fs, cr_FS); 
#endif 

  fclose (fh); 
  return 1; 
}

/**************************************************************************
* Name: Prj_GetAdj
* Desc:
* NOTE:
*
**************************************************************************/
char * Wnd_Mai::Prj_GetAdj (TextBox ^ tb)
{

  if ( isFuelUser    (tb) )  return e_User;
  if ( isFuelTypical (tb) )  return e_Typical ;
  if ( isFuelHeavy   (tb) )  return e_Heavy   ;
  if ( isFuelAbundent(tb) )  return e_Abundant;
  if ( isFuelLight   (tb) )  return e_Light   ;
  if ( isFuelSparse  (tb) )  return e_Sparse  ;
   return e_Typical;
}


/***********************************************************************
* Name: Prj_GetPath
* Desc: Get the Path from the Main Form Project Path File Name TextBox
*       IF the textbox is empty then we use the default folder - which 
*        is the user's My Documents folder
* Note-1: New Forest Service policy doesn't want any files being written
*         into the program startup folder which is under c:\Program Files
*         the new default will be the user's My Documents folder.
***********************************************************************/
String ^ Wnd_Mai::Prj_GetPath()
{
String ^ PFN; 
String ^Path; 
  PFN = "";
  Path = "";
  PFN = this->_txProFilNam->Text->ToString(); /* Get Path File Name */ 

  if ( PFN == "") {
   Path = this->s_UserFolder;
   return Path; }
 
// System::IO::Path::
// I was doing it this way which was wrong ---  Path = Path::GetFullPath(PFN); 
  Path = Path::GetDirectoryName(PFN);  /* get just the path from the path filename */ 
  return Path; 
}

/*******************************************************************************
* Get Just the file name from the Plot File Name TextBox window 
*
********************************************************************************/
String ^ Wnd_Mai::Prj_GetFileName ()
{
String ^ PFN; 
String ^FileName; 

 PFN = this->_txProFilNam->Text->ToString(); /* Get Path File Name */ 
   
 if ( PFN == "" ) 
   return e_PrjNam;
 
// System::IO::Path::
 FileName = Path::GetFileName(PFN);   
 return FileName; 
} 

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Prj_FindSw
* Desc: Look for a Project Switch in the Project File.
*   In: cr_Switch.....switch to look for
*  Out: a_Arg.........Argument Struct, switch and its args get put here
*       cr_Line.......Line of text as it was read in from file,
*                      this is the line that contains the desired switch
*                      you have in case there is an error with the arguments
*  Ret: 1 Ok,    0 Not Found
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int Prj_FindSw (FILE *fh, char cr_Switch[], d_Arg  *a_Arg, char cr_Line[])
{
char  *a, cr[300];

   strcpy (a_Arg->cr1,"");                /* Need to do this or any       */
   strcpy (a_Arg->cr2,"");                /* blank args get junk in them  */
   strcpy (a_Arg->cr3,"");
   strcpy (a_Arg->cr4,"");
   strcpy (a_Arg->cr5,"");
   strcpy (a_Arg->cr6,"");
   strcpy (cr_Line,""); 

   fseek (fh,0L,SEEK_SET);
   while (1) {
      a = fgets ( cr, 200, fh);         /* Read a line from file        */
      if ( a == NULL )                       /* End of File                  */
         break;
      if ( !strcmp (cr,"") )                 /* Empty Line                   */
         continue;
      if ( !strncmp (cr, "# ",2) )              /* comment line */ 
         continue; 
      strcpy (cr_Line,cr);
      StrRepChr (cr, '=', ' ');              /* Take these chars out         */
      StrRepChr (cr, '#', ' ');              /* Take these chars out         */

      strcpy (a_Arg->cr1,"");                /* Need to do this or any       */
      strcpy (a_Arg->cr2,"");                /* blank args get junk in them  */
      strcpy (a_Arg->cr3,"");
      strcpy (a_Arg->cr4,"");
      strcpy (a_Arg->cr5,"");
      strcpy (a_Arg->cr6,"");

      sscanf (cr, "%s %s %s %s %s %s",
          a_Arg->cr1, a_Arg->cr2,
          a_Arg->cr3, a_Arg->cr4,
          a_Arg->cr5, a_Arg->cr6);
      if ( !stricmp (a_Arg->cr1,cr_Switch) )
         return 1;    }

/* Switch wasn't found, so blank these, because they could have the */
/*  data from the last line in the file in them */ 
   strcpy (a_Arg->cr1,"");    
   strcpy (a_Arg->cr2,"");                
   strcpy (a_Arg->cr3,"");
   strcpy (a_Arg->cr4,"");
   strcpy (a_Arg->cr5,"");
   strcpy (a_Arg->cr6,"");
   strcpy (cr_Line,""); 
   return 0;
}



/*************************************************************************
* Name: Read_IniPrj
* Desc: on program startup we get the .ini file, the .ini file 
*        contains the path file name of the project file 
*       .ini file is always in the User Folder, which is different 
*        depending on OS, ex: c:\Documents and Settings\.......whatever
* Ret:   0 couldn't open .ini file 
*       -1 couldn't open prject file, or it had an error
*        1  OK - .ini file found and prj file opened ok
*************************************************************************/
int Wnd_Mai::Prj_IniFil_Read ()
{
int i; 
char cr[500], cr_PrjFN[500]; 
String ^Str; 
FILE *fh; 

  Str = this->s_UserFolder;      /* See Notes above */
  Str = Str + "\\" + e_Ini_FN ;
  _StrToChar (Str,cr); 
  fh = fopen (cr, "r");    /* Open the .ini file */
  if ( fh == NULL )        /* No .ini file found, see Note-1 above */ 
   return 0; 

  strcpy (cr_PrjFN,""); 
  fscanf (fh, "%s",cr_PrjFN);   /* Get the prj file name from the .ini file */  
  _ArgBlank (cr_PrjFN);         /* remove any filled blank embedded  chars */
  fclose (fh); 

  if ( !strcmp (cr_PrjFN,"" ) )  /* if no project fil nam in .ini file */
    return 0; 

  i = this->Prj_Open (cr_PrjFN); 
  if ( i == 0 )                 /* Error in project file */ 
    return -1; 

  return 1; 
}

/****************************************************************************
* Desc:  write out the .ini file with the project file name in it
*        .ini file is always kept in the User Folder which is under
*         win7/8  C:\Users....    XP  c:\Documents and Settins\....
* Note-1: Shouldn't have any problem opening file, but if there is
*          no big deal, program will come up next time ok without it. 
*****************************************************************************/
int Wnd_Mai::Prj_IniFil_Save ()
{
String ^Str; 
char  cr[500];
FILE *fh;

  Str = this->s_UserFolder;  /* See Notes above */

  Str = Str + "\\" + e_Ini_FN ;
  _StrToChar (Str,cr); 

/* We just overwrite the existing .ini with the new one */
  fh = fopen (cr, "w");    /* Open the .ini file */
  if ( fh == NULL )        /* See Note-1 above */ 
   return 0; 
  Str = this->_txProFilNam->Text; /* Get the prj file name in GUI */
  _StrToChar (Str,cr);   
  _ArgFill(cr);             /* Fill any blank chars that might be in path file name */  
  fprintf (fh,"%s\n",cr);   /* write it to the .ini file */ 
  fclose (fh); 
  return 1; 
}

/***************************************************************************
* Name: _ArgFill, _ArgBlank
* Desc: These two functions are used so that I can write switch arguments 
*        to the Project file and not have embedded blanks in them, so
*        we replace blanks going out and then put the blanks back when 
*        we read them in. 
***************************************************************************/
void _ArgFill (char cr[])
{
int i,j;
   j = strlen (cr);
   for ( i = 0; i < j; i++ ) {
    if ( cr[i] == ' ' )
      cr[i] = '$';
   }
}

void _ArgBlank (char cr[])
{
int i,j;
   j = strlen (cr);
   for ( i = 0; i < j; i++ ) {
    if ( cr[i] == '$' )
      cr[i] = ' ';
   }
}
