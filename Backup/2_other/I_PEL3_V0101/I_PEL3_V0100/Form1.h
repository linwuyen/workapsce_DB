#pragma once
#include "windows.h"
#include "MD.h"
#include "tmUART.h"
#include "tmInitialSettings.h"
#include <sstream>

using namespace std;

extern Struct_TMUART					CG_UART;
extern Struct_HMIProcess				CG_HMIProcess;
extern Struct_MD						CG_MD;

namespace I_PEL3_V0100 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
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
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	protected: 
	private: System::Windows::Forms::ToolStripMenuItem^  FileStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  openFileToolStripMenuItem;

	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  driverToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  driverInformationToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  toolToolStripMenuItem;


	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem2;
	private: System::Windows::Forms::ToolStripMenuItem^  resetDriverToolStripMenuItem;






	private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  aboutVersionToolStripMenuItem;
	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::ToolStripStatusLabel^  Cnt_State;
	private: System::Windows::Forms::ToolStripProgressBar^  TSPB_ProgressBar;
	private: System::Windows::Forms::ToolStripStatusLabel^  Finish_Cnt;
	private: System::Windows::Forms::ToolStripStatusLabel^  Finish_Tip;








	private: System::Windows::Forms::ColumnHeader^  Address;
	private: System::Windows::Forms::ColumnHeader^  ID;
	private: System::Windows::Forms::ColumnHeader^  Parameter;
	private: System::Windows::Forms::ColumnHeader^  Value;
	private: System::Windows::Forms::ColumnHeader^  Default;
	private: System::Windows::Forms::ColumnHeader^  Max;
	private: System::Windows::Forms::ColumnHeader^  Min;
	private: System::Windows::Forms::ColumnHeader^  Ram;
	private: System::Windows::Forms::ColumnHeader^  columnHeader1;
	private: System::Windows::Forms::ColumnHeader^  columnHeader13;
	private: System::Windows::Forms::ColumnHeader^  columnHeader3;
	private: System::Windows::Forms::ColumnHeader^  columnHeader2;
	private: System::Windows::Forms::ColumnHeader^  columnHeader7;
	private: System::Windows::Forms::ColumnHeader^  columnHeader4;
	private: System::Windows::Forms::ColumnHeader^  columnHeader5;
	private: System::Windows::Forms::ColumnHeader^  columnHeader6;
	private: System::Windows::Forms::ColumnHeader^  columnHeader8;
	private: System::Windows::Forms::GroupBox^  GB_Step1;



	private: System::Windows::Forms::RadioButton^  RB_Is485;
	private: System::Windows::Forms::TextBox^  SlaveID;
	private: System::Windows::Forms::ComboBox^  Protocol;
	private: System::Windows::Forms::Label^  LB_SlaveID;
	private: System::Windows::Forms::RadioButton^  RB_Is232;
	private: System::Windows::Forms::Label^  LB_Protocol;
	private: System::Windows::Forms::Label^  LB_Comport;
	private: System::Windows::Forms::ComboBox^  CB_ComportName;
	private: System::Windows::Forms::Button^  BT_FWEEP_Remove;
	private: System::Windows::Forms::Button^  BT_FWEEP_Add;


	private: System::Windows::Forms::Button^  BT_HWEEP_Remove;
	private: System::Windows::Forms::Button^  BT_HWEEP_Add;

	private: System::Windows::Forms::RichTextBox^  FWEEP_File_Name;
	private: System::Windows::Forms::RichTextBox^  HWEEP_File_Name;
	private: System::Windows::Forms::CheckBox^  CB_IsLoadValue;
	private: System::Windows::Forms::CheckBox^  CB_IsLoadDef;

	private: System::Windows::Forms::Button^  BT_Start;
	private: System::Windows::Forms::GroupBox^  GB_Step2;

	private: System::Windows::Forms::GroupBox^  GB_Step3;
	private: System::Windows::Forms::GroupBox^  GB_Step4;
	private: System::Windows::Forms::ListBox^  LB_Process;
	private: System::Windows::Forms::CheckBox^  CB_IsLoadMin;
	private: System::Windows::Forms::CheckBox^  CB_IsLoadMax;
	private: System::IO::Ports::SerialPort^  serialPort1;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Timer^  SlowTimer;
private: System::Windows::Forms::OpenFileDialog^  OpenPELS;

