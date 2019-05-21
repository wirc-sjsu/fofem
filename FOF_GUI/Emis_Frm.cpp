/********************************************************************************************
* Name: Grap_Emis.cpp
* Desc: Stuff for doing the Emissions graph form
*
**********************************************************************************************/

#include <stdio.h>
#include <memory.h>

#include "Emis_Frm.h"

#include "Wnd_Util.h"
#include "fof_sgv.h"
#include "fof_co.h"

float _AxisY_Interal (int i);
int _Even (float f, float g);

int _SetPer (float f);
float  _RndChk0 (float f );

/***************************************************************************
* Name: Set_XAxis   Set_YAxis
* Desc: Get the value (if any) in the X Y Textbox and set the Axis to
*        that limit on the graph 
*  Ret: 1 value found and axis set, else 0
***************************************************************************/
int Grap_Emis::Set_XAxis()
{
int i_Intv, i_X; 
float f,f_X;
char cr_ErrMes[500], cr[100]; 

  if ( _Get_TBFlo (this->_txXAxis, &f_X, cr, "X Axis Limit", cr_ErrMes)) {
    this->_chGrap_Emis->ChartAreas["ChartArea1"]->AxisX->Maximum = f_X; 
    if ( f_X <= 10 ) 
      f = f_X / 4;
    else if ( f_X <= 100 )
      f = f_X / 10; 
    else 
      f = f_X / 20; 
    f = (int) f ; 
    this->_chGrap_Emis->ChartAreas["ChartArea1"]->AxisX->Interval = f; 
    return 1; }

   this->Axis_Mngr(); 
   return 0; 


/* better make it's no 0 and also set a minimum */
  i_X = this->i_Minutes;        /* max minutes */
  if ( i_X < 5 )
   i_X = 5;

  if ( i_X <= 5 ) 
    i_Intv = 1;
  else if ( i_X <= 10 ) 
    i_Intv = 2;
  else if ( i_X <= 50 )
    i_Intv = 5;
  else if ( i_X <= 100 )    
    i_Intv = 10; 
  else 
    i_Intv = 20;   

  this->_chGrap_Emis->ChartAreas["ChartArea1"]->AxisX->Maximum = (float) i_X;
  this->_chGrap_Emis->ChartAreas["ChartArea1"]->AxisX->Interval = (float) i_Intv; 
  return 0; 
}

/***************************************************************************
* Name: Set_YAxis
* Desc: Set the Y axis based on if the user has a value entered into 
*        the Y Axis Textbox
*  Ret: 1 = a valid user value found and used
****************************************************************************/
int Grap_Emis::Set_YAxis()
{
float f, f_Y;
char cr_ErrMes[500], cr[100]; 

  if ( _Get_TBFlo (this->_txYAxis, &f_Y, cr, "Y Axis Limit", cr_ErrMes)) {
    if ( f_Y < 1.0 )
      f_Y = 1.0; 
    this->_chGrap_Emis->ChartAreas["ChartArea1"]->AxisY->Maximum = f_Y; 
    this->_chGrap_Emis->ChartAreas["ChartArea1"]->AxisY->Interval = f_Y / 4.0; 
    return 1; }
 
   this->Axis_Mngr(); 
   return 0; 

/* this will happen if user hits a 'return' with and empty text box */
  f =  this->Max_Emis() + 0.5;
  f_Y = f / 1.0; 
  if ( f_Y < 4.0 )          /* Better check just to be carefull */
    f_Y = 4.0;              /*  that's not 0 */

  this->_chGrap_Emis->ChartAreas["ChartArea1"]->AxisY->Maximum = f_Y;
 
  f = (int) f_Y / 4.0;
  this->_chGrap_Emis->ChartAreas["ChartArea1"]->AxisY->Interval = f; 

 return 0; 
}



