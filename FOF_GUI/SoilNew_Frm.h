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

/****************************************************************
* Name: SoilNew_Frm
* Desc: New Soil Graphing Window
****************************************************************/
public ref class SoilNew_Frm : public System::Windows::Forms::Form
{
public:

   d_SI *a_SI;
private: System::Windows::Forms::TextBox^  _txTime;
private: System::Windows::Forms::Button^  _btRun;
private: System::Windows::Forms::TextBox^  _txEndAt;
public: 
public: 
    d_SO *a_SO;
   int Update ( float f, int i); 

   int Reset (); 
   int Run  (); 

   int UpdateDraw(); 

/****************************************
* Constructure 
****************************************/
SoilNew_Frm(void) {
	InitializeComponent();
    a_SI = new d_SI; 
    a_SO = new d_SO; 
}

/***************************************
* Destructore 
***************************************/
~SoilNew_Frm()
{
  delete a_SI;
  delete a_SO;

  if (components) {
	delete components;
  }
}
private: System::Windows::Forms::DataVisualization::Charting::Chart^  _chSoil;
public: 

/**************************************/
	
		System::ComponentModel::Container ^components;


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
            System::Windows::Forms::DataVisualization::Charting::Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Series^  series2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            this->_chSoil = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
            this->_txTime = (gcnew System::Windows::Forms::TextBox());
            this->_btRun = (gcnew System::Windows::Forms::Button());
            this->_txEndAt = (gcnew System::Windows::Forms::TextBox());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_chSoil))->BeginInit();
            this->SuspendLayout();
            // 
            // _chSoil
            // 
            chartArea1->Area3DStyle->Enable3D = true;
            chartArea1->AxisX->Title = L"Soil Layers ";
            chartArea1->AxisY->Title = L"Temperature (c)";
            chartArea1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)), 
                static_cast<System::Int32>(static_cast<System::Byte>(224)));
            chartArea1->Name = L"ChartArea1";
            this->_chSoil->ChartAreas->Add(chartArea1);
            this->_chSoil->Location = System::Drawing::Point(27, 123);
            this->_chSoil->Name = L"_chSoil";
            series1->ChartArea = L"ChartArea1";
            series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Bar;
            series1->CustomProperties = L"DrawingStyle=Emboss, PointWidth=0.9";
            series1->Name = L"Lay1";
            series1->YValuesPerPoint = 2;
            series2->ChartArea = L"ChartArea1";
            series2->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Bar;
            series2->Name = L"Lay2";
            this->_chSoil->Series->Add(series1);
            this->_chSoil->Series->Add(series2);
            this->_chSoil->Size = System::Drawing::Size(473, 320);
            this->_chSoil->TabIndex = 0;
            this->_chSoil->Text = L"chart1";
            // 
            // _txTime
            // 
            this->_txTime->Location = System::Drawing::Point(255, 46);
            this->_txTime->Name = L"_txTime";
            this->_txTime->Size = System::Drawing::Size(71, 20);
            this->_txTime->TabIndex = 1;
            this->_txTime->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _btRun
            // 
            this->_btRun->Location = System::Drawing::Point(80, 45);
            this->_btRun->Name = L"_btRun";
            this->_btRun->Size = System::Drawing::Size(71, 32);
            this->_btRun->TabIndex = 2;
            this->_btRun->Text = L"Run";
            this->_btRun->UseVisualStyleBackColor = true;
            this->_btRun->Click += gcnew System::EventHandler(this, &SoilNew_Frm::_btRun_Click);
            // 
            // _txEndAt
            // 
            this->_txEndAt->Location = System::Drawing::Point(401, 50);
            this->_txEndAt->Name = L"_txEndAt";
            this->_txEndAt->Size = System::Drawing::Size(82, 20);
            this->_txEndAt->TabIndex = 3;
            this->_txEndAt->Text = L"200";
            this->_txEndAt->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // SoilNew_Frm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(624, 525);
            this->Controls->Add(this->_txEndAt);
            this->Controls->Add(this->_btRun);
            this->Controls->Add(this->_txTime);
            this->Controls->Add(this->_chSoil);
            this->Name = L"SoilNew_Frm";
            this->Text = L"SoilNew_Frm";
            this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &SoilNew_Frm::SoilNew_Frm_FormClosing);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_chSoil))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion


/*****************************************************************************************
* Form Closing, User hit 'X' corner button to close,
* We just want to hide the form, so we interecept, or else the form would close.
******************************************************************************************/
private: System::Void SoilNew_Frm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) 
{
 this->Hide();
  e->Cancel = true; 
}

/************************************************************
* Run Button Clicked 
**************************************************************/
private: System::Void _btRun_Click(System::Object^  sender, System::EventArgs^  e) 
{
  this->Run(); 
}



};

