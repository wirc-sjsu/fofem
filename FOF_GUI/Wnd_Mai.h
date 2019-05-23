/*************************************************************************
* Name: Wnd_Mai.h
* Desc: Main FOFEM Window Class
*
*************************************************************************/
#pragma once
#include <windows.h>

#include "wnd_def.h" 
#include "Fuel_Frm.h"
#include "Emis_Frm.h"
#include "Soil_Frm.h" 
#include "Mort_Frm.h" 
#include "Fire_Frm.h" 
#include "Set_Frm.h"


#include "fof_st.h"
#include "fof_Stt.h"
#include "fof_ci.h"
#include "CVT_CVT.H"
#include "Bat_Frm.h" 
#include "fof_iss.h" 
#include "fof_mis.h"
#include "fof_mrt.h"
#include "fof_pf2.h" 
#include "fof_Mt.h" 

#include "fof_nes.h"
#include "wnd_util.h" 
#include "abt_frm.h" 
#include "SoilNew_frm.h"
#include  "fof_mec.h"

#include "Wnd_BM.h"


/* Fuel Load Text Box Adjustment Colors */
#define e_LigR  230      /* Light - Sparse */
#define e_LigG  228
#define e_LigB  224

#define e_TypR  212      /* Typical */
#define e_TypG  208
#define e_TypB  200

#define e_HeaR  163      /* Heavy - Abundant */
#define e_HeaG  163
#define e_HeaB  163

#define e_UsrR  255      /* User */
#define e_UsrG  255
#define e_UsrB  255

#define e_TC_LigSpa 1
#define e_TC_Typ    2
#define e_TC_HeaAbu 3
#define e_TC_User   4

/*-------------------------------------------------*/
/* Indexes for the Mortality Species Dataview Grid */
#define  edg_Spe 0
#define  edg_Equ 1
#define  edg_Den 2
#define  edg_DBH 3
#define  edg_Hgt 4
#define  edg_CR  5
#define  edg_CrnDam 6
#define  edg_CKR  7         /* Crown Kill Ratio */ 
#define  edg_BeeDam 8       /* Beetle Damage */
#define  edg_BolCha 9       /* Bole Char Height */
#define  edg_Prob   10      /* Probability Mortality - calc'd value goes in this column */
#define  edg_Gra 11         /* Graph Checkbox */

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

