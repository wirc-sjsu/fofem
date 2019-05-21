/***********************************************************************
* Name: CVT_CVT.cpp
* Desc: Cover Table 
*
************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <math.h> 

#include "Wnd_Mai.h" 
#include "Wnd_Util.h"
#include "Wnd_Def.h"
#include "fof_util.h"

#include "fof_ci.h" 
#include "CVT_CVT.h"
#include "fof_fwf2.h"

 d_CVT sr_CVT[eC_CVT];    /* All Cov Typs from disk file       */

/*********************************************************************
* Name: CVT_GetCovGrp
* Desc: find the the Cover Group code in the CVT table
*       Not every Cover Type has a Cover Group code
*   In: cr_CovTyp....the full cover type description, like that in
*                    used in the GUI dropdown combo box
*  Out: cr_CovGrp....the 2/3 letter code or "" if none 
**********************************************************************/
int   CVT_GetCovGrp (char cr_CovTyp[], char cr_CovGrp[])
{
int i;

  strcpy (cr_CovGrp,""); 
  for ( i = 0; i < eC_CVT; i++ ) {
    if ( !strcmp ( sr_CVT[i].cr_CovTyp,"") ) {
      return -1; }
    if ( !strcmp ( sr_CVT[i].cr_CovTyp,cr_CovTyp) )
       break;
  }

  strcpy (cr_CovGrp, sr_CVT[i].cr_CovGrp); 

  return i;
}



/***************************************************************************
* Name: CVT_GetCovReg
* Desc: For the specified table index and Region get the Cover Type 
*       Allows caller to go thru table and grap all cover types for
*       a particular region
*   In: iX....index
*       iX_Reg..region index, 
*  Out: cr_CovTyp
****************************************************************************/
int CVT_GetCovReg (int iX, int iX_Reg, char cr_CovTyp[]) 
{
  strcpy (cr_CovTyp,""); 
  
   if ( !_stricmp ( sr_CVT[iX].cr_CovTyp, "" ) )
    return -1 ;        /* End of table */

  if ( sr_CVT[iX].ir_Region[iX_Reg] == -1 ) 
     return 0; 

  strcpy (cr_CovTyp,sr_CVT[iX].cr_CovTyp); 
  
  return 1; 
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_GetCFL 
* Desc: Get the Cover Fuel loads from CVT table
*   In: cr_CovTyp....cover type
*  Out: a_CFL.......copy of
*       cr_CovGrp...cover group 
*  Ret: index of found record, -1 not found - this would be a logic error 
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int   CVT_GetCFL ( char cr_CovTyp[], d_CFL *a_CFL,  char cr_CovGrp[])
{
int i;

  for ( i = 0; i < eC_CVT; i++ ) {
    if ( !strcmp ( sr_CVT[i].cr_CovTyp,"") ) {
      return -1; }
    if ( !strcmp ( sr_CVT[i].cr_CovTyp,cr_CovTyp) )
       break;
  }

  memcpy (a_CFL, &sr_CVT[i].s_CFL, sizeof(d_CFL) );
  strcpy (cr_CovGrp, sr_CVT[i].cr_CovGrp);

  return i;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_pcRot
* Desc: Get the percent Rotten 3+ fuel, this is only relevant for
*       FCCS cover types 
*   In: cr_CovTyp....cover type
*  Ret:  
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
float    CVT_pcRot ( char cr_CovTyp[])
{
int i;

  for ( i = 0; i < eC_CVT; i++ ) {
    if ( !strcmp ( sr_CVT[i].cr_CovTyp,"") ) {
      return 0; }
    if ( !strcmp ( sr_CVT[i].cr_CovTyp,cr_CovTyp) )
       break;
    
  }

  return sr_CVT[i].s_CFL.f_pcRot; 
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_Dummy
* Desc: This Puts a single Dummy Record into the CVT table. With its region
*       array set with any region that is not found in the CVT.
*       This is to make sure that there is at least one record for each
*       region in the CVT, even if that record is a dummy record.
* Note-1: This looks for each of the 4 regions, but FCC is for region
*         1 and 2 (Pacific & Interior West) so they'll be there, most this
*         should just find 3 & 4 missing and make the dummy record for them.
*         But that may change someday.
*   In: iX....CVT index where to put dummy record
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void CVT_Dummy(int iX)
{
   sr_CVT[iX].i_Num = 0;                     /* Cover Number as int    */
   strcpy (sr_CVT[iX].cr_CovTyp,"-> No Cover Records for Region <-");

   sr_CVT[iX].ir_Region[0] = -1;
   sr_CVT[iX].ir_Region[1] = -1;
   sr_CVT[iX].ir_Region[2] = -1;
   sr_CVT[iX].ir_Region[3] = -1;

   if ( !SAF_isReg(1) )                      /* Note-1 above                 */
     sr_CVT[iX].ir_Region[0] = 1;
   if ( !SAF_isReg(2) )
     sr_CVT[iX].ir_Region[1] = 2;
   if ( !SAF_isReg(3) )
     sr_CVT[iX].ir_Region[2] = 3;
   if ( !SAF_isReg(4) )
     sr_CVT[iX].ir_Region[3] = 4;
}