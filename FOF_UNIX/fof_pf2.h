/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_pf2.h   Post Fire Injury Mortality Table
* Desc:
*
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
#pragma once 


/* Not Applic code is used in Spe Wnd by user when a particular param/input  */
/*  is not required                                                          */
/* The Miss value is used for No Applic in number fields                     */
#define e_NA    "X"                     /* Not applic code                   */
#define e_PFI_Miss -1.0

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
typedef struct {

/* Inputs from Main Wnd Spe Wnd                                              */
       char cr_Spe[30];

       float f_Den;                     /* Density - trees per acre          */

       float f_CRP;                     /* Crown Ratio % in length or Volume */
       float f_DBH;                     /* dia breast height                 */

       float f_CKR;                     /* Cambium Kill rating               */

#define e_BtlYes  "Y"
#define e_BtlNo   "N"
       char  cr_Btl[30];                /* Beetle                           */

       float  f_PMCut;                  /* Probability Kill Cut off point    */

/* This get set, they are not inputed by user                                */
       char cr_Eq[10];
       float f_Prop;

       bool b_Graph;                   /* User wants to graph = true */ 

       } d_PFI;





















