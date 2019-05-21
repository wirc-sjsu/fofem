/***********************************************************************
* Name: Wnd_FLM.cpp
* Desc: Deal with the FLM input fuel file
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


extern d_CVT sr_CVT[];                   /* All Cov Typs from disk file       */
 
char cr_flmReg[100];
char cr_flmCode[100];
char cr_flmName[500];

float f_flmLit;
float f_flmDuf;
float f_flmDep;
float f_flm1Hr;
float f_flm10Hr;
float f_flm100Hr;
float f_flmLogs;
float f_flmHerb;
float f_flmShrub;
float f_flmFol;
float f_flmBra;
float f_flmRot;
char  cr_flmLogDis[100];
char  cr_flmSoil[100];
float f_flmLFlame;
float f_flmDuff;
float f_flmCoarse;

char  cr_flmEmiGrpFla[100];
char  cr_flmEmiGrpDuf[100];
char  cr_flmEmiGrpS1k[100];


d_CDF sr_FLM[] = {
   { "cr_flmReg",       e_Str, 100,   cr_flmReg  },
   { "cr_flmCode",      e_Str, 100,   cr_flmCode  },
   { "cr_flmName",      e_Str, 500,   cr_flmName  },
   { "f_flmLit",        e_Flo,   0,   &f_flmLit    },
   { "f_flmDuf",        e_Flo,   0,   &f_flmDuf    },
   { "f_flmDep",        e_Flo,   0,   &f_flmDep    },
   { "f_flm1Hr",        e_Flo,   0,   &f_flm1Hr    },
   { "f_flm10Hr",       e_Flo,   0,   &f_flm10Hr   },
   { "f_flm100Hr",      e_Flo,   0,   &f_flm100Hr  },
   { "f_flmLogs",       e_Flo,   0,   &f_flmLogs   },
   { "f_flmHerb",       e_Flo,   0,   &f_flmHerb   },
   { "f_flmShrub",      e_Flo,   0,   &f_flmShrub  },
   { "f_flmFol",        e_Flo,   0,   &f_flmFol    },
   { "f_flmBra",        e_Flo,   0,   &f_flmBra    },
   { "f_flmRot",        e_Flo,   0,   &f_flmRot    },
   { "cr_flmLogDis",    e_Str, 100,   cr_flmLogDis},
   { "cr_flmSoil",      e_Str, 100,   cr_flmSoil  },
//   { "f_flmLFlame",     e_Flo,   0,   &f_flmLFlame },
//   { "f_flmDuff",       e_Flo,   0,   &f_flmDuff   },
//   { "f_flmCoarse",     e_Flo,   0,   &f_flmCoarse },

   { "cr_EmiGrpFla",   e_Str, 100, cr_flmEmiGrpFla  },
   { "cr_EmiGrpDuf",   e_Str, 100, cr_flmEmiGrpDuf  },
   { "cr_EmiGrpS1k",   e_Str, 100, cr_flmEmiGrpS1k  },

   { "",  e_End, 5, "" }  };

float _chkMiss (float f);


