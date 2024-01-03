extern "C" {
   /*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name:
* Desc:
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/



/* The Coastal Plain Duff(Lit) equations numbers                             */
#define e_CP_PerEq 30    /* Percent consumed equation                        */
#define e_CP_RedEq 31    /* Depth reduction equation                         */
#define e_CP_MSEEq 32    /* Mineral SoilV Exposed equation                   */



/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
typedef struct {
   float f_Per;            /* Percent Amount Consumed */ 
   int   i_PerEqu;
   float f_Red;             /* How many inches of duff get removed */
   int   i_RedEqu;
   char  cr_ErrMess[400];

   float f_MSEPer;          /* Mineral Soil Exposer */ 
   int   i_MSEEqu;
   char  cr_MSEMess[400];


/* only used for Coastal Plain cover types SAF 70,83,.. Duff Equation        */
/*  because it also calcs the consumed litter                                */
   float f_PerLit;
   int   i_LitEqu;

}  d_DUF;

int  DUF_Mngr (d_CI *a_CI, d_DUF *a_DUF);
void   DUF_Init (d_DUF *a_DUF);


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
void  Duf_Default (d_CI *a_CI, d_DUF *a_DUF);
int  DUF_Mngr (d_CI *a_CI, d_DUF *a_DUF );
int  DUF_InteriorWest  (d_CI *a_CI, d_DUF *a_DUF);
int  DUF_PacificWest  (d_CI *a_CI, d_DUF *a_DUF);
int  DUF_NorthEast (d_CI *a_CI, d_DUF *a_DUF);
int  DUF_SouthEast (d_CI *a_CI, d_DUF *a_DUF);

void  DUF_Init (d_DUF *a_DUF);
int  BUE_Duff (d_CI *a_CI, float *af_Con, float *af_Red, float *af_MSE, char cr_ErrMes[]);

int  isPonderosa(void);

int DUF_Calc (d_CI *a_CI, d_DUF *a_DUF, float *af_Con, float *af_Post,
                float *af_Percent);


int DUF_GetDepRed(d_CI *a_CI, d_DUF *a_DUF, float *af_Post, float *af_Percent);

/* Consumed percent equations - percent of duff consumed */
void Equ_1_Per   (d_CI *a_CI, d_DUF *a_DUF);
void Equ_2_Per    (d_CI *a_CI, d_DUF *a_DUF);
void Equ_3_Per    (d_CI *a_CI, d_DUF *a_DUF);
void Equ_4_Per    (d_CI *a_CI, d_DUF *a_DUF);
void Equ_16_Per   (d_CI *a_CI, d_DUF *a_DUF);
void Equ_17_Per   (d_DUF *a_DUF);

/* Depth Reduction equations - inches of duff removed  */
void Equ_5_Red    (d_CI *a_CI, d_DUF *a_DUF);
void Equ_6_Red    (d_CI *a_CI, d_DUF *a_DUF);
void Equ_7_Red    (d_CI *a_CI, d_DUF *a_DUF);

/* Consumed percent & Depth Reduction equations */
void  Equ_3_Red (d_CI *a_CI, d_DUF *a_DUF);
void  Equ_5_RedPer (d_CI *a_CI, d_DUF *a_DUF);
void  Equ_15_RedPer(d_CI *a_CI, d_DUF *a_DUF, char cr_Pine[]);
float Equ_20_PerRed_Pocosin (d_CI *a_CI, d_DUF *a_DUF);


/* Mineral Soil Exposed */
void Equ_9_MSE    (d_CI *a_CI, d_DUF *a_DUF);
void Equ_10_MSE   (d_CI *a_CI, d_DUF *a_DUF);
void Equ_11_MSE   (d_CI *a_CI, d_DUF *a_DUF);
void Equ_12_MSE   (d_CI *a_CI, d_DUF *a_DUF);
void Equ_13_MSE   (d_CI *a_CI, d_DUF *a_DUF);
void Equ_14_MSE   (d_DUF *a_DUF);
void Equ_18_MSE   (d_DUF *a_DUF);
void Equ_202_MSE  (d_DUF *a_DUF);


/* Coastal Plain shit */ 
void  Equ_CP_Per (d_CI *a_CI, d_DUF *a_DUF);
void  Equ_CP_Red (d_CI *a_CI, d_DUF *a_DUF);
void  Equ_CP_MSE  (d_DUF *a_DUF);


void Equ_19_Per (d_CI *a_CI, d_DUF *a_DUF);
void Equ_19_Red (d_CI *a_CI, d_DUF *a_DUF);
void Equ_19_MSE  (d_DUF *a_DUF);

}