/***********************************************************************************
*
*
************************************************************************************/
int Grap_Emis::Update(d_CO *a_CO)
{
//  this->Init_Emis(); 
  this->Set_FuelCalc(a_CO); 
  this->DrawGraph(); 
  this->SetEmiTot();
  return 1; 
}
/************************************************************************
*
*
**********************************************************************/
int Grap_Emis::Clear()
{
float f;
   f = 0; 
   this->_chGrap_Emis->Series["PM25"]->Points->Clear(); 
   this->_chGrap_Emis->Series["PM10"]->Points->Clear(); 
   this->_chGrap_Emis->Series["CH4"]->Points->Clear(); 
   this->_chGrap_Emis->Series["CO2"]->Points->Clear(); 
   this->_chGrap_Emis->Series["CO"]->Points->Clear(); 
   this->_chGrap_Emis->Series["NOX"]->Points->Clear(); 
   this->_chGrap_Emis->Series["SO2"]->Points->Clear(); 
   this->_chGrap_Emis->Series["Intensity"]->Points->Clear(); 

   FloatToTextBox (this->_txGEflPM25 ,f, 2);
   FloatToTextBox (this->_txGEflPM10, f, 2);
   FloatToTextBox (this->_txGEflCH4,  f, 2);
   FloatToTextBox (this->_txGEflCO2,  f, 2);
   FloatToTextBox (this->_txGEflCO,   f, 2);
   FloatToTextBox (this->_txGEflNOX,  f, 2);
   FloatToTextBox (this->_txGEflSO2,  f, 2);

   FloatToTextBox (this->_txGEsmPM25, f, 2);
   FloatToTextBox (this->_txGEsmPM10, f, 2);
   FloatToTextBox (this->_txGEsmCH4,  f, 2);
   FloatToTextBox (this->_txGEsmCO2,  f, 2);
   FloatToTextBox (this->_txGEsmCO,   f, 2);
   FloatToTextBox (this->_txGEsmNOX,  f, 2);
   FloatToTextBox (this->_txGEsmSO2,  f, 2);

   FloatToTextBox (this->_txGEtotPM25, f, 2);
   FloatToTextBox (this->_txGEtotPM10, f, 2);
   FloatToTextBox (this->_txGEtotCH4,  f, 2);
   FloatToTextBox (this->_txGEtotCO2,  f, 2);
   FloatToTextBox (this->_txGEtotCO,   f, 2);
   FloatToTextBox (this->_txGEtotNOX,  f, 2);
   FloatToTextBox (this->_txGEtotSO2,  f, 2);

return 1; 
}

