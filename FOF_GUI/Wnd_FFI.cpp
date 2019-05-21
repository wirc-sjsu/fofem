
#include <windows.h> 
#using <mscorlib.dll>
#include <tchar.h>
#include <stdio.h>
#include <cstring>
#include <string.h>
#include <math.h> 

#include "Wnd_Mai.h" 
#include "Wnd_Def.h"
#include "Wnd_Util.h"
#include "cdf_util.h"
#include "fof_util.h" 
#include "CVT_CVT.h"
#include "fof_fwf2.h"
#include "fof_nes.h"   /* in FOFEM DLL project */ 
#include "fof_smt.h"
#include "fof_ffi.h"
#include "bat_mai.h"

#define e_PltSep  "_::_"

float _csMiss(float f, int *ai);
float  _Sum (float D2, float D3, float D4, float D5 );
int _SetYesNo ( char cr[] );
int _CrownRatio (int i);


extern d_CVT sr_CVT[];                   /* All Cov Typs from disk file       */



d_FFItre s_ft; 

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/*                Comma Delmited Description of Input File                   */
d_CDF  s_CDFTree[] = { 
                 { "cr_Plot",   e_Str, 300,  s_ft.cr_Plot },
                 { "cr_MonSta", e_Str, 90,   s_ft.cr_MonSta }, 
                 { "cr_MonOrd", e_Str, 90,   s_ft.cr_MonOrd }, 

                 { "cr_Spe",    e_Str, 50,   s_ft.cr_Spe  },
                 { "f_Den",     e_Flo,  0,  &s_ft.f_Den   },

                 { "f_DBH",     e_Flo,  0,  &s_ft.f_DBH    },
                 { "f_Hgt",     e_Flo,  0,  &s_ft.f_Hgt    },
                 { "f_CrnHgt",  e_Flo,  0,  &s_ft.f_CrnHgt },  
                 { "cr_Status", e_Str, 50,   s_ft.cr_Status},
                 { "cr_CrnCls", e_Str, 50,   s_ft.cr_CrnCls} ,
                 { "i_CrnRat",  e_Int,  0,  &s_ft.i_CrnRat}, 
  
                 { "f_BolCha",  e_Flo,  0,  &s_ft.f_BolChaHgt }, 
 
                 { "f_CrnScoPer",  e_Flo, 0,  &s_ft.f_CrnScoPer}, 
                 { "f_CrnScoHgt",  e_Flo, 0,  &s_ft.f_CrnScoHgt }, 
                 { "f_CKF",     e_Flo,  0, &s_ft.f_CKR }, 
                 { "cr_BeeDam", e_Str, 50,  s_ft.cr_BeeDam },
                 { "cr_EquTyp", e_Str, 50,  s_ft.cr_EquTyp },
                 { "f_FlaSco",  e_Flo,  0, &s_ft.f_FlaSco },          /* Flame length or scorch height */
                 { "cr_FS",     e_Str,  20, s_ft.cr_FS },          /* "F" or "S" tells whats in f_FlaSco */
                 { "cr_FirSev", e_Str,  20, s_ft.cr_FirSev },        /* Fire Severity  "L" or "",  Low */ 
                 {  "",         e_End,   0,  "" }};

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/*                      Plot Fuels                                           */
typedef struct {
   char cr_Plot[300];
   char cr_MonSta[100];
   char cr_MonOrd[100];

   float f_Duff, f_DufDep,  f_Litter, f_Lichen;  
   float f_1Hr,  f_10Hr, f_100Hr;
   float f_Dc1Sz1,  f_Dc1Sz2,  f_Dc1Sz3,  f_Dc1Sz4,  f_Dc1Sz5;
   float f_Dc2Sz1,  f_Dc2Sz2,  f_Dc2Sz3,  f_Dc2Sz4,  f_Dc2Sz5;
   float f_Dc3Sz1,  f_Dc3Sz2,  f_Dc3Sz3,  f_Dc3Sz4,  f_Dc3Sz5;
   float f_Dc4Sz1,  f_Dc4Sz2,  f_Dc4Sz3,  f_Dc4Sz4,  f_Dc4Sz5;
   float f_Dc5Sz1,  f_Dc5Sz2,  f_Dc5Sz3,  f_Dc5Sz4,  f_Dc5Sz5;
   float f_GrassDead, f_GrassLive;
   float f_ShrubLive, f_ShrubDead; 
   float f_MossLive, f_MossDead;

   float f_SAVShrub, f_SAVGrass, f_SAVOneHr; 

   float f_NotUsed;  /* point the non-used input fields here when reading in */
   
   float f_PileLoad;  /* Pile Load */

/* This holds the stuff we need summed up */
   float f_SC1, f_SC2, f_SC3, f_SC4, f_SC5;
   float f_RC1, f_RC2, f_RC3, f_RC4, f_RC5;
   float f_Grass;
   float f_Moss; 
   float f_Shrub; 

/* Expanded Emission Factor id numbers */
   char  cr_EmisSTFS[50];
   char  cr_EmisDuffRSC[50];
   char  cr_EmisCWDRSC[50];

 } d_FFIPlt;

