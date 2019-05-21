/***********************************************************************
* Name: Wnd_Calc.cpp
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
#include "bur_brn.h" 
#include "fof_nes.h"

#include "..\FOF_DLL\fof_sha.h"

extern char cr_DuffType[]; 

int  _DufMoi_Err(float f); 
int  _DufMoi_Mess(float f); 
int  gi_DufMes = 0; 


// int  CalcSub (d_FLI *a_FLI, d_CI *a_CI, d_CO *a_CO, d_SI *a_SI, d_SO *a_SO, char cr_ErrMes[]); 
int  SRM_ZeroDuffMess (d_CO *a_CO);
int  SRM_ChkCond (d_CO *a_CO );
void  SRM_SoiSimFail (d_SI *a_SI, char cr_StdId[], char cr_Mess[]);

int Set_Emission (d_CI *a_CI, d_FLI *a_FLI);

// #include "fof_dll.h" 

/**********************************************************************
* Name: Calculate
* Desc: 
* Note-1: We only want to show a Soil Error when the Soil Graph
*         window is visable, otherwise we don't care because we 
*         don't show any soil outputs on the GUI, as far as the 
*         Soil Report, we'll deal with that when the user hits
*         the Soil Report Button. 
*         THE Soil errors we'll be getting here are when the Simulate
*         fails, or there is a bad time from Burnup (If we don't fix
*         that) Other input types errors will get caught before hand. 
*  In: cr_EmFN...but a file name here to create a burnup emis file 
*                this name gets passes along to Burnup.
**********************************************************************/
int Wnd_Mai::Calculate(char cr_EmFN[], char cr_SptFN[])
{
int i, i_Per; 
float f,g, f_Herb, f_Lit, f_Fol, f_Bra;
float f_DW1, f_DW10;
float f_DW100, f_Snd_DW1k;
float f_Rot_DW1k,f_TotPer;
float f_Duff,f_DufDep, f_Shrub;
float f_Foliage, f_Branch;
char cr_ErrMes[3000], cr[1000];

d_CO CO; 
d_CI CI; 
d_SI SI;
d_SO SO;
d_FLI  s_FLI; 

  if ( this->iS_Calc == 0 )   /* Prevent going off until all combobox & init is done */ 
    return 0; 

  i_Per = 0; 
  CI_Init (&CI);
  CO_Init (&CO); 
  SI_Init (&SI);
  SO_Init (&SO);  

  if ( strcmp (cr_EmFN,"" ) ) 
    strcpy (CI.cr_EmiFN,cr_EmFN);

  i = Get_Inputs(&s_FLI);
  if ( i == 0 ) {
     myMessageBox(s_FLI.cr_ErrMes);
     return 0; }

/*.............................................*/
String ^ Str; 
//Str = _GetComboBox (this->_cbDuffType, cr_DuffType);
  Str = _GetComboBox (this->set_frm->_cb_DuffType_SW, cr_DuffType); 

/*.............................................*/

  f = e_dfm1 * 100.0; 
  if ( s_FLI.f_MoistDuff < f ) { 
    _DufMoi_Err(f); 
    return 0; }
  if ( s_FLI.f_MoistDuff == f ) 
    _DufMoi_Mess (f) ;

  if ( i ==-1 ) {  /* all inputs loads are 0 */ 
    this->ClearAll(&CO); 
    return 0; }  

// Doesn't seem to work
//  this->Cursor = Cursors::WaitCursor;  /* Wait Hourglass cursor */
// See Below....

  i = CalcSub(&s_FLI, &CI, &CO, &SI, &SO, cr_SptFN, cr_ErrMes);
  if ( i == 0 ) {                          /* Was there and error */
    if ( strstr (cr_ErrMes, e_SoilErr)){  /* was it a Soil Sim error */  
       myMessageBox(cr_ErrMes);            /* No, so display it */
       return 0; }  }                      /* See Note-1 Above */
 
/*.......................................*/
  if ( strcmp (cr_ErrMes,"") || strcmp (s_FLI.cr_ErrMes,"")  ) {
      myMessageBox(cr_ErrMes); 
      this->ClearAll(&CO); 
      return 0;  }
/*......................................*/


/* Fuel seems to be ok */
  this->fuel_frm->Update(&CO);  

/* Smoke seems to be ok */
  this->grap_emis->Update(&CO); 

/* Fire Intensity Graph */
  this->fire_frm->Update(&CO); 

/* Soil Graph, See Note-1 Above   */
  this->soil_frm3->Text = "Soil Graph";      /* Title  */
  this->soil_frm3->Update(&SI, &SO); 
  if ( this->soil_frm3->Visible && strcmp (cr_ErrMes,"")  ) {
     strcat (cr_ErrMes, "\n This message only appears when you \n"); 
     strcat (cr_ErrMes, " have the graph window active."); 
     myMessageBox (cr_ErrMes); }
 
  this->soil_frm3->UpdateWatts(); 

 if ( this->soil_frm3->Visible ) 
   SRM_ZeroDuffMess (&CO);            /* Mess if 0 duff was consumed       */

/* Pocosin warning note, */
 if ( !stricmp (CI.cr_CoverGroup, e_CVT_Pocosin) ) {
    this->soil_frm3->_lbPocoNote->Visible = true; 
    this->soil_frm3->_lbPocoMess->Visible = true; }
 else {
    this->soil_frm3->_lbPocoNote->Visible = false; 
    this->soil_frm3->_lbPocoMess->Visible = false; }


// see Above 
//  this->Cursor = Cursors::Default;  /*  cursor */

 strcpy (cr, CO.cr_BrnIg);     /* did Burnup ignite, "Yes" "No" */

   if ( this->_cbOutput->Text == e_OP_Cons ) {  /* ComboBox says show Consumd totals */
      f_Lit       = CO.f_LitCon      ;
      f_DW1       = CO.f_DW1Con      ;
      f_DW10      = CO.f_DW10Con     ;
      f_DW100     = CO.f_DW100Con    ;
      f_Snd_DW1k  = CO.f_Snd_DW1kCon ;
      f_Rot_DW1k  = CO.f_Rot_DW1kCon ;
      f_Duff      = CO.f_DufCon      ;
      f_DufDep    = CO.f_DufDepCon;
      f_Herb      = CO.f_HerCon ;
      f_Shrub     = CO.f_ShrCon;
      f_Fol       = CO.f_FolCon; 
      f_Bra       = CO.f_BraCon; 
      i_Per = 2;  }               /* decimal precision to show */


   if ( this->_cbOutput->Text == e_OP_Post ) { /* Combobox says show Post loads */
      f_Lit       = CO.f_LitPos   ;
      f_DW1       = CO.f_DW1Pos   ;
      f_DW10      = CO.f_DW10Pos  ;
      f_DW100     = CO.f_DW100Pos ;
      f_Snd_DW1k  = CO.f_S3Pos + CO.f_S6Pos + CO.f_S9Pos + CO.f_S20Pos;
      f_Rot_DW1k  = CO.f_R3Pos + CO.f_R6Pos + CO.f_R9Pos + CO.f_R20Pos;
      f_Duff      = CO.f_DufPos;
      f_DufDep    = CO.f_DufDepPos; 
      f_Herb      = CO.f_HerPos;
      f_Shrub     = CO.f_ShrPos;
      f_Fol       = CO.f_FolPos; 
      f_Bra       = CO.f_BraPos; 
      i_Per = 2; }                /* decimal precision to show */

/* Percent Consumed, this is "Percent Reduced" column on the report  */
  if ( this->_cbOutput->Text == e_OP_PCCons ) { /* Combobox says show Percent consumed */
      f_Lit = CO.f_LitPer; 
      f_DW1 = CO.f_DW1Per; 
      f_DW10 = CO.f_DW10Per; 
      f_DW100 = CO.f_DW100Per; 

/* change 5-22-14, fix bug, I would div by 0, would show on GUI as -1 */
      f_Snd_DW1k = 0; 
      g = CO.f_Snd_DW1kPre + CO.f_Rot_DW1kPre;
      if ( g > 0 ) 
        f_Snd_DW1k  = ( CO.f_Snd_DW1kCon + CO.f_Rot_DW1kCon ) / g;    /* Total consumed divided by Total Pre load */
      f_Snd_DW1k *= 100.0;            /* Get percent to whole number */
      f_Rot_DW1k = 0;                 /* need this to be 0 for adding below */

      f_Duff = CO.f_DufPer;
      f_DufDep = CO.f_DufDepPer; 

      f_Herb      = CO.f_HerPer;
      f_Shrub     = CO.f_ShrPer;
      f_Fol       = CO.f_FolPer; 
      f_Bra       = CO.f_BraPer; 

      i_Per = 0;                      /* Decimal precsion to show */
   }

   FloatToTextBox (this->_txOut_Litter, f_Lit,i_Per);  
   FloatToTextBox (this->_txOut_1Hr,   f_DW1, i_Per);
   FloatToTextBox (this->_txOut_10Hr, f_DW10,i_Per);
   FloatToTextBox (this->_txOut_100Hr,f_DW100,i_Per);

   f = f_Snd_DW1k + f_Rot_DW1k;
   FloatToTextBox (this->_txOut_1kHr,f,i_Per);
   FloatToTextBox (this->_txOut_Duff, f_Duff,i_Per);
   FloatToTextBox (this->_txOut_DufDep, f_DufDep,i_Per);
   FloatToTextBox (this->_txOut_Herb, f_Herb,i_Per);
   FloatToTextBox (this->_txOut_Shrub, f_Shrub,i_Per);
   FloatToTextBox (this->_txOut_Foliage, f_Fol,i_Per);
   FloatToTextBox (this->_txOut_Branch, f_Bra,i_Per);


/*--------------------------------------------------------------*/
/* Give the Heat/Watts Km/m2 to the Massman form1 class */
int iX, i_Tim; 
float f_PC, f_Wts;

  for ( iX = 0; iX < 1000000; iX++ ) {
    i = SHA_TP_Get (iX, &i_Tim, &f_PC, &f_Wts);
    if ( i == 0 )
      break; 
    this->BmSoil_Frm->ir_KwM2[iX] = f_PC * f_Wts; 
  }
  this->BmSoil_Frm->i_TimInc =  SHA_GetInc();

  this->BmSoil_Frm->FireTypeSet(); 

  return 1;
}

