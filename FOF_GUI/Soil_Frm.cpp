

#include <stdio.h>
#include <math.h> 
#include <string.h>

#include "fof_sgv.h"
#include "Soil_Frm.h"
#include "Wnd_Util.h"
#include "fof_sha.h" 
#include "fof_util.h"

/* identifies a title field on the Soil Form */
#define e_MaxTemp  "MaxTemp"

float _Get_Interval (float X);
float  _NormalTemp ();
float _NextMark (float f_Val);
String ^ _MakeLable (CheckBox ^cb, Label ^lb, float f);

/**************************************************************
* Name: UpdateWatts
* Desc: Watts graph on the Soil Graphing Window 
* Note-1: we want to plot points on the mid-point of each X 
*         interval, so if interval is 10, points would be
*         0,10,20..., we'll plot them at 5,15,25,....
*
***************************************************************/
int Soil_Frm::UpdateWatts()
{
int i, iX, i_Sec, i_Inc;  
float f, f_Min, f_PC, f_Wts, f_Max, f_Kw; 

   this->chart2->Series["Series1"]->Points->Clear();
   this->chart2->Series["Series1"]->Points->AddXY(0,0); 

   i = SHA_GetInc();         /* # of seconds betweenn points  */
   i_Inc = i / 2;            /* See Note-1 above */

   for ( i = 0; i < 10000; i++ ) {
     SHA_TP_Get (i, &i_Sec, &f_PC, &f_Wts);
     if ( i_Sec < 0 ) 
       break; 
     i_Sec += i_Inc;
     f_Min = ToMinutes(i_Sec);           /* seconds to minutes */
     f_Kw = f_Wts / 1000.0;              /* to kilowatts */
     this->chart2->Series["Series1"]->Points->AddXY(f_Min,f_Kw); 
   }

   f_Max = this->chart1->ChartAreas["ChartArea1"]->AxisX->Maximum;  /* X limit from Layer graph */
   this->chart2->ChartAreas["ChartArea1"]->AxisX->Minimum = 0 ;
   this->chart2->ChartAreas["ChartArea1"]->AxisX->Maximum = f_Max ;

   f = this->chart1->ChartAreas["ChartArea1"]->AxisX->Interval;     /* from Layer graph */
   this->chart2->ChartAreas["ChartArea1"]->AxisX->Interval = f;     /* set the same */

   return 1; 
}

/***************************************************************
*
* 
***************************************************************/
int Soil_Frm::Update(d_SI *a_SI, d_SO *a_SO)
{
  memcpy (this->a_SI,a_SI,sizeof(d_SI)); 
  memcpy (this->a_SO,a_SO,sizeof(d_SO));
  this->UpdateDraw();  
 return 1;
}