/*********************************************************************
*  FOFEM - Main Window Form Class 
*
*********************************************************************/
public ref class Wnd_Mai : public System::Windows::Forms::Form
{
 public:


 FILE *fh_Err; 
 String ^ s_IEM; 
private: System::Windows::Forms::Button^  _bt_FFIErr;
private: System::Windows::Forms::Button^  _btMort_Dam;
private: System::Windows::Forms::Button^  _btMort_Bol;
private: System::Windows::Forms::Button^  _btProb;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  _dgcSpecies;
public: 
private: System::Windows::Forms::DataGridViewComboBoxColumn^  _cbEquTyp;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  _dgcDensity;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  _dgcDBH;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  _dgcHeight;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  _dgcCrownRatio;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  _dgcCrnDam;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  _dgcCKR;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  _dgcBeetle;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  _dgcBole;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  _dgcProb;
private: System::Windows::Forms::DataGridViewCheckBoxColumn^  _dgcGraph;
public: System::Windows::Forms::Button^  _btSoilMassman;
private: System::Windows::Forms::Label^  label40;
public: System::Windows::Forms::Button^  _btSoiGraMas;
private: System::Windows::Forms::Label^  label41;
private: System::Windows::Forms::Label^  label43;
public: System::Windows::Forms::Label^  label422;
public: System::Windows::Forms::TextBox^  _txAmbAirTmp;

private: System::Windows::Forms::Label^  label44;

private: System::Windows::Forms::Label^  label42;


public: 
    String ^ s_IEM_FN; 
  int  IEM_Open (char cr_FN[]);
  void IEM_Add  ( char cr[]); 
  void IEM_Close();
  int  IEM_Display (); 

/*........................................................*/
  String ^ s_OS;          /* Operation System XP, Win7-8 */
private: System::Windows::Forms::CheckBox^  _cbFFI_LoadAll;
private: System::Windows::Forms::TextBox^  _txFFIErr;
private: System::Windows::Forms::Button^  _btFFI_FileClose;
private: System::Windows::Forms::Label^  label25;
private: System::Windows::Forms::ComboBox^  _cbEmiFla;
private: System::Windows::Forms::ComboBox^  _cbEmiSmo;
public: System::Windows::Forms::ComboBox^  _cbEmiDuf;
private: System::Windows::Forms::Label^  label28;
private: System::Windows::Forms::Label^  label27;
private: System::Windows::Forms::Label^  label26;
private: System::Windows::Forms::ToolStripMenuItem^  createEmissioToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  createEmissionCSVFIleToolStripMenuItem;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator3;
private: System::Windows::Forms::ToolStripMenuItem^  createEmissionCSVFileSummaryEnglishToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  settingsToolStripMenuItem;
private: System::Windows::Forms::GroupBox^  groupBox3;
public: System::Windows::Forms::Label^  label23;
public: System::Windows::Forms::Label^  label30;
public: System::Windows::Forms::Label^  label29;
public: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator4;
private: System::Windows::Forms::CheckBox^  _cb_SalRep;
private: System::Windows::Forms::GroupBox^  _gbFlaSco;

int  i_SetEvent; 

public: 

  String ^ s_EquTyp;       /* Store Mort Equation Type while Setting Mort Grid Comboboxes */

private: System::Windows::Forms::TextBox^  _txSC1C;
private: System::Windows::Forms::TextBox^  _txSC1B;
private: System::Windows::Forms::TextBox^  _txSC1A;
private: System::Windows::Forms::Label^  label31;
private: System::Windows::Forms::GroupBox^  _gbSC1;
private: System::Windows::Forms::Label^  label33;
private: System::Windows::Forms::Label^  label32;
private: System::Windows::Forms::GroupBox^  _gbSC2;
private: System::Windows::Forms::Label^  label34;
private: System::Windows::Forms::Label^  label35;
private: System::Windows::Forms::Label^  label36;
private: System::Windows::Forms::TextBox^  _txSC2C;
private: System::Windows::Forms::TextBox^  _txSC2B;
private: System::Windows::Forms::TextBox^  _txSC2A;
private: System::Windows::Forms::GroupBox^  _gbSC3;
private: System::Windows::Forms::Label^  label37;
private: System::Windows::Forms::Label^  label38;
private: System::Windows::Forms::Label^  label39;
private: System::Windows::Forms::TextBox^  _txSC3C;
private: System::Windows::Forms::TextBox^  _txSC3B;
private: System::Windows::Forms::TextBox^  _txSC3A;

  String ^ s_UserFolder;   /* .ini File Location Path, this is the User Folder */

  int CSF_Exec ();
  int CSF_Mngr (String ^ Str ); 

  int SoilReport_Massman(); 
  void SoilReport_MassmanMess(int i); 
  int  iS_MassRep; 
  int  iS_MassWnd;    /* first time switch, open Massman Graph Wnd */
/*..........................................................*/

  float  f_PreFuelClass;  /* Sav ComboBox Selection index before it changes, might need to restore */

  String ^StringToPrint; 

  char * cr_FFI_FN; 
 
  bool b_DisEvt_LogDist;    /* Disable Event - Log Distribution ComboBox */ 

  int iS_Calc;
 
  String ^ s_FFI_PthFN;   /* if FFI file is used as Fuel Class */
  String ^ sS_FFI_Prj;   
 
/* Keep the User entered fuel loads */
  String ^ s_uDuff; 
  String ^ s_uDuffDep; 
  String ^s_uLit; 
  String ^s_u1Hr; 
  String ^s_u10Hr; 
  String ^s_u100Hr; 
  String ^s_u1kHr;
  String ^ s_uHerb; 
  String ^s_uShrub; 
  String ^s_uFol;
  String ^s_uBra;
 
/* Graph Forms */
  Fuel_Frm  ^ fuel_frm;
  Grap_Emis ^ grap_emis; 
  Bat_Frm   ^ bat_frm; 
  Set_Frm   ^ set_frm; 
  Fire_Frm  ^ fire_frm;

/* Graph Window */ 
  Mort_Frm  ^ mort_frm;  /* Crown Scorch Height */
  Mort_Frm  ^ mort_Dam;  /* Crown Damage */
  Mort_Frm  ^ mort_Bol;  /* Bole Char */

 // Soil_Frm  ^ soil_frm; 
  Soil_Frm  ^ soil_frm3; 
  Soil_Frm  ^ soil_file;

  SoilNew_Frm ^SNF;      /* Soil New Form */ 

  Form1 ^BmSoil_Frm;
  int    i_BMSoil_BurnupSw; 
 

public: System::Windows::Forms::Button^  _btSoil_Frm;
public: System::Windows::Forms::Label^  label2;
public: System::Windows::Forms::Label^  label3;
public: System::Windows::Forms::Label^  label18;
public: System::Windows::Forms::Label^  label20;
public: System::Windows::Forms::Button^  _btReportEmis;
public: System::Windows::Forms::Button^  _btReportFuel;
public: System::Windows::Forms::Button^  _btReportSoil;
public: System::Windows::Forms::Label^  label21;
private: Microsoft::VisualBasic::PowerPacks::LineShape^  lineShape2;
private: Microsoft::VisualBasic::PowerPacks::LineShape^  lineShape1;
public: System::Windows::Forms::Label^  label5;
private: System::Windows::Forms::TextBox^  _txRepTot;
public: System::Windows::Forms::Button^  _btRepTotClear;
private: System::Windows::Forms::Button^  _btRepTotDisp;
public: System::Windows::Forms::ContextMenuStrip^  _cmsAdjNONE;
private: System::Windows::Forms::ToolStripMenuItem^  openProjectToolStripMenuItem; 
private: System::Windows::Forms::ToolStripMenuItem^  saveProjectToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  _cmBatch;
public: System::Windows::Forms::Button^  _btTest;
private: System::Windows::Forms::ComboBox^  _cbSpecies;
public: System::Windows::Forms::DataGridView^  _dgMort;
private: System::Windows::Forms::Button^  _btSelect;
private: System::Windows::Forms::TextBox^  _txFlaSco;
private: System::Windows::Forms::RadioButton^  _rbFlameLen;
private: System::Windows::Forms::RadioButton^  _rbScorchHeight;
private: System::Windows::Forms::Button^  _btReportMort;
private: System::Windows::Forms::Button^  _btGraphMort;
public: System::Windows::Forms::Label^  label19;
private: System::Windows::Forms::CheckBox^  _chLowFireSev;
private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  createSampleBurnupInputFilebrnToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  runBurnupFromInputFIlebrnToolStripMenuItem;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
private: System::Windows::Forms::ToolStripMenuItem^  runSoilFromInputFileToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  saveSoilTempPointsFileToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  createSampleSoilInputFIledrfexpToolStripMenuItem; 
private: System::Windows::Forms::ComboBox^  _cbFFI_Plots;
public: System::Windows::Forms::TextBox^  _txFFI_File;
private: System::Windows::Forms::GroupBox^  _gbFFI;
private: System::Windows::Forms::Label^  _lbFFIPlot;
private: Microsoft::VisualBasic::PowerPacks::ShapeContainer^  shapeContainer2;
public: Microsoft::VisualBasic::PowerPacks::ShapeContainer^  shapeContainer1;
public: Microsoft::VisualBasic::PowerPacks::RectangleShape^  rectangleShape4;
public: Microsoft::VisualBasic::PowerPacks::RectangleShape^  rectangleShape3;
public: Microsoft::VisualBasic::PowerPacks::RectangleShape^  rectangleShape2;
public: Microsoft::VisualBasic::PowerPacks::RectangleShape^  rectangleShape1;
private: System::Windows::Forms::GroupBox^  groupBox1;
private: System::Windows::Forms::Label^  label22;
private: System::Windows::Forms::HelpProvider^  helpProvider1;
private: System::Windows::Forms::ToolTip^  toolTip1;
private: System::Windows::Forms::PictureBox^  pictureBox1;
private: System::Windows::Forms::PictureBox^  _picFOFEM;
private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem1;
private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem1;
private: System::Windows::Forms::ToolStripMenuItem^  aboutToolStripMenuItem;
private: System::Windows::Forms::Button^  _btClearReport;
private: Microsoft::VisualBasic::PowerPacks::ShapeContainer^  shapeContainer3;
private: System::Windows::Forms::Label^  label24;
private: System::Windows::Forms::ToolStripMenuItem^  newProjectToolStripMenuItem;
private: System::Windows::Forms::Button^  _btTip;
private: Microsoft::VisualBasic::PowerPacks::LineShape^  lineShape3;
public: System::Windows::Forms::Button^  _btFireIntensity;
public: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem4;

/*----------------------------------------*/

 int Emis_ComboBox ();
 int Emis_SetCovTyp();
 int Emis_SetCovTyp_FFI(); 
 int FLM_SetLogSoil(); 

 int Help_Set (TextBox ^ tb, char cr[]); 
 int Help_SetRB (RadioButton ^rb, char cr[]); 
 int Help_SetCB (ComboBox ^ tb, char cr[]); 
 int Help_SetChk (CheckBox ^ tb, char cr[]); 
 int ToolTip_Set (TextBox ^ tb, char cr[]); 
 int Set_MortSpeciesHelp();
 int Help_Mngr();

 int FFI_LoadPlotNames (char cr_FN[]); 
 int FFI_LoadTreeGrid(); 
 void FFI_Close(); 
 int FFI_ChkTreeData(float f_TPA, float f_DBH, float f_Hgt, float f_HgtCrn, int i_CrnRat); 
 int FFI_LoadCVT(char cr_Pth[], char cr_ErrMes[]); 
 int FFI_GetPthFN (char cr_PthFN[]); 

int   SUF_Exec ();

// void    GetDate(char cr[]); 
int     Prj_IniFil_Read ();
int     Prj_IniFil_Save ();
int     Prj_Open (char cr_inFN[]);
int     Prj_SetComboBox (FILE *fh, char cr_PrjSw[], ComboBox ^ cb, int i_Fill);
int     Prj_SetFuelBox (FILE *fh, char cr_PrjSw[], TextBox ^ tb);
int     Prj_SetTextBox (FILE *fh, char cr_PrjSw[], TextBox ^ tb, int i_Per);
int      Prj_OpenStr(String ^ s_FN);
String ^ Prj_GetPath();
int      Prj_SaveMngr (char cr_Mode[]);
int      Prj_Save(char cr_PrjFN[], char cr_Mode[]);
String ^ Prj_GetFileName ();
char *   Prj_GetAdj (TextBox ^ tb);
int     Prj_FFI_File (FILE *fh); 

int  WM_FuelAdj_Set(); 
int  CalcSub (d_FLI *a_FLI, d_CI *a_CI, d_CO *a_CO, d_SI *a_SI, d_SO *a_SO, char cr_PtsFN[], char cr_ErrMes[]); 
int  Set_FueBoxCol (String ^ s_MenuItem, String ^ s_txBox);
void CB_Load (ComboBox^  cb);
void Moist_Change (); 
void Output_Change (); 
int  WM_ClassFile_Load (char cr_FFIPthFN[]); 
int  FLM_LoadFile  (char cr_FN[], char cr_ErrMes[]);
int  FLM_LoadFileNew  (char cr_FN[], char cr_ErrMes[]);
int  Mai_FuelTB_Load(String ^ s_TexBoxNam); 
int  CCW_Get_RegionInt(); 

void Set_AdjustmentColors(); 
void WM_FuelBoxColors (String ^ s_Adj); 
void WM_UserFuels_Save (); 
void WM_UserFuels_Set(); 
int  SAF_SetDeadLoad (TextBox ^tb, float f_Val, int i_Id, char cr_FueCat[], char cr_HvCod[]);
int  SAF_SetLiveLoad (TextBox ^tb, float f_Load, int i_Id, char cr_Code[]);
int  SAF_SetDuff (float f_Duff, float f_DufDep, char cr_CovTyp[], char cr_CovGrp[]); 
int  WD_SetWeightDist (float f_Load, float f_pcRot, char cr_WD[], d_CI *a_CI, char cr_ErrMes[]);

char AdjChar (String ^ s_TBN); 
bool isFuelLight (TextBox ^ tb );
bool isFuelSparse (TextBox ^ tb );

bool isFuelHeavy (TextBox ^ tb );
bool isFuelAbundent (TextBox ^ tb); 

bool isFuelTypical (TextBox ^ tb );
bool isFuelUser (TextBox ^ tb );

void Set_TB_ReadOnly (bool b);

int  Calculate(char cr_EmFN[], char cr_SoFN[]); 
int  ClearAll (d_CO *a_CO); 

int  Get_Inputs(d_FLI *a_FLI); 

int  Report_Mngr_Old (char cr_Mode[]); 
int  Report_Mngr (char cr_Mode[]); 

int  Report_Fuel (d_CI *a_CI, d_CO *a_CO);
int  Consumed_FlaSmo(); 

int Wnd_Mai::Report_Smoke (d_CO *a_CO, d_FLI *a_FLI);
int Wnd_Mai::Report_Soil (d_CI *a_CI, d_CO *a_CO, d_SI *a_SI, d_SO *a_SO, d_FLI *a_FLI);

int  Set_FuelTotal (d_CO *a_CO, char cr_ErrMes[]); 
int Rep_FuelTotals ();
int Rep_SmokeTotals (); 
int Rep_SoilTotals (); 
void Rep_TotalsClear (); 

void Rep_Ind ( char cr_Cat[], float f_Pre, float f_Pos);

int  Report_SubTit(d_FLI *a_FLI, char cr_Poco[]); 
void _rep (char cr[]); 

int  Report_Save(); 

int  Burnup_EmFil_Save();
int  Burnup_SamInpFile ();  /* create a sample Burnup input file */
int  Burnup_RunInpFile (); 

int  Emission_CSVFile (char cr_Mode[], char cr_Unit[]); 
int  Emission_CSVMngr (char cr_FN[], char cr_Mode[], char cr_ErrMes[]); 
int  Emission_CSVSumExpFac  (char cr_FN[], char cr_ErrMes[]); 

int  Emission_CSVSum_DefFac  (char cr_FN[], char cr_ErrMes[]); 
int  Emission_CSVShort_DefFac (char cr_FN[], char cr_ErrMes[]); 

int  Soil_CreateSample (); 
int Soil_RunInputFile (); 
int Soil_SavePointsFile (); 

int  Duff_AdjDepth(); 
int  Duff_AdjAmount(); 

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-*/
/*   Mortality Functions */ 

int   Mort_LoadSpeMstLst (); 
int  Mort_Select(); 
int   Mort_GetName(char cr_Id[], char cr_Name[]);

int Wnd_Mai::Attach_EquTypComBox (int r, int c, char cr_Mode[] );
int Wnd_Mai::Set_ReqCols (int r);
int Wnd_Mai::Set_Color (int r, int c, bool b); 

int Wnd_Mai::Mort_GraphReg();
int Wnd_Mai::Mort_GraphPFI();
int Wnd_Mai::Mort_GraphBol();
int Wnd_Mai::Mort_Graph(); 

bool Wnd_Mai::PostFire_Checked (); 
int  Wnd_Mai::MortRepTyp(); 

void Wnd_Mai::SalvageSet(char cr[]);
void Wnd_Mai::SalvageProb(int i_SizCla, float A, float B, float Prob);
void Wnd_Mai::SalvageDefault(); 

// int   Get_MortTotal(char cr_TotNam[], char cr_ErrMes[]);
int Put_SpeRec(char Spe[], char cr_EquTyp[],  char Den[], char DBH[], char Hgt[], char CR[],
               char cr_CrnDam[], char cr_CKR[], char cr_BeeDam[], char cr_BolCha[]);

void  MSR_Avg_Mort ();
void  Load_SLK ();
void  MSR_ProbMort(); 
int   MSR_PostFire_Killed  (int i_SizGrp, char cr_Mode[]);
int   MI_ChkRelHum (char cr_ErrMes[]);
void  MI_SetSFCP  (float f_Sch, d_MI *a_MI);
void  MSR_Summary();
void  MSR_Mort_Indexes();
void  MSR_Density (int i_SizGrp);

int   Load_Probability (char cr_ErrMes[]); 
int   MIS_Load (char cr_ErrMes[]); 
int   MIS_ReportNew(); 
int   TreeGrid_SetProb(); 

int   CCW_isFlaLen();
int   CCW_isCoaPlaY();
int   SalvageReport (char cr_TotNam[]); 

void  PFI_SummarySalvage(); 

int   gi_NoCovMes; 


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-*/

private: System::Windows::Forms::TextBox^  _txOut_DufDep;
private: System::Windows::Forms::Button^  _btFuel_Frm;
private: System::Windows::Forms::Button^  _btEmis_Frm;
private: System::Windows::Forms::GroupBox^  groupBox2;
public: System::Windows::Forms::TextBox^  _txMoistSoil;
public: System::Windows::Forms::ComboBox^  _cbSoil;
public: System::Windows::Forms::ContextMenuStrip^  _cmsReport;
public: System::Windows::Forms::ToolStripMenuItem^  _cmiReportCut;
public: System::Windows::Forms::ToolStripMenuItem^  _cmiReportPaste;
public: System::Windows::Forms::ToolStripMenuItem^  _cmiReportPrint;
public: System::Windows::Forms::ToolStripMenuItem^  _cmiReportSave;
private: System::Windows::Forms::ToolStripMenuItem^  _cmiClear;
private: System::Windows::Forms::ToolStripMenuItem^  _cmiCopy;
private: System::Windows::Forms::PrintDialog^  printDialog1;
private: System::Drawing::Printing::PrintDocument^  _pdReport;
private: System::Drawing::Printing::PrintDocument^  printDocument1;
private: System::Windows::Forms::ToolStripMenuItem^  _cmiPrintPreview;
public: System::Windows::Forms::PrintPreviewDialog^  printPreviewDialog1;

/*** Constructor *****************************************/
Wnd_Mai(void)
{
String ^ Str;


  this->fh_Err = NULL; 
  this->s_EquTyp = "";  
  this->i_SetEvent = 0; 

  cr_FFI_FN = new char[2000];  /* FFI file name when in use for Tree Mortality */ 
  strcpy (cr_FFI_FN,""); 
 
  this->s_FFI_PthFN = "";

  this->CSF_Exec();    /* determine location of User Folder, if needed move files, etc. */

  b_DisEvt_LogDist = false; /* Allow Log Dist ComboBox Events to go off */
  iS_Calc = 0; /* Prevent Calculate() from going off while we setup ComboBox's */

/* Give Duff something to start off with the first time the user goes to enter a User */
/*  amount, see e_DufDefRat it shows TPA/Depth ratio, I used here for default depth  */
  s_uDuff = "1.0" ;         
  s_uDuffDep = "0.1"; 

  s_uLit = "0" ; 
  s_u1Hr = "0" ; 
  s_u10Hr = "0" ; 
  s_u100Hr = "0" ; 
  s_u1kHr = "0" ; 
  s_uHerb = "0" ;  
  s_uShrub = "0" ; 
  s_uFol = "0" ; 
  s_uBra = "0" ; 
 
  InitializeComponent();

/* Soil Model Fire Intensity Efficency percent text boxes, used for testing */
// Took this off the Main Window moved to Setting Wnd 
//  this->_txSoilWL->Text = "15";
//  this->_txSoilHS->Text = "10"; 

  this->_txRotLog->Text = e_RotLogDef;    /* Percent Rotten Log textbox */
  this->_txFolBraCon->Text = "0";  /* Percent Fol Bra consumed textbox */
  
  Set_AdjustmentColors();    /* Fuel textbox context menus, adj buttons, etc */

  CB_Load (this->_cbMoisture);
  CB_Load (this->_cbRegion);
  CB_Load (this->_cbFuelClass);  
  CB_Load (this->_cbSeason);
  CB_Load (this->_cbOutput); 
  CB_Load (this->_cbFuelCat);
  CB_Load (this->_cbLogDist);
  CB_Load (this->_cbSoil); 

//void _CBI_Load_DGV (DataGridViewComboBoxColumn ^ cb, char A[], char B[], char C[], int iX)

  _CBI_Load_DGV (this->_cbEquTyp,e_CroSco,e_CroDam,e_BolCha,1); 

// CB_Load (this->_cbMoistType);  /* Moisture Duff Measured Method */

  this->_txFolBraCon->Text = e_FolBraConDef; 
  this->_txRotLog->Text = e_RotLogDef ; 

  this->fuel_frm = gcnew Fuel_Frm; 
  this->grap_emis = gcnew Grap_Emis; 
  
  this->bat_frm = gcnew Bat_Frm; 
  this->set_frm = gcnew Set_Frm;


  this->mort_frm = gcnew Mort_Frm; 
  this->mort_Dam = gcnew Mort_Frm; 
  this->mort_Bol = gcnew Mort_Frm; 


  this->fire_frm = gcnew Fire_Frm; 

//  this->soil_frm = gcnew Soil_Frm;  
  this->soil_frm3 = gcnew Soil_Frm;  /* Soil Graph Form */
  this->soil_file = gcnew Soil_Frm;  /* User File Soil Graph */

  this->set_frm->_gb_EmiMaiWnd = this->groupBox3; 

  this->SNF = gcnew SoilNew_Frm; 
  this->BmSoil_Frm = gcnew Form1; 
  this->BmSoil_Frm->s_UserFolder = this->s_UserFolder; 
  this->i_BMSoil_BurnupSw = 0;   /* one-time switch */ 
  this->iS_MassRep = 0;          /* Massman Report Button One-Time switch*/
  this->iS_MassWnd = 0;          /* First time Massman Graph Button clicked */

  this->Text = "FOFEM 6.6.1"; 

  this->_cb_SalRep->Checked = false ; 
  this->SalvageSet("Hide");   
  this->SalvageDefault();        
  

  STT_Init();    /* Report Totals */
  ST_Init();
  STS_Init(); 

/* Read in the Emissions Factor File...................... */ 
char cr_Pth[1000], cr_ErrMes[2000];
int i;
  _StrToChar (this->s_UserFolder,cr_Pth);
   i = NES_Read (cr_Pth, cr_ErrMes);
   if ( strcmp (cr_ErrMes,"") ) {
     strcat (cr_ErrMes, "\n Dude, this is bad news and requires your immediate attention."); 
	 myMessageBox (cr_ErrMes); }

   this->Emis_ComboBox(); 
/*......................................................... */


  Mort_LoadSpeMstLst();  /* Load the species master list */
 

/* Read in the FOFEM.ini, and load the project file */
  Prj_IniFil_Read (); 

/* Batch Form will use Prj file Path as its default Path */
  Str = this->Prj_GetPath(); 
  this->bat_frm->Set_FilePath(Str);  


  this->Help_Mngr(); 

 iS_Calc = 1; 

 gi_NoCovMes = 0;        /* One time Post-fire Injury warn message */  
 
 this->Calculate("",""); 
}


/**  Destructor *********************************************/
~Wnd_Mai(){
 
  if (components){
	delete components;}

  delete cr_FFI_FN; 
}



/**************************************************************/
private: System::Windows::Forms::ContextMenuStrip^  _cms_AdjDead;
private: System::Windows::Forms::ToolStripMenuItem^  _mi_Typical;
private: System::Windows::Forms::ToolStripMenuItem^  _mi_Light;
public: System::Windows::Forms::TextBox^  _txLitter;
public: System::Windows::Forms::TextBox^  _txDuff;
public: System::Windows::Forms::TextBox^  _tx1Hr;
private: System::Windows::Forms::TabControl^  _tc_Main;
private: System::Windows::Forms::TabPage^  _tcpFuel;
private: System::Windows::Forms::TabPage^  _tcpMortality;
private: System::Windows::Forms::MenuStrip^  menuStrip1;
private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;


private: System::Windows::Forms::ToolStripMenuItem^  optionsToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;

private: System::Windows::Forms::ComboBox^  _cbFuelClass;
private: System::Windows::Forms::ComboBox^  _cbRegion;
private: System::Windows::Forms::TextBox^  _txProFilNam;
public: System::Windows::Forms::TextBox^  _tx10Hr;
private: System::Windows::Forms::Label^  _lbDuff;
private: System::Windows::Forms::Label^  _lb1Hr;
private: System::Windows::Forms::Label^  _lbLitter;
private: System::Windows::Forms::Label^  _lb1kHr;
private: System::Windows::Forms::Label^  _lb100Hr;
private: System::Windows::Forms::Label^  _lb10Hr;
public: System::Windows::Forms::TextBox^  _tx1kHr;
public: System::Windows::Forms::TextBox^  _tx100Hr;
private: System::Windows::Forms::Button^  _btHeavyAbun;
private: System::Windows::Forms::Button^  _btTypical;
private: System::Windows::Forms::Button^  _btLightSpa;
private: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::ToolStripMenuItem^  _mi_Heavy;
private: System::Windows::Forms::ToolStripMenuItem^  _mi_User;
private: System::Windows::Forms::Label^  _lbFolige;
private: System::Windows::Forms::Label^  _lbBranch;
private: System::Windows::Forms::Label^  _lbHerb;
private: System::Windows::Forms::Label^  _lbShrub;
public: System::Windows::Forms::TextBox^  _txBranch;
public: System::Windows::Forms::TextBox^  _txFoliage;
public: System::Windows::Forms::TextBox^  _txShrub;
public: System::Windows::Forms::TextBox^  _txHerb;
private: System::Windows::Forms::Label^  label6;
private: System::Windows::Forms::Label^  label7;
public: System::Windows::Forms::TextBox^  textBox5;
public: System::Windows::Forms::TextBox^  textBox6;
public: System::Windows::Forms::TextBox^  textBox7;
public: System::Windows::Forms::TextBox^  textBox8;
private: System::Windows::Forms::Label^  label8;
private: System::Windows::Forms::Label^  label9;
private: System::Windows::Forms::Label^  label10;
private: System::Windows::Forms::Button^  button5;
private: System::Windows::Forms::Button^  button6;
private: System::Windows::Forms::Button^  button7;
private: System::Windows::Forms::Button^  button8;
private: System::Windows::Forms::Label^  label11;
private: System::Windows::Forms::Label^  label12;
private: System::Windows::Forms::Label^  label13;
public: System::Windows::Forms::TextBox^  textBox9;
public: System::Windows::Forms::TextBox^  textBox10;
public: System::Windows::Forms::TextBox^  textBox11;
private: System::Windows::Forms::Label^  label14;
private: System::Windows::Forms::Label^  label15;
private: System::Windows::Forms::Label^  label16;
public: System::Windows::Forms::TextBox^  textBox12; 
public: System::Windows::Forms::TextBox^  textBox13;
public: System::Windows::Forms::TextBox^  textBox14;
private: System::Windows::Forms::ContextMenuStrip^  _cms_AdjLive;
private: System::Windows::Forms::ToolStripMenuItem^  _miSparse;
private: System::Windows::Forms::ToolStripMenuItem^  _miTypicalLive;
private: System::Windows::Forms::ToolStripMenuItem^  _miAbundent;
private: System::Windows::Forms::ToolStripMenuItem^  _miUserLive;
private: System::Windows::Forms::TextBox^  _txMoist1kHr;
private: System::Windows::Forms::TextBox^  _txMoistDuff;
public: System::Windows::Forms::TextBox^  _txMoist10Hr;

private: System::Windows::Forms::Label^  _lbFolBraCon;

private: System::Windows::Forms::Label^  _lbMoistures;
private: System::Windows::Forms::Label^  _lbFuels;
private: System::Windows::Forms::ComboBox^  _cbSeason;
private: System::Windows::Forms::ComboBox^  _cbMoisture;
private: System::Windows::Forms::ComboBox^  _cbOutput;
private: System::Windows::Forms::Label^  _lbOutput;
private: System::Windows::Forms::TextBox^  _txOut_Duff;
private: System::Windows::Forms::TextBox^  _txOut_1kHr;
private: System::Windows::Forms::TextBox^  _txOut_Branch;
private: System::Windows::Forms::TextBox^  _txOut_Foliage;
private: System::Windows::Forms::TextBox^  _txOut_Shrub;
private: System::Windows::Forms::TextBox^  _txOut_Herb;
private: System::Windows::Forms::TextBox^  _txOut_1Hr;
private: System::Windows::Forms::TextBox^  _txOut_10Hr;
private: System::Windows::Forms::TextBox^  _txOut_100Hr;
private: System::Windows::Forms::TextBox^  _txOut_Litter;

private: System::Windows::Forms::RichTextBox^  _txReport;
private: System::Windows::Forms::ComboBox^  _cbCoverType;
private: System::Windows::Forms::Label^  _lbSeason;
private: System::Windows::Forms::ComboBox^  _cbFuelCat;
private: System::Windows::Forms::TextBox^  _txDuffDepth;

private: System::Windows::Forms::TextBox^  _txRotLog;
private: System::Windows::Forms::ComboBox^  _cbLogDist;
private: System::Windows::Forms::Label^  label4;
private: System::Windows::Forms::Label^  label17;

private: System::Windows::Forms::TextBox^  _txFolBraCon;
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

public: 

public: 


public: 


/*************************************************************************/
/*************************************************************************/
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->components = (gcnew System::ComponentModel::Container());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle23 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle32 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle33 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle24 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle25 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle26 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle27 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle28 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle29 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle30 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle31 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Wnd_Mai::typeid));
            this->_cms_AdjDead = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->_mi_Light = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->_mi_Typical = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->_mi_Heavy = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->_mi_User = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->_txLitter = (gcnew System::Windows::Forms::TextBox());
            this->_txDuff = (gcnew System::Windows::Forms::TextBox());
            this->_cmsReport = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->_cmiCopy = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->_cmiReportCut = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->_cmiReportPaste = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->_cmiReportPrint = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->_cmiPrintPreview = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->_cmiReportSave = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->_cmiClear = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->_tx1Hr = (gcnew System::Windows::Forms::TextBox());
            this->_tc_Main = (gcnew System::Windows::Forms::TabControl());
            this->_tcpFuel = (gcnew System::Windows::Forms::TabPage());
            this->label44 = (gcnew System::Windows::Forms::Label());
            this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
            this->label30 = (gcnew System::Windows::Forms::Label());
            this->label29 = (gcnew System::Windows::Forms::Label());
            this->label23 = (gcnew System::Windows::Forms::Label());
            this->label26 = (gcnew System::Windows::Forms::Label());
            this->label28 = (gcnew System::Windows::Forms::Label());
            this->label27 = (gcnew System::Windows::Forms::Label());
            this->_cbEmiDuf = (gcnew System::Windows::Forms::ComboBox());
            this->_cbEmiSmo = (gcnew System::Windows::Forms::ComboBox());
            this->_cbEmiFla = (gcnew System::Windows::Forms::ComboBox());
            this->_lbSeason = (gcnew System::Windows::Forms::Label());
            this->_cbSeason = (gcnew System::Windows::Forms::ComboBox());
            this->_lbDuff = (gcnew System::Windows::Forms::Label());
            this->label18 = (gcnew System::Windows::Forms::Label());
            this->_txDuffDepth = (gcnew System::Windows::Forms::TextBox());
            this->_btLightSpa = (gcnew System::Windows::Forms::Button());
            this->_btTypical = (gcnew System::Windows::Forms::Button());
            this->_btHeavyAbun = (gcnew System::Windows::Forms::Button());
            this->label20 = (gcnew System::Windows::Forms::Label());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->label17 = (gcnew System::Windows::Forms::Label());
            this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
            this->label43 = (gcnew System::Windows::Forms::Label());
            this->label422 = (gcnew System::Windows::Forms::Label());
            this->_txAmbAirTmp = (gcnew System::Windows::Forms::TextBox());
            this->_txMoistSoil = (gcnew System::Windows::Forms::TextBox());
            this->_cbSoil = (gcnew System::Windows::Forms::ComboBox());
            this->_cbLogDist = (gcnew System::Windows::Forms::ComboBox());
            this->_txRotLog = (gcnew System::Windows::Forms::TextBox());
            this->label4 = (gcnew System::Windows::Forms::Label());
            this->_txOut_DufDep = (gcnew System::Windows::Forms::TextBox());
            this->_txFolBraCon = (gcnew System::Windows::Forms::TextBox());
            this->_cbCoverType = (gcnew System::Windows::Forms::ComboBox());
            this->_cbFuelCat = (gcnew System::Windows::Forms::ComboBox());
            this->_lbOutput = (gcnew System::Windows::Forms::Label());
            this->_txOut_Duff = (gcnew System::Windows::Forms::TextBox());
            this->_cbOutput = (gcnew System::Windows::Forms::ComboBox());
            this->_txOut_1kHr = (gcnew System::Windows::Forms::TextBox());
            this->_cbFuelClass = (gcnew System::Windows::Forms::ComboBox());
            this->_txOut_Branch = (gcnew System::Windows::Forms::TextBox());
            this->_cbMoisture = (gcnew System::Windows::Forms::ComboBox());
            this->_txOut_Foliage = (gcnew System::Windows::Forms::TextBox());
            this->_txOut_Shrub = (gcnew System::Windows::Forms::TextBox());
            this->_txOut_Herb = (gcnew System::Windows::Forms::TextBox());
            this->_txOut_1Hr = (gcnew System::Windows::Forms::TextBox());
            this->_txOut_10Hr = (gcnew System::Windows::Forms::TextBox());
            this->_txOut_100Hr = (gcnew System::Windows::Forms::TextBox());
            this->_txOut_Litter = (gcnew System::Windows::Forms::TextBox());
            this->_lbFolBraCon = (gcnew System::Windows::Forms::Label());
            this->_lbMoistures = (gcnew System::Windows::Forms::Label());
            this->_lbFuels = (gcnew System::Windows::Forms::Label());
            this->_txMoist1kHr = (gcnew System::Windows::Forms::TextBox());
            this->_txMoistDuff = (gcnew System::Windows::Forms::TextBox());
            this->_txMoist10Hr = (gcnew System::Windows::Forms::TextBox());
            this->_lbFolige = (gcnew System::Windows::Forms::Label());
            this->_lbBranch = (gcnew System::Windows::Forms::Label());
            this->_lbHerb = (gcnew System::Windows::Forms::Label());
            this->_lbShrub = (gcnew System::Windows::Forms::Label());
            this->_txBranch = (gcnew System::Windows::Forms::TextBox());
            this->_cms_AdjLive = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->_miSparse = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->_miTypicalLive = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->_miAbundent = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->_miUserLive = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->_txFoliage = (gcnew System::Windows::Forms::TextBox());
            this->_txShrub = (gcnew System::Windows::Forms::TextBox());
            this->_txHerb = (gcnew System::Windows::Forms::TextBox());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->_lb1kHr = (gcnew System::Windows::Forms::Label());
            this->_lb100Hr = (gcnew System::Windows::Forms::Label());
            this->_lb10Hr = (gcnew System::Windows::Forms::Label());
            this->_tx1kHr = (gcnew System::Windows::Forms::TextBox());
            this->_tx100Hr = (gcnew System::Windows::Forms::TextBox());
            this->_tx10Hr = (gcnew System::Windows::Forms::TextBox());
            this->_lb1Hr = (gcnew System::Windows::Forms::Label());
            this->_lbLitter = (gcnew System::Windows::Forms::Label());
            this->shapeContainer1 = (gcnew Microsoft::VisualBasic::PowerPacks::ShapeContainer());
            this->lineShape3 = (gcnew Microsoft::VisualBasic::PowerPacks::LineShape());
            this->rectangleShape4 = (gcnew Microsoft::VisualBasic::PowerPacks::RectangleShape());
            this->rectangleShape3 = (gcnew Microsoft::VisualBasic::PowerPacks::RectangleShape());
            this->rectangleShape2 = (gcnew Microsoft::VisualBasic::PowerPacks::RectangleShape());
            this->rectangleShape1 = (gcnew Microsoft::VisualBasic::PowerPacks::RectangleShape());
            this->_tcpMortality = (gcnew System::Windows::Forms::TabPage());
            this->_btProb = (gcnew System::Windows::Forms::Button());
            this->_gbSC3 = (gcnew System::Windows::Forms::GroupBox());
            this->label37 = (gcnew System::Windows::Forms::Label());
            this->label38 = (gcnew System::Windows::Forms::Label());
            this->label39 = (gcnew System::Windows::Forms::Label());
            this->_txSC3C = (gcnew System::Windows::Forms::TextBox());
            this->_txSC3B = (gcnew System::Windows::Forms::TextBox());
            this->_txSC3A = (gcnew System::Windows::Forms::TextBox());
            this->_gbSC2 = (gcnew System::Windows::Forms::GroupBox());
            this->label34 = (gcnew System::Windows::Forms::Label());
            this->label35 = (gcnew System::Windows::Forms::Label());
            this->label36 = (gcnew System::Windows::Forms::Label());
            this->_txSC2C = (gcnew System::Windows::Forms::TextBox());
            this->_txSC2B = (gcnew System::Windows::Forms::TextBox());
            this->_txSC2A = (gcnew System::Windows::Forms::TextBox());
            this->_gbSC1 = (gcnew System::Windows::Forms::GroupBox());
            this->label33 = (gcnew System::Windows::Forms::Label());
            this->label32 = (gcnew System::Windows::Forms::Label());
            this->label31 = (gcnew System::Windows::Forms::Label());
            this->_txSC1C = (gcnew System::Windows::Forms::TextBox());
            this->_txSC1B = (gcnew System::Windows::Forms::TextBox());
            this->_txSC1A = (gcnew System::Windows::Forms::TextBox());
            this->_cb_SalRep = (gcnew System::Windows::Forms::CheckBox());
            this->_gbFlaSco = (gcnew System::Windows::Forms::GroupBox());
            this->_txFlaSco = (gcnew System::Windows::Forms::TextBox());
            this->_rbFlameLen = (gcnew System::Windows::Forms::RadioButton());
            this->_rbScorchHeight = (gcnew System::Windows::Forms::RadioButton());
            this->_gbFFI = (gcnew System::Windows::Forms::GroupBox());
            this->_bt_FFIErr = (gcnew System::Windows::Forms::Button());
            this->_btFFI_FileClose = (gcnew System::Windows::Forms::Button());
            this->label25 = (gcnew System::Windows::Forms::Label());
            this->_txFFIErr = (gcnew System::Windows::Forms::TextBox());
            this->_cbFFI_LoadAll = (gcnew System::Windows::Forms::CheckBox());
            this->_btTest = (gcnew System::Windows::Forms::Button());
            this->_txFFI_File = (gcnew System::Windows::Forms::TextBox());
            this->_lbFFIPlot = (gcnew System::Windows::Forms::Label());
            this->_cbFFI_Plots = (gcnew System::Windows::Forms::ComboBox());
            this->_chLowFireSev = (gcnew System::Windows::Forms::CheckBox());
            this->_btSelect = (gcnew System::Windows::Forms::Button());
            this->_dgMort = (gcnew System::Windows::Forms::DataGridView());
            this->_dgcSpecies = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->_cbEquTyp = (gcnew System::Windows::Forms::DataGridViewComboBoxColumn());
            this->_dgcDensity = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->_dgcDBH = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->_dgcHeight = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->_dgcCrownRatio = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->_dgcCrnDam = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->_dgcCKR = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->_dgcBeetle = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->_dgcBole = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->_dgcProb = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->_dgcGraph = (gcnew System::Windows::Forms::DataGridViewCheckBoxColumn());
            this->_cbSpecies = (gcnew System::Windows::Forms::ComboBox());
            this->shapeContainer3 = (gcnew Microsoft::VisualBasic::PowerPacks::ShapeContainer());
            this->lineShape2 = (gcnew Microsoft::VisualBasic::PowerPacks::LineShape());
            this->lineShape1 = (gcnew Microsoft::VisualBasic::PowerPacks::LineShape());
            this->_btSoil_Frm = (gcnew System::Windows::Forms::Button());
            this->_btEmis_Frm = (gcnew System::Windows::Forms::Button());
            this->_btFuel_Frm = (gcnew System::Windows::Forms::Button());
            this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
            this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->openProjectToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->saveProjectToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->newProjectToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->exitToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->optionsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->_cmBatch = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->createEmissionCSVFileSummaryEnglishToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->createEmissionCSVFIleToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->createEmissioToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->createSampleBurnupInputFilebrnToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->runBurnupFromInputFIlebrnToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->createSampleSoilInputFIledrfexpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->runSoilFromInputFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->saveSoilTempPointsFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripSeparator4 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->settingsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->helpToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->_cbRegion = (gcnew System::Windows::Forms::ComboBox());
            this->_txProFilNam = (gcnew System::Windows::Forms::TextBox());
            this->label6 = (gcnew System::Windows::Forms::Label());
            this->label7 = (gcnew System::Windows::Forms::Label());
            this->textBox5 = (gcnew System::Windows::Forms::TextBox());
            this->textBox6 = (gcnew System::Windows::Forms::TextBox());
            this->textBox7 = (gcnew System::Windows::Forms::TextBox());
            this->textBox8 = (gcnew System::Windows::Forms::TextBox());
            this->label8 = (gcnew System::Windows::Forms::Label());
            this->label9 = (gcnew System::Windows::Forms::Label());
            this->label10 = (gcnew System::Windows::Forms::Label());
            this->button5 = (gcnew System::Windows::Forms::Button());
            this->button6 = (gcnew System::Windows::Forms::Button());
            this->button7 = (gcnew System::Windows::Forms::Button());
            this->button8 = (gcnew System::Windows::Forms::Button());
            this->label11 = (gcnew System::Windows::Forms::Label());
            this->label12 = (gcnew System::Windows::Forms::Label());
            this->label13 = (gcnew System::Windows::Forms::Label());
            this->textBox9 = (gcnew System::Windows::Forms::TextBox());
            this->textBox10 = (gcnew System::Windows::Forms::TextBox());
            this->textBox11 = (gcnew System::Windows::Forms::TextBox());
            this->label14 = (gcnew System::Windows::Forms::Label());
            this->label15 = (gcnew System::Windows::Forms::Label());
            this->label16 = (gcnew System::Windows::Forms::Label());
            this->textBox12 = (gcnew System::Windows::Forms::TextBox());
            this->textBox13 = (gcnew System::Windows::Forms::TextBox());
            this->textBox14 = (gcnew System::Windows::Forms::TextBox());
            this->_txReport = (gcnew System::Windows::Forms::RichTextBox());
            this->printDialog1 = (gcnew System::Windows::Forms::PrintDialog());
            this->_pdReport = (gcnew System::Drawing::Printing::PrintDocument());
            this->printDocument1 = (gcnew System::Drawing::Printing::PrintDocument());
            this->printPreviewDialog1 = (gcnew System::Windows::Forms::PrintPreviewDialog());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->_btReportEmis = (gcnew System::Windows::Forms::Button());
            this->_btReportFuel = (gcnew System::Windows::Forms::Button());
            this->_btReportSoil = (gcnew System::Windows::Forms::Button());
            this->label21 = (gcnew System::Windows::Forms::Label());
            this->label5 = (gcnew System::Windows::Forms::Label());
            this->_txRepTot = (gcnew System::Windows::Forms::TextBox());
            this->_btRepTotClear = (gcnew System::Windows::Forms::Button());
            this->_btRepTotDisp = (gcnew System::Windows::Forms::Button());
            this->_cmsAdjNONE = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->toolStripMenuItem4 = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->_btReportMort = (gcnew System::Windows::Forms::Button());
            this->_btGraphMort = (gcnew System::Windows::Forms::Button());
            this->label19 = (gcnew System::Windows::Forms::Label());
            this->shapeContainer2 = (gcnew Microsoft::VisualBasic::PowerPacks::ShapeContainer());
            this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
            this->label22 = (gcnew System::Windows::Forms::Label());
            this->helpProvider1 = (gcnew System::Windows::Forms::HelpProvider());
            this->toolTip1 = (gcnew System::Windows::Forms::ToolTip(this->components));
            this->label24 = (gcnew System::Windows::Forms::Label());
            this->_btClearReport = (gcnew System::Windows::Forms::Button());
            this->_btFireIntensity = (gcnew System::Windows::Forms::Button());
            this->_btSoilMassman = (gcnew System::Windows::Forms::Button());
            this->_btSoiGraMas = (gcnew System::Windows::Forms::Button());
            this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
            this->_picFOFEM = (gcnew System::Windows::Forms::PictureBox());
            this->_btTip = (gcnew System::Windows::Forms::Button());
            this->_btMort_Dam = (gcnew System::Windows::Forms::Button());
            this->_btMort_Bol = (gcnew System::Windows::Forms::Button());
            this->label40 = (gcnew System::Windows::Forms::Label());
            this->label41 = (gcnew System::Windows::Forms::Label());
            this->_cms_AdjDead->SuspendLayout();
            this->_cmsReport->SuspendLayout();
            this->_tc_Main->SuspendLayout();
            this->_tcpFuel->SuspendLayout();
            this->groupBox3->SuspendLayout();
            this->groupBox2->SuspendLayout();
            this->_cms_AdjLive->SuspendLayout();
            this->_tcpMortality->SuspendLayout();
            this->_gbSC3->SuspendLayout();
            this->_gbSC2->SuspendLayout();
            this->_gbSC1->SuspendLayout();
            this->_gbFlaSco->SuspendLayout();
            this->_gbFFI->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_dgMort))->BeginInit();
            this->menuStrip1->SuspendLayout();
            this->_cmsAdjNONE->SuspendLayout();
            this->groupBox1->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_picFOFEM))->BeginInit();
            this->SuspendLayout();
            // 
            // _cms_AdjDead
            // 
            this->_cms_AdjDead->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->_mi_Light, this->_mi_Typical, 
                this->_mi_Heavy, this->_mi_User});
            this->_cms_AdjDead->Name = L"_cms_AdjDead";
            this->_cms_AdjDead->Size = System::Drawing::Size(108, 92);
            this->_cms_AdjDead->ItemClicked += gcnew System::Windows::Forms::ToolStripItemClickedEventHandler(this, &Wnd_Mai::_cms_AdjDead_ItemClicked);
            this->_cms_AdjDead->Opening += gcnew System::ComponentModel::CancelEventHandler(this, &Wnd_Mai::_cms_AdjDead_Opening);
            // 
            // _mi_Light
            // 
            this->_mi_Light->Name = L"_mi_Light";
            this->_mi_Light->Size = System::Drawing::Size(107, 22);
            this->_mi_Light->Text = L"Light";
            // 
            // _mi_Typical
            // 
            this->_mi_Typical->Name = L"_mi_Typical";
            this->_mi_Typical->Size = System::Drawing::Size(107, 22);
            this->_mi_Typical->Text = L"Typical";
            // 
            // _mi_Heavy
            // 
            this->_mi_Heavy->Name = L"_mi_Heavy";
            this->_mi_Heavy->Size = System::Drawing::Size(107, 22);
            this->_mi_Heavy->Text = L"Heavy";
            // 
            // _mi_User
            // 
            this->_mi_User->Name = L"_mi_User";
            this->_mi_User->Size = System::Drawing::Size(107, 22);
            this->_mi_User->Text = L"User";
            this->_mi_User->Click += gcnew System::EventHandler(this, &Wnd_Mai::_mi_User_Click);
            // 
            // _txLitter
            // 
            this->_txLitter->BackColor = System::Drawing::Color::White;
            this->_txLitter->ContextMenuStrip = this->_cms_AdjDead;
            this->_txLitter->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->helpProvider1->SetHelpKeyword(this->_txLitter, L"f1");
            this->helpProvider1->SetHelpString(this->_txLitter, L"Litter Load - tons per acre");
            this->_txLitter->Location = System::Drawing::Point(224, 58);
            this->_txLitter->Name = L"_txLitter";
            this->_txLitter->ReadOnly = true;
            this->helpProvider1->SetShowHelp(this->_txLitter, true);
            this->_txLitter->Size = System::Drawing::Size(50, 20);
            this->_txLitter->TabIndex = 8;
            this->_txLitter->Text = L"0";
            this->_txLitter->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txLitter, L"Litter Load in tons/are");
            this->_txLitter->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_txMoistDuff_KeyDown);
            this->_txLitter->Leave += gcnew System::EventHandler(this, &Wnd_Mai::_txLitter_Leave);
            // 
            // _txDuff
            // 
            this->_txDuff->BackColor = System::Drawing::Color::White;
            this->_txDuff->ContextMenuStrip = this->_cms_AdjDead;
            this->helpProvider1->SetHelpKeyword(this->_txDuff, L"f1");
            this->helpProvider1->SetHelpNavigator(this->_txDuff, System::Windows::Forms::HelpNavigator::KeywordIndex);
            this->helpProvider1->SetHelpString(this->_txDuff, L"Duff Fuel Load - tons per acre - If you enter a duff load you must also enter a d" 
                L"uff depth");
            this->_txDuff->Location = System::Drawing::Point(166, 58);
            this->_txDuff->Name = L"_txDuff";
            this->_txDuff->ReadOnly = true;
            this->helpProvider1->SetShowHelp(this->_txDuff, true);
            this->_txDuff->Size = System::Drawing::Size(50, 20);
            this->_txDuff->TabIndex = 6;
            this->_txDuff->Text = L"0";
            this->_txDuff->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txDuff, L"Duff load in tons/acre");
            this->_txDuff->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_txMoistDuff_KeyDown);
            this->_txDuff->Leave += gcnew System::EventHandler(this, &Wnd_Mai::_txDuff_Leave);
            // 
            // _cmsReport
            // 
            this->_cmsReport->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->_cmiCopy, this->_cmiReportCut, 
                this->_cmiReportPaste, this->_cmiReportPrint, this->_cmiPrintPreview, this->_cmiReportSave, this->_cmiClear});
            this->_cmsReport->Name = L"_cms_AdjDead";
            this->_cmsReport->Size = System::Drawing::Size(138, 158);
            this->_cmsReport->ItemClicked += gcnew System::Windows::Forms::ToolStripItemClickedEventHandler(this, &Wnd_Mai::_cmsReport_ItemClicked);
            // 
            // _cmiCopy
            // 
            this->_cmiCopy->Name = L"_cmiCopy";
            this->_cmiCopy->Size = System::Drawing::Size(137, 22);
            this->_cmiCopy->Text = L"Copy";
            // 
            // _cmiReportCut
            // 
            this->_cmiReportCut->Name = L"_cmiReportCut";
            this->_cmiReportCut->Size = System::Drawing::Size(137, 22);
            this->_cmiReportCut->Text = L"Cut";
            // 
            // _cmiReportPaste
            // 
            this->_cmiReportPaste->Name = L"_cmiReportPaste";
            this->_cmiReportPaste->Size = System::Drawing::Size(137, 22);
            this->_cmiReportPaste->Text = L"Paste";
            // 
            // _cmiReportPrint
            // 
            this->_cmiReportPrint->Name = L"_cmiReportPrint";
            this->_cmiReportPrint->Size = System::Drawing::Size(137, 22);
            this->_cmiReportPrint->Text = L"Print";
            // 
            // _cmiPrintPreview
            // 
            this->_cmiPrintPreview->Name = L"_cmiPrintPreview";
            this->_cmiPrintPreview->Size = System::Drawing::Size(137, 22);
            this->_cmiPrintPreview->Text = L"Print Preview";
            // 
            // _cmiReportSave
            // 
            this->_cmiReportSave->Name = L"_cmiReportSave";
            this->_cmiReportSave->Size = System::Drawing::Size(137, 22);
            this->_cmiReportSave->Text = L"Save";
            // 
            // _cmiClear
            // 
            this->_cmiClear->Name = L"_cmiClear";
            this->_cmiClear->Size = System::Drawing::Size(137, 22);
            this->_cmiClear->Text = L"Clear";
            // 
            // _tx1Hr
            // 
            this->_tx1Hr->BackColor = System::Drawing::Color::White;
            this->_tx1Hr->ContextMenuStrip = this->_cms_AdjDead;
            this->helpProvider1->SetHelpKeyword(this->_tx1Hr, L"f1");
            this->helpProvider1->SetHelpString(this->_tx1Hr, L"1 Hour fuel,   0 -> 1/4 inch - tons per acre");
            this->_tx1Hr->Location = System::Drawing::Point(278, 58);
            this->_tx1Hr->Name = L"_tx1Hr";
            this->_tx1Hr->ReadOnly = true;
            this->helpProvider1->SetShowHelp(this->_tx1Hr, true);
            this->_tx1Hr->Size = System::Drawing::Size(50, 20);
            this->_tx1Hr->TabIndex = 9;
            this->_tx1Hr->Text = L"0";
            this->_tx1Hr->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_tx1Hr, L"dead woody fuel 0-0.25 inch diameter in tons/acre\r\n1 hour fuel ");
            this->_tx1Hr->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_txMoistDuff_KeyDown);
            // 
            // _tc_Main
            // 
            this->_tc_Main->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
                | System::Windows::Forms::AnchorStyles::Right));
            this->_tc_Main->Controls->Add(this->_tcpFuel);
            this->_tc_Main->Controls->Add(this->_tcpMortality);
            this->_tc_Main->Location = System::Drawing::Point(0, 57);
            this->_tc_Main->Name = L"_tc_Main";
            this->_tc_Main->SelectedIndex = 0;
            this->_tc_Main->Size = System::Drawing::Size(924, 295);
            this->_tc_Main->TabIndex = 6;
            this->_tc_Main->TabStop = false;
            // 
            // _tcpFuel
            // 
            this->_tcpFuel->BackColor = System::Drawing::SystemColors::ScrollBar;
            this->_tcpFuel->Controls->Add(this->label44);
            this->_tcpFuel->Controls->Add(this->groupBox3);
            this->_tcpFuel->Controls->Add(this->_lbSeason);
            this->_tcpFuel->Controls->Add(this->_cbSeason);
            this->_tcpFuel->Controls->Add(this->_txDuff);
            this->_tcpFuel->Controls->Add(this->_lbDuff);
            this->_tcpFuel->Controls->Add(this->label18);
            this->_tcpFuel->Controls->Add(this->_txDuffDepth);
            this->_tcpFuel->Controls->Add(this->_btLightSpa);
            this->_tcpFuel->Controls->Add(this->_btTypical);
            this->_tcpFuel->Controls->Add(this->_btHeavyAbun);
            this->_tcpFuel->Controls->Add(this->label20);
            this->_tcpFuel->Controls->Add(this->label3);
            this->_tcpFuel->Controls->Add(this->label17);
            this->_tcpFuel->Controls->Add(this->groupBox2);
            this->_tcpFuel->Controls->Add(this->_cbLogDist);
            this->_tcpFuel->Controls->Add(this->_txRotLog);
            this->_tcpFuel->Controls->Add(this->label4);
            this->_tcpFuel->Controls->Add(this->_txOut_DufDep);
            this->_tcpFuel->Controls->Add(this->_txFolBraCon);
            this->_tcpFuel->Controls->Add(this->_cbCoverType);
            this->_tcpFuel->Controls->Add(this->_cbFuelCat);
            this->_tcpFuel->Controls->Add(this->_lbOutput);
            this->_tcpFuel->Controls->Add(this->_txOut_Duff);
            this->_tcpFuel->Controls->Add(this->_cbOutput);
            this->_tcpFuel->Controls->Add(this->_txOut_1kHr);
            this->_tcpFuel->Controls->Add(this->_cbFuelClass);
            this->_tcpFuel->Controls->Add(this->_txOut_Branch);
            this->_tcpFuel->Controls->Add(this->_cbMoisture);
            this->_tcpFuel->Controls->Add(this->_txOut_Foliage);
            this->_tcpFuel->Controls->Add(this->_txOut_Shrub);
            this->_tcpFuel->Controls->Add(this->_txOut_Herb);
            this->_tcpFuel->Controls->Add(this->_txOut_1Hr);
            this->_tcpFuel->Controls->Add(this->_txOut_10Hr);
            this->_tcpFuel->Controls->Add(this->_txOut_100Hr);
            this->_tcpFuel->Controls->Add(this->_txOut_Litter);
            this->_tcpFuel->Controls->Add(this->_lbFolBraCon);
            this->_tcpFuel->Controls->Add(this->_lbMoistures);
            this->_tcpFuel->Controls->Add(this->_lbFuels);
            this->_tcpFuel->Controls->Add(this->_txMoist1kHr);
            this->_tcpFuel->Controls->Add(this->_txMoistDuff);
            this->_tcpFuel->Controls->Add(this->_txMoist10Hr);
            this->_tcpFuel->Controls->Add(this->_lbFolige);
            this->_tcpFuel->Controls->Add(this->_lbBranch);
            this->_tcpFuel->Controls->Add(this->_lbHerb);
            this->_tcpFuel->Controls->Add(this->_lbShrub);
            this->_tcpFuel->Controls->Add(this->_txBranch);
            this->_tcpFuel->Controls->Add(this->_txFoliage);
            this->_tcpFuel->Controls->Add(this->_txShrub);
            this->_tcpFuel->Controls->Add(this->_txHerb);
            this->_tcpFuel->Controls->Add(this->label1);
            this->_tcpFuel->Controls->Add(this->_lb1kHr);
            this->_tcpFuel->Controls->Add(this->_lb100Hr);
            this->_tcpFuel->Controls->Add(this->_lb10Hr);
            this->_tcpFuel->Controls->Add(this->_tx1kHr);
            this->_tcpFuel->Controls->Add(this->_tx100Hr);
            this->_tcpFuel->Controls->Add(this->_tx10Hr);
            this->_tcpFuel->Controls->Add(this->_lb1Hr);
            this->_tcpFuel->Controls->Add(this->_lbLitter);
            this->_tcpFuel->Controls->Add(this->_txLitter);
            this->_tcpFuel->Controls->Add(this->_tx1Hr);
            this->_tcpFuel->Controls->Add(this->shapeContainer1);
            this->helpProvider1->SetHelpKeyword(this->_tcpFuel, L"f1");
            this->helpProvider1->SetHelpNavigator(this->_tcpFuel, System::Windows::Forms::HelpNavigator::Topic);
            this->helpProvider1->SetHelpString(this->_tcpFuel, L"To get help for a specific item, click on the item and/or move the cursor over it" 
                L" ");
            this->_tcpFuel->Location = System::Drawing::Point(4, 22);
            this->_tcpFuel->Name = L"_tcpFuel";
            this->_tcpFuel->Padding = System::Windows::Forms::Padding(3);
            this->helpProvider1->SetShowHelp(this->_tcpFuel, true);
            this->_tcpFuel->Size = System::Drawing::Size(916, 269);
            this->_tcpFuel->TabIndex = 0;
            this->_tcpFuel->Text = L"Fuel - Smoke - Soil";
            this->toolTip1->SetToolTip(this->_tcpFuel, L"Check this tab to estimate fuel consumption, smoke emissions, or soil heating");
            this->_tcpFuel->ToolTipText = L"Check this tab to estimate fuel consumption, smoke emissions, or soil heating";
            this->_tcpFuel->UseVisualStyleBackColor = true;
            this->_tcpFuel->Click += gcnew System::EventHandler(this, &Wnd_Mai::_tcpFuel_Click);
            this->_tcpFuel->Enter += gcnew System::EventHandler(this, &Wnd_Mai::_tcpFuel_Enter);
            // 
            // label44
            // 
            this->label44->AutoSize = true;
            this->label44->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label44->Location = System::Drawing::Point(784, 83);
            this->label44->Name = L"label44";
            this->label44->Size = System::Drawing::Size(88, 15);
            this->label44->TabIndex = 82;
            this->label44->Text = L"Soil Settings";
            // 
            // groupBox3
            // 
            this->groupBox3->BackColor = System::Drawing::Color::Transparent;
            this->groupBox3->Controls->Add(this->label30);
            this->groupBox3->Controls->Add(this->label29);
            this->groupBox3->Controls->Add(this->label23);
            this->groupBox3->Controls->Add(this->label26);
            this->groupBox3->Controls->Add(this->label28);
            this->groupBox3->Controls->Add(this->label27);
            this->groupBox3->Controls->Add(this->_cbEmiDuf);
            this->groupBox3->Controls->Add(this->_cbEmiSmo);
            this->groupBox3->Controls->Add(this->_cbEmiFla);
            this->groupBox3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->groupBox3->Location = System::Drawing::Point(236, 195);
            this->groupBox3->Name = L"groupBox3";
            this->groupBox3->Size = System::Drawing::Size(657, 57);
            this->groupBox3->TabIndex = 81;
            this->groupBox3->TabStop = false;
            this->groupBox3->Text = L"Emission Factors";
            this->groupBox3->VisibleChanged += gcnew System::EventHandler(this, &Wnd_Mai::groupBox3_VisibleChanged);
            // 
            // label30
            // 
            this->label30->AutoSize = true;
            this->label30->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label30->Location = System::Drawing::Point(464, 34);
            this->label30->Name = L"label30";
            this->label30->Size = System::Drawing::Size(32, 15);
            this->label30->TabIndex = 84;
            this->label30->Text = L"RSC";
            this->toolTip1->SetToolTip(this->label30, L"Woody Residual Smoldering \r\nCombustion Emission Factor\r\nGroup\r\n");
            // 
            // label29
            // 
            this->label29->AutoSize = true;
            this->label29->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label29->Location = System::Drawing::Point(291, 34);
            this->label29->Name = L"label29";
            this->label29->Size = System::Drawing::Size(32, 15);
            this->label29->TabIndex = 83;
            this->label29->Text = L"RSC";
            this->toolTip1->SetToolTip(this->label29, L"Duff Residual Smoldering \r\nCombustion Emission Factor\r\nGroup");
            // 
            // label23
            // 
            this->label23->AutoSize = true;
            this->label23->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label23->Location = System::Drawing::Point(9, 34);
            this->label23->Name = L"label23";
            this->label23->Size = System::Drawing::Size(95, 15);
            this->label23->TabIndex = 82;
            this->label23->Text = L"and Smoldering";
            this->toolTip1->SetToolTip(this->label23, L"Flaming and Short\r\nTerm Smoldering\r\nEmission Factor Group");
            // 
            // label26
            // 
            this->label26->AutoSize = true;
            this->label26->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label26->Location = System::Drawing::Point(7, 19);
            this->label26->Name = L"label26";
            this->label26->Size = System::Drawing::Size(113, 15);
            this->label26->TabIndex = 77;
            this->label26->Text = L"Short-term Flaming";
            this->toolTip1->SetToolTip(this->label26, L"Short-term Flaming \r\nand Smoldering\r\nEmission Factor Group ");
            // 
            // label28
            // 
            this->label28->AutoSize = true;
            this->label28->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label28->Location = System::Drawing::Point(464, 19);
            this->label28->Name = L"label28";
            this->label28->Size = System::Drawing::Size(81, 15);
            this->label28->TabIndex = 79;
            this->label28->Text = L"Coarse Wood";
            this->toolTip1->SetToolTip(this->label28, L"Woody Residual Smoldering \r\nCombustion Emission Factor\r\nGroup\r\n");
            // 
            // label27
            // 
            this->label27->AutoSize = true;
            this->label27->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label27->Location = System::Drawing::Point(292, 19);
            this->label27->Name = L"label27";
            this->label27->Size = System::Drawing::Size(29, 15);
            this->label27->TabIndex = 78;
            this->label27->Text = L"Duff";
            this->toolTip1->SetToolTip(this->label27, L"Duff Residual Smoldering \r\nCombustion Emission Factor\r\nGroup");
            // 
            // _cbEmiDuf
            // 
            this->_cbEmiDuf->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_cbEmiDuf->FormattingEnabled = true;
            this->helpProvider1->SetHelpKeyword(this->_cbEmiDuf, L"f1");
            this->_cbEmiDuf->Location = System::Drawing::Point(332, 25);
            this->_cbEmiDuf->Name = L"_cbEmiDuf";
            this->helpProvider1->SetShowHelp(this->_cbEmiDuf, true);
            this->_cbEmiDuf->Size = System::Drawing::Size(120, 21);
            this->_cbEmiDuf->TabIndex = 76;
            this->toolTip1->SetToolTip(this->_cbEmiDuf, L"Select Duff Residual Smoldering \r\nCombustion Emission Factor\r\nGroup\r\n");
            this->_cbEmiDuf->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_cbFuelClass_KeyDown);
            // 
            // _cbEmiSmo
            // 
            this->_cbEmiSmo->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_cbEmiSmo->FormattingEnabled = true;
            this->helpProvider1->SetHelpKeyword(this->_cbEmiSmo, L"f1");
            this->_cbEmiSmo->Location = System::Drawing::Point(548, 25);
            this->_cbEmiSmo->Name = L"_cbEmiSmo";
            this->helpProvider1->SetShowHelp(this->_cbEmiSmo, true);
            this->_cbEmiSmo->Size = System::Drawing::Size(104, 21);
            this->_cbEmiSmo->TabIndex = 75;
            this->toolTip1->SetToolTip(this->_cbEmiSmo, L"Select Woody Residual Smoldering \r\nCombustion Emission Factor\r\nGroup\r\n");
            this->_cbEmiSmo->SelectedIndexChanged += gcnew System::EventHandler(this, &Wnd_Mai::_cbEmiFla_SelectedIndexChanged);
            this->_cbEmiSmo->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_cbFuelClass_KeyDown);
            // 
            // _cbEmiFla
            // 
            this->_cbEmiFla->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_cbEmiFla->FormattingEnabled = true;
            this->helpProvider1->SetHelpKeyword(this->_cbEmiFla, L"f1");
            this->_cbEmiFla->Location = System::Drawing::Point(130, 25);
            this->_cbEmiFla->Name = L"_cbEmiFla";
            this->_cbEmiFla->RightToLeft = System::Windows::Forms::RightToLeft::No;
            this->helpProvider1->SetShowHelp(this->_cbEmiFla, true);
            this->_cbEmiFla->Size = System::Drawing::Size(150, 21);
            this->_cbEmiFla->TabIndex = 74;
            this->toolTip1->SetToolTip(this->_cbEmiFla, L"Select Flaming and Smoldering\r\nEmission Factor Group ");
            this->_cbEmiFla->SelectedIndexChanged += gcnew System::EventHandler(this, &Wnd_Mai::_cbEmiFla_SelectedIndexChanged);
            this->_cbEmiFla->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_cbFuelClass_KeyDown);
            // 
            // _lbSeason
            // 
            this->_lbSeason->AutoSize = true;
            this->_lbSeason->BackColor = System::Drawing::Color::Transparent;
            this->_lbSeason->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lbSeason->Location = System::Drawing::Point(762, 15);
            this->_lbSeason->Name = L"_lbSeason";
            this->_lbSeason->Size = System::Drawing::Size(55, 15);
            this->_lbSeason->TabIndex = 52;
            this->_lbSeason->Text = L"Season";
            this->toolTip1->SetToolTip(this->_lbSeason, L"Select the season of the burn");
            // 
            // _cbSeason
            // 
            this->_cbSeason->FormattingEnabled = true;
            this->helpProvider1->SetHelpKeyword(this->_cbSeason, L"f1");
            this->helpProvider1->SetHelpString(this->_cbSeason, L"Select the seaon ");
            this->_cbSeason->Location = System::Drawing::Point(822, 14);
            this->_cbSeason->Name = L"_cbSeason";
            this->helpProvider1->SetShowHelp(this->_cbSeason, true);
            this->_cbSeason->Size = System::Drawing::Size(68, 21);
            this->_cbSeason->TabIndex = 4;
            this->toolTip1->SetToolTip(this->_cbSeason, L"Select the season of the burn");
            this->_cbSeason->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_cbFuelClass_KeyDown);
            // 
            // _lbDuff
            // 
            this->_lbDuff->AutoSize = true;
            this->_lbDuff->BackColor = System::Drawing::Color::Transparent;
            this->_lbDuff->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lbDuff->Location = System::Drawing::Point(176, 40);
            this->_lbDuff->Name = L"_lbDuff";
            this->_lbDuff->Size = System::Drawing::Size(33, 15);
            this->_lbDuff->TabIndex = 8;
            this->_lbDuff->Text = L"Duff";
            this->toolTip1->SetToolTip(this->_lbDuff, L"Duff load in tons/acre");
            // 
            // label18
            // 
            this->label18->AutoSize = true;
            this->label18->BackColor = System::Drawing::Color::Transparent;
            this->label18->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label18->Location = System::Drawing::Point(172, 82);
            this->label18->Name = L"label18";
            this->label18->Size = System::Drawing::Size(41, 13);
            this->label18->TabIndex = 69;
            this->label18->Text = L"Depth";
            this->toolTip1->SetToolTip(this->label18, L"preburn depth of duff in inches ");
            // 
            // _txDuffDepth
            // 
            this->helpProvider1->SetHelpKeyword(this->_txDuffDepth, L"F1");
            this->helpProvider1->SetHelpNavigator(this->_txDuffDepth, System::Windows::Forms::HelpNavigator::Topic);
            this->helpProvider1->SetHelpString(this->_txDuffDepth, L"Duff Depth - Inches - A duff depth must be suppied when there is a duff fuel load" 
                L" ");
            this->_txDuffDepth->Location = System::Drawing::Point(166, 100);
            this->_txDuffDepth->Name = L"_txDuffDepth";
            this->helpProvider1->SetShowHelp(this->_txDuffDepth, true);
            this->_txDuffDepth->Size = System::Drawing::Size(50, 20);
            this->_txDuffDepth->TabIndex = 7;
            this->_txDuffDepth->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txDuffDepth, L"preburn depth of duff in inches ");
            this->_txDuffDepth->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_txMoistDuff_KeyDown);
            // 
            // _btLightSpa
            // 
            this->_btLightSpa->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)), 
                static_cast<System::Int32>(static_cast<System::Byte>(224)));
            this->_btLightSpa->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->helpProvider1->SetHelpKeyword(this->_btLightSpa, L"F1");
            this->helpProvider1->SetHelpString(this->_btLightSpa, L"Adjust all fuels to Light");
            this->_btLightSpa->Location = System::Drawing::Point(764, 56);
            this->_btLightSpa->Name = L"_btLightSpa";
            this->helpProvider1->SetShowHelp(this->_btLightSpa, true);
            this->_btLightSpa->Size = System::Drawing::Size(24, 22);
            this->_btLightSpa->TabIndex = 15;
            this->_btLightSpa->TabStop = false;
            this->_btLightSpa->Text = L"L";
            this->toolTip1->SetToolTip(this->_btLightSpa, L"Adjust all fuel component loadings to be less");
            this->_btLightSpa->UseVisualStyleBackColor = false;
            this->_btLightSpa->Click += gcnew System::EventHandler(this, &Wnd_Mai::_btLightSpa_Click);
            // 
            // _btTypical
            // 
            this->_btTypical->BackColor = System::Drawing::Color::Silver;
            this->_btTypical->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->helpProvider1->SetHelpKeyword(this->_btTypical, L"F1");
            this->helpProvider1->SetHelpString(this->_btTypical, L"Adjst all fuels to typical ");
            this->_btTypical->Location = System::Drawing::Point(812, 56);
            this->_btTypical->Name = L"_btTypical";
            this->helpProvider1->SetShowHelp(this->_btTypical, true);
            this->_btTypical->Size = System::Drawing::Size(24, 22);
            this->_btTypical->TabIndex = 17;
            this->_btTypical->TabStop = false;
            this->_btTypical->Text = L"T";
            this->toolTip1->SetToolTip(this->_btTypical, L"Adjust all fuel component loadings back to typical or average loadings");
            this->_btTypical->UseVisualStyleBackColor = false;
            this->_btTypical->Click += gcnew System::EventHandler(this, &Wnd_Mai::_btTypical_Click);
            // 
            // _btHeavyAbun
            // 
            this->_btHeavyAbun->BackColor = System::Drawing::Color::Gray;
            this->_btHeavyAbun->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->helpProvider1->SetHelpKeyword(this->_btHeavyAbun, L"f1");
            this->helpProvider1->SetHelpString(this->_btHeavyAbun, L"Adjust all fuels to heavy");
            this->_btHeavyAbun->Location = System::Drawing::Point(864, 56);
            this->_btHeavyAbun->Name = L"_btHeavyAbun";
            this->helpProvider1->SetShowHelp(this->_btHeavyAbun, true);
            this->_btHeavyAbun->Size = System::Drawing::Size(24, 22);
            this->_btHeavyAbun->TabIndex = 18;
            this->_btHeavyAbun->TabStop = false;
            this->_btHeavyAbun->Text = L"Heavy";
            this->toolTip1->SetToolTip(this->_btHeavyAbun, L"Adjust all fuel component loadings to be higher");
            this->_btHeavyAbun->UseVisualStyleBackColor = false;
            this->_btHeavyAbun->Click += gcnew System::EventHandler(this, &Wnd_Mai::_btHeavyAbun_Click);
            // 
            // label20
            // 
            this->label20->AutoSize = true;
            this->label20->BackColor = System::Drawing::Color::Transparent;
            this->label20->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label20->Location = System::Drawing::Point(13, 76);
            this->label20->Name = L"label20";
            this->label20->Size = System::Drawing::Size(47, 13);
            this->label20->TabIndex = 73;
            this->label20->Text = L" (T/ac)";
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->BackColor = System::Drawing::Color::Transparent;
            this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label3->Location = System::Drawing::Point(646, 83);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(91, 15);
            this->label3->TabIndex = 68;
            this->label3->Text = L"% Consumed";
            this->toolTip1->SetToolTip(this->label3, L"Enter percent of canopy fuel to be consumed in fire");
            // 
            // label17
            // 
            this->label17->AutoSize = true;
            this->label17->BackColor = System::Drawing::Color::Transparent;
            this->label17->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label17->Location = System::Drawing::Point(466, 85);
            this->label17->Name = L"label17";
            this->label17->Size = System::Drawing::Size(71, 13);
            this->label17->TabIndex = 60;
            this->label17->Tag = L"";
            this->label17->Text = L"Distribution";
            this->toolTip1->SetToolTip(this->label17, L"Enter log loading distribution from menu - Hit F1 for more details");
            // 
            // groupBox2
            // 
            this->groupBox2->BackColor = System::Drawing::Color::Transparent;
            this->groupBox2->Controls->Add(this->label43);
            this->groupBox2->Controls->Add(this->label422);
            this->groupBox2->Controls->Add(this->_txAmbAirTmp);
            this->groupBox2->Controls->Add(this->_txMoistSoil);
            this->groupBox2->Controls->Add(this->_cbSoil);
            this->groupBox2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->helpProvider1->SetHelpKeyword(this->groupBox2, L"f1");
            this->helpProvider1->SetHelpString(this->groupBox2, L"Soil Moisture. This value is only used when you are doing a soil simulation ");
            this->groupBox2->Location = System::Drawing::Point(763, 81);
            this->groupBox2->Name = L"groupBox2";
            this->helpProvider1->SetShowHelp(this->groupBox2, true);
            this->groupBox2->Size = System::Drawing::Size(130, 114);
            this->groupBox2->TabIndex = 66;
            this->groupBox2->TabStop = false;
            this->toolTip1->SetToolTip(this->groupBox2, L"Soil Moisture. This value is only used when you are doing a soil simulation ");
            // 
            // label43
            // 
            this->label43->AutoSize = true;
            this->label43->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label43->Location = System::Drawing::Point(3, 82);
            this->label43->Name = L"label43";
            this->label43->Size = System::Drawing::Size(78, 13);
            this->label43->TabIndex = 28;
            this->label43->Text = L"Amb Temp C";
            this->toolTip1->SetToolTip(this->label43, L"Ambient Temperature");
            // 
            // label422
            // 
            this->label422->AutoSize = true;
            this->label422->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label422->Location = System::Drawing::Point(6, 53);
            this->label422->Name = L"label422";
            this->label422->Size = System::Drawing::Size(55, 13);
            this->label422->TabIndex = 27;
            this->label422->Text = L"Moisture";
            this->toolTip1->SetToolTip(this->label422, L"Percent soil moisture (% volumetric)\r\nThis value is only used when you are \r\ndoin" 
                L"g a soil simulation\r\n");
            // 
            // _txAmbAirTmp
            // 
            this->_txAmbAirTmp->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_txAmbAirTmp->Location = System::Drawing::Point(89, 77);
            this->_txAmbAirTmp->Name = L"_txAmbAirTmp";
            this->_txAmbAirTmp->Size = System::Drawing::Size(32, 22);
            this->_txAmbAirTmp->TabIndex = 26;
            this->_txAmbAirTmp->Text = L"21";
            this->_txAmbAirTmp->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txMoistSoil
            // 
            this->_txMoistSoil->BackColor = System::Drawing::Color::SkyBlue;
            this->_txMoistSoil->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_txMoistSoil->Location = System::Drawing::Point(74, 52);
            this->_txMoistSoil->Name = L"_txMoistSoil";
            this->_txMoistSoil->Size = System::Drawing::Size(50, 20);
            this->_txMoistSoil->TabIndex = 24;
            this->_txMoistSoil->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txMoistSoil, L"Percent soil moisture (% volumetric)\r\nThis value is only used when you are\r\ndoing" 
                L" a soil simulation.");
            this->_txMoistSoil->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_txMoistDuff_KeyDown);
            // 
            // _cbSoil
            // 
            this->_cbSoil->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_cbSoil->FormattingEnabled = true;
            this->helpProvider1->SetHelpKeyword(this->_cbSoil, L"f1");
            this->helpProvider1->SetHelpString(this->_cbSoil, L"Select the type of soil. This setting is only used if you are doing a soil simula" 
                L"tion ");
            this->_cbSoil->Location = System::Drawing::Point(5, 19);
            this->_cbSoil->Name = L"_cbSoil";
            this->helpProvider1->SetShowHelp(this->_cbSoil, true);
            this->_cbSoil->Size = System::Drawing::Size(120, 21);
            this->_cbSoil->TabIndex = 25;
            this->toolTip1->SetToolTip(this->_cbSoil, L"Select soil type (denisty/texture). \r\nThis setting is only relevant if you \r\nare " 
                L"doing a soil simulation \r\n ");
            this->_cbSoil->SelectedIndexChanged += gcnew System::EventHandler(this, &Wnd_Mai::_cbSoil_SelectedIndexChanged);
            this->_cbSoil->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_cbFuelClass_KeyDown);
            // 
            // _cbLogDist
            // 
            this->_cbLogDist->FormattingEnabled = true;
            this->helpProvider1->SetHelpKeyword(this->_cbLogDist, L"f1");
            this->helpProvider1->SetHelpString(this->_cbLogDist, L"Select the log distribution for 3+ (1000 hour fuel) , Even  ||||,  Right ==||, Le" 
                L"ft ||==, Center =||=, End |==|,  size classes are 3, 6,  9, 20+ inch ");
            this->_cbLogDist->Location = System::Drawing::Point(470, 100);
            this->_cbLogDist->Name = L"_cbLogDist";
            this->helpProvider1->SetShowHelp(this->_cbLogDist, true);
            this->_cbLogDist->Size = System::Drawing::Size(61, 21);
            this->_cbLogDist->TabIndex = 14;
            this->toolTip1->SetToolTip(this->_cbLogDist, L"Enter log loading distribution from menu - Hit F1 for more details");
            this->_cbLogDist->SelectedIndexChanged += gcnew System::EventHandler(this, &Wnd_Mai::_cbLogDist_SelectedIndexChanged);
            this->_cbLogDist->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_cbFuelClass_KeyDown);
            // 
            // _txRotLog
            // 
            this->_txRotLog->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->helpProvider1->SetHelpKeyword(this->_txRotLog, L"f1");
            this->helpProvider1->SetHelpString(this->_txRotLog, L"percent of 3+ (1000 hour) fuels,  0 -> 100");
            this->_txRotLog->Location = System::Drawing::Point(410, 100);
            this->_txRotLog->Name = L"_txRotLog";
            this->helpProvider1->SetShowHelp(this->_txRotLog, true);
            this->_txRotLog->Size = System::Drawing::Size(50, 20);
            this->_txRotLog->TabIndex = 13;
            this->_txRotLog->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txRotLog, L"Enter proportion of log (3+ in) load that is rotten");
            this->_txRotLog->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_txMoistDuff_KeyDown);
            // 
            // label4
            // 
            this->label4->AutoSize = true;
            this->label4->BackColor = System::Drawing::Color::Transparent;
            this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label4->Location = System::Drawing::Point(402, 85);
            this->label4->Name = L"label4";
            this->label4->Size = System::Drawing::Size(58, 13);
            this->label4->TabIndex = 58;
            this->label4->Text = L"% Rotten";
            this->toolTip1->SetToolTip(this->label4, L"Enter proportion of log (3+ in) load that is rotten ");
            // 
            // _txOut_DufDep
            // 
            this->_txOut_DufDep->BackColor = System::Drawing::Color::Linen;
            this->_txOut_DufDep->Location = System::Drawing::Point(166, 186);
            this->_txOut_DufDep->Name = L"_txOut_DufDep";
            this->_txOut_DufDep->Size = System::Drawing::Size(50, 20);
            this->_txOut_DufDep->TabIndex = 61;
            this->_txOut_DufDep->TabStop = false;
            this->_txOut_DufDep->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txOut_DufDep, L"duff depth post burn calculated value.\r\nThe value shown is for the output type\r\ny" 
                L"ou have selected.\r\n    Duff depth consumed\r\n    Post duff depth\r\n    Percent of " 
                L"duff depth consumed\r\n");
            // 
            // _txFolBraCon
            // 
            this->_txFolBraCon->Location = System::Drawing::Point(662, 100);
            this->_txFolBraCon->Name = L"_txFolBraCon";
            this->_txFolBraCon->Size = System::Drawing::Size(57, 20);
            this->_txFolBraCon->TabIndex = 19;
            this->_txFolBraCon->Text = L"50";
            this->_txFolBraCon->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txFolBraCon, L"Enter percent of canopy fuel to be consumed in fire");
            this->_txFolBraCon->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_txMoistDuff_KeyDown);
            // 
            // _cbCoverType
            // 
            this->_cbCoverType->FormattingEnabled = true;
            this->helpProvider1->SetHelpKeyword(this->_cbCoverType, L"f1");
            this->helpProvider1->SetHelpNavigator(this->_cbCoverType, System::Windows::Forms::HelpNavigator::Topic);
            this->helpProvider1->SetHelpString(this->_cbCoverType, L"Select  cover/fuel type");
            this->_cbCoverType->Location = System::Drawing::Point(166, 14);
            this->_cbCoverType->Name = L"_cbCoverType";
            this->_cbCoverType->RightToLeft = System::Windows::Forms::RightToLeft::No;
            this->helpProvider1->SetShowHelp(this->_cbCoverType, true);
            this->_cbCoverType->Size = System::Drawing::Size(582, 21);
            this->_cbCoverType->TabIndex = 3;
            this->toolTip1->SetToolTip(this->_cbCoverType, L"Select a cover type from the list");
            this->_cbCoverType->SelectedIndexChanged += gcnew System::EventHandler(this, &Wnd_Mai::_cbCoverType_SelectedIndexChanged);
            this->_cbCoverType->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_cbFuelClass_KeyDown);
            // 
            // _cbFuelCat
            // 
            this->_cbFuelCat->FormattingEnabled = true;
            this->helpProvider1->SetHelpKeyword(this->_cbFuelCat, L"f1");
            this->helpProvider1->SetHelpNavigator(this->_cbFuelCat, System::Windows::Forms::HelpNavigator::Topic);
            this->helpProvider1->SetHelpString(this->_cbFuelCat, L"Select the fuel category you wish to use - Note: some individual fuel loads my ch" 
                L"ange based on this setting.");
            this->_cbFuelCat->Location = System::Drawing::Point(82, 57);
            this->_cbFuelCat->Name = L"_cbFuelCat";
            this->helpProvider1->SetShowHelp(this->_cbFuelCat, true);
            this->_cbFuelCat->Size = System::Drawing::Size(78, 21);
            this->_cbFuelCat->TabIndex = 5;
            this->toolTip1->SetToolTip(this->_cbFuelCat, L"Select the type of fuelbed: natural, slash");
            this->_cbFuelCat->SelectedIndexChanged += gcnew System::EventHandler(this, &Wnd_Mai::_cbFuelCat_SelectedIndexChanged);
            this->_cbFuelCat->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_cbFuelClass_KeyDown);
            // 
            // _lbOutput
            // 
            this->_lbOutput->AutoSize = true;
            this->_lbOutput->BackColor = System::Drawing::Color::Transparent;
            this->_lbOutput->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lbOutput->Location = System::Drawing::Point(19, 165);
            this->_lbOutput->Name = L"_lbOutput";
            this->_lbOutput->Size = System::Drawing::Size(49, 15);
            this->_lbOutput->TabIndex = 49;
            this->_lbOutput->Text = L"Output";
            this->toolTip1->SetToolTip(this->_lbOutput, L"Select type of output displayed on input page");
            // 
            // _txOut_Duff
            // 
            this->_txOut_Duff->BackColor = System::Drawing::Color::Linen;
            this->_txOut_Duff->Location = System::Drawing::Point(166, 160);
            this->_txOut_Duff->Name = L"_txOut_Duff";
            this->_txOut_Duff->ReadOnly = true;
            this->_txOut_Duff->Size = System::Drawing::Size(50, 20);
            this->_txOut_Duff->TabIndex = 48;
            this->_txOut_Duff->TabStop = false;
            this->_txOut_Duff->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txOut_Duff, L"post burn fuel ");
            // 
            // _cbOutput
            // 
            this->_cbOutput->BackColor = System::Drawing::Color::Linen;
            this->_cbOutput->FormattingEnabled = true;
            this->_cbOutput->Location = System::Drawing::Point(74, 160);
            this->_cbOutput->Name = L"_cbOutput";
            this->_cbOutput->Size = System::Drawing::Size(85, 21);
            this->_cbOutput->TabIndex = 26;
            this->toolTip1->SetToolTip(this->_cbOutput, L"Select type of output displayed on input page");
            this->_cbOutput->SelectedIndexChanged += gcnew System::EventHandler(this, &Wnd_Mai::_cbOutput_SelectedIndexChanged);
            this->_cbOutput->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_cbFuelClass_KeyDown);
            // 
            // _txOut_1kHr
            // 
            this->_txOut_1kHr->BackColor = System::Drawing::Color::Linen;
            this->_txOut_1kHr->Location = System::Drawing::Point(442, 160);
            this->_txOut_1kHr->Name = L"_txOut_1kHr";
            this->_txOut_1kHr->ReadOnly = true;
            this->_txOut_1kHr->Size = System::Drawing::Size(50, 20);
            this->_txOut_1kHr->TabIndex = 47;
            this->_txOut_1kHr->TabStop = false;
            this->_txOut_1kHr->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _cbFuelClass
            // 
            this->_cbFuelClass->BackColor = System::Drawing::Color::White;
            this->helpProvider1->SetHelpKeyword(this->_cbFuelClass, L"f1");
            this->helpProvider1->SetHelpNavigator(this->_cbFuelClass, System::Windows::Forms::HelpNavigator::Topic);
            this->helpProvider1->SetHelpString(this->_cbFuelClass, L"Select the cover type classification system that you wish to use ");
            this->_cbFuelClass->Location = System::Drawing::Point(82, 14);
            this->_cbFuelClass->Name = L"_cbFuelClass";
            this->helpProvider1->SetShowHelp(this->_cbFuelClass, true);
            this->_cbFuelClass->Size = System::Drawing::Size(78, 21);
            this->_cbFuelClass->TabIndex = 2;
            this->toolTip1->SetToolTip(this->_cbFuelClass, L"Select the cover type classification you want to use");
            this->_cbFuelClass->SelectedIndexChanged += gcnew System::EventHandler(this, &Wnd_Mai::_cbFuelClass_SelectedIndexChanged);
            this->_cbFuelClass->Enter += gcnew System::EventHandler(this, &Wnd_Mai::_cbFuelClass_Enter);
            this->_cbFuelClass->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_cbFuelClass_KeyDown);
            this->_cbFuelClass->DropDown += gcnew System::EventHandler(this, &Wnd_Mai::_cbFuelClass_DropDown);
            // 
            // _txOut_Branch
            // 
            this->_txOut_Branch->BackColor = System::Drawing::Color::Linen;
            this->_txOut_Branch->Location = System::Drawing::Point(692, 160);
            this->_txOut_Branch->Name = L"_txOut_Branch";
            this->_txOut_Branch->ReadOnly = true;
            this->_txOut_Branch->Size = System::Drawing::Size(50, 20);
            this->_txOut_Branch->TabIndex = 46;
            this->_txOut_Branch->TabStop = false;
            this->_txOut_Branch->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _cbMoisture
            // 
            this->_cbMoisture->BackColor = System::Drawing::Color::SkyBlue;
            this->_cbMoisture->FormattingEnabled = true;
            this->_cbMoisture->Location = System::Drawing::Point(82, 134);
            this->_cbMoisture->Name = L"_cbMoisture";
            this->_cbMoisture->Size = System::Drawing::Size(78, 21);
            this->_cbMoisture->TabIndex = 20;
            this->toolTip1->SetToolTip(this->_cbMoisture, L"Moisture conditions at time\r\nof burn (% gravimetric)");
            this->_cbMoisture->SelectedIndexChanged += gcnew System::EventHandler(this, &Wnd_Mai::_cbMoist_SelectedIndexChanged);
            this->_cbMoisture->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_cbFuelClass_KeyDown);
            // 
            // _txOut_Foliage
            // 
            this->_txOut_Foliage->BackColor = System::Drawing::Color::Linen;
            this->_txOut_Foliage->Location = System::Drawing::Point(638, 160);
            this->_txOut_Foliage->Name = L"_txOut_Foliage";
            this->_txOut_Foliage->ReadOnly = true;
            this->_txOut_Foliage->Size = System::Drawing::Size(50, 20);
            this->_txOut_Foliage->TabIndex = 45;
            this->_txOut_Foliage->TabStop = false;
            this->_txOut_Foliage->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txOut_Shrub
            // 
            this->_txOut_Shrub->BackColor = System::Drawing::Color::Linen;
            this->_txOut_Shrub->Location = System::Drawing::Point(566, 160);
            this->_txOut_Shrub->Name = L"_txOut_Shrub";
            this->_txOut_Shrub->ReadOnly = true;
            this->_txOut_Shrub->Size = System::Drawing::Size(50, 20);
            this->_txOut_Shrub->TabIndex = 44;
            this->_txOut_Shrub->TabStop = false;
            this->_txOut_Shrub->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txOut_Herb
            // 
            this->_txOut_Herb->BackColor = System::Drawing::Color::Linen;
            this->_txOut_Herb->Location = System::Drawing::Point(512, 160);
            this->_txOut_Herb->Name = L"_txOut_Herb";
            this->_txOut_Herb->ReadOnly = true;
            this->_txOut_Herb->Size = System::Drawing::Size(50, 20);
            this->_txOut_Herb->TabIndex = 43;
            this->_txOut_Herb->TabStop = false;
            this->_txOut_Herb->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txOut_1Hr
            // 
            this->_txOut_1Hr->BackColor = System::Drawing::Color::Linen;
            this->_txOut_1Hr->Location = System::Drawing::Point(278, 160);
            this->_txOut_1Hr->Name = L"_txOut_1Hr";
            this->_txOut_1Hr->ReadOnly = true;
            this->_txOut_1Hr->Size = System::Drawing::Size(50, 20);
            this->_txOut_1Hr->TabIndex = 42;
            this->_txOut_1Hr->TabStop = false;
            this->_txOut_1Hr->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txOut_10Hr
            // 
            this->_txOut_10Hr->BackColor = System::Drawing::Color::Linen;
            this->_txOut_10Hr->Location = System::Drawing::Point(332, 160);
            this->_txOut_10Hr->Name = L"_txOut_10Hr";
            this->_txOut_10Hr->ReadOnly = true;
            this->_txOut_10Hr->Size = System::Drawing::Size(50, 20);
            this->_txOut_10Hr->TabIndex = 41;
            this->_txOut_10Hr->TabStop = false;
            this->_txOut_10Hr->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txOut_100Hr
            // 
            this->_txOut_100Hr->BackColor = System::Drawing::Color::Linen;
            this->_txOut_100Hr->Location = System::Drawing::Point(384, 160);
            this->_txOut_100Hr->Name = L"_txOut_100Hr";
            this->_txOut_100Hr->ReadOnly = true;
            this->_txOut_100Hr->Size = System::Drawing::Size(50, 20);
            this->_txOut_100Hr->TabIndex = 40;
            this->_txOut_100Hr->TabStop = false;
            this->_txOut_100Hr->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txOut_Litter
            // 
            this->_txOut_Litter->BackColor = System::Drawing::Color::Linen;
            this->_txOut_Litter->Location = System::Drawing::Point(224, 160);
            this->_txOut_Litter->Name = L"_txOut_Litter";
            this->_txOut_Litter->ReadOnly = true;
            this->_txOut_Litter->Size = System::Drawing::Size(50, 20);
            this->_txOut_Litter->TabIndex = 39;
            this->_txOut_Litter->TabStop = false;
            this->_txOut_Litter->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _lbFolBraCon
            // 
            this->_lbFolBraCon->AutoSize = true;
            this->_lbFolBraCon->BackColor = System::Drawing::Color::Transparent;
            this->_lbFolBraCon->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lbFolBraCon->Location = System::Drawing::Point(784, 39);
            this->_lbFolBraCon->Name = L"_lbFolBraCon";
            this->_lbFolBraCon->Size = System::Drawing::Size(85, 15);
            this->_lbFolBraCon->TabIndex = 35;
            this->_lbFolBraCon->Text = L"Adjustments";
            // 
            // _lbMoistures
            // 
            this->_lbMoistures->AutoSize = true;
            this->_lbMoistures->BackColor = System::Drawing::Color::Transparent;
            this->_lbMoistures->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lbMoistures->Location = System::Drawing::Point(5, 140);
            this->_lbMoistures->Name = L"_lbMoistures";
            this->_lbMoistures->Size = System::Drawing::Size(70, 15);
            this->_lbMoistures->TabIndex = 34;
            this->_lbMoistures->Text = L"Moistures";
            this->toolTip1->SetToolTip(this->_lbMoistures, L"Moisture conditions at time\r\nof burn (% gravimetric)");
            // 
            // _lbFuels
            // 
            this->_lbFuels->AutoSize = true;
            this->_lbFuels->BackColor = System::Drawing::Color::Transparent;
            this->_lbFuels->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lbFuels->Location = System::Drawing::Point(8, 60);
            this->_lbFuels->Name = L"_lbFuels";
            this->_lbFuels->Size = System::Drawing::Size(63, 13);
            this->_lbFuels->TabIndex = 33;
            this->_lbFuels->Text = L"Fuel Type";
            this->toolTip1->SetToolTip(this->_lbFuels, L"Select the type of fuelbed-natural, pile, slash");
            // 
            // _txMoist1kHr
            // 
            this->_txMoist1kHr->BackColor = System::Drawing::Color::SkyBlue;
            this->_txMoist1kHr->Location = System::Drawing::Point(442, 134);
            this->_txMoist1kHr->Name = L"_txMoist1kHr";
            this->_txMoist1kHr->Size = System::Drawing::Size(50, 20);
            this->_txMoist1kHr->TabIndex = 23;
            this->_txMoist1kHr->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txMoist1kHr, L"Dead 1000 hour (3+ inch) woody fuel  moisture content in percent\"");
            this->_txMoist1kHr->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_txMoistDuff_KeyDown);
            // 
            // _txMoistDuff
            // 
            this->_txMoistDuff->BackColor = System::Drawing::Color::SkyBlue;
            this->_txMoistDuff->Location = System::Drawing::Point(166, 134);
            this->_txMoistDuff->Name = L"_txMoistDuff";
            this->_txMoistDuff->Size = System::Drawing::Size(50, 20);
            this->_txMoistDuff->TabIndex = 21;
            this->_txMoistDuff->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txMoistDuff, L"Duff moisture content in percent.\r\n---> click in the textbox and hit \r\n       F1 " 
                L"for specials notes on duff \r\n      moisture settings");
            this->_txMoistDuff->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_txMoistDuff_KeyDown);
            // 
            // _txMoist10Hr
            // 
            this->_txMoist10Hr->BackColor = System::Drawing::Color::SkyBlue;
            this->_txMoist10Hr->Location = System::Drawing::Point(332, 134);
            this->_txMoist10Hr->Name = L"_txMoist10Hr";
            this->_txMoist10Hr->Size = System::Drawing::Size(50, 20);
            this->_txMoist10Hr->TabIndex = 22;
            this->_txMoist10Hr->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txMoist10Hr, L"Dead 10 hour (1/4-1 inch) woody fuel moisture content in percent\"");
            this->_txMoist10Hr->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_txMoistDuff_KeyDown);
            // 
            // _lbFolige
            // 
            this->_lbFolige->AutoSize = true;
            this->_lbFolige->BackColor = System::Drawing::Color::Transparent;
            this->_lbFolige->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lbFolige->Location = System::Drawing::Point(634, 40);
            this->_lbFolige->Name = L"_lbFolige";
            this->_lbFolige->Size = System::Drawing::Size(55, 15);
            this->_lbFolige->TabIndex = 29;
            this->_lbFolige->Text = L"Foliage";
            this->toolTip1->SetToolTip(this->_lbFolige, L"load of canopy foliage fuel in tons/acre");
            // 
            // _lbBranch
            // 
            this->_lbBranch->AutoSize = true;
            this->_lbBranch->BackColor = System::Drawing::Color::Transparent;
            this->_lbBranch->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lbBranch->Location = System::Drawing::Point(693, 40);
            this->_lbBranch->Name = L"_lbBranch";
            this->_lbBranch->Size = System::Drawing::Size(52, 15);
            this->_lbBranch->TabIndex = 28;
            this->_lbBranch->Text = L"Branch";
            this->toolTip1->SetToolTip(this->_lbBranch, L"load of canopy small branch wood in tons/acre");
            // 
            // _lbHerb
            // 
            this->_lbHerb->AutoSize = true;
            this->_lbHerb->BackColor = System::Drawing::Color::Transparent;
            this->_lbHerb->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lbHerb->Location = System::Drawing::Point(520, 40);
            this->_lbHerb->Name = L"_lbHerb";
            this->_lbHerb->Size = System::Drawing::Size(38, 15);
            this->_lbHerb->TabIndex = 27;
            this->_lbHerb->Text = L"Herb";
            this->toolTip1->SetToolTip(this->_lbHerb, L"load of herbaceous fuel in tons/acre");
            this->_lbHerb->Click += gcnew System::EventHandler(this, &Wnd_Mai::_lbHerb_Click);
            // 
            // _lbShrub
            // 
            this->_lbShrub->AutoSize = true;
            this->_lbShrub->BackColor = System::Drawing::Color::Transparent;
            this->_lbShrub->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lbShrub->Location = System::Drawing::Point(571, 40);
            this->_lbShrub->Name = L"_lbShrub";
            this->_lbShrub->Size = System::Drawing::Size(45, 15);
            this->_lbShrub->TabIndex = 26;
            this->_lbShrub->Text = L"Shrub";
            this->toolTip1->SetToolTip(this->_lbShrub, L"load of shrubs in tons/acre");
            // 
            // _txBranch
            // 
            this->_txBranch->BackColor = System::Drawing::Color::White;
            this->_txBranch->ContextMenuStrip = this->_cms_AdjLive;
            this->helpProvider1->SetHelpKeyword(this->_txBranch, L"f1");
            this->helpProvider1->SetHelpString(this->_txBranch, L"Branch Fuels");
            this->_txBranch->Location = System::Drawing::Point(692, 58);
            this->_txBranch->Name = L"_txBranch";
            this->_txBranch->ReadOnly = true;
            this->helpProvider1->SetShowHelp(this->_txBranch, true);
            this->_txBranch->Size = System::Drawing::Size(50, 20);
            this->_txBranch->TabIndex = 18;
            this->_txBranch->Text = L"0";
            this->_txBranch->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txBranch, L"load of canopy small branch wood in tons/acre");
            this->_txBranch->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_txMoistDuff_KeyDown);
            // 
            // _cms_AdjLive
            // 
            this->_cms_AdjLive->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->_miSparse, this->_miTypicalLive, 
                this->_miAbundent, this->_miUserLive});
            this->_cms_AdjLive->Name = L"_cms_AdjDead";
            this->_cms_AdjLive->Size = System::Drawing::Size(122, 92);
            this->_cms_AdjLive->ItemClicked += gcnew System::Windows::Forms::ToolStripItemClickedEventHandler(this, &Wnd_Mai::_cms_AdjLive_Item);
            // 
            // _miSparse
            // 
            this->_miSparse->Name = L"_miSparse";
            this->_miSparse->Size = System::Drawing::Size(121, 22);
            this->_miSparse->Text = L"Sparse";
            // 
            // _miTypicalLive
            // 
            this->_miTypicalLive->Name = L"_miTypicalLive";
            this->_miTypicalLive->Size = System::Drawing::Size(121, 22);
            this->_miTypicalLive->Text = L"Typical";
            // 
            // _miAbundent
            // 
            this->_miAbundent->Name = L"_miAbundent";
            this->_miAbundent->Size = System::Drawing::Size(121, 22);
            this->_miAbundent->Text = L"Abundant";
            // 
            // _miUserLive
            // 
            this->_miUserLive->Name = L"_miUserLive";
            this->_miUserLive->Size = System::Drawing::Size(121, 22);
            this->_miUserLive->Text = L"User";
            // 
            // _txFoliage
            // 
            this->_txFoliage->BackColor = System::Drawing::Color::White;
            this->_txFoliage->ContextMenuStrip = this->_cms_AdjLive;
            this->helpProvider1->SetHelpKeyword(this->_txFoliage, L"f1");
            this->helpProvider1->SetHelpString(this->_txFoliage, L"Foliage Fuel ");
            this->_txFoliage->Location = System::Drawing::Point(638, 58);
            this->_txFoliage->Name = L"_txFoliage";
            this->_txFoliage->ReadOnly = true;
            this->helpProvider1->SetShowHelp(this->_txFoliage, true);
            this->_txFoliage->Size = System::Drawing::Size(50, 20);
            this->_txFoliage->TabIndex = 17;
            this->_txFoliage->Text = L"0";
            this->_txFoliage->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txFoliage, L"load of canopy foliage fuel in tons/acre");
            this->_txFoliage->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_txMoistDuff_KeyDown);
            // 
            // _txShrub
            // 
            this->_txShrub->BackColor = System::Drawing::Color::White;
            this->_txShrub->ContextMenuStrip = this->_cms_AdjLive;
            this->helpProvider1->SetHelpKeyword(this->_txShrub, L"f1");
            this->helpProvider1->SetHelpString(this->_txShrub, L"Shrub Fuel");
            this->_txShrub->Location = System::Drawing::Point(566, 58);
            this->_txShrub->Name = L"_txShrub";
            this->_txShrub->ReadOnly = true;
            this->helpProvider1->SetShowHelp(this->_txShrub, true);
            this->_txShrub->Size = System::Drawing::Size(50, 20);
            this->_txShrub->TabIndex = 16;
            this->_txShrub->Text = L"0";
            this->_txShrub->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txShrub, L"load of shrubs in tons/acre");
            this->_txShrub->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_txMoistDuff_KeyDown);
            // 
            // _txHerb
            // 
            this->_txHerb->BackColor = System::Drawing::Color::White;
            this->_txHerb->ContextMenuStrip = this->_cms_AdjLive;
            this->helpProvider1->SetHelpKeyword(this->_txHerb, L"f1");
            this->helpProvider1->SetHelpString(this->_txHerb, L"Herbaceous");
            this->_txHerb->Location = System::Drawing::Point(512, 58);
            this->_txHerb->Name = L"_txHerb";
            this->_txHerb->ReadOnly = true;
            this->helpProvider1->SetShowHelp(this->_txHerb, true);
            this->_txHerb->Size = System::Drawing::Size(50, 20);
            this->_txHerb->TabIndex = 15;
            this->_txHerb->Text = L"0";
            this->_txHerb->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txHerb, L"load of herbaceous fuel in tons/acre");
            this->_txHerb->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_txMoistDuff_KeyDown);
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->BackColor = System::Drawing::Color::Transparent;
            this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label1->Location = System::Drawing::Point(5, 15);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(77, 15);
            this->label1->TabIndex = 19;
            this->label1->Text = L"Cover Type";
            this->toolTip1->SetToolTip(this->label1, L"Select the cover type classification you want to use");
            // 
            // _lb1kHr
            // 
            this->_lb1kHr->AutoSize = true;
            this->_lb1kHr->BackColor = System::Drawing::Color::Transparent;
            this->_lb1kHr->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lb1kHr->Location = System::Drawing::Point(457, 40);
            this->_lb1kHr->Name = L"_lb1kHr";
            this->_lb1kHr->Size = System::Drawing::Size(23, 15);
            this->_lb1kHr->TabIndex = 14;
            this->_lb1kHr->Text = L"3+";
            this->toolTip1->SetToolTip(this->_lb1kHr, L"dead woody log fuel 3+ inch diameter in tons/acre\r\n1000 hour fuel");
            // 
            // _lb100Hr
            // 
            this->_lb100Hr->AutoSize = true;
            this->_lb100Hr->BackColor = System::Drawing::Color::Transparent;
            this->_lb100Hr->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lb100Hr->Location = System::Drawing::Point(396, 40);
            this->_lb100Hr->Name = L"_lb100Hr";
            this->_lb100Hr->Size = System::Drawing::Size(28, 15);
            this->_lb100Hr->TabIndex = 13;
            this->_lb100Hr->Text = L"1-3";
            this->toolTip1->SetToolTip(this->_lb100Hr, L"dead woody fuel 1-3 inch diameter  in tons/acre\r\n100 hour fuel");
            // 
            // _lb10Hr
            // 
            this->_lb10Hr->AutoSize = true;
            this->_lb10Hr->BackColor = System::Drawing::Color::Transparent;
            this->_lb10Hr->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lb10Hr->Location = System::Drawing::Point(341, 40);
            this->_lb10Hr->Name = L"_lb10Hr";
            this->_lb10Hr->Size = System::Drawing::Size(40, 15);
            this->_lb10Hr->TabIndex = 12;
            this->_lb10Hr->Text = L"1/4-1";
            this->toolTip1->SetToolTip(this->_lb10Hr, L"dead woody fuel 0.25-1.0 inch  diameter in tons/acre\r\n10 hour fuel ");
            // 
            // _tx1kHr
            // 
            this->_tx1kHr->BackColor = System::Drawing::Color::White;
            this->_tx1kHr->ContextMenuStrip = this->_cms_AdjDead;
            this->helpProvider1->SetHelpKeyword(this->_tx1kHr, L"f1");
            this->helpProvider1->SetHelpString(this->_tx1kHr, L"1000 Hour fuels, over 3 inches, tons per acre");
            this->_tx1kHr->Location = System::Drawing::Point(442, 58);
            this->_tx1kHr->Name = L"_tx1kHr";
            this->_tx1kHr->ReadOnly = true;
            this->helpProvider1->SetShowHelp(this->_tx1kHr, true);
            this->_tx1kHr->Size = System::Drawing::Size(50, 20);
            this->_tx1kHr->TabIndex = 12;
            this->_tx1kHr->Text = L"0";
            this->_tx1kHr->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_tx1kHr, L"dead woody log fuel 3+ inch diameter in tons/acre\r\n1000 hour fuel");
            this->_tx1kHr->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_txMoistDuff_KeyDown);
            // 
            // _tx100Hr
            // 
            this->_tx100Hr->BackColor = System::Drawing::Color::White;
            this->_tx100Hr->ContextMenuStrip = this->_cms_AdjDead;
            this->helpProvider1->SetHelpKeyword(this->_tx100Hr, L"f1");
            this->helpProvider1->SetHelpString(this->_tx100Hr, L"100 Hour Fuel, 1 -> 3 inches, tons per acre");
            this->_tx100Hr->Location = System::Drawing::Point(384, 58);
            this->_tx100Hr->Name = L"_tx100Hr";
            this->_tx100Hr->ReadOnly = true;
            this->helpProvider1->SetShowHelp(this->_tx100Hr, true);
            this->_tx100Hr->Size = System::Drawing::Size(50, 20);
            this->_tx100Hr->TabIndex = 11;
            this->_tx100Hr->Text = L"0";
            this->_tx100Hr->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_tx100Hr, L"dead woody fuel 1-3 inch diameter in  tons/acre\r\n100 hour fuel");
            this->_tx100Hr->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_txMoistDuff_KeyDown);
            // 
            // _tx10Hr
            // 
            this->_tx10Hr->BackColor = System::Drawing::Color::White;
            this->_tx10Hr->ContextMenuStrip = this->_cms_AdjDead;
            this->helpProvider1->SetHelpKeyword(this->_tx10Hr, L"f1");
            this->helpProvider1->SetHelpString(this->_tx10Hr, L"10 Hour fuel, 1/4 -> 1 inch, tons per acre");
            this->_tx10Hr->Location = System::Drawing::Point(332, 58);
            this->_tx10Hr->Name = L"_tx10Hr";
            this->_tx10Hr->ReadOnly = true;
            this->helpProvider1->SetShowHelp(this->_tx10Hr, true);
            this->_tx10Hr->Size = System::Drawing::Size(50, 20);
            this->_tx10Hr->TabIndex = 10;
            this->_tx10Hr->Text = L"0";
            this->_tx10Hr->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_tx10Hr, L"dead woody fuel 0.25-1.0 inch  diameter in tons/acre\r\n10 hour fuel");
            this->_tx10Hr->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_txMoistDuff_KeyDown);
            // 
            // _lb1Hr
            // 
            this->_lb1Hr->AutoSize = true;
            this->_lb1Hr->BackColor = System::Drawing::Color::Transparent;
            this->_lb1Hr->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lb1Hr->Location = System::Drawing::Point(289, 40);
            this->_lb1Hr->Name = L"_lb1Hr";
            this->_lb1Hr->Size = System::Drawing::Size(40, 15);
            this->_lb1Hr->TabIndex = 7;
            this->_lb1Hr->Text = L"0-1/4";
            this->toolTip1->SetToolTip(this->_lb1Hr, L"dead woody fuel 0-0.25 inch  diameter in tons/acre\r\n1 hour fuel \r\n");
            // 
            // _lbLitter
            // 
            this->_lbLitter->AutoSize = true;
            this->_lbLitter->BackColor = System::Drawing::Color::Transparent;
            this->_lbLitter->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lbLitter->Location = System::Drawing::Point(229, 40);
            this->_lbLitter->Name = L"_lbLitter";
            this->_lbLitter->Size = System::Drawing::Size(40, 15);
            this->_lbLitter->TabIndex = 6;
            this->_lbLitter->Text = L"Litter";
            this->toolTip1->SetToolTip(this->_lbLitter, L"Litter load in tons/acre");
            this->_lbLitter->Click += gcnew System::EventHandler(this, &Wnd_Mai::_lbLitter_Click);
            // 
            // shapeContainer1
            // 
            this->shapeContainer1->Location = System::Drawing::Point(3, 3);
            this->shapeContainer1->Margin = System::Windows::Forms::Padding(0);
            this->shapeContainer1->Name = L"shapeContainer1";
            this->shapeContainer1->Shapes->AddRange(gcnew cli::array< Microsoft::VisualBasic::PowerPacks::Shape^  >(5) {this->lineShape3, 
                this->rectangleShape4, this->rectangleShape3, this->rectangleShape2, this->rectangleShape1});
            this->shapeContainer1->Size = System::Drawing::Size(910, 263);
            this->shapeContainer1->TabIndex = 72;
            this->shapeContainer1->TabStop = false;
            // 
            // lineShape3
            // 
            this->lineShape3->BorderColor = System::Drawing::SystemColors::Window;
            this->lineShape3->BorderWidth = 5;
            this->lineShape3->Name = L"lineShape3";
            this->lineShape3->SelectionColor = System::Drawing::SystemColors::Control;
            this->lineShape3->X1 = 436;
            this->lineShape3->X2 = 492;
            this->lineShape3->Y1 = 80;
            this->lineShape3->Y2 = 80;
            // 
            // rectangleShape4
            // 
            this->rectangleShape4->BackColor = System::Drawing::Color::Transparent;
            this->rectangleShape4->BackStyle = Microsoft::VisualBasic::PowerPacks::BackStyle::Opaque;
            this->rectangleShape4->BorderColor = System::Drawing::Color::DarkGray;
            this->rectangleShape4->BorderWidth = 2;
            this->rectangleShape4->Location = System::Drawing::Point(626, 49);
            this->rectangleShape4->Name = L"rectangleShape4";
            this->rectangleShape4->Size = System::Drawing::Size(122, 70);
            // 
            // rectangleShape3
            // 
            this->rectangleShape3->BackColor = System::Drawing::Color::Transparent;
            this->rectangleShape3->BackStyle = Microsoft::VisualBasic::PowerPacks::BackStyle::Opaque;
            this->rectangleShape3->BorderColor = System::Drawing::Color::DarkGray;
            this->rectangleShape3->BorderWidth = 2;
            this->rectangleShape3->Location = System::Drawing::Point(160, 48);
            this->rectangleShape3->Name = L"rectangleShape3";
            this->rectangleShape3->Size = System::Drawing::Size(56, 73);
            // 
            // rectangleShape2
            // 
            this->rectangleShape2->BackColor = System::Drawing::Color::Transparent;
            this->rectangleShape2->BackStyle = Microsoft::VisualBasic::PowerPacks::BackStyle::Opaque;
            this->rectangleShape2->BorderColor = System::Drawing::Color::DarkGray;
            this->rectangleShape2->BorderWidth = 2;
            this->rectangleShape2->Location = System::Drawing::Point(436, 45);
            this->rectangleShape2->Name = L"rectangleShape2";
            this->rectangleShape2->Size = System::Drawing::Size(57, 36);
            // 
            // rectangleShape1
            // 
            this->rectangleShape1->BackColor = System::Drawing::Color::Transparent;
            this->rectangleShape1->BackStyle = Microsoft::VisualBasic::PowerPacks::BackStyle::Opaque;
            this->rectangleShape1->BorderColor = System::Drawing::Color::DarkGray;
            this->rectangleShape1->BorderWidth = 2;
            this->rectangleShape1->Location = System::Drawing::Point(396, 81);
            this->rectangleShape1->Name = L"rectangleShape1";
            this->rectangleShape1->Size = System::Drawing::Size(143, 40);
            // 
            // _tcpMortality
            // 
            this->_tcpMortality->BackColor = System::Drawing::Color::Transparent;
            this->_tcpMortality->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
            this->_tcpMortality->Controls->Add(this->_btProb);
            this->_tcpMortality->Controls->Add(this->_gbSC3);
            this->_tcpMortality->Controls->Add(this->_gbSC2);
            this->_tcpMortality->Controls->Add(this->_gbSC1);
            this->_tcpMortality->Controls->Add(this->_cb_SalRep);
            this->_tcpMortality->Controls->Add(this->_gbFlaSco);
            this->_tcpMortality->Controls->Add(this->_gbFFI);
            this->_tcpMortality->Controls->Add(this->_chLowFireSev);
            this->_tcpMortality->Controls->Add(this->_btSelect);
            this->_tcpMortality->Controls->Add(this->_dgMort);
            this->_tcpMortality->Controls->Add(this->_cbSpecies);
            this->_tcpMortality->Controls->Add(this->shapeContainer3);
            this->_tcpMortality->Location = System::Drawing::Point(4, 22);
            this->_tcpMortality->Name = L"_tcpMortality";
            this->_tcpMortality->Padding = System::Windows::Forms::Padding(3);
            this->_tcpMortality->Size = System::Drawing::Size(916, 269);
            this->_tcpMortality->TabIndex = 1;
            this->_tcpMortality->Text = L"Mortality";
            this->toolTip1->SetToolTip(this->_tcpMortality, L"Check this tab to estimate Tree Mortality");
            this->_tcpMortality->ToolTipText = L"Check this tab to estimate Tree Mortality";
            this->_tcpMortality->UseVisualStyleBackColor = true;
            this->_tcpMortality->Enter += gcnew System::EventHandler(this, &Wnd_Mai::_tcpMortality_Enter);
            // 
            // _btProb
            // 
            this->_btProb->Location = System::Drawing::Point(360, 90);
            this->_btProb->Name = L"_btProb";
            this->_btProb->Size = System::Drawing::Size(52, 36);
            this->_btProb->TabIndex = 101;
            this->_btProb->Text = L"Mort Prob";
            this->toolTip1->SetToolTip(this->_btProb, L"Calculate Probability of Mortalit\r\nand display in the P(m) column\r\nof the Tree Gr" 
                L"id");
            this->_btProb->UseVisualStyleBackColor = true;
            this->_btProb->Click += gcnew System::EventHandler(this, &Wnd_Mai::_btProb_Click);
            // 
            // _gbSC3
            // 
            this->_gbSC3->Controls->Add(this->label37);
            this->_gbSC3->Controls->Add(this->label38);
            this->_gbSC3->Controls->Add(this->label39);
            this->_gbSC3->Controls->Add(this->_txSC3C);
            this->_gbSC3->Controls->Add(this->_txSC3B);
            this->_gbSC3->Controls->Add(this->_txSC3A);
            this->_gbSC3->Location = System::Drawing::Point(751, 205);
            this->_gbSC3->Name = L"_gbSC3";
            this->_gbSC3->Size = System::Drawing::Size(139, 55);
            this->_gbSC3->TabIndex = 100;
            this->_gbSC3->TabStop = false;
            // 
            // label37
            // 
            this->label37->AutoSize = true;
            this->label37->Location = System::Drawing::Point(39, 30);
            this->label37->Name = L"label37";
            this->label37->Size = System::Drawing::Size(13, 13);
            this->label37->TabIndex = 97;
            this->label37->Text = L"<";
            // 
            // label38
            // 
            this->label38->AutoSize = true;
            this->label38->Location = System::Drawing::Point(101, 10);
            this->label38->Name = L"label38";
            this->label38->Size = System::Drawing::Size(29, 13);
            this->label38->TabIndex = 96;
            this->label38->Text = L"Prob";
            // 
            // label39
            // 
            this->label39->AutoSize = true;
            this->label39->Location = System::Drawing::Point(12, 10);
            this->label39->Name = L"label39";
            this->label39->Size = System::Drawing::Size(64, 13);
            this->label39->TabIndex = 95;
            this->label39->Text = L"Size Class 3";
            // 
            // _txSC3C
            // 
            this->_txSC3C->Location = System::Drawing::Point(101, 27);
            this->_txSC3C->Name = L"_txSC3C";
            this->_txSC3C->Size = System::Drawing::Size(30, 20);
            this->_txSC3C->TabIndex = 94;
            this->_txSC3C->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txSC3B
            // 
            this->_txSC3B->Location = System::Drawing::Point(56, 27);
            this->_txSC3B->Name = L"_txSC3B";
            this->_txSC3B->Size = System::Drawing::Size(30, 20);
            this->_txSC3B->TabIndex = 93;
            this->_txSC3B->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txSC3A
            // 
            this->_txSC3A->Location = System::Drawing::Point(4, 27);
            this->_txSC3A->Name = L"_txSC3A";
            this->_txSC3A->Size = System::Drawing::Size(30, 20);
            this->_txSC3A->TabIndex = 92;
            this->_txSC3A->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _gbSC2
            // 
            this->_gbSC2->Controls->Add(this->label34);
            this->_gbSC2->Controls->Add(this->label35);
            this->_gbSC2->Controls->Add(this->label36);
            this->_gbSC2->Controls->Add(this->_txSC2C);
            this->_gbSC2->Controls->Add(this->_txSC2B);
            this->_gbSC2->Controls->Add(this->_txSC2A);
            this->_gbSC2->Location = System::Drawing::Point(600, 205);
            this->_gbSC2->Name = L"_gbSC2";
            this->_gbSC2->Size = System::Drawing::Size(139, 55);
            this->_gbSC2->TabIndex = 99;
            this->_gbSC2->TabStop = false;
            // 
            // label34
            // 
            this->label34->AutoSize = true;
            this->label34->Location = System::Drawing::Point(39, 30);
            this->label34->Name = L"label34";
            this->label34->Size = System::Drawing::Size(13, 13);
            this->label34->TabIndex = 97;
            this->label34->Text = L"<";
            // 
            // label35
            // 
            this->label35->AutoSize = true;
            this->label35->Location = System::Drawing::Point(101, 10);
            this->label35->Name = L"label35";
            this->label35->Size = System::Drawing::Size(29, 13);
            this->label35->TabIndex = 96;
            this->label35->Text = L"Prob";
            // 
            // label36
            // 
            this->label36->AutoSize = true;
            this->label36->Location = System::Drawing::Point(12, 10);
            this->label36->Name = L"label36";
            this->label36->Size = System::Drawing::Size(67, 13);
            this->label36->TabIndex = 95;
            this->label36->Text = L"Size Class 2 ";
            // 
            // _txSC2C
            // 
            this->_txSC2C->Location = System::Drawing::Point(101, 27);
            this->_txSC2C->Name = L"_txSC2C";
            this->_txSC2C->Size = System::Drawing::Size(30, 20);
            this->_txSC2C->TabIndex = 94;
            this->_txSC2C->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txSC2B
            // 
            this->_txSC2B->Location = System::Drawing::Point(56, 27);
            this->_txSC2B->Name = L"_txSC2B";
            this->_txSC2B->Size = System::Drawing::Size(30, 20);
            this->_txSC2B->TabIndex = 93;
            this->_txSC2B->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txSC2A
            // 
            this->_txSC2A->Location = System::Drawing::Point(4, 27);
            this->_txSC2A->Name = L"_txSC2A";
            this->_txSC2A->Size = System::Drawing::Size(30, 20);
            this->_txSC2A->TabIndex = 92;
            this->_txSC2A->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _gbSC1
            // 
            this->_gbSC1->Controls->Add(this->label33);
            this->_gbSC1->Controls->Add(this->label32);
            this->_gbSC1->Controls->Add(this->label31);
            this->_gbSC1->Controls->Add(this->_txSC1C);
            this->_gbSC1->Controls->Add(this->_txSC1B);
            this->_gbSC1->Controls->Add(this->_txSC1A);
            this->_gbSC1->Location = System::Drawing::Point(450, 205);
            this->_gbSC1->Name = L"_gbSC1";
            this->_gbSC1->Size = System::Drawing::Size(139, 55);
            this->_gbSC1->TabIndex = 98;
            this->_gbSC1->TabStop = false;
            // 
            // label33
            // 
            this->label33->AutoSize = true;
            this->label33->Location = System::Drawing::Point(39, 30);
            this->label33->Name = L"label33";
            this->label33->Size = System::Drawing::Size(13, 13);
            this->label33->TabIndex = 97;
            this->label33->Text = L"<";
            // 
            // label32
            // 
            this->label32->AutoSize = true;
            this->label32->Location = System::Drawing::Point(101, 10);
            this->label32->Name = L"label32";
            this->label32->Size = System::Drawing::Size(29, 13);
            this->label32->TabIndex = 96;
            this->label32->Text = L"Prob";
            // 
            // label31
            // 
            this->label31->AutoSize = true;
            this->label31->Location = System::Drawing::Point(12, 10);
            this->label31->Name = L"label31";
            this->label31->Size = System::Drawing::Size(67, 13);
            this->label31->TabIndex = 95;
            this->label31->Text = L"Size Class 1 ";
            // 
            // _txSC1C
            // 
            this->_txSC1C->Location = System::Drawing::Point(101, 27);
            this->_txSC1C->Name = L"_txSC1C";
            this->_txSC1C->Size = System::Drawing::Size(30, 20);
            this->_txSC1C->TabIndex = 94;
            this->_txSC1C->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txSC1B
            // 
            this->_txSC1B->Location = System::Drawing::Point(56, 27);
            this->_txSC1B->Name = L"_txSC1B";
            this->_txSC1B->Size = System::Drawing::Size(30, 20);
            this->_txSC1B->TabIndex = 93;
            this->_txSC1B->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txSC1A
            // 
            this->_txSC1A->Location = System::Drawing::Point(4, 27);
            this->_txSC1A->Name = L"_txSC1A";
            this->_txSC1A->Size = System::Drawing::Size(30, 20);
            this->_txSC1A->TabIndex = 92;
            this->_txSC1A->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _cb_SalRep
            // 
            this->_cb_SalRep->AutoSize = true;
            this->_cb_SalRep->Location = System::Drawing::Point(148, 96);
            this->_cb_SalRep->Name = L"_cb_SalRep";
            this->_cb_SalRep->Size = System::Drawing::Size(103, 17);
            this->_cb_SalRep->TabIndex = 85;
            this->_cb_SalRep->Text = L"Salvage Report ";
            this->toolTip1->SetToolTip(this->_cb_SalRep, L"Check this box then click the \r\nMortality button below to \r\ncreate a Salvage Repo" 
                L"rt ");
            this->_cb_SalRep->UseVisualStyleBackColor = true;
            this->_cb_SalRep->CheckedChanged += gcnew System::EventHandler(this, &Wnd_Mai::_cb_SalRep_CheckedChanged);
            // 
            // _gbFlaSco
            // 
            this->_gbFlaSco->Controls->Add(this->_txFlaSco);
            this->_gbFlaSco->Controls->Add(this->_rbFlameLen);
            this->_gbFlaSco->Controls->Add(this->_rbScorchHeight);
            this->_gbFlaSco->Location = System::Drawing::Point(10, 37);
            this->_gbFlaSco->Name = L"_gbFlaSco";
            this->_gbFlaSco->Size = System::Drawing::Size(112, 80);
            this->_gbFlaSco->TabIndex = 90;
            this->_gbFlaSco->TabStop = false;
            this->_gbFlaSco->Enter += gcnew System::EventHandler(this, &Wnd_Mai::groupBox5_Enter);
            // 
            // _txFlaSco
            // 
            this->_txFlaSco->Location = System::Drawing::Point(27, 52);
            this->_txFlaSco->Name = L"_txFlaSco";
            this->_txFlaSco->Size = System::Drawing::Size(50, 20);
            this->_txFlaSco->TabIndex = 6;
            this->_txFlaSco->TabStop = false;
            this->_txFlaSco->Text = L"4.0";
            this->_txFlaSco->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txFlaSco, L"this gets filled in at run time");
            // 
            // _rbFlameLen
            // 
            this->_rbFlameLen->AutoSize = true;
            this->_rbFlameLen->Checked = true;
            this->helpProvider1->SetHelpString(this->_rbFlameLen, L"FlameLengthHelp");
            this->_rbFlameLen->Location = System::Drawing::Point(9, 31);
            this->_rbFlameLen->Name = L"_rbFlameLen";
            this->helpProvider1->SetShowHelp(this->_rbFlameLen, true);
            this->_rbFlameLen->Size = System::Drawing::Size(89, 17);
            this->_rbFlameLen->TabIndex = 7;
            this->_rbFlameLen->TabStop = true;
            this->_rbFlameLen->Text = L"Flame Length";
            this->toolTip1->SetToolTip(this->_rbFlameLen, L"Check button to enter flame length in feet");
            this->_rbFlameLen->UseVisualStyleBackColor = true;
            // 
            // _rbScorchHeight
            // 
            this->_rbScorchHeight->AutoSize = true;
            this->_rbScorchHeight->Location = System::Drawing::Point(9, 12);
            this->_rbScorchHeight->Name = L"_rbScorchHeight";
            this->_rbScorchHeight->Size = System::Drawing::Size(93, 17);
            this->_rbScorchHeight->TabIndex = 8;
            this->_rbScorchHeight->Text = L"Scorch Height";
            this->toolTip1->SetToolTip(this->_rbScorchHeight, L"Check button to use scorch height in feet");
            this->_rbScorchHeight->UseVisualStyleBackColor = true;
            // 
            // _gbFFI
            // 
            this->_gbFFI->Controls->Add(this->_bt_FFIErr);
            this->_gbFFI->Controls->Add(this->_btFFI_FileClose);
            this->_gbFFI->Controls->Add(this->label25);
            this->_gbFFI->Controls->Add(this->_txFFIErr);
            this->_gbFFI->Controls->Add(this->_cbFFI_LoadAll);
            this->_gbFFI->Controls->Add(this->_btTest);
            this->_gbFFI->Controls->Add(this->_txFFI_File);
            this->_gbFFI->Controls->Add(this->_lbFFIPlot);
            this->_gbFFI->Controls->Add(this->_cbFFI_Plots);
            this->_gbFFI->Location = System::Drawing::Point(9, 145);
            this->_gbFFI->Name = L"_gbFFI";
            this->_gbFFI->Size = System::Drawing::Size(415, 105);
            this->_gbFFI->TabIndex = 82;
            this->_gbFFI->TabStop = false;
            this->toolTip1->SetToolTip(this->_gbFFI, L"By clicking the FFI button you can load tree data directly\r\ninto the tree grid. P" 
                L"lease see the full Help for more details.\r\n\r\nThis option is not available for do" 
                L"ing Post Fire Injury.\r\n");
            // 
            // _bt_FFIErr
            // 
            this->_bt_FFIErr->Location = System::Drawing::Point(329, 76);
            this->_bt_FFIErr->Name = L"_bt_FFIErr";
            this->_bt_FFIErr->Size = System::Drawing::Size(60, 20);
            this->_bt_FFIErr->TabIndex = 85;
            this->_bt_FFIErr->Text = L"Error File";
            this->_bt_FFIErr->UseVisualStyleBackColor = true;
            this->_bt_FFIErr->Click += gcnew System::EventHandler(this, &Wnd_Mai::_bt_FFIErr_Click);
            // 
            // _btFFI_FileClose
            // 
            this->_btFFI_FileClose->Location = System::Drawing::Point(7, 74);
            this->_btFFI_FileClose->Name = L"_btFFI_FileClose";
            this->_btFFI_FileClose->Size = System::Drawing::Size(53, 24);
            this->_btFFI_FileClose->TabIndex = 84;
            this->_btFFI_FileClose->Text = L" Close";
            this->toolTip1->SetToolTip(this->_btFFI_FileClose, L"Clear the tree grid and \r\nreturn to regular mortaliy\r\nsimulation. ");
            this->_btFFI_FileClose->UseVisualStyleBackColor = true;
            this->_btFFI_FileClose->Click += gcnew System::EventHandler(this, &Wnd_Mai::_btFFI_FileClose_Click);
            // 
            // label25
            // 
            this->label25->AutoSize = true;
            this->label25->Location = System::Drawing::Point(224, 79);
            this->label25->Name = L"label25";
            this->label25->Size = System::Drawing::Size(34, 13);
            this->label25->TabIndex = 84;
            this->label25->Text = L"Errors";
            this->toolTip1->SetToolTip(this->label25, L"Number of tree records found\r\nto be in error on the selected\r\nplot.");
            // 
            // _txFFIErr
            // 
            this->_txFFIErr->Location = System::Drawing::Point(264, 76);
            this->_txFFIErr->Name = L"_txFFIErr";
            this->_txFFIErr->ReadOnly = true;
            this->_txFFIErr->Size = System::Drawing::Size(49, 20);
            this->_txFFIErr->TabIndex = 83;
            this->_txFFIErr->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->toolTip1->SetToolTip(this->_txFFIErr, L"Number of tree records\r\nfound to be in error\r\non the selected plot.");
            // 
            // _cbFFI_LoadAll
            // 
            this->_cbFFI_LoadAll->AutoSize = true;
            this->_cbFFI_LoadAll->Location = System::Drawing::Point(88, 79);
            this->_cbFFI_LoadAll->Name = L"_cbFFI_LoadAll";
            this->_cbFFI_LoadAll->Size = System::Drawing::Size(64, 17);
            this->_cbFFI_LoadAll->TabIndex = 82;
            this->_cbFFI_LoadAll->Text = L"Load All";
            this->toolTip1->SetToolTip(this->_cbFFI_LoadAll, L"--> Check this box to display all tree records\r\n including those found to be in e" 
                L"rror.\r\n--> Unchecked box will only display valid\r\ntree records.  ");
            this->_cbFFI_LoadAll->UseVisualStyleBackColor = true;
            this->_cbFFI_LoadAll->Visible = false;
            this->_cbFFI_LoadAll->CheckedChanged += gcnew System::EventHandler(this, &Wnd_Mai::_cbFFI_Plots_SelectedIndexChanged);
            // 
            // _btTest
            // 
            this->_btTest->Location = System::Drawing::Point(6, 19);
            this->_btTest->Name = L"_btTest";
            this->_btTest->Size = System::Drawing::Size(54, 23);
            this->_btTest->TabIndex = 0;
            this->_btTest->TabStop = false;
            this->_btTest->Text = L"FFI File ";
            this->toolTip1->SetToolTip(this->_btTest, L"Find the FFI database file containing your tree list data ");
            this->_btTest->UseVisualStyleBackColor = true;
            this->_btTest->Click += gcnew System::EventHandler(this, &Wnd_Mai::_btTest_Click);
            // 
            // _txFFI_File
            // 
            this->_txFFI_File->Location = System::Drawing::Point(64, 21);
            this->_txFFI_File->Name = L"_txFFI_File";
            this->_txFFI_File->ReadOnly = true;
            this->_txFFI_File->Size = System::Drawing::Size(346, 20);
            this->_txFFI_File->TabIndex = 80;
            this->_txFFI_File->TabStop = false;
            this->toolTip1->SetToolTip(this->_txFFI_File, L"Selected FFI File \r\n\r\nThis textbox will get filled in once\r\nyou open a file. ");
            // 
            // _lbFFIPlot
            // 
            this->_lbFFIPlot->AutoSize = true;
            this->_lbFFIPlot->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_lbFFIPlot->Location = System::Drawing::Point(27, 51);
            this->_lbFFIPlot->Name = L"_lbFFIPlot";
            this->_lbFFIPlot->Size = System::Drawing::Size(32, 15);
            this->_lbFFIPlot->TabIndex = 81;
            this->_lbFFIPlot->Text = L"Plot";
            this->toolTip1->SetToolTip(this->_lbFFIPlot, L"Select the plot for which you want to compute tree mortality ");
            // 
            // _cbFFI_Plots
            // 
            this->_cbFFI_Plots->FormattingEnabled = true;
            this->_cbFFI_Plots->Location = System::Drawing::Point(64, 49);
            this->_cbFFI_Plots->Name = L"_cbFFI_Plots";
            this->_cbFFI_Plots->Size = System::Drawing::Size(346, 21);
            this->_cbFFI_Plots->TabIndex = 79;
            this->_cbFFI_Plots->TabStop = false;
            this->toolTip1->SetToolTip(this->_cbFFI_Plots, L"Select the plot for which you want to compute tree mortality ");
            this->_cbFFI_Plots->SelectedIndexChanged += gcnew System::EventHandler(this, &Wnd_Mai::_cbFFI_Plots_SelectedIndexChanged);
            // 
            // _chLowFireSev
            // 
            this->_chLowFireSev->AutoSize = true;
            this->_chLowFireSev->Location = System::Drawing::Point(148, 69);
            this->_chLowFireSev->Name = L"_chLowFireSev";
            this->_chLowFireSev->Size = System::Drawing::Size(167, 17);
            this->_chLowFireSev->TabIndex = 10;
            this->_chLowFireSev->TabStop = false;
            this->_chLowFireSev->Text = L"Low Fire Severity (aspen only)";
            this->toolTip1->SetToolTip(this->_chLowFireSev, L"Select the low fire severity option");
            this->_chLowFireSev->UseVisualStyleBackColor = true;
            this->_chLowFireSev->CheckedChanged += gcnew System::EventHandler(this, &Wnd_Mai::_chLowFireSev_CheckedChanged);
            // 
            // _btSelect
            // 
            this->_btSelect->Location = System::Drawing::Point(358, 42);
            this->_btSelect->Name = L"_btSelect";
            this->_btSelect->Size = System::Drawing::Size(66, 23);
            this->_btSelect->TabIndex = 5;
            this->_btSelect->TabStop = false;
            this->_btSelect->Text = L"----------->>";
            this->toolTip1->SetToolTip(this->_btSelect, L"Click this button to move a copy of the species\r\ninto the tree grid.");
            this->_btSelect->UseVisualStyleBackColor = true;
            this->_btSelect->Click += gcnew System::EventHandler(this, &Wnd_Mai::_btSelect_Click);
            // 
            // _dgMort
            // 
            this->_dgMort->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
                | System::Windows::Forms::AnchorStyles::Left) 
                | System::Windows::Forms::AnchorStyles::Right));
            dataGridViewCellStyle23->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
            dataGridViewCellStyle23->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle23->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, 
                System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
            dataGridViewCellStyle23->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle23->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle23->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle23->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->_dgMort->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle23;
            this->_dgMort->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->_dgMort->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(12) {this->_dgcSpecies, 
                this->_cbEquTyp, this->_dgcDensity, this->_dgcDBH, this->_dgcHeight, this->_dgcCrownRatio, this->_dgcCrnDam, this->_dgcCKR, this->_dgcBeetle, 
                this->_dgcBole, this->_dgcProb, this->_dgcGraph});
            dataGridViewCellStyle32->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
            dataGridViewCellStyle32->BackColor = System::Drawing::SystemColors::Window;
            dataGridViewCellStyle32->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, 
                System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
            dataGridViewCellStyle32->ForeColor = System::Drawing::SystemColors::ControlText;
            dataGridViewCellStyle32->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle32->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle32->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
            this->_dgMort->DefaultCellStyle = dataGridViewCellStyle32;
            this->_dgMort->Location = System::Drawing::Point(428, 6);
            this->_dgMort->Name = L"_dgMort";
            dataGridViewCellStyle33->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
            dataGridViewCellStyle33->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle33->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, 
                System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
            dataGridViewCellStyle33->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle33->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle33->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle33->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->_dgMort->RowHeadersDefaultCellStyle = dataGridViewCellStyle33;
            this->_dgMort->RowHeadersWidth = 20;
            this->_dgMort->RowTemplate->Height = 18;
            this->_dgMort->Size = System::Drawing::Size(485, 195);
            this->_dgMort->TabIndex = 4;
            this->_dgMort->CellValueChanged += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &Wnd_Mai::_dgMort_CellValueChanged);
            this->_dgMort->CellLeave += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &Wnd_Mai::_dgMort_CellLeave);
            this->_dgMort->CellStateChanged += gcnew System::Windows::Forms::DataGridViewCellStateChangedEventHandler(this, &Wnd_Mai::_dgMort_CellStateChanged);
            this->_dgMort->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Wnd_Mai::_dgMort_MouseUp);
            this->_dgMort->CellEndEdit += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &Wnd_Mai::_dgMort_CellEndEdit);
            this->_dgMort->CellClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &Wnd_Mai::_dgMort_CellClick);
            this->_dgMort->EditingControlShowing += gcnew System::Windows::Forms::DataGridViewEditingControlShowingEventHandler(this, &Wnd_Mai::_dgMort_EditingControlShowing);
            this->_dgMort->CurrentCellDirtyStateChanged += gcnew System::EventHandler(this, &Wnd_Mai::_dgMort_CurrentCellDirtyStateChanged);
            this->_dgMort->SelectionChanged += gcnew System::EventHandler(this, &Wnd_Mai::_dgMort_SelectionChanged);
            this->_dgMort->ColumnAdded += gcnew System::Windows::Forms::DataGridViewColumnEventHandler(this, &Wnd_Mai::_dgMort_ColumnAdded);
            this->_dgMort->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &Wnd_Mai::_dgMort_CellContentClick);
            // 
            // _dgcSpecies
            // 
            this->_dgcSpecies->HeaderText = L"Species";
            this->_dgcSpecies->Name = L"_dgcSpecies";
            this->_dgcSpecies->ToolTipText = L"Select a species from the list";
            this->_dgcSpecies->Width = 56;
            // 
            // _cbEquTyp
            // 
            this->_cbEquTyp->DisplayStyle = System::Windows::Forms::DataGridViewComboBoxDisplayStyle::ComboBox;
            this->_cbEquTyp->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->_cbEquTyp->HeaderText = L"Eq Type";
            this->_cbEquTyp->Name = L"_cbEquTyp";
            this->_cbEquTyp->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Automatic;
            this->_cbEquTyp->ToolTipText = L"Equation type(s) available for selected species ";
            this->_cbEquTyp->Width = 74;
            // 
            // _dgcDensity
            // 
            dataGridViewCellStyle24->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle24->Format = L"N1";
            dataGridViewCellStyle24->NullValue = nullptr;
            this->_dgcDensity->DefaultCellStyle = dataGridViewCellStyle24;
            this->_dgcDensity->HeaderText = L"Den";
            this->_dgcDensity->Name = L"_dgcDensity";
            this->_dgcDensity->ToolTipText = L"Tree density (per acre)";
            this->_dgcDensity->Width = 30;
            // 
            // _dgcDBH
            // 
            dataGridViewCellStyle25->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            this->_dgcDBH->DefaultCellStyle = dataGridViewCellStyle25;
            this->_dgcDBH->HeaderText = L"DBH";
            this->_dgcDBH->Name = L"_dgcDBH";
            this->_dgcDBH->ToolTipText = L" Diameter Breast Height (in) ";
            this->_dgcDBH->Width = 31;
            // 
            // _dgcHeight
            // 
            dataGridViewCellStyle26->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            this->_dgcHeight->DefaultCellStyle = dataGridViewCellStyle26;
            this->_dgcHeight->HeaderText = L"Hgt";
            this->_dgcHeight->Name = L"_dgcHeight";
            this->_dgcHeight->ToolTipText = L"Tree height (ft)";
            this->_dgcHeight->Width = 36;
            // 
            // _dgcCrownRatio
            // 
            dataGridViewCellStyle27->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            this->_dgcCrownRatio->DefaultCellStyle = dataGridViewCellStyle27;
            this->_dgcCrownRatio->HeaderText = L"Crn Rat";
            this->_dgcCrownRatio->Name = L"_dgcCrownRatio";
            this->_dgcCrownRatio->ToolTipText = L"Live crown ratio (1-10) ";
            this->_dgcCrownRatio->Width = 30;
            // 
            // _dgcCrnDam
            // 
            dataGridViewCellStyle28->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            this->_dgcCrnDam->DefaultCellStyle = dataGridViewCellStyle28;
            this->_dgcCrnDam->HeaderText = L"Crn Dam%";
            this->_dgcCrnDam->Name = L"_dgcCrnDam";
            this->_dgcCrnDam->ToolTipText = L"Crown Damage Percent - ( 0 -> 100) ";
            this->_dgcCrnDam->Width = 38;
            // 
            // _dgcCKR
            // 
            dataGridViewCellStyle29->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            this->_dgcCKR->DefaultCellStyle = dataGridViewCellStyle29;
            this->_dgcCKR->HeaderText = L"CKR";
            this->_dgcCKR->Name = L"_dgcCKR";
            this->_dgcCKR->ToolTipText = L"Cambium kill rating (0-4)";
            this->_dgcCKR->Width = 30;
            // 
            // _dgcBeetle
            // 
            dataGridViewCellStyle30->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            this->_dgcBeetle->DefaultCellStyle = dataGridViewCellStyle30;
            this->_dgcBeetle->HeaderText = L"Btle Dam";
            this->_dgcBeetle->Name = L"_dgcBeetle";
            this->_dgcBeetle->ToolTipText = L"Beetle Damage";
            this->_dgcBeetle->Width = 30;
            // 
            // _dgcBole
            // 
            dataGridViewCellStyle31->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            this->_dgcBole->DefaultCellStyle = dataGridViewCellStyle31;
            this->_dgcBole->HeaderText = L"Bole Char";
            this->_dgcBole->Name = L"_dgcBole";
            this->_dgcBole->ToolTipText = L"Bole char height  (ft)";
            this->_dgcBole->Width = 30;
            // 
            // _dgcProb
            // 
            this->_dgcProb->HeaderText = L"Mort Prob";
            this->_dgcProb->Name = L"_dgcProb";
            this->_dgcProb->ToolTipText = L"Probability of Mortality";
            this->_dgcProb->Width = 30;
            // 
            // _dgcGraph
            // 
            this->_dgcGraph->HeaderText = L"Graph";
            this->_dgcGraph->Name = L"_dgcGraph";
            this->_dgcGraph->Resizable = System::Windows::Forms::DataGridViewTriState::True;
            this->_dgcGraph->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Automatic;
            this->_dgcGraph->ToolTipText = L"Click this box if you want to include this tree list item on the graph ";
            this->_dgcGraph->Width = 37;
            // 
            // _cbSpecies
            // 
            this->_cbSpecies->DropDownHeight = 200;
            this->_cbSpecies->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->_cbSpecies->DropDownWidth = 550;
            this->_cbSpecies->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_cbSpecies->FormattingEnabled = true;
            this->_cbSpecies->IntegralHeight = false;
            this->_cbSpecies->Location = System::Drawing::Point(8, 10);
            this->_cbSpecies->Name = L"_cbSpecies";
            this->_cbSpecies->Size = System::Drawing::Size(416, 22);
            this->_cbSpecies->TabIndex = 1;
            this->_cbSpecies->TabStop = false;
            this->toolTip1->SetToolTip(this->_cbSpecies, L"Select the tree species from this list \r\n\r\nNote:   Only the species in the the re" 
                L"gion \r\nyou have selected are shown.");
            // 
            // shapeContainer3
            // 
            this->shapeContainer3->Location = System::Drawing::Point(3, 3);
            this->shapeContainer3->Margin = System::Windows::Forms::Padding(0);
            this->shapeContainer3->Name = L"shapeContainer3";
            this->shapeContainer3->Size = System::Drawing::Size(906, 259);
            this->shapeContainer3->TabIndex = 91;
            this->shapeContainer3->TabStop = false;
            // 
            // lineShape2
            // 
            this->lineShape2->BorderColor = System::Drawing::SystemColors::Window;
            this->lineShape2->Name = L"lineShape2";
            this->lineShape2->Visible = false;
            this->lineShape2->X1 = 847;
            this->lineShape2->X2 = 851;
            this->lineShape2->Y1 = 557;
            this->lineShape2->Y2 = 593;
            // 
            // lineShape1
            // 
            this->lineShape1->Name = L"lineShape1";
            this->lineShape1->Visible = false;
            this->lineShape1->X1 = 869;
            this->lineShape1->X2 = 873;
            this->lineShape1->Y1 = 544;
            this->lineShape1->Y2 = 550;
            // 
            // _btSoil_Frm
            // 
            this->_btSoil_Frm->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
            this->_btSoil_Frm->Location = System::Drawing::Point(847, 508);
            this->_btSoil_Frm->Name = L"_btSoil_Frm";
            this->_btSoil_Frm->Size = System::Drawing::Size(60, 20);
            this->_btSoil_Frm->TabIndex = 67;
            this->_btSoil_Frm->TabStop = false;
            this->_btSoil_Frm->Text = L"Campbell";
            this->toolTip1->SetToolTip(this->_btSoil_Frm, L"Generate a graphic report on soil heating ");
            this->_btSoil_Frm->UseVisualStyleBackColor = true;
            this->_btSoil_Frm->Click += gcnew System::EventHandler(this, &Wnd_Mai::_btSoil_Frm_Click);
            // 
            // _btEmis_Frm
            // 
            this->_btEmis_Frm->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
            this->_btEmis_Frm->Location = System::Drawing::Point(841, 441);
            this->_btEmis_Frm->Name = L"_btEmis_Frm";
            this->_btEmis_Frm->Size = System::Drawing::Size(76, 36);
            this->_btEmis_Frm->TabIndex = 63;
            this->_btEmis_Frm->TabStop = false;
            this->_btEmis_Frm->Text = L"Smoke Emissions";
            this->toolTip1->SetToolTip(this->_btEmis_Frm, L"Generate a graphic report on smoke emissions ");
            this->_btEmis_Frm->UseVisualStyleBackColor = true;
            this->_btEmis_Frm->Click += gcnew System::EventHandler(this, &Wnd_Mai::_btEmis_Frm_Click);
            // 
            // _btFuel_Frm
            // 
            this->_btFuel_Frm->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
            this->_btFuel_Frm->Location = System::Drawing::Point(841, 394);
            this->_btFuel_Frm->Name = L"_btFuel_Frm";
            this->_btFuel_Frm->Size = System::Drawing::Size(76, 36);
            this->_btFuel_Frm->TabIndex = 62;
            this->_btFuel_Frm->TabStop = false;
            this->_btFuel_Frm->Text = L"Fuel Consumption";
            this->toolTip1->SetToolTip(this->_btFuel_Frm, L"Generate a bar chart on fuel consumption");
            this->_btFuel_Frm->UseVisualStyleBackColor = true;
            this->_btFuel_Frm->Click += gcnew System::EventHandler(this, &Wnd_Mai::_btFuel_Frm_Click);
            // 
            // menuStrip1
            // 
            this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->fileToolStripMenuItem, 
                this->optionsToolStripMenuItem, this->helpToolStripMenuItem});
            this->menuStrip1->Location = System::Drawing::Point(0, 0);
            this->menuStrip1->Name = L"menuStrip1";
            this->menuStrip1->Size = System::Drawing::Size(931, 24);
            this->menuStrip1->TabIndex = 7;
            this->menuStrip1->Text = L"menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->openProjectToolStripMenuItem, 
                this->saveProjectToolStripMenuItem, this->newProjectToolStripMenuItem, this->exitToolStripMenuItem1});
            this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
            this->fileToolStripMenuItem->Size = System::Drawing::Size(35, 20);
            this->fileToolStripMenuItem->Text = L"File";
            // 
            // openProjectToolStripMenuItem
            // 
            this->openProjectToolStripMenuItem->Name = L"openProjectToolStripMenuItem";
            this->openProjectToolStripMenuItem->Size = System::Drawing::Size(137, 22);
            this->openProjectToolStripMenuItem->Text = L"Open Project";
            this->openProjectToolStripMenuItem->ToolTipText = L"Larry";
            this->openProjectToolStripMenuItem->Click += gcnew System::EventHandler(this, &Wnd_Mai::openProjectToolStripMenuItem_Click);
            // 
            // saveProjectToolStripMenuItem
            // 
            this->saveProjectToolStripMenuItem->Name = L"saveProjectToolStripMenuItem";
            this->saveProjectToolStripMenuItem->Size = System::Drawing::Size(137, 22);
            this->saveProjectToolStripMenuItem->Text = L"Save Project";
            this->saveProjectToolStripMenuItem->Click += gcnew System::EventHandler(this, &Wnd_Mai::saveProjectToolStripMenuItem_Click);
            // 
            // newProjectToolStripMenuItem
            // 
            this->newProjectToolStripMenuItem->Name = L"newProjectToolStripMenuItem";
            this->newProjectToolStripMenuItem->Size = System::Drawing::Size(137, 22);
            this->newProjectToolStripMenuItem->Text = L"New Project";
            this->newProjectToolStripMenuItem->Click += gcnew System::EventHandler(this, &Wnd_Mai::newProjectToolStripMenuItem_Click);
            // 
            // exitToolStripMenuItem1
            // 
            this->exitToolStripMenuItem1->Name = L"exitToolStripMenuItem1";
            this->exitToolStripMenuItem1->Size = System::Drawing::Size(137, 22);
            this->exitToolStripMenuItem1->Text = L"Exit";
            this->exitToolStripMenuItem1->Click += gcnew System::EventHandler(this, &Wnd_Mai::exitToolStripMenuItem1_Click);
            // 
            // optionsToolStripMenuItem
            // 
            this->optionsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(15) {this->_cmBatch, 
                this->toolStripSeparator2, this->saveToolStripMenuItem, this->createEmissionCSVFileSummaryEnglishToolStripMenuItem, this->createEmissionCSVFIleToolStripMenuItem, 
                this->createEmissioToolStripMenuItem, this->toolStripSeparator3, this->createSampleBurnupInputFilebrnToolStripMenuItem, this->runBurnupFromInputFIlebrnToolStripMenuItem, 
                this->toolStripSeparator1, this->createSampleSoilInputFIledrfexpToolStripMenuItem, this->runSoilFromInputFileToolStripMenuItem, 
                this->saveSoilTempPointsFileToolStripMenuItem, this->toolStripSeparator4, this->settingsToolStripMenuItem});
            this->optionsToolStripMenuItem->Name = L"optionsToolStripMenuItem";
            this->optionsToolStripMenuItem->Size = System::Drawing::Size(56, 20);
            this->optionsToolStripMenuItem->Text = L"Options";
            // 
            // _cmBatch
            // 
            this->_cmBatch->Name = L"_cmBatch";
            this->_cmBatch->Size = System::Drawing::Size(276, 22);
            this->_cmBatch->Text = L"Batch Processing";
            this->_cmBatch->Click += gcnew System::EventHandler(this, &Wnd_Mai::_cmBatch_Click);
            // 
            // toolStripSeparator2
            // 
            this->toolStripSeparator2->Name = L"toolStripSeparator2";
            this->toolStripSeparator2->Size = System::Drawing::Size(273, 6);
            // 
            // saveToolStripMenuItem
            // 
            this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
            this->saveToolStripMenuItem->Size = System::Drawing::Size(276, 22);
            this->saveToolStripMenuItem->Text = L"Save Burnup Emission File ";
            this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &Wnd_Mai::saveToolStripMenuItem_Click);
            // 
            // createEmissionCSVFileSummaryEnglishToolStripMenuItem
            // 
            this->createEmissionCSVFileSummaryEnglishToolStripMenuItem->Name = L"createEmissionCSVFileSummaryEnglishToolStripMenuItem";
            this->createEmissionCSVFileSummaryEnglishToolStripMenuItem->Size = System::Drawing::Size(276, 22);
            this->createEmissionCSVFileSummaryEnglishToolStripMenuItem->Text = L"Create Emission CSV File - Summary ";
            this->createEmissionCSVFileSummaryEnglishToolStripMenuItem->Click += gcnew System::EventHandler(this, &Wnd_Mai::createEmissionCSVFileSummaryEnglishToolStripMenuItem_Click);
            // 
            // createEmissionCSVFIleToolStripMenuItem
            // 
            this->createEmissionCSVFIleToolStripMenuItem->Name = L"createEmissionCSVFIleToolStripMenuItem";
            this->createEmissionCSVFIleToolStripMenuItem->Size = System::Drawing::Size(276, 22);
            this->createEmissionCSVFIleToolStripMenuItem->Text = L"Create Emission CSV File - Short";
            this->createEmissionCSVFIleToolStripMenuItem->Click += gcnew System::EventHandler(this, &Wnd_Mai::createEmissionCSVFIleToolStripMenuItem_Click);
            // 
            // createEmissioToolStripMenuItem
            // 
            this->createEmissioToolStripMenuItem->Name = L"createEmissioToolStripMenuItem";
            this->createEmissioToolStripMenuItem->Size = System::Drawing::Size(276, 22);
            this->createEmissioToolStripMenuItem->Text = L"Create Emission CSV File - Full";
            this->createEmissioToolStripMenuItem->Click += gcnew System::EventHandler(this, &Wnd_Mai::createEmissioToolStripMenuItem_Click);
            // 
            // toolStripSeparator3
            // 
            this->toolStripSeparator3->Name = L"toolStripSeparator3";
            this->toolStripSeparator3->Size = System::Drawing::Size(273, 6);
            // 
            // createSampleBurnupInputFilebrnToolStripMenuItem
            // 
            this->createSampleBurnupInputFilebrnToolStripMenuItem->Name = L"createSampleBurnupInputFilebrnToolStripMenuItem";
            this->createSampleBurnupInputFilebrnToolStripMenuItem->Size = System::Drawing::Size(276, 22);
            this->createSampleBurnupInputFilebrnToolStripMenuItem->Text = L"Create Sample Burnup Input File (.brn)";
            this->createSampleBurnupInputFilebrnToolStripMenuItem->Click += gcnew System::EventHandler(this, &Wnd_Mai::createSampleBurnupInputFilebrnToolStripMenuItem_Click);
            // 
            // runBurnupFromInputFIlebrnToolStripMenuItem
            // 
            this->runBurnupFromInputFIlebrnToolStripMenuItem->Name = L"runBurnupFromInputFIlebrnToolStripMenuItem";
            this->runBurnupFromInputFIlebrnToolStripMenuItem->Size = System::Drawing::Size(276, 22);
            this->runBurnupFromInputFIlebrnToolStripMenuItem->Text = L"Run Burnup From Input File (.brn)";
            this->runBurnupFromInputFIlebrnToolStripMenuItem->Click += gcnew System::EventHandler(this, &Wnd_Mai::runBurnupFromInputFIlebrnToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this->toolStripSeparator1->Name = L"toolStripSeparator1";
            this->toolStripSeparator1->Size = System::Drawing::Size(273, 6);
            // 
            // createSampleSoilInputFIledrfexpToolStripMenuItem
            // 
            this->createSampleSoilInputFIledrfexpToolStripMenuItem->Name = L"createSampleSoilInputFIledrfexpToolStripMenuItem";
            this->createSampleSoilInputFIledrfexpToolStripMenuItem->Size = System::Drawing::Size(276, 22);
            this->createSampleSoilInputFIledrfexpToolStripMenuItem->Text = L"Create Sample Soil Input File ( .duf  .exp )";
            this->createSampleSoilInputFIledrfexpToolStripMenuItem->Click += gcnew System::EventHandler(this, &Wnd_Mai::createSampleSoilInputFIledrfexpToolStripMenuItem_Click);
            // 
            // runSoilFromInputFileToolStripMenuItem
            // 
            this->runSoilFromInputFileToolStripMenuItem->Name = L"runSoilFromInputFileToolStripMenuItem";
            this->runSoilFromInputFileToolStripMenuItem->Size = System::Drawing::Size(276, 22);
            this->runSoilFromInputFileToolStripMenuItem->Text = L"Run Soil From Input File";
            this->runSoilFromInputFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &Wnd_Mai::runSoilFromInputFileToolStripMenuItem_Click);
            // 
            // saveSoilTempPointsFileToolStripMenuItem
            // 
            this->saveSoilTempPointsFileToolStripMenuItem->Name = L"saveSoilTempPointsFileToolStripMenuItem";
            this->saveSoilTempPointsFileToolStripMenuItem->Size = System::Drawing::Size(276, 22);
            this->saveSoilTempPointsFileToolStripMenuItem->Text = L"Save Soil Temp Points File";
            this->saveSoilTempPointsFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &Wnd_Mai::saveSoilTempPointsFileToolStripMenuItem_Click);
            // 
            // toolStripSeparator4
            // 
            this->toolStripSeparator4->Name = L"toolStripSeparator4";
            this->toolStripSeparator4->Size = System::Drawing::Size(273, 6);
            // 
            // settingsToolStripMenuItem
            // 
            this->settingsToolStripMenuItem->Name = L"settingsToolStripMenuItem";
            this->settingsToolStripMenuItem->Size = System::Drawing::Size(276, 22);
            this->settingsToolStripMenuItem->Text = L"Settings";
            this->settingsToolStripMenuItem->Click += gcnew System::EventHandler(this, &Wnd_Mai::settingsToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem
            // 
            this->helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->helpToolStripMenuItem1, 
                this->aboutToolStripMenuItem});
            this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
            this->helpToolStripMenuItem->Size = System::Drawing::Size(40, 20);
            this->helpToolStripMenuItem->Text = L"Help";
            this->helpToolStripMenuItem->Click += gcnew System::EventHandler(this, &Wnd_Mai::helpToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem1
            // 
            this->helpToolStripMenuItem1->Name = L"helpToolStripMenuItem1";
            this->helpToolStripMenuItem1->Size = System::Drawing::Size(126, 22);
            this->helpToolStripMenuItem1->Text = L"User Guide";
            this->helpToolStripMenuItem1->Click += gcnew System::EventHandler(this, &Wnd_Mai::helpToolStripMenuItem1_Click);
            // 
            // aboutToolStripMenuItem
            // 
            this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
            this->aboutToolStripMenuItem->Size = System::Drawing::Size(126, 22);
            this->aboutToolStripMenuItem->Text = L"About";
            this->aboutToolStripMenuItem->Click += gcnew System::EventHandler(this, &Wnd_Mai::aboutToolStripMenuItem_Click);
            // 
            // _cbRegion
            // 
            this->_cbRegion->BackColor = System::Drawing::Color::White;
            this->_cbRegion->FormattingEnabled = true;
            this->_cbRegion->Location = System::Drawing::Point(831, 34);
            this->_cbRegion->Name = L"_cbRegion";
            this->_cbRegion->Size = System::Drawing::Size(86, 21);
            this->_cbRegion->TabIndex = 1;
            this->_cbRegion->TabStop = false;
            this->toolTip1->SetToolTip(this->_cbRegion, L"Select a US Geographic Region");
            this->_cbRegion->SelectedIndexChanged += gcnew System::EventHandler(this, &Wnd_Mai::_cbRegion_SelectedIndexChanged);
            this->_cbRegion->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Wnd_Mai::_cbFuelClass_KeyDown);
            // 
            // _txProFilNam
            // 
            this->_txProFilNam->BackColor = System::Drawing::SystemColors::ControlLight;
            this->_txProFilNam->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->helpProvider1->SetHelpKeyword(this->_txProFilNam, L"f1");
            this->helpProvider1->SetHelpString(this->_txProFilNam, L"Path and file name of currently selected project file");
            this->_txProFilNam->Location = System::Drawing::Point(4, 30);
            this->_txProFilNam->Name = L"_txProFilNam";
            this->_txProFilNam->ReadOnly = true;
            this->helpProvider1->SetShowHelp(this->_txProFilNam, true);
            this->_txProFilNam->Size = System::Drawing::Size(514, 22);
            this->_txProFilNam->TabIndex = 10;
            this->_txProFilNam->TabStop = false;
            this->toolTip1->SetToolTip(this->_txProFilNam, L"This is the name of the project file");
            // 
            // label6
            // 
            this->label6->AutoSize = true;
            this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label6->Location = System::Drawing::Point(342, 131);
            this->label6->Name = L"label6";
            this->label6->Size = System::Drawing::Size(33, 15);
            this->label6->TabIndex = 27;
            this->label6->Text = L"Duff";
            // 
            // label7
            // 
            this->label7->AutoSize = true;
            this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label7->Location = System::Drawing::Point(334, 123);
            this->label7->Name = L"label7";
            this->label7->Size = System::Drawing::Size(33, 15);
            this->label7->TabIndex = 26;
            this->label7->Text = L"Duff";
            // 
            // textBox5
            // 
            this->textBox5->ContextMenuStrip = this->_cms_AdjDead;
            this->textBox5->Location = System::Drawing::Point(546, 30);
            this->textBox5->Name = L"textBox5";
            this->textBox5->Size = System::Drawing::Size(50, 20);
            this->textBox5->TabIndex = 25;
            this->textBox5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // textBox6
            // 
            this->textBox6->ContextMenuStrip = this->_cms_AdjDead;
            this->textBox6->Location = System::Drawing::Point(489, 30);
            this->textBox6->Name = L"textBox6";
            this->textBox6->Size = System::Drawing::Size(50, 20);
            this->textBox6->TabIndex = 24;
            this->textBox6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // textBox7
            // 
            this->textBox7->ContextMenuStrip = this->_cms_AdjDead;
            this->textBox7->Location = System::Drawing::Point(433, 29);
            this->textBox7->Name = L"textBox7";
            this->textBox7->Size = System::Drawing::Size(50, 20);
            this->textBox7->TabIndex = 23;
            this->textBox7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // textBox8
            // 
            this->textBox8->ContextMenuStrip = this->_cms_AdjDead;
            this->textBox8->Location = System::Drawing::Point(377, 28);
            this->textBox8->Name = L"textBox8";
            this->textBox8->Size = System::Drawing::Size(50, 20);
            this->textBox8->TabIndex = 22;
            this->textBox8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // label8
            // 
            this->label8->AutoSize = true;
            this->label8->Location = System::Drawing::Point(582, 215);
            this->label8->Name = L"label8";
            this->label8->Size = System::Drawing::Size(77, 13);
            this->label8->TabIndex = 21;
            this->label8->Text = L"Heavy/Abund.";
            // 
            // label9
            // 
            this->label9->AutoSize = true;
            this->label9->Location = System::Drawing::Point(581, 195);
            this->label9->Name = L"label9";
            this->label9->Size = System::Drawing::Size(37, 13);
            this->label9->TabIndex = 20;
            this->label9->Text = L"typical";
            // 
            // label10
            // 
            this->label10->AutoSize = true;
            this->label10->Location = System::Drawing::Point(581, 175);
            this->label10->Name = L"label10";
            this->label10->Size = System::Drawing::Size(62, 13);
            this->label10->TabIndex = 19;
            this->label10->Text = L"light/sparse";
            // 
            // button5
            // 
            this->button5->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(190)), static_cast<System::Int32>(static_cast<System::Byte>(154)), 
                static_cast<System::Int32>(static_cast<System::Byte>(58)));
            this->button5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->button5->Location = System::Drawing::Point(559, 215);
            this->button5->Name = L"button5";
            this->button5->Size = System::Drawing::Size(16, 17);
            this->button5->TabIndex = 18;
            this->button5->Text = L"--";
            this->button5->UseVisualStyleBackColor = false;
            // 
            // button6
            // 
            this->button6->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(209)), static_cast<System::Int32>(static_cast<System::Byte>(200)), 
                static_cast<System::Int32>(static_cast<System::Byte>(135)));
            this->button6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->button6->Location = System::Drawing::Point(559, 193);
            this->button6->Name = L"button6";
            this->button6->Size = System::Drawing::Size(16, 17);
            this->button6->TabIndex = 17;
            this->button6->Text = L"--";
            this->button6->UseVisualStyleBackColor = false;
            // 
            // button7
            // 
            this->button7->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(225)), static_cast<System::Int32>(static_cast<System::Byte>(226)), 
                static_cast<System::Int32>(static_cast<System::Byte>(192)));
            this->button7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->button7->Location = System::Drawing::Point(464, 215);
            this->button7->Name = L"button7";
            this->button7->Size = System::Drawing::Size(16, 17);
            this->button7->TabIndex = 16;
            this->button7->Text = L"--";
            this->button7->UseVisualStyleBackColor = false;
            // 
            // button8
            // 
            this->button8->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(225)), static_cast<System::Int32>(static_cast<System::Byte>(226)), 
                static_cast<System::Int32>(static_cast<System::Byte>(192)));
            this->button8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->button8->Location = System::Drawing::Point(559, 171);
            this->button8->Name = L"button8";
            this->button8->Size = System::Drawing::Size(16, 17);
            this->button8->TabIndex = 15;
            this->button8->Text = L"--";
            this->button8->UseVisualStyleBackColor = false;
            // 
            // label11
            // 
            this->label11->AutoSize = true;
            this->label11->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label11->Location = System::Drawing::Point(259, 10);
            this->label11->Name = L"label11";
            this->label11->Size = System::Drawing::Size(23, 15);
            this->label11->TabIndex = 14;
            this->label11->Text = L"3+";
            // 
            // label12
            // 
            this->label12->AutoSize = true;
            this->label12->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label12->Location = System::Drawing::Point(197, 10);
            this->label12->Name = L"label12";
            this->label12->Size = System::Drawing::Size(28, 15);
            this->label12->TabIndex = 13;
            this->label12->Text = L"1-3";
            // 
            // label13
            // 
            this->label13->AutoSize = true;
            this->label13->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label13->Location = System::Drawing::Point(138, 10);
            this->label13->Name = L"label13";
            this->label13->Size = System::Drawing::Size(40, 15);
            this->label13->TabIndex = 12;
            this->label13->Text = L"1/4-1";
            // 
            // textBox9
            // 
            this->textBox9->ContextMenuStrip = this->_cms_AdjDead;
            this->textBox9->Location = System::Drawing::Point(246, 28);
            this->textBox9->Name = L"textBox9";
            this->textBox9->Size = System::Drawing::Size(50, 20);
            this->textBox9->TabIndex = 11;
            this->textBox9->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // textBox10
            // 
            this->textBox10->ContextMenuStrip = this->_cms_AdjDead;
            this->textBox10->Location = System::Drawing::Point(187, 28);
            this->textBox10->Name = L"textBox10";
            this->textBox10->Size = System::Drawing::Size(50, 20);
            this->textBox10->TabIndex = 10;
            this->textBox10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // textBox11
            // 
            this->textBox11->BackColor = System::Drawing::Color::White;
            this->textBox11->ContextMenuStrip = this->_cms_AdjDead;
            this->textBox11->Location = System::Drawing::Point(131, 28);
            this->textBox11->Name = L"textBox11";
            this->textBox11->Size = System::Drawing::Size(50, 20);
            this->textBox11->TabIndex = 9;
            this->textBox11->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // label14
            // 
            this->label14->AutoSize = true;
            this->label14->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label14->Location = System::Drawing::Point(310, 10);
            this->label14->Name = L"label14";
            this->label14->Size = System::Drawing::Size(33, 15);
            this->label14->TabIndex = 8;
            this->label14->Text = L"Duff";
            // 
            // label15
            // 
            this->label15->AutoSize = true;
            this->label15->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label15->Location = System::Drawing::Point(83, 10);
            this->label15->Name = L"label15";
            this->label15->Size = System::Drawing::Size(40, 15);
            this->label15->TabIndex = 7;
            this->label15->Text = L"0-1/4";
            // 
            // label16
            // 
            this->label16->AutoSize = true;
            this->label16->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label16->Location = System::Drawing::Point(27, 10);
            this->label16->Name = L"label16";
            this->label16->Size = System::Drawing::Size(40, 15);
            this->label16->TabIndex = 6;
            this->label16->Text = L"Litter";
            // 
            // textBox12
            // 
            this->textBox12->BackColor = System::Drawing::Color::White;
            this->textBox12->ContextMenuStrip = this->_cms_AdjDead;
            this->textBox12->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->textBox12->Location = System::Drawing::Point(17, 28);
            this->textBox12->Name = L"textBox12";
            this->textBox12->Size = System::Drawing::Size(50, 21);
            this->textBox12->TabIndex = 3;
            this->textBox12->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // textBox13
            // 
            this->textBox13->ContextMenuStrip = this->_cms_AdjDead;
            this->textBox13->Location = System::Drawing::Point(302, 28);
            this->textBox13->Name = L"textBox13";
            this->textBox13->Size = System::Drawing::Size(50, 20);
            this->textBox13->TabIndex = 4;
            this->textBox13->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // textBox14
            // 
            this->textBox14->BackColor = System::Drawing::Color::White;
            this->textBox14->ContextMenuStrip = this->_cms_AdjDead;
            this->textBox14->Location = System::Drawing::Point(73, 28);
            this->textBox14->Name = L"textBox14";
            this->textBox14->Size = System::Drawing::Size(50, 20);
            this->textBox14->TabIndex = 5;
            this->textBox14->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // _txReport
            // 
            this->_txReport->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
                | System::Windows::Forms::AnchorStyles::Left) 
                | System::Windows::Forms::AnchorStyles::Right));
            this->_txReport->BackColor = System::Drawing::Color::FloralWhite;
            this->_txReport->ContextMenuStrip = this->_cmsReport;
            this->_txReport->Font = (gcnew System::Drawing::Font(L"Lucida Console", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->_txReport->Location = System::Drawing::Point(109, 358);
            this->_txReport->Name = L"_txReport";
            this->_txReport->Size = System::Drawing::Size(715, 382);
            this->_txReport->TabIndex = 12;
            this->_txReport->TabStop = false;
            this->_txReport->Text = L"";
            // 
            // printDialog1
            // 
            this->printDialog1->AllowCurrentPage = true;
            this->printDialog1->AllowSomePages = true;
            this->printDialog1->UseEXDialog = true;
            // 
            // printDocument1
            // 
            this->printDocument1->PrintPage += gcnew System::Drawing::Printing::PrintPageEventHandler(this, &Wnd_Mai::printDocument1_PrintPage);
            this->printDocument1->BeginPrint += gcnew System::Drawing::Printing::PrintEventHandler(this, &Wnd_Mai::printDocument1_BeginPrint);
            // 
            // printPreviewDialog1
            // 
            this->printPreviewDialog1->AutoScrollMargin = System::Drawing::Size(0, 0);
            this->printPreviewDialog1->AutoScrollMinSize = System::Drawing::Size(0, 0);
            this->printPreviewDialog1->ClientSize = System::Drawing::Size(400, 300);
            this->printPreviewDialog1->Enabled = true;
            this->printPreviewDialog1->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"printPreviewDialog1.Icon")));
            this->printPreviewDialog1->MainMenuStrip = this->menuStrip1;
            this->printPreviewDialog1->Name = L"printPreviewDialog1";
            this->printPreviewDialog1->Visible = false;
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label2->Location = System::Drawing::Point(766, 36);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(53, 15);
            this->label2->TabIndex = 53;
            this->label2->Text = L"Region";
            this->toolTip1->SetToolTip(this->label2, L"Select a US Geographic Region");
            // 
            // _btReportEmis
            // 
            this->_btReportEmis->Location = System::Drawing::Point(18, 441);
            this->_btReportEmis->Name = L"_btReportEmis";
            this->_btReportEmis->Size = System::Drawing::Size(76, 36);
            this->_btReportEmis->TabIndex = 68;
            this->_btReportEmis->TabStop = false;
            this->_btReportEmis->Text = L"Smoke Emissions";
            this->toolTip1->SetToolTip(this->_btReportEmis, L"Generate a tabular report on smoke emissions ");
            this->_btReportEmis->UseVisualStyleBackColor = true;
            this->_btReportEmis->Click += gcnew System::EventHandler(this, &Wnd_Mai::_btReportEmis_Click);
            // 
            // _btReportFuel
            // 
            this->_btReportFuel->Location = System::Drawing::Point(18, 394);
            this->_btReportFuel->Name = L"_btReportFuel";
            this->_btReportFuel->Size = System::Drawing::Size(76, 36);
            this->_btReportFuel->TabIndex = 69;
            this->_btReportFuel->TabStop = false;
            this->_btReportFuel->Text = L"Fuel Consumption";
            this->toolTip1->SetToolTip(this->_btReportFuel, L"Generate a tabular report on fuel consumption");
            this->_btReportFuel->UseVisualStyleBackColor = true;
            this->_btReportFuel->Click += gcnew System::EventHandler(this, &Wnd_Mai::_btReportFuel_Click);
            // 
            // _btReportSoil
            // 
            this->_btReportSoil->Location = System::Drawing::Point(22, 514);
            this->_btReportSoil->Name = L"_btReportSoil";
            this->_btReportSoil->Size = System::Drawing::Size(60, 20);
            this->_btReportSoil->TabIndex = 70;
            this->_btReportSoil->TabStop = false;
            this->_btReportSoil->Text = L"Campbell";
            this->toolTip1->SetToolTip(this->_btReportSoil, L"Generate a tabular report on soil heating ");
            this->_btReportSoil->UseVisualStyleBackColor = true;
            this->_btReportSoil->Click += gcnew System::EventHandler(this, &Wnd_Mai::_btReportSoil_Click);
            // 
            // label21
            // 
            this->label21->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
            this->label21->AutoSize = true;
            this->label21->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)), 
                System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
            this->label21->Location = System::Drawing::Point(844, 362);
            this->label21->Name = L"label21";
            this->label21->Size = System::Drawing::Size(58, 16);
            this->label21->TabIndex = 71;
            this->label21->Text = L"Graphs";
            // 
            // label5
            // 
            this->label5->AutoSize = true;
            this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)), 
                System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
            this->label5->Location = System::Drawing::Point(19, 362);
            this->label5->Name = L"label5";
            this->label5->Size = System::Drawing::Size(63, 16);
            this->label5->TabIndex = 72;
            this->label5->Text = L"Reports";
            // 
            // _txRepTot
            // 
            this->_txRepTot->Location = System::Drawing::Point(5, 28);
            this->_txRepTot->Name = L"_txRepTot";
            this->_txRepTot->Size = System::Drawing::Size(84, 20);
            this->_txRepTot->TabIndex = 74;
            this->_txRepTot->TabStop = false;
            this->toolTip1->SetToolTip(this->_txRepTot, L"Supply a unique name for each total");
            // 
            // _btRepTotClear
            // 
            this->_btRepTotClear->Location = System::Drawing::Point(22, 82);
            this->_btRepTotClear->Name = L"_btRepTotClear";
            this->_btRepTotClear->Size = System::Drawing::Size(42, 22);
            this->_btRepTotClear->TabIndex = 75;
            this->_btRepTotClear->TabStop = false;
            this->_btRepTotClear->Text = L"Clear";
            this->toolTip1->SetToolTip(this->_btRepTotClear, L"Clear all stored total information");
            this->_btRepTotClear->UseVisualStyleBackColor = true;
            this->_btRepTotClear->Click += gcnew System::EventHandler(this, &Wnd_Mai::_btRepTotClear_Click);
            // 
            // _btRepTotDisp
            // 
            this->_btRepTotDisp->Location = System::Drawing::Point(24, 54);
            this->_btRepTotDisp->Name = L"_btRepTotDisp";
            this->_btRepTotDisp->Size = System::Drawing::Size(40, 22);
            this->_btRepTotDisp->TabIndex = 76;
            this->_btRepTotDisp->TabStop = false;
            this->_btRepTotDisp->Text = L"Write";
            this->toolTip1->SetToolTip(this->_btRepTotDisp, L"Write all stored totals to a report ");
            this->_btRepTotDisp->UseVisualStyleBackColor = true;
            this->_btRepTotDisp->Click += gcnew System::EventHandler(this, &Wnd_Mai::_btRepTotDisp_Click);
            // 
            // _cmsAdjNONE
            // 
            this->_cmsAdjNONE->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->toolStripMenuItem4});
            this->_cmsAdjNONE->Name = L"_cms_AdjDead";
            this->_cmsAdjNONE->Size = System::Drawing::Size(148, 26);
            // 
            // toolStripMenuItem4
            // 
            this->toolStripMenuItem4->Name = L"toolStripMenuItem4";
            this->toolStripMenuItem4->Size = System::Drawing::Size(147, 22);
            this->toolStripMenuItem4->Text = L"NO Adjustment";
            // 
            // _btReportMort
            // 
            this->_btReportMort->Location = System::Drawing::Point(18, 415);
            this->_btReportMort->Name = L"_btReportMort";
            this->_btReportMort->Size = System::Drawing::Size(76, 36);
            this->_btReportMort->TabIndex = 74;
            this->_btReportMort->TabStop = false;
            this->_btReportMort->Text = L"Mortality";
            this->toolTip1->SetToolTip(this->_btReportMort, L"Generate a tabular report on tree mortality by entries in tree list ");
            this->_btReportMort->UseVisualStyleBackColor = true;
            this->_btReportMort->Click += gcnew System::EventHandler(this, &Wnd_Mai::_btReportMort_Click);
            // 
            // _btGraphMort
            // 
            this->_btGraphMort->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
            this->_btGraphMort->Location = System::Drawing::Point(841, 415);
            this->_btGraphMort->Name = L"_btGraphMort";
            this->_btGraphMort->Size = System::Drawing::Size(76, 36);
            this->_btGraphMort->TabIndex = 77;
            this->_btGraphMort->TabStop = false;
            this->_btGraphMort->Text = L"Scorch Height";
            this->toolTip1->SetToolTip(this->_btGraphMort, L"Generate a graphic report on tree mortality by entries in tree list\r\n\r\n---> Also " 
                L"use this button to refresh a graph whenever you make\r\nchanges to the Tree Grid ");
            this->_btGraphMort->UseVisualStyleBackColor = true;
            this->_btGraphMort->Click += gcnew System::EventHandler(this, &Wnd_Mai::_btGraphMort_Click);
            // 
            // label19
            // 
            this->label19->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->label19->AutoSize = true;
            this->label19->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label19->Location = System::Drawing::Point(362, 745);
            this->label19->Name = L"label19";
            this->label19->Size = System::Drawing::Size(230, 13);
            this->label19->TabIndex = 78;
            this->label19->Text = L"Right Mouse Click Report Window  for Options ";
            // 
            // shapeContainer2
            // 
            this->shapeContainer2->Location = System::Drawing::Point(0, 0);
            this->shapeContainer2->Margin = System::Windows::Forms::Padding(0);
            this->shapeContainer2->Name = L"shapeContainer2";
            this->shapeContainer2->Shapes->AddRange(gcnew cli::array< Microsoft::VisualBasic::PowerPacks::Shape^  >(2) {this->lineShape2, 
                this->lineShape1});
            this->shapeContainer2->Size = System::Drawing::Size(931, 765);
            this->shapeContainer2->TabIndex = 79;
            this->shapeContainer2->TabStop = false;
            // 
            // groupBox1
            // 
            this->groupBox1->Controls->Add(this->label22);
            this->groupBox1->Controls->Add(this->_txRepTot);
            this->groupBox1->Controls->Add(this->_btRepTotDisp);
            this->groupBox1->Controls->Add(this->_btRepTotClear);
            this->groupBox1->Location = System::Drawing::Point(9, 623);
            this->groupBox1->Name = L"groupBox1";
            this->groupBox1->Size = System::Drawing::Size(96, 117);
            this->groupBox1->TabIndex = 80;
            this->groupBox1->TabStop = false;
            // 
            // label22
            // 
            this->label22->AutoSize = true;
            this->label22->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label22->Location = System::Drawing::Point(10, 7);
            this->label22->Name = L"label22";
            this->label22->Size = System::Drawing::Size(80, 15);
            this->label22->TabIndex = 77;
            this->label22->Text = L"Report Totals";
            this->toolTip1->SetToolTip(this->label22, L"Store and display report totals");
            // 
            // label24
            // 
            this->label24->AutoSize = true;
            this->label24->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label24->Location = System::Drawing::Point(524, 36);
            this->label24->Name = L"label24";
            this->label24->Size = System::Drawing::Size(84, 15);
            this->label24->TabIndex = 83;
            this->label24->Text = L"Project File ";
            this->toolTip1->SetToolTip(this->label24, L"This is the name of the project file");
            // 
            // _btClearReport
            // 
            this->_btClearReport->Location = System::Drawing::Point(31, 578);
            this->_btClearReport->Name = L"_btClearReport";
            this->_btClearReport->Size = System::Drawing::Size(48, 40);
            this->_btClearReport->TabIndex = 82;
            this->_btClearReport->TabStop = false;
            this->_btClearReport->Text = L"Clear Report";
            this->toolTip1->SetToolTip(this->_btClearReport, L"Clear all output in the FOFEM report window ");
            this->_btClearReport->UseVisualStyleBackColor = true;
            this->_btClearReport->Click += gcnew System::EventHandler(this, &Wnd_Mai::_btClearReport_Click);
            // 
            // _btFireIntensity
            // 
            this->_btFireIntensity->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
            this->_btFireIntensity->Location = System::Drawing::Point(841, 564);
            this->_btFireIntensity->Name = L"_btFireIntensity";
            this->_btFireIntensity->Size = System::Drawing::Size(76, 36);
            this->_btFireIntensity->TabIndex = 84;
            this->_btFireIntensity->TabStop = false;
            this->_btFireIntensity->Text = L"Fire Intensity";
            this->toolTip1->SetToolTip(this->_btFireIntensity, L"Generate a fire intensity graph ");
            this->_btFireIntensity->UseVisualStyleBackColor = true;
            this->_btFireIntensity->Click += gcnew System::EventHandler(this, &Wnd_Mai::_btFireIntensity_Click);
            // 
            // _btSoilMassman
            // 
            this->_btSoilMassman->Location = System::Drawing::Point(22, 541);
            this->_btSoilMassman->Name = L"_btSoilMassman";
            this->_btSoilMassman->Size = System::Drawing::Size(60, 20);
            this->_btSoilMassman->TabIndex = 87;
            this->_btSoilMassman->TabStop = false;
            this->_btSoilMassman->Text = L"Massman";
            this->toolTip1->SetToolTip(this->_btSoilMassman, L"Generate a tabular report on soil heating ");
            this->_btSoilMassman->UseVisualStyleBackColor = true;
            this->_btSoilMassman->Click += gcnew System::EventHandler(this, &Wnd_Mai::button1_Click);
            // 
            // _btSoiGraMas
            // 
            this->_btSoiGraMas->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
            this->_btSoiGraMas->Location = System::Drawing::Point(848, 532);
            this->_btSoiGraMas->Name = L"_btSoiGraMas";
            this->_btSoiGraMas->Size = System::Drawing::Size(60, 20);
            this->_btSoiGraMas->TabIndex = 89;
            this->_btSoiGraMas->TabStop = false;
            this->_btSoiGraMas->Text = L"Massman";
            this->toolTip1->SetToolTip(this->_btSoiGraMas, L"Generate a graphic report on soil heating ");
            this->_btSoiGraMas->UseVisualStyleBackColor = true;
            this->_btSoiGraMas->Click += gcnew System::EventHandler(this, &Wnd_Mai::_btSoiGraMas_Click);
            // 
            // pictureBox1
            // 
            this->pictureBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
            this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox1.Image")));
            this->pictureBox1->Location = System::Drawing::Point(859, 628);
            this->pictureBox1->Name = L"pictureBox1";
            this->pictureBox1->Size = System::Drawing::Size(38, 40);
            this->pictureBox1->TabIndex = 78;
            this->pictureBox1->TabStop = false;
            // 
            // _picFOFEM
            // 
            this->_picFOFEM->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
                | System::Windows::Forms::AnchorStyles::Left) 
                | System::Windows::Forms::AnchorStyles::Right));
            this->_picFOFEM->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_picFOFEM.Image")));
            this->_picFOFEM->Location = System::Drawing::Point(113, 358);
            this->_picFOFEM->Name = L"_picFOFEM";
            this->_picFOFEM->Size = System::Drawing::Size(707, 378);
            this->_picFOFEM->TabIndex = 81;
            this->_picFOFEM->TabStop = false;
            this->_picFOFEM->Click += gcnew System::EventHandler(this, &Wnd_Mai::_picFOFEM_Click);
            // 
            // _btTip
            // 
            this->_btTip->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
            this->_btTip->Location = System::Drawing::Point(842, 718);
            this->_btTip->Name = L"_btTip";
            this->_btTip->Size = System::Drawing::Size(75, 23);
            this->_btTip->TabIndex = 82;
            this->_btTip->TabStop = false;
            this->_btTip->Text = L"Helpful Tip ";
            this->_btTip->UseVisualStyleBackColor = true;
            this->_btTip->Click += gcnew System::EventHandler(this, &Wnd_Mai::_btTip_Click);
            // 
            // _btMort_Dam
            // 
            this->_btMort_Dam->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
            this->_btMort_Dam->Location = System::Drawing::Point(840, 466);
            this->_btMort_Dam->Name = L"_btMort_Dam";
            this->_btMort_Dam->Size = System::Drawing::Size(76, 36);
            this->_btMort_Dam->TabIndex = 85;
            this->_btMort_Dam->Text = L"Crown Damage";
            this->_btMort_Dam->UseVisualStyleBackColor = true;
            this->_btMort_Dam->Click += gcnew System::EventHandler(this, &Wnd_Mai::_btMort_Dam_Click);
            // 
            // _btMort_Bol
            // 
            this->_btMort_Bol->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
            this->_btMort_Bol->Location = System::Drawing::Point(841, 519);
            this->_btMort_Bol->Name = L"_btMort_Bol";
            this->_btMort_Bol->Size = System::Drawing::Size(76, 36);
            this->_btMort_Bol->TabIndex = 86;
            this->_btMort_Bol->Text = L"Bole Char";
            this->_btMort_Bol->UseVisualStyleBackColor = true;
            this->_btMort_Bol->Click += gcnew System::EventHandler(this, &Wnd_Mai::_btMort_Bol_Click);
            // 
            // label40
            // 
            this->label40->AutoSize = true;
            this->label40->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label40->Location = System::Drawing::Point(16, 491);
            this->label40->Name = L"label40";
            this->label40->Size = System::Drawing::Size(74, 15);
            this->label40->TabIndex = 88;
            this->label40->Text = L"Soil Reports";
            // 
            // label41
            // 
            this->label41->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
            this->label41->AutoSize = true;
            this->label41->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label41->Location = System::Drawing::Point(842, 485);
            this->label41->Name = L"label41";
            this->label41->Size = System::Drawing::Size(71, 15);
            this->label41->TabIndex = 90;
            this->label41->Text = L"Soil Graphs";
            // 
            // Wnd_Mai
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->AutoScroll = true;
            this->ClientSize = System::Drawing::Size(931, 765);
            this->Controls->Add(this->label41);
            this->Controls->Add(this->_btSoiGraMas);
            this->Controls->Add(this->label40);
            this->Controls->Add(this->_btSoilMassman);
            this->Controls->Add(this->_btMort_Bol);
            this->Controls->Add(this->_btMort_Dam);
            this->Controls->Add(this->_btFireIntensity);
            this->Controls->Add(this->_btTip);
            this->Controls->Add(this->label24);
            this->Controls->Add(this->_btClearReport);
            this->Controls->Add(this->_picFOFEM);
            this->Controls->Add(this->pictureBox1);
            this->Controls->Add(this->groupBox1);
            this->Controls->Add(this->label19);
            this->Controls->Add(this->_btGraphMort);
            this->Controls->Add(this->_btReportMort);
            this->Controls->Add(this->label5);
            this->Controls->Add(this->_btReportSoil);
            this->Controls->Add(this->_btReportFuel);
            this->Controls->Add(this->_btReportEmis);
            this->Controls->Add(this->_txProFilNam);
            this->Controls->Add(this->menuStrip1);
            this->Controls->Add(this->label21);
            this->Controls->Add(this->_tc_Main);
            this->Controls->Add(this->_txReport);
            this->Controls->Add(this->_btFuel_Frm);
            this->Controls->Add(this->_btEmis_Frm);
            this->Controls->Add(this->_btSoil_Frm);
            this->Controls->Add(this->_cbRegion);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->shapeContainer2);
            this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
            this->MainMenuStrip = this->menuStrip1;
            this->MaximumSize = System::Drawing::Size(2000, 2000);
            this->Name = L"Wnd_Mai";
            this->Text = L"FOFEM6.6";
            this->Load += gcnew System::EventHandler(this, &Wnd_Mai::Wnd_Mai_Load);
            this->ResizeBegin += gcnew System::EventHandler(this, &Wnd_Mai::Wnd_Mai_ResizeBegin);
            this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Wnd_Mai::Wnd_Mai_FormClosing);
            this->_cms_AdjDead->ResumeLayout(false);
            this->_cmsReport->ResumeLayout(false);
            this->_tc_Main->ResumeLayout(false);
            this->_tcpFuel->ResumeLayout(false);
            this->_tcpFuel->PerformLayout();
            this->groupBox3->ResumeLayout(false);
            this->groupBox3->PerformLayout();
            this->groupBox2->ResumeLayout(false);
            this->groupBox2->PerformLayout();
            this->_cms_AdjLive->ResumeLayout(false);
            this->_tcpMortality->ResumeLayout(false);
            this->_tcpMortality->PerformLayout();
            this->_gbSC3->ResumeLayout(false);
            this->_gbSC3->PerformLayout();
            this->_gbSC2->ResumeLayout(false);
            this->_gbSC2->PerformLayout();
            this->_gbSC1->ResumeLayout(false);
            this->_gbSC1->PerformLayout();
            this->_gbFlaSco->ResumeLayout(false);
            this->_gbFlaSco->PerformLayout();
            this->_gbFFI->ResumeLayout(false);
            this->_gbFFI->PerformLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_dgMort))->EndInit();
            this->menuStrip1->ResumeLayout(false);
            this->menuStrip1->PerformLayout();
            this->_cmsAdjNONE->ResumeLayout(false);
            this->groupBox1->ResumeLayout(false);
            this->groupBox1->PerformLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_picFOFEM))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    
