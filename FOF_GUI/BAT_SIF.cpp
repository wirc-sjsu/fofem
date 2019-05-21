/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: bat_sif.c
* Desc: Species input file
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
#ifdef ANSI
#define WINAPI
#else
#include <windows.h>
#endif


#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <float.h>
#include <signal.h>
#include <process.h>

#include "bat_sif.h"
#include "bat_misc.h"

#include "bat_mai.h"

#include "fof_ansi.h"

#include "fof_sgv.h"
#include "fof_sh.h"
#include "fof_ci.h"
#include "bat_bif.h"

#include "cdf_util.h"

extern char cr_ErrMes[];
char  gA[2000];
char  gB[2000];

FILE *fh_SIF;

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-   */
d_SIF gs_sif;

d_CDF  *a_CDF;

d_CDF  sr_Mcdf[] = {
     { "StandId",                e_Str,  eC_StdId,      gs_sif.cr_StdId },
     { "Tree Species",           e_Str,  eC_SIFmax,     gs_sif.cr_Spe   },
     { "Density",                e_Flo,          0,    &gs_sif.f_Den    },
     { "DBH",                    e_Flo,          0,    &gs_sif.f_DBH    },
     { "Height",                 e_Flo,          0,    &gs_sif.f_Hei    },
     { "Crown Ratio",            e_Flo,          0,    &gs_sif.f_CroRat },

//     { "Scorch Hgt - Flame Len", e_Flo,          0,    &gs_sif.f_Sch    },
     { "Scorch Hgt - Flame Len", e_Str,     eC_Sch,     gs_sif.cr_Sch  },

     { "Scorch Flame Code",      e_Str,      eC_FS,     gs_sif.cr_FS    },
     { "Fire Severity Code",     e_Str,  eC_FirSev,     gs_sif.cr_FirSev  },
     { "",            e_End,         0,        ""         }};

/* PINPON - Coastal Plain */
d_CDF  sr_PPCP[] = {
     { "StandId",                e_Str,  eC_StdId,      gs_sif.cr_StdId },
     { "Tree Species",           e_Str,  eC_SIFmax,     gs_sif.cr_Spe   },
     { "Density",                e_Flo,          0,    &gs_sif.f_Den    },
     { "DBH",                    e_Flo,          0,    &gs_sif.f_DBH    },
     { "Height",                 e_Flo,          0,    &gs_sif.f_Hei    },
     { "Crown Ratio",            e_Flo,          0,    &gs_sif.f_CroRat },
     { "Scorch Hgt - Flame Len", e_Flo,          0,    &gs_sif.f_Sch    },
     { "Scorch Flame Code",      e_Str,      eC_FS,     gs_sif.cr_FS    },
     { "Fire Severity Code",     e_Str,  eC_FirSev,     gs_sif.cr_FirSev  },
     { "Relative Humidity",      e_Flo,          0,    &gs_sif.f_RelHum  },
     { "",            e_End,         0,        ""         }};


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SIF_Next
* Desc: Get a Record from the file
*       There's 2 types of records, Prefire and PFI Postfire Injury with
*       different formats, see Notes in Bat_SIF.h where the d_SIF is
*       defined. 
* Note-1: When a mortality cutoff point is enter by the user we'll take 
*          and use it, if calculated mortality is >= the cutoff point 
*          the tree(s) will be considered dead, that is to say there
*          is 100 percent motality, less than the cutoff and tree are 
*          live 0 percent mortality.
*         When this field is left blank or a negative number is entered
*          the calculated mortality will be used, 
*  Out: a_SIF.....info from record
*  Ret: 1 ok record read, 0 End of File
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int SIF_Next (d_SIF *a_SIF, char cr_ErrMes[])
{
int i;
   strcpy (cr_ErrMes,"");
   SIF_Clean (&gs_sif);

// test larry
   i = CDF_Get (a_CDF, fh_SIF, cr_ErrMes);   /* Get Rec from comma delim fil */
   if ( i == 0 )                             /* End of file                  */
     return 0;
   if ( i == -1 )                            /* Input file isn't comma delim */
     return -1;                              /*  type, serious error         */

   strcpy (a_SIF->cr_StdId,gs_sif.cr_StdId);
   strcpy (a_SIF->cr_Spe,gs_sif.cr_Spe);
  
   if ( stricmp (gs_sif.cr_FirSev,e_PFI) ){  /* Regular - FOFEM Mort input */
     strcpy (a_SIF->cr_RecTyp,e_Reg);        /* Tell record type */
     a_SIF->f_Den = gs_sif.f_Den;           
     a_SIF->f_DBH = gs_sif.f_DBH;
     a_SIF->f_Hei = gs_sif.f_Hei; 
     a_SIF->f_CroRat = gs_sif.f_CroRat; 
     a_SIF->f_Sch = atof (gs_sif.cr_Sch); 
     strcpy (a_SIF->cr_FS,gs_sif.cr_FS);
     strcpy (a_SIF->cr_FirSev,gs_sif.cr_FirSev);  }

   else {  /* Post Fire Injury */
     strcpy (a_SIF->cr_RecTyp,e_PFI); 
     a_SIF->f_pDen = gs_sif.f_Den; 
     a_SIF->f_pCrnDam = gs_sif.f_DBH; 
     a_SIF->f_pDBH = gs_sif.f_Hei;
     a_SIF->f_pCKR = gs_sif.f_CroRat; 
     strcpy (a_SIF->cr_pBtlKil, gs_sif.cr_Sch); 
     if ( !strcmp (gs_sif.cr_FS,"") )    /* See Note-1 above */
       a_SIF->f_pPMCut = -1.0;           /* let's us know user didn't enter a cutoff */ 
     else 
       a_SIF->f_pPMCut = atof (gs_sif.cr_FS);
   } 

//   memcpy (a_SIF, &gs_sif, sizeof(d_SIF));   /* copy to callers structs      */

   return 1;
}