/*********************************************************************
*
*
*********************************************************************/
int _DufMoi_Err(float f)
{
char cr[3000], A[300]; 

  strcpy (cr, "The duff moisture amount entered is not within the parameters\n");
  strcat (cr, "of the FOFEM duff equations.\n"); 
  sprintf(A,  "Please enter a value of %4.1f or above.\n\n", f);
  strcat (cr,A); 
  sprintf(A,  "NOTE: Entering the exact lower limit value of %4.1f will cause\n",f);
  strcat (cr,A); 
  strcat (cr, "100 percent of the duff to be consumed."); 
   
  myMessageBox(cr);
  return 1;  
} 

int _DufMoi_Mess(float f)
{
char cr[3000], A[300];

  if ( gi_DufMes > 0 ) 
    return 0;
  
  gi_DufMes++; 

  strcpy  (cr, "The duff moisture amount you have entered is the exact lower\n"); 
  strcat  (cr, "limit that is allowed.\n"); 
  sprintf (A,  "This value ( %4.1f ) has a special significance in that it will\n",f); 
  strcat  (cr,A); 
  strcat  (cr, "cause 100 percent of the duff to be consumed. This action can be\n");
  strcat  (cr, "usefull if you are doing soil heating simulations.\n"); 
  strcat  (cr, "If this action is not require than you should enter a value\n");
  strcat  (cr, "that is just slightly higher than what you have currently entered.\n\n");
  strcat  (cr, " ------> This message will not be shown again.");  
 
  myMessageBox(cr);
  return 1;  
} 
 


