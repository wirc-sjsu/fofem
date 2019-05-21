/******************************************************************
* Name: wnd_def.h
* Desc: defines used on main form
*
*******************************************************************/
#pragma once 

#define e_Ini_FN  "fofem.ini" 
#define e_PrjNam  "fofem.prj"         /* default file name */

/* Type of report totals, prefire, post fire, salvage */
#define  e_RTpre  0     /* Prefire */
#define  e_RTpos  1     /* Post Fire */ 
#define  e_RTsal  2     /* Salvage */ 



#define e_OSXP "WinXP"
#define e_OSWn "Win78"
#define e_OSOth  "Other"

void _ArgBlank (char cr[]);
void _ArgFill (char cr[]);


#define e_FOF_Spp  "fof_spp.csv"  //  "fof_spp.dat"  /* Mortality species input file */
#define e_FOF_SAF  "fof_saf.csv"
#define e_FOF_NVCS "fof_NVCS.csv"
#define e_FOF_FCCS "fof_FCCS.csv"
#define e_FOF_FLM  "fof_FLM.csv" 


#define  e_NoIg  "\n Notice:-->   Simulation indicates that fuel will not ignite.\n"
#define  e_NoIgw   " Notice:-->   Simulation indicates that fuel will not ignite."

/* Soil Heating Efficency, get used on the Setting Window, and soil model  */
#define  e_EffSoilDuff  "100"
#define  e_EffSoilWL    "15"
#define  e_EffSoilHS    "10" 



/* Defaults for GUI Textboxes */
#define e_FolBraConDef  "75"    /* Foliage Branch Consmd % default */ 
#define e_RotLogDef     "50"    /* Rotten Log % default */
#define e_RotLogDefFlt  50.0


/* Moisture ComboBox Items ............................... */
#define e_MT_VeryDry "VeryDry"
#define e_MT_Dry      "Dry"
#define e_MT_Moderate "Moderate"
#define e_MT_Wet       "Wet" 


/* Fuel Category ComboBox Items............................... */
#define e_SAF_SRM "SAF/SRM"
#define e_NVCS    "NVCS"
#define e_FCCS    "FCCS"
#define e_FLM     "FLM"
#define e_FFI     "FFI" 


/* Region ComboBox Items */ 
#define e_Interior_West   "InteriorWest"
#define e_Pacific_West    "PacificWest"
#define e_North_East      "NorthEast"
#define e_South_East      "SouthEast"

/* Region numbers used in the fof_spp.dat file to identify region */
#define eN_Interior_West   1
#define eN_Pacific_West    2
#define eN_North_East      3
#define eN_South_East      4


/* Fuel Category ComboBox Items */ 
#define e_FCa_Natural  "Natural"
#define e_FCa_Pile    "Piles"
#define e_FCa_Slash   "Slash" 

/* Season Combo Box Items .................................. */
#define e_Winter  "Winter"
#define e_Spring  "Spring"
#define e_Summer  "Summer"
#define e_Fall    "Fall"

/* Log Distribuion */
#define e_Even    "Even"
#define e_Right   "Right"
#define e_Left    "Left"
#define e_EndWD     "End"
#define e_Center  "Center"
#define e_Fixed   "Fixed"  /* Used for FCCS */

/* Soil Type ComboBox Items...........                        */
#define  e_LoaSke     "Loamy-Skeletal"
#define  e_FinSil     "Fine-Silt"
#define  e_Fin        "Fine"
#define  e_CoaSil     "Coarse-Silt"
#define  e_CoaLoa     "Coarse-Loamy"


/* Outputs ComboBox Items................................ */
#define e_OP_Cons   "Consumed"
#define e_OP_Post   "Post"
#define e_OP_PCCons "Consumed %"

/* Fuel Load TextBox ContextMenu Items.....................*/
/* These match exactly with Name I used in the Form Designer */
#define e_Light  "Light"
#define e_Sparse "Sparse"
#define e_Typical "Typical"
#define e_Heavy   "Heavy"
#define e_Abundant "Abundant" 
#define e_User      "User"

/* Fuel TextBox Names - these match exactly with the Names */
/*  used in Form Designer to create the textboxes */
#define e_txLitter  "_txLitter"
#define e_tx1Hr     "_tx1Hr"
#define e_tx10Hr    "_tx10Hr"
#define e_tx100Hr   "_tx100Hr"
#define e_tx1kHr    "_tx1kHr"

#define e_txDuff     "_txDuff"
#define e_txHerb     "_txHerb"
#define e_txShrub    "_txShrub"
#define e_txFoliage  "_txFoliage"
#define e_txBranch   "_txBranch"

/*-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/*                            Moisture Method                               */
#define e_Entire   "Entire"             /* Duff Moisture Methods            */
#define e_Lower    "Lower"              /*  tells what method was used       */
#define e_NFDR     "NFDR"               /*  to measure the duff moist.       */
#define e_Adj_NFDR "AdjNFDR"



/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/*                     Fuel Load Inputs From GUI                             */
typedef struct {

   float    f_Litter;
   float    f_1Hr;
   float    f_10Hr;
   float    f_100Hr;
   float    f_1kHr;

   float    f_PerRot;

   float    f_Duff;
   float    f_DuffDepth;
   float    f_Herb ;
   float    f_Shrub;

   float    f_MoistDuff;
   float    f_Moist10Hr;
   float    f_Moist1kHr;
   float    f_MoistSoil; 

   float    f_Foliage;
   float    f_Branch;
   float    f_PerFB;     /* Percent of Fol Bra to be consumed */

   char   cr_Region [100];
   char   cr_Season[50];
   char   cr_LogDist[50]; 
   char   cr_FuelCat[50];    /* Natural, Pile,... */
   char   cr_Moisture[50];
   char   cr_FuelClass[50];
   char   cr_CoverType[1000];   
   char   cr_SoilType[100]; 


   char   cr_EmisFlame[200];
   char   cr_EmisSmold[200];
   char   cr_EmisDuff[200];
   char   cr_EmisMethod[100]; 

   char  cr_ErrMes[3000];

/* Soil Heat fire intensity efficency - for JR's test*/
   float f_SoilWlEff;
   float f_SoilHsEff; 
   float f_SoilDuffEff; 

} d_FLI;
