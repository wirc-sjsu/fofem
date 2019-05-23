/*****************************************************************
* Name: LMS.cpp - Log Message System
* Desc: This is not part of the Matlab code. 
*       These functions are used to log test error and warning
*       message. 
******************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"
#include <time.h>

FILE *fh_LMS = NULL; 

/********************************************************
* PUt a message to the file
*********************************************************/
int LMS_Mess (char A[], char B[])
{
   fprintf (fh_LMS, "------------------------------------------------\n");
   fprintf (fh_LMS, "%s\n%s\n", A,B);
   return 1; 
}
/*********************************************************
* Open Log File and insert date and time
***********************************************************/
int   LMS_Open (char cr_FN[])
{
time_t rawtime;

  time (&rawtime);
 
// printf (" The current local time is: %s", ctime (&rawtime));

  fh_LMS  = fopen (cr_FN, "w"); 
  if ( fh_LMS == NULL ) 
    return 0; 
  fprintf (fh_LMS, "Begin Logging: %s\n", ctime (&rawtime));
  return 1; 
}

/**********************************************************/
void LMS_Close ()
{
time_t rawtime;

  time (&rawtime);
  fprintf (fh_LMS, "Completed at: %s\n", ctime (&rawtime));

  if ( fh_LMS != NULL )
    fclose(fh_LMS); 
}