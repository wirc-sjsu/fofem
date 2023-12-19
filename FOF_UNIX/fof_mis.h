extern "C" {
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: FOF_MIS.h
* Desc: New version of Mortality, 
* Date: 2/23/2018
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
// #pragma once 

/*-------------------------------------------------*/

#pragma once 

#define eC_MIS 500


/*  Mortality Input Structure  */
typedef struct {
     char  cr_Spe[20];
     char  cr_EquTyp[20];
     float f_Den;
     float f_DBH;
     float f_Hgt;
     float f_CR;           /* Crown ratio */

     float f_FS;           /* Flame len or Scorch Hgt */
     char  cr_FS[10];       /* "F" or "S" - identify f_FS*/
     char  cr_FirSev[10];  /* Fire Severity "Low" or "" */ 


     float f_CrnDam;       /* Crown Damage */
     float f_CKR;          /* Cambium Kill Ratio */
     char  cr_BeeDam[10];  /* Beetle Damage */
     float f_BolCha;       /* Bole Char height */  
     bool  b_Graph;        /* Graph */

/* Outputs..................*/
     float f_Prop;         /* Propability Mortality percent */
     char  cr_Eq[10];      /* Equation used */

/* The assigned Prob Cut off point for the Size Class that */
/*  this record falls into based on its dbh */
/* Probability Cut Off point is used to do the Salvage report */   
     float f_ProbCutOff;    

   } d_MIS;

#define e_EndTbl "!!!"      /* Mark end of table - cr_Spe */
#define e_BeeDamNA  ""   // "NA"   /* Beetle Damage - Not Applicable */


void MIS_InitRec(d_MIS *a);
void MIS_Init ();

int   MIS_SetProbCutOff(); 

int   MIS_Check(char cr_ErrMes[]);
int  MIS_ChkRec (d_MIS *MIS, int i_Row, char cr_ErrMes[]) ;

int   MIS_EquTyp (char cr_EquTyp[]);
int   MIS_Get (int iX, d_MIS *a_MIS);

int _SetYesNo ( char cr[] );

int _CrownRatio (int i);
}