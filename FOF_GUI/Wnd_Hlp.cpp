#include <stdio.h>
#include <math.h> 
#include <string.h>


#include "Wnd_Mai.h" 

#include "Wnd_Util.h"

#define  MI_OpePro "Opens a project file. The Work folder will get set to " \
              "the folder where the project file is located. If you go to " \
              "a different folder to open a file, that will become your new " \
              "Work folder and be shown in the Project window." 


#define  MI_SavPro "Save will only save your screen settings to a project file, "\
               "it will not change your Work folder setting. You are free "\
               "to save to any project file, whether it's your active, an "\
               "existing one, or even a new one."


#define  MI_NewPro "New will save your screen settings to a project file, "\
               "and change your Work folder setting."


#define  MI_Exit  "Exit the FOFEM Program"


#define  tx_ProFil "This window shows your active project file name."\
               "The file path in front of the project file name will "\
               "be used as your Work folder. Whenever you go to open "\
               "or save a file the Work folder path will be displayed "\
               "as a default. "
            
#define cb_Region  "Select one of the four geographic regions. "\
                   "Only the cover types found in the selected "\
                   "region will be presented in the Cover Type Window."

#define cb_CoverClass  "Select the most appropriate cover type classification for "\
                    "this region.  You may use Society of American Foresters Society "\
                    "of Range Management (SAF/SRM)    cover types, the National Vegetation "\
                    "Classification System cover types (NVCS), or the Fuel "\
                    "Characteristic Classification System (FCCS) default fuelbeds "\
                    "to describe your cover type that is entered on the next field."

#define cb_CoverType "Select the most appropriate Cover Type that best "\
                     "describes your treatment unit vegetation conditions. "\
                     "Your choice will be used in FOFEM to select the most "\
                     "appropriate default fuel loadings and consumption algorithms."


#define cb_Season   "Select the season when the burn will occur. "\
                    "This will not affect fuel loadings but will "\
                    "be used to adjust model parameters."


#define cb_FuelCat "Select the type of fuel that best describe your fuelbed: "\
                    "natural fuels (non-activity, or untreated fuels), or the "\
                    "activity fuels of piled fuels or slash fuels. "\
                    "Your selection may alter the default fuel loadings "\
                    "or the consumption algorithms."

#define _txTPA  " Default fuel loadings (dry weight tons per acre) by fuel\n"\
               " component used to calculate Fuel Consumption, Smoke Emissions\n"\
               " and Soil Heating.  These were populated from an extensive\n"\
               " analysis of field data collected across the United States.\n\n"\
               " You can individually adjust any fuel component by right mouse clicking\n"\
               " in the desired textbox or you can adjust them all at once using the\n"\
               " Adjustments Box immediately to the right of all the fields.\n\n"\
               " Note: Adjustment options are dependent on the Cover Type you have selected."

#define txTPA "Default fuel loadings were computed as the average of\n"\
                 " the fuel component loading across all the field plots.\n"\
                 " You can adjust the load to lighter (L-one standard deviation\n"\
                 " lower), typical (T-average), and High (H-one standard deviation\n"\
                 " higher) based on the span of field data used to calculate the mean.\n\n"\
                 " Woody Fuels:\n"\
                 " 0 -> 1/4 inch = 1 hour fuels\n"\
                 " 1/4 -> 1 inch = 10 hour fuels\n"\
                 " 1 -> 3 inch = 100 hour fuels\n"\
                 " 3+ inch = 1000 hour fuels\n"


#define DuffDepth "Duff depth is expressed in inches.\n"\
                  "Note: If you modify the duff fuel loading to 0 you\n"\
                  "must also set duff depth to 0, and conversely if you\n"\
                  "modify the depth to 0 you must then set the load to 0"


#define  RottenWood3 "This is the percent of logs (3+ in down dead woody"\
                     " fuel; 1000 hr) that is considered to be rotten."


