/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name:
* Desc: Functions to calculate Mortality
* Date: 12-03
* Author: Larry Gangi
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "fof_ansi.h"
// #include  "fof_iss.h"
#include "fof_mis.h" 
#include "fof_mrt.h"
#include "fof_smt.h"
#include "fof_spp.h"
#include "fof_cct.h"
#include "fof_util.h"
#include "fof_mec.h"
#include "cdf_util.h"
#include "fof_pf2.h"


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/*                Post Fire Injury Eqs Codes Used in Fof_Spp.dat file        */
typedef struct {
           char  cr_Eq[4];
           float   (*func) (d_MIS *a_MIS);
           char  cr_Fmt[100];
        } d_EFT;

float Eq_WhiteFir_WF        (d_MIS *a_MIS);
float Eq_SubalpineFir_SF    (d_MIS *a_MIS);
float Eq_IncenseCedar_IC    (d_MIS *a_MIS);
float Eq_WesternLarch_WL    (d_MIS *a_MIS);
float Eq_WhitebarkPine_WP   (d_MIS *a_MIS);
float Eq_EngelmannSpruce_ES (d_MIS *a_MIS);
float Eq_SugarPine_SP       (d_MIS *a_MIS);
float Eq_RedFir_RF          (d_MIS *a_MIS);
float Eq_PonderosaPine_PP   (d_MIS *a_MIS);
float Eq_PonderosaKill_PK   (d_MIS *a_MIS);
float Eq_DouglasFir_DF      (d_MIS *a_MIS);

//  nt  PFI_Calc (d_MIS *a_MIS, char cr_ErrMes[]);
// int  PFI_EqFmt (char cr_Eq[], char cr_Name[], char cr_Fmt[]);
int ValidInput (d_MIS *a_MIS, char cr_Fmt[], int i_Row, char cr_ErrMes[]);
int ValidNum (char cr_Fmt[], float f_Num, float f_Min, float f_Max,
              char cr_FldNam[],  char cr_Spe[], char cr_Err[]);


/*-----------------------------------------------------------------*/
/*             Mortality Equation Field Reguired fields            */
/* Tells what column inputs are reguired in GUI Mortality Species Grid */

typedef struct {
   char cr_MrtEqu[10]; 
   char cr_ReqFld[100]; 

   float   (*func) (d_MIS *a_MIS);

} d_EFR; 

/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/* All crown scorch (previously refered to as pre-fire) mort   */
/* equations codes are numeric and all have the same input     */
/* requirements, so a single entry e_CroSco is entered in the table */
/* likewise with e_BolCha for Bole Char equations which are numberd */
/* 100 and up,   */
/* The Crown Damage (previously refered as post-fire) have alpha codes */
/*  and have various input requirements, so they are listed in the */
/*  the table individually */
/*  See the Crown Damage equation functions for details on thier  */
/*  individual inputs */
d_EFR  sr_EFR[] = {   
 /* Equ Id       required input fields   */ 
 { e_CroSco,          "dbh ht cr",      NULL   },
 { e_BolCha,          "dbh bc",         NULL   },
 { e_WhiteFir,        "dbh len ckr btl",  Eq_WhiteFir_WF },
 { e_SubalpineFir,    "dbh vol ckr",      Eq_SubalpineFir_SF },
 { e_IncenseCedar,    "len  ckr",         Eq_IncenseCedar_IC },
 { e_WesternLarch,    "vol  ckr",         Eq_WesternLarch_WL },  
 { e_WhitebarkPine,   "dbh vol ckr",      Eq_WhitebarkPine_WP },
 { e_EngelmannSpruce, "vol ckr",          Eq_EngelmannSpruce_ES }, 
 { e_SugarPine,       "dbh len ckr btl" , Eq_SugarPine_SP  },
 { e_RedFir,          "len  ckr"  ,       Eq_RedFir_RF },        
 { e_PonderosaPine,   "vol ckr btl",      Eq_PonderosaPine_PP },   
 { e_PonderosaKill,   "kil ckr btl" ,     Eq_PonderosaKill_PK }, 
 { e_DouglasFir,      "dbh vol ckr bt" ,  Eq_DouglasFir_DF },    
 { "",                 "",                NULL }
 }; 

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/* FOFEM Species Mortality Equation Table - loaded from input file   */
#define eC_smt 1000
d_SMT sr_SMT[eC_smt];

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/* These are used in Mortality to keep running totals and averages           */
float  gf_TotMort = 0;
float  gfN_TotMort = 0;
float  gf_4TotMort = 0;
float  gfN_4TotMort = 0;
float  gf_TotKilDBH = 0;
float  gf_CovTotPreLiv = 0;
float  gf_CovTotPosLiv = 0;

int PINPAL_Eq5 (d_MI *a_MI, float f_CSL, float f_BT, double *af_P);

float Whitefir (float cs);
float RedFir  (float cs);
float SubalpineFir (float cs);
float IncenseCedar (float cs);
float WesternLarch (float cs,float f_dbh);
float EngelmannSpruce  (float cs);
float WhitebarkPine (float cs, float dbh);
float SugarPine (float cs);
float PonderosaJeffreyPine (float cs);
float DouglasFir (float cs);

int _CnvReg (int i_Reg, char cr_Reg[]);
float Eq21_BlkHilPiPo (float hgt, float cbh, float dbh, float scorch, float flalen, float *af);
float  FtToMt (float Ft);
float PFI_Calc (d_MIS *a_MIS, d_MO *a_MO, char cr_ErrMes[]);
int PFI_Calc_OLD (d_MIS *a_MIS, char cr_ErrMes[]);
// int ValidInput (d_MIS *a_MIS, char cr_Fmt[], char cr[]);

float BC_Calc (d_MIS *a_MIS, d_MO *a_MO, char cr_ErrMes[]);

void  _DumpIt ();

/***********************************************************
* Name: SMT_GetName
* Desc
***********************************************************/
int     SMT_GetName ( char cr_Spe[],  char cr_Name[])
{
int iX;
   strcpy (cr_Name, "Error - SMT_GetName()");
   iX = SMT_GetIdx (cr_Spe);                 /* get idx into tbl of spe      */
   if ( iX == -1 )  

   strcpy (cr_Name,sr_SMT[iX].cr_ComNam); 
   return 1; 
}



/************************************************************
* Name: Mrt_CalcMngr
* Desc: Calculate Mortality, 
*       Looks at the Equation Type and calls the proper
*       calculation function. 
*   In: 
*************************************************************/
float  MRT_CalcMngr (d_MIS *a_MIS, d_MO *a_MO, char cr_ErrMes[])
{
float f;
d_MI s_MI; 
  f = 0;  
  strcpy (cr_ErrMes,""); 

   if (!SMT_MortEqu_Code (a_MIS->cr_Spe, a_MIS->cr_EquTyp, a_MIS->cr_Eq)) {
     sprintf (cr_ErrMes, "No %s Equation Code for %s", a_MIS->cr_EquTyp, a_MIS->cr_Spe);
     return -1.0; }

   strcpy (a_MO->cr_MortEqu,a_MIS->cr_Eq); 

  if ( !strcasecmp (a_MIS->cr_EquTyp, e_CroSco) ) {      /* Crown Scorch Equations */
    f = MRT_Calc(a_MIS, &s_MI,a_MO,cr_ErrMes);
    if ( f < 0 ) 
      return -1; 
    return f;  }

  if ( !strcasecmp (a_MIS->cr_EquTyp, e_CroDam) ) {     /* Crown Damage Equations */
    f = PFI_Calc(a_MIS, a_MO, cr_ErrMes); 
    return f; }
 
  if ( !strcasecmp (a_MIS->cr_EquTyp, e_BolCha) ) {     /* Bole Char Equations */
    f = BC_Calc (a_MIS, a_MO, cr_ErrMes);
    return f; } 
 
  strcpy (cr_ErrMes, "MRT_CalcMngr() - Logic Error - Invalid Equation Type"); 
  return -1.0;

} 

