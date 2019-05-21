#include <stdio.h>
#include <math.h> 
#include <string.h>


#include "Wnd_Mai.h" 

#include "Wnd_Def.h"

#include "Wnd_Util.h"
#include "fof_ci.h" 
#include  "CVT_CVT.h" 
#include   "fof_fwf2.h"
#include "fof_util.h" 
#include "bur_but.h"


#include "fof_sgv.h"
#include "fof_co.h" 
#include "fof_co2.h" 
#include "fof_cm.h"

#include "fof_sh.h" 
#include "fof_sd.h" 

#include "fof_suf.h" 
#include "fof_nes.h"


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
typedef struct {
     char cr[100];
     int i_Def;  /* tells which one is to be selected item */ 
   } d_CBI;

/* Moisture ............................... */
d_CBI sr_MT[] = { { e_MT_VeryDry, 0 }, { e_MT_Dry, 1 }, { e_MT_Moderate, 0 }, { e_MT_Wet, 0 }, { "", 0 } };
 
/* Fuel Category............................... */
d_CBI sr_FC[] = { {  e_SAF_SRM, 1 },  {  e_NVCS,    0 },
                  {  e_FCCS,    0 },  {  e_FLM,     0 },{  e_FFI,     0 },  {  "",        0 }};
/* Region ....................................... */
d_CBI sr_Reg[] = { { e_Interior_West, 1 }, { e_Pacific_West, 0 },
                  { e_North_East   , 0 }, { e_South_East  , 0 }, { "",   0 }};
/* Season .................................. */
d_CBI sr_Sea[] = { { e_Spring, 0 }, { e_Summer, 1 },
                  { e_Fall, 0 },  { e_Winter, 0 }, { "",   0 }};
/* Outputs ................................ */
d_CBI sr_OP[] = { { e_OP_Cons,1 }, { e_OP_Post, 0 }, { e_OP_PCCons, 0 },
                   { "",   0 }};

/* Fuel Category */
/* 10-10-12 - DL requested removing the Pile */
d_CBI sr_FCa[] = {{ e_FCa_Natural,1 }, { e_FCa_Slash, 0 }, { "",   0 }};
// d_CBI sr_FCa[] = {{ e_FCa_Natural,1 }, { e_FCa_Pile, 0 }, { e_FCa_Slash, 0 }, { "",   0 }};



/* Log Distribution */
d_CBI sr_LD[] = { {e_Even,1}, {e_Right,0}, {e_Left, 0}, {e_EndWD,0}, {e_Center,0 }, { "",0}};

/* Soil Type */
d_CBI sr_ST[] = { { e_LoaSke,0}, { e_FinSil,0}, { e_Fin,0}, { e_CoaSil,1}, { e_CoaLoa,0}, { "", 0}};  

/* Moisture Method */


/*-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-*/
/*                           Fuel Load Table                                 */
#define  e_Adj_Lig   1
#define  e_Adj_Typ   2
#define  e_Adj_Hea   3
#define  e_Adj_User  4
 #define e_Adj_Spa   5
#define  e_Adj_Abu   6

#define e_FT_Lit  1
#define e_FT_1Hr  2
#define e_FT_10Hr 3
#define e_FT_100Hr  4
#define e_FT_1kHr 5
#define e_FT_Duff 6

#define e_FT_Herb    7
#define e_FT_Shrub   8
#define e_FT_Foliage 9
#define e_FT_Branch  10

typedef struct {
    char cr_ClsNam[50];
    float f_Lit;
    float f_1Hr;
    float f_10Hr; 
    float f_100Hr;
    float f_1kHr; 
    float f_Duff;

    float f_Herb;
    float f_Shrub;
    float f_Foliage;
    float f_Branch;

    float f_User; 
 } d_FLT; 


/*-------------------------------------------------------------------------*/
float FLT_Get (int i_FueTyp, int i_Adj);
bool isFuelLight (TextBox ^ tb );
bool isFuelHeavy (TextBox ^ tb );


int _EmiChk (char Fla[], char Duf[], char Coa[]);


/******************************************************************
* Name: Set_FueBoxCol
* Desc: Set the Fuel Load TextBox color based on the the Textbox's
*        ContextMenu Item the is sent in here
*    In: s_MenuItem.....Light, Heavy, etc.
*        s_txBox........the textbox name
******************************************************************/
int Wnd_Mai::Set_FueBoxCol (String ^ s_MenuItem, String ^ s_txBox)
{
int i_FT, i_Adj;
float f;
bool b_ReadOnly;
  System::Drawing::Color color ; 
  TextBox ^ tb; 

 /* What is the Adjustment - Item from Context Menu */
  b_ReadOnly = true; 

  if ( s_MenuItem == e_Light ) {         /* Dead Fuels use Light */
     color = Color::FromArgb(e_LigR,e_LigG,e_LigB); }

  else if ( s_MenuItem == e_Sparse ) {        /* Live Fuels use Sparse */
     color = Color::FromArgb(e_LigR,e_LigG,e_LigB); }   /* User Light Color */

  else if ( s_MenuItem == e_Typical ) {
     color = Color::FromArgb(e_TypR,e_TypG,e_TypB); }

  else if ( s_MenuItem == e_Heavy ) {                 /* Dead fuels use Heavy */
     color = Color::FromArgb(e_HeaR,e_HeaG,e_HeaB); }

  else if ( s_MenuItem == e_Abundant) {               /* Live Fuels use Abundant */
     color = Color::FromArgb(e_HeaR,e_HeaG,e_HeaB); }

  else if ( s_MenuItem == e_User ) {
     color = Color::FromArgb(e_UsrR,e_UsrG,e_UsrB); 
     b_ReadOnly = false; }

  else {
     MessageBox::Show ("Logic ERROR Wnd Mai");
     return 0; }

/* What fuel loading box are we working with */
  if ( s_txBox == e_txLitter){
    tb = this->_txLitter; }

  else if ( s_txBox == e_tx1Hr){
    tb = this->_tx1Hr; }

  else if ( s_txBox == e_tx10Hr){
    tb = this->_tx10Hr; }

  else if ( s_txBox == e_tx100Hr){
    tb = this->_tx100Hr; }

 else if ( s_txBox == e_tx1kHr){
    i_FT = e_FT_1kHr;
    tb = this->_tx1kHr; }

  else if ( s_txBox == e_txDuff){
    tb = this->_txDuff; }

  else if ( s_txBox == e_txHerb){
    tb = this->_txHerb; }

  else if ( s_txBox == e_txShrub){
    tb = this->_txShrub; }

  else if ( s_txBox == e_txFoliage){
    tb = this->_txFoliage; }

  else if ( s_txBox == e_txBranch){
    tb = this->_txBranch; }

  else {
      MessageBox::Show ("Logic ERROR 2 Wnd_Mai");
      return 0; }
 
  tb->ReadOnly = b_ReadOnly; 
  tb->BackColor = color; 
      
  return 1;
}



/********************************************************************************
* Name: CBI_Load
* Desc: Load Items into a ComboBox. The Items are stored in arrays 
*   In: See below
********************************************************************************/
void Wnd_Mai::CB_Load ( ComboBox^  cb)
{
int i,j;
String ^ Str; 
d_CBI *a; 

   if ( cb->Name == "_cbMoisture" )
      a = sr_MT; 
   else if ( cb->Name == "_cbRegion") 
      a = sr_Reg; 
   else if ( cb->Name == "_cbFuelClass" )
      a =sr_FC;
   else if ( cb->Name == "_cbSeason" )
      a =sr_Sea;
   else if ( cb->Name == "_cbOutput" )
      a =sr_OP;
   else if ( cb->Name == "_cbFuelCat")
      a = sr_FCa;
   else if ( cb->Name == "_cbLogDist") 
      a = sr_LD; 
   else if ( cb->Name == "_cbSoil") 
      a = sr_ST;                      
   else {
     myMessageBox ("logic error - CB_Load() ");
     return ; }

  cb->Items->Clear();  /* Clean out any old items */
  j = 0; 
   for ( i = 0; i < 100; i++ ) {  /* Each item in table */
     if ( !strcmp (a[i].cr,"") )
       break; 
     Str = _CharToStr (a[i].cr); 
     cb->Items->Add(Str);         /* add it to the combobox */
     if ( a[i].i_Def == 1 )       
       j = i;
   }
   cb->SelectedIndex = j;         /* set as Selected item in ComboBox */
}