private: System::Windows::Forms::TextBox^  BaudRate;
private: System::Windows::Forms::Label^  LB_BaudRate;
private: System::Windows::Forms::OpenFileDialog^  OpenPHEL;
private: System::Windows::Forms::ToolStripMenuItem^  saveFileToolStripMenuItem;
private: System::Windows::Forms::OpenFileDialog^  openSettingFile;
private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::ComponentModel::IContainer^  components;






	private:
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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->FileStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->driverToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->driverInformationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->resetDriverToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboutVersionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->Cnt_State = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->TSPB_ProgressBar = (gcnew System::Windows::Forms::ToolStripProgressBar());
			this->Finish_Cnt = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->Finish_Tip = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->Address = (gcnew System::Windows::Forms::ColumnHeader());
			this->ID = (gcnew System::Windows::Forms::ColumnHeader());
			this->Parameter = (gcnew System::Windows::Forms::ColumnHeader());
			this->Value = (gcnew System::Windows::Forms::ColumnHeader());
			this->Default = (gcnew System::Windows::Forms::ColumnHeader());
			this->Max = (gcnew System::Windows::Forms::ColumnHeader());
			this->Min = (gcnew System::Windows::Forms::ColumnHeader());
			this->Ram = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader13 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader3 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader7 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader4 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader5 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader6 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader8 = (gcnew System::Windows::Forms::ColumnHeader());
			this->GB_Step1 = (gcnew System::Windows::Forms::GroupBox());
			this->BaudRate = (gcnew System::Windows::Forms::TextBox());
			this->LB_BaudRate = (gcnew System::Windows::Forms::Label());
			this->RB_Is485 = (gcnew System::Windows::Forms::RadioButton());
			this->SlaveID = (gcnew System::Windows::Forms::TextBox());
			this->Protocol = (gcnew System::Windows::Forms::ComboBox());
			this->LB_SlaveID = (gcnew System::Windows::Forms::Label());
			this->RB_Is232 = (gcnew System::Windows::Forms::RadioButton());
			this->LB_Protocol = (gcnew System::Windows::Forms::Label());
			this->LB_Comport = (gcnew System::Windows::Forms::Label());
			this->CB_ComportName = (gcnew System::Windows::Forms::ComboBox());
			this->BT_HWEEP_Add = (gcnew System::Windows::Forms::Button());
			this->BT_HWEEP_Remove = (gcnew System::Windows::Forms::Button());
			this->BT_FWEEP_Remove = (gcnew System::Windows::Forms::Button());
			this->BT_FWEEP_Add = (gcnew System::Windows::Forms::Button());
			this->HWEEP_File_Name = (gcnew System::Windows::Forms::RichTextBox());
			this->FWEEP_File_Name = (gcnew System::Windows::Forms::RichTextBox());
			this->CB_IsLoadValue = (gcnew System::Windows::Forms::CheckBox());
			this->CB_IsLoadDef = (gcnew System::Windows::Forms::CheckBox());
			this->BT_Start = (gcnew System::Windows::Forms::Button());
			this->GB_Step2 = (gcnew System::Windows::Forms::GroupBox());
			this->GB_Step3 = (gcnew System::Windows::Forms::GroupBox());
			this->CB_IsLoadMin = (gcnew System::Windows::Forms::CheckBox());
			this->CB_IsLoadMax = (gcnew System::Windows::Forms::CheckBox());
			this->GB_Step4 = (gcnew System::Windows::Forms::GroupBox());
			this->LB_Process = (gcnew System::Windows::Forms::ListBox());
			this->serialPort1 = (gcnew System::IO::Ports::SerialPort(this->components));
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->SlowTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->OpenPELS = (gcnew System::Windows::Forms::OpenFileDialog());
			this->OpenPHEL = (gcnew System::Windows::Forms::OpenFileDialog());
			this->openSettingFile = (gcnew System::Windows::Forms::OpenFileDialog());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->menuStrip1->SuspendLayout();
			this->statusStrip1->SuspendLayout();
			this->GB_Step1->SuspendLayout();
			this->GB_Step2->SuspendLayout();
			this->GB_Step3->SuspendLayout();
			this->GB_Step4->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->FileStripMenuItem1, 
				this->driverToolStripMenuItem, this->toolToolStripMenuItem, this->helpToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Padding = System::Windows::Forms::Padding(5, 2, 0, 2);
			this->menuStrip1->Size = System::Drawing::Size(794, 24);
			this->menuStrip1->TabIndex = 255;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// FileStripMenuItem1
			// 
			this->FileStripMenuItem1->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->openFileToolStripMenuItem, 
				this->saveFileToolStripMenuItem, this->toolStripMenuItem1, this->exitToolStripMenuItem});
			this->FileStripMenuItem1->Name = L"FileStripMenuItem1";
			this->FileStripMenuItem1->Size = System::Drawing::Size(38, 20);
			this->FileStripMenuItem1->Text = L"File";
			// 
			// openFileToolStripMenuItem
			// 
			this->openFileToolStripMenuItem->Enabled = false;
			this->openFileToolStripMenuItem->Name = L"openFileToolStripMenuItem";
			this->openFileToolStripMenuItem->Size = System::Drawing::Size(128, 22);
			this->openFileToolStripMenuItem->Text = L"Open File";
			this->openFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openFileToolStripMenuItem_Click);
			// 
			// saveFileToolStripMenuItem
			// 
			this->saveFileToolStripMenuItem->Enabled = false;
			this->saveFileToolStripMenuItem->Name = L"saveFileToolStripMenuItem";
			this->saveFileToolStripMenuItem->Size = System::Drawing::Size(128, 22);
			this->saveFileToolStripMenuItem->Text = L"Save File";
			this->saveFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::saveFileToolStripMenuItem_Click);
			// 
			// toolStripMenuItem1
			// 
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Size = System::Drawing::Size(125, 6);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(128, 22);
			this->exitToolStripMenuItem->Text = L"Exit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::exitToolStripMenuItem_Click);
			// 
			// driverToolStripMenuItem
			// 
			this->driverToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->driverInformationToolStripMenuItem});
			this->driverToolStripMenuItem->Name = L"driverToolStripMenuItem";
			this->driverToolStripMenuItem->Size = System::Drawing::Size(46, 20);
			this->driverToolStripMenuItem->Text = L"View";
			// 
			// driverInformationToolStripMenuItem
			// 
			this->driverInformationToolStripMenuItem->Name = L"driverInformationToolStripMenuItem";
			this->driverInformationToolStripMenuItem->Size = System::Drawing::Size(176, 22);
			this->driverInformationToolStripMenuItem->Text = L"Driver Information";
			// 
			// toolToolStripMenuItem
			// 
			this->toolToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->toolStripMenuItem2, 
				this->resetDriverToolStripMenuItem});
			this->toolToolStripMenuItem->Name = L"toolToolStripMenuItem";
			this->toolToolStripMenuItem->Size = System::Drawing::Size(45, 20);
			this->toolToolStripMenuItem->Text = L"Tool";
			// 
			// toolStripMenuItem2
			// 
			this->toolStripMenuItem2->Name = L"toolStripMenuItem2";
			this->toolStripMenuItem2->Size = System::Drawing::Size(138, 6);
			// 
			// resetDriverToolStripMenuItem
			// 
			this->resetDriverToolStripMenuItem->Enabled = false;
			this->resetDriverToolStripMenuItem->Name = L"resetDriverToolStripMenuItem";
			this->resetDriverToolStripMenuItem->Size = System::Drawing::Size(141, 22);
			this->resetDriverToolStripMenuItem->Text = L"Reset Driver";
			// 
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->aboutVersionToolStripMenuItem});
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->Size = System::Drawing::Size(46, 20);
			this->helpToolStripMenuItem->Text = L"Help";
			// 
			// aboutVersionToolStripMenuItem
			// 
			this->aboutVersionToolStripMenuItem->Name = L"aboutVersionToolStripMenuItem";
			this->aboutVersionToolStripMenuItem->Size = System::Drawing::Size(154, 22);
			this->aboutVersionToolStripMenuItem->Text = L"About Version";
			this->aboutVersionToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::aboutVersionToolStripMenuItem_Click);
			// 
			// statusStrip1
			// 
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->Cnt_State, this->TSPB_ProgressBar, 
				this->Finish_Cnt, this->Finish_Tip});
			this->statusStrip1->Location = System::Drawing::Point(0, 549);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Padding = System::Windows::Forms::Padding(1, 0, 12, 0);
			this->statusStrip1->Size = System::Drawing::Size(794, 22);
			this->statusStrip1->TabIndex = 294;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// Cnt_State
			// 
			this->Cnt_State->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei UI", 9, System::Drawing::FontStyle::Bold));
			this->Cnt_State->Name = L"Cnt_State";
			this->Cnt_State->Size = System::Drawing::Size(44, 17);
			this->Cnt_State->Text = L"Ready";
			// 
			// TSPB_ProgressBar
			// 
			this->TSPB_ProgressBar->Name = L"TSPB_ProgressBar";
			this->TSPB_ProgressBar->Size = System::Drawing::Size(100, 16);
			this->TSPB_ProgressBar->Step = 1;
			// 
			// Finish_Cnt
			// 
			this->Finish_Cnt->Name = L"Finish_Cnt";
			this->Finish_Cnt->Size = System::Drawing::Size(14, 17);
			this->Finish_Cnt->Text = L"0";
			// 
			// Finish_Tip
			// 
			this->Finish_Tip->Name = L"Finish_Tip";
			this->Finish_Tip->Size = System::Drawing::Size(49, 17);
			this->Finish_Tip->Text = L"              ";
			// 
			// Address
			// 
			this->Address->Text = L"Address";
			this->Address->Width = 54;
			// 
			// ID
			// 
			this->ID->Text = L"ID";
			this->ID->Width = 39;
			// 
			// Parameter
			// 
			this->Parameter->Text = L"Parameter";
			this->Parameter->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->Parameter->Width = 254;
			// 
			// Value
			// 
			this->Value->Text = L"Value";
			this->Value->Width = 51;
			// 
			// Default
			// 
			this->Default->Text = L"Default";
			this->Default->Width = 49;
			// 
			// Max
			// 
			this->Max->Text = L"Max";
			this->Max->Width = 53;
			// 
			// Min
			// 
			this->Min->Text = L"Min";
			this->Min->Width = 51;
			// 
			// Ram
			// 
			this->Ram->Text = L"Ram";
			this->Ram->Width = 53;
			// 
			// columnHeader1
			// 
			this->columnHeader1->Text = L"Address";
			this->columnHeader1->Width = 53;
			// 
			// columnHeader13
			// 
			this->columnHeader13->Text = L"Index";
			this->columnHeader13->Width = 47;
			// 
			// columnHeader3
			// 
			this->columnHeader3->Text = L"Item";
			this->columnHeader3->Width = 170;
			// 
			// columnHeader2
			// 
			this->columnHeader2->Text = L"Value";
			this->columnHeader2->Width = 47;
			// 
			// columnHeader7
			// 
			this->columnHeader7->Text = L"Description";
			this->columnHeader7->Width = 304;
			// 
			// columnHeader4
			// 
			this->columnHeader4->Text = L"Item";
			this->columnHeader4->Width = 110;
			// 
			// columnHeader5
			// 
			this->columnHeader5->Text = L"Source / Parameter";
			this->columnHeader5->Width = 129;
			// 
			// columnHeader6
			// 
			this->columnHeader6->Text = L"State / Value / Function";
			this->columnHeader6->Width = 132;
			// 
			// columnHeader8
			// 
			this->columnHeader8->Text = L"Description";
			this->columnHeader8->Width = 434;
			// 
			// GB_Step1
			// 
			this->GB_Step1->Controls->Add(this->BaudRate);
			this->GB_Step1->Controls->Add(this->LB_BaudRate);
			this->GB_Step1->Controls->Add(this->RB_Is485);
			this->GB_Step1->Controls->Add(this->SlaveID);
			this->GB_Step1->Controls->Add(this->Protocol);
			this->GB_Step1->Controls->Add(this->LB_SlaveID);
			this->GB_Step1->Controls->Add(this->RB_Is232);
			this->GB_Step1->Controls->Add(this->LB_Protocol);
			this->GB_Step1->Controls->Add(this->LB_Comport);
			this->GB_Step1->Controls->Add(this->CB_ComportName);
			this->GB_Step1->Location = System::Drawing::Point(539, 27);
			this->GB_Step1->Name = L"GB_Step1";
			this->GB_Step1->Size = System::Drawing::Size(243, 156);
			this->GB_Step1->TabIndex = 296;
			this->GB_Step1->TabStop = false;
			this->GB_Step1->Text = L"Step1 ( Communications )";
			// 
			// BaudRate
			// 
			this->BaudRate->Location = System::Drawing::Point(114, 67);
			this->BaudRate->Name = L"BaudRate";
			this->BaudRate->Size = System::Drawing::Size(118, 22);
			this->BaudRate->TabIndex = 224;
			this->BaudRate->Text = L"115200";
			// 
			// LB_BaudRate
			// 
			this->LB_BaudRate->AutoSize = true;
			this->LB_BaudRate->Location = System::Drawing::Point(38, 73);
			this->LB_BaudRate->Name = L"LB_BaudRate";
			this->LB_BaudRate->Size = System::Drawing::Size(53, 12);
			this->LB_BaudRate->TabIndex = 223;
			this->LB_BaudRate->Text = L"Baud rate:";
			// 
			// RB_Is485
			// 
			this->RB_Is485->AutoSize = true;
			this->RB_Is485->Location = System::Drawing::Point(114, 45);
			this->RB_Is485->Name = L"RB_Is485";
			this->RB_Is485->Size = System::Drawing::Size(55, 16);
			this->RB_Is485->TabIndex = 222;
			this->RB_Is485->Text = L"RS485";
			this->RB_Is485->UseVisualStyleBackColor = true;
			this->RB_Is485->CheckedChanged += gcnew System::EventHandler(this, &Form1::RB_Is485_CheckedChanged);
			// 
			// SlaveID
			// 
			this->SlaveID->Enabled = false;
			this->SlaveID->Location = System::Drawing::Point(115, 119);
			this->SlaveID->Name = L"SlaveID";
			this->SlaveID->Size = System::Drawing::Size(117, 22);
			this->SlaveID->TabIndex = 56;
			this->SlaveID->Text = L"17";
			// 
			// Protocol
			// 
			this->Protocol->Enabled = false;
			this->Protocol->FormattingEnabled = true;
			this->Protocol->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"Modbus ASCII", L"Modbus RTU"});
			this->Protocol->Location = System::Drawing::Point(114, 94);
			this->Protocol->Name = L"Protocol";
			this->Protocol->Size = System::Drawing::Size(118, 20);
			this->Protocol->TabIndex = 63;
			// 
			// LB_SlaveID
			// 
			this->LB_SlaveID->AutoSize = true;
			this->LB_SlaveID->Enabled = false;
			this->LB_SlaveID->Location = System::Drawing::Point(43, 123);
			this->LB_SlaveID->Name = L"LB_SlaveID";
			this->LB_SlaveID->Size = System::Drawing::Size(48, 12);
			this->LB_SlaveID->TabIndex = 55;
			this->LB_SlaveID->Text = L"Slave ID:";
			// 
			// RB_Is232
			// 
			this->RB_Is232->AutoSize = true;
			this->RB_Is232->Checked = true;
			this->RB_Is232->Location = System::Drawing::Point(36, 46);
			this->RB_Is232->Name = L"RB_Is232";
			this->RB_Is232->Size = System::Drawing::Size(55, 16);
			this->RB_Is232->TabIndex = 221;
			this->RB_Is232->TabStop = true;
			this->RB_Is232->Text = L"RS232";
			this->RB_Is232->UseVisualStyleBackColor = true;
			this->RB_Is232->CheckedChanged += gcnew System::EventHandler(this, &Form1::RB_Is232_CheckedChanged);
			// 
			// LB_Protocol
			// 
			this->LB_Protocol->AutoSize = true;
			this->LB_Protocol->Enabled = false;
			this->LB_Protocol->Location = System::Drawing::Point(44, 98);
			this->LB_Protocol->Name = L"LB_Protocol";
			this->LB_Protocol->Size = System::Drawing::Size(47, 12);
			this->LB_Protocol->TabIndex = 62;
			this->LB_Protocol->Text = L"Protocol:";
			// 
			// LB_Comport
			// 
			this->LB_Comport->AutoSize = true;
			this->LB_Comport->Location = System::Drawing::Point(41, 24);
			this->LB_Comport->Name = L"LB_Comport";
			this->LB_Comport->Size = System::Drawing::Size(50, 12);
			this->LB_Comport->TabIndex = 209;
			this->LB_Comport->Text = L"Comport:";
			// 
			// CB_ComportName
			// 
			this->CB_ComportName->FormattingEnabled = true;
			this->CB_ComportName->Location = System::Drawing::Point(114, 21);
			this->CB_ComportName->Name = L"CB_ComportName";
			this->CB_ComportName->Size = System::Drawing::Size(118, 20);
			this->CB_ComportName->TabIndex = 220;
			this->CB_ComportName->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::CB_ComportName_SelectedIndexChanged);
			this->CB_ComportName->Click += gcnew System::EventHandler(this, &Form1::CB_ComportName_Click);
			// 
			// BT_HWEEP_Add
			// 
			this->BT_HWEEP_Add->Location = System::Drawing::Point(114, 88);
			this->BT_HWEEP_Add->Name = L"BT_HWEEP_Add";
			this->BT_HWEEP_Add->Size = System::Drawing::Size(57, 23);
			this->BT_HWEEP_Add->TabIndex = 229;
			this->BT_HWEEP_Add->Text = L"Add";
			this->BT_HWEEP_Add->UseVisualStyleBackColor = true;
			this->BT_HWEEP_Add->Click += gcnew System::EventHandler(this, &Form1::BT_HWEEP_Add_Click);
			// 
			// BT_HWEEP_Remove
			// 
			this->BT_HWEEP_Remove->Location = System::Drawing::Point(175, 88);
			this->BT_HWEEP_Remove->Name = L"BT_HWEEP_Remove";
			this->BT_HWEEP_Remove->Size = System::Drawing::Size(57, 23);
			this->BT_HWEEP_Remove->TabIndex = 230;
			this->BT_HWEEP_Remove->Text = L"Remove";
			this->BT_HWEEP_Remove->UseVisualStyleBackColor = true;
			this->BT_HWEEP_Remove->Click += gcnew System::EventHandler(this, &Form1::BT_HWEEP_Remove_Click);
			// 
			// BT_FWEEP_Remove
			// 
			this->BT_FWEEP_Remove->Location = System::Drawing::Point(175, 111);
			this->BT_FWEEP_Remove->Name = L"BT_FWEEP_Remove";
			this->BT_FWEEP_Remove->Size = System::Drawing::Size(57, 23);
			this->BT_FWEEP_Remove->TabIndex = 234;
			this->BT_FWEEP_Remove->Text = L"Remove";
			this->BT_FWEEP_Remove->UseVisualStyleBackColor = true;
			this->BT_FWEEP_Remove->Click += gcnew System::EventHandler(this, &Form1::BT_FWEEP_Remove_Click);
			// 
			// BT_FWEEP_Add
			// 
			this->BT_FWEEP_Add->Location = System::Drawing::Point(114, 111);
			this->BT_FWEEP_Add->Name = L"BT_FWEEP_Add";
			this->BT_FWEEP_Add->Size = System::Drawing::Size(57, 23);
			this->BT_FWEEP_Add->TabIndex = 233;
			this->BT_FWEEP_Add->Text = L"Add";
			this->BT_FWEEP_Add->UseVisualStyleBackColor = true;
			this->BT_FWEEP_Add->Click += gcnew System::EventHandler(this, &Form1::BT_FWEEP_Add_Click);
			// 
			// HWEEP_File_Name
			// 
			this->HWEEP_File_Name->Location = System::Drawing::Point(12, 19);
			this->HWEEP_File_Name->Name = L"HWEEP_File_Name";
			this->HWEEP_File_Name->Size = System::Drawing::Size(220, 63);
			this->HWEEP_File_Name->TabIndex = 235;
			this->HWEEP_File_Name->Text = L"";
			// 
			// FWEEP_File_Name
			// 
			this->FWEEP_File_Name->Location = System::Drawing::Point(12, 43);
			this->FWEEP_File_Name->Name = L"FWEEP_File_Name";
			this->FWEEP_File_Name->Size = System::Drawing::Size(220, 63);
			this->FWEEP_File_Name->TabIndex = 236;
			this->FWEEP_File_Name->Text = L"";
			// 
			// CB_IsLoadValue
			// 
			this->CB_IsLoadValue->AutoSize = true;
			this->CB_IsLoadValue->Checked = true;
			this->CB_IsLoadValue->CheckState = System::Windows::Forms::CheckState::Checked;
			this->CB_IsLoadValue->Location = System::Drawing::Point(12, 21);
			this->CB_IsLoadValue->Name = L"CB_IsLoadValue";
			this->CB_IsLoadValue->Size = System::Drawing::Size(51, 16);
			this->CB_IsLoadValue->TabIndex = 237;
			this->CB_IsLoadValue->Text = L"Value";
			this->CB_IsLoadValue->UseVisualStyleBackColor = true;
			this->CB_IsLoadValue->CheckedChanged += gcnew System::EventHandler(this, &Form1::CB_IsLoadValue_CheckedChanged);
			// 
			// CB_IsLoadDef
			// 
			this->CB_IsLoadDef->AutoSize = true;
			this->CB_IsLoadDef->Checked = true;
			this->CB_IsLoadDef->CheckState = System::Windows::Forms::CheckState::Checked;
			this->CB_IsLoadDef->Location = System::Drawing::Point(69, 21);
			this->CB_IsLoadDef->Name = L"CB_IsLoadDef";
			this->CB_IsLoadDef->Size = System::Drawing::Size(58, 16);
			this->CB_IsLoadDef->TabIndex = 238;
			this->CB_IsLoadDef->Text = L"Default";
			this->CB_IsLoadDef->UseVisualStyleBackColor = true;
			this->CB_IsLoadDef->CheckedChanged += gcnew System::EventHandler(this, &Form1::CB_IsLoadDef_CheckedChanged);
			// 
			// BT_Start
			// 
			this->BT_Start->Font = (gcnew System::Drawing::Font(L"PMingLiU", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->BT_Start->Location = System::Drawing::Point(12, 17);
			this->BT_Start->Name = L"BT_Start";
			this->BT_Start->Size = System::Drawing::Size(220, 51);
			this->BT_Start->TabIndex = 297;
			this->BT_Start->Text = L"Start";
			this->BT_Start->UseVisualStyleBackColor = true;
			this->BT_Start->Click += gcnew System::EventHandler(this, &Form1::BT_Start_Click);
			// 
			// GB_Step2
			// 
			this->GB_Step2->Controls->Add(this->BT_HWEEP_Add);
			this->GB_Step2->Controls->Add(this->BT_HWEEP_Remove);
			this->GB_Step2->Controls->Add(this->HWEEP_File_Name);
			this->GB_Step2->Location = System::Drawing::Point(539, 189);
			this->GB_Step2->Name = L"GB_Step2";
			this->GB_Step2->Size = System::Drawing::Size(243, 122);
			this->GB_Step2->TabIndex = 298;
			this->GB_Step2->TabStop = false;
			this->GB_Step2->Text = L"Step2 ( HWEEP File )";
			// 
			// GB_Step3
			// 
			this->GB_Step3->Controls->Add(this->CB_IsLoadMin);
			this->GB_Step3->Controls->Add(this->CB_IsLoadMax);
			this->GB_Step3->Controls->Add(this->CB_IsLoadDef);
			this->GB_Step3->Controls->Add(this->FWEEP_File_Name);
			this->GB_Step3->Controls->Add(this->CB_IsLoadValue);
			this->GB_Step3->Controls->Add(this->BT_FWEEP_Add);
			this->GB_Step3->Controls->Add(this->BT_FWEEP_Remove);
			this->GB_Step3->Location = System::Drawing::Point(539, 317);
			this->GB_Step3->Name = L"GB_Step3";
			this->GB_Step3->Size = System::Drawing::Size(243, 143);
			this->GB_Step3->TabIndex = 299;
			this->GB_Step3->TabStop = false;
			this->GB_Step3->Text = L"Step3 ( FWEEP File )";
			// 
			// CB_IsLoadMin
			// 
			this->CB_IsLoadMin->AutoSize = true;
			this->CB_IsLoadMin->Checked = true;
			this->CB_IsLoadMin->CheckState = System::Windows::Forms::CheckState::Checked;
			this->CB_IsLoadMin->Location = System::Drawing::Point(184, 21);
			this->CB_IsLoadMin->Name = L"CB_IsLoadMin";
			this->CB_IsLoadMin->Size = System::Drawing::Size(43, 16);
			this->CB_IsLoadMin->TabIndex = 240;
			this->CB_IsLoadMin->Text = L"Min";
			this->CB_IsLoadMin->UseVisualStyleBackColor = true;
			this->CB_IsLoadMin->CheckedChanged += gcnew System::EventHandler(this, &Form1::CB_IsLoadMin_CheckedChanged);
			// 
			// CB_IsLoadMax
			// 
			this->CB_IsLoadMax->AutoSize = true;
			this->CB_IsLoadMax->Checked = true;
			this->CB_IsLoadMax->CheckState = System::Windows::Forms::CheckState::Checked;
			this->CB_IsLoadMax->Location = System::Drawing::Point(133, 21);
			this->CB_IsLoadMax->Name = L"CB_IsLoadMax";
			this->CB_IsLoadMax->Size = System::Drawing::Size(45, 16);
			this->CB_IsLoadMax->TabIndex = 239;
			this->CB_IsLoadMax->Text = L"Max";
			this->CB_IsLoadMax->UseVisualStyleBackColor = true;
			this->CB_IsLoadMax->CheckedChanged += gcnew System::EventHandler(this, &Form1::CB_IsLoadMax_CheckedChanged);
			// 
			// GB_Step4
			// 
			this->GB_Step4->Controls->Add(this->BT_Start);
			this->GB_Step4->Location = System::Drawing::Point(539, 466);
			this->GB_Step4->Name = L"GB_Step4";
			this->GB_Step4->Size = System::Drawing::Size(243, 78);
			this->GB_Step4->TabIndex = 300;
			this->GB_Step4->TabStop = false;
			this->GB_Step4->Text = L"Step4 ( Download )";
			// 
			// LB_Process
			// 
			this->LB_Process->FormattingEnabled = true;
			this->LB_Process->HorizontalScrollbar = true;
			this->LB_Process->ItemHeight = 12;
			this->LB_Process->Location = System::Drawing::Point(12, 288);
			this->LB_Process->Name = L"LB_Process";
			this->LB_Process->Size = System::Drawing::Size(513, 256);
			this->LB_Process->TabIndex = 301;
			// 
			// serialPort1
			// 
			this->serialPort1->DataReceived += gcnew System::IO::Ports::SerialDataReceivedEventHandler(this, &Form1::serialPort1_DataReceived);
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// SlowTimer
			// 
			this->SlowTimer->Enabled = true;
			this->SlowTimer->Interval = 500;
			// 
			// OpenPELS
			// 
			this->OpenPELS->Filter = L"Product EEP Loaders ( *.PEL, *.PEL2, *.PELB ) |*.PEL;*.PEL2;*.PELB";
			// 
			// OpenPHEL
			// 
			this->OpenPHEL->Filter = L"(Product HW EEP Loader)|*.PHEL";
			// 
			// pictureBox1
			// 
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->InitialImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox1.InitialImage")));
			this->pictureBox1->Location = System::Drawing::Point(12, 31);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(513, 240);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox1->TabIndex = 302;
			this->pictureBox1->TabStop = false;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(794, 571);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->LB_Process);
			this->Controls->Add(this->GB_Step4);
			this->Controls->Add(this->GB_Step3);
			this->Controls->Add(this->GB_Step2);
			this->Controls->Add(this->GB_Step1);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->menuStrip1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->Name = L"Form1";
			this->Text = L"I_PEL3_V1.01b";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Form1::Form1_FormClosing);
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->GB_Step1->ResumeLayout(false);
			this->GB_Step1->PerformLayout();
			this->GB_Step2->ResumeLayout(false);
			this->GB_Step3->ResumeLayout(false);
			this->GB_Step3->PerformLayout();
			this->GB_Step4->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

