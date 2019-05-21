/*****************************************************************
* Name: fof_mis.cpp
* Desc: Mortality Input Structure, 
*
*****************************************************************/
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "fof_mis.h" 
#include "fof_mrt.h"
#include "fof_mec.h"
#include "fof_iss.h"
#include "fof_smt.h"

/*...........................................................*/
/* Mortality Input Structure - Species grid get load in here */
/* don't need this many table entries for GUI, but it might */
/*  get used in the batch to hold a bunch of trees yes/no/maybe*/

d_MIS sr_MIS[eC_MIS];

float PST_GetProb (float dbh);

/**************************************************************
* Name: MIS_SetProbCutOff
* Desc: set the probability cutoff into each entry of the
*       MIS table. MIS = GUI species datagrid info
        PST table = GUI Probability Size Class info
* NOTE: the PST table must be loaded first 
**************************************************************/
int MIS_SetProbCutOff()
{
int i;
   for ( i = 0; i < eC_MIS; i++ ) {
      if ( !strcmp (sr_MIS[i].cr_Spe,e_EndTbl) )
         break;
      sr_MIS[i].f_ProbCutOff = PST_GetProb(sr_MIS[i].f_DBH); 
   } 
   return 1; 
}

/*******************************************************
* Name: MIS_EquTyp
* Desc: for the specified Equation Type return the 
*       number of them found in the sr_MIS table
*   In: e_CroSco, e_CroDam, e_BolCha
*  Ret: count 
********************************************************/
int MIS_EquTyp(char cr_EquTyp[])
{
int i,n;
  n = 0; 
  for ( i = 0; i < 1000; i++ ) {
     if ( !stricmp (sr_MIS[i].cr_Spe, e_EndTbl) ) 
       break; 
     if ( !stricmp (sr_MIS[i].cr_EquTyp,cr_EquTyp)) 
        n++; 
  } 
  return n; 
}

/*******************************************************
* Name: MIS_Get
* Desc: iteratively get specified records from table
*   In: iX....start at 0 to get first record
*  Ret: 1 ok, 0 no more
********************************************************/
int MIS_Get (int iX, d_MIS *a_MIS)
{
int i;
   for ( i = 0; i < 1000; i++ ) {
     if ( !stricmp (sr_MIS[i].cr_Spe, e_EndTbl) ) 
       return 0; 
     if ( i == iX ) {
       memcpy (a_MIS, &sr_MIS[i],sizeof(d_MIS)); 
       return 1; }
   }
   return 0; 
}

/************************************************************
* Name: MIS_Total
* Desc: Get the original density, the remaining live and the total
*       killed.
* NOTE: there is one entry in the table for every tree species the
*        user entered in Mortality GUI tree grid
************************************************************/
int MIS_Totals (float *af_Den, float *af_Live, float *af_Kill, float f_DBH)
{
int i;
float f, Den, Live, Kill, killed; 
char cr_ErrMes[1000]; 
d_MO s_MO;

   Den = Live = Kill = 0; 
   for ( i = 0; i < eC_MIS; i++ ) {
     if (!strcmp (sr_MIS[i].cr_Spe,e_EndTbl) ) 
        break; 
     if ( sr_MIS[i].f_DBH < f_DBH )
        continue;
      Den  += sr_MIS[i].f_Den;                  /* Original Density */
      killed = MIS_Killed (&sr_MIS[i]);
      Kill += killed; 
      Live +=  sr_MIS[i].f_Den - killed; 
   }
   *af_Den = Den; 
   *af_Live = Live;
   *af_Kill = Kill;  

  return 1; 
}