/********************************************************************
* Name: Moist_Change
* Desc: When Moisture ComboBox changes fill the moisture textbox
*       with the defualts for the newly selected combobox item
*********************************************************************/
void Wnd_Mai::Moist_Change()
{
String ^ Str; 
   Str = this->_cbMoisture->Text;

   if ( Str == e_MT_VeryDry ) {
     this->_txMoist10Hr->Text = "6" ;
     this->_txMoist1kHr->Text  = "10" ;
     this->_txMoistDuff->Text  = "20" ;
     this->_txMoistSoil->Text = "5" ; }

   else if ( Str == e_MT_Dry ) {
     this->_txMoist10Hr->Text = "10" ;
     this->_txMoist1kHr->Text  = "15" ;
     this->_txMoistDuff->Text  = "40" ; 
     this->_txMoistSoil->Text = "10";  }

   else if ( Str == e_MT_Moderate ) {
     this->_txMoist10Hr->Text = "16" ;
     this->_txMoist1kHr->Text  = "30" ;
     this->_txMoistDuff->Text  = "75" ;
     this->_txMoistSoil->Text = "15";  }

   else if ( Str == e_MT_Wet ) {
     this->_txMoist10Hr->Text = "22" ;
     this->_txMoist1kHr->Text  = "40" ;
     this->_txMoistDuff->Text  = "130" ; 
     this->_txMoistSoil->Text = "25" ; }
   else
     myMessageBox ("logic error - Moist_Change()  ");

}
/***************************************************************
* Output ComboBox changed 
***************************************************************/
void Wnd_Mai::Output_Change()
{
String ^ Str;
    Str = this->_cbOutput->Text;
    this->_lbOutput->Text = Str; 
}

/*********************************************************************
* Name: ClassFile_Load
* Desc: Load a Class File ( fof_saf, nvcs, fccs, flm .csv files ) 
*       AND load each of the cover types into the Cover Type 
*       Combobox and set the first cover type as the Selected item
*       in the ComboBox
* Note-1: This means this function was called because the Region 
*         comboBox was changed, so if the Cover Type ComboBox is on 
*         FFI there's no need to reload the FFI file because it
*         isn't depended on Region, it's valid for all regions. 
*         Actually the Region ComboBox should be made invisable for 
*         FFI file input - might make it invisible later  ? ? ? 
*********************************************************************/
int Wnd_Mai::WM_ClassFile_Load (char cr_FFIPthFN[])
{
int i, j,iX;
float f_pcRot; 
char  cr_Reg[100], cr[500], cr_ErrMes[3000], cr_CovTyp[200]; 
String ^ s_Reg, ^ Str, ^ s_FCMU;  

   _StrToChar(this->s_UserFolder,cr);
  strcat (cr,"\\"); 

  s_FCMU = this->_cbFuelClass->Text;    /* Fuel Class ComboBox Selected Item text */
  if ( s_FCMU == "" )                   /* if nothing ?, set to SAF */
    s_FCMU = e_SAF_SRM; 

  if ( s_FCMU == e_SAF_SRM ) {            /* to determine which      */
    strcat (cr,e_FOF_SAF);
    if ( !SAF_LoadFileNew (cr, cr_ErrMes)){  /* input Cover File we need*/
      myMessageBox (cr_ErrMes); 
      return 0;  } }

  else if ( s_FCMU == e_NVCS ) {
    strcat (cr,e_FOF_NVCS); 
    if ( !NVCS_LoadFileNew (cr, cr_ErrMes)){  /* input Cover File we need*/
      myMessageBox (cr_ErrMes); 
      return 0;  }  }

  else if ( s_FCMU == e_FCCS ) {
    strcat (cr,e_FOF_FCCS);
    if ( !FCC_LoadFileNew (cr, cr_ErrMes)){  /* input Cover File we need*/
      myMessageBox (cr_ErrMes); 
      return 0;  }  }

  else if ( s_FCMU == e_FLM ) {
    strcat (cr,e_FOF_FLM);
    if ( !this->FLM_LoadFileNew (cr, cr_ErrMes)){  /* input Cover File we need*/
      myMessageBox (cr_ErrMes); 
      return 0;  }  }
 
/* FFI Plot input file...................... */
  else if ( s_FCMU == e_FFI ) {
    if ( !strcmp (cr_FFIPthFN,"") )  /* See Note-1 above */ 
      return 1;                      
    i = this->FFI_LoadCVT(cr_FFIPthFN, cr_ErrMes); 
    if ( i == 0 ) {  /* Error reading FFI input file */
      myMessageBox (cr_ErrMes);
      return 0; }
   }

  else {
     myMessageBox ("Logic Error - CVT_Load_CovTypCB ()"); 
     return 0; }

  s_Reg = _GetComboBox (this->_cbRegion, cr_Reg);  /* Region ComboBox Item */
  if ( s_Reg == "" ) 
    s_Reg = e_Interior_West;   /* See Note-1 above */

/* Get the index to check if Cover Type is in Region */
  if ( s_Reg ==  e_Interior_West ) iX = e_Reg_IntWest;  
  if ( s_Reg ==  e_Pacific_West  ) iX = e_Reg_PacWest ; 
  if ( s_Reg ==  e_North_East    ) iX = e_Reg_Northeast;
  if ( s_Reg ==  e_South_East    ) iX = e_Reg_Southeast;
 
  this->_cbCoverType->Items->Clear();           /* Clear out the list */
  
/* Load CoverTypes for desired Region into ComboBox */
  strcpy (cr_CovTyp,"" );
  int iN = 0;  
  for ( i = 0; i < 1000; i++ ) {           /* Get each record in Cover/Fuel table */
    j = CVT_GetCovReg (i, iX,  cr);     
    if ( j == -1 )                         /* No more */
      break; 
    if ( j == 0 )                          /* Not in Region we want */
      continue; 
    if ( !strcmp(cr_CovTyp,"") )           /* Sav 1st */
      strcpy (cr_CovTyp,cr);    
    Str = _CharToStr (cr);                 /* Put it into combobox list */
    this->_cbCoverType->Items->Add(Str);
    iN++; 
  } /* for i */

   
   if ( s_FCMU == e_FCCS || s_FCMU == e_FFI ) {
     f_pcRot = CVT_pcRot (cr_CovTyp);
     FloatToTextBox (this->_txRotLog,f_pcRot,2);   
  }

  if ( iN > 0 ) 
    this->_cbCoverType->SelectedIndex = 0;   /* set ComboBox to 1st item in list */

  return 1; 
}


/**************************************************************************
* Name: isFuelLigh, isFuelHeavy, isTypical
* Desc: see if a Fuel Load TextBox's color is set to a light (heavy) adj color 
* NOTE: The only way we adjustment a textbox is set to is by it's color
***************************************************************************/
bool Wnd_Mai::isFuelLight (TextBox ^ tb )
{
 if (  tb->BackColor ==  Color::FromArgb(e_LigR,e_LigG,e_LigB) ) 
   return true;
 return false; 
}

/* For now I have Light & Sparse using the same color */
bool Wnd_Mai::isFuelSparse (TextBox ^ tb )
{
 if (  tb->BackColor ==  Color::FromArgb(e_LigR,e_LigG,e_LigB) ) 
   return true;
 return false; 
}


bool Wnd_Mai::isFuelHeavy (TextBox ^ tb )
{
 if (  tb->BackColor ==  Color::FromArgb(e_HeaR,e_HeaG,e_HeaB) ) 
   return true;
 return false; 
}

/* For now I have Heavy and Abundant using the same color */
bool Wnd_Mai::isFuelAbundent (TextBox ^ tb )
{
 if (  tb->BackColor ==  Color::FromArgb(e_HeaR,e_HeaG,e_HeaB) ) 
   return true;
 return false; 
}

bool Wnd_Mai::isFuelTypical (TextBox ^ tb )
{
 if (  tb->BackColor ==  Color::FromArgb(e_TypR,e_TypG,e_TypB) ) 
   return true;
 return false; 
}

bool Wnd_Mai::isFuelUser (TextBox ^ tb )
{
 if (  tb->BackColor ==  Color::FromArgb(e_UsrR,e_UsrG,e_UsrB) ) 
   return true;
 return false; 
}