/*************************************************************************************
* Name: Set_FuelCalc
* Desc: Set the emission values into the Grap_Emis class from FuelCalc that
*        will be need for display and graphing
* NOTE: see notes below about units etc. 
* Note-1: The first [0] in the emisson array represents 60 seconds, all the rest 
*          are 15 second, that's how Burnup does it and gives it back to us
**************************************************************************************/
void Grap_Emis::Set_FuelCalc (d_CO *a_CO)
{
int i, j, k;
float f; 
float  f_PM25, f_PM10, f_CH4, f_CO2, f_CO, f_NOX, f_SO2, f_Int; 
 

/* This come from FuelCalc/Fofem in pounds */
//   FC->Get_EmiTot ("Flame", &f_PM25, &f_PM10, &f_CH4, &f_CO2, &f_CO, &f_NOX, &f_SO2 ); 
 
  this->f_fPM25 = a_CO->f_PM25F;
  this->f_fPM10 = a_CO->f_PM10F;
  this->f_fCH4  = a_CO->f_CH4F;
  this->f_fCO2  = a_CO->f_CO2F;
  this->f_fCO   = a_CO->f_COF;
  this->f_fNOX  = a_CO->f_NOXF;
  this->f_fSO2  = a_CO->f_SO2F;

//  FC->Get_EmiTot ("Smolder", &f_PM25, &f_PM10, &f_CH4, &f_CO2, &f_CO, &f_NOX, &f_SO2 ); 
  this->f_sPM25 = a_CO->f_PM25S;
  this->f_sPM10 = a_CO->f_PM10S;
  this->f_sCH4  = a_CO->f_CH4S;
  this->f_sCO2  = a_CO->f_CO2S;
  this->f_sCO   = a_CO->f_COS;
  this->f_sNOX  = a_CO->f_NOXS;
  this->f_sSO2  = a_CO->f_SO2S;

/* Get the arrays of emissions */
/* NOTE: these come to us as grams per Square Meter, so we need to convert to pounds AND  */
/*       then to pounds per second for graphing */
  k = a_CO->iC_SGV;  
  if ( k > eC_SGV )   /* This shouldn't happen but better makes sure */  
    k = eC_SGV;
  for ( i = 0; i < k ; i++ ) {

     a_CO->sr_SGV[i].f_CO; 

//    j = FC->Get_Emis (i,  &f_PM25, &f_PM10, &f_CH4, &f_CO2, &f_CO, &f_NOX, &f_SO2, &f_Int );
//     if ( j == 0 )           /* if we get to end of array */
//        break;
     if ( a_CO->sr_SGV[i].f_PM2_5 < 0 )       /* No more values coming back, NOTE all of the emis component */
        break;               /*  have the same # of values in their arrays */
/* See Note-1 above */ 
     if ( i == 0 )          
        f = 60.0;   /* 60 seconds in 1st Burntime steps */
     else 
        f = 15.0;   /*  all others are 15 seconds */

     fr_PM25 [i] = GramSqMt_To_Pounds(a_CO->sr_SGV[i].f_PM2_5 ) / f ;  /* we want pounds per second */
     fr_PM10 [i] = GramSqMt_To_Pounds(a_CO->sr_SGV[i].f_PM10 ) / f;
     fr_CH4  [i] = GramSqMt_To_Pounds(a_CO->sr_SGV[i].f_CH4 ) / f; 
     fr_CO2  [i] = GramSqMt_To_Pounds(a_CO->sr_SGV[i].f_CO2 ) / f ;
     fr_CO   [i] = GramSqMt_To_Pounds(a_CO->sr_SGV[i].f_CO  ) / f ; 
     fr_NOX  [i] = GramSqMt_To_Pounds(a_CO->sr_SGV[i].f_NOX ) / f ;
     fr_SO2  [i] = GramSqMt_To_Pounds(a_CO->sr_SGV[i].f_SO2 ) / f ; 
     fr_Int  [i] = f_Int;                           /* Fire Intensity - Don't convert */      
   } 

  this->i_Cnt = i;         /* how many values we have to work with */
}