/************************************************************
* Name: MIS_Total_SizCls
* Desc: Get the original density, the remaining live and the total
*       killed for the specified size class
* NOTE: there is one entry in the table for every tree species the
*        user entered in Mortality GUI tree grid
*   In: Low .... lower dbh - equal to or greater than   
*       Up  .... upper dbh - less than
* Example:   1 --> 3 will get one inch and up, but Not 
*                 including 3 inch
************************************************************/
int MIS_Totals_SizCls (float *af_Den, float *af_Live, float *af_Kill, float Low, float Up)
{
int i;
float f, Den, Live, Kill, killed, f_Prob; 
char cr_ErrMes[1000]; 
d_MO s_MO;
d_MIS s_MIS; 

   Den = Live = Kill = 0; 
   for ( i = 0; i < eC_MIS; i++ ) {
     if (!strcmp (sr_MIS[i].cr_Spe,e_EndTbl) ) 
        break; 
     if ( sr_MIS[i].f_DBH >= Up )
        continue;
     if ( sr_MIS[i].f_DBH < Low ) 
        continue;

     Den  += sr_MIS[i].f_Den;                  /* Original Density */

     f_Prob = MRT_CalcMngr  (&sr_MIS[i], &s_MO, cr_ErrMes);
     if ( f_Prob < sr_MIS[i].f_ProbCutOff ) 
       Live += sr_MIS[i].f_Den;
     else 
       Kill += sr_MIS[i].f_Den;
   }

   *af_Den = Den; 
   *af_Live = Live;
   *af_Kill = Kill;  

  return 1; 
}


/********************************************************
* Desc: Check Mortality Input Struct Table 
* Note: There may or may not be and empty row on the 
*       end of table. Depending on how the GUI species 
*       grid was loaded. 
********************************************************/
int MIS_Check(char cr_ErrMes[])
{
int i;  

  strcpy (cr_ErrMes,""); 

  for ( i = 0; i < 1000; i++ ){          /* Each Tree record */
    if ( !stricmp (sr_MIS[i].cr_Spe,e_EndTbl ) )
       break;
    if ( !stricmp (sr_MIS[i].cr_Spe, "" ) ) 
       break;                  /* Check for this to just in case */

    if ( !MIS_ChkRec (&sr_MIS[i], i+1, cr_ErrMes)) 
       return 0; 

  }  /* for i */  

  return 1;
}

