/***********************************************************************
* Name: Wnd_FCCS.cpp
* Desc: Deal with the FCCS input fuel file
*
************************************************************************/
#ifdef UPDATED_OCT112012
FCCS Fuelbeds in FOFEM 6.0
FCCS fuelbed data was extracted from the most recent FCCS database and provided by Susan Pritchard 
Sept 2012. Fuelbeds 321-458 are new FCCS fuelbeds in FOFEM v6.0. (There were a number of higher 
numbered new FCCS fuelbeds in the files that Susan sent – series 500–1100 - but, based on her 
suggestion, they were not included in the FCCS file for FOFEM v6.0). 
Some fof_fccs.dat fuel loads were summed values from the FCCS database:
·	Litter load = litter load + lichen load + moss load
·	Shrub load = shrub primary load + shrub secondary load
·	Herb load = herb primary load + herb secondary load
·	Foliage load = 0.8 * (overstory load + midstory load + understory load + snag foliage load)
·	Branch load = 0.2 * (overstory load + midstory load + understory load + snag foliage load)
Some modifications were made to the FCCS data so all fuelbeds would work in FOFEM. 
Fuelbed	Change	Reason
088	Duff load set from 441.0 t/ac to 356.0 t/ac	Max duff load in BurnUp is 356.0 t/ac
168	Duff load set from 993.2 t/ac to 356.0 t/ac	Max duff load in BurnUp is 356.0 t/ac
272	Duff load set from 540.0 t/ac to 356.0 t/ac	Max duff load in BurnUp is 356.0 t/ac
323	Duff load set from 0.08 t/ac to 0.1 t/ac	Min duff load in BurnUp is 0.1 t/ac
203	Duff load was set from null to 5.0 t/ac	Set based on 10 t/ac/in
240	Duff load was set from null to 20.0 t/ac	Set based on 10 t/ac/in

In the FCCS database Susan sent the overstory, midstory and understory tree loading is assumed to 
be available crown fuel load, not total load (per. comm., Susan Pritchard) .The foliage and branch 
proportions (80% and 20%) were determined using Fig. 11 in Brown, JK, 1978. Weight and Density of 
Crowns in Rocky Mountain Conifers. USFS, RP- INT 197.
FCCS fuelbed loads for basal accumulation, needle drape, squirrel middens and ladder fuels are not
 part of any fuel component in fof_fccs.dat.

#endif 


#ifdef FCCS_NOTES
These notes go along with the fof_fccs.dat file that has contained in 
it the notes at the top of the file - sept 20 2012

FCCS Fuelbeds in FOFEM 6.0

FCCS fuelbed data was extracted from the most recent FCCS database and 
provided by Susan Pritchard Sept 2012. Fuelbeds 321-458 are new FCCS
fuelbeds in FOFEM v6.0. (There were a number of higher numbered new FCCS 
fuelbeds in the files that Susan sent – series 500–1100 - but, based on 
her suggestion, they were not included in the FCCS file for FOFEM v6.0). 
Some fof_fccs.dat fuel loads were summed values from the FCCS database:
·	Litter load = litter load + lichen load + moss load
·	Shrub load = shrub primary load + shrub secondary load
·	Herb load = herb primary load + herb secondary load
·	Foliage load = 0.8 * (overstory load + midstory load + understory load
 + snag foliage load)·	Branch load = 0.2 * (overstory load + midstory load + 
understory load + snag foliage load) When the FCCS fuelbeds were incorporated 
into FOFEM v6.0 the duff load for FCCS fuelbed 088 was set at 356.00 t/ac 
rather than at 441.00 t/ac as in the original FCCS data to avoid an error 
in the BurnUp code.
In the FCCS database Susan sent the overstory, midstory and understory tree 
loading is assumed to be available crown fuel load, not total load (per. comm., 
Susan Pritchard) .The foliage and branch proportions (80% and 20%) were 
determined using Fig. 11 in Brown, JK, 1978. Weight and Density of Crowns 
in Rocky Mountain Conifers. USFS, RP- INT 197.
FCCS fuelbed loads for basal accumulation, needle drape, squirrel middens 
and ladder fuels are not part of any fuel component in fof_fccs.dat.

#endif 


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

// #define e_FN_NVCS   "fof_nvcs.dat"

extern d_CVT sr_CVT[];                   /* All Cov Typs from disk file       */