/*************************************************************************************
* Name: SetEmiTot
* Desc: put the emission totals to their Form textboxes
* NOTE:  we round the numbers to get them to match the same outputs
*        in the Emissions Report (report in the GUI report window)
**************************************************************************************/
void Grap_Emis::SetEmiTot ()
{
/* Round these into their own variables so that I don't upset */
/* the actual orginal values, if/incase they get used elsewhere */
float fPM25 =  _RndEmiPnd(f_fPM25) ; 
float fPM10 =  _RndEmiPnd(f_fPM10) ; 
float fCH4  =  _RndEmiPnd(f_fCH4)  ; 
float fCO2  =  _RndEmiPnd(f_fCO2)  ; 
float fCO   =  _RndEmiPnd(f_fCO)   ; 
float fNOX  =  _RndEmiPnd(f_fNOX)  ; 
float fSO2  =  _RndEmiPnd(f_fSO2)  ; 
        
float sPM25 =  _RndEmiPnd(f_sPM25) ; 
float sPM10 =  _RndEmiPnd(f_sPM10) ; 
float sCH4  =  _RndEmiPnd(f_sCH4 ) ; 
float sCO2  =  _RndEmiPnd(f_sCO2)  ; 
float sCO   =  _RndEmiPnd(f_sCO )  ; 
float sNOX  =  _RndEmiPnd(f_sNOX)  ; 
float sSO2  =  _RndEmiPnd(f_sSO2)  ; 

float tPM25 = sPM25 + fPM25; 
float tPM10 = sPM10 + fPM10; 
float tCH4  = sCH4  + fCH4 ; 
float tCO2  = sCO2  + fCO2 ; 
float tCO   = sCO   + fCO  ; 
float tNOX  = sNOX  + fNOX ; 
float tSO2  = sSO2  + fSO2 ; 

  FloatToTextBox (_txGEflPM25, fPM25, _SetPer( fPM25));
  FloatToTextBox (_txGEflPM10, fPM10, _SetPer( fPM10));
  FloatToTextBox (_txGEflCH4,  fCH4,  _SetPer( fCH4 ));
  FloatToTextBox (_txGEflCO2,  fCO2,  _SetPer( fCO2 ));
  FloatToTextBox (_txGEflCO,   fCO,   _SetPer( fCO  ));
  FloatToTextBox (_txGEflNOX,  fNOX,  _SetPer( fNOX ));
  FloatToTextBox (_txGEflSO2,  fSO2,  _SetPer( fSO2 ));

  FloatToTextBox (_txGEsmPM25, sPM25,_SetPer( sPM25));
  FloatToTextBox (_txGEsmPM10, sPM10,_SetPer( sPM10));
  FloatToTextBox (_txGEsmCH4,  sCH4, _SetPer( sCH4 ));
  FloatToTextBox (_txGEsmCO2,  sCO2, _SetPer( sCO2 ));
  FloatToTextBox (_txGEsmCO,   sCO,  _SetPer( sCO  ));
  FloatToTextBox (_txGEsmNOX,  sNOX, _SetPer( sNOX ));
  FloatToTextBox (_txGEsmSO2,  sSO2, _SetPer( sSO2 ));

  FloatToTextBox (_txGEtotPM25, tPM25,  _SetPer( tPM25));
  FloatToTextBox (_txGEtotPM10, tPM10,  _SetPer( tPM10));
  FloatToTextBox (_txGEtotCH4,  tCH4,   _SetPer( tCH4 ));
  FloatToTextBox (_txGEtotCO2,  tCO2,   _SetPer( tCO2 ));
  FloatToTextBox (_txGEtotCO,   tCO,    _SetPer( tCO  ));
  FloatToTextBox (_txGEtotNOX,  tNOX,   _SetPer( tNOX ));
  FloatToTextBox (_txGEtotSO2,  tSO2,   _SetPer( tSO2 ));
}


