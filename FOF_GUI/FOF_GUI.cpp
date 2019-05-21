// FOF_GUI.cpp : main project file.

#include "Wnd_Mai.h"
int  CLB_Main (String ^ Str);
// using namespace FOF_GUI;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
int i,j; 
String ^ Str;
String ^ A; 
 
/* Check for anything on the command line, - Command line batch mode */
   A = ""; 
   j = args->Length;               /* Number of args */
   for ( i = 0; i < j; i++ ) {  
     Str = args[i]->ToString(); 
     A = A + Str;                  /* Build a single string */
     A = A + " "; }
 
// test................................
// You can test the Batch Command line by 
// putting the input file and Emission Factor file in the
//  'FOF_GUI' folder - that's where the program will default to looking
//  for the files in 
// NOTE: DON'T FORGET --> emission_factors.csv - put a copy into the FOF_GUI folder 
 //   A = "C _Test-In.txt _Test-Out.txt _Test-Run.txt _Test-Err.txt  H" ;

//  A = "C _ConE-In.txt _ConE-Out.csv _ConE-Run.txt _Cone-Err.txt  H";


// A = "M _Test-CLB.tre _Test-CLB-Out.csv _Test-CLB-Run.txt _Test-CLB-Err.txt  H";


 // A = "M _M-In.tre _M-Out.csv _M-Run.txt _M-Err.txt  H";


// test......................................

  if ( A != "" ) {
     CLB_Main (A);                 /* Check and run Batch */
     return 0; } 


	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew Wnd_Mai());
	return 0;
}
