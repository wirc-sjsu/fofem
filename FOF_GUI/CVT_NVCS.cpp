/***********************************************************************
* Name: Wnd_NVCS.cpp
* Desc: Deal with the NVCS input fuel file
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


extern d_CVT sr_CVT[];                   /* All Cov Typs from disk file       */
 
/***************************************************************************
* Name: NVCS_LoadFileNew
* Desc: Read in the new .csv NVCS cover file
*
***************************************************************************/
int NVCS_LoadFileNew (char cr_FN[], char cr_ErrMes[])
{
int i; 
   i = SAF_LoadFileNew (cr_FN, cr_ErrMes);

   return i; 
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: NVCS_LoadFile
* Desc: Read in NVCS Cover-Load File and save it in the CVT table
*   In: cr_Version....version number in head of Cover File
*       cr_FN.........Cover File Name
*  Ret: number of Cover Type records read in,   0 Error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int NVCS_LoadFile  (char cr_FN[], char cr_ErrMes[])
{
int i, iX;
FILE *fh ;
#define eC_line 300
char cr_Line [eC_line], *a, cr[200],  cr_CovTyp[200];
char cr_Pth[1000];
#define  eO 20

    strcpy (cr_ErrMes,""); 
 //   strcpy (cr_Pth,gcr_datDir);
 //    strcat (cr_Pth,cr_FN);

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
         fclose (fh);
         CVT_Dummy(iX);                /* insrt dumy reg rec           */
         return iX; }                      /* # of records loaded          */
      if ( strchr (cr_Line,'\t') ) {
        strcpy (cr_ErrMes,"NVCS_Load() - tab file error");
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

      ToStr (sr_CVT[iX].cr_CovTyp, cr_Line,23,122);
      Trim_CovTyp (sr_CVT[iX].cr_CovTyp);

      ToStr (sr_CVT[iX].cr_FueRef, cr_Line,123, 137);
      Trim_CovTyp (sr_CVT[iX].cr_FueRef);

      if ( !stricmp (sr_CVT[iX].cr_FueRef,"X"))  /* Skip recs with X       */
           continue;

      ToFlo (&sr_CVT[iX].s_CFL.f_litter, cr_Line, 138, 143 );
      ToFlo (&sr_CVT[iX].s_CFL.f_DW0,    cr_Line, 144, 149 );
      ToFlo (&sr_CVT[iX].s_CFL.f_DW10,   cr_Line, 150, 155 );
      ToFlo (&sr_CVT[iX].s_CFL.f_DW100,  cr_Line, 156, 161 );
      ToFlo (&sr_CVT[iX].s_CFL.f_DW1000, cr_Line, 162, 167 );

      sr_CVT[iX].s_CFL.f_pcRot = e_RotLogDefFlt;  /* Default Rotten, input file has none */

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

    fclose (fh);  /* shouldn't get here, we close above in for loop */
    strcpy (cr_ErrMes, "ERROR - Serious - Internal Cover Type Array to Small when load NVCS file");
    return 0;
}

/*************************************************************/
