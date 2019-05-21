/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fofs_sd.c
* Desc: Soil Duff Simulation
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/

/*...........................................................................*/
/*Consumed Duff Dept Limits...                                               */
#define e_DufDepLim  50.0

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* This amount will be used in Duff Sim when there has been no duff consumed */
/* so we have duff but none was consumed which is not the same as having     */
/* no duff to start with and thus running Exp Heat                           */
/* This amount is how much duff we'll say gets consumed just to get the      */
/* Duff Sim to do somthing, creating a little bit of heat for report/graph   */
#define  e_MinDufCon  0.1  /* in inches */


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/*                 SD - Soil Duff Simulation Input Parameters                */
typedef struct {
    char cr_Name[50];
/*  int  i_midburn;         /* midburn 720  1680 900 1100        1000  900  */
/*   int  i_burntime;        /* burntime 240  180  300  300        400  400  */
/*   int  i_stoptime;        /* stoptime 1200 2400 1800 1800      1500  1500 */
/*   float r_DuffDepth;      /* Duff Depth   */
/*   float r_DuffConsumed;   /* consume percent integer 0 -> 100 */
/*   float r_duffheat;       /* duff heat content - J/m3                     */
/*   int  i_duffdensity;     /* duff density - kg/m3                         */
 
    float r_bd;             /* soil bulk density - g/m3                     */
    float r_pd;             /* soil particle density - g/m3                 */
    float r_xo;             /* extrapolated water cont. at -1 J/kg          */
    float r_ls;             /* thermal conductivity of mineral fraction     */
    float r_ga;             /* de Vries shape factor                        */
    float r_xwo;            /* water content for liquid recirculation       */
    float r_cop;            /* power for recirculation function             */
    int  i_dt;             /* time step - s                                */
    float r_startwc;        /* starting soil water content - m3/m3          */
    float r_starttemp;      /* starting soil temperatue - C                 */

/* Layer and Display arrays,                                                 */
/* This get initalized before going into simulation, they tell the simulatn  */
/*  they define what the layers are and if it is to be outputed              */
/*  See where they get iniialized,                                           */
    float rr_z[e_mplus1+1];              /* Layer                             */
    float rr_node[e_mplus1+1];           /* 1 = Display, 0 = No Display       */

    float f_DufLoaPre; 
    float f_DufDepPre;
    float f_DufMoi; 
    float f_DufConPer;

/* Efficency fudge factor, used to send in a test value from GUI */ 
    float f_EffFF;                      
 
} d_SD;


int  SD_Mngr_Old (d_SD *a_SD, char cr_TmpFN[], char cr_ErrMes[]);
int  SD_Mngr_New (d_SD *a_SD, char cr_TmpFN[], char cr_ErrMes[]);

int  SD_Init (d_SD *a_SD,  d_SI *a_SI,  char cr_ErrMes[]);
int  SD_ChkSoiDuf (float f_DufDepPre, float f_DufDepPos, char cr_ErrMes[]);

float   SD_HeatAdj (float r_Post);
float  SD_Heatpercent(void);
void SD_Disp (d_SD *a_SD);
