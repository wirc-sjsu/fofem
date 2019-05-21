/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fir_fwf.h   Fuel Window Functions
* Desc: Function that fill in the Fuel TPA, Duff, and Moisture Windows
* Date: 6/06/99
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/




int   FWF_SetAllWnd  (d_CFL *a_CFL, d_UFL *a_UFL);
int   FWF_SetAllWnd  (d_CFL *a_CFL, d_UFL *a_UFL);


int   FWF_Mngr       (void);
void  FWF_MoistSet   (void);
void  FWF_AdjDuff    (float f_DuffTPA, float f_UsrDuff, float f_DuffDepth, char cr_Mode[]);
void  FWF_Zero_TPA   (void);
void  FWF_SetLiveTPA (int i_Id);
void  FWF_SetDeadTPA (int i_Id);
int   FWF_UserTPA    (int i_IdAmt, int i_HiWord);
int   FWF_DufDep     (int i_HiWord);
void  FWF_AdjLive    (int i_IdAmt, int i_IdCom, float f_Load, char cr_Code[], float f_User);
void  FWF_AdjDead    (int i_IdAmt, int i_IdComBox, float f_load, float f_User, char cr_HvCod[]);

void  _Warn1 (void);
void  _MakMes (char cr[], float f_Amt);
