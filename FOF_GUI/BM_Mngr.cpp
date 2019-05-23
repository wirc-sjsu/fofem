/********************************************************************
* Name: BM_Mngr
* Desc: high level functions to run and display Massman model 
* 
*********************************************************************/
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include <iostream>
#include <process.h> 
#include <setjmp.h> 
#include <time.h> 

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;

#include "Wnd_Util.h" 
#include "Wnd_BM.h"
#include "..\FOF_DLL\BM_Soil\HTAA.h"
#include "..\FOF_DLL\fof_sha.h"
// #include "..\FOF_DLL\BMSoil.h"

void  _GetRunTim (double d_delt, double jstep, char cr[]);

 void  FTBM_Message ();

int  _SetJump (char cr_ErrMes[]);
float MaxFirInt (float td);
float round (float f);

int Boundary_Burnup(d_BMI *bmi);
int _MinHrs (int i_Hrs);

int gi_Series = 0; 

extern char gcr_FuncName[]; 
extern char gcr_JumpMess[]; 
extern double bcta[]; 
extern double bcQ[]; 
extern double jstep ; 
extern double nsteps;

// d_BMI s_BMI; 

int gi_LastT; 


/*******************************************************
* Set the FireType combobox to Burnup 
********************************************************/
int Form1::SetFireTypeBurnup ()
{
   _SetComboBox (this->_cb_FireType, e_FT_Burnup); 
  return 1; 
}

/*******************************************************
* Return the number of 'runs' that have been done by the
* user, basically how many times the user hit the 
* "Start" button to perform a simulations 
* This function can be used to determine if the use
*  has done any simulations yet 
********************************************************/
int Form1::NumRuns()
{
  return this->iN_Runs;
}
/*********************************************************
*
* Note-1:  Get it up to the next hour if it a little past
*          an even hour(s) number, so if it's 6.0 leave 
*          it alone, if it's 6.1 make it 7.0
**********************************************************/
int  Form1::Burnup()
{
int i,iX, i_Tim,  i_Hr;
float f, f_Wts, f_EndTim, f_WtsMax, f_WtsMaxTim;
String ^ Str; 
char cr[100];
bool b; 

/* Determine how many minutes the burnup intensity array is */
//  i_EndTim;

  f_WtsMaxTim = 0;
  f_WtsMax = 0; 
  for ( iX = 0; iX < 1000000; iX++) {
    i = SHA_TP_Get (iX, &i_Tim, &f_Wts);
    if ( i_Tim < 0 )
      break;

    if ( f_Wts > f_WtsMax ) {
      f_WtsMax = f_Wts;
      f_WtsMaxTim = i_Tim; }

    f_EndTim = i_Tim;  }

  f =  f_EndTim / ( 60 * 60 ); /* get seconds to hours */
  f = f + 0.98;                /* See Note-1 above */                                  
  i_Hr = (int) f ;  
  sprintf (cr, "%d.0",i_Hr); 
  Str = _CharToStr(cr); 
  
  if ( this->_cbSimTime->Items->Contains(Str) == false )  /* if not already Simulation Time combobox */
    this->_cbSimTime->Items->Add(Str);                    /* Add it in */
  _SetComboBox (this->_cbSimTime,Str);                    /* Now select it */ 
  
  this->_ud_XMax->Text = Str; 

//   this->_gbFirInt->Enabled = false; 
  this->_txQabs->Enabled = false;
  this->_txMaxTim->Enabled = false; 
  this->_txFirDur->Enabled = false; 

//..........................
  sprintf (cr,"%5.2f",f_WtsMax/1000.0);
  Str = _CharToStr(cr); 
  this->_txQabs->Text = Str; 
 
  sprintf (cr,"%5.2f",f_WtsMaxTim / (60 * 60)); 
  Str = _CharToStr(cr); 
  this->_txMaxTim->Text = Str; 

  sprintf (cr,"%5.2f",f_EndTim / ( 60 * 60) ); 
  Str = _CharToStr(cr); 
  this->_txFirDur->Text = Str; 


//............................

  this->BM_Run("FirIntGraph"); 
  return 1; 
}


