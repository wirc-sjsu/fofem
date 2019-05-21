#include <stdio.h>
#include <math.h>
#include <string.h> 

#include "Bat_Frm.h"
#include "Wnd_Util.h" 
#include "bat_mai.h" 
#include "fof_util.h"

#define e_ConE "ConE-In.txt" 
#define e_Soil "Soil-In.txt"
#define e_Mort "Mort-In.tre"

int ColumnOutPut (char cr_FNin[], char cr_FNout[], char cr_ErrMes[]) ;
int _GetFld (char cr[], int iX, char cr_Fld[]) ;
void _SetAAA (char cr[]);

/******************************************************
* Name: Input
* Desc: Popup Dialog box and get iput file name
*       form the output file name and put both names
*       into the file name textboxes 
******************************************************/
int Bat_Frm::Input()
{
int i; 
char cr[1000], cr_Out[1000]; 
String ^ Str; 
String ^ s_FN;

  this->openFileDialog1->Title = "Batch File";
  this->openFileDialog1->InitialDirectory = this->s_FilePath;
  this->openFileDialog1->FileName = ""; 

  if ( this->_rbTreeMort->Checked == true ) /* if Mortality radio button is checked */ 
     this->openFileDialog1->Filter = "tre files (*.tre)|*.tre|All files (*.*)|*.*";
  else
    this->openFileDialog1->Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";

  i = (int) this->openFileDialog1->ShowDialog();
  if ( i != 1 )
    return 0; 
 
   s_FN = this->openFileDialog1->FileName;
   this->_txInputFile->Text = s_FN;    /* Put Fil Nam into the TextBox */ 
  
/* Form the output file name */
   _StrToChar (s_FN, cr); 
//   StrRepChr (cr,'.',0);         /* take off the end */     
//   strcat (cr,"-Out.txt"); 
//   Str = _CharToStr(cr); 
  
  _MakeFileName (cr, cr_Out, "-Out.csv"); 
   Str = _CharToStr(cr_Out); 
   this->_txOutputFN->Text = Str; 

   this->_txErrorFile->Clear();
   this->_txStands->Clear();
   this->_txErrors->Clear(); 
 
  return 1; 
}
/************************************************************
* Name
* Desc:
* Note-1: Flame/Scorch GUI setting is used for default values
*         if it's not entered in the input file. We check it
*         up front just in case it's needed. 
************************************************************/
int is_PFI  = 0 ;   /* one time only message switch */
void Bat_Frm::Batch_Run()
{
char cr_FNin[500], cr_FNout[500], cr_FNerr[500], cr[1500],A[500],B[500]; 
char cr_SalvFN[500]; 
char cr_ErrMes[1000], cr_FlaSco[10];  
int i,j, iN, iN_SoiErr, iS_Soil, iS_Head, i_PFI;
float f, f_FlaSco;
String ^ Str; 
FILE *fh_Err;

 /* Get the programs startup folder, folder that FuelCalc is in */
 // Str = System::Windows::Forms::Application::StartupPath;
 // _StrToChar (Str,cr_PrgPth); 

  iN = iN_SoiErr = iS_Soil = 0; 

 if ( this->_rbHeadYes->Checked )
   iS_Head = 1;
 else
   iS_Head = 0; 


 if ( this->_cbColRep->Checked == true && iS_Head == 0 ) { 
    myMessageBox ("you must check 'Yes' for 'Use Column Headings in output file' \n to create a Column Style Report"); 
    return ; } 
 
 if ( this->_rbConEmi->Checked == true )
    Str = e_ConE ;
 else if ( this->_rbConEmiSoi->Checked == true ) { 
    Str = e_Soil;
    iS_Soil = 1; }
 else if ( this->_rbTreeMort->Checked == true ) {
    Str = e_Mort;}
 else
   myMessageBox ("Create_Sample() - Logic Error");  


/* Flame Length - Scorch Height Defualts -  */ 
  if ( Str == e_Mort ) {
    if ( !_Get_TBFlo (this->_txFlaSco, &f_FlaSco, cr, "Flame/Scorch", cr_ErrMes) ) {
      myMessageBox ( "Invalid/Missing Flame/Scorch value entered on Batch Window");
      return ; }
    strcpy (cr_FlaSco,"S"); 
    if ( this->_rbFlaLen->Checked == true ) 
      strcpy (cr_FlaSco, "F" ); 
    if ( !ChkFlaSco (f_FlaSco, cr_FlaSco, cr_ErrMes)){ /* Check limitis */ 
      myMessageBox (cr_ErrMes); 
      return ; }
  }

/* Input File Name */
  Str = this->_txInputFile->Text;
  if ( Str == "" ) { 
    myMessageBox ("Please select an input file"); 
    return ; }
  _StrToChar (Str,cr_FNin); 

/* Output File Name */
  Str = this->_txOutputFN->Text;
  if ( Str == "" ) { 
    myMessageBox ("Please select an input/output file"); 
    return ; }
 _StrToChar (Str,cr_FNout); 

/* Form Error File Name based on input file name */
   _MakeFileName (cr_FNin, cr_FNerr, "-Err.txt"); 
  fh_Err = fopen (cr_FNerr, "w"); 
  if ( fh_Err == NULL ) { 
    myMessageBox ("Can't open an output error file"); 
    return ; } 

  fprintf (fh_Err, " This file contains FOFEM batch errors.\n"); 
  fprintf (fh_Err, " Some missing numeric values may be represtented as -1\n"); 
  fprintf (fh_Err, " Input file: %s \n\n", cr_FNin);
 
  Str = _CharToStr(cr_FNerr); 
  this->_txErrorFile->Text = Str; 
   
  i_PFI = 0; 
  if ( this->_rbTreeMort->Checked == true )
     i= BAT_MortExec (cr_FNin, 
                   cr_FNout,
                   &iN, 
                   iS_Head, 
                   fh_Err,
                   &i_PFI,
                   f_FlaSco,
                   cr_FlaSco,
                   cr_SalvFN);
  else 
      i = BAT_CESExec (cr_FNin,      /* input file name */
                   cr_FNout,     /* output file name */
                   &iN,          /* number of Stands processed */
                   iS_Soil,      /* 1 = do soil heating */
                   &iN_SoiErr,   /* number of soil errors */
                   iS_Head,      /* 1 = put headings in output file */
                   fh_Err,      /* error file handle */ 
                   "GUI");       /* Being run from the GUI menu */

/* Column Style Report .......................................*/
     j = 0; 
     strcpy (A,"");
     strcpy (B,"");  
     if ( this->_cbColRep->Checked == true ) {
       _MakeFileName (cr_FNout, A, "-Column.csv"); 
       if ( ColumnOutPut (cr_FNout, A, cr_ErrMes) ) 
          j++;  
       else 
         myMessageBox (cr_ErrMes);  }
     
     if ( this->_cbColRep->Checked == true && strcmp (cr_SalvFN,"" )) {
       _MakeFileName (cr_SalvFN, B, "-Column.csv"); 
       if ( ColumnOutPut (cr_SalvFN, B, cr_ErrMes) ) 
         j++; 
       else 
         myMessageBox (cr_ErrMes);  }
     
     if ( j == 1 ) 
       sprintf (cr, "Column file created.\n%s",A);
     if ( j == 2 ) 
       sprintf (cr,"Column files created.\n%s\n%s",A,B);
     if ( j > 0 ) 
       myMessageBox (cr); 
 


  f = iN;          /* # of stands processes */
  FloatToTextBox (this->_txStands,f,0);
  f = i;           /* # of errors */ 
  FloatToTextBox (this->_txErrors,f,0); 


  if ( i > 0  ) {
    sprintf (cr,"%d Errors, see error file", i);
    myMessageBox (cr); }

/* If there were Post-Fire Injury records, tell use there's no Canopy Cover calcs in the */
/*  output, (we just put zeros in) */
  if ( this->_rbTreeMort->Checked == true ) { 
    if ( i_PFI > 0 && is_PFI == 0 ) {   /* ONLY show message once */ 
      strcpy (cr, "NOTE:\n"); 
      strcpy (cr, "  Post-fire Injury batch outputs do not include Stand Canopy Cover totals,\n");  
      strcat (cr, "the total calculations require tree heights which are not included with\n");
      strcat (cr, "the inputs.\n");    
      myMessageBox (cr);
      is_PFI++; }                       /* One time only switch */
  }

  fclose (fh_Err); 

} 