/****************************************************
* Name: SetTitles
* Desc: Put some labels on the side of the soil graph
*       ex: Moisture, Soil Type, etc
****************************************************/
int Soil_Frm::SetTitles(float f_SoilMoist, char cr_SoilType[], 
                         float f_cDufPost, char cr_Unit[], int i_MaxTemp, int i_MaxTim)
{
float f; 
char  cr[1000]; 
String ^ Str;
String ^ S;

/* NOTE I can't push the last line of text to far down, for some reason   */
/*      it will only go so far down */
#define e_YBase 67   // 65 
#define e_YInc   4   // 5 

/* put the soil moisture label */
  sprintf (cr, "Soil Moisture %3.0f", f_SoilMoist);
  Str = _CharToStr (cr); 
  S = "Moisture";
  this->chart1->Titles[S]->Text = Str; 
  this->chart1->Titles[S]->Position->Auto = false;
  this->chart1->Titles[S]->Position->X = 1;
  this->chart1->Titles[S]->Position->Y =  e_YBase;   //     65;  // 70; 
  this->chart1->Titles[S]->Position->Width = 30;
  this->chart1->Titles[S]->Position->Height = 20; 
  this->chart1->Titles[S]->Visible = true; 

/* put the Soil Type Label */
  sprintf (cr, "Soil Type %s", cr_SoilType);
  Str = _CharToStr (cr); 
  S = "SoilType";
  this->chart1->Titles[S]->Text = Str; 
  this->chart1->Titles[S]->Position->Auto = false;
  this->chart1->Titles[S]->Position->X = 1;
  this->chart1->Titles[S]->Position->Y = e_YBase + e_YInc; // 70;  // 65; 
  this->chart1->Titles[S]->Position->Width = 30;
  this->chart1->Titles[S]->Position->Height = 20;
  this->chart1->Titles[S]->Visible = true; 

/* Post Duff Depth label */
  if ( !stricmp (cr_Unit, "Metric" ) ) 
      f = CentToInch (f_cDufPost);
  else 
      f = f_cDufPost;
  sprintf (cr, "Post Fire Duff Depth %4.2f (in)", f); 
  Str = _CharToStr(cr); 
  S = "PostDufDep";
  this->chart1->Titles[S]->Text = Str; 
  this->chart1->Titles[S]->Position->Auto = false;
  this->chart1->Titles[S]->Position->X = 1;
  this->chart1->Titles[S]->Position->Y = e_YBase + ( 2 * e_YInc) ; // 75;  // 70; 
  this->chart1->Titles[S]->Position->Width = 30;
  this->chart1->Titles[S]->Position->Height = 20;  
  this->chart1->Titles[S]->Visible = true; 

/* Max Tempurate label, what value we show depends on what Lay  */
/* checkbox that the user has checked, whichever is closest to surface */
//  i_Max = 0; 
//  i_MaxTim = 0; 
//  i = Get_HighLayChk();   /* lay checkbox closest to suface that is checked by user */
//  if ( i != 0 ) { 
//    i_Max = this->a_SO->ir_Temp[i-1];
//    i_MaxTim = this->a_SO->ir_Time[i-1];}

  S = e_MaxTemp;
  if ( i_MaxTemp <= 0 ) {
    strcpy (cr, "NO Heat Has Reached Soil"); 
    this->chart1->Titles[S]->ForeColor = System::Drawing::Color::Red; }
  else {
    sprintf (cr, "Max Temp %d (C) @ %d (min)", i_MaxTemp, i_MaxTim); 
    this->chart1->Titles[S]->ForeColor = System::Drawing::Color::Black;}
 
  Str = _CharToStr(cr); 
  this->chart1->Titles[S]->Text = Str; 
  this->chart1->Titles[S]->Position->Auto = false;
  this->chart1->Titles[S]->Position->X = 1;
  this->chart1->Titles[S]->Position->Y = e_YBase + ( 3 * e_YInc) ; // 80;  // 70; 
  this->chart1->Titles[S]->Position->Width = 30;
  this->chart1->Titles[S]->Position->Height = 20;
  this->chart1->Titles[S]->Visible = true; 
  return 1;
}