/*****************************************************************************/
private: System::Void _cms_AdjDead_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{

}


/****************************************************************************
* Dead Fuel Adjustment Context Menu Item Clicked  
* Litter, 1,10,100,1k,Duff
* Items -> Light, Typical, Heavy, User
* Note-1: The way we do User loads is to save them all before we
*         make any adjustments to the load boxes, then after we
*         do any adjustments we go back and check if any of the load
*         boxes have the User Color and if so - we restore it with 
*         the saved load. This broad-brush approach I think will save me
*         a bunch of other coding. 
* Note-2: The focus is not set to the textbox when the user clicks in it
*         to bring up the adjustment menu, so we need to put the cursor in it
*                 
*****************************************************************************/
private: System::Void _cms_AdjDead_ItemClicked(System::Object^  sender, System::Windows::Forms::ToolStripItemClickedEventArgs^  e) 
{
String ^ MenuItem; 
String ^ TxBoxNam; /* TextBox Name, _txLitter, _tx1Hr, etc */
TextBox ^tb;
String ^ StrA;
String ^ StrB; 

/* Get the selected ContextMenu Item and the TextBox Name*/
  MenuItem = e->ClickedItem->Text->ToString();  
  TxBoxNam = this->_cms_AdjDead->SourceControl->Name->ToString(); 

 /* See Note-1 above */
  WM_UserFuels_Save();   /* Save any user entered loads */

/* Set the TextBox color based on the Selected Menu Item */
  this->Set_FueBoxCol (MenuItem, TxBoxNam); 

/* Go determine if any adjustment is needed and put it to the fuel load textbox */ 
 this->Mai_FuelTB_Load(TxBoxNam);  

/* Put any saved user load to any text box that has User Color */
  WM_UserFuels_Set();  

/* Put the focus on the seleted textbox, See Note-2 above */
  this->_cms_AdjDead->SourceControl->Focus(); 


/* Test Test .............................*/
//  if ( MenuItem == "User" && TxBoxNam == "_txDuff" ) {
//     StrA = this->s_uDuff;
//     StrB = this->_txDuff->Text;
//     MessageBox::Show("aaa", "TEST-!!!!", MessageBoxButtons::YesNo ); 
//  }
/* Test Test .............................. */


  this->Calculate("","");
}