/*********************************************************************
* Name: Set_MenuColors
* Desc: Set the coordinated colors for the Fuel TextBoxes, Adj Buttons
*        and the adj menu items 
**********************************************************************/
void Wnd_Mai::Set_AdjustmentColors()
{
/* Set Dead Fuel TextBox ContextMenu Item Colors  */
  this->_mi_Light->BackColor   = Color::FromArgb(e_LigR,e_LigG,e_LigB);
  this->_mi_Typical->BackColor = Color::FromArgb(e_TypR,e_TypG,e_TypB);
  this->_mi_Heavy->BackColor   = Color::FromArgb(e_HeaR,e_HeaG,e_HeaB);
  this->_mi_User->BackColor    = Color::FromArgb(e_UsrR,e_UsrG,e_UsrB);

/* Set Live Fuel TextBox ContextMenu Item Colors */
  this->_miAbundent->BackColor = Color::FromArgb(e_HeaR,e_HeaG,e_HeaB);
  this->_miSparse->BackColor = Color::FromArgb(e_LigR,e_LigG,e_LigB);
  this->_miTypicalLive->BackColor = Color::FromArgb(e_TypR,e_TypG,e_TypB);
  this->_miUserLive->BackColor = Color::FromArgb(e_UsrR,e_UsrG,e_UsrB);

/* Set Loading Adjustment Button Colors */
  this->_btLightSpa->BackColor = Color::FromArgb(e_LigR,e_LigG,e_LigB);
  this->_btTypical->BackColor  = Color::FromArgb(e_TypR,e_TypG,e_TypB);
  this->_btHeavyAbun->BackColor = Color::FromArgb(e_HeaR,e_HeaG,e_HeaB);

  WM_FuelBoxColors (e_Typical);
}

/*********************************************************************
* Name: WM_FuelBoxColors
* Desc: Set the Fuel Load textboxs to the specifide Adjustment
*        color 
**********************************************************************/
void Wnd_Mai::WM_FuelBoxColors (String ^ s_Adj)
{
int R,G,B;

 if ( s_Adj == e_Light  || s_Adj == e_Sparse ) {
   R = e_LigR;
   G = e_LigG;
   B = e_LigB; }

 if ( s_Adj == e_Typical ) { 
   R = e_TypR;
   G = e_TypG;
   B = e_TypB; }

 if ( s_Adj == e_Heavy  || s_Adj == e_Abundant ) {
   R = e_HeaR;
   G = e_HeaG;
   B = e_HeaB; }

 if ( s_Adj == e_User ) {
   R = e_UsrR;
   G = e_UsrG;
   B = e_UsrB; }

/* Dead Fuel TextBoxs */
  this->_txLitter->BackColor = Color::FromArgb(R,G,B);
  this->_tx1Hr->BackColor    = Color::FromArgb(R,G,B);
  this->_tx10Hr->BackColor   = Color::FromArgb(R,G,B);
  this->_tx100Hr->BackColor  = Color::FromArgb(R,G,B);
  this->_tx1kHr->BackColor   = Color::FromArgb(R,G,B);
  this->_txDuff->BackColor   = Color::FromArgb(R,G,B);

/* Live Fuel TextBoxs */
  this->_txShrub->BackColor   = Color::FromArgb(R,G,B);
  this->_txHerb->BackColor    = Color::FromArgb(R,G,B);
  this->_txFoliage->BackColor = Color::FromArgb(R,G,B);
  this->_txBranch->BackColor  = Color::FromArgb(R,G,B);
} 

/***********************************************************
* Name: Mai_FuelAdj_Set
* Desc: According to what fuel class (SAF,NVCS,FCC,FLM)
*       enable/hide the adjustment buttons and menues
*       Fuel Adjustments are only done for SAF & NVCS
************************************************************/
int Wnd_Mai::WM_FuelAdj_Set()
{
int i; 
 
  if ( this->_cbFuelClass->Text == e_FCCS || this->_cbFuelClass->Text == e_FLM  ||
       this->_cbFuelClass->Text == e_FFI  ){
    this->_btHeavyAbun->Visible = false;    /* Hide the Adjustment buttons */
    this->_btLightSpa->Visible = false;
    this->_mi_Heavy->Visible = false;       /* Hide Contextmenu items */
    this->_mi_Light->Visible = false;       /* only want the Typical and User */
    this->_miAbundent->Visible = false;
    this->_miSparse->Visible = false;
    return 1;  
    }
 
/*--------------------------------------------*/
/* SAF, NVCS */
  this->_btHeavyAbun->Visible = true;    /* Make sure adjustment buttons are showing */
  this->_btLightSpa->Visible = true;

  this->_mi_Heavy->Visible = true;       /* show the Contextmenu items */
  this->_mi_Light->Visible = true; 
  this->_miAbundent->Visible = true;
  this->_miSparse->Visible = true; 
  return 0; 
} 

/*******************************************************************
* Name:
* Desc:
*******************************************************************/
void Wnd_Mai::WM_UserFuels_Save ()
{

  if ( this->isFuelUser(this->_txDuff ))  {  
     this->s_uDuff    = this->_txDuff->Text;
     this->s_uDuffDep = this->_txDuffDepth->Text; }

  if ( this->isFuelUser(this->_txLitter) ) this->s_uLit = this->_txLitter->Text;
  if ( this->isFuelUser(this->_tx1Hr   ) ) this->s_u1Hr    = this->_tx1Hr->Text;
  if ( this->isFuelUser(this->_tx10Hr  ) ) this->s_u10Hr   = this->_tx10Hr->Text;
  if ( this->isFuelUser(this->_tx100Hr ) ) this->s_u100Hr  = this->_tx100Hr->Text;
  if ( this->isFuelUser(this->_tx1kHr  ) ) this->s_u1kHr   = this->_tx1kHr->Text;

  if ( this->isFuelUser(this->_txHerb   ) ) this->s_uHerb  = this->_txHerb->Text;
  if ( this->isFuelUser(this->_txShrub  ) ) this->s_uShrub = this->_txShrub->Text;
  if ( this->isFuelUser(this->_txFoliage) ) this->s_uFol   = this->_txFoliage->Text;
  if ( this->isFuelUser(this->_txBranch ) ) this->s_uBra   = this->_txBranch->Text;

}
 
/*******************************************************************
* Name: UserFuels_Set
* Desc: Checks to see if a fuel TextBox is set to User and if so
*       it puts the saved user value into it.
*       The way we know is a fuel TextBox is set as User is by
*       its color. 
*******************************************************************/
void Wnd_Mai::WM_UserFuels_Set ()
{
//  if ( this->isFuelUser(this->_txDuff) ) this->_txDuff->Text =  this->s_uDuff;

  if ( this->isFuelUser(this->_txDuff )) {   
     this->_txDuff->Text      = this->s_uDuff ;
     this->_txDuffDepth->Text = this->s_uDuffDep ; }


  if ( this->isFuelUser(this->_txLitter) ) this->_txLitter->Text  = this->s_uLit  ;
  if ( this->isFuelUser(this->_tx1Hr   ) ) this->_tx1Hr->Text     = this->s_u1Hr  ;
  if ( this->isFuelUser(this->_tx10Hr  ) ) this->_tx10Hr->Text    = this->s_u10Hr ;
  if ( this->isFuelUser(this->_tx100Hr ) ) this->_tx100Hr->Text   = this->s_u100Hr;
  if ( this->isFuelUser(this->_tx1kHr  ) ) this->_tx1kHr->Text    = this->s_u1kHr ;

  if ( this->isFuelUser(this->_txHerb   ) )this->_txHerb->Text    = this->s_uHerb ;
  if ( this->isFuelUser(this->_txShrub  ) )this->_txShrub->Text   = this->s_uShrub;
  if ( this->isFuelUser(this->_txFoliage) )this->_txFoliage->Text = this->s_uFol  ;
  if ( this->isFuelUser(this->_txBranch ) )this->_txBranch->Text  = this->s_uBra  ;

} 