d_FFIPlt s_fP;

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
d_CDF s_CDFPlot[] = {

 { "cr_Plot       ", e_Str, 300,   s_fP.cr_Plot  },
 { "cr_MonSta     ", e_Str, 90,   s_fP.cr_MonSta }, 
 { "cr_MonOrd     ", e_Str, 90,   s_fP.cr_MonOrd }, 

 { "f_Duff        ", e_Flo,  0,  &s_fP.f_Duff   },
 { "f_DuffDepth   ", e_Flo,  0,  &s_fP.f_DufDep   },
 { "f_Litter      ", e_Flo,  0,  &s_fP.f_Litter },

{ "f_Lichen      ", e_Flo,  0,  &s_fP.f_Lichen },
{ "f_MossLive    ", e_Flo,  0,  &s_fP.f_MossLive },
{ "f_MossDead    ", e_Flo,  0,  &s_fP.f_MossDead },

 { "f_1Hr         ", e_Flo,  0,  &s_fP.f_1Hr    },
 { "f_10Hr        ", e_Flo,  0,  &s_fP.f_10Hr   },
 { "f_100Hr       ", e_Flo,  0,  &s_fP.f_100Hr  },
 { "f_Dc1Sz1      ", e_Flo,  0,  &s_fP.f_Dc1Sz1 },
 { "f_Dc1Sz2      ", e_Flo,  0,  &s_fP.f_Dc1Sz2 },
 { "f_Dc1Sz3      ", e_Flo,  0,  &s_fP.f_Dc1Sz3 },
 { "f_Dc1Sz4      ", e_Flo,  0,  &s_fP.f_Dc1Sz4 },
 { "f_Dc1Sz5      ", e_Flo,  0,  &s_fP.f_Dc1Sz5 },
 { "f_Dc2Sz1      ", e_Flo,  0,  &s_fP.f_Dc2Sz1 },
 { "f_Dc2Sz2      ", e_Flo,  0,  &s_fP.f_Dc2Sz2 },
 { "f_Dc2Sz3      ", e_Flo,  0,  &s_fP.f_Dc2Sz3 },
 { "f_Dc2Sz4      ", e_Flo,  0,  &s_fP.f_Dc2Sz4 },
 { "f_Dc2Sz5      ", e_Flo,  0,  &s_fP.f_Dc2Sz5 },
 { "f_Dc3Sz1      ", e_Flo,  0,  &s_fP.f_Dc3Sz1 },
 { "f_Dc3Sz2      ", e_Flo,  0,  &s_fP.f_Dc3Sz2 },
 { "f_Dc3Sz3      ", e_Flo,  0,  &s_fP.f_Dc3Sz3 },
 { "f_Dc3Sz4      ", e_Flo,  0,  &s_fP.f_Dc3Sz4 },
 { "f_Dc3Sz5      ", e_Flo,  0,  &s_fP.f_Dc3Sz5 },
 { "f_Dc4Sz1      ", e_Flo,  0,  &s_fP.f_Dc4Sz1 },
 { "f_Dc4Sz2      ", e_Flo,  0,  &s_fP.f_Dc4Sz2 },
 { "f_Dc4Sz3      ", e_Flo,  0,  &s_fP.f_Dc4Sz3 },
 { "f_Dc4Sz4      ", e_Flo,  0,  &s_fP.f_Dc4Sz4 },
 { "f_Dc4Sz5      ", e_Flo,  0,  &s_fP.f_Dc4Sz5 },
 { "f_Dc5Sz1      ", e_Flo,  0,  &s_fP.f_Dc5Sz1 },
 { "f_Dc5Sz2      ", e_Flo,  0,  &s_fP.f_Dc5Sz2 },
 { "f_Dc5Sz3      ", e_Flo,  0,  &s_fP.f_Dc5Sz3 },
 { "f_Dc5Sz4      ", e_Flo,  0,  &s_fP.f_Dc5Sz4 },
 { "f_Dc5Sz5      ", e_Flo,  0,  &s_fP.f_Dc5Sz5 },

 { "f_GrassDead   ", e_Flo,  0,  &s_fP.f_GrassDead},
 { "f_GrassLive   ", e_Flo,  0,  &s_fP.f_GrassLive},
 { "f_ShrubLive   ", e_Flo,  0,  &s_fP.f_ShrubLive},
 { "f_ShrubDead   ", e_Flo,  0,  &s_fP.f_ShrubDead},

 { "f_SAVShrub   ", e_Flo,  0,  &s_fP.f_SAVShrub},
 { "f_SAVGrass   ", e_Flo,  0,  &s_fP.f_SAVGrass},
 { "f_SAVOneHr   ", e_Flo,  0,  &s_fP.f_SAVOneHr},

 { "FracGrnCov   ", e_Flo,  0,  &s_fP.f_NotUsed},
 { "f_PileLoad", e_Flo,  0,  &s_fP.f_PileLoad},

 { "cr_EmisSTFS     ", e_Str, 50,   s_fP.cr_EmisSTFS }, 
 { "cr_EmisDuffRSC  ", e_Str, 50,   s_fP.cr_EmisDuffRSC }, 
 { "cr_EmisCWDRSC   ", e_Str, 50,   s_fP.cr_EmisCWDRSC }, 

 { "",               e_End,  0,   "" }    };


/*-------------------------------------------------------------*/
float  _Sum (float D2, float D3, float D4, float D5 );
void ChkSum_Fuel ();
void Plot_Init (d_FFIPlt *a);
void Tree_Convert (d_FFItre *a);
void _DBH_Message ();
void _fpToEnglish (d_FFIPlt *a);
void FFI_ToEnglish  (d_FFIPlt *a);
void Plt_Init (d_FFIPlt *a);
float  _Sum (float D2, float D3, float D4, float D5 );
void Sum1k (d_FFIPlt *a_fP, d_CFL * a_CFL, float *af_Snd, float *af_Rot);
float _SndRotPer (float f_Snd, float f_Rot);

void _MakErrMes (d_FFItre *a, int Row, char cr[]);

int FFI_ChkInp(d_FFItre *a, char cr[]);
int FFI_Chk_CrnSco (d_FFItre *a, char cr[]);

void  FloMisChar (float f, char fmt[], char cr[]);

void CrownRatio (d_FFItre *a);

void MisValCat (float f, char Name[], char Fmt[], char cr[]);
/*********************************************************
* Name: FFI_Close
* Desc: Close (Clear) the Mortality FFI controls, we really
*       don't "close" because the file gets open and closed
*       as it's needed. SO we just clear out the controls
*       so that they don't get saved to the Project file
*       or else confusion will ensue.
*********************************************************/
void Wnd_Mai::FFI_Close()
{
System::Windows::Forms::DialogResult r;
  r =  MessageBox::Show("Do you want to close the FFI file \n and clear tree grid ? ", "FOFEM Mortality", MessageBoxButtons::YesNo); 
  if ( r == ::DialogResult::No ) 
    return ; 
  this->_dgMort->Rows->Clear();  /* Clear Tree Grid */
  this->_txFFI_File->Text = ""; 
  this->_txFFIErr->Text = ""; 
  this->_cbFFI_Plots->Items->Clear();
  this->_cbFFI_Plots->Text = ""; 
  this->_cbFFI_Plots->Enabled = false; 
  this->_cbFFI_LoadAll->Checked = false;
  strcpy (this->cr_FFI_FN,"");   
}

