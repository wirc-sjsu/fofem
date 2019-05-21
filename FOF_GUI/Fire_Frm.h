/*****************************************************************
* Name: Fire_Frm.h
* Desc: Fire Intenisy Graph Form
*****************************************************************/
#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "fof_sgv.h"
#include "fof_co.h" 

/***********************************************************************
* Fire Intensity Graph Form 
*
**********************************************************************/
public ref class Fire_Frm : public System::Windows::Forms::Form
{
public:
  float *fr_Int;
  int   i_Cnt;

private: System::Windows::Forms::TextBox^  _txXaxis;
private: System::Windows::Forms::TextBox^  _txYaxis;
private: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::Label^  label2;
public: System::Windows::Forms::ContextMenuStrip^  _cmenuGraph;
private: 
public: System::Windows::Forms::ToolStripMenuItem^  Print;
public: System::Windows::Forms::ToolStripMenuItem^  PrintPreview;
public: System::Windows::Forms::ToolStripMenuItem^  PrintPageSetup;
public: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  saveToClipboardToolStripMenuItem;
private: System::Windows::Forms::Label^  _lbMaxInt;
public: 
  int   i_Minutes; 

 int  Update (d_CO *a_CO); 
 int  Clear (); 

 void Set_XAxis (); 
 void Set_YAxis (); 
 float f_LstMaxX; 
 float f_LstMaxY; 

/*---------------------------------------------*/
 Fire_Frm(void)
{
	InitializeComponent();
    f_LstMaxX = 10; 
    f_LstMaxY = 100; 
}

/*---------------------------------------------*/ 
~Fire_Frm()
{
	if (components)
	  delete components;
}

public: System::Windows::Forms::DataVisualization::Charting::Chart^  _chGrap_FirInt;
private: System::ComponentModel::IContainer^  components;
public: 

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->components = (gcnew System::ComponentModel::Container());
            System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea4 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
            System::Windows::Forms::DataVisualization::Charting::Series^  series4 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Title^  title4 = (gcnew System::Windows::Forms::DataVisualization::Charting::Title());
            this->_chGrap_FirInt = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
            this->_txXaxis = (gcnew System::Windows::Forms::TextBox());
            this->_txYaxis = (gcnew System::Windows::Forms::TextBox());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->_cmenuGraph = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->Print = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->PrintPreview = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->PrintPageSetup = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->saveToClipboardToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->_lbMaxInt = (gcnew System::Windows::Forms::Label());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_chGrap_FirInt))->BeginInit();
            this->_cmenuGraph->SuspendLayout();
            this->SuspendLayout();
            // 
            // _chGrap_FirInt
            // 
            this->_chGrap_FirInt->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
                | System::Windows::Forms::AnchorStyles::Left) 
                | System::Windows::Forms::AnchorStyles::Right));
            this->_chGrap_FirInt->BackGradientStyle = System::Windows::Forms::DataVisualization::Charting::GradientStyle::DiagonalRight;
            this->_chGrap_FirInt->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
            this->_chGrap_FirInt->BackImageTransparentColor = System::Drawing::Color::White;
            this->_chGrap_FirInt->BorderlineColor = System::Drawing::Color::DimGray;
            this->_chGrap_FirInt->BorderlineWidth = 4;
            this->_chGrap_FirInt->BorderSkin->BackHatchStyle = System::Windows::Forms::DataVisualization::Charting::ChartHatchStyle::Cross;
            chartArea4->Area3DStyle->Inclination = 5;
            chartArea4->Area3DStyle->IsRightAngleAxes = false;
            chartArea4->Area3DStyle->PointDepth = 300;
            chartArea4->Area3DStyle->PointGapDepth = 200;
            chartArea4->Area3DStyle->Rotation = 5;
            chartArea4->Area3DStyle->WallWidth = 3;
            chartArea4->AxisX->Title = L"Minutes";
            chartArea4->AxisY->Title = L"Kilowatts per Meter Squared";
            chartArea4->BackColor = System::Drawing::Color::White;
            chartArea4->BackHatchStyle = System::Windows::Forms::DataVisualization::Charting::ChartHatchStyle::Divot;
            chartArea4->BackImageTransparentColor = System::Drawing::Color::White;
            chartArea4->BackSecondaryColor = System::Drawing::Color::White;
            chartArea4->BorderWidth = 4;
            chartArea4->Name = L"ChartArea1";
            this->_chGrap_FirInt->ChartAreas->Add(chartArea4);
            this->_chGrap_FirInt->Location = System::Drawing::Point(11, 11);
            this->_chGrap_FirInt->Margin = System::Windows::Forms::Padding(2);
            this->_chGrap_FirInt->Name = L"_chGrap_FirInt";
            series4->BorderWidth = 2;
            series4->ChartArea = L"ChartArea1";
            series4->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
            series4->Color = System::Drawing::Color::Red;
            series4->LegendText = L"Fire Int.";
            series4->Name = L"FirInt";
            this->_chGrap_FirInt->Series->Add(series4);
            this->_chGrap_FirInt->Size = System::Drawing::Size(719, 251);
            this->_chGrap_FirInt->TabIndex = 8;
            this->_chGrap_FirInt->Text = L"chart1";
            title4->Alignment = System::Drawing::ContentAlignment::TopLeft;
            title4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            title4->Name = L"Title1";
            title4->Text = L"Fire Intensity";
            this->_chGrap_FirInt->Titles->Add(title4);
            this->_chGrap_FirInt->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &Fire_Frm::_chGrap_FirInt_MouseClick);
            // 
            // _txXaxis
            // 
            this->_txXaxis->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_txXaxis->Location = System::Drawing::Point(584, 278);
            this->_txXaxis->Name = L"_txXaxis";
            this->_txXaxis->Size = System::Drawing::Size(41, 20);
            this->_txXaxis->TabIndex = 10;
            this->_txXaxis->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->_txXaxis->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Fire_Frm::_txXaxis_KeyDown);
            // 
            // _txYaxis
            // 
            this->_txYaxis->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->_txYaxis->Location = System::Drawing::Point(480, 278);
            this->_txYaxis->Name = L"_txYaxis";
            this->_txYaxis->Size = System::Drawing::Size(41, 20);
            this->_txYaxis->TabIndex = 11;
            this->_txYaxis->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->_txYaxis->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Fire_Frm::_txYaxis_KeyDown);
            // 
            // label1
            // 
            this->label1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->label1->AutoSize = true;
            this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label1->Location = System::Drawing::Point(631, 280);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(40, 15);
            this->label1->TabIndex = 12;
            this->label1->Text = L"X Axis";
            // 
            // label2
            // 
            this->label2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->label2->AutoSize = true;
            this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label2->Location = System::Drawing::Point(526, 280);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(39, 15);
            this->label2->TabIndex = 13;
            this->label2->Text = L"Y Axis";
            this->label2->Click += gcnew System::EventHandler(this, &Fire_Frm::label2_Click);
            // 
            // _cmenuGraph
            // 
            this->_cmenuGraph->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->Print, this->PrintPreview, 
                this->PrintPageSetup, this->saveToolStripMenuItem, this->saveToClipboardToolStripMenuItem});
            this->_cmenuGraph->Name = L"_cmenuGraph";
            this->_cmenuGraph->Size = System::Drawing::Size(162, 114);
            // 
            // Print
            // 
            this->Print->Name = L"Print";
            this->Print->Size = System::Drawing::Size(161, 22);
            this->Print->Text = L"Print";
            this->Print->Click += gcnew System::EventHandler(this, &Fire_Frm::Print_Click);
            // 
            // PrintPreview
            // 
            this->PrintPreview->Name = L"PrintPreview";
            this->PrintPreview->Size = System::Drawing::Size(161, 22);
            this->PrintPreview->Text = L"Print Preview";
            this->PrintPreview->Click += gcnew System::EventHandler(this, &Fire_Frm::Print_Click);
            // 
            // PrintPageSetup
            // 
            this->PrintPageSetup->Name = L"PrintPageSetup";
            this->PrintPageSetup->Size = System::Drawing::Size(161, 22);
            this->PrintPageSetup->Text = L"Print Page Setup";
            this->PrintPageSetup->Click += gcnew System::EventHandler(this, &Fire_Frm::Print_Click);
            // 
            // saveToolStripMenuItem
            // 
            this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
            this->saveToolStripMenuItem->Size = System::Drawing::Size(161, 22);
            this->saveToolStripMenuItem->Text = L"Save To File";
            this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &Fire_Frm::Print_Click);
            // 
            // saveToClipboardToolStripMenuItem
            // 
            this->saveToClipboardToolStripMenuItem->Name = L"saveToClipboardToolStripMenuItem";
            this->saveToClipboardToolStripMenuItem->Size = System::Drawing::Size(161, 22);
            this->saveToClipboardToolStripMenuItem->Text = L"Save To Clipboard";
            this->saveToClipboardToolStripMenuItem->Click += gcnew System::EventHandler(this, &Fire_Frm::Print_Click);
            // 
            // _lbMaxInt
            // 
            this->_lbMaxInt->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
            this->_lbMaxInt->AutoSize = true;
            this->_lbMaxInt->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lbMaxInt->Location = System::Drawing::Point(100, 280);
            this->_lbMaxInt->Name = L"_lbMaxInt";
            this->_lbMaxInt->Size = System::Drawing::Size(165, 15);
            this->_lbMaxInt->TabIndex = 14;
            this->_lbMaxInt->Text = L"Max Intensity.............................";
            // 
            // Fire_Frm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(741, 307);
            this->Controls->Add(this->_lbMaxInt);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->_txYaxis);
            this->Controls->Add(this->_chGrap_FirInt);
            this->Controls->Add(this->_txXaxis);
            this->Name = L"Fire_Frm";
            this->Text = L"Fire Intensity";
            this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Fire_Frm::Fire_Frm_FormClosing);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_chGrap_FirInt))->EndInit();
            this->_cmenuGraph->ResumeLayout(false);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion


