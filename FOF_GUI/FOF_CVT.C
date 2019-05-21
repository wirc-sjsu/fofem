/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_cvt.c
* Desc: Read in Cover/Fuel .dat file, put it in a table and functions
*        to deal with it.
* Date:
*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
#include <windows.h>
#include <windowsx.h>
#include <winuser.h>
#include <winbase.h>
#include <commctrl.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define printf xprintf
void far xprintf (char *Format, ...);

#include "win_mai.h"
#include "win_ccwf.h"
#include "win_ccw.h"
#include "fof_util.h"
#include "fof_ufl.h"
#include "fof_cvt.h"
#include "fof_fwf2.h"
#include "fof_fwf.h"


extern HWND gh_MaiWnd;
extern char  gcr_datDir[];              /* start dir path to .dat files      */


#define eC_CVT 3000
d_CVT sr_CVT[eC_CVT];                   /* All Cov Typs from disk file       */d_CVT sr_CVT[eC_CVT];                   /* All Cov Typs from disk file       */

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* Activity & Treatment Code,                                                */
/* These are the 'none's that get used in append FSS Cover Type Name         */
/* So we can tell difference, when looking for to determine 'Natural, Pile...*/
#define  e_ActCat_None  "(None)"
#define  e_TreCat_None  "(none)"


void Tab_Error (char cr_Line[], char cr_FuncName[], char cr_FN[]);
int _ChkFCCFueCat (char cr[]);


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_LoadFLM
* Desc: Read in the FLM Cover-Load File and save it in the CVT table
*   In: cr_Version....version number in head of Cover File
*       cr_FN.........Cover File Name
*  Ret: number of Cover Type records read in,   0 Error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int CVT_LoadFLM (char cr_Version[], char cr_FN[])
{
int i, iX;
FILE *fh ;
#define eC_line 300
char cr_Line [eC_line], *a, cr[200],  cr_CovTyp[200];
char cr_Pth[eC_PthFil+1];
#define  eO 20

    strcpy (cr_Pth,gcr_datDir);
    strcat (cr_Pth,cr_FN);

    iX = 0;
    CVT_InitTable();                         /* Init the table               */
    fh =  fopen (cr_Pth, "r" );
    if ( fh == NULL ) {
      sprintf (cr,"Can't Open Input Cover File: %s\n", cr_Pth);
      strcat  (cr,"File must be in same directory as fofem.exe");
      Error_Window (cr, "CVT_LoadSAF");
      return 0;}

    for ( i = 0; i < eC_CVT - 2; i++ ) {
      a = fgets (cr_Line, eC_line, fh );     /* Read a line from file        */
        if ( a == NULL ) {                   /* End of File                  */
           fclose (fh);
           CVT_FLM_Dummy(iX);                /* insrt dumy reg rec           */
           return iX; }                      /* # of records loaded          */
      if ( strchr (cr_Line,'\t') ) {
        Tab_Error (cr_Line,"CVT_LoadFLM",cr_FN);
        return 0; }

      Blk_End_Line (cr_Line,eC_line);              /* Blank to end of line   */
      if ( isBlank (cr_Line) )                     /* if line is empty then  */
        continue;                                  /*  skip it               */

      if ( i == 0 )                                /* First line of file     */
        if ( !CVT_ChkVer(cr_Line,cr_Version) )     /* Make sure Cov file has */
          return 0;                                /* correct version #      */

      if ( cr_Line[0] == '#' )                     /* lines is a comment so  */
        continue;                                  /*  skip it               */

      ToInt (&sr_CVT[iX].i_Num, cr_Line, 10, 12);    /* Cover Number as int    */

/* Regions, don't care about order just if Letter is sound in string         */
      ToStr (cr, cr_Line, 1, 4);
      if ( strstr (cr,"I") )  sr_CVT[iX].ir_Region[0] = 1; /* Inter West */
      if ( strstr (cr,"P") )  sr_CVT[iX].ir_Region[1] = 2; /* Pacific West */
      if ( strstr (cr,"N") )  sr_CVT[iX].ir_Region[2] = 3; /* Northeast    */
      if ( strstr (cr,"S") )  sr_CVT[iX].ir_Region[3] = 4; /* Southeast      */

/* test-larry */

      ToStr (sr_CVT[iX].cr_CovTyp, cr_Line, 6, 70);


      ToFlo (&sr_CVT[iX].s_CFL.f_litter, cr_Line,  72, 76);
      ToFlo (&sr_CVT[iX].s_CFL.f_Duff,   cr_Line,  80, 84);
      ToFlo (&sr_CVT[iX].s_CFL.f_DufDep, cr_Line,  87, 91);

      ToFlo (&sr_CVT[iX].s_CFL.f_DW0,    cr_Line,  93,  97);
      ToFlo (&sr_CVT[iX].s_CFL.f_DW10,   cr_Line,  99, 103);
      ToFlo (&sr_CVT[iX].s_CFL.f_DW100,  cr_Line, 105, 109);
      ToFlo (&sr_CVT[iX].s_CFL.f_DW1000, cr_Line, 111, 115);

      ToFlo (&sr_CVT[iX].s_CFL.f_Herb,  cr_Line,  117, 121);
      ToFlo (&sr_CVT[iX].s_CFL.f_Shrub, cr_Line,  123, 127);

      ToFlo (&sr_CVT[iX].s_CFL.f_CroFol, cr_Line, 129, 133);
      ToFlo (&sr_CVT[iX].s_CFL.f_CroBra, cr_Line, 135, 139);

      ToFlo (&sr_CVT[iX].s_CFL.f_pcRot, cr_Line, 141, 143);

/* Log Dist and Soil Type text must be same as Text used in drop down boxes  */
      ToStr (sr_CVT[iX].s_CFL.cr_FLMLogDist, cr_Line,  145, 150);
      Trim_LT (sr_CVT[iX].s_CFL.cr_FLMLogDist);

      ToStr (sr_CVT[iX].s_CFL.cr_FLMSoil,    cr_Line, 152, 165);
      Trim_LT (sr_CVT[iX].s_CFL.cr_FLMSoil);

      iX++;

    }   /* for i */

    fclose (fh);
    Error_Window ("ERROR - Serious - Internal Cover Type Array to Small","CVT_LoadSAF");
    return 0;
}
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_FLM_Dummy
* Desc: This Puts a single Dummy Record into the CVT table. With its region
*       array set with any region that is not found in the CVT.
*       This is to make sure that there is at least one record for each
*       region in the CVT, even if that record is a dummy record.
*   In: iX....CVT index where to put dummy record
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void CVT_FLM_Dummy(int iX)
{
   sr_CVT[iX].i_Num = 0;                     /* Cover Number as int    */
   strcpy (sr_CVT[iX].cr_CovTyp,"-> No Cover Records for Region <-");

   sr_CVT[iX].ir_Region[0] = -1;
   sr_CVT[iX].ir_Region[1] = -1;
   sr_CVT[iX].ir_Region[2] = -1;
   sr_CVT[iX].ir_Region[3] = -1;

   if ( !CVT_isReg(1) )
     sr_CVT[iX].ir_Region[0] = 1;
   if ( !CVT_isReg(2) )
     sr_CVT[iX].ir_Region[1] = 2;
   if ( !CVT_isReg(3) )
     sr_CVT[iX].ir_Region[2] = 3;
   if ( !CVT_isReg(4) )
     sr_CVT[iX].ir_Region[3] = 4;
}







