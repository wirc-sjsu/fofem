
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_hsf.c
* Desc: HSF Herb,Shrub,Foliage Consumerd Manager - this is everything
*        burnup doesn't do,
*        Does Foilage Crown and Branch and The Mineral Soil Exposed
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <float.h>

#include  "bur_brn.h"
#include "fof_sgv.h"
#include  "fof_co.h"
#include  "fof_ci.h"
#include  "fof_hsf.h"
#include  "fof_lem.h"
#include  "fof_duf.h"

#include  "fof_ansi.h"

// #include  "bur_brn.h"

int _ChkLim (float f_Load, char cr_Name[], char cr_ErrMes[]);
int _Pc_CroBrn (float f_Pcn, char cr_ErrMes[]);
int  _DuffChk (d_CI *a_CI, char cr_ErrMes[]);
int _ChkLitMoist (d_CI *a_CI, char cr_ErrMes[]);
void  _CalcCP_Lit (d_CI *a_CI, d_CO *a_CO, d_DUF *a_Duf);

float HerbConsumed (d_CI *a_CI, int *ai_Eq);

float PFW_Shrub_Eq236 (float LS, d_CI *a_CI);
float PFW_Herb_Eq223 (float Lh, int *ai_EqNum);

float Ton_To_Mega (float T);
float Mega_To_Ton (float M);

float  Herb_Eq222 (float f_Lit, float f_Herb, int *ai_Equ);
float  Herb_Eq221 (float f_Herb, int *ai_Equ);
float  Herb_Eq22 (float f_Herb, int *ai_Equ);