/************************************************************************
* Name: _SetPer
* Desc: See how big a number is and return a value to indicate
*       how many decimal places we should should when outputing it
************************************************************************/
int _SetPer (float f)
{
  if ( f >= 10.0  )   /* 10 or above - no decimal places */
    return 0;
  if ( f >= 5.0 )     /* 5 -> 9.999 - one decimal place */
    return 1;
   return 2;          /* less than 5 get two decimals */
}
/*************************************************************************************
* Name: DrawGraph
* Desc: The Chart is already shown so know do the actual drawing on it
*
**************************************************************************************/
 int Grap_Emis::DrawGraph ()
{
int i,j,k,i_X, i_YAxis;
float f, X;
char cr[100], cr_ErrMes[200]; 
String ^Str;

/* If a Broadcast burn wasn't done, emission values will empty/zero */
/*  so present the graph with no tilt */
   if ( this->i_Cnt == 0 )  {
     this->_chGrap_Emis->ChartAreas["ChartArea1"]->Area3DStyle->Inclination = 0; 
     this->_upInclination->Value = (int) 0;   /* SpinBox */
     this->_chGrap_Emis->ChartAreas["ChartArea1"]->Area3DStyle->Rotation = 0; 
     this->_udRotation->Value = (int) 0; } /* SpinBox */

 
   this->_chGrap_Emis->Series["PM25"]->Points->Clear(); 
   this->_chGrap_Emis->Series["PM10"]->Points->Clear(); 
   this->_chGrap_Emis->Series["CH4"]->Points->Clear(); 
   this->_chGrap_Emis->Series["CO2"]->Points->Clear(); 
   this->_chGrap_Emis->Series["CO"]->Points->Clear(); 
   this->_chGrap_Emis->Series["NOX"]->Points->Clear(); 
   this->_chGrap_Emis->Series["SO2"]->Points->Clear(); 
   this->_chGrap_Emis->Series["Intensity"]->Points->Clear(); 
     
/* This establishes a staring point at 0, 0 */
   this->_chGrap_Emis->Series["PM25"]->Points->AddXY      (0,0); 
   this->_chGrap_Emis->Series["PM10"]->Points->AddXY      (0,0); 
   this->_chGrap_Emis->Series["CH4"]->Points->AddXY       (0,0); 
   this->_chGrap_Emis->Series["CO2"]->Points->AddXY       (0,0); 
   this->_chGrap_Emis->Series["CO"]->Points->AddXY        (0,0); 
   this->_chGrap_Emis->Series["NOX"]->Points->AddXY       (0,0); 
   this->_chGrap_Emis->Series["SO2"]->Points->AddXY       (0,0); 
   this->_chGrap_Emis->Series["Intensity"]->Points->AddXY (0,0); 

/* The first values in the array represent a 60 second second time step */
/*  by Fofem - Burnup */ 
   X = 1; 
   this->_chGrap_Emis->Series["PM25"]->Points->AddXY (X,fr_PM25[0]);   
   this->_chGrap_Emis->Series["PM10"]->Points->AddXY (X,fr_PM10[0]);   
   this->_chGrap_Emis->Series["CH4"]->Points->AddXY  (X,fr_CH4[0]);   
   this->_chGrap_Emis->Series["CO2"]->Points->AddXY  (X,fr_CO2[0]);   
   this->_chGrap_Emis->Series["CO"]->Points->AddXY   (X,fr_CO[0]);   
   this->_chGrap_Emis->Series["NOX"]->Points->AddXY  (X,fr_NOX[0]);   
   this->_chGrap_Emis->Series["SO2"]->Points->AddXY  (X,fr_SO2[0]);  
   this->_chGrap_Emis->Series["Intensity"]->Points->AddXY  (X,fr_Int[0]);   

   for ( i = 1; i < this->i_Cnt; i++ ) {
     if ( fr_PM25[i] < 0 ) /* not sure i need this, */ 
       break;          /* it would also mean no more data */

      X = X + 0.25;  /* This moves us in 15 second increments */

      this->_chGrap_Emis->Series["PM25"]->Points->AddXY      (X,fr_PM25[i]); 
      this->_chGrap_Emis->Series["PM10"]->Points->AddXY      (X,fr_PM10[i]); 
      this->_chGrap_Emis->Series["CH4"]->Points->AddXY       (X,fr_CH4[i]); 
      this->_chGrap_Emis->Series["CO2"]->Points->AddXY       (X,fr_CO2[i]); 
      this->_chGrap_Emis->Series["CO"]->Points->AddXY        (X,fr_CO[i]); 
      this->_chGrap_Emis->Series["NOX"]->Points->AddXY       (X,fr_NOX[i]); 
      this->_chGrap_Emis->Series["SO2"]->Points->AddXY       (X,fr_SO2[i]); 
      this->_chGrap_Emis->Series["Intensity"]->Points->AddXY (X,fr_Int[i]); 
   }

  this->i_Minutes = (int) (X + 1);  /* Minutes limit and X axis */

  SeriesOnOff();                    /* show each Emis component based on it's CheckBox */

  return 1;
}

/*************************************************************************************
* Name: Axis_Mngr
* Desc: Set the X & Y Axis marks along side and bottom of chart 
* 
**************************************************************************************/
void Grap_Emis::Axis_Mngr()
{
int i, i_X, i_Y; 
float f, f_Y; 
char cr[100], cr_ErrMes[500];

/* max points according to actual calcuated values */
   i_X = this->i_Minutes;        /* max minutes */
   f_Y = this->Max_Emis();       /* max emis value */
 
/* let's see if user entered Max Axis value(s) */
 if ( _Get_TBFlo (this->_txXAxis, &f, cr, "X Axis Limit", cr_ErrMes))
    i_X = (int)f; 

 if ( _Get_TBFlo (this->_txYAxis, &f, cr, "Y Axis Limit", cr_ErrMes)){
    i_Y = (int)f; 
    goto SetAx; } /* Skip adjustin top of Y for user explict value */

/* Set the top of the Y axis */
   if ( f_Y >= 100.0 ) {         /* Set to a multiple of 4 boundry */
     f_Y = f_Y + 10.0; 
     i_Y = _Even (f_Y,4.0); } 

   else if ( f_Y >= 40.0 ) { 
     f_Y = f_Y + 5.0;
     i_Y = _Even (f_Y,4.0); }

   else if ( f_Y >= 10.0 ) {    /* set to an even boundry */
     f_Y = f_Y + 3.0;
     i_Y = _Even (f_Y,2.0); }

   else if ( f_Y >= 4.0 ) { 
     f_Y = f_Y + 2.0;  
     i_Y = _Even(f_Y,2.0); }

   else if ( f_Y >= 1.0 ) {     /* Just set to 2 */
     i_Y = 2; }
   else                         /* less than one, just set to 1 */
     i_Y = 1;  
  
SetAx:
   this->Axis_Set (i_X,i_Y); 
}

