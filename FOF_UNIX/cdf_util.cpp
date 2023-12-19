/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: cdf_util.cpp      Comma Delimited File functions
* Desc:
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "cdf_util.h"
#include "fof_util.h"

//char     Get_NumTyp (char cr_Data[]);
void  Rem_LT_Blanks (char cr[]);
void    EndNull (char cr[]);
void    Left_Just ( char  cr[] );
void  Trim_LT (char cr[]);
void  StrRepChr (char cr[], char c_This,  char c_That );

// #include "fof_util.h"
// #include "fof_gen.h"



// char gCR1 [1000];
int   Get_SubStr (char cr_In[], char cr_Out[], int A, int B, char cr_Line[],char cr_ErrMes[]);
int   Find_Comma (char cr_Line[], int iN, char cr_ErrMes[]);
int  _CommentLine (char cr_Line[] );
int   iisBlankCR (char  cr[] );

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: CDF_Get
* Desc: Read in a line from the specified comma delimited file, and then
*        use the field descriptions in the CDF to read, parse the input file
*        line and put the parsed data to the locations specified in the
*        CDF, which is a record structure.
*   In: a_CDF.....Comma Delimite File structure, this descripts each field
*                  and tells where to put it
*       fh........file to read from
*  Out: cr_ErrMes...Error message or NULL if none,
*                   Check this string for NULL to see if an error occured
*                   NOTE: return value will still be 1, see error messages
*                         below
*  Ret: 1 OK record read - NOTE you still need to check cr_ErrMes for any
*          error, the -1 below is only for serious error
*       0 end of file hit
*      -1 Error, The imput files is not a comma delimited type, so this
*                 is a hard error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  CDF_Get (d_CDF *a_CDF, FILE *fh, char cr_ErrMes[])
{

/* I had these large char[] as global, but moved them in here just to be sure */
/*  not sure if it would become  're-enterent' due to control event handlers */
/*  firing off within each other */
#define eC_Line 3000
char cr_Line[eC_Line];

#define eC_Field 1000
char cr_Field [eC_Field];


int i, j, *ai, A, B, i_Ret;
long int l, *al;
float *af,f;
char *a,*ac, c, cr_tmp[1000], cr_Err[1000];
d_CDF *adr;

   i_Ret = 1;
   strcpy (cr_ErrMes,"");

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* Init the record strcut                                                    */
  adr = a_CDF;
  for ( i = 0; i < 2000; i++ ) {
    c = adr->c_Type;
    if ( c == e_End )
      break;
    if ( c ==  e_Str  )   { strcpy ((char*)adr->adr,""); }
    else if ( c == e_Flo ){ af = (float *)adr->adr; *af = e_Flo_Init; }
    else if ( c == e_Int ){ ai = (int *)adr->adr; *ai = e_Int_Init; }
    else if ( c == e_Lon ){ al = (long*)adr->adr; *al = e_Lon_Init; }
    else if ( c == e_Cha ){ ac = (char*)adr->adr; *ac = e_Cha_Init; }
    else {
       strcpy (cr_ErrMes, "ERROR - CDF_Get - Logic Error, bad type in CDF struct.");
       exit (1); }
    adr++; }

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* Get next line from file                                                   */
Get:
  a = fgets (cr_Line, eC_Line, fh);
  if ( a == NULL ) {                    /* End of file                       */
    i_Ret = 0;
    goto X; }

  StrRepChr (cr_Line,'\t',' ');         /* blnk tab chars if found           */

  if ( iisBlankCR (cr_Line) )
    goto Get;

  if ( _CommentLine (cr_Line) )
    goto Get;

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
   adr = a_CDF;
   adr--;
   for ( i = 0; i < 2000; i++ ) {
      adr++;
      c = adr->c_Type;
      if ( c == e_End )
        break;

      A = Find_Comma (cr_Line, i, cr_Err);
      B = Find_Comma (cr_Line, i+1, cr_Err);
      if ( strcmp (cr_Err,"") ){
        strcat (cr_ErrMes,cr_Err);
        return -1; }

      if ( !Get_SubStr (cr_Line, cr_Field, A, B, cr_Line, cr_Err)){
         sprintf (cr_ErrMes,"%s\nLine->%s\n At/Near Data Column %d\n", cr_Err,cr_Line,i);
         return -1; }                    /* input file format error           */

/* String .....................................                              */
      if ( c == e_Str ) {
        j = (strlen(cr_Field) - 1);
        if ( j >= adr->i_Size  ) {
          sprintf (cr_tmp,"The '%s' record field is to long, will trunc. chars to fit.\n",adr->cr_Name);
          strcat (cr_ErrMes,cr_tmp);  /* test */
          cr_Field [adr->i_Size - 1] = 0; }
        strcpy ((char*)adr->adr,cr_Field); }

/* Integer.....................................                              */
      if ( c == e_Int ) {
        if ( !strcmp(cr_Field,"") )
          continue;
        ai = (int *) adr->adr;
        if ( Get_NumTyp (cr_Field) != 'I' ) {
          sprintf (cr_tmp,"The '%s' record field has invalid integer: Data = |%s|\n",adr->cr_Name,cr_Field);
          strcat (cr_ErrMes,cr_tmp);  /* test */
          *ai = e_Int_Init;
          continue;}

        l = atol (cr_Field);
		if ( l > 2147483647 ){  /* 32 bit positvie value,  i was using 32768 */
          sprintf (cr_tmp,"The '%s' record field has an integer overflow, will set to Missing. Data=|%s|\n",adr->cr_Name,cr_Field);
          strcat (cr_ErrMes,cr_tmp);  /* test */
          *ai = e_Int_Init; }
        else
          *ai = (int) l;   }

/* Long Integer.....................................                         */
      if ( c == e_Lon ) {
        if ( !strcmp(cr_Field,"") )
          continue;
        al = (long *)adr->adr;
        if ( Get_NumTyp (cr_Field) != 'I' ) {
          sprintf (cr_tmp,"The '%s' record field has invalid integer\n",adr->cr_Name);
          strcat (cr_ErrMes,cr_tmp);  /* test */
          *al = e_Lon_Init;
          continue;}
        l = atol (cr_Field);
       *al = l; }

/* Float.......................................                              */
      if ( c == e_Flo ) {
        if ( !strcmp(cr_Field,"") )
          continue;
        af = (float *)adr->adr;
        c = Get_NumTyp (cr_Field);
        if ( c != 'F' && c!= 'I' ) {
          sprintf (cr_tmp,"The '%s' record field has invalid floating point number\n",adr->cr_Name);
          strcat (cr_ErrMes,cr_tmp);  /* test */
          *af = e_Flo_Init;
          continue;}
        f =  atof (cr_Field);
       *af = f; }

/* Character, single char field................                              */
      if ( c == e_Cha ) {
        if ( !strcmp(cr_Field,"") )
          continue;
        ac = (char *)adr->adr;
        j = (strlen(cr_Field));
        if ( j > 1 ) {
          sprintf (cr_tmp,"The '%s' record field (char field) is more than one char in length, will use 1st char.\n",adr->cr_Name);
          strcat (cr_ErrMes,cr_tmp); }  /* test */
        if ( j == 0 )
          *ac = e_Cha_Init;
        else
           *ac = cr_Field[0];  }
    }

X:

  i = strlen (cr_ErrMes);
  if ( i >= eC_ErrMes ) {
    sprintf  (cr_ErrMes,"ERROR - CDF_Get - Error Message to Small Please make bigger: %d ",i);
    return -1; }

  return i_Ret ;

}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _CommentLine
* Desc: See if a line of input is a comment line,
*       Comments lines have the first character of the lines as
*        a '#'  it can be in the first char or have blanks preceeding it
*   In: cr_Line.....input file line of characters
*  Ret: 1 = Yes it's a comment line, else 0
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  _CommentLine (char cr_Line[] )
{
int i;
  for ( i = 0; i < 20000; i++ ) {
     if ( cr_Line[i] == 0 )
       return 0;
     if ( cr_Line[i] == ' ' )
       continue;
     if ( cr_Line[i] == '#' )
        break;
     return 0; 

     }
     
  return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Get_SubStr
* Desc: Get a sub-string from another string, the sub-string is defined by
*        the 2 indexs you send in
* Note-1: There was a bug when the first field in a record/line is empty
*          example  >,3,4,abc   
*         This code checks for the specific condition and sets to blank/missing
*         
*   In: cr_In...the string
*       A,B.....1 and 2cd indexs
*  Out: cr_Out....sub-string
*  Ret: 1 ok, 0 = format input error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int   Get_SubStr (char cr_In[], char cr_Out[], int A, int B, char cr_Line[], char cr_ErrMes[])
{
int i;

// Change 8-10-14 See Note-1 above
   if ( A == 0 && B == 0  ) {
	  strcpy (cr_Out,"");
      return 1; }
// -------------------------------

   if (  A > (int) strlen(cr_In) ||  B > ( int) strlen(cr_In) )
     goto Err;

   if ( A >= B )
     goto Err;

   i = (B - A) + 1;
   strncpy (cr_Out, &cr_In[A],i);       /* Get the sub string                */
   cr_Out[i] = 0;                       /* need to null term string          */
   StrRepChr (cr_Out,',',' ');          /* get rid of any commas             */

   if ( iisBlankCR (cr_Out) ) {          /* see if its a blank/empty string   */
     strcpy (cr_Out,"");
     goto X; }

   Trim_LT (cr_Out);                    /* trim any lead/trail blanks        */
   StrRepChr (cr_Out,'"',' ');          /* remove any quotes                 */
   StrRepChr (cr_Out,'\n',0);         /* remove these to                   */
   StrRepChr (cr_Out,'\r',0);

   if ( iisBlankCR(cr_Out) ) {           /* is string just blanks             */
     strcpy (cr_Out,"");                /* set to empty string               */
     goto X; }

   Trim_LT (cr_Out);                    /* get rid of lead/trail blanks      */
X:;
   return 1;

Err:
   strcat (cr_ErrMes, "> Format of input is incorrect !\n");

   return 0;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Find_Comma
* Desc: Find a comma in a string and return its' index position
*   In: cr_Line...the string
*       iN........0 -> N  which comma you want
*                 Sending in 0 will get a 0 return index
*  Ret: index into string where the comma was found
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  Find_Comma (char cr_Line[], int iN, char cr_ErrMes[])
{
int i,i_Cnt;
   strcpy (cr_ErrMes,""); 
   if ( iN == 0 )
     return 0;

   i_Cnt = 0;

   for ( i = 0; i < 10000; i++ ) {
     if ( cr_Line[i] ==  0   )
        return i;
     if ( cr_Line[i] == '\n' )
        return i;
     if ( cr_Line[i] == '\r' )
        return i;
     if ( cr_Line[i] == ',' )
        i_Cnt++;
     if ( i_Cnt == iN )
       return i;
   }

  strcpy (cr_ErrMes, "ERROR - Find_Comma - Logic Error");
  exit (1);
  return 0;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: iisBlankCR
* Desc: See if a line contains only blanks and or car ret, line feed
*   In: cr......string
*  Ret: 1....Blank Line
*       0....not Empty
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int   iisBlankCR (char  cr[] )
{
int i;
    for ( i = 0; i < 1000; i++ ) {
      if ( cr[i] == 0 )
        return 1;
      if ( cr[i] == ' ' )
        continue ;
      if ( cr[i] == '\n' )   /*  car ret */
        continue ;
      if ( cr[i] == '\r' )   /* line feed, i think */
        continue ;
      return 0;
    }
    return 0;   /* Shouldn't get here */
}