#define WeightDist  "Select how the large (3+ inch) down dead woody fuel is"\
                    " distributed in your unit across diameter classes.\n"\
                    " There are three log diameter classes: 3->6, 6->9,"\
                    " 9->20 and over 20 inches so the five possible loading distributions are:\n"\
                    "     'Right'    Largest loading in the 20+ logs                   \n"\
                    "     'Left'     Largest amount of fuel in smallest logs 3-6 inches\n"\
                    "     'Even'    Equal amounts of fuel in each size class           \n"\
                    "     'End'      Most loadings are in 3-6 and 20+ logs             \n"\
                    "     'Center'  Most loading in 3-9 & 9-20 in diameter logs        "

#define CrownBurn  "Enter percent of canopy fuels to be burned."\
                   " This option allows you to predict emissions from crown fire."\
                   " FOFEM does not compute fire behavior, so you are required to"\
                   " enter the expected amount of canopy fuel consumed by crown fire."\
                   " Range is 0 (no crown fire) to 100% for an active crown fire."\
                   " For fires with some torching select an intermediate value."

#define MoistCond  "Enter a relative fuel moisture condition scenario.\n"\
                   "The selected scenario will determine the default  \n"\
                   "moisture contents for woody fuel and duff.  You can \n"\
                   "alter these fuel moistures if you wish. "

#define MoistDuff  "Enter the percent moisture content of the duff layer.\n"\
                   "This duff moisture is for the entire duff profile.\n\n"\
                   "NOTE: A special case exists when you enter the lower moisture limit.\n"\
                   "      This action will force all of the duff to be consumed, this action\n"\
                   "      is helpfull if you are doing soil heating simulations.\n"\
                   "      >> To determine the lower moisture limit enter a 0 and FOFEM\n"\
                   "      will tell you what the current lower limit is.\n" 

#define Moist10Hr  "Enter the percent moisture content of the down dead woody\n"\
                   "fuels that are above 0.25 in and below 1 inch in diameter\n"\
                   "(1 hr woody fuels)"

#define Moist1kHr  "Enter the percent moisture content of the down dead\n"\
                  "woody fuels that are above 3.0 in diameter (1000 hr\n"\
                  "woody fuels, or logs)."

#define MoistSoil  "Enter the percent moisture content of the soil layer.\n"\
                   "This soil moisture content is for the soil profile\n"\
                   "that is the free rooting zone."

#define OutputType  "Enter the type of output you want displayed on the\n"\
                    "FOFEM page. (the textboxes to the right of this control\n"\
                    "  Output types include:\n"\
                    "   1. Consumed-amount of fuel consumed by the fire\n"\
                    "   2. Post-amount of fuel left after the fire\n"\
                    "   3. amount of fuel consumption in percentages.\n"\
                    "This is NOT the only output from FOFEM; you can output\n"\
                    "tabular and graphic reports in the FOFEM output window."

#define OutputBox  "This value is set according to your selection in the 'Output' ComboBox.\n"\
                   "Each value is shown for the fuel component column that it is in." 

#define OutputDufDep "This Duff Depth calculated value is based on your selection in the 'Output' combobox"

#define SoilType "Enter the most appropriate soil texture type in dropdown\n"\
                 "list.  A complete description of soil types is given in\n"\
                 "the user's guide. This soil type input is for the soil\n"\
                 "profile that is the free rooting zone.\n" \
                 " Here are the options: \n\n "\
                 " Fine-Silty: by weight, < 15% of the particles are fine sand \n"\
                 "   (diameter 0.25-0.1 mm) or coarser, including fragments up \n"\
                 "   to 7.5 cm in diameter; 18 - 34% clay in the fine-earth fraction.\n\n"\
                 " Fine: a clayey particle-size class that has 35 - 59% clay in the \n"\
                 "   fine-earth fraction.                                         \n\n"\
                 " Coarse-Silty: by weight, < 15% of the particles are fine sand  \n"\
                 "   (diameter 0.25-0.1 mm) or coarser, including fragments up to \n"\
                 "   7.5 cm in diameter; less than 18% clay in the fine-earth fraction.\n\n"\
                 " Coarse-Loamy: by weight, 15% or more of the particles are fine sand \n"\
                 "   (diameter 0.25-0.1 mm) or coarser,including fragments up to 7.5 cm \n"\
                 "   in diameter; less than 18% clay in the fine-earth fraction.      \n"\
                 " Loamy-Skeletal:  rock fragments make up 35% or more by volume;     \n\n"\
                 "   enough fine earth to fill interstices larger that 1 mm; fraction \n"\
                 "   finer than 2 mm is loamy as defined for the loamy particle-size class."