/******************************************************/
#ifdef OLD_FUNC
void Grap_Emis::Axis_Mngr()
{
int i, i_X, i_Y; 
float f, f_Y, f_MaxY; 


/* Use defualt maximum X & Y axis from the emisson values */
Def:
   i_X = this->i_Minutes;        /* max minutes */
   f_Y = this->Max_Emis();       /* max emis value */
   f_MaxY = f_Y;

/* Set the top of the Y axis */
   if ( f_Y >= 100.0 ) {         /* Set to a multiple of 4 boundry */
     f_Y = f_Y + 10.0; 
     i_Y = _Even (f_Y,4.0); } 

   else if ( f_Y >= 40.0 ) { 
     f_Y = f_Y + 5.0;
     i_Y = _Even (f_Y,4.0); }

   else if ( f_Y >= 10.0 ) {    /* set to an even boundry */
     f_Y = f_Y + 3.0;
     i_Y = _Even (f_Y,2.0); }

   else if ( f_Y >= 4.0 ) { 
     f_Y = f_Y + 2.0;  
     i_Y = _Even(f_Y,2.0); }

   else if ( f_Y >= 1.0 ) {     /* Just set to 2 */
     i_Y = 2; }
   else                         /* less than one, just set to 1 */
     i_Y = 1;  

/* with my logic above, sometimes i_Y can be less than the Max Y */
/*  so I need to adjust in those cases */
  f_MaxY++;
  i = f_MaxY;
  if ( i_Y < i )
    i_Y = i; 
  
   this->Axis_Set (i_X,i_Y); 
}
#endif 

/*********************************************************************************
*
**********************************************************************************/
int _Even (float f, float g)
{
int i,j; 
float h;
     h = f / g; 
     i = (int) h;
     j = i * (int) g;; 
     return j; 
}

/****************************************************************************************
* Name: Axis_GetUser
* Desc: Get the User Entered X & Y axis limits, 
*       IF the FixedAxis CheckBox is checked AND there are valid entries in X & Y
*   Ret: 1 ok,  else 0
*****************************************************************************************/
int  Grap_Emis::Axis_GetUser(int *X, int *Y)
{
float f;     
char cr[200], cr_ErrMes[1000];

// ( !_Get_TBFlo (this->_txLitter, &a_FLI->f_Litter, cr, "Litter", a_FLI->cr_ErrMes)

  if ( this->_cbFixedAxis->Checked == false ) 
     return 0;    /* Fixed Axis CheckBox isn't set */ 

  if ( !_Get_TBFlo (this->_txXAxis, &f, cr, "X Axis Limit", cr_ErrMes)){
      myMessageBox (cr_ErrMes); 
     return 0; }            /* Error or Invalid data in textbox */
  if ( f < 2.0 )
     f = 2.0; 
  *X = (int) f; 
 
  if ( !_Get_TBFlo (this->_txYAxis, &f, cr, "Y Axis Limit", cr_ErrMes)){
     myMessageBox (cr_ErrMes);
     return 0;}
  if ( f < 1.0 )
    f = 1.0;
 
  *Y = (int) f; 
 
  return 1; 
}

