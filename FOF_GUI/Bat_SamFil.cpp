/************************************************************************
* Name: Bat_SamFil.cpp
* Desc: Create Sample Batch input files
*
*************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <float.h>

#include "bat_mai.h" 
#include "fof_ci.h"
#include "fof_sh.h" 
#include "fof_nes.h"
#include "Bat_Bif.h" 


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Bat_MakSamMort
* Desc: Make a sample input batch file for Mortality
*   In: cr_FN.......Path and File Name
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  Bat_MakSamMort (char cr_FN[])
{
FILE *fh;

// GOOD 

  fh = fopen (cr_FN,"w");
  if ( fh == NULL )
     return 0;

   fprintf (fh,"# ------------------------------------------------------------------\n");
   fprintf (fh,"# FOFEM Mortality Batch Input file \n");
   fprintf (fh,"# ------------------------------------------------------------------\n");


   fprintf (fh,"# Plot  Plost Id \n"); 
   fprintf (fh,"# MSta  Monitoring Status\n"); 
   fprintf (fh,"# MOrd  Monitorign Order Number\n"); 
   fprintf (fh,"# Spe   Tree Species\n");
   fprintf (fh,"# Exp   Tree Expansion Factor\n");
   fprintf (fh,"# DBH   Diameter          CRNSCH   BOLCHR    \n");
   fprintf (fh,"# Hgt   Tree Height       CRNSCH\n"); 
   fprintf (fh,"# CBH   Crown Base Height CRNSCH\n"); 
   fprintf (fh,"# Sta   Tree Status       CRNSCH\n"); 
   fprintf (fh,"# CC    CrownClass        CRNSCH\n"); 
   fprintf (fh,"# CR    Crown Ratio       CRNSCH\n"); 
   fprintf (fh,"# CHt   Char Height       BOLCHR \n");    
   fprintf (fh,"# CSP   Crown Scorch Percent CRCABE \n"); 
   fprintf (fh,"# CSH   Crown Scorch Height \n");   
   fprintf (fh,"# CKR   Cambium Kill Rating  CRCABE \n"); 
   fprintf (fh,"# Bee   Beetle Damage        CRCABE \n"); 
   fprintf (fh,"# Equ   Equation Type    CRNSCH  CRCABE   BOLCHR \n");   
   fprintf (fh,"# FSH   Flame Length - Scorch Height - Numeric value CRNSCH \n"); 
   fprintf (fh,"# FS    \"F\" = Flame Length, \"S\" = Scorch Height  CRNSCH \n"); 
   fprintf (fh,"# Sev   Fire Severity, \"L\" or blank                CRNSCH \n"); 
   fprintf (fh,"#\n"); 

   fprintf (fh,"#  Salvage Report Output File\n", e_SC); 
   fprintf (fh,"# To create the file enter 1,2 or 3 %s switches.\n", e_SC); 
   fprintf (fh,"# Example: \n", e_SC); 
   fprintf (fh,"#  %s-1  1  5  0.30\n", e_SC); 
   fprintf (fh,"#  % s-2  5.0 10.0  0.7\n", e_SC); 
   fprintf (fh,"#  % s-3  10.0 20.0  0.5\n", e_SC); 
   fprintf (fh,"\n\n"); 

   fprintf (fh,"# NOTE - The first row in the file MUST contain column heading.\n"); 
   fprintf (fh,"#\n"); 
   fprintf (fh,"%s,MSta,MOrd,Spe,Exp,DBH,Hgt,CBH,Sta,CC,CR,CHt,CSP,CSH,CKR,Bee,Equ,FSH,FS,Sev\n",e_PlotId); 

   fprintf (fh,"TESTFOREST1, PreBurn, 1, PICO, 10,  7.5,  56,  20,   H,  O, 60,   4,   ,    ,    , No, CRNSCH, 4, F, L\n"); 
   fprintf (fh,"TESTFOREST1, PreBurn, 1, PICO, 10,  5.9,  41,  28,   H,  S, 30,   4,   ,    ,    ,Yes, CRNSCH, 4, F,\n"); 
   fprintf (fh,"TESTFOREST1, PreBurn, 1, PSME, 10,    5,  75,    ,   D,   ,   ,   4, 30,    ,   2, NO, CRCABE, 4, F,\n");   

   fprintf (fh,"TESTFOREST2, PostBurn,2, PICO, 20,  5.8,  40,  28,   H,  S, 30,   4,   ,    ,    ,Yes, CRNSCH, 4, F,\n"); 
   fprintf (fh,"TESTFOREST2, PostBurn,2, PSME, 30,    5,  75,    ,   D,   ,   ,  12,   ,    ,    ,   , BOLCHR, 4, F,\n");  

   fclose (fh);
   return 1;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Bat_SampleFile
* Desc: Make a sample Consume Emission Soil input batch file.
*       The text below came from DL Ticket-146 - dated 5/23/17
*   In: cr_FN.......Path and File Name
*       cr_Mode....."Consume" or "Soil"
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/

int  Bat_SampleFile (char cr_FN[], char cr_Mode[])
{
FILE *fh;
 
  fh = fopen (cr_FN,"w");
  if ( fh == NULL )
     return 0;

   fprintf (fh,"# ---------------------------------------------------------------------------------\n"); 
   fprintf (fh,"# This is a sample input file for the FOFEM6 Batch Program\n"); 
   if ( !stricmp (cr_Mode, "Soil") ) 
      fprintf (fh,"# Consumed - Emission - Soil\n"); 
   else 
      fprintf (fh,"# Consumed - Emission\n"); 

   fprintf (fh,"# ---------------------------------------------------------------------------------\n"); 
   fprintf (fh,"# Units: Loading = tons/acre, depth = inches, moisture = %% , crown burned = %%\n"); 
   fprintf (fh,"# ----------------------------------------------------------------------------------\n"); 
   fprintf (fh,"# Distr. - 1000 hour fuel distribution into classes\n"); 
   fprintf (fh,"# Right, Left, Even, End, Center\n"); 
   fprintf (fh,"# ----------------------------------------------------------------------------------\n"); 
   fprintf (fh,"# Region:\n"); 
   fprintf (fh,"# SouthEast, InteriorWest, PacificWest, NorthEast\n"); 
   fprintf (fh,"# ----------------------------------------------------------------------------------\n"); 
   fprintf (fh,"# Cover Group:\n"); 
   fprintf (fh,"# GrassGroup or GG, Sagebrush or SB, ShrubGroup or SG,\n"); 
   fprintf (fh,"# Pocosin or PC, Ponderosa or PN, WhiPinHem or WPH,\n"); 
   fprintf (fh,"# RedJacPin or RJP, BalBRWSpr or BBS,  PinFlaWoo or PFL\n"); 
   fprintf (fh,"# \"\" = no cover group, or leave blank\n"); 
   fprintf (fh,"# ----------------------------------------------------------------------------------\n"); 
   fprintf (fh,"# Season:\n"); 
   fprintf (fh,"# Winter, Spring, Summer, Fall\n"); 
   fprintf (fh,"# ----------------------------------------------------------------------------------\n"); 
   fprintf (fh,"# Fuel Category:\n"); 
   fprintf (fh,"# Natural, Slash\n"); 
   fprintf (fh,"# ----------------------------------------------------------------------------------\n"); 
   fprintf (fh,"# Lines starting with hastags # followed by a space are comment lines\n"); 
   fprintf (fh,"# Lines starting with hastags # immediately followed by text are switches\n"); 
   fprintf (fh,"# that supply information or cause FOFEM to act a certain way.\n"); 
   fprintf (fh,"# for example...\n"); 
   fprintf (fh,"# This is a comment: # BurnUpFile Burnup.txt\n"); 
   fprintf (fh,"# This is a Switch:  #BurnUpFile Burnup.txt\n"); 
   fprintf (fh,"# This is a Switch:  #Season: Spring\n"); 
   fprintf (fh,"# ----------------------------------------------------------------------------------\n"); 
   fprintf (fh,"# #ConsumptionEquation\n"); 
   fprintf (fh,"# This optional switch is used to set the FOFEM equations for:\n"); 
   fprintf (fh,"# 1)litter load consumption, 2) duff load consumption, 3) duff depth reduction,\n"); 
   fprintf (fh,"# 4) mineral soil exposure, 5) herbaceous load consumption and\n");  
   fprintf (fh,"# 6) shrub load consumption.\n"); 
   fprintf (fh,"# Using this switch requires you add six equation numbers at the end of each\n"); 
   fprintf (fh,"# record in the order they appear above. Separate equation numbers with commas.\n"); 
   fprintf (fh,"# Enter equation number -1 to use default equation. \n"); 
   fprintf (fh,"# Valid equation numbers are: \n"); 
   fprintf (fh,"#   Litter: 997, 998, 999\n"); 
   fprintf (fh,"#   Duff load consumption: 1, 2, 3, 4, 5, 6, 7, 15, 16, 17, 19, 20\n"); 
   fprintf (fh,"#   Duff depth reduction: 1, 2, 3, 4, 5, 6, 7, 15, 16, 17, 19, 20\n"); 
   fprintf (fh,"#   MSE: 9, 10, 11, 12, 13, 14, 18, 202\n"); 
   fprintf (fh,"#   Herb: 22, 221, 222, 223\n"); 
   fprintf (fh,"#   Shrub: 23, 231, 232, 233, 234, 235, 236\n"); 
   fprintf (fh,"# If this switch is not used FOFEM will use default logic to set consumption\n"); 
   fprintf (fh,"# equations. Equations are described in the Scientific Content section of the FOFEM\n"); 
   fprintf (fh,"# User Guide.\n"); 

   fprintf (fh,"# ----------------------------------------------------------------------------------\n"); 
   fprintf (fh,"# #EFG-STFS n, #EFG-CWDRSC n, #EFG-DuffRSC n\n"); 
   fprintf (fh,"# This optional switch is used to set emission factor groups for short-term\n"); 
   fprintf (fh,"# flaming and smoldering (STFS), CWD residual smoldering component (CWDRSC)\n"); 
   fprintf (fh,"# and Duff residual smoldering component (DuffRSC).\n"); 
   fprintf (fh,"# When using this option all three switches are required.\n"); 
   fprintf (fh,"# Refer to Emission_Factors.csv (found here: C:\\Users...AppData\\Local\\FOFEMx.x)\n"); 
   fprintf (fh,"# for valid emission factor group numbers.\n"); 
   fprintf (fh,"# Example:\n"); 
   fprintf (fh,"# #EFG-STFS 3\n"); 
   fprintf (fh,"# #EFG-CWDRSC 7\n"); 
   fprintf (fh,"# #EFG-DuffRSC 8\n"); 
   fprintf (fh,"# Use the following switch to output all 204 emissions,if the switch is not \n"); 
   fprintf (fh,"# present only the basic seven will be outputed.\n"); 
   fprintf (fh,"# #EFG-Full  \n"); 
   fprintf (fh,"# If this switch is not used FOFEM will use default logic to set emissions factors.\n"); 
   fprintf (fh,"# ----------------------------------------------------------------------------------\n"); 
   fprintf (fh,"# #Emission-File [EmFile]\n"); 
   fprintf (fh,"# This optional switch causes an emission file to be created for each plot.\n");  
   fprintf (fh,"# You must supply an argument (e.g., 'EmFile') after the switch which will be\n"); 
   fprintf (fh,"# used as a prefix to form output file names. The emission files will go into\n"); 
   fprintf (fh,"# the same folder as the other output files.\n"); 

   fprintf (fh,"# ----------------------------------------------------------------------------------\n"); 
   fprintf (fh,"# #BurnUpFile Burnup.txt\n"); 
   fprintf (fh,"# This optional switch specifies the name of a Burnup parameter file. Specify\n"); 
   fprintf (fh,"# just the burnup file name (no file path) and place the file in the same folder\n"); 
   fprintf (fh,"# as the stand input file (this file).\n"); 
   fprintf (fh,"# ----------------------------------------------------------------------------------\n"); 
   fprintf (fh,"# #1k-SizeClass\n"); 
   fprintf (fh,"# This optional switch specifies >3 inch fuel loads are entered in size classes\n"); 
   fprintf (fh,"# in inches: 3=3-<6, 6=6-<9, 9=9-<20, 20=20+\n"); 
   fprintf (fh,"# Example:\n"); 
   fprintf (fh,"# Remove the four columns of 1000 hour data in the standard format (Load, Moist,\n"); 
   fprintf (fh,"# Rot, Distr) and insert the following nine columns of data.\n"); 
   fprintf (fh,"#  .......Sound......   ......Rotten......    1000Hr\n"); 
   fprintf (fh,"#   3    6    9    20    3    6    9    20    Moist\n"); 
   fprintf (fh,"#  0.5, 1.2, 2.3, 3.5,  0.2, 0.8, 1.1, 1.4,    50,\n"); 
   fprintf (fh,"#\n"); 
   fprintf (fh,"# Quote marks must be used if you have blank spaces in your plot numbers.\n"); 
   fprintf (fh,"# Example:   \"Plot 1\" \n"); 
   fprintf (fh,"#\n"); 
// Remove line Ticket-201
//   fprintf (fh,"# Sample Data. Note the Soil Type and Moisture columns on end of each row\n"); 
 
   if ( !stricmp (cr_Mode, "Soil") ) {
     fprintf (fh,"# ___________________________________________________________________________________________________________________________________________________________________\n"); 
     fprintf (fh,"# Stand          1     ...10...  100  ......1000 Hr........   ....Duff......               ....Crown......                 Cover               Fuel        Soil         Soil\n"); 
     fprintf (fh,"#  Id     Litt   Hr   Hr  Moist  Hr   Load Moist Rot  Distr   Load Moist  Dep  Herb  Shrub Foli Brnch Brn     Region       Group       Season  Category    Type         Moist\n"); 
     fprintf (fh,"Plot-1,   0.50, 1.00, 1.00, 16, 1.00,  1.0,  30,  50,  Even,  10.0,  75,  1.00, 0.06, 0.23, 0.25, 1.20, 50,  PacificWest,  GrassGroup,  Spring, Natural, Loamy-Skeletal,  10\n");   
     fprintf (fh,"Plot-2,   0.60, 0.33, 0.52, 10, 1.44,  6.6,  15,  50,  Even,  29.7,  40,  1.38, 0.18, 0.13, 6.00, 3.90, 75,  InteriorWest, WhiPinHem,   Summer, Natural,      Fine-Silt,  15 \n"); 
     fprintf (fh,"Plot-3,   0.90, 0.20, 0.20, 16, 1.12,  3.0,  30,  30,  Left,   5.0,  75,  0.50, 0.30, 0.50, 0.00, 0.00, 50,  Northeast,    ,            Fall,   Natural,           Fine,  20 \n"); 
     fprintf (fh,"Plot-4,   3.22, 1.20, 0.68, 22, 0.70,  0.0,  30,  50, Center,  223, 130,  10.5, 0.11, 7.53, 0.68, 7.53, 75,  Southeast,    PC,          Spring, Natural,    Coarse-Silt,  25 \n"); 
    }
   else {
     fprintf (fh,"# ___________________________________________________________________________________________________________________________________________________\n"); 
     fprintf (fh,"# Stand           1    ...10...  100   ......1000 Hr........   ....Duff......              ....Crown......                Cover                Fuel \n"); 
     fprintf (fh,"#  Id      Litt   Hr   Hr  Moist  Hr   Load Moist Rot Distr   Load Moist  Dep  Herb  Shrub Foli Brnch Brn    Region       Group       Season  Category \n"); 
     fprintf (fh,"Plot-1,   0.50, 1.00, 1.00, 16, 1.00,  1.0,  30,  50,  Even,  10.0,  75,  1.00, 0.06, 0.23, 0.25, 1.20, 50, PacificWest,  GrassGroup, Spring, Natural\n"); 
     fprintf (fh,"Plot-2,   0.60, 0.33, 0.52, 10, 1.44,  6.6,  15,  50,  Even,  29.7,  40,  1.38, 0.18, 0.13, 6.00, 3.90, 75, InteriorWest, WhiPinHem,  Summer, Natural\n"); 
     fprintf (fh,"Plot-3,   0.90, 0.20, 0.20, 16, 1.12,  3.0,  30,  30,  Left,   5.0,  75,  0.50, 0.30, 0.50, 0.00, 0.00, 50, Northeast,    ,           Fall,   Natural\n"); 
     fprintf (fh,"Plot-4,   3.22, 1.20, 0.68, 22, 0.70,  0.0,  30,  50,Center,   223, 130,  10.5, 0.11, 7.53, 0.68, 7.53, 75, Southeast,    PC,         Spring, Natural\n"); 
   }

   fclose (fh);
   return 1;
}


#ifdef OLDOLD
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Bat
* Desc: Make a sample input batch file.
*
* New Sample file from DL  3/3/17  Ticket-123 
* See files in   \FOF_GUI\Sample-Batch\
* 
*   In: cr_FN.......Path and File Name
*       cr_Mode....."Consume" or "Soil"
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  Bat_SampleFile (char cr_FN[], char cr_Mode[])
{
FILE *fh;
 
  fh = fopen (cr_FN,"w");
  if ( fh == NULL )
     return 0;

  fprintf (fh,"# ---------------------------------------------------------------------------------\n"); 
  fprintf (fh,"# This is a sample input file for the FOFEM6 Batch Program\n"); 
  fprintf (fh,"# Consumed - Emissions\n"); 
  fprintf (fh,"# ---------------------------------------------------------------------------------\n"); 
  fprintf (fh,"# Units: Loading = tons/acre, depth = inches, moisture = %% , crown burned = %%\n"); 
  fprintf (fh,"# ----------------------------------------------------------------------------------\n"); 
  fprintf (fh,"# Distr. - 1000 hour fuel distribution into classes\n"); 
  fprintf (fh,"#  Right, Left, Even, End, Center\n"); 
  fprintf (fh,"# ----------------------------------------------------------------------------------\n"); 
  fprintf (fh,"# Region:\n"); 
  fprintf (fh,"# SouthEast, InteriorWest, PacificWest, NorthEast\n"); 
  fprintf (fh,"# ----------------------------------------------------------------------------------\n"); 
  fprintf (fh,"# Cover Group:\n"); 
  fprintf (fh,"# GrassGroup or GG, Sagebrush or SB, ShrubGroup or SG,\n"); 
  fprintf (fh,"# Pocosin or PC, Ponderosa or PN, WhiPinHem or WPH,\n"); 
  fprintf (fh,"# RedJacPin or RJP, BalBRWSpr or BBS,  PinFlaWoo or PFL\n");  
  fprintf (fh,"# \"\" = no cover group, or leave blank\n"); 
  fprintf (fh,"# ----------------------------------------------------------------------------------\n");  
  fprintf (fh,"# Season:\n"); 
  fprintf (fh,"# Winter, Spring, Summer, Fall\n"); 
  fprintf (fh,"# ----------------------------------------------------------------------------------\n"); 
  fprintf (fh,"# Fuel Category:\n"); 
  fprintf (fh,"# Natural, Slash\n"); 
  fprintf (fh,"# ----------------------------------------------------------------------------------\n"); 
  fprintf (fh,"# Lines starting with hastags # followed by a space are comment lines\n"); 
  fprintf (fh,"# Lines starting with hastags # immediately followed by text are switches\n"); 
  fprintf (fh,"# that supply information or cause FOFEM to act a certain way.\n"); 
  fprintf (fh,"# for example...\n"); 
  fprintf (fh,"# This is a comment: # BurnUpFile Burnup.txt\n"); 
  fprintf (fh,"# This is a Switch:  #BurnUpFile Burnup.txt\n"); 
  fprintf (fh,"# This is a Switch:  #Season: Spring\n"); 
  fprintf (fh,"# ----------------------------------------------------------------------------------\n"); 
  fprintf (fh,"# #ConsumptionEquation\n"); 
  fprintf (fh,"# This optional switch is used to set the FOFEM equations for:\n");  
  fprintf (fh,"# 1)litter load consumption, 2) duff load consumption, 3) duff depth reduction,\n"); 
  fprintf (fh,"# 4) mineral soil exposure, 5) herbaceous load consumption and \n"); 
  fprintf (fh,"# 6) shrub load consumption.\n"); 
  fprintf (fh,"# Using this switch requires you add six equation numbers at the end of each\n"); 
  fprintf (fh,"# record in the order they appear above. Separate equation numbers with commas.\n");  
  fprintf (fh,"# Enter equation number -1 to use default equation. \n"); 
  fprintf (fh,"# Valid equation numbers are: \n"); 
  fprintf (fh,"#   Litter: 997, 998, 999\n"); 
  fprintf (fh,"#   Duff load consumption: 1, 2, 3, 4, 5, 6, 7, 15, 16, 17, 19, 20\n"); 
  fprintf (fh,"#   Duff depth reduction: 1, 2, 3, 4, 5, 6, 7, 15, 16, 17, 19, 20\n"); 
  fprintf (fh,"#   MSE: 9, 10, 11, 12, 13, 14, 18, 202\n"); 
  fprintf (fh,"#   Herb: 22, 221, 222, 223\n"); 
  fprintf (fh,"#   Shrub: 23, 231, 232, 233, 234, 235, 236\n"); 
  fprintf (fh,"# If this switch is not used FOFEM will use default logic to set consumption\n"); 
  fprintf (fh,"# equations. Equations are described in the Scientific Content section of the FOFEM\n"); 
  fprintf (fh,"# User Guide.\n"); 


/* Extended Emission Factor switch not use for Soil batch */
  if ( stricmp (cr_Mode, "Soil") ) {    /* if not Soil */ 
    fprintf (fh,"# ----------------------------------------------------------------------------------\n"); 
    fprintf (fh,"# #EFG-STFS n, #EFG-CWDRSC n, #EFG-DuffRSC n\n"); 
    fprintf (fh,"# This optional switch is used to set\n"); 
    fprintf (fh,"# emission factor groups for short-term flaming and smoldering (STFS), CWD residual\n"); 
    fprintf (fh,"# smoldering component (CWDRSC) and Duff residual smoldering component (DuffRSC).\n"); 
    fprintf (fh,"# When using this option all three switches are required.\n"); 
    fprintf (fh,"# Refer to Emission_Factors.csv (found here: C:\\Users...AppData\\Local\\FOFEMx.x)\n"); 
    fprintf (fh,"# for valid emission factor group numbers.\n"); 
    fprintf (fh,"# Example:\n"); 
    fprintf (fh,"# #EFG-STFS 3\n"); 
    fprintf (fh,"# #EFG-CWDRSC 7\n"); 
    fprintf (fh,"# #EFG-DuffRSC 8\n"); 
    fprintf (fh,"# If this switch is not used FOFEM will use default logic to set emissions factors.\n"); 
    fprintf (fh,"# ----------------------------------------------------------------------------------\n"); 
    fprintf (fh,"# #Emission-File [EmFile]\n"); 
    fprintf (fh,"# This optional switch causes an emission file to be\n");  
    fprintf (fh,"# created for each plot. You must supply an argument (e.g., 'EmFile') after the\n"); 
    fprintf (fh,"# switch which will be used as a prefix to form output file names. The emission\n"); 
    fprintf (fh,"# files will go into the same folder as the other output files.\n"); 
  } 


  fprintf (fh,"# ----------------------------------------------------------------------------------\n");  
  fprintf (fh,"# #BurnUpFile Burnup.txt\n"); 
  fprintf (fh,"# This optional switch specifies the name of a\n");  
  fprintf (fh,"# Burnup parameter file. Specify just the burnup file name (no file path) and place\n");  
  fprintf (fh,"# the file in the same folder as the stand input file (this file)\n"); 
  fprintf (fh,"# ----------------------------------------------------------------------------------\n"); 
  fprintf (fh,"# #1k-SizeClass\n"); 
  fprintf (fh,"# This optional switch specifies >3 inch fuel loads are entered in\n"); 
  fprintf (fh,"# size classes. Size classes defined in inches: 3=3-<6, 6=6-<9, 9=9-<20, 20=20+\n");  
  fprintf (fh,"# Example:\n"); 
  fprintf (fh,"# Remove the four columns of 1000 hour data in the standard format (Load, Moist,\n");  
  fprintf (fh,"# Rot, Distr) and insert the following nine columns of data.\n"); 
  fprintf (fh,"#  .......Sound......   ......Rotten......    1000Hr\n"); 
  fprintf (fh,"#   3    6    9    20    3    6    9    20    Moist\n"); 
  fprintf (fh,"#  0.5, 1.2, 2.3, 3.5,  0.2, 0.8, 1.1, 1.4,    50,\n"); 
  fprintf (fh,"#\n"); 
  fprintf (fh,"# Quote marks must be used if you have blank spaces in your plot numbers.\n"); 
  fprintf (fh,"# Example:   \"Plot 1\" \n"); 
  fprintf (fh,"#\n"); 

  if ( !stricmp (cr_Mode, "Consume") ) {
    fprintf (fh,"# Sample Data\n"); 
    fprintf (fh,"# ________________________________________________________________________________________________________________________________________________________\n"); 
    fprintf (fh,"# Stand           1    ...10...  100   ......1000 Hr........   ....Duff......              ....Crown......                  Cover                Fuel \n");   
    fprintf (fh,"#  Id      Litt   Hr   Hr  Moist  Hr   Load Moist Rot Distr   Load Moist  Dep  Herb  Shrub Foli Brnch Brn    Region       Group      Season   Category \n"); 
    fprintf (fh,"Plot-1,   0.50, 1.00, 1.00, 16, 1.00,  1.0,  30,  50,  Even,  10.0,  75,  1.00, 0.06, 0.23, 0.25, 1.20, 50, PacificWest,  GrassGroup, Spring, Natural\n"); 
    fprintf (fh,"Plot-2,   0.60, 0.33, 0.52, 10, 1.44,  6.6,  15,  50,  Even,  29.7,  40,  1.38, 0.18, 0.13, 6.00, 3.90, 75, InteriorWest, WhiPinHem,  Summer, Natural\n"); 
    fprintf (fh,"Plot-3,   0.90, 0.20, 0.20, 16, 1.12,  3.0,  30,  30,  Left,   5.0,  75,  0.50, 0.30, 0.50, 0.00, 0.00, 50, Northeast,    ,           Fall,   Natural\n");  
    fprintf (fh,"Plot-4,   3.22, 1.20, 0.68, 22, 0.70,  0.0,  30,  50,Center,   223, 130,  10.5, 0.11, 7.53, 0.68, 7.53, 75, Southeast,    PC,         Spring, Natural\n"); 
  }

