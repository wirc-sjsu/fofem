/*******************************************************************
* Name: Wnd_PFI.cpp    Post Fire Injury 
*
********************************************************************/
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "Wnd_Mai.h"
#include "wnd_util.h"
#include  "fof_util.h"
#include "cvt_cvt.h" 
#include  "fof_smt.h"
#include  "fof_iss.h"
#include  "fof_mrt.h"
#include  "fof_slk.h"
#include  "fof_st.h"
#include  "fof_mt.h"



/***************************************************************
* Name: isPFIChecked
* Desc: check the Pre and Post Fire Injury radio buttons on
*       the GUI Mortality Tab to see if the Post Fire Injuty
*       is checked
*  Ret: 1 it's checked, else 0
***************************************************************/
bool Wnd_Mai::PostFire_Checked()
{
 // I delete the PostFile Checkbox 
 // if ( this->_rbPostFire->Checked == true )
 //   return true;
  return false; 

}