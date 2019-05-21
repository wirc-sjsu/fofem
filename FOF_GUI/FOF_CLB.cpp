/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name:  fof_clb.c    Command Line Batch  Program
* Desc:  This is an option that gets run when there are arguments
*         detected on the command line, this is the first thing that
*         gets checked on program  start up at the top of WinMain()
*        This allows users to run FOFEM batch files with out using the
*         user interface.
*        See notes below for details
* Date:  05-16-07
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
//#include <windows.h>
//#include <windowsx.h>
//#include <winuser.h>
//#include <winbase.h>
//#include <shellapi.h>

//#include <commctrl.h>
//#include <process.h>
#include <stdlib.h>
#include <direct.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <ctype.h>
// #include <ctl3d.h>
#include <float.h>


#include "Wnd_Mai.h" 
#include "Wnd_Util.h"


// #include "fc_clb.h"
#include "Bat_mai.h"
#include "fof_util.h"

#include "fof_nes.h" 

#define e_BSC '+'   /* use to fill blanks in fil names */

#define e_PrgNam "fof_gui"   /* use as program name in popup help wnd             */

#define e_ModCon    "C"                 /* Consumed Emissions               */
#define e_ModSoi    "S"                 /* Consumed Emissions Soil           */
#define e_ModMor    "M"                 /* Mortality                         */

#define e_Head      "H"                 /* Column heading in output file     */


 char gCR[5000];

FILE *fh_Err = NULL ;

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
int  BatchExec (char cr_Mode[], char cr_InFN[], char cr_OutFN[], char cr_RunFN[], char cr_ErrFN[], char cr_Head[]);

void Bat_Help (char cr_PrgNam[], int iS_Err,char cr_Mess[]);

int  CLB_Main (char lp[]);

// void Message (char cr[]);

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CLB_Main
* Desc: See if the user wants to run the command line batch.
*       This gets called right after WinMain(), it checks the
*       command line to see if there is any arguments and:
*        > if arguments are found and valid the batch is run
*        > if arguments are found but invalid the Help Dialog Box is popped upj
*        > if a 'help' argument is found the Help Dialog box is popped up
*          in the above cases the return value tells the call to exit
*           the program
*        > if no arguments are found on the command line are return value is
*           is returned to the caller to let them know to run FOFEM as ususal
*           with the user interface
*
*   In: lp....the command line, this doesn't include the program name/path
*             so if it's blank there are no comman line args
*
*  Command line format:
*     Model infile outfile runfile errfile [H]
*
*  Model:
*     C = consumed emissions
*     S = consumed emissions soil
*     M = Mortality
*     infile = standard input batch file
*     outfile = standard output batch file
*     runfile = run stats, number of stands processes, number of errors
*     errfile = any errors
*     H = optional, when present cause column headings to be put in the
*          outfile
*
*  Example:
*     > fofem5 S b-sample.txt output.txt status.txt errors.txt H
*
*  SPECIAL NOTE: - concerning using blank characters in path or file names
*     > an e_BSC must be used for any blank chars or else we can't parse
*       incoming file path/file names on the command line
*
*   Ret: 1 = no arguments were found on command line so return and run
*             FOFEM5 as normal user interface version
*        0 = Batch was run, arguments were found on command line so batch
*             was run, it ran either with or without data type errors
*            OR Help dialog box was popped up
*       -1 = An attempt was made to run the batch because arguments were
*             found on the command line but a serious error occured like
*             can't open a file.
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int  CLB_Main (String ^ Str)
{
int i; 
char cr[5000];
  
    _StrToChar (Str,cr); 
  i = CLB_Main (cr); 
 
 return i; 
}


