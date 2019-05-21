/***********************************************************************
* Name: Wnd_SAF.cpp
* Desc: Deal with the SAF/SRM input fuel file
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


#include "cdf_util.h"


char gcr_WD[5000];
int giS_FCCS = 0;
bool gb_FCCS = false;

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* Burnup Weight Distrubution of 3+ Dead Wood                                */
typedef struct {
  char  cr_WD[20];
  float f_pc3_6;
  float f_pc6_9;
  float f_pc9_20;
  float f_pc20;
 } d_BWD;

/*                  3-6    6-9   9-20    20+                               */
d_BWD  sr_BWD[] = {
       {e_Even,   0.25,  0.25,  0.25,  0.25 },
       {e_Right,  0.07,  0.16,  0.27,  0.50 },
       {e_Left,   0.50,  0.27,  0.16,  0.07 },
       {e_EndWD,    0.35,  0.15,  0.15,  0.35 },
       {e_Center, 0.15,  0.35,  0.35,  0.15 },
       {"",          0,     0,     0,     0 }};


/*-----------------------------------------------------------------------*/

 extern d_CVT sr_CVT[];    /* All Cov Typs from disk file       */


/*------------------------------------------------------------*/
 int gi_Num; 
 int gi_Reg1, gi_Reg2, gi_Reg3, gi_Reg4;
 char cr_CovNam[200], cr_CovDes[200], cr_FueRef[200];
float f_litter, f_DW0, f_DW10,f_DW100, f_DW1000;
float f_pcRot, f_MeaDia, f_Duff, f_DufDep;
float f_Herb, f_Shrub, f_CroFol, f_CroBra;

 char cr_HvCod [200] ;
 char cr_Herb  [200] ;
 char cr_Shrub [200] ;
 char cr_CroFol[200] ;
 char cr_CroBra[200] ;
 char cr_AddRef[200] ;
 char cr_CovGrp[200] ;
 char cr_EmiGrpFla  [200] ;  /* wood, lit, shr, herb, fol bra */
 char cr_EmiGrpDuf  [200] ;  /* duff RSC */
 char cr_EmiGrpS1k  [200];   /* Smolder 1k wood RSC */

/* This table describes the format of the comma delimited input file */
d_CDF sr_SAF[] = {


   { "Num",     e_Int, 0, &gi_Num }, 
   { "Reg1",    e_Int, 0, &gi_Reg1 }, 
   { "Reg2",    e_Int, 0, &gi_Reg2 }, 
   { "Reg3",    e_Int, 0, &gi_Reg3 }, 
   { "Reg4",    e_Int, 0, &gi_Reg4 }, 
   { "CovNam",  e_Str, 200, cr_CovNam },
   { "CovDes",  e_Str, 200, cr_CovDes }, 
   { "FueRef",  e_Str, 200, cr_FueRef },

   { "f_litter",  e_Flo, 0,   &f_litter  },
   { "f_DW0",     e_Flo, 0,   &f_DW0     },
   { "f_DW10",    e_Flo, 0,   &f_DW10    },
   { "f_DW100",   e_Flo, 0,   &f_DW100   },
   { "f_DW1000",  e_Flo, 0,   &f_DW1000  },
   { "f_MeaDia",  e_Flo, 0,   &f_MeaDia  },
   { "f_Duff",    e_Flo, 0,   &f_Duff    },
   { "f_DufDep",  e_Flo, 0,   &f_DufDep  },
   { "cr_HvCod",  e_Str, 200, cr_HvCod   },
   { "f_Herb",    e_Flo, 0,   &f_Herb    },
   { "cr_Herb",   e_Str, 200, cr_Herb    },
   { "f_Shrub",   e_Flo, 0,   &f_Shrub   },
   { "cr_Shrub",  e_Str, 200, cr_Shrub   },
   { "f_CroFol",  e_Flo, 0,   &f_CroFol  },
   { "cr_CroFol", e_Str, 200, cr_CroFol  },
   { "f_CroBra",  e_Flo, 0,   &f_CroBra  },
   { "cr_CroBra", e_Str, 200, cr_CroBra  },
   { "cr_AddRef", e_Str, 200, cr_AddRef  },
   { "cr_CovGrp", e_Str, 200, cr_CovGrp  },
   { "cr_EmiGrpFla",   e_Str, 200, cr_EmiGrpFla  },
   { "cr_EmiGrpDuf",   e_Str, 200, cr_EmiGrpDuf  },
   { "cr_EmiGrpS1k",   e_Str, 200, cr_EmiGrpS1k  },
   { "",  e_End, 5, "" }  };