/*************************************************************
* Name: MIS_ChkRec
* Desc: Check one MIS record
*   in: MIS .. record
*       i_Row.. row number to include in error message
*               NOTE use -1 to not use
**************************************************************/
int  MIS_ChkRec (d_MIS *MIS, int i_Row, char cr_ErrMes[]) 
{
char cr[1000], cr_MrtEqu[100], cr_ReqFld[100]; 

    strcpy (cr_ErrMes, ""); 

    if ( !strcmp (MIS->cr_Spe,"" ) ) {  /* Must have a species code */
       strcpy (cr,"Missing Species"); 
       goto Err; }

    if ( MIS->f_Den <= 0 || MIS->f_Den > 500 ) {            /* must have density */
      sprintf  (cr,"Density %6.0f ",MIS->f_Den); 
      goto Err; }

/* Crown Scorch Type Equations - (pre-fire numeric equ) ......... */  
    if ( !stricmp (MIS->cr_EquTyp, e_CroSco ) ) {
      if ( MIS->f_DBH <= 0 || MIS->f_DBH > 500.0 ) { 
        sprintf  (cr,"DBH %4.1f",MIS->f_DBH); 
        goto Err; }

      if ( MIS->f_Hgt <= 0 || MIS->f_Hgt > 379.0 ) { 
        sprintf (cr,"Height %4.0f",MIS->f_Hgt); 
        goto Err; }

     if ( MIS->f_CR <= 0 || MIS->f_CR > 10.0  ) { 
        sprintf (cr,"C/R Crown Ratio %4.1f",MIS->f_CR); 
        goto Err; }
   
 }  /* if Crown Scorch */

/* Crown Damage Type Equations - (alpha code) ................. */
/* these vary in what inputs are required */
   if ( !stricmp (MIS->cr_EquTyp, e_CroDam ) ) {
 
     /* Get the Equ Reguired string to see if DBH and/or Beetle is required   */
      SMT_MortEqu_Code (MIS->cr_Spe, MIS->cr_EquTyp, cr_MrtEqu);
      EFR_Get (cr_MrtEqu, cr_ReqFld);

      if ( !strstr(cr_ReqFld,"dbh") )   /* is dbh reguired */ 
         goto Btl;                      /*  nope */
      if ( MIS->f_DBH <= 0 || MIS->f_DBH > 500.0 ) { 
        sprintf (cr,"DBH  %4.1f",MIS->f_DBH); 
        goto Err; }

Btl:
      if ( !strstr(cr_ReqFld,"btl") )  /* beetle required */ 
        goto Dam;                          /*  nope */
      if ( stricmp (MIS->cr_BeeDam, "Y") && stricmp (MIS->cr_BeeDam, "N") ) {
        sprintf (cr,"Beetle Attack %s (use Y/N)",MIS->cr_BeeDam); 
        goto Err; }

Dam:
      if ( MIS->f_CrnDam < 0 || MIS->f_CrnDam > 100.0 ) {  /* Damage code is always required */
        sprintf (cr,"Crown Damage %4.0f (0->100)",MIS->f_CrnDam); 
        goto Err; }

      if ( MIS->f_CKR < 0 || MIS->f_CKR  > 4.0 ) {    /* cambium Kill is always required */
        sprintf (cr, "CKR %3.0f - Cambium Kill Ratio 0->4", MIS->f_CKR); 
        goto Err; }
    } /* if  Crown Damage */

/* Bole Char equations always required DBH and Bole Char Height */
   if ( !stricmp (MIS->cr_EquTyp, e_BolCha ) ) {
     if ( MIS->f_BolCha <= 0 || MIS->f_BolCha > 50 ) {
       sprintf (cr,"Bole Char %3.1f",MIS->f_BolCha); 
       goto Err; }
     if ( MIS->f_DBH <= 0 || MIS->f_DBH > 500.0) { 
        sprintf (cr,"DBH %4.1f", MIS->f_DBH); 
        goto Err; }
   } /* if Bole Char */ 

  return 1; 

Err: 
    if ( i_Row < 0 ) 
      sprintf (cr_ErrMes, "Invalid/Missing Value: %s  %s ", cr, MIS->cr_Spe);
    else
      sprintf (cr_ErrMes, "Invalid/Missing Value: %s  Row: %d  %s ", cr, i_Row, MIS->cr_Spe);
    return 0;  
}

/***************************************************************
* Name: MIS_Killed
* Desc: Number of trees killed for a single MIS record, an MIS
*       record represents one entry/row in the GUI species grid
*  Ret: trees killed as Whole Number (rounded off)
***************************************************************/
float MIS_Killed (d_MIS *a_MIS)
{
float f, killed;
char cr_ErrMes[1000];
d_MO s_MO;

/* first get the mortality percent */
  f = MRT_CalcMngr (a_MIS, &s_MO, cr_ErrMes);
  killed = f * a_MIS->f_Den;  /* mortality times density */
// 5-14-18, DL don't round off  
//  killed = int (killed + 0.5);   /* round to whole number */
  return killed;
}

/********************************************************
* Init the table 
************************************************/
void MIS_Init ()
{
int i; 
   for ( i = 0; i < eC_MIS; i++ ) 
      MIS_InitRec (&sr_MIS[i]);
}

/****************************************************
* Init single record in table
*****************************************************/
void MIS_InitRec(d_MIS *a)
{
     strcpy (a->cr_Spe,e_EndTbl);
     strcpy (a->cr_EquTyp,"");
     a->f_Den = -1.0; 
     a->f_DBH = -1.0;
     a->f_Hgt = -1.0;
     a->f_CR = -1.0;           /* Crown ratio */
     a->f_CrnDam = -1.0;       /* Crown Damage */
     a->f_CKR = -1.0;          /* Cambium Kill Ratio */
     strcpy (a->cr_BeeDam,e_BeeDamNA);         /* Beetle Damage */
     a->f_BolCha = -1.0;       /* Bole Char height */  
     a->b_Graph = false;        /* Graph */

     strcpy (a->cr_Eq, "");
     a->f_Prop = -1.0; 
}