/****************************************************************************
* Live Fuel Adjustment Context Menu Item Clicked  
* Herb, Shrub, Foliage, Branch
* Items -> Sparse, Typical, Abundant, User 
*****************************************************************************/
private: System::Void _cms_AdjLive_Item(System::Object^  sender, System::Windows::Forms::ToolStripItemClickedEventArgs^  e) 
{
String ^ MenuItem; 
String ^ TxBoxNam; /* TextBox Name, _txLitter, _tx1Hr, etc */

/* See the notes I have above in the _cms_AdjDead_ItemClicked()  */
/*  it does pretty much the same thing */ 

  MenuItem = e->ClickedItem->Text->ToString();  
  TxBoxNam = this->_cms_AdjLive->SourceControl->Name->ToString(); 
 
  WM_UserFuels_Save(); 
  this->Set_FueBoxCol (MenuItem, TxBoxNam); 
  this->Mai_FuelTB_Load(TxBoxNam); 
  WM_UserFuels_Set();

  this->_cms_AdjLive->SourceControl->Focus(); 

  this->Calculate("","");
}

/*************************************************************************
* Season Combo Box
**************************************************************************/ 
private: System::Void _cbSeason_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
{
 this->Calculate("","");
 }

/*************************************************************************
* Moisture Combo Box Changed 
**************************************************************************/
private: System::Void _cbMoist_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
{
 this->Moist_Change();
 this->Calculate("","");
}
/****************************************************************************
* Fuel result Output ComboBox changed 
*****************************************************************************/
private: System::Void _cbOutput_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
{
//   this->Output_Change(); 
 this->Calculate("","");
}

