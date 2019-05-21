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

/* The total number of Graphing/Layer Check Boxes on the GUI, not including extended Massman layers */  
#define e_CBmax  14

#define e_FT_Wild "Wild Fire"
#define e_FT_Pres "Prescribed Burn"
#define e_FT_Pile "Pile Burn"
#define e_FT_Burnup  "Burnup Model"
#define e_FT_Test "Test" 

extern int gi_LastT; 


/***************************************************************/
public ref class Form1 : public System::Windows::Forms::Form
{
public:
  int iS_Init; 
  bool b_Abort; 
  bool b_Running; 
  String ^ s_UserFolder; 
  int   *ir_KwM2;  /* Heat  Kw/m2  */
  int   i_TimInc;  /* Time increments - heat at array is in */ 

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
private: System::Windows::Forms::TextBox^  _txTqMax;
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
public: 
private: System::Windows::Forms::Label^  label88;
public: 
   int Burnup(); 
   int FireTypeSet(); 
   int Load_Controls(); 
   int isChecked(int i);

   int ChkBoxArr(int ir[], int iN); 

   int FireInten_Graph(double  fr[], float f_IncSec ); 
   int BM_Run (char cr_Mode[]); 

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

  ir_KwM2 = new int [eC_nsteps]; 
  ir_KwM2[0] = -1; 
  i_TimInc = 0; 

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
 delete ir_KwM2;
}

/***********************************************************/

    private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart1;
    public: System::Windows::Forms::CheckBox^  _cbL3;
    public: System::Windows::Forms::CheckBox^  _cbL2;
    public: System::Windows::Forms::CheckBox^  _cbL1;
