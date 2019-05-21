/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_slk    Species Live Kill Table
* Desc: Table is used to store Live/Kill totals for the Mortality Report
*       Each entry is the table is for a unique species, and in that
*       record is an array for live and killed totals for each DBH
*       this is for dealing with Diameter Classes (a range of DBH is a
*       Diameter).
*       Also the Mortality report is broken up into to 2 groups,
*       Diameter Classes 2-20, and 22-40, so this tables helps out with
*       dealing with that
* Date: 8/11/99
* Author: Larry Gangi
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
#include <windows.h>
#include <windowsx.h>
#include <winuser.h>
#include <winbase.h>
#include <commctrl.h>

#include "wnd_mai.h"

#include "wnd_util.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <dos.h>

#define printf xprintf
void far xprintf (char *Format, ...);

#include "fof_SLK.h"
// #include "fof_gen.h"
#include "fof_util.h"

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
#define eC_SLK  500
d_SLK sr_SLK [eC_SLK];

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SLK_GetSpe
* NOTE: Must call this after Load_SLK
* Desc: Get the Species from the specified entry in table
*       Use this to iterativly get next species name from table
*   In: Index into table
*  Out: cr_Spe.....Species, Null when no more
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void SLK_GetSpe (int iX, char cr_Spe[])
{
  strcpy (cr_Spe, sr_SLK[iX].cr_Spe);
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SLK_Init
* Desc:
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void SLK_Init ()
{
int i;
   for ( i = 0; i < eC_SLK; i++ )
     strcpy (sr_SLK[i].cr_Spe,"");
   SLK_Zero ();
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SLK_Zero
* Desc:
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void SLK_Zero ()
{
int i,j;
   for ( i = 0; i < eC_SLK; i++ ) {
     for ( j = 0; j < eC_LivKil; j++ ) {
       sr_SLK[i].ir_Live[j] = 0;
       sr_SLK[i].ir_Kill[j] = 0; } }
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SLK_Put
* Desc: Put the Species, Live and Killed values into table
*       If species is not already in table it will be added
*       Live and Killed are added in, 
*       Trees 0->0.9 go into [0]
*             1->1.9 go into [1],
*             2->2.9 go into [2], etc 
*   In: cr_Spe.....Species
*       f_DBH......DBH
*       i_Live.....number of live trees
*       i_KIlled...number of trees killed
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  SLK_Put (char cr_Spe[], float f_DBH, int i_Live, int i_Kill)
{
int i, iX;
   if ( f_DBH >= 40.0 )                    /* Don't go over 40                  */
     iX = 39;
   else if ( f_DBH < 0 )                     /* Don't go less than 1              */
     iX = 0;
   else
     iX = (int) f_DBH;


   for ( i = 0; i < eC_SLK; i++ ) {
     if ( !strcmp (sr_SLK[i].cr_Spe, "" ) )    /* if not in table add it in  */
       strcpy (sr_SLK[i].cr_Spe,cr_Spe);

     if ( strcmp (sr_SLK[i].cr_Spe, cr_Spe ) ) /* not a matching species     */
       continue;

     sr_SLK[i].ir_Live[iX] += i_Live;          /* on match add in            */
     sr_SLK[i].ir_Kill[iX] += i_Kill;
     return 1;                                 /* Ok Get outta here          */
   }
   
   return 1;  /* get rid of compiler warning */
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SLK_ifGroup
* NOTE: Must call this after Table has been loaded with Live & Killed
* Desc: Look thru every entry in the table and see if there are any
*        values in the Live or Killed, for the specified Group 1 or 2
*        Group 1 is DBH 1 -> 20,  Group 2 is DBH 21 40
*  In: 1 or 2, Group #
* Ret: 1 there is something,   else 0
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int   SLK_ifGroup (int i_Grp)
{
int i,j, A, B;
   if ( i_Grp == 1 ) {        /* Group 1 is DBH 0 -> 19.99 */
     A = 0;
     B = 19;}
   else {                     /* Group 2 is DBH 20 -> 40+ */
     A = 20;
     B = 39;}

   for ( i = 0; i < eC_SLK; i++ ) {            /* Each entry                 */
     if ( !strcmp (sr_SLK[i].cr_Spe,"" ) )     /* Hit end                    */
       break;
     for ( j = A; j <= B; j++ ) {              /* Look at each Live Value    */
      if ( sr_SLK[i].ir_Live[j] != 0 )         /* in specifed Group          */
         return 1;                             /* if any non 0               */
      if ( sr_SLK[i].ir_Kill[j] != 0 )         /* Do same thing in Killed    */
         return 1; }
   }
   return 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SLK_GetDen - Get Density
* NOTE: Must call this after Table has been loaded with Live & Killed
*       amounts, this function will sum them to get the original stand
*       density.
* Desc: Send in the mid-point for the class, 
*       1 will sum  [0] + [1], which contains DBH 0 -> 1.99
*       3 will sum  [2] + [3]  which contains DBH 2 -> 3.99
*       39 will sum [38] + [39]  which containa DBH 38 -> 39.99 NOTE all
*        DBH 40+ are also in here
*  In:  iX..........index to table entry
*       i_SizCls....Size Class mid-point
* Ret:  Density,
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int   SLK_GetDen (int iX, int i_SizCls)
{
int i,j, i_Den;
  if ( i_SizCls < 1 || i_SizCls > 39 ) {
    myMessageBox("Logic Error - SLK_GetDen");
    return 0; }

  if ( ( i_SizCls + 1 ) % 2  != 0 ) {
    myMessageBox ("Logic Error - SLK_GetDen");
    return 0; }

  i = i_SizCls;
  j = i_SizCls - 1;

  i_Den = sr_SLK[iX].ir_Live[i] + sr_SLK[iX].ir_Live[j] +
          sr_SLK[iX].ir_Kill[i] + sr_SLK[iX].ir_Kill[j];

  return i_Den;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SLK_GetLK   - Get Live or Killed amount for a size class
* NOTE: Must call this after Table has been loaded with Live & Killed
*       amounts, this function will sum them to get the original stand
*       density.
* Desc: See "Desc: above in SLK_GetDen() 
*  In:  iX..........entry to do
*       i_SizCls....Size Class mid point
*       c...........'K' or 'L'
* Ret:  Killed amount
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int   SLK_GetLK  (int iX, int i_SizCls, char c)
{
int i,j, i_Amt;
  if ( i_SizCls < 1 || i_SizCls > 39 ) {
    myMessageBox ("Logic Error - SLK_GetLK 1");
    return 0; }

  if ( (i_SizCls + 1) % 2  != 0 ) {        /* Note-1                               */
    myMessageBox ("Logic Error - SLK_GetLK 2");
    return 0; }

  i = i_SizCls;
  j = i_SizCls - 1;

  if ( c == 'L' )
    i_Amt = sr_SLK[iX].ir_Live[i] + sr_SLK[iX].ir_Live[j];
  else
    i_Amt = sr_SLK[iX].ir_Kill[i] + sr_SLK[iX].ir_Kill[j];

  return i_Amt;
}

/********************************************************************
*
*
********************************************************************/
int  SLK_Total (float *af_Live, float *af_Kill, int X)
{
int i, j;
float Live, Kill;
 
   Live = 0;
   Kill = 0; 

   for ( i = 0; i < eC_SLK; i++ ) {
     if ( !strcmp (sr_SLK[i].cr_Spe,"" ) )
       break;
     for ( j = 0; j < eC_LivKil; j++ ) {
       if ( j >= X ) {
         Live += sr_SLK[i].ir_Live[j];
         Kill += sr_SLK[i].ir_Kill[j]; }
     } 
   }
   *af_Live = Live;
   *af_Kill = Kill; 
   return 1; 
}