/**************************************************************
* Name: UpdateDraw
* Desc: Draw the Graph
* Note-1: 
*
**************************************************************/
int Soil_Frm::UpdateDraw() 
{
int i,j, X,L,T, i_MaxMinutes; 
int iS_GotIt; 
float f,f_Tmax, f_Inc, f_Tim, f_Minutes, f_MaxTemp, f_MinTemp; 
char cr[1000], cr_Unit[50];
String ^ Str;
String ^S; 
int i_MaxTim = 0;
int i_Max;  
float f_Max = 0;

  f = SHA_GetInc();
  f_Inc = f;    // / 60;
  if ( f_Inc <= 0 ) 
     return 0; 

/* if by chance the Soil Input FIle Graph is display */
  if ( this->b_GraphFile  == true ){ /* bring it back in view Checkboxes */
    this->Show_ChkBox(true); 
    this->Set_ChkBox();
    this->Set_Visable();
    this->_cbStyle->Visible = true; 
    this->_cbStyle->Checked = false;  
    this->Set_Style(); 
    this->b_GraphFile = false; }

/* Make sure X & Y axis limit textboxes are showing in case they were */
/*  hidden if user did a soil graph using and input file */ 
  this->_txXAxis->Visible = true;
  this->_txYAxis->Visible = true; 

/* Max Tempurate label, what value we show depends on what Lay  */
/* checkbox that the user has checked, whichever is closest to surface */
  i_Max = 0; 
  i_MaxTim = 0; 
  i = Get_HighLayChk();   /* lay checkbox closest to suface that is checked by user */
  if ( i != 0 ) { 
    i_Max = this->a_SO->ir_Temp[i-1];
// 
// orig    i_MaxTim = this->a_SO->ir_TimSec[i-1] / 60;}    /* get it to minutes */
    f  = this->a_SO->ir_TimSec[i-1];
    f = f / 60.0;
    f += 0.5; 
    i_MaxTim = f; 
   }  /* if */ 

/* Put moist, soil type, etc Titles onto the graph */ 
  f = SHA_DufDepRem(); 
  strcpy (cr_Unit,"English");
  SetTitles (this->a_SI->f_SoilMoist, this->a_SI->cr_SoilType, f,cr_Unit , i_Max, i_MaxTim);

/* Clear each layer series/line on graph and establish it's starting point */
  for ( L = 1; L <= eC_Lay; L++ ) {             /* For Each Soil Layer */
    sprintf (cr, "Lay%d", L);
    Str =_CharToStr ( cr); 
    this->chart1->Series[Str]->Points->Clear();
    this->chart1->Series[Str]->Points->AddXY (0, 21.0);  /* 21 = starting soil temp in celsius */
  }
 
  f_Inc = SHA_GetInc();
  f_MaxTemp = -1; 
  f_MinTemp = 99999;
/*-----------------------------------------------------*/
  for ( L = 1; L <= eC_Lay; L++ ) {                    /* For Each Soil Layer */
    sprintf (cr, "Lay%d", L);
    Str =_CharToStr ( cr); 
    f_Tim = f_Inc / 2.0;                              /* plot on mid-point of time increments */
 
   for ( i = 0; i < 100000; i++  ) {                 /* get each temp from stored array */
 //     if ( i >= i_MaxMinutes )                        /* arbitrary number of minutes we won't go past*/
 //       break; 
      f = SHA_Get (L,i);                              /* temp for Lay & Time */ 
      if ( f < 0 )                                    /* No more heat values in this layer */
         break;
      f_Minutes = f_Tim / 60.0; 
      this->chart1->Series[Str]->Points->AddXY(f_Minutes, f); /* X, Y   plot the point */
      f_Tim += f_Inc; 
      if ( f > f_MaxTemp ) 
        f_MaxTemp = f; 
      if ( f < f_MinTemp )
        f_MinTemp = f; 
   } } /* for T for L */

//    f_Tmax = f_Tim;           /* last point on X axis we ploted */
   
/* Put up the 60 degree line across graph */ 
   this->chart1->Series["Temp-60"]->Points->Clear(); 
   this->chart1->Series["Temp-60"]->Points->AddXY (0, 60); 
   this->chart1->Series["Temp-60"]->Points->AddXY (f_Minutes, 60); 


/* Set Max X & Y Axis, if user didn't enter an amount in the GUI textbox */
  if ( !this->Set_XAxis()) {                       /* User entered X axis */
    this->chart1->ChartAreas["ChartArea1"]->AxisX->Minimum = 0 ;
    this->chart1->ChartAreas["ChartArea1"]->AxisX->Maximum = f_Minutes ;
    this->chart1->ChartAreas["ChartArea1"]->AxisX->Interval = _Get_Interval (f_Minutes);  } 

  f_MaxTemp = _NextMark (f_MaxTemp); 

  if ( f_MaxTemp < 40.0 )
     f_MaxTemp = 40; 

  if ( !this->Set_YAxis()) {                       /* User entered Y axis */
    this->chart1->ChartAreas["ChartArea1"]->AxisY->Minimum = 0 ;
    this->chart1->ChartAreas["ChartArea1"]->AxisY->Maximum = f_MaxTemp ;
    this->chart1->ChartAreas["ChartArea1"]->AxisY->Interval = _Get_Interval (f_MaxTemp);  } 

  return 1; 

}
/*****************************************************************
* Round the number up to the next 10's boundry
*   examples  45 -> 50,  103 -> 110 
*****************************************************************/
float _NextMark (float f_Val)
{
float f, g; 

   f = f_Val / 10 ;
   f++;
   f = f * 10; 
   return f; 

}
/************************************************
* Name: _NormalTemp
* Desc: Go backwards thru the Soil Layer temp
*       arrays to see where they hit a normal
*       temp (21 celsus,is defualt used in soil
*       sim.) 
* NOTE: We use this to know where on the minutes
*       X axis to stop drawing the graph
*  Ret: index into soil temp array where we found
*        the temp returned to normal.
*       
************************************************/
float  _NormalTemp ()
{
int L,T,iX;
float f; 
int ir[eC_Lay+1]; 

  for ( L = 1; L <= eC_Lay; L++ ) {   /* Each layer */
    ir[L] = 0;
    for ( T = (eC_Tim -1); T > 0; T-- ){  
      f = SHA_Get(L,T);               /* Get temp */
      if ( f >= 22.0 ){            
         ir[L] = T;
         break; }
  }}

/* See which layer is the last to come back */
/*  down to a normal temp */
  iX = 1;
  for ( L = 1; L <= eC_Lay; L++ ) {
    if (ir[L] > iX)
      iX = ir[L]; }

  f = iX; 
  return f ; 
}