/*****************************************************************
* Name: FireTypeSet
* Desc: Fire Type Combobox has changed, so adjust the related
*       controls
*****************************************************************/
int Form1::FireTypeSet()
{
char cr[100]; 
String ^ Str;

  this->_txQabs->Enabled = true;
  this->_txMaxTim->Enabled = true; 
  this->_txFirDur->Enabled = true ; 


  Str = _GetComboBox (this->_cb_FireType, cr); /* Fire Type selected */

  if ( Str == e_FT_Burnup ) {                   /* Burnup output */
    if ( this->i_FTBM == 0 ) {   /* The first time only user selects Burnup fire type - show message  */
      FTBM_Message ();
      this->i_FTBM = 1; }
    this->Burnup(); }

/* Just used for testing */ 
  if ( Str == e_FT_Test ) {
    this->_txMaxTim->Text = "0.2"; 
    this->_txFirDur->Text = "0.4";
    _SetComboBox (this->_cbSimTime, "1.0");
    this->_ud_XMax->Text = "1.0" ;  }   /* one hour, in Hour end minute spinbox */
  
  if ( Str == e_FT_Wild ) {   /* Wild Fire */
    this->_txMaxTim->Text = "3";            /* Time To Max kW/m2 - hours */
    this->_txFirDur->Text = "6";             /* Fire Duration - hours */
    this->_txQabs->Text = "20.0"; 
    _SetComboBox (this->_cbSimTime, "12");
     this->_ud_XMax->Text = "12" ;}         /* End Time - hours - max X axis Grap Temp/Moist */

  if ( Str == e_FT_Pres ) {   /* Prescribed Burn */
    this->_txMaxTim->Text = "2"; 
    this->_txFirDur->Text = "3"; 
    this->_txQabs->Text = "12.0"; 
   _SetComboBox (this->_cbSimTime, "12");
    this->_ud_XMax->Text = "12" ; } 

  if ( Str == e_FT_Pile ) {       /* Pile Burn */ 
    this->_txMaxTim->Text = "4"; 
    this->_txFirDur->Text = "80";
    this->_txQabs->Text = "16.0"; 
   _SetComboBox (this->_cbSimTime, "100"); 
    this->_ud_XMax->Text = "100" ; } 

return 1; 
}

/*******************************************************************
*
*
*******************************************************************/ 
 void  FTBM_Message ()
{
char A[3000];

    strcpy (A, "               Burnup Fire Type Option Selected\n\n"); 
    strcat (A,  "The soil simulation will use the fire intensity imported from\n"); 
    strcat (A,  "the FOFEM fuel consumption model. This is done in one of two ways:\n\n");
    strcat (A,  "1) Duff fuel is present: the intensity will come from the\n"); 
    strcat (A,  "   heat generated soley by the duff being burnt.\n"); 
    strcat (A,  "   Because the duff itself acts as an insulator as it burns only\n");
    strcat (A,  "   a portion of the fire's heat will reach the soil.\n\n");  
    strcat (A,  "2) No duff fuel: the fire intensity used will come from FOFEM's\n");
    strcat (A,  "   Burnup model, because the fire's intensity will radiate in all\n");
    strcat (A,  "   directions only a small portion of the intenstiy will be\n"); 
    strcat (A,  "   directed downward and used by the soil model\n\n"); 

    strcat (A,   "Please see the FOFEM documentation for more details.\n");
    myMessageBox (A); 

}




