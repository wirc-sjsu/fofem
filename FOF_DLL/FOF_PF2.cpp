/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_pf2i.c    Post Fire Injury Mortality Table
* Desc: Postfire Injusty equations from Sharon Hood, April 2008
* Date: 4-1-08
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include  "fof_util.h"
//#include  "fof_pf.h"
#include  "fof_pf2.h"

#include  "fof_mec.h"


#ifdef wowoowowo

float Mypow (float a, float b); 
float Mypow (float a, float b)
{
float f;
   f = pow (a,b);
 return f; 
}


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/*                  Equation Function Table                                  */
/* Note: the Eq Requirments string must match the function's required         */
/*       inputs - this string is used to valid inputs and show to user       */
/*       the strings below get appened into the GUI Species dropdown list    */
/* Note-1 2-23-2015, "dbhR" was removed, we going to make dbh reguired for   */
/*       all species input in the grid,                                      */ 
/*       not all eqs need it but dbh is needed in report for avg dbh         */
/*       This will make things less confusing - hopefully                    */
/*       Eq Code             Function                Equation Reqirmnts      */
d_EFT sr_EFT[] = {
       { e_WhiteFir,        Eq_WhiteFir_WF,        "len dbh ckr btl" },   /* dbhR See Note-1 */
       { e_SubalpineFir,    Eq_SubalpineFir_SF,    "vol dbh ckr X" },
       { e_IncenseCedar,    Eq_IncenseCedar_IC,    "len dbh ckr X" },
       { e_WesternLarch,    Eq_WesternLarch_WL,    "vol dbh ckr X" },
       { e_WhitebarkPine,   Eq_WhitebarkPine_WP,   "vol dbh ckr X" },     /* dbhR  See Note-1 */
       { e_EngelmannSpruce, Eq_EngelmannSpruce_ES, "vol dbh ckr X" },
       { e_SugarPine,       Eq_SugarPine_SP,       "len dbh ckr btl" },
       { e_RedFir,          Eq_RedFir_RF,          "len dbh ckr X" },
       { e_DouglasFir,      Eq_DouglasFir_DF,      "vol dbh ckr btl" },   /* dbhR See Note-1  */

       { e_PonderosaPine,   Eq_PonderosaPine_PP,   "vol dbh ckr btl" },
       { e_PonderosaKill,   Eq_PonderosaKill_PK,   "kil dbh ckr btl" },

       { "", NULL, "" }};


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: PFI_Calc
* Desc: Perform Calc for a species
*
*   In: a_PFI...contains species, inputs, and Eq code to use
*  Out: a_PFI...mortality in f_Prop
*
*  Ret: 1 OK, 0 error see cr_ErrMes
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int PFI_Calc (d_PFI *a_PFI, char cr_ErrMes[])
{
int i,j;
int (*func) (d_PFI *a_PFI);

   strcpy (cr_ErrMes,"");

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
   for ( i = 0; i < 1000; i++ ){        /* look for entry we want.           */
      if ( !stricmp (sr_EFT[i].cr_Eq,"")){
        sprintf (cr_ErrMes,"Invalid post fire eq %s",a_PFI->cr_Eq);
        return 0; }

      if ( stricmp (a_PFI->cr_Eq,sr_EFT[i].cr_Eq)) /* Not found              */
        continue;                                  /*  keep looking          */

      if ( !ValidInput (a_PFI, sr_EFT[i].cr_Fmt, cr_ErrMes))
         return 0;                      /* Validate all inputs we'll use     */

       func = sr_EFT[i].func;           /* Address of func for spcfid eq     */
       j = (*func) (a_PFI);             /* Execute the Eq function           */
       if ( j == 0 )                    /* did we get and error              */
         return 0;                      /*  shouldn't realy get any          */
       break;  }

   return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: ValidInput
* Desc: Valididate all inputs for a line of species input data
*       Use the functions format string (ie "Vol DBH CKR X" )
*       Each token in string is looked at and validated accordingly
*
*   In: cr_Fmt...format string from EFT table, string specifies
*                 what input are/are not required
*       a_PFI....single species record/struct
*  Out: cr.......Error message
*  Ret: 1 ok, else 0
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int ValidInput (d_PFI *a_PFI, char cr_Fmt[], char cr[])
{
char CR[10],DBH[10],CKR[10],Btl[10];

  if ( a_PFI->f_Den < 1.0 || a_PFI->f_Den > 20000 ) {
    sprintf (cr,"Invalid density specified: %s %6.2f",a_PFI->cr_Spe,a_PFI->f_Den);
    return 0; }

/* Pull out each token......                                                 */
  sscanf(cr_Fmt,"%s %s %s %s", CR,DBH,CKR,Btl);

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */

  if (!ValidNum (CR, a_PFI->f_CRP, 0, 100, "Crown Ratio", a_PFI->cr_Spe,cr))
     return 0;

  if (!ValidNum (DBH,a_PFI->f_DBH, 0, 500, "DBH", a_PFI->cr_Spe,cr))
     return 0;

  if (!ValidNum (CKR,a_PFI->f_CKR, 0, 4, "Cambium Kill", a_PFI->cr_Spe,cr))
     return 0;

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/* Beetle                                                                    */
  if ( !stricmp (Btl,e_NA) && stricmp (a_PFI->cr_Btl,e_NA)) {
    sprintf (cr,"%s - beetle code is not applicable\nPlease set to %s",a_PFI->cr_Spe,e_NA);
    return 0; }

  if ( !stricmp (Btl,e_NA))
    goto Done;

  if ( stricmp (a_PFI->cr_Btl,e_BtlYes) && stricmp (a_PFI->cr_Btl,e_BtlNo)) {
    sprintf (cr,"%s - beetle code is invlalid\nPlease set to %s or %s",a_PFI->cr_Spe,e_BtlYes,e_BtlNo);
    return 0; }

Done:
  return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: ValidNum
* Desc: Validate a numberic input, Crown Ration,DBH, or Cambium Kill
*       Checks for limits, check for not missing when requird
*       check if value when its not applicable
*  Ret:
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int ValidNum (char cr_Fmt[], float f_Num, float f_Min, float f_Max,
              char cr_FldNam[],  char cr_Spe[], char cr_Err[])
{

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/* if not Required and none entered - OK                                     */
   if ( !stricmp(cr_Fmt,e_NA) &&  f_Num == e_PFI_Miss )
     return 1;

/* if not Required but user entered something - error                        */
  if ( !stricmp(cr_Fmt,e_NA) && f_Num != e_PFI_Miss) {
    sprintf (cr_Err,"%s - %s setting is not applicable\nPlease use %s",cr_Spe,cr_FldNam,e_NA);
    return 0; }

 if ( stricmp(cr_Fmt,e_NA) && f_Num == e_PFI_Miss ) {
    sprintf (cr_Err,"%s - %s please enter the required input",cr_Spe,cr_FldNam);
    return 0; }


/* Check min and max                                                         */
  if ( f_Num < f_Min || f_Num > f_Max ) {
       sprintf (cr_Err,"%s - %s is out of limits use %4.0f -> %4.0f ",
       cr_Spe,cr_FldNam,f_Min,f_Max);
     return 0; }

  return 1;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Eq_WhiteFir_WF            ABICON
* Desc:
*   In: a_PFI->f_CRP....crown length scorched percent
*       a_PFI->f_CKR....cambium kill ratio 0-4
*       a_PFI->f_DBH....inches
*       a_PFI->cr_Btl ...Beetle Kill  e_BtlYes, e_BtlNo
*  Out: a_PFI->f_Prop ...0 -> 1.0
*  Ret:
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int Eq_WhiteFir_WF (d_PFI *a_PFI)
{
float f,cs,CKR,DBH,btl;

  cs  =  a_PFI->f_CRP;
  CKR =  a_PFI->f_CKR;
  DBH =  a_PFI->f_DBH;

  if ( !stricmp (a_PFI->cr_Btl,e_BtlYes) )
    btl =  1.0;
  else
    btl =  -1.0;

  f =  1.0 +
       exp ( - ( -3.5964 +
                ( Mypow(cs,3.0) * 0.00000628 ) +
                ( CKR * 0.3019 ) +
                ( DBH * 0.0483) +
                ( btl * 0.5209 ) ) );
  f = 1.0 / f;
  a_PFI->f_Prop = f;
  return 1;
}



/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Eq_SubalpineFir_SF   ABILAS
* Desc:
*   In: a_PFI->f_CRP....crown volume scorched percent
*       a_PFI->f_CKR....cambium kill ratio 0-4
*  Out: a_PFI->f_Prop ...0 -> 1.0
*  Ret: 1 ok
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int Eq_SubalpineFir_SF (d_PFI *a_PFI)
{
float f,cs,CKR;

  cs  =  a_PFI->f_CRP;
  CKR =  a_PFI->f_CKR;
  f = 1.0 / ( 1.0 + exp ( -
          ( -2.6036 +
          ( Mypow(cs,3.0) * 0.000004587 ) +
          ( CKR * 1.3554) ) ) );
  a_PFI->f_Prop = f;
  return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Eq_WesternLarch_WL    LAROCC
* Desc:
*   In: a_PFI->f_CRP....crown volume scorched percent
*       a_PFI->f_CKR....cambium kill ratio 0-4
*  Out: a_PFI->f_Prop ...0 -> 1.0
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int Eq_WesternLarch_WL  (d_PFI *a_PFI)
{
float f,cs,CKR;

  cs  =  a_PFI->f_CRP;
  CKR =  a_PFI->f_CKR;
  f = 1.0 / ( 1.0 + exp ( -
          ( -3.8458 +
          ( Mypow(cs,2.0) * 0.0004 ) +
          ( CKR * 0.6266) ) ) );
  a_PFI->f_Prop = f;
  return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Eq_IncenseCedar_IC    LIBDEC
* Desc:
*   In: a_PFI->f_CRP....crown length scorched percent
*       a_PFI->f_CKR....cambium kill ratio 0-4
*  Out: a_PFI->f_Prop ...0 -> 1.0
*  Ret: 1 ok
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int Eq_IncenseCedar_IC (d_PFI *a_PFI)
{
float f,cs,CKR;

  cs  =  a_PFI->f_CRP;
  CKR =  a_PFI->f_CKR;
  f = 1.0 / ( 1.0 + exp ( -
          ( -5.6465 +
          ( Mypow(cs,3.0) * 0.000007274 ) +
          ( CKR * 0.5428) ) ) );
  a_PFI->f_Prop = f;
  return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Eq_WhitebarkPine_WP   PINALB
* Desc:
*   In: a_PFI->f_CRP....crown volume scorched percent
*       a_PFI->f_CKR....cambium kill ratio 0-4
*       a_PFI->f_DBH
*  Out: a_PFI->f_Prop ...0 -> 1.0
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int Eq_WhitebarkPine_WP  (d_PFI *a_PFI)
{
float f,cs,DBH,CKR;
  cs  =  a_PFI->f_CRP;
  CKR =  a_PFI->f_CKR;
  DBH =  a_PFI->f_DBH;

  f = 1.0 / ( 1.0 + exp ( -
          ( -1.4059 +
          ( Mypow(cs,3.0) * 0.000004459 ) +
          ( (CKR*CKR) * 0.2843) +
          ( DBH * -0.1232) ) ) );
  a_PFI->f_Prop = f;
  return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Eq_EngelmannSpruce_ES   PICENG
* Desc:
*   In: a_PFI->f_CRP........crown volume scorched percent
*       a_PFI->f_CKR........cambium kill ratio
*  Out: a_PFI->f_Prop ...0 -> 1.0
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int Eq_EngelmannSpruce_ES (d_PFI *a_PFI)
{
float f,cs,CKR;
  cs  =  a_PFI->f_CRP;
  CKR =  a_PFI->f_CKR;
  f = 1.0 / ( 1.0 + exp ( -
            ( -2.9791 +
            ( cs * 0.0405 ) +
            ( CKR * 1.1596) ) ) );
  a_PFI->f_Prop = f;
  return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Eq_RedFir_RF  ABIMAG
* Desc:
*   In: a_PFI->f_CRP....crown length scorched percent
*       a_PFI->f_CKR....cambium kill ratio 0-4
*  Out: a_PFI->f_Prop ...0 -> 1.0
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int Eq_RedFir_RF (d_PFI *a_PFI)
{
float f,cs,CKR,Btl;
  cs  =  a_PFI->f_CRP;
  CKR =  a_PFI->f_CKR;

  f = 1.0 / ( 1.0 + exp ( -
            ( -4.7515 +
            ( Mypow(cs,3.0) * 0.000005989 ) +
            ( CKR * 1.0668) ) ) );

  a_PFI->f_Prop = f;
  return 1;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Eq_SugarPine_SP   PINLAM
* Desc:
*   In: a_PFI->f_CRP....crown length scorched percent
*       a_PFI->f_CKR....cambium kill ratio 0-4
*       a_PFI->f_DBH
*       a_PFI->cr_Btl ...Beetle Kill  e_BtlYes, e_BtlNo
*  Out: a_PFI->f_Prop ...0 -> 1.0
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int Eq_SugarPine_SP (d_PFI *a_PFI)
{
float f,cs,CKR,Btl;
  cs  =  a_PFI->f_CRP;
  CKR =  a_PFI->f_CKR;

  if ( !stricmp (a_PFI->cr_Btl ,e_BtlYes) )
     Btl = 1.0;
  else
     Btl = -1.0;

  f = 1.0 / ( 1.0 + exp ( -
            ( -2.7598 +
            ( (cs * cs) * 0.000642 ) +
            ( Mypow (CKR,3.0) * 0.0386) + ( Btl * 0.8485) ) ) );

  a_PFI->f_Prop = f;
  return 1;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Eq_PonderosaPine_PP  PINPON
* Desc: There are 2 ponderosa equation, (see Eq_PonderosaKill_PK)
* NOTE: Currently Jeffrey Pine also uses this
*   In: a_PFI->f_CRP....crown volume scorched percent
*       a_PFI->f_CKR....cambium kill ratio 0-4
*       a_PFI->cr_Btl ...Beetle Kill  e_BtlYes, e_BtlNo
*  Out: a_PFI->f_Prop ...0 -> 1.0
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int Eq_PonderosaPine_PP (d_PFI *a_PFI)
{
float f,cs,CKR,Btl,F;

  cs  =  a_PFI->f_CRP;
  CKR =  a_PFI->f_CKR;
  if ( !stricmp (a_PFI->cr_Btl ,e_BtlYes) )
     Btl = 1.0;
  else
     Btl =  0.0;

  F = ( -4.1914 + ( Mypow(cs,2.0) * 0.000376)
                + ( CKR * 0.5130)
                + ( Btl * 1.5873 ) );
  f = 1.0 / ( 1.0 + exp ( - (  F  ) ) );

  a_PFI->f_Prop = f;
  return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Eq_PonderosaKill_PK  PINPONk
* Desc: There are 2 ponderosa equation, (see Eq_PonderosaPine_PP)
*       This equations uses Killed percent
* NOTE: Currently Jeffrey Pine also uses this
*   In: a_PFI->f_CRP....percent of crown killed
*       a_PFI->f_CKR....cambium kill ratio 0-4
*       a_PFI->cr_Btl ...Beetle Kill  e_BtlYes, e_BtlNo
*  Out: a_PFI->f_Prop ...0 -> 1.0
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int Eq_PonderosaKill_PK (d_PFI *a_PFI)
{
float f,cs,CKR,Btl,F;

  cs  =  a_PFI->f_CRP;
  CKR =  a_PFI->f_CKR;
  if ( !stricmp (a_PFI->cr_Btl ,e_BtlYes) )
     Btl = 1.0;
  else
     Btl =  0.0;

  F = ( -3.5729 + ( Mypow(cs,2.0) * 0.000567)
                + ( CKR * 0.4573)
                + ( Btl * 1.6075) );
  f = 1.0 / ( 1.0 + exp ( - (  F  ) ) );

  a_PFI->f_Prop = f;
  return 1;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Eq_DouglasFir_DF  PSEMEN
* Desc:
*   In: a_PFI->f_CRP....crown volume scorched percent
*       a_PFI->f_CKR....cambium kill ratio 0-4
*       a_PFI->f_DBH
*       a_PFI->cr_Btl ...Beetle Kill  e_BtlYes, e_BtlNo
*  Out: a_PFI->f_Prop ...0 -> 1.0
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int Eq_DouglasFir_DF (d_PFI *a_PFI)
{
float f,cs,DBH,CKR,Btl,F;

  cs  =  a_PFI->f_CRP;
  CKR =  a_PFI->f_CKR;
  DBH =  a_PFI->f_DBH;
  if ( !stricmp (a_PFI->cr_Btl ,e_BtlYes) )
     Btl = 1.0;
  else
     Btl =  0.0;

  F = ( -1.8912 + (cs * 0.07) - (Mypow(cs,2.0) * 0.0019) +
                (Mypow(cs,3.0) * 0.000018) + (CKR * 0.5840)
       - (DBH * 0.0788) - (Btl * 0.7959) + (DBH * Btl * 0.1251)
      ) ;

  f = 1.0 / ( 1.0 + exp ( - (  F  ) ) );

  a_PFI->f_Prop = f;
  return 1;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: PFI_EqFmt
* Desc: Return a format string - string is used in the Valid Speices
*        Wnd to show what inputs are required for the particular
*        speicies equation
*       Example:
*            cr_Name --> "Alies concolor -- White Fir"
*            cr_Fmt <--- "White Fir (Vol DBH CHK Btl)"
*
*   Out: cr_Fmt...created string
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  PFI_EqFmt (char cr_Eq[], char cr_Name[], char cr_Fmt[])
{
int i;
char *a,cr[300];

/* Get the Common Name from the full sci name -- comman name string          */
  strcpy (cr,"");
  a = strstr (cr_Name,"--");
  a = a + 2;
  strcpy (cr,a);
  Trim_LT(cr);

  strcpy (cr_Fmt," No Formt ");  /* init, but shouldn't get used            */

   for ( i = 0; i < 1000; i++ ) {
      if ( !stricmp (sr_EFT[i].cr_Eq,"") ) {
        return 0; }

      if ( stricmp (cr_Eq,sr_EFT[i].cr_Eq))
        continue;

      sprintf(cr_Fmt, "%s (%s)", cr, sr_EFT[i].cr_Fmt);
      break;
   }
  return 1;
}

#endif 
