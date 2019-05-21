
#pragma once 


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* eC_TotNam is the size of the Total/Summary Id Name the the reports use    */
/*  for their Totals and Summaries, it is enter in Total Window by the user  */
/*  this define also gets used in the printf formating the Id-Name           */
#define eC_Totals   200                /* Max Num of Report Totals arrays    */
#define eC_TotNam   15                 /* Size of Report Total Name string   */

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/*                        Smoke - Summary Total Structure                    */
typedef struct  {
//   float f_TotEff;

   float f_TotPM10;
   float f_TotPM25;
   float f_TotCH4;
   float f_TotCO;
   float f_TotCO2;
   float f_TotNOX;
   float f_TotSO2;

#define  eC_STSNam eC_TotNam
    char   cr_Name[eC_STSNam + 1];
}  d_STS;

void BTS_Zero (d_STS *a_STS);
void BTS_Init (void);


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/*                        Fuel -  Summary Total Structure                    */
/* Accumulate 'Fuel Consumed Load - tons per acre'                           */
/* These are the totals you see on the Fuel Report                           */
typedef struct  {
    float  f_Litter;
    float  f_DW0;                            /* 0 -> 1/4 Wood               */
    float  f_DW10;                           /* 1/4 -> 1 Wood               */
    float  f_DW100;                          /* 1 -> 3   Wood               */
    float  f_DW1000;                         /* 3+       Wood               */
    float  f_DufAmtCon;                      /* Amount of Duff Load Consumed*/
    float  f_DufDepRed;                      /* Duff Depth amount reduced    */
    float  f_Herb;
    float  f_Shrub;
    float  f_CroBra;
    float  f_CroFol;
#define  eC_STNam eC_TotNam
    char   cr_Name[eC_STNam + 1];
}  d_ST;
/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */

int  ST_isName (char cr_TotNam[]);
int  ST_Count  (void);
void ST_Init   (void);
void ST_Add    (d_ST *a_ST, char cr_Name[]);
void ST_Zero   (d_ST *a_ST);
int  ST_Get    (d_ST *a_ST, int iX);

void  STS_Init  (void);
void  STS_Zero  (d_STS *a_STS);
void  STS_Add   (d_STS *a_STS, char cr_Name[]);
int   STS_Get   (d_STS *a_STS, int iX);
int   STS_Count (void);