/*************************************************************
* Name: FFI_LoadTreeGrid
* Desc: Load trees from an FFI into Mortality tree grid
*       This is done for the Plot selected in the Plot 
*       combobox 
* Note-1: 4-25-14 Change DL, skip seedlings
*         Seedlings will have a 0.1 DBH, set that way
*         when they are exported out of FFI,
*         compare is done as > 0 and < 0.2 to catch 0.1
*         which because it's a float i not sure if it will 
*         be exactly 0.1 to do an == compare on 
* Note-2: I changed this from a 'for' to a 'while' loop so it didn't
*          matter how many records were in the file, and I also
*          change code to stop going thru the rest of the file
*          once all the need plot records were read in. 
*          Although it didn't seem to make much different in 
*          speed of loading the records on the GUI
* Note-3: For most errors we keep going and put the data into 
*          the Tree Grid and append all the error messages together
*          to show user once the plot is loaded in. 
**************************************************************/
int Wnd_Mai::FFI_LoadTreeGrid()
{
int j,i,k, iX, iN_DBH, iN_Rec; 
float f,g,fN_Err;
char cr[2001], cr_ErrMes[3000], cr_TreeNum[20], cr_FN[500];
char *ac, cr_Plt[300], cr_Eq[100];
FILE *fh; 
d_CDF *a;
String ^Str; 
char cr_LoadAll[20];

   if ( this->_cbFFI_LoadAll->Checked == true )
      strcpy (cr_LoadAll,"Yes"); 
   else
      strcpy (cr_LoadAll,"No");

   fN_Err = 0;  

/* shouldn't even be here, combobox would be empty, if no file was opned */
   if ( !strcmp (this->cr_FFI_FN,"" ) ) {
      myMessageBox ("No FFI File Has Been Opened");
      return 0; }
     
/* Get the Plot Id we need to look for from the GUI Plot Combobox*/
  Str = this->_cbFFI_Plots->Text; 
  if ( Str == "" ) { 
     myMessageBox ("No Plot Selected");
     return 0; }
  _StrToChar (Str,cr_Plt);
  
  iN_DBH = 0; 
  this->_dgMort->Rows->Clear();          /* Clear the Tree grid */

   a = &s_CDFTree[0];                    /* Describes input Tree file format */

  _MakeFileName (this->cr_FFI_FN, cr_FN, ".tre");  /* this can handle periodes in path-file names */

  fh = fopen (cr_FN, "r");
  if ( fh == NULL ) {
    sprintf (cr, "Error Opening File: \n %s",cr_FN);
    myMessageBox(cr); 
    return 0; }

/* Read in the required Column Heading row, it must preceed and tree data rows */
/* it could be before, after or inbetween comment or blank rows, */
/* the first column in the heading row must read 'PlotId' */
   j = CDF_Get ( a, fh, cr_ErrMes); 
   if ( stricmp (s_ft.cr_Plot,e_PlotId) ) {
     strcpy (cr_ErrMes, "File does not have the required row of column heading information.\n");
     strcat (cr_ErrMes, "The row of column headings must preceed and tree data.\n");
     strcat (cr_ErrMes, "The first column heading must be 'PlotId'\n"); 
     return -1; }

  bool b_Fnd = false; 
  iX = 0; 
  iN_Rec = 0;

  _MakeFileName (this->cr_FFI_FN, cr_FN, "-Err.txt");  /* this can handle periodes in path-file names */

  this->IEM_Open(cr_FN);  /* Open the output error file */ 

/*----------------------------------------------------------------------------*/
  while (1) {                               /* for each rec, see Note-2 above */
    j = CDF_Get ( a, fh, cr_ErrMes); 
    if ( j == 0 ) 
      break;                                    /* end of file */
    if ( j == -1 ) {
      sprintf (cr, "Error Reading FFI Tree File: %s ", cr_ErrMes); 
      myMessageBox (cr); 
      return 0; }
     sprintf (cr, "%s%s%s%s%s", s_ft.cr_Plot,e_PltSep,s_ft.cr_MonSta,e_PltSep,s_ft.cr_MonOrd );
     if ( stricmp (cr,cr_Plt) ) {              /* Only want records for this plot # */
       if ( b_Fnd == true )                    /* We already got the records we need */
         break;                                /* so no need to read rest of file   */
       continue; }        

     b_Fnd = true;                            /* OK, found the plot we want */
     iN_Rec++;                                /* counts each rec with in a plot  */

     if ( !stricmp (s_ft.cr_Status,"D") )     /* Skip Dead Trees */ 
        continue; 

     if ( s_ft.f_DBH > 0 && s_ft.f_DBH < 0.2 )  /* Skip Seedling - See Note-1 above */
        continue; 

     if ( strcmp (cr_ErrMes, "" ) ) {           /* Bad column data in input file  */
        _MakErrMes(&s_ft,iN_Rec,cr_ErrMes);     /* possible alpha in numeric field */
        IEM_Add (cr_ErrMes);                    /* See Note-3 above */
        fN_Err++;  }

/* Make sure species is valid, don't bother loading into GUI, it will just */
/*  cause headaches trying to flag it as an error later on */
    j = SMT_GetIdx (s_ft.cr_Spe); 
     if ( j < 0 ) {
       sprintf (cr,"Invalid Species found in input file - Record will NOT be loaded.\n");
      _MakErrMes (&s_ft, iN_Rec, cr);
       IEM_Add(cr);
       fN_Err++;
       continue; } 
        
/* Check Equ Type  */
    if ( !strcmp (s_ft.cr_EquTyp,"" ) )
      strcpy (s_ft.cr_EquTyp, e_CroSco);  /* if none, set it to Crown Scorch */    

    j = SMT_MortEqu_Code (s_ft.cr_Spe, s_ft.cr_EquTyp, cr_Eq);
    if ( j == 0 ) {
      sprintf (cr, "Invalid Equation Type: %s, - Record Will NOT be loaded\n", s_ft.cr_EquTyp);
     _MakErrMes(&s_ft,iN_Rec,cr); 
      IEM_Add (cr);
      fN_Err++; 
      continue;   }

/* Need to do set these before we attempt to validate them */
/* If Crown Ratio is Missing try to calc it from Heights */ 
    CrownRatio (&s_ft);     
   
/* Get Yes/No to  Y or N */
   _SetYesNo (s_ft.cr_BeeDam);  


/* Check inputs according to Equation Type */
    FFI_ChkInp(&s_ft, cr);
    if ( strcmp (cr,"" ) ) {         /* if there was an error - log it */
      _MakErrMes (&s_ft,iN_Rec,cr); 
      IEM_Add (cr); 
      fN_Err++; } 

     this->_dgMort->Rows->Add(1);
     
     Str = _CharToStr (s_ft.cr_Spe);                         /* Species */
     this->_dgMort->Rows[iX]->Cells [edg_Spe]->Value = Str;
  
     FloMisChar (s_ft.f_Den,"%3.1f",cr);                     /* Density - Trees Per Acre */
     Str = _CharToStr (cr); 
     this->_dgMort->Rows[iX]->Cells [edg_Den]->Value = Str;
      
     FloMisChar (s_ft.f_DBH,"%3.1f",cr);                     /* DBH */
     Str = _CharToStr (cr); 
     this->_dgMort->Rows[iX]->Cells [edg_DBH]->Value = Str;
 
     FloMisChar (s_ft.f_Hgt,"%3.1f",cr);                      /* Height */ 
     Str = _CharToStr (cr); 
     this->_dgMort->Rows[iX]->Cells [edg_Hgt]->Value = Str;

     FloMisChar (s_ft.i_CrnRat,"%d",cr);
     Str = _CharToStr (cr);                                   /* Crown Ratio */
     this->_dgMort->Rows[iX]->Cells [edg_CR]->Value = Str;

     FloMisChar (s_ft.f_BolChaHgt,"%3.1f",cr);                /* Bole Char */ 
     Str = _CharToStr (cr); 
     this->_dgMort->Rows[iX]->Cells [edg_BolCha]->Value = Str;

     FloMisChar (s_ft.f_CrnScoPer,"%3.0f",cr);                    /* Crown Damage percent */
     Str = _CharToStr (cr);                                   
     this->_dgMort->Rows[iX]->Cells [edg_CrnDam]->Value = Str;

     FloMisChar (s_ft.f_CKR,"%3.0f",cr);                       /* Cambium Kill Rating */
     Str = _CharToStr (cr); 
     this->_dgMort->Rows[iX]->Cells [edg_CKR]->Value = Str;
 
     Str = _CharToStr (s_ft.cr_BeeDam);                         /* Beetle Damage Y/N */
     this->_dgMort->Rows[iX]->Cells [edg_BeeDam]->Value = Str;

     Str = _CharToStr (s_ft.cr_EquTyp);                         /* Equation Type */
     if ( Str == "" ) 
       Str = e_CroSco;
     this->_dgMort->Rows[iX]->Cells[edg_Equ]->Value = Str; 

/* Set Tree Grid cols Gray/White according to Equation type required input */
     this->Set_ReqCols(iX);                                     

     iX++; 
   }   /* While 1 - getting records for plot*/


/* put the number of errors to the GUI */
  FloatToTextBox (this->_txFFIErr,fN_Err,0); 

/* Close the Error file, it may or may not have errors in it*/
  this->IEM_Close(); 

/* Display Error Window with error in it */
  if ( fN_Err > 0 ) {  /* if we had any errors, show err message */ 
//  this popped up a box to display the s_IEM error string, see IEM_Add()
//     MessageBox::Show ( this->s_IEM, "FOFEM FFI Input File Errors/Warnings"); 
     sprintf (cr,"%4.0f Plots containing errors\n",fN_Err);
     strcat (cr, "\nNOTE:\n"); 
     strcat (cr, "Temporary corrections can be made in the tree grid.\n");   
     strcat (cr, "Permanent changes must be made in the TRE file.\n");
     strcat (cr, "Click the Error File button to open a detailed error file.\n"); 
     myMessageBox (cr); 
     // this->IEM_Display(); /* pop open the error file */  
   } 

  return 1; 
}
/*********************************************************
* Name: CrownRation
* Desc: Convert the Crown Ration 1,2,3... ----> 10,20,30....
*       Also if Missing try to Calc from heights
*********************************************************/
void CrownRatio (d_FFItre *a)
{
int i, j; 
float f,g;

/* Crown Ratio, FFI data can have actual crown ratio or crown to height and we'll calc it*/
  if ( a->i_CrnRat >= 0 ) {           /* if have FFI Crown Ratio */
     i = _CrownRatio (a->i_CrnRat);   /* convert  0,10,20-->100*/
     a->i_CrnRat = i;                 
     return; }

   if ( a->f_Hgt <= 0 ) 
     return;                    /* Can't calc if Missing  */
   if ( a->f_CrnHgt < 0 ) 
      return;                   /* Can't Calc if Missing */ 
   if ( a->f_Hgt <= a->f_CrnHgt )
      return;                   /* shouldn't happen, but can't Calc */ 

   f = a->f_Hgt - a->f_CrnHgt;    /* Height of crown */
   g = f / s_ft.f_Hgt;          /* a percent of tot hgt */
   i = g * 100.0; 
   j = _CrownRatio (i);
   a->i_CrnRat = j;
}