/****************************************************************************************
* Fuel Class ComboBox changed, combobox items are - SAF/SRM, NVCS, etc.
* Note-1: If user wants to use an FFI plot file than we need to popup the 
*         OpenFile DialBox because if user Cancels, we don't want to do 
*         all that other shit in this function
*         If they cancel we also need to restore the previously Selected item
*         in the FuelClass combobox.
* Note-2: The Project files is being read in and has set the Fuel Class ComboBox
*          with FFI and caused this event to go off,
*         the project file is also giving us the Pth file name of the FFI input file 
*          to use, so we don't want to put up a OpenFile Dialog box
****************************************************************************************/
private: System::Void _cbFuelClass_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
 {
int i; 
char cr_FFIPthFN[1000]; 

/* User Wants to use an FFI plot file - See Note-1 above */
  if (this->_cbFuelClass->Text == e_FFI ) { 
    if ( this->sS_FFI_Prj == "Yes" ) {  /* Prj File, see Note-2 above */
      _StrToChar (this->s_FFI_PthFN,cr_FFIPthFN);
      goto AA; }
    if ( !this->FFI_GetPthFN(cr_FFIPthFN)) {     /* Let User Select a File */
      this->_cbFuelClass->SelectedIndex = this->f_PreFuelClass;  /* See Note-1 above */
      return; }
    this->s_FFI_PthFN = _CharToStr(cr_FFIPthFN);  /* save the path file name */
  }   

AA:
/* FCCS Fuel Class needs to have "Fixed" put into the Log Dist ComboBox */
  this->b_DisEvt_LogDist = true;
  if ( this->_cbFuelClass->Text == e_FCCS || this->_cbFuelClass->Text == e_FFI ) {
     this->_cbLogDist->Items->Add (e_Fixed);
     i = this->_cbLogDist->FindString(e_Fixed);
     this->_cbLogDist->SelectedIndex = 5; }
  else { 
     this->_cbLogDist->Items->Clear();
     CB_Load (this->_cbLogDist);}

  this->b_DisEvt_LogDist = false ;

/* Check and turn on/off the Adjustment buttons, and contextmenus */
  i = WM_FuelAdj_Set();  
 
  if ( i == 1 ) { 
    WM_FuelBoxColors(e_Typical);  /* Set Fuel textboxs to Adjment color */
    Mai_FuelTB_Load("All");  }
 
  WM_ClassFile_Load(cr_FFIPthFN); 
 }