/*-----------------------------------------------------------------------*/

int   WD_WeiDis (d_BWD *a_BWD, char cr_WD[]);
int  _FCC_OvridMes (float f_pc, float f_3Plus, char cr_CovCls[]);
void _SetFlo (float *a, float b);

/***********************************************************************
* Name: SAF_LoadFileNew
* Desc: Read the SAF cover load file into the CVT table structure
* NOTE: this is the new Version that reads in the .csv file, the
*       previous version was fixed column.
***********************************************************************/
int SAF_LoadFileNew (char cr_FN[], char cr_ErrMes[])
{
int i,j, iX;
char  cr[3000];
FILE *fh ;
d_CDF  *a;
    strcpy (cr_ErrMes,"");   

	a = &sr_SAF[0]; 

    iX = 0;
    SAF_InitTable();                         /* Init the table               */
	fh =  fopen (cr_FN, "r" );
    if ( fh == NULL ) {
      sprintf (cr_ErrMes,"Can't Open Input Cover File: %s\n", cr_FN);
      strcat  (cr_ErrMes,"File must be User Folder");
      return 0;}

    for ( i = 0; i < eC_CVT - 2; i++ ) {

      j = CDF_Get (a,fh,cr);

      if ( j == 0 )        /* End of File */ 
	    break; 

	  if ( j == -1 ) { 
		sprintf (cr_ErrMes, "Invalid file format detected\n%s\n%s\n Approximate row # %d",cr, cr_FN,i);
	    fclose (fh); 
		return 0; }

      sr_CVT[iX].ir_Region[0] = gi_Reg1; 
      sr_CVT[iX].ir_Region[1] = gi_Reg2;
      sr_CVT[iX].ir_Region[2] = gi_Reg3; 
      sr_CVT[iX].ir_Region[3] = gi_Reg4; 
      if ( sr_CVT[iX].ir_Region[0] == 0 ) sr_CVT[iX].ir_Region[0] = -1;
      if ( sr_CVT[iX].ir_Region[1] == 0 ) sr_CVT[iX].ir_Region[1] = -1;
      if ( sr_CVT[iX].ir_Region[2] == 0 ) sr_CVT[iX].ir_Region[2] = -1;
      if ( sr_CVT[iX].ir_Region[3] == 0 ) sr_CVT[iX].ir_Region[3] = -1;

      Trim_LT (cr_CovNam);
      Trim_LT (cr_CovDes);  
      sprintf (sr_CVT[iX].cr_CovTyp,"%s - %s",cr_CovNam,cr_CovDes); 

      strcpy (sr_CVT[iX].cr_FueRef, cr_FueRef);

      _SetFlo (&sr_CVT[iX].s_CFL.f_litter,f_litter);
      _SetFlo (&sr_CVT[iX].s_CFL.f_DW0,   f_DW0);
      _SetFlo (&sr_CVT[iX].s_CFL.f_DW10,  f_DW10);
      _SetFlo (&sr_CVT[iX].s_CFL.f_DW100, f_DW100);
      _SetFlo (&sr_CVT[iX].s_CFL.f_DW1000,f_DW1000);

      sr_CVT[iX].s_CFL.f_pcRot = e_RotLogDefFlt;    /* default percent Rotten, SAF has none in input file */

      _SetFlo (&sr_CVT[iX].s_CFL.f_MeaDia, f_MeaDia);
      _SetFlo (&sr_CVT[iX].s_CFL.f_Duff,   f_Duff);
      _SetFlo (&sr_CVT[iX].s_CFL.f_DufDep, f_DufDep);

      Trim_LT (cr_HvCod);
      strcpy (sr_CVT[iX].s_CFL.cr_HvCod,cr_HvCod);

      _SetFlo (&sr_CVT[iX].s_CFL.f_Herb,f_Herb);
      Trim_LT (cr_Herb);
      strcpy (sr_CVT[iX].s_CFL.cr_Herb,cr_Herb);

      _SetFlo (&sr_CVT[iX].s_CFL.f_Shrub, f_Shrub);
      Trim_LT(cr_Shrub);
      strcpy  (sr_CVT[iX].s_CFL.cr_Shrub,cr_Shrub);

      _SetFlo (&sr_CVT[iX].s_CFL.f_CroFol, f_CroFol);
      Trim_LT (cr_CroFol);
      strcpy  (sr_CVT[iX].s_CFL.cr_CroFol,cr_CroFol);

      _SetFlo (&sr_CVT[iX].s_CFL.f_CroBra, f_CroBra);
      Trim_LT(cr_CroBra);
      strcpy (sr_CVT[iX].s_CFL.cr_CroBra,cr_CroBra );

      Trim_LT (cr_AddRef);
      strcpy (sr_CVT[iX].cr_AddRef,cr_AddRef);

      Trim_LT (cr_CovGrp);
      strcpy (sr_CVT[iX].cr_CovGrp,cr_CovGrp);

      Trim_LT (cr_EmiGrpFla);
      strcpy (sr_CVT[iX].s_CFL.cr_EmiGrpFla,cr_EmiGrpFla);

      Trim_LT (cr_EmiGrpDuf);
      strcpy (sr_CVT[iX].s_CFL.cr_EmiGrpDuf,cr_EmiGrpDuf);

      Trim_LT (cr_EmiGrpS1k);
      strcpy (sr_CVT[iX].s_CFL.cr_EmiGrpS1k,cr_EmiGrpS1k);
	  
      iX++;
    }

  fclose (fh); 
  return 1; 
}