/* The Only difference with the Soil input is that it has soil type and soil moisture on the end */
  if ( !stricmp (cr_Mode, "Soil" ) ) {
    fprintf (fh,"# Sample Data. Note the Soil Type and Moisture columns on end of each row\n"); 
    fprintf (fh,"# ________________________________________________________________________________________________________________________________________________________\n"); 
    fprintf (fh,"# Stand          1     ...10...  100  ......1000 Hr........   ....Duff......               ....Crown......                   Cover               Fuel       Soil         Soil\n");  
    fprintf (fh,"#  Id     Litt   Hr   Hr  Moist  Hr   Load Moist Rot  Distr   Load Moist  Dep  Herb  Shrub Foli Brnch Brn     Region       Group      Season  Category    Type         Moist\n"); 
    fprintf (fh,"Plot-1,   0.50, 1.00, 1.00, 16, 1.00,  1.0,  30,  50,  Even,  10.0,  75,  1.00, 0.06, 0.23, 0.25, 1.20, 50,  PacificWest,  GrassGroup,  Spring, Natural, Loamy-Skeletal,  10  \n"); 
    fprintf (fh,"Plot-2,   0.60, 0.33, 0.52, 10, 1.44,  6.6,  15,  50,  Even,  29.7,  40,  1.38, 0.18, 0.13, 6.00, 3.90, 75,  InteriorWest, WhiPinHem,   Summer, Natural,      Fine-Silt,  15 \n"); 
    fprintf (fh,"Plot-3,   0.90, 0.20, 0.20, 16, 1.12,  3.0,  30,  30,  Left,   5.0,  75,  0.50, 0.30, 0.50, 0.00, 0.00, 50,  Northeast,    ,            Fall,   Natural,           Fine,  20 \n");
    fprintf (fh,"Plot-4,   3.22, 1.20, 0.68, 22, 0.70,  0.0,  30,  50, Center,  223, 130,  10.5, 0.11, 7.53, 0.68, 7.53, 75,  Southeast,    PC,          Spring, Natural,    Coarse-Silt,  25 \n"); 
  }

  fclose (fh); 
  return 1; 
 }

#endif 
