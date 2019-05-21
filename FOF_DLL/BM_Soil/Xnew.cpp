/**********************************************************************
* Name: Xnew.cpp
* Desc: I was setting this to have all the "New" memory allocation
*       go thru here
*
*********************************************************************/
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMSoil.h"
#ifdef WWWWWWWWW

#define eC_New 10000

typedef struct  {
     char cr_Id[50];
     char cr_Grp[50]; 
     int i_Hits; 
     double *adr;  
    } d_XNew; 

d_XNew  XN[eC_New];

/***********************************************************/
void  xNew_Init () 
{
int i;
   for ( i = 0; i < eC_New; i++ ){ 
      strcpy (XN[i].cr_Id,"");
      strcpy (XN[i].cr_Grp,""); 
      XN[i].i_Hits = 0; 
      XN[i].adr = NULL;  
   } /* for i */ 
   }

/*********************************************************
*
*********************************************************/
double *xNew (int i_Size, char cr_Id[], char cr_Grp[] )
{
int i; 
double *a;
  
// don't use this func

  a = new double [i_Size] ;   /* Get some memory */
 
  for ( i = 0; i < eC_New; i++ ) { 
    if ( strcmp (XN[i].cr_Id,"" ))   /* find empty slot in tble */
      continue;   
    strcpy (XN[i].cr_Id,cr_Id);      /* Save stuff */
    strcpy (XN[i].cr_Grp,cr_Grp); 
    XN[i].adr = a;
    XN[i].i_Hits++;                  /* count */
    return a;
  }
  
  return NULL; 
}


/*************************************************
*
*
************************************************/
int xDelGrp (char cr_Grp[])
{
int i, N; 
char A[500];
   N = 0; 
   for ( i = 0; i < eC_New; i++ ) { 
     if ( stricmp (XN[i].cr_Grp,cr_Grp) )
       continue; 

     strcpy (XN[i].cr_Id,""); 
     if ( XN[i].i_Hits > 1 ){
       sprintf (A, " Hit count > %d, %s",XN[i].i_Hits, XN[i].cr_Id);
 //      LMS_Mess("xDelGrp()", A); 
} 
      delete XN[i].adr;
      XN[i].adr = NULL;
      XN[i].i_Hits = 0; 
      strcpy (XN[i].cr_Id,""); 
      N++; 
   } 
 // printf ("xDelGrp: %s  %d \n", cr_Grp, N); 
  return 1; 
}


/*****************************************************
*
******************************************************/
int xDelete (double *a)
{
int i;
char A[500];

   for ( i = 0; i < eC_New; i++ ) {
     if ( XN[i].adr != a )   /* look for adr in table */ 
       continue; 
  
     strcpy (XN[i].cr_Id,""); 
     if ( XN[i].i_Hits > 1 ){
       sprintf (A, " Hit count > %d, %s",XN[i].i_Hits, XN[i].cr_Id);
    //   LMS_Mess("xDelete()", A); 
} 
      XN[i].adr = NULL;
      XN[i].i_Hits = 0; 
      strcpy (XN[i].cr_Id,""); 
      delete a; 
      return 1; 
   } /* for i */

   LMS_Mess ("xDelete", "ERROR - can't find address \n"); 
   return 0;  
}


/*****************************************************
*
******************************************************/
int xCheck ()
{
int i, N;
char A[500]; 
    N = 0; 
   for ( i = 0; i < eC_New; i++ ) {
     if ( !strcmp (XN[i].cr_Id,"") ) 
       continue;
     sprintf (A,"unrelease address found Id: %s", XN[i].cr_Id);     
     LMS_Mess ("xCheck()", A); 
     N++;
  }
  return 0; 
}


#endif 