/**********************************************************
* Name: FLoMisChar
* Desc: 
***********************************************************/
void  FloMisChar (float f, char fmt[], char cr[])
{
int i;
  i = int (f);      /* incase format is for decimal/integer */
  strcpy (cr,"");

  if ( f < 0 )      /* Assume any < 0 if a missing value */
    return;         /* so return "" */

  if ( strstr (fmt,"d") ) {  /* do decimal/integer */
    sprintf (cr,fmt,i);
    return ; } 
  
  sprintf (cr,fmt,f); 

}

/******************************************************
* Name: FFI_ChkInp 
* Desc: Check the FFI Tree file record inputs based
*       on their Equation Type setting
* Note-1: Crown Ratio is 0 -> 100 in the file but
*         0 -> 10 in the GUI
*  Out: Error messages - one or error messages could be
*        in this string - ( they get appeneded )
*  Ret: 1 ok, 0 error 
*******************************************************/
int FFI_ChkInp(d_FFItre *a, char cr[])
{
int i; 
char cr_MrtEqu[100], cr_ReqFld[100];
char  A[500];

  strcpy (cr,""); 

/*......................................................*/
/* Crown Scorch Equation Inputs  */
  if ( strcmp (a->cr_EquTyp, e_CroSco) )
     goto CrnDam;           /* Not Crown Scorch */
  if ( a->f_DBH <= 0 || a->f_DBH > 500.0 )  
    MisValCat (a->f_DBH, "DBH", "%4.2f", cr);

  if ( a->f_Hgt <= 0 || a->f_Hgt > 379.0 ) 
    MisValCat (a->f_Hgt,"Tree Height","%4.0f",cr);
 
  if ( a->i_CrnRat <= 0 || a->i_CrnRat > 100  )   /* See Note-1 above */
     MisValCat (a->i_CrnRat,"Crown Ratio","%4.0f",cr);

  goto Ret; 

/*......................................................*/
/* Crown Damage Equation Inputs  */
CrnDam:
  if ( stricmp (a->cr_EquTyp, e_CroDam ) )  
     goto BolCha;      /* Not Crown Damage */

  SMT_MortEqu_Code (a->cr_Spe, a->cr_EquTyp, cr_MrtEqu);
  EFR_Get (cr_MrtEqu, cr_ReqFld);     /* will tell us what's required */
    
  if ( !strstr(cr_ReqFld,"dbh") )   /* is dbh reguired */ 
     goto Btl;                      /*  nope */
  if ( a->f_DBH <= 0 || a->f_DBH > 500.0 )
     MisValCat (a->f_DBH,"DBH", "%4.1f",cr); 
Btl:
   if ( !strstr(cr_ReqFld,"btl") )  /* beetle required */ 
      goto Dam;                     /* Nope */               
   if ( !stricmp (a->cr_BeeDam, "Y"))   goto Dam;
   if ( !stricmp (a->cr_BeeDam, "N"))   goto Dam; 
   if ( !stricmp (a->cr_BeeDam, "yes")) goto Dam;
   if ( !stricmp (a->cr_BeeDam, "no"))  goto Dam; 
   sprintf (A,"Invalid/Missing Beetle Attack Code: %s (use Y/N  Yes/No )\n",a->cr_BeeDam); 
   strcat (cr,A); 

Dam:
   if ( a->f_CrnScoPer < 0 || a->f_CrnScoPer > 100.0 )  /* Damage code is always required */
     MisValCat (a->f_CrnScoPer, "Crown Scorch/Damage Percent","%4.0f", cr);    

   if ( a->f_CKR < 0 || a->f_CKR  > 4.0 )     /* cambium Kill is always required */
     MisValCat (a->f_CKR, "Cambium Kill Rating", "%4.0f", cr); 

   goto Ret; 

/*.................................................................*/
/* Check Bole Char equation inputs */
BolCha:
/* Bole Char equations always required DBH and Bole Char Height */
   if ( stricmp (a->cr_EquTyp, e_BolCha ) ) 
     goto LogErr;

   if ( a->f_BolChaHgt <= 0 || a->f_BolChaHgt > 50 ) 
     MisValCat (a->f_BolChaHgt, "Bole Char Height", "%3.1f", cr); 

   if ( a->f_DBH <= 0 || a->f_DBH > 500.0) 
     MisValCat (a->f_DBH,"DBH", "%4.1f",cr); 
 
   goto Ret;  

LogErr:
  strcpy (cr, "\n\nLogic Error - bad equ typ\n\n");

Ret: 
  i = strlen (cr);
  if ( i > 0 )   /* if there's something in the string */
    return 0;    /* it's an erro message(s) */
  return 1; 
}


