/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Wnd_RepMrt.cpp
* Desc: New version of Mortality, 
*       All 3 mortality equation types are incorperated into the 
*       GUI mortality species grid and mortality report(s)
* Date: 2/23/2018
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/


#include <windows.h>

#include <stdio.h>
#include <math.h>
#include <string.h>


#include "Wnd_Mai.h"
#include "wnd_util.h"
#include  "fof_util.h"
#include "cvt_cvt.h" 
#include  "fof_smt.h"
#include  "fof_iss.h"
#include  "fof_mrt.h"
#include  "fof_slk.h"
#include  "fof_st.h"
#include  "fof_mt.h"
#include  "fof_mec.h" 
#include "fof_mrt.h"
#include "fof_iss.h" 
#include "fof_mis.h"
#include "fof_pst.h" 

/* Probability Size Class Table */ 
d_PST sr_PST[eC_PST]; 


#define  e_RepSep2 "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"

float _gf_ScoHgt;

extern d_MIS sr_MIS[];

int Put_Prob (int ix, TextBox^ a, TextBox^ b, TextBox^ d, char cr_ErrMes[]);


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: TreeGrid_SetProb
* Desc: Fill in the Probability of Mortality column of the tree grid
*       fills in each row.  
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int Wnd_Mai::TreeGrid_SetProb()
{
int i,n;
float f_Prob;
char  cr_ErrMes[1000], cr[20]; 
String ^ Str; 
d_MIS MIS; 
d_MO MO; 

/* Load the GUI Mort Spe Grid into MIS table */
   n = MIS_Load(cr_ErrMes);
   if ( n <= 0 ) {
     myMessageBox (cr_ErrMes);
     return 0; }

   for ( i = 0; i < n; i++ ) {   /* Each row in the tree grid */ 
      MIS_Get(i, &MIS);          /* Get row of data */ 
      f_Prob = MRT_CalcMngr  (&MIS, &MO, cr_ErrMes);
      sprintf (cr, "%4.2f",f_Prob);
      Str = _CharToStr(cr); 
      this->_dgMort->Rows[i]->Cells [edg_Prob]->Value = Str; } 

   return 1; 
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: MRS_ReportNew
* Desc: 
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int Wnd_Mai::MIS_ReportNew()
{
int i, iN, i_RT;
float f, f_FS,f_RelHum;
char cr[500], cr_ErrMes[1500], cr_Out[100]; 
char cr_TotNam[eC_TotNam], cr_Date[30], cr_CP[40];

d_MO  s_MO;
String ^ Str; 

   MO_Init (&s_MO);
   i_RT = e_RTpre;   /* Assume doing a regular (non-salvage) report */

/* Load the GUI Mort Spe Grid into MIS table */
   i = MIS_Load(cr_ErrMes);
   if ( i <= 0 ) {
     myMessageBox (cr_ErrMes);
     return 0; }

/* If Salvage Report Checkbox is selected, Load GUI Prob Cutoff Size Class info */
/*  need to do this here before we start outputing the report */
   if ( this->_cb_SalRep->Checked ) { 
     if ( !this->Load_Probability(cr_ErrMes) ){  /* Load GUI prob size class to PST table */
        myMessageBox (cr_ErrMes); 
        return 0;}  
     MIS_SetProbCutOff();        /* set the Prob Siz Cls into MIS table*/
     i_RT = e_RTsal;                  /* Salvage Type Report */ 
   } 

/* Get Total's Name if there is one */
   _GetTextBox (this->_txRepTot,cr_TotNam);
   if ( stricmp (cr_TotNam,"") ) {                /* if have a Total Name         */
     if ( MS_isTotNam(cr_TotNam,i_RT)) {     /* is it already in use         */
       myMessageBox("Please change or clear the Total name. That one has already been used.");
       return 0; }  }     

/* Are any Crown Scorch Type equations set to be used */
   i = MIS_EquTyp(e_CroSco); 
   if ( i == 0 )  
     goto Skip;

/* Get the Flame Len - Scorch Hgt */
   if ( !_Get_TBFlo (this->_txFlaSco, &f_FS, cr_Out, "Flame/Scorch", cr_ErrMes) ) {
     myMessageBox ("Numeric value is required for 'Flame Length - Scorch Height - MSR_Mngr()");
     return 0; }

   if ( this->_rbScorchHeight->Checked ) {                  /* usr has specified Scrch Hght  */
     if ( f_FS < e_ScoLow || f_FS > e_ScoHig) {
       sprintf (cr_ErrMes,"Scorch Height is out of %4.0f -> %4.0f range ", e_ScoLow,e_ScoHig);
       myMessageBox (cr_ErrMes);
       return 0; } }
   else {
     if ( f_FS < e_FlaLow || f_FS > e_FlaHig ) {
       sprintf (cr_ErrMes,"Flame Length is out of %4.0f -> %4.0f range ", e_FlaLow, e_FlaHig);
       myMessageBox (cr_ErrMes);
       return 0; } }

   if ( this->_rbScorchHeight->Checked ) {  /* usr has specified Scrch Hght  */
     _gf_ScoHgt = f_FS;     }
   else if ( this->_rbFlameLen->Checked )   /* usr has specified Scrch Hght  */
     _gf_ScoHgt = Calc_Scorch (f_FS);        /* Calc Scorch Height from Flam  */
   else {                                   /* Must be Flam Len so...        */
     myMessageBox ("Please set 'Flame Length' or 'Scorch Height' Button! - MSR_Mngr");
     return 0; }

Skip:

   _rep ( "\n\n");
   _rep ( e_RepSep2);
   GetDate (cr_Date);
   sprintf (cr, " TITLE: Results of FOFEM model execution on date: %s\n\n",cr_Date);
   _rep (cr);

   if ( this->_cb_SalRep->Checked == true )      /* Salvage Report Checkbox */ 
     _rep (" SALVAGE REPORT:\n\n");                   
   else
     _rep (" TREE MORTALITY MODULE:\n\n");

   Str = _GetComboBox (this->_cbRegion,cr_Out); 
   sprintf ( cr,  " REGION: %s\n",cr_Out);
   _rep (cr); 


/* Are any Crown Scorch Type equations set to be used */
   i = MIS_EquTyp(e_CroSco); 
   if ( i == 0 )  
     goto Skip2;

   if ( this->_rbFlameLen->Checked ) {                  /* Did user enter a Flam Len    */
     _rep (   "\n");
     sprintf (cr,   " FLAME LENGTH (FT):%6.2f\n",f_FS);
     _rep (cr); 
     f = Calc_Scorch (f_FS);
     sprintf (cr,   "     Scorch Height:%6.2f\n",f);
     _rep (cr);  }
   else {                                    /* user enter a Scorch Height   */
     _rep (   "\n");
     sprintf (cr,   " SCORCH HEIGHT (FT):%6.2f\n",f_FS);
     _rep (cr); 
     f = Calc_Flame (f_FS);                  /* Calc Flame Length from Scorch */
     sprintf (cr,   "       Flame Length:%6.2f\n",f);
     _rep (cr);   }

Skip2: 

/* Salvage Report if Checkbox is selected, See above where we load GUI info  */
   if ( this->_cb_SalRep->Checked ) { 
     this->SalvageReport(cr_TotNam);            /* do report */ 
     return 1;  } 

   Load_SLK ();                              /* Set Mort Live-Killed totals  */

   if ( SLK_ifGroup (1) )  MSR_Density (1); 
   if ( SLK_ifGroup (2) )  MSR_Density (2); 

   if ( SLK_ifGroup (1) )  MSR_PostFire_Killed  (1,"LIVE");    
   if ( SLK_ifGroup (2) )  MSR_PostFire_Killed  (2,"LIVE");  

   if ( SLK_ifGroup (1) )  MSR_PostFire_Killed  (1,"KILLED"); 
   if ( SLK_ifGroup (2) )  MSR_PostFire_Killed  (2,"KILLED");

   MSR_ProbMort ();
   MSR_Avg_Mort ();
   MSR_Mort_Indexes();
 
   return 1; 
}

/**********************************************************
* Name: MIS_Load
* Desc: Load the MIS (Mortality Inputs) table from the 
*       GUI Mortality Species Grid
* NOTE: See MIS_Init() how the tables is marked for end of 
*       table. 
* Note-1: Put Flame Len - Scorch Height and Fire Severity 
*          in every record to make it more convenient to 
*          pass around. 
* Note-2 This will commit any data entered in the grid even if the 
*         user didn't hit the Enter key.
*  Ret: Number of records loaded into table
*       -1  no records loaded
*        0  error found 
***********************************************************/
int Wnd_Mai::MIS_Load(char cr_ErrMes[])
{
int i, r;
float f_Den, f_FS; 
bool b;  
char cr_Out[400], cr_Err[500], cr_Spe[100], cr_FirSev[10], cr_ErrMesFS[500];
char  cr_FS[20];
String ^ Str; 

   this->_btSelect->Focus() ;  /* See Note-2 above */

   strcpy (cr_ErrMesFS,""); 
   strcpy (cr_ErrMes,""); 
   MIS_Init(); 

/* Flame - Scorch checkbox */
  if (  this->_rbFlameLen->Checked == true )
    strcpy (cr_FS,"F"); 
  else 
    strcpy (cr_FS,"S"); 

/* Flame - scorch value  */
  if ( !_Get_TBFlo (this->_txFlaSco, &f_FS, cr_Out, "Flame/Scorch", cr_ErrMes))
     f_FS = 0;   /* if we get an error, we deal with it below  */ 

/* Fire Severity */
   if ( this->_chLowFireSev->Checked == true ) 
     strcpy (cr_FirSev,"Low");
   else
     strcpy (cr_FirSev,""); 

/*........................................................*/
/*  each entry in the Species Data Grid */  
   r = this->_dgMort->RowCount - 1;

   for ( i = 0; i <= r; i++ ) {
     Str = this->_dgMort->Rows[i]->Cells[edg_Spe]->FormattedValue->ToString();
     if ( Str == "" && i == r ) 
        break; 
     _StrToChar (Str,sr_MIS[i].cr_Spe);

     Str = this->_dgMort->Rows[i]->Cells[edg_Equ]->FormattedValue->ToString();
     _StrToChar (Str,sr_MIS[i].cr_EquTyp);

/* Beetle Damage */
     Str = this->_dgMort->Rows[i]->Cells[edg_BeeDam]->FormattedValue->ToString();
     if ( Str == "" ) 
        Str = e_BeeDamNA; 
     _StrToChar (Str,sr_MIS[i].cr_BeeDam);

     sr_MIS[i].b_Graph = this->_dgMort->Rows[i]->Cells[edg_Gra]->Selected;

     _Get_DGFlo (this->_dgMort, i, edg_Den, &sr_MIS[i].f_Den, cr_Out, "Density", cr_Err);
     _Get_DGFlo (this->_dgMort, i, edg_DBH, &sr_MIS[i].f_DBH, cr_Out, "DBH", cr_Err);
     _Get_DGFlo (this->_dgMort, i, edg_Hgt, &sr_MIS[i].f_Hgt, cr_Out, "Height", cr_Err);
     _Get_DGFlo (this->_dgMort, i, edg_CR, &sr_MIS[i].f_CR, cr_Out, "C/R Crown Ratio", cr_Err);
     _Get_DGFlo (this->_dgMort, i, edg_CrnDam, &sr_MIS[i].f_CrnDam, cr_Out, "Crn Dam %", cr_Err);
     _Get_DGFlo (this->_dgMort, i, edg_CKR, &sr_MIS[i].f_CKR, cr_Out, "Cambium Kill Ratio", cr_Err);
     _Get_DGFlo (this->_dgMort, i, edg_BolCha, &sr_MIS[i].f_BolCha, cr_Out, "Bole Char Height", cr_Err);
   
/* Set Note-1 above */
      strcpy (sr_MIS[i].cr_FS,cr_FS);           /* Flame len - scorch hgt */
      sr_MIS[i].f_FS = f_FS;
      strcpy (sr_MIS[i].cr_FirSev, cr_FirSev);  /* Fire Severity */

/* Graphing Checkbox */ 
      Str = this->_dgMort->Rows[i]->Cells[edg_Gra]->FormattedValue->ToString();
      if ( Str == "True" )
        sr_MIS[i].b_Graph = true;
      else 
        sr_MIS[i].b_Graph = false; 
 
  }  /* for i */ 


  if ( i == 0 ) {
    strcpy (cr_ErrMes, "No Species Entered"); 
    return -1; }

  i = MIS_EquTyp(e_CroSco);  /* do we have any Crown Scorch Equations being used */
  if ( i == 0 )              
    goto SkpFS;              /* Nope, so we don't care if we got an FS error above */

  if ( strcmp (cr_ErrMesFS,"" )) {    /* if we had and FS error above */
    strcpy (cr_ErrMes,cr_ErrMesFS);
    return 0; }

SkpFS: 

  i = MIS_Check(cr_ErrMes);
  if ( i == 0 ) 
    return 0; 

   return r; 
} 

/**********************************************************
* Name: Load_Probability
* Desc: Load the Probability Size Class Table from the 
*       GUI
* Note-1: The user can enter 1,2 or 3 size classes 
*         each of the 3 classes have a low, upper limit 
*         and the probability cutoff,
*         If any entry in a single size class is left blank 
*         then it will not get used, this gives the user
*         a quick way to include/exclude the size class   
**********************************************************/
int Wnd_Mai::Load_Probability (char cr_ErrMes[])
{
int i; 
  
  strcpy (cr_ErrMes,""); 
  PST_Init();

/* See Note-1 above  */
  if ( !Put_Prob(0, this->_txSC1A, this->_txSC1B, this->_txSC1C, cr_ErrMes))
    return 0;  

  if ( !Put_Prob(1, this->_txSC2A, this->_txSC2B, this->_txSC2C, cr_ErrMes))
    return 0; 
 
  if ( !Put_Prob(2, this->_txSC3A, this->_txSC3B, this->_txSC3C, cr_ErrMes))
    return 0; 
 
  if ( !PST_Check() ) {
    strcpy (cr_ErrMes, "Invalid or Missing data entered for Size Class Probability Cutoff\n");  
    strcat (cr_ErrMes, "Make sure Size Classes:\n");
    strcat (cr_ErrMes, " > do not overlap\n"); 
    strcat (cr_ErrMes, " > entered size classes are ordered smallest to the left\n");  
    strcat (cr_ErrMes, "Click the Salvage Report Checkbox if classes are not currently displayed");       
    return 0; }

  return 1; 
}

/**************************************************************
* Name: Put_Prob
* Desc: Put a single entry from the Prob Cutoff Size Class GUI
*       into the PST table

***************************************************************/
int _IsItBlank (char cr[]);


int Put_Prob (int ix, TextBox^ a, TextBox^ b, TextBox^ c, char cr_ErrMes[])
{
int i;
float low, up, cut; 
char cr[100];  /* See Note-1 above */ 
char AA[500], BB[500], CC[500]; 

//char A[500], B[500], C[500]; 

 
  strcpy (cr_ErrMes,""); 
 _Get_TBFlo (a, &low, cr, "Size Class lower limit", AA);
 _Get_TBFlo (b, &up,  cr, "Size Class upper limit", BB);
 _Get_TBFlo (c, &cut, cr, "Size Class Probability cutoff", CC);

/* Check for blank data entered */
/* Must all be filled in or all blank */
  i = 0; 
  i += _IsItBlank (AA);
  i += _IsItBlank (BB);
  i += _IsItBlank (CC);
  if ( i == 3 ){          /* All blank - ok */
     PST_Put(ix, -1,-1,-1); 
     return 1; }
  if ( i > 0 ) {
     strcpy (cr_ErrMes, "Invalid Probability Size Class Input\n");
     strcat (cr_ErrMes, "Fill in or blank out all 3 values"); 
     return 0; }

/* Check if we had any Error Messages */ 
  if ( strcmp(AA,"") ){
     strcpy (cr_ErrMes,AA); 
     return 0;  }
 
  if ( strcmp(BB,"") ){
     strcpy (cr_ErrMes,BB);
     return 0;  }

  if ( strcmp(CC,"") ){
     strcpy (cr_ErrMes,CC); 
     return 0;  }

/* OK, put the entry into the PST table */
  PST_Put(ix, low,up,cut); 

  return 1; 
}

/*****************************************************
* Error message contains 'blank' if the textbox input
* was blank, else 
*  Ret: 1...means the input was blank
*     
*****************************************************/
int _IsItBlank (char cr[])
{
   if ( strstr (cr,"blank") )
     return 1;
   return 0; 
}

/********************************************************************
* Name: MIS_AvgDBHKill
* Desc: for the entire table get the average DBH of the killed 
*        trees
* NOTE: there is one entry in the table for every tree species the
*        user entered in Mortality GUI tree grid
********************************************************************/
float MIS_AvgDBHKill ()
{
int i;
char cr_ErrMes[1000]; 
float f, f_DBH, f_Kill, killed;
d_MO s_MO; 

   f_DBH = f_Kill = 0; 
 
   for ( i = 0; i < eC_MIS; i++ ) {
     if (!strcmp (sr_MIS[i].cr_Spe,e_EndTbl) ) 
        break; 
     killed = MIS_Killed (&sr_MIS[i]); 
     f_DBH  += sr_MIS[i].f_DBH * killed;
     f_Kill += killed;
   }

   if ( f_Kill != 0 ) 
      f = f_DBH / f_Kill;  
   else 
      f = 0; 
   return f; 
}

/********************************************************************
* Name: MIS_BasalArea
* Desc: Get Basal Areas for entire table
*       Returned in Square Feet  
* NOTE: there is one entry in the table for every tree species the
*        user entered in Mortality GUI tree grid
********************************************************************/
int  MIS_BasalArea (float *af_Den, float *af_Live, float *af_Kill )
{
int i;
float Den, Live, Kill, killed, f_Live; 
   Den = Live = Kill = 0; 

   for ( i = 0; i < eC_MIS; i++ ) {
     if (!strcmp (sr_MIS[i].cr_Spe,e_EndTbl) ) 
        break; 
      Den  += Basal_Area (sr_MIS[i].f_DBH, sr_MIS[i].f_Den);                  /* Original Density */
      killed = MIS_Killed (&sr_MIS[i]);
      f_Live = sr_MIS[i].f_Den - killed;
      Live += Basal_Area (sr_MIS[i].f_DBH, f_Live); 
      Kill += Basal_Area (sr_MIS[i].f_DBH, killed);
   }
   *af_Den = Den / 144.0;     /* from sq/in to sq/ft */
   *af_Live = Live / 144.0;
   *af_Kill = Kill / 144.0;    
   return 1; 
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: MSR_Prob_Mort_Scorch
* Desc: Mortality Stand Report
*       Probability of Mortality section of report
*       
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Wnd_Mai::MSR_ProbMort()
{
int i;
float f_Sch, f_Prob, f, f_BaArTo, f_BaArKi;
float f_Killed, f_CC, f_TotCov;

char  cr_ErrMes[300], cr[300], cr_CKR[20]; 
char  cr_Id[30], cr_Name[100]; 
//d_ISS s_ISS;
d_MIS s_MIS; 
// d_MI s_MI;
d_MO s_MO;

   MO_Init (&s_MO);

   _rep (  "\n");
   _rep (  "\n\n");
   _rep (  "           PROBABILITY OF MORTALITY FOR EACH SPECIES/DIAMETER ENTRY\n");

/* See if there are any Crown Scorch equation types being used */
   i = MIS_EquTyp(e_CroSco); 
   if ( i == 0 )          /* None */
     goto CroDam ; 

   _rep (  "\n");
   _rep (  " Species    DBH   Height   Crn  Mort   Mortality Equation\n");
   _rep (  " Code      (inch) (feet)   Rat  Prob     Id       Name\n");
   _rep (  " ___________________________________________________________________________\n");

   for ( i = 0; i < 1000; i++ ) {            /* each spe selected by user    */
     if ( !MIS_Get (i, &s_MIS) )              /* get a struct                 */
        break;                               /*  all done                    */

     if ( stricmp ( s_MIS.cr_EquTyp,  e_CroSco) )
       continue; 

     f_Prob = MRT_CalcMngr  (&s_MIS, &s_MO, cr_ErrMes);
     this->Mort_GetName(s_MO.cr_MortEqu,cr_Name);
     sprintf (cr,  " %-7s   %5.1f    %3.0f     %1.0f   %4.2f     %s      %s\n",
         s_MIS.cr_Spe,  s_MIS.f_DBH, s_MIS.f_Hgt, s_MIS.f_CR, s_MIS.f_Prop, s_MO.cr_MortEqu,cr_Name);
     _rep (cr); 
   }  /* for i end */

/*-------------------------------------------------------------------------  */
CroDam:
/* See if there are any Crown Damage equation types being used */
   i = MIS_EquTyp(e_CroDam); 
   if ( i == 0 )          /* None */
     goto BolCha ; 

  _rep (  "\n\n");
   _rep (  " Species  Crn     DBH   CKR   Btle  Num  Mort  Mortality Equation\n");
   _rep (  " Code     Dam\%  (inch)        Dam   Trs  Prob    Id       Name\n");
   _rep (  " _________________________________________________________________________\n");

   for ( i = 0; i < 1000; i++ ) {            /* each spe selected by user    */
     if ( !MIS_Get (i, &s_MIS) )              /* get a struct                 */
        break;                               /*  all done                    */
     if ( stricmp ( s_MIS.cr_EquTyp,  e_CroDam) )
       continue; 

     f_Prob = MRT_CalcMngr(&s_MIS,&s_MO,cr_ErrMes);

     if ( s_MIS.f_CKR == e_PFI_Miss )
       sprintf (cr_CKR,"%s", e_NA);
     else
       sprintf (cr_CKR,"%1.0f", s_MIS.f_CKR);
 
     this->Mort_GetName(s_MIS.cr_Eq, cr_Name); 
     sprintf (cr,  " %-6s  %4.0f    %4.1f    %1s     %1s   %4.0f  %4.2f    %2s      %s\n",
                s_MIS.cr_Spe, s_MIS.f_CrnDam, s_MIS.f_DBH,
                cr_CKR,          s_MIS.cr_BeeDam,
                s_MIS.f_Den,  f_Prob,
                s_MIS.cr_Eq, cr_Name);
     _rep (cr);
   }

/*..................................................................*/
BolCha:
/* See if there are any Crown Damage equation types being used */
   i = MIS_EquTyp(e_BolCha); 
   if ( i == 0 )          /* None */
     goto Ret ; 

  _rep (  "\n\n");
  _rep (  " Species    DBH     Bole   Num   Mort   Mortality  Equation\n");
  _rep (  " Code      (inch)   Char   Trs   Prob     Id        Name\n");
  _rep (  " _________________________________________________________________________\n");

   for ( i = 0; i < 1000; i++ ) {            /* each spe selected by user    */
     if ( !MIS_Get (i, &s_MIS) )              /* get a struct                 */
        break;                               /*  all done                    */
     if ( stricmp ( s_MIS.cr_EquTyp,  e_BolCha) )
       continue; 
     f_Prob = MRT_CalcMngr(&s_MIS,&s_MO,cr_ErrMes); 
     this->Mort_GetName(s_MIS.cr_Eq, cr_Name); 

     sprintf (cr,  " %-6s     %3.0f     %4.2f  %4.0f   %0.2f     %3s      %s\n",
            s_MIS.cr_Spe, s_MIS.f_DBH, s_MIS.f_BolCha, s_MIS.f_Den, s_MIS.f_Prop, s_MIS.cr_Eq,  cr_Name);  
     _rep (cr); 
  }  /* for i */ 

Ret:;    
} 

/****************************************************
* Name: PST_GetProb
* Desc: Get a probability cut off point for the 
*       specified dbh 
*       Looks thru the table to find out which size
*       class the dbh falls into and return its
*       prob cut
*   Ret: the cutoff point, 
*        -1 if the dbh doesn't fall within any
*         size class
****************************************************/
float PST_GetProb (float dbh)
{
int i; 

  for ( i = 0; i < eC_PST; i++ ) {
    if ( sr_PST[i].low < 0 )  /* No entry */  
       continue;
    if ( dbh >= sr_PST[i].low && dbh < sr_PST[i].up )
      return  sr_PST[i].cut; 
  }
   return -1; 
}

/*****************************************************
* Name: PST_Check 
* Desc: make sure size classes are correct and don't 
*       over lap
*       Size classes are defined, as A => ... < B
*       Ex:  3 -> 6   start and include 3, and up to 
*        but not including 6 
*  Ret: 1 ok
*       0 error, -1 table is empty 
*****************************************************/
int PST_Check ()
{
int i, j; 
d_PST pst[eC_PST]; 

/* Load a temp table that does NOT contain blank entries */
    j = 0;
    for ( i = 0; i < eC_PST; i++ ) {
      if ( sr_PST[i].low < 0 ) 
        continue; 
      pst[j].low = sr_PST[i].low; 
      pst[j].up = sr_PST[i].up; 
      pst[j].cut = sr_PST[i].cut;
      j++;  }

      
    for ( i = 0; i < j; i++ ) {
     if ( pst[i].low >=  pst[i].up ) 
       return 0; 
     if ( pst[i].cut < 0 )
       return 0; 
     if ( pst[i].cut > 1.0 )
       return 0; 
    }

/* only one record so no need to compare it to other entries */
    if ( j == 1 )   
      return 1; 

/* See if any size classes over lap */   
    for ( i = 0; i < (j - 1); i++ ) {
      if ( pst[i].low >= pst[i+1].low )
        return 0; 
      if ( pst[i].up > pst[i+1].low )
        return 0; 
   } /* for i */ 

   return 1; 
}

/******************************************************
* Name: PST_Put 
* Desc: put entry into PST table
******************************************************/
int PST_Put (int ix, float low, float up, float cut)
{
int i; 
   if ( ix >= eC_PST ) /* shouldn't happen */
     return 0;
   sr_PST[ix].low = low;     
   sr_PST[ix].up = up; 
   sr_PST[ix].cut = cut; 
   return 1;  
} 

/*******************************************************/
void PST_Init ()
{
int i; 
  for ( i = 0; i < eC_PST; i++ ) {
     sr_PST[i].low = -1.0;
     sr_PST[i].up = -1.0;
     sr_PST[i].cut = -1.0;  } 
}

/****************************************************
*
*****************************************************/
int PST_Get (int i, float *low, float *up, float *cut)
{
    if ( i >= eC_PST )
      return 0; 
  *low = sr_PST[i].low;
  *up = sr_PST[i].up;
  *cut = sr_PST[i].cut;
   return 1; 
}

/*********************************************************
* Name: SalvageReport
* Desc: Do when GUI salvage report checkbox is checked
*       Report relies on GUI Size Class input settings
*********************************************************/ 
void _RemMinOne (char cr[]);

int Wnd_Mai::SalvageReport(char cr_TotNam[])
{
int i; 
char cr[1000], cr_ErrMes[1000], A[20]; 
float f, fN, f_Prob, f_CutOff, f_TotProb, f_TotKil; 
float f_TotBas, f_TotBasKil; 
d_MT  s_MT; 
d_MIS s_MIS; 
d_MO s_MO;

   fN = 0; 
   f_TotProb = f_TotBas = f_TotBasKil = f_TotKil = 0; 

/* Load the GUI Mort Spe Grid into MIS table */
   i = MIS_Load(cr_ErrMes);
   if ( i <= 0 ) {
     myMessageBox (cr_ErrMes);
     return 0; }

   _rep ("\n");
   strcpy (cr, " Species    DBH    Height   Crn    Crn   CKR  Btle  Bole   Mort     Cut  Status\n");
   _rep(cr);  


   strcpy (cr, " Code      (inch)  (feet)   Rat    Dam\%       Dam   Char   Prob     Off \n");
   _rep(cr);  
   _rep (  " _____________________________________________________________________________\n");



  for ( i = 0; i < 1000; i++ ) {            /* each spe selected by user    */
     if ( !MIS_Get (i, &s_MIS) )              /* get a struct                 */
        break;                               /*  all done                    */

     f_Prob = MRT_CalcMngr  (&s_MIS, &s_MO, cr_ErrMes);


     if ( f_Prob >= s_MIS.f_ProbCutOff ) {
        strcpy (A,"K");
        f_TotBasKil += Basal_Area (s_MIS.f_DBH,1.0);
        f_TotKil++;  }
     else 
        strcpy (A,"L"); 

     f_TotBas += Basal_Area (s_MIS.f_DBH,1.0);  

     sprintf (cr,  " %-7s   %5.1f   %4.0f    %3.0f     %3.0f  %3.0f    %-2s   %3.0f    %4.2f", 
                s_MIS.cr_Spe,s_MIS.f_DBH, s_MIS.f_Hgt, s_MIS.f_CR,   
                s_MIS.f_CrnDam, s_MIS.f_CKR, 
                s_MIS.cr_BeeDam, s_MIS.f_BolCha,
                s_MIS.f_Prop);
     _RemMinOne (cr);   /* Blank out -1 missing values */ 
     _rep (cr); 

   
    if ( s_MIS.f_ProbCutOff >= 0 ) 
      sprintf (cr, "    %4.2f    %s\n", s_MIS.f_ProbCutOff, A);
    else 
      strcpy (cr,"\n"); 

    _rep (cr); 

     f_TotProb += f_Prob;
     fN++; 
   }  /* for i end */


/*-----------------------------------------------------------------------*/
/* Size Class Section of Salvage Report */
float Low, Up, Cut; 
float Den, Live, Kill; 


  _rep ("\n\n\n TREE DENSITY\n"); 
  _rep (" Trees per acre\n\n");
  _rep ("    Size Class      Pre-fire      Killed       Post-fire\n"); 

  for ( i = 0; i < 3; i++ ) {
     PST_Get(i, &Low, &Up, &Cut); 
     if ( Low < 0 ) 
       continue;

     sprintf (cr, "  %4.1f  -  %4.1f   ",Low,Up);
    _rep (cr); 

     MIS_Totals_SizCls (&Den, &Live, &Kill, Low, Up);
     sprintf (cr,"   %4.0f         %4.0f         %4.0f \n",Den, Kill, Live); 
    _rep (cr);  
 }


/*-----------------------------------------------------------*/
/*  Do we need to save any totals */
  if ( !strcmp (cr_TotNam,"" ))  /* No save totals to do */
    return 1; ; 
 
  strcpy (s_MT.cr_TotNam,cr_TotNam);   /* Save total name */
  s_MT.f_SalMor = 0; 
  s_MT.f_SalKil = 0; 
  s_MT.f_SalBA = 0; 

  if ( fN == 0 )                   /* hummm, had to records, prevent div by 0 */ 
    goto Sav;

  s_MT.f_SalMor = f_TotProb / fN;   /* Average mort probability of all trees */

  s_MT.f_SalKil = f_TotKil / fN;        

  if ( f_TotBas != 0 ) 
    s_MT.f_SalBA =  f_TotBasKil / f_TotBas ;

Sav:
  MT_Save(&s_MT, e_RTsal);

  return 1;     
}

/********************************************************
* Blank out 'missing' values in the string 
* 
********************************************************/
void _RemMinOne (char cr[])
{
int i; 
char *a; 

/* Need to do in this order -1.00 -1.0  -1 */
/*  or else '.' '0' could get left in the string */
   for ( i = 0; i < 1000; i++ )  {
      a = strstr (cr, "-1.00");
      if ( a != NULL ) 
         memcpy (a,"     ",5);
      if ( a == NULL ) 
         break; }

   for ( i = 0; i < 1000; i++ )  {
      a = strstr (cr, "-1.0");
      if ( a != NULL ) 
         memcpy (a,"    ",4);
      if ( a == NULL ) 
         break; }

   for ( i = 0; i < 1000; i++ )  {
      a = strstr (cr, "-1");
      if ( a != NULL ) 
         memcpy (a,"  ",2);
      if ( a == NULL ) 
         break; }
}


