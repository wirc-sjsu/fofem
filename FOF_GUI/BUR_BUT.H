/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: bur_but.h
* Desc: Some burnup misc/utility kind of functions
*
* Date: 3/17/05
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/



void  But_RemoveTmp (void);

int But_RunBrnFil (char cr_FN[], char cr_Err[]);
int But_RBIF (char cr_FN[], char cr_AmtFN[], char cr_EmisFN[], char cr_Err[]);

int  But_CreInpFil (d_CI *a_CI, char cr_FN[], char cr_ErrMes[]);
