#pragma once 
#include <float.h> 

/* Fire Types - used on GUI combobox */
#define e_FT_Wild "Wild Fire"
#define e_FT_Pres "Prescribed Burn"
#define e_FT_Pile "Pile Burn"
#define e_FT_Burnup  "Burnup Model"
#define e_FT_Test "Test" 

#define eC_SwQQ 1001

#define e_JmpExp 1   /* use for doing longjmp() function */

/*------------------------------------------*/
/*     BM inputs  */
typedef struct {

    float f_Moist;        /* Moisture */ 
    float f_SoiBulDen;   /* Soil Bulk Density - program variable = den0, g/cm3 */ 
    float f_SoiParDen;   /* Soil Particle Density - program variable = parden, g/cm3 */ 

    float f_AmbAirTmp;    /* Ambient Air Temperature */

    float f_Qabs;        /* Watts feed into model */ 

    double d_SimTime;     /* Simulation Time Minutes  */ 

    float  f_BurnTime;   /* Burn time Minutes */ 
    float  f_MaxWatTim;  /* Time (minutes) that max watts is applied in heat curve */

    char   cr_TemMoi[20];           /* "Temp" or "Moist" - graph temperature or moisture - radio buttons */ 
    char cr_ErrMes[3000]; 
 
    char  cr_FirTyp[30];    /* Fire Type */

 } d_BMI;


void BMI_Init (d_BMI *b);

/* Max Burntime - Fire Duration */
#define e_MaxBurnTime  100.0
#define e_MinBurnTime  0.25

/* Maximum number of hours to run a simulation */ 
#define e_MaxHourSim 120  //  120

/* Time To Maximum fire intensity -  */
#define e_Max_MaxWatTim 4.0  /* upper limit */
#define e_Min_MaxWatTim  0.05 /* Lower Limit */

/* Moisture limits */
#define e_MaxMoist 0.25
#define e_MinMoist 0.01

/* Soil Bulk Density Limits */
#define e_MaxSoiBul 1.8
#define e_MinSoiBul 0.70

/* Soil Particle Density Limits */
#define e_MaxSoiPar 2.9
#define e_MinSoiPar 2.3


/* CHANGE - 9-6-2018 Bill email to change to 3.0 */
#define e_delt  3.0    /* orig  1.2 time increments */

/* Maximum number of time (nsteps) steps we'll allow */
/* The Massman simulation array sizes are set to this */
/* See Model_Params.cpp where nsteps is calcuated     */
/* nsteps is the number of time steps the model will use */
/* eC_nsteps is the array size */
/* This should cover 100 hours */
#define  SECS  ( e_MaxHourSim * 60 * 60 )
#define eC_nsteps (int) ( SECS / e_delt ) + 100


/* Half Width used to calc watt curve in BoundaryUBFD()*/
#define  e_tQmax   1000.0 
#define  e_BurnTime  1.5   /* hours */ 
#define  e_MaxTim    0.6   /* Hours, time that max watts are applied to heat curve */

/* Watts w/m2 used BoundaryUBFD(), for intial fire intensity */
#define  e_Qabs  31.0 

/*----------------------------------------------------*/
/* Depth Stuff */ 
/* CHANGE - 9-6-2018 Bill email to change to .6 and .002 */
#define e_Depth  0.60  // orig -> 0.20   // Model_Param.m
/* Soil Depth is divided by this amount to get the */
/*  the number of layers/levels */
#define e_delz 0.002   // orig --> 0.001  // Model_Param.m


/*----------------------------------------------------*/
/* Soil Stuff  */ 
/* New values from BM 11-2-18 */
#define e_SoiWatPot 1.2845e-04    /* normalized soil water potential */
#define e_SoiBulDen 1.25     //  1.56         /* Soil Bulk Density - mega grams */ 
#define e_SoiParDen 2.65     //  2.82         /* Soil Particle Density - mega grams */
#define e_SoiVolMoist  0.1                 /* Volumetric Soil Moisture */