void display_PHEL(void)
{
	LB_Process->Items->Add( "讀取HWEEP檔案" );
	if( CG_MD.TargetPHEL.Items.size() != 0 ){
		String^ display_str;
		for( int i = 0; i < (int)CG_MD.TargetPHEL.Items.size(); i++ ){
			display_str = gcnew String( tm_DisplayPHELItem( (Struct_PHEL*)&CG_MD.TargetPHEL, (Struct_PHEL_Item *)&CG_MD.TargetPHEL.Items[i] ).c_str() );
			LB_Process->Items->Add( display_str );
		}
	}
	LB_Process->TopIndex = LB_Process->Items->Count - 1;
}

void display_PELB(void)
{
	LB_Process->Items->Add( "讀取FWEEP檔案" );
	String^ display_str;
	for( int i = 0; i < PELB_MAJOR_MAX; i++ ){
		LB_Process->Items->Add( "ID" + "	" +
								"Value" + "	" +
								"Def" + "	" +
								"Max" + "	" +
								"Min" );

		for( int j = 0; j < PELB_MINOR_MAX; j++ ){
			display_str = Number2String( i + 1, 2 ) + "-" + Number2String( j + 1, 2 ) + "	" +
				Convert::ToString( CG_MD.TargetPELB.Data[ PA_SUBITEM_VALUE ][ i ][ j ] ) + "	" +
				Convert::ToString( CG_MD.TargetPELB.Data[ PA_SUBITEM_DEFAULT ][ i ][ j ] ) + "	" +
				Convert::ToString( CG_MD.TargetPELB.Data[ PA_SUBITEM_MAX ][ i ][ j ] ) + "	" +
				Convert::ToString( CG_MD.TargetPELB.Data[ PA_SUBITEM_MIN ][ i ][ j ] );

			LB_Process->Items->Add( display_str );
		}
	}
	
	LB_Process->TopIndex = LB_Process->Items->Count - 1;
}