/***********************************************************************
* Name: BM_Run
* Desc: Run Massman model, get GUI inputs, run, graph. . . 
*
**********************************************************************/
int Form1::BM_Run(char cr_Mode[])
{
int i,j, i_TimMax; 
float f,g, f_Min, f_MaxTemp, f_MaxTime, f_psin, f_Hour; 
double d; 
char cr_Out[1000], cr_ErrMes[1000], cr[1000]; 
char cr_Temp[1000], cr_Moist[1000]; 
double d_Min,d_delt;
float  f_delt; 
String ^ Str; 
System::Windows::Forms::DialogResult r;
time_t a;
time_t b;
double seconds,min; 

//  if ( !stricmp (cr_Mode,"Draw") ) 
//     goto Draw; 

/* Get the FireType from combobox */
  Str = _GetComboBox (this->_cb_FireType, this->xbmi->cr_FirTyp);

/* Clear out the Messagebox on the GUI and Update() the GUI so it happens */
/*  now before the model starts running */ 
   this->_txMess->Text = ""; 
   Update ();       /* Cause GUI update - to clear the text box */ 

   strcpy (gcr_FuncName,"");   /* Messages if math exception occurs */ 
   strcpy (gcr_JumpMess,""); 

/* Put up the Wait Hourglass cursor */
/* NOTE: I didn't have to restore the Default because it happened */
/*  automatically after it ran ? ? - not sure why, but that Cursor::Default */
/*  below might need to be placed at the end of this function if you */
/*  encounter a problem with it not restoring */ 
 this->Cursor->Current = Cursors::WaitCursor; 
//  this->Cursor->Current = Cursors::Default;   See Notes above  

/*.-.-.-.-.-.-.-.-.-.-.-.--.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/*  Gather up the inputs from the GUI  */

/* Get the Ambient Air Temperature */ 
  if ( !_Get_TBFlo( this->_txBM_AmbAirTmp, &this->xbmi->f_AmbAirTmp, cr_Out, "Ambient Air Temperature",  cr_ErrMes)){
     myMessageBox ("Invalid/Missing - Ambient Air Temperature"); 
     return 0; }

/* Check Ambient Air Temperature Limts */ 
  if (this->xbmi->f_AmbAirTmp < 0.2 || this->xbmi->f_AmbAirTmp > 50.0 ) {
     myMessageBox ("Air Temperature is out of range, use  0.2 -> 50.0"); 
     return 0;  }


/* Get Fire Duration - Burntime - used to create heat curve */
  if ( !_Get_TBFlo( this->_txFirDur, &this->xbmi->f_BurnTime, cr_Out, "Burn Time",  cr_ErrMes)){
     myMessageBox ("Invalid/Missing - Fire Intensity - Burn Time"); 
     return 0; }

/* Get Maximum Watt Time - applied in heat curve */
  if ( !_Get_TBFlo( this->_txMaxTim, &this->xbmi->f_MaxWatTim, cr_Out, "Max Watt Time",  cr_ErrMes)){
     myMessageBox ("Invalid/Missing - Fire Intensity - Max Watt Time "); 
     return 0; }

/* Get Watts - used to create heat curve */
  if ( !_Get_TBFlo( this->_txQabs, &this->xbmi->f_Qabs, cr_Out, "KW/m2",  cr_ErrMes)){
     myMessageBox ("Invalid/Missing - Fire Intensity - starting KW/m2"); 
     return 0;} 
  this->xbmi->f_Qabs *= 1000.0;   /* Kilowatts to watts */ 

/* Get Moisture from textbox */
  if ( !_Get_TBFlo( this->_txBM_Moist, &this->xbmi->f_Moist, cr_Out, "Moisture",  cr_ErrMes)){
     myMessageBox ("Invalid numberic value in moisture box"); 
     return 0; }

/* Get Soil Bulk Density l from textbox */
  if ( !_Get_TBFlo( this->_txBM_SoiBulDen, &this->xbmi->f_SoiBulDen, cr_Out,"Soil Bulk Density",  cr_ErrMes)){
     myMessageBox ("Invalid numberic value in Soil Bulk Density box"); 
     return 0; }
  
/* Get Soil Particle Density from textbox */
  if ( !_Get_TBFlo( this->_txBM_SoiParDen, &this->xbmi->f_SoiParDen, cr_Out,"Soil Particle Density",  cr_ErrMes)){
     myMessageBox ("Invalid numberic value in Soil Particle Density box"); 
     return 0; }
  
/* Graph Temp or moisture - radio checkboxes */ 
   GraphType (this->xbmi->cr_TemMoi); 

/* Get Simulation Time - hours */ 
  Str = _GetComboBox (this->_cbSimTime, cr); 
  f_Hour =  atof (cr);
  f_Min = f_Hour * 60.0; 
  this->xbmi->d_SimTime = (double) f_Min; 
  if ( f_Hour < 0.5 || f_Hour > e_MaxHourSim ){ 
    myMessageBox ("Invalid Simulation Time" ); 
    return 0; } 

  if ( !stricmp (this->xbmi->cr_FirTyp,e_FT_Burnup) )
    goto NoChk;

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.--.-.-.--.-.-.-.-.-.-.-.-.-.-.-.-.*/
/*  Check Some Inputs   */
 
/* Fire Duration - Burn Time */ 
 if ( this->xbmi->f_BurnTime > e_MaxBurnTime ) {
    sprintf (cr, "Maximum Fire Duration Time exceeded. Limit: %4.0f ",e_MaxBurnTime ); 
    myMessageBox (cr); 
    return 0; }
 if ( this->xbmi->f_BurnTime < e_MinBurnTime ) {
    sprintf (cr, "Minimum Fire Duration Time below limit.  Lower Limit: %4.2f ",e_MinBurnTime ); 
    myMessageBox (cr); 
    return 0; }

/* Time to Max - time (hrs) that maximum fire intensity kW/m2 is applied */
  if ( this->xbmi->f_MaxWatTim > e_Max_MaxWatTim ){
   sprintf (cr,"Time To Max - exceeds upper limit. Upper Limit: %0.2f", e_Max_MaxWatTim);  
   myMessageBox (cr); 
   return 0; }

  if ( this->xbmi->f_MaxWatTim < e_Min_MaxWatTim ){
   sprintf (cr,"Time To Max - is below lower limit. Lower Limit: %0.2f", e_Min_MaxWatTim);  
   myMessageBox (cr); 
   return 0; }

/* Time to maximum fire intensity - kW/m2 entered */
 if ( this->xbmi->f_MaxWatTim >= this->xbmi->f_BurnTime ){
   sprintf (cr,"Fire Intensity Time To Max (%4.2f) exceeds or equals Fire Duration (%4.2f)",this->xbmi->f_MaxWatTim, this->xbmi->f_BurnTime);  
   myMessageBox (cr); 
   return 0; }

/* Moisture ............................ */
  if ( this->xbmi->f_Moist < e_MinMoist || this->xbmi->f_Moist > e_MaxMoist ){
     sprintf (cr,"Soil Moisture ( %4.2f) out of range. Low: %4.2f, Upper: %4.2f", this->xbmi->f_Moist,e_MinMoist,e_MaxMoist); 
     myMessageBox (cr); 
     return 0;  }

/* Soil Bulk Density  ............................ */
  if ( this->xbmi->f_SoiBulDen < e_MinSoiBul || this->xbmi->f_SoiBulDen > e_MaxSoiBul ){
     sprintf (cr,"Soil Bulk Density ( %4.2f) out of range. Low: %4.2f, Upper: %4.2f", this->xbmi->f_SoiBulDen,e_MinSoiBul,e_MaxSoiBul); 
     myMessageBox (cr); 
     return 0;  }


/* Soil Particle Density  ............................ */
  if ( this->xbmi->f_SoiParDen < e_MinSoiPar || this->xbmi->f_SoiParDen > e_MaxSoiPar ){
     sprintf (cr,"Soil Particle Density ( %4.2f) out of range. Low: %4.2f, Upper: %4.2f", this->xbmi->f_SoiParDen,e_MinSoiPar,e_MaxSoiPar); 
     myMessageBox (cr); 
     return 0;  }





/* Check fire intensity - max is dependent on the Burn Duration Time  */
  f = MaxFirInt (this->xbmi->f_BurnTime);
  g = this->xbmi->f_Qabs / 1000.0;   /* get it back to kW/m2 for checking */ 
  if ( g > f ) {
    sprintf (cr, "Fire Intensity Max (%4.0f) exceeded for Fire Duration (%4.1f)\n Limit: %5.1f",g,this->xbmi->f_BurnTime,f); 
    myMessageBox (cr); 
    return 0;       }

NoChk: 
/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/

  time (&a); /* starting System time, testing long long simulation takes to run = see below  */

  HTA_Init ();

  i = FOFEM_HMV_Model (this->xbmi, "Boundry"); 
  if ( i <= 0 ) { 
    myMessageBox (this->xbmi->cr_ErrMes);
    return 0; }


  float f_bcQInc = e_delt;                 /* Heat array time step increment - seconds */
  FireInten_Graph(bcQ, f_bcQInc); 

  if ( !stricmp (cr_Mode, "FirIntGraph") )
    return 1;  

  gi_LastT = 0; 
  this->_cb_FireType->Enabled = false ; 
  this->_cbSimTime->Enabled = false; 

  i = FOFEM_HMV_Model (xbmi, ""); 

  this->_cb_FireType->Enabled = true; 
  this->_cbSimTime->Enabled = true; 

  if ( i == 0 || i == 1 ) {          /* user stopped */
     OutputFile("Temp", cr_Temp);
     OutputFile ("Moist",cr_Moist); 
     time (&b); 
     seconds = difftime (b,a);
     if ( i == 0 ) 
       sprintf (cr,"Simulation Stopped - CPU Time\nSec: %5.0f    -    Min: %5.1f\n Output Files:\n%s\n%s",seconds,seconds/60.0,cr_Temp,cr_Moist); 
     else
       sprintf (cr,"Simulation Finished - CPU Time\nSec: %5.0f    -    Min: %5.1f\n Output Files:\n%s\n%s",seconds,seconds/60.0,cr_Temp,cr_Moist); 
     myMessageBox (cr); 
     return 1; }

  if ( i == -1 ) { 
     myMessageBox (this->xbmi->cr_ErrMes); 
     return 1; }
  
}
/***************************************************************
* Name: Graph_TempMoist 
* Desc: Graph the Temperature or Moisture 
*
****************************************************************/
int Form1::Graph_TempMoist (d_BMI *bmi)
{
int i, i_TimMax, i_Lay;
int L,T;
float f_Heat, f_Moist, f_Time, f_psin; 
char cr[100]; 
String ^ Str; 

   GraphType (bmi->cr_TemMoi);        /* get GraphType again here incase didn't do it above */
   i_TimMax = Set_Axis(); 

   i_Lay = HTA_Layers();

/* Draw Graph, Massman - Temperature or Moisture Graph  */

  for ( L = 1; L <= i_Lay ; L++ ) {     /* For Each Soil Layer */
    sprintf (cr, "B_Lay%d", L);         /* Make sure each old line */
    Str =_CharToStr ( cr);              /* is cleared */ 

    this->chart1->Series[Str]->BorderWidth  = 3;     

    if ( gi_LastT > 0 )                /* Already did the very first drawing */
     goto Skip;                      /* So go pickup were we left off */ 

    this->chart1->Series[Str]->Points->Clear();

    if ( !stricmp (bmi->cr_TemMoi, "Temp") ) 
      this->chart1->Series[Str]->Points->AddXY (0, 21.0);  /* 21 = starting soil temp in celsius */
    else {      /* Moisture */  
      HTA_Get (L, 0, &f_Heat, &f_Moist, &f_psin, &f_Time);
      this->chart1->Series[Str]->Points->AddXY (0, f_Moist); 
    } /* else */

Skip:
    if ( !this->isChecked (L) ) 
      continue; 

    for ( T = gi_LastT; T < 1000000; T++ ) {
       i = HTA_Get (L, T, &f_Heat, &f_Moist, &f_psin, &f_Time);   /* Get data */
       if ( i == 0 ) 
          break; 
       f_Time = f_Time / 60.0;   /* Get to hours */
       if ( !stricmp (this->xbmi->cr_TemMoi, "Temp") ) 
         this->chart1->Series[Str]->Points->AddXY (f_Time, f_Heat);   /* X, Y */
       else if ( !stricmp (this->xbmi->cr_TemMoi, "Moist" ) ) 
         this->chart1->Series[Str]->Points->AddXY (f_Time, f_Moist);   /* X, Y */
       else    /* Graph Moist / Temp */    
         this->chart1->Series[Str]->Points->AddXY (f_Heat, f_Moist); 

    } /* For T */ 
  }  /* For L */ 

  gi_LastT = T; /* Save so we can pickup where we left off */

  return 1; 
}