/**************************************************************************
* Name: Mai_FuelTB_Load
* Desc: Load the fuel value into one or all Fuel Textbox(s)
*       This function will determine what settings are on the one the
*        window - Fuel Category ComboBox, Fuel Load Cover Combobox
* NOTE: we look at each Fuel Load TextBox to see what color it is so
*        that we know what fuel adjustment to use on it.   
**************************************************************************/
int Wnd_Mai::Mai_FuelTB_Load( String ^ s_TexBoxNam)  
{
int iX; 
float f, f_Adj; 
char cr_CovTyp[500],cr_FuelClass[100], cr_CovGrp[30];
char cr_HvCod[50], cr_FueCat[50];
d_CFL s_CFL; 
String ^ Str, ^ s_FueCat;

/* Get selected Cover Type item from combobox */
   Str = this->_cbCoverType->Text; 
  _StrToChar (Str, cr_CovTyp);

/* Get the fuels record from table (read in .cvs file) */
  iX = CVT_GetCFL ( cr_CovTyp, &s_CFL, cr_CovGrp);
  if ( iX < 0 ) {
     myMessageBox ("Logic Error - Mai_FuelTB_Load");
     return 0;    /* Shouldn't happen, would be a logic error */
   }
/* Fuel Category ComboBox Item - Natural, Pile, Slash */
   Str = this->_cbFuelCat->Text;
  _StrToChar (Str, cr_FueCat);

   strcpy (cr_HvCod, s_CFL.cr_HvCod);     /* Harvest Debris Code */

/* Dead Fuels.................... */
   if ( s_TexBoxNam == e_txLitter || s_TexBoxNam == "All" )
     SAF_SetDeadLoad (this->_txLitter,s_CFL.f_litter, e_TPALit,   cr_FueCat, cr_HvCod);

   if ( s_TexBoxNam == e_tx1Hr  || s_TexBoxNam == "All" )
     SAF_SetDeadLoad (this->_tx1Hr,   s_CFL.f_DW0,    e_TPAW0,   cr_FueCat, cr_HvCod);
 
   if ( s_TexBoxNam == e_tx10Hr || s_TexBoxNam == "All"  )
     SAF_SetDeadLoad (this->_tx10Hr,  s_CFL.f_DW10,   e_TPAW01,  cr_FueCat, cr_HvCod);
  
   if ( s_TexBoxNam == e_tx100Hr || s_TexBoxNam == "All"  )
     SAF_SetDeadLoad (this->_tx100Hr, s_CFL.f_DW100,  e_TPAW13, cr_FueCat, cr_HvCod);
  
   if ( s_TexBoxNam == e_tx1kHr  || s_TexBoxNam == "All" )
     SAF_SetDeadLoad (this->_tx1kHr,  s_CFL.f_DW1000, e_TPAW3,  cr_FueCat, cr_HvCod);

/* Duff */
   if ( s_TexBoxNam == e_txDuff  || s_TexBoxNam == "All" )
     SAF_SetDuff (s_CFL.f_Duff, s_CFL.f_DufDep, cr_CovTyp, cr_CovGrp); 

/* Live Fuels */
  if ( s_TexBoxNam == e_txHerb  || s_TexBoxNam == "All" )
     SAF_SetLiveLoad(this->_txHerb, s_CFL.f_Herb, e_TPAHer, s_CFL.cr_Herb); 

  if ( s_TexBoxNam == e_txShrub  || s_TexBoxNam == "All" )
     SAF_SetLiveLoad(this->_txShrub, s_CFL.f_Shrub, e_TPAShr, s_CFL.cr_Shrub); 

  if ( s_TexBoxNam == e_txFoliage  || s_TexBoxNam == "All" )
     SAF_SetLiveLoad(this->_txFoliage, s_CFL.f_CroFol, e_TPAFol, s_CFL.cr_CroFol); 

  if ( s_TexBoxNam == e_txBranch  || s_TexBoxNam == "All" )
     SAF_SetLiveLoad(this->_txBranch, s_CFL.f_CroBra, e_TPASmB, s_CFL.cr_CroBra); 

 Str = this->_cbFuelClass->Text->ToString(); 
 if ( Str == e_FCCS && s_TexBoxNam == "All" )
    FloatToTextBox (this->_txRotLog, s_CFL.f_pcRot, 2);  
 else if ( Str == e_FFI && s_TexBoxNam == "All" )
    FloatToTextBox (this->_txRotLog, s_CFL.f_pcRot, 2);  
 else if ( s_TexBoxNam == "All" ) 
    FloatToTextBox (this->_txRotLog, s_CFL.f_pcRot, 2);  

return 1; 
}

/**********************************************************
*
*
***********************************************************/
int Wnd_Mai::CCW_Get_RegionInt ()
{
String ^ Str; 
  Str = this->_cbRegion->SelectedText;
  if ( Str == e_Interior_West ) return 1; 
  if ( Str == e_Pacific_West ) return 2; 
  if (Str == e_North_East ) return 3; 
  if (Str == e_South_East ) return 4; 
  myMessageBox ("CCW_Get_RegionInt() Logic Error"); 
 }

/*******************************************************
* Name:
* Desc: Save the Burnup Emission file 
********************************************************/
int Wnd_Mai::Burnup_EmFil_Save()
{
int i; 
char cr_FN[500];
String ^ Str;  

System::Windows::Forms::SaveFileDialog FD;

 #define e_EmFN  "txt files (*.txt)|*.txt|All files (*.*)|*.*"

  strcpy (cr_FN,""); 
 
/* Pop Up a SaveFileDialog to get a file name */
/* Use the Path & File Name from the Project File TextBox */
  FD.Title = "Save FOFEM Emission File"; 
  FD.InitialDirectory = this->Prj_GetPath();  
  FD.FileName = "Emis.txt"; 
  FD.Filter = e_EmFN;
  i = (int) FD.ShowDialog(); 
  if ( i != 1 )
    return 0; 

  Str = FD.FileName;  /* Get Path + File Name from DialogBox */ 
 _StrToChar (Str,cr_FN);

  this->Calculate(cr_FN,"");

  return 1; 
 }

/**********************************************************
* Name: Burnup_SamInpFil
* Desc: Create a sample burnup input file
*       This file can be read in for the Run Burnup Input
*       File option.                                  
**********************************************************/
int Wnd_Mai::Burnup_SamInpFile()
{int i; 
char cr_FN[500], cr_ErrMes[500];
String ^ Str;  
System::Windows::Forms::SaveFileDialog FD;
d_CO CO; 
d_CI CI; 
d_SI SI;
d_SO SO;
d_FLI  s_FLI; 

#define e_SBFN  "brn files (*.brn)|*.brn|All files (*.*)|*.*"
 
  CI_Init (&CI);
  CO_Init (&CO); 
  SI_Init (&SI);
  SO_Init (&SO);  

/* Get inputs from GUI */
  i = Get_Inputs(&s_FLI);
  if ( i == 0 ) {
     myMessageBox(s_FLI.cr_ErrMes);
     return 0; }

  if ( i ==-1 ) {  /* all inputs loads are 0 */ 
    this->ClearAll(&CO); 
    return 0; }  

/* Run a Calc, just so it can error check the inputs */
  if ( !CalcSub(&s_FLI, &CI, &CO, &SI, &SO, "", cr_ErrMes)) {
     myMessageBox(cr_ErrMes);
     return 0; }


/*--------------------------------*/
  strcpy (cr_FN,""); 
 
/* Pop Up a SaveFileDialog to get a file name */
/* Use the Path & File Name from the Project File TextBox */
  FD.Title = "Save FOFEM Sample Burnup Input File"; 
  FD.InitialDirectory = this->Prj_GetPath();  
  FD.FileName = "Burnup-In.brn"; 
  FD.Filter = e_SBFN;
  i = (int) FD.ShowDialog(); 
  if ( i != 1 )
    return 0; 

  Str = FD.FileName;  /* Get Path + File Name from DialogBox */ 
 _StrToChar (Str,cr_FN);

  if ( !But_CreInpFil (&CI, cr_FN,  cr_ErrMes)) {
   myMessageBox (cr_ErrMes); 
   return 0; }

  return 1; 
} 

/*****************************************************
*
*
******************************************************/
int Wnd_Mai::Burnup_RunInpFile()
{
int i; 
char cr_FN[500], cr_ErrMes[500], cr_ConFN[500], cr_EmiFN[500], cr[1000]; 
String ^ Str;  
System::Windows::Forms::OpenFileDialog FD;
System::Windows::Forms::SaveFileDialog SD;
#define e_SBFN  "brn files (*.brn)|*.brn|All files (*.*)|*.*"
 
#define e_ConFN  "txt files (*.txt)|*.txt|All files (*.*)|*.*"

/*--------------------------------*/
/* Get Input File Name */ 
  strcpy (cr_FN,""); 
 
/* Pop Up a SaveFileDialog to get a file name */
/* Use the Path & File Name from the Project File TextBox */
  FD.Title = "FOFEM - Run Burnup Input File"; 
  FD.InitialDirectory = this->Prj_GetPath();  
  FD.FileName = "Burnup-In.brn"; 
  FD.Filter = e_SBFN;
  i = (int) FD.ShowDialog(); 
  if ( i != 1 )
    return 0; 

  Str = FD.FileName;  /* Get Path + File Name from DialogBox */ 
 _StrToChar (Str,cr_FN);


/*-----------------------------------*/
/* Get Output Consumed File Name */ 
  strcpy (cr_ConFN,""); 
  SD.Title = "Save Consumed File"; 
  SD.InitialDirectory = this->Prj_GetPath();  
  SD.FileName = "BurnCon.txt"; 
  SD.Filter = e_ConFN;
  i = (int) SD.ShowDialog(); 

  Str = SD.FileName;  /* Get Path + File Name from DialogBox */ 
 _StrToChar (Str,cr_ConFN);


/*-----------------------------------*/
/* Get Output Consumed File Name */ 
  strcpy (cr_EmiFN,""); 
  SD.Title = "Save Emission File"; 
  SD.InitialDirectory = this->Prj_GetPath();  
  SD.FileName = "BurnEmis.txt"; 
  SD.Filter = e_ConFN;
  i = (int) SD.ShowDialog(); 

  Str = SD.FileName;  /* Get Path + File Name from DialogBox */ 
 _StrToChar (Str,cr_EmiFN);

  if ( !strcmp (cr_ConFN,"") && !strcmp (cr_EmiFN,"") ) { 
    myMessageBox ("No Input Files Selected");
    return 0; } 

  if ( !But_RBIF (cr_FN, cr_ConFN, cr_EmiFN, cr_ErrMes)) {
    myMessageBox (cr_ErrMes);
    return 0;}

  sprintf (cr,"Files Created: \n%s\n%s", cr_ConFN,cr_EmiFN); 
  myMessageBox (cr); 

 return 1; 
}

