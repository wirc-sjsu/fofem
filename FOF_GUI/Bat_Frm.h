#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


/******************************************************************************
* Batch Processing Form Class
*******************************************************************************/
public ref class Bat_Frm : public System::Windows::Forms::Form
{
public:
   
   String ^ s_FilePath; 
   void Set_FilePath (String ^); 

   int Create_Sample(); 

   void Batch_Run (); 
   int Input (); 


/*--------------------------------------------------------*/
/* Constructor */ 
Bat_Frm(void)
{
  InitializeComponent();		
}
/*-------------------------------------------------------*/
~Bat_Frm()
{
  if (components){
	delete components;}
}

/***********************************************************/
private: System::Windows::Forms::RadioButton^  _rbConEmi;
private: System::Windows::Forms::RadioButton^  _rbConEmiSoi;
private: System::Windows::Forms::RadioButton^  _rbTreeMort;
private: System::Windows::Forms::GroupBox^  _gbModelType;
private: System::Windows::Forms::Button^  _btSample;
private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;
private: System::Windows::Forms::Button^  _btInput;
private: System::Windows::Forms::TextBox^  _txInputFile;
private: System::Windows::Forms::TextBox^  _txOutputFN;
private: System::Windows::Forms::Button^  _btRun;
private: System::Windows::Forms::TextBox^  _txStands;
private: System::Windows::Forms::TextBox^  _txErrors;
private: System::Windows::Forms::Label^  _lbStands;
private: System::Windows::Forms::Label^  _lbErrors;
private: System::Windows::Forms::TextBox^  _txErrorFile;

private: System::Windows::Forms::Label^  _lbErrFile;
private: System::Windows::Forms::Label^  _lbOutFN;
private: System::Windows::Forms::Label^  _lbNote1;
private: System::Windows::Forms::Label^  _lbNote2;
private: System::Windows::Forms::RadioButton^  _rbHeadYes;
private: System::Windows::Forms::RadioButton^  _rbHeadNo;
private: System::Windows::Forms::GroupBox^  groupBox1;
private: System::Windows::Forms::Label^  _lbHead2;
private: System::Windows::Forms::Label^  _lbHead1;
private: System::Windows::Forms::GroupBox^  _gb_FlaSco;
private: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::RadioButton^  _rbScoHei;

private: System::Windows::Forms::RadioButton^  _rbFlaLen;
private: System::Windows::Forms::TextBox^  _txFlaSco;
private: System::Windows::Forms::CheckBox^  _cbColRep;






public: 

public: 




	
System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->_rbConEmi = (gcnew System::Windows::Forms::RadioButton());
            this->_rbConEmiSoi = (gcnew System::Windows::Forms::RadioButton());
            this->_rbTreeMort = (gcnew System::Windows::Forms::RadioButton());
            this->_gbModelType = (gcnew System::Windows::Forms::GroupBox());
            this->_btSample = (gcnew System::Windows::Forms::Button());
            this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
            this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
            this->_btInput = (gcnew System::Windows::Forms::Button());
            this->_txInputFile = (gcnew System::Windows::Forms::TextBox());
            this->_txOutputFN = (gcnew System::Windows::Forms::TextBox());
            this->_btRun = (gcnew System::Windows::Forms::Button());
            this->_txStands = (gcnew System::Windows::Forms::TextBox());
            this->_txErrors = (gcnew System::Windows::Forms::TextBox());
            this->_lbStands = (gcnew System::Windows::Forms::Label());
            this->_lbErrors = (gcnew System::Windows::Forms::Label());
            this->_txErrorFile = (gcnew System::Windows::Forms::TextBox());
            this->_lbErrFile = (gcnew System::Windows::Forms::Label());
            this->_lbOutFN = (gcnew System::Windows::Forms::Label());
            this->_lbNote1 = (gcnew System::Windows::Forms::Label());
            this->_lbNote2 = (gcnew System::Windows::Forms::Label());
            this->_rbHeadYes = (gcnew System::Windows::Forms::RadioButton());
            this->_rbHeadNo = (gcnew System::Windows::Forms::RadioButton());
            this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
            this->_lbHead2 = (gcnew System::Windows::Forms::Label());
            this->_lbHead1 = (gcnew System::Windows::Forms::Label());
            this->_gb_FlaSco = (gcnew System::Windows::Forms::GroupBox());
            this->_rbScoHei = (gcnew System::Windows::Forms::RadioButton());
            this->_rbFlaLen = (gcnew System::Windows::Forms::RadioButton());
            this->_txFlaSco = (gcnew System::Windows::Forms::TextBox());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->_cbColRep = (gcnew System::Windows::Forms::CheckBox());
            this->_gbModelType->SuspendLayout();
            this->groupBox1->SuspendLayout();
            this->_gb_FlaSco->SuspendLayout();
            this->SuspendLayout();
            // 
            // _rbConEmi
            // 
            this->_rbConEmi->AutoSize = true;
            this->_rbConEmi->Location = System::Drawing::Point(16, 21);
            this->_rbConEmi->Name = L"_rbConEmi";
            this->_rbConEmi->Size = System::Drawing::Size(174, 20);
            this->_rbConEmi->TabIndex = 0;
            this->_rbConEmi->TabStop = true;
            this->_rbConEmi->Text = L"Consumed Emissions";
            this->_rbConEmi->UseVisualStyleBackColor = true;
            this->_rbConEmi->Click += gcnew System::EventHandler(this, &Bat_Frm::_rbConEmi_Click);
            // 
            // _rbConEmiSoi
            // 
            this->_rbConEmiSoi->AutoSize = true;
            this->_rbConEmiSoi->Location = System::Drawing::Point(209, 21);
            this->_rbConEmiSoi->Name = L"_rbConEmiSoi";
            this->_rbConEmiSoi->Size = System::Drawing::Size(197, 20);
            this->_rbConEmiSoi->TabIndex = 1;
            this->_rbConEmiSoi->TabStop = true;
            this->_rbConEmiSoi->Text = L"Consumed Emission Soil";
            this->_rbConEmiSoi->UseVisualStyleBackColor = true;
            this->_rbConEmiSoi->Click += gcnew System::EventHandler(this, &Bat_Frm::_rbConEmiSoi_Click);
            // 
            // _rbTreeMort
            // 
            this->_rbTreeMort->AutoSize = true;
            this->_rbTreeMort->Location = System::Drawing::Point(416, 21);
            this->_rbTreeMort->Name = L"_rbTreeMort";
            this->_rbTreeMort->Size = System::Drawing::Size(122, 20);
            this->_rbTreeMort->TabIndex = 2;
            this->_rbTreeMort->TabStop = true;
            this->_rbTreeMort->Text = L"Tree Mortality";
            this->_rbTreeMort->UseVisualStyleBackColor = true;
            this->_rbTreeMort->CheckedChanged += gcnew System::EventHandler(this, &Bat_Frm::_rbTreeMort_CheckedChanged);
            // 
            // _gbModelType
            // 
            this->_gbModelType->Controls->Add(this->_rbConEmi);
            this->_gbModelType->Controls->Add(this->_rbTreeMort);
            this->_gbModelType->Controls->Add(this->_rbConEmiSoi);
            this->_gbModelType->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_gbModelType->Location = System::Drawing::Point(12, 12);
            this->_gbModelType->Name = L"_gbModelType";
            this->_gbModelType->Size = System::Drawing::Size(596, 55);
            this->_gbModelType->TabIndex = 3;
            this->_gbModelType->TabStop = false;
            this->_gbModelType->Text = L"Model Type";
            // 
            // _btSample
            // 
            this->_btSample->Location = System::Drawing::Point(12, 301);
            this->_btSample->Name = L"_btSample";
            this->_btSample->Size = System::Drawing::Size(60, 33);
            this->_btSample->TabIndex = 4;
            this->_btSample->Text = L"Sample";
            this->_btSample->UseVisualStyleBackColor = true;
            this->_btSample->Click += gcnew System::EventHandler(this, &Bat_Frm::_btSample_Click);
            // 
            // openFileDialog1
            // 
            this->openFileDialog1->FileName = L"openFileDialog1";
            // 
            // _btInput
            // 
            this->_btInput->Location = System::Drawing::Point(12, 83);
            this->_btInput->Name = L"_btInput";
            this->_btInput->Size = System::Drawing::Size(58, 28);
            this->_btInput->TabIndex = 5;
            this->_btInput->Text = L"Input";
            this->_btInput->UseVisualStyleBackColor = true;
            this->_btInput->Click += gcnew System::EventHandler(this, &Bat_Frm::_btInput_Click);
            // 
            // _txInputFile
            // 
            this->_txInputFile->Location = System::Drawing::Point(98, 88);
            this->_txInputFile->Name = L"_txInputFile";
            this->_txInputFile->Size = System::Drawing::Size(501, 20);
            this->_txInputFile->TabIndex = 6;
            // 
            // _txOutputFN
            // 
            this->_txOutputFN->Location = System::Drawing::Point(98, 125);
            this->_txOutputFN->Name = L"_txOutputFN";
            this->_txOutputFN->Size = System::Drawing::Size(501, 20);
            this->_txOutputFN->TabIndex = 7;
            // 
            // _btRun
            // 
            this->_btRun->Location = System::Drawing::Point(12, 213);
            this->_btRun->Name = L"_btRun";
            this->_btRun->Size = System::Drawing::Size(60, 26);
            this->_btRun->TabIndex = 8;
            this->_btRun->Text = L"Run";
            this->_btRun->UseVisualStyleBackColor = true;
            this->_btRun->Click += gcnew System::EventHandler(this, &Bat_Frm::_btRun_Click);
            // 
            // _txStands
            // 
            this->_txStands->Location = System::Drawing::Point(162, 197);
            this->_txStands->Name = L"_txStands";
            this->_txStands->Size = System::Drawing::Size(68, 20);
            this->_txStands->TabIndex = 9;
            this->_txStands->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txErrors
            // 
            this->_txErrors->Location = System::Drawing::Point(162, 231);
            this->_txErrors->Name = L"_txErrors";
            this->_txErrors->Size = System::Drawing::Size(68, 20);
            this->_txErrors->TabIndex = 10;
            this->_txErrors->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _lbStands
            // 
            this->_lbStands->AutoSize = true;
            this->_lbStands->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lbStands->Location = System::Drawing::Point(95, 197);
            this->_lbStands->Name = L"_lbStands";
            this->_lbStands->Size = System::Drawing::Size(51, 15);
            this->_lbStands->TabIndex = 11;
            this->_lbStands->Text = L"Stands";
            // 
            // _lbErrors
            // 
            this->_lbErrors->AutoSize = true;
            this->_lbErrors->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lbErrors->Location = System::Drawing::Point(95, 231);
            this->_lbErrors->Name = L"_lbErrors";
            this->_lbErrors->Size = System::Drawing::Size(46, 15);
            this->_lbErrors->TabIndex = 12;
            this->_lbErrors->Text = L"Errors";
            // 
            // _txErrorFile
            // 
            this->_txErrorFile->Location = System::Drawing::Point(98, 160);
            this->_txErrorFile->Name = L"_txErrorFile";
            this->_txErrorFile->Size = System::Drawing::Size(501, 20);
            this->_txErrorFile->TabIndex = 13;
            // 
            // _lbErrFile
            // 
            this->_lbErrFile->AutoSize = true;
            this->_lbErrFile->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lbErrFile->Location = System::Drawing::Point(9, 165);
            this->_lbErrFile->Name = L"_lbErrFile";
            this->_lbErrFile->Size = System::Drawing::Size(67, 15);
            this->_lbErrFile->TabIndex = 14;
            this->_lbErrFile->Text = L"Error File";
            // 
            // _lbOutFN
            // 
            this->_lbOutFN->AutoSize = true;
            this->_lbOutFN->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lbOutFN->Location = System::Drawing::Point(9, 126);
            this->_lbOutFN->Name = L"_lbOutFN";
            this->_lbOutFN->Size = System::Drawing::Size(77, 15);
            this->_lbOutFN->TabIndex = 15;
            this->_lbOutFN->Text = L"Output File";
            // 
            // _lbNote1
            // 
            this->_lbNote1->AutoSize = true;
            this->_lbNote1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lbNote1->Location = System::Drawing::Point(89, 299);
            this->_lbNote1->Name = L"_lbNote1";
            this->_lbNote1->Size = System::Drawing::Size(369, 15);
            this->_lbNote1->TabIndex = 16;
            this->_lbNote1->Text = L"To create a sample batch input file select a \'Model Type\'";
            // 
            // _lbNote2
            // 
            this->_lbNote2->AutoSize = true;
            this->_lbNote2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lbNote2->Location = System::Drawing::Point(107, 318);
            this->_lbNote2->Name = L"_lbNote2";
            this->_lbNote2->Size = System::Drawing::Size(311, 15);
            this->_lbNote2->TabIndex = 17;
            this->_lbNote2->Text = L"button above and then click the \'Sample\' button";
            // 
            // _rbHeadYes
            // 
            this->_rbHeadYes->AutoSize = true;
            this->_rbHeadYes->Checked = true;
            this->_rbHeadYes->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_rbHeadYes->Location = System::Drawing::Point(160, 16);
            this->_rbHeadYes->Name = L"_rbHeadYes";
            this->_rbHeadYes->Size = System::Drawing::Size(48, 19);
            this->_rbHeadYes->TabIndex = 18;
            this->_rbHeadYes->TabStop = true;
            this->_rbHeadYes->Text = L"Yes";
            this->_rbHeadYes->UseVisualStyleBackColor = true;
            // 
            // _rbHeadNo
            // 
            this->_rbHeadNo->AutoSize = true;
            this->_rbHeadNo->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_rbHeadNo->Location = System::Drawing::Point(160, 35);
            this->_rbHeadNo->MaximumSize = System::Drawing::Size(43, 19);
            this->_rbHeadNo->Name = L"_rbHeadNo";
            this->_rbHeadNo->Size = System::Drawing::Size(43, 19);
            this->_rbHeadNo->TabIndex = 19;
            this->_rbHeadNo->Text = L"No";
            this->_rbHeadNo->UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this->groupBox1->Controls->Add(this->_lbHead2);
            this->groupBox1->Controls->Add(this->_lbHead1);
            this->groupBox1->Controls->Add(this->_rbHeadNo);
            this->groupBox1->Controls->Add(this->_rbHeadYes);
            this->groupBox1->Location = System::Drawing::Point(243, 187);
            this->groupBox1->Name = L"groupBox1";
            this->groupBox1->Size = System::Drawing::Size(217, 64);
            this->groupBox1->TabIndex = 20;
            this->groupBox1->TabStop = false;
            // 
            // _lbHead2
            // 
            this->_lbHead2->AutoSize = true;
            this->_lbHead2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lbHead2->Location = System::Drawing::Point(8, 35);
            this->_lbHead2->Name = L"_lbHead2";
            this->_lbHead2->Size = System::Drawing::Size(123, 15);
            this->_lbHead2->TabIndex = 21;
            this->_lbHead2->Text = L"         in output file";
            // 
            // _lbHead1
            // 
            this->_lbHead1->AutoSize = true;
            this->_lbHead1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lbHead1->Location = System::Drawing::Point(8, 16);
            this->_lbHead1->Name = L"_lbHead1";
            this->_lbHead1->Size = System::Drawing::Size(148, 15);
            this->_lbHead1->TabIndex = 20;
            this->_lbHead1->Text = L"Include column labels";
            // 
            // _gb_FlaSco
            // 
            this->_gb_FlaSco->Controls->Add(this->_rbScoHei);
            this->_gb_FlaSco->Controls->Add(this->_rbFlaLen);
            this->_gb_FlaSco->Controls->Add(this->_txFlaSco);
            this->_gb_FlaSco->Controls->Add(this->label1);
            this->_gb_FlaSco->Location = System::Drawing::Point(466, 187);
            this->_gb_FlaSco->Name = L"_gb_FlaSco";
            this->_gb_FlaSco->Size = System::Drawing::Size(148, 146);
            this->_gb_FlaSco->TabIndex = 21;
            this->_gb_FlaSco->TabStop = false;
            // 
            // _rbScoHei
            // 
            this->_rbScoHei->AutoSize = true;
            this->_rbScoHei->Location = System::Drawing::Point(26, 13);
            this->_rbScoHei->Name = L"_rbScoHei";
            this->_rbScoHei->Size = System::Drawing::Size(93, 17);
            this->_rbScoHei->TabIndex = 3;
            this->_rbScoHei->TabStop = true;
            this->_rbScoHei->Text = L"Scorch Height";
            this->_rbScoHei->UseVisualStyleBackColor = true;
            // 
            // _rbFlaLen
            // 
            this->_rbFlaLen->AutoSize = true;
            this->_rbFlaLen->Checked = true;
            this->_rbFlaLen->Location = System::Drawing::Point(26, 33);
            this->_rbFlaLen->Name = L"_rbFlaLen";
            this->_rbFlaLen->Size = System::Drawing::Size(89, 17);
            this->_rbFlaLen->TabIndex = 2;
            this->_rbFlaLen->TabStop = true;
            this->_rbFlaLen->Text = L"Flame Length";
            this->_rbFlaLen->UseVisualStyleBackColor = true;
            // 
            // _txFlaSco
            // 
            this->_txFlaSco->Location = System::Drawing::Point(47, 55);
            this->_txFlaSco->Name = L"_txFlaSco";
            this->_txFlaSco->Size = System::Drawing::Size(46, 20);
            this->_txFlaSco->TabIndex = 1;
            this->_txFlaSco->Text = L"4";
            this->_txFlaSco->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(2, 82);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(144, 52);
            this->label1->TabIndex = 0;
            this->label1->Text = L"Required when using Crown\r\nScorch (CRNSCH) equations\r\nand flame/scorch height not" 
                L" \r\nincluded in the.TRE file. \r\n";
            // 
            // _cbColRep
            // 
            this->_cbColRep->AutoSize = true;
            this->_cbColRep->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_cbColRep->Location = System::Drawing::Point(234, 265);
            this->_cbColRep->Name = L"_cbColRep";
            this->_cbColRep->RightToLeft = System::Windows::Forms::RightToLeft::Yes;
            this->_cbColRep->Size = System::Drawing::Size(208, 19);
            this->_cbColRep->TabIndex = 22;
            this->_cbColRep->Text = L"Include Column Style Report";
            this->_cbColRep->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
            this->_cbColRep->UseVisualStyleBackColor = true;
            this->_cbColRep->CheckedChanged += gcnew System::EventHandler(this, &Bat_Frm::_cbColRep_CheckedChanged);
            // 
            // Bat_Frm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(623, 346);
            this->Controls->Add(this->_cbColRep);
            this->Controls->Add(this->_gb_FlaSco);
            this->Controls->Add(this->groupBox1);
            this->Controls->Add(this->_lbNote2);
            this->Controls->Add(this->_lbNote1);
            this->Controls->Add(this->_lbOutFN);
            this->Controls->Add(this->_lbErrFile);
            this->Controls->Add(this->_txErrorFile);
            this->Controls->Add(this->_lbErrors);
            this->Controls->Add(this->_lbStands);
            this->Controls->Add(this->_txErrors);
            this->Controls->Add(this->_txStands);
            this->Controls->Add(this->_btRun);
            this->Controls->Add(this->_txOutputFN);
            this->Controls->Add(this->_txInputFile);
            this->Controls->Add(this->_btInput);
            this->Controls->Add(this->_btSample);
            this->Controls->Add(this->_gbModelType);
            this->Name = L"Bat_Frm";
            this->Text = L"FOFEM Batch";
            this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Bat_Frm::Bat_Frm_FormClosing);
            this->_gbModelType->ResumeLayout(false);
            this->_gbModelType->PerformLayout();
            this->groupBox1->ResumeLayout(false);
            this->groupBox1->PerformLayout();
            this->_gb_FlaSco->ResumeLayout(false);
            this->_gb_FlaSco->PerformLayout();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion

/*************************************************************
* Form Closing - upper right corner X clicked
*************************************************************/
private: System::Void Bat_Frm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
 {
  this->Hide();
  e->Cancel = true; 
 }

/************************************************************
* Sample Button Clicked 
*************************************************************/
private: System::Void _btSample_Click(System::Object^  sender, System::EventArgs^  e) 
{
   this->Create_Sample(); 
}

/*********************************************************************************
* Input Button Clicked 
**********************************************************************************/
private: System::Void _btInput_Click(System::Object^  sender, System::EventArgs^  e)
{
   this->Input(); 
}

/***************************************************************************
* Run Button Clicked 
***************************************************************************/
private: System::Void _btRun_Click(System::Object^  sender, System::EventArgs^  e)
{
   this->Batch_Run(); 
}



private: System::Void _rbTreeMort_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
  this->_gb_FlaSco->Show(); 
}
private: System::Void _rbConEmiSoi_Click(System::Object^  sender, System::EventArgs^  e)
 {
this->_gb_FlaSco->Hide(); 
 }
private: System::Void _rbConEmi_Click(System::Object^  sender, System::EventArgs^  e) 
{
this->_gb_FlaSco->Hide(); 
}


/*****************************************************************
* Column Report checked box changed
******************************************************************/
private: System::Void _cbColRep_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
}



};