/*****************************************************
* Name: MisValCat
* Desc: Make error error message and Append
*****************************************************/
void MisValCat (float f, char Name[], char Fmt[], char cr[])
{
int i; 
char A[200];
   i = strlen(cr);
   if ( f < 0 ) { 
     sprintf (&cr[i], "Missing %s\n",Name);
     return; }
   sprintf (A, "Invalid %s: %s\n",Name, Fmt); 
   sprintf (&cr[i],A, f);
}

/*******************************************************
* Name: _MakErrMes
* Desc: Form plot error message
* NOTE: cr[] contains the actual error messages and will
*        get append to the end of the Plot data
*******************************************************/
void _MakErrMes (d_FFItre *a, int Row, char cr[])
{
char A[500],B[300],C[300],D[2000]; 

/* format the Plots data */
   sprintf (A, "\n>>----> Plot: %s-%s-%s, Row: %d, Spe: %s, Den: %4.1f\n", a->cr_Plot, a->cr_MonSta, a->cr_MonOrd, Row,a->cr_Spe, a->f_Den);

   sprintf (B, "          Dia:%4.1f, TreHgt:%4.1f, CrnBas:%4.1f, TreSta: %s, CrnClass: %s, CrnRatio: %d\n",
                  a->f_DBH, a->f_Hgt, a->f_CrnHgt, a->cr_Status, a->cr_CrnCls, a->i_CrnRat);
 
   sprintf (C, "          CharHgt: %3.1f, CrnScoPer: %3.1f,  CrnScoHgt: %3.1f,CKR: %2.0f, BtlDam: %s, EquaTyp: %s, FSH: %s, FS: %4.1f, Sev: %s\n",
                  a->f_BolChaHgt, a->f_CrnScoPer, a->f_CrnScoHgt, a->f_CKR, a->cr_BeeDam, a->cr_EquTyp, a->cr_FS, a->f_FlaSco,a->cr_FirSev); 

   strcpy (D,cr);
   sprintf (cr,"%s%s%s%s", A,B,C,D); 
}