/****************************************************************************
* Name: ClearAll
* Desc: Put 0 in all the output textboxs and clear the graphs
****************************************************************************/
int Wnd_Mai::ClearAll(d_CO *a_CO)
{
float f; 
  f = 0; 
d_SO SO; 
d_SI SI;
  SI_Init (&SI); 
  SO_Init (&SO); 

//  int Fire_Frm::Update(d_CO *a_CO)

  this->fire_frm->Clear (); 

  this->soil_frm3->Clear(); 

  this->fuel_frm->Update(a_CO);
  this->grap_emis->Clear(); 
  
  FloatToTextBox (this->_txOut_Litter, f,2);  
  FloatToTextBox (this->_txOut_1Hr,    f, 2);
  FloatToTextBox (this->_txOut_10Hr,   f,2);
  FloatToTextBox (this->_txOut_100Hr,  f,2);
  FloatToTextBox (this->_txOut_1kHr,   f,2);
  FloatToTextBox (this->_txOut_Duff,   f,2);
  FloatToTextBox (this->_txOut_DufDep, f,2);
  FloatToTextBox (this->_txOut_Herb,   f,2);
  FloatToTextBox (this->_txOut_Shrub,  f,2);
  FloatToTextBox (this->_txOut_Foliage,f,2);
  FloatToTextBox (this->_txOut_Branch, f,2);

  return 1; 
}


