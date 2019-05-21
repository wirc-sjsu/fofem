/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_unix.c
* Desc: These are the functions that are not standard library functions
*
* Author: Larry Gangi
* Date:   May 2005
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include  "fof_ansi.h"

char strA[3000];
char strB[3000];

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: xstrcmpi
* Desc: insensitive case string compare
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int xstrcmpi  (char a[], char b[])
{
int i;
   strcpy (strA,a);
   strcpy (strB,b);
   xstrupr (strA);
   xstrupr (strB);
   return (strcmp (strA,strB));
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: xstrupr
* Desc: convert a string to upper case, strupr is not on Unix
* In/Out:
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  xstrupr (char cr[])
{
int i;
char c;
  for ( i = 0; i < 20000; i++ ) {
    if ( cr[i] == 0 )
      break;
    c = toupper (cr[i]);
    cr[i] = c; }
}