/********************************************************
*  Form Closing - X in upper right corner of Form was 
*  click.
*  We don't want the form to actually close, just hide it
*  if it closes it loses its information
*********************************************************/	
private: System::Void Fire_Frm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) 
{
  this->Hide();
  e->Cancel = true;     
}

/********************************************************************
* X Axis TextBox - when text changes redo the axis 
**********************************************************************/
private: System::Void _txXaxis_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
 {
  if ( e->KeyCode ==   System::Windows::Forms::Keys::Enter)
     this->Set_XAxis(); 
  }

/********************************************************************
* Y Axis TextBox - when text changes redo the axis 
**********************************************************************/
private: System::Void _txYaxis_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{
  if ( e->KeyCode ==   System::Windows::Forms::Keys::Enter)
     this->Set_YAxis(); 
}


private: System::Void label2_Click(System::Object^  sender, System::EventArgs^  e)
{}


/*********************************************************
* Mouse Click on the Fire Intesity Graph Chart area
*  this positions the popup context menu (print,etc)
**********************************************************/
private: System::Void _chGrap_FirInt_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
int width, height; 
bool found; 
// System::Windows::Forms::DataVisualization::Charting::Chart^  _chart;
String ^Str; 

/* Now was it a Right Mouse Click */
   if ( e->Button == Windows::Forms::MouseButtons::Left )  /* We Want Right Mouse BUtton */
     return; 

