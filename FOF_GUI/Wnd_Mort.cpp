/***************************************************************
* Name: Wnd_Mort.cpp
* Desc: GUI, high level related stuff to deal with the 
*        mortality features
*
***************************************************************/
#include <stdio.h>
#include <math.h> 
#include <string.h>

#include "Wnd_Mai.h" 
#include "Wnd_Def.h"
#include "Wnd_Util.h"   
#include "fof_smt.h"
#include "fof_util.h"
#include "fof_pf2.h" 
#include "fof_mrt.h"

int _GetEquTyp (char In[], char Out[]);

String ^ _SetMissing (String ^S);

int PFI_Calc_OLD(d_PFI *a, char cr_ErrMes[])
{
 return 1; 
}

/***********************************************
*
**************************************************/
int Wnd_Mai::Mort_Graph()
{
   if ( this->PostFire_Checked() )
     this->Mort_GraphPFI();
   else 
     this->Mort_GraphReg(); 

  return 1; 
} 
/*****************************************************
*
**
***************************************************/
int Wnd_Mai::Mort_GraphPFI()
{
int i,  iN; 
float f, f_CR, f_Prob, f_PMCut;    
float fr_CR[500], fr_Prob[500];
char cr_Out[100], cr_ErrMes[500], cr_Spe[50], cr[500]; 
d_MO MO; 
String ^ Str; 
d_MIS  MIS; 

#define e_CR_Lim 100.0
#define e_CR_Inc 10.0

/* clear first incase there's an error, DL wants it cleared */ 
    this->mort_Dam->_chMort->Series->Clear(); 

// NOTE NOTE check what kind of error messages comes back from this 
   i =  this->MIS_Load(cr_ErrMes); 
   if ( i == 0 )  {
     myMessageBox (cr_ErrMes); 
     return 0; }

   f_PMCut = 0.5;  /* draws a red horizontal line on grahp  */

  this->mort_Dam->InitGraphPFI();
  iN = 0; 

  for ( i = 0; i < 1000; i++ ) {    /* each record in the Tree Grid */
    if ( !MIS_Get (i, &MIS) )        /* no more */  
      break; 
    if ( stricmp (MIS.cr_EquTyp, e_CroDam) ) /* only want crown damage */
       continue; 
    if ( MIS.b_Graph == false )      /* did user click graph checkbox */  
      continue; 
 
    iN = 0;   
    for (f_CR = 0; f_CR <= e_CR_Lim; f_CR = f_CR + e_CR_Inc) {
       MIS.f_CrnDam = f_CR;                /* Crown Damage % go across X axis            */
       f_Prob = MRT_CalcMngr(&MIS,&MO,cr_ErrMes);
       if ( f_Prob < 0 ) {
         myMessageBox(cr_ErrMes); 
         return 0; }
       fr_Prob[iN] = f_Prob;
       fr_CR[iN] = f_CR; 
       iN++; 
    }  /* for f_CR */ 

    fr_Prob[iN] = -1.0;                                  /* Mark end of array */
    fr_CR[iN] = -1.0; 
// 11-1-16 Ticket-101  add the CKR to the legend text 
    sprintf (cr, "%d-%s-%1.0f-%1.0f", i+1, MIS.cr_Spe,MIS.f_DBH,MIS.f_CKR);     /* form a name for Axis Label, Row # Species DBH */
// 3-17-15 DL request Ticket-1   sprintf (cr, "%s-%2.0f", PFI.cr_Spe,PFI.f_DBH);     /* form a name for Axis Label */
 
   Str = _CharToStr (cr); 
    this->mort_Dam->UpdateGraph("PFI", fr_Prob,fr_CR, Str, i + 1); /* Go graph it */
  }  /* for i */ 

/*------------------------------------------------*/
/* Draw the Probability Mortality Cutoff Horizontal Line */
/* I just use the UpdateGraph() to draw a straight line */
/* I add that 0.01 to avoid the line being drawn right on */
/*  top of an axis line if user sets Cutoff at 0.2, 0.4, etc */ 
  if ( iN == 0 )   /* No was drawn on the graph */  
    return 1; 

/* Don't draw red probability line un Salvage Report box is checked  */
  if ( this->_cb_SalRep->Checked == false ) 
    return 1; 

  for ( i = 0; i < iN; i++ )
    fr_Prob[i] = f_PMCut + 0.005; 
 
   sprintf (cr,"CutOff %2.1f", f_PMCut);
   Str = _CharToStr (cr); 
   this->mort_Dam->UpdateGraph("PFI",fr_Prob,fr_CR, Str , -1); /* Go graph it */
 
  return 1; 
}

