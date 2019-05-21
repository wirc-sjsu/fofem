#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


/*****************************************************************/
public ref class Mort_Frm : public System::Windows::Forms::Form
{
public:

   int UpdateGraph (char cr_Mode[], float Prob[],float FS[],  String ^ Str, int iX);

  int  Set_SecondAxis ();

  int InitGraph (char cr_FS[]); 
  int InitGraphPFI (); 
  int InitGraphBol (float max, float intv); 

/*-------------------------------------*/
/* Constructor */
Mort_Frm(void)
{
  InitializeComponent();
}


~Mort_Frm()
{
  if (components){
	delete components;}
}
public: System::Windows::Forms::DataVisualization::Charting::Chart^  _chMort;
public: System::Windows::Forms::ContextMenuStrip^  _cmenuGraph;
public: System::Windows::Forms::ToolStripMenuItem^  Print;
public: System::Windows::Forms::ToolStripMenuItem^  PrintPreview;
public: System::Windows::Forms::ToolStripMenuItem^  PrintPageSetup;
public: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  saveToClipboardToolStripMenuItem;
private: System::Windows::Forms::Label^  label1;
private: System::ComponentModel::IContainer^  components;
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
            System::Windows::Forms::DataVisualization::Charting::Title^  title1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Title());
            System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Mort_Frm::typeid));
            this->_chMort = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
            this->_cmenuGraph = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->Print = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->PrintPreview = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->PrintPageSetup = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->saveToClipboardToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->label1 = (gcnew System::Windows::Forms::Label());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_chMort))->BeginInit();
            this->_cmenuGraph->SuspendLayout();
            this->SuspendLayout();
            // 
            // _chMort
            // 
            this->_chMort->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
                | System::Windows::Forms::AnchorStyles::Left) 
                | System::Windows::Forms::AnchorStyles::Right));
            this->_chMort->BorderlineColor = System::Drawing::Color::DimGray;
            this->_chMort->BorderlineDashStyle = System::Windows::Forms::DataVisualization::Charting::ChartDashStyle::Solid;
            this->_chMort->BorderlineWidth = 2;
            chartArea1->Area3DStyle->Enable3D = true;
            chartArea1->Area3DStyle->Inclination = 0;
            chartArea1->Area3DStyle->Rotation = 0;
            chartArea1->AxisX->Enabled = System::Windows::Forms::DataVisualization::Charting::AxisEnabled::True;
            chartArea1->AxisX->Interval = 20;
            chartArea1->AxisX->LineColor = System::Drawing::Color::LightGray;
            chartArea1->AxisX->MajorGrid->LineColor = System::Drawing::Color::LightGray;
            chartArea1->AxisX->Maximum = 240;
            chartArea1->AxisX->Minimum = 0;
            chartArea1->AxisX->ScrollBar->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), 
                static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)));
            chartArea1->AxisX->ScrollBar->ButtonColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), 
                static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
            chartArea1->AxisX->TextOrientation = System::Windows::Forms::DataVisualization::Charting::TextOrientation::Horizontal;
            chartArea1->AxisX->Title = L"Flame Length - feet";
            chartArea1->AxisX2->Enabled = System::Windows::Forms::DataVisualization::Charting::AxisEnabled::False;
            chartArea1->AxisX2->Interval = 10;
            chartArea1->AxisX2->MajorGrid->Enabled = false;
            chartArea1->AxisX2->Maximum = 20;
            chartArea1->AxisX2->Minimum = 0;
            chartArea1->AxisX2->ScaleBreakStyle->Enabled = true;
            chartArea1->AxisX2->Title = L"Flame Length - feet";
            chartArea1->AxisY->LineColor = System::Drawing::Color::LightGray;
            chartArea1->AxisY->MajorGrid->LineColor = System::Drawing::Color::LightGray;
            chartArea1->AxisY->Title = L"Mortality - percent";
            chartArea1->BackColor = System::Drawing::Color::White;
            chartArea1->BackSecondaryColor = System::Drawing::Color::White;
            chartArea1->CursorY->Position = 60;
            chartArea1->Name = L"ChartArea1";
            this->_chMort->ChartAreas->Add(chartArea1);
            this->_chMort->ContextMenuStrip = this->_cmenuGraph;
            legend1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)), 
                static_cast<System::Int32>(static_cast<System::Byte>(224)));
            legend1->BorderColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(0)), 
                static_cast<System::Int32>(static_cast<System::Byte>(0)));
            legend1->Docking = System::Windows::Forms::DataVisualization::Charting::Docking::Left;
            legend1->LegendStyle = System::Windows::Forms::DataVisualization::Charting::LegendStyle::Column;
            legend1->Name = L"Legend1";
            this->_chMort->Legends->Add(legend1);
            this->_chMort->Location = System::Drawing::Point(12, 12);
            this->_chMort->Name = L"_chMort";
            series1->BorderWidth = 2;
            series1->ChartArea = L"ChartArea1";
            series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
            series1->Color = System::Drawing::Color::Red;
            series1->Legend = L"Legend1";
            series1->Name = L"Species1";
            series1->XAxisType = System::Windows::Forms::DataVisualization::Charting::AxisType::Secondary;
            this->_chMort->Series->Add(series1);
            this->_chMort->Size = System::Drawing::Size(658, 380);
            this->_chMort->TabIndex = 2;
            this->_chMort->Text = L"chart1";
            title1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            title1->Name = L"MainTitle";
            title1->Text = L"Tree Mortality";
            this->_chMort->Titles->Add(title1);
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
            this->Print->Click += gcnew System::EventHandler(this, &Mort_Frm::Print_Click);
            // 
            // PrintPreview
            // 
            this->PrintPreview->Name = L"PrintPreview";
            this->PrintPreview->Size = System::Drawing::Size(172, 22);
            this->PrintPreview->Text = L"Print Preview";
            this->PrintPreview->Click += gcnew System::EventHandler(this, &Mort_Frm::Print_Click);
            // 
            // PrintPageSetup
            // 
            this->PrintPageSetup->Name = L"PrintPageSetup";
            this->PrintPageSetup->Size = System::Drawing::Size(172, 22);
            this->PrintPageSetup->Text = L"Print Page Setup";
            this->PrintPageSetup->Click += gcnew System::EventHandler(this, &Mort_Frm::Print_Click);
            // 
            // saveToolStripMenuItem
            // 
            this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
            this->saveToolStripMenuItem->Size = System::Drawing::Size(172, 22);
            this->saveToolStripMenuItem->Text = L"Save To File";
            this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &Mort_Frm::Print_Click);
            // 
            // saveToClipboardToolStripMenuItem
            // 
            this->saveToClipboardToolStripMenuItem->Name = L"saveToClipboardToolStripMenuItem";
            this->saveToClipboardToolStripMenuItem->Size = System::Drawing::Size(172, 22);
            this->saveToClipboardToolStripMenuItem->Text = L"Save To Clipboard";
            this->saveToClipboardToolStripMenuItem->Click += gcnew System::EventHandler(this, &Mort_Frm::Print_Click);
            // 
            // label1
            // 
            this->label1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->label1->AutoSize = true;
            this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)), 
                System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
            this->label1->Location = System::Drawing::Point(177, 395);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(337, 15);
            this->label1->TabIndex = 6;
            this->label1->Text = L"Right Mouse Click Graph for Print and Save Options";
            // 
            // Mort_Frm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->BackColor = System::Drawing::SystemColors::Control;
            this->ClientSize = System::Drawing::Size(682, 419);
            this->Controls->Add(this->_chMort);
            this->Controls->Add(this->label1);
            this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
            this->Name = L"Mort_Frm";
            this->Text = L"Species Mortality";
            this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Mort_Frm::Mort_Frm_FormClosing);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_chMort))->EndInit();
            this->_cmenuGraph->ResumeLayout(false);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion

