/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_fwf     Fuel Window Functions
* Desc: Function that fill in the Fuel TPA, Duff, and Moisture Windows
*       Adjustment Loading Tables are also in here.
* Date: 6/06/99
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
#include <windows.h>
#include <windowsx.h>
#include <winuser.h>
#include <winbase.h>
#include <commctrl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "fof_ci.h"

#include "win_ccwf.h"
#include "win_ccw.h"
#include "fof_ufl.h"
#include "fof_cvt.h"
#include "fof_fwf2.h"
#include "fof_fwf.h"
#include "fof_gen.h"
#include "fof_util.h"
#include "fof_gcs.h"

#include  "bur_brn.h"

// #include  "burnup.h"

#define printf xprintf
void far xprintf (char *Format, ...);

extern  HWND gh_MaiWnd;

extern  d_HD sr_HD[];
extern  d_DA sr_DA[];
extern  d_LA sr_LA [];

extern float fr_N_DufMoi[];
extern float fr_A_DufMoi[];
extern float fr_DufMoi  [];
extern float fr_DW10    [];
extern float fr_DW1000  [];



/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: FWF_Mngr
* Desc: Put all the Fuel Values to the screen Windows
*       Gets the appropriate CFL (Fuel Loading Record) based on Region
*       Fuel Model, then put the values to their repective Wnds.
*       Adjusts to Moist and TPA are made according to settings such as
*       Sparse, Typical, etc.....
*  Ret: 1 OK, 0 Error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int   FWF_Mngr ()
{
int i;
d_CFL  s_CFL;
d_UFL  s_UFL;
   i = CVT_GetActRec (&s_CFL);          /* get the appropriate Fuel Load Rec */
   if ( i == 0 ) {                      /* don't have one                    */
     FWF_Zero_TPA ();                   /* Put 0s in TPA Amt windows         */
     return 0; }
   UFL_Get(&s_UFL);                     /* Get any User entered TPA Amouts   */
   FWF_SetAllWnd(&s_CFL,&s_UFL);        /* Now calc & set Fuel Loads to Scr  */

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* When FCCS Cover is selected we need to put the FCCS's percent rot from    */
/*  input .dat file.                                                         */
/* Also need put the 'fixed' in the Distribution Dropdown box                */
/* Alternately we need to do the opposite when SAF/NVCS is selected          */
  if ( CCW_isFCC()){
    CCW_SetWndTxtfl (s_CFL.f_pcRot, e_CCW_Rot);  /* FCCS percent rotten      */
    CCW_DistFCC (1); }

  else if ( CCW_isFLM()){
    CCW_SetWndTxtfl (s_CFL.f_pcRot, e_CCW_Rot);  /* FLM  percent rotten      */
    CCW_DistSelect (s_CFL.cr_FLMLogDist);        /* Set Log Dist             */
    CCW_SoilTypeSelect (s_CFL.cr_FLMSoil);       /* Soil Type dropdown       */
/* Change 6-28-10 - Updated FLM changes */
    CCW_SetSeason(e_Sea_Summer);
    CCW_SetMoisture (e_Moi_Dry);  }


  else {                                         /* Not FCCS                 */
    CCW_SetWndTxt (e_CCW_Rot,e_DefRotWood);      /* Default Rotten percent   */
    CCW_DistFCC (0); }

   return 1;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: FWF_SetAllWnd
* Desc: Put, TPA, Duff Depth, Wood Diameter, and Duff and Wood Moistures
*       to Wnds
*  Ret: 1 OK
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int  FWF_SetAllWnd(d_CFL *a_CFL, d_UFL *a_UFL)
{

   FWF_AdjDead(e_CCW_TPALit,e_CCW_TPALic,a_CFL->f_litter,a_UFL->f_usrLitter, a_CFL->cr_HvCod);
   FWF_AdjDead (e_CCW_TPAW0, e_CCW_TPA0c, a_CFL->f_DW0,   a_UFL->f_usrDW0,    a_CFL->cr_HvCod);
   FWF_AdjDead (e_CCW_TPAW01,e_CCW_TPA01c,a_CFL->f_DW10,  a_UFL->f_usrDW10,   a_CFL->cr_HvCod);
   FWF_AdjDead (e_CCW_TPAW13,e_CCW_TPA13c,a_CFL->f_DW100, a_UFL->f_usrDW100,  a_CFL->cr_HvCod);
   FWF_AdjDead (e_CCW_TPAW3, e_CCW_TPA3c, a_CFL->f_DW1000,a_UFL->f_usrDW1000, a_CFL->cr_HvCod);

   FWF_AdjDuff (a_CFL->f_Duff, a_UFL->f_usrDuff, a_CFL->f_DufDep, "SetDepth");     /* Adjust Duff                 */

/* Set this from Fuel File Rec according Cmd Fil Sw args Typical Spars Abun  */
   FWF_AdjLive (e_CCW_TPAHer, e_CCW_TPAHerc, a_CFL->f_Herb,  a_CFL->cr_Herb,  a_UFL->f_usrHerb);
   FWF_AdjLive (e_CCW_TPAShr, e_CCW_TPAShrc, a_CFL->f_Shrub, a_CFL->cr_Shrub, a_UFL->f_usrShrub);
   FWF_AdjLive (e_CCW_TPAFol, e_CCW_TPAFolc, a_CFL->f_CroFol,a_CFL->cr_CroFol,a_UFL->f_usrCroFol);
   FWF_AdjLive (e_CCW_TPASmB, e_CCW_TPASmBc, a_CFL->f_CroBra,a_CFL->cr_CroBra,a_UFL->f_usrCroBra);
// Remove-MeaDia
//   CCW_SetWndTxtfl (a_CFL->f_MeaDia, e_CCW_WooDia);
   FWF_MoistSet ();                       /* Set Moisture Values          */
   return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: FWF_MoistSet
* Desc: Set Duff, DW10 and DW1000 Moisture Windows
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
void FWF_MoistSet ()
{
int  i;
char c;
float  f_MoistDuff;
  c = CCW_MoistCond ();                      /* Get the Moisture Condtion    */
  if ( c == 'V' )       /* Very Dry                           */
    i = 0;
  if ( c == 'D' )       /* Dry                                */
    i = 1;
  if ( c == 'M' )       /* Moderate                           */
    i = 2;
  if ( c == 'W' )       /* Wet                                */
    i = 3;

  if ( CCW_isDufNFDR() )                     /* Duff Code: NFDR              */
    f_MoistDuff = fr_N_DufMoi[i];
  else if ( CCW_isDufAdjNFDR() )             /* Duff Code: Adj-NFDR          */
    f_MoistDuff   = fr_A_DufMoi[i];
  else                                       /* Duff Code: Entire or Lower   */
    f_MoistDuff   = fr_DufMoi[i];

//  CCW_SetWndTxtfl (f_MoistDuff, e_CCW_DufMoi);
  CCW_SetWndTxtInt (f_MoistDuff, e_CCW_DufMoi);

// CCW_SetWndTxtfl (fr_DW10[i],   e_CCW_WooMoi01);   /* Woody 10 hr           */
  CCW_SetWndTxtInt (fr_DW10[i],   e_CCW_WooMoi01);   /* Woody 10 hr           */

//  CCW_SetWndTxtfl (fr_DW1000[i], e_CCW_WooMoi3); /* Woody 1000 hr         */
  CCW_SetWndTxtInt (fr_DW1000[i], e_CCW_WooMoi3); /* Woody 1000 hr         */
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: FWF_Zero_TPA
* Desc: put a 0 in all Tons Per Acre amount windows
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
void FWF_Zero_TPA()
{
  CCW_ZerWnd (e_CCW_TPALit);
  CCW_ZerWnd (e_CCW_TPAW0);
  CCW_ZerWnd (e_CCW_TPAW01);
  CCW_ZerWnd (e_CCW_TPAW13);
  CCW_ZerWnd (e_CCW_TPAW3);
  CCW_ZerWnd (e_CCW_TPADuf);
  CCW_ZerWnd (e_CCW_DufDep);
  CCW_ZerWnd (e_CCW_TPAHer);
  CCW_ZerWnd (e_CCW_TPAShr);
  CCW_ZerWnd (e_CCW_TPAFol);
  CCW_ZerWnd (e_CCW_TPASmB);
  CCW_ZerWnd (e_CCW_WooDia);
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: FWF_SetLiveTPA
* Desc: Set a Live (Herb, Shuurb, Crown Foliage, Crown Branch)
*        Tons Per Acre amount window for the specified Id
*        sent in. Remember Tons Per Acre amounts get set according to
*         their combo box selection, Typical, Sparse, Abundant, or User
*   In: i_Id......Combo Box Id, id as used in CCW
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
void  FWF_SetLiveTPA  (int i_Id)
{
d_UFL  s_UFL;
d_CFL  s_CFL;

    UFL_Get(&s_UFL);
    CVT_GetActRec(&s_CFL);
    switch ( i_Id ) {
       case e_CCW_TPAHerc:
          FWF_AdjLive (e_CCW_TPAHer, i_Id, s_CFL.f_Herb, s_CFL.cr_Herb, s_UFL.f_usrHerb);
          break;
       case e_CCW_TPAShrc:
          FWF_AdjLive (e_CCW_TPAShr, i_Id, s_CFL.f_Shrub,s_CFL.cr_Shrub,s_UFL.f_usrShrub);
          break;
        case e_CCW_TPAFolc:
          FWF_AdjLive (e_CCW_TPAFol, i_Id, s_CFL.f_CroFol,s_CFL.cr_CroFol,s_UFL.f_usrCroFol);
          break;
        case e_CCW_TPASmBc:
          FWF_AdjLive (e_CCW_TPASmB, i_Id, s_CFL.f_CroBra,s_CFL.cr_CroBra,s_UFL.f_usrCroBra);
          break;
        default:
          Error_Window ("Logic Error in Case Statement", "FWF_SetLiveTPA");
          break;
    }
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: FWF_SetDeadTPA
* Desc: Set a Dead Fuel (Litter, Wood 0-1, 1-3, and 3+)
*        Tons Per Acre amount window for the specified Id
*        sent in. Remember Tons Per Acre amounts get set according to
*         their combo box selection, Typical, Ligh, Heavy, or User
*   In: i_Id......Selection Combo Box Id, Id from the CCW table
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
void  FWF_SetDeadTPA  (int i_Id)
{
d_UFL  s_UFL;
d_CFL  s_CFL;

    UFL_Get(&s_UFL);
    CVT_GetActRec(&s_CFL);
    switch ( i_Id ) {
        case e_CCW_TPALic:
          FWF_AdjDead (e_CCW_TPALit,i_Id,s_CFL.f_litter,s_UFL.f_usrLitter, s_CFL.cr_HvCod);
          break;
        case e_CCW_TPA0c:
          FWF_AdjDead (e_CCW_TPAW0,i_Id,s_CFL.f_DW0,  s_UFL.f_usrDW0, s_CFL.cr_HvCod);
          break;
        case e_CCW_TPA01c:
          FWF_AdjDead (e_CCW_TPAW01,i_Id,s_CFL.f_DW10,  s_UFL.f_usrDW10, s_CFL.cr_HvCod);
          break;
        case e_CCW_TPA13c:
          FWF_AdjDead (e_CCW_TPAW13,i_Id,s_CFL.f_DW100, s_UFL.f_usrDW100, s_CFL.cr_HvCod);
          break;
        case e_CCW_TPA3c:
          FWF_AdjDead (e_CCW_TPAW3, i_Id,s_CFL.f_DW1000,s_UFL.f_usrDW1000, s_CFL.cr_HvCod);
          break;
        default:
          Error_Window ("Logic Error in Case Statement", "FWF_SetDeadTPA");
          break; }
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: FWF_UserTPA
* Desc: See if the user changes the Tons Per Acre amount in one of
*        the Edit Windows,
*       Remember the focus of the cursor (mouse) will stay in the
*       Edit Window between the EN_SETFOCUS & EN_KILLFOCUS
*   In: i_IdAmt.....id of Tons Per Acre amount window
*       i_HiWord....parameter that we get in the Windows Callback function
*  Ret: 1 OK,    0 Error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
static int iS_Chgd;
static float gf_Amt;
int   FWF_UserTPA (int i_IdAmt, int i_HiWord)
{
int i_IdComBox;
char cr[300];
HWND hCB;

        if ( i_IdAmt == e_CCW_TPALit ) i_IdComBox = e_CCW_TPALic;
   else if ( i_IdAmt == e_CCW_TPAW0 )  i_IdComBox = e_CCW_TPA0c;
   else if ( i_IdAmt == e_CCW_TPAW01 ) i_IdComBox = e_CCW_TPA01c;
   else if ( i_IdAmt == e_CCW_TPAW13 ) i_IdComBox = e_CCW_TPA13c;
   else if ( i_IdAmt == e_CCW_TPAW3  ) i_IdComBox = e_CCW_TPA3c;
   else if ( i_IdAmt == e_CCW_TPADuf ) i_IdComBox = e_CCW_TPADufc;
   else if ( i_IdAmt == e_CCW_TPAHer ) i_IdComBox = e_CCW_TPAHerc;
   else if ( i_IdAmt == e_CCW_TPAShr ) i_IdComBox = e_CCW_TPAShrc;
   else if ( i_IdAmt == e_CCW_TPAFol ) i_IdComBox = e_CCW_TPAFolc;
   else if ( i_IdAmt == e_CCW_TPASmB ) i_IdComBox = e_CCW_TPASmBc;
   else {
     Error_Window ("Logic Error", "FWF_UserTPA");
     return 0; }

   if (i_HiWord ==  EN_SETFOCUS){                   /* when cursor enters wnd*/
     iS_Chgd = 0;                                   /* switch detects usr chg*/
     return 1; }

   if (i_HiWord == EN_CHANGE) {                     /* user has changed      */
     iS_Chgd++;                                     /*  something            */
     if (!CCW_GetWndFloat (i_IdAmt,&gf_Amt)) {       /* chk it 1 char at a tim*/
        strcpy (cr,"Invalid Numeric Value\n");
        strcat (cr," NOTE: To enter a decimal fractions use a leading 0\n");
        strcat (cr,"       for example:\n");
        strcat (cr,"       correct 0.2    incorrect .2 \n");
        Warning_Window (cr, "ChkSet_User");}
     return 1; }

   if (i_HiWord == EN_KILLFOCUS){                   /* cursor leaves wnd     */
     if ( iS_Chgd > 0 ) {                           /* did usr chng anything */
       UFL_SetUser (i_IdAmt,gf_Amt);                /* Store usr entrd valu  */
       hCB = CCW_GethWnd (i_IdComBox);
       SendMessage (hCB,CB_SETCURSEL,eX_User,0);}} /* Set combobox to 'User'*/
  return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: FWF_AdjDuff
* Desc: Put Duff Depth and Duff TPA to their Wnds, these values
*       get adjusted as needed and then put to Wnd.
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
void FWF_AdjDuff (float f_DuffTPA, float f_UsrDuff, float f_DuffDepth, char cr_Mode[])
{
float  f_Duff, f_Dep;
char c, cr1[300], cr2[300], cr_CovTyp[300];

   if ( CCW_isFCC () || CCW_isFLM() ) {  /* FCC, don't need do any  */
     CCW_SetWndTxtfl (f_DuffTPA,  e_CCW_TPADuf);  /* adjusting just but to   */
     CCW_SetWndTxtfl (f_DuffDepth,e_CCW_DufDep);  /*  Wnds                   */
     goto Y; }

  f_Duff = f_DuffTPA;
  f_Dep = f_DuffDepth;

  c = CCW_MoistCond ();                    /* Moisture Condition           */

/*...........................................................................*/
/* Pocosin Cover Type, see Manual Appendix B, Equation 20 definitions        */
  if ( CVT_isPocosin() ) {                   /*  Pond Pine - Pocosin           */
    if ( c == 'W' ) {                      /* Wet                            */
      f_Duff = 11.0;
      f_Dep = 1.0; }
    if ( c == 'M' ) {                      /* Moderate                       */
      f_Duff = 55.0;
      f_Dep = 5.0; }
    if ( c == 'D' ) {                      /* Dry,                           */
      f_Duff = 220.0;
      f_Dep = 14.0; }
    if ( c == 'V' ) {                      /* VeryDry                        */
      f_Duff = 375.0;
      f_Dep = 25.0; } }

  if ( CCW_FueAjsUser(e_CCW_TPADufc) ) {   /* If set to user enterd          */
    f_Duff = f_UsrDuff;
    goto X; }

/*...........................................................................*/
  if ( CCW_FueAjsLig(e_CCW_TPADufc) )  /*  Adst Light  */
    f_Duff = f_Duff * e_AdjDufLit;
  if ( CCW_FueAjsHea(e_CCW_TPADufc) )  /*  Adst Heavy  */
    f_Duff = f_Duff * e_AdjDufHvy;

  if ( f_Duff == 0 && f_Dep > 0 ) {           /* No Duff Tons per Acre       */
    f_Duff = f_Dep * 10;                      /*  so calc an amount          */
    strcpy (cr1,"No Duff Amount Available, program will calculate and set to:\n");
    sprintf (cr2,"  Duff Depth: %6.2f * 10 = %6.2f Duff Amt.\n", f_Dep, f_Duff);
    strcat (cr1,cr2);

    CCW_CovTyp (cr_CovTyp);                         /* Get Cov Typ from Wnd    */
    strcat(cr1,"\n");
    strcat(cr1,cr_CovTyp);
    Warning_Window (cr1,"FWF_AdjDuff");  }

X:
   CCW_SetWndTxtfl (f_Duff, e_CCW_TPADuf);
   if ( !stricmp (cr_Mode,"SetDepth") )
      CCW_SetWndTxtfl (f_Dep, e_CCW_DufDep);
   if ( CVT_isPocosin() )
      CCW_SetWndTxtfl (f_Dep, e_CCW_DufDep);
Y:;

}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: FWF_DufDep
* Desc: Handles the user changing the Duf Depth amount.
*       When the user enters a new duf depth then we calculate a new
*       duff amount of tons per acre and ask the user if they want
*       to have the program put it into the Duff tons per acre window
*       and set the Duff Combo Box to 'User'
*  Ret: 1 OK,    0 Error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
static float gf_DD;
static float gf_DDOrg;
int   FWF_DufDep (int i_HiWord)
{
int i;
float f_Ratio, f_Amt;
HWND hCB;
char cr[1500];

/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
   if (i_HiWord ==  EN_SETFOCUS){                /* when cursor enters wnd   */
     CCW_GetWndFloat (e_CCW_DufDep, &gf_DDOrg);  /* sav orgnal duf dep       */
     return 1; }

/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
   if (i_HiWord == EN_KILLFOCUS) {               /* Focus leaves Wnd         */
     if (!CCW_GetWndFloat (e_CCW_DufDep,&gf_DD)) {
        Warning_Window ("Invalid Entry in Duff Depth", "FWF_DufDep");
        return 0; }
     if ( gf_DDOrg == gf_DD )                    /* Has value changed        */
        return 0;
     f_Ratio = CVT_DDRatio ();                   /* Duf to Depth ratio       */
     if ( f_Ratio == 0 ) {                       /* if can't get a ratio     */
       f_Ratio = e_DufDefRat;                    /* use the default ratio    */
    //   _Warn1 ();  /* use to just do this */
    //   return 1;   /*  warning message    */
     }

     f_Amt =  f_Ratio * gf_DD;                   /* ratio times new duf dep  */
     if ( f_Amt > KgSq_To_TPA (e_wdf2) )         /* don't go past the burnup */
       f_Amt = KgSq_To_TPA (e_wdf2);             /* upper limit on duff amt  */
     _MakMes (cr,f_Amt);                         /* form the message for box */
     i = MessageBox (gh_MaiWnd,cr,"",MB_YESNO);  /* Ask usr before changin   */
     if ( i == IDYES ) {
       CCW_SetWndTxtfl (f_Amt, e_CCW_TPADuf);    /* New amt to Duf TPA Wnd   */
       UFL_SetUser (e_CCW_TPADuf,gf_Amt);        /* Store as usr entrd amt   */
       hCB = CCW_GethWnd (e_CCW_TPADufc);        /* Duf Amt Combo Box        */
       SendMessage (hCB,CB_SETCURSEL,eX_User,0);}/* now set it to 'User'     */
     CCW_SetWndTxtfl (gf_DD, e_CCW_DufDep);      /* Duf Dep to Wnd, formated */
     return 1; }
   return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _MakMes
* Desc: Make the message to ask user, just put it here to keep code cleaner
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
void   _MakMes (char cr[], float f_Amt)
{
char cr_tmp[200];
  strcpy  (cr, "                                       Attention  User !\n\n");
  strcat  (cr, "The Duff depth you set will get reset back to its' original value if you:\n");
  strcat  (cr, " - change the Region\n");
  strcat  (cr, " - change the Cover Type\n");
  strcat  (cr, " - change the Fuel Category\n");
  strcat  (cr, " - change Moist Condition while using 'Pond Pine - Pocosin' cover type\n");
  strcat  (cr, " - change Rough Age while using Southeast region\n\n");
  sprintf (cr_tmp, "Now, do you also want to adjust the 'Duff Tons Per Acre' amount to %6.2f ?\n",f_Amt);
  strcat  (cr, cr_tmp);
  strcat  (cr, "This amount is calculated based on the new 'Duff Depth' you enter.\n");
  strcat  (cr, "And will be set and treated as a 'User' entered amount.\n\n");
  strcat  (cr, "\n\n                                 Set New Duff Tons Per Acre Amount ? \n");
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _Warn1
* Desc: A warning message for user, just put it here to make code cleaner.
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
void _Warn1 ()
{
char cr[600];
  strcpy (cr, "Cannot calculate and set a new duff tons per acre amount.\n");
  strcat (cr, "Missing duff amount or duff depth in fuel loading file");
  Warning_Window (cr, "FWF_DufDep");
}
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: FWF_AdjDead
* Desc: Set the a Dead Fuel Tons Per Acre into is proper Window
*       This works on one at a time, Litter, DW0-1 DW1-3, or DW3+
*       See code below for details
* Note-1: value only gets changed when Light or Heavy
* Note-2: values can get adjust to less than 0 for Pile/Slash (Non-Natural)
*          for 'Light', it doesn't look like the original .F77 caught this
*          but thought I should put it in.
* Note-3: Harvesting Debris, Code comes from Fuel .dat file, program
*         adjust loadings for Lit,Duf,Wood on Slash-Piles, See Manual
*         Harvesting Debris Group table 7 page 63
*   In: i_IdAmt......CCW Id for the Tons Per Acre 'amount' window
*       i_IdComBox...CCW Id for the ComboBox
*       f_load.......fuel loading amount from the Fuel Loading Record
*       cr_HvCod.....Haversting Debris Group Code
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
void FWF_AdjDead (int i_IdAmt, int i_IdComBox, float f_load, float f_User,
                  char cr_HvCod[])
{
float f, f_adj;

   if ( CCW_isFCC () || CCW_isFLM()) {       /* FCC, don't do any adjustmnts */
     f = f_load;                             /* just go put it to Wnd        */
     goto X; }

  if ( CCW_FueAjsUser (i_IdComBox) ) {       /* is the Adjustmnt User        */
     f = f_User;                             /* Wnd has User enter Load Amt  */
     goto X; }                               /* so don't adjust it           */

  f = f_load;                                /* Note-1                       */
/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
/* Natural - Use Dead Fuel Adjustment                                        */
  if ( GCS_isNatural() ) {                   /* is Category Natural          */
    if ( CCW_FueAjsLig (i_IdComBox) ) {      /* is the Adjustmnt Light       */
       f_adj = DA_AdjLight(i_IdAmt);
       f = f * f_adj; }                      /* adjust it                    */
    if ( CCW_FueAjsHea(i_IdComBox) ) {       /* is the Adjustmnt Heavy       */
       f_adj = DA_AdjHeavy(i_IdAmt);
       f = f * f_adj; }                       /* adjust it                    */
    goto X; }

/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
/* Piles & Slash -> Use the Harvest Adjustment                               */
  if ( CCW_FueAjsLig (i_IdComBox))           /* Light                        */
    f += FWF_HDAdj (i_IdAmt,cr_HvCod,'L');   /* Add Harvest Debris Loading   */
  else if ( CCW_FueAjsHea (i_IdComBox))      /* Heavy                        */
    f += FWF_HDAdj (i_IdAmt,cr_HvCod,'H');   /* Add Harvest Debris Loading   */
  else                                       /* Medium                       */
    f += FWF_HDAdj (i_IdAmt,cr_HvCod,'M');   /* Add Harvest Debris Loading   */
X:
  if ( f < 0 )                               /* See Note-2 above             */
    f = 0;
 CCW_SetWndTxtfl (f, i_IdAmt);
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: FWF_AdjLive
* Desc: Adjust a Live Loading, (Herb, Shrub, Crown Foliage & Branch)
*       base on user window setting of TPA Combo Box, (Typical,Sparse,Abundant)
*       put the adjust loading to its TPA Amount window.
*   In: i_IdAmt...CCW code of the TPA amount window
*     i_IdCom.....CCW code of the TPA Combo Box, (Typical,Sparse,Abundant)
*     f_Load......Loading amount
*     cr_Code.....Adjustment code, code coming from from Fuel File
*     f_User......User entered loading if any
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
void FWF_AdjLive (int i_IdAmt, int i_IdCom, float f_Load, char cr_Code[], float f_User)
{
float f,f_Adj;
char c, cr1[300], cr2[300];


   if ( CCW_isFCC() || CCW_isFLM()) {     /* FCC, don't do any adjustmnts */
     f = f_Load;                          /* just go put it to Wnd        */
     goto Done; }

  c = CCW_TypSpaAbu (i_IdCom);           /* What is Wnd Box set to            */
  if ( c == 'U' ) {                      /* User                              */
    f = f_User;
    goto Done; }
  if ( c == 'T' ) {                      /* Typical so no need to adjust     */
    f = f_Load;
    goto Done; }

  f_Adj = FWF_LAadj (i_IdAmt, cr_Code, c);  /* Get Adjustment factor, spar-abun */
  if ( f_Adj <= 0 ) {
    strcpy (cr1,"Logic Error  - don't know  Spare-Abundant adjust factor\n");
    sprintf (cr2, "%s - %c ", cr_Code, c);
    strcat (cr1,cr2);
    Error_Window (cr1,"FWF_AdjLive");
    f = f_Load;
    goto Done; }

  f = f_Load * f_Adj;

Done:
   CCW_SetWndTxtfl (f, i_IdAmt);
}