/* So we can position the ContextMenu in the middle of the chart */
  width = this->_chGrap_FirInt->Size.Width; 
  height = this->_chGrap_FirInt->Size.Height;

  width = width / 2;
  height = height / 2; 
  this->_cmenuGraph->Show(this->_chGrap_FirInt, width,height);  /* Popup the ContextMenu */ 
}

/**************************************************************
*
*
**************************************************************/
private: System::Void Print_Click(System::Object^  sender, System::EventArgs^  e)
 {
String ^ Str; 

  Str =  sender->ToString();
 
  if ( Str == "Print") 
    this->_chGrap_FirInt->Printing->Print(true);
  else if ( Str == "Print Preview") 
    this->_chGrap_FirInt->Printing->PrintPreview(); 
  else if ( Str == "Print Page Setup" ) 
   this->_chGrap_FirInt->Printing->PageSetup(); 
 
  else if ( Str == "Save To Clipboard" ) {
   System::IO::MemoryStream ^ stream   = gcnew System::IO::MemoryStream(); 
   this->_chGrap_FirInt->SaveImage (stream, System::Drawing::Imaging::ImageFormat::Bmp); 
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
       this->_chGrap_FirInt->SaveImage ( saveFileDialog1->FileName,  System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Bmp); 
    if ( saveFileDialog1->FileName->EndsWith( "jpg" ) )
      this->_chGrap_FirInt->SaveImage (saveFileDialog1->FileName,  System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Jpeg); 
    if ( saveFileDialog1->FileName->EndsWith( "gif" ) )
     this->_chGrap_FirInt->SaveImage (saveFileDialog1->FileName, System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Gif); 
    if ( saveFileDialog1->FileName->EndsWith( "png" ) )
     this->_chGrap_FirInt->SaveImage (saveFileDialog1->FileName, System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Png); 
    if ( saveFileDialog1->FileName->EndsWith( "emf" ) )
      this->_chGrap_FirInt->SaveImage (saveFileDialog1->FileName, System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Emf); 
    if ( saveFileDialog1->FileName->EndsWith( "tif" ) )
      this->_chGrap_FirInt->SaveImage (saveFileDialog1->FileName,System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Tiff); 
  } /* else if Save To File */
 

 }



};

