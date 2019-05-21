
#pragma once

//#include <stdio.h>
//#include <stdlib.h> 
//#include <string.h>
//#include <math.h> 

 #include "fof_sgv.h"
 #include "fof_co.h"


using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


// namespace FOF_GUI {

/**********************************************************************
* Fuel Form of Graphs 
**********************************************************************/
	public ref class Fuel_Frm : public System::Windows::Forms::Form
	{
public:
    float f_YAxisMax;
     
    int Update(d_CO *a_CO); 
	
/*--------------------------------------------------*/
/* Constructor */	
Fuel_Frm(void)
{
  InitializeComponent();
  f_YAxisMax = -1; 
}


~Fuel_Frm()
{
  if (components)	{
	delete components;}
}

/*******************************************************************/
    public: System::Windows::Forms::DataVisualization::Charting::Chart^  _chFuel;
    private: System::Windows::Forms::CheckBox^  _chFuelYaxis;
    public: 

    public: System::Windows::Forms::ContextMenuStrip^  _cmenuGraph;
    private: 
    public: System::Windows::Forms::ToolStripMenuItem^  Print;
    public: System::Windows::Forms::ToolStripMenuItem^  PrintPreview;
    public: System::Windows::Forms::ToolStripMenuItem^  PrintPageSetup;
    public: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  saveToClipboardToolStripMenuItem;
    private: System::Windows::Forms::Label^  label1;
    public: 
    private: System::ComponentModel::IContainer^  components;
    public: 

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
            System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
            System::Windows::Forms::DataVisualization::Charting::CustomLabel^  customLabel1 = (gcnew System::Windows::Forms::DataVisualization::Charting::CustomLabel());
            System::Windows::Forms::DataVisualization::Charting::CustomLabel^  customLabel2 = (gcnew System::Windows::Forms::DataVisualization::Charting::CustomLabel());
            System::Windows::Forms::DataVisualization::Charting::CustomLabel^  customLabel3 = (gcnew System::Windows::Forms::DataVisualization::Charting::CustomLabel());
            System::Windows::Forms::DataVisualization::Charting::CustomLabel^  customLabel4 = (gcnew System::Windows::Forms::DataVisualization::Charting::CustomLabel());
            System::Windows::Forms::DataVisualization::Charting::CustomLabel^  customLabel5 = (gcnew System::Windows::Forms::DataVisualization::Charting::CustomLabel());
            System::Windows::Forms::DataVisualization::Charting::CustomLabel^  customLabel6 = (gcnew System::Windows::Forms::DataVisualization::Charting::CustomLabel());
            System::Windows::Forms::DataVisualization::Charting::CustomLabel^  customLabel7 = (gcnew System::Windows::Forms::DataVisualization::Charting::CustomLabel());
            System::Windows::Forms::DataVisualization::Charting::CustomLabel^  customLabel8 = (gcnew System::Windows::Forms::DataVisualization::Charting::CustomLabel());
            System::Windows::Forms::DataVisualization::Charting::CustomLabel^  customLabel9 = (gcnew System::Windows::Forms::DataVisualization::Charting::CustomLabel());
            System::Windows::Forms::DataVisualization::Charting::CustomLabel^  customLabel10 = (gcnew System::Windows::Forms::DataVisualization::Charting::CustomLabel());
            System::Windows::Forms::DataVisualization::Charting::Legend^  legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
            System::Windows::Forms::DataVisualization::Charting::Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Title^  title1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Title());
            System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Fuel_Frm::typeid));
            this->_chFuel = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
            this->_chFuelYaxis = (gcnew System::Windows::Forms::CheckBox());
            this->_cmenuGraph = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->Print = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->PrintPreview = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->PrintPageSetup = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->saveToClipboardToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->label1 = (gcnew System::Windows::Forms::Label());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_chFuel))->BeginInit();
            this->_cmenuGraph->SuspendLayout();
            this->SuspendLayout();
            // 
            // _chFuel
            // 
            this->_chFuel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
                | System::Windows::Forms::AnchorStyles::Left) 
                | System::Windows::Forms::AnchorStyles::Right));
            chartArea1->Area3DStyle->Enable3D = true;
            chartArea1->Area3DStyle->Inclination = 10;
            chartArea1->Area3DStyle->IsClustered = true;
            chartArea1->Area3DStyle->Rotation = 10;
            customLabel1->GridTicks = System::Windows::Forms::DataVisualization::Charting::GridTickTypes::Gridline;
            customLabel1->Text = L"Litter";
            customLabel1->ToPosition = 1.5;
            customLabel2->FromPosition = 1.5;
            customLabel2->GridTicks = System::Windows::Forms::DataVisualization::Charting::GridTickTypes::Gridline;
            customLabel2->Text = L"1 Hr";
            customLabel2->ToPosition = 2.5;
            customLabel3->FromPosition = 2.5;
            customLabel3->Text = L"10 Hr";
            customLabel3->ToPosition = 3.5;
            customLabel4->FromPosition = 3.5;
            customLabel4->Text = L"100 Hr";
            customLabel4->ToPosition = 4.5;
            customLabel5->FromPosition = 4.5;
            customLabel5->GridTicks = System::Windows::Forms::DataVisualization::Charting::GridTickTypes::Gridline;
            customLabel5->Text = L"1000 Hr";
            customLabel5->ToPosition = 5.5;
            customLabel6->FromPosition = 5.5;
            customLabel6->GridTicks = System::Windows::Forms::DataVisualization::Charting::GridTickTypes::Gridline;
            customLabel6->Text = L"Duff";
            customLabel6->ToPosition = 6.5;
            customLabel7->FromPosition = 6.5;
            customLabel7->GridTicks = System::Windows::Forms::DataVisualization::Charting::GridTickTypes::Gridline;
            customLabel7->Text = L"Herb";
            customLabel7->ToPosition = 7.5;
            customLabel8->FromPosition = 7.5;
            customLabel8->GridTicks = System::Windows::Forms::DataVisualization::Charting::GridTickTypes::Gridline;
            customLabel8->Text = L"Shrub";
            customLabel8->ToPosition = 8.5;
            customLabel9->FromPosition = 8.5;
            customLabel9->GridTicks = System::Windows::Forms::DataVisualization::Charting::GridTickTypes::Gridline;
            customLabel9->Text = L"Foliage";
            customLabel9->ToPosition = 9.5;
            customLabel10->FromPosition = 9.5;
            customLabel10->GridTicks = System::Windows::Forms::DataVisualization::Charting::GridTickTypes::Gridline;
            customLabel10->Text = L"Branch";
            customLabel10->ToPosition = 10.5;
            chartArea1->AxisX->CustomLabels->Add(customLabel1);
            chartArea1->AxisX->CustomLabels->Add(customLabel2);
            chartArea1->AxisX->CustomLabels->Add(customLabel3);
            chartArea1->AxisX->CustomLabels->Add(customLabel4);
            chartArea1->AxisX->CustomLabels->Add(customLabel5);
            chartArea1->AxisX->CustomLabels->Add(customLabel6);
            chartArea1->AxisX->CustomLabels->Add(customLabel7);
            chartArea1->AxisX->CustomLabels->Add(customLabel8);
            chartArea1->AxisX->CustomLabels->Add(customLabel9);
            chartArea1->AxisX->CustomLabels->Add(customLabel10);
            chartArea1->AxisX->MajorGrid->Enabled = false;
            chartArea1->AxisX->TextOrientation = System::Windows::Forms::DataVisualization::Charting::TextOrientation::Horizontal;
            chartArea1->AxisX->TitleAlignment = System::Drawing::StringAlignment::Near;
            chartArea1->AxisY->MajorGrid->LineColor = System::Drawing::Color::LightGray;
            chartArea1->AxisY->TextOrientation = System::Windows::Forms::DataVisualization::Charting::TextOrientation::Rotated270;
            chartArea1->AxisY->Title = L"Loading - tons/acre  ";
            chartArea1->BackColor = System::Drawing::Color::White;
            chartArea1->Name = L"ChartArea1";
            chartArea1->Position->Auto = false;
            chartArea1->Position->Height = 80;
            chartArea1->Position->Width = 90;
            chartArea1->Position->X = 3;
            chartArea1->Position->Y = 10;
            this->_chFuel->ChartAreas->Add(chartArea1);
            legend1->Alignment = System::Drawing::StringAlignment::Center;
            legend1->BackColor = System::Drawing::Color::SeaShell;
            legend1->Docking = System::Windows::Forms::DataVisualization::Charting::Docking::Bottom;
            legend1->LegendStyle = System::Windows::Forms::DataVisualization::Charting::LegendStyle::Row;
            legend1->Name = L"Legend1";
            this->_chFuel->Legends->Add(legend1);
            this->_chFuel->Location = System::Drawing::Point(12, 12);
            this->_chFuel->Name = L"_chFuel";
            series1->BackHatchStyle = System::Windows::Forms::DataVisualization::Charting::ChartHatchStyle::LightUpwardDiagonal;
            series1->ChartArea = L"ChartArea1";
            series1->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
                static_cast<System::Int32>(static_cast<System::Byte>(0)));
            series1->CustomProperties = L"DrawSideBySide=True";
            series1->Legend = L"Legend1";
            series1->LegendText = L"Preburn Load ";
            series1->Name = L"Series1";
            series2->ChartArea = L"ChartArea1";
            series2->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)), 
                static_cast<System::Int32>(static_cast<System::Byte>(128)));
            series2->Legend = L"Legend1";
            series2->LegendText = L"Consumed Load";
            series2->Name = L"Series2";
            series3->ChartArea = L"ChartArea1";
            series3->Color = System::Drawing::Color::Gray;
            series3->Legend = L"Legend1";
            series3->LegendText = L"Postburn Load";
            series3->Name = L"Series3";
            this->_chFuel->Series->Add(series1);
            this->_chFuel->Series->Add(series2);
            this->_chFuel->Series->Add(series3);
            this->_chFuel->Size = System::Drawing::Size(601, 390);
            this->_chFuel->TabIndex = 2;
            this->_chFuel->Text = L"chart1";
            title1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            title1->Name = L"Title1";
            title1->Text = L"Preburn, Consumed and Postburn Fuel Loading";
            this->_chFuel->Titles->Add(title1);
            this->_chFuel->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &Fuel_Frm::_chFuel_MouseClick);
            // 
            // _chFuelYaxis
            // 
            this->_chFuelYaxis->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_chFuelYaxis->AutoSize = true;
            this->_chFuelYaxis->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F));
            this->_chFuelYaxis->Location = System::Drawing::Point(12, 408);
            this->_chFuelYaxis->Name = L"_chFuelYaxis";
            this->_chFuelYaxis->Size = System::Drawing::Size(61, 17);
            this->_chFuelYaxis->TabIndex = 3;
            this->_chFuelYaxis->Text = L"Fix Axis";
            this->_chFuelYaxis->UseVisualStyleBackColor = true;
            // 
            // _cmenuGraph
            // 
            this->_cmenuGraph->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->Print, this->PrintPreview, 
                this->PrintPageSetup, this->saveToolStripMenuItem, this->saveToClipboardToolStripMenuItem});
            this->_cmenuGraph->Name = L"_cmenuGraph";
            this->_cmenuGraph->Size = System::Drawing::Size(173, 114);
            this->_cmenuGraph->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &Fuel_Frm::_cmenuGraph_MouseClick);
            // 
            // Print
            // 
            this->Print->Name = L"Print";
            this->Print->Size = System::Drawing::Size(172, 22);
            this->Print->Text = L"Print";
            this->Print->Click += gcnew System::EventHandler(this, &Fuel_Frm::Print_Click);
            // 
            // PrintPreview
            // 
            this->PrintPreview->Name = L"PrintPreview";
            this->PrintPreview->Size = System::Drawing::Size(172, 22);
            this->PrintPreview->Text = L"Print Preview";
            this->PrintPreview->Click += gcnew System::EventHandler(this, &Fuel_Frm::Print_Click);
            // 
            // PrintPageSetup
            // 
            this->PrintPageSetup->Name = L"PrintPageSetup";
            this->PrintPageSetup->Size = System::Drawing::Size(172, 22);
            this->PrintPageSetup->Text = L"Print Page Setup";
            this->PrintPageSetup->Click += gcnew System::EventHandler(this, &Fuel_Frm::Print_Click);
            // 
            // saveToolStripMenuItem
            // 
            this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
            this->saveToolStripMenuItem->Size = System::Drawing::Size(172, 22);
            this->saveToolStripMenuItem->Text = L"Save To File";
            this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &Fuel_Frm::Print_Click);
            // 
            // saveToClipboardToolStripMenuItem
            // 
            this->saveToClipboardToolStripMenuItem->Name = L"saveToClipboardToolStripMenuItem";
            this->saveToClipboardToolStripMenuItem->Size = System::Drawing::Size(172, 22);
            this->saveToClipboardToolStripMenuItem->Text = L"Save To Clipboard";
            this->saveToClipboardToolStripMenuItem->Click += gcnew System::EventHandler(this, &Fuel_Frm::Print_Click);
            // 
            // label1
            // 
            this->label1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(151, 412);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(252, 13);
            this->label1->TabIndex = 5;
            this->label1->Text = L"Right Mouse Click Graph for Print and Save Options";
            // 
            // Fuel_Frm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(625, 434);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->_chFuel);
            this->Controls->Add(this->_chFuelYaxis);
            this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
            this->Name = L"Fuel_Frm";
            this->Text = L"FOFEM Fuels";
            this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Fuel_Frm::Fuel_Frm_FormClosing);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_chFuel))->EndInit();
            this->_cmenuGraph->ResumeLayout(false);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
   
