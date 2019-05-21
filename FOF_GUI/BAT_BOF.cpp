/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: bat_bof.c
* Desc: Batch Output File functions
*
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
#ifdef ANSI
#define WINAPI
#else
#include <windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <float.h>

#include  "fof_sgv.h"
#include "fof_ci.h" 
#include "fof_co.h"
#include "fof_sh.h"
#include "fof_iss.h"
#include "fof_mis.h"
#include "fof_mrt.h"
#include "bat_bof.h"

#include "fof_nes.h"

#include "unit_Abv.h"

#include "fof_util.h"

extern double gdN_Flame;  
extern double gdN_Coarse; 
extern double gdN_Duff;   

FILE *fh_BOF;


int BOF_ExpEmi (FILE *fh, d_CI *a_CI, d_CO *CO);

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: BOF_MortHeading
* Desc: Put Mortality heading labels to the batch output file.
*       This should be the first line in the file, so call this right
*        after opening file.
*   In: 0 = No soil lables, 1 = do soil labels
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  BOF_MortHeading ()
{
/* 3-25-15, new heading */
  fprintf (fh_BOF, "Std, DenPre,DenPost,DenKld, BAPre,BAPost,BAKld, CCPre,CCPost,CCDiff, DBHKldAvg, MortAvgPct4, MortAvgPct\n");

// Old .....
//   fprintf (fh_BOF,"Std, AvgMrt, TotKilled, AvgDBHKld, AvgMrt4, PreTrs, BAPre, BAPost, BAKilled, CCPre, CCPost\n");

return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: BOF_PutHeading
* Desc: Put the heading labels to the batch output file.
*       This should be the first line in the file, so call this right
*        after opening file.
*   In: 0 = No soil lables, 1 = do soil labels
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  BOF_PutHeading (int iS_Soil, char cr_EmiFla[], char cr_EmiFull[], char cr_UseEqu[])
{
int i,j,k; 
float f_FacFla; 
char cr_CovTyp[100], cr_Name[100], cr_Code[50], cr[100];
char Line2[5000];
   
   strcpy (Line2,""); 

   fprintf (fh_BOF, "Std,LitPre,LitCon,LitPos,");
   fprintf (fh_BOF, "DW1Pre,DW1Con,DW1Pos,");
   fprintf (fh_BOF, "DW10Pre,DW10Con,DW10Pos,");
   fprintf (fh_BOF, "DW100Pre,DW100Con,DW100Pos,");

   fprintf (fh_BOF, "DW1kSndPre,DW1kSndCon,DW1kSndPos,");
   fprintf (fh_BOF, "DW1kRotPre,DW1kRotCon,DW1kRotPos,");
   fprintf (fh_BOF, "DufPre,DufCon,DufPos,");
   fprintf (fh_BOF, "HerPre,HerCon,HerPos,");
   fprintf (fh_BOF, "ShrPre,ShrCon,ShrPos,");
   fprintf (fh_BOF, "FolPre,FolCon,FolPos,");
   fprintf (fh_BOF, "BraPre,BraCon,BraPos,");

   strcat (Line2, "Id,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac,T/ac"); 

   fprintf (fh_BOF,"MSE,DufDepPre,DufDepCon,DufDepPos,");
     strcat (Line2, ",percent,inch,inch,inch");


   if ( !strcmp (cr_EmiFla,"") ) { 
     fprintf (fh_BOF,"PM10F,PM10S,PM25F,PM25S,CH4F,CH4S,COF,COS,");
     strcat (Line2, ",lb/ac,lb/ac,lb/ac,lb/ac,lb/ac,lb/ac,lb/ac,lb/ac");
     fprintf (fh_BOF,"CO2F,CO2S,NOXF,NOXS,SO2F,SO2S,FlaDur,SmoDur,FlaCon,SmoCon");
     strcat (Line2, ",lb/ac,lb/ac,lb/ac,lb/ac,lb/ac,lb/ac,sec,sec,T/ac,T/ac");
     goto Soil; }


 /* Expanded Emissions - Column Headings */ 
   if ( !strcmp (cr_EmiFull, "" ) ) {   /* Just doing the basic 7 */
     fprintf (fh_BOF,"PM10F,PM10S,PM25F,PM25S,CH4F,CH4S,COF,COS,");
     fprintf (fh_BOF,"CO2F,CO2S,NOXF,NOXS,SO2F,SO2S,FlaDur,SmoDur,FlaCon,SmoCon");
     strcat (Line2, ",lb/ac,lb/ac,lb/ac,lb/ac,lb/ac,lb/ac,lb/ac,lb/ac,lb/ac,lb/ac,lb/ac,lb/ac,lb/ac,lb/ac,sec,sec,T/ac,T/ac"); 
     goto Soil; }

/* Do the full Expaned emissions col headings */
/* Use the Code and the Name, because some of Codes are used more than once */
   for ( j = 0; j < 10000; j++ ) {
      k = NES_Get_Factor(cr_EmiFla, j, cr_CovTyp, &f_FacFla, cr_Name, cr_Code);
 	  if ( k == 0 )         /* No more */ 
		break;
      StrRepChr (cr_Name, ',', ';');  /* Get rid of any commas */ 
      StrRepChr (cr_Code, ',', ';');  /* Get rid of any commas */ 
      fprintf (fh_BOF, "%s: %s,",cr_Code,cr_Name);
      strcat (Line2, ",lb/ac"); }

   fprintf (fh_BOF,"FlaDur,SmoDur,FlaCon,SmoCon"); 
   strcat (Line2, ",sec,sec,T/ac,T/ac"); 


Soil:
   if ( iS_Soil == 1 ) {                /* If don't want Soil labels         */
      fprintf (fh_BOF,",Lay0,Lay2,Lay4,Lay6,Lay60d,Lay275d");
      sprintf (cr,",C%c,C%c,C%c,C%c",0x00B0,0x00B0,0x00B0,0x00B0);
      strcat (Line2,cr);
      strcat (Line2, ",cm,cm"); }


    if ( !stricmp (cr_UseEqu, "Yes" ) ) 
      fprintf (fh_BOF,",Lit-Equ,DufCon-Equ,DufRed-Equ,MSE-Equ,Herb-Equ,Shurb-Equ");

   fprintf (fh_BOF, "\n"); 
   fprintf (fh_BOF, "%s\n",Line2); 

   return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name:
* Desc:
* Ret:
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  BOF_Put (char cr_StdId[], d_CO *a_CO, d_CI *a_CI, int iS_Soil, d_SO *a_SO)
{
int i; 
char A[1000]; 

/* Change 3-29-05 DL requested we add the Duff Depth pre & post              */
/* Change 3-9-16, DL said add Pre fuels */

/* It's easier to make printf format statement in a loop */
   strcpy (A, "\"%s\","); 
  
  for ( i = 0; i < 37; i++ ) 
     strcat (A, "%5.2f,"); 
  
   fprintf (fh_BOF,A,
     cr_StdId,
     a_CO->f_LitPre,       a_CO->f_LitCon,        a_CO->f_LitPos,     
     a_CO->f_DW1Pre,       a_CO->f_DW1Con,        a_CO->f_DW1Pos,     
     a_CO->f_DW10Pre,      a_CO->f_DW10Con,       a_CO->f_DW10Pos,    
     a_CO->f_DW100Pre,     a_CO->f_DW100Con,      a_CO->f_DW100Pos,   
     a_CO->f_Snd_DW1kPre,  a_CO->f_Snd_DW1kCon,   a_CO->f_Snd_DW1kPos,
     a_CO->f_Rot_DW1kPre,  a_CO->f_Rot_DW1kCon,   a_CO->f_Rot_DW1kPos,
                                                     
     a_CO->f_DufPre,       a_CO->f_DufCon,        a_CO->f_DufPos,     
     a_CO->f_HerPre,       a_CO->f_HerCon,        a_CO->f_HerPos,     
     a_CO->f_ShrPre,       a_CO->f_ShrCon,        a_CO->f_ShrPos,     
     a_CO->f_FolPre,       a_CO->f_FolCon,        a_CO->f_FolPos,     
     a_CO->f_BraPre,       a_CO->f_BraCon,        a_CO->f_BraPos,     
 
     a_CO->f_MSEPer,                       /* Mineral Soil Exposer percent       */

     a_CO->f_DufDepPre,
     a_CO->f_DufDepCon, 
     a_CO->f_DufDepPos);


/* Change 3-4-05, added Flame & Smolder time duration on end of output         */
/* Change 3-23-05, added Flame & Smolder Consumed amounts                    */
/*  fprintf (fh_BOF,"%3.0f,%3.0f,%3.0f,%3.0f,%3.0f,%3.0f,%3.0f,%3.0f,%3.0f,%3.0f,%3.0f,%3.0f,%3.0f,%3.0f", */
/* Do Burnup defaults or Expanded Basic seven */
/* if were doing the Expanded Basic Seven we get them back from Burnup the same way */
/* we get the Burnup default ones, for the Full Expanded list, see functin call below */
    if ( a_CI->f_CriInt < 0 || !stricmp (a_CI->cr_EmiFull,"") ) { 
      fprintf (fh_BOF,"%3.0f,%3.0f,%3.0f,%3.0f,%3.0f,%3.0f,%3.0f,%5.0f,%5.0f,%5.0f,%3.0f,%3.0f,%3.0f,%3.0f,%6.0f,%6.0f,%6.2f,%6.2f",
       a_CO->f_PM10F, a_CO->f_PM10S,
       a_CO->f_PM25F, a_CO->f_PM25S,
       a_CO->f_CH4F,  a_CO->f_CH4S,
       a_CO->f_COF,   a_CO->f_COS,
       a_CO->f_CO2F,  a_CO->f_CO2S,
       a_CO->f_NOXF,  a_CO->f_NOXS,
       a_CO->f_SO2F,  a_CO->f_SO2S,
       a_CO->f_FlaDur, a_CO->f_SmoDur,
       a_CO->f_FlaCon, a_CO->f_SmoCon); 
      goto Soil; 
    }

/* Expanded Emissions - Full 204 */
    if  ( a_CI->f_CriInt > 0 )  {
      BOF_ExpEmi (fh_BOF, a_CI, a_CO);
    }

Soil:
    if ( iS_Soil == 1 ) {
     fprintf (fh_BOF, ",%3d,%3d,%3d,%3d,%3d,%3d",
       a_SO->ir_Temp[0],
       a_SO->ir_Temp[2],
       a_SO->ir_Temp[4],
       a_SO->ir_Temp[6],
       a_SO->i_LayMaxDeg1,
       a_SO->i_LayMaxDeg2); }


    if ( !stricmp (a_CI->cr_BatchEqu, "Yes")) {
       fprintf (fh_BOF, ",%3d,%3d,%3d,%3d,%3d,%3d", 
            a_CO->i_LitEqu,
            a_CO->i_DufEqu,
            a_CO->i_DufDepEqu,
            a_CO->i_MSEEqu,
            a_CO->i_HerEqu,
            a_CO->i_ShrEqu);  }

   fprintf (fh_BOF,"\n");

   return 1;
}

/***************************************************************
* Name: BOF_ExpEmi
* Desc: Ouput the full list of 204 Expaned emissions to file 
* Note: We calculate ALL the emission amounts here (outside 
*       of burnup) even though we could get them from Burnup.
*       See the BOF_Put() function where we do pull the basic
*       seven Expaned emissions from burnup. 
***************************************************************/
int BOF_ExpEmi (FILE *fh, d_CI *CI, d_CO *CO)
{
int i, j, k; 
char cr_EmiFla[100], cr_CovTyp[100],cr_Name[100], cr_Code[100], cr[20];
float f, f_Tot; 
float f_ConFla, f_ConCoa, f_ConDuf, f_FacFla, f_FacDuf, f_FacCoa; 
float f_EmiFla,f_EmiCoa,f_EmiDuf; 

  f_ConFla = f_ConCoa = f_ConDuf = 0;

/* Get Total Consumed Fuel Amounts, */
/* Coarse = smoldering 1k wood where fire intensity < 15 kW */
  for ( i = 0; i < CO->iC_SGV; i++ ) {
    if ( CO->sr_SGV[i].f_Sec < 0 )
	  break;
    f_ConFla += CO->sr_SGV[i].f_FlaCon;
	f_ConCoa += CO->sr_SGV[i].f_SmoCon;
    f_ConDuf += CO->sr_SGV[i].f_DuffCon;  }

/* for all of the emission codes pull their factors from emis table */
/* apply to consumed amounts and convert to pounds */
/* See Note-1 above */ 
   for ( j = 0; j < 100000; j++ ) {
      k = NES_Get_Factor(CI->cr_EmiFla, j, cr_CovTyp, &f_FacFla, cr_Name, cr_Code);
 	  if ( k == 0 )         /* No more */ 
		break; 
      NES_Get_Factor(CI->cr_EmiSmo, j, cr_CovTyp, &f_FacCoa, cr_Name, cr_Code);
      NES_Get_Factor(CI->cr_EmiDuf, j, cr_CovTyp, &f_FacDuf, cr_Name, cr_Code);

	  f_EmiFla = f_ConFla * f_FacFla;               /* Tot Consmd * Emis Compnt Factr */
      f_EmiCoa = f_ConCoa * f_FacCoa;
	  f_EmiDuf = f_ConDuf * f_FacDuf; 
      f_Tot = f_EmiFla + f_EmiCoa + f_EmiDuf; 
      f =  GramSqMt_To_Pounds (f_Tot); 

      if ( f < 1.0 ) 
        strcpy (cr,"%4.2f,");   /* decimal precsion of format statement */
      else if ( f < 10.0 )
        strcpy (cr, "%4.1f,");
      else 
        strcpy (cr, "%4.0f,");
      fprintf (fh, cr, f); 
   }

   fprintf (fh, "%6.0f,%6.0f,%6.2f,%6.2f", CO->f_FlaDur, CO->f_SmoDur, CO->f_FlaCon, CO->f_SmoCon); 

   return 1; 
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: BOF_Open, BOF_Close
* Desc: Open/Close the batch output file
* Ret:  see below
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int BOF_Open (char cr_OutFN[])
{
   fh_BOF = fopen (cr_OutFN,"w");
   if ( fh_BOF == NULL )
     return 0;
   return 1;
}

void  BOF_Close  ()
{
  if ( fh_BOF != NULL )
    fclose (fh_BOF);
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: BOF_PutMort
* Desc: Put the output Mortality record
*   In: a_SIF...Species Mort Input Record
*       f_Prob..Calculated Probability of Mortality
*
* Change - see Ticket-2
* Following is what DL requested, in that order and a couple new items
*  Stand, Pre-fire tree density, Post-fire tree density, Trees killed,
*  Pre-fire basal area, Post-fire basal area, Basal area killed,
*  Pre-fire canopy cover, Post-fire canopy cover, Canopy cover change, 
*  Average DBH of killed trees,   Average mortality of trees over 4 inches DBH,   Average mortality.
* 
* Ret:
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void BOF_PutMort(char cr_StdId[])
{
float f_PosDen, f_CanDiff,  f_pcKil; 
float f_Den, f_Live, f_Kill; 
float f_BasDen, f_BasLive, f_BasKill; 
float f_CCDen, f_CCLive, f_CCKill; 
float f_4Den, f_4Live, f_4Kill;
float f_AvgDBHKill, f_AvgMort, f_4AvgMort; 
char  cr[100]; 

/*.........................................................*/

// new 

  ISS_Total(&f_Den, &f_Live, &f_Kill, 0); 
  if ( f_Den <= 0 ) 
     f_AvgMort = 0; 
  else
     f_AvgMort = (f_Kill / f_Den) * 100.0; 

  ISS_Total(&f_4Den, &f_4Live, &f_4Kill, 4); 
  if ( f_4Den <= 0 ) 
    f_4AvgMort = 0;
  else
    f_4AvgMort = (f_4Kill / f_4Den) * 100.0;

  ISS_BasalArea (&f_BasDen, &f_BasLive, &f_BasKill); 
 
  f_AvgDBHKill = ISS_AvgDBHKill(); 

  ISS_CrownCover(&f_CCDen, &f_CCLive, &f_CCKill, cr);

/*---------------------------------------------------------*/
  fprintf (fh_BOF, "%s, %1.0f,%1.0f,%1.0f, %3.2f,%3.2f,%3.2f, %1.0f,%1.0f,%1.0f, %1.1f,%1.0f,%1.0f\n",
          cr_StdId,
          f_Den, f_Live, f_Kill,
          f_BasDen, f_BasLive, f_BasKill , 
          f_CCDen,  f_CCLive, f_CCKill, 
          f_AvgDBHKill, f_4AvgMort, f_AvgMort);


}


