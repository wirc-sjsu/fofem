#include <stdio.h>
#include <string.h>
#include <math.h>

#include "Mort_Frm.h"
#include "Wnd_Util.h"
  
using namespace System::Windows::Forms::DataVisualization::Charting;

Color GetColor (int i);

/*********************************************************************
* Name: InitGraphPFI
* Desc: Init and clear the Mortality graph with the settings to do
*        a Postfire Injurty Graph
*       Do this everytime before redrawing it
*********************************************************************/
int Mort_Frm::InitGraphPFI()
{
  this->_chMort->Series->Clear(); 

  this->_chMort->Titles["MainTitle"]->Text = "Tree Mortality - Crown Damage/CKR/Beetle Model";
  
  this->_chMort->ChartAreas["ChartArea1"]->AxisY->Title = "Mortality Probability" ; 
  this->_chMort->ChartAreas["ChartArea1"]->AxisY->Minimum = 0 ; 
  this->_chMort->ChartAreas["ChartArea1"]->AxisY->Maximum = 1.10 ; 
  this->_chMort->ChartAreas["ChartArea1"]->AxisY->Interval = 0.20; 

  this->_chMort->ChartAreas["ChartArea1"]->AxisX->Title = "Crown Damage Percent" ; 
  this->_chMort->ChartAreas["ChartArea1"]->AxisX->Minimum = 0 ;
  this->_chMort->ChartAreas["ChartArea1"]->AxisX->Maximum = 100 ; 
  this->_chMort->ChartAreas["ChartArea1"]->AxisX->Interval = 10;  
  return 1; 
}

/************************************************************
* Name: InitGraphBol
* Desc: Initialize Bole char graph
*************************************************************/
int Mort_Frm::InitGraphBol(float max, float intv)
{
  this->_chMort->Series->Clear(); 

  this->_chMort->Titles["MainTitle"]->Text = "Tree Mortality - Bole Char Height Model";
  
  this->_chMort->ChartAreas["ChartArea1"]->AxisY->Title = "Mortality Probability" ; 
  this->_chMort->ChartAreas["ChartArea1"]->AxisY->Minimum = 0 ; 
  this->_chMort->ChartAreas["ChartArea1"]->AxisY->Maximum = 1.10 ; 
  this->_chMort->ChartAreas["ChartArea1"]->AxisY->Interval = 0.20; 

  this->_chMort->ChartAreas["ChartArea1"]->AxisX->Title = "Bole Char Height (ft)" ; 
  this->_chMort->ChartAreas["ChartArea1"]->AxisX->Minimum = 0 ;
  this->_chMort->ChartAreas["ChartArea1"]->AxisX->Maximum = max ; 
  this->_chMort->ChartAreas["ChartArea1"]->AxisX->Interval = intv;  
  return 1; 
}

/*********************************************************************
* Name: InitGraph
* Desc: Init and clear the Mortality graph.
*       Do this everytime before redrawing it
*  In: cr_FS..."FlameLength" or "ScorchHeight"
*********************************************************************/
int Mort_Frm::InitGraph(char cr_FS[])
{
  this->_chMort->Series->Clear(); 

 this->_chMort->Titles["MainTitle"]->Text = "Tree Mortality - Crown Scorch Model";
  
  this->_chMort->ChartAreas["ChartArea1"]->AxisY->Title = "Mortality Probability" ; 
  this->_chMort->ChartAreas["ChartArea1"]->AxisY->Minimum = 0 ; 
  this->_chMort->ChartAreas["ChartArea1"]->AxisY->Maximum = 110 ; 
  this->_chMort->ChartAreas["ChartArea1"]->AxisY->Interval = 20; 

  if ( !stricmp (cr_FS,"ScorchHeight")) { 
    this->_chMort->ChartAreas["ChartArea1"]->AxisX->Title = "Scorch Height (ft)" ; 
    this->_chMort->ChartAreas["ChartArea1"]->AxisX->Minimum = 0 ;
    this->_chMort->ChartAreas["ChartArea1"]->AxisX->Maximum = 240 ; 
    this->_chMort->ChartAreas["ChartArea1"]->AxisX->Interval = 20; } 
  else  {
    this->_chMort->ChartAreas["ChartArea1"]->AxisX->Title = "Flame Length (ft)" ; 
    this->_chMort->ChartAreas["ChartArea1"]->AxisX->Minimum = 0 ;
    this->_chMort->ChartAreas["ChartArea1"]->AxisX->Maximum = 20 ;
    this->_chMort->ChartAreas["ChartArea1"]->AxisX->Interval = 5; }
  
// this->_chMort->ChartAreas["ChartArea1"]->AxisX2->Enabled = AxisEnabled::True;

  return 1; 
}