/**********************************************************************
*
*
**********************************************************************/
void Soil_Frm::Clear()
{
 this->chart1->Series["Lay1"]->Points->Clear();
 this->chart1->Series["Lay2"]->Points->Clear();
 this->chart1->Series["Lay3"]->Points->Clear();
 this->chart1->Series["Lay4"]->Points->Clear();
 this->chart1->Series["Lay5"]->Points->Clear();
 this->chart1->Series["Lay6"]->Points->Clear();
 this->chart1->Series["Lay7"]->Points->Clear();
 this->chart1->Series["Lay8"]->Points->Clear();
 this->chart1->Series["Lay9"]->Points->Clear();
 this->chart1->Series["Lay10"]->Points->Clear();
 this->chart1->Series["Lay11"]->Points->Clear();
 this->chart1->Series["Lay12"]->Points->Clear();
 this->chart1->Series["Lay13"]->Points->Clear();
 this->chart1->Series["Lay14"]->Points->Clear();

 SetTitles( 0, "", 0, "",  0, 0);

/* Pocosin Cover Type warning/Note message */ 
 this->_lbPocoMess->Visible = false; 
 this->_lbPocoNote->Visible = false; 

   this->chart2->Series["Series1"]->Points->Clear();  /* kilowatt graph */

}


/**************************************************************
*
**************************************************************/
int Soil_Frm::Init()
{
int i;
char cr[100];
String ^ Str;

 // myMessageBox ("Test"); 

  this->a_SO = new d_SO;
  this->a_SI = new d_SI; 
 
  this->_cbStyle->Checked = false; 

  this->_cbL1->BackColor =  this->chart1->Series["Lay1"]->Color;
  this->_cbL2->BackColor =  this->chart1->Series["Lay2"]->Color;
  this->_cbL3->BackColor =  this->chart1->Series["Lay3"]->Color;
  this->_cbL4->BackColor =  this->chart1->Series["Lay4"]->Color;
  this->_cbL5->BackColor =  this->chart1->Series["Lay5"]->Color;
  this->_cbL6->BackColor =  this->chart1->Series["Lay6"]->Color;
  this->_cbL7->BackColor =  this->chart1->Series["Lay7"]->Color;
  this->_cbL8->BackColor =  this->chart1->Series["Lay8"]->Color;
  this->_cbL9->BackColor =  this->chart1->Series["Lay9"]->Color;
  this->_cbL10->BackColor = this->chart1->Series["Lay10"]->Color;
  this->_cbL11->BackColor = this->chart1->Series["Lay11"]->Color;
  this->_cbL12->BackColor = this->chart1->Series["Lay12"]->Color;
  this->_cbL13->BackColor = this->chart1->Series["Lay13"]->Color;
  this->_cbL14->BackColor = this->chart1->Series["Lay14"]->Color;

//  Str = "---";
   Str = "-"; 
  this->_cbL1->Text = Str;
  this->_cbL2->Text = Str;
  this->_cbL3->Text = Str;
  this->_cbL4->Text = Str;
  this->_cbL5->Text = Str;
  this->_cbL6->Text = Str;
  this->_cbL7->Text = Str;
  this->_cbL8->Text = Str;
  this->_cbL9->Text = Str;
  this->_cbL10->Text = Str;
  this->_cbL11->Text = Str;
  this->_cbL12->Text = Str;
  this->_cbL13->Text = Str;
  this->_cbL14->Text = Str;


  this->_cbL1->Checked = true;
  this->_cbL3->Checked = true;
  this->_cbL5->Checked = true;
  this->_cbL7->Checked = true;
  this->_cbL9->Checked = true;
  this->_cbL11->Checked = true;
  this->_cbL13->Checked = true;

  return 1; 
}