void display_PEL2(void)
{
	LB_Process->Items->Add( "讀取FWEEP檔案" );
	String^ display_str;
	for( int i = 0; i < PEL2_MAJOR_MAX; i++ ){
		LB_Process->Items->Add( "ID" + "	" +
								"Value" + "	" +
								"Def" + "	" +
								"Max" + "	" +
								"Min" );

		for( int j = 0; j < PEL2_MINOR_MAX; j++ ){
			display_str = Number2String( i + 1, 2 ) + "-" + Number2String( j + 1, 2 ) + "	" +
				Convert::ToString( CG_MD.TargetPEL2.Data[ PA_SUBITEM_VALUE ][ i ][ j ] ) + "	" +
				Convert::ToString( CG_MD.TargetPEL2.Data[ PA_SUBITEM_DEFAULT ][ i ][ j ] ) + "	" +
				Convert::ToString( CG_MD.TargetPEL2.Data[ PA_SUBITEM_MAX ][ i ][ j ] ) + "	" +
				Convert::ToString( CG_MD.TargetPEL2.Data[ PA_SUBITEM_MIN ][ i ][ j ] );

			LB_Process->Items->Add( display_str );
		}
	}
	
	LB_Process->TopIndex = LB_Process->Items->Count - 1;
}

void display_PEL(void)
{
	LB_Process->Items->Add( "讀取FWEEP檔案" );
	String^ display_str;
	for( int i = 0; i < PEL_MAJOR_MAX; i++ ){
		LB_Process->Items->Add( "ID" + "	" +
								"Value" + "	" +
								"Def" + "	" +
								"Max" + "	" +
								"Min" );

		for( int j = 0; j < PEL_MINOR_MAX; j++ ){
			display_str = Number2String( i + 1, 2 ) + "-" + Number2String( j + 1, 2 ) + "	" +
				Convert::ToString( CG_MD.TargetPEL.Data[ PA_SUBITEM_VALUE ][ i ][ j ] ) + "	" +
				Convert::ToString( CG_MD.TargetPEL.Data[ PA_SUBITEM_DEFAULT ][ i ][ j ] ) + "	" +
				Convert::ToString( CG_MD.TargetPEL.Data[ PA_SUBITEM_MAX ][ i ][ j ] ) + "	" +
				Convert::ToString( CG_MD.TargetPEL.Data[ PA_SUBITEM_MIN ][ i ][ j ] );

			LB_Process->Items->Add( display_str );
		}
	}
	
	LB_Process->TopIndex = LB_Process->Items->Count - 1;
}

