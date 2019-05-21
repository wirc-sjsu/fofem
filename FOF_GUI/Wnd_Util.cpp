
/*******************************************************************************
* Name: Wnd_Util.cpp
* Desc: Window / Form utilities
* 
*******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS    /* Gets rid of string function warnings */

#include <windows.h> 
#include <winbase.h> 
#include <stdio.h>
#include <string.h>
#include <math.h> 

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;
using namespace::System::IO;

#include "Wnd_Util.h"
#include "fof_util.h"

/***********************************************************************
* Name:  _RndEmiPnd
* Desc: used to round emission amounts that are in pound unitsl 
*       and return as a whole number  
************************************************************************/
float   _RndEmiPnd(float f)
{
int i; 
float g;
 //if ( f < 0.0001 ) 
//   return 0;
 g = f + 0.5;
 i = g;
 return (float) i; 

}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: GetDate
* Desc: Get the current date
*  Out: cr_Date.....date comes back as mm/dd/yyyy
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void GetDate(char cr[]) 
{
SYSTEMTIME st;
   GetLocalTime (&st);
   sprintf (cr, "%d/%d/%d \n", st.wMonth, st.wDay, st.wYear);
}

/****************************************************
* Put a char[] to a textbox
*
****************************************************/
void _PutTextBox (TextBox ^ tb, char cr[])
{
String ^ Str;
   Str = _CharToStr (cr); 
   tb->Text = Str; 

}

/*********************************************************
* Get the text from a TextBox
* Out: cr...."" or text 
*******************************************************/
int _GetTextBox (TextBox ^ tb, char cr[])
{
   strcpy (cr,"");

   if ( String::IsNullOrEmpty(tb->Text) ) 
     return 0; 

  _StrToChar (tb->Text,cr);
   return 1; 

}
/********************************************************************************
* Set a ComboBox to the specifed Item in its list. 
*********************************************************************************/
int _SetComboBox (ComboBox ^cb, String ^Str)
{
int i; 

  i = (int) cb->Items->Count;   /* could blowup below trying to excess empty combobox */
  if ( i <= 0 ) 
    return 0; 

   if ( String::IsNullOrEmpty(Str) )
     return 0; 
   i = cb->FindString (Str,0);
   if ( i < 0 )                    /* Item wasn't found in ComboBox's list of items */ 
     i = 0;                       

   cb->SelectedIndex = i;          /* Select combobox with found item */

   return i;
 }

/****************************************************************************
* Name: _GetComboBox
* Desc: Get the text that is selected in the combobox
*   In: cb....combobox
*  Out: cr....char string
*  Ret: ......the String 
* Note-1 I'm not sure, but I check it like this for Null just incase
*         or else it might blow up. 
****************************************************************************/
String ^ _GetComboBox (ComboBox ^cb, char cr[])
{
String ^Str;    
  Str = "";
  strcpy (cr,""); 

  if ( String::IsNullOrEmpty (cb->Text ) )  /* See Note-1 above */
    return Str; 

  Str = cb->Text; 
 _StrToChar (Str, cr);
  return Str;
}

/**********************************************************************************
* Name: _FloToStr 
* Desc: Convert a float to a string 
*   In: f...float
*       i_Per...number of decimal points to format to
*       NOTE....the sprintf() will automaticaly do the proper rounding off 
* 
*********************************************************************************/
String ^  _FloToStr (float f, int i_Per)
{
char cr[22];
String ^Str; 

  if ( i_Per == 0 ) 
    sprintf_s (cr,"%1.0f",f);  
  else if ( i_Per == 1 ) 
    sprintf_s (cr,"%1.1f",f);  
  else if ( i_Per == 2 ) 
    sprintf_s (cr,"%1.2f",f);  
  else if ( i_Per == 3 ) 
    sprintf_s (cr,"%1.3f",f);  
  else    /* should get this...but */
    sprintf_s (cr,"%1.0f",f); 
 
  Str = Marshal::PtrToStringAnsi((IntPtr) (char *) cr);

   return Str;

}

