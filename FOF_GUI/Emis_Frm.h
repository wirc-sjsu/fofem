#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#define private public
#define protected public

// #include "..\fc_dll\fc_dll.h"

#include "fof_sgv.h"
#include "fof_co.h" 
// #include "..\fc_gui\frm_util.h"

/*******************************************************************/	
public ref class Grap_Emis : public System::Windows::Forms::Form
{
public:
    
 /* this has to be big enough to accomondate array coming out of FOFEM's Burnup */
/* I could use eC_SGV in FOF_SGV.H, #define eC_fr eC_SGV, but if I have this GUI */
/* start using the actual FOFEM.DLL someday things would get sticky */
#define eC_fr 2000 
 
 float *fr_PM25;
  float *fr_PM10;
  float *fr_CH4;
  float *fr_CO2;
  float *fr_CO;
  float *fr_NOX;
  float *fr_SO2;
  float *fr_Int;
  int   i_Cnt;
  int   i_Minutes; 

  float  f_fPM25;
  float  f_fPM10;
  float  f_fCH4;
  float  f_fCO2;
  float  f_fCO ;
  float  f_fNOX;
  float  f_fSO2;

  float  f_sPM25;
  float  f_sPM10;
  float  f_sCH4;
  float  f_sCO2;
  float  f_sCO ;
  float  f_sNOX;
  float  f_sSO2;

private: System::Windows::Forms::TextBox^  _txXAxis;
private: System::Windows::Forms::TextBox^  _txYAxis; 
private: System::Windows::Forms::Label^  label10;
private: System::Windows::Forms::CheckBox^  _cbFixedAxis;
public: System::Windows::Forms::Button^  _btGE_Draw;
private: System::Windows::Forms::Label^  label12;
public: System::Windows::Forms::TextBox^  _txGEtotPM25;
public: System::Windows::Forms::TextBox^  _txGEtotSO2;
public: System::Windows::Forms::TextBox^  _txGEtotNOX;
public: System::Windows::Forms::TextBox^  _txGEtotCO;
public: System::Windows::Forms::TextBox^  _txGEtotCO2;
public: System::Windows::Forms::TextBox^  _txGEtotCH4;
public: System::Windows::Forms::TextBox^  _txGEtotPM10;
private: System::Windows::Forms::Label^  label13;
private: System::Windows::Forms::Label^  label14;
private: System::Windows::Forms::Label^  label15; 
private: System::Windows::Forms::Label^  label16;
public: System::Windows::Forms::ContextMenuStrip^  _cmenuGraph;
public: System::Windows::Forms::ToolStripMenuItem^  Print;
public: System::Windows::Forms::ToolStripMenuItem^  PrintPreview;
public: System::Windows::Forms::ToolStripMenuItem^  PrintPageSetup;
public: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  saveToClipboardToolStripMenuItem;
public: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
public: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem1;
public: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem2;
public: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem3;
public: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem4;
private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem5;
private: System::Windows::Forms::Label^  label17;
public: System::Windows::Forms::Label^  label18;
private: System::Windows::Forms::ToolTip^  toolTip1;
 
private: System::Windows::Forms::Label^  label11;
  
/*-----------------------------------------------------------------*/
 
    int Set_XAxis();
    int Set_YAxis (); 

    int Update (d_CO *a_CO);
   int Clear (); 
   void Set_FuelCalc(d_CO *a_CO);
   int DrawGraph ();
   void SeriesOnOff (); 
   void SetEmiTot (); 
//   void Set_UserAxis (); 
   float  Max_Emis (); 
   int FixedAxisButtonChanged();
   void Axis_Set (int X, int Y);
   void Axis_Mngr ();  
   int  Axis_GetUser (int *X, int *Y);
   void Init_Emis (); 

/*-------------------------------------------------------*/
Grap_Emis(void)
{
   InitializeComponent();
   Init_Emis (); 
}

/*------------------------------------------------------*/
 ~Grap_Emis()
{
  if (components)
    delete components;

  delete [] fr_PM25 ;
  delete [] fr_PM10 ;
  delete [] fr_CH4  ;
  delete [] fr_CO2  ;
  delete [] fr_CO   ;
  delete [] fr_NOX  ;
  delete  [] fr_SO2  ;
  delete  [] fr_Int  ;
}

/*-----------------------------------------------------------*/
  System::Windows::Forms::CheckBox^  _cbGE_PM25;
  System::Windows::Forms::CheckBox^  _cbGE_PM10;
  System::Windows::Forms::CheckBox^  _cbGE_CH4;
  System::Windows::Forms::CheckBox^  _cbGE_CO2;
  System::Windows::Forms::CheckBox^  _cbGE_CO;
  System::Windows::Forms::CheckBox^  _cbGE_NOX;
  System::Windows::Forms::CheckBox^  _cbGE_SO2;
  System::Windows::Forms::DataVisualization::Charting::Chart^  _chGrap_Emis;

private: System::Windows::Forms::CheckBox^  _cbGE_Intensity;
public: System::Windows::Forms::NumericUpDown^  _udRotation;
private: System::Windows::Forms::NumericUpDown^  _upInclination;
private: System::Windows::Forms::TextBox^  _txGEflPM25;
private: System::Windows::Forms::TextBox^  _txGEflCO2;
private: System::Windows::Forms::TextBox^  _txGEflCO;
private: System::Windows::Forms::TextBox^  _txGEflNOX;
private: System::Windows::Forms::TextBox^  _txGEflSO2;
private: System::Windows::Forms::TextBox^  _txGEflCH4;
private: System::Windows::Forms::TextBox^  _txGEflPM10;
private: System::Windows::Forms::TextBox^  _txGEsmPM25;
private: System::Windows::Forms::TextBox^  _txGEsmPM10;
private: System::Windows::Forms::TextBox^  _txGEsmCH4;
private: System::Windows::Forms::TextBox^  _txGEsmCO2;
private: System::Windows::Forms::TextBox^  _txGEsmCO;
private: System::Windows::Forms::TextBox^  _txGEsmNOX;
private: System::Windows::Forms::TextBox^  _txGEsmSO2;
private: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::Label^  label2;
private: System::Windows::Forms::Label^  label3;
private: System::Windows::Forms::Label^  label4;
private: System::Windows::Forms::Label^  label5;
private: System::Windows::Forms::Label^  label6;
private: System::Windows::Forms::Label^  label7;
private: System::Windows::Forms::Label^  label8;
private: System::Windows::Forms::Label^  label9;
private: System::ComponentModel::IContainer^  components;
public: 

public: 

public: 

public: 

public: 

public: 



#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->components = (gcnew System::ComponentModel::Container());
            System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea7 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
            System::Windows::Forms::DataVisualization::Charting::Legend^  legend7 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
            System::Windows::Forms::DataVisualization::Charting::Series^  series49 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series50 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series51 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series52 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series53 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series54 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series55 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series56 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Title^  title7 = (gcnew System::Windows::Forms::DataVisualization::Charting::Title());
            this->_cbGE_CO2 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbGE_PM10 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbGE_NOX = (gcnew System::Windows::Forms::CheckBox());
            this->_cbGE_SO2 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbGE_CO = (gcnew System::Windows::Forms::CheckBox());
            this->_cbGE_PM25 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbGE_CH4 = (gcnew System::Windows::Forms::CheckBox());
            this->_chGrap_Emis = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
            this->_cbGE_Intensity = (gcnew System::Windows::Forms::CheckBox());
            this->_udRotation = (gcnew System::Windows::Forms::NumericUpDown());
            this->_upInclination = (gcnew System::Windows::Forms::NumericUpDown());
            this->_txGEflPM25 = (gcnew System::Windows::Forms::TextBox());
            this->_txGEflCO2 = (gcnew System::Windows::Forms::TextBox());
            this->_txGEflCO = (gcnew System::Windows::Forms::TextBox());
            this->_txGEflNOX = (gcnew System::Windows::Forms::TextBox());
            this->_txGEflSO2 = (gcnew System::Windows::Forms::TextBox());
            this->_txGEflCH4 = (gcnew System::Windows::Forms::TextBox());
            this->_txGEflPM10 = (gcnew System::Windows::Forms::TextBox());
            this->_txGEsmPM25 = (gcnew System::Windows::Forms::TextBox());
            this->_txGEsmPM10 = (gcnew System::Windows::Forms::TextBox());
            this->_txGEsmCH4 = (gcnew System::Windows::Forms::TextBox());
            this->_txGEsmCO2 = (gcnew System::Windows::Forms::TextBox());
            this->_txGEsmCO = (gcnew System::Windows::Forms::TextBox());
            this->_txGEsmNOX = (gcnew System::Windows::Forms::TextBox());
            this->_txGEsmSO2 = (gcnew System::Windows::Forms::TextBox());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->label4 = (gcnew System::Windows::Forms::Label());
            this->label5 = (gcnew System::Windows::Forms::Label());
            this->label6 = (gcnew System::Windows::Forms::Label());
            this->label7 = (gcnew System::Windows::Forms::Label());
            this->label8 = (gcnew System::Windows::Forms::Label());
            this->label9 = (gcnew System::Windows::Forms::Label());
            this->_txXAxis = (gcnew System::Windows::Forms::TextBox());
            this->_txYAxis = (gcnew System::Windows::Forms::TextBox());
            this->label10 = (gcnew System::Windows::Forms::Label());
            this->label11 = (gcnew System::Windows::Forms::Label());
            this->_cbFixedAxis = (gcnew System::Windows::Forms::CheckBox());
            this->_btGE_Draw = (gcnew System::Windows::Forms::Button());
            this->label12 = (gcnew System::Windows::Forms::Label());
            this->_txGEtotPM25 = (gcnew System::Windows::Forms::TextBox());
            this->_txGEtotSO2 = (gcnew System::Windows::Forms::TextBox());
            this->_txGEtotNOX = (gcnew System::Windows::Forms::TextBox());
            this->_txGEtotCO = (gcnew System::Windows::Forms::TextBox());
            this->_txGEtotCO2 = (gcnew System::Windows::Forms::TextBox());
            this->_txGEtotCH4 = (gcnew System::Windows::Forms::TextBox());
            this->_txGEtotPM10 = (gcnew System::Windows::Forms::TextBox());
            this->label13 = (gcnew System::Windows::Forms::Label());
            this->label14 = (gcnew System::Windows::Forms::Label());
            this->label15 = (gcnew System::Windows::Forms::Label());
            this->label16 = (gcnew System::Windows::Forms::Label());
            this->_cmenuGraph = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->Print = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->PrintPreview = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->PrintPageSetup = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->saveToClipboardToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripMenuItem3 = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripMenuItem4 = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripMenuItem5 = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->label17 = (gcnew System::Windows::Forms::Label());
            this->label18 = (gcnew System::Windows::Forms::Label());
            this->toolTip1 = (gcnew System::Windows::Forms::ToolTip(this->components));
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_chGrap_Emis))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_udRotation))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_upInclination))->BeginInit();
            this->_cmenuGraph->SuspendLayout();
            this->contextMenuStrip1->SuspendLayout();
            this->SuspendLayout();
            // 
            // _cbGE_CO2
            // 
            this->_cbGE_CO2->AutoSize = true;
            this->_cbGE_CO2->Location = System::Drawing::Point(4, 110);
            this->_cbGE_CO2->Margin = System::Windows::Forms::Padding(2);
            this->_cbGE_CO2->Name = L"_cbGE_CO2";
            this->_cbGE_CO2->Size = System::Drawing::Size(47, 17);
            this->_cbGE_CO2->TabIndex = 0;
            this->_cbGE_CO2->Text = L"CO2";
            this->_cbGE_CO2->UseVisualStyleBackColor = true;
            this->_cbGE_CO2->CheckedChanged += gcnew System::EventHandler(this, &Grap_Emis::_cbGE_CO2_CheckedChanged);
            // 
            // _cbGE_PM10
            // 
            this->_cbGE_PM10->AutoSize = true;
            this->_cbGE_PM10->Location = System::Drawing::Point(4, 37);
            this->_cbGE_PM10->Margin = System::Windows::Forms::Padding(2);
            this->_cbGE_PM10->Name = L"_cbGE_PM10";
            this->_cbGE_PM10->Size = System::Drawing::Size(57, 17);
            this->_cbGE_PM10->TabIndex = 1;
            this->_cbGE_PM10->Text = L"PM 10";
            this->_cbGE_PM10->UseVisualStyleBackColor = true;
            this->_cbGE_PM10->CheckedChanged += gcnew System::EventHandler(this, &Grap_Emis::_cbGE_PM10_CheckedChanged);
            // 
            // _cbGE_NOX
            // 
            this->_cbGE_NOX->AutoSize = true;
            this->_cbGE_NOX->Location = System::Drawing::Point(4, 133);
            this->_cbGE_NOX->Margin = System::Windows::Forms::Padding(2);
            this->_cbGE_NOX->Name = L"_cbGE_NOX";
            this->_cbGE_NOX->Size = System::Drawing::Size(49, 17);
            this->_cbGE_NOX->TabIndex = 2;
            this->_cbGE_NOX->Text = L"NOX";
            this->_cbGE_NOX->UseVisualStyleBackColor = true;
            this->_cbGE_NOX->CheckedChanged += gcnew System::EventHandler(this, &Grap_Emis::_cbGE_NOX_CheckedChanged);
            // 
            // _cbGE_SO2
            // 
            this->_cbGE_SO2->AutoSize = true;
            this->_cbGE_SO2->Location = System::Drawing::Point(4, 157);
            this->_cbGE_SO2->Margin = System::Windows::Forms::Padding(2);
            this->_cbGE_SO2->Name = L"_cbGE_SO2";
            this->_cbGE_SO2->Size = System::Drawing::Size(47, 17);
            this->_cbGE_SO2->TabIndex = 3;
            this->_cbGE_SO2->Text = L"SO2";
            this->_cbGE_SO2->UseVisualStyleBackColor = true;
            this->_cbGE_SO2->CheckedChanged += gcnew System::EventHandler(this, &Grap_Emis::_cbGE_SO2_CheckedChanged);
            // 
            // _cbGE_CO
            // 
            this->_cbGE_CO->AutoSize = true;
            this->_cbGE_CO->Location = System::Drawing::Point(4, 86);
            this->_cbGE_CO->Margin = System::Windows::Forms::Padding(2);
            this->_cbGE_CO->Name = L"_cbGE_CO";
            this->_cbGE_CO->Size = System::Drawing::Size(41, 17);
            this->_cbGE_CO->TabIndex = 4;
            this->_cbGE_CO->Text = L"CO";
            this->_cbGE_CO->UseVisualStyleBackColor = true;
            this->_cbGE_CO->CheckedChanged += gcnew System::EventHandler(this, &Grap_Emis::_cbGE_CO_CheckedChanged);
            // 
            // _cbGE_PM25
            // 
            this->_cbGE_PM25->AutoSize = true;
            this->_cbGE_PM25->Checked = true;
            this->_cbGE_PM25->CheckState = System::Windows::Forms::CheckState::Checked;
            this->_cbGE_PM25->Location = System::Drawing::Point(4, 10);
            this->_cbGE_PM25->Margin = System::Windows::Forms::Padding(2);
            this->_cbGE_PM25->Name = L"_cbGE_PM25";
            this->_cbGE_PM25->Size = System::Drawing::Size(60, 17);
            this->_cbGE_PM25->TabIndex = 5;
            this->_cbGE_PM25->Text = L"PM 2.5";
            this->_cbGE_PM25->UseVisualStyleBackColor = true;
            this->_cbGE_PM25->CheckedChanged += gcnew System::EventHandler(this, &Grap_Emis::_cbGE_PM25_CheckedChanged);
            // 
            // _cbGE_CH4
            // 
            this->_cbGE_CH4->AutoSize = true;
            this->_cbGE_CH4->Location = System::Drawing::Point(4, 63);
            this->_cbGE_CH4->Margin = System::Windows::Forms::Padding(2);
            this->_cbGE_CH4->Name = L"_cbGE_CH4";
            this->_cbGE_CH4->Size = System::Drawing::Size(47, 17);
            this->_cbGE_CH4->TabIndex = 6;
            this->_cbGE_CH4->Text = L"CH4";
            this->_cbGE_CH4->UseVisualStyleBackColor = true;
            this->_cbGE_CH4->CheckedChanged += gcnew System::EventHandler(this, &Grap_Emis::_cbGE_CH4_CheckedChanged);
            // 
            // _chGrap_Emis
            // 
            this->_chGrap_Emis->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
                | System::Windows::Forms::AnchorStyles::Left) 
                | System::Windows::Forms::AnchorStyles::Right));
            this->_chGrap_Emis->BackGradientStyle = System::Windows::Forms::DataVisualization::Charting::GradientStyle::DiagonalRight;
            this->_chGrap_Emis->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
            this->_chGrap_Emis->BackImageTransparentColor = System::Drawing::Color::White;
            this->_chGrap_Emis->BorderlineColor = System::Drawing::Color::DimGray;
            this->_chGrap_Emis->BorderlineWidth = 4;
            this->_chGrap_Emis->BorderSkin->BackHatchStyle = System::Windows::Forms::DataVisualization::Charting::ChartHatchStyle::Cross;
            chartArea7->Area3DStyle->Enable3D = true;
            chartArea7->Area3DStyle->Inclination = 0;
            chartArea7->Area3DStyle->IsRightAngleAxes = false;
            chartArea7->Area3DStyle->PointDepth = 300;
            chartArea7->Area3DStyle->PointGapDepth = 200;
            chartArea7->Area3DStyle->Rotation = 0;
            chartArea7->Area3DStyle->WallWidth = 3;
            chartArea7->AxisX->Title = L"Burn Time - min";
            chartArea7->AxisY->Title = L"Emissions - lb/ac/sec";
            chartArea7->BackColor = System::Drawing::Color::Linen;
            chartArea7->BackHatchStyle = System::Windows::Forms::DataVisualization::Charting::ChartHatchStyle::Divot;
            chartArea7->BackImageTransparentColor = System::Drawing::Color::White;
            chartArea7->BackSecondaryColor = System::Drawing::Color::White;
            chartArea7->BorderWidth = 4;
            chartArea7->Name = L"ChartArea1";
            this->_chGrap_Emis->ChartAreas->Add(chartArea7);
            legend7->Name = L"Legend1";
            legend7->ShadowColor = System::Drawing::Color::LightGray;
            this->_chGrap_Emis->Legends->Add(legend7);
            this->_chGrap_Emis->Location = System::Drawing::Point(65, 10);
            this->_chGrap_Emis->Margin = System::Windows::Forms::Padding(2);
            this->_chGrap_Emis->Name = L"_chGrap_Emis";
            series49->ChartArea = L"ChartArea1";
            series49->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
            series49->Color = System::Drawing::Color::Red;
            series49->Legend = L"Legend1";
            series49->LegendText = L"Fire Int.";
            series49->Name = L"Intensity";
            series50->ChartArea = L"ChartArea1";
            series50->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Area;
            series50->Legend = L"Legend1";
            series50->LegendText = L"CO2";
            series50->Name = L"CO2";
            series51->ChartArea = L"ChartArea1";
            series51->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Area;
            series51->Legend = L"Legend1";
            series51->LegendText = L"CO";
            series51->Name = L"CO";
            series52->ChartArea = L"ChartArea1";
            series52->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Area;
            series52->Legend = L"Legend1";
            series52->LegendText = L"PM 10";
            series52->Name = L"PM10";
            series53->ChartArea = L"ChartArea1";
            series53->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Area;
            series53->Legend = L"Legend1";
            series53->LegendText = L"PM 2.5";
            series53->Name = L"PM25";
            series54->ChartArea = L"ChartArea1";
            series54->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Area;
            series54->Legend = L"Legend1";
            series54->LegendText = L"CH4";
            series54->Name = L"CH4";
            series55->ChartArea = L"ChartArea1";
            series55->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Area;
            series55->Legend = L"Legend1";
            series55->LegendText = L"NOX";
            series55->Name = L"NOX";
            series56->ChartArea = L"ChartArea1";
            series56->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Area;
            series56->Legend = L"Legend1";
            series56->LegendText = L"SO2";
            series56->Name = L"SO2";
            this->_chGrap_Emis->Series->Add(series49);
            this->_chGrap_Emis->Series->Add(series50);
            this->_chGrap_Emis->Series->Add(series51);
            this->_chGrap_Emis->Series->Add(series52);
            this->_chGrap_Emis->Series->Add(series53);
            this->_chGrap_Emis->Series->Add(series54);
            this->_chGrap_Emis->Series->Add(series55);
            this->_chGrap_Emis->Series->Add(series56);
            this->_chGrap_Emis->Size = System::Drawing::Size(686, 270);
            this->_chGrap_Emis->TabIndex = 7;
            this->_chGrap_Emis->Text = L"chart1";
            title7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            title7->Name = L"Title1";
            title7->Text = L"Smoke Emissions";
            this->_chGrap_Emis->Titles->Add(title7);
            this->_chGrap_Emis->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &Grap_Emis::_chGrap_Emis_MouseClick);
            this->_chGrap_Emis->Click += gcnew System::EventHandler(this, &Grap_Emis::_chGrap_Emis_Click);
            // 
            // _cbGE_Intensity
            // 
            this->_cbGE_Intensity->AutoSize = true;
            this->_cbGE_Intensity->Location = System::Drawing::Point(6, 249);
            this->_cbGE_Intensity->Margin = System::Windows::Forms::Padding(2);
            this->_cbGE_Intensity->Name = L"_cbGE_Intensity";
            this->_cbGE_Intensity->Size = System::Drawing::Size(15, 14);
            this->_cbGE_Intensity->TabIndex = 9;
            this->_cbGE_Intensity->UseVisualStyleBackColor = true;
            this->_cbGE_Intensity->Visible = false;
            this->_cbGE_Intensity->CheckedChanged += gcnew System::EventHandler(this, &Grap_Emis::_cbGE_Intensity_CheckedChanged);
            // 
            // _udRotation
            // 
            this->_udRotation->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_udRotation->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 0});
            this->_udRotation->Location = System::Drawing::Point(597, 371);
            this->_udRotation->Margin = System::Windows::Forms::Padding(2);
            this->_udRotation->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {200, 0, 0, 0});
            this->_udRotation->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {200, 0, 0, System::Int32::MinValue});
            this->_udRotation->Name = L"_udRotation";
            this->_udRotation->Size = System::Drawing::Size(49, 20);
            this->_udRotation->TabIndex = 13;
            this->_udRotation->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->_udRotation->ValueChanged += gcnew System::EventHandler(this, &Grap_Emis::numericUpDown1_ValueChanged);
            // 
            // _upInclination
            // 
            this->_upInclination->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_upInclination->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 0});
            this->_upInclination->Location = System::Drawing::Point(650, 371);
            this->_upInclination->Margin = System::Windows::Forms::Padding(2);
            this->_upInclination->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {185, 0, 0, 0});
            this->_upInclination->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {185, 0, 0, System::Int32::MinValue});
            this->_upInclination->Name = L"_upInclination";
            this->_upInclination->Size = System::Drawing::Size(49, 20);
            this->_upInclination->TabIndex = 14;
            this->_upInclination->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->_upInclination->ValueChanged += gcnew System::EventHandler(this, &Grap_Emis::_upInclination_ValueChanged);
            // 
            // _txGEflPM25
            // 
            this->_txGEflPM25->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_txGEflPM25->BackColor = System::Drawing::SystemColors::InactiveBorder;
            this->_txGEflPM25->Location = System::Drawing::Point(77, 327);
            this->_txGEflPM25->Margin = System::Windows::Forms::Padding(2);
            this->_txGEflPM25->Name = L"_txGEflPM25";
            this->_txGEflPM25->ReadOnly = true;
            this->_txGEflPM25->Size = System::Drawing::Size(53, 20);
            this->_txGEflPM25->TabIndex = 15;
            this->_txGEflPM25->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->_txGEflPM25->TextChanged += gcnew System::EventHandler(this, &Grap_Emis::textBox1_TextChanged);
            // 
            // _txGEflCO2
            // 
            this->_txGEflCO2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_txGEflCO2->BackColor = System::Drawing::SystemColors::InactiveBorder;
            this->_txGEflCO2->Location = System::Drawing::Point(317, 327);
            this->_txGEflCO2->Margin = System::Windows::Forms::Padding(2);
            this->_txGEflCO2->Name = L"_txGEflCO2";
            this->_txGEflCO2->ReadOnly = true;
            this->_txGEflCO2->Size = System::Drawing::Size(53, 20);
            this->_txGEflCO2->TabIndex = 16;
            this->_txGEflCO2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txGEflCO
            // 
            this->_txGEflCO->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_txGEflCO->BackColor = System::Drawing::SystemColors::InactiveBorder;
            this->_txGEflCO->Location = System::Drawing::Point(257, 327);
            this->_txGEflCO->Margin = System::Windows::Forms::Padding(2);
            this->_txGEflCO->Name = L"_txGEflCO";
            this->_txGEflCO->ReadOnly = true;
            this->_txGEflCO->Size = System::Drawing::Size(53, 20);
            this->_txGEflCO->TabIndex = 17;
            this->_txGEflCO->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->_txGEflCO->TextChanged += gcnew System::EventHandler(this, &Grap_Emis::textBox3_TextChanged);
            // 
            // _txGEflNOX
            // 
            this->_txGEflNOX->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_txGEflNOX->BackColor = System::Drawing::SystemColors::InactiveBorder;
            this->_txGEflNOX->Location = System::Drawing::Point(377, 327);
            this->_txGEflNOX->Margin = System::Windows::Forms::Padding(2);
            this->_txGEflNOX->Name = L"_txGEflNOX";
            this->_txGEflNOX->ReadOnly = true;
            this->_txGEflNOX->Size = System::Drawing::Size(53, 20);
            this->_txGEflNOX->TabIndex = 18;
            this->_txGEflNOX->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txGEflSO2
            // 
            this->_txGEflSO2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_txGEflSO2->BackColor = System::Drawing::SystemColors::InactiveBorder;
            this->_txGEflSO2->Location = System::Drawing::Point(437, 327);
            this->_txGEflSO2->Margin = System::Windows::Forms::Padding(2);
            this->_txGEflSO2->Name = L"_txGEflSO2";
            this->_txGEflSO2->ReadOnly = true;
            this->_txGEflSO2->Size = System::Drawing::Size(53, 20);
            this->_txGEflSO2->TabIndex = 19;
            this->_txGEflSO2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txGEflCH4
            // 
            this->_txGEflCH4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_txGEflCH4->BackColor = System::Drawing::SystemColors::InactiveBorder;
            this->_txGEflCH4->Location = System::Drawing::Point(197, 327);
            this->_txGEflCH4->Margin = System::Windows::Forms::Padding(2);
            this->_txGEflCH4->Name = L"_txGEflCH4";
            this->_txGEflCH4->ReadOnly = true;
            this->_txGEflCH4->Size = System::Drawing::Size(53, 20);
            this->_txGEflCH4->TabIndex = 20;
            this->_txGEflCH4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txGEflPM10
            // 
            this->_txGEflPM10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_txGEflPM10->BackColor = System::Drawing::SystemColors::InactiveBorder;
            this->_txGEflPM10->Location = System::Drawing::Point(137, 327);
            this->_txGEflPM10->Margin = System::Windows::Forms::Padding(2);
            this->_txGEflPM10->Name = L"_txGEflPM10";
            this->_txGEflPM10->ReadOnly = true;
            this->_txGEflPM10->Size = System::Drawing::Size(53, 20);
            this->_txGEflPM10->TabIndex = 21;
            this->_txGEflPM10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txGEsmPM25
            // 
            this->_txGEsmPM25->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_txGEsmPM25->BackColor = System::Drawing::SystemColors::InactiveBorder;
            this->_txGEsmPM25->Location = System::Drawing::Point(77, 357);
            this->_txGEsmPM25->Margin = System::Windows::Forms::Padding(2);
            this->_txGEsmPM25->Name = L"_txGEsmPM25";
            this->_txGEsmPM25->ReadOnly = true;
            this->_txGEsmPM25->Size = System::Drawing::Size(53, 20);
            this->_txGEsmPM25->TabIndex = 22;
            this->_txGEsmPM25->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txGEsmPM10
            // 
            this->_txGEsmPM10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_txGEsmPM10->BackColor = System::Drawing::SystemColors::InactiveBorder;
            this->_txGEsmPM10->Location = System::Drawing::Point(137, 357);
            this->_txGEsmPM10->Margin = System::Windows::Forms::Padding(2);
            this->_txGEsmPM10->Name = L"_txGEsmPM10";
            this->_txGEsmPM10->ReadOnly = true;
            this->_txGEsmPM10->Size = System::Drawing::Size(53, 20);
            this->_txGEsmPM10->TabIndex = 23;
            this->_txGEsmPM10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txGEsmCH4
            // 
            this->_txGEsmCH4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_txGEsmCH4->BackColor = System::Drawing::SystemColors::InactiveBorder;
            this->_txGEsmCH4->Location = System::Drawing::Point(197, 357);
            this->_txGEsmCH4->Margin = System::Windows::Forms::Padding(2);
            this->_txGEsmCH4->Name = L"_txGEsmCH4";
            this->_txGEsmCH4->ReadOnly = true;
            this->_txGEsmCH4->Size = System::Drawing::Size(53, 20);
            this->_txGEsmCH4->TabIndex = 24;
            this->_txGEsmCH4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txGEsmCO2
            // 
            this->_txGEsmCO2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_txGEsmCO2->BackColor = System::Drawing::SystemColors::InactiveBorder;
            this->_txGEsmCO2->Location = System::Drawing::Point(317, 357);
            this->_txGEsmCO2->Margin = System::Windows::Forms::Padding(2);
            this->_txGEsmCO2->Name = L"_txGEsmCO2";
            this->_txGEsmCO2->ReadOnly = true;
            this->_txGEsmCO2->Size = System::Drawing::Size(53, 20);
            this->_txGEsmCO2->TabIndex = 25;
            this->_txGEsmCO2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txGEsmCO
            // 
            this->_txGEsmCO->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_txGEsmCO->BackColor = System::Drawing::SystemColors::InactiveBorder;
            this->_txGEsmCO->Location = System::Drawing::Point(257, 357);
            this->_txGEsmCO->Margin = System::Windows::Forms::Padding(2);
            this->_txGEsmCO->Name = L"_txGEsmCO";
            this->_txGEsmCO->ReadOnly = true;
            this->_txGEsmCO->Size = System::Drawing::Size(53, 20);
            this->_txGEsmCO->TabIndex = 26;
            this->_txGEsmCO->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txGEsmNOX
            // 
            this->_txGEsmNOX->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_txGEsmNOX->BackColor = System::Drawing::SystemColors::InactiveBorder;
            this->_txGEsmNOX->Location = System::Drawing::Point(377, 357);
            this->_txGEsmNOX->Margin = System::Windows::Forms::Padding(2);
            this->_txGEsmNOX->Name = L"_txGEsmNOX";
            this->_txGEsmNOX->ReadOnly = true;
            this->_txGEsmNOX->Size = System::Drawing::Size(53, 20);
            this->_txGEsmNOX->TabIndex = 27;
            this->_txGEsmNOX->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txGEsmSO2
            // 
            this->_txGEsmSO2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_txGEsmSO2->BackColor = System::Drawing::SystemColors::InactiveBorder;
            this->_txGEsmSO2->Location = System::Drawing::Point(437, 357);
            this->_txGEsmSO2->Margin = System::Windows::Forms::Padding(2);
            this->_txGEsmSO2->Name = L"_txGEsmSO2";
            this->_txGEsmSO2->ReadOnly = true;
            this->_txGEsmSO2->Size = System::Drawing::Size(53, 20);
            this->_txGEsmSO2->TabIndex = 28;
            this->_txGEsmSO2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // label1
            // 
            this->label1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(25, 331);
            this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(43, 13);
            this->label1->TabIndex = 29;
            this->label1->Text = L"Flaming";
            // 
            // label2
            // 
            this->label2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(23, 360);
            this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(45, 13);
            this->label2->TabIndex = 30;
            this->label2->Text = L"Smolder";
            // 
            // label3
            // 
            this->label3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(83, 311);
            this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(41, 13);
            this->label3->TabIndex = 31;
            this->label3->Text = L"PM 2.5";
            // 
            // label4
            // 
            this->label4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->label4->AutoSize = true;
            this->label4->Location = System::Drawing::Point(145, 311);
            this->label4->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
            this->label4->Name = L"label4";
            this->label4->Size = System::Drawing::Size(38, 13);
            this->label4->TabIndex = 32;
            this->label4->Text = L"PM 10";
            // 
            // label5
            // 
            this->label5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->label5->AutoSize = true;
            this->label5->Location = System::Drawing::Point(209, 311);
            this->label5->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
            this->label5->Name = L"label5";
            this->label5->Size = System::Drawing::Size(28, 13);
            this->label5->TabIndex = 33;
            this->label5->Text = L"CH4";
            // 
            // label6
            // 
            this->label6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->label6->AutoSize = true;
            this->label6->Location = System::Drawing::Point(334, 311);
            this->label6->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
            this->label6->Name = L"label6";
            this->label6->Size = System::Drawing::Size(28, 13);
            this->label6->TabIndex = 34;
            this->label6->Text = L"CO2";
            // 
            // label7
            // 
            this->label7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->label7->AutoSize = true;
            this->label7->Location = System::Drawing::Point(271, 311);
            this->label7->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
            this->label7->Name = L"label7";
            this->label7->Size = System::Drawing::Size(22, 13);
            this->label7->TabIndex = 35;
            this->label7->Text = L"CO";
            // 
            // label8
            // 
            this->label8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->label8->AutoSize = true;
            this->label8->Location = System::Drawing::Point(387, 311);
            this->label8->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
            this->label8->Name = L"label8";
            this->label8->Size = System::Drawing::Size(30, 13);
            this->label8->TabIndex = 36;
            this->label8->Text = L"NOX";
            // 
            // label9
            // 
            this->label9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->label9->AutoSize = true;
            this->label9->Location = System::Drawing::Point(447, 311);
            this->label9->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
            this->label9->Name = L"label9";
            this->label9->Size = System::Drawing::Size(28, 13);
            this->label9->TabIndex = 37;
            this->label9->Text = L"SO2";
            // 
            // _txXAxis
            // 
            this->_txXAxis->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_txXAxis->Location = System::Drawing::Point(613, 331);
            this->_txXAxis->Margin = System::Windows::Forms::Padding(2);
            this->_txXAxis->Name = L"_txXAxis";
            this->_txXAxis->Size = System::Drawing::Size(32, 20);
            this->_txXAxis->TabIndex = 38;
            this->_txXAxis->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->_txXAxis->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Grap_Emis::_txXAxis_KeyDown);
            // 
            // _txYAxis
            // 
            this->_txYAxis->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_txYAxis->Location = System::Drawing::Point(664, 331);
            this->_txYAxis->Margin = System::Windows::Forms::Padding(2);
            this->_txYAxis->Name = L"_txYAxis";
            this->_txYAxis->Size = System::Drawing::Size(32, 20);
            this->_txYAxis->TabIndex = 39;
            this->_txYAxis->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->_txYAxis->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Grap_Emis::_txYAxis_KeyDown);
            // 
            // label10
            // 
            this->label10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->label10->AutoSize = true;
            this->label10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label10->Location = System::Drawing::Point(676, 310);
            this->label10->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
            this->label10->Name = L"label10";
            this->label10->Size = System::Drawing::Size(14, 15);
            this->label10->TabIndex = 40;
            this->label10->Text = L"Y";
            // 
            // label11
            // 
            this->label11->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->label11->AutoSize = true;
            this->label11->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label11->Location = System::Drawing::Point(622, 310);
            this->label11->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
            this->label11->Name = L"label11";
            this->label11->Size = System::Drawing::Size(15, 15);
            this->label11->TabIndex = 41;
            this->label11->Text = L"X";
            // 
            // _cbFixedAxis
            // 
            this->_cbFixedAxis->AutoSize = true;
            this->_cbFixedAxis->Location = System::Drawing::Point(718, 390);
            this->_cbFixedAxis->Margin = System::Windows::Forms::Padding(2);
            this->_cbFixedAxis->Name = L"_cbFixedAxis";
            this->_cbFixedAxis->Size = System::Drawing::Size(73, 17);
            this->_cbFixedAxis->TabIndex = 42;
            this->_cbFixedAxis->Text = L"Fixed Axis";
            this->_cbFixedAxis->UseVisualStyleBackColor = true;
            this->_cbFixedAxis->Visible = false;
            this->_cbFixedAxis->CheckedChanged += gcnew System::EventHandler(this, &Grap_Emis::_cbFixedAxis_CheckedChanged);
            // 
            // _btGE_Draw
            // 
            this->_btGE_Draw->BackColor = System::Drawing::Color::Honeydew;
            this->_btGE_Draw->Location = System::Drawing::Point(718, 297);
            this->_btGE_Draw->Margin = System::Windows::Forms::Padding(2);
            this->_btGE_Draw->Name = L"_btGE_Draw";
            this->_btGE_Draw->Size = System::Drawing::Size(39, 19);
            this->_btGE_Draw->TabIndex = 12;
            this->_btGE_Draw->Text = L"Refresh";
            this->_btGE_Draw->UseVisualStyleBackColor = false;
            this->_btGE_Draw->Visible = false;
            this->_btGE_Draw->Click += gcnew System::EventHandler(this, &Grap_Emis::_btGE_Draw_Click);
            // 
            // label12
            // 
            this->label12->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->label12->AutoSize = true;
            this->label12->Location = System::Drawing::Point(522, 376);
            this->label12->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
            this->label12->Name = L"label12";
            this->label12->Size = System::Drawing::Size(61, 13);
            this->label12->TabIndex = 43;
            this->label12->Text = L"Rotate Axis";
            // 
            // _txGEtotPM25
            // 
            this->_txGEtotPM25->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_txGEtotPM25->BackColor = System::Drawing::SystemColors::InactiveBorder;
            this->_txGEtotPM25->Location = System::Drawing::Point(77, 387);
            this->_txGEtotPM25->Margin = System::Windows::Forms::Padding(2);
            this->_txGEtotPM25->Name = L"_txGEtotPM25";
            this->_txGEtotPM25->ReadOnly = true;
            this->_txGEtotPM25->Size = System::Drawing::Size(53, 20);
            this->_txGEtotPM25->TabIndex = 44;
            this->_txGEtotPM25->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txGEtotSO2
            // 
            this->_txGEtotSO2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_txGEtotSO2->BackColor = System::Drawing::SystemColors::InactiveBorder;
            this->_txGEtotSO2->Location = System::Drawing::Point(437, 387);
            this->_txGEtotSO2->Margin = System::Windows::Forms::Padding(2);
            this->_txGEtotSO2->Name = L"_txGEtotSO2";
            this->_txGEtotSO2->ReadOnly = true;
            this->_txGEtotSO2->Size = System::Drawing::Size(53, 20);
            this->_txGEtotSO2->TabIndex = 45;
            this->_txGEtotSO2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txGEtotNOX
            // 
            this->_txGEtotNOX->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_txGEtotNOX->BackColor = System::Drawing::SystemColors::InactiveBorder;
            this->_txGEtotNOX->Location = System::Drawing::Point(377, 387);
            this->_txGEtotNOX->Margin = System::Windows::Forms::Padding(2);
            this->_txGEtotNOX->Name = L"_txGEtotNOX";
            this->_txGEtotNOX->ReadOnly = true;
            this->_txGEtotNOX->Size = System::Drawing::Size(53, 20);
            this->_txGEtotNOX->TabIndex = 46;
            this->_txGEtotNOX->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txGEtotCO
            // 
            this->_txGEtotCO->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_txGEtotCO->BackColor = System::Drawing::SystemColors::InactiveBorder;
            this->_txGEtotCO->Location = System::Drawing::Point(257, 387);
            this->_txGEtotCO->Margin = System::Windows::Forms::Padding(2);
            this->_txGEtotCO->Name = L"_txGEtotCO";
            this->_txGEtotCO->ReadOnly = true;
            this->_txGEtotCO->Size = System::Drawing::Size(53, 20);
            this->_txGEtotCO->TabIndex = 47;
            this->_txGEtotCO->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txGEtotCO2
            // 
            this->_txGEtotCO2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_txGEtotCO2->BackColor = System::Drawing::SystemColors::InactiveBorder;
            this->_txGEtotCO2->Location = System::Drawing::Point(317, 387);
            this->_txGEtotCO2->Margin = System::Windows::Forms::Padding(2);
            this->_txGEtotCO2->Name = L"_txGEtotCO2";
            this->_txGEtotCO2->ReadOnly = true;
            this->_txGEtotCO2->Size = System::Drawing::Size(53, 20);
            this->_txGEtotCO2->TabIndex = 48;
            this->_txGEtotCO2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txGEtotCH4
            // 
            this->_txGEtotCH4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_txGEtotCH4->BackColor = System::Drawing::SystemColors::InactiveBorder;
            this->_txGEtotCH4->Location = System::Drawing::Point(197, 387);
            this->_txGEtotCH4->Margin = System::Windows::Forms::Padding(2);
            this->_txGEtotCH4->Name = L"_txGEtotCH4";
            this->_txGEtotCH4->ReadOnly = true;
            this->_txGEtotCH4->Size = System::Drawing::Size(53, 20);
            this->_txGEtotCH4->TabIndex = 49;
            this->_txGEtotCH4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txGEtotPM10
            // 
            this->_txGEtotPM10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_txGEtotPM10->BackColor = System::Drawing::SystemColors::InactiveBorder;
            this->_txGEtotPM10->Location = System::Drawing::Point(137, 387);
            this->_txGEtotPM10->Margin = System::Windows::Forms::Padding(2);
            this->_txGEtotPM10->Name = L"_txGEtotPM10";
            this->_txGEtotPM10->ReadOnly = true;
            this->_txGEtotPM10->Size = System::Drawing::Size(53, 20);
            this->_txGEtotPM10->TabIndex = 50;
            this->_txGEtotPM10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // label13
            // 
            this->label13->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->label13->AutoSize = true;
            this->label13->Location = System::Drawing::Point(24, 388);
            this->label13->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
            this->label13->Name = L"label13";
            this->label13->Size = System::Drawing::Size(31, 13);
            this->label13->TabIndex = 51;
            this->label13->Text = L"Total";
            // 
            // label14
            // 
            this->label14->AutoSize = true;
            this->label14->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label14->Location = System::Drawing::Point(2, 222);
            this->label14->Margin = System::Windows::Forms::Padding(0);
            this->label14->Name = L"label14";
            this->label14->Size = System::Drawing::Size(51, 15);
            this->label14->TabIndex = 52;
            this->label14->Text = L"Intensity";
            this->label14->Visible = false;
            // 
            // label15
            // 
            this->label15->AutoSize = true;
            this->label15->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label15->Location = System::Drawing::Point(25, 247);
            this->label15->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
            this->label15->Name = L"label15";
            this->label15->Size = System::Drawing::Size(28, 15);
            this->label15->TabIndex = 53;
            this->label15->Text = L"Fire";
            this->label15->Visible = false;
            // 
            // label16
            // 
            this->label16->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->label16->AutoSize = true;
            this->label16->Location = System::Drawing::Point(240, 415);
            this->label16->Name = L"label16";
            this->label16->Size = System::Drawing::Size(87, 13);
            this->label16->TabIndex = 54;
            this->label16->Text = L"Pounds Per Acre";
            // 
            // _cmenuGraph
            // 
            this->_cmenuGraph->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->Print, this->PrintPreview, 
                this->PrintPageSetup, this->saveToolStripMenuItem, this->saveToClipboardToolStripMenuItem});
            this->_cmenuGraph->Name = L"_cmenuGraph";
            this->_cmenuGraph->Size = System::Drawing::Size(173, 114);
            // 
            // Print
            // 
            this->Print->Name = L"Print";
            this->Print->Size = System::Drawing::Size(172, 22);
            this->Print->Text = L"Print";
            this->Print->Click += gcnew System::EventHandler(this, &Grap_Emis::Print_Click);
            // 
            // PrintPreview
            // 
            this->PrintPreview->Name = L"PrintPreview";
            this->PrintPreview->Size = System::Drawing::Size(172, 22);
            this->PrintPreview->Text = L"Print Preview";
            this->PrintPreview->Click += gcnew System::EventHandler(this, &Grap_Emis::Print_Click);
            // 
            // PrintPageSetup
            // 
            this->PrintPageSetup->Name = L"PrintPageSetup";
            this->PrintPageSetup->Size = System::Drawing::Size(172, 22);
            this->PrintPageSetup->Text = L"Print Page Setup";
            this->PrintPageSetup->Click += gcnew System::EventHandler(this, &Grap_Emis::Print_Click);
            // 
            // saveToolStripMenuItem
            // 
            this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
            this->saveToolStripMenuItem->Size = System::Drawing::Size(172, 22);
            this->saveToolStripMenuItem->Text = L"Save To File";
            this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &Grap_Emis::Print_Click);
            // 
            // saveToClipboardToolStripMenuItem
            // 
            this->saveToClipboardToolStripMenuItem->Name = L"saveToClipboardToolStripMenuItem";
            this->saveToClipboardToolStripMenuItem->Size = System::Drawing::Size(172, 22);
            this->saveToClipboardToolStripMenuItem->Text = L"Save To Clipboard";
            this->saveToClipboardToolStripMenuItem->Click += gcnew System::EventHandler(this, &Grap_Emis::Print_Click);
            // 
            // contextMenuStrip1
            // 
            this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->toolStripMenuItem1, 
                this->toolStripMenuItem2, this->toolStripMenuItem3, this->toolStripMenuItem4, this->toolStripMenuItem5});
            this->contextMenuStrip1->Name = L"_cmenuGraph";
            this->contextMenuStrip1->Size = System::Drawing::Size(173, 114);
            // 
            // toolStripMenuItem1
            // 
            this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
            this->toolStripMenuItem1->Size = System::Drawing::Size(172, 22);
            this->toolStripMenuItem1->Text = L"Print";
            // 
            // toolStripMenuItem2
            // 
            this->toolStripMenuItem2->Name = L"toolStripMenuItem2";
            this->toolStripMenuItem2->Size = System::Drawing::Size(172, 22);
            this->toolStripMenuItem2->Text = L"Print Preview";
            // 
            // toolStripMenuItem3
            // 
            this->toolStripMenuItem3->Name = L"toolStripMenuItem3";
            this->toolStripMenuItem3->Size = System::Drawing::Size(172, 22);
            this->toolStripMenuItem3->Text = L"Print Page Setup";
            // 
            // toolStripMenuItem4
            // 
            this->toolStripMenuItem4->Name = L"toolStripMenuItem4";
            this->toolStripMenuItem4->Size = System::Drawing::Size(172, 22);
            this->toolStripMenuItem4->Text = L"Save To File";
            // 
            // toolStripMenuItem5
            // 
            this->toolStripMenuItem5->Name = L"toolStripMenuItem5";
            this->toolStripMenuItem5->Size = System::Drawing::Size(172, 22);
            this->toolStripMenuItem5->Text = L"Save To Clipboard";
            // 
            // label17
            // 
            this->label17->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->label17->AutoSize = true;
            this->label17->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label17->Location = System::Drawing::Point(254, 297);
            this->label17->Name = L"label17";
            this->label17->Size = System::Drawing::Size(252, 13);
            this->label17->TabIndex = 55;
            this->label17->Text = L"Right Mouse Click Graph for Print and Save Options";
            // 
            // label18
            // 
            this->label18->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->label18->AutoSize = true;
            this->label18->Location = System::Drawing::Point(526, 338);
            this->label18->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
            this->label18->Name = L"label18";
            this->label18->Size = System::Drawing::Size(69, 13);
            this->label18->TabIndex = 56;
            this->label18->Text = L"Set Axis Limit";
            this->toolTip1->SetToolTip(this->label18, L"Type an axis limit in the textbox\r\nand hit return key \r\nTo revert back to default" 
                L" clear\r\nthe textbox and hit return key.");
            // 
            // Grap_Emis
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->BackColor = System::Drawing::SystemColors::Control;
            this->ClientSize = System::Drawing::Size(762, 432);
            this->Controls->Add(this->label18);
            this->Controls->Add(this->_upInclination);
            this->Controls->Add(this->_udRotation);
            this->Controls->Add(this->label10);
            this->Controls->Add(this->_txYAxis);
            this->Controls->Add(this->label11);
            this->Controls->Add(this->label12);
            this->Controls->Add(this->_cbFixedAxis);
            this->Controls->Add(this->_txXAxis);
            this->Controls->Add(this->_btGE_Draw);
            this->Controls->Add(this->label16);
            this->Controls->Add(this->label15);
            this->Controls->Add(this->label17);
            this->Controls->Add(this->label14);
            this->Controls->Add(this->label4);
            this->Controls->Add(this->label13);
            this->Controls->Add(this->_txGEtotPM10);
            this->Controls->Add(this->_cbGE_Intensity);
            this->Controls->Add(this->_txGEtotCH4);
            this->Controls->Add(this->_txGEtotNOX);
            this->Controls->Add(this->_txGEtotCO);
            this->Controls->Add(this->_txGEtotSO2);
            this->Controls->Add(this->_txGEtotPM25);
            this->Controls->Add(this->label9);
            this->Controls->Add(this->_txGEtotCO2);
            this->Controls->Add(this->label8);
            this->Controls->Add(this->label5);
            this->Controls->Add(this->_chGrap_Emis);
            this->Controls->Add(this->label7);
            this->Controls->Add(this->label3);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->_txGEsmSO2);
            this->Controls->Add(this->label6);
            this->Controls->Add(this->_txGEsmNOX);
            this->Controls->Add(this->_txGEsmCH4);
            this->Controls->Add(this->_txGEsmPM10);
            this->Controls->Add(this->_txGEsmCO);
            this->Controls->Add(this->_txGEsmPM25);
            this->Controls->Add(this->_txGEflPM10);
            this->Controls->Add(this->_txGEflCH4);
            this->Controls->Add(this->_txGEflSO2);
            this->Controls->Add(this->_txGEsmCO2);
            this->Controls->Add(this->_txGEflNOX);
            this->Controls->Add(this->_txGEflPM25);
            this->Controls->Add(this->_cbGE_CH4);
            this->Controls->Add(this->_txGEflCO);
            this->Controls->Add(this->_cbGE_PM25);
            this->Controls->Add(this->_cbGE_CO);
            this->Controls->Add(this->_cbGE_SO2);
            this->Controls->Add(this->_cbGE_NOX);
            this->Controls->Add(this->_txGEflCO2);
            this->Controls->Add(this->_cbGE_PM10);
            this->Controls->Add(this->_cbGE_CO2);
            this->Margin = System::Windows::Forms::Padding(2);
            this->MaximumSize = System::Drawing::Size(1000, 800);
            this->Name = L"Grap_Emis";
            this->RightToLeft = System::Windows::Forms::RightToLeft::No;
            this->Text = L"FOFEM Smoke Emissions";
            this->Load += gcnew System::EventHandler(this, &Grap_Emis::Grap_Emis_Load);
            this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Grap_Emis::Grap_Emis_FormClosing);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_chGrap_Emis))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_udRotation))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_upInclination))->EndInit();
            this->_cmenuGraph->ResumeLayout(false);
            this->contextMenuStrip1->ResumeLayout(false);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion


/**********************************************************************************************/
private: System::Void _cbGE_PM10_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
{this->SeriesOnOff();}

private: System::Void _cbGE_PM25_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{ this->SeriesOnOff();}

private: System::Void _cbGE_CH4_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
{ this->SeriesOnOff();  }

private: System::Void _cbGE_CO_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
{ this->SeriesOnOff();  }

private: System::Void _cbGE_CO2_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{ this->SeriesOnOff();  }

private: System::Void _cbGE_NOX_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
{ this->SeriesOnOff();  }

private: System::Void _cbGE_SO2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
{  this->SeriesOnOff();  }

private: System::Void _cbGE_Intensity_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{this->SeriesOnOff(); }


/********************************************************************************************
* Refresh Button 
*********************************************************************************************/
private: System::Void _btGE_Draw_Click(System::Object^  sender, System::EventArgs^  e) 
{
  if ( this->_cbFixedAxis->Checked == false ) 
    return;
 this->FixedAxisButtonChanged();
}

/******************************************************************************************************
* Spin Graph Horizontal X Axis Rotate
* Note: graph was crashing when it rotated to 90, I don't think
*       you're allowed to rotate the graph past that, so I decided
*       to just limit the rotation on both the X and Y, 90 degrees
*       is plenty, I dont think anyone ever rotates them anyway
******************************************************************************************************/
private: System::Void numericUpDown1_ValueChanged(System::Object^  sender, System::EventArgs^  e)
 {
 int i;
 i = (int) this->_udRotation->Value;

#ifdef wowo
 if ( i == -185 )
    i = 180;
 if ( i == 185 )
    i = -180;
#endif 

// fix 8-8-18  See notes above
  if ( i == 90 )
    i = 85;

 if ( i == -90 )
    i = -85 ;
 

 this->_chGrap_Emis->ChartAreas["ChartArea1"]->Area3DStyle->Rotation = i; 
 
 this->_udRotation->Value = i;
 }