/*------------------------------------------------------------*/
  
 char  cr_Reg1[100], cr_Reg2[100], cr_Reg3[100], cr_Reg4[200];

  char  cr_Name[1000];
  char  cr_F[100];
  float fcLitter;
  float fcDuff;
  float fcDepth;
  float fcShrub;
  float fcHerb;
  float fc1Hr;
  float fc10Hr;
  float fc100Hr;
  float fc3_9S;
  float fc9_20S;
  float fc20pS;
  float fc3_9R;
  float fc9_20R;
  float fc20pR;
  float fcFoli;
  float fcBran;
  char  cr_CTG[100];

  char   cr_cEmiGrpFla [100];
  char   cr_cEmiGrpDuf [100];
  char   cr_cEmiGrpS1k [100];


/* This table describes the format of the comma delimited input file */
d_CDF sr_FCC[] = {

   { "Reg1",    e_Str, 100, cr_Reg1 },
   { "Reg2",    e_Str, 100, cr_Reg2 },
   { "Reg3",    e_Str, 100, cr_Reg3 },
   { "Reg4",    e_Str, 100, cr_Reg4 },

   { "_Name",     e_Str, 1000,  cr_Name   },
   { "F",         e_Str, 100,   cr_F      },
   { "fcLitter",  e_Flo, 0,    &fcLitter  },
   { "fcDuff",    e_Flo, 0,     &fcDuff   },
   { "fcDepth",   e_Flo, 0,     &fcDepth  },
   { "fcShrub",   e_Flo, 0,     &fcShrub  },
   { "fcHerb",    e_Flo, 0,     &fcHerb   },
   { "fc1Hr",     e_Flo, 0,     &fc1Hr    },
   { "fc10Hr",    e_Flo, 0,     &fc10Hr   },
   { "fc100Hr",   e_Flo, 0,     &fc100Hr  },
   { "fc3_9S",    e_Flo, 0,     &fc3_9S   },
   { "fc9_20S",   e_Flo, 0,     &fc9_20S  },
   { "fc20pS",    e_Flo, 0,     &fc20pS   },
   { "fc3_9R",    e_Flo, 0,     &fc3_9R   },
   { "fc9_20R",   e_Flo, 0,     &fc9_20R  },
   { "fc20pR",    e_Flo, 0,     &fc20pR   },
   { "fcFoli",    e_Flo, 0,     &fcFoli   },
   { "fcBran",    e_Flo, 0,     &fcBran   },
   { "cr_CTG",    e_Str, 100,   cr_CTG    },
   { "cr_EmiGrpFla",   e_Str, 100, cr_cEmiGrpFla  },
   { "cr_EmiGrpDuf",   e_Str, 100, cr_cEmiGrpDuf  },
   { "cr_EmiGrpS1k",   e_Str, 100, cr_cEmiGrpS1k  },
   { "",  e_End, 5, "" }  };

float _chkset (float f);