/****************************************************************************************
*
*
*****************************************************************************************/
void Grap_Emis::Axis_Set(int X, int Y)
{
float f,g; 
  this->_chGrap_Emis->ChartAreas["ChartArea1"]->AxisX->Maximum = X; 
  this->_chGrap_Emis->ChartAreas["ChartArea1"]->AxisX->Minimum = 0; 
 
/* Set the X Axis Label marker interval based on the Max X */
   if ( X < 10 )
     g = 2;
   else if ( X <= 40 ) 
     g = 5;
   else 
     g = 10;  
 this->_chGrap_Emis->ChartAreas["ChartArea1"]->AxisX->Interval = g; 
// original  this->_chGrap_Emis->ChartAreas["ChartArea1"]->AxisX->Interval = 2; 

  this->_chGrap_Emis->ChartAreas["ChartArea1"]->AxisY->Minimum = 0; 
  this->_chGrap_Emis->ChartAreas["ChartArea1"]->AxisY->Maximum = Y; 
  f  =  _AxisY_Interal (Y);
  this->_chGrap_Emis->ChartAreas["ChartArea1"]->AxisY->Interval = f; 
}

/****************************************************************************************
*
* For Now this will just set the interval into 4 marks, so that 4 numbers go
*  up the Y axis marks
*
****************************************************************************************/
float _AxisY_Interal (int i)
{
float f,g; 
    f = i;

//  if ( i >= 8 ) 
//     g = f * 0.125;  
//  else 
     g = f * 0.25;  
  return g;
}
/**************************************************************************************
* Name: SeriesOnOff
* Desc: Set each Series in the Chart to display or not display based on if its
*       CheckBox is checked
*
***************************************************************************************/
void Grap_Emis::SeriesOnOff()
{
int i_YAxis; 
char cr[100]; 
 
// this->FixedAxisButtonChanged();     /* See user has the Fix Axis button set */

  this->Axis_Mngr();

 if ( this->_cbGE_PM25->Checked )
   this->_chGrap_Emis->Series["PM25"]->Enabled = 1;
 else  
   this->_chGrap_Emis->Series["PM25"]->Enabled = 0;

 if ( this->_cbGE_PM10->Checked )
   this->_chGrap_Emis->Series["PM10"]->Enabled = 1;
 else  
   this->_chGrap_Emis->Series["PM10"]->Enabled = 0;

 if ( this->_cbGE_CH4->Checked )
   this->_chGrap_Emis->Series["CH4"]->Enabled = 1;
 else  
   this->_chGrap_Emis->Series["CH4"]->Enabled = 0;

 if ( this->_cbGE_CO2->Checked )
   this->_chGrap_Emis->Series["CO2"]->Enabled = 1;
 else  
   this->_chGrap_Emis->Series["CO2"]->Enabled = 0;

 if ( this->_cbGE_CO->Checked )
   this->_chGrap_Emis->Series["CO"]->Enabled = 1;
 else  
   this->_chGrap_Emis->Series["CO"]->Enabled = 0;

 if ( this->_cbGE_NOX->Checked )
   this->_chGrap_Emis->Series["NOX"]->Enabled = 1;
 else  
   this->_chGrap_Emis->Series["NOX"]->Enabled = 0;

 if ( this->_cbGE_SO2->Checked )
   this->_chGrap_Emis->Series["SO2"]->Enabled = 1;
 else  
   this->_chGrap_Emis->Series["SO2"]->Enabled = 0;

 if ( this->_cbGE_Intensity->Checked )
   this->_chGrap_Emis->Series["Intensity"]->Enabled = 1;
 else  
   this->_chGrap_Emis->Series["Intensity"]->Enabled = 0;
}



/*********************************************************************************************
* Name: Set_UserAxis
* Desc: Get the user entered (Form TextBox) X & Y axis limits and set the Chart
*********************************************************************************************/
#ifdef wowo
void Grap_Emis::Set_UserAxis()
{
int X,Y;
float f;  
String ^Str;
char cr[100], cr_ErrMes[1000];
    
  if ( !_Get_WndFlo (this->_txYAxis, &f, cr,  "Y Axis Limit",cr_ErrMes)){
     myMessageBox(cr_ErrMes);
     return; }
  if ( f <= 0 )       /* If blank or invalid */
    return; 
  Y = (int) f; 

  if ( !_Get_WndFlo (this->_txXAxis, &f, cr, "X Axis Limit", cr_ErrMes)) {
    myMessageBox(cr_ErrMes); 
    return; }}
  if ( f <= 0 )     /* if blank or invalid */
     return; 
  X = (int) f; 
  
  this->_chGrap_Emis->ChartAreas["ChartArea1"]->AxisY->Maximum = Y; 
  this->_chGrap_Emis->ChartAreas["ChartArea1"]->AxisX->Maximum = X; 
}
 
