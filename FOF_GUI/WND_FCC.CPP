/***********************************************************************
* Name: Wnd_FCC.cpp
* Desc: Deal with the FCC input fuel file
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
#include "Wnd_SAF.h"
#include "fof_fwf2.h"


extern d_CVT sr_SAF[];                   /* All Cov Typs from disk file       */

#ifdef eoeo
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: FCC_LoadFCC
* Desc: Read in the FCC File extract Cover info from it and load SAF/CVT
*         table
* CHANGE: Modified 8-8-08 to read in new version FCCS data
*
*   In: cr_Version....version number in head of Cover File
*       cr_FN.........File Name
*  Ret: 1 OK,  0 Error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int FCC_LoadFile  (char cr_FN[], char cr_ErrMes[])
{
int i, iX;
float s,r,f;
FILE *fh ;
#define eC_fLine 400
char cr_Line [eC_fLine], *a, cr[300];

    iX = 0;
    SAF_InitTable();                         /* Init the table               */
    fh =  fopen (cr_FN, "r" );
    if ( fh == NULL ) {
      sprintf (cr_ErrMes,"Can't Open Input Cover File: %s\n", cr_FN);
      strcat  (cr,"File must be in same directory as program");
      return 0;}

    for ( i = 0; i < eC_SAF - 2; i++ ) {
      a = fgets (cr_Line, eC_fLine, fh );         /* Read a line from file   */
        if ( a == NULL ) {                        /* End of File             */
           fclose (fh);
           CVT_FCC_Dummy(iX);                     /* chk & put dummy reg rec */
           return 1; }
      if ( strchr (cr_Line,'\t') ) {
        strcpy (cr_ErrMes,"FCC_LoadFile - has tab error");
        return 0; }

      Blk_End_Line (cr_Line,eC_fLine);            /* Blank to end of line    */
      if ( isBlank (cr_Line) )                    /* if line is empty then   */
        continue;                                 /*  skip it                */

      if ( cr_Line[0] == '#' )                    /* lines is a comment so  */
        continue;                                 /*  skip it               */

      sr_SAF[iX].i_Num = 0;                  /* Cover Number as int    */


      ToStr (cr, cr_Line, 8, 99);            /* Name                         */
      Trim_LT (cr);
      strcpy (sr_SAF[iX].cr_CovTyp,cr);     /* put in table          */


      sr_SAF[iX].ir_Region[0] = -1;
      sr_SAF[iX].ir_Region[1] = -1;
      sr_SAF[iX].ir_Region[2] = -1;
      sr_SAF[iX].ir_Region[3] = -1;

      ToStr (cr, cr_Line, 2, 5);
      if ( strstr (cr,"I") )  sr_SAF[iX].ir_Region[0] = 1; /* Inter West */
      if ( strstr (cr,"P") )  sr_SAF[iX].ir_Region[1] = 2; /* Pacific West */
      if ( strstr (cr,"N") )  sr_SAF[iX].ir_Region[2] = 3; /* Northeast    */
      if ( strstr (cr,"S") )  sr_SAF[iX].ir_Region[3] = 4; /* Southeast      */

/* Fuel Cat - for now I'm setting FCC up to use the Fuel Cat Dropdown Box    */
//      ToStr (sr_SAF[iX].cr_FCCFuelCat, cr_Line, 100, 101);
//      Trim_LT (sr_SAF[iX].cr_FCCFuelCat);
//      if ( !_ChkFCCFueCat (sr_SAF[iX].cr_FCCFuelCat) ) {
//        fclose (fh);
//        return 0; }

      ToFlo (&sr_SAF[iX].s_CFL.f_litter,cr_Line, 102,107); /* Litter         */

      ToFlo (&sr_SAF[iX].s_CFL.f_Shrub, cr_Line, 123,128);/* Shrub               */
      strcpy (sr_SAF[iX].s_CFL.cr_Shrub,e_LA_NA);         /* Not Applicable code */

      ToFlo (&sr_SAF[iX].s_CFL.f_Herb,cr_Line, 130, 135);/* Herb                */
      strcpy (sr_SAF[iX].s_CFL.cr_Herb,e_LA_NA);         /* Not Applicable code */

      ToFlo (&sr_SAF[iX].s_CFL.f_DW0,   cr_Line,137,142); /* Dead Wood 1 hr      */

      ToFlo (&sr_SAF[iX].s_CFL.f_DW10,  cr_Line,144,159); /* Dead Wood 10 hr     */

      ToFlo (&sr_SAF[iX].s_CFL.f_DW100, cr_Line,151,156); /* Dead Wood 100 hr    */

/* Coarse wood is 3-9, 9-20, 20+, Sound and Rotten                           */
      ToFlo (&sr_SAF[iX].s_CFL.f_39s,  cr_Line, 158, 163);
      ToFlo (&sr_SAF[iX].s_CFL.f_920s, cr_Line, 165, 170);
      ToFlo (&sr_SAF[iX].s_CFL.f_20ps, cr_Line, 172, 177);

      ToFlo (&sr_SAF[iX].s_CFL.f_39r,  cr_Line, 179, 184);
      ToFlo (&sr_SAF[iX].s_CFL.f_920r, cr_Line, 186, 191);
      ToFlo (&sr_SAF[iX].s_CFL.f_20pr, cr_Line, 193, 198);

      s = sr_SAF[iX].s_CFL.f_39s + sr_SAF[iX].s_CFL.f_920s + sr_SAF[iX].s_CFL.f_20ps;
      r = sr_SAF[iX].s_CFL.f_39r + sr_SAF[iX].s_CFL.f_920r + sr_SAF[iX].s_CFL.f_20pr;
      f = s + r;
      sr_SAF[iX].s_CFL.f_DW1000 = f;              /* Total all coarse wood   */
      if ( f == 0 )
         sr_SAF[iX].s_CFL.f_pcRot = 0;
      else
         sr_SAF[iX].s_CFL.f_pcRot = (r / f) * 100.0;  /* percent that is rotten  */


      ToFlo (&sr_SAF[iX].s_CFL.f_Duff,  cr_Line, 109, 114); /* Duff Amt     */
      ToFlo (&sr_SAF[iX].s_CFL.f_DufDep,cr_Line, 116, 121); /* Duff Depth    */

      strcpy (sr_SAF[iX].s_CFL.cr_HvCod, e_HD_NA);       /* Harvst Debis Cod not applic */

      sr_SAF[iX].s_CFL.f_CroFol = 0;                     /* Have No Crn Foliage */
      strcpy (sr_SAF[iX].s_CFL.cr_CroFol,e_LA_NA);

      sr_SAF[iX].s_CFL.f_CroBra = 0;                     /* Have No Crn Branch  */
      strcpy (sr_SAF[iX].s_CFL.cr_CroBra,e_LA_NA);

      sr_SAF[iX].s_CFL.f_MeaDia = 5;                     /* Hav no Wd3+ mean    */

      strcpy (sr_SAF[iX].s_CFL.cr_Comment, "");

      ToStr (cr, cr_Line, 200, 202);              /* Cover Group             */
      Trim_LT (cr);
      strcpy (sr_SAF[iX].cr_CovGrp,cr);

      iX++;

    }   /* for i */
 
   fclose (fh);
    strcpy (cr_ErrMes,"ERROR - Serious - Internal Cover Type Array to Small");
    return 0;
}
#endif 
