/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: ansi_mai.c
* Desc: Example front end code to be compiled and linked with the FOFEM ANSI
*        Code.
*       Program demonstrates how to use the Consumed, Emissions, Soil and
*        Tree Mortality FOFEM functions.
*
* Author: Larry Gangi
* Date:  July  2005
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
#include <stdio.h>
#include <string.h>
#include <math.h>

/* Include header files for, Consumed, Emission, Soil Heating                */
#include  "fof_sgv.h"
#include  "fof_ci.h"
#include  "fof_co.h"
#include  "fof_co2.h"
#include  "fof_cm.h"
#include  "fof_ansi.h"
#include  "fof_sh.h"
#include  "fof_disp.h"

/* Include header files for Tree Mortaity                                    */
#include "fof_mis.h"
#include "fof_iss.h"
#include "fof_mrt.h"
#include "fof_nes.h" 

/* Define test functions                                                     */
//int  Mortality (void);
int  ConEmiSoi (void);
//void Show_Mort(d_MO *a_MO );


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: main
* Desc: Sample code main function
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int   main ()
{

   ConEmiSoi ();              /* Consumed,Emission,Soil sample function  */

   //Mortality ();              /* Tree Mortality sample function          */

   return 0;

}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: ConEmiSoi
* Desc: Sample function shows how to run the Consumed, Emissions and
*        Soil Heating calulations.
*  Ret: 0 OK, 1 Error
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  ConEmiSoi ()
{
int i;
char cr_ErrMes[3000];
/* Declare input and output structures                                       */
d_CI s_CI;                             /* Consumed inputs                    */
d_CO s_CO;                             /* Consumed outputs                   */
d_SI s_SI;                             /* Soil inputs                        */
d_SO s_SO;                             /* Soil outputs                       */

   CI_Init (&s_CI);                     /* Init Consumed Input Struct        */
   CO_Init (&s_CO);                     /* Init Consumed Output Struct       */

   s_CI.f_Duff = 1;                     /* Duff load                         */
   s_CI.f_DufDep = 1;                   /* Duff Depth                        */
   s_CI.f_MoistDuff = 10;               /* Duff Moisture                     */

   s_CI.f_Herb = 1;                     /* Herbaceous load                   */
   s_CI.f_Shrub = 1;                    /* Shrub load                        */

   s_CI.f_CroFol = 1;                   /* Crown Foliage load                */
   s_CI.f_CroBra = 1;                   /* Crown Branch load                 */
   s_CI.f_Pc_CroBrn = 50;               /* Percent of Crown that will burn   */

   s_CI.f_MoistDW10 =  20;              /* Moisture Down Wood 10 hr wood     */
   s_CI.f_MoistDW1000 = 20;             /* Moisture Down Wood 1000 hr wood   */

   s_CI.f_Lit = 1;                      /* Litter load                       */

   s_CI.f_DW1 = 1;                      /* Down Wood 1 hour fuel load        */
   s_CI.f_DW10 = 1;                     /* Down Wood 10 hour fuel load       */
   s_CI.f_DW100 = 1;                    /* Down Wood 100 hour fuel load      */

/* Sound & Rotten Down Wood 1000 hour loads                                  */
/* for this example we use a 1 ton load and distrubute it evenly into        */
/*  each size class, sound and rotten                                        */
   s_CI.f_Snd_DW3 = 1.0 / 8.0;
   s_CI.f_Snd_DW6 = 1.0 / 8.0; ;
   s_CI.f_Snd_DW9 = 1.0 / 8.0; ;
   s_CI.f_Snd_DW20 = 1.0 / 8.0; ;

   s_CI.f_Rot_DW3 = 1.0 / 8.0; ;
   s_CI.f_Rot_DW6 = 1.0 / 8.0; ;
   s_CI.f_Rot_DW9 = 1.0 / 8.0; ;
   s_CI.f_Rot_DW20 = 1.0 / 8.0; ;

   strcpy (s_CI.cr_Region, INTERIORWEST);         /* Set Region              */
   strcpy (s_CI.cr_FuelCategory, NATURAL);        /* Fuel Category           */
   strcpy (s_CI.cr_Season, SUMMER);               /* Season                  */
   strcpy (s_CI.cr_DufMoiMet,ENTIRE);             /* Moisture Measured Method*/

/* If the plot's cover type is in a particular Cover Group                   */
/* strcpy (s_CI.cr_CoverGroup,PONDEROSA);  */

/*.....................................................................*/
/* Emission Settings                                                   */

/* To use the Burnup original emission factors just need to set */ 
s_CI.f_CriInt = -1;
goto SkipEmis;


/* To use the New Expanded emission factors - do the following */
   i = NES_Read ("", cr_ErrMes);    /* Open and load the Emission Factor file */
   if ( i <= 0 ) {
     printf ("%s",cr_ErrMes);
     return 0; }

   s_CI.f_CriInt = e_CriInt;    /* Set Critical Fire Intensity break point */

/* retreive factors and load to CI input structure, where they'll get pass in  */  
   i = NES_Get_MajFactor(e_DefFlaGrp, &s_CI.f_fCO, &s_CI.f_fCO2, 
                         &s_CI.f_fCH4, &s_CI.f_fPM25, &s_CI.f_fPM10, 
                         &s_CI.f_fNOX, &s_CI.f_fSO2);

   i = NES_Get_MajFactor(e_DefSmoGrp, &s_CI.f_sCO, &s_CI.f_sCO2, 
                         &s_CI.f_sCH4, &s_CI.f_sPM25, &s_CI.f_sPM10, 
                         &s_CI.f_sNOX, &s_CI.f_sSO2);

   i = NES_Get_MajFactor(e_DefDufGrp, &s_CI.f_dCO, &s_CI.f_dCO2, 
                         &s_CI.f_dCH4, &s_CI.f_dPM25, &s_CI.f_dPM10, 
                         &s_CI.f_dNOX, &s_CI.f_dSO2);

SkipEmis:

/* To create a fuel load output or emissions file enter a file name          */
strcpy (s_CI.cr_LoadFN, "load.txt");
strcpy (s_CI.cr_EmiFN,  "emis.txt");

/* Display the Consumed Inputs */
Disp_ConIn (&s_CI);

/* Call the Consume Manager, and get back answers                            */
   if ( !CM_Mngr (&s_CI, &s_CO,cr_ErrMes)){
     printf ("ERROR - \n");
     printf ("%s\n",cr_ErrMes);
     return 1; }

/* Display outputs from consumed simulation                                  */
   Disp_DHSF (&s_CO);                  /* Duff Herb Shrub Branch             */
   Disp_BrnUp (&s_CO);                 /* DownWood, Litter, Emissions        */
   Disp_ConTot (&s_CO);                /* Consumed totals                    */


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* Do Soil Heating                                                           */
/* Some Soil Inputs are taken from the Consumed outputs, see s_CO below      */
/*                                                                           */
   SI_Init (&s_SI);                     /* Initialize these first            */
   SO_Init (&s_SO);

   s_SI.f_DufDepPre = s_CO.f_DufDepPre; /* Set PreFire & PostFire Duff Depth */
   s_SI.f_DufDepPos = s_CO.f_DufDepPos; /*  use values from Consumd Simulaton*/

   s_SI.f_SoilMoist = 15;               /* Duff Moisutre                     */

   strcpy (s_SI.cr_BrnIg,s_CO.cr_BrnIg);/* Tells if Consumd Simlation ignitd*/
   strcpy (s_SI.cr_SoilType,FINE_SILT);
   strcpy (s_SI.cr_MoistCond,MODERATE);


/* Do Soil Heating simulation,                                              */
/* soil.tmp = save soil heating layer file, else use "" for none             */
   if (!SH_Mngr (&s_SI, &s_SO, s_CO.fr_SFI,s_CO.fr_SFIhs,   "soil.tmp", cr_ErrMes)) {
     printf ("ERROR - %s\n", cr_ErrMes);
     return 1; }

   Disp_Soil (&s_SO);                   /* Display results                   */

   
   return 0;
}