/*****************************************************
* Name: Mort_GraphBol
* Desc: Bole Char Height Graph 
***************************************************/
int Wnd_Mai::Mort_GraphBol()
{
int i,  iN; 
float f, f_CR, f_Prob, f_PMCut;    
float fr_CR[500], fr_Prob[500];
float f_max, f_intv;

char cr_Out[100], cr_ErrMes[500], cr_Spe[50], cr[500]; 
d_MO MO; 
String ^ Str; 
d_MIS  MIS; 

   f_max = 30.0;   /* Maximum Bole Char height on X axis */
   f_intv = 2.0;    /* Interval plot on X axis */

/* clear first incase there's an error, DL wants it cleared */ 
   this->mort_Bol->_chMort->Series->Clear(); 

   i =  this->MIS_Load(cr_ErrMes);  /* load in trees from GUI grid */
   if ( i == 0 )  {
     myMessageBox (cr_ErrMes); 
     return 0; }

  this->mort_Bol->InitGraphBol(f_max, f_intv);
  iN = 0; 

  for ( i = 0; i < 1000; i++ ) {    /* each record in the Tree Grid */
    if ( !MIS_Get (i, &MIS) )        /* no more */  
      break; 
    if ( stricmp (MIS.cr_EquTyp, e_BolCha) ) /* only want Bole Char  */
       continue; 
    if ( MIS.b_Graph == false )      /* did user click graph checkbox */  
      continue; 
 
    iN = 0;   
    for (f = 0; f <= f_max; f += f_intv) {
       MIS.f_BolCha = f;                /* Bole Char height go across X axis            */
       f_Prob = MRT_CalcMngr(&MIS,&MO,cr_ErrMes);
       if ( f_Prob < 0 ) {
         myMessageBox(cr_ErrMes); 
         return 0; }
       fr_Prob[iN] = f_Prob;
       fr_CR[iN] = f; 
       iN++; 
    }  /* for f_CR */ 

    fr_Prob[iN] = -1.0;                                  /* Mark end of array */
    fr_CR[iN] = -1.0; 
// 11-1-16 Ticket-101  add the CKR to the legend text 
// orig    sprintf (cr, "%d-%s-%1.0f-%1.0f", i+1, MIS.cr_Spe,MIS.f_DBH,MIS.f_CKR);     /* form a name for Axis Label, Row # Species DBH */

    sprintf (cr, "%d-%s-%1.0f", i+1, MIS.cr_Spe,MIS.f_DBH);     /* form a name for Axis Label, Row # Species DBH */

// 3-17-15 DL request Ticket-1   sprintf (cr, "%s-%2.0f", PFI.cr_Spe,PFI.f_DBH);     /* form a name for Axis Label */
 
   Str = _CharToStr (cr); 
    this->mort_Bol->UpdateGraph("PFI", fr_Prob,fr_CR, Str, i + 1); /* Go graph it */
  }  /* for i */ 

/*------------------------------------------------*/
/* Draw the Probability Mortality Cutoff Horizontal Line */
/* I just use the UpdateGraph() to draw a straight line */
/* I add that 0.01 to avoid the line being drawn right on */
/*  top of an axis line if user sets Cutoff at 0.2, 0.4, etc */ 
//  if ( iN == 0 )   /* No was drawn on the graph */  
//    return 1; 

//  for ( i = 0; i < iN; i++ )
//    fr_Prob[i] = f_PMCut + 0.005; 
 
//   sprintf (cr,"CutOff %2.1f", f_PMCut);
//   Str = _CharToStr (cr); 
//   this->mort_Bol->UpdateGraph("PFI",fr_Prob,fr_CR, Str , -1); /* Go graph it */
 
  return 1; 
}

