#pragma once
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <math.h> 
#include "fof_sh.h" 

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#define private public 


#define  e_TitleUF  "FOFEM Soil Heating User File"

/***********************************************************************************/
public ref class Soil_Frm : public System::Windows::Forms::Form
{
	public:
        String ^ s_Chart; 
        float *FR; 
        float f_DufMoi; 

public: System::Windows::Forms::ContextMenuStrip^  _cmFirIntGra;
public: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem1;
public: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem2;
public: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem3;
public: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem4;
public: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem5;
public: System::Windows::Forms::Label^  _lbPocoNote;
public: System::Windows::Forms::Label^  _lbPocoMess;

        float f_DufDepPos;  /* post fire duff depth */
        int Graph_File( char cr_Init[]); 

        
        bool b_GraphFile;     /* true means User Soil File Graph is showing */
        int Update (d_SI *a_SI, d_SO *a_SO); 
        int UpdateDraw(); 
        int UpdateWatts (); 
        int SetTitles(float f_SoilMoist, char cr_SoilType[], 
                         float f_cDufPost, char cr_Unit[], int i_MaxTemp, int i_MaxTim);
        bool Set_Visable (); 
        void Show_ChkBox(bool b); 
        void Set_ChkBox();
        int Get_HighLayChk(); 
        int Init (); 
        void Clear (); 
        void Set_Style(); 
        int Set_XAxis();
        int Set_YAxis();

        char *cr_SoilType;
        float  f_SoilMoist; 
        d_SO *a_SO; 

    public: System::Windows::Forms::ContextMenuStrip^  _cmSoilGraph;
    public: System::Windows::Forms::ToolStripMenuItem^  Print;
    public: System::Windows::Forms::ToolStripMenuItem^  PrintPreview;
    public: System::Windows::Forms::ToolStripMenuItem^  PrintPageSetup;
    public: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  saveToClipboardToolStripMenuItem;
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::Label^  label4;
    private: System::Windows::Forms::Label^  label5;
    private: System::Windows::Forms::Label^  label6;
    private: System::Windows::Forms::Label^  label7;
    private: System::Windows::Forms::Label^  label8;
    private: System::Windows::Forms::Label^  label9;
    private: System::Windows::Forms::Label^  label10;
    private: System::Windows::Forms::Label^  label11;
    private: System::Windows::Forms::Label^  label12;
    private: System::Windows::Forms::Label^  label13;
    private: System::Windows::Forms::Label^  label14;
    private: System::Windows::Forms::Label^  label15;
    private: System::Windows::Forms::TextBox^  _txXAxis;
    private: System::Windows::Forms::Label^  label16;
    private: System::Windows::Forms::TextBox^  _txYAxis;
    public: System::Windows::Forms::Label^  label17;
    private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart2; 
    public: 
        d_SI *a_SI; 

/*-------------------------------------------------------*/
/* Constructor  */
Soil_Frm(void)
{
  InitializeComponent();
  Init(); 
  a_SO = new d_SO;
  a_SI = new d_SI; 
  b_GraphFile = false; 
  FR = new float[20]; 

    }

/*----------------------------------------------------*/
/* Destructor */
 ~Soil_Frm()
{
  delete a_SO;
  delete a_SI;
  delete FR; 
  if (components){
	delete components;}
}
/******************************************************/
  
  private: System::Windows::Forms::TextBox^  textBox1;
    private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart1;
    public: System::Windows::Forms::CheckBox^  _cbL1;
    public: System::Windows::Forms::CheckBox^  _cbL2;
    public: System::Windows::Forms::CheckBox^  _cbL3;
    public: System::Windows::Forms::CheckBox^  _cbL4;
    public: System::Windows::Forms::CheckBox^  _cbL5;
    public: System::Windows::Forms::CheckBox^  _cbL9;
    public: System::Windows::Forms::CheckBox^  _cbL8;
    public: System::Windows::Forms::CheckBox^  _cbL7;
    public: System::Windows::Forms::CheckBox^  _cbL6;
    public: System::Windows::Forms::CheckBox^  _cbL10;
    public: System::Windows::Forms::CheckBox^  _cbL14;
    public: System::Windows::Forms::CheckBox^  _cbL13;
    public: System::Windows::Forms::CheckBox^  _cbL12;
    public: System::Windows::Forms::CheckBox^  _cbL11;

    private: System::Windows::Forms::CheckBox^  _cbStyle;
private: System::ComponentModel::IContainer^  components;
public: 

public: 

public: 

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
            System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
            System::Windows::Forms::DataVisualization::Charting::Legend^  legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
            System::Windows::Forms::DataVisualization::Charting::Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series4 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series5 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series6 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series7 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series8 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series9 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series10 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series11 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series12 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series13 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series14 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series15 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Title^  title1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Title());
            System::Windows::Forms::DataVisualization::Charting::Title^  title2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Title());
            System::Windows::Forms::DataVisualization::Charting::Title^  title3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Title());
            System::Windows::Forms::DataVisualization::Charting::Title^  title4 = (gcnew System::Windows::Forms::DataVisualization::Charting::Title());
            System::Windows::Forms::DataVisualization::Charting::Title^  title5 = (gcnew System::Windows::Forms::DataVisualization::Charting::Title());
            System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea2 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
            System::Windows::Forms::DataVisualization::Charting::Series^  series16 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Title^  title6 = (gcnew System::Windows::Forms::DataVisualization::Charting::Title());
            System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Soil_Frm::typeid));
            this->textBox1 = (gcnew System::Windows::Forms::TextBox());
            this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
            this->_cbL1 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbL2 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbL3 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbL4 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbL5 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbL9 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbL8 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbL7 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbL6 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbL10 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbL14 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbL13 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbL12 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbL11 = (gcnew System::Windows::Forms::CheckBox());
            this->_cbStyle = (gcnew System::Windows::Forms::CheckBox());
            this->_cmSoilGraph = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->Print = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->PrintPreview = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->PrintPageSetup = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->saveToClipboardToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->label4 = (gcnew System::Windows::Forms::Label());
            this->label5 = (gcnew System::Windows::Forms::Label());
            this->label6 = (gcnew System::Windows::Forms::Label());
            this->label7 = (gcnew System::Windows::Forms::Label());
            this->label8 = (gcnew System::Windows::Forms::Label());
            this->label9 = (gcnew System::Windows::Forms::Label());
            this->label10 = (gcnew System::Windows::Forms::Label());
            this->label11 = (gcnew System::Windows::Forms::Label());
            this->label12 = (gcnew System::Windows::Forms::Label());
            this->label13 = (gcnew System::Windows::Forms::Label());
            this->label14 = (gcnew System::Windows::Forms::Label());
            this->label15 = (gcnew System::Windows::Forms::Label());
            this->_txXAxis = (gcnew System::Windows::Forms::TextBox());
            this->label16 = (gcnew System::Windows::Forms::Label());
            this->_txYAxis = (gcnew System::Windows::Forms::TextBox());
            this->label17 = (gcnew System::Windows::Forms::Label());
            this->chart2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
            this->_cmFirIntGra = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripMenuItem3 = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripMenuItem4 = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripMenuItem5 = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->_lbPocoNote = (gcnew System::Windows::Forms::Label());
            this->_lbPocoMess = (gcnew System::Windows::Forms::Label());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->BeginInit();
            this->_cmSoilGraph->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart2))->BeginInit();
            this->_cmFirIntGra->SuspendLayout();
            this->SuspendLayout();
            // 
            // textBox1
            // 
            this->textBox1->Location = System::Drawing::Point(103, 276);
            this->textBox1->Name = L"textBox1";
            this->textBox1->Size = System::Drawing::Size(56, 20);
            this->textBox1->TabIndex = 0;
            // 
            // chart1
            // 
            this->chart1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
                | System::Windows::Forms::AnchorStyles::Right));
            this->chart1->BorderlineColor = System::Drawing::Color::Black;
            this->chart1->BorderlineDashStyle = System::Windows::Forms::DataVisualization::Charting::ChartDashStyle::Solid;
            chartArea1->Area3DStyle->Inclination = 0;
            chartArea1->Area3DStyle->Rotation = 0;
            chartArea1->AxisX->IsLabelAutoFit = false;
            chartArea1->AxisX->MajorGrid->LineColor = System::Drawing::Color::LightGray;
            chartArea1->AxisX->ScrollBar->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), 
                static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)));
            chartArea1->AxisX->ScrollBar->ButtonColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), 
                static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
            chartArea1->AxisX->TextOrientation = System::Windows::Forms::DataVisualization::Charting::TextOrientation::Horizontal;
            chartArea1->AxisX->Title = L"Time - min";
            chartArea1->AxisY->IsLabelAutoFit = false;
            chartArea1->AxisY->MajorGrid->LineColor = System::Drawing::Color::LightGray;
            chartArea1->AxisY->Title = L"Soil Temperature - deg C";
            chartArea1->BackColor = System::Drawing::Color::White;
            chartArea1->BorderDashStyle = System::Windows::Forms::DataVisualization::Charting::ChartDashStyle::Solid;
            chartArea1->CursorY->Position = 60;
            chartArea1->IsSameFontSizeForAllAxes = true;
            chartArea1->Name = L"ChartArea1";
            this->chart1->ChartAreas->Add(chartArea1);
            legend1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)), 
                static_cast<System::Int32>(static_cast<System::Byte>(224)));
            legend1->BorderColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(0)), 
                static_cast<System::Int32>(static_cast<System::Byte>(0)));
            legend1->Docking = System::Windows::Forms::DataVisualization::Charting::Docking::Left;
            legend1->LegendStyle = System::Windows::Forms::DataVisualization::Charting::LegendStyle::Column;
            legend1->Name = L"Legend1";
            this->chart1->Legends->Add(legend1);
            this->chart1->Location = System::Drawing::Point(94, 12);
            this->chart1->Name = L"chart1";
            series1->ChartArea = L"ChartArea1";
            series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::SplineArea;
            series1->Color = System::Drawing::Color::Red;
            series1->Legend = L"Legend1";
            series1->LegendText = L"Surface";
            series1->Name = L"Lay1";
            series2->ChartArea = L"ChartArea1";
            series2->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Area;
            series2->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)), 
                static_cast<System::Int32>(static_cast<System::Byte>(0)));
            series2->Legend = L"Legend1";
            series2->LegendText = L"1 cm";
            series2->Name = L"Lay2";
            series3->ChartArea = L"ChartArea1";
            series3->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Area;
            series3->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)), 
                static_cast<System::Int32>(static_cast<System::Byte>(128)));
            series3->Legend = L"Legend1";
            series3->LegendText = L"2 cm";
            series3->Name = L"Lay3";
            series4->ChartArea = L"ChartArea1";
            series4->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Area;
            series4->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
                static_cast<System::Int32>(static_cast<System::Byte>(128)));
            series4->Legend = L"Legend1";
            series4->LegendText = L"3 cm";
            series4->Name = L"Lay4";
            series5->ChartArea = L"ChartArea1";
            series5->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Area;
            series5->Color = System::Drawing::Color::Yellow;
            series5->Legend = L"Legend1";
            series5->LegendText = L"4 cm";
            series5->Name = L"Lay5";
            series6->ChartArea = L"ChartArea1";
            series6->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Area;
            series6->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
                static_cast<System::Int32>(static_cast<System::Byte>(192)));
            series6->Legend = L"Legend1";
            series6->LegendText = L"5 cm";
            series6->Name = L"Lay6";
            series7->ChartArea = L"ChartArea1";
            series7->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Area;
            series7->Color = System::Drawing::Color::Lime;
            series7->Legend = L"Legend1";
            series7->LegendText = L"6 cm";
            series7->Name = L"Lay7";
            series8->ChartArea = L"ChartArea1";
            series8->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Area;
            series8->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
                static_cast<System::Int32>(static_cast<System::Byte>(192)));
            series8->Legend = L"Legend1";
            series8->LegendText = L"7 cm";
            series8->Name = L"Lay8";
            series9->ChartArea = L"ChartArea1";
            series9->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Area;
            series9->Color = System::Drawing::Color::Cyan;
            series9->Legend = L"Legend1";
            series9->LegendText = L"8 cm";
            series9->Name = L"Lay9";
            series10->ChartArea = L"ChartArea1";
            series10->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Area;
            series10->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
                static_cast<System::Int32>(static_cast<System::Byte>(255)));
            series10->Legend = L"Legend1";
            series10->LegendText = L"9 cm";
            series10->Name = L"Lay10";
            series11->ChartArea = L"ChartArea1";
            series11->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Area;
            series11->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(128)), 
                static_cast<System::Int32>(static_cast<System::Byte>(255)));
            series11->Legend = L"Legend1";
            series11->LegendText = L"10 cm";
            series11->Name = L"Lay11";
            series12->ChartArea = L"ChartArea1";
            series12->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Area;
            series12->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
                static_cast<System::Int32>(static_cast<System::Byte>(255)));
            series12->Legend = L"Legend1";
            series12->LegendText = L"11 cm";
            series12->Name = L"Lay12";
            series13->ChartArea = L"ChartArea1";
            series13->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Area;
            series13->Color = System::Drawing::Color::Maroon;
            series13->Legend = L"Legend1";
            series13->LegendText = L"12 cm";
            series13->Name = L"Lay13";
            series14->ChartArea = L"ChartArea1";
            series14->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Area;
            series14->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
                static_cast<System::Int32>(static_cast<System::Byte>(0)));
            series14->Legend = L"Legend1";
            series14->LegendText = L"13 cm";
            series14->Name = L"Lay14";
            series15->ChartArea = L"ChartArea1";
            series15->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
            series15->Legend = L"Legend1";
            series15->Name = L"Temp-60";
            this->chart1->Series->Add(series1);
            this->chart1->Series->Add(series2);
            this->chart1->Series->Add(series3);
            this->chart1->Series->Add(series4);
            this->chart1->Series->Add(series5);
            this->chart1->Series->Add(series6);
            this->chart1->Series->Add(series7);
            this->chart1->Series->Add(series8);
            this->chart1->Series->Add(series9);
            this->chart1->Series->Add(series10);
            this->chart1->Series->Add(series11);
            this->chart1->Series->Add(series12);
            this->chart1->Series->Add(series13);
            this->chart1->Series->Add(series14);
            this->chart1->Series->Add(series15);
            this->chart1->Size = System::Drawing::Size(574, 401);
            this->chart1->TabIndex = 1;
            this->chart1->Text = L"chart1";
            title1->Alignment = System::Drawing::ContentAlignment::TopLeft;
            title1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            title1->Name = L"SoilType";
            title1->Text = L"...";
            title1->Visible = false;
            title2->Alignment = System::Drawing::ContentAlignment::TopLeft;
            title2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            title2->Name = L"Moisture";
            title2->Text = L"0";
            title2->Visible = false;
            title3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            title3->Name = L"MainTitle";
            title3->Text = L"Soil Heating ";
            title4->Alignment = System::Drawing::ContentAlignment::TopLeft;
            title4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            title4->Name = L"MaxTemp";
            title4->Text = L"...";
            title4->Visible = false;
            title5->Alignment = System::Drawing::ContentAlignment::TopLeft;
            title5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            title5->Name = L"PostDufDep";
            title5->Text = L"...";
            title5->Visible = false;
            this->chart1->Titles->Add(title1);
            this->chart1->Titles->Add(title2);
            this->chart1->Titles->Add(title3);
            this->chart1->Titles->Add(title4);
            this->chart1->Titles->Add(title5);
            this->chart1->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &Soil_Frm::chart1_MouseClick);
            // 
            // _cbL1
            // 
            this->_cbL1->AutoSize = true;
            this->_cbL1->Location = System::Drawing::Point(12, 21);
            this->_cbL1->Name = L"_cbL1";
            this->_cbL1->Size = System::Drawing::Size(15, 14);
            this->_cbL1->TabIndex = 2;
            this->_cbL1->UseVisualStyleBackColor = true;
            this->_cbL1->CheckedChanged += gcnew System::EventHandler(this, &Soil_Frm::_cbL1_CheckedChanged);
            // 
            // _cbL2
            // 
            this->_cbL2->AutoSize = true;
            this->_cbL2->Location = System::Drawing::Point(12, 44);
            this->_cbL2->Name = L"_cbL2";
            this->_cbL2->Size = System::Drawing::Size(15, 14);
            this->_cbL2->TabIndex = 3;
            this->_cbL2->UseVisualStyleBackColor = true;
            this->_cbL2->CheckedChanged += gcnew System::EventHandler(this, &Soil_Frm::_cbL1_CheckedChanged);
            // 
            // _cbL3
            // 
            this->_cbL3->AutoSize = true;
            this->_cbL3->Location = System::Drawing::Point(12, 64);
            this->_cbL3->Name = L"_cbL3";
            this->_cbL3->Size = System::Drawing::Size(15, 14);
            this->_cbL3->TabIndex = 4;
            this->_cbL3->UseVisualStyleBackColor = true;
            this->_cbL3->CheckedChanged += gcnew System::EventHandler(this, &Soil_Frm::_cbL1_CheckedChanged);
            // 
            // _cbL4
            // 
            this->_cbL4->AutoSize = true;
            this->_cbL4->Location = System::Drawing::Point(12, 84);
            this->_cbL4->Name = L"_cbL4";
            this->_cbL4->Size = System::Drawing::Size(15, 14);
            this->_cbL4->TabIndex = 5;
            this->_cbL4->UseVisualStyleBackColor = true;
            this->_cbL4->CheckedChanged += gcnew System::EventHandler(this, &Soil_Frm::_cbL1_CheckedChanged);
            // 
            // _cbL5
            // 
            this->_cbL5->AutoSize = true;
            this->_cbL5->Location = System::Drawing::Point(12, 104);
            this->_cbL5->Name = L"_cbL5";
            this->_cbL5->Size = System::Drawing::Size(15, 14);
            this->_cbL5->TabIndex = 6;
            this->_cbL5->UseVisualStyleBackColor = true;
            this->_cbL5->CheckedChanged += gcnew System::EventHandler(this, &Soil_Frm::_cbL1_CheckedChanged);
            // 
            // _cbL9
            // 
            this->_cbL9->AutoSize = true;
            this->_cbL9->Location = System::Drawing::Point(12, 184);
            this->_cbL9->Name = L"_cbL9";
            this->_cbL9->Size = System::Drawing::Size(15, 14);
            this->_cbL9->TabIndex = 7;
            this->_cbL9->UseVisualStyleBackColor = true;
            this->_cbL9->CheckedChanged += gcnew System::EventHandler(this, &Soil_Frm::_cbL1_CheckedChanged);
            // 
            // _cbL8
            // 
            this->_cbL8->AutoSize = true;
            this->_cbL8->Location = System::Drawing::Point(12, 164);
            this->_cbL8->Name = L"_cbL8";
            this->_cbL8->Size = System::Drawing::Size(15, 14);
            this->_cbL8->TabIndex = 8;
            this->_cbL8->UseVisualStyleBackColor = true;
            this->_cbL8->CheckedChanged += gcnew System::EventHandler(this, &Soil_Frm::_cbL1_CheckedChanged);
            // 
            // _cbL7
            // 
            this->_cbL7->AutoSize = true;
            this->_cbL7->Location = System::Drawing::Point(12, 144);
            this->_cbL7->Name = L"_cbL7";
            this->_cbL7->Size = System::Drawing::Size(15, 14);
            this->_cbL7->TabIndex = 9;
            this->_cbL7->UseVisualStyleBackColor = true;
            this->_cbL7->CheckedChanged += gcnew System::EventHandler(this, &Soil_Frm::_cbL1_CheckedChanged);
            // 
            // _cbL6
            // 
            this->_cbL6->AutoSize = true;
            this->_cbL6->Location = System::Drawing::Point(12, 124);
            this->_cbL6->Name = L"_cbL6";
            this->_cbL6->Size = System::Drawing::Size(15, 14);
            this->_cbL6->TabIndex = 10;
            this->_cbL6->UseVisualStyleBackColor = true;
            this->_cbL6->CheckedChanged += gcnew System::EventHandler(this, &Soil_Frm::_cbL1_CheckedChanged);
            // 
            // _cbL10
            // 
            this->_cbL10->AutoSize = true;
            this->_cbL10->Location = System::Drawing::Point(12, 204);
            this->_cbL10->Name = L"_cbL10";
            this->_cbL10->Size = System::Drawing::Size(15, 14);
            this->_cbL10->TabIndex = 11;
            this->_cbL10->UseVisualStyleBackColor = true;
            this->_cbL10->CheckedChanged += gcnew System::EventHandler(this, &Soil_Frm::_cbL1_CheckedChanged);
            // 
            // _cbL14
            // 
            this->_cbL14->AutoSize = true;
            this->_cbL14->Location = System::Drawing::Point(12, 282);
            this->_cbL14->Name = L"_cbL14";
            this->_cbL14->Size = System::Drawing::Size(15, 14);
            this->_cbL14->TabIndex = 12;
            this->_cbL14->UseVisualStyleBackColor = true;
            this->_cbL14->CheckedChanged += gcnew System::EventHandler(this, &Soil_Frm::_cbL1_CheckedChanged);
            // 
            // _cbL13
            // 
            this->_cbL13->AutoSize = true;
            this->_cbL13->Location = System::Drawing::Point(12, 262);
            this->_cbL13->Name = L"_cbL13";
            this->_cbL13->Size = System::Drawing::Size(15, 14);
            this->_cbL13->TabIndex = 13;
            this->_cbL13->UseVisualStyleBackColor = true;
            this->_cbL13->CheckedChanged += gcnew System::EventHandler(this, &Soil_Frm::_cbL1_CheckedChanged);
            // 
            // _cbL12
            // 
            this->_cbL12->AutoSize = true;
            this->_cbL12->Location = System::Drawing::Point(12, 242);
            this->_cbL12->Name = L"_cbL12";
            this->_cbL12->Size = System::Drawing::Size(15, 14);
            this->_cbL12->TabIndex = 14;
            this->_cbL12->UseVisualStyleBackColor = true;
            this->_cbL12->CheckedChanged += gcnew System::EventHandler(this, &Soil_Frm::_cbL1_CheckedChanged);
            // 
            // _cbL11
            // 
            this->_cbL11->AutoSize = true;
            this->_cbL11->Location = System::Drawing::Point(12, 222);
            this->_cbL11->Name = L"_cbL11";
            this->_cbL11->Size = System::Drawing::Size(15, 14);
            this->_cbL11->TabIndex = 15;
            this->_cbL11->UseVisualStyleBackColor = true;
            this->_cbL11->CheckedChanged += gcnew System::EventHandler(this, &Soil_Frm::_cbL1_CheckedChanged);
            // 
            // _cbStyle
            // 
            this->_cbStyle->AutoSize = true;
            this->_cbStyle->Location = System::Drawing::Point(12, 320);
            this->_cbStyle->Name = L"_cbStyle";
            this->_cbStyle->Size = System::Drawing::Size(51, 17);
            this->_cbStyle->TabIndex = 17;
            this->_cbStyle->Text = L"Lines";
            this->_cbStyle->UseVisualStyleBackColor = true;
            this->_cbStyle->CheckedChanged += gcnew System::EventHandler(this, &Soil_Frm::_cbStyle_CheckedChanged);
            // 
            // _cmSoilGraph
            // 
            this->_cmSoilGraph->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->Print, this->PrintPreview, 
                this->PrintPageSetup, this->saveToolStripMenuItem, this->saveToClipboardToolStripMenuItem});
            this->_cmSoilGraph->Name = L"_cmenuGraph";
            this->_cmSoilGraph->Size = System::Drawing::Size(162, 114);
            this->_cmSoilGraph->Opening += gcnew System::ComponentModel::CancelEventHandler(this, &Soil_Frm::_cmSoilGraph_Opening);
            // 
            // Print
            // 
            this->Print->Name = L"Print";
            this->Print->Size = System::Drawing::Size(161, 22);
            this->Print->Text = L"Print";
            this->Print->Click += gcnew System::EventHandler(this, &Soil_Frm::Print_Click);
            // 
            // PrintPreview
            // 
            this->PrintPreview->Name = L"PrintPreview";
            this->PrintPreview->Size = System::Drawing::Size(161, 22);
            this->PrintPreview->Text = L"Print Preview";
            this->PrintPreview->Click += gcnew System::EventHandler(this, &Soil_Frm::Print_Click);
            // 
            // PrintPageSetup
            // 
            this->PrintPageSetup->Name = L"PrintPageSetup";
            this->PrintPageSetup->Size = System::Drawing::Size(161, 22);
            this->PrintPageSetup->Text = L"Print Page Setup";
            this->PrintPageSetup->Click += gcnew System::EventHandler(this, &Soil_Frm::Print_Click);
            // 
            // saveToolStripMenuItem
            // 
            this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
            this->saveToolStripMenuItem->Size = System::Drawing::Size(161, 22);
            this->saveToolStripMenuItem->Text = L"Save To File";
            this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &Soil_Frm::Print_Click);
            // 
            // saveToClipboardToolStripMenuItem
            // 
            this->saveToClipboardToolStripMenuItem->Name = L"saveToClipboardToolStripMenuItem";
            this->saveToClipboardToolStripMenuItem->Size = System::Drawing::Size(161, 22);
            this->saveToClipboardToolStripMenuItem->Text = L"Save To Clipboard";
            this->saveToClipboardToolStripMenuItem->Click += gcnew System::EventHandler(this, &Soil_Frm::Print_Click);
            // 
            // label2
            // 
            this->label2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
            this->label2->AutoSize = true;
            this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label2->Location = System::Drawing::Point(251, 429);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(252, 13);
            this->label2->TabIndex = 19;
            this->label2->Text = L"Right Mouse Click Graph for Print and Save Options";
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(40, 22);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(44, 13);
            this->label1->TabIndex = 20;
            this->label1->Text = L"Surface";
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(40, 44);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(30, 13);
            this->label3->TabIndex = 21;
            this->label3->Text = L"1 cm";
            // 
            // label4
            // 
            this->label4->AutoSize = true;
            this->label4->Location = System::Drawing::Point(40, 64);
            this->label4->Name = L"label4";
            this->label4->Size = System::Drawing::Size(30, 13);
            this->label4->TabIndex = 22;
            this->label4->Text = L"2 cm";
            // 
            // label5
            // 
            this->label5->AutoSize = true;
            this->label5->Location = System::Drawing::Point(40, 84);
            this->label5->Name = L"label5";
            this->label5->Size = System::Drawing::Size(30, 13);
            this->label5->TabIndex = 23;
            this->label5->Text = L"3 cm";
            // 
            // label6
            // 
            this->label6->AutoSize = true;
            this->label6->Location = System::Drawing::Point(40, 105);
            this->label6->Name = L"label6";
            this->label6->Size = System::Drawing::Size(30, 13);
            this->label6->TabIndex = 24;
            this->label6->Text = L"4 cm";
            // 
            // label7
            // 
            this->label7->AutoSize = true;
            this->label7->Location = System::Drawing::Point(40, 185);
            this->label7->Name = L"label7";
            this->label7->Size = System::Drawing::Size(30, 13);
            this->label7->TabIndex = 25;
            this->label7->Text = L"8 cm";
            // 
            // label8
            // 
            this->label8->AutoSize = true;
            this->label8->Location = System::Drawing::Point(40, 165);
            this->label8->Name = L"label8";
            this->label8->Size = System::Drawing::Size(30, 13);
            this->label8->TabIndex = 26;
            this->label8->Text = L"7 cm";
            // 
            // label9
            // 
            this->label9->AutoSize = true;
            this->label9->Location = System::Drawing::Point(40, 145);
            this->label9->Name = L"label9";
            this->label9->Size = System::Drawing::Size(30, 13);
            this->label9->TabIndex = 27;
            this->label9->Text = L"6 cm";
            // 
            // label10
            // 
            this->label10->AutoSize = true;
            this->label10->Location = System::Drawing::Point(40, 124);
            this->label10->Name = L"label10";
            this->label10->Size = System::Drawing::Size(30, 13);
            this->label10->TabIndex = 28;
            this->label10->Text = L"5 cm";
            // 
            // label11
            // 
            this->label11->AutoSize = true;
            this->label11->Location = System::Drawing::Point(40, 205);
            this->label11->Name = L"label11";
            this->label11->Size = System::Drawing::Size(30, 13);
            this->label11->TabIndex = 29;
            this->label11->Text = L"9 cm";
            // 
            // label12
            // 
            this->label12->AutoSize = true;
            this->label12->Location = System::Drawing::Point(40, 223);
            this->label12->Name = L"label12";
            this->label12->Size = System::Drawing::Size(36, 13);
            this->label12->TabIndex = 30;
            this->label12->Text = L"10 cm";
            // 
            // label13
            // 
            this->label13->AutoSize = true;
            this->label13->Location = System::Drawing::Point(40, 243);
            this->label13->Name = L"label13";
            this->label13->Size = System::Drawing::Size(36, 13);
            this->label13->TabIndex = 31;
            this->label13->Text = L"11 cm";
            // 
            // label14
            // 
            this->label14->AutoSize = true;
            this->label14->Location = System::Drawing::Point(40, 263);
            this->label14->Name = L"label14";
            this->label14->Size = System::Drawing::Size(36, 13);
            this->label14->TabIndex = 32;
            this->label14->Text = L"12 cm";
            // 
            // label15
            // 
            this->label15->AutoSize = true;
            this->label15->Location = System::Drawing::Point(40, 283);
            this->label15->Name = L"label15";
            this->label15->Size = System::Drawing::Size(36, 13);
            this->label15->TabIndex = 33;
            this->label15->Text = L"13 cm";
            // 
            // _txXAxis
            // 
            this->_txXAxis->Location = System::Drawing::Point(94, 421);
            this->_txXAxis->Name = L"_txXAxis";
            this->_txXAxis->Size = System::Drawing::Size(46, 20);
            this->_txXAxis->TabIndex = 34;
            this->_txXAxis->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->_txXAxis->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Soil_Frm::_txXAxis_KeyDown);
            // 
            // label16
            // 
            this->label16->AutoSize = true;
            this->label16->Location = System::Drawing::Point(156, 428);
            this->label16->Name = L"label16";
            this->label16->Size = System::Drawing::Size(36, 13);
            this->label16->TabIndex = 35;
            this->label16->Text = L"X Axis";
            // 
            // _txYAxis
            // 
            this->_txYAxis->Location = System::Drawing::Point(7, 369);
            this->_txYAxis->Name = L"_txYAxis";
            this->_txYAxis->Size = System::Drawing::Size(41, 20);
            this->_txYAxis->TabIndex = 36;
            this->_txYAxis->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->_txYAxis->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Soil_Frm::_txYAxis_KeyDown);
            // 
            // label17
            // 
            this->label17->AutoSize = true;
            this->label17->Location = System::Drawing::Point(54, 403);
            this->label17->Name = L"label17";
            this->label17->Size = System::Drawing::Size(36, 13);
            this->label17->TabIndex = 37;
            this->label17->Text = L"Y Axis";
            // 
            // chart2
            // 
            this->chart2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
                | System::Windows::Forms::AnchorStyles::Right));
            chartArea2->AxisX->Title = L"Time - min ";
            chartArea2->AxisY->Title = L"Kilowatts";
            chartArea2->Name = L"ChartArea1";
            this->chart2->ChartAreas->Add(chartArea2);
            this->chart2->Location = System::Drawing::Point(231, 460);
            this->chart2->Name = L"chart2";
            series16->ChartArea = L"ChartArea1";
            series16->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::SplineArea;
            series16->Name = L"Series1";
            this->chart2->Series->Add(series16);
            this->chart2->Size = System::Drawing::Size(437, 180);
            this->chart2->TabIndex = 38;
            this->chart2->Text = L"chart2";
            title6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            title6->Name = L"Title1";
            title6->Text = L"Fire Intensity at Soil Surface";
            this->chart2->Titles->Add(title6);
            this->chart2->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &Soil_Frm::chart2_MouseClick);
            // 
            // _cmFirIntGra
            // 
            this->_cmFirIntGra->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->toolStripMenuItem1, 
                this->toolStripMenuItem2, this->toolStripMenuItem3, this->toolStripMenuItem4, this->toolStripMenuItem5});
            this->_cmFirIntGra->Name = L"_cmenuGraph";
            this->_cmFirIntGra->Size = System::Drawing::Size(162, 114);
            this->_cmFirIntGra->Opening += gcnew System::ComponentModel::CancelEventHandler(this, &Soil_Frm::_cmFirIntGra_Opening);
            // 
            // toolStripMenuItem1
            // 
            this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
            this->toolStripMenuItem1->Size = System::Drawing::Size(161, 22);
            this->toolStripMenuItem1->Text = L"Print";
            this->toolStripMenuItem1->Click += gcnew System::EventHandler(this, &Soil_Frm::Print_Click);
            // 
            // toolStripMenuItem2
            // 
            this->toolStripMenuItem2->Name = L"toolStripMenuItem2";
            this->toolStripMenuItem2->Size = System::Drawing::Size(161, 22);
            this->toolStripMenuItem2->Text = L"Print Preview";
            this->toolStripMenuItem2->Click += gcnew System::EventHandler(this, &Soil_Frm::Print_Click);
            // 
            // toolStripMenuItem3
            // 
            this->toolStripMenuItem3->Name = L"toolStripMenuItem3";
            this->toolStripMenuItem3->Size = System::Drawing::Size(161, 22);
            this->toolStripMenuItem3->Text = L"Print Page Setup";
            this->toolStripMenuItem3->Click += gcnew System::EventHandler(this, &Soil_Frm::Print_Click);
            // 
            // toolStripMenuItem4
            // 
            this->toolStripMenuItem4->Name = L"toolStripMenuItem4";
            this->toolStripMenuItem4->Size = System::Drawing::Size(161, 22);
            this->toolStripMenuItem4->Text = L"Save To File";
            this->toolStripMenuItem4->Click += gcnew System::EventHandler(this, &Soil_Frm::Print_Click);
            // 
            // toolStripMenuItem5
            // 
            this->toolStripMenuItem5->Name = L"toolStripMenuItem5";
            this->toolStripMenuItem5->Size = System::Drawing::Size(161, 22);
            this->toolStripMenuItem5->Text = L"Save To Clipboard";
            this->toolStripMenuItem5->Click += gcnew System::EventHandler(this, &Soil_Frm::Print_Click);
            // 
            // _lbPocoNote
            // 
            this->_lbPocoNote->AutoSize = true;
            this->_lbPocoNote->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lbPocoNote->Location = System::Drawing::Point(64, 507);
            this->_lbPocoNote->Name = L"_lbPocoNote";
            this->_lbPocoNote->Size = System::Drawing::Size(76, 20);
            this->_lbPocoNote->TabIndex = 39;
            this->_lbPocoNote->Text = L"* NOTE * ";
            // 
            // _lbPocoMess
            // 
            this->_lbPocoMess->AutoSize = true;
            this->_lbPocoMess->Location = System::Drawing::Point(26, 538);
            this->_lbPocoMess->Name = L"_lbPocoMess";
            this->_lbPocoMess->Size = System::Drawing::Size(176, 39);
            this->_lbPocoMess->TabIndex = 40;
            this->_lbPocoMess->Text = L"Soil heating model for pocosin types\r\nhas not been tested and the results\r\nmay no" 
                L"t be accurate. ";
            // 
            // Soil_Frm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(698, 652);
            this->Controls->Add(this->_lbPocoMess);
            this->Controls->Add(this->_lbPocoNote);
            this->Controls->Add(this->chart2);
            this->Controls->Add(this->label17);
            this->Controls->Add(this->_txYAxis);
            this->Controls->Add(this->label16);
            this->Controls->Add(this->_txXAxis);
            this->Controls->Add(this->label15);
            this->Controls->Add(this->label14);
            this->Controls->Add(this->label13);
            this->Controls->Add(this->label12);
            this->Controls->Add(this->label11);
            this->Controls->Add(this->label10);
            this->Controls->Add(this->label9);
            this->Controls->Add(this->label8);
            this->Controls->Add(this->label7);
            this->Controls->Add(this->label6);
            this->Controls->Add(this->label5);
            this->Controls->Add(this->label4);
            this->Controls->Add(this->label3);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->_cbStyle);
            this->Controls->Add(this->_cbL11);
            this->Controls->Add(this->_cbL12);
            this->Controls->Add(this->_cbL13);
            this->Controls->Add(this->_cbL14);
            this->Controls->Add(this->_cbL10);
            this->Controls->Add(this->_cbL6);
            this->Controls->Add(this->_cbL7);
            this->Controls->Add(this->_cbL8);
            this->Controls->Add(this->_cbL9);
            this->Controls->Add(this->_cbL5);
            this->Controls->Add(this->_cbL4);
            this->Controls->Add(this->_cbL3);
            this->Controls->Add(this->_cbL2);
            this->Controls->Add(this->_cbL1);
            this->Controls->Add(this->chart1);
            this->Controls->Add(this->textBox1);
            this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
            this->Name = L"Soil_Frm";
            this->Text = L"Soil Heating Graph ";
            this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Soil_Frm::Soil_Frm_FormClosing);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->EndInit();
            this->_cmSoilGraph->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart2))->EndInit();
            this->_cmFirIntGra->ResumeLayout(false);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
   