/*********************************************************
* Name: UpdateGraph
* Desc: Draw a line on the graph
* Note-1: in certain situations the Prob line will get drawn on a straight
*         vertical line, and if it falls on say 20, 40,.. 100 then it's 
*         hard to distinghush from the axis line, so take of 0.1 makes the
*         line get drawn just below the axis line and be more noticable
*         
*   In: cr_Mode..."PFI" or "Reg"
*       Prob...the array of species probablity of mortality percents
*       FS.....flame/scorch height associated with each Prob
*       S_Name..Species name and DBB
*       iX......an incrementing number sent it, see code below
*               -1 to draw the cutoff line when doing a PFI graph 
*********************************************************/
int Mort_Frm::UpdateGraph( char cr_Mode[], float Prob[], float FS[], String ^ s_Name, int iX)
{
int i, j; 
float f; 
char cr[100];
String ^ s_Series;  
Color color; 
Series ^ se= (gcnew Series()) ;

  if ( iX < 0 ) /* 7/18/18 - DL, skip drawing the Red Prob cut off line  */
    return 1;   
 
  if ( iX < 0 )         /* PFI graph uses for doing Cut Off horzontal line */ 
    color = Color::Red;
  else 
    color = GetColor (iX); 
  
  sprintf (cr,"Series-%d",iX);  /* Make up unique name everytime  */ 
  s_Series = _CharToStr (cr);

  se->LegendText = s_Name;  /* This speceis-dbh will go in the legend box */
  se->ChartArea = L"ChartArea1";
  se->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
  se->Color = color ;
  se->Legend = L"Legend1";
  se->Name = s_Series ;           
 
  this->_chMort->Series->Add(se);

  for ( i = 0; i < 1000; i++ ) {  /* draw the line */
      if ( Prob[i] < 0 )
        break; 
      
     if ( !stricmp (cr_Mode,"Reg") ){  /* Doing Regular Mort */ 
         f = Prob[i] - 0.1;  /* See Note-1 above */
         if ( f < 0 ) 
           f = 0;   }  
     else 
         f = Prob[i];                 /* Doing Post Fire Injury */
      
      this->_chMort->Series[s_Series]->Points->AddXY ( FS[i],f );
 
  } /* for */

  return 1; 
}

/*************************************************************
* Name: Set_SecondAxis
* Desc: We need to create a Series  with a SecondaryAxis attached
*       to it and then draw a line on the chart in order
*       for the Seconday Axis to show up on the chart. 
*       I use the secondary axis to display the Scorch Height
*       on the X axis.
*       
*************************************************************/
int Mort_Frm::Set_SecondAxis()
{
int i, j; 
char cr[100];
String ^ s_Series;  
Color color; 
//System::Windows::Forms::DataVisualization::Charting::Series^  series1 
//  = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
Series ^ series1 = ( gcnew Series());
   s_Series = "Dummy";   /* Give the series a name */
   
   series1->LegendText = "Species DBH"; /* We'll turn this into a title in legend box */

   series1->ChartArea = L"ChartArea1";
   series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
   series1->Color = Color::Transparent;  /* actual chart line and line in legend is transpart */
   series1->Legend = L"Legend1";
   series1->Name = s_Series ;           
   series1->XAxisType = System::Windows::Forms::DataVisualization::Charting::AxisType::Secondary;
   this->_chMort->Series->Add(series1);

/* Draw a short vertical line on the 0 Y axis, so it won't show. */
   for ( i = 0; i < 10; i++ )
      this->_chMort->Series[s_Series]->Points->AddXY (i, 0);

   return 1; 
}

/************************************************************
* This gets me a different color for each line on the
* graph.
* I put a bunch in here so I can pick different colors if
* we want to change them, 
* Only the first few will probably ever get used. 
*
*  NOTE NOTE I save Color::Red; for doing the PFT mort cutoff
*              line
*
*
**************************************************************/
Color GetColor (int i)
{
      if ( i == 1 ) return Color::Green ;
 else if ( i ==  2 ) return Color::Blue  ;
 else if ( i ==  3 ) return Color::Yellow;
 else if ( i ==  4 ) return Color::Black;
 else if ( i ==  5 ) return Color::Brown ;
 else if ( i ==  6 ) return Color::Cyan  ;
 else if ( i ==  7 ) return Color::Orange;
 else if ( i ==  8 ) return Color::DarkBlue;
 else if ( i ==  9 ) return Color::DarkGray;
 else if ( i == 10 ) return Color::Maroon  ;
 else if ( i == 11 ) return Color::DarkGreen;
 else if ( i == 12 ) return Color::DarkOrange;
 else if ( i == 13 ) return Color::DarkRed ;
 else if ( i == 14 ) return Color::Khaki   ;
 else if ( i == 15 ) return Color::Magenta ;
 else if ( i == 16 ) return Color::Navy    ;
 else if ( i == 17 ) return Color::Violet  ;
 else if ( i == 18 ) return Color::AliceBlue ;
 else if ( i == 19 ) return Color::Beige     ;
 else if ( i == 20 ) return Color::Bisque    ;
 else if ( i == 21 ) return Color::BlueViolet;
 else if ( i == 22 ) return Color::Chartreuse;
 else if ( i == 23 ) return Color::Chocolate ;
 else if ( i == 24 ) return Color::Crimson   ;
 else if ( i == 25 ) return Color::DarkKhaki ;
 else if ( i == 26 ) return Color::DarkMagenta;
 else if ( i == 27 ) return Color::DarkOrchid ;
 else if ( i == 28 ) return Color::DarkViolet ;
 else if ( i == 29 ) return Color::DeepPink   ;
 else if ( i == 30 ) return Color::DeepSkyBlue;
 else if ( i == 31 ) return Color::DodgerBlue ;
 else if ( i == 32 ) return Color::Firebrick  ;
 else if ( i == 33 ) return Color::Gainsboro  ;
 else if ( i == 34 ) return Color::Goldenrod  ;
 else if ( i == 35 ) return Color::Gray       ;
 else if ( i == 36 ) return Color::GreenYellow;
 else if ( i == 37 ) return Color::HotPink    ;
 else if ( i == 38 ) return Color::IndianRed  ;
 else if ( i == 39 ) return Color::Indigo     ;
 else if ( i == 40 ) return Color::Lavender   ;
 else if ( i == 41 ) return Color::MediumBlue ;
 else if ( i == 42 ) return Color::Gold;
 return Color::Blue;
}