/******************************************************************************************************
* Spin Graph Vertical Y Axis Inclination 
*******************************************************************************************************/
private: System::Void _upInclination_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
{
int i;

 i = (int) this->_upInclination->Value;
#ifdef wowow
 if ( i == -95 )
    i = 90;
 if ( i == 95 )
    i = -90;
#endif 
// fixed 8-8-18, see notes in X rotation function above 
  if ( i == 90 )
    i = 85;

 if ( i == -90 )
    i = -85 ;

 this->_chGrap_Emis->ChartAreas["ChartArea1"]->Area3DStyle->Inclination = i; 
 this->_upInclination->Value = i; 
}

/*******************************************************************************************/
private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e)
{ }
private: System::Void textBox3_TextChanged(System::Object^  sender, System::EventArgs^  e) 
{}

/**************************************************************************************/
private: System::Void Grap_Emis_Load(System::Object^  sender, System::EventArgs^  e) 
{}

private: System::Void _cbFixedAxis_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
{
  this->FixedAxisButtonChanged();
}

/***************************************************************************************
* Graph Emission Form Closing - upper right X close form clicked.
*  We want to just hide the form - so that if user leaves it open they can watch 
*  it update it after each FuelCalc run
****************************************************************************************/
private: System::Void Grap_Emis_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
  this->Hide();
  e->Cancel = true; 
}

