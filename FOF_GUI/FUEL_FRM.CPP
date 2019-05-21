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

#include "Fuel_Frm.h"

/******************************************************************************
* Name: 
* Desc: 
*
*******************************************************************************/
int Fuel_Frm::Update(d_CO *a_CO)
{
int X;
char cr[100], cr_ErrMes[2000];
float f,g;
float f_Pre,f_Con, f_Pos;

 f_Pre = a_CO->f_Snd_DW1kPre + a_CO->f_Rot_DW1kPre;

 this->_chFuel->Series["Series1"]->Points->Clear(); 
 this->_chFuel->Series["Series2"]->Points->Clear(); 
 this->_chFuel->Series["Series3"]->Points->Clear(); 
 

 if ( this->_chFuelYaxis->Checked == true && this->f_YAxisMax > 0 ) {
//   if ( !_Get_TBFlo (this->_txFuelAxis, &f, cr, "Fuel Axis TextBox", cr_ErrMes))
//      myMessageBox (cr); 
//    X = (int) f; 
  f = this->f_YAxisMax; 
   }
 

  else {
   f = 1.0 ;
   if ( a_CO->f_LitPre   > f ) f = a_CO->f_LitPre; 
   if ( a_CO->f_DW1Pre   > f ) f = a_CO->f_DW1Pre; 
   if ( a_CO->f_DW10Pre  > f ) f = a_CO->f_DW10Pre; 
   if ( a_CO->f_DW100Pre > f ) f = a_CO->f_DW100Pre;
   if ( f_Pre > f ) f = f_Pre;
   if ( a_CO->f_DufPre > f ) f = a_CO->f_DufPre ;
   if ( a_CO->f_HerPre > f ) f = a_CO->f_HerPre ;
   if ( a_CO->f_ShrPre > f ) f = a_CO->f_ShrPre ;
   if ( a_CO->f_FolPre > f ) f = a_CO->f_FolPre ;
   if ( a_CO->f_BraPre > f ) f = a_CO->f_BraPre ;
   if (f > 1.0 ){
     g = f / 0.5;
     f = g * 0.5;
     f = f + 0.5;}    } 

this->f_YAxisMax = f; 

  this->_chFuel->ChartAreas["ChartArea1"]->AxisY->Maximum = f; 
   this->_chFuel->ChartAreas["ChartArea1"]->AxisY->Minimum = 0; 
  if ( f <= 2.0 ) 
    g = 0.25;
  else if ( f <= 6.0 )
    g = 0.50;
  else if ( f <= 10 ) 
    g = 2.0;
  else if ( f <= 30 )
    g = 5; 
  else if ( f <= 50 )
    g = 10; 
  else if ( f <= 100 )
    g = 20;
  else 
    g = 50 ;
     

  this->_chFuel->ChartAreas["ChartArea1"]->AxisY->Interval = g; 

// Set Y axis custom labels
// this->_chFuel->ChartAreas["ChartArea1"]->AxisX->CustomLabels->Add(0.5, 1.5, "Litter");
// this->_chFuel->ChartAreas["ChartArea1"]->AxisX->CustomLabels->Add(1.5, 2.5, "1 Hour");
// this->_chFuel->ChartAreas["Default"]->AxisX->CustomLabels[element]->Grid = GridTickTypes.All;

  this->_chFuel->Series["Series1"]->Points->AddY ( a_CO->f_LitPre ); 
  this->_chFuel->Series["Series2"]->Points->AddY ( a_CO->f_LitCon ); 
  this->_chFuel->Series["Series3"]->Points->AddY ( a_CO->f_LitPos ); 

  this->_chFuel->Series["Series1"]->Points->AddY ( a_CO->f_DW1Pre); 
  this->_chFuel->Series["Series2"]->Points->AddY ( a_CO->f_DW1Con ); 
  this->_chFuel->Series["Series3"]->Points->AddY ( a_CO->f_DW1Pos ); 

  this->_chFuel->Series["Series1"]->Points->AddY ( a_CO->f_DW10Pre); 
  this->_chFuel->Series["Series2"]->Points->AddY ( a_CO->f_DW10Con ); 
  this->_chFuel->Series["Series3"]->Points->AddY ( a_CO->f_DW10Pos ); 

  this->_chFuel->Series["Series1"]->Points->AddY ( a_CO->f_DW100Pre); 
  this->_chFuel->Series["Series2"]->Points->AddY ( a_CO->f_DW100Con ); 
  this->_chFuel->Series["Series3"]->Points->AddY ( a_CO->f_DW100Pos ); 


  f_Pre = a_CO->f_Snd_DW1kPre + a_CO->f_Rot_DW1kPre;
  f_Con = a_CO->f_Snd_DW1kCon + a_CO->f_Rot_DW1kCon;
  f_Pos = a_CO->f_Snd_DW1kPos + a_CO->f_Rot_DW1kPos;

  this->_chFuel->Series["Series1"]->Points->AddY ( f_Pre);
  this->_chFuel->Series["Series2"]->Points->AddY ( f_Con );
  this->_chFuel->Series["Series3"]->Points->AddY ( f_Pos ); 


  this->_chFuel->Series["Series1"]->Points->AddY ( a_CO->f_DufPre);
  this->_chFuel->Series["Series2"]->Points->AddY ( a_CO->f_DufCon );
  this->_chFuel->Series["Series3"]->Points->AddY ( a_CO->f_DufPos ); 

  this->_chFuel->Series["Series1"]->Points->AddY ( a_CO->f_HerPre);
  this->_chFuel->Series["Series2"]->Points->AddY ( a_CO->f_HerCon );
  this->_chFuel->Series["Series3"]->Points->AddY ( a_CO->f_HerPos );

  this->_chFuel->Series["Series1"]->Points->AddY ( a_CO->f_ShrPre);
  this->_chFuel->Series["Series2"]->Points->AddY ( a_CO->f_ShrCon );
  this->_chFuel->Series["Series3"]->Points->AddY ( a_CO->f_ShrPos );

  this->_chFuel->Series["Series1"]->Points->AddY ( a_CO->f_FolPre);
  this->_chFuel->Series["Series2"]->Points->AddY ( a_CO->f_FolCon );
  this->_chFuel->Series["Series3"]->Points->AddY ( a_CO->f_FolPos );

  this->_chFuel->Series["Series1"]->Points->AddY ( a_CO->f_BraPre);
  this->_chFuel->Series["Series2"]->Points->AddY ( a_CO->f_BraCon );
  this->_chFuel->Series["Series3"]->Points->AddY ( a_CO->f_BraPos );

return 1; 
}
