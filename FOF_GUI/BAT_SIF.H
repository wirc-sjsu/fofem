/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: bat_sif.h
* Desc: defines for Species Input File
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/

#define e_EOF  "~EOF"

#define e_IF_PPCP "#PPCP"

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* Batch Mortality Species Input File Struct                */
/* This struct will hold the Prefire and Post-fire inputs   */
/* Both type input records are initialy read into the Prefire fields   */
/*  the cr_FirSev field is then checked to see if its a PFI record, if */
/*  so the data is moved to its respect PFI fields                     */
/* Note-1: This field needs to be read in as a char string in order to  */
/*  accomondate both input record types, for a Prefire record it can be */
/*  convert and put into f_Sch, for Post-fire its put into the         */
/*  put into cr_pBtlKil  */  


#define e_PFI "PFI"                /* See Notes below for Post Fire Injury */ 
#define e_Reg "Reg"  

#define eC_SIFmax 40

typedef struct  {
   char  cr_RecTyp[20];            /* Record Type, see #define above */ 

   char cr_StdId[100];             /* Stand Id                               */
   char cr_Spe[eC_SIFmax];         /* Species Name                           */
   float f_Den;                    /* Density                                */
   float f_DBH;                    /* DBH                                    */
   float f_Hei;                    /* Height                                 */
   float f_CroRat;                 /* Crown Ratio                            */

   float f_Sch;                    /* Scorch Height-Flame Length             */
#define eC_Sch 20
   char  cr_Sch[eC_Sch];           /* See Note-1 above */

#define eC_FS 10
   char  cr_FS[eC_FS];             /* "S" or "F"  Scorch Hgt of Flame Len    */

#define eC_FirSev 10
#define e_Low "L"
   char  cr_FirSev[eC_FirSev];     /* "L", "PFI" or blank */

   float  f_RelHum;                /* Relative Humidity                      */



/* . . . . . . . . . . . . . . . . . */
/*  PFI - Post Fire Injury inputs    */ 
/* These fields get filled on a PFI record,   */
/* see above cr_FirSev = e_PFI indicated rec type */
    float f_pDen;                   /* Density */ 
    float f_pCrnDam;                /* Crown Damage */
    float f_pDBH;
    float f_pCKR;                   /* Cambium Kill Ratio */ 
    char  cr_pBtlKil[20];           /* Beetle Kill "Y" "N" */
    float f_pPMCut;                 /* Probability Mortality Cut off */ 


}  d_SIF;

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
int  SIF_Next (d_SIF *a_SIF, char cr_ErrMes[]);
int  SIF_Open (char cr_FN[]);
void SIF_Close (void);
void SIF_Clean (d_SIF *a_SIF);
void SIF_Display (d_SIF *a_SIF);
int SIF_isPPCP (void);



int SIF_isPFI (d_SIF *a_SIF);
