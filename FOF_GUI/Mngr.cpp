/***************************************************************************************
* Name: Mngr.cpp
* Desc: 
* Fix:  1-19-18  Fixed the input file graph bug,
****************************************************************************************/
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <setjmp.h> 

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;

#include "Wnd_Util.h" 
#include "Wnd_BM.h"
#include "..\FOF_DLL\BM_Soil\HTAA.h"
//#include "BMSoil.h"
//#include "fof_se.h"

#define eC_FI  1200*60*60
float  fr_FI[eC_FI]; 
 
int  _StrToChar (String ^Str, char cr_To[]);
void  myMessageBox ( char cr[]);
int   HT_Get (int iX, int i_Lay, int *ai_TimeSec, float *af_Temp, float *af_Moist);

extern double bcta[];
extern double bcta[];
extern double bcQ[];


/******************************************************
* Name: FireInten_Graph
* Desc: Draw the fire intenisty (watts) graph
*******************************************************/
int Form1::FireInten_Graph(double fr[], float f_IncSec)
{
int i; 
float f_TotSec, r, t;
char cr[100];
float f_MaxHr; 
float f_MaxFI = 0; 

  _StrToChar (this->_ud_XMax->Text,cr);
  f_MaxHr = atof (cr);

/*-------------------------------------------------------*/
/* draw the Heat/Rabs Graph  */ 
  f_TotSec = 0; 

  this->chart2_Rabs->Series["Rabs"]->Points->Clear();

  for ( i = 1; i < 1000000; i++ ) {
    r = (float)fr[i]; 
    r = r / 1000.0; 
    if ( i > e_MaxHourSim && fr[i] == 0 ) 
      break; 
 
   if ( r > f_MaxFI ) 
       f_MaxFI = r;    /* use to set Y axis below */ 

    t = f_TotSec / (60.0 * 60.0);    /* Total Seconds to minutes */ 

    this->chart2_Rabs->Series["Rabs"]->Points->AddXY (t,r);

    f_TotSec += f_IncSec;   /* Total up the seconds */  
  } /* for i */ 

  this->chart2_Rabs->ChartAreas["ChartArea1"]->AxisX->Minimum = 0;
  this->chart2_Rabs->ChartAreas["ChartArea1"]->AxisX->Maximum = f_MaxHr ;

  f_MaxFI = f_MaxFI + ( 0.25 * f_MaxFI ); 
  this->chart2_Rabs->ChartAreas["ChartArea1"]->AxisY->Maximum = f_MaxFI ;

 // this->chart2_Rabs->ChartAreas["ChartArea1"]->AxisY->Title = "Watts w/m2"; 

  return 1; 
}


/***********************************************************
* Name: ChkBoxArr
* Desc: Mark an array to signify which checkboxes are 
*        checked
* NOTE: surface check box is [1]
*   In: iN ... size of array, can be any size as long 
*              as it's large enough, 
*              Unused [] at end of array
***********************************************************/
int Form1::ChkBoxArr(int ir[], int iN)
{
int i; 
  for ( i = 0; i < iN; i++ ) 
     ir[i] = 0; 

  if (  this->_cbL1->Checked )  ir[1] =  1;
  if (  this->_cbL2->Checked )  ir[2] =  1;
  if (  this->_cbL3->Checked )  ir[3] =  1;
  if (  this->_cbL4->Checked )  ir[4] =  1;
  if (  this->_cbL5->Checked )  ir[5] =  1;
  if (  this->_cbL6->Checked )  ir[6] =  1;
  if (  this->_cbL7->Checked )  ir[7] =  1;
  if (  this->_cbL8->Checked )  ir[8] =  1;
  if (  this->_cbL9->Checked )  ir[9] =  1;
  if (  this->_cbL10->Checked ) ir[10] =  1;
  if (  this->_cbL11->Checked ) ir[11] =  1;
  if (  this->_cbL12->Checked ) ir[12] =  1;
  if (  this->_cbL13->Checked ) ir[13] =  1;
  if (  this->_cbL14->Checked ) ir[14] =  1;

  if (  this->_cb15->Checked )  ir[15] =  1;
  if (  this->_cb16->Checked )  ir[16] =  1;
  if (  this->_cb17->Checked )  ir[17] =  1;
  if (  this->_cb18->Checked )  ir[18] =  1;
  if (  this->_cb19->Checked )  ir[19] =  1;
  if (  this->_cb20->Checked ) ir[20] =  1;
  if (  this->_cb21->Checked ) ir[21] =  1;


  if ( iN >= 22 )   /* if array is long enough */
    ir[22] = -1;    /* mark end */ 

  return 21; 

}  

