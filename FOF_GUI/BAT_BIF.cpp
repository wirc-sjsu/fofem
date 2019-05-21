
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: bat_bif.c
* Desc: Batch Input File functions
*
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
#ifdef ANSI
#define WINAPI
#else
#include <windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <float.h>

FILE *fh_BIF;

#include  "fof_sgv.h"
#include "fof_sh.h"
#include "fof_ci.h"
#include "bat_mai.h"
#include "bat_bif.h"
#include "cdf_util.h"
#include "bat_bif2.h"
#include "bat_misc.h"
#include "fof_util.h"
#include "fof_ansi.h"
#include "fof_gen.h"


#define  e_Delim ","           /* Delimiter in input file                    */

#define eC_IS  1000
char  cr_IS[eC_IS], A[eC_IS], B[eC_IS];

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: BIF_Get
* Desc: Read a single record (line) from the input file and load up
*        CI and SI structs.
*       The input data can be of 4 types.
*         1. Regular, 1000 hour fuels in total
*         2. like above but with Soil data on end
*         3. 1000 hour fuels in size classes in sound and rotten
*         4. like 3. with soil data on end
*        The caller determines input type from switches found in the
*         top of the input file, see params below
* Note-2: The input file can have the 1000 hour fuel in either of 2 formats
*        one format has total, percent rotten and skew, the other format
*        has 8 amounts, 4 size class in sound and rotten amounts, in this
*        size class format there is no need for a skew or a percent rotten
*        as the loads are already distribute as needed.
* Note-1: Need to init the global structs here, they will get init with
*         some values that Burnup and Soil will need, need to init these
*         globals becasue after the CDF_Get() function is called they then
*         get copied over onto the callers CI and SI, so we can really init
*         the callers CI or SI directly.
*   In: iS_Soil... 1 = there is soil data in file, so read it in
*       cr_1kFmt.. e_IF_1kSC = means the 1000 Hour fuel is formated as
*                   a individual size classes instead of standared total way
*  Out: a_CI......Loaded with inputs from file, see Note-1 above
*       a_SI......Loaded with inputs from file
*       cr_StdId...Stand Id, first field of the input file, identifies record
*       cr_ErrMes.."" or Error Message
*
* NOTE: You can check for an Error using cr_ErrMes or -1 return value
* Ret: 1 OK, NOTE - still can have a an error need to check cr_ErrMes to
*         see if it's NULL or has a message in it
*      0 End of File
*     -1 Serious Error, input file is not correctly formated, doesn't appear
*         to be comma delimited correctly
*        NOTE all other errors you need to check cr_ErrMes, read above
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int BIF_Get (d_CI *a_CI, d_SI *a_SI, char cr_StdId[], char cr_ErrMes[],
             int iS_Soil, char cr_1kFmt[])
{
int i;
d_CDF *a_CDF;

    strcpy (cr_ErrMes,"");

/* Determine the type of Batch input file and set approprate CDF table       */
    if ( !strcmp (cr_1kFmt, "")){            /* Normal input                 */
      if ( iS_Soil == 0 )
        a_CDF = sr_cdr;
      else
        a_CDF = sr_cdrS; }

    else if ( !strcmp(cr_1kFmt,e_IF_1kSC)){   /* 1000 Hour Snd/Rot typ input  */
      if ( iS_Soil == 0 )
        a_CDF = sr_cdrSR;
      else
        a_CDF = sr_cdrSRS; }

    else if ( !strcmp(cr_1kFmt,e_IF_CPLP))    /* Coast Plain longleaf pine   */
      a_CDF = sr_cdrCP;

    else {
      ErrBat_Window ("Logic Error", "BIF_Get()");
      exit (1); }
 
/*...........................................................................*/
/* Init global struct, read in input and copy to callers structs             */
    CI_Init (&gs_CI);                        /* See Note-1 above             */
    SI_Init (&gs_SI);

   i = CDF_Get (a_CDF, fh_BIF, cr_ErrMes);   /* Get Rec from comma delim fil */
   if ( i == 0 )                             /* End of file                  */
     return 0;
   if ( i == -1 )                            /* Input file isn't comma delim */
     return -1;                              /*  type, serious error         */

   strcpy (cr_StdId, gcr_StdId);
   memcpy (a_CI, &gs_CI, sizeof (d_CI) );    /* copy to callers structs      */
   memcpy (a_SI, &gs_SI, sizeof (d_SI) );
   return 1;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: BIF_Switch
* Desc: Look for the 1000 hour or the Coastal Plain input file switches
*       NOTE: only one or none is valid
*     > Coastal Plain - input file is a standard format except with the
*        inclusion of litter moisture. See notes in bat_bif2.h
*
*     > 1000 Hour Format size class switch in batch input file.
*        When present it means the the input file is formated with
*        1000 hr broken into size classes, sound and rotten
*
*  Out: cr_Fmt....."" means no size class switch found, input is standard
*                  e_IF_1kSC = size class switch found in file
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int BIF_Switch (char cr_Fmt[], int iS_Soil, char cr_ErrMes[])
{
int iN;
char  *a;

  strcpy (cr_ErrMes,"");
  strcpy (cr_Fmt,"");
  iN = 0;

  while (1) {                              /* Look thru file for switch      */
    a = fgets (cr_IS, 1000, fh_BIF);
    if ( a == NULL )                       /* End of File, no switch found   */
       break;                              /*  use default                   */

    if ( isBlankCR (cr_IS) )               /* Empty Line                     */
       continue;

    sscanf (cr_IS,"%s",A);

    if ( !xstrcmpi (A, e_IF_1kSC)){    /* Look for 1k size class switch  */
      strcpy (cr_Fmt,e_IF_1kSC);
      iN++; }

    if ( !xstrcmpi (A, e_IF_CPLP)){    /* Look for Coast Plina longleaf  */
      strcpy (cr_Fmt,e_IF_CPLP);
      iN++; }
    if ( iN == 2 ) {
      sprintf (cr_ErrMes, "Invalid input file switch setting, please use only one, %s or %s", e_IF_1kSC, e_IF_CPLP);
      return 0; }

  } /* while */


// test-lf larry
/* Coast Plain switch isn't valid for soil input files................       */
  if ( !xstrcmpi (cr_Fmt,e_IF_CPLP) &&
       iS_Soil == 1 ) {
     sprintf (cr_ErrMes, "Invalid file switch (%s) used in Soil input file", e_IF_CPLP);
     return 0;  }

  fseek (fh_BIF,0L,SEEK_SET);                /* Set to begin of file         */
  return 1;
}


/****************************************************************************
* Name: BIF_Switch2
* Desc: Look for an Emission output file and and a 
*       Burnup input file switch if they exist in the batch
*       file. 
****************************************************************************/
int BIF_Switch2 (char cr_BurnUpFN[], char cr_EmisSw[], char cr_ErrMes[])
{
char  *a;

  strcpy (cr_ErrMes,"");
  strcpy (cr_BurnUpFN,"");
  strcpy (cr_EmisSw,"");

  while (1) {                              /* Look thru file for switch      */
    a = fgets (cr_IS, 1000, fh_BIF);
    if ( a == NULL )                       /* End of File, no switch found   */
       break;                              /*  use default                   */

    if ( isBlankCR (cr_IS) )               /* Empty Line                     */
       continue;

    strcpy (A,"");
    strcpy (B,"");
    sscanf (cr_IS,"%s %s",A,B);

    if ( !xstrcmpi (A, e_IF_EmisFile)){   /* Look for Emission  switch  */
      if ( !strcmp (B,"") ){
       sprintf (cr_ErrMes,"Invalid batch input file switch %s - no file name prefix specified",e_IF_EmisFile);
       return 0; }
    strcpy (cr_EmisSw,B); }

    if ( !xstrcmpi (A, e_IF_BurnUpFN)){    /* Look for Burnup file name switch*/
      if ( !strcmp (B,"") ){
       sprintf (cr_ErrMes,"Invalid batch input file switch %s - no file name specified",e_IF_BurnUpFN);
       return 0; }
      strcpy (cr_BurnUpFN,B);  }

  } /* while */

  fseek (fh_BIF,0L,SEEK_SET);                /* Set to begin of file         */
  return 1;
}

/****************************************************************************
* Name: BIF_Switch_UsrEqu
* Desc: Find the User Equation switch in the file.  
*       If switch isn't found then we need to take the entries
*       off the end of the comma delmited file description tables that
*       cause the equation number columns to be read in. 
*       We do this for all the description tables even though only the
*        selected/approbriate one will get used, it's just easy 
*        to do it to them all than figure out which one is selected.
*       See notes where the tables are declared, sr_cdr[], etc.
* Note-1: Only one of these gets used - depending on the type of input
*         but it's easiest to just reset them all 
* Ret:  1 switch found
*       0 switch not found  
****************************************************************************/

void BIG_ResetUEN (d_CDF *a);

int BIF_Switch_UsrEqu ()
{
int i, i_Ret;
char  *a;

/* Reset the e_EqLitter entry in the batch input file CDF tables */
/* see the sr_cdr[] (and related) defines for detail comments */ 
  BIG_ResetUEN (sr_cdr);            /* see Note-1 above */
  BIG_ResetUEN (sr_cdrS);
  BIG_ResetUEN (sr_cdrSR);
  BIG_ResetUEN (sr_cdrSRS);


  fseek (fh_BIF,0L,SEEK_SET);                /* Set to begin of file         */
  i_Ret = 0; 
  while (1) {                              /* Look thru file for switch      */
    a = fgets (cr_IS, 1000, fh_BIF);
    if ( a == NULL )                       /* End of File, no switch found   */
       break;                           

    if ( isBlankCR (cr_IS) )               /* Empty Line                     */
       continue;

    strcpy (A,"");
    sscanf (cr_IS,"%s %s",A,B);

/* if the User Equation Switch is found, leave the CDF tables alone */
/*  they are set to read in the equations numbers from the file */
    if ( !xstrcmpi (A, e_IF_UseEqu) ) {
      i_Ret = 1;                        /* Switch found, beat it */   
      goto X; }

  } /* while */


/* Switch Not found, adjust CDF tables - see Notes above */
/*  Consumed table */
  for ( i = 0; i < 1000; i++ ) {  
    if ( !stricmp (sr_cdr[i].cr_Name, e_EqLitter) ) {
      strcpy (sr_cdr[i].cr_Name,"");
      sr_cdr[i].c_Type = e_End; 
//      sr_cdr[i].adr = gcr_End;
      break;   } }

/* Soil table  */
  for ( i = 0; i < 1000; i++ ) {  
    if ( !stricmp (sr_cdrS[i].cr_Name,e_EqLitter ) ) {
      strcpy (sr_cdrS[i].cr_Name,"");
      sr_cdrS[i].c_Type = e_End; 
      sr_cdrS[i].adr = gcr_End;
      break;   } }

/* Consumed - 1000 Hour Sound Rotten table  */
  for ( i = 0; i < 1000; i++ ) {  
    if ( !stricmp (sr_cdrSR[i].cr_Name, e_EqLitter) ) {
      strcpy (sr_cdrSR[i].cr_Name,"");
      sr_cdrSR[i].c_Type = e_End; 
      sr_cdrSR[i].adr = gcr_End;
      break;   } }

/* Soil - 1000 Hour Sound Rotten table  */
  for ( i = 0; i < 1000; i++ ) {  
    if ( !stricmp (sr_cdrSRS[i].cr_Name, e_EqLitter) ) {
      strcpy (sr_cdrSRS[i].cr_Name,"");
      sr_cdrSRS[i].c_Type = e_End; 
      sr_cdrSRS[i].adr = gcr_End;
      break;   } }


X:
  fseek (fh_BIF,0L,SEEK_SET);                /* Set to begin of file         */
  return i_Ret;
}

/**************************************************************
* Name: BIG_ResetUEN
* Desc: Reset the comma delimited tables that are used to
*       describe and read in the batch input files. 
*       We need to check and make sure the Litter Equation 
*       Number description row is in the table, it may have
*       been set to "" on a previous run, indicating that the
*       user is not using User Equations Numbers in their 
*       batch input file
**************************************************************/
void BIG_ResetUEN (d_CDF *a)
{
int i; 

  for ( i = 0; i < 1000; i++ ) {  
    if ( !stricmp(a->cr_Name, e_EqLitter))  
 //   if ( !stricmp (sr_cdr[i].cr_Name, e_EqLitter) ) 
       break; 
   if ( !stricmp (a->cr_Name, "") ) {
      strcpy (a->cr_Name,e_EqLitter);
      a->c_Type = e_Int; 
      break;   } 
   a++; 
   } /* for i */ 

}


//99999999999999999999999999999999999999999999999999999999999999999999999999


/***************************************************************************
* Name: BIF_SwitchEG
* Desc: Look for and read in the Emission Group Factor Numbers Switches
*       The switches are optional. 
*       There are 3 switches, none, 1,2 or 3 could be there, if a switch
*        is found it needs to have an argument, the argument will get 
*        checked later. 
*  Out: cr_Flame, cr_Smold, cr_Duff.
*       cr_Full..."Full", says users wants all 204 expaned emis outputed 
****************************************************************************/
int BIF_SwitchEG (char cr_Flame[], char cr_Smold[], char cr_Duff[], char cr_Full[], char cr_ErrMes[])
{
int iN; 
char  *a;
  strcpy (cr_ErrMes,"");
  strcpy (cr_Flame,"");
  strcpy (cr_Smold,"");
  strcpy (cr_Duff,"");
  strcpy (cr_Full,"");  
  iN = 0; 

  while (1) {                              /* Look thru file for switch      */
    a = fgets (cr_IS, 1000, fh_BIF);
    if ( a == NULL )                       /* End of File, no switch found   */
       break;                              /*  use default                   */

    if ( isBlankCR (cr_IS) )               /* Empty Line                     */
       continue;

    strcpy (A,"");
    strcpy (B,"");
    sscanf (cr_IS,"%s %s",A,B);

    if ( !xstrcmpi (A, e_IF_Flame)){   /* Look for Emission  switch  */
      if ( !strcmp (B,"") ){
         sprintf (cr_ErrMes, "Input File Switch %s missing argument", e_IF_Flame);
         return 0; }
      strcpy (cr_Flame,B);
      iN++; }

    if ( !xstrcmpi (A, e_IF_Smold)){   /* Look for Emission  switch  */
      if ( !strcmp (B,"") ){
         sprintf (cr_ErrMes, "Input File Switch %s missing argument", e_IF_Smold);
         return 0; }
      strcpy (cr_Smold,B); 
      iN++; }

    if ( !xstrcmpi (A, e_IF_Duff)){   /* Look for Emission  switch  */
      if ( !strcmp (B,"") ){
         sprintf (cr_ErrMes, "Input File Switch %s missing argument", e_IF_Duff);
         return 0; }
      strcpy (cr_Duff,B); 
      iN++; }

    if ( !xstrcmpi (A, e_IF_Full))
      strcpy (cr_Full, "Full");



  } /* while */

  if ( iN != 0 && iN != 3 ) {
    sprintf (cr_ErrMes, "Input File has a missing or invalid Expanded Emission Factor switch" );
    strcpy (cr_Full,""); 
    return 0; }

  if ( iN == 0 )                /* Won't be doing Expanded Emis */
    strcpy (cr_Full,"");        /* Make sure this is blank */

  fseek (fh_BIF,0L,SEEK_SET);                /* Set to begin of file         */
  return 1;
}

//int _Chk_EG (char cr_Switch[], char cr_Arg[], char cr_ErrMes[])
//{
//  return 1; 
//}


/****************************************************************************
****************************************************************************
****************************************************************************
***************************************************************************/

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: BIF_Open   BIF_Close
* Desc: Open/Close batch input file
* Ret: see below
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int BIF_Open (char cr_InFN[])
{
   fh_BIF = fopen (cr_InFN,"r");
   if ( fh_BIF == NULL )
     return 0;
   return 1;
}
void  BIF_Close  ()
{
  if ( fh_BIF != NULL )
    fclose (fh_BIF);
}