/*******************************************************
* Name: Mort_GraphReg
* Desc: Graph the Regular Mortality 
*       Here we get and check the Spe DataGrid, calc
*       an array of mortality percents and then send
*       them to the mort graph form to do the actual
*       graphing. 
*******************************************************/
int Wnd_Mai::Mort_GraphReg()
{
int i,  iN; 
float f, fl, f_Prob,  Prob[1000], FS[1000];   
char cr_Out[100], cr_ErrMes[500], cr_Spe[50], cr[100];
char cr_FS[50]; 
d_MIS MIS; 
// d_MI MI; 
d_MO MO; 
// d_ISS ISS; 
String ^ Str; 

#define e_LstFla 20.0
#define e_IncFla 0.1 //   original  0.5 

  if ( this->PostFire_Checked() ){   /* Postfire Injury checkbox is checked */
   myMessageBox ("Logic Error - Mort_GraphReg()"); 
   return 0;}                         /* make sure not called */


  if ( this->_rbFlameLen->Checked )   /* Tells us if Flame Length */
    strcpy (cr_FS,"FlameLength");      /* or the Scorch Height Radio */
  else                                /* button is checked */
    strcpy (cr_FS,"ScorchHeight");


/* 3-5-2015, moved this up here from below so that if there was */
/* a bad input in the tree grid the graph clears, DL requested */
  this->mort_frm->InitGraph(cr_FS);         /* clear last graph, and init */


/* Load the GUI Mort Spe Grid into MIS table */
   i = MIS_Load(cr_ErrMes);
   if ( i <= 0 ) {
     myMessageBox (cr_ErrMes);
     return 0; }

/* OK - GUI Inputs look good, so we're ready to graph */
// I moved this above
//   this->mort_frm->InitGraph(cr_FS);         /* clear last graph, and init */

// not sure what this did or why I took it out ?
//  this->mort_frm->Set_SecondAxis();    /* so graph will have a Scorch Hgt Axis */

  for ( i = 0; i < 1000; i++ ) {       /* Each Spe in DateGrid */
    if ( !MIS_Get (i, &MIS) )  
      break;
    if ( MIS.b_Graph == false )        /* did user select it for graphing */
       continue;
    if ( stricmp (MIS.cr_EquTyp,e_CroSco) )   /* Only Want Crown Scorch Eqs */ 
      continue; 

    strcpy (MIS.cr_FS,e_Scorch);
    strcpy (MIS.cr_FirSev,""); 
    if ( this->_chLowFireSev->Checked )
      strcpy (MIS.cr_FirSev,"Low"); 

    MO_Init (&MO);                  /* better do this even though not used */
    iN = 0; 
 
/* TEST ---> Get the Probibitly Mortality across a span of Scorch Heights  */
/*  and load up arrays with values */
    for ( fl = 0; fl < e_LstFla; fl += e_IncFla ){ /* run thru flame lens */
      MIS.f_FS = Calc_Scorch(fl);                                    
      f_Prob = MRT_CalcMngr(&MIS, &MO, cr_ErrMes) ;
      Prob[iN] = f_Prob * 100.0 ;            /* This will be the Y axis point */
      if ( !stricmp (cr_FS, "FlameLength"))  /* This will be the X axis point */
        FS[iN] = fl;
      else
        FS[iN] = MIS.f_FS; 
      iN++;  }

    Prob[iN] = -1.0;                                  /* Mark end of array */
    FS[iN] = -1.0; 
    sprintf (cr, "%d-%s-%2.0f", i+1, MIS.cr_Spe,MIS.f_DBH);     /* form name for Axis Label, Row# Species DBH */
//   sprintf (cr, "%s-%2.0f", MI.cr_Spe,MI.f_DBH);     /* form a name for Axis Label */
 
    Str = _CharToStr (cr); 
    this->mort_frm->UpdateGraph("Reg", Prob,FS, Str, i + 1); /* Go graph it */

  } /* for i - each Species  */

  return 1; 
} 