/********************************************************************
* Name: MIS_CrownCover
* Desc: Get Crown Cover for entire table    
* NOTE: there is one entry in the table for every tree species the
*        user entered in Mortality GUI tree grid
*
********************************************************************/
int  MIS_CrownCover (float *af_Den, float *af_Live, float *af_Kill, char A[] )
{
int i, ii,jj,kk, i_Live;
float f,g; 
float f_CC, Den, Live, Kill, f_Killed; 

  *af_Den  = 0; 
  *af_Live = 0;
  *af_Kill = 0; 
   strcpy (A,""); 
   Den = Live = Kill = 0; 
   for ( i = 0; i < eC_MIS; i++ ) {
     if (!strcmp (sr_MIS[i].cr_Spe,e_EndTbl) ) 
        break;
     if ( sr_MIS[i].f_Hgt <= 0 )    /* Need Height - See SMT_CalcCrnCov() */  
        continue;  
     f_Killed = MIS_Killed (&sr_MIS[i]); 
     f_CC = SMT_CalcCrnCov (sr_MIS[i].cr_Spe, sr_MIS[i].f_DBH, sr_MIS[i].f_Hgt);
     Den  += f_CC * sr_MIS[i].f_Den;                   /* Crwn Cov or orig density  */
     Kill += f_CC * f_Killed;                /*  area of trees killed        */
     i_Live = sr_MIS[i].f_Den - f_Killed;
     Live += f_CC * (float) i_Live;                            /* diff of areas is liv area */

   }   /* for */

   if ( Den == 0 )                      /* See Note-1 above */  
    return 0; 

/* This portion of the code was done in the Mort report right before displaying it */
  f = CC_Overlap (Den);                   /* percent of cover per acre   */
  g = CC_Overlap (Live);                  /*  considering overlap        */

  if ( f < 1.0 ){                            /* for really small amounts    */
    strcpy (A,"(less than)");               /* we'll so 1 and "less than"  */
    f = g  = 1;}                             /*  on the report              */
  else
    strcpy (A,"");

  ii = f + 0.5;                               /* whole numbers, this helps    */
  jj = g + 0.5;                               /* with 'Change', so don't get  */
  kk = ii - jj;                                 /* 3 - 1 = 1 becase float roundin */

  Den = ii; 
  Live = jj;
  Kill = kk; 

  *af_Den  = Den; 
  *af_Live = Live;
  *af_Kill = Kill; 
  
  return 1; 
}

/*****************************************************
*
*****************************************************/
int _SetYesNo ( char cr[] )
{
   if ( !stricmp (cr, "yes" ) )
     strcpy (cr, "Y");
   else if ( !stricmp(cr, "y") )
     strcpy (cr, "Y");
   else if ( !stricmp (cr, "no" ) )
     strcpy (cr,"N");
   else if ( !stricmp (cr, "n" ) ) 
     strcpy (cr,"N");
   else { 
     strcpy (cr,""); 
     return 0; }
   return 1; 
}

/*****************************************************************
* Name: _CrownRatio
* Desc: get a Crown Ratio, 0 -> 10 given the percent of crown  
*   In: i....percent of crown 0 -> 100 (whole number)
* NOTE: IT'S up to caller to check for negative numbers and 
*        not send in here
****************************************************************/
int _CrownRatio (int i)
{
  if ( i <= 0 ) return 0;
  if ( i < 15 ) return 1; 
  if ( i < 25 ) return 2;
  if ( i < 35 ) return 3;
  if ( i < 45 ) return 4;
  if ( i < 55 ) return 5;
  if ( i < 65 ) return 6;
  if ( i < 75 ) return 7;
  if ( i < 85 ) return 8;
  if ( i < 95 ) return 9;
  return 10 ;
}