/****************************************************************************
*
* Note-1:  A watt is 1 joule per second, joule is a unit of work or energy
*          kW kilowatt is 1000 joules released per second
****************************************************************************/
int Wnd_Mai::CalcSub(d_FLI *a_FLI, d_CI *a_CI, d_CO *a_CO, d_SI *a_SI, d_SO *a_SO, 
                     char cr_PtsFN[],  char cr_ErrMes[])
{
int i; 
float f_Snd, f_Rot; 
char  cr[1000]; 
double d_dfi, d_tdf, d_Duf_CPTS, d_Watts, d_TotJoules, d_BurnTime, d_MidBurn; 
float f_DuffLoad, f_DuffMoist; 
float fr_FI[eC_SGV];
d_SI SI; 

/* Still need to hook this up */
//   strcpy (cr_PtsFN,"");

   strcpy (cr_ErrMes,""); 

   a_CI->f_Duff = a_FLI->f_Duff;            /* Duff load                         */
   a_CI->f_DufDep = a_FLI->f_DuffDepth;     /* Duff Depth                        */
   a_CI->f_MoistDuff = a_FLI->f_MoistDuff;  /* Duff Moisture                     */

   a_CI->f_Herb   = a_FLI->f_Herb;          /* Herbaceous load                   */
   a_CI->f_Shrub  = a_FLI->f_Shrub;         /* Shrub load                        */

   a_CI->f_CroFol = a_FLI->f_Foliage;                  /* Crown Foliage & Branch           */
   a_CI->f_CroBra = a_FLI->f_Branch;                  /*  we don't do these               */

   a_CI->f_Pc_CroBrn = a_FLI->f_PerFB;               /* Percent of Crown that will burn   */

   a_CI->f_MoistDW10   = a_FLI->f_Moist10Hr;  /* Moisture Down Wood 10 hr wood  */
   a_CI->f_MoistDW1000 = a_FLI->f_Moist1kHr;/* Moisture Down Wood 1000 hr wood*/

   a_CI->f_Lit   = a_FLI->f_Litter;
   a_CI->f_DW1   = a_FLI->f_1Hr;
   a_CI->f_DW10  = a_FLI->f_10Hr;
   a_CI->f_DW100 = a_FLI->f_100Hr;

   f_Rot = a_FLI->f_PerRot / 100.0 ;
   strcpy (a_CI->cr_WD, a_FLI->cr_LogDist);
   a_CI->f_DW1000 = a_FLI->f_1kHr; 

   strcpy (a_CI->cr_Region, a_FLI->cr_Region);      /* Set Region              */
   strcpy (a_CI->cr_Season, a_FLI->cr_Season);            /* Season                  */
   strcpy (a_CI->cr_FuelCategory, a_FLI->cr_FuelCat);   

/* NOTE user ENTIRE for now - need to resolve, we might Moist Method on a different Form */
   strcpy (a_CI->cr_DufMoiMet, "ENTIRE");         /* Always Entire           */

   strcpy (a_CI->cr_CoverClass, a_FLI->cr_FuelClass);
   CVT_GetCovGrp (a_FLI->cr_CoverType,a_CI->cr_CoverGroup);


   strcpy (a_CI->cr_LoadFN, "");                   /* NO output files wanted  */
 
// Caller would have set this or Null'd it 
//  strcpy (a_CI->cr_EmiFN,  "");

/* Distribute the 3+ Logs into classes */
  i = WD_SetWeightDist (a_FLI->f_1kHr, a_FLI->f_PerRot, a_FLI->cr_LogDist,  a_CI, cr_ErrMes);

/* Emissions - set factors into a_CI for doing the 'New' emissions */
/* Might not need these if user does 'Old' method, but get'em anyway */
  i = Set_Emission (a_CI, a_FLI);
  if ( i == 0 ) { 
     myMessageBox ("Invalid Emission Group Number"); 
  return 0; }

/* Now how does user want emissions done - Old or New way, according to Setting Wnd radio buttons */
   if  ( !stricmp (a_FLI->cr_EmisMethod,"Old" ))    /* if old way */
      a_CI->f_CriInt = -1;            /* this will tell burnup to do Old way */ 
  
  /*------------------------------------------------------------------*/
/* Call the Consume Manager, and get back answers                            */
   if ( !CM_Mngr (a_CI, a_CO, cr_ErrMes)){
     return 0; }

// Caller should have done this 
 //  SI_Init (a_SI); 
 
   if ( !SRM_ChkCond (a_CO) )          /* Chk for odd Duff Err Condition    */
      return 0;

/* Get the fire intesity array from CO struct into the Soil input Struct */
/* Both arrays are defined to be the same size */ 
   for ( i = 0; i < eC_SGV; i++ ) 
     fr_FI[i] = a_CO->sr_SGV[i].f_Inten; 

   strcpy (a_SI->cr_BrnIg, a_CO->cr_BrnIg);    /* Tells if Burnup Ignited       */
   strcpy (a_SI->cr_SoilType,a_FLI->cr_SoilType);
   strcpy (a_SI->cr_MoistCond,a_FLI->cr_Moisture);
   a_SI->f_SoilMoist = a_FLI->f_MoistSoil;

   a_SI->f_DufLoaPre  = a_CI->f_Duff;
   a_SI->f_DufConPer = a_CO->f_DufPer;
   a_SI->f_DufMoi = a_CI->f_MoistDuff ; 

   a_SI->f_DufDepPre = a_CO->f_DufDepPre;
   a_SI->f_DufDepPos = a_CO->f_DufDepPos; 

   a_SI->f_SoilWlEff = a_FLI->f_SoilWlEff;
   a_SI->f_SoilHsEff = a_FLI->f_SoilHsEff;
   a_SI->f_SoilDuffEff = a_FLI->f_SoilDuffEff; 


/*-----------------------------*/

   if ( !SH_Mngr (a_SI, a_SO, a_CO->fr_SFI, a_CO->fr_SFIhs, cr_PtsFN, cr_ErrMes)) {
     return 0; } 

//    if ( !strcmp (cr_ErrMes,e_SoiSimFail))
//     SRM_SoiSimFail (a_SI,"","");         
//   else
//     myMessageBox (cr_ErrMes);
//   return 0; }
     
   return 1;
}