/***************************************************************
* Name: OutputFile
* Desc: Output the temp or moistures 
*   In: cr_Mode   "Temp" or "Moist" 
*  Out: cr_Mess....path file name of created file or 
*                  error message
****************************************************************/
int Form1::OutputFile(char cr_Mode[], char cr_Mess[])
{
int i, L, T; 
char cr[1000], cr_FN[1000];
float f_Heat, f_Moist, f_psin, f_Time;

String ^Str;
FILE *fh;

  if ( !stricmp (cr_Mode, "Temp") ) 
     Str = this->s_UserFolder + "//BM_Tempature.csv";
  else 
     Str = this->s_UserFolder + "//BM_Moisture.csv";

  _StrToChar (Str,cr_FN); 
  fh = fopen (cr_FN, "w"); 
  if ( fh == NULL ) {
    sprintf (cr_Mess, "Can't open output file:\n%s",cr_FN);
    return 0; }

   fprintf (fh, "time(m),surface,1cm,2cm,3cm,4cm,5cm,6cm,7cm,8cm,9cm,10cm,11cm,12cm,13cm,14cm,15cm,16cm,17cm,18cm,19cm,20cm\n");

    for ( T = 0; T < 200000; T++  ) {
      for ( L = 1; L <= e_BM_Lay ; L++ ) {  
         i = HTA_Get (L, T, &f_Heat, &f_Moist, &f_psin, &f_Time);   /* Get data */
         if ( i == 0 ) 
             goto Done; 
         if ( L == 1 ) 
           fprintf (fh, "%5.2f", f_Time);   
         if ( !stricmp (cr_Mode, "Temp" ) )                 
           fprintf (fh, ",%4.0f",f_Heat);
         else
           fprintf (fh, ",%4.2f",f_Moist);
      }
      fprintf (fh, "\n"); 
    } 
Done:
    strcpy (cr_Mess,cr_FN); 
    fclose (fh); 
  return 1; 
}

