/***************************************************************************************
* Name: FOF_NES.cpp
* Desc: New Emission System
*       The new emission system contains the extensive list of emission factors
*       and replace the original way we did emssions 
* NOTES about the input file
*       The file format has to be exact
*       Note the 1st row, 1st column is blank, just the lone comma followed by ##
*       The first 8 componets need to be as shown below they are the major ones that
*       FOFEM deals with, after these the others can come in any order becuase
*       they will be tied to the text names in the column headings 
*       I do some checks in the code to make sure the format is correct, if it's not
*       the program will pop up a message the first time it tries to use it and the file
*       will need to be fixed
*       Don't use any blank lines in the first portion of the file, at this point i'm not
*        sure if we'll use the descriptions on the end of the file and if there will be
*        any blanks lines there 
*       File Format......
*          ,##,Carbon Dioxide,Carbon Monoxide,Methane,Nitrogen Oxides,Ammonia,Sulfur Dioxide,PM2.5 ,PM10, ......  
*          Group #,# Cover Type,CO2,CO,CH4,NOx as NO,NH3,SO2,PM2.5 ,PM10. ........
*          1,Southeastern Forest,1703,76,2.32,1.70,0.14,1.06,12.58,14.8, .......
*
* SPECIAL NOTES 
*      > Smoldering Emission Factor records are indentified by the 
*        "RSC" text found in their Cover Type Name. 
*      >             
*
* Date: 7-30-14
***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>

#include "FOF_NES.h"

/* NOTE NOTE NOTE ...................*/
/* The are copies of functions in FOFEM and FuelCalc 'util' file functions */
/* Things were getting messing trying to use this file for FOFEM and FuelCalc */
/* so I just put copies of the functions right in this file and prefixed */
/* them is the _ underscore */
int   _IsBlank      (char  cr[] );
void  _Trim_LT (char cr[]);
void  _StrRepChr (char cr[], char c_This,  char c_That );
void  _Left_Just ( char  cr[] );

/*----------------------------------------------------------*/
/* indexs to get from factor array                          */
/* NOTE NOTE ---> These are fixed, the input file needs    */
/*  see notes in this file header    */
/* NOTICE there is no 4 index, because it's not a critical/major emis componet */
#define ex_CO2   0
#define ex_CO    1
#define ex_CH4   2
#define ex_NOX   3      /* NOTE no 4 index, see above note */
#define ex_SO2   4
#define ex_PM25  5
#define ex_PM10  6

/* This appears in the Cover Type text of the emission file if the type */
/*  is a residual smoldering type, this is the only way to tell which one */
/*  are residual smoldering */
#define e_RSC  "RSC"     

/* When found in a Group Name along with RSC indicates the entry is  */
/*  Coarse Woody Smoldering emssion factors                          */
#define e_Woody "Woody"

/* This define what type of Group Emissiong the Row/Record is  */
#define e_TyFSTS  "STFS"      /* used to be ->  "FSTS"      /* Flame Emission Group record */
#define e_TyDuff  "DuffRSC"   /* Duff Emission Group record */
#define e_TyCoar  "CWDRSC"    /* Coarse Woody RSC - smoloder */

/*--------------------------------------*/
/*            Name Code Table          */
typedef struct {
#define eC_Name 150
	char cr_Name[eC_Name];
#define eC_Code 50 
	char cr_Code[eC_Code];
} d_NCT; 

#define eC_NCT 300
d_NCT sr_NCT[eC_NCT];


/*--------------------------------------*/
/*       Emission Factor Table          */
typedef struct {
	char cr_GrpNum[10];    /* Group Number */
	char cr_CovTyp[300];   /* Cover Type */
	char cr_Type[50];      /* Type - Flame, Duff, 1k wood smolder */
#define eC_EA 400
	float fr_EA[eC_EA];    /* Emission Array */
} d_NES;

#define eC_NES 50
d_NES sr_NES[eC_NES];

/*--------------------------------------------*/
void NCT_Init();
void NCT_Put (char cr_Name[], char cr_Code[]);
int NCT_Load ();
int NES_Load ();
void NES_Init ();

/*---------------------------------------------*/
FILE *fh_NES;

#define ROW 10000
char  Row1[ROW], Row2[ROW];


/*****************************************************************
* Name: NES_Get_CriFirInt ()
* Desc: The Critical Fire Intensity Point.
*       Intensity > 15 kW/sq. m use non-smoldering emissions factors 
*       Intensity < 15kW/sq. m use residual smoldering emissions factors 
*       
*****************************************************************/
float NES_Get_CriFirInt ()
{
  return e_CriInt;
}

