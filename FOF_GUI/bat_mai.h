/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: bat_mai.h
* Desc:
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/

/* used to form Mort Salvage output file */
#define  e_SalvExt  "-Salvage.csv"


#define   eC_StdId     100       /* Max size of a Stand Id            */

/* Column heading for input batch files, this is used to verify the column heading row */
#define   e_PlotId   "PlotId"

void ErrBat_Window (char A[], char B[]); 
int  BAT_MortExec (char cr_InFN[], char cr_OutFN[], int *aiN_Std, int iS_CL, FILE *fh_Err, int *ai_PFI, float f_FlaSco, char cr_FlaSco[], char cr_SalvFN[]);

int  BAT_CESExec (char cr_InFN[], char cr_OutFN[], int *iN, int iS_Soil,
                  int *ai_SoilErr, int iS_CL, FILE *fh_Err, char cr_Mode[]);

// int  Bat_MakSamSoil (char cr_FN[]);
int  Bat_SampleFile (char cr_FN[], char cr_Mode[]);


int   Bat_MakSamCES (char cr_FN[], int iS_Soil);
int   Bat_MakSamMort (char cr_FN[]);

int ChkFlaSco (float f_FS, char cr_FS[], char cr_Err[]);