/****************************************************************************
* Form Closing upper-right X clicked 
*****************************************************************************/
private: System::Void Fuel_Frm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
{
  this->Hide();          
  e->Cancel = true; 
}
   


 private: System::Void _cmenuGraph_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
 {
 }

/****************************************************************
* Contextmenu Item Clicked 
****************************************************************/
private: System::Void Print_Click(System::Object^  sender, System::EventArgs^  e) 
{
String ^ Str; 

  Str =  sender->ToString();
 
  if ( Str == "Print") 
    this->_chFuel->Printing->Print(true);
  else if ( Str == "Print Preview") 
    this->_chFuel->Printing->PrintPreview(); 
  else if ( Str == "Print Page Setup" ) 
   this->_chFuel->Printing->PageSetup(); 
 
  else if ( Str == "Save To Clipboard" ) {
   System::IO::MemoryStream ^ stream   = gcnew System::IO::MemoryStream(); 
   this->_chFuel->SaveImage (stream, System::Drawing::Imaging::ImageFormat::Bmp); 
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
       this->_chFuel->SaveImage ( saveFileDialog1->FileName,  System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Bmp); 
    if ( saveFileDialog1->FileName->EndsWith( "jpg" ) )
      this->_chFuel->SaveImage (saveFileDialog1->FileName,  System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Jpeg); 
    if ( saveFileDialog1->FileName->EndsWith( "gif" ) )
     this->_chFuel->SaveImage (saveFileDialog1->FileName, System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Gif); 
    if ( saveFileDialog1->FileName->EndsWith( "png" ) )
     this->_chFuel->SaveImage (saveFileDialog1->FileName, System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Png); 
    if ( saveFileDialog1->FileName->EndsWith( "emf" ) )
      this->_chFuel->SaveImage (saveFileDialog1->FileName, System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Emf); 
    if ( saveFileDialog1->FileName->EndsWith( "tif" ) )
      this->_chFuel->SaveImage (saveFileDialog1->FileName,System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Tiff); 
  } /* else if Save To File */
 
}


/*************************************************************************
* Mouse Click On Fuel Graph
***************************************************************************/
private: System::Void _chFuel_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
 {
int width, height; 
bool found; 
// System::Windows::Forms::DataVisualization::Charting::Chart^  _chart;
String ^Str; 

/* Now was it a Right Mouse Click */
   if ( e->Button == Windows::Forms::MouseButtons::Left )  /* We Want Right Mouse BUtton */
     return; 

/* So we can position the ContextMenu in the middle of the chart */
  width = this->_chFuel->Size.Width; 
  height = this->_chFuel->Size.Height;

  width = width / 2;
  height = height / 2; 
  this->_cmenuGraph->Show(this->_chFuel, width,height);  /* Popup the ContextMenu */ 
}


/******************************************************************************/
};
// }
