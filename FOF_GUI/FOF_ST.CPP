/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_st.c
* Desc: Summary Total-er
*        Individual runs or Fuel or Smoke reports use this to save their
*         totals so then can put a summary to the report when the user
*         requests it
* Date:  8/20/99
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
//#include <windows.h>
//#include <windowsx.h>
//#include <winuser.h>
//#include <winbase.h>
//#include <commctrl.h>


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>


// #include "win_mai.h"
#include "fof_st.h"
#include "fof_util.h"
#include "wnd_mai.h"
#include "Wnd_Util.h"

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
#define  eC_STS eC_Totals
d_STS sr_STS[eC_STS];                /* Smoke Totals                         */
int   giN_BatS = -1;                 /* counts number of batch runs */

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
#define  eC_ST  eC_Totals
d_ST  sr_ST[eC_ST];                  /* Fuel Totals                          */
int   giN_Bat = -1;                  /* counts number of batch runs */

void Error_Window(char A[], char B[]); 

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: STS_Init
* Desc: Init the Smoke Totals table
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void STS_Init ()
{
int i;
  for ( i = 0; i < eC_STS; i++ )
    STS_Zero (&sr_STS[i]);
  giN_BatS = -1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: STS_Zero
* Desc: 0 out a single Smoke Total Structure
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void STS_Zero (d_STS *a_STS)
{
//   a_STS->f_TotEff = 0;
   a_STS->f_TotPM10 = 0;
   a_STS->f_TotPM25 = 0;
   a_STS->f_TotCH4 = 0;
   a_STS->f_TotCO = 0;
   a_STS->f_TotCO2 = 0;
   a_STS->f_TotNOX = 0;
   a_STS->f_TotSO2 = 0;
   strcpy (a_STS->cr_Name,"");
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: STS_Count
* Desc: See how many entries are in the table
*  Ret: number of entries in table
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int   STS_Count ()
{
int i;
  for ( i = 0; i < eC_STS; i++ ) {
    if ( !strcmp (sr_STS[i].cr_Name,"") )       /* Hit end of tbl and didn't  */
      return i;
  }
  Error_Window ("Logic Error", "STS_Count");
  return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: STS_Add
* Desc: Make an entry into the Smoke Totals table
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void STS_Add (d_STS *a_STS, char cr_Name[])
{
  giN_BatS++;
  if ( giN_BatS == eC_STS ) {
    Error_Window ("Batch Total Array Size Exceeded","STS_Add");
    return; }
//  sr_STS[giN_BatS].f_TotEff  = a_STS->f_TotEff;

  sr_STS[giN_BatS].f_TotPM10 = a_STS->f_TotPM10;
  sr_STS[giN_BatS].f_TotPM25 = a_STS->f_TotPM25;
  sr_STS[giN_BatS].f_TotCH4  = a_STS->f_TotCH4;
  sr_STS[giN_BatS].f_TotCO2  = a_STS->f_TotCO2;
  sr_STS[giN_BatS].f_TotCO   = a_STS->f_TotCO;

  sr_STS[giN_BatS].f_TotNOX  = a_STS->f_TotNOX;
  sr_STS[giN_BatS].f_TotSO2  = a_STS->f_TotSO2;

  if ( strlen (cr_Name) <= eC_STSNam )
    strcpy (sr_STS[giN_BatS].cr_Name,cr_Name);
  else
    strncpy (sr_STS[giN_BatS].cr_Name,cr_Name,eC_STSNam);
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: STS_Get
* Desc: Get an entry back from the table.
*       You can get each entry out by sending in an incrementing index.
*   In: iX....index into table or struct you want back
*  Out: a_STS
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  STS_Get (d_STS *a_STS, int iX)
{
  if ( iX > giN_BatS )
    return 0;
//  a_STS->f_TotEff   = sr_STS[iX].f_TotEff;

  a_STS->f_TotPM10  = sr_STS[iX].f_TotPM10;
  a_STS->f_TotPM25  = sr_STS[iX].f_TotPM25;
  a_STS->f_TotCH4   = sr_STS[iX].f_TotCH4;
  a_STS->f_TotCO2   = sr_STS[iX].f_TotCO2;
  a_STS->f_TotCO    = sr_STS[iX].f_TotCO;

  a_STS->f_TotNOX   = sr_STS[iX].f_TotNOX;
  a_STS->f_TotSO2   = sr_STS[iX].f_TotSO2;

  strcpy (a_STS->cr_Name, sr_STS[iX].cr_Name);
  return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: ST_Init
* Desc:
* Date:
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void ST_Init ()
{
int i;
  for ( i = 0; i < eC_ST; i++ )
    ST_Zero (&sr_ST[i]);

  giN_Bat = -1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: ST_Zero
* Desc:
* Date:
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void ST_Zero (d_ST *a_ST)
{
  a_ST->f_Litter = 0;
  a_ST->f_DW0 = 0;
  a_ST->f_DW10 = 0;
  a_ST->f_DW100 = 0;
  a_ST->f_DW1000 = 0;
  a_ST->f_DufAmtCon = 0;
  a_ST->f_DufDepRed = 0;
  a_ST->f_Herb = 0;
  a_ST->f_Shrub = 0;
  a_ST->f_CroBra = 0;
  a_ST->f_CroFol = 0;
  strcpy (a_ST->cr_Name,"");
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: ST_Add
* Desc:
* Date:
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void ST_Add (d_ST *a_ST, char cr_Name[])
{
  giN_Bat++;
  if ( giN_Bat == eC_ST ) {
    Error_Window ("Batch Total Array Size Exceeded","ST_Add");
    return; }
  sr_ST[giN_Bat].f_Litter = a_ST->f_Litter;
  sr_ST[giN_Bat].f_DW0    = a_ST->f_DW0;
  sr_ST[giN_Bat].f_DW10   = a_ST->f_DW10;
  sr_ST[giN_Bat].f_DW100  = a_ST->f_DW100;
  sr_ST[giN_Bat].f_DW1000 = a_ST->f_DW1000;
  sr_ST[giN_Bat].f_DufAmtCon = a_ST->f_DufAmtCon;
  sr_ST[giN_Bat].f_DufDepRed = a_ST->f_DufDepRed;
  sr_ST[giN_Bat].f_Herb   = a_ST->f_Herb;
  sr_ST[giN_Bat].f_Shrub  = a_ST->f_Shrub;
  sr_ST[giN_Bat].f_CroBra = a_ST->f_CroBra;
  sr_ST[giN_Bat].f_CroFol = a_ST->f_CroFol;
  if ( strlen (cr_Name) <= eC_STNam )
    strcpy (sr_ST[giN_Bat].cr_Name,cr_Name);
  else
    strncpy (sr_ST[giN_Bat].cr_Name,cr_Name,eC_STNam);
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: ST_Get
* Desc: Get next s_ST from table, keep sending in an incrementing index
*   In: iX......index
*  Out: a_ST
*  Ret: 1 Ok,   0 No More
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int ST_Get (d_ST *a_ST, int iX)
{
   if ( iX > giN_Bat )
     return 0;
   a_ST->f_Litter    = sr_ST[iX].f_Litter;
   a_ST->f_DW0       = sr_ST[iX].f_DW0;
   a_ST->f_DW10      = sr_ST[iX].f_DW10;
   a_ST->f_DW100     = sr_ST[iX].f_DW100;
   a_ST->f_DW1000    = sr_ST[iX].f_DW1000;
   a_ST->f_DufAmtCon = sr_ST[iX].f_DufAmtCon;
   a_ST->f_DufDepRed = sr_ST[iX].f_DufDepRed;
   a_ST->f_Herb     = sr_ST[iX].f_Herb;
   a_ST->f_Shrub    = sr_ST[iX].f_Shrub;
   a_ST->f_CroBra   = sr_ST[iX].f_CroBra;
   a_ST->f_CroFol   = sr_ST[iX].f_CroFol;
   strcpy (a_ST->cr_Name,sr_ST[iX].cr_Name);
   return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: ST_isName
* Desc: See if the specified name is in the table
*   In: cr_TotNam....name to look for
*  Ret: 1 Name is in Table,    0 Name not found in table
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int   ST_isName (char cr_TotNam[])
{
int i;
  for ( i = 0; i < eC_ST; i++ ) {
    if ( !strcmp (sr_ST[i].cr_Name,"") )       /* Hit end of tbl and didn't  */
      return 0;                                /* Name not found             */
    if ( !strcmp (sr_ST[i].cr_Name,cr_TotNam)) /* name found in table        */
      return 1; }

  Error_Window ("Logic Error", "ST_isName");
  return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: ST_Count
* Desc: See how many entries are in the table
*  Ret: number of entries in table
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int   ST_Count ()
{
int i;
  for ( i = 0; i < eC_ST; i++ ) {
    if ( !strcmp (sr_ST[i].cr_Name,"") )       /* Hit end of tbl and didn't  */
      return i;
  }
  Error_Window ("Logic Error", "ST_Count");
  return 1;
}

/***********************************************************
* Stub this for now 
**********************************************************/
void Error_Window(char A[], char B[])
{
char cr[2000];
  
  sprintf (cr, "%s - %s", A, B); 
  myMessageBox (cr); 

} 