/********************************************************
* Name: Soil_CreateSample
* Desc: Create the sample soil input files,
*
**********************************************************/
int Wnd_Mai::Soil_CreateSample()
{
int i; 
char cr_FN[1000], cr_ErrMes[2000];
String ^ Str; 

System::Windows::Forms::SaveFileDialog SD;
#define e_DufFil  "duf files (*.duf)|*.duf|All files (*.*)|*.*"
#define e_ExpFil  "exp files (*.exp)|*.exp|All files (*.*)|*.*"

d_CO CO; 
d_CI CI; 
d_SI SI;
d_SO SO;
d_FLI  s_FLI; 

/* Need to run a calc first, we'll need a few of the outputs */
  CI_Init (&CI);
  CO_Init (&CO); 
  SI_Init (&SI);
  SO_Init (&SO);  

  i = Get_Inputs(&s_FLI);            /* Get GUI inputs, we'll need a few of them  */
  if ( i == 0 ) {                    
    myMessageBox(s_FLI.cr_ErrMes);
    return 0; }

  i = CalcSub(&s_FLI, &CI, &CO, &SI, &SO, "", cr_ErrMes);
  if ( i == 0 ) {                          /* Was there and error */
    if ( strstr (cr_ErrMes, e_SoilErr)){  /* was it a Soil Sim error */  
       myMessageBox(cr_ErrMes);            /* No, so display it */
       return 0; }  }                      /* See Note-1 Above */
 

  if ( s_FLI.f_Duff <= 0 ) 
     goto NonDuff; 

/*-----------------------------------*/
/* Sample Duff File  */
  SD.Title = "Sample Soil Duff File"; 
  SD.InitialDirectory = this->Prj_GetPath();  
  SD.FileName = "Sample.duf"; 
  SD.Filter = e_DufFil;
  i = (int) SD.ShowDialog(); 
  if ( i == 0 ) 
    return 0; 
  Str = SD.FileName;  /* Get Path + File Name from DialogBox */ 
 _StrToChar (Str,cr_FN);
  
  SUF_Sample_Duff (cr_FN, &s_FLI, CO.f_DufPer); 
  return 1; 

/*------------------------------------*/
/* Sample Exp (non-duff) File         */ 
NonDuff: 
  SD.Title = "Sample Soil Non-Duff File"; 
  SD.InitialDirectory = this->Prj_GetPath();  
  SD.FileName = "Sample.exp"; 
  SD.Filter = e_ExpFil;
  i = (int) SD.ShowDialog(); 
  if ( i == 0 ) 
    return 0;

  Str = SD.FileName;  /* Get Path + File Name from DialogBox */ 
 _StrToChar (Str,cr_FN);

  SUF_Sample_Exp (cr_FN, &s_FLI, &CO); 
 
  return 1; 

} 
/****************************************************************
*
*
********************************************************************/
int Wnd_Mai::Soil_RunInputFile()
{
  this->SUF_Exec();
  return 1; 
}

/****************************************************************
*
*
*****************************************************************/
int Wnd_Mai::Soil_SavePointsFile()
{
int i; 
char cr_FN[500], cr[1000];
String ^ Str; 
System::Windows::Forms::SaveFileDialog SD;
System::Windows::Forms::DialogResult  dr;

#define e_SOFil  "points files (*.txt)|*.txt|All files (*.*)|*.*"

/* Save Dialog Box to get file name  */
  SD.Title = "Output Soil Points File"; 
  SD.InitialDirectory = this->Prj_GetPath();  
  SD.FileName = "SoilPts.txt"; 
  SD.Filter = e_SOFil;
  dr = SD.ShowDialog(); 
  if ( dr != System::Windows::Forms::DialogResult::OK )
    return 0;

  Str = SD.FileName;  /* Get Path + File Name from DialogBox */ 
 _StrToChar (Str,cr_FN);

/* Run Calc, gets and checks screen inputs, soil will run using the file name */
  i = this->Calculate("",cr_FN);   
  if ( i == 1 ) {
    sprintf (cr, "Soil Points File has been created using the current screen inputs.\n %s",cr_FN);
    myMessageBox (cr);
  } 

  return 1; 
} 

/*****************************************************************
* Name: Set_TB_ReadOnly
* Desc: set all fuel input textboxes to bool b
*       true = readonly, else false 
*****************************************************************/
void Wnd_Mai::Set_TB_ReadOnly (bool b)
{
   this->_txLitter->ReadOnly = b;
   this->_tx1Hr->ReadOnly = b;
   this->_tx10Hr->ReadOnly = b;
   this->_tx100Hr->ReadOnly = b;
   this->_tx1kHr->ReadOnly = b;
   this->_txDuff->ReadOnly = b;
   this->_txHerb->ReadOnly = b;
   this->_txShrub->ReadOnly = b;
   this->_txFoliage->ReadOnly = b;
   this->_txBranch->ReadOnly = b;
}


/***********************************************************************
* Name: CSF_Exec
* Desc: Determine where the User FOFEM is and save the Path 
*       if the folder isn't there create it and copy the samples
*       files to it and create an .ini file and project file.
*       NOTE the only time the folder isn't there is the first time
*       FOFEM is run after instalation. 
*       The folder location path is different for, Win7/8 and XP
*        machines, 
*       Example
*       c:\Users\larry\AppData\Local\FOFEM  Forest Service win7/8
*       c:\Documents and Settings\larry\Application Data\Local Data\FOFEM
* NOTE: the FC-UD.txt file (always in Program Startup folder) can
*       dictate the 'FOFEM' folder name, what ever name is 
*       in the file will get used, see Note-3 below
* Note-3 This will let us install multiple copies/version of FOFEM
*        so that they both will have their own user FOFEM folder
*        or else each version would be trying to use C:\...\FOFEM 
*
***********************************************************************/
int Wnd_Mai::CSF_Exec()
{
int i;
char cr[1000], cr_PthFN[1000]; 
String ^ Str, ^ A, ^ s_PthFN, ^ s_FSFC;
bool b; 
array<String^> ^ line;

// #define e_FSWinPath "C:\\Users\\Public\\AppData" 

#define e_UPXP   "C:\\Documents and Settings"
#define e_UPWn   "C:\\Users"

/* use this if by chance there is no FOF-UD.txt file found */
#define e_FSFC    "FOFEM6" 

/* File that contains what name we'll use for the User FOFEM folder */
    A = System::Windows::Forms::Application::StartupPath; /* Where .exe is */
    s_PthFN = A + "\\FOF-UD.txt";   /*  */
    try {
       line = IO::File::ReadAllLines(s_PthFN); } 
    catch (Exception ^ err ) {
        MessageBox::Show("Warning - No FOF-UD.txt file found. \n Will use 'FOFEM as a user folder name", "FOFEM");
        s_FSFC = e_FSFC;    /* If file not found */
        goto AA;  }
    
    s_FSFC = line[0];   /* User FOFEM folder name */
AA:    
   
/*------------------------------------------------------------*/
/* Let's see what OS we're running on                         */
    String ^ s_Major = Environment::OSVersion->Version->Major.ToString();
    String ^ s_Minor = Environment::OSVersion->Version->Minor.ToString();

    if ( s_Major == "5" && s_Minor == "1" )
       this->s_OS = e_OSXP; 
    else if ( s_Major == "6" && s_Minor == "1" )   /* Win 7 */
       this->s_OS = e_OSWn;
    else if ( s_Major == "6" && s_Minor == "2" )   /* Win 8 */
       this->s_OS = e_OSWn;
    else 
       this->s_OS = e_OSOth;

/* Based what Op Sys - form User Path */
    A = Environment::ExpandEnvironmentVariables("%userprofile%");
    if ( this->s_OS == e_OSWn )        /* Win 7, 8 */
       Str = A + "\\AppData\\Local\\" + s_FSFC;
    else if ( this->s_OS == e_OSXP )    /* XP  */
       Str = A + "\\Application Data\\Local Settings\\" + s_FSFC;
    else                         /* Other, ? ? */
       Str = "C:\\" + s_FSFC;

/* We now the path - Ex c:\Users\larry\AppData\Local\FOFEM */
/* So ksee if it's already there or we need to create and load files to it */
    if ( System::IO::Directory::Exists(Str) ){ /* Folder files alread in place */
        goto SetIniLoc; }
     else{  /* Create Folder and move files into it */
        System::IO::Directory::CreateDirectory(Str); 
        this->CSF_Mngr (Str); 
        goto SetIniLoc;  }

SetIniLoc:
  this->s_UserFolder = Str; 

  return 1; 
}