/************************************************************************
* Menu Item Clicked 
************************************************************************/
private: System::Void Print_Click(System::Object^  sender, System::EventArgs^  e)
 {
String ^ Str; 

  Str =  sender->ToString();
 
  if ( Str == "Print") 
    this->_chGrap_Emis->Printing->Print(true);
  else if ( Str == "Print Preview") 
    this->_chGrap_Emis->Printing->PrintPreview(); 
  else if ( Str == "Print Page Setup" ) 
   this->_chGrap_Emis->Printing->PageSetup(); 
 
  else if ( Str == "Save To Clipboard" ) {
   System::IO::MemoryStream ^ stream   = gcnew System::IO::MemoryStream(); 
   this->_chGrap_Emis->SaveImage (stream, System::Drawing::Imaging::ImageFormat::Bmp); 
   Bitmap ^ bmp = gcnew Bitmap (stream); 
   Clipboard::SetDataObject(bmp); }

  else if ( Str == "Save To File" ) {
    SaveFileDialog ^ saveFileDialog1 = gcnew SaveFileDialog();
    saveFileDialog1->Filter = "Bitmap (*.bmp)|*.bmp|JPEG (*.jpg)|*.jpg|EMF (*.emf)|*.emf|PNG (*.png)|*.png|GIF (*.gif)|*.gif|TIFF (*.tif)|*.tif";
    saveFileDialog1->FilterIndex = 2 ;
    saveFileDialog1->RestoreDirectory = true ;
    int i = (int) saveFileDialog1->ShowDialog();     
    if ( i == 0 )   return;       
    System::Windows::Forms::DataVisualization::Charting::ChartImageFormat  ^ format = 
       gcnew System::Windows::Forms::DataVisualization::Charting::ChartImageFormat;

    if ( saveFileDialog1->FileName->EndsWith( "bmp" ) )
       this->_chGrap_Emis->SaveImage ( saveFileDialog1->FileName,  System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Bmp); 
    if ( saveFileDialog1->FileName->EndsWith( "jpg" ) )
      this->_chGrap_Emis->SaveImage (saveFileDialog1->FileName,  System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Jpeg); 
    if ( saveFileDialog1->FileName->EndsWith( "gif" ) )
     this->_chGrap_Emis->SaveImage (saveFileDialog1->FileName, System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Gif); 
    if ( saveFileDialog1->FileName->EndsWith( "png" ) )
     this->_chGrap_Emis->SaveImage (saveFileDialog1->FileName, System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Png); 
    if ( saveFileDialog1->FileName->EndsWith( "emf" ) )
      this->_chGrap_Emis->SaveImage (saveFileDialog1->FileName, System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Emf); 
    if ( saveFileDialog1->FileName->EndsWith( "tif" ) )
      this->_chGrap_Emis->SaveImage (saveFileDialog1->FileName,System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Tiff); 
  } /* else if Save To File */

 }