/*********************************************************/
int  Wnd_Mai::IEM_Open (char cr_FN[])
{
char cr[1000]; 

/* Save Path File Name of error file to display with NotePad later */ 
  this->s_IEM_FN = _CharToStr(cr_FN);
 
  this->fh_Err = fopen (cr_FN,"w"); 
  if ( this->fh_Err == NULL ) {
    sprintf (cr, "Can't Open Output Error File\n %s\n",cr_FN); 
    myMessageBox (cr); 
    return 0; }

/* Put this at the top of the file */
  fprintf (this->fh_Err, " File Location: %s\n",cr_FN); 
  fprintf (this->fh_Err, " Some missing numeric values may be shown as '-1'\n");
  fprintf (this->fh_Err, " Row number represents file record within selected plot\n\n"); 
  return 1; 
}

/*******************************************************
*
*******************************************************/
void Wnd_Mai::IEM_Close()
{
   if ( this->fh_Err == NULL ) 
     return;  
   fclose (this->fh_Err); 
   this->fh_Err = NULL; 
}
/*****************************************************
*
******************************************************/
void Wnd_Mai::IEM_Add ( char cr[])
{
String ^ Str; 

// this saves messages in a big long string, I was using to 
//  test 
//   Str = _CharToStr (cr);
//  this->s_IEM = this->s_IEM + Str; 

   fprintf (this->fh_Err,cr); 
}
/*****************************************************
* Name: IEM_Display
* Desc: Display the FFI Error File
*       errors when loading Tree Grid from FFI 
*       .tre tree file
*****************************************************/
int Wnd_Mai::IEM_Display ()
{
bool b; 
String ^ s_Arg; 
System::Diagnostics::Process ^reg;

/* Make sure there's an error file, user may not have */
/* done a run to create one yet */
  if ( this->s_IEM_FN == "" ) {
    myMessageBox ("No FFI Error File Found");
    return 0; }

  s_Arg = this->s_IEM_FN;
  reg  = gcnew System::Diagnostics::Process;
  reg->StartInfo->FileName = "notepad.exe";
  reg->StartInfo->Arguments = s_Arg;
  reg->StartInfo->UseShellExecute = false ; 
  reg->StartInfo->CreateNoWindow = true; 
  reg->StartInfo->RedirectStandardOutput = true; 
  b = reg->Start(); 
  if ( b == false ){ 
    myMessageBox ("Can't Open Error File");
    return 0; }
  return 1; 
}

/*****************************************************************
* Name: FFI_ChkTreeData
* Desc: 
*  Ret: 1 error,  0 no error
*****************************************************************/
int Wnd_Mai::FFI_ChkTreeData(float f_TPA, float f_DBH, float f_Hgt, float f_HgtCrn, int i_CrnRat)
{

  if ( f_TPA <= 0 ) 
    return 1; 
  if ( f_DBH <= 0 )
    return 1; 

  if ( f_Hgt <= 0 )
    return 1; 

/* We need a crown ratio, but we'll settle for crown height, */
/*  or else we can't use the recoord */

  if ( i_CrnRat >= 0 ) 
     goto OK;               /* we're good to go */

  if ( f_HgtCrn < 0 )
    return 1;               /* bad news, no crn rat or crn height */

  if ( f_Hgt <= f_HgtCrn )  /* make sure tree hgt is > than crn hgt */
     return 1; 

OK:
  return 0; 
}
 
/*******************************************************************************
* Name: FFI_LoadPlotNames
* Desc: Load the FFI Plot Names from a tree file into GUI combobox
*       Tree file contains multiple records per Plot-MonStat-MonOrd
*         but we just need a list of unique ones 
*   In: cr_FilNam...if NULL, function pops up a get file window
*  Ret: 1 ok file open, else 0 error 
*******************************************************************************/
int Wnd_Mai::FFI_LoadPlotNames(char cr_FNin[])
{
int i,j, x, iN;
FILE *fh;
String ^Str, ^Plt, ^MS, ^MO; 
char cr[1000];
char cr_ErrMes[1000]; 
char cr_FN[500];
char *adr, *ac;  
d_CDF *a;
String ^ s_FilNam;
String ^ s_Plt;

System::Windows::Forms::OpenFileDialog FD;

// #define e_FFIfil  "ffi files (*.ffi)|*.ffi|All files (*.*)|*.*"

#define e_FFIfil  "tre files (*.tre)|*.tre|All files (*.*)|*.*"

   if ( strcmp (cr_FNin,"" ) ) {
     strcpy (cr_FN, cr_FNin); 
     s_FilNam = _CharToStr (cr_FN); 
     goto GotFN; }

/* Put up an OpenFileDialog Box, and use the Path that is in the Project File */
/*  TextBox  */
  FD.Title = "Open FFI File"; 
  FD.InitialDirectory = this->Prj_GetPath();  
  FD.FileName = ""; 
  FD.Filter = e_FFIfil;
  i = (int) FD.ShowDialog(); 
  if ( i != 1 )           /* User Canceled out out Dialog Box */
    return 0; 
  s_FilNam = FD.FileName;  /* Get Path + File Name */
 _StrToChar (s_FilNam,cr_FN);

GotFN:
  strcpy (this->cr_FFI_FN,cr_FN);  /* Save FFI File Name */ 
  this->_txFFI_File->Text = s_FilNam;  

/*-----------------------------------------------*/
   a = &s_CDFTree[0];
 
   fh = fopen (cr_FN, "r");
   if ( fh == NULL ) {
     return 0; }

  this->_cbFFI_Plots->Items->Clear();   /* Clear out Plot #s in ComboBox */

/* Read in the required Column Heading row, it must preceed and tree data rows */
/* it could be before, after or inbetween comment or blank rows, */
/* the first column in the heading row must read 'PlotId' */
   j = CDF_Get ( a, fh, cr_ErrMes); 
   if ( stricmp (s_ft.cr_Plot,e_PlotId) ) {
     strcpy (cr_ErrMes, "File does not have the required row of column heading information.\n");
     strcat (cr_ErrMes, "The row of column headings must preceed and tree data.\n");
     strcat (cr_ErrMes, "The first column heading must be 'PlotId'\n"); 
     return -1; }
     
  iN = 0; 
  s_Plt = "";
  for ( i = 0; i < 5000; i++ ) {

     j = CDF_Get ( a, fh, cr_ErrMes); 
     if ( j == 0 ) 
       break;      /* end of file */
     if ( j == -1 || strcmp (cr_ErrMes,"") ) {
       sprintf (cr, "Error Reading FFI Plot File: %s ", cr_ErrMes); 
       myMessageBox (cr); 
       break; }

     Plt = _CharToStr (s_ft.cr_Plot);  /* Plot */
     MS = _CharToStr (s_ft.cr_MonSta); /* Monitor Status */
     MO = _CharToStr (s_ft.cr_MonOrd); /* Monitor Order Number */
     Str = Plt + e_PltSep + MS + e_PltSep + MO; 

/* See Note-1 above  */
     if ( Str != s_Plt ) { 
       this->_cbFFI_Plots->Items->Add (Str);   
       s_Plt = Str; }

     iN++; 
   }  /* for i */ 

  fclose (fh); 


   if ( iN > 0 ) {   
     this->_cbFFI_Plots->SelectedIndex = 0; 
     this->_cbFFI_Plots->Enabled = true;  }  /* Make Combobox active */ 

 
  return 1; 
}

