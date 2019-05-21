/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_cvt.h
* Desc:
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/

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
// test fccs

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
} d_CFL;


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/*                        Cover Type Table                                   */
typedef struct  {
   int  i_Num;

/* Change 7-27-06..........................                                  */
/* #define  e_CT  eC_WndTxt */      /* Make this to Max size of Child Window Txt  */
/* using eC_WndTxt, was getting in the way when doing DEQ Pre-fuel load stuff*/
/* Just need to make sure e_CT is defined larger than eC_WndTxt and we're ok */

#define  e_CT  200
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


int CVT_GetCovTyp(int iX, int iX_Reg, char cr[]); 
int   CVT_GetActRec (d_CFL *a_CFL, char cr_CovTyp[]);

// the follow are the old one, 


int   CVT_GetActRec (d_CFL *a_CFL);
int   CVT_LoadExec     (char cr_Version[]);
int   CVT_Load         ( char cr_FN[], char cr_ErrMes[]);
int   CVT_LoadFCC      (char cr_Version[], char cr_FN[]);
void  CVT_InitTable    (void);
int   CVT_GetName      (int iX, char cr_CovTyp[]);
int   CVT_ChkReg       (int iX, int i_Reg);
void  Trim_CovTyp      (char cr[]);
int   CVT_CovTypNum    (void);
int   CVT_ChkVer       (char cr_Line[], char cr_Version[]);
int   CVT_isReg        (int i_Reg);
void  CVT_FCC_Dummy    (int iX);
int   CVT_isNaturalFCC (void);
int   CVT_isPileFCC    (void);
int   CVT_isSlashFCC   (void);
void  Add_Sep          (char cr_Cov[], char cr[]);
float CVT_DDRatio      (void);
void  CVT_Display      (void);
void  CFL_Init         (d_CFL *a_CFL);
void  CVT_DupCovTyp    (void);
int   CVT_FCCPerCent   (float *af_9_20pc, float *af_20pc);
void  CVT_FuelRef (char cr_FueRef[],  char cr_AddRef[]);

void  CVT_GetCoverGroup (char cr_CovGrp[]);
int   CVT_isPocosin (void);
int CVT_LoadFLM (char cr_Version[], char cr_FN[]);
void CVT_FLM_Dummy(int iX);
void  CVT_FCC_FuelCat (char cr_FC[]);