/****************************************************
*
*
****************************************************/
int Bat_Frm::Create_Sample()
{
int i; 
char cr_FN[500];
String ^ s_FN;
String ^ Str; 

 if ( this->_rbConEmi->Checked == true )
    Str = e_ConE ;
 else if ( this->_rbConEmiSoi->Checked == true ) 
    Str = e_Soil;
 else if ( this->_rbTreeMort->Checked == true )
    Str = e_Mort;
 else
   myMessageBox ("Create_Sample() - Logic Error");  

  this->saveFileDialog1->Title = "Batch File Save Sample";
  this->saveFileDialog1->InitialDirectory = this->s_FilePath;
  this->saveFileDialog1->FileName = Str; 
 

  if ( this->_rbTreeMort->Checked == true ) /* if Mortality radio button is checked */ 
     this->openFileDialog1->Filter = "tre files (*.tre)|*.tre|All files (*.*)|*.*";
  else
     this->saveFileDialog1->Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";

  i = (int) this->saveFileDialog1->ShowDialog();
  if ( i != 1 )
    return 0; 

  s_FN = this->saveFileDialog1->FileName;
  _StrToChar(s_FN,cr_FN);

  if ( Str == e_ConE ) 
     Bat_SampleFile (cr_FN,"Consume");

  if ( Str == e_Soil ) 
     Bat_SampleFile (cr_FN,"Soil");

  if ( Str == e_Mort ) 
     Bat_MakSamMort (cr_FN); 

 return 1; 
} 

void Bat_Frm::Set_FilePath(String ^ s)
{
  this->s_FilePath = s; 
}