/* Maxium number of Layers Allowed - does not control how many just used to check and  */
/*  Changed when e_Depth was changed (see above) caused array to over-run  */
#define eC_MaxLev 500  // orig 250

#define eC_Mx 500      // orig 220 

#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

#define e_pi 3.14159265358979  /* Use same pi value that is in Matlab */

double  xlog (double f) ;
double  xZero (char cr[]) ; 
double xexp (double f);
double xsqrt (double f ) ;
double xnthroot (double f, double to ) ;
double nthroot (double f, double g);
double xpow (double base, double to);

int HMV_Model(d_BMI *bmi, char cr_Mode[]);
void HMV_Model_Thread(void *param); 

void Physical_Constants_HMV();
void  Water_Vapor_Constants_0(); 
void Water_Vapor_Constants_2();
void Water_Vapor_Constants ();
void  densatmaxHMV( double tempk,   double *esat, double *ss, double *csat, double  *dcsatdT);

void  Liquid_Water_Constants();
void Liquid_Water_Constants_2();
void  calmaxmulaHMV  (double TMax, double *muvx, double *muvTx, double *lamvx, double *lavTx, double *temRx );
double Vmult5 (double a1, double a2, double a3, double a4, double a5, double a6, double temR);
void calmaxdenwHMV (double TMax, double *denw, double *denwi, double *denwN, double *denwdT );
void calmaxmulaWHMV (double TMax, double denwNx, double temRx, double muvx, double muvTx, double lamvx, double lavTx, double denix, double denwdTx, 
                     double *muwx, double *muwTx, double *dmTmx, double *zlmwx, double *lawTx ); 
void  Dry_Air_Constants_HMV(); 
void Model_Switch_HMV ();
void PsinTmpnT (double tempai);
int Soil_Time_Depth_Param_HMV(d_BMI *bmi);
int Soil_Model_Data_Files_HMV(d_BMI *bmi); 
int Quincy1G(d_BMI *bmi);
int Model_Param(d_BMI *bmi); 
int Vec_Mult (double A[], double B[], double C[], double N);

void   LoadQuincy1();

void BoundaryU (d_BMI *bmi);

void BoundaryUBFD (d_BMI *bmi);

void  BoundaryLHB17dBFD ( d_BMI *bmi); 

void BoundaryIR();


void caleta4 (double *bceta4, double *bcta, double *bcva, double N);

int SolveHMV (char cr_ErrMes[]);

int CrankNicolson (int i_TimStp, char cr_ErrMes[]);
int  Crank_Nicolson (int jstep, char cr_ErrMes[]) ;

void calprofP (double *den, double *xm, double *poros, double *temp, double *theta, double *cds, 
                double den0, double tempi, double thetai, double pard, double nlev);

void calConCoef2(double *Rcoef, double *dRcdT, double *dRcdp, double psin, double temp[], double nlevel );
void calConCoef2_Ary(double *Rcoef, double *dRcdT, double *dRcdp, double *psin, double *temp, double nlevel );

void calpsinProf (double psin[], double psini, double nlev);

void calthetaCSr(double *theta, double thpsin[], double *thpsini, double *thtemp, 
                 double *rfac,  double *drfacT, double *drfacp,  double *psin, double nlevel);

void calparx(double *thpx, double *thetax, double *apsi4, double *thznr, double *psiX, double nlevel );

void  caltempkHMV( double *tempk, double *tempki, double *tempr, double *temp, double nlevel);

void rhoveqHMV (double *rhoveq, double *tempk, double *psin, double nlevel);

void calvaporHMV (double *vapres, double *rhov, double *tempk, double nlevel );

void calcsHMV(double *cs, double *csi, double *csd, double *den, double *temp, double *theta, double nlevel );
void caldryvis (double *mud, double *tempk, double nlevel) ;

void  calmulaHMV(double *TempR, double *temR, double *muv, double *muvT, double *lamv, double *lavT,  double *tempk, double *tempki, double nlevel );

void calden2HMV(bool *LGL, double *denw, double *denwi, double *denwdT, double *denwN, double *TempR, double *tempk, double nlev, int i_Step );


