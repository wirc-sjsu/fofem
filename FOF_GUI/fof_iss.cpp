/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_iss          Individual Species Structure
* Desc: There is a table of ISS records declared in here.
*        Each entry int the table is used to hold a Species,
*        Density, DBH, Height, Crown Ratio, that the user has choosen
*        to run the mortaity report on.
* Date: 8/11/99
* Author: Larry Gangi
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <dos.h>

#include "fof_iss.h"
#include "fof_smt.h"
#include "fof_util.h"


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* Table will hold one entry for each species the user enters                */
/* That means there can be more than one entry for a given species, ie       */
/* for example, PinPon can be in here more than once                         */
#define eC_ISS  150
d_ISS  sr_ISS [eC_ISS + 1];  /* One dummy on end processing if full          */


// float CC_Overlap( float f_SqFtCov);

/***********************************************************************
* Name: ISS_SetKill
* Desc: Set the number of trees that are killed. 
*       This can be used later
************************************************************************/
void  ISS_SetKill (int i_Kill, int iX)
{
   sr_ISS[iX].i_Killed = i_Kill;

}
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: ISS_InitTbl
* Desc: Input any input stuff.
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void ISS_InitTbl ()
{
int i;
   for ( i = 0; i < eC_ISS + 1; i++ )
     ISS_Init (&sr_ISS[i]);
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: ISS_Init
* Desc: Input any input stuff.
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void ISS_Init (d_ISS *a_ISS)
{
     strcpy (a_ISS->cr_Spe,"");
     a_ISS->i_Den = 0;
     a_ISS->i_Killed = 0; 
     a_ISS->f_DBH = 0;
     a_ISS->i_TreHgt = 0;
     a_ISS->i_CroRat = 0;
     a_ISS->b_Graph = false; 
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: ISS_Set
* Desc: Set an entry in table
*   In: a_ISS....struct loaded with what needs to go into the table
*  Ret: 1 ok,  0 error see code below
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  ISS_Set  (d_ISS *a_ISS)
{
int i;
   for ( i = 0; i < eC_ISS; i++ ) {
     if (!strcmp (sr_ISS[i].cr_Spe,"") ) {        /* Find 1st avail entry      */
      strcpy (sr_ISS[i].cr_Spe,a_ISS->cr_Spe);
      sr_ISS[i].i_Den    =     a_ISS->i_Den;
      sr_ISS[i].f_DBH    =     a_ISS->f_DBH;
      sr_ISS[i].i_TreHgt =     a_ISS->i_TreHgt;
      sr_ISS[i].i_CroRat =     a_ISS->i_CroRat;
      sr_ISS[i].b_Graph  =     a_ISS->b_Graph; 
      sr_ISS[i].i_Killed =     a_ISS->i_Killed; 
      return 1; } }
   return 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: ISS_Get
* Desc: Iteratively get the specifed entry in table
*   In: iX.......index of entry to send out
*  Out: a_ISS....struct
* NOTE: when a_ISS->cr_Spe field is null its done.
*  Ret: 1 OK,  0 no more
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  ISS_Get  (d_ISS *a_ISS, int iX)
{
  memcpy (a_ISS, &sr_ISS[iX], sizeof (d_ISS) );
  if ( !strcmp (sr_ISS[iX].cr_Spe,"") )    /* If no more */
     return 0;
  return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: ISS_Count
* Desc: Get the number of entries in the table
*  Ret: number of entries in table
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  ISS_Count ()
{
int i;
   for ( i = 0; i < eC_ISS; i++ ) {
     if (!strcmp (sr_ISS[i].cr_Spe,"") )
       break;
   }
   return i;
}

/********************************************************************
* Name: ISS_Total
* Desc: Get the original density, the remaining live and the total
*       killed.
* NOTE: there is one entry in the table for every tree species the
*        user entered in Mortality GUI tree grid
********************************************************************/
int  ISS_Total (float *af_Den, float *af_Live, float *af_Kill, float f_DBH )
{
int i;
float Den, Live, Kill; 
   Den = Live = Kill = 0; 
   for ( i = 0; i < eC_ISS; i++ ) {
     if (!strcmp (sr_ISS[i].cr_Spe,"") ) 
        break; 
     if ( sr_ISS[i].f_DBH < f_DBH )
        continue;
      Den  += sr_ISS[i].i_Den;                  /* Original Density */
      Live += sr_ISS[i].i_Den - sr_ISS[i].i_Killed;
      Kill += sr_ISS[i].i_Killed;
   }
   *af_Den = Den; 
   *af_Live = Live;
   *af_Kill = Kill;    

   return 1; 
}

/********************************************************************
* Name: ISS_AvgDBHKill
* Desc: for the entire table get the average DBH of the killed 
*        trees
* NOTE: there is one entry in the table for every tree species the
*        user entered in Mortality GUI tree grid
********************************************************************/
float ISS_AvgDBHKill ()
{
int i;
float f, f_DBH, f_Kill;

   f_DBH = f_Kill = 0; 
 
   for ( i = 0; i < eC_ISS; i++ ) {
     if (!strcmp (sr_ISS[i].cr_Spe,"") ) 
        break; 
     f_DBH  += sr_ISS[i].f_DBH * sr_ISS[i].i_Killed;
     f_Kill += sr_ISS[i].i_Killed;
   }

   if ( f_Kill != 0 ) 
      f = f_DBH / f_Kill;  
   else 
      f = 0; 

   return f; 
}

/********************************************************************
* Name: ISS_BasalArea
* Desc: Get Basal Areas for entire table
*       Returned in Square Feet  
* NOTE: there is one entry in the table for every tree species the
*        user entered in Mortality GUI tree grid
********************************************************************/
int  ISS_BasalArea (float *af_Den, float *af_Live, float *af_Kill )
{
int i, i_Live;
float Den, Live, Kill; 
   Den = Live = Kill = 0; 
   for ( i = 0; i < eC_ISS; i++ ) {
     if (!strcmp (sr_ISS[i].cr_Spe,"") ) 
        break; 
      Den  += Basal_Area (sr_ISS[i].f_DBH,sr_ISS[i].i_Den);                  /* Original Density */
      i_Live = sr_ISS[i].i_Den - sr_ISS[i].i_Killed;
      Live += Basal_Area (sr_ISS[i].f_DBH, i_Live); 
      Kill += Basal_Area (sr_ISS[i].f_DBH,sr_ISS[i].i_Killed);
   }
   *af_Den = Den / 144.0;     /* from sq/in to sq/ft */
   *af_Live = Live / 144.0;
   *af_Kill = Kill / 144.0;    
   return 1; 
}

/********************************************************************
* Name: ISS_CrownCover
* Desc: Get Crown Cover for entire table    
* NOTE: there is one entry in the table for every tree species the
*        user entered in Mortality GUI tree grid
* Note-1: This functions gets called in the course of doing PFI - Post-fire
*         Injury mortality batch totals, but PFI input doesn't have
*         tree height, so we don't want to do crown cover and return
*         zeros so the batch can output zeros
********************************************************************/
int  ISS_CrownCover (float *af_Den, float *af_Live, float *af_Kill, char A[] )
{
int i, ii,jj,kk, i_Live;
float f,g; 
float f_CC, Den, Live, Kill; 

  *af_Den  = 0; 
  *af_Live = 0;
  *af_Kill = 0; 
   strcpy (A,""); 
   Den = Live = Kill = 0; 
   for ( i = 0; i < eC_ISS; i++ ) {
     if (!strcmp (sr_ISS[i].cr_Spe,"") ) 
        break;
     if ( sr_ISS[i].i_TreHgt <= 0 )    /* See Note-1 above */  
        continue;  
     f_CC = SMT_CalcCrnCov (sr_ISS[i].cr_Spe, (float)sr_ISS[i].f_DBH, (float)sr_ISS[i].i_TreHgt);
     Den  += f_CC * (float) sr_ISS[i].i_Den;                   /* Crwn Cov or orig density  */
     Kill += f_CC * (float) sr_ISS[i].i_Killed;                /*  area of trees killed        */
     i_Live = sr_ISS[i].i_Den - sr_ISS[i].i_Killed;
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


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CC_Overlap
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
float CC_Overlap( float f_SqFtCov)
{
float x,p;
#define  e_SqFtAcre 43560.0

   x = (f_SqFtCov / e_SqFtAcre);
   p = 100.0 * ( 1.0 -  exp ( -x ) ) ;
   return p;
}


#ifdef wow
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: ISS_Display
* Desc: Used for testing
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void ISS_Display ()
{
int i;
   printf ("--------------- ISS Table -------------------\n");
   for ( i = 0; i < eC_ISS; i++ ) {
     if (!strcmp (sr_ISS[i].cr_Spe,"") )          /* Find 1st avail entry      */
       break;
     printf ("%s,  Den:%d,   DBH:%d,   TreHgt:%d,   Crown:%d\n",
             sr_ISS[i].cr_Spe, sr_ISS[i].i_Den,
             sr_ISS[i].i_DBH,  sr_ISS[i].i_TreHgt,
             sr_ISS[i].i_CroRat);
   }
}
#endif
