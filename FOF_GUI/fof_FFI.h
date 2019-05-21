/*--------------------------------------------------*/
/*                  FFI Tree File inputs            */
/* Note-1: Crown Scorch Height, not presently using this */
/*         but possible use in the future */
typedef struct { 
    char cr_Plot[300];
    char cr_MonSta[100];
    char cr_MonOrd[100];

    char cr_Spe[50];
    float f_Den;
    float f_DBH;    
    float f_Hgt;            
    float f_CrnHgt;         /* Crown Height - height to crown */
    char cr_Status[50];     /* Tree Status, */
    char cr_CrnCls[50];     /* Crown Class */
    int   i_CrnRat;         /* Crown Ratio, 0,10,20--> 100 */
  
    float f_BolChaHgt;      /* Bole Char Height */
 
/* Inputs for the Crown Scorch Equ Typ (Post Fire) */
    float f_CrnScoPer;         /* Crown Scorch Percent */
    float f_CrnScoHgt;      /* Crown Scorch Height - See Note-1 above */
    float f_CKR;            /* Cambium Kill Rating */
    char  cr_BeeDam[50];    /* Beetle Damage "Y" "N" */ 

    char   cr_EquTyp[50];   /* Equation Type */ 
   
    float f_FlaSco;         /* Flame length or scorch height */
    char  cr_FS[20];        /* "F" or "S" tells whats in f_FlaSco */
    char  cr_FirSev[20];    /* Fire Severity  "L" or "",  Low */ 

   } d_FFItre;