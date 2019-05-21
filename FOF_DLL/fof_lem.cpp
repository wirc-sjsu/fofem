/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_lem.c         Logic Error Manager
* Desc: This is a place to pass logic errors for the library code that
*        doesn't have an interface. So I can deal with it all here
*        either using MessageBoxes or output files or whatever.
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/


#include <stdio.h>
#include <string.h>

#include  "fof_lem.h"

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: LEM_Put
* Desc: Log logic error message
*       This function was basically put in place while developing to catch
*       any logic errors in the code. Chance are it will never to get 
*       called now, so I have it doing nothing. But the output could get
*       directed the console or a file. 
*       
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void   LEM_Put (char cr_Title[], char cr_Mes[])
{
char cr[200];
   strcpy (cr, "LOGIC ERROR - ");
   strcat (cr, cr_Title);

 //  printf ("---------------------------------------------------\n");
//   printf (" Title: %s \n",cr);
//   printf ("        %s \n",cr_Mes);
//   printf ("---------------------------------------------------\n");

 //  MessageBox (NULL, cr_Mes, cr,MB_OK);

}