/********************************************************************
* Name: EFR_Get   - Equation Feilds Required
* Desc: Return the Required Fields strings,      
*   In: See notes above the sr_EFR[] declaration
*********************************************************************/
int EFR_Get (char  cr_MrtEqu[],  char cr_ReqFld[])
{
int i, x; 
char c; 
char cr_Equ[100];

   strcpy (cr_ReqFld, ""); 
   strcpy (cr_Equ,cr_MrtEqu);

   c = Get_NumTyp (cr_MrtEqu);  /* see if Equ Code is numeric  */
   if ( c == 'I' ) {            /* it's a numberic value so  */
      i = atoi (cr_MrtEqu); 
      if ( i >= 100 ) 
        strcpy (cr_Equ, e_BolCha);   /* it's a bole char equation */ 
      else
        strcpy (cr_Equ, e_CroSco); 
   }
 
   for ( i = 0; i < 1000; i++ ) {
     if (  !strcasecmp (sr_EFR[i].cr_MrtEqu,"") ) 
        break;
     if ( !strcasecmp (sr_EFR[i].cr_MrtEqu, cr_Equ ) ){  
       strcpy (cr_ReqFld, sr_EFR[i].cr_ReqFld); 
       return 1; } 

   }  /* for i */ 

   return 0; 
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: MRT_Calc
* Desc: Calculate the Probility of Mortality, Basal Area, Trees Killed
*        Canopy Cover. See the d_MO structure defines in fof_mt.h
*        This function is called with a Species, Density etc, see below.
*        Individual calcs are made for the species, the values are also
*         accumulated so that the caller can have accumlated totals and
*         averages, for Prob Mort, Tree Totals, Basal Area, Canopy Cover.
*
*        For example the FOFEM Bach programs uses this function. The Batch
*         program processes multiple Stands, when beginning a Stand it
*         initializes the MO struct to zero it out, then it begins calling
*         this function for every species in a particular Stand, values
*         continue to get accumulated and get averaged. When the Batch
*         program has a Stand change the Stand values are ready in the
*         MO struct. The MO can be initialize and the next Stand can be
*         started.
*        Running totals and averages are kept in the MO, thru the use of
*         values in MO and some global variables the values are kept
*         current so the caller doesn't have to do anything, just use
*         the totals and averages when they are ready.
*
* Note-1: We use to calculate these if they were missing but now we make
*         the user enter them, so when I clean up the code I just left
*         these old lines here, so I don't have to change the rest of code.
* Note-2: Equation 5 can get done in a varity of ways but that are all
*          considerated to be Eq 5 and report back to caller in d_MO as a 5
*          1. coastal plain with relative humidity
*          2. coastal plain no relative humidity
*          3. non-coastal plain uses previous versions equation 1, even
*              though we report back in d_MO we used eq 5
*
* Note-3: change 7-19-12 I add the compare for "L" because the batch file uses
*         "L" or "" - I could catch it when I read in the file and covert to
"         "Low" but I decided to just check for "L" here 
*   In: cr_Spe......Species                 - reguired
*       f_DBH.......Diameter Breast Height  - reguired
*       f_FS........Flame Length or Scorch Height  - reguired
*       cr_FS.......e_Flame, e_Scorch see defines in fof_mt.h - reguired
*       f_Hgt.......Height                  - reguired
*       f_CrnRat....Crown Ratio             - reguired
*       cr_FirSev..."Low" or "" Fire Severity, - reguired, you must set
*                     if to "Low" or ""
*  Out: cr_ErrMes...Error Message
* NOTE: Check for an Error by checking the cr_ErrMes, it will be "" if no
*        errors occured
*  Ret: percent of mortality
*       -1.0 if Error - see cr_ErrMes
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float  MRT_Calc  (d_MIS *a_MIS, d_MI *a_MI, d_MO *a_MO, char cr_ErrMes[])
{
double f,B, f_BT, f_DBHcm, f_Hgt, f_LCR, f_HCR, f_B,  f_CK, f_CSL, f_P, f_Fl, f_CH;
float  f_BlkHilFlaLen, f_CBH; 
float  f_Scorch,f_DBH, ff;
int  i_MortEqu;
char cr_Spe[25], cr_FS[10], cr_FirSev[10], cr[100];

 
   strcpy (cr_ErrMes,"");

   strcpy (cr_Spe, a_MIS->cr_Spe);           /* put here so easier to use    */
   f_DBH = a_MIS->f_DBH;
   strcpy (cr_FS, a_MIS->cr_FS);
   strcpy (cr_FirSev,a_MIS->cr_FirSev);
   strcpy (cr_ErrMes,"");

/* Assume we're given Scorch Height */
   f_Scorch = a_MIS->f_FS;
   f_BlkHilFlaLen = Calc_Flame(a_MIS->f_FS);   /* For Black Hills PiPo */

/* But if it's Flame Length - convert */ 
   if ( !xstrcmpi (cr_FS,e_Flame) ) {        /* if flame length              */
     f_Scorch = Calc_Scorch (a_MIS->f_FS);    /*  convert to scorch height    */
     f_BlkHilFlaLen = a_MIS->f_FS;            /*  save Fla Len for Black Hills PiPo */
   }

   else if (xstrcmpi (cr_FS,e_Scorch)){       /* check code                   */
     sprintf (cr_ErrMes,"Invalid Scorch/Flame Parameter (%s), use %s or %s", cr_FS, e_Flame, e_Scorch);
     return -1.0;}

/* NOTE NOTE - FuelCalc relies on the exact text "Species" being in this error message */
   if (SMT_GetIdx (cr_Spe) == -1) {          /* Chk for Valid Spe            */
     sprintf (cr_ErrMes,"Invalid Species: %s", cr_Spe);
     return -1.0;}

   f_BT = SMT_CalcBarkThick (cr_Spe,f_DBH,cr_ErrMes); /* Calc a Bark THickness       */

   f_Hgt = a_MIS->f_Hgt;                     /* Note-1                        */
   f_LCR = a_MIS->f_CR;  /* crown ratio */

   f_HCR = f_Hgt * ( f_LCR / 10.0 );
   f_B = f_Scorch - (f_Hgt - f_HCR);
   if ( f_B <= 0 )
     f_B = 0;
   if ( f_B > f_HCR )
     f_B = f_HCR;

/* Calc crown killed                                                         */
/*  f_CK = % Crown Volume Scorched, f_CSL = % Crown Height/Length Scorched   */
/*   % = 0 -> 100, scorched basically means killed                            */
   if ( f_HCR != 0 ) {
     f_CK = 100.0 * (f_B * (2.0 * f_HCR - f_B) / ( f_HCR * f_HCR ) );
     f_CSL = 100.0 * (f_B / f_HCR); }
   else {
     f_CK = 0;
     f_CSL = 0;
     strcpy (cr_ErrMes, "Mortality Calculaton is attempting to Divide by 0"); 
     return -1.0;}

/*...........................................................................*/
   i_MortEqu = atoi (a_MIS->cr_Eq);    /* Get equation number               */

   switch ( i_MortEqu ) {
                    
/*...................................................*/
/* Change - 8-20-2012 */
/* New formula from DL  */
/* we were having trouble with this, the original paper was  */
/* saying that proportion of crown scorch was 0->1 but we found */
/* that we had to use a value at 1->10   */
     case 5:  
       if ( f_CSL <= 0 ) { 
          f_P = 0;
          break; }
        f_DBHcm = InchToCent(f_DBH);
        f_BT = 0.435 + ( 0.031 * f_DBHcm ) ;
        f = f_CK / 10.0;                       /* see comments just above */
        B = 0.169 + ( 5.136 * f_BT) + (14.492 * Sqr(f_BT)) - (0.348 * Sqr(f));
        f_P = 1.0 / (1.0 + exp (B)); 
        sprintf (cr, "%20.18f ", f_P); 
        if ( f_P > 1.0 )               /* I don't know if these conditions could */ 
          f_P = 1.0;                   /* happen but I'm checking anyway  */ 
        if ( f_P < 0 ) 
          f_P = 0; 
        break;
/*...................................................*/ 

 //    case 5:  
     case 1:

 //      if ( i_MortEqu == 5 ) {          /* See Notes in PINPAL_Eq()   */
 //        if (PINPAL_Eq5(a_MI,f_CSL,f_BT,&f_P))/* If it was in Coast Plan and done  */
 //          break; }                     /* else fall thru use follwng eq     */

       if ( f_DBH >= 1 )
          f_P = 1.0 / (1.0 + exp(-1.941 + ( 6.316 * (1.0 - exp(-f_BT) ) ) - 0.000535 * (f_CK * f_CK) ) );
       else if ( f_CSL > 50 )
          f_P = 1;
       else if ( f_Hgt < 3 )
          f_P = 1;
       else {
          f_BT = SMT_CalcBarkThick (cr_Spe,1,cr_ErrMes);
          f_P = 1.0 / (1.0 + exp (-1.941 + (6.316 * (1.0 -  exp(-f_BT))) - 0.000535 * (f_CK * f_CK) ));
          f_P = f_P + (1.0-f_P) * (1.0 - ((f_Hgt - 3.0) / (((1.0/f_DBH) * f_Hgt) - 3.0))); }
       break;
     case 3:
          if ( f_DBH > 1 )
             f_P = 1.0 / (1.0 + exp (-1.941 + (6.316 * (1.0 - exp(-f_BT))) - 0.000535 * (f_CK * f_CK) ));
          else if ( f_CSL > 50 )
            f_P = 1.0;
          else if ( f_Hgt < 3 )
            f_P = 1.0;
          else {
            f_BT = SMT_CalcBarkThick (cr_Spe,1,cr_ErrMes);
            f_P = 1.0 / (1.0 + exp(-1.941 + (6.316 * (1.0 - exp(-f_BT))) - 0.000535 * (f_CK * f_CK)));
            f_P = f_P + (1.0 - f_P) * (1.0 - (( f_Hgt - 3.0) / (((1.0 / f_DBH) * f_Hgt) - 3.0))); }
          if ( f_P < 0.8)
             f_P = 0.8;
          break;

     case 4:
           f_Fl = Calc_Flame(f_Scorch);
           f_CH = f_Fl / 1.8 ;
           if ( !xstrcmpi (cr_FirSev,"Low") || !xstrcmpi (cr_FirSev,"L" )) {        /* Fire Severity - See Note-3 Above     */
             f_P = 1.0 / (1.0 + exp( (0.251 * f_DBH * 2.54) - (0.07 * f_CH * 2.54 * 12.0) - 4.407) ); }
           else
             f_P = 1.0 / (1.0 + exp( (0.0858 * f_DBH * 2.54) - (0.118 * f_CH * 2.54 * 12.0) - 2.157) );
          break;

/* test-pfi */
/* New equations from Sharon - april/may 2008                                */
     case 10:
          f_P = Whitefir (f_CSL); /* ABICON                            */
          break;

     case 11:
          f_P = SubalpineFir (f_CK);    /* ABILAS                            */
          break;

     case 12:
          f_P = IncenseCedar (f_CSL);   /* LIBDEC                            */
          break;

     case 14:
          f_P = WesternLarch (f_CK,f_DBH);    /* LAROCC                            */
          break;

     case 15:
          f_P = EngelmannSpruce (f_CK); /* PICENG                            */
          break;

     case 16:
          f_P = RedFir (f_CSL);         /*  ABIMAG                          */
          break;

     case 17:
          f_P = WhitebarkPine (f_CK,f_DBH); /*  PINALB                      */
          break;

     case 18:
          f_P = SugarPine (f_CSL);
          break;  

     case 19:
          f_P = PonderosaJeffreyPine(f_CK);
          break;

     case 20:
          f_P = DouglasFir(f_CK);
          break;

/* Change 9-6-2016 - new Black Hills PiPo, see DL's .docx document saved in fofem project folder */ 
     case 21:    
          f = a_MIS->f_CR / 10.0;  /* crown ratio */
          f_CBH = a_MIS->f_Hgt - (a_MIS->f_Hgt * f);       
          f_P = Eq21_BlkHilPiPo (a_MIS->f_Hgt, f_CBH, a_MIS->f_DBH, f_Scorch,  f_BlkHilFlaLen, &ff);
          break; 

     default:
        sprintf (cr_ErrMes, "Equation Not implemented,  Equ Num: %d\n", i_MortEqu);
        f_P = 0;
        return -1.0; 
        break;
   }  /* switch end */

  a_MIS->f_Prop = f_P;  /* will also get put into a_MO as whole int value */ 

  MRT_Total (a_MIS, a_MO, f_P);           /* Accum and Calc, avergest etc.     */

  return f_P;
}


/******************************************************************
* Name: Eq21_BlkHilPiPo
* Desc: Black Hills Ponderosa Pine PiPo
*
* (1) Seedlings (Height <1.37 m (4.5 ft)):
* P(m) = 1/1 + EXP(-(2.714 + (4.08 * flame length) + (-3.63 * height))) 
*
* (2) Saplings (Height >1.37 m (4.5 ft) and DBH <10.2 cm (4 in.)):
* P(m) = 1/1 + EXP(-(-0.7661 + (2.7981 * flame length) + (-1.2487 * height)
*
*  (3) Other trees (DBH >10.2 (4 in.)):
*  P(m) = 1/1 + EXP(-(1.104 + (DBH * -0.156) + (0.013* pct live crown length scorched) + (0.001 * DBH * pct live crown length scorched)))
*
* pct live crown length scorched =
*  ((max height of crown length scorched � canopy base height)/(tree height � canopy base height)) *100
*
*  In: hgt.... Tree height - feet
*      cbh.... crown base height - feet
*      dbh.... dia breast height - inch
*      scorch. scorch height - feet 
*      flalen  flame length - feet 
* Out: af....the calculate mortality
* Ret: the mortality 0 -> 1.0 - this value is check to see if it
*      exceeds the 0.5 point, if so it is consided dead, in which 
*      case 1.0 is returned
******************************************************************/
float Eq21_BlkHilPiPo (float hgt, float cbh, float dbh, float scorch, float flalen, float *af)
{
float f,g, f_Hgt, f_DBH,  f_FlaLen, f_CBH, f_CLS, f_Scorch;  

  f_Hgt =     FtToMt (hgt); 
  f_FlaLen =  FtToMt (flalen); 
  f_DBH =     InchToCent (dbh);
  f_Scorch =  FtToMt (scorch); 
  f_CBH =     FtToMt (cbh); 

/* 1-27-2017  Ticket-106, make sure scorch hit crown, to f_CLS doesn't get a negative value - */
/* Calc the percent of the crown lenght that gets scorched */ 
  if ( f_Scorch > f_CBH ) {                          /* make sure scorch hits the crown */
    f_CLS = (f_Scorch - f_CBH) / ( f_Hgt - f_CBH );  /* (length of crown scorched) / ( length of crown)  */
    f_CLS = f_CLS * 100.0; }  
  else 
    f_CLS = 0; 


  if ( f_Hgt < 1.37 )  {                     /* Seedling < 4.5 feet, 1.37 meter */
    g = exp(-(2.714 + (4.08 * f_FlaLen) + (-3.63 * f_Hgt))); 
    f = 1.0 / ( 1.0 + g); }  

  else if ( f_Hgt >= 1.37 && f_DBH < 10.2 ) {    /* Sapling >= 4.5 feet (1.37 meter) and < 4 inch (10.2cm) DBH*/
    g = exp(-(-0.7661 + (2.7981 * f_FlaLen) + (-1.2487 * f_Hgt)));
    f = 1.0 / (1.0 + g); }
 
/* Other trees (DBH >10.2 (4 in.)) */ \
  else {
    g = exp(-(1.104 + (f_DBH * -0.156) + (0.013 * f_CLS) + (0.001 * f_DBH * f_CLS)));
    f = 1.0 / ( 1.0 + g ); }

 *af = f; 

// 2-6-17 Ticket-106, DL said take this out, not sure why we had it in here 
 // if ( f >= 0.5 )
 //   f = 1.0; 

  return f; 
}




/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: FtToMt
* Desc: Convert Feet to Meters
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float  FtToMt (float Ft)
{
float f;
   f = Ft * 0.3048;
   return f;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: InchToCent
* Desc: Convert Inches to Centimeters
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
//float InchToCent (float f_Inch)
//{
 //  return ( f_Inch * 2.54 );
//}



/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: EngelmannSpruce       PICENG
* Desc: Pre-fire regular fofem mortality
*       New Eq from Sharon Hood, April 2008
* NOTE: See where this function is called from to get its Eq number
*   In: cs....crown VOLUME scorched percent
*  Ret: mortality probability 0 -> 1.0
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float EngelmannSpruce  (float cs)
{
float f;
     f = 1.0 / ( 1.0 + exp( - (0.0845 + ( cs * 0.0445 ) )));
  return f;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: WesternLarch    LAROCC
* Desc: Pre-fire regular fofem mortality
*       New Eq from Sharon Hood, April 2008
* NOTE: See where this function is called from to get its Eq number
*   In: cs....crown VOLUME scorched percent
*  Ret: mortality probability 0 -> 1.0
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float WesternLarch (float cs, float dbh)
{
float f;
     f = 1.0 / ( 1.0 + exp( - ( -1.6594 +
        ( cs * 0.0327 ) -
        ( dbh * 0.1241) )) );

  return f;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: IncenseCedar     LIBDEC
* Desc: Pre-fire regular fofem mortality
*       New Eq from Sharon Hood, April 2008
* NOTE: See where this function is called from to get its Eq number
*   In: cs....crown LENGTH scorched percent
*  Ret: mortality probability 0 -> 1.0
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float IncenseCedar (float cs)
{
float f;
     f = 1.0 / ( 1.0 + exp( - ( -4.2466 +
     ( pow((float)cs,(float)3.0) * 0.000007172 ) )));
  return f;
}




/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SugarPine
* Desc: Pre-fire regular fofem mortality
*       New Eq from Sharon Hood, April 2008
* NOTE: See where this function is called from to get its Eq number
*   In: cs....crown LENGTH scorched percent
*  Ret: mortality probability 0 -> 1.0
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float SugarPine (float cs)
{
float f;
     f = 1.0 / ( 1.0 + exp( - ( -2.0588 +
        ( pow((float)cs,(float)2.0) * 0.000814 ) )));

  return f;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: PonderosaJeffreyPine
* Desc: Pre-fire regular fofem mortality
*       New Eq from Sharon Hood, April 2008
* NOTE: See where this function is called from to get its Eq number
*   In: cs....crown VOLUME scorched percent
*  Ret: mortality probability 0 -> 1.0
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float PonderosaJeffreyPine (float cs)
{
float f;
     f = 1.0 / ( 1.0 + exp ( - ( -2.7103 +
        ( pow((float)cs,(float)3.0) * 0.000004093) )));

  return f;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: DouglasFir
* Desc: Pre-fire regular fofem mortality
*       New Eq from Sharon Hood, April 2008
* NOTE: See where this function is called from to get its Eq number
*   In: cs....crown VOLUME scorched percent
*  Ret: mortality probability 0 -> 1.0
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float  DouglasFir (float cs)
{
float f;
     f = 1.0 / ( 1.0 + exp ( - ( -2.0346 +
        ( cs * 0.0906) -
        ( pow((float)cs,(float)2.0) * 0.0022) +
        ( pow((float)cs,(float)3.0) * 0.000019) )));

  return f;
}



/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: WhitebarkPine  PINALB
* Desc: Pre-fire regular fofem mortality
*       New Eq from Sharon Hood, April 2008
* NOTE: See where this function is called from to get its Eq number
*   In: cs....crown VOLUME scorched percent
*       f_dbh....diam breast height
*  Ret: mortality probability 0 -> 1.0
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float WhitebarkPine (float cs, float dbh)
{
float f;
     f = 1.0 / ( 1.0 + exp( - ( -0.3268 +
        ( cs * 0.1387 ) -
        ( pow((float)cs,(float)2.0) * 0.0033 ) +
        ( pow((float)cs,(float)3.0) * 0.000025 ) -
        ( dbh * 0.0676) )) );
  return f;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: RedFir    ABIMAG
* Desc: Pre-fire regular fofem mortality
*       New Eq from Sharon Hood, May 2008
* NOTE: See where this function is called from to get its Eq number
*   In: cs....crown LENGHT scorched percent
*  Ret: mortality probability 0 -> 1.0
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float RedFir  (float cs)
{
float f;
   f = 1.0 / (1.0 + exp( - ( -2.3085 +
       ( pow ((float)cs,(float)3.0) * 0.000004059) )) );
  return f;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SubalpineFir     ABILAS
* Desc: Pre-fire regular fofem mortality
*       New Eq from Sharon Hood, April 2008
* NOTE: See where this function is called from to get its Eq number
*   In: cs....crown VOLUME scorched percent
*       DBH...dbh inch
*  Ret: mortality probability 0 -> 1.0
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float SubalpineFir (float cs)
{
float f;
// printf ("Vol Kill: %6.2f\n",cs);
     f = 1.0 / ( 1.0 + exp( - ( -1.6950 +
     ( cs * 0.2071 ) -
     ( pow((float)cs,(float)2.0) * 0.0047 ) +
     ( pow((float)cs,(float)3.0) * 0.000035 ) )));
  return f;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Whitefir   ABICON
* Desc: Pre-fire regular fofem mortality
*       New Eq from Sharon Hood, April 2008
* NOTE: See where this function is called from to get its Eq number
*   In: cs....crown LENGHT scorched percent
*  Ret: mortality probability 0 -> 1.0
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float Whitefir (float cs)
{
float f;
   f = 1.0 / (1.0 + exp( - ( -3.5083 +
       ( cs * 0.0956) -
       ( pow ((float)cs,(float)2.0) * 0.00184 ) +
       ( pow ((float)cs,(float)3.0) * 0.000017 ) ) ) );
  return f;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _PINPAL_Eq5
* Desc: PINPAL longleaf pine uses a special mortality eq developed by
*        Geoff Wang in Coastal Plain, Equation number 5
*       The equation only applys to PINPALs in Coastal Plains of the
*        Southeast.
* NOTE: See Documentation in CoastPln sub-directory, Had a bunch of trouble
*        with Geoff & Steve sending me stuff that didn't work and changing
*        things - So I don't have a lot of confidence in this stuff but
*        it's what they gave me.
*
*   In: a_MI->cr_CP..."Y" means we are in a coastal plain of the Southeast
*       a_MI->f_RelHum...0 means the rel hum isn't known
*                        1->100 rel hum
*       f_CSL.....percent of crown that gets scorched 0 -> 100
*       f_BT......Bark thickness in inches, this function will convert it
*
* NOTE: NOTE: Inputs (RelHum, CSL, BT) get units changed as needed
*
*  Out: *af_P.....probability of mortality NOTE this number will still need
*                  to be checked to see if tree actually gets killed
*  Ret: 1 calc was do,
*       0 calc not done because not in Coastal Plain
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int PINPAL_Eq5 (d_MI *a_MI, float f_CSL, float f_BT, double *af_P)
{
float f,SCH, f_P, f_BTc, f_RelHum;

  if ( strcmp (a_MI->cr_CP,"Y"))        /* If Not Coastal Plain            */
    return 0;                           /* return, call will deal with it    */

  SCH = f_CSL / 100.0;                  /* Need as a decimal percent         */
  f_RelHum = a_MI->f_RelHum / 100.0;
  f_BTc = InchToCent (f_BT);            /* to centimeters                    */

/* Now for PINPAL in the southeast coastal plain without Rel Humidity input  */
  if ( a_MI->f_RelHum == e_RHNonen){   /* No Rel Hum entered by user         */
    f =  -1.507 + (4.45 * SCH) - (4.309 * Sqr(SCH)) + (12.7888 * f_BTc) ;
    f_P = 1.0 / (1.0 + exp (f)) ;
    *af_P = f_P;
/*    printf ("P: %f, SCH: %f, BTc: %f  \n", f_P,SCH, f_BTc ); */
    return 1;  }


/* With Relative Humidity input.............                                 */
   f = -2.191 + (6.482*Sqr(f_RelHum) + (30.076*Sqr(f_BTc)));
   f_P = 1.0 / (1.0 + exp (f)) ;
  *af_P = f_P;
/*  printf ("P = %f,  BTc = %3.2f,   RH = %3.2f \n", f_P,  f_BTc, f_RelHum); */
   return 1;


}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: MRT_Total
* Desc: Do individual species and stand level calcs and totals.
*       This function calculations the stand level values everytime it
*        is called and plugs them into the MO structure so that they
*        are always available to the caller.
*       PLEASE see notes in calling function.
* Note-1: change 11-05--5
*        I caught this div by 0 bug when I was test batch input files,
*        the only time accumulate total killed 'a_MO->f_TotKilled' will
*        be 0 is when nothing has been killed on the stand yet in the
*        course of processing each tree record, this div by 0 inparticular
*        will happen when the first tree record for the stand has nothing
*        killed (no mortality)

*   In: a_MIS....Mortality Inputs
*       f_Prob..Probability of Mortality for the species
*  Out: a_MO....Mortalitity Output struct
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  MRT_Total (d_MIS *a_MIS, d_MO *a_MO, float f_Prob)
{
float f;

/* Individual Calcs..........................................................*/
   a_MO->f_Mort = f_Prob * 100.0; /* Individual Species Mort */
   a_MO->f_Killed = f_Prob * a_MIS->f_Den;    /* get # trees killed      */
   a_MO->f_Killed += 0.5;
   a_MO->f_Killed = (int) a_MO->f_Killed;
   if ( a_MO->f_Killed > a_MIS->f_Den )
      a_MO->f_Killed = a_MIS->f_Den; 

/* Individual Basal Area................                                     */
   a_MO->f_BasPre = Basal_Area(a_MIS->f_DBH,a_MIS->f_Den); /* Tot Basal Area              */
   a_MO->f_BasKil = Basal_Area(a_MIS->f_DBH,a_MO->f_Killed);     /* Killed basal area           */
   a_MO->f_BasPos = a_MO->f_BasPre - a_MO->f_BasKil;

   a_MO->f_BasPre = a_MO->f_BasPre / 144.0;  /* Get Sqr inches to Sqr Feet   */
   a_MO->f_BasKil = a_MO->f_BasKil / 144.0;
   a_MO->f_BasPos = a_MO->f_BasPos / 144.0;

/* Basal Area Totals .................................                               */
   a_MO->f_BasTotPre += a_MO->f_BasPre;      /* Total Basal Areas            */
   a_MO->f_BasTotKil += a_MO->f_BasKil;
   a_MO->f_BasTotPos += a_MO->f_BasPos;

/* Canopy Cover .............................................................*/
   f = SMT_CalcCrnCov (a_MIS->cr_Spe,          /* Crwn Cover Area for 1 tree   */
       a_MIS->f_DBH, a_MIS->f_Hgt);

   a_MO->f_CovPreLiv = f * a_MIS->f_Den;      /* Crwn Cov area * number trees */
   a_MO->f_CovPosLiv = a_MO->f_CovPreLiv - (f * a_MO->f_Killed);

   gf_CovTotPreLiv += a_MO->f_CovPreLiv;   /* Need to accum these before doing*/
   gf_CovTotPosLiv += a_MO->f_CovPosLiv;   /*  overlap                        */

   a_MO->f_CovPreLiv = MRT_Overlap (a_MO->f_CovPreLiv);
   a_MO->f_CovPosLiv = MRT_Overlap (a_MO->f_CovPosLiv);

/* Canopy Cover Totals..........................                                    */
   a_MO->f_CovTotPreLiv = MRT_Overlap (gf_CovTotPreLiv);
   a_MO->f_CovTotPosLiv = MRT_Overlap (gf_CovTotPosLiv);
   a_MO->f_CovTotPreLiv = (int) (a_MO->f_CovTotPreLiv + 0.5);
   a_MO->f_CovTotPosLiv = (int) (a_MO->f_CovTotPosLiv + 0.5);

/* Note: I do this here but not on the actual FOFEM6 Mortality Report, so    */
/*  there can be a slight difference between them                            */
/* The FOFEM6 report does some of its own calcs, totals, averages and        */
/*  use the ones in the MO struct                                            */
   if ( a_MO->f_CovTotPreLiv < 1.0 )         /* Show at least 1 percent      */
      a_MO->f_CovTotPreLiv = 1;
   if ( a_MO->f_CovTotPosLiv < 1.0 )
      a_MO->f_CovTotPosLiv = 1;

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* Accumulate for Stand                                                      */
   a_MO->f_TotPreTree += a_MIS->f_Den;         /* Total Prefire trees         */

   gf_TotMort += f_Prob * 100.0;               /* Tot Prob of Mort            */
   gfN_TotMort++;                            /* count for averaging         */
   a_MO->f_AvgMort = (int)((gf_TotMort / gfN_TotMort) + 0.5);

   if ( a_MIS->f_DBH >= 4.0 ) {               /* Average Prob of Mort on      */
     gf_4TotMort += f_Prob * 100.0;        /*  trees over 4 inches         */
     gfN_4TotMort++;
     a_MO->f_4AvgMort = (int) ((gf_4TotMort / gfN_4TotMort) + 0.5); }


   a_MO->f_TotKilled += a_MO->f_Killed;       /* total up                    */
   gf_TotKilDBH += a_MIS->f_DBH * a_MO->f_Killed;
/* Change 11-5-05 - Fix to catch divide by 0 - See Note-1 above              */
   if ( a_MO->f_TotKilled != 0 )
      a_MO->f_AvgDBHKilled = gf_TotKilDBH / a_MO->f_TotKilled;

}




/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: MO_Init
* Desc: Zeros out all variables need to do Mortality running totals
*        and averages.
*       See Notes in heading of MT_Calc()
* NOTE: the global variable that get init too.
* In/Out: a_MO....Mortality Output struct
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  MO_Init (d_MO *a_MO)
{
   a_MO->f_TotPreTree = 0;
   a_MO->f_Mort   = 0;
   a_MO->f_Killed = 0;
   a_MO->f_BasPre = 0;
   a_MO->f_BasKil = 0;
   a_MO->f_BasPos = 0;
   a_MO->f_CovPreLiv = 0;
   a_MO->f_CovPosLiv = 0;

/* Stand Level Totals,Averages                                                */
   a_MO->f_AvgMort = 0;
   a_MO->f_4AvgMort = 0;
   a_MO->f_TotKilled = 0;
   a_MO->f_AvgDBHKilled = 0;
   a_MO->f_BasTotPre = 0;
   a_MO->f_BasTotKil = 0;
   a_MO->f_BasTotPos = 0;
   a_MO->f_CovTotPreLiv = 0;
   a_MO->f_CovTotPosLiv = 0;

   strcpy (a_MO->cr_MortEqu,"");

/* Global Static Totals                                                      */
   gf_TotMort = 0;
   gfN_TotMort = 0;
   gf_4TotMort = 0;
   gfN_4TotMort = 0;
   gf_TotKilDBH = 0;
   gf_CovTotPreLiv = 0;
   gf_CovTotPosLiv = 0;
}



/*******************************************************************
********************************************************************/
d_SMT  smt;
char gcr[100]; 

d_CDF sr_Spp[] = {
   { "Seq Num",     e_Str, 100, gcr},
   { "cr_Spe",      e_Str,  30, smt.cr_Spe  },
   { "cr_Name",     e_Str, 200, smt.cr_Name }, 
   { "cr_ComNam",   e_Str, 200, smt.cr_ComNam }, 
 
   { "cr_MrtEqu",   e_Str,  20, smt.cr_MrtEqu }, 
   { "i_BrkEqu",    e_Int,   0, &smt.i_BrkEqu }, 
   { "i_No",        e_Int,   0, &smt.i_No },
 
   { "i_Reg1",    e_Int,   0, &smt.i_Reg1 }, 
   { "i_Reg2",    e_Int,   0, &smt.i_Reg2 }, 
   { "i_Reg3",    e_Int,   0, &smt.i_Reg3 }, 
   { "i_Reg4",    e_Int,   0, &smt.i_Reg4 }, 
 


//   { "cr_PFIEqu",  e_Str,  10, smt.cr_PFIEqu  },
//   { "cr_PFIO",    e_Str,   10, smt.cr_PFIO }, 

 // { "cr_PFI",    e_Str,  20, smt.cr_PFIEqu}, 

 //  { "cr_PFIL",  e_Str,  10, smt.cr_PFIO}, 

 
   { "",           e_End,   5,   NULL }  };


/*********************************************************************
* Name: MRT_LoadSpeCSV 
* Desc: Read in the FOFEM species mortality equation file fof_spp.csv
*       This reads in the .csv file, and replaces the old function 
*       that read the .dat file. 
*
**********************************************************************/
int MRT_LoadSpeCSV (char cr_Pth[], char cr_ErrMes[])
{
int i,j;
float f; 
char cr[2000]; // R1[20], R2[20], R3[20], R4[20]; 
FILE *fh;
d_CDF  *a;
d_CCT s_CCT;

   strcpy (cr_ErrMes,"");
   a = &sr_Spp[0]; 
   SMT_InitTable();

   if ( !strcmp (cr_Pth,"")) {         /* Load table using hard coded       */
     MRT_InitST();                    /*  default table                    */
     return 1; }

   fh = fopen (cr_Pth, "r" );
   if ( fh == NULL ) {
     sprintf (cr_ErrMes, "Can't Open Input FOFEM Mortality Species Equation File:\n %s \n", cr_Pth);
     strcat  (cr_ErrMes, "Check path and file name.");
     return 0;}

   for ( i = 0; i < eC_smt; i++ ) {
     j = CDF_Get (a,fh,cr);
     if ( j == 0 )        /* End of File */ 
       break;
     if ( j == -1 ) {
       sprintf (cr_ErrMes, "Invalid file format detected\n%s\n%s\n Approximate row # %d",cr, cr_Pth,i);
       fclose (fh);
       return 0; }

      strcpy (sr_SMT[i].cr_Spe,smt.cr_Spe );
      xstrupr(sr_SMT[i].cr_Spe );

      strcpy (sr_SMT[i].cr_Name, smt.cr_Name);
      Trim_LT(sr_SMT[i].cr_Name );

      strcpy (sr_SMT[i].cr_ComNam, smt.cr_ComNam); 
      Trim_LT (sr_SMT[i].cr_ComNam); 

      strcpy (sr_SMT[i].cr_MrtEqu, smt.cr_MrtEqu); 
      sr_SMT[i].i_BrkEqu = smt.i_BrkEqu;  
      sr_SMT[i].i_No = smt.i_No; 
      if ( !CCT_Get (sr_SMT[i].i_No, &s_CCT)) {
         sprintf (cr_ErrMes,"Species: %s, in File:%s, has invalid Crown Equation Code (%d)", sr_SMT[i].cr_Spe, cr_Pth,sr_SMT[i].i_No);
         return 0; }

      sr_SMT[i].i_Reg1 = smt.i_Reg1;      
      sr_SMT[i].i_Reg2 = smt.i_Reg2;      
      sr_SMT[i].i_Reg3 = smt.i_Reg3;      
      sr_SMT[i].i_Reg4 = smt.i_Reg4;   

//      strcpy (sr_SMT[i].cr_PFIEqu, smt.cr_PFIEqu);
//      strcpy (sr_SMT[i].cr_PFIO, smt.cr_PFIO);

      f = SMT_CalcBarkThick (sr_SMT[i].cr_Spe, (float)1,cr_ErrMes);/* Note-1      */
      if ( f == -1 ) {
        sprintf (cr_ErrMes,"Species: %s, in File:%s, has invalid Bark Eq. number",
              sr_SMT[i].cr_Spe, cr_Pth);
        return 0; }


 
      if (sr_SMT[i].i_Reg1 == -1 &&
          sr_SMT[i].i_Reg2 == -1 &&
          sr_SMT[i].i_Reg3 == -1 &&
          sr_SMT[i].i_Reg4 == -1 ) {
        sprintf (cr_ErrMes,"Species: %s, in File:%s, has no Region",
              sr_SMT[i].cr_Spe, cr_Pth);
        return 0; }

   } /* for i */ 

   fclose (fh); 

//  _DumpIt ();

   return 1; 
} 

/*---------------------------------------------------------------------
* Used to create new 6.5 Version of fof_spp.csv file
*  
*/
void  _DumpIt ()
{
int i, N; 
FILE  *fh;

   fh = fopen ("FOF_SPP-WOW.csv", "w" );
   if ( fh == NULL ) 
     return ;
   N = 10; 
   for ( i = 0; i < 100000; i++ ) {
      if ( !strcmp (sr_SMT[i].cr_Spe,"") ) 
         break;
      if ( !strcasecmp (sr_SMT[i].cr_PFIO, "P" ))
         goto PFI;

     fprintf (fh, "%d,%s,%s,%s,%s,%d,%d,%d,%d,%d,%d\n",  
        N,
        sr_SMT[i].cr_Spe,
        sr_SMT[i].cr_Name,
        sr_SMT[i].cr_ComNam,
        sr_SMT[i].cr_MrtEqu,
        sr_SMT[i].i_BrkEqu,
        sr_SMT[i].i_No,      /* crown Eq code */
        sr_SMT[i].i_Reg1,
        sr_SMT[i].i_Reg2,
        sr_SMT[i].i_Reg3,
        sr_SMT[i].i_Reg4);
        N += 10; 

    if ( !strcmp (sr_SMT[i].cr_PFIEqu, ""))
       continue;  
PFI:
     fprintf (fh, "%d,%s,%s,%s,%s,%d,%d,%d,%d,%d,%d\n",  
        N, 
        sr_SMT[i].cr_Spe,
        sr_SMT[i].cr_Name,
        sr_SMT[i].cr_ComNam,
        sr_SMT[i].cr_PFIEqu,
        sr_SMT[i].i_BrkEqu,
        sr_SMT[i].i_No,     /* crown Eq code */
        sr_SMT[i].i_Reg1,
        sr_SMT[i].i_Reg2,
        sr_SMT[i].i_Reg3,
        sr_SMT[i].i_Reg4); 
        N += 10; 

 }

  fclose (fh); 

}


/*****************************************************
*
******************************************************/
int _CnvReg (int i_Reg, char cr_Reg[])
{
  strcpy (cr_Reg,""); 
  if ( i_Reg <= 0 ) 
     return 0; 
  sprintf (cr_Reg,"%d",i_Reg);
  return 1; 
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: MRT_LoadSpe
* Desc: This is the old function to read in the fof_spp.dat file, we now
*       use a fof_spp.csv file.
* NOTE: I just spliced in the call to the new function here to make things
*        easier for now. 
*       >> the cr_Ver[] is obsolete 
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int   MRT_LoadSpe (char cr_Pth[],char cr_Ver[],  char cr_ErrMes[])
{
int i; 
/* 7-18-2016 - modify to read in fof_spp.dat file as a .CSV file */
/*  I just dropped the new function call in here so all calls */
   i =  MRT_LoadSpeCSV (cr_Pth, cr_ErrMes);
   return i; 



#ifdef OLDCODE
int i, iX;
float f;
FILE *fh;
#define ec_line 300
char cr_Line [ec_line], *a;
d_CCT s_CCT;



   strcpy (cr_ErrMes,"");

   iX = 0;
    SMT_InitTable();

    if ( !strcmp (cr_Pth,"")) {         /* Load table using hard coded       */
       MRT_InitST();                    /*  default table                    */
       return 1; }

    fh = fopen (cr_Pth, "r" );
    if ( fh == NULL ) {
      sprintf (cr_ErrMes, "Can't Open Input Species File: %s\n", cr_Pth);
      strcat  (cr_ErrMes, "Check path and file name.");
      return 0;}

    for ( i = 0; i < eC_smt; i++ ) {
      a = fgets (cr_Line, ec_line, fh );     /* Read a line from file        */
        if ( a == NULL ) {                   /* End of File                 */
           fclose (fh);
           return iX; }                        /* Hum didn't find rec we want */

      Blk_End_Line (cr_Line,ec_line);              /* Blank to end of line   */
      if ( isBlank (cr_Line) )                     /* Watch for empty lines  */
        continue;                                  /*  skip them             */


/* Make sure Version number in file match that of FOFEM */
      if ( i == 0 && strcmp (cr_Ver,"")  ) {  /* First line of file     */
        if ( !strstr (cr_Line,cr_Ver) ) {
          sprintf (cr_ErrMes,"Incorrect version number in %s file", cr_Pth);
          fclose (fh);
          return 0; }  }

      if ( cr_Line[0] == '#' )                     /* lines is a comment so  */
        continue;                                  /*  skip it               */
      ToStr (sr_SMT[iX].cr_Spe, cr_Line, 1, 7);
      Trim_LT(sr_SMT[iX].cr_Spe );
      xstrupr(sr_SMT[iX].cr_Spe );

      ToStr (sr_SMT[iX].cr_Name, cr_Line, 9, 54);
      Trim_LT(sr_SMT[iX].cr_Name );
      ToInt (&sr_SMT[iX].i_MrtEqu, cr_Line, 55, 56);
      ToInt (&sr_SMT[iX].i_BrkEqu, cr_Line, 58, 60 );
      ToInt (&sr_SMT[iX].i_Reg1, cr_Line, 62,62);
      ToInt (&sr_SMT[iX].i_Reg2, cr_Line, 63,63);
      ToInt (&sr_SMT[iX].i_Reg3, cr_Line, 64,64);
      ToInt (&sr_SMT[iX].i_Reg4, cr_Line, 65,65);

      ToInt (&sr_SMT[iX].i_No,cr_Line,67,68);/* Crown Equ. Code,             */
      if ( !CCT_Get (sr_SMT[iX].i_No, &s_CCT)) {
         sprintf (cr_ErrMes,"Species: %s, in File:%s, has invalid Crown Equation Code (%d)", sr_SMT[iX].cr_Spe, cr_Pth,sr_SMT[iX].i_No);
         return 0; }

      ToStr (sr_SMT[iX].cr_PFIEqu, cr_Line, 70, 71);
      Trim_LT(sr_SMT[iX].cr_PFIEqu);

      ToStr (sr_SMT[iX].cr_PFIO, cr_Line, 73, 73);
      Trim_LT(sr_SMT[iX].cr_PFIO);

      f = SMT_CalcBarkThick (sr_SMT[iX].cr_Spe, (float)1,cr_ErrMes);/* Note-1      */
      if ( f == -1 ) {
        sprintf (cr_ErrMes,"Species: %s, in File:%s, has invalid Bark Eq. number",
              sr_SMT[iX].cr_Spe, cr_Pth);
        return 0; }

      if ( sr_SMT[iX].i_MrtEqu != 1  &&
           sr_SMT[iX].i_MrtEqu != 3  &&
           sr_SMT[iX].i_MrtEqu != 5  &&
           sr_SMT[iX].i_MrtEqu != 4  &&
/* test-pfi */
           sr_SMT[iX].i_MrtEqu != 10 &&
           sr_SMT[iX].i_MrtEqu != 11 &&
           sr_SMT[iX].i_MrtEqu != 12 &&
           sr_SMT[iX].i_MrtEqu != 14 &&
           sr_SMT[iX].i_MrtEqu != 15 &&
           sr_SMT[iX].i_MrtEqu != 16 &&
           sr_SMT[iX].i_MrtEqu != 17 &&
           sr_SMT[iX].i_MrtEqu != 18 &&
           sr_SMT[iX].i_MrtEqu != 19 &&
           sr_SMT[iX].i_MrtEqu != 20 ){
/* test-pfi */

        sprintf (cr_ErrMes,"Species: %s, in File:%s, has invalid Mortality Eq. number",
              sr_SMT[iX].cr_Spe, cr_Pth);
        return 0; }

      if (sr_SMT[iX].i_Reg1 == -1 &&
          sr_SMT[iX].i_Reg2 == -1 &&
          sr_SMT[iX].i_Reg3 == -1 &&
          sr_SMT[iX].i_Reg4 == -1 ) {
        sprintf (cr_ErrMes,"Species: %s, in File:%s, has no Region",
              sr_SMT[iX].cr_Spe, cr_Pth);
        return 0; }

      iX++; }

 strcpy (cr_ErrMes,"Too Many Species in Species File");

 return 0;

#endif 

}
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SMT_InitTable
* Desc: Initialize the empty table
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  SMT_InitTable ()
{
int i;
  for ( i = 0; i < eC_smt -1; i++ ) {
    strcpy (sr_SMT[i].cr_Spe,"");
    strcpy (sr_SMT[i].cr_Name,"");
    strcpy (sr_SMT[i].cr_ComNam,""); 
    strcpy (sr_SMT[i].cr_MrtEqu,""); 
    sr_SMT[i].i_BrkEqu = -1;
    sr_SMT[i].i_Reg1 = -1;
    sr_SMT[i].i_Reg2 = -1;
    sr_SMT[i].i_Reg3 = -1;
    sr_SMT[i].i_Reg4 = -1;
    sr_SMT[i].i_No = -1;
/* test-pfi */
    strcpy (sr_SMT[i].cr_PFIEqu,"");
    strcpy (sr_SMT[i].cr_PFIO,"");

  }
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SMT_NotImp
* Desc: Checks to see if any of the Equation numbers or the Crown
*        Ratio  is set to -1 meaning missing/not implented yet
*        I check for 0 or less just in case a 0 comes in, but the
*        fields get set to -1 for missing when the table is read in
*        from the species file.
*   In: iX.....index into table
*  Ret: 1 OK,    0 not implented
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int SMT_NotImp (int iX)
{
   if ( !strcmp (sr_SMT[iX].cr_MrtEqu,"")) 
         return 0;

   if ( sr_SMT[iX].i_BrkEqu <= 0 )
     return 0;
   return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SMT_Get
* Desc: Get a copy of a record in the table
*       Send in a iterative index to got all recs from table.
*   In: iX....index into table
*  Ret: 1 OK,    0 hit end record of table
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int SMT_Get (int iX,  d_SMT  *a_SMT)
{
  if ( !strcmp (sr_SMT[iX].cr_Spe,"") )
    return 0;
  memcpy (a_SMT,&sr_SMT[iX],sizeof(d_SMT));
  return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SMT_ChkReg
* Desc: See species is in a region,
*   In: a_SMT........Specie Master Record
*       i_Reg........Region
*  Ret: 1 Yes, Species is in region,   0 NO its not
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  SMT_ChkReg (d_SMT *a_SMT, int i_Reg)
{
   if      ( i_Reg == 1 && a_SMT->i_Reg1 == 1 )
     return 1;
   else if ( i_Reg == 2 && a_SMT->i_Reg2 == 2 )
     return 1;
   else if ( i_Reg == 3 && a_SMT->i_Reg3 == 3 )
     return 1;
   else if ( i_Reg == 4 && a_SMT->i_Reg4 == 4 )
     return 1;
   return 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SMT_ChkRegion
* Desc: See if the Species is in the regions
* Note-1: regions are marked by putting the region number in the region
*         variable, ahh not real elegant, but what's the diff
*   In: cr_Spe.......Species to locate
*       i_Reg........Region
*  Ret: 1 Yes, Species is in region,   0 NO its not
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int     SMT_ChkRegion ( char cr_Spe[],  int i_Reg)
{
int iX;
   iX = SMT_GetIdx (cr_Spe);                 /* get idx into tbl of spe      */
   if ( iX == -1 )                           /* Hum, didn't even find it     */
     return 0;
   if      ( i_Reg == 1 && sr_SMT[iX].i_Reg1 == 1 )    /* See Note-1         */
     return 1;
   else if ( i_Reg == 2 && sr_SMT[iX].i_Reg2 == 2 )
     return 1;
   else if ( i_Reg == 3 && sr_SMT[iX].i_Reg3 == 3 )
     return 1;
   else if ( i_Reg == 4 && sr_SMT[iX].i_Reg4 == 4 )
     return 1;
   return 0;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SMT_CalcBarkThick
* Desc: Calculate the Bark Thickness
*   In: cr_Spe....Species
*       f_DBH.....DBH
* Note-1: Equation(s) that do not use a bark thickness equation
*         Ex: PIPA2
*  Out: cr_ErrMes....Error Message
*  Ret: bark thinkness,
*      -1 if no equation
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float SMT_CalcBarkThick (char cr_Spe[], float f_DBH, char cr_ErrMes[])
{
int iX, i_BrkEqu;
float f,g;
   iX = SMT_GetIdx (cr_Spe);
   if ( iX < 0 ) {
     strcpy (cr_ErrMes,"Logic Error - Can't Find Species in function SMT_CalcBarkThick");
     return -1; }

   i_BrkEqu = sr_SMT[iX].i_BrkEqu;

   if      ( i_BrkEqu == 1  ) f = 0.019;
   else if ( i_BrkEqu == 2  ) f = 0.022;
   else if ( i_BrkEqu == 3  ) f = 0.024;
   else if ( i_BrkEqu == 4  ) f = 0.025;
   else if ( i_BrkEqu == 5  ) f = 0.026;
   else if ( i_BrkEqu == 6  ) f = 0.027;
   else if ( i_BrkEqu == 7  ) f = 0.028;
   else if ( i_BrkEqu == 8  ) f = 0.029;
   else if ( i_BrkEqu == 9  ) f = 0.03 ;
   else if ( i_BrkEqu == 10 ) f = 0.031;
   else if ( i_BrkEqu == 11 ) f = 0.032;
   else if ( i_BrkEqu == 12 ) f = 0.033;
   else if ( i_BrkEqu == 13 ) f = 0.034;
   else if ( i_BrkEqu == 14 ) f = 0.035;
   else if ( i_BrkEqu == 15 ) f = 0.036;
   else if ( i_BrkEqu == 16 ) f = 0.037;
   else if ( i_BrkEqu == 17 ) f = 0.038;
   else if ( i_BrkEqu == 18 ) f = 0.039;
   else if ( i_BrkEqu == 19 ) f = 0.04 ;
   else if ( i_BrkEqu == 20 ) f = 0.041;
   else if ( i_BrkEqu == 21 ) f = 0.042;
   else if ( i_BrkEqu == 22 ) f = 0.043;
   else if ( i_BrkEqu == 23 ) f = 0.044;
   else if ( i_BrkEqu == 24 ) f = 0.045;
   else if ( i_BrkEqu == 25 ) f = 0.046;
   else if ( i_BrkEqu == 26 ) f = 0.047;
   else if ( i_BrkEqu == 27 ) f = 0.048;
   else if ( i_BrkEqu == 28 ) f = 0.049;
   else if ( i_BrkEqu == 29 ) f = 0.05 ;
   else if ( i_BrkEqu == 30 ) f = 0.052;
   else if ( i_BrkEqu == 31 ) f = 0.055;
   else if ( i_BrkEqu == 32 ) f = 0.057;
   else if ( i_BrkEqu == 33 ) f = 0.059;
   else if ( i_BrkEqu == 34 ) f = 0.06 ;
   else if ( i_BrkEqu == 35 ) f = 0.062;
   else if ( i_BrkEqu == 36 ) f = 0.063;
   else if ( i_BrkEqu == 37 ) f = 0.068;
   else if ( i_BrkEqu == 38 ) f = 0.072;
   else if ( i_BrkEqu == 39 ) f = 0.081;
   else if ( i_BrkEqu == 100 )  /* See Note-1 above */  
     f = 0;
   else
     return -1;

   g = f * f_DBH;
   return g;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SMT_MortEqu
* Desc: Get Mortality Equation number
*       Not All equation codes are numeric, so this function 
*       should only be called when doing a Pre-Fire Mort equation
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int SMT_MortEqu (char cr_Spe[])
{
int i, iX;
   iX = SMT_GetIdx (cr_Spe);
   i = atoi (sr_SMT[iX].cr_MrtEqu) ;
   return i; 
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SMT_MortEqu_Code
* Desc: Get Mortality Equation Code for the specified Species and
*       Equation Type 
*   In: cr_Spe....Species Code
*       cr_EquTyp....Equation Type #define - see  fof_mec.h
*  Out: cr_MrtEqu.....Mortality Equation
*  Ret: 1 found, 0 not found 
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int SMT_MortEqu_Code (char cr_Spe[], char cr_EquTyp[], char cr_MrtEqu[])
{
int i;
char cr[100]; 

   strcpy (cr_MrtEqu,""); 
   for ( i = 0; i < 1000; i++ ) {
     if ( !strcasecmp (sr_SMT[i].cr_Spe,"" ) ) /* End of table */
       return 0;                            
     if ( strcasecmp (sr_SMT[i].cr_Spe, cr_Spe) ) 
       continue; 
/* if Species matches, now check equation type */
       _EquTyp (sr_SMT[i].cr_MrtEqu, cr);
        if ( !strcasecmp (cr_EquTyp, cr) ) {    
           strcpy (cr_MrtEqu, sr_SMT[i].cr_MrtEqu);
           return 1; }   
   }  /* for i */ 
  
   return 0; 
}


/************************************************************
* Name: _EquTyp 
* Desc: Look at a Mortality Code to determine what Equation
*       type it is;
*       1 -> 99  Are crown scorch (pre-fire) type
*       100 ->   are  bole char
*       alpha codes are crown damage 
*   In: Mortality Equation Code
*  Out: Equation Type - see #define fof_mec.h 
************************************************************/
int _EquTyp (char cr_MrtEqu[],char cr_EquTyp[])
{
int i; 
char c;

   c = Get_NumTyp (cr_MrtEqu);  /* see if Equ Code is numeric  */
   if ( c != 'I' ) {            /* Not numeric */
      strcpy (cr_EquTyp,e_CroDam); /* so it's a crown damage code */
      return 1; }   

/* it's a numberic code so  */
   i = atoi (cr_MrtEqu); 
   if ( i < 100 ) 
     strcpy (cr_EquTyp, e_CroSco);   /* it's a crown scorch */ 
   else
     strcpy (cr_EquTyp, e_BolCha);  /* bole char */
   
   return 1; 
}



/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SMT_PFIEqu
* Desc: Get Post Fire Injury Mortality Equation number for
*       Only some species have these eq codes assigned to them
*   In: cr_Spe.......species code
*  Out : cr_Eq.......Eqution code/num
*  Ret: 1 ok, 
*       0 species not found in Eq table,  
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
/* test-pfi */
int   SMT_PFIEqu (char cr_Spe[], char cr_Eq[])
{
int iX;
   strcpy (cr_Eq,""); 
   iX = SMT_GetIdx (cr_Spe);
   if ( iX < 0 ) 
     return 0;
   strcpy (cr_Eq,sr_SMT[iX].cr_PFIEqu);
   return 1; 
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SMT_isPFI
* Desc: See if the record has a Post Fire Injury Eq,
*   In: a_SMT...record
*  Ret: 1 Yes, 0 No
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
/* test-pfi */
int  SMT_isPFI (d_SMT *a_SMT)
{
   if ( !strcmp (a_SMT->cr_PFIEqu,""))
     return 0;
   return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SMT_GetIdx
* Desc: Look for Species in the SMT and return the table index
*   In: cr_Spe.......Species to locate
*  Ret: index of species in table
*       -1 if not found
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  SMT_GetIdx (char cr_Spe[])
{
int i;
char cr_tmp[100];
   strcpy (cr_tmp,cr_Spe);
   xstrupr (cr_tmp);                        /* got to upper case */

   for ( i = 0; i < 1000; i++ ) {
     if ( !strcmp (sr_SMT[i].cr_Spe, "") )
        break;
     if ( !strcmp (sr_SMT[i].cr_Spe, cr_tmp) )
       return i;
   }
   return -1;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SMT_DispTbl
* Desc: Display all the records in the SMT
*       Needed for testing
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void SMT_DispTbl ()
{
int i;
  for ( i = 0; i < 10000; i++ ) {
    if (sr_SMT[i].cr_Spe[0] == 0 )
       break;
    printf ("%8s, %60s, %2s, %2d,    Reg: %2d, %2d, %2d, %2d, %d \n",
        sr_SMT[i].cr_Spe,
        sr_SMT[i].cr_Name,
        sr_SMT[i].cr_MrtEqu,
        sr_SMT[i].i_BrkEqu,
        sr_SMT[i].i_Reg1,
        sr_SMT[i].i_Reg2,
        sr_SMT[i].i_Reg3,
        sr_SMT[i].i_Reg4,
        sr_SMT[i].i_No);  }
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SMT_CalcCrnCov
* Desc: Calculate the Crown Cover, based on the Crown Cover using the
*        coefficients in the CCT (Crown Cover Table).
*       Species in the fof_spp.dat file are assigned 2 char Crown Cover Codes
*        which are used to find the coefficients in the CCT Table
*
*   In: cr_Spe... Species codes
*       f_Dia.....Diamenter
*       f_Hgt.....Tree Height
*  Ret: Crown Cover - in square feet
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float  SMT_CalcCrnCov (char cr_Spe[], float f_Dia, float f_Hgt)
{
int iX;
float f,r,a;
d_CCT s_CCT;

   if ( f_Hgt <= 0 ) 
     return 0;
   if ( f_Dia <= 0 )
     return 0 ;  

   iX = SMT_GetIdx (cr_Spe);             /* Get index into Species table     */
   CCT_Get (sr_SMT[iX].i_No, &s_CCT);

/* Get Diameter of Crown using Coefficients                                  */
   if ( f_Hgt <= 4.5 )                       /* Small trees                  */
      f = s_CCT.f_r * f_Dia;
   else {                                    /* Large Trees                  */
      f = pow((float)f_Dia,(float)s_CCT.f_b); /* raise it to power            */
      f = f * s_CCT.f_a; }                   /*  and times                   */

/* Use Diameter of Crown to get Area..........                               */
   r = f / 2.0;                              /* Get diameter to radius       */
   r = r * r;                                /* pi r sqrd gets area of crwn  */
   a = 3.14159 * r ;
   return a;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CCT_Get
* Desc: Get the record from the Crown Cover Coefficient Table
*   In: i_No.....Fvs Species Index No to identify the coefficient record
*                    in the table.
*  Ret: 1 Code found,
*       0 Code not found, default last record returned
* NOTE: the last record of table has default coeff.
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int CCT_Get (int i_No, d_CCT *a_CCT)
{
int i;
   for ( i = 0; i < 500; i++ ) {
     if ( sr_CCT[i].i_No == -1 )  {                    /* hit end of table and not found */
       memcpy (a_CCT, &sr_CCT[i], sizeof(d_CCT));
       return 0; }

     if ( sr_CCT[i].i_No == i_No ) {
       memcpy (a_CCT, &sr_CCT[i], sizeof(d_CCT));
       return 1; }
 
   }

   return 0;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CCT_Display
* Desc:
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  CCT_Display (d_CCT *a_CTT)
{
  printf ("--------------CCT--------------------\n");
  printf ("%d, %s, %f, %f, %f\n",
 a_CTT->i_No,  a_CTT->cr_CC, a_CTT->f_a, a_CTT->f_b, a_CTT->f_r);
  printf ("--------------CCT--------------------\n");

}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: MRT_InitST
* Desc: Load the Species Mortality in Memory Table.
*       The SMT table can either get load from the fof_spp.dat file
*        of from the Memory Table
*
*   Old Function the load the Internal FOFEM mortality equation table
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  MRT_InitST ()
{
int i;
  for ( i = 0; i < 10000; i++ ) {
    memcpy (&sr_SMT[i], &sr_MSMT[i], sizeof (d_SMT));
    if ( !strcmp (sr_SMT[i].cr_Spe,"") )
      break;
  }
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: MRT_Overlap
* Desc: Given a square foot amount of Canopy Cover, this will return
*        what percentage that is of and acre,
* NOTE: This will calculation the amount of overlap.
*       For example. if you send in a large amount like 43000 sq feet,
*        which is almost equal to the number squar feet in an acre,
*        you will get back a number around 67, because of the overlap.
*       You would have to send in a number up over 200,000 before you
*        would get back 99 for a percent.
*       Also for example, a number like 400 square feet sent in would get
*         you a return value of about 1 percent
* NOTE: this function will never return a value greater than 100
*   In: canopy cover in square feet.
*  Ret: percent as a whole number  0 > 100
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float MRT_Overlap( float f_SqFtCov)
{
float x,p;
#define  e_SqFtAcre 43560.0

   x = (f_SqFtCov / e_SqFtAcre);
   p = 100.0 * ( 1.0 -  exp ( -x ) ) ;
   return p;
}




//99999999999999999999999999999999999999999999999999999999999999



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

#ifdef wowowow
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
#endif 

int PFI_Calc_OLD (d_MIS *a_MIS, char cr_ErrMes[])
{
 return 1; 
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: PFI_Calc
* Desc: Perform Calc for a species
*
*   In: a_MIS...contains species, inputs, and Eq code to use
*  Out: a_MIS...mortality in f_Prop
*
*  Ret: 1 OK, 0 error see cr_ErrMes
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float  PFI_Calc (d_MIS *a_MIS, d_MO *a_MO, char cr_ErrMes[])
{
int i;
float f; 
float (*func) (d_MIS *a_MIS);

   strcpy (cr_ErrMes,"");

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
   for ( i = 0; i < 1000; i++ ){        /* look for entry we want.           */
      if ( !strcasecmp (sr_EFR[i].cr_MrtEqu,"")){
        sprintf (cr_ErrMes,"Invalid post fire eq %s",a_MIS->cr_Eq);
        return 0; }

      if ( strcasecmp (sr_EFR[i].cr_MrtEqu,a_MIS->cr_Eq))    /* Not found              */
        continue;                              /*  keep looking          */

      if ( !ValidInput (a_MIS, sr_EFR[i].cr_ReqFld, i+1, cr_ErrMes))
         return 0;                      /* Validate all inputs we'll use     */

       func = sr_EFR[i].func;           /* Address of func for spcfid eq     */
       f = (*func) (a_MIS);             /* Execute the Eq function           */
      
       break;  }

   a_MIS->f_Prop = f;  /* Will also get put into a_MO as a whole int value */  

   MRT_Total (a_MIS, a_MO, f);           /* Accum and Calc, avergest etc.     */

   return f;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: ValidInput
* Desc: Valididate all inputs for a line of species input data
*       Use the functions format string (ie "Vol DBH CKR X" )
*       Each token in string is looked at and validated accordingly
*
*   In: cr_ReqFld...format string from EFT table, string specifies
*                 what input are/are not required
*       a_MIS....single species record/struct
*       i_Row....row number in GUI Tree Grid of species being checked
*  Out: cr_ErrMes.......Error message
*  Ret: 1 ok, else 0
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int ValidInput (d_MIS *a_MIS, char cr_ReqFld[], int i_Row, char cr_ErrMes[])
{
char cr[500];

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/* DBH and Beetle Kill might be required, so look in Required Field String */
   if ( strstr(cr_ReqFld,"dbh") ) {   /* is dbh reguired */ 
      if (a_MIS->f_DBH < 0 || a_MIS->f_DBH > 500.0 ) { 
         sprintf  (cr, "DBH %5.0f", a_MIS->f_DBH);
         goto Err; } }

   if  ( strstr (cr_ReqFld, "btl" ) ) {
     if ( strcasecmp (a_MIS->cr_BeeDam, "Y") && strcasecmp (a_MIS->cr_BeeDam, "N") ) {
        sprintf (cr,"Beetle Attack: %s  (use Y/N)",a_MIS->cr_BeeDam);  
        goto Err;  } }

/* Crown Damage and Cambium Kill are always required */
  if ( a_MIS->f_CrnDam < 0 || a_MIS->f_CrnDam > 100.0 ) {
     sprintf (cr, "Crown Damage Ratio: %4.1f",a_MIS->f_CrnDam); 
     goto Err; }

  if ( a_MIS->f_CKR < 0 || a_MIS->f_CKR > 4.0 ) {
     sprintf (cr, "Cambium Kill: %3.0f",a_MIS->f_CKR); 
     goto Err; }

  if ( a_MIS->f_Den < 1.0 || a_MIS->f_Den > 20000 ) {
     sprintf  (cr, "Density %6.1f", a_MIS->f_Den); 
     goto Err; } 

  return 1; 

Err: 
  sprintf (cr_ErrMes, "Invalid input: %s \n Row: %d, Species: %s",cr,i_Row,a_MIS->cr_Spe); 
  return 0;
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
   if ( !strcasecmp(cr_Fmt,e_NA) &&  f_Num == e_PFI_Miss )
     return 1;

/* if not Required but user entered something - error                        */
  if ( !strcasecmp(cr_Fmt,e_NA) && f_Num != e_PFI_Miss) {
    sprintf (cr_Err,"%s - %s setting is not applicable\nPlease use %s",cr_Spe,cr_FldNam,e_NA);
    return 0; }

 if ( strcasecmp(cr_Fmt,e_NA) && f_Num == e_PFI_Miss ) {
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
*   In: a_MIS->f_CrnDam....crown length scorched percent
*       a_MIS->f_CKR....cambium kill ratio 0-4
*       a_MIS->f_DBH....inches
*       a_MIS->cr_Btl ...Beetle Kill  e_BtlYes, e_BtlNo
*  Out: a_MIS->f_Prop ...0 -> 1.0
*  Ret:
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float Eq_WhiteFir_WF (d_MIS *a_MIS)
{
float f,cs,CKR,DBH,btl;

  cs  =  a_MIS->f_CrnDam;
  CKR =  a_MIS->f_CKR;
  DBH =  a_MIS->f_DBH;

  if ( !strcasecmp (a_MIS->cr_BeeDam,e_BtlYes) )
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
  a_MIS->f_Prop = f;
  return f;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Eq_SubalpineFir_SF   ABILAS
* Desc:
*   In: a_MIS->f_CrnDam....crown volume scorched percent
*       a_MIS->f_CKR....cambium kill ratio 0-4
*  Out: a_MIS->f_Prop ...0 -> 1.0
*  Ret: 1 ok
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float  Eq_SubalpineFir_SF (d_MIS *a_MIS)
{
float f,cs,CKR;

  cs  =  a_MIS->f_CrnDam;
  CKR =  a_MIS->f_CKR;
  f = 1.0 / ( 1.0 + exp ( -
          ( -2.6036 +
          ( Mypow(cs,3.0) * 0.000004587 ) +
          ( CKR * 1.3554) ) ) );
  a_MIS->f_Prop = f;
  return f;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Eq_WesternLarch_WL    LAROCC
* Desc:
*   In: a_MIS->f_CrnDam....crown volume scorched percent
*       a_MIS->f_CKR....cambium kill ratio 0-4
*  Out: a_MIS->f_Prop ...0 -> 1.0
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float Eq_WesternLarch_WL  (d_MIS *a_MIS)
{
float f,cs,CKR;

  cs  =  a_MIS->f_CrnDam;
  CKR =  a_MIS->f_CKR;
  f = 1.0 / ( 1.0 + exp ( -
          ( -3.8458 +
          ( Mypow(cs,2.0) * 0.0004 ) +
          ( CKR * 0.6266) ) ) );
  a_MIS->f_Prop = f;
  return f;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Eq_IncenseCedar_IC    LIBDEC
* Desc:
*   In: a_MIS->f_CrnDam....crown length scorched percent
*       a_MIS->f_CKR....cambium kill ratio 0-4
*  Out: a_MIS->f_Prop ...0 -> 1.0
*  Ret: 1 ok
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float Eq_IncenseCedar_IC (d_MIS *a_MIS)
{
float f,cs,CKR;

  cs  =  a_MIS->f_CrnDam;
  CKR =  a_MIS->f_CKR;
  f = 1.0 / ( 1.0 + exp ( -
          ( -5.6465 +
          ( Mypow(cs,3.0) * 0.000007274 ) +
          ( CKR * 0.5428) ) ) );
  a_MIS->f_Prop = f;
  return f;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Eq_WhitebarkPine_WP   PINALB
* Desc:
*   In: a_MIS->f_CrnDam....crown volume scorched percent
*       a_MIS->f_CKR....cambium kill ratio 0-4
*       a_MIS->f_DBH
*  Out: a_MIS->f_Prop ...0 -> 1.0
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float Eq_WhitebarkPine_WP  (d_MIS *a_MIS)
{
float f,cs,DBH,CKR;
  cs  =  a_MIS->f_CrnDam;
  CKR =  a_MIS->f_CKR;
  DBH =  a_MIS->f_DBH;

  f = 1.0 / ( 1.0 + exp ( -
          ( -1.4059 +
          ( Mypow(cs,3.0) * 0.000004459 ) +
          ( (CKR*CKR) * 0.2843) +
          ( DBH * -0.1232) ) ) );
  a_MIS->f_Prop = f;
  return f;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Eq_EngelmannSpruce_ES   PICENG
* Desc:
*   In: a_MIS->f_CrnDam........crown volume scorched percent
*       a_MIS->f_CKR........cambium kill ratio
*  Out: a_MIS->f_Prop ...0 -> 1.0
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float Eq_EngelmannSpruce_ES (d_MIS *a_MIS)
{
float f,cs,CKR;
  cs  =  a_MIS->f_CrnDam;
  CKR =  a_MIS->f_CKR;
  f = 1.0 / ( 1.0 + exp ( -
            ( -2.9791 +
            ( cs * 0.0405 ) +
            ( CKR * 1.1596) ) ) );
  a_MIS->f_Prop = f;
  return f;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Eq_RedFir_RF  ABIMAG
* Desc:
*   In: a_MIS->f_CrnDam....crown length scorched percent
*       a_MIS->f_CKR....cambium kill ratio 0-4
*  Out: a_MIS->f_Prop ...0 -> 1.0
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float Eq_RedFir_RF (d_MIS *a_MIS)
{
float f,cs,CKR,Btl;
  cs  =  a_MIS->f_CrnDam;
  CKR =  a_MIS->f_CKR;

  f = 1.0 / ( 1.0 + exp ( -
            ( -4.7515 +
            ( Mypow(cs,3.0) * 0.000005989 ) +
            ( CKR * 1.0668) ) ) );

  a_MIS->f_Prop = f;
  return f;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Eq_SugarPine_SP   PINLAM
* Desc:
*   In: a_MIS->f_CrnDam....crown length scorched percent
*       a_MIS->f_CKR....cambium kill ratio 0-4
*       a_MIS->f_DBH
*       a_MIS->cr_Btl ...Beetle Kill  e_BtlYes, e_BtlNo
*  Out: a_MIS->f_Prop ...0 -> 1.0
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float Eq_SugarPine_SP (d_MIS *a_MIS)
{
float f,cs,CKR,Btl;
  cs  =  a_MIS->f_CrnDam;
  CKR =  a_MIS->f_CKR;

  if ( !strcasecmp (a_MIS->cr_BeeDam ,e_BtlYes) )
     Btl = 1.0;
  else
     Btl = -1.0;

  f = 1.0 / ( 1.0 + exp ( -
            ( -2.7598 +
            ( (cs * cs) * 0.000642 ) +
            ( Mypow (CKR,3.0) * 0.0386) + ( Btl * 0.8485) ) ) );

  a_MIS->f_Prop = f;
  return f;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Eq_PonderosaPine_PP  PINPON
* Desc: There are 2 ponderosa equation, (see Eq_PonderosaKill_PK)
* NOTE: Currently Jeffrey Pine also uses this
*   In: a_MIS->f_CrnDam....crown volume scorched percent
*       a_MIS->f_CKR....cambium kill ratio 0-4
*       a_MIS->cr_Btl ...Beetle Kill  e_BtlYes, e_BtlNo
*  Out: a_MIS->f_Prop ...0 -> 1.0
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float Eq_PonderosaPine_PP (d_MIS *a_MIS)
{
float f,cs,CKR,Btl,F;

  cs  =  a_MIS->f_CrnDam;
  CKR =  a_MIS->f_CKR;
  if ( !strcasecmp (a_MIS->cr_BeeDam,e_BtlYes) )
     Btl = 1.0;
  else
     Btl =  0.0;

  F = ( -4.1914 + ( Mypow(cs,2.0) * 0.000376)
                + ( CKR * 0.5130)
                + ( Btl * 1.5873 ) );
  f = 1.0 / ( 1.0 + exp ( - (  F  ) ) );

  a_MIS->f_Prop = f;
  return f;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Eq_PonderosaKill_PK  PINPONk
* Desc: There are 2 ponderosa equation, (see Eq_PonderosaPine_PP)
*       This equations uses Killed percent
* NOTE: Currently Jeffrey Pine also uses this
*   In: a_MIS->f_CrnDam....percent of crown killed
*       a_MIS->f_CKR....cambium kill ratio 0-4
*       a_MIS->cr_Btl ...Beetle Kill  e_BtlYes, e_BtlNo
*  Out: a_MIS->f_Prop ...0 -> 1.0
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float Eq_PonderosaKill_PK (d_MIS *a_MIS)
{
float f,cs,CKR,Btl,F;

  cs  =  a_MIS->f_CrnDam;
  CKR =  a_MIS->f_CKR;
  if ( !strcasecmp (a_MIS->cr_BeeDam,e_BtlYes) )
     Btl = 1.0;
  else
     Btl =  0.0;

  F = ( -3.5729 + ( Mypow(cs,2.0) * 0.000567)
                + ( CKR * 0.4573)
                + ( Btl * 1.6075) );
  f = 1.0 / ( 1.0 + exp ( - (  F  ) ) );

  a_MIS->f_Prop = f;
  return f;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Eq_DouglasFir_DF  PSEMEN
* Desc:
*   In: a_MIS->f_CrnDam....crown volume scorched percent
*       a_MIS->f_CKR....cambium kill ratio 0-4
*       a_MIS->f_DBH
*       a_MIS->cr_Btl ...Beetle Kill  e_BtlYes, e_BtlNo
*  Out: a_MIS->f_Prop ...0 -> 1.0
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float Eq_DouglasFir_DF (d_MIS *a_MIS)
{
float f,cs,DBH,CKR,Btl,F;

  cs  =  a_MIS->f_CrnDam;  // a_PFI->f_CRP;
  CKR =  a_MIS->f_CKR;     // a_PFI->f_CKR;
  DBH =  a_MIS->f_DBH;     // a_PFI->f_DBH;
  if ( !strcasecmp (a_MIS->cr_BeeDam, e_BtlYes)) //  a_PFI->cr_Btl ,e_BtlYes) )
     Btl = 1.0;
  else
     Btl =  0.0;

  F = ( -1.8912 + (cs * 0.07) - (Mypow(cs,2.0) * 0.0019) +
                (Mypow(cs,3.0) * 0.000018) + (CKR * 0.5840)
       - (DBH * 0.0788) - (Btl * 0.7959) + (DBH * Btl * 0.1251)
      ) ;

  f = 1.0 / ( 1.0 + exp ( - (  F  ) ) );

  a_MIS->f_Prop = f;
  return f;
}


/*****************************************************************************************
******************************************************************************************/
/* Bole Char Equations table */
typedef struct { 
    char  cr_Equ[20];                 /* Equation Number */
    char cr_ComNam[100];       /* Common Name */
    char cr_SceNam[100];        /* Scientific Name */
    char cr_PriSpe[30];        /* Primary Species - code */
    float B1, B2, B3;          /* Coefficents */
    } d_BCT;

d_BCT sr_BCT[] = {
   { "100", "Red maple", "Acer rubrum", "ACRU",             2.3014, -0.3267, 1.1137 },  
   { "101", "Flowering dogwood","Cornus florida","COFL2", -0.8727, -0.1814, 4.1947  },
   { "102", "Blackgum","Nyssa sylvatica","NYSY",           2.7899, -0.5511, 1.2888  },
   { "103", "Sourwood","Oxydendrum arboreum","OXAR",       1.9438, -0.4602, 1.6352  },
   { "104", "White oak","Quercus alba","QUAL",            -1.8137, -0.0603, 0.8666  },
   { "105", "Scarlet oak","Quercus coccinea","QUCO2",     -1.6262, -0.0339, 0.6901  },
   { "106", "Blackjack oak","Quercus marilandica","QUMA3", 0.3714, -0.1005, 1.5577  },
   { "107", "Chestnut oak","Quercus montana","QUMO4",     -1.4416, -0.1469, 1.3159  },
   { "108", "Black oak","Quercus velutina","QUVE",         0.1122, -0.1287, 1.2612  },
   { "109", "Sassafras","Sassafras albidum","SAAL5",       1.6779, -1.0299, 10.2855 },
   { ""   , "", "", "", 0,0,0}}; 

/***********************************************************************************
* Name: BC_Calc
* Desc: Bole Char - mortality probability
*       Southern US broadleaf species (Tara L. Keyser, others)
*       International Journal of Wildland Fire
*       https://doi.org/10.1071/WF17058
*
*  This is taken directly from DL's spread sheet, F,G, etc are the 
*  spread sheet colums letters 
*  coefficients 
  G = B1      spreadsheet colum - formula coefficent 
  H = B2
  I = B3

  E = DBH  cm 
  F = Bole Char Hgt  meter
  =1/(1+EXP((-1*(G2+(H2*E2)+(I2*F2)))))   from DL's spreadsheet

*
**********************************************************************************/
float BC_Calc (d_MIS *a_MIS, d_MO *a_MO, char cr_ErrMes[])
{
int i;  
float f;
float G, H, E, I, F; 
  f = 0; 

   for ( i = 0; i < 100; i++ ) {
     if ( strcmp (sr_BCT[i].cr_Equ, a_MIS->cr_Eq) )
      continue; 
     if ( !strcmp (sr_BCT[i].cr_Equ,"") )
       return -1;             /* Error - didn't find equ number in tabl */
     break;
   }

  G = sr_BCT[i].B1;     /* spreadsheet colum - formula coefficent */  
  H = sr_BCT[i].B2;
  I = sr_BCT[i].B3;
  E = InchToCent (a_MIS->f_DBH);     
  F = FtToMt (a_MIS->f_BolCha);  
  f = 1.0 / (1.0 + exp((-1.0*(G+(H*E)+(I*F)))));  
  a_MIS->f_Prop = f;  /* will also get put into a_MO as whole int value */ 

// test..................................
float a,b,c; 

a = -1.0*(G+(H*E)+(I*F));
b =  exp(a);


// test.................................


  MRT_Total (a_MIS, a_MO, f);           /* Accum and Calc, avergest etc.     */
  return f;
}