/****************************************************************
* Name:  FOFEM_HMV_Model
* Desc:  This is a modified version of HMV_Model() (massman function)
*        It was done to make it easier to put out the progress
*        time to the GUI,
*        I moved in the SolveHMV loop code to. 
* Note-1: Call just needed the fire intensity array created, so return 
*         here once the fire intensity array has been created.
* Note-2: can't use setjmp() in managed code so I was trying to put it 
*         in another function, I used setjmp in the original SolveHMV()
*         before calling CrankNicolson().  I couldn't get it to work 
*         here, the longjump() my xpow() didn't work 
*  Ret:  1   ran to completion
*        0   user stopped 
*       -1   Error 
******************************************************************/
int Form1::FOFEM_HMV_Model(d_BMI *bmi, char cr_Mode[])
{
int i,j,  n, iN; 
double f,g,h,rem ; 
int r, c; 
double  fr[4]; 
char cr[100];
String ^ Str; 

   strcpy (bmi->cr_ErrMes,""); 

//%__________________________________________________________________________
//% Choose models of physical processes  
   Model_Switch_HMV(); 

//%__________________________________________________________________________
//% Asign general physical constants and related model paramters 
   Physical_Constants_HMV();

//%__________________________________________________________________________
//% Input soil parameters 
//% Assign spatial grid scale and time step
//% Initialze vertical profiles of soil bulk density, mineral fraction, 
//% total porosity, soil moisture, soil temperature, and the 
//% normalized soil water potential      
  if ( !Soil_Time_Depth_Param_HMV(bmi)) {
    return -1; }

// Added 12/2/7
  if ( !stricmp (bmi->cr_FirTyp,e_FT_Burnup) ) 
    Boundary_Burnup (bmi);
  else 
 //   BoundaryLHB17dBFD (bmi); 
    BoundarydBFD (bmi);

//% Input Upper Boundary Conditions = UBC
//   BoundaryUBFD(bmi); 
//  BoundaryU(bmi);

  if ( !stricmp (cr_Mode, "Boundry") ) /* See Note-1 above */
     return 1; 

// See Note-2 above 
//  i = _SetJump (cr_ErrMes); 

/*----------------------------------------------------*/
 AFT_Init(bmi); 

/* this loop code was in SolveHMV.cpp   */

    this->b_Abort = false; 
    this->b_Running = true; 
    this->ClearTempGraph(); 
    iN = 0 ; 
    for ( jstep = 1; jstep <= nsteps; jstep++ )  {
if ( jstep == 106230 )
   j++; 
      if ( this->b_Abort == true ) { 
        this->b_Running = false;
        return 0; }  /* User Stopped */ 

      Application::DoEvents();  /* Make textbox update */   
      i = CrankNicolson((int)jstep, bmi->cr_ErrMes);   
      if ( i == 0 ) {  /* Error */ 
         this->b_Running == false;
         return -1; }

      if ( this->xbmi->d_SimTime <= _MinHrs(2) )   /* If simulation time < 2 hours */
        n = (int) e_delt * 20 ;                    /*  Graph every 1 minute */
      else  if ( this->xbmi->d_SimTime <= _MinHrs (12) )   
        n = (int) e_delt * 100;                    /* Graph every 5 minutes */ 
      else 
        n = (int) e_delt * 200;                    /* Graph every 10 minutes; */  

      iN++; 
      if ( iN < n )                                /* Is it time to graph */
        continue;                                  /* Nope */ 
      iN = 0; 

      _GetRunTim (e_delt,jstep, cr);  /* Get Simulation current run time */
      Str = _CharToStr (cr);          /*  display on GUI */
      this->_txMess->Text = Str;

      Graph_TempMoist (bmi);          /* update the graph */ 

   }  /* for */


   this->b_Running = false; 

// test 
//  BM_Run("Draw"); 

   return 1; 

}