/*****************************************************************************************
* Form Closing, User hit 'X' corner button to close,
* We just want to hide the form, so we interecept, or else the form would close.
******************************************************************************************/
 private: System::Void Soil_Frm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) 
{
  this->Hide();
  e->Cancel = true; 
}
   

/*************************************************************************
* Redraw button clicked 
*************************************************************************/
 private: System::Void _btRedraw_Click(System::Object^  sender, System::EventArgs^  e)
 {
 }

/**************************************************************************
* Style Checkbox changed/clicked  
* Causes graph to display as solid splines or lines 
*************************************************************************/
private: System::Void _cbStyle_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
{
  this->Set_Style();   /* Set graph to lines or soild layer display */

  if ( this->Text == e_TitleUF )
    this->Graph_File("");
  else
    this->UpdateDraw();      /* redraw graph */
}

/*********************************************************************
* Layer Checkbox was clicked 
*********************************************************************/
private: System::Void _cbL1_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
  this->Set_Visable();

  if ( this->Text == e_TitleUF )
     this->Graph_File("");
  else 
     this->UpdateDraw();   /* draw the standard (non user file) soil graph  */
}


/****************************************************************************
* Mouse Clicked on Soil heating layer graph chart, will pop up the context
*  menu for Print, Save, etc...
******************************************************************************/
private: System::Void chart1_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
{
int width, height; 
bool found; 
// System::Windows::Forms::DataVisualization::Charting::Chart^  _chart;
String ^Str; 

/* Now was it a Right Mouse Click */
   if ( e->Button == Windows::Forms::MouseButtons::Left )  /* We Want Right Mouse BUtton */
     return; 

   this->s_Chart = "Layer"; 

/* So we can position the ContextMenu in the middle of the chart */
  width = this->chart1->Size.Width; 
  height = this->chart1->Size.Height;

  width = width / 2;
  height = height / 2; 
 
  this->_cmSoilGraph->Show(this->chart1, width,height);  /* Popup the ContextMenu */ 

}