/******************************************************************
*
*
*
******************************************************************/
void Soil_Frm::Set_Style()
{
int L;
char cr[100];
String ^ Str; 

 for ( L = 1; L <= eC_Lay; L++ ) {  /* For each Layer */
    sprintf (cr, "Lay%d", L);       /* form the layer's serices name */
    Str =_CharToStr ( cr); 
    if ( this->_cbStyle->Checked == true ) /* the 'line' style checkbox */
      this->chart1->Series[Str]->ChartType = 
        System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
    else 
      this->chart1->Series[Str]->ChartType = 
        System::Windows::Forms::DataVisualization::Charting::SeriesChartType::SplineArea;
 }
}

/***************************************************************
* Name: Set_Visable
* Desc: Set which layer will be shown on the graph based
*        on which layer checkboxs are checked
***************************************************************/
bool Soil_Frm::Set_Visable() 
{

   this->chart1->Series["Lay1"]->Enabled = false;
   this->chart1->Series["Lay2"]->Enabled = false;
   this->chart1->Series["Lay3"]->Enabled = false;
   this->chart1->Series["Lay4"]->Enabled = false;
   this->chart1->Series["Lay5"]->Enabled = false;
   this->chart1->Series["Lay6"]->Enabled = false;
   this->chart1->Series["Lay7"]->Enabled = false;
   this->chart1->Series["Lay8"]->Enabled = false;
   this->chart1->Series["Lay9"]->Enabled = false;
   this->chart1->Series["Lay10"]->Enabled = false;
   this->chart1->Series["Lay11"]->Enabled = false;
   this->chart1->Series["Lay12"]->Enabled = false;
   this->chart1->Series["Lay13"]->Enabled = false;
   this->chart1->Series["Lay14"]->Enabled = false;
   
   if ( this->_cbL1->Checked  ) this->chart1->Series["Lay1"]->Enabled = true;
   if ( this->_cbL2->Checked  ) this->chart1->Series["Lay2"]->Enabled = true;
   if ( this->_cbL3->Checked  ) this->chart1->Series["Lay3"]->Enabled = true;
   if ( this->_cbL4->Checked  ) this->chart1->Series["Lay4"]->Enabled = true;
   if ( this->_cbL5->Checked  ) this->chart1->Series["Lay5"]->Enabled = true;
   if ( this->_cbL6->Checked  ) this->chart1->Series["Lay6"]->Enabled = true;
   if ( this->_cbL7->Checked  ) this->chart1->Series["Lay7"]->Enabled = true;
   if ( this->_cbL8->Checked  ) this->chart1->Series["Lay8"]->Enabled = true;
   if ( this->_cbL9->Checked  ) this->chart1->Series["Lay9"]->Enabled = true;
   if ( this->_cbL10->Checked ) this->chart1->Series["Lay10"]->Enabled = true;
   if ( this->_cbL11->Checked ) this->chart1->Series["Lay11"]->Enabled = true;
   if ( this->_cbL12->Checked ) this->chart1->Series["Lay12"]->Enabled = true;
   if ( this->_cbL13->Checked ) this->chart1->Series["Lay13"]->Enabled = true;
   if ( this->_cbL14->Checked ) this->chart1->Series["Lay14"]->Enabled = true;
   
   return false ;
}

/***************************************************
*
***************************************************/
void Soil_Frm::Show_ChkBox(bool b)
{

  this->_cbL1->Visible = b; 
  this->_cbL2->Visible = b; 
  this->_cbL3->Visible = b; 
  this->_cbL4->Visible = b; 
  this->_cbL5->Visible = b; 
  this->_cbL6->Visible = b; 
  this->_cbL7->Visible = b; 
  this->_cbL8->Visible = b; 
  this->_cbL9->Visible = b; 
  this->_cbL10->Visible = b; 
  this->_cbL11->Visible = b; 
  this->_cbL12->Visible = b; 
  this->_cbL13->Visible = b; 
  this->_cbL14->Visible = b; 
}

/****************************************************
*
******************************************************/
   void Soil_Frm::Set_ChkBox()
{
  this->_cbL1->Checked = true;
  this->_cbL2->Checked = false; 
  this->_cbL3->Checked = true;
 this->_cbL4->Checked = false; 
  this->_cbL5->Checked = true;
 this->_cbL6->Checked = false; 
  this->_cbL7->Checked = true;
 this->_cbL8->Checked = false; 
  this->_cbL9->Checked = true;
 this->_cbL10->Checked = false; 
  this->_cbL11->Checked = true;
 this->_cbL12->Checked = false; 
  this->_cbL13->Checked = true;
 this->_cbL14->Checked = false; 
}