private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {			 
			 try{	
				 tmVariableInitial_MD();		 
				 if( !tmInit_OpenFile( INIT_FILE_NAME ) ){
					 MessageBox::Show( "重新開啟就能解決此問題",
										"找不到初始化檔案",
										MessageBoxButtons::OK, 
										MessageBoxIcon::Error );
					 Close();
				 }else{	
					 CG_MD.ComName = tmString2Wstring( tmInit_GetItem( INIT_COM_NAME ) );
					 CG_MD.BaudRate = Convert::ToInt32( tmInit_GetItem( INIT_BAUDRATE ) );
					 CG_MD.Protocol = Convert::ToInt32( tmInit_GetItem( INIT_PROTOCOL ) );
					 CG_MD.IsRS485 = Convert::ToInt32( tmInit_GetItem( INIT_ISRS485 ) );
					 CG_MD.SlaveID = Convert::ToInt32( tmInit_GetItem( INIT_SLAVEID ) );
					 CG_MD.HWEEP_Path = tmString2Wstring( tmInit_GetItem( INIT_HWEEP_PATH ) );
					 CG_MD.FWEEP_Path = tmString2Wstring( tmInit_GetItem( INIT_FWEEP_PATH ) );
			 
					 // Com 
					 String^ com_name_old = gcnew String( CG_MD.ComName.c_str() );
					 CB_ComportName->Items->Clear();
					 CB_ComportName->Items->Add( com_name_old );
					 CB_ComportName->SelectedIndex = 0;

					 BaudRate->Text = Convert::ToString( CG_MD.BaudRate );
					 Protocol->SelectedIndex = CG_MD.Protocol;
					 if( CG_MD.IsRS485 ){
						 RB_Is485->Checked = true;
						 RB_Is485_CheckedChanged( sender, e);
					 }else{
						 RB_Is232->Checked = true;
						 RB_Is232_CheckedChanged( sender, e);
					 }
					 SlaveID->Text = Convert::ToString( CG_MD.SlaveID );

					 HWEEP_File_Name->Text = tmInit_GetItem( INIT_HWEEP_PATH );
					 FWEEP_File_Name->Text = tmInit_GetItem( INIT_FWEEP_PATH );

					 if( HWEEP_File_Name->Text != "" ){
						 tm_OpenPHEL( ( Struct_PHEL *)&CG_MD.TargetPHEL, HWEEP_File_Name->Text );
						 LB_Process->Items->Clear();
						 display_PHEL();
					 }

					 if( FWEEP_File_Name->Text != "" ){

						 if( String::Compare( FWEEP_File_Name->Text, FWEEP_File_Name->Text->Length-5, ".PELB", 0, 5 ) == 0 ){
							 tm_OpenPELB( ( Struct_PELB *)&CG_MD.TargetPELB, FWEEP_File_Name->Text );
							 display_PELB();
						 }else if( String::Compare( FWEEP_File_Name->Text, FWEEP_File_Name->Text->Length-5, ".PEL2", 0, 5 ) == 0 ){
							 tm_OpenPEL2( ( Struct_PEL2 *)&CG_MD.TargetPEL2, FWEEP_File_Name->Text );
							 display_PEL2();
						 }else if( String::Compare( FWEEP_File_Name->Text, FWEEP_File_Name->Text->Length-4, ".PEL", 0, 4 ) == 0 ){
							 tm_OpenPEL( ( Struct_PEL *)&CG_MD.TargetPEL, FWEEP_File_Name->Text );
							 display_PEL();
						 }
					 }

				 }
			 }catch (Exception^ ex) {
				 MessageBox::Show( ex->Message );	
			 }	
		 }