/************************************************************************
* Name: FCC_LoadFileNew
* Desc: Read in the .cvs FCCS cover type load file
*       
* Note-1: There are four separate columns for region, any region code
*         could be in any column, 
*         examples:
*         P,I,,   means we have pacific and interior west
*         I,,,    means we have just interior west
*         
***********************************************************************/
int FCC_LoadFileNew (char cr_FN[], char cr_ErrMes[])
{
int i,j, iX;
float s,r,f;
char  cr[3000], A[100];

FILE *fh ;
d_CDF  *a;
    strcpy (cr_ErrMes,"");   

	a = &sr_FCC[0]; 

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

/* Regions - See Note-1 above */
      sr_CVT[iX].ir_Region[0] = -1;
      sr_CVT[iX].ir_Region[1] = -1;
      sr_CVT[iX].ir_Region[2] = -1;
      sr_CVT[iX].ir_Region[3] = -1;

	  strcpy (A,"");                   /* put a codes in one string */
	  strcat (A,cr_Reg1);             
	  strcat (A,cr_Reg2);
	  strcat (A,cr_Reg3);
	  strcat (A,cr_Reg4);
/* check for each region code, see Note-1 above */
      if ( strstr (A,"I") )          /* Interior West */
         sr_CVT[iX].ir_Region[0] = 1; 
      if ( strstr (A,"P") )          /* Pacific */
         sr_CVT[iX].ir_Region[1] = 2;
      if ( strstr (A,"N") )          /* Northeast */
         sr_CVT[iX].ir_Region[2] = 3;
      if ( strstr (A,"S") )          /* Southest */
         sr_CVT[iX].ir_Region[3] = 4;

      Trim_LT (cr_Name);
      strcpy (sr_CVT[iX].cr_CovTyp,cr_Name);     /* put in table          */

      sr_CVT[iX].s_CFL.f_litter = _chkset(fcLitter);     /* Litter */

      sr_CVT[iX].s_CFL.f_Shrub  = _chkset(fcShrub);
      strcpy (sr_CVT[iX].s_CFL.cr_Shrub,e_LA_NA);        /* Not Applicable code */

      sr_CVT[iX].s_CFL.f_Herb = _chkset(fcHerb);         /* Herb */
      strcpy (sr_CVT[iX].s_CFL.cr_Herb,e_LA_NA);         /* Not Applicable code */

      sr_CVT[iX].s_CFL.f_DW0 = _chkset (fc1Hr);          /* Dead Wood 1 hr      */

      sr_CVT[iX].s_CFL.f_DW10 = _chkset (fc10Hr);        /* Dead Wood 10 hr     */

      sr_CVT[iX].s_CFL.f_DW100 = _chkset (fc100Hr);      /* Dead Wood 100 hr    */

/* Coarse wood is 3-9, 9-20, 20+, Sound and Rotten                           */
      sr_CVT[iX].s_CFL.f_39s  = _chkset (fc3_9S);
      sr_CVT[iX].s_CFL.f_920s = _chkset (fc9_20S);
      sr_CVT[iX].s_CFL.f_20ps = _chkset (fc20pS);

      sr_CVT[iX].s_CFL.f_39r  = _chkset (fc3_9R);
      sr_CVT[iX].s_CFL.f_920r = _chkset (fc9_20R);
      sr_CVT[iX].s_CFL.f_20pr = _chkset (fc20pR);

      s = sr_CVT[iX].s_CFL.f_39s + sr_CVT[iX].s_CFL.f_920s + sr_CVT[iX].s_CFL.f_20ps;
      r = sr_CVT[iX].s_CFL.f_39r + sr_CVT[iX].s_CFL.f_920r + sr_CVT[iX].s_CFL.f_20pr;
      f = s + r;
      sr_CVT[iX].s_CFL.f_DW1000 = f;              /* Total all coarse wood   */
      if ( f == 0 )
         sr_CVT[iX].s_CFL.f_pcRot = 0;
      else
         sr_CVT[iX].s_CFL.f_pcRot = (r / f) * 100.0;  /* percent that is rotten  */

      sr_CVT[iX].s_CFL.f_Duff   = _chkset (fcDuff);
      sr_CVT[iX].s_CFL.f_DufDep = _chkset (fcDepth);

      strcpy (sr_CVT[iX].s_CFL.cr_HvCod, e_HD_NA);       /* Harvst Debis Cod not applic */

      sr_CVT[iX].s_CFL.f_CroFol = _chkset (fcFoli);     /* crown foliage  */
      strcpy (sr_CVT[iX].s_CFL.cr_CroFol,e_LA_NA);

      sr_CVT[iX].s_CFL.f_CroBra = _chkset (fcBran);     /* crown branch  */
      strcpy (sr_CVT[iX].s_CFL.cr_CroBra,e_LA_NA);

      sr_CVT[iX].s_CFL.f_MeaDia = 5;                     /* Hav no Wd3+ mean    */

      strcpy (sr_CVT[iX].s_CFL.cr_Comment, "");

      Trim_LT (cr_CTG);
      strcpy (sr_CVT[iX].cr_CovGrp,cr_CTG);

      strcpy (sr_CVT[iX].s_CFL.cr_EmiGrpFla,cr_cEmiGrpFla);
      strcpy (sr_CVT[iX].s_CFL.cr_EmiGrpDuf,cr_cEmiGrpDuf);
	  strcpy (sr_CVT[iX].s_CFL.cr_EmiGrpS1k,cr_cEmiGrpS1k); 

      iX++; 

	} /* for i */

	fclose (fh);

	return 1; 
 }

