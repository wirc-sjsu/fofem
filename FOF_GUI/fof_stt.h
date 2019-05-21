/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Soi_STT    Soil Totals Table
* Desc:
* Date: 12/31/99
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
#pragma once 

typedef struct {
    char cr_Name[eC_TotNam];
    int  i_Lay1;
    int  i_Lay2;
 } d_STT;

void  STT_Init (void);
int   STT_Save (char cr_Name[], int i_Lay1, int i_Lay2);
int   STT_Get  (int iX, char cr_Name[], int *ai_Lay1, int *ai_Lay2);
int   STT_Count (void);
