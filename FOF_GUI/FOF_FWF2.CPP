/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_fwf2   Fuel Window Functions
*                  Excludes any Windows stuff,includes,etc
* Desc: Function that fill in the Fuel TPA, Duff, and Moisture Windows
*       Adjustment Loading Tables are also in here.
* Date: 7/27/06
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



#include "fof_fwf2.h"
//#include "win_ccw2.h"
#include "fof_util.h"

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/*                       Harvesting Debris Adjustments                       */
/* Pile & Slash use these                                                    */
/* Codes are used in the SAF & NVCS cover files                              */
/* Havest Debris code are used to adjust the Lit & Wood, when they are       */
/*  Slash or Piles, and depend on the Load Adjustment Setting, (wnd button)  */
/* There is no need to adjust for 'Natural'                                  */
/* Fuel Loads get adjusted by adding the values in the table below           */
/* NOTE: Default loads for missing codes do nothing to the loads, see        */
/*        entries in table below for empty strings.                          */
/*                                                                           */
/*                        Typ  Heavy  Light                                  */
d_HD sr_HD[] =  {
   { e_TPALit, "GA",  2.5,  3.0,  2.0 },
   { e_TPAW0,  "GA",  1.5,  2.5,  1.0 },
   { e_TPAW01, "GA",  3.5,  5.0,  1.5 },
   { e_TPAW13, "GA",  8.0,  12.0, 4.0 },
   { e_TPAW3,  "GA", 12.0,  22.0, 2.0 },

   { e_TPALit, "GB",  1.5,  2.0,  1.0 },
   { e_TPAW0,  "GB",  2.5,  3.5 , 1.0 },
   { e_TPAW01, "GB",  4.5,  6.5,  3.0 },
   { e_TPAW13, "GB", 12.0,  17.0, 7.0 },
   { e_TPAW3,  "GB", 15.0,  25.0, 5.0 },

   { e_TPALit, "GC",  2.0,   2.5,  1.5 },
   { e_TPAW0,  "GC",  2.5,   3.5,  1.0 },
   { e_TPAW01, "GC",  4.5,   6.5,  3.0 },
   { e_TPAW13, "GC",  8.0,  11.0,  5.0 },
   { e_TPAW3,  "GC", 20.0,  30.0, 10.0 },

   { e_TPALit, "GD", 0.0, 0.0, 0.0},    /* For Group Code GD, we don't   */
   { e_TPAW0,  "GD", 0.0, 0.0, 0.0},    /* adjust anything so 0 gets     */
   { e_TPAW01, "GD", 0.0, 0.0, 0.0},    /* added for Typ Light or Heavy  */
   { e_TPAW13, "GD", 0.0, 0.0, 0.0},
   { e_TPAW3,  "GD", 0.0, 0.0, 0.0},

/* This was Table 8 in Fofem Manual page 64.                                 */
/* For Heavy & Light we just 1.5 and .5 them                                 */
/*                      Typical   Heavy       Light                          */
   { e_TPALit, "GE",  3.0,  3.0 * 1.5,  3.0 * 0.5},
   { e_TPAW0,  "GE",  3.3,  3.3 * 1.5,  3.3 * 0.5},
   { e_TPAW01, "GE",  3.3,  3.3 * 1.5,  3.3 * 0.5},
   { e_TPAW13, "GE",  3.0,  3.0 * 1.5,  3.0 * 0.5},
   { e_TPAW3,  "GE",  3.0,  3.0 * 1.5,  3.0 * 0.5},

   { e_TPALit, "GF",  2.1,  2.1 * 1.5,  2.1 * 0.5 },
   { e_TPAW0,  "GF",  2.3,  2.3 * 1.5,  2.3 * 0.5 },
   { e_TPAW01, "GF",  2.3,  2.3 * 1.5,  2.3 * 0.5 },
   { e_TPAW13, "GF",  2.1,  2.1 * 1.5,  2.1 * 0.5 },
   { e_TPAW3,  "GF",  2.1,  2.1 * 1.5,  2.1 * 0.5 },

   { e_TPALit, "GG",  1.3,  1.3 * 1.5,  1.3 * 0.5 },
   { e_TPAW0,  "GG",  1.4,  1.4 * 1.5,  1.4 * 0.5 },
   { e_TPAW01, "GG",  1.4,  1.4 * 1.5,  1.4 * 0.5 },
   { e_TPAW13, "GG",  1.3,  1.3 * 1.5,  1.3 * 0.5 },
   { e_TPAW3,  "GG",  1.3,  1.3 * 1.5,  1.3 * 0.5 },

   { e_TPALit, "GH",  0.3,  0.3 * 1.5,  0.3 * 0.5 },
   { e_TPAW0,  "GH",  0.4,  0.4 * 1.5,  0.4 * 0.5 },
   { e_TPAW01, "GH",  0.4,  0.4 * 1.5,  0.4 * 0.5 },
   { e_TPAW13, "GH",  0.3,  0.3 * 1.5,  0.3 * 0.5 },
   { e_TPAW3,  "GH",  0.3,  0.3 * 1.5,  0.3 * 0.5 },

   { e_TPALit, "GI",  5.0,  5.0 * 1.5,  5.0 * 0.5 },
   { e_TPAW0,  "GI",  5.4,  5.4 * 1.5,  5.4 * 0.5 },
   { e_TPAW01, "GI",  5.4,  5.4 * 1.5,  5.4 * 0.5 },
   { e_TPAW13, "GI",  4.6,  4.6 * 1.5,  4.6 * 0.5 },
   { e_TPAW3,  "GI",  4.6,  4.6 * 1.5,  4.6 * 0.5 },

   { e_TPALit, "GJ",  3.6,  3.6 * 1.5,  3.6 * 0.5 },
   { e_TPAW0,  "GJ",  4.0,  4.0 * 1.5,  4.0 * 0.5 },
   { e_TPAW01, "GJ",  4.0,  4.0 * 1.5,  4.0 * 0.5 },
   { e_TPAW13, "GJ",  3.4,  3.4 * 1.5,  3.4 * 0.5 },
   { e_TPAW3,  "GJ",  3.4,  3.4 * 1.5,  3.4 * 0.5 },

   { e_TPALit, "GK",  2.3,  2.3 * 1.5,  2.3 * 0.5 },
   { e_TPAW0,  "GK",  2.5,  2.5 * 1.5,  2.5 * 0.5 },
   { e_TPAW01, "GK",  2.5,  2.5 * 1.5,  2.5 * 0.5 },
   { e_TPAW13, "GK",  2.2,  2.2 * 1.5,  2.2 * 0.5 },
   { e_TPAW3,  "GK",  2.2,  2.2 * 1.5,  2.2 * 0.5 },

   { e_TPALit, "GL",  0.6,  0.6 * 1.5,  0.6 * 0.5 },
   { e_TPAW0,  "GL",  0.7,  0.7 * 1.5,  0.7 * 0.5 },
   { e_TPAW01, "GL",  0.7,  0.7 * 1.5,  0.7 * 0.5 },
   { e_TPAW13, "GL",  0.7,  0.7 * 1.5,  0.7 * 0.5 },
   { e_TPAW3,  "GL",  0.7,  0.7 * 1.5,  0.7 * 0.5 },

   { e_TPALit, e_HD_NA, 0.0, 0.0, 0.0 }, /* Not Applicable Codes             */
   { e_TPAW0,  e_HD_NA, 0.0, 0.0, 0.0 }, /* Means do nothing                 */
   { e_TPAW01, e_HD_NA, 0.0, 0.0, 0.0 }, /* Set to 0 because we add this amt */
   { e_TPAW13, e_HD_NA, 0.0, 0.0, 0.0 }, /*  when doing adjustments          */
   { e_TPAW3,  e_HD_NA, 0.0, 0.0, 0.0 },

   { e_TPALit, "", 0.0, 0.0, 0.0},       /* For Group Code GD, we don't   */
   { e_TPAW0,  "", 0.0, 0.0, 0.0},       /* adjust anything so 0 gets     */
   { e_TPAW01, "", 0.0, 0.0, 0.0},       /* added for Typ Light or Heavy  */
   { e_TPAW13, "", 0.0, 0.0, 0.0},
   { e_TPAW3,  "", 0.0, 0.0, 0.0},

   { -1,           "",      0.0, 0.0, 0.0 }};


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/*                              Dead Adjustments                             */
/* Litter, & Down Woody....                                                  */
/* use to adjust Loadings to Light or Heavy - Fuel Category Natural          */
d_DA sr_DA[] = {
     {  e_TPALit, 0.5, 2.0 },   /* Litter     old -> 0.5, 1.2         */
     {  e_TPAW0,  0.5, 1.5 },   /* Down Wood  old -> 0.7, 1.3         */
     {  e_TPAW01, 0.5, 1.5 },   /* Down Wood  old -> 0.7, 1.3         */
     {  e_TPAW13, 0.5, 1.5 },   /* Down Wood  old -> 0.7, 1.3         */
     {  e_TPAW3,  0.5, 2.0 },   /* Down Wood  old -> 0.4, 1.6         */
     {  -1,           0.0, 0.0 } };

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/*                             Live Adjustments                              */
/* Herb, Shrub, Crown Foliage & Crown Branch                                 */
/* using their Adjustment Codes in the Fuel .dat file                        */
/* Live Loads get adjusted by multilying by values in table below            */
/* NOTE: Defualt setting for a missing code (null string), there is an       */
/*        entry below,                                                       */
/* NOTE: set 'NA' to 1 because we use Adj to multply                         */
/*       Null strings which are missing codes get adjust as shown below      */
/*                                                                           */
d_LA sr_LA [] = {
   /*                       Spa  Abu  */
   { e_TPAHer, "H1",    0.5,  1.5  },
   { e_TPAHer, "H2",    0.66, 1.33 },
   { e_TPAHer, "H3",    0.5,  2.0  },
   { e_TPAHer, e_LA_NA, 1.0,  1.0  },
   { e_TPAHer, "",      0.5,  1.5  },  /* Missing Codes                  */

   { e_TPAShr, "S1",    0.5, 1.5 },
   { e_TPAShr, "S2",    0.0, 2.0 },
   { e_TPAShr, e_LA_NA, 1.0, 1.0 },
   { e_TPAShr, "",      0.5, 1.5 },    /* Missing Codes                  */

   { e_TPAFol, "F1",    0.5, 1.5 },
   { e_TPAFol, "F2",    0.5, 2.0 },
   { e_TPAFol, e_LA_NA, 1.0, 1.0 },
   { e_TPAFol, "",      0.5, 1.5 },    /* Missing Codes                  */

   { e_TPASmB, "B1",    0.5, 1.5 },
   { e_TPASmB, "B2",    0.5, 2.0 },
   { e_TPASmB, e_LA_NA, 1.0, 1.0 },
   { e_TPASmB, "",      0.5, 1.5 },    /* Missing Codes                  */

   { -1,            "",   ' ', 0 }};

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/*                                Moisture                                   */
// 9-10-00 changed...........
// float fr_DufMoi[4] =   { 40, 75, 120,  200 };  /* Duff Code: Entire & Lower  */

