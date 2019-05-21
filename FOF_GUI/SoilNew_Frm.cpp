#include <windows.h>

#include <stdio.h>
#include <math.h> 
#include <string.h>

#include "fof_sgv.h"
#include "Soil_Frm.h"
#include "Wnd_Util.h"
#include "fof_sha.h" 
#include "fof_util.h"


#include "SoilNew_Frm.h"

/********************************************
*
***********************************************/
int SoilNew_Frm::Reset()
{
   this->_chSoil->ChartAreas["ChartArea1"]->AxisX->Maximum = 15; 
   this->_chSoil->ChartAreas["ChartArea1"]->AxisY->Maximum = 500; 
  
 this->_chSoil->ChartAreas["ChartArea1"]->AxisX->IsReversed = true; 

   return 1; 
} 

/*************************************************
*
*
**************************************************/
int SoilNew_Frm::Run()
{
float x, y, f_EndAt; 
int i, i_Time, i_EndAt; 
char cr_Out[100], cr_Name[100], cr_ErrMes[1000]; 

//int _Get_TBFlo (TextBox^ a, float *af, char cr_Out[], char cr_Name[], char cr_ErrMes[]);
 
   i = _Get_TBFlo ( this->_txEndAt, &f_EndAt, cr_Out, "End At Time", cr_ErrMes); 

   i_EndAt = (int) f_EndAt; 
     this->Reset(); 

  x = 2;

  for ( i_Time = 1; i_Time < i_EndAt; i_Time++ ) {
     this->Update(x, i_Time);
     Application::DoEvents(); 
     Sleep(60); 
     x = x + 1; 
//  this->SNF->Update(&SI, &SO, f, i);
  
  }

  return 1; 
}


/***************************************************
*
****************************************************/
int SoilNew_Frm::Update( float f, int i_Time)
{ 
int Layer, Time; 
float f_Temp;  
char cr[100];
String ^ Str; 
 
  sprintf (cr, "%d",i_Time); 
  Str = _CharToStr(cr); 
  this->_txTime->Text = Str; 

  this->_chSoil->Series["Lay1"]->Points->Clear(); 
 
   for ( Layer = 1; Layer <= 14; Layer++ ) {
      f_Temp = SHA_Get (Layer,i_Time); 
      this->_chSoil->Series["Lay1"]->Points->AddXY (Layer, f_Temp); 
    }

   return 1; 


  this->_chSoil->Series["Lay1"]->Points->Clear(); 
  this->_chSoil->Series["Lay2"]->Points->Clear(); 
   
  this->_chSoil->Series["Lay1"]->Points->AddXY (2, f); 

  this->_chSoil->Series["Lay2"]->Points->AddXY (4, f+2); 


  this->_chSoil->Series["Lay2"]->Points->AddXY (6, f+7); 
 
    
  return 1; 

  memcpy (this->a_SI,a_SI,sizeof(d_SI)); 
  memcpy (this->a_SO,a_SO,sizeof(d_SO));
  this->UpdateDraw();  
 return 1;
}




/***************************************************
*
****************************************************/
int SoilNew_Frm::UpdateDraw()
{
int i,j, L, T, i_MaxMinutes;
float f, f_Tmax;
char cr[1000];
String ^ Str; 

#ifdef wowow
  

  i_MaxMinutes = 100; 
   f_Tmax = 2; 


      this->_chSoil->Series[Str]->Points->AddXY (T, f);   /* X, Y */

  } /* for L */ 

#endif 

  return 1; 


}