/*********************************************************************************
* Context Menu Item Click
* The context menu pops up when user right clicks on a graph chart
* This function is connect to every item in context menu list.
* There are separate context menus for the Soil Layer and Fire Intensity charts
*  and this function is connect to both.
**********************************************************************************/
private: System::Void Print_Click(System::Object^  sender, System::EventArgs^  e) 
{
String ^ Str; 
System::Windows::Forms::DataVisualization::Charting::Chart^  chart;


/* What Chart was the Context Menu on... */
  if ( this->s_Chart == "Layer" ) 
    chart = this->chart1;         /* Soil Layer chart */
  else 
    chart = this->chart2;         /* Fire Intensity chart */

  Str =  sender->ToString();
 
  if ( Str == "Print") 
    chart->Printing->Print(true);
  else if ( Str == "Print Preview") 
    chart->Printing->PrintPreview(); 
  else if ( Str == "Print Page Setup" ) 
    chart->Printing->PageSetup(); 
 
  else if ( Str == "Save To Clipboard" ) {
    System::IO::MemoryStream ^ stream   = gcnew System::IO::MemoryStream(); 
    chart->SaveImage (stream, System::Drawing::Imaging::ImageFormat::Bmp); 
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
       chart->SaveImage ( saveFileDialog1->FileName,  System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Bmp); 
    if ( saveFileDialog1->FileName->EndsWith( "jpg" ) )
      chart->SaveImage (saveFileDialog1->FileName,  System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Jpeg); 
    if ( saveFileDialog1->FileName->EndsWith( "gif" ) )
     chart->SaveImage (saveFileDialog1->FileName, System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Gif); 
    if ( saveFileDialog1->FileName->EndsWith( "png" ) )
     chart->SaveImage (saveFileDialog1->FileName, System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Png); 
    if ( saveFileDialog1->FileName->EndsWith( "emf" ) )
      chart->SaveImage (saveFileDialog1->FileName, System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Emf); 
    if ( saveFileDialog1->FileName->EndsWith( "tif" ) )
      chart->SaveImage (saveFileDialog1->FileName,System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Tiff); 
  } /* else if Save To File */

}