private: System::Void Form1_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
			 try{

				 if( serialPort1->IsOpen ){
					serialPort1->ReadExisting();
					serialPort1->Close();
				 }	

				 // Com 
				 /*
				 if( CB_ComportName->SelectedIndex >= 0 ){
					 CG_MD.ComName = tmString2Wstring( CB_ComportName->Items[ CB_ComportName->SelectedIndex ] );
				 }*/

				 CG_MD.BaudRate = Convert::ToInt32( BaudRate->Text );
				 CG_MD.Protocol = Protocol->SelectedIndex;
				 CG_MD.IsRS485 = Convert::ToInt32( RB_Is485->Checked );
				 CG_MD.SlaveID = Convert::ToInt32( SlaveID->Text );

				 String^ com_name = gcnew String( CG_MD.ComName.c_str() );
				 tmInit_SetItem( com_name, INIT_COM_NAME );
				 tmInit_SetItem( Convert::ToString( CG_MD.BaudRate ), INIT_BAUDRATE );
				 tmInit_SetItem( Convert::ToString( CG_MD.Protocol ), INIT_PROTOCOL );
				 tmInit_SetItem( Convert::ToString( CG_MD.IsRS485 ), INIT_ISRS485 );
				 tmInit_SetItem( Convert::ToString( CG_MD.SlaveID ), INIT_SLAVEID );
				 tmInit_SetItem( HWEEP_File_Name->Text, INIT_HWEEP_PATH );
				 tmInit_SetItem( FWEEP_File_Name->Text, INIT_FWEEP_PATH );

				 tmInit_SaveFile( INIT_FILE_NAME );

			 }catch (Exception^ ex) {
				 MessageBox::Show( ex->Message );
				 Close();			
			 }
		 }
	private: System::Void serialPort1_DataReceived(System::Object^  sender, System::IO::Ports::SerialDataReceivedEventArgs^  e) {
				 try{
					 tmUART_DataReceived( serialPort1, &CG_UART, CG_MD.Protocol );
				 }catch (Exception^ ex) {
					 MessageBox::Show( ex->Message );
					 Close();			
				 }
			 }