/********************************************************************
*  check for a missing value, if missing return 0
*******************************************************************/
float _chkset (float f)
{
  if ( f < 0 )
    return 0;
  return f;
}

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

    for ( i = 0; i < eC_CVT - 2; i++ ) {
      a = fgets (cr_Line, eC_fLine, fh );         /* Read a line from file   */
        if ( a == NULL ) {                        /* End of File             */
           fclose (fh);
           CVT_Dummy(iX);                     /* chk & put dummy reg rec */
           return 1; }
      if ( strchr (cr_Line,'\t') ) {
        strcpy (cr_ErrMes,"FCC_LoadFile - has tab error");
        return 0; }

      Blk_End_Line (cr_Line,eC_fLine);            /* Blank to end of line    */
      if ( isBlank (cr_Line) )                    /* if line is empty then   */
        continue;                                 /*  skip it                */

      if ( cr_Line[0] == '#' )                    /* lines is a comment so  */
        continue;                                 /*  skip it               */

      sr_CVT[iX].i_Num = 0;                  /* Cover Number as int    */

      ToStr (cr, cr_Line, 8, 99);            /* Name                         */
      Trim_LT (cr);
      strcpy (sr_CVT[iX].cr_CovTyp,cr);     /* put in table          */

      sr_CVT[iX].ir_Region[0] = -1;
      sr_CVT[iX].ir_Region[1] = -1;
      sr_CVT[iX].ir_Region[2] = -1;
      sr_CVT[iX].ir_Region[3] = -1;

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

      ToFlo (&sr_CVT[iX].s_CFL.f_litter,cr_Line, 102,107); /* Litter         */

      ToFlo (&sr_CVT[iX].s_CFL.f_Shrub, cr_Line, 123,128);/* Shrub               */
      strcpy (sr_CVT[iX].s_CFL.cr_Shrub,e_LA_NA);         /* Not Applicable code */

      ToFlo (&sr_CVT[iX].s_CFL.f_Herb,cr_Line, 130, 135);/* Herb                */
      strcpy (sr_CVT[iX].s_CFL.cr_Herb,e_LA_NA);         /* Not Applicable code */

      ToFlo (&sr_CVT[iX].s_CFL.f_DW0,   cr_Line,137,142); /* Dead Wood 1 hr      */

      ToFlo (&sr_CVT[iX].s_CFL.f_DW10,  cr_Line,144,159); /* Dead Wood 10 hr     */

      ToFlo (&sr_CVT[iX].s_CFL.f_DW100, cr_Line,151,156); /* Dead Wood 100 hr    */

/* Coarse wood is 3-9, 9-20, 20+, Sound and Rotten                           */
      ToFlo (&sr_CVT[iX].s_CFL.f_39s,  cr_Line, 158, 163);
      ToFlo (&sr_CVT[iX].s_CFL.f_920s, cr_Line, 165, 170);
      ToFlo (&sr_CVT[iX].s_CFL.f_20ps, cr_Line, 172, 177);

      ToFlo (&sr_CVT[iX].s_CFL.f_39r,  cr_Line, 179, 184);
      ToFlo (&sr_CVT[iX].s_CFL.f_920r, cr_Line, 186, 191);
      ToFlo (&sr_CVT[iX].s_CFL.f_20pr, cr_Line, 193, 198);

      s = sr_CVT[iX].s_CFL.f_39s + sr_CVT[iX].s_CFL.f_920s + sr_CVT[iX].s_CFL.f_20ps;
      r = sr_CVT[iX].s_CFL.f_39r + sr_CVT[iX].s_CFL.f_920r + sr_CVT[iX].s_CFL.f_20pr;
      f = s + r;
      sr_CVT[iX].s_CFL.f_DW1000 = f;              /* Total all coarse wood   */
      if ( f == 0 )
         sr_CVT[iX].s_CFL.f_pcRot = 0;
      else
         sr_CVT[iX].s_CFL.f_pcRot = (r / f) * 100.0;  /* percent that is rotten  */


      ToFlo (&sr_CVT[iX].s_CFL.f_Duff,  cr_Line, 109, 114); /* Duff Amt     */
      ToFlo (&sr_CVT[iX].s_CFL.f_DufDep,cr_Line, 116, 121); /* Duff Depth    */

      strcpy (sr_CVT[iX].s_CFL.cr_HvCod, e_HD_NA);       /* Harvst Debis Cod not applic */

//..................................................
/* change - 9-27-12 the new FCCS data DL gave me on 9-20-12 contains */
/*  foliage and branch fuel   */

//   sr_CVT[iX].s_CFL.f_CroFol = 0;                     /* Have No Crn Foliage */
     ToFlo (&sr_CVT[iX].s_CFL.f_CroFol,cr_Line, 200, 205); /* crown foliage  */
     strcpy (sr_CVT[iX].s_CFL.cr_CroFol,e_LA_NA);

//    sr_CVT[iX].s_CFL.f_CroBra = 0;                     /* Have No Crn Branch  */
      ToFlo (&sr_CVT[iX].s_CFL.f_CroBra,cr_Line, 207, 212); /* crown branch  */
      strcpy (sr_CVT[iX].s_CFL.cr_CroBra,e_LA_NA);

//...................................................................

      sr_CVT[iX].s_CFL.f_MeaDia = 5;                     /* Hav no Wd3+ mean    */

      strcpy (sr_CVT[iX].s_CFL.cr_Comment, "");

      ToStr (cr, cr_Line, 214, 216);              /* Cover Group             */
      Trim_LT (cr);
      strcpy (sr_CVT[iX].cr_CovGrp,cr);

      iX++;

    }   /* for i */
 
   fclose (fh);
    strcpy (cr_ErrMes,"ERROR - Serious - Internal Cover Type Array to Small");
    return 0;
}