/***********************************************************
*
**
**********************************************************/
int Form1::isChecked(int i)
{
  if ( i == 1  && this->_cbL1->Checked ) return 1;
  if ( i == 2  && this->_cbL2->Checked ) return 1;
  if ( i == 3  && this->_cbL3->Checked ) return 1;
  if ( i == 4  && this->_cbL4->Checked ) return 1;
  if ( i == 5  && this->_cbL5->Checked ) return 1;
  if ( i == 6  && this->_cbL6->Checked ) return 1;
  if ( i == 7  && this->_cbL7->Checked ) return 1;
  if ( i == 8  && this->_cbL8->Checked ) return 1;
  if ( i == 9  && this->_cbL9->Checked ) return 1;
  if ( i == 10 && this->_cbL10->Checked ) return 1;
  if ( i == 11 && this->_cbL11->Checked ) return 1;
  if ( i == 12 && this->_cbL12->Checked ) return 1;
  if ( i == 13 && this->_cbL13->Checked ) return 1;
  if ( i == 14 && this->_cbL14->Checked ) return 1;

/* Only relevant with Massman model */ 
  if ( i == 15  && this->_cb15->Checked ) return 1;
  if ( i == 16  && this->_cb16->Checked ) return 1;
  if ( i == 17  && this->_cb17->Checked ) return 1;
  if ( i == 18  && this->_cb18->Checked ) return 1;
  if ( i == 19 && this->_cb19->Checked ) return 1;
  if ( i == 20 && this->_cb20->Checked ) return 1;
  if ( i == 21 && this->_cb21->Checked ) return 1;   

  return 0;
}


