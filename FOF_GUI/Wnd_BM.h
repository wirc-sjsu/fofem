/*****************************************************************************
* Name: Wnd_BM.h
* Desc: Massman Graphing Window
*   
*
******************************************************************************/
#pragma once

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

#include "..\FOF_GUI\Wnd_Util.h"
#include "..\FOF_DLL\BM_Soil\BMSoil.h"
#include "Help_Form.h"

/* The total number of Graphing/Layer Check Boxes on the GUI, not including extended Massman layers */  
#define e_CBmax  14

extern int gi_LastT; 

/***************************************************************/
public ref class Form1 : public System::Windows::Forms::Form
{
public:

  int i_FTBM;   /* Fire Type Burnup Message - do once switch */ 
  int iN_Runs;  
  d_BMI *xbmi; 
  int iS_Init; 
  bool b_Abort; 
  bool b_Running; 
  String ^ s_UserFolder; 
//  int   *ir_KwM2;  /* Heat  Kw/m2  */
//  int   i_TimInc;  /* Time increments - heat at array is in */ 

public: System::Windows::Forms::CheckBox^  _cb15;
public: System::Windows::Forms::CheckBox^  _cb16;
public: System::Windows::Forms::CheckBox^  _cb17;
public: System::Windows::Forms::CheckBox^  _cb18;
public: System::Windows::Forms::CheckBox^  _cb19;
public: System::Windows::Forms::CheckBox^  _cb20;
public: System::Windows::Forms::CheckBox^  _cb21;
private: System::Windows::Forms::GroupBox^  groupBox1;
private: System::Windows::Forms::Button^  _bt_FirIntGraph;
private: System::Windows::Forms::GroupBox^  _gbFirInt;
private: System::Windows::Forms::TextBox^  _txFirDur;
private: System::Windows::Forms::TextBox^  _txQabs;
private: System::Windows::Forms::Label^  label5;
private: System::Windows::Forms::Label^  label6;
private: System::Windows::Forms::ComboBox^  _cbSimTime;
private: System::Windows::Forms::TextBox^  _txMaxTim;
private: System::Windows::Forms::Label^  label77;
private: System::Windows::Forms::Label^  label7;
public: System::Windows::Forms::Button^  _bt_Quit;
public: System::Windows::Forms::Button^  button2;
private: System::Windows::Forms::ComboBox^  _cb_FireType;
private: System::Windows::Forms::Label^  label2;
public: System::Windows::Forms::Label^  label3;
public: System::Windows::Forms::Label^  label4;
private: System::Windows::Forms::Button^  _btHelpRef;
public: System::Windows::Forms::Label^  label90;
public: System::Windows::Forms::Label^  label8;
private: System::Windows::Forms::GroupBox^  _gbAxisLimit;
private: System::Windows::Forms::TextBox^  _txBM_AmbAirTmp;
public: 
private: System::Windows::Forms::Label^  label9;

public: 
private: System::Windows::Forms::TextBox^  textBox1;
public: 
public: 
private: System::Windows::Forms::Label^  label88;
public: 
   int Burnup(); 
   int FireTypeSet(); 
   int Load_Controls(); 
   int isChecked(int i);
   int SetFireTypeBurnup(); 
   int NumRuns (); 

   int ChkBoxArr(int ir[], int iN); 

   int FireInten_Graph(double  fr[], float f_IncSec ); 
   int BM_Run (char cr_Mode[]); 
  int Graph_TempMoist (d_BMI *bmi);
  int FOFEM_HMV_Model(d_BMI *bmi, char cr_Mode[]);
  int OutputFile(char cr_Mode[], char cr_Out[]); 

   int ClearTempGraph (); 
   int Set_Axis (); 

#define e_Temp 1
#define e_Moist 2
#define e_MoistTemp 3
   int GraphType(char cr_TemMoi[]); 

   System::Drawing::Color GetLay_Color (int i_Lay); 

public: System::Windows::Forms::NumericUpDown^  _ud_XMax;
public: System::Windows::Forms::NumericUpDown^  _ud_YMax;
public: System::Windows::Forms::NumericUpDown^  _ud_YMin;
public: System::Windows::Forms::CheckBox^  _cbL4;
public: System::Windows::Forms::CheckBox^  _cbL5;
public: System::Windows::Forms::CheckBox^  _cbL6;
public: System::Windows::Forms::CheckBox^  _cbL7;
public: System::Windows::Forms::CheckBox^  _cbL8;
public: System::Windows::Forms::CheckBox^  _cbL9;
public: System::Windows::Forms::CheckBox^  _cbL10;
public: System::Windows::Forms::CheckBox^  _cbL11;
public: System::Windows::Forms::CheckBox^  _cbL12;
public: System::Windows::Forms::CheckBox^  _cbL13;
public: System::Windows::Forms::CheckBox^  _cbL14;
public: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart2_Rabs;
private: System::Windows::Forms::Label^  _lb_Temperature;
private: System::Windows::Forms::Label^  label15;
private: System::Windows::Forms::Label^  label17;
private: System::Windows::Forms::Button^  _bt_AxReset;
private: System::Windows::Forms::ToolTip^  toolTip1;
private: System::Windows::Forms::TextBox^  _txMess;
private: System::Windows::Forms::Label^  label23;
private: System::Windows::Forms::Button^  _btBM_GraphClear;
private: System::Windows::Forms::RadioButton^  _rb_Temp;
private: System::Windows::Forms::RadioButton^  _rb_Moist;
private: System::Windows::Forms::TextBox^  _txBM_Moist;
private: System::Windows::Forms::Label^  label37;
private: System::Windows::Forms::TextBox^  _txBM_SoiBulDen;
private: System::Windows::Forms::TextBox^  _txBM_SoiParDen;
private: System::Windows::Forms::Label^  label39;
private: System::Windows::Forms::Label^  label40;
private: System::Windows::Forms::Label^  label41;
private: System::Windows::Forms::Label^  label42;
private: System::Windows::Forms::Button^  button1;
private: System::Windows::Forms::Button^  _btBM_DrawGraph;

public: 
  

/*----------------------------------------------------*/
Form1(void)
{
String ^ A; 
String ^ Str; 

  InitializeComponent();
  this->iS_Init = 1; 

  i_FTBM = 0;           /* Fire Type Burnup Message - do once switch */ 
  iN_Runs = 0; 

  xbmi = new d_BMI();  

//  ir_KwM2 = new int [eC_nsteps]; 
// ir_KwM2[0] = -1; 
//  i_TimInc = 0; 

  this->b_Abort = false; 
  this->b_Running = false; 
  Load_Controls();  

//  A = Environment::ExpandEnvironmentVariables("%userprofile%");
 // Str = A + "\\BM_Soil";
//  if ( !System::IO::Directory::Exists(Str) ){     /* Folder files alread in place */
//        System::IO::Directory::CreateDirectory(Str);   }

AAA: 
  
  this->iS_Init = 0; 
}

/*----------------------------------------------------*/	
~Form1()
{
 if (components)
	delete components;
  delete xbmi; 
// delete ir_KwM2;
}

/***********************************************************/

    private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart1;
    public: System::Windows::Forms::CheckBox^  _cbL3;
    public: System::Windows::Forms::CheckBox^  _cbL2;
    public: System::Windows::Forms::CheckBox^  _cbL1;
private: System::ComponentModel::IContainer^  components;
public: 

public: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->components = (gcnew System::ComponentModel::Container());
            System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea7 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
            System::Windows::Forms::DataVisualization::Charting::Legend^  legend4 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
            System::Windows::Forms::DataVisualization::Charting::Series^  series151 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series152 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series153 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series154 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series155 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series156 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series157 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series158 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series159 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series160 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series161 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series162 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series163 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series164 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series165 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series166 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series167 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series168 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series169 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series170 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series171 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series172 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series173 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series174 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series175 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series176 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series177 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series178 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series179 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series180 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series181 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series182 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series183 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series184 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series185 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series186 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series187 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series188 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series189 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series190 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series191 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series192 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series193 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series194 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series195 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series196 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series197 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series198 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series199 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Title^  title7 = (gcnew System::Windows::Forms::DataVisualization::Charting::Title());
            System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea8 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
            System::Windows::Forms::DataVisualization::Charting::Series^  series200 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Title^  title8 = (gcnew System::Windows::Forms::DataVisualization::Charting::Title());
            this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
            this->_cbL3 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbL2 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbL1 = (gcnew System::Windows::Forms::CheckBox());
            this->_ud_XMax = (gcnew System::Windows::Forms::NumericUpDown());
            this->_ud_YMax = (gcnew System::Windows::Forms::NumericUpDown());
            this->_ud_YMin = (gcnew System::Windows::Forms::NumericUpDown());
            this->_cbL4 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbL5 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbL6 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbL7 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbL8 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbL9 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbL10 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbL11 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbL12 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbL13 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbL14 = (gcnew System::Windows::Forms::CheckBox());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->chart2_Rabs = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
            this->_lb_Temperature = (gcnew System::Windows::Forms::Label());
            this->label15 = (gcnew System::Windows::Forms::Label());
            this->label17 = (gcnew System::Windows::Forms::Label());
            this->_bt_AxReset = (gcnew System::Windows::Forms::Button());
            this->toolTip1 = (gcnew System::Windows::Forms::ToolTip(this->components));
            this->_txBM_Moist = (gcnew System::Windows::Forms::TextBox());
            this->_txBM_SoiBulDen = (gcnew System::Windows::Forms::TextBox());
            this->_txBM_SoiParDen = (gcnew System::Windows::Forms::TextBox());
            this->_txMaxTim = (gcnew System::Windows::Forms::TextBox());
            this->_txMess = (gcnew System::Windows::Forms::TextBox());
            this->label23 = (gcnew System::Windows::Forms::Label());
            this->_btBM_GraphClear = (gcnew System::Windows::Forms::Button());
            this->_rb_Temp = (gcnew System::Windows::Forms::RadioButton());
            this->_rb_Moist = (gcnew System::Windows::Forms::RadioButton());
            this->label37 = (gcnew System::Windows::Forms::Label());
            this->label39 = (gcnew System::Windows::Forms::Label());
            this->label40 = (gcnew System::Windows::Forms::Label());
            this->label42 = (gcnew System::Windows::Forms::Label());
            this->_btBM_DrawGraph = (gcnew System::Windows::Forms::Button());
            this->_cb15 = (gcnew System::Windows::Forms::CheckBox());
            this->_cb16 = (gcnew System::Windows::Forms::CheckBox());
            this->_cb17 = (gcnew System::Windows::Forms::CheckBox());
            this->_cb18 = (gcnew System::Windows::Forms::CheckBox());
            this->_cb19 = (gcnew System::Windows::Forms::CheckBox());
            this->_cb20 = (gcnew System::Windows::Forms::CheckBox());
            this->_cb21 = (gcnew System::Windows::Forms::CheckBox());
            this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
            this->_bt_FirIntGraph = (gcnew System::Windows::Forms::Button());
            this->_gbFirInt = (gcnew System::Windows::Forms::GroupBox());
            this->label90 = (gcnew System::Windows::Forms::Label());
            this->label88 = (gcnew System::Windows::Forms::Label());
            this->label6 = (gcnew System::Windows::Forms::Label());
            this->label5 = (gcnew System::Windows::Forms::Label());
            this->_txQabs = (gcnew System::Windows::Forms::TextBox());
            this->_txFirDur = (gcnew System::Windows::Forms::TextBox());
            this->_cbSimTime = (gcnew System::Windows::Forms::ComboBox());
            this->_bt_Quit = (gcnew System::Windows::Forms::Button());
            this->_cb_FireType = (gcnew System::Windows::Forms::ComboBox());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->label4 = (gcnew System::Windows::Forms::Label());
            this->_btHelpRef = (gcnew System::Windows::Forms::Button());
            this->_gbAxisLimit = (gcnew System::Windows::Forms::GroupBox());
            this->_txBM_AmbAirTmp = (gcnew System::Windows::Forms::TextBox());
            this->label9 = (gcnew System::Windows::Forms::Label());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_ud_XMax))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_ud_YMax))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_ud_YMin))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart2_Rabs))->BeginInit();
            this->groupBox1->SuspendLayout();
            this->_gbFirInt->SuspendLayout();
            this->_gbAxisLimit->SuspendLayout();
            this->SuspendLayout();
            // 
            // chart1
            // 
            chartArea7->AxisX->ScaleView->MinSize = 5;
            chartArea7->AxisX->ScaleView->MinSizeType = System::Windows::Forms::DataVisualization::Charting::DateTimeIntervalType::Number;
            chartArea7->AxisX->ScaleView->Position = 0;
            chartArea7->AxisX->ScaleView->SizeType = System::Windows::Forms::DataVisualization::Charting::DateTimeIntervalType::Number;
            chartArea7->AxisX->ScaleView->SmallScrollMinSizeType = System::Windows::Forms::DataVisualization::Charting::DateTimeIntervalType::Number;
            chartArea7->AxisX->ScaleView->SmallScrollSizeType = System::Windows::Forms::DataVisualization::Charting::DateTimeIntervalType::Number;
            chartArea7->AxisX->Title = L"Simulation Time (hrs)";
            chartArea7->AxisX->ToolTip = L"Simulation Time in Hours";
            chartArea7->AxisY->Title = L"Temperature Celsius";
            chartArea7->AxisY->ToolTip = L"Soil Temperature Celsius\\n Soil Moisture Volumetric m3/m3";
            chartArea7->Name = L"ChartArea1";
            chartArea7->Position->Auto = false;
            chartArea7->Position->Height = 90;
            chartArea7->Position->Width = 98;
            chartArea7->Position->X = 2;
            chartArea7->Position->Y = 9;
            this->chart1->ChartAreas->Add(chartArea7);
            legend4->Enabled = false;
            legend4->Name = L"Legend1";
            legend4->Position->Auto = false;
            legend4->Position->Height = 19.58763F;
            legend4->Position->Width = 14.28571F;
            legend4->Position->X = 2.71429F;
            legend4->Position->Y = 3;
            this->chart1->Legends->Add(legend4);
            this->chart1->Location = System::Drawing::Point(80, 6);
            this->chart1->Name = L"chart1";
            series151->BorderWidth = 3;
            series151->ChartArea = L"ChartArea1";
            series151->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series151->Color = System::Drawing::Color::Red;
            series151->Legend = L"Legend1";
            series151->Name = L"A_Lay1";
            series152->BorderWidth = 3;
            series152->ChartArea = L"ChartArea1";
            series152->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series152->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)), 
                static_cast<System::Int32>(static_cast<System::Byte>(128)));
            series152->Legend = L"Legend1";
            series152->Name = L"A_Lay2";
            series153->BorderWidth = 3;
            series153->ChartArea = L"ChartArea1";
            series153->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series153->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)), 
                static_cast<System::Int32>(static_cast<System::Byte>(0)));
            series153->Legend = L"Legend1";
            series153->LegendText = L"2 cm";
            series153->Name = L"A_Lay3";
            series154->BorderWidth = 3;
            series154->ChartArea = L"ChartArea1";
            series154->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series154->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
                static_cast<System::Int32>(static_cast<System::Byte>(128)));
            series154->Legend = L"Legend1";
            series154->LegendText = L"4 cm";
            series154->Name = L"A_Lay4";
            series155->BorderWidth = 3;
            series155->ChartArea = L"ChartArea1";
            series155->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series155->Color = System::Drawing::Color::Lime;
            series155->Legend = L"Legend1";
            series155->LegendText = L"cm 2";
            series155->Name = L"A_Lay5";
            series156->BorderWidth = 3;
            series156->ChartArea = L"ChartArea1";
            series156->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series156->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
                static_cast<System::Int32>(static_cast<System::Byte>(0)));
            series156->Legend = L"Legend1";
            series156->LegendText = L"cm 2";
            series156->Name = L"A_Lay6";
            series157->BorderWidth = 3;
            series157->ChartArea = L"ChartArea1";
            series157->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series157->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
                static_cast<System::Int32>(static_cast<System::Byte>(192)));
            series157->Legend = L"Legend1";
            series157->LegendText = L"cm 2";
            series157->Name = L"A_Lay7";
            series158->BorderWidth = 3;
            series158->ChartArea = L"ChartArea1";
            series158->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series158->Color = System::Drawing::Color::Teal;
            series158->Legend = L"Legend1";
            series158->LegendText = L"cm 2";
            series158->Name = L"A_Lay8";
            series159->BorderWidth = 3;
            series159->ChartArea = L"ChartArea1";
            series159->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series159->Color = System::Drawing::Color::Green;
            series159->Legend = L"Legend1";
            series159->LegendText = L"cm 2";
            series159->Name = L"A_Lay9";
            series160->BorderWidth = 3;
            series160->ChartArea = L"ChartArea1";
            series160->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series160->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
                static_cast<System::Int32>(static_cast<System::Byte>(0)));
            series160->Legend = L"Legend1";
            series160->LegendText = L"cm 2";
            series160->Name = L"A_Lay10";
            series161->BorderWidth = 3;
            series161->ChartArea = L"ChartArea1";
            series161->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series161->Color = System::Drawing::Color::Silver;
            series161->Legend = L"Legend1";
            series161->LegendText = L"cm 2";
            series161->Name = L"A_Lay11";
            series162->BorderWidth = 3;
            series162->ChartArea = L"ChartArea1";
            series162->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series162->Color = System::Drawing::Color::Gray;
            series162->Legend = L"Legend1";
            series162->LegendText = L"cm 2";
            series162->Name = L"A_Lay12";
            series163->BorderWidth = 3;
            series163->ChartArea = L"ChartArea1";
            series163->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series163->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(0)), 
                static_cast<System::Int32>(static_cast<System::Byte>(192)));
            series163->Legend = L"Legend1";
            series163->LegendText = L"cm 2";
            series163->Name = L"A_Lay13";
            series164->BorderWidth = 3;
            series164->ChartArea = L"ChartArea1";
            series164->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series164->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
                static_cast<System::Int32>(static_cast<System::Byte>(64)));
            series164->Legend = L"Legend1";
            series164->LegendText = L"cm 2";
            series164->Name = L"A_Lay14";
            series165->BorderColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
                static_cast<System::Int32>(static_cast<System::Byte>(192)));
            series165->BorderWidth = 2;
            series165->ChartArea = L"ChartArea1";
            series165->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series165->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
                static_cast<System::Int32>(static_cast<System::Byte>(192)));
            series165->Legend = L"Legend1";
            series165->Name = L"B_Lay1";
            series166->BorderWidth = 2;
            series166->ChartArea = L"ChartArea1";
            series166->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series166->Legend = L"Legend1";
            series166->Name = L"B_Lay2";
            series167->BorderWidth = 2;
            series167->ChartArea = L"ChartArea1";
            series167->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series167->Legend = L"Legend1";
            series167->Name = L"B_Lay3";
            series168->BorderWidth = 2;
            series168->ChartArea = L"ChartArea1";
            series168->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series168->Legend = L"Legend1";
            series168->Name = L"B_Lay4";
            series169->BorderWidth = 2;
            series169->ChartArea = L"ChartArea1";
            series169->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series169->Legend = L"Legend1";
            series169->Name = L"B_Lay5";
            series170->BorderWidth = 2;
            series170->ChartArea = L"ChartArea1";
            series170->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series170->Legend = L"Legend1";
            series170->Name = L"B_Lay6";
            series171->BorderWidth = 2;
            series171->ChartArea = L"ChartArea1";
            series171->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series171->Legend = L"Legend1";
            series171->Name = L"B_Lay7";
            series172->BorderWidth = 2;
            series172->ChartArea = L"ChartArea1";
            series172->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series172->Legend = L"Legend1";
            series172->Name = L"B_Lay8";
            series173->BorderWidth = 2;
            series173->ChartArea = L"ChartArea1";
            series173->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series173->Legend = L"Legend1";
            series173->Name = L"B_Lay9";
            series174->BorderWidth = 2;
            series174->ChartArea = L"ChartArea1";
            series174->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series174->Legend = L"Legend1";
            series174->Name = L"B_Lay10";
            series175->BorderWidth = 2;
            series175->ChartArea = L"ChartArea1";
            series175->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series175->Legend = L"Legend1";
            series175->Name = L"B_Lay11";
            series176->BorderWidth = 2;
            series176->ChartArea = L"ChartArea1";
            series176->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series176->Legend = L"Legend1";
            series176->Name = L"B_Lay12";
            series177->BorderWidth = 2;
            series177->ChartArea = L"ChartArea1";
            series177->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series177->Legend = L"Legend1";
            series177->Name = L"B_Lay13";
            series178->BorderWidth = 2;
            series178->ChartArea = L"ChartArea1";
            series178->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series178->Legend = L"Legend1";
            series178->Name = L"B_Lay14";
            series179->BorderWidth = 2;
            series179->ChartArea = L"ChartArea1";
            series179->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series179->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
                static_cast<System::Int32>(static_cast<System::Byte>(255)));
            series179->Legend = L"Legend1";
            series179->Name = L"B_Lay15";
            series180->BorderWidth = 2;
            series180->ChartArea = L"ChartArea1";
            series180->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series180->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)), 
                static_cast<System::Int32>(static_cast<System::Byte>(255)));
            series180->Legend = L"Legend1";
            series180->Name = L"B_Lay16";
            series181->BorderWidth = 2;
            series181->ChartArea = L"ChartArea1";
            series181->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series181->Color = System::Drawing::Color::Fuchsia;
            series181->Legend = L"Legend1";
            series181->Name = L"B_Lay17";
            series182->BorderWidth = 2;
            series182->ChartArea = L"ChartArea1";
            series182->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series182->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(0)), 
                static_cast<System::Int32>(static_cast<System::Byte>(192)));
            series182->Legend = L"Legend1";
            series182->Name = L"B_Lay18";
            series183->BorderWidth = 2;
            series183->ChartArea = L"ChartArea1";
            series183->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series183->Color = System::Drawing::Color::Purple;
            series183->Legend = L"Legend1";
            series183->Name = L"B_Lay19";
            series184->BorderWidth = 2;
            series184->ChartArea = L"ChartArea1";
            series184->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series184->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(0)), 
                static_cast<System::Int32>(static_cast<System::Byte>(64)));
            series184->Legend = L"Legend1";
            series184->Name = L"B_Lay20";
            series185->BorderWidth = 2;
            series185->ChartArea = L"ChartArea1";
            series185->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series185->Color = System::Drawing::Color::Black;
            series185->Legend = L"Legend1";
            series185->Name = L"B_Lay21";
            series186->ChartArea = L"ChartArea1";
            series186->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series186->Legend = L"Legend1";
            series186->Name = L"F_Lay1";
            series187->ChartArea = L"ChartArea1";
            series187->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series187->Legend = L"Legend1";
            series187->Name = L"F_Lay2";
            series188->ChartArea = L"ChartArea1";
            series188->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series188->Legend = L"Legend1";
            series188->Name = L"F_Lay3";
            series189->ChartArea = L"ChartArea1";
            series189->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series189->Legend = L"Legend1";
            series189->Name = L"F_Lay4";
            series190->ChartArea = L"ChartArea1";
            series190->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series190->Legend = L"Legend1";
            series190->Name = L"F_Lay5";
            series191->ChartArea = L"ChartArea1";
            series191->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series191->Legend = L"Legend1";
            series191->Name = L"F_Lay6";
            series192->ChartArea = L"ChartArea1";
            series192->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series192->Legend = L"Legend1";
            series192->Name = L"F_Lay7";
            series193->ChartArea = L"ChartArea1";
            series193->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series193->Legend = L"Legend1";
            series193->Name = L"F_Lay8";
            series194->ChartArea = L"ChartArea1";
            series194->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series194->Legend = L"Legend1";
            series194->Name = L"F_Lay9";
            series195->ChartArea = L"ChartArea1";
            series195->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series195->Legend = L"Legend1";
            series195->Name = L"F_Lay10";
            series196->ChartArea = L"ChartArea1";
            series196->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series196->Legend = L"Legend1";
            series196->Name = L"F_Lay11";
            series197->ChartArea = L"ChartArea1";
            series197->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series197->Legend = L"Legend1";
            series197->Name = L"F_Lay12";
            series198->ChartArea = L"ChartArea1";
            series198->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series198->Legend = L"Legend1";
            series198->Name = L"F_Lay13";
            series199->ChartArea = L"ChartArea1";
            series199->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series199->Legend = L"Legend1";
            series199->Name = L"F_Lay14";
            this->chart1->Series->Add(series151);
            this->chart1->Series->Add(series152);
            this->chart1->Series->Add(series153);
            this->chart1->Series->Add(series154);
            this->chart1->Series->Add(series155);
            this->chart1->Series->Add(series156);
            this->chart1->Series->Add(series157);
            this->chart1->Series->Add(series158);
            this->chart1->Series->Add(series159);
            this->chart1->Series->Add(series160);
            this->chart1->Series->Add(series161);
            this->chart1->Series->Add(series162);
            this->chart1->Series->Add(series163);
            this->chart1->Series->Add(series164);
            this->chart1->Series->Add(series165);
            this->chart1->Series->Add(series166);
            this->chart1->Series->Add(series167);
            this->chart1->Series->Add(series168);
            this->chart1->Series->Add(series169);
            this->chart1->Series->Add(series170);
            this->chart1->Series->Add(series171);
            this->chart1->Series->Add(series172);
            this->chart1->Series->Add(series173);
            this->chart1->Series->Add(series174);
            this->chart1->Series->Add(series175);
            this->chart1->Series->Add(series176);
            this->chart1->Series->Add(series177);
            this->chart1->Series->Add(series178);
            this->chart1->Series->Add(series179);
            this->chart1->Series->Add(series180);
            this->chart1->Series->Add(series181);
            this->chart1->Series->Add(series182);
            this->chart1->Series->Add(series183);
            this->chart1->Series->Add(series184);
            this->chart1->Series->Add(series185);
            this->chart1->Series->Add(series186);
            this->chart1->Series->Add(series187);
            this->chart1->Series->Add(series188);
            this->chart1->Series->Add(series189);
            this->chart1->Series->Add(series190);
            this->chart1->Series->Add(series191);
            this->chart1->Series->Add(series192);
            this->chart1->Series->Add(series193);
            this->chart1->Series->Add(series194);
            this->chart1->Series->Add(series195);
            this->chart1->Series->Add(series196);
            this->chart1->Series->Add(series197);
            this->chart1->Series->Add(series198);
            this->chart1->Series->Add(series199);
            this->chart1->Size = System::Drawing::Size(745, 222);
            this->chart1->TabIndex = 0;
            this->chart1->Text = L"chart1";
            title7->Alignment = System::Drawing::ContentAlignment::TopCenter;
            title7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            title7->Name = L"Title1";
            title7->Text = L"Soil Temperature";
            this->chart1->Titles->Add(title7);
            // 
            // _cbL3
            // 
            this->_cbL3->AutoSize = true;
            this->_cbL3->Location = System::Drawing::Point(11, 64);
            this->_cbL3->Name = L"_cbL3";
            this->_cbL3->Size = System::Drawing::Size(49, 17);
            this->_cbL3->TabIndex = 25;
            this->_cbL3->Text = L"2 cm";
            this->_cbL3->UseVisualStyleBackColor = true;
            this->_cbL3->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox4_CheckedChanged);
            // 
            // _cbL2
            // 
            this->_cbL2->AutoSize = true;
            this->_cbL2->Location = System::Drawing::Point(11, 44);
            this->_cbL2->Name = L"_cbL2";
            this->_cbL2->Size = System::Drawing::Size(49, 17);
            this->_cbL2->TabIndex = 24;
            this->_cbL2->Text = L"1 cm";
            this->_cbL2->UseVisualStyleBackColor = true;
            this->_cbL2->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox4_CheckedChanged);
            // 
            // _cbL1
            // 
            this->_cbL1->AutoSize = true;
            this->_cbL1->Checked = true;
            this->_cbL1->CheckState = System::Windows::Forms::CheckState::Checked;
            this->_cbL1->Location = System::Drawing::Point(11, 23);
            this->_cbL1->Name = L"_cbL1";
            this->_cbL1->Size = System::Drawing::Size(63, 17);
            this->_cbL1->TabIndex = 23;
            this->_cbL1->Text = L"Surface";
            this->_cbL1->UseVisualStyleBackColor = true;
            // 
            // _ud_XMax
            // 
            this->_ud_XMax->BackColor = System::Drawing::Color::White;
            this->_ud_XMax->Location = System::Drawing::Point(20, 120);
            this->_ud_XMax->Margin = System::Windows::Forms::Padding(2);
            this->_ud_XMax->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {122, 0, 0, 0});
            this->_ud_XMax->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
            this->_ud_XMax->Name = L"_ud_XMax";
            this->_ud_XMax->Size = System::Drawing::Size(64, 22);
            this->_ud_XMax->TabIndex = 150;
            this->_ud_XMax->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->_ud_XMax->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {12, 0, 0, 0});
            // 
            // _ud_YMax
            // 
            this->_ud_YMax->BackColor = System::Drawing::Color::White;
            this->_ud_YMax->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
            this->_ud_YMax->Location = System::Drawing::Point(20, 34);
            this->_ud_YMax->Margin = System::Windows::Forms::Padding(2);
            this->_ud_YMax->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {2000, 0, 0, 0});
            this->_ud_YMax->Name = L"_ud_YMax";
            this->_ud_YMax->ReadOnly = true;
            this->_ud_YMax->Size = System::Drawing::Size(64, 22);
            this->_ud_YMax->TabIndex = 152;
            this->_ud_YMax->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_ud_YMax, L"Set the Soil Graph X axis \r\nupper temperature");
            this->_ud_YMax->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {500, 0, 0, 0});
            // 
            // _ud_YMin
            // 
            this->_ud_YMin->BackColor = System::Drawing::Color::White;
            this->_ud_YMin->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
            this->_ud_YMin->Location = System::Drawing::Point(20, 77);
            this->_ud_YMin->Margin = System::Windows::Forms::Padding(2);
            this->_ud_YMin->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
            this->_ud_YMin->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, System::Int32::MinValue});
            this->_ud_YMin->Name = L"_ud_YMin";
            this->_ud_YMin->ReadOnly = true;
            this->_ud_YMin->Size = System::Drawing::Size(64, 22);
            this->_ud_YMin->TabIndex = 153;
            this->_ud_YMin->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _cbL4
            // 
            this->_cbL4->AutoSize = true;
            this->_cbL4->Location = System::Drawing::Point(11, 83);
            this->_cbL4->Name = L"_cbL4";
            this->_cbL4->Size = System::Drawing::Size(55, 17);
            this->_cbL4->TabIndex = 154;
            this->_cbL4->Text = L" 3 cm ";
            this->_cbL4->UseVisualStyleBackColor = true;
            this->_cbL4->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox4_CheckedChanged);
            // 
            // _cbL5
            // 
            this->_cbL5->AutoSize = true;
            this->_cbL5->Location = System::Drawing::Point(11, 103);
            this->_cbL5->Name = L"_cbL5";
            this->_cbL5->Size = System::Drawing::Size(55, 17);
            this->_cbL5->TabIndex = 155;
            this->_cbL5->Text = L" 4 cm ";
            this->_cbL5->UseVisualStyleBackColor = true;
            this->_cbL5->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox4_CheckedChanged);
            // 
            // _cbL6
            // 
            this->_cbL6->AutoSize = true;
            this->_cbL6->Location = System::Drawing::Point(11, 122);
            this->_cbL6->Name = L"_cbL6";
            this->_cbL6->Size = System::Drawing::Size(55, 17);
            this->_cbL6->TabIndex = 156;
            this->_cbL6->Text = L" 5 cm ";
            this->_cbL6->UseVisualStyleBackColor = true;
            this->_cbL6->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox4_CheckedChanged);
            // 
            // _cbL7
            // 
            this->_cbL7->AutoSize = true;
            this->_cbL7->Location = System::Drawing::Point(11, 140);
            this->_cbL7->Name = L"_cbL7";
            this->_cbL7->Size = System::Drawing::Size(55, 17);
            this->_cbL7->TabIndex = 157;
            this->_cbL7->Text = L" 6 cm ";
            this->_cbL7->UseVisualStyleBackColor = true;
            this->_cbL7->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox4_CheckedChanged);
            // 
            // _cbL8
            // 
            this->_cbL8->AutoSize = true;
            this->_cbL8->Location = System::Drawing::Point(11, 159);
            this->_cbL8->Name = L"_cbL8";
            this->_cbL8->Size = System::Drawing::Size(55, 17);
            this->_cbL8->TabIndex = 158;
            this->_cbL8->Text = L" 7 cm ";
            this->_cbL8->UseVisualStyleBackColor = true;
            this->_cbL8->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox4_CheckedChanged);
            // 
            // _cbL9
            // 
            this->_cbL9->AutoSize = true;
            this->_cbL9->Location = System::Drawing::Point(11, 178);
            this->_cbL9->Name = L"_cbL9";
            this->_cbL9->Size = System::Drawing::Size(55, 17);
            this->_cbL9->TabIndex = 159;
            this->_cbL9->Text = L" 8 cm ";
            this->_cbL9->UseVisualStyleBackColor = true;
            this->_cbL9->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox4_CheckedChanged);
            // 
            // _cbL10
            // 
            this->_cbL10->AutoSize = true;
            this->_cbL10->Location = System::Drawing::Point(11, 198);
            this->_cbL10->Name = L"_cbL10";
            this->_cbL10->Size = System::Drawing::Size(55, 17);
            this->_cbL10->TabIndex = 160;
            this->_cbL10->Text = L" 9 cm ";
            this->_cbL10->UseVisualStyleBackColor = true;
            this->_cbL10->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox4_CheckedChanged);
            // 
            // _cbL11
            // 
            this->_cbL11->AutoSize = true;
            this->_cbL11->Location = System::Drawing::Point(11, 217);
            this->_cbL11->Name = L"_cbL11";
            this->_cbL11->Size = System::Drawing::Size(61, 17);
            this->_cbL11->TabIndex = 161;
            this->_cbL11->Text = L" 10 cm ";
            this->_cbL11->UseVisualStyleBackColor = true;
            this->_cbL11->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox4_CheckedChanged);
            // 
            // _cbL12
            // 
            this->_cbL12->AutoSize = true;
            this->_cbL12->Location = System::Drawing::Point(206, 477);
            this->_cbL12->Name = L"_cbL12";
            this->_cbL12->Size = System::Drawing::Size(61, 17);
            this->_cbL12->TabIndex = 162;
            this->_cbL12->Text = L" 11 cm ";
            this->_cbL12->UseVisualStyleBackColor = true;
            // 
            // _cbL13
            // 
            this->_cbL13->AutoSize = true;
            this->_cbL13->Location = System::Drawing::Point(11, 236);
            this->_cbL13->Name = L"_cbL13";
            this->_cbL13->Size = System::Drawing::Size(61, 17);
            this->_cbL13->TabIndex = 163;
            this->_cbL13->Text = L" 12 cm ";
            this->_cbL13->UseVisualStyleBackColor = true;
            this->_cbL13->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox4_CheckedChanged);
            // 
            // _cbL14
            // 
            this->_cbL14->AutoSize = true;
            this->_cbL14->Location = System::Drawing::Point(273, 478);
            this->_cbL14->Name = L"_cbL14";
            this->_cbL14->Size = System::Drawing::Size(61, 17);
            this->_cbL14->TabIndex = 164;
            this->_cbL14->Text = L" 13 cm ";
            this->_cbL14->UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(8, 7);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(56, 13);
            this->label1->TabIndex = 166;
            this->label1->Text = L"Soil Depth";
            // 
            // chart2_Rabs
            // 
            chartArea8->AxisX->Title = L"Simulation Time (hrs)";
            chartArea8->AxisY->Title = L"Fire Intensity \\n  kW/m2";
            chartArea8->Name = L"ChartArea1";
            chartArea8->Position->Auto = false;
            chartArea8->Position->Height = 94;
            chartArea8->Position->Width = 99;
            chartArea8->Position->X = 1;
            chartArea8->Position->Y = 6;
            this->chart2_Rabs->ChartAreas->Add(chartArea8);
            this->chart2_Rabs->Location = System::Drawing::Point(79, 235);
            this->chart2_Rabs->Name = L"chart2_Rabs";
            series200->BorderWidth = 4;
            series200->ChartArea = L"ChartArea1";
            series200->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
            series200->Name = L"Rabs";
            this->chart2_Rabs->Series->Add(series200);
            this->chart2_Rabs->Size = System::Drawing::Size(747, 149);
            this->chart2_Rabs->TabIndex = 178;
            this->chart2_Rabs->Text = L"chart2";
            title8->Alignment = System::Drawing::ContentAlignment::TopCenter;
            title8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            title8->Name = L"Title1";
            title8->Text = L"Fire Intensity at Soil Surface";
            this->chart2_Rabs->Titles->Add(title8);
            // 
            // _lb_Temperature
            // 
            this->_lb_Temperature->AutoSize = true;
            this->_lb_Temperature->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lb_Temperature->Location = System::Drawing::Point(10, 18);
            this->_lb_Temperature->Name = L"_lb_Temperature";
            this->_lb_Temperature->Size = System::Drawing::Size(88, 13);
            this->_lb_Temperature->TabIndex = 181;
            this->_lb_Temperature->Text = L"Upper Temp ( C )";
            this->toolTip1->SetToolTip(this->_lb_Temperature, L"Set the Soil Graph X axis \r\nupper temperature");
            // 
            // label15
            // 
            this->label15->AutoSize = true;
            this->label15->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label15->Location = System::Drawing::Point(10, 61);
            this->label15->Name = L"label15";
            this->label15->Size = System::Drawing::Size(88, 13);
            this->label15->TabIndex = 182;
            this->label15->Text = L"Lower Temp ( C )";
            // 
            // label17
            // 
            this->label17->AutoSize = true;
            this->label17->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label17->Location = System::Drawing::Point(16, 104);
            this->label17->Name = L"label17";
            this->label17->Size = System::Drawing::Size(75, 13);
            this->label17->TabIndex = 184;
            this->label17->Text = L"End Time (hrs)";
            // 
            // _bt_AxReset
            // 
            this->_bt_AxReset->ForeColor = System::Drawing::SystemColors::ControlText;
            this->_bt_AxReset->Location = System::Drawing::Point(80, 390);
            this->_bt_AxReset->Name = L"_bt_AxReset";
            this->_bt_AxReset->Size = System::Drawing::Size(50, 25);
            this->_bt_AxReset->TabIndex = 186;
            this->_bt_AxReset->Text = L"Start";
            this->_bt_AxReset->UseVisualStyleBackColor = true;
            this->_bt_AxReset->Click += gcnew System::EventHandler(this, &Form1::_bt_AxReset_Click);
            // 
            // _txBM_Moist
            // 
            this->_txBM_Moist->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_txBM_Moist->Location = System::Drawing::Point(367, 432);
            this->_txBM_Moist->Name = L"_txBM_Moist";
            this->_txBM_Moist->RightToLeft = System::Windows::Forms::RightToLeft::No;
            this->_txBM_Moist->Size = System::Drawing::Size(50, 21);
            this->_txBM_Moist->TabIndex = 228;
            this->_txBM_Moist->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txBM_Moist, L"Soil Moisture by Volume\r\n Limits:  0.001 --> 0.25\r\n");
            this->_txBM_Moist->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::_txBM_Moist_KeyDown);
            // 
            // _txBM_SoiBulDen
            // 
            this->_txBM_SoiBulDen->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_txBM_SoiBulDen->Location = System::Drawing::Point(566, 432);
            this->_txBM_SoiBulDen->Name = L"_txBM_SoiBulDen";
            this->_txBM_SoiBulDen->RightToLeft = System::Windows::Forms::RightToLeft::No;
            this->_txBM_SoiBulDen->Size = System::Drawing::Size(50, 21);
            this->_txBM_SoiBulDen->TabIndex = 232;
            this->_txBM_SoiBulDen->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txBM_SoiBulDen, L"Soil Bulk Density - Mg/cm3\r\n Limits:  0.01 --> 0.25");
            // 
            // _txBM_SoiParDen
            // 
            this->_txBM_SoiParDen->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_txBM_SoiParDen->Location = System::Drawing::Point(774, 432);
            this->_txBM_SoiParDen->Name = L"_txBM_SoiParDen";
            this->_txBM_SoiParDen->RightToLeft = System::Windows::Forms::RightToLeft::No;
            this->_txBM_SoiParDen->Size = System::Drawing::Size(50, 21);
            this->_txBM_SoiParDen->TabIndex = 233;
            this->_txBM_SoiParDen->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txBM_SoiParDen, L"Soil Particle Density  Mg/cm3\r\n Limits:   2.3 --> 2.9");
            // 
            // _txMaxTim
            // 
            this->_txMaxTim->Location = System::Drawing::Point(19, 105);
            this->_txMaxTim->Name = L"_txMaxTim";
            this->_txMaxTim->Size = System::Drawing::Size(61, 20);
            this->_txMaxTim->TabIndex = 260;
            this->_txMaxTim->Text = L"0.6";
            this->_txMaxTim->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txMaxTim, L"Time (hr) of maximum \r\nwatts are applied ");
            // 
            // _txMess
            // 
            this->_txMess->BackColor = System::Drawing::SystemColors::InactiveBorder;
            this->_txMess->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->_txMess->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_txMess->Location = System::Drawing::Point(6, 414);
            this->_txMess->Name = L"_txMess";
            this->_txMess->ReadOnly = true;
            this->_txMess->Size = System::Drawing::Size(54, 21);
            this->_txMess->TabIndex = 194;
            this->_txMess->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // label23
            // 
            this->label23->AutoSize = true;
            this->label23->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label23->Location = System::Drawing::Point(637, 398);
            this->label23->Name = L"label23";
            this->label23->Size = System::Drawing::Size(133, 16);
            this->label23->TabIndex = 197;
            this->label23->Text = L"Simulation Time (hrs)";
            // 
            // _btBM_GraphClear
            // 
            this->_btBM_GraphClear->ForeColor = System::Drawing::SystemColors::ControlDarkDark;
            this->_btBM_GraphClear->Location = System::Drawing::Point(261, 392);
            this->_btBM_GraphClear->Name = L"_btBM_GraphClear";
            this->_btBM_GraphClear->Size = System::Drawing::Size(50, 25);
            this->_btBM_GraphClear->TabIndex = 216;
            this->_btBM_GraphClear->Text = L"Clear";
            this->_btBM_GraphClear->UseVisualStyleBackColor = true;
            this->_btBM_GraphClear->Click += gcnew System::EventHandler(this, &Form1::_btBM_GraphClear_Click);
            // 
            // _rb_Temp
            // 
            this->_rb_Temp->AutoSize = true;
            this->_rb_Temp->Checked = true;
            this->_rb_Temp->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_rb_Temp->Location = System::Drawing::Point(12, 23);
            this->_rb_Temp->Name = L"_rb_Temp";
            this->_rb_Temp->Size = System::Drawing::Size(85, 17);
            this->_rb_Temp->TabIndex = 225;
            this->_rb_Temp->TabStop = true;
            this->_rb_Temp->Text = L"Temperature";
            this->_rb_Temp->UseVisualStyleBackColor = true;
            this->_rb_Temp->CheckedChanged += gcnew System::EventHandler(this, &Form1::_rb_Temp_CheckedChanged);
            // 
            // _rb_Moist
            // 
            this->_rb_Moist->AutoSize = true;
            this->_rb_Moist->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_rb_Moist->Location = System::Drawing::Point(13, 43);
            this->_rb_Moist->Name = L"_rb_Moist";
            this->_rb_Moist->Size = System::Drawing::Size(65, 17);
            this->_rb_Moist->TabIndex = 226;
            this->_rb_Moist->TabStop = true;
            this->_rb_Moist->Text = L"Moisture";
            this->_rb_Moist->UseVisualStyleBackColor = true;
            this->_rb_Moist->CheckedChanged += gcnew System::EventHandler(this, &Form1::_rb_Moist_CheckedChanged);
            // 
            // label37
            // 
            this->label37->AutoSize = true;
            this->label37->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label37->ForeColor = System::Drawing::Color::Black;
            this->label37->Location = System::Drawing::Point(223, 430);
            this->label37->Name = L"label37";
            this->label37->Size = System::Drawing::Size(140, 30);
            this->label37->TabIndex = 229;
            this->label37->Text = L"Volumetric Soil Moisture\r\n    0.10    (m3/m3)";
            // 
            // label39
            // 
            this->label39->AutoSize = true;
            this->label39->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label39->ForeColor = System::Drawing::SystemColors::ControlText;
            this->label39->Location = System::Drawing::Point(432, 430);
            this->label39->Name = L"label39";
            this->label39->Size = System::Drawing::Size(132, 30);
            this->label39->TabIndex = 234;
            this->label39->Text = L"Soil Bulk Density 1.250\r\n      ( Mg/m3)";
            // 
            // label40
            // 
            this->label40->AutoSize = true;
            this->label40->ForeColor = System::Drawing::SystemColors::Desktop;
            this->label40->Location = System::Drawing::Point(254, 447);
            this->label40->Name = L"label40";
            this->label40->Size = System::Drawing::Size(0, 13);
            this->label40->TabIndex = 235;
            // 
            // label42
            // 
            this->label42->AutoSize = true;
            this->label42->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label42->ForeColor = System::Drawing::SystemColors::ControlText;
            this->label42->Location = System::Drawing::Point(623, 430);
            this->label42->Name = L"label42";
            this->label42->Size = System::Drawing::Size(149, 30);
            this->label42->TabIndex = 236;
            this->label42->Text = L"Soil Particle Density 2.650\r\n             ( Mg/m3)";
            // 
            // _btBM_DrawGraph
            // 
            this->_btBM_DrawGraph->ForeColor = System::Drawing::SystemColors::ControlDarkDark;
            this->_btBM_DrawGraph->Location = System::Drawing::Point(198, 391);
            this->_btBM_DrawGraph->Name = L"_btBM_DrawGraph";
            this->_btBM_DrawGraph->Size = System::Drawing::Size(54, 25);
            this->_btBM_DrawGraph->TabIndex = 241;
            this->_btBM_DrawGraph->Text = L"Redraw";
            this->_btBM_DrawGraph->UseVisualStyleBackColor = true;
            this->_btBM_DrawGraph->Click += gcnew System::EventHandler(this, &Form1::_btBM_DrawGraph_Click);
            // 
            // _cb15
            // 
            this->_cb15->AutoSize = true;
            this->_cb15->Location = System::Drawing::Point(11, 256);
            this->_cb15->Name = L"_cb15";
            this->_cb15->Size = System::Drawing::Size(61, 17);
            this->_cb15->TabIndex = 243;
            this->_cb15->Text = L" 14 cm ";
            this->_cb15->UseVisualStyleBackColor = true;
            this->_cb15->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox4_CheckedChanged);
            // 
            // _cb16
            // 
            this->_cb16->AutoSize = true;
            this->_cb16->Location = System::Drawing::Point(340, 477);
            this->_cb16->Name = L"_cb16";
            this->_cb16->Size = System::Drawing::Size(61, 17);
            this->_cb16->TabIndex = 244;
            this->_cb16->Text = L" 15 cm ";
            this->_cb16->UseVisualStyleBackColor = true;
            // 
            // _cb17
            // 
            this->_cb17->AutoSize = true;
            this->_cb17->Location = System::Drawing::Point(11, 277);
            this->_cb17->Name = L"_cb17";
            this->_cb17->Size = System::Drawing::Size(61, 17);
            this->_cb17->TabIndex = 245;
            this->_cb17->Text = L" 16 cm ";
            this->_cb17->UseVisualStyleBackColor = true;
            this->_cb17->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox4_CheckedChanged);
            // 
            // _cb18
            // 
            this->_cb18->AutoSize = true;
            this->_cb18->Location = System::Drawing::Point(407, 477);
            this->_cb18->Name = L"_cb18";
            this->_cb18->Size = System::Drawing::Size(61, 17);
            this->_cb18->TabIndex = 246;
            this->_cb18->Text = L" 17 cm ";
            this->_cb18->UseVisualStyleBackColor = true;
            // 
            // _cb19
            // 
            this->_cb19->AutoSize = true;
            this->_cb19->Location = System::Drawing::Point(11, 296);
            this->_cb19->Name = L"_cb19";
            this->_cb19->Size = System::Drawing::Size(61, 17);
            this->_cb19->TabIndex = 247;
            this->_cb19->Text = L" 18 cm ";
            this->_cb19->UseVisualStyleBackColor = true;
            this->_cb19->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox4_CheckedChanged);
            // 
            // _cb20
            // 
            this->_cb20->AutoSize = true;
            this->_cb20->Location = System::Drawing::Point(487, 476);
            this->_cb20->Name = L"_cb20";
            this->_cb20->Size = System::Drawing::Size(61, 17);
            this->_cb20->TabIndex = 248;
            this->_cb20->Text = L" 19 cm ";
            this->_cb20->UseVisualStyleBackColor = true;
            this->_cb20->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox4_CheckedChanged);
            // 
            // _cb21
            // 
            this->_cb21->AutoSize = true;
            this->_cb21->Location = System::Drawing::Point(12, 316);
            this->_cb21->Name = L"_cb21";
            this->_cb21->Size = System::Drawing::Size(61, 17);
            this->_cb21->TabIndex = 249;
            this->_cb21->Text = L" 20 cm ";
            this->_cb21->UseVisualStyleBackColor = true;
            this->_cb21->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox4_CheckedChanged);
            // 
            // groupBox1
            // 
            this->groupBox1->Controls->Add(this->_rb_Moist);
            this->groupBox1->Controls->Add(this->_rb_Temp);
            this->groupBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->groupBox1->Location = System::Drawing::Point(839, 8);
            this->groupBox1->Name = L"groupBox1";
            this->groupBox1->Size = System::Drawing::Size(113, 68);
            this->groupBox1->TabIndex = 250;
            this->groupBox1->TabStop = false;
            this->groupBox1->Text = L"Graph Type";
            // 
            // _bt_FirIntGraph
            // 
            this->_bt_FirIntGraph->Location = System::Drawing::Point(18, 181);
            this->_bt_FirIntGraph->Name = L"_bt_FirIntGraph";
            this->_bt_FirIntGraph->Size = System::Drawing::Size(70, 26);
            this->_bt_FirIntGraph->TabIndex = 252;
            this->_bt_FirIntGraph->Text = L"Graph Watts";
            this->_bt_FirIntGraph->UseVisualStyleBackColor = true;
            this->_bt_FirIntGraph->Click += gcnew System::EventHandler(this, &Form1::_bt_FirIntGraph_Click);
            // 
            // _gbFirInt
            // 
            this->_gbFirInt->Controls->Add(this->label90);
            this->_gbFirInt->Controls->Add(this->label88);
            this->_gbFirInt->Controls->Add(this->_txMaxTim);
            this->_gbFirInt->Controls->Add(this->label6);
            this->_gbFirInt->Controls->Add(this->label5);
            this->_gbFirInt->Controls->Add(this->_txQabs);
            this->_gbFirInt->Controls->Add(this->_txFirDur);
            this->_gbFirInt->Controls->Add(this->_bt_FirIntGraph);
            this->_gbFirInt->Location = System::Drawing::Point(839, 239);
            this->_gbFirInt->Name = L"_gbFirInt";
            this->_gbFirInt->Size = System::Drawing::Size(107, 217);
            this->_gbFirInt->TabIndex = 255;
            this->_gbFirInt->TabStop = false;
            // 
            // label90
            // 
            this->label90->AutoSize = true;
            this->label90->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label90->Location = System::Drawing::Point(5, 10);
            this->label90->Name = L"label90";
            this->label90->Size = System::Drawing::Size(96, 32);
            this->label90->TabIndex = 262;
            this->label90->Text = L"Fire Intensity at\r\n  Soil Surface ";
            // 
            // label88
            // 
            this->label88->AutoSize = true;
            this->label88->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label88->Location = System::Drawing::Point(9, 89);
            this->label88->Name = L"label88";
            this->label88->Size = System::Drawing::Size(88, 13);
            this->label88->TabIndex = 261;
            this->label88->Text = L"Time to Max (hrs)";
            // 
            // label6
            // 
            this->label6->AutoSize = true;
            this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label6->Location = System::Drawing::Point(14, 48);
            this->label6->Name = L"label6";
            this->label6->Size = System::Drawing::Size(75, 13);
            this->label6->TabIndex = 258;
            this->label6->Text = L"Max  (kW/m2)";
            // 
            // label5
            // 
            this->label5->AutoSize = true;
            this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label5->Location = System::Drawing::Point(7, 135);
            this->label5->Name = L"label5";
            this->label5->Size = System::Drawing::Size(90, 13);
            this->label5->TabIndex = 257;
            this->label5->Text = L"Fire Duration (hrs)";
            // 
            // _txQabs
            // 
            this->_txQabs->Location = System::Drawing::Point(23, 63);
            this->_txQabs->Name = L"_txQabs";
            this->_txQabs->Size = System::Drawing::Size(53, 20);
            this->_txQabs->TabIndex = 256;
            this->_txQabs->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txFirDur
            // 
            this->_txFirDur->Location = System::Drawing::Point(23, 151);
            this->_txFirDur->MaximumSize = System::Drawing::Size(53, 20);
            this->_txFirDur->Name = L"_txFirDur";
            this->_txFirDur->Size = System::Drawing::Size(53, 20);
            this->_txFirDur->TabIndex = 255;
            this->_txFirDur->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _cbSimTime
            // 
            this->_cbSimTime->FormattingEnabled = true;
            this->_cbSimTime->Items->AddRange(gcnew cli::array< System::Object^  >(7) {L"1.0", L"2.0", L"4.0", L"6.0", L"12", L"24", 
                L"100"});
            this->_cbSimTime->Location = System::Drawing::Point(774, 396);
            this->_cbSimTime->Name = L"_cbSimTime";
            this->_cbSimTime->RightToLeft = System::Windows::Forms::RightToLeft::Yes;
            this->_cbSimTime->Size = System::Drawing::Size(50, 21);
            this->_cbSimTime->TabIndex = 256;
            // 
            // _bt_Quit
            // 
            this->_bt_Quit->Location = System::Drawing::Point(142, 390);
            this->_bt_Quit->Name = L"_bt_Quit";
            this->_bt_Quit->Size = System::Drawing::Size(50, 25);
            this->_bt_Quit->TabIndex = 259;
            this->_bt_Quit->Text = L"Stop";
            this->_bt_Quit->UseVisualStyleBackColor = true;
            this->_bt_Quit->Click += gcnew System::EventHandler(this, &Form1::_bt_Quit_Click);
            // 
            // _cb_FireType
            // 
            this->_cb_FireType->FormattingEnabled = true;
            this->_cb_FireType->Location = System::Drawing::Point(512, 395);
            this->_cb_FireType->Name = L"_cb_FireType";
            this->_cb_FireType->Size = System::Drawing::Size(100, 21);
            this->_cb_FireType->TabIndex = 260;
            this->_cb_FireType->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::_cb_FireType_SelectedIndexChanged);
            this->_cb_FireType->Click += gcnew System::EventHandler(this, &Form1::_cb_FireType_Click);
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label2->Location = System::Drawing::Point(437, 398);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(66, 16);
            this->label2->TabIndex = 261;
            this->label2->Text = L"Fire Type";
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label3->Location = System::Drawing::Point(2, 374);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(66, 16);
            this->label3->TabIndex = 262;
            this->label3->Text = L"Run Time";
            // 
            // label4
            // 
            this->label4->AutoSize = true;
            this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label4->Location = System::Drawing::Point(10, 394);
            this->label4->Name = L"label4";
            this->label4->Size = System::Drawing::Size(49, 16);
            this->label4->TabIndex = 263;
            this->label4->Text = L"Hr::Min";
            // 
            // _btHelpRef
            // 
            this->_btHelpRef->Location = System::Drawing::Point(7, 439);
            this->_btHelpRef->Name = L"_btHelpRef";
            this->_btHelpRef->Size = System::Drawing::Size(50, 36);
            this->_btHelpRef->TabIndex = 264;
            this->_btHelpRef->Text = L"Helpful Tips";
            this->_btHelpRef->UseVisualStyleBackColor = true;
            this->_btHelpRef->Click += gcnew System::EventHandler(this, &Form1::_btHelpRef_Click);
            // 
            // _gbAxisLimit
            // 
            this->_gbAxisLimit->Controls->Add(this->label17);
            this->_gbAxisLimit->Controls->Add(this->label15);
            this->_gbAxisLimit->Controls->Add(this->_lb_Temperature);
            this->_gbAxisLimit->Controls->Add(this->_ud_YMin);
            this->_gbAxisLimit->Controls->Add(this->_ud_YMax);
            this->_gbAxisLimit->Controls->Add(this->_ud_XMax);
            this->_gbAxisLimit->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_gbAxisLimit->Location = System::Drawing::Point(839, 82);
            this->_gbAxisLimit->Name = L"_gbAxisLimit";
            this->_gbAxisLimit->Size = System::Drawing::Size(113, 147);
            this->_gbAxisLimit->TabIndex = 265;
            this->_gbAxisLimit->TabStop = false;
            this->_gbAxisLimit->Text = L"Axis Limits";
            // 
            // _txBM_AmbAirTmp
            // 
            this->_txBM_AmbAirTmp->Location = System::Drawing::Point(167, 432);
            this->_txBM_AmbAirTmp->Name = L"_txBM_AmbAirTmp";
            this->_txBM_AmbAirTmp->Size = System::Drawing::Size(36, 20);
            this->_txBM_AmbAirTmp->TabIndex = 266;
            this->_txBM_AmbAirTmp->Text = L"19";
            this->_txBM_AmbAirTmp->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // label9
            // 
            this->label9->AutoSize = true;
            this->label9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label9->Location = System::Drawing::Point(88, 431);
            this->label9->Name = L"label9";
            this->label9->Size = System::Drawing::Size(75, 30);
            this->label9->TabIndex = 267;
            this->label9->Text = L"Ambient Air \r\nTemp 21 (C)\r\n";
            // 
            // Form1
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(956, 477);
            this->Controls->Add(this->label9);
            this->Controls->Add(this->_txBM_AmbAirTmp);
            this->Controls->Add(this->_gbAxisLimit);
            this->Controls->Add(this->_btHelpRef);
            this->Controls->Add(this->label4);
            this->Controls->Add(this->label3);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->_cb_FireType);
            this->Controls->Add(this->_bt_Quit);
            this->Controls->Add(this->_cbSimTime);
            this->Controls->Add(this->_gbFirInt);
            this->Controls->Add(this->groupBox1);
            this->Controls->Add(this->_cb21);
            this->Controls->Add(this->_cb20);
            this->Controls->Add(this->_cb19);
            this->Controls->Add(this->_cb18);
            this->Controls->Add(this->_cb17);
            this->Controls->Add(this->_cb16);
            this->Controls->Add(this->_cb15);
            this->Controls->Add(this->_btBM_DrawGraph);
            this->Controls->Add(this->label42);
            this->Controls->Add(this->label40);
            this->Controls->Add(this->label39);
            this->Controls->Add(this->_txBM_SoiParDen);
            this->Controls->Add(this->_txBM_SoiBulDen);
            this->Controls->Add(this->label37);
            this->Controls->Add(this->_txBM_Moist);
            this->Controls->Add(this->_btBM_GraphClear);
            this->Controls->Add(this->label23);
            this->Controls->Add(this->_txMess);
            this->Controls->Add(this->_bt_AxReset);
            this->Controls->Add(this->chart2_Rabs);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->_cbL14);
            this->Controls->Add(this->_cbL13);
            this->Controls->Add(this->_cbL12);
            this->Controls->Add(this->_cbL11);
            this->Controls->Add(this->_cbL10);
            this->Controls->Add(this->_cbL9);
            this->Controls->Add(this->_cbL8);
            this->Controls->Add(this->_cbL7);
            this->Controls->Add(this->_cbL6);
            this->Controls->Add(this->_cbL5);
            this->Controls->Add(this->_cbL4);
            this->Controls->Add(this->_cbL3);
            this->Controls->Add(this->_cbL2);
            this->Controls->Add(this->_cbL1);
            this->Controls->Add(this->chart1);
            this->MaximumSize = System::Drawing::Size(964, 504);
            this->Name = L"Form1";
            this->Text = L"FOFEM - Massman Soil Analysis ";
            this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Form1::Form1_FormClosing);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_ud_XMax))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_ud_YMax))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_ud_YMin))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart2_Rabs))->EndInit();
            this->groupBox1->ResumeLayout(false);
            this->groupBox1->PerformLayout();
            this->_gbFirInt->ResumeLayout(false);
            this->_gbFirInt->PerformLayout();
            this->_gbAxisLimit->ResumeLayout(false);
            this->_gbAxisLimit->PerformLayout();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
 