#endif 

/*********************************************************************************************
* Name: Max_Emis
* Desc: Find the Highest Emission value for any Emission that has it CheckBox turned on
*        AND adjust it slightly upward for Y axis display purposes. 
*********************************************************************************************/
float  Grap_Emis::Max_Emis ()
{
int i;
float f; 
   f = 0; 
  for ( i = 0; i < this->i_Cnt; i++ ) {
    if ( this->_cbGE_PM25->Checked  && this->fr_PM25[i] > f ) 
       f = this->fr_PM25[i]; 
    if ( this->_cbGE_PM10->Checked  && this->fr_PM10[i] > f ) 
       f = this->fr_PM10[i]; 
    if ( this->_cbGE_CH4->Checked   && this->fr_CH4 [i] > f )
       f = this->fr_CH4 [i]; 
    if ( this->_cbGE_CO2->Checked   && this->fr_CO2 [i] > f ) 
       f = this->fr_CO2 [i]; 
    if ( this->_cbGE_CO->Checked    && this->fr_CO  [i] > f ) 
       f = this->fr_CO  [i]; 
    if ( this->_cbGE_NOX->Checked   && this->fr_NOX [i] > f ) 
       f = this->fr_NOX [i]; 
    if ( this->_cbGE_SO2->Checked   && this->fr_SO2 [i] > f ) 
       f = this->fr_SO2 [i]; 
    if ( this->_cbGE_Intensity->Checked && this->fr_Int[i] > f ) 
       f = this->fr_Int[i];  }

   return  f; 
}

/***********************************************************************************************
* The Fixed Axis Button was just 'click'
*
*************************************************************************************************/
 int Grap_Emis::FixedAxisButtonChanged()
{
int i, X, Y;
float f; 

   if ( this->_cbFixedAxis->Checked ) {
     if ( !this->Axis_GetUser (&X,  &Y) ) {    /* NOTE Error message will get done in here */
       this->_cbFixedAxis->Checked = false; 
       return 0; } }

   this->Axis_Mngr();
   return 1; 
}

/*****************************************************************************
* Name: Init_Emis
* Desc: Intialize all the Emission values need for graphing, 
* NOTE: Do NOT put stuff in here unless you want it to get cleared 
*       See where the calls to this function come from. 
******************************************************************************/
void Grap_Emis::Init_Emis()
{
int i; 
   fr_PM25 = new float[eC_fr];
   fr_PM10 = new float[eC_fr];
   fr_CH4  = new float[eC_fr];
   fr_CO2  = new float[eC_fr];
   fr_CO   = new float[eC_fr];
   fr_NOX  = new float[eC_fr];
   fr_SO2  = new float[eC_fr];
   fr_Int  = new float[eC_fr];
   i_Cnt = 0; 
   i_Minutes = 0; 

   for ( i = 0; i < eC_fr; i++ ) {
    fr_PM25[i] = 0; 
    fr_PM10[i] = 0;
    fr_CH4 [i] = 0;
    fr_CO2 [i] = 0;
    fr_CO  [i] = 0;
    fr_NOX [i] = 0;
    fr_SO2 [i] = 0;
    fr_Int [i] = 0; }

    f_fPM25 = 0;
    f_fPM10 = 0;
    f_fCH4 = 0;
    f_fCO2 = 0;
    f_fCO = 0 ;
    f_fNOX = 0;
    f_fSO2 = 0;

    f_sPM25 = 0;
    f_sPM10 = 0;
    f_sCH4 = 0;
    f_sCO2 = 0;
    f_sCO  = 0;
    f_sNOX = 0;
    f_sSO2 = 0;


}