/***************************************************************************
* Mort Form Closing - upper corner X clicked 
****************************************************************************/
private: System::Void Mort_Frm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
 {
  this->Hide();
  e->Cancel = true; 
 }

/*************************************************************************
* Context Menu Item Clicked 
**************************************************************************/
private: System::Void Print_Click(System::Object^  sender, System::EventArgs^  e)
{
String ^ Str; 

  Str =  sender->ToString();
 
  if ( Str == "Print") 
    this->_chMort->Printing->Print(true);
  else if ( Str == "Print Preview") 
    this->_chMort->Printing->PrintPreview(); 
  else if ( Str == "Print Page Setup" ) 
   this->_chMort->Printing->PageSetup(); 
 
  else if ( Str == "Save To Clipboard" ) {
   System::IO::MemoryStream ^ stream   = gcnew System::IO::MemoryStream(); 
   this->_chMort->SaveImage (stream, System::Drawing::Imaging::ImageFormat::Bmp); 
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
       this->_chMort->SaveImage ( saveFileDialog1->FileName,  System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Bmp); 
    if ( saveFileDialog1->FileName->EndsWith( "jpg" ) )
      this->_chMort->SaveImage (saveFileDialog1->FileName,  System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Jpeg); 
    if ( saveFileDialog1->FileName->EndsWith( "gif" ) )
     this->_chMort->SaveImage (saveFileDialog1->FileName, System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Gif); 
    if ( saveFileDialog1->FileName->EndsWith( "png" ) )
     this->_chMort->SaveImage (saveFileDialog1->FileName, System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Png); 
    if ( saveFileDialog1->FileName->EndsWith( "emf" ) )
      this->_chMort->SaveImage (saveFileDialog1->FileName, System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Emf); 
    if ( saveFileDialog1->FileName->EndsWith( "tif" ) )
      this->_chMort->SaveImage (saveFileDialog1->FileName,System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Tiff); 
  } /* else if Save To File */

}


};