/*****************************************************************************************\
* Button - Run Massman 
******************************************************************************************/
private: System::Void _bt_AxReset_Click(System::Object^  sender, System::EventArgs^  e) 
{
  if ( this->b_Running == true ) /* make sure it's not already running */
    return;
  this->iN_Runs++; 
  BM_Run("Run"); 
}

/********************************************************************
* Button - Clear Graph 
*********************************************************************/
private: System::Void _btBM_GraphClear_Click(System::Object^  sender, System::EventArgs^  e)
{
 if ( this->iN_Runs == 0 )     /* Don't try to Clear before any simulation has been run */
    return;                    /*  or could get a crash */
  gi_LastT = 0; 
  ClearTempGraph (); 
}
/***********************************************************
* Massman Redraw graph button clicked 
***********************************************************/
private: System::Void _btBM_DrawGraph_Click(System::Object^  sender, System::EventArgs^  e) 
{
 if ( this->iN_Runs == 0 )     /* Don't try to Redraw before any simulation has been run */
    return;                    /*  or could get a crash */
 gi_LastT = 0; 
 Graph_TempMoist (this->xbmi);
}

/**********************************************************************
* Massman Moisture Text Box - Keydown
***********************************************************************/
private: System::Void _txBM_Moist_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{
 // if ( e->KeyCode ==   System::Windows::Forms::Keys::Enter)
 //   MessageBox::Show("Return"); 
}