/**********************************************************************
* Mouse Click on Graph 
***********************************************************************/
private: System::Void _chGrap_Emis_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
{
int width, height; 
bool found; 
// System::Windows::Forms::DataVisualization::Charting::Chart^  _chart;
String ^Str; 

/* Now was it a Right Mouse Click */
   if ( e->Button == Windows::Forms::MouseButtons::Left )  /* We Want Right Mouse BUtton */
     return; 

/* So we can position the ContextMenu in the middle of the chart */
  width = this->_chGrap_Emis->Size.Width; 
  height = this->_chGrap_Emis->Size.Height;

  width = width / 2;
  height = height / 2; 
  this->_cmenuGraph->Show(this->_chGrap_Emis, width,height);  /* Popup the ContextMenu */ 

}

/*******************************************************************/
private: System::Void _chGrap_Emis_Click(System::Object^  sender, System::EventArgs^  e) {
         }


/*********************************************************************************
* Graph Emissions Form - X/Y Axis TextBox - Key Down 
*********************************************************************************/
private: System::Void _txXAxis_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{
  if ( e->KeyCode ==   System::Windows::Forms::Keys::Enter)
    // this->Set_XAxis(); 
      this->Axis_Mngr();
}

private: System::Void _txYAxis_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{
  if ( e->KeyCode ==   System::Windows::Forms::Keys::Enter)
    // this->Set_YAxis();
    this->Axis_Mngr();  
}


};  /* Grap_Emis  -  end class */