/***********************************************************
* Load/Set and inital values to the Screen Controls 
***********************************************************/
int Form1::Load_Controls()
{
int L; 
float f;
char  cr[100]; 
String ^ A; 
String ^ CB; 

/* Make the Layer checkbox colors match the lines on the chart */
  this->_cbL1->BackColor =  this->chart1->Series["A_Lay1"]->Color;
  this->_cbL2->BackColor =  this->chart1->Series["A_Lay2"]->Color;
  this->_cbL3->BackColor =  this->chart1->Series["A_Lay3"]->Color;
  this->_cbL4->BackColor =  this->chart1->Series["A_Lay4"]->Color;
  this->_cbL5->BackColor =  this->chart1->Series["A_Lay5"]->Color;
  this->_cbL6->BackColor =  this->chart1->Series["A_Lay6"]->Color;
  this->_cbL7->BackColor =  this->chart1->Series["A_Lay7"]->Color;
  this->_cbL8->BackColor =  this->chart1->Series["A_Lay8"]->Color;
  this->_cbL9->BackColor =  this->chart1->Series["A_Lay9"]->Color;
  this->_cbL10->BackColor = this->chart1->Series["A_Lay10"]->Color;
  this->_cbL11->BackColor = this->chart1->Series["A_Lay11"]->Color;
  this->_cbL12->BackColor = this->chart1->Series["A_Lay12"]->Color;
  this->_cbL13->BackColor = this->chart1->Series["A_Lay13"]->Color;
  this->_cbL14->BackColor = this->chart1->Series["A_Lay14"]->Color;

/* Set colors for extended Massman model Layer Checkboxes, do this by using */
/* the extended Massman chart 'series' colors */ 
  this->_cb15->BackColor = this->chart1->Series["B_Lay15"]->Color; 
  this->_cb16->BackColor = this->chart1->Series["B_Lay16"]->Color; 
  this->_cb17->BackColor = this->chart1->Series["B_Lay17"]->Color; 
  this->_cb18->BackColor = this->chart1->Series["B_Lay18"]->Color; 
  this->_cb19->BackColor = this->chart1->Series["B_Lay19"]->Color; 
  this->_cb20->BackColor = this->chart1->Series["B_Lay20"]->Color; 
  this->_cb21->BackColor = this->chart1->Series["B_Lay21"]->Color; 

 /* Make all File Temperature Series Graph Lines same color as the Model ones */
  this->chart1->Series["B_Lay1"]->Color  =  this->chart1->Series["A_Lay1"]->Color;
  this->chart1->Series["B_Lay2"]->Color  =  this->chart1->Series["A_Lay2"]->Color;
  this->chart1->Series["B_Lay3"]->Color  =  this->chart1->Series["A_Lay3"]->Color;
  this->chart1->Series["B_Lay4"]->Color  =  this->chart1->Series["A_Lay4"]->Color;
  this->chart1->Series["B_Lay5"]->Color  =  this->chart1->Series["A_Lay5"]->Color;
  this->chart1->Series["B_Lay6"]->Color  =  this->chart1->Series["A_Lay6"]->Color;
  this->chart1->Series["B_Lay7"]->Color  =  this->chart1->Series["A_Lay7"]->Color;
  this->chart1->Series["B_Lay8"]->Color  =  this->chart1->Series["A_Lay8"]->Color;
  this->chart1->Series["B_Lay9"]->Color  =  this->chart1->Series["A_Lay9"]->Color;
  this->chart1->Series["B_Lay10"]->Color = this->chart1->Series["A_Lay10"]->Color;
  this->chart1->Series["B_Lay11"]->Color = this->chart1->Series["A_Lay11"]->Color;
  this->chart1->Series["B_Lay12"]->Color = this->chart1->Series["A_Lay12"]->Color;
  this->chart1->Series["B_Lay13"]->Color = this->chart1->Series["A_Lay13"]->Color;
  this->chart1->Series["B_Lay14"]->Color = this->chart1->Series["A_Lay14"]->Color;


/* Series graphing lines - set thickness */
  this->chart1->Series["B_Lay1"]->BorderWidth = 1;   
  this->chart1->Series["B_Lay2"]->BorderWidth = 1;   
  this->chart1->Series["B_Lay3"]->BorderWidth = 1;   
  this->chart1->Series["B_Lay4"]->BorderWidth = 1;   
  this->chart1->Series["B_Lay5"]->BorderWidth = 1;   
  this->chart1->Series["B_Lay6"]->BorderWidth = 1;   
  this->chart1->Series["B_Lay7"]->BorderWidth = 1;   
  this->chart1->Series["B_Lay8"]->BorderWidth = 1;   
  this->chart1->Series["B_Lay9"]->BorderWidth = 1;   
  this->chart1->Series["B_Lay10"]->BorderWidth = 1;   
  this->chart1->Series["B_Lay11"]->BorderWidth = 1;   
  this->chart1->Series["B_Lay12"]->BorderWidth = 1;   
  this->chart1->Series["B_Lay13"]->BorderWidth = 1;   
  this->chart1->Series["B_Lay14"]->BorderWidth = 1; 

  this->chart1->Series["B_Lay15"]->BorderWidth = 1;   /* Extended Massman Series */
  this->chart1->Series["B_Lay16"]->BorderWidth = 1; 
  this->chart1->Series["B_Lay17"]->BorderWidth = 1; 
  this->chart1->Series["B_Lay18"]->BorderWidth = 1; 
  this->chart1->Series["B_Lay19"]->BorderWidth = 1; 
  this->chart1->Series["B_Lay20"]->BorderWidth = 1; 
  this->chart1->Series["B_Lay21"]->BorderWidth = 1; 

  
/* Load Massman Soil Input textboxes */ 
  sprintf (cr, "%6.2f", e_SoiVolMoist);   /* Soil Moisture */ 
  A = _CharToStr(cr);  
  this->_txBM_Moist->Text = A; 

  sprintf (cr, "%6.3f", e_SoiBulDen);   /* Soil Buil Density */ 
  A = _CharToStr(cr);  
  this->_txBM_SoiBulDen->Text = A;  

  sprintf (cr, "%6.3f", e_SoiParDen);  /* Soil Particle Density */
  A = _CharToStr(cr);  
  this->_txBM_SoiParDen->Text = A;  

   
  sprintf (cr, "%6.1f", e_BurnTime);  /* Burn Time - Fire Duration */
  A = _CharToStr(cr);  
  this->_txFirDur->Text = A;  

  sprintf (cr, "%6.1f", e_MaxTim);    /* Time To Max watts applied to heat curve */
  A = _CharToStr(cr);  
  this->_txMaxTim->Text = A;  


  sprintf (cr, "%6.2f", e_Qabs);  /* Initial Fire Intensity Kw/m2 */
  A = _CharToStr(cr);  
  this->_txQabs->Text = A;  


  this->_cbSimTime->SelectedIndex = 2;  /* Simulation Time Combobox, select first item in list */

/* Fire Type ComboBox  */  
//   this->_cb_FireType->Items->Add (e_FT_Test); 
  this->_cb_FireType->Items->Add (e_FT_Wild); 
  this->_cb_FireType->Items->Add (e_FT_Pres); 
  this->_cb_FireType->Items->Add (e_FT_Pile); 
  this->_cb_FireType->Items->Add (e_FT_Burnup); 

 _SetComboBox (this->_cb_FireType, e_FT_Wild);

// _SetComboBox (this->_cb_FireType, e_FT_Test);


  return 1; 
}