/*****************************************************************
* Name: NES_Get_MajFactor
* Desc: Get the major emission factors for the specifed group
*       number.
*  Ret: 0 Group Number NOT found, would be a logic errer - hummm
*
******************************************************************/
int NES_Get_MajFactor(char cr_GrpNum[],
                     float *CO, float *CO2, float *CH4, float *PM25,
                     float *PM10, float *NOX, float *SO2)
{
int i; 
  
    for ( i = 0; i < eC_NES; i++ ) {
	  if ( !strcmp (sr_NES[i].cr_GrpNum, "" ))
            return 0;                               /* Error Group Num Not found */
      if ( strcmp (sr_NES[i].cr_GrpNum, cr_GrpNum ))
         continue; 
      *CO   = sr_NES[i].fr_EA[ex_CO  ];
      *CO2  = sr_NES[i].fr_EA[ex_CO2 ];
      *CH4  = sr_NES[i].fr_EA[ex_CH4 ];
      *PM25 = sr_NES[i].fr_EA[ex_PM25];
      *PM10 = sr_NES[i].fr_EA[ex_PM10];
      *NOX  = sr_NES[i].fr_EA[ex_NOX ];
      *SO2  = sr_NES[i].fr_EA[ex_SO2 ];
      return 1;
   }
   return 0;
}

/**********************************************************************
* Name: NES_GetGrpCov  Get Group Number and Cover Type
* Desc: Iteratively get each entry in the Emission Factor Table
*  In:  ix.....start at 0
* Out:  group and cover type, will be "" when no more
*       *aiS_Def..group number is one of the defaults
*                 0 = no, 1 Flame Def, 2 Smold Def
* Ret:  
*       1 = Flaming 
*       2 = Duff RSC residual smoldering
*       3 = Coarse Woody RSC - smoldering
**********************************************************************/
int  NES_GetGrpCov (int ix, char cr_GrpNum[], char cr_CovTyp[], int *aiS_Def)
{
  *aiS_Def = 0;
  strcpy (cr_GrpNum, sr_NES[ix].cr_GrpNum);

  if ( !strcmp (cr_GrpNum, e_DefFlaGrp))
     *aiS_Def = 1; 
  if ( !strcmp (cr_GrpNum, e_DefDufGrp))
     *aiS_Def = 2; 
  if ( !strcmp (cr_GrpNum, e_DefSmoGrp))
     *aiS_Def = 3; 

     
  strcpy (cr_CovTyp, sr_NES[ix].cr_CovTyp);

  if ( !stricmp (sr_NES[ix].cr_Type,e_TyFSTS) )
    return 1; 
  else if ( !stricmp (sr_NES[ix].cr_Type,e_TyDuff) )
    return 2; 
  else if ( !stricmp (sr_NES[ix].cr_Type,e_TyCoar) )
    return 3; 


//  if ( strstr(cr_CovTyp, e_RSC ) ) { /* If Cover type is a residual smoldering type */
//    if ( strstr (cr_CovTyp, e_Woody ) ) 
//		return 2; 
//     return 0;     
//  }

  return -1 ; 

}

/************************************************************************
* NAME: NES_GetFactor
* Desc: Interatively get each emissions factor for the specified
*        emission group number 
*  In: cr_GrpNum...Group nummber  1,2,...
*      ix          incrementing index to get next factor in list
*                  starting at 0, see function return code 
* Out: cr_CovTyp...Cover type of Group Number 
*      f_Factor....Emission Factor
*      cr_Name.....
* Ret: 1 = factor being returned
*      0 = no more
*     -1 = error, no such Group Number 
*************************************************************************/
int NES_Get_Factor(char cr_GrpNum[], int ix, char cr_CovTyp[], float *af_Factor,
			   char cr_Name[],  char cr_Code[])
{
int i;
float f; 

  strcpy (cr_Name, "");
  strcpy (cr_Code, ""); 
  strcpy (cr_CovTyp, ""); 
  
  for ( i = 0; i < eC_NES; i++ ) {
  
	if ( !strcmp (sr_NES[i].cr_GrpNum, "" ))
	  return -1;                               /* Error Group Num Not found */

	if (strcmp (sr_NES[i].cr_GrpNum, cr_GrpNum))  /* Not one we want */
	     continue;

	  strcpy (cr_CovTyp, sr_NES[i].cr_CovTyp); 
      *af_Factor = sr_NES[i].fr_EA[ix];
	  if ( *af_Factor < 0 )
	    return 0; 

      strcpy (cr_Name, sr_NCT[ix].cr_Name);
	  strcpy (cr_Code, sr_NCT[ix].cr_Code); 
	  return 1;
  }
  return -1;

}

