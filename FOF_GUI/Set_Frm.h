#pragma once

#include "Wnd_Def.h" 

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

/* Used in iS_EmiUni */
#define e_EU_Tons 1
#define e_EU_Pnd  2

#define e_EUcbTon  "T/ac"
#define e_EUcbPnd  "lb/ac"
#define e_EUcbMet  "g/sq. m"

/*************************************************************
* FOFEM - Settings Window
*
*
*************************************************************/
public ref class Set_Frm : public System::Windows::Forms::Form
{
public:

	int iS_EmiUni;
	int EU_Unit (char cr[]); 
    bool isEmisOld (); 
private: System::Windows::Forms::GroupBox^  _gb_Emis;
public: 

public: 

    System::Windows::Forms::GroupBox^  _gb_EmiMaiWnd;
		
/************************************************************
* Constructor
************************************************************/
Set_Frm(void)
{
	InitializeComponent();

/* Load Settings form Units ComboBox Items */
	this->_cb_EmiUni->Items->Add(e_EUcbPnd); 
	this->_cb_EmiUni->Items->Add(e_EUcbTon); 
	this->_cb_EmiUni->Items->Add(e_EUcbMet); 
    this->_cb_EmiUni->SelectedIndex = 0;     /* Show the first item is list as Selected Item */

    this->_txSoilDuff_SW->Text =  e_EffSoilDuff ;
    this->_txSoilWL_SW->Text   =  e_EffSoilWL   ;
    this->_txSoilHS_SW->Text   =  e_EffSoilHS   ; 

    this->_cb_DuffType_SW->Items->Add ("PeatLow");
    this->_cb_DuffType_SW->Items->Add ("PeatHigh");
    this->_cb_DuffType_SW->Items->Add ("DuffLow");
    this->_cb_DuffType_SW->Items->Add ("DuffHigh");

    this->_cb_DuffType_SW->SelectedIndex = 0; 

}

/**********************************************
* Destructor 
************************************************/
~Set_Frm()
{
  if (components){
	delete components;}
}
/*--------------------------------------------------*/

private: System::Windows::Forms::ComboBox^  _cb_EmiUni; 
private: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::TextBox^  _txSoilWL_SW;
public: System::Windows::Forms::TextBox^  _txSoilHS_SW;
public: System::Windows::Forms::TextBox^  _txSoilDuff_SW;


private: System::Windows::Forms::Label^  label2;
public: 
private: System::Windows::Forms::Label^  label3;
private: System::Windows::Forms::Label^  label4;
private: System::Windows::Forms::Label^  label5;
private: System::Windows::Forms::Label^  label6;
private: System::Windows::Forms::Label^  label7;
private: System::Windows::Forms::ComboBox^  _cb_DuffType_SW;
private: System::Windows::Forms::Label^  label8;

private: System::Windows::Forms::ToolTip^  toolTip1;
private: System::Windows::Forms::Label^  label9;
private: System::Windows::Forms::GroupBox^  groupBox1;
public: System::Windows::Forms::Label^  label10;
private: System::Windows::Forms::RadioButton^  _rb_EmiOld;
public: 
private: System::Windows::Forms::RadioButton^  _rb_EmiNew;
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

public: 
public: 

public: 


/************************************************/
private:


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->components = (gcnew System::ComponentModel::Container());
            this->_cb_EmiUni = (gcnew System::Windows::Forms::ComboBox());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->_txSoilWL_SW = (gcnew System::Windows::Forms::TextBox());
            this->_txSoilHS_SW = (gcnew System::Windows::Forms::TextBox());
            this->_txSoilDuff_SW = (gcnew System::Windows::Forms::TextBox());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->label4 = (gcnew System::Windows::Forms::Label());
            this->label5 = (gcnew System::Windows::Forms::Label());
            this->label6 = (gcnew System::Windows::Forms::Label());
            this->label7 = (gcnew System::Windows::Forms::Label());
            this->_cb_DuffType_SW = (gcnew System::Windows::Forms::ComboBox());
            this->label8 = (gcnew System::Windows::Forms::Label());
            this->toolTip1 = (gcnew System::Windows::Forms::ToolTip(this->components));
            this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
            this->label10 = (gcnew System::Windows::Forms::Label());
            this->label9 = (gcnew System::Windows::Forms::Label());
            this->_rb_EmiOld = (gcnew System::Windows::Forms::RadioButton());
            this->_rb_EmiNew = (gcnew System::Windows::Forms::RadioButton());
            this->_gb_Emis = (gcnew System::Windows::Forms::GroupBox());
            this->groupBox1->SuspendLayout();
            this->_gb_Emis->SuspendLayout();
            this->SuspendLayout();
            // 
            // _cb_EmiUni
            // 
            this->_cb_EmiUni->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_cb_EmiUni->FormattingEnabled = true;
            this->_cb_EmiUni->Location = System::Drawing::Point(249, 32);
            this->_cb_EmiUni->Name = L"_cb_EmiUni";
            this->_cb_EmiUni->Size = System::Drawing::Size(85, 21);
            this->_cb_EmiUni->TabIndex = 0;
            this->toolTip1->SetToolTip(this->_cb_EmiUni, L"select the output units used for \r\nemission outputs ");
            this->_cb_EmiUni->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Set_Frm::_cb_EmiUni_KeyDown);
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label1->Location = System::Drawing::Point(227, 10);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(137, 16);
            this->label1->TabIndex = 1;
            this->label1->Text = L"Emission Output Units";
            // 
            // _txSoilWL_SW
            // 
            this->_txSoilWL_SW->Location = System::Drawing::Point(38, 56);
            this->_txSoilWL_SW->Name = L"_txSoilWL_SW";
            this->_txSoilWL_SW->Size = System::Drawing::Size(67, 20);
            this->_txSoilWL_SW->TabIndex = 2;
            this->_txSoilWL_SW->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txSoilWL_SW, L"Wood Liter");
            // 
            // _txSoilHS_SW
            // 
            this->_txSoilHS_SW->Location = System::Drawing::Point(37, 82);
            this->_txSoilHS_SW->Name = L"_txSoilHS_SW";
            this->_txSoilHS_SW->Size = System::Drawing::Size(67, 20);
            this->_txSoilHS_SW->TabIndex = 3;
            this->_txSoilHS_SW->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txSoilHS_SW, L"Herb / Shrub");
            // 
            // _txSoilDuff_SW
            // 
            this->_txSoilDuff_SW->Location = System::Drawing::Point(39, 29);
            this->_txSoilDuff_SW->Name = L"_txSoilDuff_SW";
            this->_txSoilDuff_SW->Size = System::Drawing::Size(67, 20);
            this->_txSoilDuff_SW->TabIndex = 4;
            this->_txSoilDuff_SW->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txSoilDuff_SW, L"Duff ");
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(107, 32);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(65, 13);
            this->label2->TabIndex = 5;
            this->label2->Text = L"Default: 100";
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(110, 60);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(59, 13);
            this->label3->TabIndex = 6;
            this->label3->Text = L"Default: 15";
            // 
            // label4
            // 
            this->label4->AutoSize = true;
            this->label4->Location = System::Drawing::Point(111, 85);
            this->label4->Name = L"label4";
            this->label4->Size = System::Drawing::Size(59, 13);
            this->label4->TabIndex = 7;
            this->label4->Text = L"Default: 10";
            // 
            // label5
            // 
            this->label5->AutoSize = true;
            this->label5->Location = System::Drawing::Point(5, 32);
            this->label5->Name = L"label5";
            this->label5->Size = System::Drawing::Size(30, 13);
            this->label5->TabIndex = 8;
            this->label5->Text = L"Duff:";
            this->toolTip1->SetToolTip(this->label5, L"Duff");
            // 
            // label6
            // 
            this->label6->AutoSize = true;
            this->label6->Location = System::Drawing::Point(5, 60);
            this->label6->Name = L"label6";
            this->label6->Size = System::Drawing::Size(32, 13);
            this->label6->TabIndex = 9;
            this->label6->Text = L"W/L:";
            this->toolTip1->SetToolTip(this->label6, L"Wood Liter ");
            // 
            // label7
            // 
            this->label7->AutoSize = true;
            this->label7->Location = System::Drawing::Point(6, 84);
            this->label7->Name = L"label7";
            this->label7->Size = System::Drawing::Size(30, 13);
            this->label7->TabIndex = 10;
            this->label7->Text = L"H/S:";
            this->toolTip1->SetToolTip(this->label7, L"Herb / Shrub");
            // 
            // _cb_DuffType_SW
            // 
            this->_cb_DuffType_SW->FormattingEnabled = true;
            this->_cb_DuffType_SW->Location = System::Drawing::Point(71, 32);
            this->_cb_DuffType_SW->Name = L"_cb_DuffType_SW";
            this->_cb_DuffType_SW->Size = System::Drawing::Size(66, 21);
            this->_cb_DuffType_SW->TabIndex = 11;
            this->toolTip1->SetToolTip(this->_cb_DuffType_SW, L"Controls/Adjusts the amount of fire intensity that duff \r\nburning generates. ");
            // 
            // label8
            // 
            this->label8->AutoSize = true;
            this->label8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label8->Location = System::Drawing::Point(23, 10);
            this->label8->Name = L"label8";
            this->label8->Size = System::Drawing::Size(176, 16);
            this->label8->TabIndex = 12;
            this->label8->Text = L"Duff Heat Production Source";
            this->toolTip1->SetToolTip(this->label8, L"Controls/Adjusts the amount of fire intensty\r\nduff burning generates");
            // 
            // groupBox1
            // 
            this->groupBox1->Controls->Add(this->label10);
            this->groupBox1->Controls->Add(this->label7);
            this->groupBox1->Controls->Add(this->label6);
            this->groupBox1->Controls->Add(this->label5);
            this->groupBox1->Controls->Add(this->label4);
            this->groupBox1->Controls->Add(this->label3);
            this->groupBox1->Controls->Add(this->label2);
            this->groupBox1->Controls->Add(this->_txSoilDuff_SW);
            this->groupBox1->Controls->Add(this->_txSoilHS_SW);
            this->groupBox1->Controls->Add(this->_txSoilWL_SW);
            this->groupBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->groupBox1->Location = System::Drawing::Point(17, 65);
            this->groupBox1->Name = L"groupBox1";
            this->groupBox1->Size = System::Drawing::Size(177, 111);
            this->groupBox1->TabIndex = 15;
            this->groupBox1->TabStop = false;
            this->toolTip1->SetToolTip(this->groupBox1, L"The percent of fire intensity (heat) that\r\nreaches the soil surface when doing a " 
                L"\r\nsoil heating simulation. \r\n");
            // 
            // label10
            // 
            this->label10->AutoSize = true;
            this->label10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label10->Location = System::Drawing::Point(7, 0);
            this->label10->Name = L"label10";
            this->label10->Size = System::Drawing::Size(97, 16);
            this->label10->TabIndex = 16;
            this->label10->Text = L"Heat Efficiency";
            // 
            // label9
            // 
            this->label9->AutoSize = true;
            this->label9->Location = System::Drawing::Point(121, 191);
            this->label9->Name = L"label9";
            this->label9->Size = System::Drawing::Size(197, 26);
            this->label9->TabIndex = 14;
            this->label9->Text = L"Refesh Calculation on the Main Window\r\nafter changing any of these settings. \r\n";
            // 
            // _rb_EmiOld
            // 
            this->_rb_EmiOld->AutoSize = true;
            this->_rb_EmiOld->Checked = true;
            this->_rb_EmiOld->Location = System::Drawing::Point(19, 24);
            this->_rb_EmiOld->Name = L"_rb_EmiOld";
            this->_rb_EmiOld->Size = System::Drawing::Size(68, 20);
            this->_rb_EmiOld->TabIndex = 16;
            this->_rb_EmiOld->TabStop = true;
            this->_rb_EmiOld->Text = L"Default";
            this->_rb_EmiOld->UseVisualStyleBackColor = true;
            this->_rb_EmiOld->CheckedChanged += gcnew System::EventHandler(this, &Set_Frm::_rb_EmiOld_CheckedChanged);
            // 
            // _rb_EmiNew
            // 
            this->_rb_EmiNew->AutoSize = true;
            this->_rb_EmiNew->Location = System::Drawing::Point(20, 48);
            this->_rb_EmiNew->Name = L"_rb_EmiNew";
            this->_rb_EmiNew->Size = System::Drawing::Size(88, 20);
            this->_rb_EmiNew->TabIndex = 17;
            this->_rb_EmiNew->Text = L"Expanded";
            this->_rb_EmiNew->UseVisualStyleBackColor = true;
            // 
            // _gb_Emis
            // 
            this->_gb_Emis->Controls->Add(this->_rb_EmiNew);
            this->_gb_Emis->Controls->Add(this->_rb_EmiOld);
            this->_gb_Emis->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_gb_Emis->Location = System::Drawing::Point(217, 65);
            this->_gb_Emis->Name = L"_gb_Emis";
            this->_gb_Emis->Size = System::Drawing::Size(157, 77);
            this->_gb_Emis->TabIndex = 18;
            this->_gb_Emis->TabStop = false;
            this->_gb_Emis->Text = L"Emission Factors";
            // 
            // Set_Frm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(393, 238);
            this->Controls->Add(this->_gb_Emis);
            this->Controls->Add(this->groupBox1);
            this->Controls->Add(this->label9);
            this->Controls->Add(this->label8);
            this->Controls->Add(this->_cb_DuffType_SW);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->_cb_EmiUni);
            this->Name = L"Set_Frm";
            this->Text = L"FOFEM Settings";
            this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Set_Frm::Set_Frm_FormClosing);
            this->groupBox1->ResumeLayout(false);
            this->groupBox1->PerformLayout();
            this->_gb_Emis->ResumeLayout(false);
            this->_gb_Emis->PerformLayout();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
/**************************************************************
* Form Closing - user click X in upper right corner of form
*  just hide the form, it was created in the main windows form
*  and ->Show() there, so we don't want to destroy it, just hide it
**************************************************************/
private: System::Void Set_Frm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
{
  this->Hide();
  e->Cancel = true; 
}
/*****************************************************************
* KeyDown event - this makes combobox read only, there is no
*  setting in the Properties that makes it read only that i've found
  Only the Up and Down keys are allowed to keep working
*****************************************************************/
private: System::Void _cb_EmiUni_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{
 if ( e->KeyCode == Keys::Down ) 
   return; 
 if ( e->KeyCode == Keys::Up ) 
   return; 
 e->SuppressKeyPress = true; 
}


/***********************************************************************
* Emission Factor Radio Button changed
* One of the two radio buttons has been clicked
* The buttons signify if the user wants to use the old or new
* emission factors
***********************************************************************/
private: System::Void _rb_EmiOld_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
  if ( this->_rb_EmiNew->Checked == true )    /* New Factor button selected */
    this->_gb_EmiMaiWnd->Visible = true;      /* Show New Emis Factor controls on Main Wnd */ 
  else 
    this->_gb_EmiMaiWnd->Visible = false;     /* hide controls */
}




};


// }