/***************************************************************
* Name: ColumnOutPut
* Desc: Create the batch column style output .csv file
*       Files is created from the standard batch output file
*       using the 2 rows of column headings in the standard output
*       file to determine the names and the number of column data
*       to output. 
*      
****************************************************************/
int ColumnOutPut (char cr_FNin[], char cr_FNout[], char cr_ErrMes[]) 
{
int i, j, i_FldNum;
#define e_cr 9000
char cr[e_cr]; 
char cr_FldNam[100], cr_Fld[100]; 
char *a, cr_PlotId[100], A[100]; 
char  cr_Data[100]; 
FILE  *fh; // , *fh_Out:    
FILE *fh_Out; 

  strcpy (cr_ErrMes,""); 

  fh = fopen (cr_FNin,"r"); 
  if ( fh == NULL ) {
    sprintf (cr_ErrMes, "Can't open input file:\n %s", cr_FNin); 
    return 0; }

  fh_Out = fopen (cr_FNout, "w");
  if ( fh_Out == NULL ){
    sprintf (cr_ErrMes, "Can't Open output file:\n %s",cr_FNout); 
    return 0; }

  i_FldNum = 2; 

/* Start at begin of file and look for the first */
/* Column heading row */ 
  while (1) {
     a = fgets (cr,e_cr,fh);
     if ( a == NULL )   /* this might happen if file is empty */ 
       goto Close; 

     _GetFld (cr,1,cr_Fld);    /* look for column heading */
     if ( !stricmp (cr_Fld, "Plot" ))
       goto Head1;
     if ( !stricmp (cr_Fld, "Std" ))
       goto Head1;

     continue;      

Head1:
/* Have 1st Header Row */ 
    _GetFld (cr,i_FldNum, cr_Fld); 
    if ( !strcmp (cr_Fld,"") )
      goto Close; 
    strcpy (cr_FldNam,cr_Fld);

/* Read 2cd Heading Row */ 
     a = fgets (cr,e_cr,fh);
    _GetFld (cr,1,cr_Fld); 
    _GetFld (cr,i_FldNum, cr_Fld);
    strcat (cr_FldNam, "-"); 
    strcat (cr_FldNam,cr_Fld);  /* form column name with both row headings */

/* 1st Data Row */ 
   for ( i = 0; i < 300000; i++ ) {
     a = fgets (cr,e_cr,fh);
     if ( a == NULL ) {  /* end of file */
       fseek (fh,0L,SEEK_SET);  
       i_FldNum++; 
       if ( i_FldNum == 400 )   /* Shouldn't happen */  
         goto Close ; 
       break; }

    _GetFld (cr,1,cr_PlotId); 
    _GetFld (cr,i_FldNum,cr_Data); 

     fprintf (fh_Out,"%s,%s,%s\n", cr_PlotId, cr_FldNam,cr_Data); 
   }  /* for i */ 

 }  /* while (1) */ 

Close: 
  fclose (fh);   
  fclose  (fh_Out);
  return 1;  
}


/************************************************
* Name: _GetFld
* Desc: Get the specifided token from the string
*       ALWAYS send in a 1 (iX) first to start
*       a new string, after that you can make 
*       only ONE call to get the next token you
*       want, after that you'll have to reset
*       with new string and iX = 1
*  In:  iX   1 -> n
* Out:  cr_Fld...token, "" when no more
* Ret:  0 when no more tokens
************************************************/

char AAA[10000]; 
int _GetFld (char cr[], int iX, char cr_Fld[]) 
{
int i; 
char *a;
  strcpy (cr_Fld, "" ); 
  if ( iX == 1 ) { 
    _SetAAA(cr);          /* See notes in _SetAAA() */ 
    a = strtok (AAA,",");  /* Establish the string to tokenize */
    strcpy (cr_Fld,a);    /* and get first token */
    return 1; }

  for ( i = 2; 1000; i++ ) {
    a = strtok (NULL,",\n\r"); 
    if ( a == NULL ) 
      return 0; 
    if ( i == iX ){
      strcpy (cr_Fld,a);
      return i;}         
  } 
}

/*******************************************************
* Name: _SetAAA
* Desc: used by _GetFld
*       copy cr to AAA inserting a blank ' ' inbetween
*       any double commas  ,, -->  , ,
*       AAA will get used by strtok(), strtok needs to 
*       have the ' ' between commas, strtok() also 
*       trashes the string it working with 
*********************************************************/
void _SetAAA (char cr[])
{
int i,j;

   if ( !strcmp (cr, "" ))
     strcpy (AAA,"");

   for ( i = 0; i < 10000; i++ )  
      AAA[i] = NULL; 

   j = 0; 
   for ( i = 0; i < 10000; i++ )  {
     if ( cr[i] == NULL )
        break; 
     if ( !memcmp (&cr[i], ",,",2) ) {
       strcpy (&AAA[j], ", ");
       j += 2; 
       continue ; }

     AAA[j] = cr[i]; 
     j++; 
   }   /* for i */ 

  j = j - 2; 
  if ( AAA[j] == ',' )    /* special case, if comma on end of string */
    strcpy (&AAA[j],", "); 
}