/******************************************************
* Name: CSF_Mngr
* Desc: Copy Sample files to User FOFEM Folder
*       also setup the .ini and .prj files 
*
******************************************************/
int Wnd_Mai::CSF_Mngr(String ^ ToPath)
{
int i, j;
bool b;  
char cr[1000];
String ^ FrPath;
String ^ From;
String ^ To; 
String ^ s_FN; 
String ^ s_FilToMov; 
String ^ s_Err, ^ A, ^ s_PrjFN, ^ s_PltFN; 
array<String^> ^ liness;

/* Folder FOFEM exe is in, sample files are here */
    FrPath = System::Windows::Forms::Application::StartupPath;

/* File that contains the names of the files we need move */
    s_FN = FrPath + "\\FOF-MovFil.txt"; 
    try {
       liness = IO::File::ReadAllLines(s_FN); } 
    catch ( Exception ^ err ){
       MessageBox::Show ("Can Open/Find FOF-MovFil.txt file while trying \n to create User FOFEM folder", "FOFEM"); 
       return 0; }

    j = liness->Length;

/* Move each of the file */
    for ( i = 0; i < j; i++ ) {
      if ( liness[i] == "" ) 
        continue ;
       s_FilToMov = liness[i];
       From = FrPath + "\\" + s_FilToMov;
       To   = ToPath + "\\" + s_FilToMov;
       try {
          System::IO::File::Copy(From, To); } 
       catch (Exception ^ err ){
           A = err->Message->ToString();
           s_Err = "Error attempting to copy data files to User folder\n" + From + "\n -> " + A;
           MessageBox::Show(s_Err, "FOFEM") ;
       }   
     } /* for i */ 

  
/*---------------------------------------------------------*/
/* Create a FOFEM.ini file that points to the Sample project file */
/*  in the User Folder */
    s_FN = ToPath + "\\" + e_Ini_FN ;
    A = ToPath + "\\" + e_PrjNam; 
    _StrToChar (A, cr);         /*  needs char array embedded */
    _ArgFill (cr);              /*   blanks filled */
    s_PrjFN = _CharToStr(cr);
    try {
       IO::File::AppendAllText(s_FN, s_PrjFN); } 
    catch (Exception ^ err ) {
       MessageBox::Show(err->Message->ToString(), "FOFEM");  } 

  return 1; 
}



/*****************************************************************
* Name: Emis_ComboBox 
* Desc: Load the Emission Flame & Smolder ComboBoxs with the 
*       Emission Group Number + Group Name
* NOTE: The Emission Factor file must be read into first and
*        have its table/struct loaded 
******************************************************************/
int Wnd_Mai::Emis_ComboBox()
{
int i,j, iS_Def;
char cr_GrpNum[500], cr_CovTyp[500], cr[500]; 
String ^ Str;
String ^ s_FlaDef, ^s_DufDef;
 
  s_FlaDef = "";
  s_DufDef = "";
  
  for ( i = 0; i < 1000; i++ ) {
    j = NES_GetGrpCov (i, cr_GrpNum, cr_CovTyp,&iS_Def);
    if ( !strcmp (cr_GrpNum,"") )
	   break;
	sprintf (cr, "%s - %s", cr_GrpNum, cr_CovTyp);
	Str = _CharToStr(cr);
	if ( j == 1 )                        /* its a flaming type */
	   this->_cbEmiFla->Items->Add(Str);
    if ( j == 2 ) 
	   this->_cbEmiDuf->Items->Add(Str); 
	if ( j == 3 ) 
	   this->_cbEmiSmo->Items->Add(Str); 

	if ( j < 0 ) {
		sprintf (cr, "Invalid Type code found in Emission Factor File: %s  ",cr_CovTyp);
		myMessageBox (cr);
	    continue; }

	if ( iS_Def == 1 ) 
	  s_FlaDef = Str;
	if ( iS_Def == 2 ) 
	  s_DufDef = Str;
    
  } /* for i */

  i = this->_cbEmiFla->FindString(s_FlaDef);
  if ( i >= 0 ) 
    this->_cbEmiFla->SelectedIndex = i; 

  i = this->_cbEmiDuf->FindString(s_DufDef);
  if ( i >= 0 ) 
	this->_cbEmiDuf->SelectedIndex = i; 

  this->_cbEmiSmo->SelectedIndex = 0; 

  return 1; 
}

/******************************************************************
* Name: Emis_SetCovTyp
* Desc: Select the Emission item based on the Selected item in 
*       the Cover Type ComboBox 
******************************************************************/
int Wnd_Mai::Emis_SetCovTyp()
{
int i, fl, sm, df;
char cr[500], cr_CovTyp[500], cr_EmiFla[50], cr_EmiSmo[50];
char cr_Mess[2000];
d_CFL s_CFL;
String ^ Str;

/* Prevent this function from running until after Emis ComboBoxs have been loaded */
   i = this->_cbEmiFla->Items->Count;
   if ( i == 0 ) 
     return 0; 
   
   strcpy (cr_Mess,"");
   fl = sm = 0;

/* See if we're dealing with an FFI input file........................... */
   i = Emis_SetCovTyp_FFI();
   if ( i == 1 )           /* OK we took care of an FFI file */   
     return 1;             /*  so we're done */ 


/* Get selected Cover Type item from combobox */
   Str = this->_cbCoverType->Text; 
  _StrToChar (Str, cr_CovTyp);

/* Get the fuels record from table  */
  CVT_GetCFL ( cr_CovTyp, &s_CFL, cr);


/* Flaming ComboBox .................................................*/
  Str = _CharToStr(s_CFL.cr_EmiGrpFla);         /* # from Cover Table */
  if ( Str == "" ) {
    Str = e_DefFlaGrp;                         /* none - set default */
    fl = 1;  }
  i = this->_cbEmiFla->FindString(Str);
  if ( i < 0 ) {                                /* humm, all numbers in file should be ok */
    sprintf (cr_Mess, "Invalid Flaming Emission Group Number (%s) for cover type\n%s",s_CFL.cr_EmiGrpFla,cr_CovTyp);
	myMessageBox (cr_Mess); 
    i = 0; }
  this->_cbEmiFla->SelectedIndex = i;           /* Seleted item in combobox */


/* Smoldering Coarse Woody ComboBox .............................................. */
  Str = _CharToStr(s_CFL.cr_EmiGrpS1k);
  if ( Str == "" ) {
    Str = e_DefSmoGrp;
    sm = 1;  }

  i = this->_cbEmiSmo->FindString(Str);
  if ( i < 0 ) {
    sprintf (cr_Mess, "Invalid Smoldering Coarse Wood Emission Group Number (%s) for cover type\n%s",s_CFL.cr_EmiGrpS1k,cr_CovTyp);
	myMessageBox (cr_Mess);
    i = 0; }

  this->_cbEmiSmo->SelectedIndex = i; 


/* Duff  ComboBox .............................................. */
  Str = _CharToStr(s_CFL.cr_EmiGrpDuf);
  if ( Str == "" ) {
    Str = e_DefDufGrp;
    df = 1;  }

  i = this->_cbEmiDuf->FindString(Str);
  if ( i < 0 ) {
    sprintf (cr_Mess, "Invalid Duff Emission Group Number (%s) for cover type\n%s",s_CFL.cr_EmiGrpDuf,cr_CovTyp);
	myMessageBox (cr_Mess);
    i = 0; }

  this->_cbEmiDuf->SelectedIndex = i; 


  if ( fl == 1 || sm == 1 || df == 1 ) {
    sprintf (cr_Mess, "No emission factors found in input file for the plots listed below.\nDefault emissions factors will be used\n %s",cr_CovTyp);
	myMessageBox (cr_Mess); }


  return 1; 
}