/****************************************************************
* Name: Put_SpeRec
* Desc: Put a Row into the Species Mortality grid
****************************************************************/
int Wnd_Mai::Put_SpeRec( char Spe[], char EquTyp[], char Den[], char DBH[], char Hgt[], char CR[],
     char CrnDam[], char CKR[], char BeeDam[], char BolCha[])
{ 
int ix; 
String ^ Str; 
  ix = this->_dgMort->Rows->Add();

  Str = _CharToStr (Spe);  
  this->_dgMort->Rows[ix]->Cells[edg_Spe]->Value = Str ; 

/* Set Equation Type Combobox  */
/* NOTE -> we set this twice, the first one causes the ComboBox  */
/* Event Handler to go off and sets up the combobox with the     */
/* proper Equation Types, the second set actually causes the one */
/* we want to be selected */ 
  Str = _CharToStr (EquTyp);
  this->_dgMort->Rows[ix]->Cells[edg_Equ]->Value = Str; 
  Str = _CharToStr (EquTyp);
  this->_dgMort->Rows[ix]->Cells[edg_Equ]->Value = Str; 

/* Now we can set the Required Column colors for the selected */
/*  Equation Type selected above */
  this->Set_ReqCols(ix); 


/* Crown Scorch Inputs  */
  Str = _CharToStr (Den);
  this->_dgMort->Rows[ix]->Cells[edg_Den]->Value = _SetMissing (Str) ;

  Str = _CharToStr (DBH);
  this->_dgMort->Rows[ix]->Cells[edg_DBH]->Value = _SetMissing (Str);

  Str = _CharToStr (Hgt);
  this->_dgMort->Rows[ix]->Cells[edg_Hgt]->Value = _SetMissing (Str) ;

  Str = _CharToStr (CR);
  this->_dgMort->Rows[ix]->Cells[edg_CR]->Value = _SetMissing (Str);

/* Crown Damage inputs */ 
  Str = _CharToStr (CrnDam);
  this->_dgMort->Rows[ix]->Cells[edg_CrnDam]->Value = _SetMissing (Str);

  Str = _CharToStr (CKR);
  this->_dgMort->Rows[ix]->Cells[edg_CKR]->Value = _SetMissing (Str) ;

  Str = _CharToStr (BeeDam);
  this->_dgMort->Rows[ix]->Cells[edg_BeeDam]->Value = Str ;

/* Bolchar height */  
  Str = _CharToStr (BolCha);
  this->_dgMort->Rows[ix]->Cells[edg_BolCha]->Value = _SetMissing (Str) ;

  return 1; 
}

/****************************************************************
* Mortality Species Grid values coming in from the project 
* file could have been saved with missing column values, 
* this detects them and returns a "" string 
****************************************************************/
String ^ _SetMissing (String ^S)
{
String ^ s;
  if ( S == "-1" )
    s = "";
  else if ( S == "-1.0" )
   s = "";
  else 
   s = S;
  return s; 
}