/****************************************************************************************
* Region ComboBox Changed
*****************************************************************************************/
private: System::Void _cbRegion_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
{
  WM_ClassFile_Load(""); 
  Mort_LoadSpeMstLst(); 
}

/*****************************************************************************
* Fuel Load Cover Type ComboBox 
* Note-1: This changes all fuel load values to Typical, otherwise the fuel loads
*         would stay at any adjustment value they might have such as sparse, abundant, etc 
******************************************************************************/
private: System::Void _cbCoverType_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
{
 
  WM_UserFuels_Save(); 

// change 9-12-2013, see Note-1 above
  WM_FuelBoxColors(e_Typical);    /* Set fuel textboxs to typical color */
// ..................            

  this->Mai_FuelTB_Load("All");  
  WM_UserFuels_Set(); 

 Emis_SetCovTyp();       /* Set the Emis Group ComboBoxes based on Cover Type */

 FLM_SetLogSoil();  /* FLM cover type need Log Dist and Soil ComboBoxs set */
 

 WD_FCCS_MessOff (); 
  this->Calculate("","");
  WD_FCCS_MessOn();

}

/**************************************************************************************
* Adjustment Heavy Abundant Button Clicked
**************************************************************************************/
private: System::Void _btHeavyAbun_Click(System::Object^  sender, System::EventArgs^  e) 
{
  WM_UserFuels_Save(); 
  WM_FuelBoxColors(e_Heavy);   /* Set fuel textboxs to Heavy color */
  this->Mai_FuelTB_Load("All"); 
  this->Set_TB_ReadOnly (true);  /* set all fuel textboxes to readonly */
  this->Calculate("","");
}