#define EmisFlame "Select the most appropriate Active Flaming and\n"\
                  " Smoldering emission factor group"

#define EmisSmold "Select the most appropriate Coarse Woody\n"\
                  " RSC emission factor group"

#define EmisDuff "Select the most appropriate Duff RSC\n"\
                  " emission factor group"


#define FuelReport "Click this Button to create a tabular report describing"\
                   "consumption by fuel component."

#define SmokeReport  "Click this Button to create a tabular report of smoke"\
                     "emissions such as PM2.5 and CO2."

#define SoilReport "Click this Button to create a report detailing soil"\
                   "temperatures at various depths below the soil surface."

#define ClearReport  "Click this Button to clear all calculations from the output window"

#define FuelGraph  "Click this Button to create bar graphs describing consumption by fuel component."

#define SmokeGraph  "Click this Button to create a stacked graph of smoke emissions such as PM2.5 and CO2."

#define Soil_Graph  "Click this Button to create a graph showing soil temperatures at various depths below the soil surface over time of burning."

#define Totals  "Supply a short unique name in the textbox. The name is used to\n"\
                "stratify your FOFEM results into a summary table.\n" \
                "A total will be stored each time you create a report\n"\
                "Any number of totals can be stored.\n"\
                "Use the 'Write' button to create the Totals report.\n"\
                "Use the 'Clear' button to clear all the stored totals\n"\
                "and their names"



#ifdef xxxxxxxxxxxxxx
#define SpeciesCB "List of tree species for the selected region.\n"\
                  "Use the ---->> Button to select a species into\n"\
                  "tree grid.\n\n"\
                  "If you make changes to the grid and view a graph\n"\
                  "you will need to refresh the graph by hitting\n"\
                  "the Mortality Graph button"
#endif 

#define SpeciesCB "\n                   All input fields are required\n\n"\
"Species : Species codes available for the currently selected region.\n"\
"  Use the --->> Button to move the selected species into the\n"\
"  tree grid or type in the desired species code.\n"\
"Density : Tree density. (trees per acre)\n"\
"DBH : Diameter breast height. (inches) \n"\
"Height : Tree height (feet) \n"\
"C / R : Crown ratio (1 to 10)\n\n"\
"If you make changes in the data grid you will need to refresh the graph\n"\
"by clicking the Mortality graph button.\n"


#ifdef wowowoowow               
#define SpeciesCBPost "Tree Grid and Species selection list.\n"\
                  "Species list contains trees for current region\n"\
                  "Use the ---->> Button to select a species into the tree grid\n"\
                  "Note: each species in the dropdown list has some extra text\n"\
                  "      to help show you the inputs you need to enter for \n"\
                  "      the particular species\n"\
                  "len = crown scorched as a percentage of length,   0 -> 100\n"\
                  "vol = crown scorched as a percentage of volume,   0 -> 100\n"\
                  "kil = crown scorched as a percentage of bud kill, 0 -> 100\n"\
                  "dbh = diameter breast height, not required by mortality model\n"\
                  "dbhR = diameter breast height, Required by mortality model\n"\
                  "ckr = Cambium Kill Rating , a value from 0 to 4 \n"\
                  "btl = beetle kill\n"\
                  "      Y = the tree is attacked by beetles;\n"\
                  "      N= the tree in not attacked by beetles\n"\
                  "X = indicates beetle kill is not applicable\n"\
                  " Please see full Help for more details" 