private: System::Void openFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void saveFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 Close();
		 }
private: System::Void CB_ComportName_Click(System::Object^  sender, System::EventArgs^  e) {
			 try{
				 CB_ComportName->Items->Clear();
				 //CB_ComportName->Items->AddRange( System::IO::Ports::SerialPort::GetPortNames() );
				 array<String^>^ G_SerialPorts = System::IO::Ports::SerialPort::GetPortNames();//serialPort1->GetPortNames();
				 for( int i = 0; i < G_SerialPorts->Length; i++ ){
					 CB_ComportName->Items->Add( G_SerialPorts[i] );
				 }
			 }catch(Exception^ ex){
				 MessageBox::Show( ex->Message );
			 }
		 }
private: System::Void CB_ComportName_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 try{		
				CG_MD.ComIndex = CB_ComportName->SelectedIndex;
				//MarshalString ( Convert::ToString( CB_ComportName->Items[ CB_ComportName->SelectedIndex ] ), CG_MD.ComName );
				CG_MD.ComName = tmString2Wstring( Convert::ToString( CB_ComportName->Items[ CB_ComportName->SelectedIndex ] ) );

			 }catch(Exception^ ex){
				 MessageBox::Show( ex->Message );
			 }
		 }
private: System::Void RB_Is232_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 try{
				 if( RB_Is232->Checked ){
					 LB_Protocol->Enabled = false;
					 LB_SlaveID->Enabled = false;
					 Protocol->Enabled = false;
					 SlaveID->Enabled = false;
					 CG_MD.IsRS485 = 0;
					 CG_MD.Protocol = MB_SLV_ASCII;
				 }		 
			 }catch (Exception^ ex) {
				 MessageBox::Show( ex->Message );
			 }		 
		 }
private: System::Void RB_Is485_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 try{
				 if( RB_Is485->Checked ){
					 LB_Protocol->Enabled = true;
					 LB_SlaveID->Enabled = true;
					 Protocol->Enabled = true;
					 SlaveID->Enabled = true;
					 CG_MD.IsRS485 = 1;
					 CG_MD.Protocol = Protocol->SelectedIndex;

				 }		 
			 }catch (Exception^ ex) {
				 MessageBox::Show( ex->Message );
			 }
		 }
private: System::Void BT_HWEEP_Add_Click(System::Object^  sender, System::EventArgs^  e) {
			 try{			 
				 if( OpenPHEL->ShowDialog() == System::Windows::Forms::DialogResult::OK ){		 
					 HWEEP_File_Name->Text = OpenPHEL->FileName;

					 tm_OpenPHEL( ( Struct_PHEL *)&CG_MD.TargetPHEL, HWEEP_File_Name->Text );
					 LB_Process->Items->Clear();
					 display_PHEL();
					 CG_MD.Finished_Cnt = 0;

				 }		
			 }catch(Exception^ ex){
				 MessageBox::Show( ex->Message );
			 }
		 }
private: System::Void BT_HWEEP_Remove_Click(System::Object^  sender, System::EventArgs^  e) {
			 try{			 
				 HWEEP_File_Name->Text = "";
				 CG_MD.Finished_Cnt = 0;
			 }catch(Exception^ ex){
				 MessageBox::Show( ex->Message );
			 }
		 }
private: System::Void CB_IsLoadValue_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 CG_MD.Finished_Cnt = 0;
		 }
private: System::Void CB_IsLoadDef_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 CG_MD.Finished_Cnt = 0;
		 }
private: System::Void CB_IsLoadMax_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 CG_MD.Finished_Cnt = 0;
		 }
private: System::Void CB_IsLoadMin_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 CG_MD.Finished_Cnt = 0;
		 }