/*************************************************************
* Name: SIF_isPFI
* Desc: is the record a Post Fire Injury
*  Ret: 1  Yes it's a PFI
*       0  it's a regular FOFEM type mortality record 
**************************************************************/
int SIF_isPFI (d_SIF *a_SIF)
{
 if ( !stricmp (a_SIF->cr_RecTyp,e_PFI) )
   return 1; 
 return 0; 
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SIF_Close
* Desc: Close the file, whatelse can I say!
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void SIF_Close ()
{
  fclose (fh_SIF);
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SIF_Diplay
* Desc:
*   In: a_SIF......adr of SIF struct
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void SIF_Display (d_SIF *a_SIF)
{
   printf ("%s, %s, %5.1f, %4.1f, %4.1f, %4.1f, %4.1f, %s, %s, %4.1f \n",
   a_SIF->cr_StdId,
   a_SIF->cr_Spe,
   a_SIF->f_Den,
   a_SIF->f_DBH,
   a_SIF->f_Hei,
   a_SIF->f_CroRat,
   a_SIF->f_Sch,
   a_SIF->cr_FS,
   a_SIF->cr_FirSev,
   a_SIF->f_RelHum);

}
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SIF_Clean
* Desc: init all fields in SIF struct
*   In: a_SIF......adr of SIF struct
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void SIF_Clean (d_SIF *a_SIF)
{
   strcpy (a_SIF->cr_StdId,"");
   strcpy (a_SIF->cr_Spe,"");
   a_SIF->f_Den     = 0;
   a_SIF->f_DBH     = 0;
   a_SIF->f_Hei     = 0;
   a_SIF->f_CroRat  = 0;

   a_SIF->f_Sch     = 0;
   strcpy (a_SIF->cr_Sch,""); 

   strcpy (a_SIF->cr_FS,"");
   strcpy (a_SIF->cr_FirSev,"");
   a_SIF->f_RelHum  = 0;


   strcpy (a_SIF->cr_RecTyp,""); 

/* Post Fire Inputs */ 
   a_SIF->f_pDen = 0;         
   a_SIF->f_pCrnDam = 0;      
   a_SIF->f_pCKR = 0;         
   strcpy (a_SIF->cr_pBtlKil,""); 
   a_SIF->f_pPMCut = 0;       
   a_SIF->f_pDBH = 0;

}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SIF_Open
* Desc: Open the Species Input File
*   In: cr_FN...File Name
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int SIF_Open (char cr_FN[])
{
   fh_SIF = fopen (cr_FN,"r");
   if ( fh_SIF == NULL )
     return 0;
   return 1;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SIF_isPPCP
* Desc: See if the PINPON Coastal Plain switch is in the Mortaility Batch
*        input file
*  Ret: 1 = yes, 0 = no
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int SIF_isPPCP ()
{
int i_Ret;
char  *a;

  i_Ret = 0;

  while (1) {                              /* Look thru file for switch      */
    a = fgets (gA, 1000, fh_SIF);
    if ( a == NULL )                       /* End of File, no switch found   */
       break;                              /*  use default                   */

    if ( isBlankCR (gA) )               /* Empty Line                     */
       continue;


    sscanf (gA,"%s",gB);
    if ( !xstrcmpi (gB, e_IF_PPCP)){     /* Look for PINPAL Coastal Plan Swtch  */
      i_Ret = 1;                         /* Found it                         */
      break; }

  } /* while */

  fseek (fh_SIF,0L,SEEK_SET);                /* Set to begin of file         */

/* depending on switch found set proper table to be used for getting input   */
  if ( i_Ret == 0 )
    a_CDF = sr_Mcdf;
  else
    a_CDF = sr_PPCP;

  return i_Ret;
}