/*******************************************************************
* Return the number of minutes for the specified hour
*     ex:  send in 2 ---> 120 
*******************************************************************/
int _MinHrs (int i_Hrs)
{
int i;
   i = i_Hrs * 60;
   return i ; 
}

/************************************************************
*
*
*************************************************************/
extern double force  [eC_nsteps]; 
extern double ttme  [eC_nsteps]; 

extern double js    [];
extern double bcQ   [];
extern double bcva  []; 
extern double bcta  []; 
extern double bcra  []; 
extern double forIR  [];
extern double eta4  [];
extern double delt,nsteps; 
extern double vaporf,vapori; 
extern double tempaf, tempai; 
extern double  h2omol, rgas, temp0; 
double _CalcA1 (double t_m, double t_d);
double  asinh(double x);

double  asinh(double x);

extern double delz; 

/*************************************************************************/

int Boundary_Burnup(d_BMI *bmi)
{
int i, js,nn;
double tm, td, altd, l, d; 
double a[3];

/* See Note-1 above */
   for ( js = 0; js < eC_nsteps; js++ ) {
     bcQ[js] = 0; 
     bcva[js] = 0;     
     bcta[js] = 0;     
     bcra[js] = 0;     
     eta4[js] = 0; 
     force[js] = 0; }


// js        = 1:nsteps;
// ttme      = (js-1)*delt/3600;
   for ( js = 1; js <= nsteps; js++ )     // ttme   = (js-1)*delt/3600;
     ttme[js] = (js-1) * delt / 3600.0;

/* GUI inputs tm = time(hr) when max watts are applied, td = burn time (hr) */
   tm = bmi->f_MaxWatTim;           // tm        = 0.6;
   td = bmi->f_BurnTime;           // td        = 17.834 

   altd = 2.0 * xlog(10) /  xpow ( (asinh(0.5*td/tm)), 2 );   // a1td      = 2*log(10)/(asinh(0.5*td/tm))^2;

   a[1] = altd;                            // a  = [a1td tm];
   a[2] = tm; 

    force [1] = 0;                         // force(1)  = 0;
    for ( nn = 2; nn <= nsteps; nn++ ) {   // nn  = 2:nsteps;
      l = xlog(ttme[nn] / a[2] );          // force(nn) = exp(-a(1)*( log(ttme(nn)/a(2))  ) .^2); 
      l = l * l; 
      d = -a[1] * l; 
    
      force[nn] = xexp (d);  
   }                         

   BoundaryIR();   // BoundaryIR
 
// bcQ       = Qabs*force;
// bcva      = vapori + (vaporf-vapori)*force;
// bcta      = tempai + (tempaf-tempai)*force;
// bcra      = bcva*h2omol./(rgas*(bcta+temp0));
// eta4      = caleta4(bcta,bcva);

float f_Sec,f_PC, f_Wts;
int   i_Sec, i_Tim; 
   for ( nn = 1; nn <= nsteps; nn++ ) {

     f_Sec = (nn * delt) + 0.5; 
     i_Sec = (int) f_Sec; 
     i  = SHA_TP_GetByTime (i_Sec, &i_Tim, &f_PC, &f_Wts);
     if ( i < 0 ) 
        break; 

       bcQ[nn] = f_Wts ; 


 //     bcQ[nn] = bmi->f_Qabs * force[nn];       // bcQ = Qabs*force;

      bcva[nn] = vapori + (vaporf-vapori)  *force[nn];              // bcva      = vapori + (vaporf-vapori)*force;

      bcta[nn] = tempai + (tempaf-tempai)  *force[nn];                            // bcta      = tempai + (tempaf-tempai)*force;

      bcra[nn] = bcva[nn] * h2omol / ( rgas * (bcta[nn] + temp0 ));   // bcra      = bcva*h2omol./(rgas*(bcta+temp0));

   } /* for */ 

   caleta4(eta4,bcta,bcva,nsteps);    // eta4      = caleta4(bcta,bcva);

   return 1; 
}