/***************************************************************************
* Name: Emis_SetCovTyp_FFI()
* Desc: Set the GUI emission settings according to the emis codes found
*        in the FFI input file for the selected plot/cover record. 
*       
* 
***************************************************************************/
int Wnd_Mai::Emis_SetCovTyp_FFI()
{
int i; 
char cr_CovTyp[100], cr[1000];
String ^ Str;
d_CFL s_CFL;  

  if (this->_cbFuelClass->Text != e_FFI )
    return 0; 

/* Get selected Cover Type item from combobox */
   Str = this->_cbCoverType->Text; 
  _StrToChar (Str, cr_CovTyp);

/* Get the fuels record from table  */
   CVT_GetCFL ( cr_CovTyp, &s_CFL, cr);


   i = _EmiChk (s_CFL.cr_EmiGrpFla, s_CFL.cr_EmiGrpDuf, s_CFL.cr_EmiGrpS1k); 
   if ( i == 0 ){ 
     sprintf (cr, "Error - Missing emission code in FFI input file for\n for %s", cr_CovTyp); 
     myMessageBox (cr);
     return 0; }

/* FFI file says to use Burnup old/orig emis factors, so on the Settings Window check  */
/*  the Emission Factors Default radio button, that hide the Expanded Emis comboboxes */
/*  on the main Wnd and cause the Burnup orig factors to be used */ 
   if ( i == 3 ) {     
     this->set_frm->_rb_EmiOld->Checked = true; 
     return 1; }

   this->set_frm->_rb_EmiNew->Checked = true;     /* Setting Wnd Emis Factor Expanded radio button */

/* We checked above, so at this point we know we have all 3 Expanded emis or */
/*  they are all blank (use defaults)  */
   
/* Flaming ComboBox .................................................*/
  Str = _CharToStr(s_CFL.cr_EmiGrpFla);         /* # from Cover Table */
  if ( Str == "" ) 
    Str = e_DefFlaGrp;                         /* none - set default */
  i = this->_cbEmiFla->FindString(Str);
  if ( i < 0 ) {                                /* humm, all numbers in file should be ok */
    sprintf (cr, "Invalid Expanded Emission Factor Code\nFlaming and Short-term Smoldering (%s)\n%s",s_CFL.cr_EmiGrpFla,cr_CovTyp);
	myMessageBox (cr); 
    i = 0; }
  this->_cbEmiFla->SelectedIndex = i;           /* Seleted item in combobox */


/*  Coarse Woody RSC ComboBox .............................................. */
  Str = _CharToStr(s_CFL.cr_EmiGrpS1k);
  if ( Str == "" ) 
    Str = e_DefSmoGrp;  
  i = this->_cbEmiSmo->FindString(Str);
  if ( i < 0 ) {
    sprintf (cr, "Invalid Expanded Emission Factor Code\n Coarse Wood RSC (%s)\n %s",s_CFL.cr_EmiGrpS1k,cr_CovTyp);
	myMessageBox (cr);
    i = 0; }
  this->_cbEmiSmo->SelectedIndex = i; 


/* Duff  ComboBox .............................................. */
  Str = _CharToStr(s_CFL.cr_EmiGrpDuf);
  if ( Str == "" ) 
    Str = e_DefDufGrp;
  i = this->_cbEmiDuf->FindString(Str);
  if ( i < 0 ) {
    sprintf (cr, "Invalid Expanded Emission Factor Code\n Duff RSC (%s) \n %s",s_CFL.cr_EmiGrpDuf,cr_CovTyp);
	myMessageBox (cr);
    i = 0; }
  this->_cbEmiDuf->SelectedIndex = i; 


  return 1; 
}

/*****************************************************************
* Name: _EmiChk
* Desc: check the various combinations of emission factors coming in
*       from the FFI input plot file.
* Ret:
*      2  All three fields were blank, so FuelCalc will use the 
*         emission settings on the GUI, which could be the Expaned
*         emis factor codes when the are showing or Burnups default
*         emis factors (original ones burnup always used)
*      0  Error - only 1 or 2 fields had codes in them 
*      3  All three fields contain zeros, which means use Burnup's
*         default original emis factors
*      1  the fields contain Expanded emiss factor codes 
*
******************************************************************/
int _EmiChk (char Fla[], char Duf[], char Coa[])
{
int i;
  i = 0; 
  if ( strcmp (Fla,"") ) i++;
  if ( strcmp (Duf,"") ) i++;
  if ( strcmp (Coa,"") ) i++; 
  if ( i == 0 )
    return 2;           /* Ok they're all blank */

  if ( i != 3 )         /* a field is missing */ 
    return 0;           /* Error */

/* Must have 3 zeros, 1 or 2 is an error  */
/* 3 zeros indicates user wants to use burnup's default emis fac */
  i = 0; 
  if ( !stricmp (Fla,"0") ) i++;   
  if ( !stricmp (Duf,"0") ) i++;
  if ( !stricmp (Coa,"0") ) i++;

  if ( i == 1 || i == 2 )  /* Need none or 3 zeros */ 
    return 0;              /* Error */ 

  if ( i == 3 )    /* 3 zeros - User wants burnup's default emis factors  */ 
    return 3; 

  return 1;        /* Have all 3 Expaned Emis Factors */ 

}

/***************************************************************************
*
*
***************************************************************************/
int Wnd_Mai::FLM_SetLogSoil()
{
int iX, i; 

char cr_CovTyp[500],cr_FuelClass[100], cr_CovGrp[30];
d_CFL s_CFL; 
String ^ Str, ^ Log, ^ Soil; 

   Str = this->_cbFuelClass->Text;
   if ( Str != e_FLM )
     return 0; 

/* Get selected Cover Type item from combobox */
   Str = this->_cbCoverType->Text; 
  _StrToChar (Str, cr_CovTyp);

/* Get the fuels record from table (read in .cvs file) */
  iX = CVT_GetCFL ( cr_CovTyp, &s_CFL, cr_CovGrp);
  if ( iX < 0 ) {
     return 0;    /* Shouldn't happen, would be a logic error */
   }

  Log = _CharToStr (s_CFL.cr_FLMLogDist);
  Soil = _CharToStr (s_CFL.cr_FLMSoil); 

  i = this->_cbLogDist->FindString(Log);
  if ( i >= 0 ) 
     this->_cbLogDist->SelectedIndex = i; 

  i = this->_cbSoil->FindString(Soil);
  if ( i >= 0 )
	 this->_cbSoil->SelectedIndex = i;

	return 1; 
}


/*******************************************************
* Name: Emission_CSVFile
* Desc: Save Emissions to a .csv file 
* Note-1: Run FOFEM again just to make sure any edits user did to the input
*          fuel textboxs are reflected to the GUI output boxes, when we 
*          the CalcSub() it won't do that
*  In: cr_Mode.... "Short" for short version, only major emissions
*                  "Full" for full version
*                  "Summary"....does a row for each emis component
*      cr_Unit.....only used for Summary
*                  "English"  "Metric" 
********************************************************/
int Wnd_Mai::Emission_CSVFile (char cr_Mode[], char cr_Unit[])
{
int i; 
bool b_Exp; 
char cr_FN[1000], cr_ErrMes[1000], cr_EU[20]; 
String ^ Str;  

System::Windows::Forms::SaveFileDialog FD;

#define e_EmCSV  "csv files (*.csv)|*.csv|All files (*.*)|*.*"

  strcpy (cr_FN,""); 

/* Get Default or Expanded Emis Factor from Settings Wnd */ 
  b_Exp = false; 
  if ( this->set_frm->_rb_EmiNew->Checked == true )  
     b_Exp = true;

  if ( b_Exp == false && !stricmp (cr_Mode,"Full" ) ) {
     myMessageBox ("The menu item: \n    Emission CSV File - Full\n is only for use with the Expanded emission factors.");
     return 0; }
/* Get Emis output units from Settings Wnd */
  this->set_frm->EU_Unit(cr_EU); 


/* Pop Up a SaveFileDialog to get a file name */
/* Use the Path & File Name from the Project File TextBox */
  FD.Title = "Create FOFEM Emission File"; 
  FD.InitialDirectory = this->Prj_GetPath();  
 
  if  ( !stricmp (cr_Mode,"Short" ))
     Str = "Emission-Short"; 
  else if ( !stricmp (cr_Mode,"Full" ))
    Str = "Emission-Full"; 
  else if ( !stricmp (cr_Mode, "Summary") )
	Str = "Emission-Summary"; 
  else                          /* This shouldn't happen */
    Str = "Emission-WTF";

  if ( b_Exp == true )          /* Expanded Emission Factors */
     Str += "-Expanded"; 
  else
     Str += "-Default";      /* Default emssions factors */

  if ( !stricmp (cr_EU, "Pound" )) 
     Str += "-Pound.csv";
  else if ( !stricmp (cr_EU, "Ton") )
     Str += "-Ton.csv"; 
  else 
     Str += "-Grams.csv"; 

  FD.FileName = Str; 
  
  FD.Filter = e_EmCSV;
  i = (int) FD.ShowDialog(); 
  if ( i != 1 )
    return 0; 

  Str = FD.FileName;  /* Get Path + File Name from DialogBox */ 
 _StrToChar (Str,cr_FN);

/* Expanded Emis Factor Summary */
  if ( !stricmp (cr_Mode,"Summary") && b_Exp == true ) {
    i = Emission_CSVSumExpFac (cr_FN,cr_ErrMes);
	if ( i == 0 ) {
	  myMessageBox (cr_ErrMes);
	  return 0; }
    goto Calc; 
  }

/* Default Emis Factor Summary */
  if ( !stricmp (cr_Mode,"Summary") && b_Exp == false ) {
    i = Emission_CSVSum_DefFac (cr_FN,cr_ErrMes);
	if ( i == 0 ) {
	  myMessageBox (cr_ErrMes);
	  return 0; }
    goto Calc; 
  }

 if ( b_Exp == true ) {
   if ( !Emission_CSVMngr (cr_FN, cr_Mode, cr_ErrMes)) {
	 myMessageBox (cr_ErrMes);
     return 0; } }

 if ( b_Exp == false ) {
   if ( !Emission_CSVShort_DefFac (cr_FN, cr_ErrMes)) {
	 myMessageBox (cr_ErrMes);
     return 0; } }
 

Calc:
  this->Calculate("","");  /* See Note-1 above */ 

  return 1; 
 }


