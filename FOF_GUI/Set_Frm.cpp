
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Set_Frm.h"


/***********************************************
* Name: isEmisOl
* Desc: is the Old Emission Factor radio button 
*       checked
*       Old is the original way that FOFEM/burnup
*       does the emissions
***********************************************/
bool Set_Frm::isEmisOld()
{
   if ( this->_rb_EmiOld->Checked == true )
     return true;
   return false;
}
/***********************************************
* Name: EU_Unit
* Desc: What is the Emission Output Unit set as
*
***********************************************/
int Set_Frm::EU_Unit(char cr[])
{
String ^ Str;
  Str = this->_cb_EmiUni->Text;
  if ( Str == e_EUcbTon )
    strcpy (cr, "Ton");
  else if ( Str == e_EUcbPnd )
    strcpy (cr,"Pound");                
  else
    strcpy (cr,"Metric"); 
  
  return 1;   
}