/************************************************************************
* Name: NES_Read
* Desc  Read in the emission factor file
*  -->  ALSO do some checking to make sure the file is somewhat 
*        formatted correctly 
*   In: cr_Path.....path to the Emission Factor Group file 
*                   NOTE this function will append the proper file name
*                   to the path
*                --> "" a Null is sent in when running the batch in command
*                    line mode, this will cause the program to look in the
*                    folder that the batch is being run from.
*  Ret: 1 = ok
*       < 0 - see below
************************************************************************/	   
int NES_Read (char cr_Path[], char cr_ErrMes[])
{
int i,j, i_Ret;
char    cr_GrpNum[200],  cr_Name[200], cr_Code[200],  cr_CovTyp[200];
char cr_FN[1000]; 

float f_Factor;

  strcpy (cr_ErrMes, "");

  i_Ret = 1;

  if ( !stricmp (cr_Path,"") ) 
     strcpy (cr_FN,e_EmiFN); 
  else 
     sprintf (cr_FN, "%s\\%s",cr_Path, e_EmiFN); 


  fh_NES = fopen (cr_FN, "r"); 
  if ( fh_NES == NULL ) {
    sprintf (cr_ErrMes, "Error opening or can't find input emission file: %s \n", cr_FN);
	return 0; }

  i = NCT_Load ();    /* Load Name Code Table */
  if ( i < 0 ) {  
    strcpy (cr_ErrMes, "Input Emission File has invalid format. \n Look at a previous file that had a valid format\n");
    return 0; }

  i = NES_Load ();    /* Load Emission Factor Table */


  strcpy (cr_GrpNum,"1"); 
  for ( i = 0; i < 8; i++ ) {

    j = NES_Get_Factor(cr_GrpNum, i, cr_CovTyp, &f_Factor, cr_Name, cr_Code);
    if ( j < 0 ) {
       i_Ret = -1;
       goto Close; }

    if ( i == 0  &&  stricmp (cr_Code,"CO2"))      { i_Ret = -2; goto Close; }
    if ( i == 1  &&  stricmp (cr_Code,"CO"))       { i_Ret = -3; goto Close; }
    if ( i == 2  &&  stricmp (cr_Code,"CH4"))      { i_Ret = -4; goto Close; }
    if ( i == 3  &&  stricmp (cr_Code,"NOx as NO")){ i_Ret = -5; goto Close; }
    if ( i == 4  &&  stricmp (cr_Code,"SO2"))      { i_Ret = -7; goto Close; }
    if ( i == 5  &&  stricmp (cr_Code,"PM2.5"))    { i_Ret = -8; goto Close; }
    if ( i == 6  &&  stricmp (cr_Code,"PM10"))     { i_Ret = -9; goto Close; }
  }

 //  NES_Display ();

Close:

  if ( i_Ret < 0 )
    sprintf (cr_ErrMes, "Emission File Component(s) is not in expected order.\n%s",cr_FN);

  fclose (fh_NES); 

  return i_Ret; 

  }

/*******************************************************************
*
* Note-1: We assume that when we hit a blank line there is no more
*         emissions data rows. There may be comments in the file 
*         following the blank line.
* Note-2: the blank line following the data lines could be filled 
*          with commas. DL keeps the emission file as an Excel file
*          and exports it as a .csv file, so I ran into a case where
*          the blank line really was filled with ",,,,,,,,,,  " all
*          commas. 
*******************************************************************/
int NES_Load ()
{
int i, iX, ix;
float f;
char *a, *r1, *r2;
char *sav1, *sav2;

 NES_Init ();

 iX = 0;
 for ( i = 0; i < 100000; i++ ) {
    
    a = fgets (Row1, ROW, fh_NES);
	if ( a == NULL ) 
	  break; 

     if ( !strcmp (Row1,"\n"))   /* See Note-1 */
       break; 

     if ( Row1[0] == ',' )       /* See Note-2 */
       break; 

    _StrRepChr (Row1, 13, 0 );    /* carrage return */
    _StrRepChr (Row1, 10, 0 );    /* line feed */
    _StrRepChr (Row1,  9, ' ');   /* Tabs, just incase there are any */

    if ( _IsBlank (Row1) )
	  continue; 



    r1 = strtok_s (Row1,",", &sav1);    /* Group Number */
	_Trim_LT (r1);
	strcpy (sr_NES[iX].cr_GrpNum,r1);

    r1 = strtok_s (sav1,",", &sav1);    /* Cover Type */
    _Trim_LT (r1);
	strcpy (sr_NES[iX].cr_CovTyp,r1);


    r1 = strtok_s (sav1,",", &sav1);    /* Cover Type */
    _Trim_LT (r1);
	strcpy (sr_NES[iX].cr_Type,r1);


    r1 = strtok_s (sav1,",", &sav1);    /* Emission Factor */

    ix = 0; 
    while (r1 != NULL) {                /* load factor array from file */
      _Trim_LT (r1);
	  f = atof (r1); 
      sr_NES[iX].fr_EA[ix] = f;
	  ix++;
      r1 = strtok_s (sav1,",", &sav1);
	} 
	 
	iX++;

  } /* for i */ 

 return 1; 
}