/*******************************************************
* Name: Set_Axis
* Desc: Get the X & Y axis values from the GUI up-down
*       controls and set the chart graph axis 
*  Ret: i_TimMax...X axis limit (time - minutes)
*******************************************************/
int Form1::Set_Axis()
{
int i_TimMax; 
float f_Min, f_Max; 
char cr[100]; 

/* Min Max X axis limits - Minutes  ------------------------ */
  f_Min = 0;

  _StrToChar (this->_ud_XMax->Text,cr);
  f_Max = atof (cr);
  if ( f_Max < 1.0 ) { 
    myMessageBox ("X axis max setting invalid"); 
    return 0; }

  this->chart1->ChartAreas["ChartArea1"]->AxisX->Minimum = f_Min;
  this->chart1->ChartAreas["ChartArea1"]->AxisX->Maximum = f_Max ;

  i_TimMax = f_Max;    /* Time (minutes) of x axis */
/*------------------------------------------------------- */  
/*   Min Max Y axis limits - Temperature - Moisture       */

   if ( this->GraphType(cr) == e_Moist || this->GraphType(cr) == e_MoistTemp ) {
     this->chart1->ChartAreas["ChartArea1"]->AxisY->Maximum = 0.3 ;
     this->chart1->ChartAreas["ChartArea1"]->AxisY->Minimum = 0;
     this->chart1->ChartAreas["ChartArea1"]->AxisY->Title = L"Volumetric Moisture (m3/m3)";
     return i_TimMax; } 
      
  _StrToChar (this->_ud_YMin->Text,cr);
  f_Min = atof (cr);
  _StrToChar (this->_ud_YMax->Text,cr);
  f_Max = atof (cr);
  if ( f_Min >= f_Max ) { 
    myMessageBox ("Y axis min/max setting invalid"); 
    return 0; }

  this->chart1->ChartAreas["ChartArea1"]->AxisY->Minimum = f_Min;
  this->chart1->ChartAreas["ChartArea1"]->AxisY->Maximum = f_Max ;
  
  this->chart1->ChartAreas["ChartArea1"]->AxisY->Title = "Temperature C";

  return i_TimMax; 
}

/********************************************************
* Name: GetLay_Color
* Desc: Get the Color that is used by the CheckBox for 
*        the Layer
**********************************************************/
System::Drawing::Color Form1::GetLay_Color (int i_Lay)
{
// System::Drawing::Color  Color; 

  if ( i_Lay == 1 )  return this->_cbL1->BackColor;     // =   this->chart1->Series["A_Lay1"]->Color;
  if ( i_Lay == 2 )  return  this->_cbL2->BackColor;    
  if ( i_Lay == 3 )  return  this->_cbL3->BackColor;   
  if ( i_Lay == 4 )  return  this->_cbL4->BackColor;   
  if ( i_Lay == 5 )  return  this->_cbL5->BackColor;   
  if ( i_Lay == 6 )  return  this->_cbL6->BackColor;   
  if ( i_Lay == 7 )  return  this->_cbL7->BackColor;   
  if ( i_Lay == 8 )  return  this->_cbL8->BackColor;   
  if ( i_Lay == 9 )  return  this->_cbL9->BackColor;   
  if ( i_Lay == 10 )  return  this->_cbL10->BackColor;  
  if ( i_Lay == 11 )  return  this->_cbL11->BackColor;  
  if ( i_Lay == 12 )  return  this->_cbL12->BackColor;  
  if ( i_Lay == 13 )  return  this->_cbL13->BackColor;  
  if ( i_Lay == 14 )  return  this->_cbL14->BackColor;  
  
  return this->_cbL1->BackColor;  
} 





 