/**************************************************************
* Soil Profile Graph Window Button Clicked 
***************************************************************/
private: System::Void _btBM_Profile_Click(System::Object^  sender, System::EventArgs^  e) 
{
}

/***************************************************************************
* Layer Check Box 
***************************************************************************/
private: System::Void checkBox4_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
  gi_LastT = 0; 
}

/*****************************************************************************
* Temperature Radio Check box
******************************************************************************/
private: System::Void _rb_Temp_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
{

   this->chart1->Titles["Title1"]->Text = "Soil Temperature";   //     this->chart1->Title = "Soil Temperature"; 
   this->_ud_YMax->Visible = true;
   this->_ud_YMin->Visible = true;
   this->_lb_Temperature->Visible = true; 
   this->label15->Visible = true; 
   gi_LastT = 0; 

 if ( this->iN_Runs == 0 )     /* Don't try  before any simulation has been run */
    return;                    /*  or could get a crash */


   Graph_TempMoist (this->xbmi);
}

/***************************************************************************
* Moisture Radio Check box 
*********************************************************************************/
private: System::Void _rb_Moist_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
   this->chart1->Titles["Title1"]->Text = "Soil Moisture";
   this->_ud_YMax->Visible = false;
   this->_ud_YMin->Visible = false;
   this->_lb_Temperature->Visible = false; 
   this->label15->Visible = false; 
   gi_LastT = 0; 

 if ( this->iN_Runs == 0 )     /* Don't try before any simulation has been run */
    return;                    /*  or could get a crash */

   Graph_TempMoist (this->xbmi);
 }