void  calmulaWHMV( 
    double *muw, double muwT[], double dmTm[], double lamw[], double lawT[], 
    double tempki[], bool LGL[], double denwN[], double temR[], 
    double muv[], double muvT[], double lamv[], double lavT[],
    double denwi[], double denwdT[], double nlev );

void calxhiv1(double xhiv1[], double dxhivdT[], double dxhivdr[], double vapres[], double tempki[], double rhov[], double nlevel );
void  calcondry( double zlamda[], double zlamdT[], double tempk[], double tempki[], double nlevel );
void calgascomb(double  mois[], double vapor[], double dry[], double mvapor[],  double mdry[], double xhiv[], double nlevel );

void densatHMV(double esat[], double ss[], double csat[], double dcsatdT[], double tempk[],  double tempki[], double nlevel );


void calstefan1NR(double stefan[], double stefpT[], double stefpr[],      
                  double vapres[], double tempki[], double rhov[], double nlevel );


void  caldiffHMVNRa( double diffa[], double diffH[], double diffs[], double difsdT[], double difsdr[], 
                     double  xhiv[], double dxhivdT[], double dxhivdr[], double tempki[], double tempr[], 
                     double stef[], double stefpT[], double stefpr[], double nlevel);   



void calconHMVl( double *tcon, double *xa, double *fw, double *hv, double *dhvdT, double *ka, 
double *TempR, double *temR, double *tempk, double *tempki, 
double *diff, double *zlamda, double *zlamw, double *ss,
double  *xm, double *poros, double *theta, double *psin, double nlev );


void calconHMVll  (
   double tcon[], double xa[], double fw[], double hv[], double dhvdT[], double ka[], double zlamv[],
   double TempR[], double temR[], double tempk[], double tempki[], double temp[], 
   double diff[], double zlamda[], double zlamw[], double ss[],
   double xm[], double poros[], double theta[], double psin[], double nlev );





void calconRAD (double *tcon, double *tconR,  double *poros, double *theta, double *tempk, double *tcon1, double nlevel );
void vapdiffEHMV (double *enh, double *vdiff, double *lv, 
                  double *diffs, double *xa, double *poros, 
                  double *fw,  double *ka, double *hv, double nlevel );

void vaporTempdiff  (double *DiffT, double *vdiff, double *tempki, double nlevel );

void  calcsHMVnT (double *denwnT, double *csx, double *csxi,
                  double *cs, double *denw, double *lv, double *thtemp, double nlevel );

void calepssurfHMV (double *sdialc, double *ddiaT, double *stensn, double *dstensdT, 
                    double *tempk, double *tempki, double *TempR, double nlev );

int   LMS_Open (char cr_FN[]);
void LMS_Close ();
int LMS_Mess (char A[], char B[]);


void calhydrauKF( double *Knf, double *KHf, double *dKnfdT, double *dKnfdp, double *dKHfdT, double *dKHfdp,
                  double *por, double *tempk, double *tempki, double *psin,
                  double *denw, double *muw, double *dmTm, double *sdialc, double *ddiaT, 
                  double *stensn, double *dstensdT, double nlev );


void calSw(double *Sw, int *LGLth,  double *theta, double *poros, double nlev );


void calcp0HMV (double *cp0, double *dcpT,
                double x10[], double sqtrm, double sqqtr,
                double sqtri, double sqqti, double tempr );


void calcpaHMVNR (double *rhcpv, double *rhcpa, double *drhcaT, double *drhcar, 
                  double *tempki, double *tempr, double *rhov, double nlevel );

void calhydrauVA(double *Kn, double *KH, double *dKHdT, double *dKHdt, double *dKHdp,
                 double *Rcoef, double *muw, double *dmTm, double *denw, double *Sw,
                 double *poros, double *thpsin, double nlevel );

void  calsurfdT (double *Kd, double *Km, 
                 double *tempr, double *theta, double *Sw,
                 double *por, double *thpsin, double *thtemp, double nlevel); 

