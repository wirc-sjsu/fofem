/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: bat_bif.h
* Desc: Batch Input File defines
*
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/


/* Input Tree file switch - Size Class Probability */
/* There can be 1,2 or 3 switches used */
/* Ex:    #SalDia-1  1   5   0.6 */
/*        #SalDai-2  5   8   0.4 */
/*        #SalDia-3  8  20   0.1 */
#define e_SC   "#SalDia"

/*.....................................................*/
/* User Equations Numbers  */
#define e_IF_UseEqu  "#ConsumptionEquation" 

/*.....................................................*/
/* Emission Factor Group Number Switches */
#define e_IF_Flame "#EFG-STFS"
#define e_IF_Smold "#EFG-CWDRSC"
#define e_IF_Duff  "#EFG-DuffRSC"

/* This switch when present signals to output the full 204 expanded emissions */
/*  rather than the basic 7 */
#define e_IF_Full  "#EFG-Full"  

/*..................................................... */
#define e_IF_EmisFile  "#Emission-File"   /* make an emision file of each plot */

/* use a burnup parameter file in addition to the regular inputs files */
#define e_IF_BurnUpFN "#BurnUpFile"

/*...........................................................................*/
/* Batch Consumed Input File Switches                                        */
/* Tells when data is format for 1000 hour size classes                      */
#define e_IF_1kSC  "#1k-SizeClass"

/* Tells when data is formated for Coastal Pine Longleaf pine                */
#define e_IF_CPLP  "#CP-Longleaf"

/*...........................................................................*/
int BIF_Get (d_CI *a_CI, d_SI *a_SI, char cr_StdId[], char cr_ErrMes[],
             int iS_Soil, char cr_1kFMt[]);

int  BIF_Open (char cr_InFN[]);
void BIF_Close  (void);
int  BIF_Switch (char cr_Fmt[], int iS_Soil, char cr_ErrMes[]);
int BIF_Switch2 (char cr_BurnUpFN[], char cr_EmisSw[], char cr_ErrMes[]);
int BIF_SwitchEG (char cr_Flame[], char cr_Smold[], char cr_Duff[], char cr_Full[], char cr_ErrMes[]);