/************************************************************************
* Name: Duff_AdjDepth
* Desc: Adjust/Set the Duff Depth textbox when the user enters a User
*        Duff Amount. 
*       The user must select "User" from the adjustment popup menu, then
*        enter an amount of duff, after hitting return key the depth
*        will get adjusted and set according to the ratio that the 
*        selected cover type has between duff amount and depth, if there
*        is none a default ratio is used. 
* NOTE: The way I have it setup is so that whenever the user comes back
*        and does "User" enter amount again, the previous User settings
*        will show up, we could have set it to clear the amount or whatever
*        but just putting the previous amount and depth in is straight
*        foward and helps avoid all the "what ifs" 
**************************************************************************/
int Wnd_Mai::Duff_AdjDepth()
{
int  i, iX;
float f,  pc, f_Duff;
char cr_CovTyp[400], cr_CovGrp[100], cr_Out[500], cr_ErrMes[1000];
String ^A;
String ^B;
String ^ Str; 
d_CFL s_CFL;

    A = this->s_uDuff;            /* Saved User Entered Duff Amount */
    B = this->_txDuff->Text;      /* What's in Duff box */

    if ( A == B )                 /* Did user type in a duff amount */
      goto Ret;                   /*  No, so do nothing */

//   MessageBox::Show("Duff Loading Has Changed", "Title", MessageBoxButtons::YesNo ); 

/* Get selected Cover Type item from combobox */
   Str = this->_cbCoverType->Text; 
  _StrToChar (Str, cr_CovTyp);

/* Get the fuels record from table (read in .cvs file) */
  iX = CVT_GetCFL ( cr_CovTyp, &s_CFL, cr_CovGrp);
  if ( iX < 0 ) {
     myMessageBox ("Logic Error - Wnd_Mai::Duff_AdjDepth");
     return 0;  }  /* Shouldn't happen, would be a logic error */


  i = _Get_TBFlo (this->_txDuff, &f_Duff, cr_Out, "Duff Load", cr_ErrMes);

  if ( s_CFL.f_DufDep == 0 || s_CFL.f_Duff == 0 )  /* Look at Duff & Depth in Cover Fuel Record */
     f = f_Duff / e_DufDefRat;                     /* if none to use for a ratio use defualt */
  else {  
     pc = s_CFL.f_DufDep / s_CFL.f_Duff;           /* get a Duff to Dept ratio */
     f = f_Duff * pc; } 

  FloatToTextBox (this->_txDuffDepth, f, 2);
  
  this->s_uDuff = _FloToStr (f_Duff, 2);
  this->s_uDuffDep = _FloToStr (f,2); 

Ret:
  return 1; 
} 

/******************************************************************************
* Name: Duff_AdjAmount
* Desc: Adjust the Duff Amount (TPA) when the user changes the Duff Depth
*
*******************************************************************************/
int Wnd_Mai::Duff_AdjAmount()
{
int  i, iX;
float f,  pc, f_Dep;
char cr_CovTyp[400], cr_CovGrp[100], cr_Out[500], cr_ErrMes[1000];
String ^ Str; 
d_CFL s_CFL;


/* Get selected Cover Type item from combobox */
   Str = this->_cbCoverType->Text; 
  _StrToChar (Str, cr_CovTyp);

/* Get the fuels record from table (read in .cvs file) */
  iX = CVT_GetCFL ( cr_CovTyp, &s_CFL, cr_CovGrp);
  if ( iX < 0 ) {
     myMessageBox ("Logic Error - Wnd_Mai::Duff_AdjDepth");
     return 0;  }  /* Shouldn't happen, would be a logic error */

/* Get user entered Duff Depth from GUI */
  i = _Get_TBFlo (this->_txDuffDepth, &f_Dep, cr_Out, "Duff Load", cr_ErrMes);

  if ( s_CFL.f_DufDep == 0 || s_CFL.f_Duff == 0 )  /* Look at Duff & Depth in Cover Fuel Record */
     f = f_Dep / e_DufDefRat;                      /* if none to use for a ratio use defualt */
  else {  
     pc = s_CFL.f_Duff / s_CFL.f_DufDep;           /* get a Duff to Dept ratio */
     f = f_Dep * pc; }                             /* Calc new duff amt/tpa, based on depth */

  FloatToTextBox (this->_txDuff, f, 2);            /* Put new amt to duff textbox */
  
  this->s_uDuffDep = _FloToStr (f_Dep, 2);         /* Save the new Duff and Dept as */
  this->s_uDuff = _FloToStr (f,2);                 /*  current user values */

/* Now that we changed the Duff Amt/TPA, the Duff Textbox has to set as a "User" */
  this->_txDuff->BackColor = Color::FromArgb(e_UsrR,e_UsrG,e_UsrB);   /* Change color */
  this->_txDuff->ReadOnly = false;                                    /* write enable */

Ret:
  return 1; 
} 


/************************************************************************
* Name: MortRepTyp
* Desc: see what Mortality report type is selected by looking at the
*       prefire - post fire radio buttons and the post fire salvage checkbox 
************************************************************************/
int  Wnd_Mai::MortRepTyp()
{ 
 if ( PostFire_Checked() == false )  /* Not Post radiobutton */
   return e_RTpre;                   /* so it's prefire */

/* see what kind of post fire report */
 if ( this->_cb_SalRep->Checked == true )  /* salvage checkbox checked */ 
    return e_RTsal; 

 return e_RTpos; 
}


/*********************************************************************
* Name: SalvageSet
* Desc: When the "Salvage" report button is click we need to show
*       the Size Class Probability controls and adjust the Mortality
*       Grid to make room for them. 
*********************************************************************/
void Wnd_Mai::SalvageSet(char cr[])
{
   if ( !stricmp (cr,"Show") ) {
     this->_gbSC1->Show(); 
     this->_gbSC2->Show(); 
     this->_gbSC3->Show();
     this->_dgMort->Height = 198; }
   else {
     this->_gbSC1->Hide(); 
     this->_gbSC2->Hide(); 
     this->_gbSC3->Hide(); 
     this->_dgMort->Height = 254; }  
}
/********************************************************************
* Name: SalvageProb
* Desc: Set a Salvage Report 
*   In: i_SizCla...1,2,3
*       A,B........low and high limits of size clsss (DBH inches)
*                  Ex: 4 -> 20  size class would include 4 inch DBH
*                  20 is not included, on values less than 20 would
*                  be included in the size class
*       Prob.....  Probability value 0.0 -> 1.0 
********************************************************************/
void Wnd_Mai::SalvageProb(int i_SizCla, float A, float B, float Prob)
{
  if ( i_SizCla == 1 ) {
    FloatToTextBox (this->_txSC1A, A, 0);
    FloatToTextBox (this->_txSC1B, B, 0);
    FloatToTextBox (this->_txSC1C, Prob, 1); }

 if ( i_SizCla == 2 ) {
    FloatToTextBox (this->_txSC2A, A, 0);
    FloatToTextBox (this->_txSC2B, B, 0);
    FloatToTextBox (this->_txSC2C, Prob, 1); }

 if ( i_SizCla == 3 ) {
    FloatToTextBox (this->_txSC3A, A, 0);
    FloatToTextBox (this->_txSC3B, B, 0);
    FloatToTextBox (this->_txSC3C, Prob, 1); }
}

/*********************************************************
* Name: SalvageDefault
* Desc: Set default Salvage Report Size Classes and 
*       probabilty values 
**********************************************************/
void Wnd_Mai::SalvageDefault()
{
   SalvageProb(1, 0, 12,  0.4);
   SalvageProb(2, 12, 20, 0.6);
   SalvageProb(3, 20, 99, 0.8);
}


