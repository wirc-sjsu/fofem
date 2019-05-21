#pragma once

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/*                         Individual Species Structure                      */
typedef struct  {
#define  eC_ISS_SPe 10
   char cr_Spe [ eC_ISS_SPe + 1 ];
   int  i_Den;                          /* Density                           */
   int  i_Killed; 

/* 12-16-2016 - change from int to float */ 
   float  f_DBH;                        /* Diam Breast Height                */

   int  i_TreHgt;                       /* Tree Height                       */

   int  i_CroRat;                       /* Crown Ration                      */
   bool b_Graph;     /* Graph check box from grid */

}  d_ISS;

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
void ISS_InitTbl (void);
void ISS_Init (d_ISS *a_ISS);
int  ISS_Set  (d_ISS *a_ISS);
void ISS_Display (void);
int  ISS_Get  (d_ISS *a_ISS, int iX);
int  ISS_Count (void);

int  ISS_Total (float *af_Den, float *af_Live, float *af_Kill, float f_DBH );


void  ISS_SetKill (int i_Kill, int iX);

float ISS_AvgDBHKill ();

int  ISS_BasalArea (float *af_Den, float *af_Live, float *af_Kill );
int  ISS_CrownCover (float *af_Den, float *af_Live, float *af_Kill, char A[] );


float CC_Overlap( float f_SqFtCov);