void calrhev(double *rh, double *rhove, double *drevdp, double *drevdT,
             double *tempki, double *csat, double *dcsatT, double *psin, double nlevel);

void  calAwa( double *Awa, double *dAwadt, double *dAwadp,
              double *Sw,  double *por,    double *thpsin,
              int *LGL, double nlevel );

void  calAwaP( double Awa[], double dAwadt[], double dAwadp[],
              double Sw[],  double por[],    double thpsin[],
              int LGL[], double nlevel );


void  calConCoef5 (double *Concoef, double *dCondT, double *dCondp,
                   double *psin, double *tempki,double nlevel );



void  calVsourceGNRa1( double *sourcevx, double *dSvdTx, double *dSvdpx, double *dSvdrx, double *Srhov, 
                      double *Awa, double *dAwadt, double *dAwadp, double *thtemp, double *rhove, double *rhov, double *Concoef, 
                      double *dCondT, double *dCondp, double *drevdT, double *drevdp, double *tempk, double *tempki, double nlevel );


void caluHMV(double *u, double *delu, double *advdif, double *adtcon,
             double *xa, double difcoef, double delz, double *sourcev, double *rhov,
             double *rhcpa, double NLEV, double NL );

void harmean(double *hmean, double co, double *A,  double nlev );


void  EBcoef2(double *_emis0, double *_IR, double *_CHp, double *_RH, double *_IRl, double *_CEp, double *_RE, 
    double theta0, double bcra, double bct, double Htrans, double tmp, double tmpk, double tmpki,
    double forIR, double eta40, double rhov0, double CE, double rh0, double CU, double u0, double lv0);

// old 
// void  EBcoef(double *_emis0, double *_IR, double *_CHp, double *_RH, double *_IRl, double *_CEp, double *_RE, 
//             double theta0,double bct, double *eta, double Htrans, double tmp, double tmpk,
//             double tmpki, double CE, double rh0, double CU, double u0, double lv0, double xa0 );




void  caldiseq ( double *diseq,  double *rhove, double *rhov, double nlevel );



void  calDENVEL(double *uu1, double *deldifu, double *Lambdau,
                double *Rcoef, double *Sw, double *por, double *theta,
                double *mug, double *vapres, double *rhcpa, double nlevel );

void  caldelHMV( double *u, double *delu, double *advdif, double *adtcon, 
                 double *uu1, double *Lambdau, double delz, double delvel, 
                 double *rhov, double bcra, double rh0, double CE,
                 double *vdiff, double nlev, double nl );


void calQHCN (double *QH, double bcQ, double bct, double bcr,
              double emis0, double CHp, double CEp, double IR, double IRl );


void  xNew_Init ();
double *xNew (int i_Size, char cr_Id[], char cr_Grp[] );
int xDelete (double *a);
int xCheck ();
int xDelGrp (char cr_Grp[]);


void calEBCN (double *_Radiant, double *_Convec, double *_LE, double *_G0, 
              double bcQ,   double bct, double bcr,
              double emis0, double CHp, double CEp,
              double RE,    double IR,  double rhov0,
              double tmp );


int ACS_Check (double fr[], char cr_Id[]);

void ACS_Init (double f_Size);






int  AccumulateHMV (int i_TimStp);
int  AFT_Init(d_BMI *bmi);
int  AFT_Close(); 



int  calhydrauWRC (double Kn[], double KH[], double dKHdT[], double dKHdt[], double dKHdp[],
               double psin[], double muw[], double dmTm[], double denw[], double nlevel); 
;

void calpsinProf (double *psin, double psiniFY, double nlevel);

int calthetaFYr( double *theta, double *thpsin,  double *thpsini, double *thtemp,
                 double *rfac, double *drfacT, double *drfacp, double *psin, double *poros,
                 double nlevel );

double PSINi (double theta, double a0,double a[], double b[], double w);

int WesternUS01 (d_BMI *bmi);

void BoundarydBFD (d_BMI *bmi);

void BoundaryIR_T1 (double  force[]);