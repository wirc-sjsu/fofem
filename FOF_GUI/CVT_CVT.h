/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Wnd_SAF.h
* Desc:
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
#pragma once 

#define eC_CVT 1500

/* Indexes used to determine the region of a a fuel class entry in CVT table  */
#define e_Reg_IntWest   0 
#define e_Reg_PacWest   1
#define e_Reg_Northeast 2
#define e_Reg_Southeast 3

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/*                       Cover Fuel Loading Struct                           */
typedef struct {

   float f_litter;                      /* Litter                            */
   float f_DW0;                         /* Down Wood 0 -> 1/4   1 Hour       */
   float f_DW10;                        /* Down Wood 1/4 -> 1, 10 Hour       */
   float f_DW100;                       /* Down Wood 1 -> 3,  100 Hour       */
   float f_DW1000;                      /* Down Wood 3 ->,   1000 Hour       */

   float f_39s;
   float f_920s;
   float f_20ps;

   float f_39r;
   float f_920r;
   float f_20pr;

   float f_pcRot;                     /* percent of rotten */

/* FFI 3+ size classes, input file has individual class loads for sound & rot */
   float f_Snd3, f_Snd6, f_Snd9, f_Snd12, f_Snd20;
   float f_Rot3, f_Rot6, f_Rot9, f_Rot12, f_Rot20;

/*---------------------*/

   float f_MeaDia;                      /* Mean Dia                          */
   float f_Duff;                        /* Duff                              */
   float f_DufDep;                      /* Duff Depth                        */

#define  eC_CFLCo  5
   char  cr_HvCod[eC_CFLCo];           /* Harvesting Debris Code             */

   float f_Herb;
   char  cr_Herb[eC_CFLCo];
   float f_Shrub;
   char  cr_Shrub[eC_CFLCo];
   float f_CroFol;
   char  cr_CroFol[eC_CFLCo];
   float f_CroBra;
   char  cr_CroBra[eC_CFLCo];

   char  cr_FLMSoil[40];               /* FLM defualt soil type from .dat fil*/
   char  cr_FLMLogDist[40];            /* FLM defulat log dist from .dat fil */

#define eC_CFLCom 50
   char cr_Comment[ eC_CFLCom + 1];

/* Emission Group factors */
   char cr_EmiGrpFla[20];  /* Flaming, all nonduff */
   char cr_EmiGrpDuf[20];  /* Duff smolder RSC */
   char cr_EmiGrpS1k[20];  /* 1k wood, smodlerin */


} d_CFL;


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/*                        Cover Type Table                                   */
typedef struct  {
   int  i_Num;

/* Change 7-27-06..........................                                  */
/* #define  e_CT  eC_WndTxt */      /* Make this to Max size of Child Window Txt  */
/* using eC_WndTxt, was getting in the way when doing DEQ Pre-fuel load stuff*/
/* Just need to make sure e_CT is defined larger than eC_WndTxt and we're ok */

#define  e_CT  300
  char cr_CovTyp[e_CT + 1];

  int  ir_Region [4];

#define  e_FueRef 40
   char cr_FueRef[e_FueRef + 1];
   char cr_AddRef[e_FueRef + 1];

/*...........................................................................*/
/* The Cover Group Code that are used in the species cover input .dat files  */

#define e_CVT_ShrubGroup "SG"
#define e_CVT_SageBrush  "SB"
#define e_CVT_GrassGroup "GG"
#define e_CVT_BalBRWSpr  "BBS"
#define e_CVT_Ponderosa  "PN"
#define e_CVT_Pocosin    "PC"
#define e_CVT_WhiPinHem  "WPH"
#define e_CVT_RedJacPin  "RJP"
#define e_CVT_CoastPlain "CP"      /* Coastal Plain */

#define eC_CvGrp 10            /* Cover Group code, GrassGroup, Ponderso,etc */
   char cr_CovGrp[eC_CvGrp+1];

#define e_FCC_Natural   "N"
#define e_FCC_Piles     "P"
#define e_FCC_Slash     "S"
   char cr_FCCFuelCat[10];   /* FCC Fuel Category - Nature,Piles,Slash */

   d_CFL s_CFL;




}  d_CVT;

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
void  SAF_InitTable ();
void  SAF_Init (d_CFL *a_CFL);
void  Trim_CovTyp (char cr[]);

int   SAF_isReg (int i_Reg);


int SAF_LoadFileNew (char cr_FN[], char cr_ErrMes[]);
int SAF_LoadFile  ( char cr_FN[], char cr_ErrMes[]);

int SAF_GetRefer (char cr_CovTyp[], char cr_FueRef[], char cr_AddRef[]);

int SAF_WeightDist (d_CI *a_CI, char cr_WD[], float f_Load, float f_pc) ;


int NVCS_LoadFileNew (char cr_FN[], char cr_ErrMes[]);
int NVCS_LoadFile  (char cr_FN[], char cr_ErrMes[]);
int FCC_LoadFileNew  (char cr_FN[], char cr_ErrMes[]);


int   CVT_GetCovGrp (char cr_CovTyp[], char cr_CovGrp[]);
float CVT_pcRot ( char cr_CovTyp[]);
void  CVT_Dummy(int iX);
int   CVT_GetCovReg (int iX, int iX_Reg, char cr[]); 
int   CVT_GetCFL ( char cr_CovTyp[], d_CFL *a_CFL, char cr_CovGrp[]);
void WD_FCCS_MessOn ();
void WD_FCCS_MessOff ();