private: System::ComponentModel::IContainer^  components;
public: 

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
            System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea5 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
            System::Windows::Forms::DataVisualization::Charting::Legend^  legend3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
            System::Windows::Forms::DataVisualization::Charting::Series^  series101 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series102 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series103 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series104 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series105 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series106 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series107 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series108 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series109 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series110 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series111 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series112 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series113 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series114 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series115 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series116 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series117 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series118 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series119 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series120 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series121 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series122 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series123 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series124 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series125 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series126 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series127 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series128 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series129 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series130 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series131 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series132 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series133 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series134 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series135 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series136 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series137 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series138 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series139 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series140 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series141 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series142 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series143 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series144 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series145 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series146 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series147 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series148 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series149 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Title^  title5 = (gcnew System::Windows::Forms::DataVisualization::Charting::Title());
            System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea6 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
            System::Windows::Forms::DataVisualization::Charting::Series^  series150 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Title^  title6 = (gcnew System::Windows::Forms::DataVisualization::Charting::Title());
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
            this->label88 = (gcnew System::Windows::Forms::Label());
            this->label6 = (gcnew System::Windows::Forms::Label());
            this->label5 = (gcnew System::Windows::Forms::Label());
            this->_txQabs = (gcnew System::Windows::Forms::TextBox());
            this->_txTqMax = (gcnew System::Windows::Forms::TextBox());
            this->_cbSimTime = (gcnew System::Windows::Forms::ComboBox());
            this->_bt_Quit = (gcnew System::Windows::Forms::Button());
            this->_cb_FireType = (gcnew System::Windows::Forms::ComboBox());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->label4 = (gcnew System::Windows::Forms::Label());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_ud_XMax))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_ud_YMax))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_ud_YMin))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart2_Rabs))->BeginInit();
            this->groupBox1->SuspendLayout();
            this->_gbFirInt->SuspendLayout();
            this->SuspendLayout();
            // 
            // chart1
            // 
            chartArea5->AxisX->ScaleView->MinSize = 5;
            chartArea5->AxisX->ScaleView->MinSizeType = System::Windows::Forms::DataVisualization::Charting::DateTimeIntervalType::Number;
            chartArea5->AxisX->ScaleView->Position = 0;
            chartArea5->AxisX->ScaleView->SizeType = System::Windows::Forms::DataVisualization::Charting::DateTimeIntervalType::Number;
            chartArea5->AxisX->ScaleView->SmallScrollMinSizeType = System::Windows::Forms::DataVisualization::Charting::DateTimeIntervalType::Number;
            chartArea5->AxisX->ScaleView->SmallScrollSizeType = System::Windows::Forms::DataVisualization::Charting::DateTimeIntervalType::Number;
            chartArea5->AxisX->Title = L"Hours";
            chartArea5->AxisY->Title = L"Temperature Celsius";
            chartArea5->Name = L"ChartArea1";
            chartArea5->Position->Auto = false;
            chartArea5->Position->Height = 94;
            chartArea5->Position->Width = 82;
            chartArea5->Position->X = 13;
            chartArea5->Position->Y = 3;
            this->chart1->ChartAreas->Add(chartArea5);
            legend3->Enabled = false;
            legend3->Name = L"Legend1";
            legend3->Position->Auto = false;
            legend3->Position->Height = 19.58763F;
            legend3->Position->Width = 14.28571F;
            legend3->Position->X = 2.71429F;
            legend3->Position->Y = 3;
            this->chart1->Legends->Add(legend3);
            this->chart1->Location = System::Drawing::Point(76, 12);
            this->chart1->Name = L"chart1";
            series101->BorderWidth = 3;
            series101->ChartArea = L"ChartArea1";
            series101->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
            series101->Color = System::Drawing::Color::Red;
            series101->Legend = L"Legend1";
            series101->Name = L"A_Lay1";
            series102->BorderWidth = 3;
            series102->ChartArea = L"ChartArea1";
            series102->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series102->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)), 
                static_cast<System::Int32>(static_cast<System::Byte>(128)));
            series102->Legend = L"Legend1";
            series102->Name = L"A_Lay2";
            series103->BorderWidth = 3;
            series103->ChartArea = L"ChartArea1";
            series103->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series103->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)), 
                static_cast<System::Int32>(static_cast<System::Byte>(0)));
            series103->Legend = L"Legend1";
            series103->LegendText = L"2 cm";
            series103->Name = L"A_Lay3";
            series104->BorderWidth = 3;
            series104->ChartArea = L"ChartArea1";
            series104->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series104->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
                static_cast<System::Int32>(static_cast<System::Byte>(128)));
            series104->Legend = L"Legend1";
            series104->LegendText = L"4 cm";
            series104->Name = L"A_Lay4";
            series105->BorderWidth = 3;
            series105->ChartArea = L"ChartArea1";
            series105->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series105->Color = System::Drawing::Color::Lime;
            series105->Legend = L"Legend1";
            series105->LegendText = L"cm 2";
            series105->Name = L"A_Lay5";
            series106->BorderWidth = 3;
            series106->ChartArea = L"ChartArea1";
            series106->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series106->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
                static_cast<System::Int32>(static_cast<System::Byte>(0)));
            series106->Legend = L"Legend1";
            series106->LegendText = L"cm 2";
            series106->Name = L"A_Lay6";
            series107->BorderWidth = 3;
            series107->ChartArea = L"ChartArea1";
            series107->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series107->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
                static_cast<System::Int32>(static_cast<System::Byte>(192)));
            series107->Legend = L"Legend1";
            series107->LegendText = L"cm 2";
            series107->Name = L"A_Lay7";
            series108->BorderWidth = 3;
            series108->ChartArea = L"ChartArea1";
            series108->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series108->Color = System::Drawing::Color::Teal;
            series108->Legend = L"Legend1";
            series108->LegendText = L"cm 2";
            series108->Name = L"A_Lay8";
            series109->BorderWidth = 3;
            series109->ChartArea = L"ChartArea1";
            series109->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series109->Color = System::Drawing::Color::Green;
            series109->Legend = L"Legend1";
            series109->LegendText = L"cm 2";
            series109->Name = L"A_Lay9";
            series110->BorderWidth = 3;
            series110->ChartArea = L"ChartArea1";
            series110->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series110->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
                static_cast<System::Int32>(static_cast<System::Byte>(0)));
            series110->Legend = L"Legend1";
            series110->LegendText = L"cm 2";
            series110->Name = L"A_Lay10";
            series111->BorderWidth = 3;
            series111->ChartArea = L"ChartArea1";
            series111->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series111->Color = System::Drawing::Color::Silver;
            series111->Legend = L"Legend1";
            series111->LegendText = L"cm 2";
            series111->Name = L"A_Lay11";
            series112->BorderWidth = 3;
            series112->ChartArea = L"ChartArea1";
            series112->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series112->Color = System::Drawing::Color::Gray;
            series112->Legend = L"Legend1";
            series112->LegendText = L"cm 2";
            series112->Name = L"A_Lay12";
            series113->BorderWidth = 3;
            series113->ChartArea = L"ChartArea1";
            series113->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series113->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(0)), 
                static_cast<System::Int32>(static_cast<System::Byte>(192)));
            series113->Legend = L"Legend1";
            series113->LegendText = L"cm 2";
            series113->Name = L"A_Lay13";
            series114->BorderWidth = 3;
            series114->ChartArea = L"ChartArea1";
            series114->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series114->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
                static_cast<System::Int32>(static_cast<System::Byte>(64)));
            series114->Legend = L"Legend1";
            series114->LegendText = L"cm 2";
            series114->Name = L"A_Lay14";
            series115->BorderColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
                static_cast<System::Int32>(static_cast<System::Byte>(192)));
            series115->BorderDashStyle = System::Windows::Forms::DataVisualization::Charting::ChartDashStyle::Dot;
            series115->ChartArea = L"ChartArea1";
            series115->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series115->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
                static_cast<System::Int32>(static_cast<System::Byte>(192)));
            series115->Legend = L"Legend1";
            series115->Name = L"B_Lay1";
            series116->BorderDashStyle = System::Windows::Forms::DataVisualization::Charting::ChartDashStyle::Dash;
            series116->BorderWidth = 2;
            series116->ChartArea = L"ChartArea1";
            series116->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series116->Legend = L"Legend1";
            series116->Name = L"B_Lay2";
            series117->BorderDashStyle = System::Windows::Forms::DataVisualization::Charting::ChartDashStyle::Dash;
            series117->BorderWidth = 2;
            series117->ChartArea = L"ChartArea1";
            series117->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series117->Legend = L"Legend1";
            series117->Name = L"B_Lay3";
            series118->BorderDashStyle = System::Windows::Forms::DataVisualization::Charting::ChartDashStyle::Dash;
            series118->BorderWidth = 2;
            series118->ChartArea = L"ChartArea1";
            series118->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series118->Legend = L"Legend1";
            series118->Name = L"B_Lay4";
            series119->BorderDashStyle = System::Windows::Forms::DataVisualization::Charting::ChartDashStyle::Dash;
            series119->BorderWidth = 2;
            series119->ChartArea = L"ChartArea1";
            series119->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series119->Legend = L"Legend1";
            series119->Name = L"B_Lay5";
            series120->BorderDashStyle = System::Windows::Forms::DataVisualization::Charting::ChartDashStyle::Dash;
            series120->BorderWidth = 2;
            series120->ChartArea = L"ChartArea1";
            series120->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series120->Legend = L"Legend1";
            series120->Name = L"B_Lay6";
            series121->BorderDashStyle = System::Windows::Forms::DataVisualization::Charting::ChartDashStyle::Dash;
            series121->BorderWidth = 2;
            series121->ChartArea = L"ChartArea1";
            series121->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series121->Legend = L"Legend1";
            series121->Name = L"B_Lay7";
            series122->BorderDashStyle = System::Windows::Forms::DataVisualization::Charting::ChartDashStyle::Dash;
            series122->BorderWidth = 2;
            series122->ChartArea = L"ChartArea1";
            series122->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series122->Legend = L"Legend1";
            series122->Name = L"B_Lay8";
            series123->BorderDashStyle = System::Windows::Forms::DataVisualization::Charting::ChartDashStyle::Dash;
            series123->BorderWidth = 2;
            series123->ChartArea = L"ChartArea1";
            series123->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series123->Legend = L"Legend1";
            series123->Name = L"B_Lay9";
            series124->BorderDashStyle = System::Windows::Forms::DataVisualization::Charting::ChartDashStyle::Dash;
            series124->BorderWidth = 2;
            series124->ChartArea = L"ChartArea1";
            series124->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series124->Legend = L"Legend1";
            series124->Name = L"B_Lay10";
            series125->BorderDashStyle = System::Windows::Forms::DataVisualization::Charting::ChartDashStyle::Dash;
            series125->BorderWidth = 2;
            series125->ChartArea = L"ChartArea1";
            series125->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series125->Legend = L"Legend1";
            series125->Name = L"B_Lay11";
            series126->BorderDashStyle = System::Windows::Forms::DataVisualization::Charting::ChartDashStyle::Dash;
            series126->BorderWidth = 2;
            series126->ChartArea = L"ChartArea1";
            series126->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series126->Legend = L"Legend1";
            series126->Name = L"B_Lay12";
            series127->BorderDashStyle = System::Windows::Forms::DataVisualization::Charting::ChartDashStyle::Dash;
            series127->BorderWidth = 2;
            series127->ChartArea = L"ChartArea1";
            series127->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series127->Legend = L"Legend1";
            series127->Name = L"B_Lay13";
            series128->BorderDashStyle = System::Windows::Forms::DataVisualization::Charting::ChartDashStyle::Dash;
            series128->BorderWidth = 2;
            series128->ChartArea = L"ChartArea1";
            series128->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series128->Legend = L"Legend1";
            series128->Name = L"B_Lay14";
            series129->ChartArea = L"ChartArea1";
            series129->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series129->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
                static_cast<System::Int32>(static_cast<System::Byte>(255)));
            series129->Legend = L"Legend1";
            series129->Name = L"B_Lay15";
            series130->ChartArea = L"ChartArea1";
            series130->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series130->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)), 
                static_cast<System::Int32>(static_cast<System::Byte>(255)));
            series130->Legend = L"Legend1";
            series130->Name = L"B_Lay16";
            series131->ChartArea = L"ChartArea1";
            series131->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series131->Color = System::Drawing::Color::Fuchsia;
            series131->Legend = L"Legend1";
            series131->Name = L"B_Lay17";
            series132->ChartArea = L"ChartArea1";
            series132->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series132->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(0)), 
                static_cast<System::Int32>(static_cast<System::Byte>(192)));
            series132->Legend = L"Legend1";
            series132->Name = L"B_Lay18";
            series133->ChartArea = L"ChartArea1";
            series133->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series133->Color = System::Drawing::Color::Purple;
            series133->Legend = L"Legend1";
            series133->Name = L"B_Lay19";
            series134->ChartArea = L"ChartArea1";
            series134->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series134->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(0)), 
                static_cast<System::Int32>(static_cast<System::Byte>(64)));
            series134->Legend = L"Legend1";
            series134->Name = L"B_Lay20";
            series135->ChartArea = L"ChartArea1";
            series135->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series135->Color = System::Drawing::Color::Black;
            series135->Legend = L"Legend1";
            series135->Name = L"B_Lay21";
            series136->ChartArea = L"ChartArea1";
            series136->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series136->Legend = L"Legend1";
            series136->Name = L"F_Lay1";
            series137->ChartArea = L"ChartArea1";
            series137->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series137->Legend = L"Legend1";
            series137->Name = L"F_Lay2";
            series138->ChartArea = L"ChartArea1";
            series138->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series138->Legend = L"Legend1";
            series138->Name = L"F_Lay3";
            series139->ChartArea = L"ChartArea1";
            series139->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series139->Legend = L"Legend1";
            series139->Name = L"F_Lay4";
            series140->ChartArea = L"ChartArea1";
            series140->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series140->Legend = L"Legend1";
            series140->Name = L"F_Lay5";
            series141->ChartArea = L"ChartArea1";
            series141->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series141->Legend = L"Legend1";
            series141->Name = L"F_Lay6";
            series142->ChartArea = L"ChartArea1";
            series142->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series142->Legend = L"Legend1";
            series142->Name = L"F_Lay7";
            series143->ChartArea = L"ChartArea1";
            series143->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series143->Legend = L"Legend1";
            series143->Name = L"F_Lay8";
            series144->ChartArea = L"ChartArea1";
            series144->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series144->Legend = L"Legend1";
            series144->Name = L"F_Lay9";
            series145->ChartArea = L"ChartArea1";
            series145->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series145->Legend = L"Legend1";
            series145->Name = L"F_Lay10";
            series146->ChartArea = L"ChartArea1";
            series146->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series146->Legend = L"Legend1";
            series146->Name = L"F_Lay11";
            series147->ChartArea = L"ChartArea1";
            series147->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series147->Legend = L"Legend1";
            series147->Name = L"F_Lay12";
            series148->ChartArea = L"ChartArea1";
            series148->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series148->Legend = L"Legend1";
            series148->Name = L"F_Lay13";
            series149->ChartArea = L"ChartArea1";
            series149->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series149->Legend = L"Legend1";
            series149->Name = L"F_Lay14";
            this->chart1->Series->Add(series101);
            this->chart1->Series->Add(series102);
            this->chart1->Series->Add(series103);
            this->chart1->Series->Add(series104);
            this->chart1->Series->Add(series105);
            this->chart1->Series->Add(series106);
            this->chart1->Series->Add(series107);
            this->chart1->Series->Add(series108);
            this->chart1->Series->Add(series109);
            this->chart1->Series->Add(series110);
            this->chart1->Series->Add(series111);
            this->chart1->Series->Add(series112);
            this->chart1->Series->Add(series113);
            this->chart1->Series->Add(series114);
            this->chart1->Series->Add(series115);
            this->chart1->Series->Add(series116);
            this->chart1->Series->Add(series117);
            this->chart1->Series->Add(series118);
            this->chart1->Series->Add(series119);
            this->chart1->Series->Add(series120);
            this->chart1->Series->Add(series121);
            this->chart1->Series->Add(series122);
            this->chart1->Series->Add(series123);
            this->chart1->Series->Add(series124);
            this->chart1->Series->Add(series125);
            this->chart1->Series->Add(series126);
            this->chart1->Series->Add(series127);
            this->chart1->Series->Add(series128);
            this->chart1->Series->Add(series129);
            this->chart1->Series->Add(series130);
            this->chart1->Series->Add(series131);
            this->chart1->Series->Add(series132);
            this->chart1->Series->Add(series133);
            this->chart1->Series->Add(series134);
            this->chart1->Series->Add(series135);
            this->chart1->Series->Add(series136);
            this->chart1->Series->Add(series137);
            this->chart1->Series->Add(series138);
            this->chart1->Series->Add(series139);
            this->chart1->Series->Add(series140);
            this->chart1->Series->Add(series141);
            this->chart1->Series->Add(series142);
            this->chart1->Series->Add(series143);
            this->chart1->Series->Add(series144);
            this->chart1->Series->Add(series145);
            this->chart1->Series->Add(series146);
            this->chart1->Series->Add(series147);
            this->chart1->Series->Add(series148);
            this->chart1->Series->Add(series149);
            this->chart1->Size = System::Drawing::Size(749, 222);
            this->chart1->TabIndex = 0;
            this->chart1->Text = L"chart1";
            title5->Alignment = System::Drawing::ContentAlignment::TopLeft;
            title5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            title5->Name = L"Title1";
            title5->Text = L"Soil Layers";
            this->chart1->Titles->Add(title5);
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
            this->_ud_XMax->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {2, 0, 0, 0});
            this->_ud_XMax->Location = System::Drawing::Point(858, 209);
            this->_ud_XMax->Margin = System::Windows::Forms::Padding(2);
            this->_ud_XMax->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {122, 0, 0, 0});
            this->_ud_XMax->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
            this->_ud_XMax->Name = L"_ud_XMax";
            this->_ud_XMax->ReadOnly = true;
            this->_ud_XMax->Size = System::Drawing::Size(64, 20);
            this->_ud_XMax->TabIndex = 150;
            this->_ud_XMax->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->_ud_XMax->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {12, 0, 0, 0});
            // 
            // _ud_YMax
            // 
            this->_ud_YMax->BackColor = System::Drawing::Color::White;
            this->_ud_YMax->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
            this->_ud_YMax->Location = System::Drawing::Point(858, 113);
            this->_ud_YMax->Margin = System::Windows::Forms::Padding(2);
            this->_ud_YMax->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {2000, 0, 0, 0});
            this->_ud_YMax->Name = L"_ud_YMax";
            this->_ud_YMax->ReadOnly = true;
            this->_ud_YMax->Size = System::Drawing::Size(64, 20);
            this->_ud_YMax->TabIndex = 152;
            this->_ud_YMax->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->_ud_YMax->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {500, 0, 0, 0});
            // 
            // _ud_YMin
            // 
            this->_ud_YMin->BackColor = System::Drawing::Color::White;
            this->_ud_YMin->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
            this->_ud_YMin->Location = System::Drawing::Point(858, 158);
            this->_ud_YMin->Margin = System::Windows::Forms::Padding(2);
            this->_ud_YMin->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
            this->_ud_YMin->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, System::Int32::MinValue});
            this->_ud_YMin->Name = L"_ud_YMin";
            this->_ud_YMin->ReadOnly = true;
            this->_ud_YMin->Size = System::Drawing::Size(64, 20);
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
            this->label1->Location = System::Drawing::Point(19, 7);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(36, 13);
            this->label1->TabIndex = 166;
            this->label1->Text = L"Depth";
            // 
            // chart2_Rabs
            // 
            chartArea6->AxisX->Title = L"Hours";
            chartArea6->AxisY->Title = L"Watts  W/m2";
            chartArea6->Name = L"ChartArea1";
            chartArea6->Position->Auto = false;
            chartArea6->Position->Height = 96;
            chartArea6->Position->Width = 84;
            chartArea6->Position->X = 12;
            chartArea6->Position->Y = 2;
            this->chart2_Rabs->ChartAreas->Add(chartArea6);
            this->chart2_Rabs->Location = System::Drawing::Point(78, 245);
            this->chart2_Rabs->Name = L"chart2_Rabs";
            series150->BorderWidth = 4;
            series150->ChartArea = L"ChartArea1";
            series150->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
            series150->Name = L"Rabs";
            this->chart2_Rabs->Series->Add(series150);
            this->chart2_Rabs->Size = System::Drawing::Size(747, 139);
            this->chart2_Rabs->TabIndex = 178;
            this->chart2_Rabs->Text = L"chart2";
            title6->Alignment = System::Drawing::ContentAlignment::TopLeft;
            title6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            title6->Name = L"Title1";
            title6->Text = L"Fire Intensity";
            this->chart2_Rabs->Titles->Add(title6);
            // 
            // _lb_Temperature
            // 
            this->_lb_Temperature->AutoSize = true;
            this->_lb_Temperature->Location = System::Drawing::Point(854, 97);
            this->_lb_Temperature->Name = L"_lb_Temperature";
            this->_lb_Temperature->Size = System::Drawing::Size(66, 13);
            this->_lb_Temperature->TabIndex = 181;
            this->_lb_Temperature->Text = L"Upper Temp";
            // 
            // label15
            // 
            this->label15->AutoSize = true;
            this->label15->Location = System::Drawing::Point(855, 141);
            this->label15->Name = L"label15";
            this->label15->Size = System::Drawing::Size(66, 13);
            this->label15->TabIndex = 182;
            this->label15->Text = L"Lower Temp";
            // 
            // label17
            // 
            this->label17->AutoSize = true;
            this->label17->Location = System::Drawing::Point(868, 192);
            this->label17->Name = L"label17";
            this->label17->Size = System::Drawing::Size(52, 13);
            this->label17->TabIndex = 184;
            this->label17->Text = L"End Hour";
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
            this->_txBM_Moist->Location = System::Drawing::Point(442, 431);
            this->_txBM_Moist->Name = L"_txBM_Moist";
            this->_txBM_Moist->RightToLeft = System::Windows::Forms::RightToLeft::No;
            this->_txBM_Moist->Size = System::Drawing::Size(50, 21);
            this->_txBM_Moist->TabIndex = 228;
            this->_txBM_Moist->Text = L"0.142";
            this->_txBM_Moist->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txBM_Moist, L"Moisture by Volume\r\nDefault: 0.142\r\nSee program variable - thetai\r\n");
            this->_txBM_Moist->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::_txBM_Moist_KeyDown);
            // 
            // _txBM_SoiBulDen
            // 
            this->_txBM_SoiBulDen->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_txBM_SoiBulDen->Location = System::Drawing::Point(648, 431);
            this->_txBM_SoiBulDen->Name = L"_txBM_SoiBulDen";
            this->_txBM_SoiBulDen->RightToLeft = System::Windows::Forms::RightToLeft::No;
            this->_txBM_SoiBulDen->Size = System::Drawing::Size(50, 21);
            this->_txBM_SoiBulDen->TabIndex = 232;
            this->_txBM_SoiBulDen->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txBM_SoiBulDen, L"Soil Bulk Density 1.56 gm/cm3");
            // 
            // _txBM_SoiParDen
            // 
            this->_txBM_SoiParDen->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_txBM_SoiParDen->Location = System::Drawing::Point(866, 434);
            this->_txBM_SoiParDen->Name = L"_txBM_SoiParDen";
            this->_txBM_SoiParDen->RightToLeft = System::Windows::Forms::RightToLeft::No;
            this->_txBM_SoiParDen->Size = System::Drawing::Size(50, 21);
            this->_txBM_SoiParDen->TabIndex = 233;
            this->_txBM_SoiParDen->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txBM_SoiParDen, L"Soil Particle Density 2.82 g/cm3");
            // 
            // _txMaxTim
            // 
            this->_txMaxTim->Location = System::Drawing::Point(21, 108);
            this->_txMaxTim->Name = L"_txMaxTim";
            this->_txMaxTim->Size = System::Drawing::Size(61, 20);
            this->_txMaxTim->TabIndex = 260;
            this->_txMaxTim->Text = L"0.6";
            this->_txMaxTim->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txMaxTim, L"Time (hr) of maximum \r\nwatts are applied ");
            // 
            // _txMess
            // 
            this->_txMess->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_txMess->Location = System::Drawing::Point(11, 366);
            this->_txMess->Name = L"_txMess";
            this->_txMess->Size = System::Drawing::Size(54, 21);
            this->_txMess->TabIndex = 194;
            this->_txMess->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // label23
            // 
            this->label23->AutoSize = true;
            this->label23->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label23->Location = System::Drawing::Point(618, 400);
            this->label23->Name = L"label23";
            this->label23->Size = System::Drawing::Size(148, 16);
            this->label23->TabIndex = 197;
            this->label23->Text = L"Simulation Time (hours)";
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
            this->_rb_Temp->Location = System::Drawing::Point(12, 27);
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
            this->_rb_Moist->Location = System::Drawing::Point(13, 50);
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
            this->label37->Location = System::Drawing::Point(308, 434);
            this->label37->Name = L"label37";
            this->label37->Size = System::Drawing::Size(128, 15);
            this->label37->TabIndex = 229;
            this->label37->Text = L" Moisture/theta   0.142";
            // 
            // label39
            // 
            this->label39->AutoSize = true;
            this->label39->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label39->ForeColor = System::Drawing::SystemColors::ControlText;
            this->label39->Location = System::Drawing::Point(503, 433);
            this->label39->Name = L"label39";
            this->label39->Size = System::Drawing::Size(143, 15);
            this->label39->TabIndex = 234;
            this->label39->Text = L"Soil Bulk Density  Mg/m3";
            // 
            // label40
            // 
            this->label40->AutoSize = true;
            this->label40->ForeColor = System::Drawing::SystemColors::Desktop;
            this->label40->Location = System::Drawing::Point(374, 447);
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
            this->label42->Location = System::Drawing::Point(705, 434);
            this->label42->Name = L"label42";
            this->label42->Size = System::Drawing::Size(160, 15);
            this->label42->TabIndex = 236;
            this->label42->Text = L"Soil Particle Density  Mg/m3";
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
            this->groupBox1->Location = System::Drawing::Point(832, 8);
            this->groupBox1->Name = L"groupBox1";
            this->groupBox1->Size = System::Drawing::Size(113, 82);
            this->groupBox1->TabIndex = 250;
            this->groupBox1->TabStop = false;
            this->groupBox1->Text = L"Graph Type";
            // 
            // _bt_FirIntGraph
            // 
            this->_bt_FirIntGraph->Location = System::Drawing::Point(19, 17);
            this->_bt_FirIntGraph->Name = L"_bt_FirIntGraph";
            this->_bt_FirIntGraph->Size = System::Drawing::Size(70, 26);
            this->_bt_FirIntGraph->TabIndex = 252;
            this->_bt_FirIntGraph->Text = L"Graph Watts";
            this->_bt_FirIntGraph->UseVisualStyleBackColor = true;
            this->_bt_FirIntGraph->Click += gcnew System::EventHandler(this, &Form1::_bt_FirIntGraph_Click);
            // 
            // _gbFirInt
            // 
            this->_gbFirInt->Controls->Add(this->label88);
            this->_gbFirInt->Controls->Add(this->_txMaxTim);
            this->_gbFirInt->Controls->Add(this->label6);
            this->_gbFirInt->Controls->Add(this->label5);
            this->_gbFirInt->Controls->Add(this->_txQabs);
            this->_gbFirInt->Controls->Add(this->_txTqMax);
            this->_gbFirInt->Controls->Add(this->_bt_FirIntGraph);
            this->_gbFirInt->Location = System::Drawing::Point(839, 239);
            this->_gbFirInt->Name = L"_gbFirInt";
            this->_gbFirInt->Size = System::Drawing::Size(109, 180);
            this->_gbFirInt->TabIndex = 255;
            this->_gbFirInt->TabStop = false;
            this->_gbFirInt->Text = L"Fire Intensity";
            // 
            // label88
            // 
            this->label88->AutoSize = true;
            this->label88->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label88->Location = System::Drawing::Point(5, 89);
            this->label88->Name = L"label88";
            this->label88->Size = System::Drawing::Size(98, 15);
            this->label88->TabIndex = 261;
            this->label88->Text = L"Watts  Max Time";
            // 
            // label6
            // 
            this->label6->AutoSize = true;
            this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label6->Location = System::Drawing::Point(14, 49);
            this->label6->Name = L"label6";
            this->label6->Size = System::Drawing::Size(75, 15);
            this->label6->TabIndex = 258;
            this->label6->Text = L"Watts  W/m2";
            // 
            // label5
            // 
            this->label5->AutoSize = true;
            this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label5->Location = System::Drawing::Point(5, 133);
            this->label5->Name = L"label5";
            this->label5->Size = System::Drawing::Size(86, 15);
            this->label5->TabIndex = 257;
            this->label5->Text = L"Burn Time (hr)";
            // 
            // _txQabs
            // 
            this->_txQabs->Location = System::Drawing::Point(25, 66);
            this->_txQabs->Name = L"_txQabs";
            this->_txQabs->Size = System::Drawing::Size(53, 20);
            this->_txQabs->TabIndex = 256;
            this->_txQabs->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txTqMax
            // 
            this->_txTqMax->Location = System::Drawing::Point(25, 154);
            this->_txTqMax->MaximumSize = System::Drawing::Size(53, 20);
            this->_txTqMax->Name = L"_txTqMax";
            this->_txTqMax->Size = System::Drawing::Size(53, 20);
            this->_txTqMax->TabIndex = 255;
            this->_txTqMax->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _cbSimTime
            // 
            this->_cbSimTime->FormattingEnabled = true;
            this->_cbSimTime->Items->AddRange(gcnew cli::array< System::Object^  >(7) {L"1.0", L"2.0", L"4.0", L"6.0", L"12", L"24", 
                L"120"});
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
            this->_cb_FireType->Location = System::Drawing::Point(501, 395);
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
            this->label2->Location = System::Drawing::Point(421, 398);
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
            this->label3->Location = System::Drawing::Point(4, 395);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(70, 16);
            this->label3->TabIndex = 262;
            this->label3->Text = L"Simulation";
            // 
            // label4
            // 
            this->label4->AutoSize = true;
            this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label4->Location = System::Drawing::Point(14, 347);
            this->label4->Name = L"label4";
            this->label4->Size = System::Drawing::Size(49, 16);
            this->label4->TabIndex = 263;
            this->label4->Text = L"Hr::Min";
            // 
            // Form1
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(956, 470);
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
            this->Controls->Add(this->label17);
            this->Controls->Add(this->label15);
            this->Controls->Add(this->_lb_Temperature);
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
            this->Controls->Add(this->_ud_YMin);
            this->Controls->Add(this->_ud_YMax);
            this->Controls->Add(this->_ud_XMax);
            this->Controls->Add(this->_cbL3);
            this->Controls->Add(this->_cbL2);
            this->Controls->Add(this->_cbL1);
            this->Controls->Add(this->chart1);
            this->MaximumSize = System::Drawing::Size(964, 497);
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

  BM_Run("Run"); 
}