#ifdef WRERE
/***************************************************************
* Name: _SetJump
* Desc: Note currently used, see function heading notes in 
*       FOFEM_HMV_Model() above 
***************************************************************/
int  _SetJump (char cr_ErrMes[])
{
int i; 
/*----------------------------------------------------*/
/* Will jump directly back to here if we hit a math error, see my functions  */
/*  xexp(), xpow(), etc. . . */  
//  extern jmp_buf g_env; 
extern char gcr_JumpMess[]; 
extern char gcr_FuncName[]; 

  i = 0; 
  i = setjmp(g_env);
  if ( i > 0 ) {
    sprintf (cr_ErrMes, "JumpMess = %s, Func: %s\n",gcr_JumpMess,gcr_FuncName); 
    return -1;  } 
  return 1; 
}
#endif 

/****************************************************************
*
*
***************************************************************/
void  _GetRunTim (double d_delt, double jstep,  char cr[])
{
float f_Sec, f_Hr;
int   i,j, i_TotSec, i_TotMin, i_Hrs, i_Mins;

   j = 0; 

   f_Sec = d_delt * jstep; 
   i_TotSec = (int) f_Sec;
   i_TotMin = i_TotSec / 60; 

   i_Hrs = i_TotMin / 60; 

   i = i_Hrs * 60;

   i_Mins = i_TotMin - i; 

   sprintf (cr,"%d::%02d",i_Hrs, i_Mins); 

   if ( i_Hrs < 88 ) 
     return;

   j++; 
} 


