#include "Fire_Frm.h"

#define _CRT_SECURE_NO_WARNINGS
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
#include "fof_cm.h"

#include "Fire_Frm.h"


float _Rnd10 (float f);

/*******************************************************************
* Name: Set_XAxis
* Desc: Reset the X axis on the graph - the graph will automatically
*       redraw when this is set
*       This gets called when user enter/changes the X axis 
*       textBox 
*
*  NOTE NOTE ---->
*   there is no allowance yet for when the user erases a previous 
*   entry in the X or Y textboxes, we would want the graph to 
*   go back to using the original value for the axis, but this
*   would require it having been saved when the graph was first
*   drawn, I'm not going to worry about this for now, 
********************************************************************/
void Fire_Frm::Set_XAxis()
{
float f_X; 
char cr[500], cr_ErrMes[500]; 

  if ( _Get_TBFlo (this->_txXaxis, &f_X, cr, "X Axis Limit", cr_ErrMes))
    this->_chGrap_FirInt->ChartAreas["ChartArea1"]->AxisX->Maximum = f_X; 
} 

/*******************************************************************
* Name: Set_YAxis
* Desc: Reset the Y axis on the graph - the graph will automatically
*       redraw when this is set
*       This gets called when user enter/changes the Y axis 
*       textBox 
* SEE NOTES for the previous function Set_XAxis()
********************************************************************/
void Fire_Frm::Set_YAxis()
{
float f_Y; 
char cr[500], cr_ErrMes[500]; 

  if ( _Get_TBFlo (this->_txYaxis, &f_Y, cr, "Y Axis Limit", cr_ErrMes))
    this->_chGrap_FirInt->ChartAreas["ChartArea1"]->AxisY->Maximum = f_Y;
} 


/**********************************************************
*
*
***********************************************************/
int Fire_Frm::Update(d_CO *a_CO)
{
int i, k, i_MaxTim; 
float f, X,Y, f_X, f_Y,  f_Max, f_MaxSec; 
char  cr[500], cr_ErrMes[500];
  
  this->_chGrap_FirInt->Series["FirInt"]->Points->Clear(); 

  this->_chGrap_FirInt->ChartAreas["ChartArea1"]->AxisX->Minimum = 0; 
  this->_chGrap_FirInt->ChartAreas["ChartArea1"]->AxisY->Minimum = 0; 

/* If User has a value in the X or Y axis textbox than use it */
  f_Y = f_X = -1.0; 

  if ( _Get_TBFlo (this->_txXaxis, &f_X, cr, "X Axis Limit", cr_ErrMes))
    this->_chGrap_FirInt->ChartAreas["ChartArea1"]->AxisX->Maximum = f_X; 

  if ( _Get_TBFlo (this->_txYaxis, &f_Y, cr, "Y Axis Limit", cr_ErrMes))
    this->_chGrap_FirInt->ChartAreas["ChartArea1"]->AxisY->Maximum = f_Y; 


/* Establish X = 0 axis as start of graph line */
  this->_chGrap_FirInt->Series["FirInt"]->Points->AddXY (0,0); 

/* first Fire Intesity in array is for 60 seconds (1 minute) */
/*  other values in array are for 15 second intervals - see below */
  X = 1;                /* Go to 1 minute mark on X axis */
  f = a_CO->sr_SGV[0].f_Inten; 
  this->_chGrap_FirInt->Series["FirInt"]->Points->AddXY (X,f); 

  k = a_CO->iC_SGV;  
  if ( k > eC_SGV )   /* This shouldn't happen but better makes sure */  
    k = eC_SGV;
 
/* Do the 15 second fire inten intervals */
  f_Max = f; 
  i_MaxTim = 0; 
  for ( i = 1; i < k ; i++ ) {
    if ( a_CO->sr_SGV[i].f_PM2_5 < 0 )       /* No more values coming back, NOTE all of the emis component */
        break; 
    X = X + 0.25;  /* This moves us in 15 second increments */

    f = a_CO->sr_SGV[i].f_Inten;
    this->_chGrap_FirInt->Series["FirInt"]->Points->AddXY (X,f); 
 
    if ( f > f_Max ){  
      f_Max = f; 
      i_MaxTim = i;
    }
 }

 
  if ( f_X < 0 ) {
    X = _Rnd10 (X); 
    this->_chGrap_FirInt->ChartAreas["ChartArea1"]->AxisX->Maximum = X; }
  
  
  if ( f_Y < 0 ) {
    Y = _Rnd10 (f_Max); 
    this->_chGrap_FirInt->ChartAreas["ChartArea1"]->AxisY->Maximum = Y; }
  
// test........................................................
String ^ S; 
String ^ Str; 

/* Get minutes that max occurred at */
  i = i_MaxTim / 4;  /* each increment it 15 seconds */
  i = i + 1;         /* Add that first 60 second inc */
  
  sprintf (cr, "Max Intensity: %3.0f,  minute: %d", f_Max, i );
  Str = _CharToStr (cr); 
  this->_lbMaxInt->Text = Str; 

 return 1; 

}

/*****************************************************************
* Name: _Rnd10
* Desc: round up to the next 10 th place
*       example  237.4 -> 240,  333.0 -> 340  
******************************************************************/
float _Rnd10 (float f)
{
int i; 
float g;
    g =  f / 10.0; 
    i = g; 
    i++;   
    g = i * 10; 
  
    return g; 
}