/**********************************************************************
*
*
***********************************************************************/
void NES_Display ()
{
int i,j,k;
float f; 

  for ( i = 0; i < eC_NES; i++ ) {
      if ( !strcmp (sr_NES[i].cr_GrpNum,"") )
	     break; 
	  printf (" %s  %s\n", sr_NES[i].cr_GrpNum, sr_NES[i].cr_CovTyp);
      k = 0; 
	  for ( j = 0; j < eC_EA; j++ ) {
         f = sr_NES[i].fr_EA[j];
		 if ( f < 0 )
		   break; 
	     printf ("%7.3f",f);
         k++;
		 if ( k == 10 ) {
		   printf ("\n");
		   k = 0; } }
      printf ("\n\n");
  }
}

/**********************************************************************
*
*
***********************************************************************/
void NES_Init ()
{
int i,j;

  for ( i = 0; i < eC_NES; i++ ) {
	  strcpy (sr_NES[i].cr_GrpNum, ""); 
	  strcpy (sr_NES[i].cr_CovTyp, "");
	  strcpy (sr_NES[i].cr_Type,""); 
	  for ( j = 0; j < eC_EA; j++ ) {
         sr_NES[i].fr_EA[j] = -1; } }
}

/************************************************************************
* Name: NCT_Load
* Desc  Load the Name and Code table from the emissions factor file
*       The Names are on the 1st row of the file and the codes are
*       the second.
* NOTE --> I do some check to make sure the input file is in the
*          right format, it has to be exact. 
* Note-1: the input file is a dump Execl comma delimited csv file. 
*         when the text in a field actually contain a comma then 
*         Excel will doulbe quote the field, so I have to deal with that
*         I scan the line, replace commas with $ and remove the quotes 
*         later I put the commas back in the text.  
* Note-2: These ## are the first token we get when parsing the file
*         even though it is in the second column of the input file,
*         probably because the first column is blank . ? ?
* Ret: 0 ok  
*
************************************************************************/ 
int NCT_Load ()
{
int i,j, i_Ret;
int x; 
char *a, *r1, *r2;
char *sav1, *sav2;

  i_Ret = 0; 

  for ( x = 0; x < 2; x++ ) {

    a = fgets (Row2, ROW, fh_NES);

    _StrRepChr (Row2, 13, 0 );    /* carrage return */
    _StrRepChr (Row2, 10, 0 );    /* line feed */
    _StrRepChr (Row2,  9, ' ');   /* Tabs, just incase there are any */

 /* See Note-1 above, this deals with embedded commas in strings */
	j = 0;
    for ( i = 0; i < 1000000; i++ )  {
      if ( Row2[i] == NULL )
        break;
      if ( Row2[i] == '"' && j == 0  )
        j = 1;
      else if ( Row2[i] == '"' && j == 1 )
        j = 0;

      if ( Row2[i] == ',' && j == 1 )
         Row2[i] = '$';  }

    for ( i = 0; i < 100000; i++ )  {
      if ( Row2[i] == NULL )
        break;
      if ( Row2[i] == '"' )
        Row2[i] = ' '; }

    if ( x == 0 )
          strcpy (Row1,Row2);

  } /* for x */

/*..............................................*/

  NCT_Init();               /* Init the Name Code Table */
 
  r1 = strtok_s (Row1,",", &sav1);
  _Trim_LT(r1);
  if ( stricmp (r1,"##") ){ /* See Note-2 above */
     i_Ret = -1;
	 goto Ret; }

/* Change - 10-9-14, DL want to add this into the file */
  r1 = strtok_s (sav1,",", &sav1);
  _Trim_LT(r1);
  if ( stricmp (r1,"units:g/kg") ){ 
     i_Ret = -1;
	 goto Ret; }

  r1 = strtok_s (sav1,",", &sav1);   /* 1st Name, well use it in a bit */

  r2 = strtok_s (Row2,",", &sav2);
  _Trim_LT(r2);
  if ( stricmp (r2, "Group #") ) {
	 i_Ret =  -2;
	 goto Ret; }

  r2 = strtok_s (sav2,",", &sav2);
  _Trim_LT(r2);
  if ( stricmp (r2, "# Cover Type") ) {   /* "# Description" */
	 i_Ret = -3;
	 goto Ret; }

  r2 = strtok_s (sav2,",", &sav2);
  _Trim_LT(r2);
  if ( stricmp (r2, "Type") ) {
	 i_Ret = -4;
	 goto Ret; }




  r2 = strtok_s (sav2,",", &sav2);  /* 1st Code we'll be using */

  while (r1 != NULL) {
    _Trim_LT (r1);
    _StrRepChr (r1, '$', ',' );

    _Trim_LT (r2);
    _StrRepChr (r2, '$', ',' );
	
	NCT_Put (r1, r2);

    r1 = strtok_s (sav1,",", &sav1);
	r2 = strtok_s (sav2,",", &sav2); }
    
 
Ret:

  return i_Ret;

}