/**************************************************************************************
* Adjustment Typical Button Clicked
**************************************************************************************/
private: System::Void _btTypical_Click(System::Object^  sender, System::EventArgs^  e) 
{
 WM_UserFuels_Save(); 
 WM_FuelBoxColors(e_Typical);    /* Set fuel textboxs to typical color */
 this->Mai_FuelTB_Load("All"); 
 this->Set_TB_ReadOnly (true);  /* set all fuel textboxes to readonly */
 this->Calculate("","");
}

/**************************************************************************************
* Adjustment Light Sparse Button Clicked
**************************************************************************************/
private: System::Void _btLightSpa_Click(System::Object^  sender, System::EventArgs^  e)
{
 WM_UserFuels_Save(); 
 WM_FuelBoxColors(e_Light);    /* Set fuel textboxes to Light color */
 this->Mai_FuelTB_Load("All"); 
 this->Set_TB_ReadOnly (true);  /* set all fuel textboxes to readonly */
 this->Calculate("","");

}
/******************************************************************************
* Fuel Category ComboBox changed - Nature, Piles, Slash
*******************************************************************************/
private: System::Void _cbFuelCat_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
{
 WM_UserFuels_Save();
 this->Mai_FuelTB_Load("All");
 WM_UserFuels_Set();  
 this->Calculate("","");
}


/***********************************************************************************
* Main Menu - Report - Fuel, Smoke, Soil  
*************************************************************************************/
private: System::Void createToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
 {
   this->Report_Mngr("Fuel");
 }
private: System::Void smokeReportToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
 {
 this->Report_Mngr("Smoke");
 }

private: System::Void soilReportToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
{
 this->Report_Mngr("Soil");
}

/******************************************************************************
* Report Button Clicked, Fuel, Emission Soil 
********************************************************************************/
private: System::Void _btReportFuel_Click(System::Object^  sender, System::EventArgs^  e) 
{this->_picFOFEM->Hide();  this->Report_Mngr("Fuel");}
private: System::Void _btReportSoil_Click(System::Object^  sender, System::EventArgs^  e)
{this->_picFOFEM->Hide();  this->Report_Mngr("Soil");}
private: System::Void _btReportEmis_Click(System::Object^  sender, System::EventArgs^  e)
 {this->_picFOFEM->Hide();  this->Report_Mngr("Smoke");}



/***********************************************************************************
* Report Total Display button hit, so need to do the report totals into the 
*   report window
***********************************************************************************/
private: System::Void _btRepTotDisp_Click(System::Object^  sender, System::EventArgs^  e)
 {
int i; 
/* Fuels Tabl is showing */
  if ( this->_tcpFuel->Visible ) {
    this->Rep_FuelTotals();
    this->Rep_SmokeTotals(); 
    this->Rep_SoilTotals(); }

/* Mortality Tab is showing */
  if ( this->_tcpMortality->Visible ) {
    if ( this->_cb_SalRep->Checked )
      PFI_SummarySalvage(); 
 //   else if ( this->PostFire_Checked() )
 //     PFI_Summary ();
    else 
      this->MSR_Summary();          /* Regular MOrtality */
   }  /* if */ 
 }



/******************************************************************************
* Report Totals Clear button clicked 
******************************************************************************/
private: System::Void _btRepTotClear_Click(System::Object^  sender, System::EventArgs^  e) 
{
int i_RT;

 if ( this->_tcpFuel->Visible == true ) {  /* The Fuels Tab is showing */
    this->Rep_TotalsClear();               /* so clear fuel report totals */
    this->_txRepTot->Text = ""; 
    return; } 

/* Mortality Tab is showing, could be prefire, postfire or salvage report */ 
  i_RT = MortRepTyp();            /* get the report type that is set  */
  MT_Init (i_RT); 
  this->_txRepTot->Text = ""; 

}

/*******************************************************************************
* Graph Fuel button clicked 
*******************************************************************************/
private: System::Void _btFuel_Frm_Click(System::Object^  sender, System::EventArgs^  e) 
{

  this->Calculate("","");
 
  this->fuel_frm->Show();
  this->fuel_frm->BringToFront(); 
}
/********************************************************************************
* Log Distribution ComboBox changed
* My way of disabling this event functin from going off, there isn't a way
*  to do this in the Control object itself 
*********************************************************************************/
private: System::Void _cbLogDist_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
{
  if ( this->b_DisEvt_LogDist ) 
    return;
  this->Calculate("",""); 
}