#endif 
// S---------------------------------------------------------------------------------
#define SpeciesCBPost  "\n                                        All input fields are required\n\n"\
"Each species in the dropdown list has text in parenthesis to identify inputs to the\n"\
"mortality model.\n\n"\
"Species : Species codes available for the currently selected region. Use the --->> Button\n"\
"to move the selected species into the tree grid or type in the desired species code. \n"\
"Density : Tree density. (trees per acre)\n"\
"Crown Dam. % : Percent fire caused crown damage.\n"\
"   len = crown scorched as a percent of total crown length. (0 to 100)\n"\
"   vol = crown scorched as a percent of total crown volume. (0 to 100)\n"\
"   kil = crown killed (bud kill) as a percent of total crown volume. (0 to 100)\n"\
"DBH : Diameter breast height. (inches)\n"\
"CKR : Cambium kill rating\n"\
"   ckr reguired for all species. (0 to 4)\n"\
"Beetle Y/N : Evidence of successful beetle attack\n"\
"   btl = Beetle attack (Y = tree was attacked by beetles, N = tree was not attacked by\n"\
"   beetles)\n"\
"   X = Beetle attack is not required by the mortality model. (Enter X).\n\n"\
"If you make changes in the data grid you will need to refresh the graph by clicking the \n"\
"Mortality graph button.\n"

// E--------------------------------------------------------------------------------

#define FlameScorch "Check the Flame Length or the Scorch Height button\n"\
                    "and set the approbriate value in the textbox"

/* Tool Tip */
#define FlaSco   "Enter flame length / scorch height in feet"  

#define PFI_Box  "Enter a Probability Mortality Cutoff value between 0 -> 1.0 " 

#define FFI_Help "By clicking the FFI button you can load tree data directly\n"\
                 "into the Tree Grid from and FFI input file.\n"\
                 "Please see full Help for more details\n\n"\
                 "Note: this option is not available for Post Fire Injury\n"\
                 "because FFI files do not contain the required information"

#define OutputToolTip "Calculated post fire fuel loading" 

/* Tool tip */
#define PFI_PM "Enter the Probability Mortality Cutoff 0 -> 1.0"

