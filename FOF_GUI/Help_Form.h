#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

/**************************************************************/
public ref class Help_Form : public System::Windows::Forms::Form
{
public:
		
Help_Form(void)
{
InitializeComponent();
}

	
~Help_Form(){
if (components){
	delete components;}
}

public: System::Windows::Forms::RichTextBox^  richTextBox1;
 
	
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Help_Form::typeid));
            this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
            this->SuspendLayout();
            // 
            // richTextBox1
            // 
            this->richTextBox1->BackColor = System::Drawing::SystemColors::Info;
            this->richTextBox1->Dock = System::Windows::Forms::DockStyle::Fill;
            this->richTextBox1->Location = System::Drawing::Point(0, 0);
            this->richTextBox1->Margin = System::Windows::Forms::Padding(50);
            this->richTextBox1->Name = L"richTextBox1";
            this->richTextBox1->Size = System::Drawing::Size(632, 451);
            this->richTextBox1->TabIndex = 0;
            this->richTextBox1->Text = L"";
            // 
            // Help_Form
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(632, 451);
            this->Controls->Add(this->richTextBox1);
  //          this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
            this->Name = L"Help_Form";
            this->Text = L"Help ";
            this->ResumeLayout(false);

        }
#pragma endregion
	};

