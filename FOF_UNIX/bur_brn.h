extern "C" {
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: bur_brn.h
* Desc: Defines for Burnup code.
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/




/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* This is for use with the FRAMES Web fofem program                         */
/* See bur_brn.c  EFM_Open()                                                 */
#define e_FrameFN       "#FRAMES#"     /* NOTE - this is case sensitive */
#define e_Framefh       (FILE *) -1
#define e_Frame_Start   "Emis-Start"
#define e_Frame_End     "Emis-End"



/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
#define e_BurEqu 999                    /* Burnpup Equation number           */

/* Combustion Efficiencies, Flaming and Smoldering                           */
#define e_ComEffFla 0.97
#define e_ComEffSmo 0.67

/* These get used to adjust values going into the tpig & tchar arrays        */
/*  while they get loaded before burnup runs                                 */
#define e_tpig_adj    273.0
#define e_tchar_adj   273.0

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* Defines Burnup uses in 'BRN_CheckData' function to check limits           */
#define e_cht1  1000.0                       /* burnup's limit checks for    */

/*........................................................                   */
/* original  #define e_cht2  2000.0                                          */
/* ER said ok to change to 3000                                              */
#define e_cht2  3000.0                       /* 'cheat' arrary               */


#define e_tig1  200.0
#define e_tig2  400.0

#define e_tch1  250.0
#define e_tch2  500.0

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* Change 7-18-12 - change lower limit, a user was using lower limits - 
*                  I tried some test with lowering the limit and it seemed 
*                  to be OK, not sure how/who came up with the original
*                  lower limit
* NOTE see fof_ci.h  e_DufMin - both need to match */
 #define e_wdf1 0.022   // this is kg/m2 - and equal to 0.1 tons per acre 
/*  #define  e_wdf1 (double) 0.1  */                 /* Orig - duff loading limits, kg/m2 */


/* #define  wdf2 (double) 30.0    Original Amount */
/* changed 12/28/01, they wanted higher, some cover types have lots of duf   */
/*  80 kg/m2 is about 357 tons per acre */
#define  e_wdf2 (double) 80.0                  /* kg/m2                       */


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
#define  e_dfm1  (double) 0.1                  /* Duff Moisture limits        */
#define  e_dfm2  (double) 1.972

#define  e_fms1  (double) 0.01                 /* Moisture Limits             */
#define  e_fms2  (double) 3.0

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* These are the actual Fuel Load Limits the Burnup calc uses                */
/* #define  small (double) 1.e-06                                           */
#define  e_small (double) 1.e-08               /* load limits                 */
#define  e_big   (double) 1.e+06

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* Defualt settings for Burnup input parameters, like the ones used when     */
/* Building an input file for burnup to use                                  */
#define  e_MAX_TIMES       3000
#define  e_INTENSITY       50.00
#define  e_IG_TIME         60.00
#define  e_WINDSPEED        0.00
#define  e_DEPTH            0.3
#define  e_AMBIENT_TEMP    27.00
#define  e_R0               1.83
#define  e_DR               0.40
#define  e_TIMESTEP        15.00

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/*                   Sigmas input parameters used by burnup                  */
#define  e_SURat_Lit       8200.0
#define  e_SURat_DW1       1480.0
#define  e_SURat_DW10       394.0
#define  e_SURat_DW100      105.0
#define  e_SURat_DWk_3_6     39.4
#define  e_SURat_DWk_6_9     21.9
#define  e_SURat_DWk_9_20    12.7
#define  e_SURat_DWk_20      5.91


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* These get loaded into burnup arrays along with the individual fuel loads  */
/*  before runing Burnup                                                     */
/* below we have #defines for running Burnup with and without input file     */
/* ***>>>   NOTE READ this                                                   */
/* NOTE READ                                                                 */
/* If you change any of these you got change them in both places, in the     */
/*  printf format statement and the individual #defines below                */

/* printf format statements for creating burnup input file                   */
#define  e_SoundFmt  "1 %2d %13.8f  18600.0  %5.3f  513.0  %7.2f 2750.0  0.133 327.0 377.0 0.05\n"
#define  e_RottenFmt "1 %2d %13.8f  18600.0  %5.3f  224.0  %7.2f 2750.0  0.133 302.0 377.0 0.05\n"


/* Defines sent into Burnup directly, when NOT using an input file           */
/* **>> NOTE READ explaination above, before CHANGING any of these           */
#define e_htval 18600.0          /* low heat of combustion , J / kg          */
#define e_Snd_dendry  513.0      /* ovendry mass density, kg/cum Sound       */
#define e_Rot_dendry  224.0      /* ovendry mass density, kg/cum Rotten      */
#define e_cheat       2750.0     /* specific heat capacity (J / K) / kg dry mass                   */
#define e_condry      0.133      /* thermal conductivity W / m  K , ovendry  */
#define e_Snd_tpig    327.0      /* ignition temperature , K   Sound         */
#define e_Rot_tpig    302.0      /* ignition temperature , K   Rotten        */
#define e_tchar       377.0      /* char temperature , K                     */
#define e_ash         0.05       /* mineral content , fraction dry mass      */


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
#define false 0
#define true  1
#define bool  int

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/

/* This was orginally 150, this is used to set array sizies                  */
/* #define MAXNO  150  */              /* Max # of Fuel Loadings allowed    */
#ifdef DOS
  #define MAXNO   12                   /* Max # of Fuel Loadings allowed    */
#else
  #define MAXNO   20                     /* Max # of Fuel Loadings allowed    */
#endif

#define MAXTYPES 3
#define MAXKL  (MAXNO * ( MAXNO + 1 ) / 2 + MAXNO )
#define MXSTEP 20

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* This struct is used to check data in the input Burnup loading file        */
typedef struct {
     int i_MAX_TIMES;
     int i_INTENSITY;
     int i_IG_TIME;
     int i_WINDSPEED;
     int i_DEPTH;
     int i_AMBIENT_TEMP;
     int i_r0;
     int i_dr;
     int i_TIMESTEP;
     int i_DUFF_LOAD;
     int i_DUFF_MOIST;
  } d_CFF;

void   CFF_Init    (d_CFF *a_CFF);
int    CFF_ChkAll  (d_CFF  *a_CFF);



/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/



int   BRN_Run_UIF (char cr_InFN[], char cr_Err[], float f_DufConPerCent,
                    char cr_LoadFN[], char cr_HeatFN[], float f_BrnCon);
int   BRN_Run (char cr_LoadFN[], char cr_HeatFN[], float f_DufConPerCent, 
               float f_ConHerb, float f_ConShrb, float f_ConBraFol, char cr_ErrMes[]);

void  BRN_Init(void);

int   BRN_ReadInFil (char cr_FN[], char cr_Err[]);
int   BRN_CheckData (char cr_ErrMes[]);
void  BRN_SetFireDat (long NumIter, double Fi, double Ti, double U, double D, double Tamb,
                  double R0, double Dr, double Dt, double Wdf, double Dfm);
int   BRN_ChkSwi  (char cr[]);
float BRN_Intensity (float f_Con, float f_Seconds );
int BRN_SetFuel (int *aiX, char cr_SR[], float f_Load, float f_Moist, float f_Sigma);




int     BurnupNone (char cr_HeatFN[], float f_Con );
void    Heat_Heading (FILE  *fh);
void    Heat_HeadingFS (FILE  *fh);
void    Arrays (void);
long    loc (long k, long l);
double  func (double h, double theta);
double  ff (double x, double tpfi, double tpig);
int     Start(double tis, long now, long *ncalls, double *ad_Con);
void    OverLaps(void);
double  FireIntensity(double *ad_pcSmoCon);
double  DryTime(double enu, double theta);
int     Stash(char *HistFile,  double tis, long now);
void    Sorter(void);
double  TIgnite(double tpdr, double tpig, double tpfi, double cond, double chtd, double fmof, double dend, double hbar);
double  TempF(double q, double r);
void    HeatExchange (double dia, double tf, double ts, double *hfm, double *hbar, double cond, double *en);
void    DuffBurn (double wdf, double dfm, double *dfi, double *tdf,
                  float f_DufConPerCent, double *ad_Duf_CPTS);
void    Step (double dt, double tin, double fid, long *ncalls, double *ad_Con);
int     Summary (double d_Time, char *OutFile,char cr_ErrMes[]);
long    Nint(double input);
void    CalculateEmissions(int i_Fst);


double  pow2(double input);

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/*                           Emission Sturcture                              */
typedef  struct {
   double d_FlaCon;                    /* Store consumed for time step      */
   double d_SmoCon;

   double dN_FlaCon;                   /* Store consumed totals             */
   double dN_SmoCon;

   double d_PM25;                      /* single time step amounts           */
   double d_PM10;
   double d_CH4;
   double d_CO2;
   double d_CO;
   double d_NOX;
   double d_SO2;

/* The burnup ES_Calc() default emission calc function will use this */
   double d_pcSmo;    /* percent of fuel consumed in smoldering  */  

/*..........................................................................*/
/* Running Totals */
   double dN_PM25F;                    /* Flame Totals                       */
   double dN_CH4F ;
   double dN_COF  ;
   double dN_CO2F ;
   double dN_PM10F;
   double dN_NOXF;
   double dN_SO2F;

   double dN_PM25S;                    /* Smoldering Totals                  */
   double dN_CH4S ;
   double dN_COS  ;
   double dN_CO2S ;
   double dN_PM10S;
   double dN_NOXS;
   double dN_SO2S;

   double dN_PM25S_Duff;               /* Smoldering Duff - Only               */
   double dN_CH4S_Duff ;               /* NOTE see Smoldering above*/
   double dN_COS_Duff  ;
   double dN_CO2S_Duff ;
   double dN_PM10S_Duff;
   double dN_NOXS_Duff;
   double dN_SO2S_Duff;

   double  dN_Cnt;     /* # values put into the Running Totals */

/*..........................................................................*/
/* amounts at each time step */
   double d_PM25F;                    /* Flame Totals                       */
   double d_CH4F ;
   double d_COF  ;
   double d_CO2F ;
   double d_PM10F;
   double d_NOXF;
   double d_SO2F;

   double d_PM25S;                    /* Smoldering Totals                  */
   double d_CH4S ;                    /* NOTE THIS INCLUDES DUFF */
   double d_COS  ;
   double d_CO2S ;
   double d_PM10S;
   double d_NOXS;
   double d_SO2S;

   double d_PM25S_Duff;               /* Smoldering Duff - Only               */
   double d_CH4S_Duff ;               /* NOTE see Smoldering above*/
   double d_COS_Duff  ;
   double d_CO2S_Duff ;
   double d_PM10S_Duff;
   double d_NOXS_Duff;
   double d_SO2S_Duff;


   double d_FlaDur;                     /* Duration, last time step that     */
   double d_SmoDur;                     /* consumed something                */
   double d_DuffCon;
   double d_HSFBCon; 

/* Consumer Fuel Amounts per time step, these correspond to the Emission Factor Groups */
 //  double d_FlameCon;                               
//   double d_SmolderCon;

//   double d_HSFBCon; 

   /* Final Totals */
   double dN_FlameCon;                               
   double dN_SmolderCon;
   double dN_DuffCon;
   double dN_HSFBCon; 


/* Remaining Fuel Load - at this time step */
   double d_rm_Lit;
   double d_rm_1Hr;
   double d_rm_10Hr;
   double d_rm_100Hr;
   double d_rm_1kHr;

/* Consumed Fuel Load - a this time step */
   double d_cn_Lit;
   double d_cn_1Hr;
   double d_cn_10Hr;
   double d_cn_100Hr;
   double d_cn_1kHr;         /* Flame + Smolder consumed */

/* percent of the above Consumed Fuel for this time step that was smodering */
/* Lit, 1Hr, 10Hr, 100Hr  will always be 100 percent smolder or 0 for flame */
/* 1kHr can be 0 -> 100 because 1kHr has sub compoents 3,6,9,20 - Snd * Rot */
/*   so some may go up in flame and other in smolder  */
   double d_cn_LitSmoPerCent;
   double d_cn_1HrSmoPerCent;
   double d_cn_10HrSmoPerCent;
   double d_cn_100HrSmoPerCent;   
   double d_cn_1kSmoPerCent;    /* percent of 1k that was smoldeing */ 
   double d_cn_SmoConPC;        /* all lit wood consumend in smolder */

   double d_cn_1kHrFla;     
   double d_cn_1kHrSmo; 

/* Emission Factors */ 
double  d_pm10f, d_pm10s; 
double  d_pm25f, d_pm25s;
double  d_ch4f, d_ch4s;
double  d_cof, d_cos;
double  d_co2f,  d_co2s;
double  d_noxf,  d_noxs; 
double  d_so2f, d_so2s;

 } d_ES;

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
int  ES_Calc (d_ES *a_ES, double d_WooLit,  double d_Duff, double d_HSFB,
               double d_pcSmo, double d_time);

void  ES_Init (d_ES *a_ES);
float ES_FlaDur (void);
float ES_SmoDur (void);
float ES_PM25F(void);
float ES_PM10F(void);
float ES_CH4F (void);
float ES_COF  (void);
float ES_CO2F (void);
float ES_NOXF (void);
float ES_SO2F (void);


float ES_PM25S(void);
float ES_PM10S(void);
float ES_CH4S (void);
float ES_COS  (void);
float ES_CO2S (void);
float ES_NOXS (void);
float ES_SO2S (void);


float ES_PM25S_Duff(void);
float ES_PM10S_Duff(void);
float ES_CH4S_Duff (void);
float ES_COS_Duff  (void);
float ES_CO2S_Duff (void);
float ES_NOXS_Duff (void);
float ES_SO2S_Duff (void);

void ES_GetEmi (char cr[], double *flame, double *smolder); 


float ES_FlaCon (void);
float ES_SmoCon (void);

void   Save_SGV (d_ES *a_ES, double d_time, double d_FirInt);
double Get_Cons (double d_old, double d_new);
double Duff_CPTS (double *ad_Duf_Tot, double d_Duf_Sec, double d_NumSec);
int    Bur_ChkArgs (char cr_Line[]);
void   Bur_Error   (char cr_Err[], char cr1[], char cr_Line[]);
int    EFM_Write  (d_ES  *a_ES, double d_Time, double d_FirInt);
int    EFM_Open   (char cr_FN[]);
void   EFM_Close  (void);
void   Bur_RemoveTmp (void);
void   Bur_SumDivErr (void);
void  bstrupr (char cr[]);

void BRN_SetEmis (float f_CriInt,
   float f_fCO, float f_fCO2, float f_fCH4, float f_fPM25, float f_fPM10, float f_fNOX, float f_fSO2,
   float f_sCO, float f_sCO2, float f_sCH4, float f_sPM25, float f_sPM10, float f_sNOX, float f_sSO2,
   float f_dCO, float f_dCO2, float f_dCH4, float f_dPM25, float f_dPM10, float f_dNOX, float f_dSO2);

double FIC_Percent ();
void FIC_Init ();
double FIC_SmoPercent (double d_SURat);
double FIC_1kSmoPercent ();
bool _isEqual (double a, double b);
void FIC_Put (double d_SURat, double d_dendry, double term, double test, double amount, char cr_FS[]);
void _CompDump (float test, float cmpark, float term, char FS[], float wnoduff, double sigma);

int ES_FlaSmo (d_ES *a, float *af_Smo, float *af_Fla, float *af_1kSmo, float *af_1kFla);

double  Min (double a, double b);
double  Max (double a, double b);

int _ChkLimM (double d, double low, double hi);
int  ES_Calc_NEW (d_ES *a_ES, double d_WooLit,  double d_Duff, double d_HSFB, double d_time, float f_FirInt);
double  ES_SetComponents (int iS_Init, float *af_TPA, double d_Time, d_ES *a_ES);
int ES_SetComp (int i, double d_Rem, double d_Con, d_ES *a_ES );

}