/***************************************************************
*
****************************************************************/
int Wnd_Mai::Help_Mngr()
{
String ^ Str; 

/* Main Tab Controls */  
// These didn't seem to work 
//  this->toolTip1->SetToolTip(this->_tcpFuel, L"Click this tab to input data to compute fuel consumption, smoke emissions, and soil heating");
//  this->toolTip1->SetToolTip(this->_tcpMortality, L"Click this tab to input data to compute tree mortality");

/*-----------------------------------------------------------------*/
/*  Main Menu Item  Tool Tips  */
/* It appears that there is no easy way to hookup F1 help to Menu Items */
/* Main Menu -> File -> Item Tool Tip */
  this->openProjectToolStripMenuItem->ToolTipText = MI_SavPro;
  this->saveProjectToolStripMenuItem->ToolTipText = MI_OpePro;
  this->newProjectToolStripMenuItem->ToolTipText = MI_NewPro;
  this->exitToolStripMenuItem1->ToolTipText = MI_Exit;

/* Main Menu -> Options -> Item Tool Tips */
  this->_cmBatch->ToolTipText = "This option is used to mulitple plots from a single input file";
  this->createSampleBurnupInputFilebrnToolStripMenuItem->ToolTipText = "Create a sample Burnup input file";
  this->saveToolStripMenuItem->ToolTipText = "Used to save a Burnup emissions file";
  this->runBurnupFromInputFIlebrnToolStripMenuItem->ToolTipText = "Run Burnup from and inputs file";
  this->runSoilFromInputFileToolStripMenuItem->ToolTipText = "Run the soil simulation from an input file";
  this->saveSoilTempPointsFileToolStripMenuItem->ToolTipText = "Save the soil layer temperature points to a file";
  this->createSampleSoilInputFIledrfexpToolStripMenuItem->ToolTipText = "Create a sample soil input file";



/*-----------------------------------------------------------------*/
/* F1 Help ComboBoxes  */
  Help_SetCB (this->_cbRegion, cb_Region); 
  Help_SetCB (this->_cbFuelClass, cb_CoverClass);   /* SAF, NVCS, etc */ 
  Help_SetCB (this->_cbCoverType, cb_CoverType);    /* Indvidual type combobox */
  Help_SetCB (this->_cbSeason, cb_Season); 
  Help_SetCB (this->_cbFuelCat,cb_FuelCat);        /* Natural, Pile Slash */ 
  Help_SetCB (this->_cbLogDist, WeightDist); 
  Help_SetCB (this->_cbMoisture, MoistCond); 
  Help_SetCB (this->_cbOutput, OutputType); 
  Help_SetCB (this->_cbSoil, SoilType); 

  Help_SetCB (this->_cbEmiFla, EmisFlame);    /* Emission ComboBoxes */
  Help_SetCB (this->_cbEmiSmo, EmisSmold);
  Help_SetCB (this->_cbEmiDuf, EmisDuff); 


/* F1 Help TextBoxes */
  Help_Set (this->_txProFilNam, tx_ProFil);          /* Project file text box */

  Help_Set (this->_txDuff,txTPA); 
  Help_Set (this->_tx1Hr, txTPA); 
  Help_Set (this->_tx10Hr, txTPA);
  Help_Set (this->_tx100Hr, txTPA);
  Help_Set (this->_tx1kHr, txTPA);
  Help_Set (this->_txLitter, txTPA);
  Help_Set (this->_txHerb, txTPA);
  Help_Set (this->_txShrub, txTPA);
  Help_Set (this->_txFoliage, txTPA);
  Help_Set (this->_txBranch, txTPA);
  Help_Set (this->_txRotLog,RottenWood3); 
  Help_Set (this->_txFolBraCon,CrownBurn); 
 
  Help_Set  (this->_txDuffDepth,DuffDepth); 

/* F1 Moisture textboxes */
  Help_Set (this->_txMoistDuff,MoistDuff); 
  Help_Set (this->_txMoist10Hr,Moist10Hr); 
  Help_Set (this->_txMoist1kHr,Moist1kHr);
  Help_Set (this->_txMoistSoil,MoistSoil); 

/* F1 Output textboxes */ 
  Help_Set (this->_txOut_Duff,OutputBox); 
  Help_Set (this->_txOut_Litter,OutputBox);
  Help_Set (this->_txOut_1Hr,OutputBox);
  Help_Set (this->_txOut_10Hr,OutputBox);
  Help_Set (this->_txOut_100Hr,OutputBox);
  Help_Set (this->_txOut_1kHr,OutputBox);
  Help_Set (this->_txOut_Herb,OutputBox);
  Help_Set (this->_txOut_Shrub,OutputBox);
  Help_Set (this->_txOut_Foliage,OutputBox);
  Help_Set (this->_txOut_Branch,OutputBox);

  Help_Set (this->_txOut_DufDep,OutputDufDep); 

  Help_Set (this->_txRepTot,Totals);    /* Report Totals textbox */

   
/*----------------------------------------------*/
/* Mortality Tab F1 help */ 
 //  if ( this->_chPostFire->Checked ) 
 //     Help_SetCB (this->_cbSpecies, SpeciesCBPost);
 //  else 
 //       Help_SetCB (this->_cbSpecies, SpeciesCB);

  this->Set_MortSpeciesHelp();   /* for Tree list and TextBox */
  
/* Tree Data Grids */

  this->helpProvider1->SetHelpString(this->_dgMort,SpeciesCB); 
 
 Help_SetRB (this->_rbFlameLen,FlameScorch);
 Help_SetRB (this->_rbScorchHeight,FlameScorch);

/* FFI Input File Options Group Box - help if for all controls in the GroupBox */
 this->helpProvider1->SetHelpKeyword(this->_gbFFI, L"f1");
 this->helpProvider1->SetHelpString(this->_gbFFI, FFI_Help);

 
/*-------------------------------------------------*/
/* Tool Tips for output textboxes */
/* Most Tool Tips I entered directing into the IDE */
/* these here will allow me to easily change them */
 ToolTip_Set (this->_txOut_Duff, OutputToolTip);
 ToolTip_Set (this->_txOut_Litter, OutputToolTip);
 ToolTip_Set (this->_txOut_1Hr, OutputToolTip);
 ToolTip_Set (this->_txOut_10Hr, OutputToolTip);
 ToolTip_Set (this->_txOut_100Hr, OutputToolTip);
 ToolTip_Set (this->_txOut_1kHr, OutputToolTip);
 ToolTip_Set (this->_txOut_Herb, OutputToolTip);
 ToolTip_Set (this->_txOut_Shrub, OutputToolTip);
 ToolTip_Set (this->_txOut_Foliage, OutputToolTip);
 ToolTip_Set (this->_txOut_Branch, OutputToolTip);


  return 1; 
}