/****************************************************
* Name: Mort_Select
* Desc: Mortality Tab Window - The Select Button was
*       clicked so we need to move the species code from 
*       the Species ComboBox to the DataGridView
*       species column. 
*  Ret: row number of insert 
****************************************************/
int Wnd_Mai::Mort_Select()
{
int i, j, iN; 
char cr[500], A[100], cr_EquTyp[100]; 
String ^ Str;
DataGridView ^dg; 

 // if ( this->PostFire_Checked() )   /* Postfire Injury checkbox is checked */
 //   dg = this->_dgPFI;
 // else 
    dg = this->_dgMort;               /* Regular Mortality */

  Str = this->_cbSpecies->SelectedItem->ToString(); /* Get selected Species */ 
 _StrToChar(Str,cr);  
  sscanf (cr, "%s", A);                   /* Pull out just the species code */
  Str = _CharToStr(A);                     /* into a String */

/* Save Equation Type for the Selected Species, Equ Typ is in the Item String */
  _GetEquTyp (cr, cr_EquTyp);               /* Parse it out of string */
  this->s_EquTyp = _CharToStr(cr_EquTyp);   /* Pass it */

/*----------------------------------*/ 
/* Look for the 1st row with an empty Species column */
/* and insert the Species, if none found then add    */
/* a new row and put the species in it */
  dg->Rows->Add(1); 

  while (1) {
  iN = 0; 
  j = dg->Rows->Count; 

    for ( i = 0; i < j; i++ ) { 
      if ( String::IsNullOrEmpty(dg->Rows[i]->Cells[edg_Spe]->FormattedValue->ToString())) {
        dg->Rows[i]->Cells[edg_Spe]->Value = Str; 
        iN++;  
        break;}  }

    if ( iN > 0 ) 
      break; 
 // dg->AllowUserToAddRows = false; 
 // dg->Rows->Add(1);           
 } /* while */

  return i;   /* return row */

}

/********************************************************
* Name _GetEquTyp
* Desc: Pull out the Equation Type from a char string
*        
*********************************************************/
int _GetEquTyp (char In[], char Out[])  
{
int i,j,k;
char  cr[100]; 
  strcpy (Out,e_CroSco);  /* incase none found */ 
  j = 0; 
  for ( i = 0; i < 2000; i++ ) {
    if ( In[i] == NULL ) 
      return 0 ;
    if ( In[i] == '(' )  /* Equ Typ starts here */
      break; 
  } /* for */

  i++; 
  for ( j = 0; j < 100; j++ ){ 
    if ( In[j] == NULL ) 
      return 0;           /* Shouldn't happen */ 
    cr[j] = In[i++];
    if ( cr[j] == ')' )
      break ; 
  } /* for j */

  cr[j] = 0; 
  strcpy (Out,cr); 
  return 1; 
}

/*************************************************************
* Name: Mort_LoadSpeMstLst()
* Desc: Load Species into the Species Master List CombBox
* Note-1: Records in the fof_spp.csv input file that are 
*         designated with e_PFIO ("P") are not to be put
*         into the list of prefire species. The e_PFIO records
*         are to be display in the post-fire species list only
*
*************************************************************/
int Wnd_Mai::Mort_LoadSpeMstLst()
{
int i, iN, i_Reg, a,b; 
char cr[1500], cr_ErrMes[1000], cr_Fmt[100], A[300];
char cr_EquTyp[100], cr_ReqFld[100];
String ^ Str; 
d_SMT SMT; 

 _StrToChar(this->s_UserFolder,cr);
  strcat(cr,"\\");
  strcat(cr,e_FOF_Spp); 

  i = MRT_LoadSpe (cr,"",cr_ErrMes);
  if ( i == 0 ) {
    myMessageBox (cr_ErrMes);
    return 0; }

  if ( this->_cbRegion->Text == e_Pacific_West )
    i_Reg = eN_Pacific_West;

  else if ( this->_cbRegion->Text == e_Interior_West ) 
    i_Reg = eN_Interior_West;

  else if ( this->_cbRegion->Text == e_North_East )
    i_Reg = eN_North_East;
       
  else if ( this->_cbRegion->Text == e_South_East )
    i_Reg = eN_South_East;
       
  else {
    myMessageBox ("Logic Error - Mort_Init");
    i_Reg = eN_Interior_West;}

  this->_cbSpecies->Items->Clear(); 

  iN = 0;
  for ( i = 0; i < 1000; i++ ) {
    if ( !SMT_Get (i, &SMT))
      break;

    if ( !SMT_ChkReg (&SMT, i_Reg) )  /* Check Region */ 
       continue; 

  
    EFR_Get (SMT.cr_MrtEqu, cr_ReqFld);

   _EquTyp (SMT.cr_MrtEqu, cr_EquTyp);

   sprintf (A,"%s, %s", SMT.cr_Name,SMT.cr_ComNam); 
   sprintf (cr, "%-6s %-30s (%-4s) %-s", SMT.cr_Spe, A, cr_EquTyp, cr_ReqFld);
   
    Str = _CharToStr(cr); 
    this->_cbSpecies->Items->Add(Str);  /* put into combobox */
    iN++;
  } 

  if ( iN > 0 ) { 
    this->_cbSpecies->SelectedIndex = 0; 
    goto Ret; } 

  if ( this->_tcpMortality->Visible == true ) {
     strcpy (cr, "There are no Post Fire Injury Mortality equations available for\n");
     strcat (cr, "the selected region.\n");
     strcat (cr, "You are receiving this message because you have the Post Fire \n");
     strcat (cr, "Injury checkbox selected on the Mortality tab. \n");
     strcat (cr, "However if you previously entered post fire data it will be placed \n"); 
     strcat (cr, "into the data grid at this time. \n"); 
     myMessageBox (cr); }
  
Ret:
  return 1;
}

