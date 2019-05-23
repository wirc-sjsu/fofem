#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;



	public ref class Abt_Frm : public System::Windows::Forms::Form
	{
	public:
		Abt_Frm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Abt_Frm()
		{
			if (components)
			{
				delete components;
			}
		}
    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::Label^  label2;


    private: System::Windows::Forms::Label^  label5;
    private: System::Windows::Forms::Label^  label6;
    private: System::Windows::Forms::Label^  label7;
    private: System::Windows::Forms::Label^  label8;
    private: System::Windows::Forms::Label^  label9;
    private: System::Windows::Forms::PictureBox^  pictureBox1;
    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::PictureBox^  pictureBox2;
    private: System::Windows::Forms::Label^  label4;

    protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Abt_Frm::typeid));
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->label5 = (gcnew System::Windows::Forms::Label());
            this->label6 = (gcnew System::Windows::Forms::Label());
            this->label7 = (gcnew System::Windows::Forms::Label());
            this->label8 = (gcnew System::Windows::Forms::Label());
            this->label9 = (gcnew System::Windows::Forms::Label());
            this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
            this->label4 = (gcnew System::Windows::Forms::Label());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->BeginInit();
            this->SuspendLayout();
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label1->Location = System::Drawing::Point(124, 20);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(113, 24);
            this->label1->TabIndex = 0;
            this->label1->Text = L"F O F E M ";
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(133, 50);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(60, 13);
            this->label2->TabIndex = 1;
            this->label2->Text = L"Version 6.6";
            // 
            // label5
            // 
            this->label5->AutoSize = true;
            this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label5->Location = System::Drawing::Point(74, 78);
            this->label5->Name = L"label5";
            this->label5->Size = System::Drawing::Size(245, 16);
            this->label5->TabIndex = 4;
            this->label5->Text = L"Rocky Mountain Research Station ";
            this->label5->Click += gcnew System::EventHandler(this, &Abt_Frm::label5_Click);
            // 
            // label6
            // 
            this->label6->AutoSize = true;
            this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label6->Location = System::Drawing::Point(93, 105);
            this->label6->Name = L"label6";
            this->label6->Size = System::Drawing::Size(199, 16);
            this->label6->TabIndex = 5;
            this->label6->Text = L"Missoula Fire Sciences Lab";
            // 
            // label7
            // 
            this->label7->AutoSize = true;
            this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label7->Location = System::Drawing::Point(114, 165);
            this->label7->Name = L"label7";
            this->label7->Size = System::Drawing::Size(142, 13);
            this->label7->TabIndex = 6;
            this->label7->Text = L"5775 US Highway 10 W";
            // 
            // label8
            // 
            this->label8->AutoSize = true;
            this->label8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label8->Location = System::Drawing::Point(124, 194);
            this->label8->Name = L"label8";
            this->label8->Size = System::Drawing::Size(121, 13);
            this->label8->TabIndex = 7;
            this->label8->Text = L"Missoula, MT 59808";
            // 
            // label9
            // 
            this->label9->AutoSize = true;
            this->label9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label9->Location = System::Drawing::Point(139, 222);
            this->label9->Name = L"label9";
            this->label9->Size = System::Drawing::Size(95, 13);
            this->label9->TabIndex = 8;
            this->label9->Text = L"www.firelab.org";
            // 
            // pictureBox1
            // 
            this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox1.Image")));
            this->pictureBox1->InitialImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox1.InitialImage")));
            this->pictureBox1->Location = System::Drawing::Point(168, 256);
            this->pictureBox1->Name = L"pictureBox1";
            this->pictureBox1->Size = System::Drawing::Size(37, 36);
            this->pictureBox1->TabIndex = 9;
            this->pictureBox1->TabStop = false;
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label3->Location = System::Drawing::Point(107, 134);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(160, 16);
            this->label3->TabIndex = 10;
            this->label3->Text = L"Fire Modeling Institute";
            // 
            // pictureBox2
            // 
            this->pictureBox2->BackColor = System::Drawing::SystemColors::Info;
            this->pictureBox2->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox2.Image")));
            this->pictureBox2->Location = System::Drawing::Point(2, 2);
            this->pictureBox2->Name = L"pictureBox2";
            this->pictureBox2->Size = System::Drawing::Size(362, 337);
            this->pictureBox2->TabIndex = 11;
            this->pictureBox2->TabStop = false;
            // 
            // label4
            // 
            this->label4->AutoSize = true;
            this->label4->BackColor = System::Drawing::Color::WhiteSmoke;
            this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label4->Location = System::Drawing::Point(42, 85);
            this->label4->Name = L"label4";
            this->label4->Size = System::Drawing::Size(105, 20);
            this->label4->TabIndex = 12;
            this->label4->Text = L"Version  6.6";
            // 
            // Abt_Frm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(366, 341);
            this->Controls->Add(this->label4);
            this->Controls->Add(this->pictureBox2);
            this->Controls->Add(this->label3);
            this->Controls->Add(this->pictureBox1);
            this->Controls->Add(this->label9);
            this->Controls->Add(this->label8);
            this->Controls->Add(this->label7);
            this->Controls->Add(this->label6);
            this->Controls->Add(this->label5);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
            this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
            this->MaximizeBox = false;
            this->MaximumSize = System::Drawing::Size(430, 400);
            this->MinimizeBox = false;
            this->Name = L"Abt_Frm";
            this->Text = L"FOFEM6.6";
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: System::Void label5_Click(System::Object^  sender, System::EventArgs^  e) {
             }
};