/*************************************************************************
* Soil Type ComboBox Changed 
***********************************************************************/
private: System::Void _cbSoil_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
 {
   this->Calculate("",""); 
 }

/*******************************************************************************
* Graph Emis Button clicked 
*******************************************************************************/
private: System::Void _btEmis_Frm_Click(System::Object^  sender, System::EventArgs^  e)
 {
   this->Calculate("",""); 
   this->grap_emis->Show();
   this->grap_emis->BringToFront(); 
 }

/*******************************************************************************
* Campbell Graph Soil Button clicked 
*******************************************************************************/
private: System::Void _btSoil_Frm_Click(System::Object^  sender, System::EventArgs^  e) 
{
   this->Calculate("",""); 
   this->soil_frm3->Show();
   this->soil_frm3->BringToFront(); 

/* Soil User Input File Graph form */
//  this->soil_file->Show(); 
//   this->soil_file->BringToFront(); causes exception 

/* Soil Dynamic Graph Form - Shows a moving graph of soil layers */
//   this->SNF->Show();
//   this->SNF->BringToFront(); 

}

/************************************************************************
* Massman Soil Graph button clicked 
************************************************************************/
private: System::Void _btSoiGraMas_Click(System::Object^  sender, System::EventArgs^  e)
{
int i; 
float f; 
char cr_Out[500],cr_Name[500],cr_ErrMes[1000];

/* First time the user opens the Massman Soil Window, assign Air Temp and Soil moist */
/*  from main window to Massman graph window */
   if ( this->iS_MassWnd == 0 ) {
      this->BmSoil_Frm->_txBM_AmbAirTmp->Text = this->_txAmbAirTmp->Text;
      i =  _Get_TBFlo (this->_txMoistSoil, &f, cr_Out,cr_Name,cr_ErrMes);
      f = f * 0.010; 
      FloatToTextBox (this->BmSoil_Frm->_txBM_Moist,f,2); 
      this->iS_MassWnd++; 
   }

   this->BmSoil_Frm->Show();
   this->BmSoil_Frm->BringToFront(); 

/* The first time the Massman Graph Button is hit - set the FireType to Burnup */
   if ( this->i_BMSoil_BurnupSw == 0 ) { 
     this->BmSoil_Frm->SetFireTypeBurnup(); 
     this->i_BMSoil_BurnupSw++; }
}

/*******************************************************************************
* Fire Intenisty Graph Button Clicked 
********************************************************************************/
private: System::Void _btFireIntensity_Click(System::Object^  sender, System::EventArgs^  e)
{
   this->Calculate("",""); 
   this->fire_frm->Show();
   this->fire_frm->BringToFront(); 
}

/***********************************************************************************
*
************************************************************************************/
private: System::Void Wnd_Mai_ResizeBegin(System::Object^  sender, System::EventArgs^  e)
 {
int i; 
  i = this->Height;
 }

/************************************************************************************
* Report Wnd ContextMenu Item clicked 
*
* Note-1:  If I didn't hide the popped up contextmenu then it stayed on the screen 
*          while the other Dialogbox is being shown, I don't know why that
*          only happens for some DialogBoxes 
*************************************************************************************/
private: System::Void _cmsReport_ItemClicked(System::Object^  sender, System::Windows::Forms::ToolStripItemClickedEventArgs^  e)
{
String ^ s_MenuItem;
// System::Windows::Controls::PrintDialog pd = new PrintDialog();
// this->printDialog1->Document = this->_txReport;

  s_MenuItem = e->ClickedItem->Text->ToString(); 

  if ( s_MenuItem == "Copy")
     this->_txReport->Copy();

  if ( s_MenuItem == "Cut") 
    this->_txReport->Cut();
 
  if ( s_MenuItem == "Paste" )
    this->_txReport->Paste();

  if ( s_MenuItem == "Save" ){
     this->_cmsReport->Hide();  /* See Note-1 above */
     this->Report_Save();  }

  if ( s_MenuItem == "Clear")
   this->_txReport->Clear(); 

 /* Print..... */
  if ( s_MenuItem == "Print" ) { 
    this->_cmsReport->Hide();  /* See Note-1 above */
    System::Windows::Forms::DialogResult r; 
    this->StringToPrint = this->_txReport->Text; 
    printDocument1->DocumentName = "FOFEM Report";
    this->printDialog1->Document = printDocument1; 
    r = this->printDialog1->ShowDialog();
    if ( r == System::Windows::Forms::DialogResult::OK )
      this->printDialog1->Document->Print(); 
  }
  

/* Print Preview */
  if ( s_MenuItem == "Print Preview" ) {
     this->StringToPrint = this->_txReport->Text; 
     printDocument1->DocumentName = "FOFEM Report"; /* shows in the little Page Wnds that popup */  
     printDocument1->PrinterSettings->PrintRange = Printing::PrintRange::AllPages;
     printPreviewDialog1->Document = printDocument1; /* Associate PrintDocuemnt obj with */    
     printPreviewDialog1->ShowDialog();              /* Show the PrintPreviewDialog box */

 } /* if ( print preview  */
}

/***********************************************************************************
* See Print and Print Preview above
* this function goes off for every page that needs to be printed/preview
* We basically keep feeding it the remaining portion of the string that
* hasn't been printed yet. 
***********************************************************************************/
private: System::Void printDocument1_PrintPage(System::Object^  sender, System::Drawing::Printing::PrintPageEventArgs^  e)
 {
int Chars, Lines, checkPrint; 
String ^ Str; 
String ^ substr; 

/* Set the amount of text the page will hold */
  e->Graphics->MeasureString(this->StringToPrint,
                             this->printPreviewDialog1->Font,
                             e->MarginBounds.Size,
                             StringFormat::GenericTypographic, Chars, Lines);

/* put the text */
  e->Graphics->DrawString (this->StringToPrint, 
                           this->_txReport->Font, 
                           Brushes::Black,
                           e->MarginBounds, 
                           StringFormat::GenericTypographic); 
 
 /* Portion of string still needing to be printed */
 substr = this->StringToPrint->Substring(Chars);

  if (  substr->Length > 0 ) {
    e->HasMorePages = true;
    this->StringToPrint = substr; }  /* we'll print this next */
  else 
    e->HasMorePages = false ;      
 }

/*********************************************************************************/
private: System::Void printDocument1_BeginPrint(System::Object^  sender, System::Drawing::Printing::PrintEventArgs^  e) 
{ 
}

/***********************************************************************************
* 
***********************************************************************************/
private: System::Void printPreviewToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
{int i;}


/***********************************************************************************
* FuelClass Keydown event - this makes comboboxs Readonly
* NOTE--> I point all the comboboxes to this function, no need to have a bunch
*         of individual one. 
*         Up and Down keys are allowed to keep working 
*
* Note-1: I decided I don't want to let the user use up down keys in FuelClass combobox
* because it will make dealing with the FFI selection difficult if user 
* decides to Cancel when asked to select the file name. 
**********************************************************************************/
private: System::Void _cbFuelClass_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) 
{

/* See Note-1 above */ 
 if ( sender == this->_cbFuelClass )  /* Fuel Class ComboBox, SAF, FCCS,etc */
   goto Skip; 
  
 if ( e->KeyCode == Keys::Down ) 
   return; 
 if ( e->KeyCode == Keys::Up ) 
   return; 

Skip:
 e->SuppressKeyPress = true; 

}

/************************************************************************
* Main Menu -> File -> New Project
**************************************************************************/
private: System::Void newProjectToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
{
  this->Prj_SaveMngr("New"); 
}


/*******************************************************************************
* Main Menu -> File -> Save Project
*******************************************************************************/
private: System::Void saveProjectToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
 {
   this->Prj_SaveMngr("Save"); 
 }
/*******************************************************************************
* Main Menu -> File -> Open Project
*******************************************************************************/

private: System::Void openProjectToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
 {
   this->Prj_Open(""); 
   this->Calculate("",""); 
 }

/********************************************************************************
* User clicked the X in the upper right corner of the form 
* For now I'm just going to save the .ini file
********************************************************************************/
private: System::Void Wnd_Mai_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
 {
   if ( this->BmSoil_Frm->b_Running == true ) { 
     myMessageBox ("Stop the Massman Soil Simulation before exiting FOFEM"); 
     e->Cancel = true; 
     return ; }

   Prj_IniFil_Save ();  /* Save the .ini file */ 
 }

/*********************************************************************************
* Main Menu - Option - Batch Processing 
*********************************************************************************/
private: System::Void _cmBatch_Click(System::Object^  sender, System::EventArgs^  e)
 {
   this->bat_frm->Show();   /* Show the Batch Form */
 }

/****************************************************************************
* Mortality Tab FFI Input file Button Clicked 
*
*****************************************************************************/
private: System::Void _btTest_Click(System::Object^  sender, System::EventArgs^  e)
 {
char cr[1000];
  if ( this->PostFire_Checked() ) {
    strcpy (cr, "Please uncheck the Post Fire Injury button. \n");
    strcat (cr, "FFI input files do not contain enough information to do Post Fire Injury");  
    myMessageBox (cr);
    return; }

  this->FFI_LoadPlotNames("");   /* Open file and load plot names into combobox */
}

/***************************************************************************
* Mortality of Fuel Tab Clicked 
****************************************************************************/
private: System::Void _tcpMortality_Enter(System::Object^  sender, System::EventArgs^  e)
 {
char  cr[2000]; 
/* Switched to Mortality Tab..so hide Fuel Report * Graph buttons */
  this->_btReportEmis->Hide();
  this->_btReportFuel->Hide();
  this->_btReportSoil->Hide();
  this->_btSoilMassman->Hide(); 


/* Hide the Graph buttons for Fuel stuff */
  this->_btEmis_Frm->Hide();   /* Emis graph form button */
  this->_btSoil_Frm->Hide(); 
  this->_btFuel_Frm->Hide(); 
  this->_btFireIntensity->Hide();
  this->_btSoiGraMas->Hide();  

  this->label40->Hide(); 
  this->label41->Hide(); 

  this->_btReportMort->Show();
  this->_btGraphMort->Show(); 
  this->_btMort_Bol->Show(); 
  this->_btMort_Dam->Show(); 


#ifdef OLDOLD
  We Care about this situation with the New way we do Mortality 
  if ( this->_cbRegion->Text->ToString() == e_CI_PacificWest )
    return; 
  if ( this->_cbRegion->Text->ToString() == e_CI_InteriorWest )
    return; 
  
/* Post Fire and Southeast or Northeast is selected */
  strcpy (cr, "There are no Post Fire Injury Mortality equations available for\n");
  strcat (cr, "the selected region.\n");
  strcat (cr, "If there is any information showing in the species datagrid\n");
  strcat (cr, "it may have been previously entered under a different region.\n");
  myMessageBox (cr);
#endif  
}

/***************************************************************************
* Fuel Tab Clicked 
****************************************************************************/
private: System::Void _tcpFuel_Enter(System::Object^  sender, System::EventArgs^  e)
 {
/* Switched to the Fuel Tab - so show the report & graph buttons */
  this->_btReportEmis->Show();  /* Report buttons */
  this->_btReportFuel->Show();
  this->_btReportSoil->Show();
  this->_btSoilMassman->Show();

  this->_btEmis_Frm->Show();   /* Emis graph form button */
  this->_btSoil_Frm->Show(); 
  this->_btFuel_Frm->Show(); 
  this->_btFireIntensity->Show(); 
  this->_btSoiGraMas->Show(); 

  this->label40->Show();
  this->label41->Show(); 

  this->_btReportMort->Hide();
  this->_btGraphMort->Hide(); 
  this->_btMort_Bol->Hide(); 
  this->_btMort_Dam->Hide(); 
}

#ifdef OLD_NOTUSED
/************************************************************************
* Mortality Tab - Postfire Injury Checkbox changed
* Mortality Regular and Postfire Injury have diff spec lists associated
* with them. 

* Note-1: Draws either the Regular Mort Graph or the PFI graph 
*  doing this auto redraw can cause a popup warning when 
*  tree grid is empty, but leaving this redraw out will leave the
*  old graph showing, I tried clearing the but then the user
*  has to figure out to hit the "Mort" graph button again - either 
*  way I can't win............
************************************************************************/
private: System::Void _chPostFire_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
  this->Mort_LoadSpeMstLst(); /* Load the Species Master List */
//  this->Mort_PFIGUI();        /* Show/Hide certain controls */
 // MT_Init();                  /* Clear the Totals */

  this->Set_MortSpeciesHelp();  /* Set the F1 help for Tree Grid & Spec Combbox accordingly */
 
/* Note-1 above */
  if ( this->iS_Calc != 0 )   /* Don't this is during startup */
    this->Mort_Graph(); 

// see Note-1 above.
// if ( this->_chPostFire->Checked ) 
//    this->mort_frm->InitGraphPFI();
//  else 
//    this->mort_frm->InitGraph(); 

}
#endif 

/**********************************************************************
* Mortality Graph Button Clicked 
**********************************************************************/
private: System::Void _btGraphMort_Click(System::Object^  sender, System::EventArgs^  e)
{
  this->Mort_Graph(); 

/* Show it after creating or it won't stay on top of main wnd */
  this->mort_frm->Show ();  
  this->mort_frm->BringToFront(); 
}

/**********************************************************
* Mortality Crown Damage Graph Button Clicked
**********************************************************/
private: System::Void _btMort_Dam_Click(System::Object^  sender, System::EventArgs^  e)
{
  this->Mort_GraphPFI();   /* Draw Graph */
  this->mort_Dam->Show(); 
  this->mort_Dam->BringToFront();
}

/**********************************************************
* Mortality Bole Char Graph Button Clicked 
**********************************************************/
private: System::Void _btMort_Bol_Click(System::Object^  sender, System::EventArgs^  e)
{
  this->Mort_GraphBol(); 
  this->mort_Bol->Show(); 
  this->mort_Bol->BringToFront();
}

/*****************************************************************************
* Mortality Tab Species DataGridView Checkbox clicked
* NOTE NOTE -> this is the Regular Mortality DataGridView,
*            We don't ->Show() the graph here, just update it here, the user
*            may just be filling in the Spe DateGrid and not have opened 
*            the graph window yet, But if they do have it open already
*            they'll see it change when user clicks a box  
* Note-1  We have to move the Focus off the checkbox so that the change
*         will actualy get committed to the form, else when we try to read it
*         when we do the Graph we won't see the change. 
******************************************************************************/
private: System::Void _dgMort_CellContentClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e)
 {
  if ( e->ColumnIndex != edg_Gra )  /* We only want the Graph Checkbox col */
   return ; 

  this->_tcpMortality->Focus();  /* see Note-1 above */
  this->Mort_GraphReg();         /* Go update the graph */
 }

/******************************************************************
* Mortality Tab - Low Fire Severity checkbox changed 
******************************************************************/
private: System::Void _chLowFireSev_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
  this->Mort_GraphReg();         /* Go update the graph */
}

 
/********************************************************************
* Main Menu -> Options -> Create Burnup Emissions File 
********************************************************************/
private: System::Void saveToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
{
  this->Burnup_EmFil_Save(); 
}

/********************************************************************
*  Main Menu -> Options -> Create Emission .CSV file 
*   Full - ALL fuel Components
********************************************************************/
private: System::Void createEmissioToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
{
   this->Emission_CSVFile ("Full","");
}
/********************************************************************
* Main Menu -> Options -> Create Emission .CSV file - short version
*    Short - Major fuel Components -  
********************************************************************/ 
private: System::Void createEmissionCSVFIleToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
{
   this->Emission_CSVFile ("Short","");
}

/*******************************************************************
* Main Menu - > Options -> Create Emission .CSV file
*           Summary - one line for each Emission component
*******************************************************************/
private: System::Void createEmissionCSVFileSummaryToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
{
   this->Emission_CSVFile("Summary","Metric"); 
}

/***********************************************************
* Main Menu -> Options -> Create Emission .CSV file
*          Summary - one line for each Emis component
******************************************************************/
private: System::Void createEmissionCSVFileSummaryEnglishToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
{
   this->Emission_CSVFile("Summary", "English" ); 
}


/**********************************************************************
* Main Menu -> Item - Create Sample Burnup input file 
***********************************************************************/
private: System::Void createSampleBurnupInputFilebrnToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
{
  this->Burnup_SamInpFile();
}

/******************************************************************
* Main Menu -> Item - Run Burnup from Input file
******************************************************************/
private: System::Void runBurnupFromInputFIlebrnToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
{
  this->Burnup_RunInpFile (); 
}


/*********************************************************************
* Main Menu -> Item - Create Sample Soil Input files 
*********************************************************************/
private: System::Void createSampleSoilInputFIledrfexpToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
 {
   this->Soil_CreateSample (); 
 }

/**********************************************************************
* Main Menu -> Item - Run Soil From Input file 
**********************************************************************/
private: System::Void runSoilFromInputFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
{
   this->Soil_RunInputFile (); 
}

/*************************************************************************
* Main Menu -> Item - Save Soil Points File 
**************************************************************************/
private: System::Void saveSoilTempPointsFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
{
  this->Soil_SavePointsFile(); 
}

/*********************************************************************
* Mortality Tab - FFI Load File button clicked 
* NOTE: I had had this action coming off of the Main Menu Options 
*       but decied to just put a button on the Mortality Tab page
**********************************************************************/
private: System::Void fFIInputToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
{
  this->FFI_LoadPlotNames(""); 
}

/*******************************************************************
* Mortality Tab - FFI Plot number ComboBox Changed 
*****************************************************************/
private: System::Void _cbFFI_Plots_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
{
char cr[1000]; 

  if ( this->_txFFI_File->Text == "" )
     return;  

  if ( this->PostFire_Checked() ) {
    myMessageBox ("Uncheck the Post Fire Injury Checkbox to use FFI tree data"); 
    return; }

 // this->mort_frm->InitGraph("FlameLength"); 
  this->FFI_LoadTreeGrid();  /* Load Tree Grid with plot's tree data from file */
}

/*************************************************************
* Fuel,Moisture,etc Textbox event - Check for the Enter Key
*  and perform the Calculation
* We want to do an automatic Calculation when the user changes
*  the text in a fuel box, Mositure box, Rotten, Fol/Bra percent
**************************************************************/
private: System::Void _txMoistDuff_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{

/* if a User Duff AMOUNT was entered and Enter Key hit, if so we need  */
/*  to go adjust the duff depth accordingly  */
  if ( sender == this->_txDuff && e->KeyCode ==   System::Windows::Forms::Keys::Enter)
     this->Duff_AdjDepth();    

 /* Duff DEPTH */
 if ( sender == this->_txDuffDepth && e->KeyCode ==   System::Windows::Forms::Keys::Enter)
     this->Duff_AdjAmount();   


  if ( e->KeyCode ==   System::Windows::Forms::Keys::Enter)
    this->Calculate("",""); 
}

/******************************************************************
* FOFEM Picture clicked on - so hide it.
* the fofem picture is put up over the Report window on startup
* 
*******************************************************************/
private: System::Void _picFOFEM_Click(System::Object^  sender, System::EventArgs^  e)
 {
this->_picFOFEM->Hide(); 
 }
/****************************************************************
* Main Menu -> Help -> Help 
****************************************************************/
private: System::Void helpToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e)
{  
int i; 
String ^ Str, ^ Path, ^ A; 
char   cr[1000];

  Path = System::Windows::Forms::Application::StartupPath;  /* Program starup dir */
  Str = Path + "\\FOFEM6_Help.pdf";
  i = System::IO::File::Exists(Str);            /* make sure it's there or */
  if ( i == 0 ) {                               /* we'll get an exception below */
    A = "Help file is missing ? ? \n" + Str;
    _StrToChar (A,cr); 
    myMessageBox (cr); 
    return; }
 
  System::Diagnostics::Process::Start(Str);
}

/**************************************************************
* Main Menu -> File -> Exit
* Note-1: according to my notes in FuelCalc where I close 
*         the application, if I don't do the Close() the
*         program may not always close the window off the screen
**************************************************************/
private: System::Void exitToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e)
{
int i; 
System::Windows::Forms::DialogResult r;
 
  r =  MessageBox::Show("Do you want to exit FOFEM ? ", "Exiting FOFEM", MessageBoxButtons::YesNo); 
  if ( r == ::DialogResult::No ) 
    return ; 
 
  r =  MessageBox::Show("Do you want to save your settings ? ", "Exiting FOFEM", MessageBoxButtons::YesNo);    
  if ( r == ::DialogResult::Yes )  
      Prj_SaveMngr ("Save");
  
  Prj_IniFil_Save ();    /* Go save .ini file */ 
  this->Close();             /* See Note-1 above */
  Application::Exit(); 
}

/***********************************************************
* Main Menu -> Help -> About
************************************************************/
private: System::Void aboutToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
{
Abt_Frm ^ af;
  af = gcnew Abt_Frm;  
  af->Show(); 
  af->BringToFront(); 
}

/******************************************************************************
* Clear Report Button 
*******************************************************************************/
private: System::Void _btClearReport_Click(System::Object^  sender, System::EventArgs^  e)
{
 this->_txReport->Clear(); 
}

/**************************************************************************
* Helpfull Tip button clicked
**************************************************************************/
private: System::Void _btTip_Click(System::Object^  sender, System::EventArgs^  e)
 {
char cr[1000];
  strcpy (cr, "                          Ways to get help\n\n");

  strcat (cr, " 1. Hover the cursor over a Button, Textbox, Dropdown or Label for a breif tip.\n");
  strcat (cr, " 2. Use F1 after clicking in a Textbox or a Dropdown list to get more detail\n");
  strcat (cr, " 3. Go to the complete Help documentation using the Main Menu -> Help\n\n");
  myMessageBox (cr); 
 }

/*************************************************************************************
*  FuelClass ComboBox is clicked and dropped down - this will save the
*  selection in the combobox before it gets changed. We might need this incase
*  the user selects FuelClass "FFI" but then does a Cancel when they 
*  go to open the actual FFI file with an Open File Dialog Box 
*************************************************************************************/
private: System::Void _cbFuelClass_DropDown(System::Object^  sender, System::EventArgs^  e) 
{
  this->f_PreFuelClass = this->_cbFuelClass->SelectedIndex;
}

/*******************************************************************
* Mortality Window/Tab - FFI Clear button clicked
*******************************************************************/
private: System::Void _btFFI_FileClose_Click(System::Object^  sender, System::EventArgs^  e)
{
  this->FFI_Close();
}

/******************************************************************
* Emission Flame/Smolder Group Number ComboBox itme changed 
*******************************************************************/
private: System::Void _cbEmiFla_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
{
  this->Calculate("",""); 
}

/************************************************************************
* Main Menu -> Options -> Settings 
**********************************************************************/
private: System::Void settingsToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
{
  this->set_frm->Show(); 
}

/********************************************************************
* Mortality Report Button Clicked 
********************************************************************/
private: System::Void _btReportMort_Click(System::Object^  sender, System::EventArgs^  e) 
{
int i; 
char cr_ErrMes[2000];

  this->_picFOFEM->Hide(); 
  this->MIS_ReportNew();
  return ; 
}

/*************************************************************************
* Move Species Button Clicked 
* Need to move the selected item in the Species combobox to the 
* the Species DataGrid 
*************************************************************************/
private: System::Void _btSelect_Click(System::Object^  sender, System::EventArgs^  e) 
{
int c,r;
  r = Mort_Select();
  this->Set_ReqCols(r); 

}

/********************************************************************
* Mortality DataGridView - cell value changed 
* When a species is entered we need to deal with it's combobox
*  see call Set_EqComBox
* NOTE: This function can get call in two ways
*       1. Clicking the '---->' button to load from the Species Master 
*           combobox
*       2. the user types in a species 
*********************************************************************/
private: System::Void _dgMort_CellValueChanged(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) 
{
int r,c;  
char cr_Mode[100];
/* Get row & col of cell that changed */
   c =  e->ColumnIndex; 
   r = e->RowIndex; 
   if ( r < 0 ) 
     return; 

   if ( c != edg_Spe )    
     return;               /* Only want when Species Col changed */ 

   this->Attach_EquTypComBox(r,c,cr_Mode);  /* go deal with it's combobox */ 

/* if user typed the Species into the Species Cell */
   if ( !stricmp (cr_Mode,"Hand") ) 
      this->Set_ReqCols(r); 

}

/***********************************************************************
* EditControlShowing
* Use this Event Handler to set the SelectedIndexChanged event handler
* for a Mort Datagrid combobox
* See Wnd_Mai::Set_EquComBox() which adds a new combobox to the Datagrid
*  here we need to attach a SelectedIndexChanged handler to it
***********************************************************************/
private: System::Void _dgMort_EditingControlShowing(System::Object^  sender, System::Windows::Forms::DataGridViewEditingControlShowingEventArgs^  e)
{
int r,c; 
ComboBox ^ cb; 
  
  c = this->_dgMort->CurrentCell->ColumnIndex; 
  r = this->_dgMort->CurrentCell->RowIndex; 
  if ( c != edg_Equ ) 
    return; 

  cb =  (ComboBox ^ ) e->Control ; 
  cb->SelectedIndexChanged  -= gcnew System::EventHandler(this, &Wnd_Mai::_EquTyp_ComboBoxChanged);
  cb->SelectedIndexChanged  += gcnew System::EventHandler(this, &Wnd_Mai::_EquTyp_ComboBoxChanged);
 
/* This is a work around for the MicroSoft ComboBox box that cause the ComboBox background */
/* color to change to black */
 e->CellStyle->BackColor = this->_dgMort->DefaultCellStyle->BackColor;
}

/*==========================================================================
*  The SelectedIndexChanged EventHandler for DataGridView Equation 
*  Type ComboBox 
*  See function above 
*==========================================================================*/
private: System::Void _EquTyp_ComboBoxChanged (System::Object^  sender, System::EventArgs^  e)
{
int i, r, c;
String ^ Str;
  c = this->_dgMort->CurrentCell->ColumnIndex; 
  r = this->_dgMort->CurrentCell->RowIndex; 
  this->Set_ReqCols(r); 
}
/*****************************************************************************
* Mortality Salvage Report Checkbox Changed
*****************************************************************************/
private: System::Void _cb_SalRep_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
String ^ Str; 
   if ( this->_cb_SalRep->Checked == true )  /* Salave box is checked  */ 
     this->SalvageSet("Show");               /* show the Size Class Prob controls */ 
   else 
     this->SalvageSet("Hide"); 
}

/****************************************************************************
* FFI Error File Button Clicked 
* file contains errors encountered while loading  mortality tree grid
*****************************************************************************/
private: System::Void _bt_FFIErr_Click(System::Object^  sender, System::EventArgs^  e) 
{
  this->IEM_Display();   /* Opens up Notepad with FFI Error file */
}

private: System::Void _btProb_Click(System::Object^  sender, System::EventArgs^  e) 
{
  this->TreeGrid_SetProb(); 
}

/**********************************************************************************
* Main Wnd Emission Expaned Factor GroupBox visible changed - this means the user has
*  changed the type of emissions factors on the Setting Wnd - Default or Expanded
*  I put this in to insure that when the burnup input file option is run that 
*  that burnup is set to use the proper emission factors - 
********************************************************************************/
private: System::Void groupBox3_VisibleChanged(System::Object^  sender, System::EventArgs^  e)
{
int i; 
    i++; 
   this->Calculate("",""); 
}

/***************************************************************************
* Mortality Tree Grid - Cell Clicked 
****************************************************************************/
private: System::Void _dgMort_CellClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) 
{}
private: System::Void _dgMort_CellStateChanged(System::Object^  sender, System::Windows::Forms::DataGridViewCellStateChangedEventArgs^  e)
{}
private: System::Void _dgMort_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{}
private: System::Void _dgMort_CellLeave(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e)
{}
private: System::Void _dgMort_CellEndEdit(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) 
{}
private: System::Void _dgMort_ColumnAdded(System::Object^  sender, System::Windows::Forms::DataGridViewColumnEventArgs^  e) 
{}
private: System::Void _dgMort_SelectionChanged(System::Object^  sender, System::EventArgs^  e)
{ }
private: System::Void _dgMort_CurrentCellDirtyStateChanged(System::Object^  sender, System::EventArgs^  e) 
{}
private: System::Void _cms_AdjDead_Opening(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) 
{}
private: System::Void _mi_User_Click(System::Object^  sender, System::EventArgs^  e) 
{}
private: System::Void _txDuff_Leave(System::Object^  sender, System::EventArgs^  e)
{}
private: System::Void _tcpFuel_Click(System::Object^  sender, System::EventArgs^  e) 
{}
private: System::Void groupBox5_Enter(System::Object^  sender, System::EventArgs^  e)
{}
private: System::Void _txLitter_Leave(System::Object^  sender, System::EventArgs^  e)
{}
private: System::Void Wnd_Mai_Load(System::Object^  sender, System::EventArgs^  e)
{}
private: System::Void _lbLitter_Click(System::Object^  sender, System::EventArgs^  e) 
{}
private: System::Void _cbFFI_LoadAll_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{}
private: System::Void _cbFuelClass_Enter(System::Object^  sender, System::EventArgs^  e) 
{}
private: System::Void helpToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
{}
private: System::Void _lbHerb_Click(System::Object^  sender, System::EventArgs^  e)
{ }

/********************************************************************
* Massman Soil Report button 
*********************************************************************/
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
{
int i;
   this->_picFOFEM->Hide();  /* Hide FOFEM picture if it's still up */

   i = this->BmSoil_Frm->NumRuns(); 
   this->SoilReport_MassmanMess(i); 
       
   this->SoilReport_Massman(); 
}








}; 

// }
