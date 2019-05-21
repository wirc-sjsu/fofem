/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_mt.c
* Desc: Mortality Stand Report Total Module
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
#include <windows.h>
#include <windowsx.h>
#include <winuser.h>
#include <winbase.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "wnd_def.h" 

#include  "fof_util.h"
#include  "fof_st.h"
#include  "fof_mt.h"


d_MT *Get_Adr (int i_RT) ;

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/*                       Mortality Totals array                              */
d_MT sr_MT[eC_Totals];
d_MT sr_Pos[eC_Totals];
d_MT sr_Sal[eC_Totals];

/************************************************
*
*
************************************************/
d_MT *Get_Adr (int i_RT) 
{
d_MT *a;
   if ( i_RT == e_RTpre ) 
     return  &sr_MT[0]; 
   else if ( i_RT == e_RTpos ) 
     return  &sr_Pos[0]; 
   return &sr_Sal[0]; 
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: MT_Init
* Desc: Init the Mortality Totals table
*   In: i_RT... report type code
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void   MT_Init (int i_RT)
{
int i;
d_MT  *a; 

/* Get the address of the table to init, based on RT Report Type code */
   a = Get_Adr (i_RT);  

   for ( i = 0; i < eC_Totals; i++ ) {
     strcpy (a->cr_TotNam,"");
     a->i_PerMor = -1;
     a->i_PerConBA = -1;
     a->f_LenHei = -1;
     a->i_PreLivCC = -1;               /* Prefire Live Canopy Cover %  */
     a->i_PosLivCC = -1;               /* Postfire Live Canopy Cover %  */

     a->f_SalMor = -1; 
     a->f_SalKil = -1; 
     a->f_SalBA = -1;

     strcpy (a->cr_LenHei,""); 

     a++; 
  } /* for i */

 //    strcpy (sr_MT[i].cr_TotNam,"");
 //    sr_MT[i].i_PerMor = -1;
 //    sr_MT[i].i_PerConBA = -1;
 //    sr_MT[i].f_LenHei = -1;
 //    sr_MT[i].i_PreLivCC = -1;               /* Prefire Live Canopy Cover %  */
 //    sr_MT[i].i_PosLivCC = -1;               /* Postfire Live Canopy Cover %  */

//     sr_MT[i].f_SalMor = -1; 
//     sr_MT[i].f_SalKil = -1; 
//     sr_MT[i].f_SalBA = -1;

//     strcpy (sr_MT[i].cr_LenHei,""); 
//   }  /* for i */ 

}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: MT_Save
* Desc: Save to Mortality Totals table
*  Ret: 1 OK, 0 error see below
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int   MT_Save (d_MT *a_MT, int i_RT)
{
int i,j;
  j = eC_Totals - 1;
d_MT *a;

   a = Get_Adr (i_RT);  

  for ( i = 0; i < j; i++ ) {
    if ( strcmp (a->cr_TotNam,"" )) {
       a++; 
       continue; } 
   
    memcpy (a, a_MT, sizeof(d_MT));
    return 1; 

  } /* for i */ 

/*   Maximum number of Mortality Summaries Exceeded                          */
  return 0;                             /* Table is Full                     */
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: MT_Get
* Desc: Get total from table
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int   MT_Get (int iX, d_MT *a_MT, int i_RT)
{
int i; 
d_MT *a;

  strcpy (a_MT->cr_TotNam,"");
  if ( iX >= eC_Totals ) {
    return 0; }

   a = Get_Adr (i_RT);  

   for ( i = 0; i < eC_Totals; i++ ) { 
     if ( i == iX ) { 
       memcpy ( a_MT, a, sizeof(d_MT));
       return 1; }
     a++; 
   } /* for i */ 
  
   return 0; 

}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: MT_isTotNam
* Desc: See if the Total Name is in the Table.
*   In: cr_TotNam....name to look for
*       i_RT.....report type, see caller for the defines
*  Ret: 1 yes its in table, 0 No its not in table
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  MS_isTotNam (char cr_TotNam[], int i_RT)
{
int i;
d_MT *a;

  a = Get_Adr (i_RT);  

  for ( i = 0; i < eC_Totals; i++ ) {
    if ( !strcmp (a->cr_TotNam,"") )         /* Not Found              */
      return 0;
    if ( !strcmp (a->cr_TotNam,cr_TotNam) )  /* Found it               */
      return 1;
    a++; 
  }

// ---------------------------------------------
 // for ( i = 0; i < eC_Totals; i++ ) {
//    if ( !strcmp (sr_MT[i].cr_TotNam,"") )         /* Not Found              */
//      return 0;
//    if ( !strcmp (sr_MT[i].cr_TotNam,cr_TotNam) )  /* Found it               */
//      return 1;
//  }
/*  Shouldn't get here       */
  return 0;
}