/*************************************************************/
void NCT_Init ()
{
int i; 
	for ( i = 0; i < eC_NCT; i++ ) {
	  strcpy (sr_NCT[i].cr_Name,"");
      strcpy (sr_NCT[i].cr_Code,"");
	} 
}

/*************************************************************/
void NCT_Display ()
{
int i; 
	for ( i = 0; i < eC_NCT; i++ ) {
	  if ( !stricmp (sr_NCT[i].cr_Name,""))
		 break;
      printf ("%3d  %-30s    %s \n", i+1, sr_NCT[i].cr_Code, sr_NCT[i].cr_Name);
	} 
}

/**************************************************************/
void NCT_Put (char cr_Name[], char cr_Code[])
{
int i;
   for ( i = 0; i < eC_NCT; i++ ) {
	 if ( stricmp (sr_NCT[i].cr_Name,""))
	   continue;
	 strcpy (sr_NCT[i].cr_Name,cr_Name);
	 strcpy (sr_NCT[i].cr_Code,cr_Code); 
	 return;
   } 
}




//******************************************************************************
//******************************************************************************
//******************************************************************************

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _Trim_LT
* Desc: Remove Leading & Trailing blanks from a string.
* NOTE: This will leave any embedded blanks intact.
*       see the Rem_LT_Blanks function
*   In: cr.......String
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void     _Trim_LT (char cr[])
{
int i,j;
   _Left_Just (cr);                /* remove leading */
   j = strlen(cr);
   if ( j == 0 )
     goto Z;
   j--;
   for ( i = j; i > 0; i--) {     /* go to end of string and work */
     if ( cr[i] != ' ' )          /* back until a char is hit */
       break;                     /* then null term the string */
     if ( cr[i] == ' ' )
       cr[i] = 0;
   }
Z:;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
*  Name: _Left_Just
*  Desc: Left Justify a null terminated string
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void    _Left_Just ( char  cr[] )
{
int     i, j;
   if ( cr[1] == 0 ) goto Z;                 /* Empty String */
   for ( i = 0; i < 1000; i++ ) {            /* Find first non blank */
     if ( cr[i] != ' ' )
        break ;   }
   if ( i == 0 )  goto Z;                    /* Already Left Justified */
   for ( j = 0; j < 1000; j++ ) {            /* Left Justify it          */
    cr [j]  =  cr [i++];
    if ( cr[j] == 0 )
       break;     }
Z:;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _StrRepChr
* Desc: Replace all the characters in a string with another character
*       String must be NULL terminated
*   In: c_This....find this char in string
*       c_That....new char to be put into string
* In/Out:  cr.....null term string
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  _StrRepChr (char cr[], char c_This,  char c_That )
{
int i;
    for ( i = 0; i < 20000; i++ ) {
      if ( cr[i] == 0 )
        break;
      if ( cr[i] == c_This )
        cr[i] = c_That;
    }
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: isBlank
* Desc: See if a line contains only blanks or is empty.
*   In: cr......string
*  Ret: 1....Blank Line
*       0....not Empty
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int   _IsBlank (char  cr[] )
{
int i;
    for ( i = 0; i < 10000; i++ ) {
      if ( cr[i] == 0 )
        return 1;
      if ( cr[i] == ' ' )
        continue ;
      return 0;
    }
    return 0;   /* Shouldn't get here */
}