/*******************************************************************
* Name: myMessageBox
* Desc: MessageBox I setup to just use char[]
*
*******************************************************************/
void  myMessageBox ( char cr[])
{
String ^Str ;
    Str = Marshal::PtrToStringAnsi((IntPtr) (char *) cr);	
    MessageBox::Show(Str,"FOFEM");
}

/**********************************************************************************
* Convert a char[] to a String 
*********************************************************************************/
String ^  _CharToStr (char cr[])
{
String ^ Str;
  Str =  Marshal::PtrToStringAnsi((IntPtr) (char *) cr);
  return Str; 
}

/*******************************************************************************
* Convert a String to char[]
*******************************************************************************/
int  _StrToChar (String ^Str, char cr_To[])
{
char *adr;  
   if ( String::IsNullOrEmpty(Str) ) {
     strcpy (cr_To,"");
     return 1; }
   adr = (char*) Marshal::StringToHGlobalAnsi(Str).ToPointer();
   strcpy (cr_To,adr);
   Marshal::FreeHGlobal(IntPtr(adr));
   return 1; 
}

/*************************************************************************
* Name: FloatToTextBox
* Desc: put a float value into a window textbox
*    
*************************************************************************/
void FloatToTextBox (TextBox^ a, float f, int i_Per)
{
String ^Str;
char cr[100]; 

  if ( i_Per <= 0 ) 
     sprintf (cr,"%1.0f",f);
  else if ( i_Per == 1 ) 
     sprintf (cr,"%1.1f",f);
  else if ( i_Per == 2 ) 
     sprintf (cr,"%1.2f",f);
  else if ( i_Per == 3 ) 
     sprintf (cr,"%1.3f",f);
  else 
     sprintf (cr,"%1.4f",f); 


  if ( f == 0 ) 
     strcpy (cr,"0"); 

  Str = Marshal::PtrToStringAnsi((IntPtr) (char *) cr);
  a->Text = Str;

}


/*******************************************************************************
* Name: _Get_TBFol
* Desc: Get a float number from a textbox
* NOTE: Negative numbers are considered to be an error 
*       Blank/Empty textbox is considered to be an error 
*   In: a ... textbox
*       cr_Name...name/text to use in error message,
*  Out: af...the number, 
*            NOTE NOTE this does not get set if an error occurs
*            this is so the call can put their own missing value  
*       cr_Out.....text from textbox, 
*       cr_ErrMes
*******************************************************************************/
int _Get_TBFlo (TextBox^ a, float *af, char cr_Out[], char cr_Name[], char cr_ErrMes[])
{
float f; 
char *adr;  
char cr_BoxText[100];
char c; 
  
   strcpy (cr_ErrMes,""); 
   adr = (char*) Marshal::StringToHGlobalAnsi(a->Text).ToPointer(); 
   strcpy (cr_BoxText,adr); 
   Marshal::FreeHGlobal(IntPtr(adr));

   c = Get_NumTyp (cr_BoxText); 
   if ( c == 'X' ) {
      if ( !strcmp (cr_BoxText,"") )
        strcpy (cr_BoxText,"blank");
      sprintf (cr_ErrMes, "%s contains (%s) an invalid number",cr_Name,cr_BoxText); 
      return 0; }
 
   f = (float)atof (cr_BoxText); 
   if ( f < 0 ) {
      sprintf (cr_ErrMes, "%s contains (%s) negative number",cr_Name,cr_BoxText);  
      return 0; }

   strcpy (cr_Out,cr_BoxText); 
   *af = f; 
   return 1; 
}