/************************************************************************
* Name: Set_Emission
* Desc: Set the emission factors into the CI based on the user selected
*       group number/name in the FLI, FLI has the GUI inputs
* Note-1 The Critical Fire Intensity see comments for variable defininte
*        in the d_CI struct.
*************************************************************************/
int Set_Emission (d_CI *a_CI, d_FLI *a_FLI)
{
int i; 
char cr_GrpNum[100];


  a_CI->f_CriInt = NES_Get_CriFirInt();    /* See Note-1 */

/* Flaming */
  sscanf (a_FLI->cr_EmisFlame,"%s",cr_GrpNum);  /* Get Grp Num off front of text */
  
  i = NES_Get_MajFactor(cr_GrpNum,
                        &a_CI->f_fCO, 
                        &a_CI->f_fCO2,
                        &a_CI->f_fCH4, 
                        &a_CI->f_fPM25, 
                        &a_CI->f_fPM10, 
                        &a_CI->f_fNOX, 
                        &a_CI->f_fSO2);
  if ( i == 0 )         /* Grp Num not found - this would be a logic error */
	 return 0;

/* Smoldering Coarse Woody  */
  sscanf (a_FLI->cr_EmisSmold,"%s",cr_GrpNum);  /* Get Grp Num off front of text */
  
  i = NES_Get_MajFactor(cr_GrpNum,
                        &a_CI->f_sCO, 
                        &a_CI->f_sCO2,
                        &a_CI->f_sCH4, 
                        &a_CI->f_sPM25, 
                        &a_CI->f_sPM10, 
                        &a_CI->f_sNOX, 
                        &a_CI->f_sSO2);
  if ( i == 0 )
	  return 0;


/* Duff */
  sscanf (a_FLI->cr_EmisDuff,"%s",cr_GrpNum);  /* Get Grp Num off front of text */
  
  i = NES_Get_MajFactor(cr_GrpNum,
                        &a_CI->f_dCO, 
                        &a_CI->f_dCO2,
                        &a_CI->f_dCH4, 
                        &a_CI->f_dPM25, 
                        &a_CI->f_dPM10, 
                        &a_CI->f_dNOX, 
                        &a_CI->f_dSO2);
  if ( i == 0 )
	  return 0;

  return 1;
}