/**************************************************************
*
*
*
*
**************************************************************/
int Wnd_Mai::FLM_LoadFileNew (char cr_FN[], char cr_ErrMes[])
{

int i,j, iX;
char  cr[3000];
FILE *fh ;
d_CDF  *a;

    strcpy (cr_ErrMes,"");   

	a = &sr_FLM[0]; 

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

      if ( strstr (cr_flmReg,"I") )  sr_CVT[iX].ir_Region[0] = 1; /* Inter West */
      if ( strstr (cr_flmReg,"P") )  sr_CVT[iX].ir_Region[1] = 2; /* Pacific West */
      if ( strstr (cr_flmReg,"N") )  sr_CVT[iX].ir_Region[2] = 3; /* Northeast    */
      if ( strstr (cr_flmReg,"S") )  sr_CVT[iX].ir_Region[3] = 4; /* Southeast      */

      sprintf (cr, "%s %s",cr_flmCode, cr_flmName);
      strcpy (sr_CVT[iX].cr_CovTyp,cr);

      sr_CVT[iX].s_CFL.f_litter  = _chkMiss (f_flmLit);
      sr_CVT[iX].s_CFL.f_Duff    = _chkMiss (f_flmDuf);
      sr_CVT[iX].s_CFL.f_DufDep  = _chkMiss (f_flmDep);
      sr_CVT[iX].s_CFL.f_DW0     = _chkMiss (f_flm1Hr);
      sr_CVT[iX].s_CFL.f_DW10    = _chkMiss (f_flm10Hr);
      sr_CVT[iX].s_CFL.f_DW100   = _chkMiss (f_flm100Hr);
      sr_CVT[iX].s_CFL.f_DW1000  = _chkMiss (f_flmLogs);
      sr_CVT[iX].s_CFL.f_Herb    = _chkMiss (f_flmHerb);
      sr_CVT[iX].s_CFL.f_Shrub   = _chkMiss (f_flmShrub);
      sr_CVT[iX].s_CFL.f_CroFol  = _chkMiss (f_flmFol);
      sr_CVT[iX].s_CFL.f_CroBra  = _chkMiss (f_flmBra);
      sr_CVT[iX].s_CFL.f_pcRot   = _chkMiss (f_flmRot);

/* Log Dist and Soil Type text must be same as Text used in drop down boxes  */
      strcpy (sr_CVT[iX].s_CFL.cr_FLMLogDist, cr_flmLogDis);
      Trim_LT (sr_CVT[iX].s_CFL.cr_FLMLogDist);

      strcpy (sr_CVT[iX].s_CFL.cr_FLMSoil,  cr_flmSoil);
      Trim_LT (sr_CVT[iX].s_CFL.cr_FLMSoil);

/* Emission Group Number */
      Trim_LT (cr_flmEmiGrpFla);
      strcpy (sr_CVT[iX].s_CFL.cr_EmiGrpFla,cr_flmEmiGrpFla);

      Trim_LT (cr_flmEmiGrpDuf);
      strcpy (sr_CVT[iX].s_CFL.cr_EmiGrpDuf,cr_flmEmiGrpDuf);

      Trim_LT (cr_flmEmiGrpS1k);
      strcpy (sr_CVT[iX].s_CFL.cr_EmiGrpS1k,cr_flmEmiGrpS1k);
	  

      iX++;

    }


  return 1;
}

/*********************************************************
* If missing value ( -1 ) return 0;
*********************************************************/
float _chkMiss (float f)
{
  if ( f < 0 )
    return 0;
  return f;
}



// ****************************************************
#ifdef wowowowo
      i_Num, cr_Line,  9, 11);    /* Cover Number as int    */

      sr_CVT[iX].ir_Region[0] = -1;
      sr_CVT[iX].ir_Region[1] = -1;
      sr_CVT[iX].ir_Region[2] = -1;
      sr_CVT[iX].ir_Region[3] = -1;
     
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


#endif 



/******************************************************************
* Name: FLM_Load
*
******************************************************************/


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: FLM_LoadFile
* Desc: Read in the FLM Cover-Load File and save it in the CVT table
*   In: cr_Version....version number in head of Cover File
*       cr_FN.........Cover File Name
*  Ret: number of Cover Type records read in,   0 Error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int Wnd_Mai::FLM_LoadFile  (char cr_FN[], char cr_ErrMes[])
{
int i, iX;
FILE *fh ;
#define eC_line 300
char cr_Line [eC_line], *a, cr[200],  cr_CovTyp[200];
#define  eO 20

 FLM_LoadFileNew (cr_FN,cr_ErrMes);
 return 0; 

    iX = 0;
    SAF_InitTable();                         /* Init the table               */
    fh =  fopen (cr_FN, "r" );
    if ( fh == NULL ) {
      sprintf (cr_ErrMes,"Can't Open Input Cover File: %s\n", cr_FN);
      strcat  (cr_ErrMes,"File must be in same directory as program");
      return 0;}

    for ( i = 0; i < eC_CVT - 2; i++ ) {
      a = fgets (cr_Line, eC_line, fh );     /* Read a line from file        */
        if ( a == NULL ) {                   /* End of File                  */
          break; }                      /* # of records loaded          */
      if ( strchr (cr_Line,'\t') ) {
        strcpy (cr_ErrMes, "Tab_Error loading FLM cover file");
        return 0; }

      Blk_End_Line (cr_Line,eC_line);              /* Blank to end of line   */
      if ( isBlank (cr_Line) )                     /* if line is empty then  */
        continue;                                  /*  skip it               */

      if ( cr_Line[0] == '#' )                     /* lines is a comment so  */
        continue;                                  /*  skip it               */

      ToInt (&sr_CVT[iX].i_Num, cr_Line,  9, 11);    /* Cover Number as int    */

/* Regions, don't care about order just if Letter is sound in string         */
      sr_CVT[iX].ir_Region[0] = -1;
      sr_CVT[iX].ir_Region[1] = -1;
      sr_CVT[iX].ir_Region[2] = -1;
      sr_CVT[iX].ir_Region[3] = -1;
     
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
    CVT_Dummy(iX);                /* insrt dumy reg rec           */
     
    return 1;

}