/****************************************************************
* Name: Set_MortSpeciesHelp
* Desc: Set the F1 help for the Species combobox to to match
*       if the Post Fire Injury checkbox is checked.
*       We need to give the user different help for each
**************************************************************/
int Wnd_Mai::Set_MortSpeciesHelp()
{
  if  ( this->PostFire_Checked() ) {            /* if doing Post Fire Injury */
    Help_SetCB (this->_cbSpecies, SpeciesCBPost);
    Help_Set   (this->_txFlaSco,PFI_Box);
    ToolTip_Set (this->_txFlaSco,PFI_PM);}
  else {
    Help_SetCB (this->_cbSpecies, SpeciesCB);
    Help_Set   (this->_txFlaSco,FlameScorch); 
    ToolTip_Set (this->_txFlaSco,FlaSco);}

  return 1; 
}

 

/***************************************************************************
*
*
****************************************************************************/
int Wnd_Mai::Help_SetRB(RadioButton ^ rb, char cr[] )
{
String ^ Str;

 Str = _CharToStr (cr); 
 this->helpProvider1->SetHelpKeyword(rb, L"f1");
 this->helpProvider1->SetHelpString(rb, Str);
 return 1; 
}



/***************************************************************************
*
*
****************************************************************************/
int Wnd_Mai::Help_Set(TextBox ^ tb, char cr[] )
{
String ^ Str;

 Str = _CharToStr (cr); 
 this->helpProvider1->SetHelpKeyword(tb, L"f1");
 this->helpProvider1->SetHelpString(tb, Str);
 return 1; 
} 

/***************************************************************************
*
*
****************************************************************************/
int Wnd_Mai::Help_SetCB(ComboBox ^ cb, char cr[] )
{
String ^ Str;

 Str = _CharToStr (cr); 
 this->helpProvider1->SetHelpKeyword(cb, L"f1");
 this->helpProvider1->SetHelpString(cb, Str);
 return 1; 
}

/***************************************************************************
*
*
****************************************************************************/
int Wnd_Mai::Help_SetChk(CheckBox ^ ck, char cr[] )
{
String ^ Str;

 Str = _CharToStr (cr); 
 this->helpProvider1->SetHelpKeyword(ck, L"f1");
 this->helpProvider1->SetHelpString(ck, Str);
 return 1; 
}





/*****************************************************************
*
*****************************************************************/
 int Wnd_Mai::ToolTip_Set (TextBox ^ tb, char cr[])
{ 
String ^ Str;

 Str = _CharToStr (cr); 
 this->toolTip1->SetToolTip(tb, Str);

  return 1; 
}

//      this->toolTip1->SetToolTip(this->_txProFilNam, L"Project File ");