/********************************************************************
* Button - Clear Graph 
*********************************************************************/
private: System::Void _btBM_GraphClear_Click(System::Object^  sender, System::EventArgs^  e)
{
  gi_LastT = 0; 
  ClearTempGraph (); 
}
/***********************************************************
* Massman Redraw graph button clicked 
***********************************************************/
private: System::Void _btBM_DrawGraph_Click(System::Object^  sender, System::EventArgs^  e) 
{
 gi_LastT = 0; 
 BM_Run("Draw"); 
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
   this->_ud_YMax->Visible = true;
   this->_ud_YMin->Visible = true;
   this->_lb_Temperature->Visible = true; 
   this->label15->Visible = true; 
   gi_LastT = 0; 
  this->BM_Run("Draw"); 
}

/***************************************************************************
* Moisture Radio Check box 
*********************************************************************************/
private: System::Void _rb_Moist_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
   this->_ud_YMax->Visible = false;
   this->_ud_YMin->Visible = false;
   this->_lb_Temperature->Visible = false; 
   this->label15->Visible = false; 
   gi_LastT = 0; 
 this->BM_Run("Draw"); 
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
{
   
}

/*****************************************************************************
* Fire Type Combobox changed  
******************************************************************************/
private: System::Void _cb_FireType_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
{
  if ( this->b_Running == true )   /* Make sure a simulation isn't currently running */ 
    return; 
  FireTypeSet(); 
}




};

