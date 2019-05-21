/***************************************************************************************
* Name: FOF_NES.h
* Desc: New Emission System
*       The new emission system contains the extensive list of emission factors
*       and replace the original way we did emssions 
* Date: 7-30-14
***************************************************************************************/

/* Critical Fire Intensity divider, this determines if flame or smoldering */
/* emissions factors will get used.                                        */
#define e_CriInt 15.0     

#define e_EmiFN   "Emission_Factors.csv"

/* Default Emission Group numbers, */
#define e_DefFlaGrp   "3"     /* Changed DL requested 12-12-14  old default "2" */
/* DL changed 1-12-2015 */
#define e_DefDufGrp  "8"
#define e_DefSmoGrp  "7"
/* old */
//  #define e_DefDufGrp  "7"
// #define e_DefSmoGrp  "6"


int NES_Read (char cr_Path[], char cr_ErrMes[]);

int NES_Get_Factor(char cr_GrpNum[], int ix, char cr_CovTyp[], float *af_Factor,
			   char cr_Name[],  char cr_Code[]);

void NCT_Display(); 
void NES_Display ();
int  NES_GetGrpCov (int ix, char cr_GrpNum[], char cr_CovTyp[], int *aiS_Def);

int NES_Get_MajFactor(char cr_GrpNum[],
                     float *CO, float *CO2, float *CH4, float *PM25,
                     float *PM10, float *NOX, float *SO2);

float NES_Get_CriFirInt (); 