/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_LoadFCC
* Desc: Read in the FCC File extract Cover info from it and load CVT
* CHANGE: Modified 8-8-08 to read in new version FCCS data
*
*   In: cr_Version....version number in head of Cover File
*       cr_FN.........File Name
*  Ret: 1 OK,  0 Error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int CVT_LoadFCC  (char cr_Version[], char cr_FN[])
{
int i, iX;
float s,r,f;
FILE *fh ;
#define eC_fLine 400
char cr_Line [eC_fLine], *a, cr[300];
char cr_Pth[eC_PthFil+1];

    strcpy (cr_Pth,gcr_datDir);
    strcat (cr_Pth,cr_FN);

    iX = 0;
    CVT_InitTable();                         /* Init the table               */
    fh =  fopen (cr_Pth, "r" );
    if ( fh == NULL ) {
      sprintf (cr,"Can't Open Input Cover File: %s\n", cr_Pth);
      strcat  (cr,"File must be in same directory as fofem.exe");
      Error_Window (cr, "CVT_Load");
      return 0;}

    for ( i = 0; i < eC_CVT - 2; i++ ) {
      a = fgets (cr_Line, eC_fLine, fh );         /* Read a line from file   */
        if ( a == NULL ) {                        /* End of File             */
           fclose (fh);
           CVT_FCC_Dummy(iX);                     /* chk & put dummy reg rec */
           return 1; }
      if ( strchr (cr_Line,'\t') ) {
        Tab_Error (cr_Line,"CVT_LoadFCC",cr_FN);
        return 0; }

      Blk_End_Line (cr_Line,eC_fLine);            /* Blank to end of line    */
      if ( isBlank (cr_Line) )                    /* if line is empty then   */
        continue;                                 /*  skip it                */

      if ( i == 0 )                               /* First line of file      */
        if ( !CVT_ChkVer(cr_Line,cr_Version) )    /* Make sure Cov file has  */
          return 0;                               /* correct version #      */

      if ( cr_Line[0] == '#' )                    /* lines is a comment so  */
        continue;                                 /*  skip it               */

      sr_CVT[iX].i_Num = 0;                  /* Cover Number as int    */


      ToStr (cr, cr_Line, 8, 99);            /* Name                         */
      Trim_LT (cr);
      strcpy (sr_CVT[iX].cr_CovTyp,cr);     /* put in table          */


      ToStr (cr, cr_Line, 2, 5);
      if ( strstr (cr,"I") )  sr_CVT[iX].ir_Region[0] = 1; /* Inter West */
      if ( strstr (cr,"P") )  sr_CVT[iX].ir_Region[1] = 2; /* Pacific West */
      if ( strstr (cr,"N") )  sr_CVT[iX].ir_Region[2] = 3; /* Northeast    */
      if ( strstr (cr,"S") )  sr_CVT[iX].ir_Region[3] = 4; /* Southeast      */

/* Fuel Cat - for now I'm setting FCC up to use the Fuel Cat Dropdown Box    */
//      ToStr (sr_CVT[iX].cr_FCCFuelCat, cr_Line, 100, 101);
//      Trim_LT (sr_CVT[iX].cr_FCCFuelCat);
//      if ( !_ChkFCCFueCat (sr_CVT[iX].cr_FCCFuelCat) ) {
//        fclose (fh);
//        return 0; }

      ToFlo (&sr_CVT[iX].s_CFL.f_litter,cr_Line, 103,108); /* Litter         */

      ToFlo (&sr_CVT[iX].s_CFL.f_Shrub, cr_Line, 124,129);/* Shrub               */
      strcpy (sr_CVT[iX].s_CFL.cr_Shrub,e_LA_NA);         /* Not Applicable code */

      ToFlo (&sr_CVT[iX].s_CFL.f_Herb,cr_Line, 131, 136);/* Herb                */
      strcpy (sr_CVT[iX].s_CFL.cr_Herb,e_LA_NA);         /* Not Applicable code */

      ToFlo (&sr_CVT[iX].s_CFL.f_DW0,   cr_Line,138,143); /* Dead Wood 1 hr      */

      ToFlo (&sr_CVT[iX].s_CFL.f_DW10,  cr_Line,145,150); /* Dead Wood 10 hr     */

      ToFlo (&sr_CVT[iX].s_CFL.f_DW100, cr_Line,152,157); /* Dead Wood 100 hr    */

/* Coarse wood is 3-9, 9-20, 20+, Sound and Rotten                           */
      ToFlo (&sr_CVT[iX].s_CFL.f_39s,  cr_Line, 159, 164);
      ToFlo (&sr_CVT[iX].s_CFL.f_920s, cr_Line, 166, 171);
      ToFlo (&sr_CVT[iX].s_CFL.f_20ps, cr_Line, 173, 178);

      ToFlo (&sr_CVT[iX].s_CFL.f_39r,  cr_Line, 180, 185);
      ToFlo (&sr_CVT[iX].s_CFL.f_920r, cr_Line, 187, 192);
      ToFlo (&sr_CVT[iX].s_CFL.f_20pr, cr_Line, 194, 199);

      s = sr_CVT[iX].s_CFL.f_39s + sr_CVT[iX].s_CFL.f_920s + sr_CVT[iX].s_CFL.f_20ps;
      r = sr_CVT[iX].s_CFL.f_39r + sr_CVT[iX].s_CFL.f_920r + sr_CVT[iX].s_CFL.f_20pr;
      f = s + r;
      sr_CVT[iX].s_CFL.f_DW1000 = f;              /* Total all coarse wood   */
      if ( f == 0 )
         sr_CVT[iX].s_CFL.f_pcRot = 0;
      else
         sr_CVT[iX].s_CFL.f_pcRot = (r / f) * 100.0;  /* percent that is rotten  */


      ToFlo (&sr_CVT[iX].s_CFL.f_Duff,  cr_Line, 110, 115); /* Duff Amt     */
      ToFlo (&sr_CVT[iX].s_CFL.f_DufDep,cr_Line, 117, 122); /* Duff Depth    */

      strcpy (sr_CVT[iX].s_CFL.cr_HvCod, e_HD_NA);       /* Harvst Debis Cod not applic */

      sr_CVT[iX].s_CFL.f_CroFol = 0;                     /* Have No Crn Foliage */
      strcpy (sr_CVT[iX].s_CFL.cr_CroFol,e_LA_NA);

      sr_CVT[iX].s_CFL.f_CroBra = 0;                     /* Have No Crn Branch  */
      strcpy (sr_CVT[iX].s_CFL.cr_CroBra,e_LA_NA);

      sr_CVT[iX].s_CFL.f_MeaDia = 5;                     /* Hav no Wd3+ mean    */

      strcpy (sr_CVT[iX].s_CFL.cr_Comment, "");

      ToStr (cr, cr_Line, 201, 203);              /* Cover Group             */
      Trim_LT (cr);
      strcpy (sr_CVT[iX].cr_CovGrp,cr);

      iX++;

    }   /* for i */
    fclose (fh);
    Error_Window ("ERROR - Serious - Internal Cover Type Array to Small","CVT_LoadFCC");
    return 0;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _ChkFCCFueCat
* Desc: Check the FCC Fuel Category that has been read in from the
*       fof_Fccs.dat file
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int _ChkFCCFueCat (char cr[])
{
char A[300];
  if ( !stricmp (cr,e_FCC_Natural) ) return 1;
  if ( !stricmp (cr,e_FCC_Piles) ) return 1;
  if ( !stricmp (cr,e_FCC_Slash) ) return 1;

  sprintf  (A,"Invalid FCC Fuel Category (%s) in .dat fuels file", cr);
  Error_Window (A, "CVT_Load");
  return 0;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_isPocosin
* Desc: See if the Cover Type that is currently set in the user interface
*        is a Pocosin (in a Pocosin cover group)
*  Ret: 1 = yes, 0 = no
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int CVT_isPocosin ()
{
char cr[30];
   CVT_GetCoverGroup (cr);
   if ( !strcmpi (cr,e_CVT_Pocosin) )
     return 1;
   return 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_FCC_FuelCat
* Desc: Get FCC Fuel Category from the Currently selected FCC cover type
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  CVT_FCC_FuelCat (char cr_FC[])
{
int i;
char cr_CovTyp[eC_WndTxt];

  strcpy (cr_FC,"");
  CCW_CovTyp (cr_CovTyp);                    /* Get Currnt Cov in Wnd        */

  for ( i = 0; i < eC_CVT; i++ ) {           /* Look thru table for it       */
    if ( !strcmp (sr_CVT[i].cr_CovTyp,"")){  /* shouldn't realy not find it  */
      break; }
    if ( !strcmp (sr_CVT[i].cr_CovTyp,cr_CovTyp) ){     /*  all done               */
      strcpy (cr_FC,sr_CVT[i].cr_FCCFuelCat);
      break; }
  }
}




/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_GetCoverGroup
* Desc: Get the Cover Group code for the cover type that is currently
*        set in the user interface.
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  CVT_GetCoverGroup (char cr_CovGrp[])
{
int i;
char cr_CovTyp[eC_WndTxt];

  strcpy (cr_CovGrp,"");
  CCW_CovTyp (cr_CovTyp);                    /* Get Currnt Cov in Wnd        */

  for ( i = 0; i < eC_CVT; i++ ) {           /* Look thru table for it       */
    if ( !strcmp (sr_CVT[i].cr_CovTyp,"")){  /* shouldn't realy not find it  */
      break; }
    if ( !strcmp (sr_CVT[i].cr_CovTyp,cr_CovTyp) ){     /*  all done               */
      strcpy (cr_CovGrp,sr_CVT[i].cr_CovGrp);
      break; }
  }
}



/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_FuelRef
* Desc: Get the Fuel and Additional Fuel References.
*       If there are none the strings will be empty (null)
*       These are fields in the cover/fuel records.
*  Ret:
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void CVT_FuelRef (char cr_FueRef[],  char cr_AddRef[])
{
int i;
char cr_CovTyp[e_CT+1];

  strcpy (cr_FueRef,"");
  strcpy (cr_AddRef,"");
  CCW_CovTyp (cr_CovTyp);                    /* Get Cover Type from Screen   */
  for ( i = 0; i < eC_CVT; i++ ) {
    if ( !strcmp ( sr_CVT[i].cr_CovTyp,"") ) {
      Error_Window ("Logic Error - Can't Find Cover Record in Table","CVT_FuelRef");
      break; }
    if ( !strcmp (sr_CVT[i].cr_CovTyp,cr_CovTyp)){
      strcpy (cr_FueRef,sr_CVT[i].cr_FueRef);      /* Fuel Reference               */
      strcpy (cr_AddRef,sr_CVT[i].cr_AddRef);      /* Additional Fuel Reference    */
      break;}
  }
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_isNaturalFCC
* Desc: When set to FCC Cover Types see if the currently set Cover Type
*       in the Cover Wnd is a 'Natural', we look for  "(None)", in cover
*       Name, (note the upper case 'N') this is Activity Code = none
*       meaning it's natural.
*  Ret: 1 its Natural,  else 0
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int   CVT_isNaturalFCC ()
{
char cr_FC[10];
   CVT_FCC_FuelCat (cr_FC);
   if ( !stricmp (cr_FC,e_FCC_Natural) )
     return 1;
   return 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_isPileFCC
* Desc: When set to FCC Cover Types see if the currently set Cover Type
*       in the Cover Wnd is a 'Pile',
*  Ret: 1 its Pile,  else 0
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int   CVT_isPileFCC ()
{
char cr_FC[10];
   CVT_FCC_FuelCat (cr_FC);
   if ( !stricmp (cr_FC,e_FCC_Piles) )
     return 1;
   return 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_isSlashFCC
* Desc: When set to FCC Cover Types see if the currently set Cover Type
*       is Slash
*  Ret: 1 its Slash,  else 0
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int   CVT_isSlashFCC ()
{
char cr_FC[10];
   CVT_FCC_FuelCat (cr_FC);
   if ( !stricmp (cr_FC,e_FCC_Slash) )
     return 1;
   return 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_CovTypNum
* Desc: Get the Cover Type Name from Window then look thru the CVT for its
*        match and return its number.
*  Ret: Cover Type Number
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  CVT_CovTypNum ()
{
int i;
char cr_CovTyp[eC_WndTxt];
  CCW_CovTyp (cr_CovTyp);                         /* Get Cov Typ from Wnd    */
  for ( i = 0; i < eC_CVT; i++ ) {                /* Look thru table for it  */
    if ( !strcmp (sr_CVT[i].cr_CovTyp,"") ) {     /* No more recs            */
      Error_Window ("Logic Error - Can't Locate Cov Rec in Table", "CVT_CovTypNum");
      return 0; }
    if ( !strcmp (sr_CVT[i].cr_CovTyp,cr_CovTyp) ){     /*  all done               */
      return sr_CVT[i].i_Num; } }
   Error_Window ("Logic Error - Can't Locate Cov Rec in Table", "CVT_CovTypNum");
   return 0;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_ChkReg
* Desc: see if the Region is in Region array
*       Checks a specifed CVT record in the table, you send in index
*       Also see  'CVT_CheRegion'
* Note-1: see notes CVT_CheRegion
*   In: iX.......index into CVT Table
*       i_Reg....region
*  Ret: 1 OK region is in Cover Type,  else 0
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int CVT_ChkReg (int iXX, int i_Reg)
{
int iX;
   iX = i_Reg - 1;                             /* adujust this               */
   if ( sr_CVT[iXX].ir_Region[iX] == i_Reg )    /* see Note-1                 */
     return 1;                                 /* OK region is in Cov Typ    */
   return 0;                                   /* Shouldn't get here         */
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Trim_CovTyp
* Desc: Trim the trailing blanks off the end
*   In: cr......string
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Trim_CovTyp (char cr[])
{
int i,j;
   j = strlen (cr) - 1 ;                /* end of string                     */
   for ( i = j; i >= 0; i-- ) {         /* go backward thru string           */
     if ( cr[i] != ' '  )               /* look for 1st non-blank char       */
       break;                           /* done                              */
     cr[i] = 0; }                       /* null term, each blank till done   */
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_ChkVer
* Desc: Check the Cover Type File Version number
*   In: cr_Line.....line from file to be checked
*       cr_Version...version number (ex: 5.10 )
*  Ret: 1 OK,  0 Error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  CVT_ChkVer (char cr_Line[], char cr_Version[])
{
char cr[300];
  if ( strstr (cr_Line,cr_Version) )
    return 1;
  sprintf (cr,"Cover Type File has wronge version number. Correct # is: %s\n",cr_Version);
  Error_Window (cr,"CVT_ChkVer");
  return 0;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_GetName
* Desc: Get the Cover Type Name from Table, use for an 'iterative get'
*   In: iX....incrementing index, to get each one
*  Out: cr_CovTyp....Cover Type Name, NULL when no more
#  Ret: 1 ok,  0 no more
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  CVT_GetName (int iX, char cr_CovTyp[])
{
   strcpy (cr_CovTyp, sr_CVT[iX].cr_CovTyp);
   if ( !strcmp (sr_CVT[iX].cr_CovTyp,"") )
     return 0;
   return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_LoadExec
* Desc: Load a Cover Type Disk File into CVT table.
*       Function looks at the Cover Id Window Combo Box on screen
* Note-1: This function call will check for any duplicate Cover Type
*         names that are now in the loaded CVT,
*         This check is really only need for development, once the cover
*         files are made they should be ok, but you never know, I just
*         left the check in to make sure.
*         This function will get run whenever ther user changes the
*         (FCC,SAF,NVCS) dropdown window, so the files can get checked
*         un-necessaryly more than once, but this shouldn't be a problem
*         because once a user normal user sets the (FCC,SAF,NVCS) Dropdown
*         they won't be changing it much, like I do under developement.
*  Ret: 1 OK,  0 Error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  CVT_LoadExec (char cr_Version[])
{
char  cr_WndTxt[eC_WndTxt];

   CCW_GetWndTxt (e_CCW_Cid, cr_WndTxt);          /* Get Cover Id frm Wnd    */
   if ( !strcmp (cr_WndTxt,e_SAF) ) {             /* to determine which      */
     if ( !CVT_Load (cr_Version, e_FN_SAF) )      /* input Cover File we need*/
        return 0; }

   else  if ( !strcmp (cr_WndTxt,e_NVCS) ) {
     if ( !CVT_Load (cr_Version, e_FN_NVCS) )
        return 0; }

   else  if ( !strcmp (cr_WndTxt,e_FCC) ) {
     if ( !CVT_LoadFCC (cr_Version, e_FN_FCC) )
        return 0; }

   else  if ( !strcmp (cr_WndTxt,e_FLM) ) {
     if ( !CVT_LoadFLM (cr_Version, e_FN_FLM) )
        return 0; }


   else  {
      Error_Window ("Logic Error", "CVT_LoadExec");
      return 0; }

   CVT_DupCovTyp();                          /* See Note-1 above             */

// CVT_Display  ();

   return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_Load
* Desc: Read in an SAF or NVCS Cover-Load File and save it in the CVT table
*   In: cr_Version....version number in head of Cover File
*       cr_FN.........Cover File Name
*  Ret: number of Cover Type records read in,   0 Error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int CVT_Load  (char cr_Version[], char cr_FN[])
{
int i, iX;
FILE *fh ;
#define eC_line 300
char cr_Line [eC_line], *a, cr[200],  cr_CovTyp[200];
char cr_Pth[eC_PthFil+1];
#define  eO 20

    strcpy (cr_Pth,gcr_datDir);
    strcat (cr_Pth,cr_FN);

    iX = 0;
    CVT_InitTable();                         /* Init the table               */
    fh =  fopen (cr_Pth, "r" );
    if ( fh == NULL ) {
      sprintf (cr,"Can't Open Input Cover File: %s\n", cr_Pth);
      strcat  (cr,"File must be in same directory as fofem.exe");
      Error_Window (cr, "CVT_LoadSAF");
      return 0;}

    for ( i = 0; i < eC_CVT - 2; i++ ) {
      a = fgets (cr_Line, eC_line, fh );     /* Read a line from file        */
        if ( a == NULL ) {                   /* End of File                  */
           fclose (fh);
           CVT_FCC_Dummy(iX);                /* insrt dumy reg rec           */
           return iX; }                      /* # of records loaded          */
      if ( strchr (cr_Line,'\t') ) {
        Tab_Error (cr_Line,"CVT_LoadSAF",cr_FN);
        return 0; }

      Blk_End_Line (cr_Line,eC_line);              /* Blank to end of line   */
      if ( isBlank (cr_Line) )                     /* if line is empty then  */
        continue;                                  /*  skip it               */

      if ( i == 0 )                                /* First line of file     */
        if ( !CVT_ChkVer(cr_Line,cr_Version) )     /* Make sure Cov file has */
          return 0;                                /* correct version #      */

      if ( cr_Line[0] == '#' )                     /* lines is a comment so  */
        continue;                                  /*  skip it               */

      ToInt (&sr_CVT[iX].i_Num, cr_Line, 1, 4 );    /* Cover Number as int    */

      ToInt (&sr_CVT[iX].ir_Region[0], cr_Line,5,6); /* Region #s       */
      ToInt (&sr_CVT[iX].ir_Region[1], cr_Line,7,8);
      ToInt (&sr_CVT[iX].ir_Region[2], cr_Line,9,10);
      ToInt (&sr_CVT[iX].ir_Region[3], cr_Line,11,12);
      if ( sr_CVT[iX].ir_Region[0] == 0 ) sr_CVT[iX].ir_Region[0] = -1;
      if ( sr_CVT[iX].ir_Region[1] == 0 ) sr_CVT[iX].ir_Region[1] = -1;
      if ( sr_CVT[iX].ir_Region[2] == 0 ) sr_CVT[iX].ir_Region[2] = -1;
      if ( sr_CVT[iX].ir_Region[3] == 0 ) sr_CVT[iX].ir_Region[3] = -1;

      if ( !strcmp (cr_FN,e_FN_SAF) ) {       /* If Doing SAF File           */
        ToStr (cr, cr_Line, 13, 22);          /* SAF Number                  */
        Trim_CovTyp (cr);
        strcat (cr," - ");                    /* put a dash on end           */
        ToStr (cr_CovTyp, cr_Line, 23,102);   /* Cover Type Name             */
        Trim_CovTyp (cr_CovTyp);
        strcat (cr,cr_CovTyp);                /* append them                 */
        strcpy (sr_CVT[iX].cr_CovTyp,cr); }

      else {                                  /* If Doing NVCS File          */
        ToStr (sr_CVT[iX].cr_CovTyp, cr_Line,23,122);
        Trim_CovTyp (sr_CVT[iX].cr_CovTyp); }


      ToStr (sr_CVT[iX].cr_FueRef, cr_Line,123, 137);
      Trim_CovTyp (sr_CVT[iX].cr_FueRef);

      if ( !strcmp (cr_FN,e_FN_NVCS) ) {           /* If Doing NVCS File     */
        if ( !stricmp (sr_CVT[iX].cr_FueRef,"X"))  /* Skip recs with X       */
          continue; }

      ToFlo (&sr_CVT[iX].s_CFL.f_litter, cr_Line, 138, 143 );
      ToFlo (&sr_CVT[iX].s_CFL.f_DW0,    cr_Line, 144, 149 );
      ToFlo (&sr_CVT[iX].s_CFL.f_DW10,   cr_Line, 150, 155 );
      ToFlo (&sr_CVT[iX].s_CFL.f_DW100,  cr_Line, 156, 161 );
      ToFlo (&sr_CVT[iX].s_CFL.f_DW1000, cr_Line, 162, 167 );

      ToFlo (&sr_CVT[iX].s_CFL.f_MeaDia, cr_Line, 168, 173);
      ToFlo (&sr_CVT[iX].s_CFL.f_Duff,   cr_Line, 174, 179);
      ToFlo (&sr_CVT[iX].s_CFL.f_DufDep, cr_Line, 180, 185);
      ToStr (sr_CVT[iX].s_CFL.cr_HvCod, cr_Line,  186, 187);
      Trim_LT (sr_CVT[iX].s_CFL.cr_HvCod);

      ToFlo (&sr_CVT[iX].s_CFL.f_Herb, cr_Line, 188, 193);
      ToStr (sr_CVT[iX].s_CFL.cr_Herb, cr_Line, 194, 195);
      Trim_LT (sr_CVT[iX].s_CFL.cr_Herb);

      ToFlo (&sr_CVT[iX].s_CFL.f_Shrub, cr_Line, 196, 201);
      ToStr (sr_CVT[iX].s_CFL.cr_Shrub, cr_Line, 202, 203);
      Trim_LT(sr_CVT[iX].s_CFL.cr_Shrub);

      ToFlo (&sr_CVT[iX].s_CFL.f_CroFol, cr_Line,204,209);
      ToStr (sr_CVT[iX].s_CFL.cr_CroFol, cr_Line,210,211);
      Trim_LT (sr_CVT[iX].s_CFL.cr_CroFol);

      ToFlo (&sr_CVT[iX].s_CFL.f_CroBra, cr_Line,212,217);
      ToStr (sr_CVT[iX].s_CFL.cr_CroBra, cr_Line,218,219);
      Trim_LT(sr_CVT[iX].s_CFL.cr_CroBra);

      ToStr (sr_CVT[iX].cr_AddRef, cr_Line,220,234);
      Trim_CovTyp (sr_CVT[iX].cr_AddRef);

      ToStr (sr_CVT[iX].cr_CovGrp, cr_Line,235,237);
      Trim_CovTyp (sr_CVT[iX].cr_CovGrp);

      iX++;

    }   /* for i */

    fclose (fh);
    Error_Window ("ERROR - Serious - Internal Cover Type Array to Small","CVT_LoadSAF");
    return 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_FCC_Dummy
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
void CVT_FCC_Dummy(int iX)
{
   sr_CVT[iX].i_Num = 0;                     /* Cover Number as int    */
   strcpy (sr_CVT[iX].cr_CovTyp,"-> No Cover Records for Region <-");

   sr_CVT[iX].ir_Region[0] = -1;
   sr_CVT[iX].ir_Region[1] = -1;
   sr_CVT[iX].ir_Region[2] = -1;
   sr_CVT[iX].ir_Region[3] = -1;

   if ( !CVT_isReg(1) )                      /* Note-1 above                 */
     sr_CVT[iX].ir_Region[0] = 1;
   if ( !CVT_isReg(2) )
     sr_CVT[iX].ir_Region[1] = 2;
   if ( !CVT_isReg(3) )
     sr_CVT[iX].ir_Region[2] = 3;
   if ( !CVT_isReg(4) )
     sr_CVT[iX].ir_Region[3] = 4;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_isReg
* Desc: Look thru all records in the CVT and see if any of them have
*        the specified region. As soon as one is found return.
*        Don't want to know how many just that there is a least one in
*        the table somewhere.
*   In: i_Reg......region to look for
*  Ret: 1 found a record that has the region
*       0 no records found with region
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  CVT_isReg (int i_Reg)
{
int i,j;
  for ( i = 0; i < eC_CVT -1; i++ ) {        /* look at every rec            */
    if ( !strcmp (sr_CVT[i].cr_CovTyp,"") ) /* Hit end                      */
       return 0;                             /* none found                   */
    for ( j = 0; j < 4; j++ ) {              /* look in region arrary for    */
      if ( sr_CVT[i].ir_Region[j] == i_Reg ) /* region                       */
        return 1; }                          /* found it                     */
  }
  return 0;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Add_Sep
* Desc: Help piece together the Cover Type Name for the FCC data file
*   In: cr...........this gets appended
*  Out: cr_Cov.......Cover Type to append to
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Add_Sep (char cr_Cov[], char cr[])
{
  Trim_LT (cr);                         /* Trim lead & trail blanks          */
  strcat (cr_Cov,"-");                  /* a spacer                          */
  strcat (cr_Cov,cr);                   /* cat it on                         */
  if ( strlen (cr_Cov) > e_CT ) {       /* watch out, shouldn't happen       */
    Error_Window ("Appended FCC Cover Type To Large","Add_Sep");
    cr_Cov[e_CT] = 0; }                 /* null term it, in limits           */
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_DDRatio
* Desc: Return the Duff TPA Amount to Duff Depth Ratio
*  Ret: ratio,  0 if can't do it, see code
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
float CVT_DDRatio ()
{
float f;
d_CFL s_CFL;
  if ( !CVT_GetActRec (&s_CFL) )          /* get active Fuel Loading Rec     */
    return 0;                             /* There is none                   */
  if ( s_CFL.f_DufDep == 0 )              /* if none,                        */
    return 0;
  if ( s_CFL.f_Duff == 0 )                /* if none,                        */
    return 0;
  f = s_CFL.f_Duff / s_CFL.f_DufDep;      /* calc the ratio                  */
  return f;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_Display
* Desc: Not used here in case we need to check something
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void   CVT_Display  ()
{
int i;
char  cr_CovTyp[200];

    for ( i = 0; i < 20000; i++ ) {
      if ( !strcmp (sr_CVT[i].cr_CovTyp,"") ) {
        printf ("Records: %d \n", i );
        break; }

        strcpy (cr_CovTyp,sr_CVT[i].cr_CovTyp);
        cr_CovTyp[10] = 0;

     printf ( ">>%s, %s, Herb:%5.2f %s, Shr:%5.2f %s,  Fol:%5.2f %s,  Bra:%5.2f %s,  Ref:%s,  AdRef:%s, \n",
          cr_CovTyp,
          sr_CVT[i].s_CFL.cr_HvCod,
          sr_CVT[i].s_CFL.f_Herb,
          sr_CVT[i].s_CFL.cr_Herb,
          sr_CVT[i].s_CFL.f_Shrub,
          sr_CVT[i].s_CFL.cr_Shrub,
          sr_CVT[i].s_CFL.f_CroFol,
          sr_CVT[i].s_CFL.cr_CroFol,
          sr_CVT[i].s_CFL.f_CroBra,
          sr_CVT[i].s_CFL.cr_CroBra,
          sr_CVT[i].cr_FueRef,
          sr_CVT[i].cr_AddRef);
//   printf ( ">>%s, Lit:%5.2f, DW1:%5.2f, DW10:%5.2f, DW100:%5.2f, DW1000:%5.2f, Dia:%5.2f, Duf:%5.2f, Dep:%5.2f,\n",
//        cr_CovTyp,
//        sr_CVT[i].s_CFL.f_litter,
//        sr_CVT[i].s_CFL.f_DW0,
//        sr_CVT[i].s_CFL.f_DW10,
//        sr_CVT[i].s_CFL.f_DW100,
//        sr_CVT[i].s_CFL.f_DW1000,
//        sr_CVT[i].s_CFL.f_MeaDia,
//        sr_CVT[i].s_CFL.f_Duff,
//        sr_CVT[i].s_CFL.f_DufDep);
    }
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_InitTable
* Desc: Go thru table and init all fields
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  CVT_InitTable ()
{
int i,j;
  for ( i = 0; i < eC_CVT -1; i++ ) {
    sr_CVT[i].i_Num = 0;
    strcpy (sr_CVT[i].cr_CovTyp,"");
    strcpy (sr_CVT[i].cr_FueRef,"");
    strcpy (sr_CVT[i].cr_CovGrp,"");
    for ( j = 0; j < 4; j++ )
      sr_CVT[i].ir_Region[j] = 0;
    CFL_Init (&sr_CVT[i].s_CFL);    }


}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CFL_Init
* Desc: Init the CFL (Cover Fuel Loading) Structure
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  CFL_Init (d_CFL *a_CFL)
{
   a_CFL->f_litter = 0;
   a_CFL->f_DW0 = 0;
   a_CFL->f_DW10 = 0;
   a_CFL->f_DW100 = 0;
   a_CFL->f_DW1000 = 0;
   a_CFL->f_MeaDia = 0;
   a_CFL->f_Duff = 0;
   a_CFL->f_DufDep = 0;
   strcpy (a_CFL->cr_HvCod,"");
   a_CFL->f_Herb  = 0;
   strcpy (a_CFL->cr_Herb,"");
   a_CFL->f_Shrub = 0;
   strcpy (a_CFL->cr_Shrub,"");
   a_CFL->f_CroFol = 0;
   strcpy (a_CFL->cr_CroFol,"");
   a_CFL->f_CroBra = 0;
   strcpy (a_CFL->cr_CroBra,"");
   strcpy (a_CFL->cr_Comment,"");

   strcpy (a_CFL->cr_FLMSoil,"");
   strcpy (a_CFL->cr_FLMLogDist,"");

}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_DupCovTyp
* Desc: Check for any duplicate Cover Type Names in the CVT Load Table
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void   CVT_DupCovTyp  ()
{
int i,j;
char cr[300], cr1[200];
    for ( i = 0; i < 20000; i++ ) {
      for ( j = i + 1 ; j < 20000; j++ ) {
        if ( !strcmp (sr_CVT[j].cr_CovTyp,"") ) {
           break;  }
        if ( !stricmp (sr_CVT[i].cr_CovTyp,sr_CVT[j].cr_CovTyp) ) {
           strcpy  (cr, "Duplicate Cover Type Names Found in Input File\n");
           sprintf (cr1,">%s<",sr_CVT[i].cr_CovTyp);
           strcat  (cr,cr1);
           Error_Window (cr,"CVT_DupCovTyp");
           exit (1); } }
      if ( !strcmp (sr_CVT[i].cr_CovTyp,"") )
         break;
    }
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Tab_Error
* Desc: Display an Error Message say tabs characters have been found in
*        a file.
*   In: cr_Line......Line containing tabs
*       cr_FuncName..Name of function that called this one
*       cr_FN........File Name containing tabs
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void Tab_Error (char cr_Line[], char cr_FuncName[], char cr_FN[])
{
char cr[500];
  sprintf (cr,"ERROR - %s - Tab characters found input file %s\n",cr_FuncName,cr_FN);
  strcat  (cr," Use a Text Editor that will not put or remove tabs from file.\n");
  strcat  (cr, cr_Line);
  Error_Window (cr,"Tab_Error");
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CVT_GetActRec
* Desc: Find the active (cover type use has set Window) Cover/Fuel Record
*       in the Cover Table and send back a copy of ther CFL struct that is
*       in that record.
*  Out: a_CFL.......copy of
*  Ret: 1 OK, 0 Error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int   CVT_GetActRec (d_CFL *a_CFL)
{
int i;
char cr_CovTyp[eC_WndTxt];

  CCW_CovTyp (cr_CovTyp);            /* Get Cover Type from Screen   */

  for ( i = 0; i < eC_CVT; i++ ) {
    if ( !strcmp ( sr_CVT[i].cr_CovTyp,"") ) {
      Error_Window ("Can't Find Cover Type Name Record in CVT Table","CVT_GetActRec");
      return 0; }
    if ( !strcmp ( sr_CVT[i].cr_CovTyp,cr_CovTyp) )
       break;
  }

  memcpy (a_CFL, &sr_CVT[i].s_CFL, sizeof(d_CFL) );

  return 1;
}