/************************************************************************
*
************************************************************************/
private: System::Void _bt_FirIntGraph_Click(System::Object^  sender, System::EventArgs^  e)
{
  if ( this->b_Running == true ) 
    return ;                 /* don't anything a simulation is running  */
  this->BM_Run("FirIntGraph");
}

/******************************************************************************
* Abort-Quit Button hit 
******************************************************************************/
private: System::Void _bt_Quit_Click(System::Object^  sender, System::EventArgs^  e)
{
   this->b_Abort = true; 
}
/*****************************************************************************************
* Form Closing, User hit 'X' corner button to close,
* We just want to hide the form, so we interecept, or else the form would close.
******************************************************************************************/
private: System::Void Form1_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
{
  if ( this->b_Running == true ) {
   myMessageBox ("Please stop the simulation before closing window"); 
 e->Cancel = true; 
   return; }

  this->Hide();
  e->Cancel = true; 
}

private: System::Void _txBM_Minutes_TextChanged(System::Object^  sender, System::EventArgs^  e) 
{}

private: System::Void _cb_FireType_Click(System::Object^  sender, System::EventArgs^  e)
{}

/*****************************************************************************
* Fire Type Combobox changed  
******************************************************************************/
private: System::Void _cb_FireType_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
{
  if ( this->b_Running == true )   /* Make sure a simulation isn't currently running */ 
    return; 
  FireTypeSet(); 
}

/**************************************************************************
* HElp Reference Button was clicked 
***************************************************************************/
private: System::Void _btHelpRef_Click(System::Object^  sender, System::EventArgs^  e) 
{
String ^Str; 
Help_Form  ^hf; 

   hf = gcnew Help_Form;
/* Get the directory that FuelCalc is run from */
  Str = System::Windows::Forms::Application::StartupPath;
  Str = Str + "\\" + "BM_Reference.rtf";
  hf->richTextBox1->LoadFile(Str); 
  hf->Show(); 
}




};