/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: HSF_Mngr
* Desc: Consumed Manager this does
*       Duff, Duff Depth, Mineral Soil Exposer, Herb, Shrub, Foliage, Branch
*   In: a_CI.....See the notes at the top of this file
*  Out: a_CO.....Output values
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int HSF_Mngr (d_CI *a_CI, d_CO *a_CO, char cr_ErrMes[])
{
d_DUF s_Duf;

  strcpy (cr_ErrMes,"");
  DUF_Init (&s_Duf); 

  if ( !HSF_Check (a_CI,cr_ErrMes) )   /* check inputs for HSF              */
     return 0;

/* Duff .....................................................................*/
   a_CO->f_DufPre = a_CI->f_Duff;
   DUF_Mngr (a_CI, &s_Duf);
   if ( strcmp (s_Duf.cr_ErrMess,"") ) {
     strcpy (cr_ErrMes, s_Duf.cr_ErrMess); 
     return 0; } 

   a_CO->i_DufEqu = DUF_Calc (a_CI, &s_Duf, &a_CO->f_DufCon, &a_CO->f_DufPos, &a_CO->f_DufPer);

/* Duff Depth ...............................................................*/
   a_CO->f_DufDepPre = a_CI->f_DufDep;

   DUF_GetDepRed (a_CI,&s_Duf,&a_CO->f_DufDepPos,&a_CO->f_DufDepPer);
   a_CO->f_DufDepCon = s_Duf.f_Red;
   a_CO->i_DufDepEqu = s_Duf.i_RedEqu;

   a_CO->f_MSEPer = s_Duf.f_MSEPer;         /* Mineral Soil                 */
   a_CO->i_MSEEqu = s_Duf.i_MSEEqu;

/* Litter ...................................................................*/
/* We only need to deal with litter here if the Coastal Plain Duff eq did    */
/*  the litter, else litter is done in burnup                                */
   if ( s_Duf.i_LitEqu == e_CP_PerEq )
     _CalcCP_Lit (a_CI, a_CO, &s_Duf);  /* See notes in called func heading  */

/* Herb  ....................................................................*/
   a_CO->f_HerPre = a_CI->f_Herb;
   a_CO->i_HerEqu = Calc_Herb (a_CI, &a_CO->f_HerCon, &a_CO->f_HerPos, &a_CO->f_HerPer);

/* Shrub ....................................................................*/
   a_CO->f_ShrPre = a_CI->f_Shrub;
   a_CO->i_ShrEqu = Calc_Shrub (a_CI, &a_CO->f_ShrCon, &a_CO->f_ShrPos, &a_CO->f_ShrPer);

/* Foliage ..................................................................*/
   a_CO->f_FolPre = a_CI->f_CroFol;
   a_CO->i_FolEqu = Calc_CrownFoliage(a_CI, &a_CO->f_FolCon, &a_CO->f_FolPos, &a_CO->f_FolPer);

/* Branch ...................................................................*/
   a_CO->f_BraPre = a_CI->f_CroBra;
   a_CO->i_BraEqu = Calc_CrownBranch (a_CI, &a_CO->f_BraCon, &a_CO->f_BraPos, &a_CO->f_BraPer);

   return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _CalcCP_Lit
* Desc: Calc the Coastal Plain Litter amounts into the CO (consumed
*         output) struct.
*       THIS functin is only used for the cover type Coastal Plain because
*        litter consumed is calc'd along with the Duff and put in the
*        d_DUF struct.
*       NORMALLY litter is done in Burnup
* NOTE: See the fof_bcm.c BCM_Mngr() where we deal with this again
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void _CalcCP_Lit (d_CI *a_CI, d_CO *a_CO, d_DUF *a_Duf)
{
   if ( a_Duf->i_LitEqu != e_CP_PerEq )  /* See Note-1 above                */
     return;
   a_CO->i_LitEqu = a_Duf->i_LitEqu;
   a_CO->f_LitPre = a_CI->f_Lit;
   a_CO->f_LitCon = (a_Duf->f_PerLit * a_CI->f_Lit) / 100.0 ;
   a_CO->f_LitPos = a_CO->f_LitPre - a_CO->f_LitCon;
   a_CO->f_LitPer = a_Duf->f_PerLit;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Calc_Shrub
* Desc: Calculate Amount of Shrub Reduction in tons per acre
* Note-1: See notes in the called function
*   In: a_CI.....Consume Input struct
*  Out: af.......Amount of Shrub Reduction in tons per acre
*  Ret: shrub reduction equation used to make calculation
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int Calc_Shrub (d_CI *a_CI, float *af_Con, float *af_Post, float *af_Percent)
{
int i_Equ;
/*...........................................................................*/

   if ( CI_isSageBrush(a_CI) ) {             /* For Sagebrush - Cover Type   */
     if ( CI_isFall(a_CI))                   /* Fall - season                */
        i_Equ = 233;
     else
        i_Equ = 232; }                       /* spring, summer winter        */

   else if ( CI_isPinFlaWoo(a_CI) )          /* Pine Flatwoods - southeast*/
      i_Equ = 236;  

   else if ( CI_isShrubGroup(a_CI))          /* For Chaparral,Deser Shrb, etc*/
      i_Equ = 231;

   else if ( CI_isSouthEast(a_CI)) {             /* Southeast Region             */
     if ( CI_isPocosin(a_CI))                    /* Pocosin - Cover Type         */
       if ( CI_isSpring(a_CI) || CI_isWinter(a_CI) )
          i_Equ = 233;                       /* Spring  Winter               */
       else
          i_Equ = 235;                       /* Summer Fall                  */
     else
       i_Equ = 234;                          /* Non - Pocosin - Southeast    */
   }
   else
     i_Equ = 23;                             /* everything else              */

   *af_Con = Shrub_Equ(a_CI, i_Equ);

/* If doing Batch with User specified Equation # and there is a non-missing  */
/*  equations number to use, if not keep the value done above, we previously  */
/*  checked for invalid eq # */
   if ( !xstrcmpi (a_CI->cr_BatchEqu,"Yes") ){
      if ( a_CI->i_EqShrub > 0 ) {   
        *af_Con = Shrub_Equ(a_CI, a_CI->i_EqShrub); 
        i_Equ = a_CI->i_EqShrub; }
   }

   if ( *af_Con > a_CI->f_Shrub )           /* Can't consume more that we   */
      *af_Con = a_CI->f_Shrub;              /* started with                 */
   if ( *af_Con < 0 )                        /* make sure not less then 0    */
      *af_Con = 0;

   if ( a_CI->f_Shrub != 0 ) {
     *af_Post = a_CI->f_Shrub - *af_Con;              /* Post Amount         */
     *af_Percent = (*af_Con / a_CI->f_Shrub) * 100;}  /* percent             */
   else
     *af_Con = *af_Post = *af_Percent = 0;

   return i_Equ;                             /* Equation used                */
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Shrub_Equ
*   In: a_FUL.....Fuel Loading Struct, this has tons per acre, moiture etc.
*       i_Equ....equation to use
*  Ret: amount of shrub reduction
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float  Shrub_Equ (d_CI *a_CI, int i_Equ )
{
float   f, f_Percent;
char cr[300];
  if ( i_Equ == 23 )   return  ( a_CI->f_Shrub * .60 );
  if ( i_Equ == 231 )  return  ( a_CI->f_Shrub * .80 );
  if ( i_Equ == 232 )  return  ( a_CI->f_Shrub * .50 );
  if ( i_Equ == 233 )  return  ( a_CI->f_Shrub * .90 );
  if ( i_Equ == 235 )  return  ( a_CI->f_Shrub * .80 );

  if ( i_Equ == 234 ) {
    f_Percent = Equ_234_Per (a_CI);
    return (a_CI->f_Shrub * f_Percent); }

  if ( i_Equ == 236 ) {   /* Pine Flatwoods */ 
    f = PFW_Shrub_Eq236 (a_CI->f_Shrub, a_CI);
    return f; } 

  sprintf (cr,"ERROR - Shrub_Equ - Shrub Equation %d Not Implemented\n",i_Equ);
  LEM_Put ("Shrub_Equ()", cr);
  return -1;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equ_234_Per
* Desc: Calculat percent of reduction
* Note-1: Put this check in just to make sure, not sure what that calculation
*          will do.
*   In: a_CI......Consumed Input struct
*  Ret: percent
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float Equ_234_Per (d_CI  *a_CI)
{
float f_W, f_WPRE, f_ShrReg, f;

   f_W = Equation_16 (a_CI);
   if ( f_W == 0 )
     return 0;

   f_WPRE = a_CI->f_Lit + a_CI->f_Duff + a_CI->f_DW10 + a_CI->f_DW1;
   if ( f_WPRE == 0 )
     return 0;

/* original code f_ShrReg = a_FUL->f_Shrub + a_FUL->f_Tree; */
   f_ShrReg = a_CI->f_Shrub;
   if ( f_ShrReg == 0 )
     return 0;

   f = (((3.2484 + (0.4322 * f_WPRE) +  (0.6765 * (f_ShrReg)) - (0.0276 * a_CI->f_MoistDuff) - ( 5.0796 / f_WPRE) ) - f_W) / (f_ShrReg) );

   if ( f < 0 ) {                     /* See Note-1 above */
/* This message did come out during testing for SAF 40 */
/*   sprintf (cr,"percent %f is less than 0, will set to 0",f); */
/*   Warning_Window (cr,"Equ_234_Per"); */
     f = 0; }
   if ( f > 100 ) {
/*   Warning_Window ("percent is greater than 100, will set to 100","Equ_234_Per"); */
     f = 100; }

   return  f;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Equation 16
* Desc: Gets used by various other Equations
*   In: a_CI.....Fuel Loading struct
*  Ret:
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float Equation_16 (d_CI  *a_CI)
{
float f_WPRE, f_W;

   f_WPRE = a_CI->f_Lit + a_CI->f_Duff + a_CI->f_DW10 + a_CI->f_DW1;
   if ( f_WPRE == 0 )
     return 0;
   f_W = 3.4958 + (.3833 * f_WPRE) - (.0237 * a_CI->f_MoistDuff) - (5.6075 / f_WPRE);
   return f_W;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Calc_Herb
* Desc: Calculate Amount of Herbacious Reduction in tons per acre
* Note-1: New Herb consumed, this new way treats Southeast differently
*       originaly all regions would consume 100 percent 
*       DL sent to me in an email approx Nov 2014, implemented
*       Jan 2015
*
*  from DL's email - 
*  this is for Southeast ONLY
*    Herb Load Consumed = -0.059 + (0.004 * Litter Fuel Load) + (0.917 * Herb Fuel Load)
*    If Herb Load Consumed <0 then Herb Load Consumed = 0
*
*   In: a_CI.....Burn Data Input
*  Out: af.......Amount of Herbacious Reduction in tons per acre
*  Ret: Herbacious reduction equation used to make calculation
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int   Calc_Herb (d_CI *a_CI, float *af_Con, float *af_Post, float *af_Percent)
{
int i_Equ;
float f_Con; 
char cr_ErrMes[1000]; 

   if ( CI_isPinFlaWoo (a_CI) ) {            /* Pine Flatwoods */ 
     *af_Con = PFW_Herb_Eq223 (a_CI->f_Herb, &i_Equ); }

/* See Note-1 above */
   else if ( !xstrcmpi (a_CI->cr_Region,SOUTHEAST) )
     *af_Con = Herb_Eq222 (a_CI->f_Lit, a_CI->f_Herb, &i_Equ);
  
   else if ( CI_isGrassGroup(a_CI) && CI_isSpring (a_CI) )
      *af_Con = Herb_Eq221 (a_CI->f_Herb, &i_Equ);

   else 
      *af_Con = Herb_Eq22(a_CI->f_Herb, &i_Equ);           /* 100 Percent                  */
  
/* If running Batch with User Specified Equation numbers */ 
/* Note, af_Con and i_Equ will return as set above if there is no */
/* user equation number set                                */
   if ( !xstrcmpi (a_CI->cr_BatchEqu,"Yes") )
      BUE_Herb (a_CI, af_Con, &i_Equ, cr_ErrMes);  /* previous check for bad eq num */
    
  
CON:
   if ( *af_Con > a_CI->f_Herb )            /* Don't make consumed more than*/
      *af_Con = a_CI->f_Herb;               /* we start with                */
   if ( *af_Con < 0 )
      *af_Con = 0;

   if ( a_CI->f_Herb != 0 ) {
     *af_Post = a_CI->f_Herb - *af_Con;              /* Post Amount         */
     *af_Percent = (*af_Con / a_CI->f_Herb) * 100;}  /* percen              */
   else
     *af_Con = *af_Post = *af_Percent = 0;

   return i_Equ;

}

/*************************************************************/
float  Herb_Eq222 (float f_Lit, float f_Herb, int *ai_Equ)
{
float f; 
   f = -0.059 + (0.004 * f_Lit) + (0.917 * f_Herb);
   *ai_Equ = 222;  
   return f; 
}

/*************************************************************/
float  Herb_Eq221 (float f_Herb, int *ai_Equ)
{
float f;
  f =  f_Herb  * 0.9;
  *ai_Equ = 221;
  return f; 
}

/**************************************************************/
float  Herb_Eq22 (float f_Herb, int *ai_Equ)
{
   *ai_Equ = 22;
   return f_Herb;   /* 100 percent */ 
}

/*********************************************************************
* Name: BUE_Herb
* Desc: Used for Batch user specifed equation numbers
*   In: af_Con, ai_Equ ---> NOTE fill these in with the values you 
*        want returned if there is 'missing/no' equation number 
**********************************************************************/
int BUE_Herb (d_CI *a_CI, float *af_Con, int *ai_Equ, char cr_ErrMes[])
{
int i_Equ; 
    
   strcpy (cr_ErrMes,""); 

   if ( a_CI->i_EqHerb <= 0 )   /* No Equation number specified */
      return 1;                 /* See Notes above */

   switch ( a_CI->i_EqHerb ) {
     case 223:   
        *af_Con = PFW_Herb_Eq223 (a_CI->f_Herb, ai_Equ);
         break;
     case 22:
        *af_Con = Herb_Eq22 (a_CI->f_Herb, ai_Equ);
        break; 
     case 221:
        *af_Con = Herb_Eq221 (a_CI->f_Herb, ai_Equ);
         break; 
     case 222:
        *af_Con = Herb_Eq222 (a_CI->f_Lit, a_CI->f_Herb, ai_Equ);
         break;
     default:
       sprintf (cr_ErrMes,"Invalid Herbaceous equations number: %d\n",a_CI->i_EqHerb);
       strcat  (cr_ErrMes,"    Valid Numbers: 22, 221, 222, 223"); 
       return 0; 
   } 
 
   return 1; 
}


#ifdef   WWWWWWWWWWWWWWWWW
   decided to put code in main herb function above
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: HerbConsumed 
* Desc: New Herb consumed, this new way treats Southeast differently
*       originaly all regions would consume 100 percent 
*       DL sent to me in an email approx Nov 2014, implemented
*       Jan 2015
*
*  from DL's email - 
*  this is for Southeast ONLY
*    Herb Load Consumed = -0.059 + (0.004 * Litter Fuel Load) + (0.917 * Herb Fuel Load)
*    If Herb Load Consumed <0 then Herb Load Consumed = 0
*
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float HerbConsumed (d_CI *a_CI, int *ai_Eq)
{
float f; 

  *ai_Eq = 22; 

  if ( stricmp (a_CI->cr_Region,SOUTHEAST) )
     return a_CI->f_Herb;

  f = -0.059 + (0.004 * a_CI->f_Lit) + (0.917 * a_CI->f_Herb);

  if (  f < 0 ) 
    f = 0; 

  return f;
}
#endif 


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: LitterSouthEast
* Desc: It's Burnup's job to do litter so this function will get called
*       to short circut Burnup. 
*       Not much of an equation, but I wanted to keep it in with the other
*       equations. In particular if it should need to be called from
*       more than one location.
* Ret: consumed amount
*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*/
float LitterSouthEast (float f_Litter, int *ai_Equ)
{
  *ai_Equ = 998;  /* DL said to use this number */
  return  f_Litter * 0.8;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Calc_CrownBranch
* Desc: Calculate Amount of Crown Branch
*       Everything gets reduced by 50 percent equation 38
* Note-1: According to the Fofem Manual Equation 38, 50 percent gets
*         burned, but as per request E.R. 1/6/00 we now use % Crown Wnd
*         and apply that to calculation.
*         For example if user enters 10 in % Crown Burn Wnd then we
*         take 10 percent of 50 percent and do reset of calculation.
* Note-2: See notes in the called function
*   In: a_CI.....Burn Data Input
*  Out: see code
*  Ret:  reduction equation used to make calculation
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  Calc_CrownBranch (d_CI *a_CI, float *af_Con, float *af_Post, float *af_Percent)
{
float f;
  f = a_CI->f_Pc_CroBrn / 100;                    /* % Crown Burn from Wnd  */
  *af_Con = a_CI->f_CroBra * 0.50 * f;            /* Note-1                 */
  if ( a_CI->f_CroBra != 0 ) {
     *af_Post = a_CI->f_CroBra - *af_Con;             /* Post Amount        */
     *af_Percent = (*af_Con / a_CI->f_CroBra) * 100;} /* percent            */
  else
     *af_Con = *af_Post = *af_Percent = 0;

   return 38;                                      /* Equ 38 for all         */
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Calc_CrownFoliage
* Desc: Calculate Amount of Crown Foliage Consumed, Consumed amount is
*        simpley calculated from percent send in
*   In: a_CI.....Burn Data Input
*  Out: see code
*  Ret:  reduction equation used to make calculation
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  Calc_CrownFoliage (d_CI *a_CI, float *af_Con, float *af_Post, float *af_Percent)
{
float f;
  if ( a_CI->f_CroFol == 0 ) {              /* No Crn Folag to start with   */
     *af_Con = 0;                            /* so none consumed             */
     *af_Percent = 0;
     *af_Post = 0;
     goto X ; }                              /* Equ 37 for all               */

  f = a_CI->f_Pc_CroBrn / 100;              /* % Crown Burn that will burn  */
  *af_Con = a_CI->f_CroFol * f;             /* amount consumed              */
  *af_Post = a_CI->f_CroFol - *af_Con;      /* Post Amount                  */
  *af_Percent = a_CI->f_Pc_CroBrn;          /* percent                      */
X:
  return 37;                                /* Equ 37 for all               */
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: HSF_Check
* Desc: Make sure that the Region,Fuel Category,etc fields that need
*        to be set are set and valid.
*  NOTE: See code below as to what can be missing,
*
*
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  HSF_Check (d_CI *a_CI, char cr_ErrMes[])
{

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/* test-ld larry */
   if ( !_ChkLitMoist (a_CI, cr_ErrMes))/* Litter Moist, only Coast Plain    */
      return 0;
/* test-ld larry */

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/* Check Loads, percents, duff....                                           */
   if ( !_ChkLim (a_CI->f_Herb,"Herb",cr_ErrMes) )
      return 0;
   if ( !_ChkLim (a_CI->f_Shrub,"Shrub",cr_ErrMes) )
      return 0;
   if ( !_ChkLim (a_CI->f_CroFol,"Crown Foliage",cr_ErrMes) )
      return 0;
   if ( !_ChkLim (a_CI->f_CroBra,"Crown Branch",cr_ErrMes) )
      return 0;

   if ( !_Pc_CroBrn (a_CI->f_Pc_CroBrn,cr_ErrMes))  /* Crown Percent Check   */
      return 0;

   if ( !_DuffChk (a_CI,cr_ErrMes))
      return 0;

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/* Regions, is required                                                      */
  if (CI_isSouthEast   (a_CI)) goto A;
  if (CI_isInteriorWest(a_CI)) goto A;
  if (CI_isPacificWest (a_CI)) goto A;
  if (CI_isNorthEast   (a_CI)) goto A;
  sprintf(cr_ErrMes,"No/Invalid Region, Valid: %s, %s, %s, %s",
    e_CI_SouthEast, e_CI_InteriorWest, e_CI_PacificWest, e_CI_NorthEast);
  return 0;

A:
/* Fuel Category is required                                                 */
  if ( CI_isPiles  (a_CI)) goto C;
  if ( CI_isNatural(a_CI)) goto C;
  if ( CI_isSlash  (a_CI)) goto C;
  sprintf(cr_ErrMes,"No/Invalid Fuel Category (%s), Valid: %s, %s, %s)",
      a_CI->cr_FuelCategory,
      e_Natural, e_Slash, e_Piles);
  return 0;

C:
/* Duff Mositure Measured Method                                             */
  if ( CI_isDufEntire (a_CI)) goto D;
  if ( CI_isDufLower  (a_CI)) goto D;
  if ( CI_isDufNFDR   (a_CI)) goto D;
  if ( CI_isDufAdjNFDR(a_CI)) goto D;
  if ( !strcmp (a_CI->cr_DufMoiMet,"") )
     strcpy (cr_ErrMes, "Missing Duff Moisture Measure Method");
  else
     sprintf (cr_ErrMes, "Invalid Duff Moisture Measure Method '%s'\n    Valid Codes = %s, %s, %s, %s, %s",
      a_CI->cr_DufMoiMet,
      e_Entire, e_Lower, e_NFDR, e_Adj_NFDR, e_DufMoiMetDefault);
  return 0;

D:
/* Season                                                  */
  if ( CI_isSpring (a_CI)) goto E;
  if ( CI_isFall   (a_CI)) goto E;
  if ( CI_isSummer (a_CI)) goto E;
  if ( CI_isWinter (a_CI)) goto E;
  sprintf (cr_ErrMes, "Invalid/Missing Season: (%s)\n    Valid: %s, %s, %s, %s",
    a_CI->cr_Season, e_Spring, e_Summer, e_Fall, e_Winter);
  return 0;

E:
  if ( CI_isPinFlaWoo  (a_CI)) goto F; 
  if ( CI_isGrassGroup (a_CI)) goto F;
  if ( CI_isShrubGroup (a_CI)) goto F;
  if ( CI_isShrubGroupChaparral (a_CI)) goto F; 
  if ( CI_isSageBrush  (a_CI)) goto F;
  if ( CI_isPocosin    (a_CI)) goto F;
  if ( CI_isPonderosa  (a_CI)) goto F;
  if ( CI_isWhiPinHem  (a_CI)) goto F;
  if ( CI_isRedJacPin  (a_CI)) goto F;
  if ( CI_isBalBRWSpr  (a_CI)) goto F;
  if ( CI_isCoastPlain (a_CI)) goto F;
  if (!CI_isCoverGroup  (a_CI)) goto F;         /* None set,  ok             */
  sprintf (cr_ErrMes, "Invalid Cover Type (%s), Valid...\n %s, %s, %s, %s \n %s, %s, %s, %s %s or leave blank (\"\")",
     a_CI->cr_CoverGroup,
     e_CVT_GrassGroup, e_CVT_ShrubGroup,e_CVT_SageBrush,  e_CVT_Pocosin,
     e_CVT_Ponderosa,  e_CVT_WhiPinHem, e_CVT_RedJacPin,  e_CVT_BalBRWSpr, e_CVT_PinFlaWoo);
  return 0;

F:
  return 1;
}

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _ChkLitMoist
* Desc: Check litter moisture - ONLY relevant for Cover Type group Coastal
*        Plain,
*   In: see below
*  Ret: 1 = limit ok or not a coastal plain so not checked
*       0 = out of limits
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
/* test-ld larry */
int _ChkLitMoist (d_CI *a_CI, char cr_ErrMes[])
{

  if ( !CI_isCoastPlain (a_CI) )        /* Not Coastal Plain so don't need   */
     return 1;                          /*  to check                         */

  if ( a_CI->f_LitMoi >= e_LitMoiMin && a_CI->f_LitMoi <= e_LitMoiMax )
     return 1;                          /* OK, within limits                 */

  sprintf (cr_ErrMes, "_ChkLitMois() Litter Moisture %6.2f is out of limits (%6.2f -> %6.2f)",
      a_CI->f_LitMoi, e_LitMoiMin, e_LitMoiMax);
  strcat (cr_ErrMes,"\n     Litter Moisture is required for Coastal Plain cover types");
  return 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _ChkLim
* Desc: Check Load limits for Herb,Shr,Foliage Crown & Branch
*   In: see belowwo
*  Ret: 1 OK,  0 Error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int _ChkLim (float f_Load, char cr_Name[], char cr_ErrMes[])
{
   if ( f_Load >= e_CI_LoadLimLow &&  f_Load <= e_CI_LoadLimUp )
     return 1;

   sprintf (cr_ErrMes, "_ChkLim() %s Fuel Load %6.2f is out limit (%d -> %d )",
     cr_Name, f_Load, (int)e_CI_LoadLimLow,  (int)e_CI_LoadLimUp );
   return 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _Pc_CroBrn
* Desc: Check the Percent Crown Burn
*   In: see below
*  Ret: 1 OK,  0 Error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int _Pc_CroBrn (float f_Pc, char cr_ErrMes[])
{
   if ( f_Pc >= 0 && f_Pc <= 100 )
     return 1;

  sprintf (cr_ErrMes,"Percent of Crown Foliage Burn %6.2f is out of limit 0 -> 100",f_Pc);
  return 0;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _DuffChk
* Desc: Check the Duff Load, Depth and Moisture
* Note-1: This use to not check Duff Moist just Duff and DuffDep, so
*          zero or less than min mosit would go thru and blowup a log() func
*          in a Mineral Soil Exposed, This only affected the Batch, FOFEM5
*          would catch it.
*   In: see below
*  Ret: 1 OK,  0 Error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  _DuffChk (d_CI *a_CI, char cr_ErrMes[])
{
float Low,Hi;


/* Change 9-27-05, See Note-1 above                                          */
   if ( a_CI->f_Duff == 0 && a_CI->f_DufDep == 0 )
     goto Moist;

   if ( a_CI->f_Duff < e_DufMin  || a_CI->f_Duff > e_DufMax ) {
     sprintf (cr_ErrMes, "Duff Fuel Load %6.2f is out of limits (%6.2f -> %6.2f)",
        a_CI->f_Duff, e_DufMin, e_DufMax );
     strcat (cr_ErrMes,"\n Duff load can only be set to 0 when duff depth is also set to 0");
     return 0; }

   if ( a_CI->f_DufDep < e_DufDepMin  || a_CI->f_DufDep > e_DufDepMax ) {
     sprintf (cr_ErrMes, "Duff Depth %6.2f is out limits (%6.2f -> %6.2f)",
        a_CI->f_DufDep, e_DufDepMin, e_DufDepMax);
     return 0; }

Moist:
   Low = (e_dfm1 * 100);
   Hi  = (e_dfm2 * 100);
   if ( a_CI->f_MoistDuff < Low || a_CI->f_MoistDuff > Hi ){
     sprintf (cr_ErrMes, "Duff Moisture  %6.2f is out limits (%6.2f -> %6.2f)",
        a_CI->f_MoistDuff, Low, Hi);
     return 0; }

   if ( a_CI->f_Duff == 0 && a_CI->f_DufDep == 0 )
      goto Ret;

   if ( a_CI->f_Duff == 0 ) {
     strcpy (cr_ErrMes, "Duff Load is 0, Set Duff Load & Depth to 0 or both to non-zero values");
     return 0; }

   if ( a_CI->f_DufDep == 0 ) {
     strcpy (cr_ErrMes, "Duff Depth is 0, Set Duff Load & Depth to 0 or both to non-zero values");
     return 0; }
Ret:
  return 1;
}


/*******************************************************************************************
*
*                     Pine Flatwoods Equations - Southeast
*
********************************************************************************************/

/******************************************************
* Name: PFW_Shrub_Eq236
* Desc: Pine Flatwoods - southeast 
*       Shrub consumed 
*       This function works in megagrams per hectare 
*       but caller will send in load as tons per acre 
*   In: LS....shrub load - tons per acre
*  Ret: consumed amount - tons per acre
*
* From DL's email document 
*          SE_Flatwoods_Shrub_Litter_Herb_Consumption.docx
*  
*  ln Cs = -0.1889 + (0.9049 * ln Ls) + (0.0676 * season)
*  Cs=shrub load consumed (including saw palmetto)
*  Ls=prefire shrub loading (including saw palmetto)
*  Season = if spring or summer=1, if fall or winter=0
*  Loading = Mg ha-1
*  -> ln is natural log 
*
* Conversion factor: 1 Mg ha-1 = 0.446 T ac-1
*
* The equuvalent form of above equation: 
*  Cs = EXP(   (-0.1889 +(0.9049 * ln(prefire shrub load)) + (0.0676 * season)   )
*
* Tested and matched up with DL spreadsheet SE_Flatwoods_Shrub_Litter_Consumption.xlsx
* 
********************************************************/
float PFW_Shrub_Eq236 (float LS, d_CI *a_CI) 
{
float f,load, Cs, Ls, lnLs, lnCs; 
float season; 
 
   Ls = Ton_To_Mega(LS);    /* Tons per acre --> megagrams per hectare */

   season = 0; 
   if ( CI_isSummer (a_CI) ) 
       season = 1.0; 
   if ( CI_isSpring (a_CI) ) 
       season = 1.0; 
 
   load = log (Ls);
   Cs = exp( -0.1889 +(0.9049 * load) + (0.0676 * season) );

   f = Mega_To_Ton (Cs);  /* get it back to Tons per acre */  

   if ( f > LS )          /* might happen on small loades */ 
     f = LS; 
   return f; 
}

/***************************************************
* Name: PFW_Litter_Eq997
* Desc: Pine Flatwoods - southeast
*       I tested this against DL's spreadsheet 
*  NOTE: see comments in PFW_Shrub_Eq236()
*
*  In: load....litter load (tons per acre)
*      moist....moisture percent 0 -> 100 (gravimetric)
* 
***************************************************/
float PFW_Litter_Eq997 (float load, float moist, int *ai_EqNum)
{
float f,g,x, Load; 
     
   Load =  Ton_To_Mega(load);    /* Megagrams per hectare */

   g = (0.2871+(0.914*sqrt(Load))-(0.0101*moist));   /* Mine */ 
 
   f = pow (g,(float)2.0);
   
   g = Mega_To_Ton (f);  /* get it back to Tons per acre */  

   if ( g > load )       /* might consume slighty more than load */ 
     g = load;           /*  so don't let it */ 

   *ai_EqNum = 997;

   return g; 
}

/*****************************************************
* Name: PFW_Herb_Eq223
* Desc: Pine Flatwoods Herb consumption
*       this operates in metric - but we don't need to
*       do any coversion because it just a fixed percent
*       amount that gets consumed 
* Ch = Lh * 0.9944
* Ch = herb load consumed
* Lh = prefire herb loading
* Loading = Mg ha-1
*  In:    Lh....pre herb load (tons per acre)
*               See note above
* Out:  ai_EqNum...equation number 
* Ret: consumed load - tons per acre
*****************************************************/
float PFW_Herb_Eq223 (float Lh, int *ai_EqNum)
{
float Ch;
   *ai_EqNum = 223; 
   Ch = Lh * 0.9944;
   return Ch; 
}

/*********************************************************
* To convert tons per acre --> megagrams per hectare
* and back. The conversion factor was taken from DL's 
* email document - see notes in PFW_Shrub_Eq236
**********************************************************/
float Ton_To_Mega (float T)
{
float mega;
   mega =  T /  0.446; 
  return mega; 
}

float Mega_To_Ton (float M)
{
float ton; 
  ton = M * 0.446; 
  return ton; 
}