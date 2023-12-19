extern "C" {
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_mrt.h Species Master Table
* Desc:
* Date: 2/4/04
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/

#pragma once 

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/*                        Input Mortality Struct                             */
typedef struct {
     char  cr_Spe[20];                  /* Species                           */
     float f_DBH;                       /* Diameter, inches                  */
     float f_FS;                        /* Flame Lenght or Scorch Height feet*/
     char  cr_FS[10];                   /* "F" or "S" identifies f_FS field  */
     float f_Hgt;                       /* Species Height feet               */
     float f_CroRat;                    /* Crown Ration 1 -> 10              */
     char  cr_FirSev[10];               /* Fire Severity Code "Low" or ""  */
     float f_Den;                       /* Trees per acre                    */

/* Relative Humity is only needed for Eq 5 PINPAL spcificly in Coastal Plain */
/* if PINPAL is in a Coastal Plain set "Y" and give a Rel Hum or a 0         */
/* PINPAL Coastal Plain has 2 eqs one uses Rel Hum one doesn't               */
/* if PINPAL isn't in coastal plain or won't have any set to "N"             */
/* See F1 help and Robo for whats a Coastal Plain                            */
#define e_RHNonen -1.0                   /* Default/None/Don't know Rel Hum   */
#define e_RHLow   1.0                    /* Limits, 0 is to low but will alow */
#define e_RHHigh  100.0                  /* for checking a 0                  */

     float f_RelHum;                    /* Relative Humity                   */

     char  cr_CP[5];                    /* Coastal Plain - "Y" "N"           */

/* Interior West - Black Hills - Ponderosa Pine - PiPo */
     char  cr_BlkHil[5];                

   } d_MI;

// void MI_ISS (d_MI *a_MI, d_ISS *a_ISS);
void MI_SetSFCP  (float f_Sch, d_MI *a_MI);
int MIS_Totals (float *af_Den, float *af_Live, float *af_Kill, float f_DBH);
int MIS_Totals_SizCls (float *af_Den, float *af_Live, float *af_Kill, float Low, float Up);

float MIS_AvgDBHKill ();
float MIS_Killed (d_MIS *a_MIS);

int  MIS_BasalArea (float *af_Den, float *af_Live, float *af_Kill );

int  MIS_CrownCover (float *af_Den, float *af_Live, float *af_Kill, char A[] );




/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/*                        Output Mortality Struct                            */
typedef struct {

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* These values are calculated for a single Species                          */
   float f_Mort;                        /* Individ Spe Probility of Mortality*/
   float f_TotPreTree;                  /* Total Prefire Trees               */
   float f_Killed;

   float f_BasPre;                      /* Prefire Basal Area                */
   float f_BasKil;                      /* Basal Area of Killed trees        */
   float f_BasPos;                      /* Post fire Basal Area              */

   float f_CovPreLiv;                   /* Prefire Canopy Cover              */
   float f_CovPosLiv;                   /* Postfire Canopy Cover             */


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* These values are accumulated for the Stand                                */
   float f_AvgMort;                     /* Averged all Mortality             */
   float f_4AvgMort;                    /* Averged all Mortality > 4 DBH    */
   float f_TotKilled;                   /* Total Killed Trees                */
   float f_AvgDBHKilled;                /* Avg DBH of Killed Trees           */

   float f_BasTotPre;                   /* Basal Area, Prefire               */
   float f_BasTotKil;                   /* Basal Area of Killed trees        */
   float f_BasTotPos;                   /* Basal Area Post fire,(Pre - Killd)*/

   float f_CovTotPreLiv;                /* Prefire Canopy Cover              */
   float f_CovTotPosLiv;                /* Postfire Canopy Cover             */

   char cr_MortEqu[10]; 

   } d_MO;


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
#define e_Flame  "F"
#define e_Scorch "S"

/* Low and High Limits on Flame Length and Scorch Height                     */
#define e_FlaLow   1.0
#define e_FlaHig   20.0

#define e_ScoLow   1.0
#define e_ScoHig   245.0

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/

void  MRT_InitST (void);
void  MRT_Total (d_MIS *a_MIS, d_MO *a_MO, float f_Prob);
float MRT_Overlap( float f_SqFtCov);

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/*

#ifdef FOF_DLL_EXPORT
extern "C" {
   __declspec(dllexport) int  MRT_LoadSpe (char cr_Pth[], char cr_Ver[], char cr_ErrMes[]);
   __declspec(dllexport) void MO_Init (d_MO *a_MO);
   __declspec(dllexport) float MRT_Calc  (d_MI *a_MI, d_MO *a_MO, char cr_ErrMes[]);
}
#endif

#ifdef FOF_DLL_IMPORT
extern "C" {
   __declspec(dllimport)  int MRT_LoadSpe (char cr_Pth[], char cr_Ver[],char cr_ErrMes[]);
   __declspec(dllimport)  void  MO_Init (d_MO *a_MO);
   __declspec(dllimport)  float MRT_Calc  (d_MI *a_MI, d_MO *a_MO, char cr_ErrMes[]);
}
#endif
*/


int    MRT_LoadSpe (char cr_Pth[], char cr_Ver[], char cr_ErrMes[]);

int MRT_LoadSpeCSV (char cr_Pth[], char cr_ErrMes[]);
void   MO_Init (d_MO *a_MO);
float  MRT_Calc  (d_MIS *a_MIS, d_MI *a_MI, d_MO *a_MO, char cr_ErrMes[]);

float  MRT_CalcMngr (d_MIS *a_MIS, d_MO *a_MO, char cr_ErrMes[]);




int EFR_Get (char  cr_EquId[], char cr_ReqFld[]);

int SMT_MortEqu_Code (char cr_Spe[],  char cr_EquTyp[], char cr_MrtEqu[]);

int _EquTyp (char cr_MrtEqu[],char cr_EquTyp[]);
}