/******************************************************************************
* Add 5/23/12
* DL made this table of Equation mortality numbers and Ids with their
* names, the names have now been added to the mortality reports,
* This was an easy way to let users know that hardwood tree use confir 
*  equations and which one. 
******************************************************************************/

typedef struct {
   char cr_Id[10];
   char cr_Name[100];
    } d_MIN;

d_MIN sr_MIN[] = {

/* Scorch Height - (Prefire equations) */
        { "1",     "Other - based on conifer species" },
        { "3",     "Spruces - other than Engelmann" },
        { "4",     "Quaking aspen" },
        { "5",     "Longleaf pine" },
        { "10",    "White fir" },
        { "11",    "Subalpine fir and grand fir" },
        { "12",    "Incense cedar" },
        { "14",    "Western larch" },
        { "15",    "Engelmann spruce" },
        { "16",    "Red fir" },
        { "17",    "Whitebark pine and lodgepole pine" },
        { "18",    "Sugar pine" },
        { "19",    "Ponderosa pine and Jeffery pine" },
        { "20",    "Douglas-fir" },
        { "21",    "Ponderosa pine - Black Hills"}, 

/* Crown Damage  (Postfire equations) */
        { "WF",   "White fir" },
        { "SF",   "Subalpine fir and grand fir" },
        { "IC",   "Incense cedar" },
        { "WL",   "Western larch" },
        { "WP",   "Whitebark pine and lodgepole pine" },
        { "ES",   "Engelmann spruce" },
        { "SP",   "Sugar pine" },
        { "RF",   "Red fir" },
        { "DF",   "Douglas-fir" },
        { "PP",   "Ponderosa/Jeffery pine crown scorch" },
        { "PK",   "Ponderosa/Jeffery pine crown killed" },

/* Bole Char */
        { "100", "Red maple"}, 
        { "101", "Flowering dogwood"},
        { "102", "Blackgum"},
        { "103", "Sourwood"},
        { "104", "White oak"},
        { "105", "Scarlet oak"},
        { "106", "Blackjack oak"},
        { "107", "Chestnut oak"}, 
        { "108", "Black oak"},
        { "109", "Sassafras"},

        { "", "" }};


/**********************************************************
* Get the Equations Number/Id's name
*
***********************************************************/
int Wnd_Mai::Mort_GetName(char cr_Id[], char cr_Name[])
{
int i;
   strcpy (cr_Name,""); 
   for ( i = 0; i < 1000; i++ ) {
     if ( !stricmp (sr_MIN[i].cr_Id,"") )   /* End of table, shouldn't happen */
       break; 

     if ( stricmp (sr_MIN[i].cr_Id,cr_Id) )
        continue; 

     strcpy (cr_Name, sr_MIN[i].cr_Name);  /* Match found */ 
     return 1; 
   }

  strcpy (cr_Name, "Fix This No Name Assigned"); 
  return 0;
}