/****************************************************
* Name: Get_HighLayChk
* Desc: See what layer checkbox is closest to the
*       surface that is Checked
******************************************************/
int Soil_Frm::Get_HighLayChk()
{
  if ( this->_cbL1->Checked ) return 1;
  if ( this->_cbL2->Checked ) return 2;
  if ( this->_cbL3->Checked ) return 3;
  if ( this->_cbL4->Checked ) return 4;
  if ( this->_cbL5->Checked ) return 5;
  if ( this->_cbL6->Checked ) return 6;
  if ( this->_cbL7->Checked ) return 7;
  if ( this->_cbL8->Checked ) return 8;
  if ( this->_cbL9->Checked ) return 9;
  if ( this->_cbL10->Checked ) return 10;
  if ( this->_cbL11->Checked) return 11;
  if ( this->_cbL12->Checked ) return 12;
  if ( this->_cbL13->Checked ) return 13;
  if ( this->_cbL14->Checked ) return 14;
  return 0; 
  }


/*******************************************************************
* Name: Set_XAxis
* Desc: Reset the X axis on the graph - the graph will automatically
*       redraw when this is set
*       This gets called when user enter/changes the X axis 
*       textBox
*  Ret: 1 = text found in textbox and graph set
*       0 = no text found, graph not set
********************************************************************/
int Soil_Frm::Set_XAxis()
{
float f, f_X; 
char cr[500], cr_ErrMes[500];
 
/* Only set when something is found in the Axis textbox */
 if ( _Get_TBFlo (this->_txXAxis, &f_X, cr, "X Axis Limit", cr_ErrMes)) {
   if ( f_X > eC_Tim )      /* max minutes soil sim will run */ 
     f_X = eC_Tim; 
   this->chart1->ChartAreas["ChartArea1"]->AxisX->Maximum = f_X;

   f = _Get_Interval (f_X); 
   this->chart1->ChartAreas["ChartArea1"]->AxisX->Interval = f; 
   return 1;}
 return 0; 
 }

/***************************************************************
* same as above except for th Y axis 
****************************************************************/
int Soil_Frm::Set_YAxis()
{
float f, f_Y; 
char cr[500], cr_ErrMes[500];
 
/* Only set when something is found in the Axis textbox */
 if ( _Get_TBFlo (this->_txYAxis, &f_Y, cr, "Y Axis Limit", cr_ErrMes)) {
   if ( f_Y > 1200.0 )            /* I just a number to limit */
     f_Y = 1200.0; 
   this->chart1->ChartAreas["ChartArea1"]->AxisY->Maximum = f_Y; 
       
   f = _Get_Interval (f_Y); 
   this->chart1->ChartAreas["ChartArea1"]->AxisY->Interval = f; 
   return 1;}
 return 0; 
 }



/*****************************************************************
* Name: Get_Interval
* Desc: based on the total size of the axis, get an interval that 
*       is used to make on the graph's axis
* NOTE: I wrote this for X axis, but it'll work for Y too 
******************************************************************/
float _Get_Interval (float X)
{
  if ( X <= 10.0 ) return 1.0;
  if ( X <= 20.0 ) return 2.0;
  if ( X <= 50.0 ) return 5.0;
  if ( X <= 100.0 ) return 10.0;
  if ( X <= 200.0 ) return 20.0; 
  if ( X <= 500.0 ) return 50.0;
  return 100.0;
}


