/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_pf.c    Post Fire Injury Mortality Table
* Desc:
* Date: 3/30/08
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
#include <windows.h>
#include <windowsx.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "Wnd_Mai.h"
#include "wnd_util.h"

#include  "fof_smt.h"
#include  "fof_st.h"
#include  "fof_mt.h"
#include  "fof_pf2.h"
#include  "fof_util.h"
#include  "fof_slk.h"

#define  e_PFI_Name  "Post Fire Injury"

#define eC_PFI 300
d_PFI  sr_PFI[eC_PFI];

#define  eC_Puf  5000
char cr_Puf[eC_Puf];

int   gi_TotOrgPFI = 0;
float gf_AvgDBHPFI = 0;
int   gi_TotKilPFI = 0;

#define  e_RepSep "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"


/******************************************************************
* Name: PFI_SummarySalvage
* Desc: Totals for Salvage Report 
*       Remeber each tree entry on the Salvage report
*        represent just one tree
*******************************************************************/
void Wnd_Mai::PFI_SummarySalvage()
{
int i,j,k;
char cr[500]; 

d_MT s_MT;

  j = eC_TotNam - 5;                          /* see Note-1                  */
  k = j / 2;
  _rep ("\n\n");
  sprintf  (cr, "%*c            Mortality Salvage Summary\n\n",j,' ');
  _rep (cr); 
//  sprintf (cr,"%*cName     Percentages of . . .    \n",j,' ');
//  _rep (cr); 
  sprintf (cr, "%*c Id     %c Mortality    %c Killed    %c Basal Killed\n",j,' ','%','%','%');
  _rep (cr); 

  for ( i = 0; i < eC_Totals; i++ ) {
    MT_Get (i, &s_MT, e_RTsal);
    if ( !strcmp (s_MT.cr_TotNam,"") )
       break;

    sprintf (cr, "%*s      %4.2f          %4.2f           %4.2f\n",
       eC_TotNam, s_MT.cr_TotNam,
       s_MT.f_SalMor,
       s_MT.f_SalKil, 
       s_MT.f_SalBA);
    _rep (cr);  

  }  /* for i */ 
 
}