int  CLB_Main (char lp[])
{
int  argc;
char   cr_InFN[500], cr_OutFN[500], cr_ErrFN[500];
char  cr_RunFN[500], cr_Mode[500], cr_Head[100];

  fh_Err = NULL ;

  strcpy(cr_Mode,"");
  strcpy(cr_InFN,"");
  strcpy(cr_OutFN,"");
  strcpy(cr_RunFN,"");
  strcpy(cr_ErrFN,"");
  strcpy(cr_Head,"");

  sscanf (lp,"%s %s %s %s %s %s %s", cr_Mode,cr_InFN,cr_OutFN,cr_RunFN,cr_ErrFN,cr_Head);

  argc = 0;
  if ( strcmp (cr_Mode,  "")) argc++;
  if ( strcmp (cr_InFN,  "")) argc++;
  if ( strcmp (cr_OutFN, "")) argc++;
  if ( strcmp (cr_RunFN, "")) argc++;
  if ( strcmp (cr_ErrFN, "")) argc++;
  if ( strcmp (cr_Head,  "")) argc++;
  if ( argc == 0 )                      /* No Args on command line           */
    return 1;                           /* tell caller to run normal FOFEM   */

/* Look for a   Help switch -                                                */
  if ( cr_Mode[0] == 'H' || cr_Mode[0] == 'h' || cr_Mode[0] == '?' ) {
    Bat_Help (e_PrgNam, 0, "");
    return 0;}

  if ( argc < 5 ) {            /* Need 1st 5 arguments, mode & fil nams */
    Bat_Help (e_PrgNam, 1, "Invalid number of command line arguments");
    return -1; }

  if ( stricmp (cr_Mode, e_ModCon) &&
       stricmp (cr_Mode, e_ModSoi) &&
       stricmp (cr_Mode, e_ModMor)) {
    Bat_Help (e_PrgNam, 1, "Invalid model argument");
    return -1; }

  StrRepChr (cr_InFN, e_BSC,' ');
  StrRepChr (cr_OutFN,e_BSC,' ');
  StrRepChr (cr_RunFN,e_BSC,' ');
  StrRepChr (cr_ErrFN,e_BSC,' ');

  if (!BatchExec (cr_Mode, cr_InFN, cr_OutFN, cr_RunFN, cr_ErrFN, cr_Head))
    return -1;                          /* Serious error                     */

  return 0;                             /* Says Batch was run                */

}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Batch
* Desc: Run batch, checks command line arguments, etc
*       Calls the stock Batch FOFEM processing function
* Note-1: The batch function has some call back w
*
*  Ret: 1 ran ok, but there might be data type errors
*       0 Error Can't open file
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  BatchExec (char cr_Mode[], char cr_InFN[], char cr_OutFN[], char cr_RunFN[],
             char cr_ErrFN[],    char cr_Head[])
{
int  iN_OutRec, iS_Soil,iN_SoilErr,iS_CL, iN_Err, i_PFI;
FILE *fh, *fh_Err;
char  cr_SalvFN[500];   /* won't use this but need to pass param */

   iS_Soil = 0;
   iS_CL = 0;

   fh = fopen (cr_RunFN,"w");           /* Open the 'Run' output file        */
   if ( fh == NULL )
     return -1;

   fh_Err = fopen (cr_ErrFN,"w");       /* Open Error text output file       */
   if ( fh_Err == NULL ) {
     fclose (fh);
     return -1; }

   fprintf (fh_Err, " This file contains FOFEM command line batch errors.\n"); 
   fprintf (fh_Err, " Some missing numeric values may be represtented as -1\n"); 
   fprintf (fh_Err, " Input file: %s \n\n", cr_InFN);

   if ( !stricmp (cr_Head, e_Head) )    /* column heading in output file     */
     iS_CL = 1;                         /* Yes                               */


   if ( !stricmp (cr_Mode, e_ModMor) )  /* Is it Mortality Model             */
     goto Mortality;

   if ( !stricmp (cr_Mode, e_ModSoi) )  /* Soil model                        */
     iS_Soil = 1;                       /* Yes                               */

   iN_Err = BAT_CESExec (cr_InFN, cr_OutFN, &iN_OutRec, iS_Soil,
                     &iN_SoilErr, iS_CL, fh_Err, "CommandLine");

   fprintf (fh,"Stands: %d \n",iN_OutRec);
   fprintf (fh,"Errors: %d \n",iN_Err);

   goto Done;


Mortality:

   iN_Err = BAT_MortExec (cr_InFN, cr_OutFN, &iN_OutRec, iS_CL, fh_Err, &i_PFI, -1, "",cr_SalvFN);

Done:


   fprintf (fh,"Stands: %d \n",iN_OutRec);
   fprintf (fh,"Errors: %d \n",iN_Err);
   fprintf (fh,"\n");
   fprintf (fh,"Input : %s \n",cr_InFN);
   fprintf (fh,"Output: %s \n",cr_OutFN);
   fprintf (fh,"Errors: %s \n",cr_ErrFN);



   fclose (fh_Err);
   fclose (fh);

  return 1;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Bat_Help
* Desc: Popup Window explain how to use program
*   In: cr_PrgNam...this program name
*       iS_Err......if a 1 an error mess is display see code below
*       cr_Mess.....If present will get displayed
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void Bat_Help (char cr_PrgNam[], int iS_Err,char cr_Mess[])
{
char cr[100];

  Rem_Path (cr_PrgNam, cr);             /* Get Program Name with no extension*/
  StrRepChr (cr, '.', '\0');

  strcpy (gCR,"");

  if ( strcmp (cr_Mess,"") )
   sprintf (&gCR[strlen(gCR)],"%s\n",cr_Mess);

  if ( iS_Err == 1 )
    strcat (&gCR[strlen(gCR)],"\nERROR - Incorrect Command Line !\n\n");


  strcat (&gCR[strlen(gCR)],"This window explains how to use FOFEM6 to process input batch files\n");
  strcat (&gCR[strlen(gCR)]," from the command line (without using the user interface).\n");
  strcat (&gCR[strlen(gCR)]," \n");

  strcat (&gCR[strlen(gCR)],"command line syntax: \n");
  sprintf (&gCR[strlen(gCR)],"     > %s   %s | %s | %s   infile  outfile  runfile  errfile   [H] \n", cr_PrgNam, e_ModCon,e_ModSoi,e_ModMor);

  strcat (&gCR[strlen(gCR)]," \n");
  strcat (&gCR[strlen(gCR)],"Specify the model to run, use one of the following: \n");
  sprintf(&gCR[strlen(gCR)], " %s = Consumed Emissions     \n" ,e_ModCon  );
  sprintf(&gCR[strlen(gCR)], " %s = Consumed Emisions Soil \n" ,e_ModSoi  );
  sprintf(&gCR[strlen(gCR)], " %s = Mortality              \n" ,e_ModMor  );
  strcat (&gCR[strlen(gCR)], "\n");
  strcat (&gCR[strlen(gCR)], "Name input/output files:\n");
  strcat (&gCR[strlen(gCR)], " infile  = stand data input file \n");
  strcat (&gCR[strlen(gCR)], " outfile = calculated values for each stand, output file created by program\n");
  strcat (&gCR[strlen(gCR)], " runfile = runtime information, number of stands processed,etc., output file created by program\n");
  strcat (&gCR[strlen(gCR)], " errfile = errors messages, output file created by program\n");
  strcat (&gCR[strlen(gCR)], " All four file names are required.\n");
  sprintf(&gCR[strlen(gCR)], "NOTE: you must use a '%c' character in place of any embedded blank spaces characters \n", e_BSC);
  sprintf(&gCR[strlen(gCR)], "       that may be present in path/file names.\n", e_BSC);
  strcat (&gCR[strlen(gCR)], "\n");
  sprintf(&gCR[strlen(gCR)], " %s = Optional argument, when present column headings will be placed in\n", e_Head);
  strcat (&gCR[strlen(gCR)], "      the outfile.\n");
  strcat (&gCR[strlen(gCR)], "\n");
  sprintf (&gCR[strlen(gCR)], " The  %s - Emission Factor Group file must be located in the folder from which are you are running the batch process.  \n", e_EmiFN);
  strcat (&gCR[strlen(gCR)], "\n");
  strcat (&gCR[strlen(gCR)], "Please see FOFEM6 help for full details on running batch files and their formats.\n");
  strcat (&gCR[strlen(gCR)], "\n");
  strcat (&gCR[strlen(gCR)], "To invoke this Help Window:\n");
  sprintf(&gCR[strlen(gCR)], "%s H | h | ? \n",cr_PrgNam);
  strcat (&gCR[strlen(gCR)], "\n");
  strcat (&gCR[strlen(gCR)], "This window is being shown because you requested it or an invalid set of arguments\n");
  strcat (&gCR[strlen(gCR)], "was detected on the command line when invoking the program.\n");
  strcat (&gCR[strlen(gCR)], "\n");

  myMessageBox (gCR);
}
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Message
* Desc: Put message box for other module so it doesn't need to
*        deal with Windows stuff
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
//void Message (char cr[])
//{
   //  (NULL, cr, "FOFEM5 Command Line Batch - Help Window", MB_OK);
//}