/****************************************************************************
* Mouse Clicked  on Soil Fire Intensity graph chart, will pop up the
*  context menu for Print, Save, etc.... 
******************************************************************************/
private: System::Void chart2_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
int width, height; 
bool found; 
// System::Windows::Forms::DataVisualization::Charting::Chart^  _chart;
String ^Str; 

/* Now was it a Right Mouse Click */
   if ( e->Button == Windows::Forms::MouseButtons::Left )  /* We Want Right Mouse BUtton */
     return; 

/* Tell Print_Click() we're dealing with the Fire Intensity Chart  */
  this->s_Chart = "Intensity"; 

/* So we can position the ContextMenu in the middle of the chart */
  width = this->chart2->Size.Width; 
  height = this->chart2->Size.Height;

  width = width / 2;
  height = height / 2; 
 
  this->_cmFirIntGra->Show(this->chart2, width,height);  /* Popup the ContextMenu */ 
}

/********************************************************************
* Y & X Axis TextBox - when text changes redo the axis
*  I redraw the whole graph when the user changes and axis, which is 
*   over kill but it's not a performance issue and saves doing a bunch
*   of coding. 
*  There's two instances of the soil graph window, the regular one and
*   the one for the user input file. 
**********************************************************************/
private: System::Void _txXAxis_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{
  if ( e->KeyCode != System::Windows::Forms::Keys::Enter) 
    return ; 
  if ( this->Text == e_TitleUF )   /* Soil Window Title tells us */
    this->Graph_File("");          /*  if it's Soil User File Graph */
  else 
    this->UpdateDraw(); 
  
  this->UpdateWatts ();            /* redraw fire intensity graph */
}

private: System::Void _txYAxis_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) 
{
  if ( e->KeyCode !=   System::Windows::Forms::Keys::Enter)
    return; 
  if ( this->Text == e_TitleUF ) 
    this->Graph_File(""); 
  else 
    this->UpdateDraw(); 
}


private: System::Void _cmSoilGraph_Opening(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) { }
private: System::Void _cmFirIntGra_Opening(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) { }


};
// }