/**************************************************************************
* Name: Get_Inputs
* Desc: Get all the inputs from the GUI
***************************************************************************/
int Wnd_Mai::Get_Inputs(d_FLI *a_FLI)
{
int j; 
float f,f_Rot, f_Snd;
char cr[500];
String ^ Str; 

/*..................................................................*/
/* Soil Heating Fire Intensity Efficency - testing stuff */
A:
 if ( !_Get_TBFlo (this->set_frm->_txSoilWL_SW, &a_FLI->f_SoilWlEff, cr, "WL Fire Intensity Efficency", a_FLI->cr_ErrMes)) {
    this->set_frm->_txSoilWL_SW->Text = e_EffSoilWL;   /* Make sure we have something - default */
    goto A; }
B:
 if ( !_Get_TBFlo (this->set_frm->_txSoilHS_SW, &a_FLI->f_SoilHsEff, cr, "HS Fire Intensity Efficency", a_FLI->cr_ErrMes)) {
    this->set_frm->_txSoilHS_SW->Text = e_EffSoilHS;
    goto B; }
C:
 if ( !_Get_TBFlo (this->set_frm->_txSoilDuff_SW, &a_FLI->f_SoilDuffEff, cr, "Duff Fire Intensity Efficency", a_FLI->cr_ErrMes)) {
    this->set_frm->_txSoilDuff_SW->Text = e_EffSoilDuff;
    goto C; }

/*..................................................................*/

  if ( !_Get_TBFlo (this->_txLitter, &a_FLI->f_Litter, cr, "Litter", a_FLI->cr_ErrMes))
    return 0;

  if ( !_Get_TBFlo (this->_tx1Hr, &a_FLI->f_1Hr, cr, "1 Hr", a_FLI->cr_ErrMes))
    return 0;

  if ( !_Get_TBFlo (this->_tx10Hr, &a_FLI->f_10Hr, cr, "10 Hr", a_FLI->cr_ErrMes))
    return 0;

  if ( !_Get_TBFlo (this->_tx100Hr, &a_FLI->f_100Hr, cr, "100 Hr", a_FLI->cr_ErrMes))
    return 0;

  if ( !_Get_TBFlo (this->_tx1kHr, &a_FLI->f_1kHr, cr, "1000 Hr", a_FLI->cr_ErrMes))
    return 0;

  if ( !_Get_TBFlo (this->_txDuff, &a_FLI->f_Duff, cr, "Duff", a_FLI->cr_ErrMes))
    return 0;

  if ( !_Get_TBFlo (this->_txDuffDepth, &a_FLI->f_DuffDepth, cr, "Duff Depth", a_FLI->cr_ErrMes))
    return 0;

  if ( !_Get_TBFlo (this->_txHerb, &a_FLI->f_Herb, cr, "Herb", a_FLI->cr_ErrMes))
    return 0;

  if ( !_Get_TBFlo (this->_txShrub, &a_FLI->f_Shrub, cr, "Shrub", a_FLI->cr_ErrMes))
    return 0;

  if ( !_Get_TBFlo (this->_txFoliage, &a_FLI->f_Foliage, cr, "Foliage", a_FLI->cr_ErrMes))
    return 0;

  if ( !_Get_TBFlo (this->_txBranch, &a_FLI->f_Branch, cr, "Branch", a_FLI->cr_ErrMes))
    return 0;

/* Foliage Branch percent to be consumed */
  if ( !_Get_TBFlo (this->_txFolBraCon, &a_FLI->f_PerFB, cr, "Percent Foliage Branch Consumed", a_FLI->cr_ErrMes))
    return 0;

/* Percent 1000 Hour rotten classes */
  if ( !_Get_TBFlo (this->_txRotLog, &a_FLI->f_PerRot, cr, "Rotten percent", a_FLI->cr_ErrMes))
    return 0; 
 
/*---------------------------------------------------------------------------------*/
/* Moistures */
  if ( !_Get_TBFlo (this->_txMoistDuff, &a_FLI->f_MoistDuff, cr, "Duff Moisture", a_FLI->cr_ErrMes))
    return 0;

  if ( !_Get_TBFlo (this->_txMoist10Hr, &a_FLI->f_Moist10Hr, cr, "10 Hr Moisture", a_FLI->cr_ErrMes))
    return 0;

  if ( !_Get_TBFlo (this->_txMoist1kHr, &a_FLI->f_Moist1kHr, cr, "1000 Hr Moisture", a_FLI->cr_ErrMes))
    return 0;

 if ( !_Get_TBFlo (this->_txMoistSoil, &a_FLI->f_MoistSoil, cr, "Soil Moisture", a_FLI->cr_ErrMes))
    return 0;

  
  Str = _GetComboBox (this->_cbLogDist,  a_FLI->cr_LogDist);
  Str = _GetComboBox (this->_cbSeason,   a_FLI->cr_Season);
  Str = _GetComboBox (this->_cbRegion,   a_FLI->cr_Region);
  Str = _GetComboBox (this->_cbFuelCat,  a_FLI->cr_FuelCat);
  Str = _GetComboBox (this->_cbMoisture, a_FLI->cr_Moisture);
  Str = _GetComboBox (this->_cbFuelClass,a_FLI->cr_FuelClass);
  Str = _GetComboBox (this->_cbCoverType,a_FLI->cr_CoverType);
  Str = _GetComboBox (this->_cbSoil, a_FLI->cr_SoilType); 

/* Emission settings for 'New' get them requardless of Method to use 'New' or 'Old' */
  Str = _GetComboBox (this->_cbEmiFla, a_FLI->cr_EmisFlame);
  Str = _GetComboBox (this->_cbEmiSmo, a_FLI->cr_EmisSmold); 
  Str = _GetComboBox (this->_cbEmiDuf, a_FLI->cr_EmisDuff); 

  if ( this->set_frm->isEmisOld() )              /* Emission Method, Settings Wnd radio buttons */
     strcpy (a_FLI->cr_EmisMethod,"Old");        /* original way of doing emissions */            
  else 
     strcpy (a_FLI->cr_EmisMethod,"New");        /* new method uses input file */     

/*-----------------------------------------------------------*/
/* Check for all 0 loads */
   if ( a_FLI->f_Litter  <= 0 &&
         a_FLI->f_1Hr     <= 0 &&
         a_FLI->f_10Hr    <= 0 &&
         a_FLI->f_100Hr   <= 0 &&
         a_FLI->f_1kHr    <= 0 &&
         a_FLI->f_Duff    <= 0 &&
         a_FLI->f_Herb    <= 0 &&
         a_FLI->f_Shrub   <= 0 &&
         a_FLI->f_Foliage <= 0 &&
         a_FLI->f_Branch  <= 0 ) {
      strcpy (a_FLI->cr_ErrMes,"All input fuel loads are 0");
      return -1; }

 return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SRM_ChkCond
* Desc: Check for some odd error condtions that may occur.
*       I don't think any off these conditions ever happen but I had the
*        checks in orginal code, so I kept them in here.
*   Ret: 1 OK,  else 0
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int  SRM_ChkCond (d_CO *a_CO )
{
float f_Con, f_Pre, f_Post;
char  cr[300];

  f_Pre = a_CO->f_DufDepPre;                /* Pre-Fire Duff Depth          */
  f_Post = a_CO->f_DufDepPos;               /* Post-Fire Duff Depth         */

  if ( f_Post == -1 ) {
    strcpy (cr, "Cannot run soil heat, because fuel equations could not\n");
    strcat (cr, "calculate the duff depth reduction. SRM_ChkCond()");
    myMessageBox (cr);
    return 0; }

  f_Con = f_Pre - f_Post;                    /* Consumed Duff Depth          */

  if ( f_Con < 0 ) {                         /* Watch out                    */
    sprintf (cr, "Consumed Duff Depth is Negative: %f\n", f_Con);
    strcat (cr, "Can not continue do to error SRM_ChkCond() ");
    myMessageBox (cr);
    return 0; }

  if ( f_Con > e_DufDepLim ) {
    sprintf (cr, "The consumed duff depth (%5.1f), exceeds the soil simulation limit (%5.1f) \n", f_Con, e_DufDepLim);
    strcat  (cr, "Please check duff depth setting. SRM_ChkCond()");
    myMessageBox (cr);
    return 0; }

   return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SRM_ZeroDuff
* Desc: See if there was no reduction of duff depth, if so give use the
*        message shown below.
* Note-1: If there was no prefire duff then the 'no duff' simulation was
*          run in which case the message is no appropriate.
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int  SRM_ZeroDuffMess (d_CO *a_CO)
{
char cr_ErrMes[3000];

/* Change 3-18-06 added this check................                           */
   if ( a_CO->f_DufPre == 0 )           /* See Note-1 above                  */
     return 1;

   if ( a_CO->f_DufDepCon == 0 ) {       /* Let user know                     */
    strcpy (cr_ErrMes,"The duff depth equation under these conditions has predicted\n"); 
    strcat (cr_ErrMes,"little or no depth reduction. \n");
    strcat (cr_ErrMes,"Therefore, the effect on soil heating will be minimal.\n ");
    strcat (cr_ErrMes,"This condition my be cause by the duff moisture level\n");
    strcat (cr_ErrMes,"You can try setting the duff moisture to 10 percent\n"); 
    strcat (cr_ErrMes,"which will force the equation to consume all of the duff."); 
    myMessageBox (cr_ErrMes);
    return 0; }

   return 1;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SRM_SoiSimFail
* Desc: Popup Window Message when the Soil Model Fails
* NOTE: This function can get called while doing regular FOFEM report or
*        graph or when the Batch is running.
*       See In Params below, I have this set up so that the Batch will send
*        in some addtional message text
*   In: a_SI.....soil input struct
*       cr_StdId..used by Batch
*       cr_Mess..used by Batch, see NOTE above
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
void  SRM_SoiSimFail (d_SI *a_SI, char cr_StdId[], char cr_Mess[])
{
char cr[100];
char cr_ErrMes[5000];
#define e_SRMSSF1 " The Soil Heating Model cannot make predicts using the inputs specified below.\n\n"

#define e_SRMSSF2 "There are some combinations of inputs that the soil model cannot handle, as such\n" \
                  " this message will appear to inform you that the soil calculations were \n" \
                  " not completed.   \n \n" \
                  "  Some conditions (inputs) that may cause this problem are:\n" \
                  "    >  high (wet) moisture content \n" \
                  "    >  large fuel loadings for smaller fuel components such as litter,small wood,etc      \n" \
                  "    >  a high fire intensity, fire intenisty is internally calculated using \n" \
                  "        the Burnup fuel consumption model \n" \
                  "  You may find that by reducing one or more of the above numbers the model will run without incident. \n"

   strcpy (cr_ErrMes,e_SRMSSF1);

   if ( strcmp (cr_StdId,"" )) {        /* Batch is caller sending in Std id */
     sprintf (cr, " Stand Id: %s \n", cr_StdId);
     strcat (cr_ErrMes,cr); }

   sprintf (cr, " Duff Depth Pre-Fire:%5.2f \n",   a_SI->f_DufDepPre);
   strcat (cr_ErrMes,cr);

   sprintf (cr, " Duff Depth Post-Fire:%5.2f \n",  a_SI->f_DufDepPos);
   strcat (cr_ErrMes,cr);

   sprintf (cr, " Soil Moisture:%5.2f \n",         a_SI->f_SoilMoist);
   strcat (cr_ErrMes,cr);

   sprintf (cr, " Ignition: %s \n",                a_SI->cr_BrnIg);
   strcat (cr_ErrMes,cr);

   sprintf (cr, " Soil Type: %s \n",               a_SI->cr_SoilType);
   strcat (cr_ErrMes,cr);

   sprintf (cr, " Moisture Condition: %s \n",      a_SI->cr_MoistCond);
   strcat (cr_ErrMes,cr);

//   sprintf (cr, " Flame Time (seconds):%d \n",     a_SI->i_Time);
//   strcat (cr_ErrMes,cr);

//   sprintf (cr, " Fire intensity:%5.2f \n\n",        a_SI->f_fi);
//   strcat (cr_ErrMes,cr);

   strcat (cr_ErrMes,e_SRMSSF2);

   if ( strcmp (cr_Mess,"")){
    strcat (cr_ErrMes,"\nNOTE:\n");       /* See notes above                */
    strcat (cr_ErrMes,cr_Mess);}

   myMessageBox (cr_ErrMes);


}