/*******************************************************************************
* Name: _Get_DGFol
* Desc: From a DataGridView Get a float number 
* NOTE: Negative numbers are considered to be an error 
*       Blank/Empty textbox is considered to be an error 
*   In: dg ... DataGridView
*       cr_Name...name/text to use in error message,
*  Out: af...the number, 
*            NOTE NOTE this does not get set if an error occurs
*            this is so the call can put their own missing value  
*       cr_Out.....text from textbox, 
*       cr_ErrMes
*******************************************************************************/
int _Get_DGFlo (DataGridView ^dg, int row, int col, float *af, char cr_Out[], char cr_Name[], char cr_ErrMes[])
{
float f; 
char c; 
String ^ Str; 

   strcpy (cr_Out,""); 
   strcpy (cr_ErrMes,"");    
  
   if ( String::IsNullOrEmpty (dg->Rows[row]->Cells[col]->FormattedValue->ToString() ) ){   
     sprintf (cr_ErrMes, "Invalid/Missing Value - %s",cr_Name); 
     return 0; }

   Str = dg->Rows[row]->Cells[col]->FormattedValue->ToString();

  _StrToChar (Str,cr_Out); 

   c = Get_NumTyp (cr_Out); 
   if ( c == 'X' ) {
      if ( !strcmp (cr_Out,"") )
        strcpy (cr_Out,"blank");
      sprintf (cr_ErrMes, "Invalid/Missing Value - %s %s",cr_Name,cr_Out); 
      return 0; }
 
   f = (float)atof (cr_Out); 
   if ( f < 0 ) {
      sprintf (cr_ErrMes, "Invalid/Missing Value - %s %s",cr_Name,cr_Out);  
      return 0; }
 
   *af = f; 
   return 1; 
}


/******************************************************************
* Name: _MakeFileName
* Desc: Replace or add a file name extention on to an existing 
*        path file name. 
* NOTE--> caller needs to supply a period in front of extention 
*          if they want one.
*       I wrote this to handle path names that might have embedded
*       spaces or periods in them, 
*  In: cr_In...path file name and extention if any
*      cr_Apd..Extention or text to append, ex:  ".tre" "-err.txt"
* Out: cr_Out..result
*      
******************************************************************/
int _MakeFileName (char cr_In[], char cr_Out[], char cr_Apd[])
{
String ^ Str;
String ^ Path, ^ File;

  Str = _CharToStr(cr_In); 
  Path = Path::GetDirectoryName(Str); 
  File = Path::GetFileNameWithoutExtension(Str); 
  if ( Path != "" ) 
     Str = Path + "\\" + File;
  else 
     Str = File; 

  _StrToChar (Str,cr_Out);
  strcat (cr_Out,cr_Apd); 
  return 1; 
}


/************************************************************************
* Name: _CBI_Load_DGV
* Desc:  Load a list of Items to a DataGridView ComboBox, 
*        Send in 1,2,or 3 items in the order you want them loaded
*   cb...ComboBox
*   A,B,C... Items to load
*   iX       1,2,3  which item to show as selected 
************************************************************************/
void _CBI_Load_DGV (DataGridViewComboBoxColumn ^ cb, char A[], char B[], char C[], int iX)
{ 
int i,j;
String ^ Str; 
String ^ s_Def;


   if ( strcmp (A,"") ) {
     Str = _CharToStr (A); 
     cb->Items->Add(Str); }         /* add it to the combobox */

   if ( strcmp (B,"") ) {
     Str = _CharToStr (B); 
     cb->Items->Add(Str); }         /* add it to the combobox */

   if ( strcmp (C,"") ) {
     Str = _CharToStr (C); 
     cb->Items->Add(Str); }         /* add it to the combobox */

 
   if ( iX == 1 )  
     s_Def = _CharToStr(A); 
   else if ( iX == 2 )  
     s_Def = _CharToStr(B); 
   else if ( iX == 3 )  
     s_Def = _CharToStr(C); 
   else 
     s_Def = "";

   cb->DefaultCellStyle->NullValue = s_Def; 
   cb->ValueMember = s_Def; 

}
