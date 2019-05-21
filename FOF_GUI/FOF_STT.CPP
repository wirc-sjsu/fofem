/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_STT    Soil Totals Table
* Desc:
* Date: 12/31/99
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
//#include <windows.h>
//#include <windowsx.h>
//#include <winuser.h>
//#include <winbase.h>
//#include <commctrl.h>
//#include <process.h>

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

// #include  "win_mai.h"
#include  "fof_util.h"
#include  "fof_st.h"
#include  "fof_stt.h"

#include "wnd_mai.h"
#include "Wnd_Util.h"


#define eC_STT eC_Totals
d_STT sr_STT[eC_STT];


void Error_Wind(char A[], char B[]); 

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: STT_Init
* Desc: Init the STT Table
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
void  STT_Init ()
{
int i;
  for ( i = 0; i < eC_STT; i++ ) {
    strcpy (sr_STT[i].cr_Name,"");
    sr_STT[i].i_Lay1 = -1;
    sr_STT[i].i_Lay2 = -1;
  }
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: STT_Save
* Desc: Save a char Name and 2 ints
* Note-1: Want make sure there is always at least 1 empty record at end
*          of table so that the STT_Get function can detect the end.
*  Ret: 1 OK,  0 Error Table is full
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int   STT_Save (char cr_Name[], int i_Lay1, int i_Lay2)
{
int i,j;
  j = eC_STT - 1;                             /* Note-1                      */
  for ( i = 0; i < j; i++ ) {                 /* go thru and look for 1st    */
    if ( !strcmp (sr_STT[i].cr_Name,"")) {     /*  empty rec                  */
      strcpy (sr_STT[i].cr_Name,cr_Name);
      sr_STT[i].i_Lay1 = i_Lay1;
      sr_STT[i].i_Lay2 = i_Lay2;
      return 1; }                             /* OK                          */
  }
  Error_Wind ("Maximum number of Soil Summaries Exceeded", "STT_Save");
  return 0;                                   /* Table is Full               */
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: STT_Get
* Desc: Iterative get next record
*  Ret: iX.....index, start out 0
*  Out: cr_Name....Total Name, if NULL ("") then there are no more
*  ai_Lay1, ai_Lay2....layer 1 and 2
*  Ret: 1 OK,  0 No More
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int   STT_Get (int iX, char cr_Name[], int *ai_Lay1, int *ai_Lay2)
{
   if ( iX >= eC_STT ) {
     Error_Wind ("Logic Error, Trying to read past end", "STT_Get");
     return 0; }

   if ( !strcmp (sr_STT[iX].cr_Name,"") )
     return 0;

   strcpy (cr_Name,sr_STT[iX].cr_Name);
   *ai_Lay1 = sr_STT[iX].i_Lay1;
   *ai_Lay2 = sr_STT[iX].i_Lay2;
   return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: STT_Count
* Desc: return number of totals stored in table
*  Ret: number of total stored
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int   STT_Count ()
{
int i;
  for ( i = 0; i < eC_STT; i++ ) {
    if ( !strcmp (sr_STT[i].cr_Name,""))
      break;
  }
  return i;
}


/***********************************************************
* Stub this for now 
**********************************************************/
void Error_Wind(char A[], char B[])
{
char cr[2000];
  
  sprintf (cr, "%s - %s", A, B); 
  myMessageBox (cr); 

}