/******************************************************************
* Name: FFI_LoadCVT
* Desc: Read in FFI plot file and load 
* Note-1: First line of file has column headings
*  In: cr_PthFN.... Path and file name of FFI input plot file
******************************************************************/
 int Wnd_Mai::FFI_LoadCVT(char cr_PthFN[], char cr_ErrMes[])
{
int i,j, iN, iX, i_Err;
float f, f_Snd, f_Rot; 
FILE *fh ;
#define eC_line 300
char cr_Line [eC_line], cr[2000], cr_Err[2000];
char  cr_CovTyp[200];
// char cr_Pth[2000];
#define  eO 20
 String ^ Str, ^Plt, ^MS; 
d_CDF *a;

   strcpy (cr_ErrMes,""); 

 //  if ( ! FFI_GetPthFN(cr_PthFN) )
 //    return -1; 
    
   iN = 0; 
   strcpy (cr_ErrMes,"");    

   fh =  fopen (cr_PthFN, "r" );
    if ( fh == NULL ) {
      sprintf (cr_ErrMes,"Can't Open FFI input plot file: %s\n", cr_PthFN);
      return 0;}

   fgets (cr, 1000, fh);    /* See Note-1 above */

   SAF_InitTable();                         /* Init the table               */
 
   a = &s_CDFPlot[0];   /* Describes FFI Plot file colmn layout */
   iX = 0;
 
  for ( i = 0; i < 5000; i++ ) {
     j = CDF_Get ( a, fh, cr_Err); 
     if ( j == 0 ) 
       break;      /* end of file */
     if ( j == -1 ) {
       sprintf (cr_ErrMes, "Error Reading FFI Plot File.\n %s ", cr_Err); 
       fclose (fh); 
       return 0; }

     Plt = _CharToStr (s_fP.cr_Plot); 
     MS = _CharToStr (s_fP.cr_MonSta); 
     Str = Plt + e_PltSep + MS; 
     this->_cbFFI_Plots->Items->Add (Str);   
  
     sr_CVT[iX].ir_Region[0] = 1;
     sr_CVT[iX].ir_Region[1] = 2;
     sr_CVT[iX].ir_Region[2] = 3;
     sr_CVT[iX].ir_Region[3] = 4;

     sprintf (cr, "%s%s%s", s_fP.cr_Plot,e_PltSep,s_fP.cr_MonSta);
     strcpy (sr_CVT[iX].cr_CovTyp,cr); 

     sr_CVT[iX].s_CFL.f_Duff   = _csMiss (s_fP.f_Duff, &i_Err);
     sr_CVT[iX].s_CFL.f_DufDep = _csMiss (s_fP.f_DufDep, &i_Err); 

     s_fP.f_Grass = _Sum (s_fP.f_GrassDead, s_fP.f_GrassLive, 0, 0 );
     sr_CVT[iX].s_CFL.f_Herb = s_fP.f_Grass; 
 
     s_fP.f_Shrub = _Sum (s_fP.f_ShrubLive, s_fP.f_ShrubDead, 0, 0 ); 
     sr_CVT[iX].s_CFL.f_Shrub = s_fP.f_Shrub;

     sr_CVT[iX].s_CFL.f_litter = _csMiss (s_fP.f_Litter, &i_Err);
/* DL said put Moss and Lichen into Litter */
     s_fP.f_Moss  = _Sum (s_fP.f_MossLive,  s_fP.f_MossDead, 0, 0 );
     sr_CVT[iX].s_CFL.f_litter += s_fP.f_Moss;
     sr_CVT[iX].s_CFL.f_litter += _csMiss (s_fP.f_Lichen, &i_Err); 

     sr_CVT[iX].s_CFL.f_DW0    = _csMiss (s_fP.f_1Hr, &i_Err);
     sr_CVT[iX].s_CFL.f_DW10   = _csMiss (s_fP.f_10Hr, &i_Err);
     sr_CVT[iX].s_CFL.f_DW100  = _csMiss (s_fP.f_100Hr, &i_Err);

/* Do individual 3+ Siz Cls snd & rot into s_CFL, we'll use the individual */
/* amounts when we calc consumed */
     Sum1k (&s_fP, &sr_CVT[iX].s_CFL, &f_Snd, &f_Rot);
 
     sr_CVT[iX].s_CFL.f_pcRot = _SndRotPer (f_Snd,f_Rot);
     sr_CVT[iX].s_CFL.f_DW1000 = f_Snd + f_Rot; 

/* Set some default emission group codes, FFI doesn't these codes in the file */
/* these codes won't even matter until we start using the new emission stuff  */
/* but set them anyway to avoid getting a warning message when GUI checks */
     strcpy (sr_CVT[iX].s_CFL.cr_EmiGrpDuf, s_fP.cr_EmisDuffRSC); 
     strcpy (sr_CVT[iX].s_CFL.cr_EmiGrpFla, s_fP.cr_EmisSTFS);
     strcpy (sr_CVT[iX].s_CFL.cr_EmiGrpS1k,s_fP.cr_EmisCWDRSC); 

     iX++; 
     iN++; 
  }
  fclose (fh); 
  return 1; 
} 