/*******************************************************************
* Check for missing value ( -1 ) and set to 0
* missing float values readin from file will -1
*******************************************************************/
void _SetFlo (float *a, float b)
{
  if ( b < 0 )
    *a = 0;
  else 
    *a = b; 
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SAF_LoadFile
*
*      NOTE NOTE ----- This is the old version, see SAF_LoadFileNew () 
* 
* Desc: Read in an SAF or NVCS Cover-Load File and save it in the CVT table
*   In: cr_Version....version number in head of Cover File
*       cr_FN.........Cover File Name
*  Ret: number of Cover Type records read in,   0 Error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int SAF_LoadFile ( char cr_FN[], char cr_ErrMes[])
{
int i, iX;
FILE *fh ;
#define eC_line 300
char cr_Line [eC_line], *a, cr[200];
char  cr_CovTyp[200];
// char cr_Pth[2000];
#define  eO 20

    strcpy (cr_ErrMes,"");    

     iX = 0;
    SAF_InitTable();                         /* Init the table               */
    fh =  fopen (cr_FN, "r" );
    if ( fh == NULL ) {
      sprintf (cr_ErrMes,"Can't Open Input Cover File: %s\n", cr_FN);
      strcat  (cr_ErrMes,"File must be in same directory as fofem.exe");
      return 0;}

    for ( i = 0; i < eC_CVT - 2; i++ ) {
      a = fgets (cr_Line, eC_line, fh );     /* Read a line from file        */
        if ( a == NULL ) {                   /* End of File                  */
           fclose (fh);
           CVT_Dummy(iX);                /* insrt dumy reg rec           */
           return iX; }                      /* # of records loaded          */
      if ( strchr (cr_Line,'\t') ) {
        strcpy (cr_ErrMes,"CVT_Load - Tab Error ");
        return 0; }

      Blk_End_Line (cr_Line,eC_line);              /* Blank to end of line   */
      if ( isBlank (cr_Line) )                     /* if line is empty then  */
        continue;                                  /*  skip it               */

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

      ToStr (cr, cr_Line, 13, 22);          /* SAF Number                  */
      Trim_CovTyp (cr);
      strcat (cr," - ");                    /* put a dash on end           */
      ToStr (cr_CovTyp, cr_Line, 23,102);   /* Cover Type Name             */
      Trim_CovTyp (cr_CovTyp);
      strcat (cr,cr_CovTyp);                /* append them                 */
      strcpy (sr_CVT[iX].cr_CovTyp,cr);

      ToStr (sr_CVT[iX].cr_FueRef, cr_Line,123, 137);
      Trim_CovTyp (sr_CVT[iX].cr_FueRef);

      ToFlo (&sr_CVT[iX].s_CFL.f_litter, cr_Line, 138, 143 );
      ToFlo (&sr_CVT[iX].s_CFL.f_DW0,    cr_Line, 144, 149 );
      ToFlo (&sr_CVT[iX].s_CFL.f_DW10,   cr_Line, 150, 155 );
      ToFlo (&sr_CVT[iX].s_CFL.f_DW100,  cr_Line, 156, 161 );
      ToFlo (&sr_CVT[iX].s_CFL.f_DW1000, cr_Line, 162, 167 );

      sr_CVT[iX].s_CFL.f_pcRot = e_RotLogDefFlt;    /* default percent Rotten, SAF has none in input file */

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
    return 0;
}



/**************************************************************************
*
*
**************************************************************************/
int Wnd_Mai::SAF_SetDeadLoad (TextBox ^tb, float f_Val, int i_Id, char cr_FueCat[], char cr_HvCod[])
{
float f_Adj; 

  f_Adj = 1.0; 
  if ( !_stricmp (cr_FueCat,e_FCa_Natural )) {
    if ( isFuelLight (tb))
      f_Adj = DA_AdjLight (i_Id);
    if ( isFuelHeavy (tb)) 
      f_Adj = DA_AdjHeavy (i_Id); 
    f_Val = f_Val * f_Adj; 
  }
 
  if ( !_stricmp (cr_FueCat,e_FCa_Pile)  || !_stricmp (cr_FueCat,e_FCa_Slash )) {
    if ( isFuelLight (tb))
      f_Val += FWF_HDAdj (i_Id, cr_HvCod, 'L'); 
    else if ( isFuelHeavy (tb)) 
      f_Val += FWF_HDAdj (i_Id, cr_HvCod, 'H'); 
    else 
      f_Val += FWF_HDAdj (i_Id, cr_HvCod, 'M'); }

Done: 
   if ( f_Val < 0 ) 
     f_Val = 0; 
   FloatToTextBox (tb, f_Val, 2);

 return 1; 
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: FWF_AdjLive
* Desc: Adjust a Live Loading, (Herb, Shrub, Crown Foliage & Branch)
*       base on user window setting of TPA Combo Box, (Typical,Sparse,Abundant)
*       put the adjust loading to its TPA Amount window.
*   In: 
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int Wnd_Mai::SAF_SetLiveLoad(TextBox ^tb, float f_Load, int i_Id, char cr_Code[]) 
{
float f,f_Adj;
char c, cr1[300], cr2[300];

  if ( this->isFuelTypical(tb) ) {                      /* Typical so no need to adjust     */
    f = f_Load;
    goto Done; }

  
  if ( this->isFuelSparse(tb) )
    c = 'S';
  else if ( this->isFuelAbundent(tb) )
    c = 'A'; 
  else if ( this->isFuelTypical(tb) ) {
    f_Adj = 1.0;   /* for typical there is nothing to adjust */
    goto Adj; }    
  else if ( this->isFuelUser(tb) ) {
    f_Adj = 1.0; 
    goto Adj; } 
  else  
 /* This really shouldn't happen, would be logic error */
    myMessageBox ("Logic Error SAF_SetLiveLoad() #2 ");  

  f_Adj = FWF_LAadj (i_Id, cr_Code, c);  /* Get Adjustment factor, spar-abun */
  if ( f_Adj <= 0 ) {
    strcpy (cr1,"Logic Error SAF_SetLiveLoad()  - don't know  Spare-Abundant adjust factor\n");
    sprintf (cr2, "%s - %c ", cr_Code, c);
    strcat (cr1,cr2);
    myMessageBox (cr1);
    f = f_Load;
    goto Done; }

Adj:
  f = f_Load * f_Adj;

Done: 
   if ( f < 0 ) 
     f = 0; 
   FloatToTextBox (tb, f, 2);
   return 1; 
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SAF_InitTable
* Desc: Go thru table and init all fields
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  SAF_InitTable ()
{
int i,j;
  for ( i = 0; i < eC_CVT -1; i++ ) {
    sr_CVT[i].i_Num = 0;

    strcpy (sr_CVT[i].cr_CovTyp,""); 
    strcpy (sr_CVT[i].cr_FueRef,"");
    strcpy (sr_CVT[i].cr_CovGrp,"");


    for ( j = 0; j < 4; j++ )
      sr_CVT[i].ir_Region[j] = 0;
    SAF_Init (&sr_CVT[i].s_CFL);    }
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SAF_Init
* Desc: Init the CFL (Cover Fuel Loading) Structure
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  SAF_Init (d_CFL *a_CFL)
{
   a_CFL->f_litter = 0;
   a_CFL->f_DW0 = 0;
   a_CFL->f_DW10 = 0;
   a_CFL->f_DW100 = 0;
   a_CFL->f_DW1000 = 0;
   a_CFL->f_MeaDia = 0;
   a_CFL->f_Duff = 0;
   a_CFL->f_DufDep = 0;

   a_CFL->f_pcRot = 0; 

   a_CFL->f_39s = a_CFL->f_920s = a_CFL->f_20ps = 0;
   a_CFL->f_39r = a_CFL->f_920r = a_CFL->f_20pr = 0;

/* FFI 3+ size classes, input file has individual class loads for sound & rot */
   a_CFL->f_Snd3 = a_CFL->f_Snd6 = a_CFL->f_Snd9 = a_CFL->f_Snd12 = a_CFL->f_Snd20 = 0;
   a_CFL->f_Rot3 = a_CFL->f_Rot6 = a_CFL->f_Rot9 = a_CFL->f_Rot12 = a_CFL->f_Rot20 = 0;

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


   strcpy (a_CFL->cr_EmiGrpFla,"");   /* Emission Group Flame and Smoldering numbers */
   strcpy (a_CFL->cr_EmiGrpDuf,"");
   strcpy (a_CFL->cr_EmiGrpS1k,"");

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
* Name: SAF_isReg
* Desc: Look thru all records in the SAF and see if any of them have
*        the specified region. As soon as one is found return.
*        Don't want to know how many just that there is a least one in
*        the table somewhere.
*   In: i_Reg......region to look for
*  Ret: 1 found a record that has the region
*       0 no records found with region
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  SAF_isReg (int i_Reg)
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



/**************************************************************************
*
*************************************************************************/
int   SAF_GetRefer (char cr_CovTyp[], char cr_FueRef[], char cr_AddRef[])
{
int i;

  strcpy (cr_FueRef,"");
  strcpy (cr_AddRef,""); 
  for ( i = 0; i < eC_CVT; i++ ) {
    if ( !strcmp ( sr_CVT[i].cr_CovTyp,"") ) {
      return -1; }
    if ( !strcmp ( sr_CVT[i].cr_CovTyp,cr_CovTyp) )
       break;
  }

  strcpy (cr_FueRef, sr_CVT[i].cr_FueRef);
  strcpy (cr_AddRef,  sr_CVT[i].cr_AddRef);  

  return i;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: FWF_AdjDuff
* Desc: Put Duff Depth and Duff TPA to their Wnds, these values
*       get adjusted as needed and then put to Wnd.
* Note-1:  Some duff loads would get adjusted to below the lower duff
*           limit, so we catch it here. 
*           I think we need that lower limit for the duff equations or 
*           or burnup because something will blowup if to low a number 
*           is used. 
* Note-2: 9-5-14, DL wanted the depth to adjust to when the load got 
*          adjusted, Note-> FOFEM 5 didn't do that, using the adjustment
*          #defines will adjust the depth to the same proportions as load
*   In: f_DuffTPA....duff load
*       f_DUffDepth..depth
*       cr_CovTyp....cover/fuel type from screen  
*       cr_CovGrp....cover group
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int Wnd_Mai::SAF_SetDuff (float f_DuffTPA, float f_DuffDepth, char cr_CovTyp[], char cr_CovGrp[])
{
float f_Duff, f_Dep;
char c, cr1[300], cr2[300];
String ^ Str; 

  f_Duff = f_DuffTPA;
  f_Dep = f_DuffDepth;

/* Only do adjust for SAF and NVCS, because others don't have adjust buttons */
/* See Note-2 above */
  Str = this->_cbFuelClass->Text;
  if ( Str != e_SAF_SRM && Str != e_NVCS ) 
    goto PutDuff; 

/*...........................................................................*/
/* Pocosin Cover Type, see Manual Appendix B, Equation 20 definitions        */
  if ( !_stricmp (cr_CovGrp, e_CVT_Pocosin)) {  /*  Pond Pine - Pocosin           */
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

/*...........................................................................*/
  if ( this->isFuelLight(this->_txDuff) )  {      /* what is current adj setting on Duff box  */
    f_Duff = f_Duff * e_AdjDufLit;
	f_Dep = f_Dep * e_AdjDufLit;         /* See Note-2 */
/* Change 6-6-12 see Note-1 above  */
   if ( f_Duff < e_DufMin && f_Duff != 0 ) 
      f_Duff = e_DufMin; 
  }
  
  if ( this->isFuelHeavy(this->_txDuff) ) {       
    f_Duff = f_Duff * e_AdjDufHvy;
	f_Dep = f_Dep * e_AdjDufHvy; }    /* See Note-2 */

  if ( f_Duff == 0 && f_Dep > 0 ) {           /* No Duff Tons per Acre       */
    f_Duff = f_Dep * 10;                      /*  so calc an amount          */
    strcpy (cr1,"No Duff Amount Available, program will calculate and set to:\n");
    sprintf (cr2,"  Duff Depth: %6.2f * 10 = %6.2f Duff Amt.\n", f_Dep, f_Duff);
    strcat (cr1,cr2);
    strcat(cr1,"\n");
    strcat(cr1,cr_CovTyp);
    myMessageBox (cr1);  }

PutDuff:
  FloatToTextBox (this->_txDuff, f_Duff, 2);
  FloatToTextBox (this->_txDuffDepth, f_Dep, 2);

  return 1;
}


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* SAF/SRM & NVCS = cover type classifications                               */

int SAF_WeightDist (d_CI *a_CI, char cr_WD[], float f_Load, float f_pc) 
{
char cr_ErrMes[500];
d_BWD s_BWD; 

  if ( !WD_WeiDis (&s_BWD,cr_WD) ) {      /* tells how we'll distribute        */
    sprintf (cr_ErrMes,"Invalid Weight Distribution indicatior: %s ",cr_WD);
    myMessageBox (cr_ErrMes); 
    return 0; }

   a_CI->f_Rot_DW3  = f_Load * f_pc * s_BWD.f_pc3_6;
   a_CI->f_Rot_DW6  = f_Load * f_pc * s_BWD.f_pc6_9;
   a_CI->f_Rot_DW9  = f_Load * f_pc * s_BWD.f_pc9_20;
   a_CI->f_Rot_DW20 = f_Load * f_pc * s_BWD.f_pc20;

   f_pc = 1.0 - f_pc;                  /* percent that is sound             */

   a_CI->f_Snd_DW3  = f_Load * f_pc * s_BWD.f_pc3_6;
   a_CI->f_Snd_DW6  = f_Load * f_pc * s_BWD.f_pc6_9;
   a_CI->f_Snd_DW9  = f_Load * f_pc * s_BWD.f_pc9_20;
   a_CI->f_Snd_DW20 = f_Load * f_pc * s_BWD.f_pc20;
   return 1;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: WD_WeiDis
* Desc: Look thru BWD Weight Distribution table and get the struct
*        that matches the define sent in, see table define
*  Out: a_BWD
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int WD_WeiDis (d_BWD *a_BWD, char cr_WD[])
{
int i;
  for ( i = 0; i < 100; i++ ) {
    if ( !strcmp ( sr_BWD[i].cr_WD,""))      /* Not found                    */
      return 0;

 
    if ( !stricmp ( sr_BWD[i].cr_WD,cr_WD) ){ /* is this the one we want      */
      memcpy (a_BWD,&sr_BWD[i], sizeof(d_BWD));
      return 1;  }                             /* OK found                     */
  
  }
  return 0;            /* Error Shouldn't get here */
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: WD_SetWeightDist
* Desc: Distribute the 1000 Hr fuel into individual size class loads
* Note-1: For the Batch program Cover Classifciation (SAF/NVCS/FCC)
*          is not used, just Cover Group Type, so this will not get
*          used because cr_CoverClass is not set to FCC
*   In: f_Load.....1000 hr load
*       f_pcRot....precent that is rotten, 0 --> 100, whole number
*       cr_WD......e_Even,e_Right,etc...see defines in above table
*  Out: a_CI.......see code below
*       cr_ErrMes..any error message
*  Ret: 1 OK, 0 error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int Wnd_Mai::WD_SetWeightDist (float f_Load, float f_pcRot, char cr_WD_In[], d_CI *a_CI, char cr_ErrMes[])
{
int i;
char  A[20], B[20], C[20], D[20], cr_CovTyp[300], cr_CovGrp[30];
char  cr[1000];
char  cr_WD[30];
d_BWD  s_BWD;
float  f_pc, f_3Plus;
d_CFL s_CFL;
String ^ Str; 

  strcpy (cr_ErrMes,"");
  strcpy (cr_WD, cr_WD_In); 

  if ( f_pcRot < 0 || f_pcRot > 100 ) {
    sprintf (cr_ErrMes,"Percent rotten %f is out of range ( 0 --> 100 )",f_pcRot);
    return 0; }

  f_pc = f_pcRot * 0.01;                /* calc % rot as a decimal           */

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* FCC = cover type classification                                           */
/* See Note-1 Above, about Batch Program                                     */
  if ( strcmp (a_CI->cr_CoverClass,e_FCCS) && strcmp (a_CI->cr_CoverClass,e_FFI) ) /* Not doing FCC or FFI so skip     */
     goto WD;         /* Go do normal distribution */

  Str = this->_cbCoverType->Text;
  _StrToChar(Str,cr_CovTyp);

  CVT_GetCFL (cr_CovTyp, &s_CFL, cr_CovGrp);    /* setting from CVT tabl, FCC loads    */

  if ( !_Get_TBFlo (this->_txRotLog, &f_pc, cr, "Foliage/Branch % consumed", cr_ErrMes))
    return 0; 
  
  sprintf (A,"%6.2f",s_CFL.f_pcRot);    /* to string, float compare could    */
  sprintf (B,"%6.2f",f_pc);             /* get percision problem             */

  if ( !_Get_TBFlo (this->_tx1kHr, &f_3Plus, cr, "1000 Hr", cr_ErrMes))
    return 0;

/* Happens when the Cover Type ComboBox Changes, and the Event Handler function */
/*  just wants the Fixed used */
  if ( gb_FCCS == false )                 /* Some Event wants mess disabled */ 
    goto Fixed; 

/* Need these to see if user changed the 1000 fuel of rotten from their orginal */
/*  setting in the input file (CVT table ) */
  sprintf (C,"%6.2f",s_CFL.f_DW1000);
  sprintf (D,"%6.2f",f_3Plus);

 /* if user changes 1000hr fuel, rotten percent or Fixed skew */
/*  give them a message - only does it once */
 if ( strcmp (A,B) || strcmp (C,D) || stricmp (cr_WD,e_Fixed) ) { 
    _FCC_OvridMes (s_CFL.f_pcRot,s_CFL.f_DW1000, a_CI->cr_CoverClass);  /* Explain and get 'Continue         */
    goto UserChanged; }

 Fixed:

 if ( !strcmp (a_CI->cr_CoverClass,e_FCCS) ) {
  a_CI->f_Rot_DW3  = s_CFL.f_39r * 0.5; /* FCCS has 3 -> 9 combined so       */
  a_CI->f_Rot_DW6  = s_CFL.f_39r * 0.5; /* we divy up evenly                 */
  a_CI->f_Rot_DW9  = s_CFL.f_920r;
  a_CI->f_Rot_DW20 = s_CFL.f_20pr;

  a_CI->f_Snd_DW3  = s_CFL.f_39s * 0.5;
  a_CI->f_Snd_DW6  = s_CFL.f_39s * 0.5;
  a_CI->f_Snd_DW9  = s_CFL.f_920s;
  a_CI->f_Snd_DW20 = s_CFL.f_20ps;
  return 1;  }

 if ( !strcmp (a_CI->cr_CoverClass,e_FFI) ) {
  a_CI->f_Rot_DW3  = s_CFL.f_Rot3; 
  a_CI->f_Rot_DW6  = s_CFL.f_Rot6; 
  a_CI->f_Rot_DW9  = s_CFL.f_Rot9 + s_CFL.f_Rot12;
  a_CI->f_Rot_DW20 = s_CFL.f_Rot20;

  a_CI->f_Snd_DW3  = s_CFL.f_Snd3; 
  a_CI->f_Snd_DW6  = s_CFL.f_Snd6; 
  a_CI->f_Snd_DW9  = s_CFL.f_Snd9 + s_CFL.f_Snd12;
  a_CI->f_Snd_DW20 = s_CFL.f_Snd20;
  return 1;  }

  myMessageBox ("Logic Error - in function Wnd_Mai::WD_SetWeightDist()"); 

/* At this point we know the user change something, load, roten, or skew */
/*  also at this point we can't do a Fixed because we have different load or rot */
/*  rather than try to get all the way out of here, we just warn and set to do */
/*  and Even skew */
UserChanged: 
   if ( !stricmp (cr_WD,e_Fixed) ) { 
     strcpy (cr, "You CANNOT Use a 'Fixed' log distribution after changing the\n");
     strcat (cr, " 3+ fuel load or rotten percent.\n 'Even' will be used for this calculation.\n\n"); 
     strcat (cr, "To reset fuel load right click box and select Typical\n");
     strcat (cr, "To reset rotten percent, select a different cover type and \n");
     strcat (cr, "and reselect the current one.\n"); 
     myMessageBox(cr); 
     strcpy (cr_WD,e_Even); }

     f_pc = f_pc * 0.01;           

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* SAF/SRM & NVCS = cover type classifications                               */
WD:

  if ( !WD_WeiDis (&s_BWD,cr_WD) ) {      /* tells how we'll distribute        */
    sprintf (cr_ErrMes,"Invalid Weight Distribution indicatior: %s ",cr_WD);
    return 0; }

   a_CI->f_Rot_DW3  = f_Load * f_pc * s_BWD.f_pc3_6;
   a_CI->f_Rot_DW6  = f_Load * f_pc * s_BWD.f_pc6_9;
   a_CI->f_Rot_DW9  = f_Load * f_pc * s_BWD.f_pc9_20;
   a_CI->f_Rot_DW20 = f_Load * f_pc * s_BWD.f_pc20;

   f_pc = 1.0 - f_pc;                  /* percent that is sound             */

   a_CI->f_Snd_DW3  = f_Load * f_pc * s_BWD.f_pc3_6;
   a_CI->f_Snd_DW6  = f_Load * f_pc * s_BWD.f_pc6_9;
   a_CI->f_Snd_DW9  = f_Load * f_pc * s_BWD.f_pc9_20;
   a_CI->f_Snd_DW20 = f_Load * f_pc * s_BWD.f_pc20;

   return 1;
}

 
/***************************************************************
*
**************************************************************/

void WD_FCCS_MessOff ()
{
   gb_FCCS = false;
}
void WD_FCCS_MessOn ()
{
   gb_FCCS = true; 
}
/************************************************************************
*
*  In: cr_CovCls....FCCS or FFI
*************************************************************************/
int  _FCC_OvridMes (float f_pc, float f_3Plus, char cr_CovCls[])
{
int i;
char  A[1000], cr[50];
String ^ Str;
String ^ s_Tlt;
DialogResult dr;

  if ( giS_FCCS > 0 )    /* Only show message once */
    return 1;
  if ( !stricmp (cr_CovCls, e_FCCS) )
    strcpy (cr,"FCCS");
  else if ( !stricmp (cr_CovCls, e_FFI) )
    strcpy (cr, "FFI"); 
  else {
    myMessageBox ("Logic Error - function _FCC_OvridMes ()"); 
    return 0; }

  sprintf (A, "For %s fuel types FOFEM normally uses a 'Fixed' log distribution\n",cr);

  strcpy  (gcr_WD,"  You have changed one of the following inputs.\n");
  strcat  (gcr_WD,"   >  3+ Fuel\n");
  strcat  (gcr_WD,"   >  Log Rotten\n");
  strcat  (gcr_WD,"   >  Log Loading Distribution\n");
 
  strcat  (gcr_WD,A); 
  strcat  (gcr_WD," and the 3+ fuel loading and rotten percentages for the fuel type.\n"); 
  strcat  (gcr_WD," However, you are allowed to modify them.\n\n");
  strcat  (gcr_WD," If you edit the fuel load or the rotten percentage then you CANNOT\n");
  strcat  (gcr_WD," use 'Fixed' for log distribution, because it is relevant only to\n");
  strcat  (gcr_WD," the default load and percentage. \n\n"); 
   
  strcat  (gcr_WD," If you wish to restore the default settings:\n");
  strcat  (gcr_WD,"  > Select a new cover type and then re-select the current cover type\n");
  strcat  (gcr_WD,"  > Right mouse click the 3+ fuel load textbox, select Typical\n");
  strcat  (gcr_WD,"  > Select the Log Distribution to 'Fixed'\n\n"); 

  strcat  (gcr_WD," This message will not be displayed again in this session.\n\n");
 
  sprintf (A, "NOTE --> %s 3+ Fuel Settings", cr);  /* Message Box Title */
  s_Tlt = _CharToStr(A); 
  Str = _CharToStr(gcr_WD); 
  dr = MessageBox::Show (Str,s_Tlt, MessageBoxButtons::OK);
 
  giS_FCCS++;  /* So message only shows once */
  return 0;
}