/********************************************************************
* Name: Set_EquComBox
* Desc: Set the Equation Type ComboBox items for Species code on 
*        the same row. 
*       The species could have 1,2 or 3 different mortality equations 
*        associated with it, Crown Scorch (prefire) Crown Damage or
*        Bole Char. 
* NOTE: Even though the Grid was setup with a ComboBox column for the
*       Equation Type, those comboboxes can NOT be individually 
*       loaded with different items, so we have to create a new
*       ComboBoxCell, load the items we want and then attach it 
*       to the grid for that row.   
********************************************************************/
int Wnd_Mai::Attach_EquTypComBox (int row, int col, char cr_Mode[] )
{
int i, a,b,c; 
char cr[1000], cr_Spe[100], cr_MrtEqu[100];
String ^Str;
// d_EFR s_EFR; 
DataGridViewComboBoxCell ^comboCell ;

   strcpy (cr_Mode,""); 

// NOTE NOTE - try putting this down after we know we have a valid species
   comboCell = gcnew DataGridViewComboBoxCell();

   if ( col != edg_Spe )   /* make sure we're on a Species cell change */
      return 0;            /*  caller should have check */

/* Get the Species from specified row */
    Str = this->_dgMort->Rows[row]->Cells[edg_Spe]->Value->ToString(); 
   _StrToChar (Str,cr_Spe);

/* If Species is part of a Type put the Type(Item) into the ComboBox*/
   a = SMT_MortEqu_Code (cr_Spe, e_CroSco, cr_MrtEqu); /* Crown Scorch Type */
   if ( a == 1 ) 
      comboCell->Items->Add (e_CroSco);

   b = SMT_MortEqu_Code (cr_Spe, e_CroDam, cr_MrtEqu); /* Crown Damage Type */
   if ( b == 1 ) 
      comboCell->Items->Add (e_CroDam);

   c = SMT_MortEqu_Code (cr_Spe, e_BolCha, cr_MrtEqu); /* Bole Char Type */
   if ( c == 1 ) 
     comboCell->Items->Add (e_BolCha);

   if ( a == 0 && b == 0 && c == 0 ) { 
     sprintf (cr,"Invalid Species Entered: %s",cr_Spe);
     myMessageBox (cr);
     return 0; }  

/* Add(replace) the New ComboBox the the Grid */
    this->_dgMort->Rows[row]->Cells[edg_Equ] = comboCell; 

/* If an Equ Type was put in this->s_EquTyp that means */
/*  the species was put into the Datagrid with the Move --> button */
/*  so we already know and have what Item(Equ Typ) needs to be selected */
/*  in the Equ Typ combobox  */
   if ( this->s_EquTyp != "" ) {  
     Str = this->s_EquTyp;        /* we have Equ Type to use */
     this->s_EquTyp = "";
     strcpy (cr_Mode,"Move"); 
     this->_dgMort->Rows[row]->Cells[edg_Equ]->Value = Str;
     goto Ret; } 

/* set the Selected Item */
    if ( a  == 1 ) 
       this->_dgMort->Rows[row]->Cells[edg_Equ]->Value = e_CroSco;
    else if ( b == 1 ) 
       this->_dgMort->Rows[row]->Cells[edg_Equ]->Value = e_CroDam;
    else if ( c == 1 ) 
       this->_dgMort->Rows[row]->Cells[edg_Equ]->Value = e_BolCha;

    strcpy (cr_Mode, "Hand"); 
 //  Str =   this->_dgMort->Rows[row]->Cells[edg_Equ]->FormattedValue->ToString();
Ret:
   return 1;
}