/*************************************************************
* Name Graph_File()
* Desc: Make a graph when the user is doing the
*       Soil Inputs File Option
* Note-1:  
*      > array position [1] is surface layer [2] is next layer down, [0] isn't used
*      > the Label names are not in any kind of reason manner
*      > user can specify any number (up to limit) of layers they want to do
*         and at what depth they are, so i check the array and if there's 
*         a depth in a [] then I display a checkbox and label,
* 
*************************************************************/
int Soil_Frm::Graph_File(char cr_Mode[] )
{
int i, LL, i_MaxMinutes, L, T;
float f, f_MaxTemp, f_MaxTime; 
char cr[500]; 
String ^ Str, ^ S; 

   this->Text = e_TitleUF;

   if ( stricmp (cr_Mode, "Init" ) )
      goto A; 
/* See Note-1 above................................ */
   this->_cbL1->Visible  = true;                   /* Always display surface */
   this->label1->Visible = true;
   this->label1->Text = "Surface"; 
   this->chart1->Series["Lay1"]->LegendText = "Surface"; 

   this->chart1->Series["Lay2"]->LegendText =  _MakeLable (this->_cbL2,this->label3,  this->FR[2]);    /* First layer under the surface */
   this->chart1->Series["Lay3"]->LegendText =  _MakeLable (this->_cbL3,this->label4,  this->FR[3]);
   this->chart1->Series["Lay4"]->LegendText =  _MakeLable (this->_cbL4,this->label5,  this->FR[4]);
   this->chart1->Series["Lay5"]->LegendText =  _MakeLable (this->_cbL5,this->label6,  this->FR[5]);
   this->chart1->Series["Lay6"]->LegendText =  _MakeLable (this->_cbL6,this->label10, this->FR[6]);
   this->chart1->Series["Lay7"]->LegendText =  _MakeLable (this->_cbL7,this->label9,  this->FR[7]);
   this->chart1->Series["Lay8"]->LegendText =  _MakeLable (this->_cbL8,this->label8,  this->FR[8]);
   this->chart1->Series["Lay9"]->LegendText =  _MakeLable (this->_cbL9,this->label7,  this->FR[9]);
   this->chart1->Series["Lay10"]->LegendText =  _MakeLable (this->_cbL10,this->label11, this->FR[10]);
   this->chart1->Series["Lay11"]->LegendText =  _MakeLable (this->_cbL11,this->label12, this->FR[11]);
   this->chart1->Series["Lay12"]->LegendText =  _MakeLable (this->_cbL12,this->label13, this->FR[12]);
   this->chart1->Series["Lay13"]->LegendText =  _MakeLable (this->_cbL13,this->label14, this->FR[13]);
   this->chart1->Series["Lay14"]->LegendText =  _MakeLable (this->_cbL14,this->label15, this->FR[14]);
 
A: 

/* Max Tempurate label, what value we show depends on what Lay  */
/* checkbox that the user has checked, whichever is closest to surface */
  int i_Max = 0; 
  int i_MaxTim = 0; 
  i = Get_HighLayChk();   /* lay checkbox closest to suface that is checked by user */
  
/* Get the Max Temp and Time/Mintes */
  f_MaxTemp = 0; 
  f_MaxTime = 0; 

  for ( T = 0; T < eC_Tim; T++  ) {
     f = SHA_Get (i,T); 
     if ( f < 0 ) 
       break; 
     if ( f > f_MaxTemp ){
       f_MaxTemp = f;
       f_MaxTime = T; }  
  } /* for */

  f = f_MaxTime * SHA_GetInc(); 
  f = f / 60.0;   /* seconds to minutes */
  SetTitles(this->f_SoilMoist,
            "UserFile", 
            this->f_DufDepPos,
            "Inches",
            f_MaxTemp,
            f);



// 8888888888888888888888888888888888888888888888888

float f_Inc, f_Tim, f_Minutes,  f_MinTemp; 

/* Clear each layer series/line on graph and establish it's starting point */
  for ( L = 1; L <= eC_Lay; L++ ) {             /* For Each Soil Layer */
    sprintf (cr, "Lay%d", L);
    Str =_CharToStr ( cr); 
    this->chart1->Series[Str]->Points->Clear();
    this->chart1->Series[Str]->Points->AddXY (0, 21.0);  /* 21 = starting soil temp in celsius */
  }
 
  f_Inc = SHA_GetInc();
  f_MaxTemp = -1; 
  f_MinTemp = 99999;
/*-----------------------------------------------------*/
  for ( L = 1; L <= eC_Lay; L++ ) {                    /* For Each Soil Layer */
    sprintf (cr, "Lay%d", L);
    Str =_CharToStr ( cr); 
    f_Tim = f_Inc / 2.0;                              /* plot on mid-point of time increments */
 
   for ( i = 0; i < 100000; i++  ) {                 /* get each temp from stored array */
 //     if ( i >= i_MaxMinutes )                        /* arbitrary number of minutes we won't go past*/
 //       break; 
      f = SHA_Get (L,i);                              /* temp for Lay & Time */ 
      if ( f < 0 )                                    /* No more heat values in this layer */
         break;
      f_Minutes = f_Tim / 60.0; 
      this->chart1->Series[Str]->Points->AddXY(f_Minutes, f); /* X, Y   plot the point */
      f_Tim += f_Inc; 
      if ( f > f_MaxTemp ) 
        f_MaxTemp = f; 
      if ( f < f_MinTemp )
        f_MinTemp = f; 
   } } /* for T for L */

//    f_Tmax = f_Tim;           /* last point on X axis we ploted */
   
/* Put up the 60 degree line across graph */ 
   this->chart1->Series["Temp-60"]->Points->Clear(); 
   this->chart1->Series["Temp-60"]->Points->AddXY (0, 60); 
   this->chart1->Series["Temp-60"]->Points->AddXY (f_Minutes, 60); 


/* Set Max X & Y Axis, if user didn't enter an amount in the GUI textbox */
  if ( !this->Set_XAxis()) {                       /* User entered X axis */
    this->chart1->ChartAreas["ChartArea1"]->AxisX->Minimum = 0 ;
    this->chart1->ChartAreas["ChartArea1"]->AxisX->Maximum = f_Minutes ;
    this->chart1->ChartAreas["ChartArea1"]->AxisX->Interval = _Get_Interval (f_Minutes);  } 

  f_MaxTemp = _NextMark (f_MaxTemp); 

  if ( f_MaxTemp < 40.0 )
     f_MaxTemp = 40; 

  if ( !this->Set_YAxis()) {                       /* User entered Y axis */
    this->chart1->ChartAreas["ChartArea1"]->AxisY->Minimum = 0 ;
    this->chart1->ChartAreas["ChartArea1"]->AxisY->Maximum = f_MaxTemp ;
    this->chart1->ChartAreas["ChartArea1"]->AxisY->Interval = _Get_Interval (f_MaxTemp);  } 

/* Make sure Pocosin warning message doesn't show */
 this->_lbPocoMess->Visible = false; 
 this->_lbPocoNote->Visible = false; 

// 888888888888888888888888888888888888888888888888888


#ifdef llllllllllllllllll
  this->chart1->ChartAreas["ChartArea1"]->AxisY->Minimum = 0 ;
  this->chart1->ChartAreas["ChartArea1"]->AxisY->Maximum = f_MaxTemp ;

  this->chart1->ChartAreas["ChartArea1"]->AxisX->Minimum = 0 ;
  if ( !this->Set_XAxis() )  /* See if user has set the X axis */
    this->chart1->ChartAreas["ChartArea1"]->AxisX->Maximum = f_MaxTime ;
 

  i = 0;
  LL = eC_Lay;
  i_MaxMinutes = 1000; 

  for ( L = 1; L <= eC_Lay; L++ ) {
    sprintf (cr, "Lay%d", L);
    Str =_CharToStr ( cr); 
    this->chart1->Series[Str]->Points->Clear();
    this->chart1->Series[Str]->Enabled = false;
   
    for ( T = 1; T < eC_Tim; T = T + 10  ) {
      if ( T >= eC_Tim )   /* this might happen because we inc with T + 10 */
       break;
      if ( T >= i_MaxMinutes )
        break; 
 //     if ( T == 1 )
 //       this->chart1->Series[Str]->Points->Clear();
      
      f = SHA_Get (L,T); 
      if ( f < 0 ) 
         break;
  //    if ( T == 1 ) 
        this->chart1->Series[Str]->Enabled = true;

      this->chart1->Series[Str]->Points->AddXY (T, f);   /* X, Y */
    
      if ( L == 7 ) {
        if ( f > 60 ) 
           i++; }
   } /* for T */
  } /* for L */ 
 

#endif 
  return 1; 
}

/*************************************************************
*  
*
**************************************************************/
String ^ _MakeLable (CheckBox ^cb, Label ^lb, float f)
{
char cr[100];
String ^ Str; 

  if ( f == 0 ) {         /* If no layer depth was specifed */
    cb->Visible = false;  /* hide checkbox */
    cb->Checked = false;   
    lb->Visible = false;  /* hide lable */
    return ""; }

  cb->Visible = true;     /* show the stuff */
  cb->Checked = true; 
  lb->Visible = true; 

  sprintf (cr,"%3.1f cm",f);   /* Depth label */
  Str = _CharToStr(cr); 
  lb->Text = Str; 
  return Str; 
} 