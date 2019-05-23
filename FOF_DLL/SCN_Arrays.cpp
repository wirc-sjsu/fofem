
/**********************************************************************
* Name: SCN_Arrays.cpp  - Save CrankNicolson
* Desc: Save the arrays that are create in the CrankNicolson function
*        
* Author: LG 
***********************************************************************/
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

#include "SCN_Arrays.h" 

/*------------------------------------------------------
*               Save CrankNicolson array struct 
* cr_Name - any name you wish to assign to the array, could 
*           just be the variable name 
* a_Lev - - the array declared in the CrankNicolson function to hold values
*           created at each soil level 
* a_Tim   - an array of double pointers - one for each time step 
*           attached to each of these pointers will be an array of soil level
*           layer values that are created at each time step.
* cr_Auto - "Yes" or "No" - a Yes will cause the array to be saved with the
*            SCN_Save() function that goes thru and does all 
*           "No" means you'll have to do the Save explicitly   
* In Short - we are saving values for each time step and each soil level 
*/
typedef struct {
   char  cr_Name[50];   /* Name of array */ 
   char  cr_Auto[10];   /* Yes or No */
   double *a_Lev;        /* array of Soil Level Values */
   double **a_Tim;       /* Time Steps */
  } d_SCN; 

#define eC_SCN 100 
d_SCN SCN[eC_SCN]; 

int gi_TimStps = 0; 
int gi_Levels = 0; 

/*****************************************************************
* Name: SCN_Init
*
******************************************************************/
void SCN_Init ( int i_Steps, int i_Levels)
{
int i;
   gi_TimStps = i_Steps; 
   gi_Levels = i_Levels; 

   for ( i = 0; i < eC_SCN; i++ ) {
      strcpy (SCN[i].cr_Name,""); 
      strcpy (SCN[i].cr_Auto,""); 
      SCN[i].a_Lev = NULL; 
      SCN[i].a_Tim = NULL;
   }
}

/**********************************************************************
* Name:
* Desc: Setup an individual Level array for use. 
*       Arrays must be attached here before it can be used
*        for saving. 
* In:  cr_Name... name you wish to assing to the array entry
*      a_Lev..... the array in CrankNicolson() the will be holding
*                 the values at each step
*      cr_Auto... Yes or No 
* NOTE NOTE ... the first values in the Level arrays are at [1]
                the first time step is at [1]
**********************************************************************/
int SCN_SetLevAry (char cr_Name[], double *a_Lev, char cr_Auto[])
{
int i,j,k;
  for ( i = 0; i < eC_SCN; i++ ) {
    if ( strcmp (SCN[i].cr_Name,"") ) 
      continue; 
 
    strcpy (SCN[i].cr_Name, cr_Name);
    strcpy (SCN[i].cr_Auto, cr_Auto); 
    SCN[i].a_Lev = a_Lev; 

    SCN[i].a_Tim = new double * [gi_TimStps+1];

    SCN[i].a_Tim[0] = NULL;     /* Don't use - see note above */

    for ( j = 1; j <= gi_TimStps; j++ ) { 
      SCN[i].a_Tim[j] = new double [gi_Levels+1]; 
      for ( k = 1; k <= gi_Levels; k++ )
        SCN[i].a_Tim[j][k] = -1.0; 
    }  /* for j */

    break; 
  } /* for i */ 

  return 1; 
}

/****************************************************************
* Name: SCN_Save
* Desc: Save all the Level arraya to their Time Step array
*   In: i_Step...time step to save to
****************************************************************/
int SCN_Save (int i_Step )
{
int i,lvl; 
double *To, *Frm; 
 
  if ( i_Step > gi_TimStps )  /* Make sure within */
    return 0; 

  for ( i = 0; i < eC_SCN; i++ ) {
    if ( !strcmp ( SCN[i].cr_Name,"") )
      break; 
    if ( !stricmp ( SCN[i].cr_Auto, "No") ) 
       continue;                  /* This is not set for Auto Save  */
    To = SCN[i].a_Tim[i_Step];
    Frm = SCN[i].a_Lev;     
    for ( lvl = 1; lvl <= gi_Levels; lvl++ ) 
       To[lvl] = Frm[lvl];

   } /* for i */

    return 1; 
}

/****************************************************************
* Name: SCN_SaveInd
* Desc: Save an individual Level array to the specified time 
*       step
*   In: adr....address of the Level array to save
*       i_Step...time step to save to
****************************************************************/
int SCN_SaveIndv (double *adr, int i_Step )
{
int i,lvl; 
double *To, *Frm; 
 
  if ( i_Step > gi_TimStps )  /* Make sure within */
    return 0; 

  for ( i = 0; i < eC_SCN; i++ ) {
    if ( SCN[i].a_Lev != adr )
      continue; 
 
    To = SCN[i].a_Tim[i_Step];
    Frm = SCN[i].a_Lev;     
    for ( lvl = 1; lvl <= gi_Levels; lvl++ ) 
       To[lvl] = Frm[lvl];
    break; 
  } /* for i */

    return 1; 
}


/**********************************************************************
*
* 
*      
**********************************************************************/
int SCN_Close ()
{
int i,j;
  
  for ( i = 0; i < eC_SCN; i++ ) {
    if ( strcmp (SCN[i].cr_Name,"") ) 
      break; 
 
   for ( j = 1; j <= gi_TimStps; j++ ) 
     delete SCN[i].a_Tim[j] ;
  
   delete SCN[i].a_Tim;

   strcat (SCN[i].cr_Name, "-Deleted"); 

  } /* for i */ 

  return 1; 
}


/*******************************************************************
*
*
*******************************************************************/
int  SCN_Display (char cr[])
{
int i,stp,lvl; 
FILE *fh; 
char cr_FN[1000]; 

   sprintf (cr_FN, "SCN-Dump-%s.txt",cr); 
   fh = fopen (cr_FN, "w"); 
   if ( fh == NULL ) 
     return 0;  
   
   for ( i = 0; i < eC_SCN; i++ ) {
      if ( !strcmp (SCN[i].cr_Name, "") )    /* End */
        break; 
     fprintf (fh, ">-------------------------------------------------------<\n"); 
     fprintf (fh, " Array: %s \n", SCN[i].cr_Name); 
     for ( stp = 1; stp <= gi_TimStps; stp++ ) {  
       fprintf  ( fh, "%d: ", stp ); 
       for ( lvl = 1; lvl <= gi_Levels; lvl++ ) 
         fprintf (fh, " %f", SCN[i].a_Tim[stp][lvl]); 
       fprintf (fh, "\n"); 
     } /* for stp */
   } /* for i */ 


  fclose (fh); 
  return 1; 
}