/******************************************************************
* Name: Set_ReqCols   Set Required Columns background colors
*
* Note-1: The Crown Damage type mortality codes use a variety of
*          inputs so we need to look them up by their equation codes
*         Crown Scorch type equations all use the same inputs, and 
*         Bole Char always uses it's same two inputs dbh and bole
*         char height
******************************************************************/
int Wnd_Mai::Set_ReqCols (int r )
{
int i; 
char cr[1000], cr_EquTyp[200], cr_ReqFld[100], cr_Spe[100], cr_MrtEqu[100]; 
String ^ s_Spe;
String ^ s_EquTyp; 
String ^ Str; 

/* Get Species Code from Species Grid GUI */
  s_Spe = this->_dgMort->Rows[r]->Cells[edg_Spe]->FormattedValue->ToString();
  if ( s_Spe == "" ) 
      return 0; 
  _StrToChar (s_Spe,cr_Spe);  

/* Get the Equation Type from the Combo Box */
// s_EquTyp = this->_dgMort->Rows[r]->Cells[edg_Equ]->FormattedValue->ToString();
  s_EquTyp = this->_dgMort->Rows[r]->Cells[edg_Equ]->EditedFormattedValue->ToString();
 _StrToChar (s_EquTyp,cr_EquTyp); 
 

// sprintf (cr, "%s %s", cr_Spe, cr_EquTyp);
// myMessageBox(cr); 
// test .................


/* if Equation Type is Crown Scorch (pre-fire) get it's Required Field String */
  if ( !stricmp (cr_EquTyp,e_CroSco) ) {       
    EFR_Get (e_CroSco, cr_ReqFld);
    goto SetColor; }

/* if Equation Type is Bole Char get it's required Field String */
  if ( !stricmp (cr_EquTyp,e_BolCha)) {      /* Bole Char */
    EFR_Get (e_BolCha, cr_ReqFld);
    goto SetColor; }

/* if Equations Type is Crown Damage type See Note-1 above */
  if ( !SMT_MortEqu_Code (cr_Spe, e_CroDam, cr_MrtEqu)) { /* Need to get it's Mort Equ Code */
    sprintf (cr, "Invalid Species: %s (row %d)",cr_Spe,r);
    myMessageBox (cr);
    return 0; }

/* Get Crown Damage type Reguired Field string based on it's Mortality Code */
  EFR_Get (cr_MrtEqu, cr_ReqFld); 


SetColor:
   Set_Color (r, edg_DBH, false);  
   Set_Color (r, edg_Hgt, false);  
   Set_Color (r, edg_CR, false);   
   Set_Color (r, edg_CrnDam, false); 
   Set_Color (r, edg_CKR, false);   
   Set_Color (r, edg_BeeDam, false); 
   Set_Color (r, edg_BolCha, false); 



   if ( strstr (cr_ReqFld, "dbh" ) )    
      Set_Color (r, edg_DBH, true);  
   if ( strstr (cr_ReqFld, "ht" ) )
      Set_Color (r, edg_Hgt, true);  
   if ( strstr (cr_ReqFld, "cr" ) )   
      Set_Color (r, edg_CR, true);   
   if ( strstr (cr_ReqFld, "len" ) )  
      Set_Color (r, edg_CrnDam, true); 
   if ( strstr (cr_ReqFld, "vol" ) )  
      Set_Color (r, edg_CrnDam, true); 
   if ( strstr (cr_ReqFld, "kil" ) )  
      Set_Color (r, edg_CrnDam, true); 
   if ( strstr (cr_ReqFld, "ckr" ) ) 
      Set_Color (r, edg_CKR, true);   
   if ( strstr (cr_ReqFld, "btl" ) ) 
     Set_Color (r, edg_BeeDam, true); 
   if ( strstr (cr_ReqFld, "bc" ) ) 
    Set_Color (r, edg_BolCha, true); 

   return 1; 
}

/****************************************************************
*
****************************************************************/
int Wnd_Mai::Set_Color (int r, int c, bool b)
{
System::Drawing::Color  cc; 

  if ( b == true ) 
    cc = Color::White; 
  else
    cc = Color::DarkGray;
  this->_dgMort[c,r]->Style->BackColor = cc; 
  return 1; 
} 