/****************************************************************
* Name: GraphClear
* Desc: Clear the temp / moisture graph
*****************************************************************/
int Form1::ClearTempGraph()
{
int L, i_Lays;
char cr[100]; 
String ^ Str; 

/* Number of layers - is the number of graph series lines */  
   i_Lays =  HTA_Layers ();

/* Clear every series even though not all were drawn */
/* Main Wnd graph */ 
   for ( L = 1; L <= i_Lays; L++ ) {        /* For Each Soil Layer */
     sprintf (cr, "B_Lay%d", L);          /* Make sure each old line */
     Str =_CharToStr ( cr);                /* is cleared */ 
     this->chart1->Series[Str]->Points->Clear();
  }

   return 1; 
}

/**********************************************************************
* Name: GraphType
* Desc: Check the GUI Radio buttons to see the graph type
*        user has selected. 
**********************************************************************/
int Form1::GraphType(char cr_TemMoi[])
{ 
  strcpy (cr_TemMoi,""); 
  if ( this->_rb_Temp->Checked == true ) { 
    strcpy (cr_TemMoi, "Temp");     /* User wants to graph temperatures */  
    return e_Temp; } 
  else if ( this->_rb_Moist->Checked == true ) {
    strcpy (cr_TemMoi, "Moist");    /* User wants to graph moisture */
    return e_Moist; } 
  else { 
    strcpy (cr_TemMoi, "MoistTemp");     /* User wants Moist / Temp Graph */ 
    return e_MoistTemp; } 
 
  return 0; 
}


/***************************************************************
* Name: MaxFirInt
* Desc: Calculate a maximum fire intenstiy using fire duration
*   In: td....fire duration - hours
*  Ret: Fire Intenstiy kW/m2
*****************************************************************/
float MaxFirInt (float td)
{
float MaxInt;
float p1 =  1.727e-06;
float p2 = -4.603e-04;
float p3 =  4.439e-02;  
float p4 = -2.101;
float p5 =  69.9; 

MaxInt = round(td*(td*(td*(p1*td+p2)+p3)+p4)+p5);

  return MaxInt;
}
/***********************************************************/
float round (float f)
{
float g,h;
    g = f + 0.5;
    h = (int) g;
    return h; 
}