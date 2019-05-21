/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_mt.h
* Desc: Mortality Stand Report Totals
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/

#pragma once

/* Three different mortality type reports will use the same struture */
/*  to make life easier */ 

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
typedef struct {
  char cr_TotNam[eC_TotNam];                 /* Total id-name                */
  int  i_PerMor;                             /* percent Mortality            */
  int  i_PerConBA;                           /* percent consumed basal area  */
  float f_LenHei;                            /* Flame Len - Scorch Height    */
  char  cr_LenHei[15];                       /* tells if len or hei          */
  int  i_PreLivCC;                           /* Prefire Live Canopy Cover %  */
  int  i_PosLivCC;                           /* Postfire Live Canopy Cover %  */
 
/* Salvage Report Totals */
  float f_SalMor;
  float f_SalKil; 
  float f_SalBA;                            /* Postfire Salvage Rep - killed */

} d_MT;


void  MT_Init (int i_RT);
int   MT_Save (d_MT *a_MT, int i_RT);
int   MT_Get  (int iX, d_MT *a_MT, int i_RT);
int   MS_isTotNam (char cr_TotNam[], int i_RT);