/* NOTE NOTE NOTE:                                                           */
/*  DON'T change the order of these values in the array, FOFEM & DEQ         */
/*  depending on the same ordering, Duff, DW10 and DW1000 moist values order */
float fr_N_DufMoi[4] = { 13, 15,  20,   30 };  /* Duff Code: NFDR            */
float fr_A_DufMoi[4] = { 18, 21,  28,   42 };  /* Duff Code: Adj-NFDR        */

/* Change 6-13-3 for Fofem 5.1 version.............................          */
float fr_DufMoi[4] =   { 20, 40,  75,  130 };  /* Duff Code: Entire & Lower  */
float fr_DW10  [4]   = {  6, 10,  16,   22 };
float fr_DW1000[4]   = { 10, 15,  30,   40 };

// Old values..................
// float fr_DufMoi[4] =   { 40, 75, 100,  175 };  /* Duff Code: Entire & Lower  */
// float fr_DW10  [4]   = { 10, 15,  25,   40 };
// float fr_DW1000[4]   = { 13, 15,  20,   30 };

void Error_Window2 (char A[], char B[])
{
int i;
   i++;
} 

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: DA_AdjLight
* Desc: Get the Dead Fuel Light Adjustment factor
*       This is for Litter, and the Down Woody
*   In: i_IdAmt........id in table - see table declaration
*  Ret: Light Adjustment,   0 Error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
float DA_AdjLight (int i_IdAmt)
{
int i;
   for ( i = 0; i < 1000; i++ ) {
     if ( sr_DA[i].i_IdAmt == -1 )
        return 0;
     if ( sr_DA[i].i_IdAmt == i_IdAmt )
       return sr_DA[i].f_Light; }
  return 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: DA_AdjHeavy
* Desc: Get the Dead Fuel Heavy Adjustment factor
*       This is for Litter, and the Down Woody
*   In: i_IdAmt........id in table - see table declaration
*  Ret: Light Adjustment,   0 Error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
float DA_AdjHeavy (int i_IdAmt)
{
int i;
   for ( i = 0; i < 1000; i++ ) {
     if ( sr_DA[i].i_IdAmt == -1 )
        return 0;
     if ( sr_DA[i].i_IdAmt == i_IdAmt )
       return sr_DA[i].f_Heavy;  }
  return 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: FWF_HDAdj
* Desc: Get the Harvesting Debris Adjustment amount for Harvest Debris
*        Code table
* NOTE: See the see table declaration
*   In: cr_HvCod....Harvesting Debris Code, This the Code that
*                   comes from the Fuel .dat file
*       c_MHL......'M','L','H', Mean, Light, Heavy
*  Ret: Amount
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
float FWF_HDAdj (int i_IdAmt, char cr_Code[], char c_MHL)
{
int i;
char cr[300], cr1[100];

   for ( i = 0; i < 1000; i++ ) {            /* Look thru table              */
     if ( sr_HD[i].i_IdAmt == -1 )           /* Should have found an entry   */
       goto Error;                           /*  even for missing (null)     */
     if ( sr_HD[i].i_IdAmt != i_IdAmt )      /* look at type, Lit, DW1, etc  */
       continue;
     if ( stricmp (sr_HD[i].cr_Code,cr_Code))/* look at code, GA, GB etc     */
       continue;

     if ( c_MHL == 'M' )                     /* Found now get adj val for    */
       return sr_HD[i].f_Mean;               /*  mean(typical), hvy or lit   */
     else if ( c_MHL == 'H' )
       return sr_HD[i].f_Heav;
     else if ( c_MHL == 'L' )
       return sr_HD[i].f_Ligh;
     else
       break;  }
Error:
       strcpy  (cr,"Invalid Haversting Debris Code for this Cover type.\n");
       strcat  (cr,"See the cover .dat file\n");
       sprintf (cr1,"Code: |%s|   Length: %d",cr_Code,strlen(cr_Code));
       strcat  (cr,cr1);
       if ( isBlank (cr_Code) )
         strcat (cr," - This code is Blank");
       Error_Window2 (cr,"FWF_HarDeb");
       return 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: FWF_LAadj
* Desc: Get the Sparse Abundent adjustment value from the Sparse-Abundant
*       table.
* NOTE: See Table declaration comments
*   In: i_IdAmt.....CCW Id see table declaration
*       cr_Code.....Code from fuel file, s_CFL.cr_Herb, Shrub, Fol Bran
*       c_LA........'S', 'A'  Load Adjustment Sparse or Abundent 
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
float FWF_LAadj (int i_IdAmt,  char cr_Code[], char c_LA )
{
int i;
char cr[300];

   for ( i = 0; i < 1000; i++ ) {
     if ( sr_LA[i].i_IdAmt == -1 )           /* Hit end                      */
       goto Error;
     if ( sr_LA[i].i_IdAmt != i_IdAmt )      /* Match Id                     */
       continue;
     if ( stricmp (sr_LA[i].cr_Code,cr_Code))/* Match Code                   */
       continue;
     if ( c_LA == 'S' )                      /* Sparse                       */
        return sr_LA[i].f_SpaAdj;
     else if ( c_LA == 'A' )                 /* Abundant                     */
        return sr_LA[i].f_AbuAdj;
     else
        break;  }                       /* Shouldn't get here           */
Error:
    sprintf (cr," Invalid Sparse-Abundant Code: %s - %c", cr_Code,c_LA);
    Error_Window2 (cr, "FSF_LA");
    return 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: FWF_LAValid
* Desc: See if a code is in the table Sparse, Abundent Code Table
*   In: i_IdAmt........id in table - see table declaration
*       cr_Code........Code
*  Ret: 1 ok,  0 not found
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int  FWF_LAValid (int i_IdAmt, char cr_Code[])
{
int i;
   for ( i = 0; i < 1000; i++ ) {
     if ( sr_LA[i].i_IdAmt == -1 )
        return 0;
     if ( sr_LA[i].i_IdAmt == i_IdAmt && !stricmp (sr_LA[i].cr_Code,cr_Code))
       return 1; }
   return 0;
}
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: FWF_HDValid
* Desc: See if a code is in the table
*   In: cr_HvCod....Harvesting Debris Code
*  Ret: 1 ok,  0 not found
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int  FWF_HDValid (char cr_Code[])
{
int i;
   for ( i = 0; i < 1000; i++ ) {
     if ( sr_HD[i].i_IdAmt == -1 )
       return 0;
     if ( !stricmp (sr_HD[i].cr_Code,cr_Code))
       return 1; }
   return 0;
}