/*******************************************************************
* Name: _Sum
* Desc:  sum up the 4 value as long as it's not a missing value
*******************************************************************/
float  _Sum (float D2, float D3, float D4, float D5 )
{
float f;
   f = 0;

/* if it's 0 it doesn't matter if we add it */
/* a Missing value is a -1 I think */
/* we shouldn't get any other neg, if do just ignore them */
   if ( D2 > 0 )   
     f += D2;

   if ( D3 > 0 )
     f += D3;

   if ( D4 > 0 )
     f += D4;

   if ( D5 > 0 )
     f += D5;

   return  f;
}




/******************************************************
* Name: _csMiss
* Desc: Check for a missing/negative value
*  Out: ai = 0 Missing/neg value found, else 1
*  Ret: Value or 0 if missing
*******************************************************/
float _csMiss(float f, int *ai)
{
  *ai = 1; 
  if ( f >= 0 ) 
    return f;
  *ai = 0; 
  return 0;
}


/*****************************************************
* Name: _SndRotPer
* Desc: calc what percent of the total 1000 hr DW
*        is rotten 
*****************************************************/
float _SndRotPer (float f_Snd, float f_Rot)
{
float f, f_Tot;
  if ( f_Snd == 0 && f_Rot == 0 ) 
     return 0; 

  if ( f_Rot == 0 )  /* No Rot so Rot is 0 percent of DW */ 
     return 0; 

  if ( f_Snd == 0 )  /* no snd so all wood is rotten */
     return 100.0;     /* 100 percent of DW is rotten */

  f_Tot = f_Snd + f_Rot;
  f = f_Rot / f_Tot ;  /* have snd and rot so tak a percent */
  f = f * 100.0;
  return f; 

}

/*****************************************************
* Name: FFI_GetPthFN
* Desc: Open a File DialogBox for getting an FFI
*        plot file name.
* Out: cr_PthFN...selected path file name
* Ret: 1 User selected a file, else 0
*****************************************************/
int Wnd_Mai::FFI_GetPthFN(char cr_PthFN[])
{
int i; 
String ^ s_FilNam;
System::Windows::Forms::OpenFileDialog FD;

#define e_FFIfil  "ffi files (*.ffi)|*.ffi|All files (*.*)|*.*"

  strcpy (cr_PthFN,""); 

/* Put up an OpenFileDialog Box, and use the Path that is in the Project File */
/*  TextBox  */
  FD.Title = "Open FFI File"; 
  FD.InitialDirectory =  this->Prj_GetPath ();  /* Project Path from TextWnd Main screen */
  FD.FileName = ""; 
  FD.Filter = e_FFIfil;
  i = (int) FD.ShowDialog(); 
  if ( i != 1 )           /* User Canceled out out Dialog Box */
    return 0; 
  s_FilNam = FD.FileName;  /* Get Path + File Name */
 _StrToChar (s_FilNam,cr_PthFN);

  return 1; 
}

/****************************************************************
* Name: Sum1k
* Desc: Sum up total Sound and total rotten
*       Sound = Decay class 1, 2, 3
*       Rotten = Decay class 4, 5  
****************************************************************/
void Sum1k (d_FFIPlt *a_fP, d_CFL *a_CFL, float *af_Snd, float *af_Rot)
{
   a_fP->f_SC1 =  _Sum (a_fP->f_Dc1Sz1, a_fP->f_Dc2Sz1, a_fP->f_Dc3Sz1, 0);
   a_fP->f_SC2 =  _Sum (a_fP->f_Dc1Sz2, a_fP->f_Dc2Sz2, a_fP->f_Dc3Sz2, 0);
   a_fP->f_SC3 =  _Sum (a_fP->f_Dc1Sz3, a_fP->f_Dc2Sz3, a_fP->f_Dc3Sz3, 0);
   a_fP->f_SC4 =  _Sum (a_fP->f_Dc1Sz4, a_fP->f_Dc2Sz4, a_fP->f_Dc3Sz4, 0);
   a_fP->f_SC5 =  _Sum (a_fP->f_Dc1Sz5, a_fP->f_Dc2Sz5, a_fP->f_Dc3Sz5, 0);

/* Take the FFI decay class 4 & 5 and sum them into My Rotten size classes */
   a_fP->f_RC1 = _Sum (a_fP->f_Dc4Sz1, a_fP->f_Dc5Sz1, 0, 0 );
   a_fP->f_RC2 = _Sum (a_fP->f_Dc4Sz2, a_fP->f_Dc5Sz2, 0, 0 );
   a_fP->f_RC3 = _Sum (a_fP->f_Dc4Sz3, a_fP->f_Dc5Sz3, 0, 0 );
   a_fP->f_RC4 = _Sum (a_fP->f_Dc4Sz4, a_fP->f_Dc5Sz4, 0, 0 );
   a_fP->f_RC5 = _Sum (a_fP->f_Dc4Sz5, a_fP->f_Dc5Sz5, 0, 0 );

   *af_Snd = a_fP->f_SC1 + a_fP->f_SC2 + a_fP->f_SC3 + a_fP->f_SC4 + a_fP->f_SC5; 
   *af_Rot = a_fP->f_RC1 + a_fP->f_RC2 + a_fP->f_RC3 + a_fP->f_RC4 + a_fP->f_RC5;

   a_CFL->f_Snd3  = a_fP->f_SC1;
   a_CFL->f_Snd6  = a_fP->f_SC2;
   a_CFL->f_Snd9  = a_fP->f_SC3;
   a_CFL->f_Snd12 = a_fP->f_SC4;
   a_CFL->f_Snd20 = a_fP->f_SC5;

   a_CFL->f_Rot3  = a_fP->f_RC1;
   a_CFL->f_Rot6  = a_fP->f_RC2;
   a_CFL->f_Rot9  = a_fP->f_RC3;
   a_CFL->f_Rot12 = a_fP->f_RC4;
   a_CFL->f_Rot20 = a_fP->f_RC5;
}