private: System::Void BT_FWEEP_Add_Click(System::Object^  sender, System::EventArgs^  e) {
			 try{			 
				 if( OpenPELS->ShowDialog() == System::Windows::Forms::DialogResult::OK ){		 
					 FWEEP_File_Name->Text = OpenPELS->FileName;	
					 LB_Process->Items->Clear();
					 if( String::Compare( FWEEP_File_Name->Text, FWEEP_File_Name->Text->Length-5, ".PELB", 0, 5 ) == 0 ){
						 tm_OpenPELB( ( Struct_PELB *)&CG_MD.TargetPELB, FWEEP_File_Name->Text );
						 display_PELB();
					 }else if( String::Compare( FWEEP_File_Name->Text, FWEEP_File_Name->Text->Length-5, ".PEL2", 0, 5 ) == 0 ){
						 tm_OpenPEL2( ( Struct_PEL2 *)&CG_MD.TargetPEL2, FWEEP_File_Name->Text );
						 display_PEL2();
					 }else if( String::Compare( FWEEP_File_Name->Text, FWEEP_File_Name->Text->Length-4, ".PEL", 0, 4 ) == 0 ){
						 tm_OpenPEL( ( Struct_PEL *)&CG_MD.TargetPEL, FWEEP_File_Name->Text );
						 display_PEL();
					 }
					 CG_MD.Finished_Cnt = 0;

				 }		
			 }catch(Exception^ ex){
				 MessageBox::Show( ex->Message );
			 }
		 }
private: System::Void BT_FWEEP_Remove_Click(System::Object^  sender, System::EventArgs^  e) {
			 try{			 
				 FWEEP_File_Name->Text = "";	
				 CG_MD.Finished_Cnt = 0;
			 }catch(Exception^ ex){
				 MessageBox::Show( ex->Message );
			 }
		 }
private: System::Void BT_Start_Click(System::Object^  sender, System::EventArgs^  e) {
			 try{

				 CG_MD.HWEEP_Path = tmString2Wstring( HWEEP_File_Name->Text );
				 CG_MD.FWEEP_Path = tmString2Wstring( FWEEP_File_Name->Text );
				 
				 CG_MD.ToLoadValue = CB_IsLoadValue->Checked;
				 CG_MD.ToLoadDef = CB_IsLoadDef->Checked;
				 CG_MD.ToLoadMax = CB_IsLoadMax->Checked;
				 CG_MD.ToLoadMin = CB_IsLoadMin->Checked;

				 if( !serialPort1->IsOpen ){
					 serialPort1->PortName = CB_ComportName->Text;
					 serialPort1->BaudRate = Convert::ToInt32(BaudRate->Text);

					 //
					 CG_MD.SlaveID = Convert::ToInt32( SlaveID->Text );
					 RB_Is485_CheckedChanged( sender, e);
					 RB_Is232_CheckedChanged( sender, e);
					 //

					 serialPort1->Open();
					 LB_Process->Items->Clear();
					 CG_MD.ComState = STATE_OPENED;
				 }
				 
				 //serialPort1->DtrEnable = true;
				 //serialPort1->DtrEnable = false;

			 
				 BT_Start->Enabled = false;

			 }catch(Exception^ ex){
				 MessageBox::Show( ex->Message );
			 }
		 }

private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
			 
			 this->timer1->Enabled = false;	

			 try{

				 tmMD_TimerProcess();

				 if( CG_MD.ComState == STATE_OPENED ){
					 
					 switch( CG_HMIProcess.Error_Index ){
					 case ERROR_INDEX_NO_ERROR:
					 default:
						 Cnt_State->Text = L"連線正常";
						 Cnt_State->ForeColor = Color::Green;
						 break;
					 case ERROR_INDEX_TIMEOUT:
						 Cnt_State->Text = L"連線逾時";
						 Cnt_State->ForeColor = Color::Red;
						 break;
					 case ERROR_INDEX_FRAME:
						 Cnt_State->Text = L"資料錯誤";
						 Cnt_State->ForeColor = Color::Red;
						 break;
					 case ERROR_INDEX_CMD_DENY:
						 Cnt_State->Text = L"指令拒絕";
						 Cnt_State->ForeColor = Color::Red;
						 break;
					 }

					 Finish_Tip->Text = "資料寫入中";

				 }else{
					 Cnt_State->Text = L"連接埠未開啟";
					 Cnt_State->ForeColor = Color::Black;
				 }

				 tmHMIProcess_Handler[ CG_HMIProcess.State ] ();

				 TSPB_ProgressBar->Value = CG_MD.ProgressBarValue;

				 if( CG_MD.ToResetDriver ){
					 CG_MD.ToResetDriver = 0;
					 GetLocalTime( ( SYSTEMTIME* )&CG_MD.Time_Stamp );
					 serialPort1->DtrEnable = true;
					 serialPort1->DtrEnable = false;
				 }

				 /*
				 if( CG_HMIProcess.State_Old != CG_HMIProcess.State ){
					 CG_HMIProcess.SubState = 0;
				 }
				 CG_HMIProcess.State_Old = CG_HMIProcess.State;
				 */

				 if( CG_MD.Message.size() != 0 ){
					 String^ dummy_str;
					 for( int i = 0; i < (int)CG_MD.Message.size(); i++ ){
						 dummy_str = gcnew String( CG_MD.Message[i].c_str() );
						 LB_Process->Items->Add( dummy_str );
						 LB_Process->TopIndex = LB_Process->Items->Count - 1;
					 }
					 CG_MD.Message.clear();
				 }

				 tmUART_CheckRequest( serialPort1, &CG_UART );


				 if( CG_HMIProcess.State == HMIPROCESS_STATE_DONE ){
					 if( serialPort1->IsOpen ){
						serialPort1->ReadExisting();
						serialPort1->Close();
					 }	
					 CG_MD.ComState = STATE_CLOSED;
					 BT_Start->Enabled = true;
					 CG_HMIProcess.State = HMIPROCESS_STATE_IDLE;

					 if( !CG_MD.TimeOut_Flag && !CG_MD.Error_Flag ){
						 CG_MD.Finished_Cnt++;				 
						 Finish_Tip->Text = "完成";
						 tmSaveResult( CG_MD.PHEL_Result & CG_MD.FWEEP_Result );
					 }
				 }
				 
				 Finish_Cnt->Text = Convert::ToString( CG_MD.Finished_Cnt );

				 if( CG_MD.TimeOut_Flag ){
					 CG_MD.TimeOut_Flag = 0;
					 Finish_Tip->Text = "中斷";
					 MessageBox::Show( "沒有回應, 請確認接線與設定是否正確...:(" );
				 }

				 if( CG_MD.Error_Flag ){
					 CG_MD.Error_Flag = 0;
					 Finish_Tip->Text = "中斷";
					 MessageBox::Show( "發生異常, 提前結束...:(" );
				 }

			 }catch (Exception^ ex) {
				 MessageBox::Show( ex->Message );		 
			 }
			 this->timer1->Enabled = true;

		 }

private: System::Void aboutVersionToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
};
}

