/***********************************************************************
* Name: Wnd_Rep.c
* Desc: Report writing functions
* Author: Larry Gangi
* 
************************************************************************/
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <math.h>

#include <stdarg.h>

#include "Wnd_Mai.h" 
#include "Wnd_Util.h"
#include "Wnd_Def.h"
#include "fof_util.h"
#include "fof_ci.h"
#include "CVT_CVT.h"
#include "fof_fwf2.h"
#include "fof_sgv.h"
#include "fof_co.h" 
#include "fof_co2.h" 
#include "fof_cm.h"
#include "fof_Duf.h" 
#include "bur_brn.h" 
#include "fof_sh.h"
#include "fof_sha.h" 
#include "fof_st.h"
#include "fof_Stt.h"

#include "Unit_Abv.h"

#include "..\FOF_DLL\BM_Soil\HTAA.h"

 
#ifdef OLDOLD
/* Standard Unit Formats */
/* NOTE these #defines don't get used everywhere in the code but where not used */
/*  the text used is and should be the exact same as defined here  */
#define e_TPA "T/ac"     /* Ticket #5 - DL said change --> old  "t/ac" */
#define e_lbs   "lb/ac"
#define e_Gram  "g/sq.m"
#define e_Kg    "kg/sq.m"

#define e_GrKg  "g/kg"

#define e_Kw    "kW/sq.m"   /* fire intensity - Kilowatts per Square Meter */

#define e_Seconds "sec" 
#endif 

/* use to convert the emission factors from Grams Per Kilogram to lb (pounds) per Ton */
// #define e_gkTolb 2.0


/* We'll say the fire is out when it gets down to
*  this small amount, becuase there will be some very
*  small amount of consumption that goes on and on */
#define  e_out  0.0001    

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
#define  e_Adj_Na   ' '       /* send back to call to identify Wnd Adj Load  */
#define  e_Adj_Usr  'u'       /* user, Sparse/Abundant  Light/Heavy          */
#define  e_Adj_HA   '+'
#define  e_Adj_LS   '-'

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* Used in creating the Default Emission .csv output files  */
typedef struct {
       char cr_Code[10];
       char cr_Name[100];
       } d_EF;

#define  e_CO2  "CO2"
#define  e_CO   "CO" 
#define  e_CH4  "CH4"
#define  e_NOX  "NOX"
#define  e_SO2  "SO2"
#define  e_PM25 "PM2.5"
#define  e_PM10 "PM10"


/* NOTE - set name for the one you want used to sr_EF[], other as Xsr_EF[] */  
/* This is the order of the Expanded emis CSV file */
d_EF sr_EF [] = { 
       { e_CO2,  "Carbon Dioxide"},
       { e_CO,   "Carbon Monoxide"},
       { e_CH4,  "Methane"},
       { e_NOX,  "Nitrogen Oxides"},
       { e_SO2,  "Sulfor Oxides"},
       { e_PM25, "PM2.5"},
       { e_PM10, "PM10"},
       {  "", ""}}; 

/* This is the order that matches GUI smoke reports */
d_EF Xsr_EF [] = { 
       { e_PM10, "PM10"},
       { e_PM25, "PM2.5"},
       { e_CH4,  "Methane"},
       { e_CO,   "Carbon Monoxide"},
       { e_CO2,  "Carbon Dioxide"},
       { e_NOX,  "Nitrogen Oxides"},
       { e_SO2,  "Sulfor Oxides"},
       {  "", ""}}; 


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/

float _GetDefEmi (d_SGV *a, char cr_Code[]);
int   CalcSub (d_FLI *a_FLI, d_CI *a_CI, d_CO *a_CO, d_SI *a_SI, d_SO *a_SO, char cr_ErrMes[]); 
void  Rep_SavAdj (char c, char cr_adj[]);
int   Fuel_Total(d_CO *a_CO, char cr_ErrMes[]);
void  Rep_SetST (d_ST  *a_ST,  d_CO  *a_CO);
int  _TimeOut (d_SGV *a_SGV, char cr_Cmp[], char cr_Time[] );
float _Con (d_SGV *a, char cr_Con[], float *af_SmoPercent);
float _Load (d_SGV *a, char cr_Con[]);
void  _GetFmt (float f, char cr[], char cr_Unit[]);
void _PutFuel (FILE *fh, float f_Fuel, char cr_EU[]);

float _CnvEmis (float f, char cr_EU[]);
float _CnvFuel (float val, char cr[]);
void _GetEmiGrp (char cr_In[], char cr_Out[]);
int _TotSmoFla (float *af_Fla,  float *af_Smo );

void  _MakeTot (float f_fla, float f_Smo, float f_Smo_Duff, float *af_smo, float *af_tot );
void _EmisConv (double *ad_FacFla, double *ad_FacSom, float fc );



/**********************************************************************
* Name: Report_Mngr
* Desc: Create specified report
* 
* NOTE: I created this function and the other functions to create the
*        individual reports after BK suggested he didn't want the
*        fuels info in the Smoke and Soil reports
*       See Report_Mngr_Old()
* Note-1: We don't want soil error message being displayed unless the
*         user is doing a Soil report, so we let them go by and it
*         a soil report is being done we display it and drop it in 
*         the report -
*  In: cr_CmdTyp...."Fuel", "Smoke", "Soil"  type of report 
***********************************************************************/
int Wnd_Mai::Report_Mngr (char cr_CmdTyp[]) 
{
int i; 
float f_Percent; 
char c, cr_Name[100], cr_Poco[10]; 
char cr_ErrMes[3000], cr[1000], cr_adj[100], cr_Date[100]; 
char cr_SoilErr[3000];
d_CO CO,  *a_CO;
d_CI CI,  *a_CI;
d_SI SI,  *a_SI;
d_SO SO,  *a_SO;
d_FLI  s_FLI; 
d_ST ST;
d_STT STT; 
d_STS STS; 

  a_CO = &CO;  /* just doing this to not re-write old code that uses a_CO-> */
  a_CI = &CI; 
  a_SI = &SI;
  a_SO = &SO; 

  CI_Init (&CI);
  CO_Init (&CO); 
  SI_Init (&SI);
  SO_Init (&SO);  

  strcpy (cr_Poco,"");   /* will have to be blank if not a Pocosin type */

  if ( !Get_Inputs(&s_FLI)) {
     myMessageBox(s_FLI.cr_ErrMes);
     return 0; }
  strcpy (cr_SoilErr,""); 
  i = CalcSub(&s_FLI, &CI, &CO, &SI, &SO, "", cr_ErrMes);
  if ( i == 0 ) {                         /* We have an err mess */
    if ( !strstr (cr_ErrMes,e_SoilErr)){  /* if its not a soil err mess */
      myMessageBox(cr_ErrMes);            /* display it */
      return 0; }                         /* See Note-1 above */
    strcpy (cr_SoilErr,cr_ErrMes);        /* we'll use this below */  
  }                        /* Deal with any soil err mess below */

  if ( !Set_FuelTotal(&CO,cr_ErrMes) ) {
     myMessageBox (cr_ErrMes);
     return 0; }
 
   _rep ("\n\n");
   GetDate (cr_Date); 
   sprintf (cr, "           TITLE: Results of FOFEM model execution on date: %s\n",cr_Date);
   _rep (cr); 

   if ( !stricmp (cr_CmdTyp, "Fuel") ) 
      _rep ("                           FUEL CONSUMPTION CALCULATIONS\n\n");
   if ( !stricmp (cr_CmdTyp, "Smoke") ) 
     _rep ("                    FUEL EMISSIONS CALCULATIONS\n\n");
   if ( !stricmp (cr_CmdTyp, "Soil") ) {
     _rep ("                           Soil Heat Report - Campbell\n\n");
      if ( !stricmp (a_CI->cr_CoverGroup, e_CVT_Pocosin ) ) 
        strcpy (cr_Poco, "***");  
   }
  
   Report_SubTit (&s_FLI,cr_Poco);


 if ( !stricmp (cr_CmdTyp, "Fuel") ) 
   Report_Fuel (a_CI, a_CO);

  if ( !stricmp (cr_CmdTyp, "Smoke") ) 
   Report_Smoke (a_CO,&s_FLI);

  if ( !stricmp (cr_CmdTyp, "Soil") ) {
    Report_Soil (a_CI, a_CO, a_SI, a_SO, &s_FLI);
    if ( strcmp (cr_SoilErr,"") ) {     /* See above, if there is text in here */ 
       _rep ("\n\n NOTE: "); 
       _rep (cr_SoilErr); 
       myMessageBox (cr_SoilErr);       /* its a soil err message */
       return 0; }
 
  }

/* 8-21-2014 I put this here so that the Output Textboxes and any open */
/*  Graphs get updated after doing a report, otherwise a user could    */
/* Modify an input fuel textbox, click a report button and the Output */
/* textboxes values and open graphs wouldn't update */
/* It was easier just to run Calcuate() and let it update boxs graphs */
/*  even though it recalcs everything again after the report just did  */
   Calculate ("",""); 
 
  
  return 1; 
}

/**********************************************************
* Name: Report_Soil
* Desc: Soil report 
*
***********************************************************/
int Wnd_Mai::Report_Soil (d_CI *a_CI, d_CO *a_CO, d_SI *a_SI, d_SO *a_SO, d_FLI *a_FLI)
{

char cr[500], cr_Name[100]; 

int i,j,i_Deg, i_Time, i_Max1, i_Max2, i_Inc;
float  r,r_Max;

/* NEED TO DEAL WITH THIS */
char cr_InFN[1000];
    strcpy( cr_InFN,"");

/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
/* Do Heading and check each layer                                           */

   if ( !stricmp (cr_InFN,"")) {
//     sprintf (cr," Cover Type.....: %s\n", a_CI->cr_CoverClass);
//     _rep(cr);

     sprintf (cr," Duff Consumption Equation: %d\n",a_CO->i_DufEqu);
//     sprintf (cr," Duff Depth Reduction Equation: %d\n",a_CO->i_DufEqu);
     _rep (cr); 

     sprintf (cr," Duff Depth.....: Pre-Fire: %6.2f cm.,  Post-Fire: %6.2f cm.\n\n",a_SO->f_cDufPre,a_SO->f_cDufPost);  
     _rep(cr);   }
   else{
     sprintf (cr,"User Input File: %s\n",cr_InFN);
    _rep (cr); }

   _rep ("                        Soil Layer Maximum Temperature\n\n");
//   _rep ("                 ( measurements are in centimeters and Celsius )\n\n");
   _rep ("Depth (cm)    0    1    2    3    4    5    6    7    8    9   10   11   12   13\n");


/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
/* Print out Max Temp for each Layer                                         */
   _rep ("Temp (C)   ");
   for ( i = 1; i <= eC_Lay; i++ ) {             /* Check each Layer          */
     r_Max = 0;
     for ( j = 1; j < eC_Tim; j++ ) {            /* For each Temp in Layer   */
        r = SHA_Get(i,j);                        /* Get Temp                 */
        if ( r == e_SHA_Init )                   /* Skip init values         */
          continue;
        if ( r > r_Max )                         /* Look for highest         */
           r_Max = r; }                          /*  and save it             */
      i_Deg = r_Max;
      sprintf (cr,"%4d ",i_Deg); 
      _rep (cr); }

      _rep ("\n"); 

/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
/* Print out Time that Max Temp occurs for each Layer                        */
float f_Time, f_Inc;
   _rep ("Time (min)");
   f_Inc = SHA_GetInc();                         /* Get Time Increment used */
   for ( i = 1; i <= eC_Lay; i++ ) {             /* For each Layer          */
     f_Time = 0;
     r_Max = 0;
     for ( j = 1; j < eC_Tim; j++ ) {            /* For each Temp in Layer   */
        r = SHA_Get(i,j);                        /* Get Temp                 */
        if ( r == e_SHA_Init )                   /* Skip init values         */
          continue;
        if ( r > r_Max ){                        /* Look for highest         */
           r_Max = r;                            /*  and save it             */
           f_Time = ( (float)j * f_Inc) / 60.0;} 
      }  /* for j */
      i_Time = ( f_Time + 0.5 );
      sprintf (cr,"%5d",i_Time); 
      _rep (cr); 
    }  /* for i */ 
   _rep ("\n\n");


/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
/* Do Max Temp at Layer, put to report and save for needed totals later      */
   i_Max1 = -1;
   i_Max2 = -1;

   for ( i = 1; i <= eC_Lay; i++ ) {             /* For each Layer          */
     for ( j = 1; j < eC_Tim; j++ ) {            /* For each Temp in Layer   */
        r = SHA_Get(i,j);                        /* Get Temp                 */
        if ( r == e_SHA_Init )                   /* Skip init values         */
          continue;
        if ( r > (float)e_Max1 )
          i_Max1 = i;
        if ( r > (float) e_Max2 )
          i_Max2 = i; } } 


   if ( i_Max1 == -1 ){
      sprintf (cr, "Max Depth Having %d degrees: - None - \n", e_Max1);
     _rep (cr); }
   else {
     sprintf (cr, "Max Depth Having %d degrees: %d\n", e_Max1, i_Max1 - 1);
     _rep (cr); }

   if ( i_Max2 == -1 ) {
     sprintf (cr,"Max Depth Having %d degrees: - None - \n", e_Max2);
     _rep (cr); }
   else {
     sprintf (cr,"Max Depth Having %d degrees: %d\n", e_Max2, i_Max2 - 1 );
     _rep (cr); }
 
  if ( !stricmp (a_CI->cr_CoverGroup, e_CVT_Pocosin ) )
     _rep ("\n  *** Soil heating model for pocosin types has not been tested and the results\n      may not be accurate.\n"); 
   
  if ( !stricmp (cr_InFN,"") ) {            /* if not run a user input file */
     if (!stricmp (a_SO->cr_Model,e_SM_Duff))/* Duf sim was run and post duf */
       if ( a_SO->f_Heatpc < 0.03 ) {      /* insulated most of heat       */
          sprintf (cr, "Due to Post Duff Depth a minimal amount of heat will be transferd to soil.\n");
          _rep (cr); }
 
 //    if (!stricmp (a_SO->cr_Model,e_SM_ZDuff)){ /* Exp Heat was run         */
 //      if ( strcmp(a_CO->cr_SHC,"") ) {       /* if Have Min Soil Heat Messag */
 //        sprintf (cr, "%s\n",a_CO->cr_SHC); 
 //        _rep (cr); } }                        /* put it to report             */
     

     if ( !stricmp ( a_FLI->cr_FuelCat,e_FCa_Pile )) {
       _rep (" >>-------> These predictions apply to area(s) underneath the Piles,\n");
       _rep ("            we assume that no soil heating takes place elsewhere.\n"); 
     }

     if ( !stricmp(a_SI->cr_BrnIg,"NO"))     /* Burnup ran & didn't ignite   */
       _rep (e_NoIg);

 
/* if we need to Save Totals - Fuel Totals Saver already check Name and all for error */
//  if ( this->_chRepTot->Checked ) {
    _GetTextBox (this->_txRepTot,cr_Name);
    if ( strcmp (cr_Name,"" ) ) {
 //   if ( CCW_isTotal () ) {                   /* if Totals Radio But is Chkd  */
 //      CCW_TotalNam (cr);                      /* get the Total Name           */
       if ( i_Max1 != -1 )                     /* if not still at init values  */
         i_Max1 = i_Max1 -1;                   /* adjust to proper layer #     */
       if ( i_Max2 != -1 )                     /* ie. 1->0, 2->1, etc          */
         i_Max2 = i_Max2 -1;                   /* See Note-1 above             */
       STT_Save (cr_Name, i_Max1 , i_Max2); }
   }

 return 1; 
} 



/**********************************************************
* Name: Report_Smoke
* Desc: Emissions report -> Report Window 
* 
***********************************************************/
int Wnd_Mai::Report_Smoke (d_CO *a_CO, d_FLI *a_FLI)
{
char cr[500], cr_Name[100]; 
float   f_PM10F, f_PM25F, f_CH4F, f_COF, f_CO2F;
float   f_PM10S, f_PM25S, f_CH4S, f_COS, f_CO2S;
float   f_NOXF,  f_NOXS,  f_SO2F, f_SO2S;
// float   Rnd; 
int   i,j;
float  f_Fla, f_Smo, f_Tot, f_AvgComEff;
int   i_FlaDur,i_SmoDur;
float f_TimLit, f_Tim1Hr, f_Tim10Hr, f_Tim100Hr, f_Tim1kHr, f_TimDuff;
char  cr_TimLit[100], cr_Tim1Hr[100], cr_Tim10Hr[100], cr_Tim100Hr[100], cr_Tim1kHr[100], cr_TimDuff[100];
char  cr_FlaDuf[20], cr_SmoDuf[20], cr_Time[50], cr_EU[40];
d_STS STS; 
char  cr_Un[30], cr_Ut[30];
   
   this->set_frm->EU_Unit(cr_EU);  /* Outpt Emis Unit frm Settings Wnd */ 

/* NOTE Emissions are in Pounds per ton in the a_CO  */

   if ( !stricmp (cr_EU,"Pound") ) {
     strcpy (cr_Un,e_lbAc); 
	 strcpy (cr_Ut,e_TonAc);
 //    Rnd =  0.5;    /* Makes it easy to test and change rounding */ 
	 f_PM10F = _RndEmiPnd(a_CO->f_PM10F);
     f_PM25F = _RndEmiPnd(a_CO->f_PM25F);
     f_CH4F  = _RndEmiPnd(a_CO->f_CH4F ); 
     f_COF   = _RndEmiPnd(a_CO->f_COF );  
     f_CO2F  = _RndEmiPnd(a_CO->f_CO2F);  
     f_NOXF =  _RndEmiPnd(a_CO->f_NOXF ); 
     f_SO2F =  _RndEmiPnd(a_CO->f_SO2F);  

     f_PM10S = _RndEmiPnd(a_CO->f_PM10S);
     f_PM25S = _RndEmiPnd(a_CO->f_PM25S);
     f_CH4S  = _RndEmiPnd(a_CO->f_CH4S); 
     f_COS   = _RndEmiPnd(a_CO->f_COS ); 
     f_CO2S  = _RndEmiPnd(a_CO->f_CO2S); 
     f_NOXS =  _RndEmiPnd(a_CO->f_NOXS);    /* no NOX are created during smoldering */ 
     f_SO2S =  _RndEmiPnd(a_CO->f_SO2S);   }

   else if ( !stricmp (cr_EU, "Ton") ) {
	 strcpy (cr_Un,e_TonAc);
	 strcpy (cr_Ut,e_TonAc);
     f_PM10F = a_CO->f_PM10F / 2000.0 ;
     f_PM25F = a_CO->f_PM25F / 2000.0 ;
     f_CH4F  = a_CO->f_CH4F  / 2000.0 ;
     f_COF   = a_CO->f_COF   / 2000.0 ;
     f_CO2F  = a_CO->f_CO2F  / 2000.0 ;
     f_NOXF =  a_CO->f_NOXF  / 2000.0 ;
     f_SO2F =  a_CO->f_SO2F  / 2000.0 ;

     f_PM10S = a_CO->f_PM10S / 2000.0 ;
     f_PM25S = a_CO->f_PM25S / 2000.0 ;
     f_CH4S  = a_CO->f_CH4S  / 2000.0 ;
     f_COS   = a_CO->f_COS   / 2000.0 ;
     f_CO2S  = a_CO->f_CO2S  / 2000.0 ;
     f_NOXS =  a_CO->f_NOXS  / 2000.0 ;
     f_SO2S =  a_CO->f_SO2S  / 2000.0 ; }

   else {
	 strcpy (cr_Un, e_GrSqM);
	 strcpy (cr_Ut, e_KgSqM); 
     f_PM10F = PndAcre_To_GramSqMt (a_CO->f_PM10F);
     f_PM25F = PndAcre_To_GramSqMt (a_CO->f_PM25F);
     f_CH4F  = PndAcre_To_GramSqMt (a_CO->f_CH4F );
     f_COF   = PndAcre_To_GramSqMt (a_CO->f_COF  );
     f_CO2F  = PndAcre_To_GramSqMt (a_CO->f_CO2F );
     f_NOXF =  PndAcre_To_GramSqMt (a_CO->f_NOXF );
     f_SO2F =  PndAcre_To_GramSqMt (a_CO->f_SO2F );

     f_PM10S = PndAcre_To_GramSqMt (a_CO->f_PM10S);
     f_PM25S = PndAcre_To_GramSqMt (a_CO->f_PM25S);
     f_CH4S  = PndAcre_To_GramSqMt (a_CO->f_CH4S );
     f_COS   = PndAcre_To_GramSqMt (a_CO->f_COS  );
     f_CO2S  = PndAcre_To_GramSqMt (a_CO->f_CO2S );
     f_NOXS =  PndAcre_To_GramSqMt (a_CO->f_NOXS );
     f_SO2S =  PndAcre_To_GramSqMt (a_CO->f_SO2S ); }


     if ( !stricmp (a_FLI->cr_EmisMethod,"New")) {
       _rep ("\n Emission Factor Groups\n"); 
       sprintf (cr, " Flaming and short term smoldering: %s\n", a_FLI->cr_EmisFlame);
       _rep (cr); 
       sprintf (cr, " Duff Residual Smoldering: %s\n", a_FLI->cr_EmisDuff);      
       _rep (cr); 
       sprintf (cr, " Coarse Wood Residual Smoldering: %s\n", a_FLI->cr_EmisSmold); 
       _rep (cr); } 

    _rep ("\n\n\n");
	sprintf (cr, "                       Emissions  -- %s\n",cr_Un);
	_rep (cr); 
// These headings are for the new emissions
//    _rep ("                 flaming & short    residual        total\n");
//    _rep ("                 term smoldering   smoldering         \n");    
    _rep ("                      flaming      smoldering       total\n"); 
    _rep (" --------------------------------------------------------------\n");

   
  if ( !stricmp (cr_EU,"Pound") ) {
    sprintf (cr, "    CO 2             %6.0f        %6.0f         %6.0f \n", f_CO2F , f_CO2S,  f_CO2F  + f_CO2S); _rep (cr); 
    sprintf (cr, "    CO               %6.0f        %6.0f         %6.0f \n", f_COF  , f_COS,   f_COF   + f_COS);  _rep (cr);
    sprintf (cr, "    CH 4             %6.0f        %6.0f         %6.0f \n", f_CH4F , f_CH4S,  f_CH4F  + f_CH4S); _rep (cr);
    sprintf (cr, "    NOX              %6.0f        %6.0f         %6.0f \n", f_NOXF , f_NOXS,  f_NOXF  + f_NOXS); _rep (cr);
    sprintf (cr, "    SO2              %6.0f        %6.0f         %6.0f \n", f_SO2F , f_SO2S,  f_SO2F  + f_SO2S); _rep (cr);
    sprintf (cr, "    PM 2.5           %6.0f        %6.0f         %6.0f \n", f_PM25F, f_PM25S, f_PM25F + f_PM25S);_rep (cr);
	sprintf (cr, "    PM 10            %6.0f        %6.0f         %6.0f \n", f_PM10F, f_PM10S, f_PM10F + f_PM10S);_rep (cr);
  }

  if ( !stricmp (cr_EU,"Ton") ) {
    sprintf (cr, "    CO 2             %6.3f          %6.3f        %6.3f \n", f_CO2F , f_CO2S ,  f_CO2F  + f_CO2S);_rep (cr);
    sprintf (cr, "    CO               %6.3f          %6.3f        %6.3f \n", f_COF  , f_COS  ,  f_COF   + f_COS);_rep (cr);
    sprintf (cr, "    CH 4             %6.3f          %6.3f        %6.3f \n", f_CH4F , f_CH4S ,  f_CH4F  + f_CH4S);_rep (cr);
    sprintf (cr, "    NOX              %6.3f          %6.3f        %6.3f \n", f_NOXF , f_NOXS ,  f_NOXF  + f_NOXS);_rep (cr);
    sprintf (cr, "    SO2              %6.3f          %6.3f        %6.3f \n", f_SO2F , f_SO2S ,  f_SO2F  + f_SO2S);_rep (cr);
    sprintf (cr, "    PM 2.5           %6.3f          %6.3f        %6.3f \n", f_PM25F, f_PM25S, f_PM25F + f_PM25S);_rep (cr);
    sprintf (cr, "    PM 10            %6.3f          %6.3f        %6.3f \n", f_PM10F, f_PM10S, f_PM10F + f_PM10S);_rep (cr);
  }

  if ( !stricmp (cr_EU,"Metric") ) {
    sprintf (cr, "    CO 2             %6.0f        %6.0f        %6.0f \n", f_CO2F , f_CO2S,  f_CO2F  + f_CO2S);_rep (cr);
    sprintf (cr, "    CO               %6.0f        %6.0f        %6.0f \n", f_COF  , f_COS,   f_COF   + f_COS);_rep (cr);
    sprintf (cr, "    CH 4             %6.0f        %6.0f        %6.0f \n", f_CH4F , f_CH4S,  f_CH4F  + f_CH4S);_rep (cr);
    sprintf (cr, "    NOX              %6.0f        %6.0f        %6.0f \n", f_NOXF , f_NOXS,  f_NOXF  + f_NOXS);_rep (cr);
    sprintf (cr, "    SO2              %6.0f        %6.0f        %6.0f \n", f_SO2F , f_SO2S,  f_SO2F  + f_SO2S);_rep (cr);
    sprintf (cr, "    PM 2.5           %6.0f        %6.0f        %6.0f \n", f_PM25F, f_PM25S, f_PM25F + f_PM25S);_rep (cr);
    sprintf (cr, "    PM 10            %6.0f        %6.0f        %6.0f \n", f_PM10F, f_PM10S, f_PM10F + f_PM10S);_rep (cr);
  }

  _rep (" --------------------------------------------------------------\n");


/* if we need to Save Totals - Fuel Totals Saver already check Name and all for error */
//  if ( this->_chRepTot->Checked ) {
    _GetTextBox (this->_txRepTot,cr_Name);
    if ( strcmp (cr_Name,"")) {
      STS.f_TotPM10 = a_CO->f_PM10F + a_CO->f_PM10S;      /* Save for doing Summaries     */
      STS.f_TotPM25 = a_CO->f_PM25F + a_CO->f_PM25S;
      STS.f_TotCH4  = a_CO->f_CH4F  + a_CO->f_CH4S;
      STS.f_TotCO2  = a_CO->f_CO2F  + a_CO->f_CO2S;
      STS.f_TotCO   = a_CO->f_COF   + a_CO->f_COS;
      STS.f_TotNOX  = a_CO->f_NOXF  + a_CO->f_NOXS;
      STS.f_TotSO2  = a_CO->f_SO2F  + a_CO->f_SO2S;
      STS_Add(&STS, cr_Name); 
   }

  i_FlaDur = (int)a_CO->f_FlaDur;
  i_SmoDur = (int)a_CO->f_SmoDur;
  HrMinSec (i_FlaDur, cr_FlaDuf);
  HrMinSec (i_SmoDur, cr_SmoDuf);

/* 8-20-18 - change how we get total smo fla tot */
// f_Fla = a_CO->f_FlaCon;               /* Tot Flam & Smold Consmd from brnup*/
//  f_Smo = a_CO->f_SmoCon;               /* these tots include all fuel loads */
//  f_Tot = f_Fla + f_Smo;                /* duff,shr,dw,etc..-> in Tons Per Acre  */

/* 8-20-18 Get by adding up each fule compoent at each time step */
 _TotSmoFla (&f_Fla, &f_Smo );
  f_Tot = f_Fla + f_Smo;  
 
  _rep ("\n\n");
  _rep ("                                      Consumption     Duration\n");
//  sprintf (cr, "                                        %s    hour:min:sec\n", cr_Ut);
  sprintf (cr, "                                          T/ac    hour:min:sec\n");
  _rep (cr); 

 // if ( !stricmp (cr_EU,"Metric") ) {
 //    f_Fla = TPA_To_KiSq (f_Fla); 
 //    f_Smo = TPA_To_KiSq (f_Smo); 
 //    f_Tot = TPA_To_KiSq (f_Tot);  } 

/* NOTE: decided for now to always show consumed fuel as Tons Per Acre */ 

  sprintf (cr," Short Term Flaming and Smoldering:   %8.2f        %s\n",f_Fla, cr_FlaDuf);
  _rep (cr);
  sprintf (cr,"               residual smoldering:   %8.2f        %s\n",f_Smo, cr_SmoDuf);
  _rep (cr);
  sprintf (cr,"                             Total:   %8.2f\n",f_Tot);
  _rep (cr); 

/* Burn out times for each fuel component */
  f_TimLit   = _TimeOut (&a_CO->sr_SGV[0],  "Litter",cr_TimLit  );
  f_Tim1Hr   = _TimeOut (&a_CO->sr_SGV[0],  "1Hr",   cr_Tim1Hr  );
  f_Tim10Hr  = _TimeOut (&a_CO->sr_SGV[0],  "10Hr",  cr_Tim10Hr );
  f_Tim100Hr = _TimeOut (&a_CO->sr_SGV[0],  "100Hr", cr_Tim100Hr);
  f_Tim1kHr  = _TimeOut (&a_CO->sr_SGV[0],  "1kHr",  cr_Tim1kHr );
  f_TimDuff  = _TimeOut (&a_CO->sr_SGV[0],  "Duff",  cr_TimDuff );

  sprintf (cr, "\n\n                              Burnout Time \n");
  _rep (cr); 

  sprintf (cr, "                        Component   hour:min:sec  total seconds \n");
  _rep (cr); 
                                            
  sprintf (cr, "                         Litter       %s      %4.0f \n", cr_TimLit , f_TimLit  );
  _rep (cr);

  sprintf (cr, "                         1 Hour       %s      %4.0f \n", cr_Tim1Hr , f_Tim1Hr  );
  _rep (cr);

  sprintf (cr, "                         10 Hour      %s      %4.0f \n", cr_Tim10Hr, f_Tim10Hr );
  _rep (cr);

  sprintf (cr, "                         100 Hour     %s      %4.0f \n", cr_Tim100Hr,f_Tim100Hr);
  _rep (cr);

  sprintf (cr, "                         1000 Hour    %s      %4.0f \n", cr_Tim1kHr, f_Tim1kHr );
  _rep (cr);

  sprintf (cr, "                         Duff         %s      %4.0f \n", cr_TimDuff, f_TimDuff );
  _rep (cr); 

 // sprintf (cr, "\n                    fuel consumed amounts less than %7.6f are considered out \n",e_out); 
 // _rep (cr); 

  if ( f_Tim1Hr < 0 || f_Tim10Hr < 0 || f_Tim100Hr < 0 || f_Tim1kHr < 0 || f_TimDuff < 0 ) 
     _rep ("                    -1 = component is still burning\n"); 
       

// took this out when we started doing emissions the new way
 // if ( f_Tot != 0 )
 //    f_AvgComEff = ((f_Fla * e_ComEffFla) + (f_Smo * e_ComEffSmo)) / f_Tot;
 // else
 //    f_AvgComEff = 0;
//  sprintf (cr, " Unit Average Combustion Efficiency:  %8.2f\n",f_AvgComEff);
//  _rep (cr); 

  return 1; 
}

/***********************************************************
* Name: _TotSmoFla
* Desc: Get the total consumed fuels 
*       Go thru the SGV table adding up each fuel
*       component at each time step. 
***********************************************************/ 
int _TotSmoFla (float *af_Fla,  float *af_Smo )
{
int i; 
float fN_Fla, fN_Smo;
float f_Fla, f_Smo;
char  AA[10][20];

   fN_Fla = fN_Smo = 0; 

   strcpy (AA[0], "1kHr"); 
   strcpy (AA[1], "100Hr"); 
   strcpy (AA[2], "10Hr"); 
   strcpy (AA[3], "1Hr");
   strcpy (AA[4], "Litter");
   strcpy (AA[5], "Duff");
   strcpy (AA[6], "HSFB");
   strcpy (AA[7], "");

   for ( i = 0; i < 100; i++ ) {
     if ( !stricmp (AA[i], "" ) ) 
        break; 
     SGV_GetFS (AA[i], &f_Fla, &f_Smo);
     fN_Fla += f_Fla;
     fN_Smo += f_Smo; 
   }

  *af_Fla = fN_Fla;
  *af_Smo = fN_Smo; 
  return 1; 
}

/*************************************************************
* Name: _TimeOut
* Desc: Determine the burnout time for a fuel component
*   In:  a...first SGV record of the arry of these
*        N..size of SGV arrary
*        cr_Cmp...component name, see below
* Out: cr_Time...in Hr:Mi:Se
*************************************************************/
int _TimeOut (d_SGV *a_SGV,  char cr_Cmp[], char cr_Time[] )
{
int i,n;
float f, f_Sec, f_Con, f_Load, f_RemLoa;

/* Look in first time step to see if there is any fuel for component */
   f_Load = _Load (&a_SGV[0], cr_Cmp);
   if ( f_Load == 0 ) {      /* If no fuel load in */
     f_Sec = 0;               /*  burnout is at 0 */
     goto Time; }

/* Look in the SGV array of burnup results to get burnout time */
   f_Sec = SGV_BurnOutTime(cr_Cmp);  

Time:
   HrMinSec ( (int) f_Sec,  cr_Time );

   return  f_Sec; 
}


/**********************************************************
* Get the remaining fuel load for the
*  specifed fuel component
**********************************************************/
float _Load (d_SGV *a, char cr_Cmp[])
{
float f;

  if ( !stricmp (cr_Cmp,"Litter") ) 
    f  = a->f_rm_Lit;
  else if ( !stricmp (cr_Cmp,"1Hr") ) 
    f  = a->f_rm_1Hr;
  else if ( !stricmp (cr_Cmp,"10Hr") ) 
    f  = a->f_rm_10Hr;
  else if ( !stricmp (cr_Cmp, "100Hr") )
    f = a->f_rm_100Hr;
  else if ( !stricmp (cr_Cmp, "1kHr") )
    f = a->f_rm_1kHr;

/* NOTE - Duff is really the Consumed amount, we don't have */
/*  the fuel load but it should work out ok anyway for the caller */
  else if ( !stricmp (cr_Cmp, "Duff") )
    f = a->f_DuffCon;
  else 
    myMessageBox ("Logic Error _Load()");
    
  return f; 
}

/*************************************************************
* Name: Report_Fuel
* Desc: Do Just the Fuel Report
*       I made this function to do just the Fuels after BK
*       suggested not doing putting the fuel info on the
*       emissions or soil reports. 
**************************************************************/
int  Wnd_Mai::Report_Fuel (d_CI *a_CI, d_CO *a_CO)
{
float f_Percent, f_Pre, f_Pos;
char c; 
char cr_Date[100], cr[500], cr_adj[100], cr_CmdTyp[50];
/* Carbon Defines */
#define e_LitDufCar 0.37
#define e_LivDeaCar 0.50 

/* This will make it do the individual 3+ size classes */
   strcpy (cr_CmdTyp, "Smoke" ); 


   _rep ("\n");
   _rep ("                              FUEL CONSUMPTION TABLE \n");
   _rep (" Fuel                   Preburn   Consumed  Postburn  Percent   Equation\n");
   _rep (" Component                Load      Load      Load    Reduced   Reference  Moist.\n");
 
   sprintf (cr, " Name                    (%s)    (%s)    (%s)     (%c)     Number      (%c)\n", e_TonAc, e_TonAc, e_TonAc,  '%', '%');
   _rep(cr); 
   _rep (" _____________________________________________________________________________\n");

 
/*...........................................................................*/
/* Litter                                                                    */
   c = this->AdjChar (e_txLitter);           /* typical, light, heavy        */
   Rep_SavAdj (c,cr_adj);                    /* so we know we had one        */
// test-ld larry
   if ( a_CO->i_LitEqu != e_CP_PerEq ) {
      sprintf (cr," Litter                %7.2f %c %7.2f   %7.2f   %6.1f     %3d\n",
            a_CO->f_LitPre,c, a_CO->f_LitCon, a_CO->f_LitPos,a_CO->f_LitPer, a_CO->i_LitEqu);
      _rep (cr); }   
   else {  /* For Coastal Plain we need to show Litter Moisture */
      sprintf (cr, " Litter                %7.2f %c %7.2f   %7.2f   %6.1f     %3d      %5.1f \n",
            a_CO->f_LitPre,c, a_CO->f_LitCon, a_CO->f_LitPos,a_CO->f_LitPer, a_CO->i_LitEqu,a_CI->f_LitMoi);
      _rep(cr); }

/*...........................................................................*/
/* DW 1 Hr                                                                   */
   c = this->AdjChar (e_tx1Hr);
   Rep_SavAdj (c,cr_adj);
   sprintf (cr, " Wood (0-1/4 inch)     %7.2f %c %7.2f   %7.2f   %6.1f     %3d\n",
             a_CO->f_DW1Pre,c,a_CO->f_DW1Con,a_CO->f_DW1Pos,
             a_CO->f_DW1Per,a_CO->i_DW1Equ);
   _rep (cr); 

/*...........................................................................*/
/* DW 10 Hr                                                                  */
   c = this->AdjChar (e_tx10Hr);
   Rep_SavAdj (c,cr_adj);
   sprintf (cr, " Wood (1/4-1 inch)     %7.2f %c %7.2f   %7.2f   %6.1f     %3d      %5.1f\n",
             a_CO->f_DW10Pre,c,a_CO->f_DW10Con,a_CO->f_DW10Pos,
             a_CO->f_DW10Per,a_CO->i_DW10Equ, a_CI->f_MoistDW10);
   _rep (cr); 

/*...........................................................................*/
/* DW 100 Hr                                                                  */
   c = AdjChar (e_tx100Hr);
   Rep_SavAdj (c,cr_adj);
   sprintf (cr, " Wood (1-3 inch)       %7.2f %c %7.2f   %7.2f   %6.1f     %3d \n",
             a_CO->f_DW100Pre,c,a_CO->f_DW100Con,a_CO->f_DW100Pos,
             a_CO->f_DW100Per,a_CO->i_DW100Equ);
   _rep(cr);

/*...........................................................................*/
/* DW 1000 Hr                                                                */
   c = AdjChar (e_tx1kHr);
   Rep_SavAdj (c,cr_adj);

   sprintf (cr, " Wood (3+ inch) Sound  %7.2f %c %7.2f   %7.2f   %6.1f     %3d      %5.1f\n",
             a_CO->f_Snd_DW1kPre, c, a_CO->f_Snd_DW1kCon,
             a_CO->f_Snd_DW1kPos, a_CO->f_Snd_DW1kPer,
             a_CO->i_Snd_DW1kEqu, a_CI->f_MoistDW1000);
   _rep (cr); 

   if ( !stricmp (cr_CmdTyp, "Smoke") ) {
    Rep_Ind ("3->6 ", a_CI->f_Snd_DW3,  a_CO->f_S3Pos);
    Rep_Ind ("6->9 ", a_CI->f_Snd_DW6,  a_CO->f_S6Pos);
    Rep_Ind ("9->20", a_CI->f_Snd_DW9,  a_CO->f_S9Pos);
    Rep_Ind ("20-> ", a_CI->f_Snd_DW20, a_CO->f_S20Pos); }

   sprintf (cr," Wood (3+ inch) Rotten %7.2f %c %7.2f   %7.2f   %6.1f     %3d      %5.1f\n",
             a_CO->f_Rot_DW1kPre, c, a_CO->f_Rot_DW1kCon,
             a_CO->f_Rot_DW1kPos, a_CO->f_Rot_DW1kPer,
             a_CO->i_Rot_DW1kEqu, a_CI->f_MoistDW1000);
   _rep (cr); 

   if ( !stricmp (cr_CmdTyp, "Smoke") ) {
    Rep_Ind ("3->6 ", a_CI->f_Rot_DW3,  a_CO->f_R3Pos);
    Rep_Ind ("6->9 ", a_CI->f_Rot_DW6,  a_CO->f_R6Pos);
    Rep_Ind ("9->20", a_CI->f_Rot_DW9,  a_CO->f_R9Pos);
    Rep_Ind ("20-> ", a_CI->f_Rot_DW20, a_CO->f_R20Pos); }

/*...........................................................................*/
/* Duff Tons per Acre                                                        */
   c = AdjChar (e_txDuff);
   Rep_SavAdj (c,cr_adj);
   sprintf (cr," Duff                  %7.2f %c %7.2f   %7.2f   %6.1f     %3d      %5.1f\n",
             a_CO->f_DufPre, c, a_CO->f_DufCon,
             a_CO->f_DufPos, a_CO->f_DufPer,
             a_CO->i_DufEqu, a_CI->f_MoistDuff);
   _rep(cr);

/*...........................................................................*/
/* Herbs                                                                     */
   c = AdjChar (e_txHerb);
   Rep_SavAdj (c,cr_adj);
   sprintf (cr, " Herbaceous            %7.2f %c %7.2f   %7.2f   %6.1f     %3d\n",
            a_CO->f_HerPre, c,a_CO->f_HerCon,
            a_CO->f_HerPos, a_CO->f_HerPer,
            a_CO->i_HerEqu);
   _rep (cr);
/*...........................................................................*/
/* Shrub                                                                     */
   c = AdjChar (e_txShrub);
   Rep_SavAdj (c,cr_adj);
   sprintf (cr," Shrubs                %7.2f %c %7.2f   %7.2f   %6.1f     %3d\n",
            a_CO->f_ShrPre, c,a_CO->f_ShrCon,
            a_CO->f_ShrPos, a_CO->f_ShrPer,
            a_CO->i_ShrEqu);
   _rep(cr);

/*...........................................................................*/
/* Crown Foliage                                                             */
   c = AdjChar (e_txFoliage);
   Rep_SavAdj (c,cr_adj);
   sprintf (cr, " Crown foliage         %7.2f %c %7.2f   %7.2f   %6.1f     %3d\n",
            a_CO->f_FolPre, c,a_CO->f_FolCon,
            a_CO->f_FolPos, a_CO->f_FolPer,
            a_CO->i_FolEqu);
   _rep(cr); 

/*...........................................................................*/
/* Branch Foliage                                                           */
   c = AdjChar (e_txBranch);
   Rep_SavAdj (c,cr_adj);
   sprintf (cr,  " Crown branchwood      %7.2f %c %7.2f   %7.2f   %6.1f     %3d\n",
            a_CO->f_BraPre, c,a_CO->f_BraCon,
            a_CO->f_BraPos, a_CO->f_BraPer,
            a_CO->i_BraEqu);
   _rep (cr); 

/*...........................................................................*/

   if ( a_CO->f_TotPre == 0 )               /* Had no Loads                 */
      f_Percent = 0;
   else                                      /* Get percent of consumed      */
      f_Percent = ( a_CO->f_TotCon / a_CO->f_TotPre ) * 100;
   _rep ( " ___________________________________________________________________________\n");
   sprintf (cr,  " Total Fuels           %7.2f   %7.2f   %7.2f   %6.1f\n\n", a_CO->f_TotPre, a_CO->f_TotCon, a_CO->f_TotPos, f_Percent);
   _rep (cr);

   if ( cr_adj[0] == e_Adj_Usr )                       /* if we put any    */
     _rep (" 'u'  Preburn Load is User adjusted\n"); /* adj marks on the */

   if ( cr_adj[1] == e_Adj_HA )                        /* report we want to*/
     _rep (" '+'  Preburn Load is Heavy/Abundant\n");/* explain them to  */

   if ( cr_adj[2] == e_Adj_LS )                        /* user             */
     _rep (" '-'  Preburn Load is Light/Sparse\n");

   _rep ("\n\n");
  // _rep (" FIRE EFFECTS ON FOREST FLOOR COMPONENTS\n");
   _rep (" FIRE EFFECTS ON FOREST FLOOR\n");
 //  _rep ("\n");

//   sprintf (cr, " Duff Depth Consumed (in)  %6.1f     Equation:%3d\n", a_CO->f_DufDepCon, a_CO->i_DufDepEqu);
//   _rep (cr); 
   sprintf (cr,"   Mineral Soil Exposed (%c)",'%');
   _rep (cr); 
   sprintf (cr,"  %6.1f     Equation:%3d\n", a_CO->f_MSEPer,a_CO->i_MSEEqu);
   _rep (cr); 


/*--------------------------------------------------------------------*/
/* Carbon Table stuff */
   float f_PreTot = 0; 
   float f_PosTot = 0; 

   _rep ("\n\n\n");  
   _rep ("   Ground and Surface Fuel Carbon Loading\n\n");
   _rep (" Fuel                  Preburn      Postburn \n");
   _rep (" Component             Carbon        Carbon \n");

   sprintf (cr, " Name                  (%s)        (%s)\n", e_TonAc, e_TonAc);
   _rep (cr); 


   _rep (" ____________________________________________\n");
  
/* Litter.......... */
   f_Pre = a_CO->f_LitPre*e_LitDufCar;
   f_Pos = a_CO->f_LitPos*e_LitDufCar;
   sprintf (cr, " Litter               %7.2f      %7.2f\n" ,f_Pre,f_Pos );
   _rep (cr);
   f_PreTot += f_Pre;
   f_PosTot += f_Pos;
 
/* Wood - add all the wood together  */
   f_Pre = a_CO->f_DW1Pre + a_CO->f_DW10Pre + a_CO->f_DW100Pre +
           a_CO->f_Snd_DW1kPre + a_CO->f_Rot_DW1kPre;
   f_Pre = f_Pre * e_LivDeaCar;

   f_Pos = a_CO->f_DW1Pos + a_CO->f_DW10Pos + a_CO->f_DW100Pos +
           a_CO->f_Snd_DW1kPos + a_CO->f_Rot_DW1kPos;
   f_Pos = f_Pos * e_LivDeaCar;

   sprintf (cr, " Wood                 %7.2f      %7.2f \n" , f_Pre,  f_Pos);
   _rep (cr); 
   f_PreTot += f_Pre;
   f_PosTot += f_Pos;

/* Duff................................. */
   f_Pre = a_CO->f_DufPre * e_LitDufCar;
   f_Pos = a_CO->f_DufPos * e_LitDufCar;
   sprintf (cr, " Duff                 %7.2f      %7.2f \n" ,f_Pre,f_Pos );
   _rep (cr); 
   f_PreTot += f_Pre;
   f_PosTot += f_Pos;

/* Herbs...........................*/
   f_Pre = a_CO->f_HerPre * e_LivDeaCar;
   f_Pos = a_CO->f_HerPos * e_LivDeaCar;
   sprintf (cr, " Herbaceous           %7.2f      %7.2f \n" ,f_Pre,f_Pos );
   _rep (cr);   
   f_PreTot += f_Pre;
   f_PosTot += f_Pos;

/* Shrub...........................*/
   f_Pre = a_CO->f_ShrPre * e_LivDeaCar;
   f_Pos = a_CO->f_ShrPos * e_LivDeaCar;
   sprintf (cr, " Shrub                %7.2f      %7.2f \n" ,f_Pre,f_Pos );
   _rep (cr);  
   f_PreTot += f_Pre;
   f_PosTot += f_Pos;

/* Foliage Branch..................*/
   f_Pre = ( a_CO->f_FolPre+ a_CO->f_BraPre ) * e_LivDeaCar ;
   f_Pos = ( a_CO->f_FolPos + a_CO->f_BraPos ) * e_LivDeaCar ;
   sprintf (cr, " Foliage+Branch       %7.2f      %7.2f \n" ,  f_Pre, f_Pos);
   _rep (cr);
   f_PreTot += f_Pre;
   f_PosTot += f_Pos;

   _rep (" ____________________________________________\n");
   sprintf (cr," Total                %7.2f      %7.2f \n", f_PreTot, f_PosTot);
  _rep (cr); 

// 9-17-2018, Ticket-218, remove Consume Fuel Component table 
// Consumed_FlaSmo ();

 return 1;  
} 

/**********************************************************************
*
*  
*  In: cr_CmdTyp...."Fuel", "Smoke", "Soil"  type of report 
***********************************************************************/
int Wnd_Mai::Report_Mngr_Old (char cr_CmdTyp[]) 
{
float f_Percent; 
char c, cr_Name[100];
char cr_ErrMes[3000], cr[1000], cr_adj[100], cr_Date[100]; 
d_CO CO,  *a_CO;
d_CI CI,  *a_CI;
d_SI SI,  *a_SI;
d_SO SO,  *a_SO;
d_FLI  s_FLI; 
d_ST ST;
d_STT STT; 
d_STS STS; 

  a_CO = &CO;  /* just doing this to not re-write old code that uses a_CO-> */
  a_CI = &CI; 
  a_SI = &SI;
  a_SO = &SO; 

  CI_Init (&CI);
  CO_Init (&CO); 
  SI_Init (&SI);
  SO_Init (&SO);  

  if ( !Get_Inputs(&s_FLI)) {
     myMessageBox(s_FLI.cr_ErrMes);
     return 0; }

  if ( !CalcSub(&s_FLI, &CI, &CO, &SI, &SO, "", cr_ErrMes)) {
     myMessageBox(cr_ErrMes);
     return 0; }

  if ( !Set_FuelTotal(&CO,cr_ErrMes) ) {
     myMessageBox (cr_ErrMes);
     return 0; }
 
   GetDate (cr_Date); 
   sprintf (cr, " TITLE: Results of FOFEM model execution on date: %s\n",cr_Date);
   _rep (cr); 
   _rep ("\n");
   _rep ("                       FUEL CONSUMPTION CALCULATIONS\n");
 
   Report_SubTit (&s_FLI, "");
   _rep ("\n");
   _rep ("                                  FUEL CONSUMPTION TABLE \n");
   _rep (" Fuel                 Preburn   Consumed  Postburn  Percent   Equation           \n");
   _rep (" Component              Load      Load      Load    Reduced   Reference  Moist.\n");
 
   sprintf (cr, " Name                  (%s)    (%s)    (%s)     (%c)     Number      (%c)\n", e_TonAc, e_TonAc, e_TonAc,  '%','%');
   _rep(cr); 
   _rep (" _____________________________________________________________________________\n");

 
/*...........................................................................*/
/* Litter                                                                    */
   c = this->AdjChar (e_txLitter);           /* typical, light, heavy        */
   Rep_SavAdj (c,cr_adj);                    /* so we know we had one        */
// test-ld larry
   if ( a_CO->i_LitEqu != e_CP_PerEq ) {
      sprintf (cr," Litter                %7.2f %c %7.2f   %7.2f   %6.1f     %3d\n",
            a_CO->f_LitPre,c, a_CO->f_LitCon, a_CO->f_LitPos,a_CO->f_LitPer, a_CO->i_LitEqu);
      _rep (cr); }   
   else {
      sprintf (cr, " Litter                %7.2f %c %7.2f   %7.2f   %6.1f     %3d      %5.1f\n",
            a_CO->f_LitPre,c, a_CO->f_LitCon, a_CO->f_LitPos,a_CO->f_LitPer, a_CO->i_LitEqu,a_CI->f_LitMoi);
      _rep(cr); }

/*...........................................................................*/
/* DW 1 Hr                                                                   */
   c = this->AdjChar (e_tx1Hr);
   Rep_SavAdj (c,cr_adj);
   sprintf (cr, " Wood (0-1/4 inch)     %7.2f %c %7.2f   %7.2f   %6.1f     %3d\n",
             a_CO->f_DW1Pre,c,a_CO->f_DW1Con,a_CO->f_DW1Pos,
             a_CO->f_DW1Per,a_CO->i_DW1Equ);
   _rep (cr); 

/*...........................................................................*/
/* DW 10 Hr                                                                  */
   c = this->AdjChar (e_tx10Hr);
   Rep_SavAdj (c,cr_adj);
   sprintf (cr, " Wood (1/4-1 inch)     %7.2f %c %7.2f   %7.2f   %6.1f     %3d      %5.1f\n",
             a_CO->f_DW10Pre,c,a_CO->f_DW10Con,a_CO->f_DW10Pos,
             a_CO->f_DW10Per,a_CO->i_DW10Equ, a_CI->f_MoistDW10);
   _rep (cr); 

/*...........................................................................*/
/* DW 100 Hr                                                                  */
   c = AdjChar (e_tx100Hr);
   Rep_SavAdj (c,cr_adj);
   sprintf (cr, " Wood (1-3 inch)       %7.2f %c %7.2f   %7.2f   %6.1f     %3d \n",
             a_CO->f_DW100Pre,c,a_CO->f_DW100Con,a_CO->f_DW100Pos,
             a_CO->f_DW100Per,a_CO->i_DW100Equ);
   _rep(cr);

/*...........................................................................*/
/* DW 1000 Hr                                                                */
   c = AdjChar (e_tx1kHr);
   Rep_SavAdj (c,cr_adj);

   sprintf (cr, " Wood (3+ inch) Sound  %7.2f %c %7.2f   %7.2f   %6.1f     %3d      %5.1f\n",
             a_CO->f_Snd_DW1kPre, c, a_CO->f_Snd_DW1kCon,
             a_CO->f_Snd_DW1kPos, a_CO->f_Snd_DW1kPer,
             a_CO->i_Snd_DW1kEqu, a_CI->f_MoistDW1000);
   _rep (cr); 

   if ( !stricmp (cr_CmdTyp, "Smoke") ) {
    Rep_Ind ("3->6 ", a_CI->f_Snd_DW3,  a_CO->f_S3Pos);
    Rep_Ind ("6->9 ", a_CI->f_Snd_DW6,  a_CO->f_S6Pos);
    Rep_Ind ("9->20", a_CI->f_Snd_DW9,  a_CO->f_S9Pos);
    Rep_Ind ("20-> ", a_CI->f_Snd_DW20, a_CO->f_S20Pos); }

   sprintf (cr," Wood (3+ inch) Rotten %7.2f %c %7.2f   %7.2f   %6.1f     %3d      %5.1f\n",
             a_CO->f_Rot_DW1kPre, c, a_CO->f_Rot_DW1kCon,
             a_CO->f_Rot_DW1kPos, a_CO->f_Rot_DW1kPer,
             a_CO->i_Rot_DW1kEqu, a_CI->f_MoistDW1000);
   _rep (cr); 

   if ( !stricmp (cr_CmdTyp, "Smoke") ) {
    Rep_Ind ("3->6 ", a_CI->f_Rot_DW3,  a_CO->f_R3Pos);
    Rep_Ind ("6->9 ", a_CI->f_Rot_DW6,  a_CO->f_R6Pos);
    Rep_Ind ("9->20", a_CI->f_Rot_DW9,  a_CO->f_R9Pos);
    Rep_Ind ("20-> ", a_CI->f_Rot_DW20, a_CO->f_R20Pos); }

/*...........................................................................*/
/* Duff Tons per Acre                                                        */
   c = AdjChar (e_txDuff);
   Rep_SavAdj (c,cr_adj);
   sprintf (cr," Duff                  %7.2f %c %7.2f   %7.2f   %6.1f     %3d      %5.1f\n",
             a_CO->f_DufPre, c, a_CO->f_DufCon,
             a_CO->f_DufPos, a_CO->f_DufPer,
             a_CO->i_DufEqu, a_CI->f_MoistDuff);
   _rep(cr);

/*...........................................................................*/
/* Herbs                                                                     */
   c = AdjChar (e_txHerb);
   Rep_SavAdj (c,cr_adj);
   sprintf (cr, " Herbaceous            %7.2f %c %7.2f   %7.2f   %6.1f     %3d\n",
            a_CO->f_HerPre, c,a_CO->f_HerCon,
            a_CO->f_HerPos, a_CO->f_HerPer,
            a_CO->i_HerEqu);
   _rep (cr);
/*...........................................................................*/
/* Shrub                                                                     */
   c = AdjChar (e_txShrub);
   Rep_SavAdj (c,cr_adj);
   sprintf (cr," Shrubs                %7.2f %c %7.2f   %7.2f   %6.1f     %3d\n",
            a_CO->f_ShrPre, c,a_CO->f_ShrCon,
            a_CO->f_ShrPos, a_CO->f_ShrPer,
            a_CO->i_ShrEqu);
   _rep(cr);

/*...........................................................................*/
/* Crown Foliage                                                             */
   c = AdjChar (e_txFoliage);
   Rep_SavAdj (c,cr_adj);
   sprintf (cr, " Crown foliage         %7.2f %c %7.2f   %7.2f   %6.1f     %3d\n",
            a_CO->f_FolPre, c,a_CO->f_FolCon,
            a_CO->f_FolPos, a_CO->f_FolPer,
            a_CO->i_FolEqu);
   _rep(cr); 

/*...........................................................................*/
/* Branch Foliage                                                           */
   c = AdjChar (e_txBranch);
   Rep_SavAdj (c,cr_adj);
   sprintf (cr,  " Crown branchwood      %7.2f %c %7.2f   %7.2f   %6.1f     %3d\n",
            a_CO->f_BraPre, c,a_CO->f_BraCon,
            a_CO->f_BraPos, a_CO->f_BraPer,
            a_CO->i_BraEqu);
   _rep (cr); 

/*...........................................................................*/

   if ( a_CO->f_TotPre == 0 )               /* Had no Loads                 */
      f_Percent = 0;
   else                                      /* Get percent of consumed      */
      f_Percent = ( a_CO->f_TotCon / a_CO->f_TotPre ) * 100;
   _rep ( " ___________________________________________________________________________\n");
   sprintf (cr,  " Total Fuels           %7.2f   %7.2f   %7.2f   %6.1f\n\n", a_CO->f_TotPre, a_CO->f_TotCon, a_CO->f_TotPos, f_Percent);
   _rep (cr);

   if ( cr_adj[0] == e_Adj_Usr )                       /* if we put any    */
     _rep (" 'u'  Preburn Load is User adjusted\n"); /* adj marks on the */

   if ( cr_adj[1] == e_Adj_HA )                        /* report we want to*/
     _rep (" '+'  Preburn Load is Heavy/Abundant\n");/* explain them to  */

   if ( cr_adj[2] == e_Adj_LS )                        /* user             */
     _rep (" '-'  Preburn Load is Light/Sparse\n");

   _rep ("\n\n");
   _rep (" FIRE EFFECTS ON FOREST FLOOR COMPONENTS\n");
   _rep ("\n");

   sprintf (cr, " Duff Depth Consumed (in)  %6.1f     Equation:%3d\n", a_CO->f_DufDepCon, a_CO->i_DufDepEqu);
   _rep (cr); 
   sprintf (cr," Mineral Soil Exposed (%c)",'%');
   _rep (cr); 
   sprintf (cr,"  %6.1f     Equation:%3d\n", a_CO->f_MSEPer,a_CO->i_MSEEqu);
   _rep (cr); 

 
   if ( !stricmp (cr_CmdTyp,"Fuel"))
     return 1; 
   if ( !stricmp (cr_CmdTyp,"Soil")) 
     goto Soil; 

/*___________________________________________________________________*/
/*         Smoke Report */
int   i_PM10F, i_PM25F, i_CH4F, i_COF, i_CO2F;
int   i_PM10S, i_PM25S, i_CH4S, i_COS, i_CO2S;
int   i_NOXF, i_NOXS, i_SO2F, i_SO2S;


float  f_Fla, f_Smo, f_Tot, f_AvgComEff;
int   i_FlaDur,i_SmoDur;
char  cr_FlaDuf[20], cr_SmoDuf[20];


   i_PM10F = a_CO->f_PM10F + 0.5;
   i_PM25F = a_CO->f_PM25F + 0.5;
   i_CH4F  = a_CO->f_CH4F  + 0.5;
   i_COF   = a_CO->f_COF   + 0.5;
   i_CO2F  = a_CO->f_CO2F  + 0.5;
   i_NOXF =  a_CO->f_NOXF  + 0.5;
   i_SO2F =  a_CO->f_SO2F  + 0.5;


   i_PM10S = a_CO->f_PM10S + 0.5;
   i_PM25S = a_CO->f_PM25S + 0.5;
   i_CH4S  = a_CO->f_CH4S  + 0.5;
   i_COS   = a_CO->f_COS   + 0.5;
   i_CO2S  = a_CO->f_CO2S  + 0.5;
   i_NOXS =  a_CO->f_NOXS  + 0.5;
   i_SO2S =  a_CO->f_SO2S  + 0.5;


  _rep ("\n\n\n");
  sprintf (cr, "                     Emissions  -- (%s) \n", e_lbAc);
  _rep (cr); 
  _rep ("                    flaming      smoldering      total\n");
  _rep (" --------------------------------------------------------------\n");
  sprintf (cr, " PM 10           %8d         %8d      %8d \n", i_PM10F, i_PM10S, i_PM10F + i_PM10S);
  _rep (cr);
  sprintf (cr, " PM 2.5          %8d         %8d      %8d \n", i_PM25F, i_PM25S, i_PM25F + i_PM25S);
  _rep (cr);
  sprintf (cr, " CH 4            %8d         %8d      %8d \n", i_CH4F , i_CH4S,  i_CH4F  + i_CH4S);
  _rep (cr); 
  sprintf (cr, " CO              %8d         %8d      %8d \n", i_COF  , i_COS,   i_COF   + i_COS);
  _rep (cr);
  sprintf (cr, " CO 2            %8d         %8d      %8d \n", i_CO2F , i_CO2S,  i_CO2F  + i_CO2S);
  _rep (cr);
  sprintf (cr, " NOX             %8d         %8d      %8d \n", i_NOXF , i_NOXS,  i_NOXF  + i_NOXS);
  _rep (cr);
  sprintf (cr, " SO2             %8d         %8d      %8d \n", i_SO2F , i_SO2S,  i_SO2F  + i_SO2S);
  _rep (cr); 

  _rep (" --------------------------------------------------------------\n");


/* if we need to Save Totals - Fuel Totals Saver already check Name and all for error */
//  if ( this->_chRepTot->Checked ) {
    _GetTextBox (this->_txRepTot,cr_Name);
    if ( strcmp (cr_Name,"")) {
      STS.f_TotPM10 = i_PM10F + i_PM10S;      /* Save for doing Summaries     */
      STS.f_TotPM25 = i_PM25F + i_PM25S;
      STS.f_TotCH4  = i_CH4F  + i_CH4S;
      STS.f_TotCO2  = i_CO2F  + i_CO2S;
      STS.f_TotCO   = i_COF   + i_COS;
      STS.f_TotNOX  = i_NOXF  + i_NOXS;
      STS.f_TotSO2  = i_SO2F  + i_SO2S;
      STS_Add(&STS, cr_Name); 
   }

  i_FlaDur = (int)a_CO->f_FlaDur;
  i_SmoDur = (int)a_CO->f_SmoDur;
  HrMinSec (i_FlaDur, cr_FlaDuf);
  HrMinSec (i_SmoDur, cr_SmoDuf);

  f_Fla = a_CO->f_FlaCon;               /* Tot Flam & Smold Consmd from brnup*/
  f_Smo = a_CO->f_SmoCon;               /* these tots include all fuel loads */
  f_Tot = f_Fla + f_Smo;                /* duff,shr,dw,etc.. everything      */

  _rep ("\n\n");
  _rep ("                                  Consumption     Duration\n");
  sprintf (cr, "                                     (%s)    hour:min:sec\n", e_TonAc);
  _rep (cr); 
  sprintf (cr,"                         Flaming:     %8.2f       %s\n",f_Fla, cr_FlaDuf);
  _rep (cr);
  sprintf (cr,"                         Smoldering:  %8.2f       %s\n",f_Smo, cr_SmoDuf);
  _rep (cr);
  sprintf (cr,"                         Total:       %8.2f\n",f_Tot);
  _rep (cr); 

  if ( f_Tot != 0 )
     f_AvgComEff = ((f_Fla * e_ComEffFla) + (f_Smo * e_ComEffSmo)) / f_Tot;
  else
     f_AvgComEff = 0;

  sprintf (cr, " Unit Average Combustion Efficiency:  %8.2f\n",f_AvgComEff);
  _rep (cr); 

  goto Done;


/********************************************************************************/  
/*                           Soil  Soil Soil                                    */

Soil: 

int i,j,i_Deg, i_Time, i_Max1, i_Max2;
float  r,r_Max;
// char   cr_WndTxt[eC_WndTxt], cr_Cid[eC_WndTxt];

 //  CCW_GetWndTxt (e_CCW_CovTyp,cr_WndTxt);
//   CCW_GetWndTxt (e_CCW_Cid, cr_Cid);        /* Get text from Wnd            */

/* NEED TO DEAL WITH THIS */
char cr_InFN[1000];
    strcpy( cr_InFN,"");


/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
/* Do Heading and check each layer                                           */
   _rep ("\n\n\n                                       Soil Heat Report\n\n");
   if ( !stricmp (cr_InFN,"")) {
     sprintf (cr," Cover Type.....: %s\n", CI.cr_CoverClass);
     _rep(cr);
     sprintf (cr," Duff Depth.....: Pre-Fire: %6.2f cm.,  Post-Fire: %6.2f cm.\n\n",a_SO->f_cDufPre,a_SO->f_cDufPost);  
     _rep(cr);   }
   else{
     sprintf (cr,"User Input File: %s\n",cr_InFN);
    _rep (cr); }

   _rep ("                               Soil Layer Maximum Temperature\n");
   _rep ("                         ( measurements are in centimeters and Celsius )\n\n");
   _rep ("Depth     0    1    2    3    4    5    6    7    8    9   10   11   12   13\n");



/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
/* Print out Max Temp for each Layer                                         */
   _rep ("Temp.  ");
   for ( i = 1; i <= eC_Lay; i++ ) {             /* Check each Layer          */
     r_Max = 0;
     for ( j = 1; j < eC_Tim; j++ ) {            /* For each Temp in Layer   */
        r = SHA_Get(i,j);                        /* Get Temp                 */
        if ( r == e_SHA_Init )                   /* Skip init values         */
          continue;
        if ( r > r_Max )                         /* Look for highest         */
           r_Max = r; }                          /*  and save it             */
      i_Deg = r_Max;
      sprintf (cr,"%4d ",i_Deg); 
      _rep (cr); }

//    printf ("\n",i_Deg);
      _rep ("\n"); 



/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
/* Print out Time that Max Temp occurs for each Layer                        */
   _rep ("Time  ");
   for ( i = 1; i <= eC_Lay; i++ ) {             /* For each Layer          */
     i_Time = 0;
     r_Max = 0;
     for ( j = 1; j < eC_Tim; j++ ) {            /* For each Temp in Layer   */
        r = SHA_Get(i,j);                        /* Get Temp                 */
        if ( r == e_SHA_Init )                   /* Skip init values         */
          continue;
        if ( r > r_Max ){                        /* Look for highest         */
           r_Max = r;                            /*  and save it             */
           i_Time = j;} }
      sprintf (cr,"%5d",i_Time); 
      _rep (cr); }
   _rep ("\n\n");



/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
/* Do Max Temp at Layer, put to report and save for needed totals later      */
   i_Max1 = -1;
   i_Max2 = -1;

   for ( i = 1; i <= eC_Lay; i++ ) {             /* For each Layer          */
     for ( j = 1; j < eC_Tim; j++ ) {            /* For each Temp in Layer   */
        r = SHA_Get(i,j);                        /* Get Temp                 */
        if ( r == e_SHA_Init )                   /* Skip init values         */
          continue;
        if ( r > (float)e_Max1 )
          i_Max1 = i;
        if ( r > (float) e_Max2 )
          i_Max2 = i; } } 


   if ( i_Max1 == -1 ){
      sprintf (cr, "Max Depth Having %d degrees: - None - \n", e_Max1);
     _rep (cr); }
   else {
     sprintf (cr, "Max Depth Having %d degrees: %d\n", e_Max1, i_Max1 - 1);
     _rep (cr); }

   if ( i_Max2 == -1 ) {
     sprintf (cr,"Max Depth Having %d degrees: - None - \n", e_Max2);
     _rep (cr); }
   else {
     sprintf (cr,"Max Depth Having %d degrees: %d\n", e_Max2, i_Max2 - 1 );
     _rep (cr); }
 


  if ( !stricmp (cr_InFN,"") ) {            /* if not run a user input file */
     if (!stricmp (a_SO->cr_Model,e_SM_Duff))/* Duf sim was run and post duf */
       if ( a_SO->f_Heatpc < 0.03 ) {      /* insulated most of heat       */
          sprintf (cr, "Due to Post Duff Depth a minimal amount of heat will be transferd to soil.\n");
          _rep (cr); }
 
//     if (!stricmp (a_SO->cr_Model,e_SM_ZDuff)){ /* Exp Heat was run         */
//       if ( strcmp(a_CO->cr_SHC,"") ) {       /* if Have Min Soil Heat Messag */
//         sprintf (cr, "%s\n",a_CO->cr_SHC); 
//         _rep (cr); } }                        /* put it to report             */
     

     if ( !stricmp ( s_FLI.cr_FuelCat,e_FCa_Pile )) {
       _rep (" >>-------> These predictions apply to area(s) underneath the Piles,\n");
       _rep ("            we assume that no soil heating takes place elsewhere.\n"); 
     }

     if ( !stricmp(a_SI->cr_BrnIg,"NO"))     /* Burnup ran & didn't ignite   */
       _rep (e_NoIg);

 
/* if we need to Save Totals - Fuel Totals Saver already check Name and all for error */
//  if ( this->_chRepTot->Checked ) {
    _GetTextBox (this->_txRepTot,cr_Name);
    if ( strcmp (cr_Name,"" ) ) {
 //   if ( CCW_isTotal () ) {                   /* if Totals Radio But is Chkd  */
 //      CCW_TotalNam (cr);                      /* get the Total Name           */
       if ( i_Max1 != -1 )                     /* if not still at init values  */
         i_Max1 = i_Max1 -1;                   /* adjust to proper layer #     */
       if ( i_Max2 != -1 )                     /* ie. 1->0, 2->1, etc          */
         i_Max2 = i_Max2 -1;                   /* See Note-1 above             */
       STT_Save (cr_Name, i_Max1 , i_Max2); }
   }


Done:
 //  this->_txReport->Pri
 

 // richTextBox1.SelectedRtf = @"{\rtf1 \par \page}";


   _rep ("\f"); 
 
 return 1;

}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SRM_Report
* Desc: Do the Soil Report
* Note-1: There some funny stuff going on with the Soil Layers, who
*          who calls the first one '0', who calls it '1', I think
*          the Soil code (taken from Pascal) used '1', that's why
*          the for loop below starts at '1', but we call the layers
*          on the report starting with '0', so we have to adjust
*          them before storing them.
*   In: a_SI....Soil Inputs
*       a_SO....Soil Output Struct, has outputs from soil heating simulation
*       a_CO....Consumed Outuput Struct, from simulation
*       cr_InFN....User Input file name,
*                - "" send in null for normal running when doing from
*                  Wnd values
*                - when running soil from a user supplied input file
*                  send in that file name, it will get put on the report
*  Ret: 1 OK,
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int SRM_Report (d_SI *a_SI, d_SO *a_SO, d_CO *a_CO, char cr_InFN[])
{

#ifdef wowowow

int i,j,i_Deg, i_Time, i_Max1, i_Max2;
REAL  r,r_Max;
char  cr[300], cr_WndTxt[eC_WndTxt], cr_Cid[eC_WndTxt];

   CCW_GetWndTxt (e_CCW_CovTyp,cr_WndTxt);
   CCW_GetWndTxt (e_CCW_Cid, cr_Cid);        /* Get text from Wnd            */

/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
/* Do Heading and check each layer                                           */
   printf ("\n\n\n                                       Soil Heat Report\n\n");
   if ( !stricmp (cr_InFN,"")) {
     printf (" Cover Type.....: %s - %s\n",cr_Cid, cr_WndTxt);
     printf (" Duff Depth.....: Pre-Fire: %6.2f cm.,  Post-Fire: %6.2f cm.\n\n",a_SO->f_cDufPre,a_SO->f_cDufPost);  }
   else
     printf ("User Input File: %s\n",cr_InFN);

   printf ("                               Soil Layer Maximum Temperature\n");
   printf ("                         ( measurements are in centimeters and Celsius )\n\n");
   printf ("Depth     0    1    2    3    4    5    6    7    8    9   10   11   12   13\n");

/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
/* Print out Max Temp for each Layer                                         */
   printf ("Temp.  ");
   for ( i = 1; i <= eC_Lay; i++ ) {             /* Check each Layer          */
     r_Max = 0;
     for ( j = 1; j < eC_Tim; j++ ) {            /* For each Temp in Layer   */
        r = SHA_Get(i,j);                        /* Get Temp                 */
        if ( r == e_SHA_Init )                   /* Skip init values         */
          continue;
        if ( r > r_Max )                         /* Look for highest         */
           r_Max = r; }                          /*  and save it             */
      i_Deg = r_Max;
      printf ("%4d ",i_Deg); }
    printf ("\n",i_Deg);

/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
/* Print out Time that Max Temp occurs for each Layer                        */
   printf ("Time  ");
   for ( i = 1; i <= eC_Lay; i++ ) {             /* For each Layer          */
     i_Time = 0;
     r_Max = 0;
     for ( j = 1; j < eC_Tim; j++ ) {            /* For each Temp in Layer   */
        r = SHA_Get(i,j);                        /* Get Temp                 */
        if ( r == e_SHA_Init )                   /* Skip init values         */
          continue;
        if ( r > r_Max ){                        /* Look for highest         */
           r_Max = r;                            /*  and save it             */
           i_Time = j;} }
      printf ("%5d",i_Time);  }
   printf ("\n\n");

/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
/* Do Max Temp at Layer, put to report and save for needed totals later      */
   i_Max1 = -1;
   i_Max2 = -1;

   for ( i = 1; i <= eC_Lay; i++ ) {             /* For each Layer          */
     for ( j = 1; j < eC_Tim; j++ ) {            /* For each Temp in Layer   */
        r = SHA_Get(i,j);                        /* Get Temp                 */
        if ( r == e_SHA_Init )                   /* Skip init values         */
          continue;
        if ( r > e_Max1 )
          i_Max1 = i;
        if ( r > e_Max2 )
          i_Max2 = i; } }


   if ( i_Max1 == -1 )
     printf ("Max Depth Having %d degrees: - None - \n", e_Max1);
   else
     printf ("Max Depth Having %d degrees: %d\n", e_Max1, i_Max1 - 1);

   if ( i_Max2 == -1 )
     printf ("Max Depth Having %d degrees: - None - \n", e_Max2);
   else
     printf ("Max Depth Having %d degrees: %d\n", e_Max2, i_Max2 - 1 );

   if ( !stricmp (cr_InFN,"") ) {            /* if not run a user input file */
     if (!stricmp (a_SO->cr_Model,e_SM_Duff))/* Duf sim was run and post duf */
       if ( a_SO->f_Heatpc < 0.03 )       /* insulated most of heat       */
          printf ("Due to Post Duff Depth a minimal amount of heat will be transferd to soil.\n");

     if (!stricmp (a_SO->cr_Model,e_SM_ZDuff)){ /* Exp Heat was run         */
       if ( strcmp(a_CO->cr_SHC,"") )        /* if Have Min Soil Heat Messag */
         printf ("%s\n",a_CO->cr_SHC);}      /* put it to report             */

     if ( CCW_isPiles () ) {
       printf (" >>-------> These predictions apply to area(s) underneath the Piles,\n");
       printf ("            we assume that no soil heating takes place elsewhere.\n"); }

     if ( !stricmp(a_SI->cr_BrnIg,"NO"))     /* Burnup ran & didn't ignite   */
       printf (e_NoIg);

     if ( CCW_isTotal () ) {                   /* if Totals Radio But is Chkd  */
       CCW_TotalNam (cr);                      /* get the Total Name           */
       if ( i_Max1 != -1 )                     /* if not still at init values  */
         i_Max1 = i_Max1 -1;                   /* adjust to proper layer #     */
       if ( i_Max2 != -1 )                     /* ie. 1->0, 2->1, etc          */
         i_Max2 = i_Max2 -1;                   /* See Note-1 above             */
       STT_Save (cr, i_Max1 , i_Max2); }

   }

#endif 
   return 1;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Rep_Ind
* Desc: Put the Consmed and Post loads and percent consumed to report
*       This will calc the consumed amount and percent
*       This is for 3-6, 6-9, 9-20, 20+ sound and rotten woods
*   In: cr_Cat...the category
*       f_Pre, f_Pos......pre and post load for the category
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void Wnd_Mai::Rep_Ind ( char cr_Cat[], float f_Pre, float f_Pos)
{
float f_Con, f_Percent;
char cr[1000];

   f_Con = f_Pre - f_Pos;
   if ( f_Pre != 0 )
      f_Percent = (f_Con / f_Pre) * 100;
   else
      f_Percent = 0;

   sprintf (cr, "       %s           %7.2f   %7.2f   %7.2f   %6.1f     \n",
             cr_Cat, f_Pre, f_Con, f_Pos, f_Percent);
    _rep (cr); 
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Report_SubTit
* Desc: Put the 'REGION', 'COVER_TYPE', etc.. (see code) to the Output Report
*   In: d_FLI....direct inputs from GUI
*       cr_Poco... "***" use this when doing a Soil Report and Pocosin is 
*                  the cover type, - if it's a Pocosin these astericks 
*                  get be place after the Cover Type and signal user there is
*                  a Pocosin foot note at the end of the report.  
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int Wnd_Mai::Report_SubTit (d_FLI *a_FLI, char cr_Poco[])  
{
char cr_FueRef[e_FueRef+1], cr_AddRef[e_FueRef+1];
char cr[1000];

   sprintf (cr," Region:         %s\n", a_FLI->cr_Region);
   _rep(cr);
 
   sprintf (cr," Cover Type:     %s   %s\n",a_FLI->cr_CoverType, cr_Poco);
   _rep(cr);

   sprintf (cr," Fuel Type:      %s \n", a_FLI->cr_FuelCat);
   _rep(cr);
 
   SAF_GetRefer (a_FLI->cr_CoverType,  cr_FueRef,  cr_AddRef);
   if ( strcmp (cr_FueRef,"") ) {
     sprintf (cr," Fuel Reference: %s \n",cr_FueRef);
     _rep(cr); }
   if ( strcmp (cr_AddRef,"") ){
     sprintf (cr," Additional Reference: %s \n",cr_AddRef);
     _rep(cr); }

   return 1; 
}


/***********************************************************************
*
***********************************************************************/
void Wnd_Mai::_rep(char cr[])
{
String ^ s;
   if ( !stricmp (cr,"NewLine")) {
     this->_txReport->AppendText (System::Environment::NewLine); 
     return; }

   s =  _CharToStr (cr);
   this->_txReport->AppendText(s); 
}

/**********************************************************************
* Name: AdjChar
* Desc: Get the fuel adjustment character '-' '+' 'u' that are used
*        report
*   In: the #define name of the Fuel TextBox
***********************************************************************/
char Wnd_Mai::AdjChar(String ^ s_TBN)
{
char  c;
TextBox ^ tb; 

  c = e_Adj_Na;   /* blank */

  if ( s_TBN == e_txLitter ) tb = this->_txLitter;
  if ( s_TBN == e_tx1Hr    ) tb = this->_tx1Hr   ;
  if ( s_TBN == e_tx10Hr   ) tb = this->_tx10Hr  ;
  if ( s_TBN == e_tx100Hr  ) tb = this->_tx100Hr ;
  if ( s_TBN == e_tx1kHr   ) tb = this->_tx1kHr  ;

  if ( s_TBN == e_txDuff   ) tb = this->_txDuff  ;
  if ( s_TBN == e_txHerb   ) tb = this->_txHerb  ;
  if ( s_TBN == e_txShrub  ) tb = this->_txShrub ;
  if ( s_TBN == e_txFoliage) tb = this->_txFoliage;
  if ( s_TBN == e_txBranch ) tb = this->_txBranch;

  if ( this->isFuelLight (tb ) || this->isFuelSparse (tb )  )
     c = e_Adj_LS;

  if ( this->isFuelHeavy (tb ) || this->isFuelAbundent (tb )  )
     c = e_Adj_HA;

  if ( this->isFuelUser (tb )  )
     c = e_Adj_Usr;

  return c; 
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Rep_SavAdj
* Desc: This keeps track of when we get a User/Sparse/Abun/Light/Heavy
*       adjustment value used on the fuel report.
*       If User/Sparse/Abun/Light/Heavy char gets sent in we save it
*        in a certain spot in array, let we can check it
*   In: c........see defines
*  Out: cr_adj...set as needed
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Rep_SavAdj (char c, char cr_adj[])
{
  if ( c == e_Adj_Usr ) cr_adj[0] = e_Adj_Usr;    /* User                    */
  if ( c == e_Adj_HA )  cr_adj[1] = e_Adj_HA;     /* Heavy - abundant        */
  if ( c == e_Adj_LS )  cr_adj[2] = e_Adj_LS;     /* Light - sparse          */
}


/********************************************************
* Name: Set_FuelTotal
* Desc: Check and Save the Fuel Totals
*       First we make sure that the GUI settings are set
*       and correct.
* NOTE: Fuel Totals are save for all reports - Fuel
*        Smoke and Soil. 
********************************************************/
int Wnd_Mai::Set_FuelTotal(d_CO *a_CO, char cr_ErrMes[])
{
char cr_Name[100]; 
d_ST ST; 

 strcpy (cr_ErrMes,""); 

 _GetTextBox (this->_txRepTot,cr_Name);

 if ( !strcmp (cr_Name,"") ) 
   return 1; 

/* Check if we need to even save  totals */

#ifdef wowo
if ( this->_chRepTot->Checked == false && !strcmp (cr_Name,"") )
  return 1;  
 
 if ( this->_chRepTot->Checked == true && !strcmp (cr_Name,"") ) { 
  strcpy (cr_ErrMes, "Uncheck the Report Total checkbox or enter a Summary Name");
  return 0; } 

 if ( this->_chRepTot->Checked == false && strcmp (cr_Name,"") ) { 
  strcpy (cr_ErrMes, "Please heck the Report Total checkbox or remove the Summary Name text");
  return 0; } 
#endif

 

  if ( ST_isName(cr_Name) ) {
   strcpy (cr_ErrMes, "Please change or clear the Total name has already been used"); 
   return 0; }

 Rep_SetST (&ST,a_CO);
 ST_Add(&ST,cr_Name); 

 return 1; 
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Rep_FuelTotals
* Desc: Put the Fuel Report Totals to the Report Window
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int Wnd_Mai::Rep_FuelTotals ()
{
int i,j;
char cr[1000];
d_ST s_ST;
  j = eC_TotNam - 5;            

  _rep ("\n\n\n Report Totals:\n");
  sprintf  (cr, "                               Fuel Consumption Summary - (%s)\n\n",e_TonAc);
  _rep (cr); 
 
  sprintf (cr, "%*c        ______   Wood    Wood    Wood    Wood    ____    ____   _____   Crown   Crown\n",j,' ');
  _rep (cr); 
  sprintf (cr, "%*c Id     Litter  0->1/4  1/4->1   1->3      3+    Duff    Herb   Shrub   Folge   Brnch\n",j,' ');
  _rep (cr); 

  for ( i = 0; i < 10000;  i++ ) {
     if ( !ST_Get (&s_ST,i) )
       break;
     sprintf (cr, "%*s %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f\n",
      eC_TotNam,
      s_ST.cr_Name,
      s_ST.f_Litter, s_ST.f_DW0, s_ST.f_DW10, s_ST.f_DW100,
      s_ST.f_DW1000, s_ST.f_DufAmtCon, s_ST.f_Herb,
      s_ST.f_Shrub,  s_ST.f_CroFol, s_ST.f_CroBra); 
     _rep(cr); }

    return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Smoke_Totals
* Desc: Put smoke totals to report
* Note-1: This will help position the Caption over the Summary Id-Name
*         The 'eC_TotNam' define is the size of of the Id-Name and is
*         also used to in the printf formating
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int Wnd_Mai::Rep_SmokeTotals ()
{
int   i,j;
char cr[3000];
d_STS s_STS;

  if ( STS_Count() == 0 )    /* No Emis Totals */
   return 1; 

  j = eC_TotNam - 5;                          /* see Note-1                  */
  _rep ("\n\n\n Report Totals:\n");
//  _rep ("\n\n\n");
   sprintf  (cr, "                                Smoke Emission Summary (%s) \n\n", e_lbAc);
  _rep (cr); 

  sprintf (cr, "%*c Id       PM10        PM2.5       CH4         CO          CO2       NOX       SO2\n",j,' ');
  _rep (cr);

  for ( i = 0; i < 10000;  i++ ) {
     if ( !STS_Get (&s_STS,i) )
        break;
     sprintf (cr,"%*s %9.1f   %9.1f   %9.1f   %9.1f   %9.1f %8.1f %9.1f\n",
       eC_TotNam,
       s_STS.cr_Name,
       s_STS.f_TotPM10,
       s_STS.f_TotPM25,
       s_STS.f_TotCH4,
       s_STS.f_TotCO,
       s_STS.f_TotCO2,

       s_STS.f_TotNOX,
       s_STS.f_TotSO2);
      _rep (cr); 
     }
   return 1; 
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SRM_Totals was  Soi_Totals
* Desc: Put the Soil Totals to the Report
* NOTE: these aren't really totals put the deepest layers that
*        reached some specific degrees, way are saying "what is the deepest
*        layer that reached 60 and 270 degrees"  see defines for actual
*        tempuratures used.
* Note-1: No Layer reached the temperature
* Note-2: This will help position the Caption over the Summary Id-Name
*         The 'eC_TotNam' define is the size of of the Id-Name and is
*         also used to in the printf formating
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int Wnd_Mai::Rep_SoilTotals() 
{
int i, j, k, i_Lay1, i_Lay2;
char cr[1000], cr_Name[100];

  if ( STT_Count () == 0 ) 
    return 1; 


  j = eC_TotNam - 5;                          /* see Note-2                  */
  k = j / 2;
  _rep ("\n\n\n");
  _rep (" Report Totals:\n");
  sprintf (cr, "%*c            Soil Summary\n",k,' ');
  _rep (cr);
  sprintf (cr, "%*c        Depth (cm)   Depth (cm) \n",j,' ');
  _rep (cr);
  sprintf (cr, "%*c Id      >%dC         >%dC\n",j,' ', e_Max1, e_Max2);
  _rep (cr);

  _rep (" -----------------------------------------\n"); 
 

  for ( i = 0; i < 10000; i++ ) {            /* Get each one that was saved  */
   if ( !STT_Get(i,cr_Name,&i_Lay1,&i_Lay2))      /* Get name & layers            */
     break;
   sprintf (cr, "%*s", eC_TotNam, cr_Name);            /* print name to Report         */
   _rep (cr); 
   
   if ( i_Lay1 == -1 )                       /* print layers to report       */
      _rep ("   None ");
   else {
      sprintf (cr,"     %2d ", i_Lay1);
      _rep (cr); }
   if ( i_Lay2 == -1 )
      _rep ("         None    \n");
   else {
      sprintf (cr,"           %2d \n", i_Lay2); 
      _rep (cr); }
 
   } /* for i */
 
 return 1; 
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Rep_SetST
* Desc: Put all the calculated Conumsumed amounts into the 'Save Total' struct
*   In: a_CO.....loaded with calcs
*  Out  a_ST......will get loaded here
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void   Rep_SetST (d_ST  *a_ST,  d_CO  *a_CO)
{
  a_ST->f_Litter       = a_CO->f_LitCon;
  a_ST->f_DW0          = a_CO->f_DW1Con;
  a_ST->f_DW10         = a_CO->f_DW10Con;
  a_ST->f_DW100        = a_CO->f_DW100Con;
  a_ST->f_DW1000       = a_CO->f_Snd_DW1kCon + a_CO->f_Rot_DW1kCon;
  a_ST->f_DufAmtCon    = a_CO->f_DufCon;
  a_ST->f_DufDepRed    = a_CO->f_DufDepCon;
  a_ST->f_Herb         = a_CO->f_HerCon;
  a_ST->f_Shrub        = a_CO->f_ShrCon;
  a_ST->f_CroBra       = a_CO->f_FolCon;
  a_ST->f_CroFol       = a_CO->f_BraCon;
}

/**********************************************************************
*
*
*********************************************************************/
void Wnd_Mai::Rep_TotalsClear()
{
  ST_Init();
  STT_Init(); 
  STS_Init(); 
}

/**************************************************************
* Name: Report_Save
* Desc: Save the text that's in the Report RichTextBox
* NOTE: ---> this RichTextBox function saves it as a 
*            wordprocessor file, I didn't try to figure out
*            how to save it as a text file and I don't know if 
*            I'll need to
**************************************************************/
int Wnd_Mai::Report_Save()
{
int i;
String ^ Str; 

System::Windows::Forms::SaveFileDialog FD;

#define e_RepFilter  "doc files (*.doc)|*.doc|All files (*.*)|*.*"
#define e_RepFN  "FOF-Rep.doc"

/* Pop Up a SaveFileDialog to get a file name */
/* Use the Path & File Name from the Project File TextBox */
  FD.Title = "Save FOFEM Report To File"; 
  FD.InitialDirectory = this->Prj_GetPath();  /* Path of current in use Project file */
  FD.FileName = e_RepFN;  
  FD.Filter = e_RepFilter;
  i = (int) FD.ShowDialog(); 
  if ( i != 1 )
    return 0; 

  Str = FD.FileName;  /* Get Path + File Name from DialogBox */ 
//  _StrToChar (Str,cr_FN); 
 
  this->_txReport->SaveFile(Str);  /* See NOTE above */

  return 1; 
}

/*****************************************************************************
* Name: Emission_CSVMngr
* Desc: Create the Emission .csv file  
*  In: cr_FN.....output path file name
*      cr_Mode..."Full"  to do the full list of emission components
*                "Short" to do just the major emission components, CO2, CO etc. 
*****************************************************************************/
int Wnd_Mai::Emission_CSVMngr (char cr_FN[], char cr_Mode[], char cr_ErrMes[]) 
{
int i,j,k,ix; 
float f,c,d,g, f_Emi, f_Percent, f_Factor;
float f_Flame,f_Coarse,f_Duff;
float f_FacFla,f_FacCoa, f_FacDuf;
float f_SmoCon, f_DuffCon; 
char cr_GrpNum[100], cr_CovTyp[300], cr_Name[500], cr_Code[100];
char cr_EmiFla[20], cr_EmiCoa[20], cr_EmiDuf[20]; 
char cr_Unit [3000], cr_EU[100], cr[5000], cr_Fmt[100]; 

#define eC_Emi 300
int   iN_Emi, iX_Emi;
float fr_Emi[eC_Emi];

#define eC_Fue 30
int   iN_Fue, iX_Fue;
float fr_Fue[eC_Fue];

d_CO CO,  *a_CO;
d_CI CI,  *a_CI;
d_SI SI,  *a_SI;
d_SO SO,  *a_SO;
d_FLI  s_FLI; 
d_ST ST;
d_STT STT; 
d_STS STS; 
FILE  *fh;

/* See Note-2 above */
#define e_Crit 7
// #define e_NH3  4

  for ( i = 0; i < eC_Emi; i++ ) 
    fr_Emi[i] = 0;

  for ( i = 0; i < eC_Fue; i++ )
    fr_Fue[i] = 0;

  this->set_frm->EU_Unit(cr_EU);  /* Emission Unit from Settings Window */
  
  strcpy (cr_ErrMes,""); 
  strcpy (cr_Unit,""); 
  fh = fopen (cr_FN, "w" ) ;
  if ( fh == NULL ) {   
	sprintf (cr_ErrMes, "Can't open file: %s", cr_FN); 
    return 0; }

  a_CO = &CO;  /* just doing this to not re-write old code that uses a_CO-> */
  a_CI = &CI; 
  a_SI = &SI;
  a_SO = &SO; 

  CI_Init (&CI);
  CO_Init (&CO); 
  SI_Init (&SI);
  SO_Init (&SO);  

/* Get Inputs and Run FOFEM */
  if ( !Get_Inputs(&s_FLI)) {             /* Get inputs from GUI */
     myMessageBox(s_FLI.cr_ErrMes);
     return 0; }
  i = CalcSub(&s_FLI, &CI, &CO, &SI, &SO, "", cr_ErrMes);
  if ( i == 0 )                         
    return 0;                          
  
/* Get the 3 Emission Group Numbers from GUI dropdowns */
  _GetEmiGrp (s_FLI.cr_EmisFlame, cr_EmiFla); 
  _GetEmiGrp (s_FLI.cr_EmisSmold, cr_EmiCoa);
  _GetEmiGrp (s_FLI.cr_EmisDuff,  cr_EmiDuf); 

/* Put 1st line to file - Headings......................................... */
  fprintf (fh, "Time, Fire-Intensity, Flame-Con, Coarse-Con, Duff-Con, Litter-Fuel, Litter-Con, 1Hr-Fuel, 1Hr-Con, 10Hr-Fuel, 10Hr-Con, 100Hr-Fuel, 100Hr-Con, 1kHr-Fuel, 1kHr-ConFla, 1kHr-ConSmo");

/* Put the Emission Component Codes to the top of File and build the string of unit codes column */
/*  headings that will go on 2cd line of file */ 
   for ( j = 0; j < 100000; j++ ) {
     if ( !strcmp (cr_Mode,"Short") && j == e_Crit )      /* Only doing Major Emissions */
	    break; 

     k = NES_Get_Factor(cr_EmiDuf, j, cr_CovTyp, &f_FacDuf, cr_Name, cr_Code);
	 if ( k == 0 ) 
		break; 

     StrRepChr (cr_Code, ' ', '-'); 
     StrRepChr (cr_Code, ',', ';'); 
     fprintf (fh, " ,%s", cr_Code); 
   
	 strcat (cr_Unit,",");                 /* build unit string */
	 if ( !stricmp (cr_EU,"Ton"))
       strcat (cr_Unit, e_TonAc);
	 else if ( !stricmp (cr_EU,"Pound"))
       strcat (cr_Unit, e_lbAc);
	 else
       strcat (cr_Unit, e_GrSqM) ;
    }  /* for j */ 

   fprintf (fh, "\n"); 
   strcat (cr_Unit, "\n"); 

/* Build beginnning of the 2cd line of file, */ 
   sprintf (cr,"Seconds,%s", e_KwSqM);    /* time = seconds, Fire Intensty = Kilowatt sq mt */
   for ( i = 0; i < 14; i++ ) { 
      strcat (cr,",");
      strcat (cr,e_TonAc);  }    /* Consumed loads always as Tons per acre - T/ac */

   fprintf (fh,cr);              /* Put first section of line 2 - consumed units */ 
   fprintf (fh, cr_Unit);        /* Put second section of line 2 - emission unit codes */

/* Put the Emission values to the file...................... */
/*  One line per time step */
   for ( i = 0; i < eC_SGV; i++ ) {       /* For each Time Step */
     if ( CO.sr_SGV[i].f_Sec < 0 )      /* All done */ 
        break;

/* Smolder includes the Duff, so we want to remove it */ 
     f_SmoCon = CO.sr_SGV[i].f_SmoCon;
     f_DuffCon = CO.sr_SGV[i].f_DuffCon;
     f_SmoCon = f_SmoCon - f_DuffCon; 

     /* Time, Intensity, fuel amounts */
     fprintf (fh, "%4.0f, %4.3f", CO.sr_SGV[i].f_Sec,  CO.sr_SGV[i].f_Inten);  

/* NOTE: fuel amounts in SGV are in Kg/m2, so convert to T/ac for outputing */ 
     fprintf (fh, ",%4.3f", _CnvFuel(CO.sr_SGV[i].f_FlaCon, "Ton"));
     fprintf (fh, ",%4.3f", _CnvFuel(f_SmoCon,"Ton"));
     fprintf (fh, ",%4.3f", _CnvFuel(f_DuffCon,  "Ton"));

     fprintf (fh, ",%6.3f", _CnvFuel(CO.sr_SGV[i].f_rm_Lit,"Ton")); 
     fprintf (fh, ",%6.3f", _CnvFuel(CO.sr_SGV[i].f_cn_Lit,"Ton"));
     fprintf (fh, ",%6.3f", _CnvFuel(CO.sr_SGV[i].f_rm_1Hr,"Ton"));
     fprintf (fh, ",%6.3f", _CnvFuel(CO.sr_SGV[i].f_cn_1Hr,"Ton")); 
     fprintf (fh, ",%6.3f", _CnvFuel(CO.sr_SGV[i].f_rm_10Hr,"Ton"));
     fprintf (fh, ",%6.3f", _CnvFuel(CO.sr_SGV[i].f_cn_10Hr,"Ton")); 
     fprintf (fh, ",%6.3f", _CnvFuel(CO.sr_SGV[i].f_rm_100Hr,"Ton"));
     fprintf (fh, ",%6.3f", _CnvFuel(CO.sr_SGV[i].f_cn_100Hr,"Ton")); 
     fprintf (fh, ",%6.3f", _CnvFuel(CO.sr_SGV[i].f_rm_1kHr,"Ton")); 
//     fprintf (fh, ",%6.3f", _CnvFuel(CO.sr_SGV[i].f_cn_1kHr,"Ton")); 

    fprintf (fh, ",%6.3f", _CnvFuel(CO.sr_SGV[i].f_cn_1kHrFla,"Ton")); 
    fprintf (fh, ",%6.3f", _CnvFuel(CO.sr_SGV[i].f_cn_1kHrSmo,"Ton")); 



/*................................................. */
/* Total up consumed amt at each time step  */     
     fr_Fue[0] +=  CO.sr_SGV[i].f_FlaCon;
     fr_Fue[1] +=  f_SmoCon;
     fr_Fue[2] +=  f_DuffCon;

/* Consumed amount - add up amt for each time step */
     fr_Fue[4]  += CO.sr_SGV[i].f_cn_Lit;
     fr_Fue[6]  += CO.sr_SGV[i].f_cn_1Hr;
     fr_Fue[8]  += CO.sr_SGV[i].f_cn_10Hr;
     fr_Fue[10] += CO.sr_SGV[i].f_cn_100Hr;
 //    fr_Fue[12] += CO.sr_SGV[i].f_cn_1kHr;

     fr_Fue[12] += CO.sr_SGV[i].f_cn_1kHrFla;
     fr_Fue[13] += CO.sr_SGV[i].f_cn_1kHrSmo;

/*..................................................*/

/* NOTE: consumed fuels are in Kg/m2 and that is how we need them  */
/* when we apply the emission factors to them below */
     f_Flame  = CO.sr_SGV[i].f_FlaCon;    /* Fuel Amts Consumed by Flaming */
     f_Coarse = f_SmoCon;   /*   Coarse Wood smolderin       */
     f_Duff   = f_DuffCon;     /*   Duff Smoldering             */

     iX_Emi = 0;

     for ( j = 0; j < 100000; j++ ) {           /* Each Emission Component       */
       if ( !strcmp (cr_Mode, "Short") && j == e_Crit )  /* Just doing major ones */
         break;
  //     if ( !strcmp (cr_Mode, "Short") &&  j == e_NH3 )  /* See Note-2 above */
  //       continue;

       k = NES_Get_Factor(cr_EmiFla, j, cr_CovTyp, &f_FacFla, cr_Name, cr_Code);
       k = NES_Get_Factor(cr_EmiCoa, j, cr_CovTyp, &f_FacCoa, cr_Name, cr_Code);
       k = NES_Get_Factor(cr_EmiDuf, j, cr_CovTyp, &f_FacDuf, cr_Name, cr_Code);
       if ( k == 0 )
         break;

       f = f_Flame  * f_FacFla;         /* Emis Factors times Consume Fuel Amts */
       c = f_Coarse * f_FacCoa;
       d = f_Duff   * f_FacDuf;
       g = f + c + d;                   /* This is in gram per square meter */
       f_Emi = _CnvEmis (g,cr_EU);      /* Convert to requested unit */     
      _GetFmt (f_Emi, cr_Fmt,cr_EU);    /* Get a format statement */
       fprintf (fh,cr_Fmt,f_Emi);       /* Output Emission value */ 

       fr_Emi[iX_Emi++] += f_Emi;       /* Total up each emis component */
     } /* for j */

     fprintf (fh, "\n");

    } /* for i */


 /* Remaining Fuel Loads are the values in the last time step  */
     i--; 
     fr_Fue[3]  = CO.sr_SGV[i].f_rm_Lit;
     fr_Fue[5]  = CO.sr_SGV[i].f_rm_1Hr;
     fr_Fue[7]  = CO.sr_SGV[i].f_rm_10Hr;
     fr_Fue[9]  = CO.sr_SGV[i].f_rm_100Hr;
     fr_Fue[11] = CO.sr_SGV[i].f_rm_1kHr;

/* Output totals...................*/
   fprintf (fh, "Total,");

   for ( i = 0; i < 14; i++ ){           /* output fuel loads as T/ac */
     f = _CnvFuel (fr_Fue[i],"Ton");     
     fprintf (fh, ",%4.3f",f);  
   }

   for ( i = 0; i < iX_Emi; i++ ){       /* output emission */
     _GetFmt (fr_Emi[i], cr_Fmt,cr_EU);  /* Get a format statement */
     fprintf (fh, cr_Fmt, fr_Emi[i]);    /* Emission amts */
   }

   fprintf (fh, "\n");
   fclose (fh); 
   return 1; 
} 

/****************************************************************
* Name: _CnvFuel
* Desc: Convert Fuel amount from Kilograms Per Square Meter (kg/m2)
*       to one of the following ...
*       Tons per acre 
*       Pounds per acre
*
****************************************************************/
float _CnvFuel (float val, char cr[])
{
float f,g;
  
  if ( !stricmp (cr,"Ton") ) {
    f = KgSq_To_TPA (val);    /* convert to tons */
    return f; }

  if (  !stricmp (cr,"Pound") ) {
     f = KgSq_To_TPA (val);   /* convert to tons */
     g = f * 2000.0;          /* get to pounds */
     return g; }
  
   return val;                /* don't convert */
}

/******************************************************************
*
******************************************************************/
void _GetEmiGrp (char cr_In[], char cr_Out[])
{
	sscanf (cr_In, "%s",cr_Out);
}

/****************************************************************
* Name: Emission_CSVShort_DefFac
* Desc: Create output .csv file - for the short list of emission
*       components which in the case of the Default emission
*       factors are just the 7 major components, CO2, CO, PM10, etc
        This is the counter part to the Expaned Factor output 
*        .csv file 
****************************************************************/
int  Wnd_Mai::Emission_CSVShort_DefFac (char cr_FN[], char cr_ErrMes[])
{
int i,j,k;
float f, ff;  
float f_Fla, f_Smo, f_Duf;
#define eC_Tot 20
#define eC_Emi 13           /* Start of Emis Conpent totals */
float f_Emi, fr_Tot[eC_Tot]; 
char cr_EU[20],cr_Unit[300],cr[300],cr_Head[300]; 
char  cr_Fmt[50] ;
char cr_FUnit[10]; 
FILE *fh; 
d_CO CO,  *a_CO;
d_CI CI,  *a_CI;
d_SI SI,  *a_SI;
d_SO SO,  *a_SO;
d_FLI  s_FLI; 

  strcpy (cr_ErrMes,""); 
  strcpy (cr_Unit,""); 

  fh = fopen (cr_FN, "w" ) ;
  if ( fh == NULL ) {   
	sprintf (cr_ErrMes, "Can't open file: %s", cr_FN); 
    return 0; }

  a_CO = &CO;  /* just doing this to not re-write old code that uses a_CO-> */
  a_CI = &CI; 
  a_SI = &SI;
  a_SO = &SO; 

  CI_Init (&CI);
  CO_Init (&CO); 
  SI_Init (&SI);
  SO_Init (&SO);  

  for ( i = 0; i < eC_Tot; i++ )  /* Init for final totals */ 
     fr_Tot[i] = 0;  

  this->set_frm->EU_Unit(cr_EU);  /* Emission Unit from Settings Window */

/* Get Inputs and Run FOFEM */
  if ( !Get_Inputs(&s_FLI)) {             /* Get inputs from GUI */
     myMessageBox(s_FLI.cr_ErrMes);
     return 0; }
  i = CalcSub(&s_FLI, &CI, &CO, &SI, &SO, "", cr_ErrMes);
  if ( i == 0 )                         
    return 0;  

/* Put 1st line to file - Headings......................................... */
//  fprintf (fh, "Time, Fire-Intensity, Flame-Con, Coarse-Con, Duff-Con, Litter-Fuel, Litter-Con, 1Hr-Fuel, 1Hr-Con, 10Hr-Fuel, 10Hr-Con, 100Hr-Fuel, 100Hr-Con, 1kHr-Fuel, 1kHr-Con");
  fprintf (fh, "Time, Fire-Intensity, Flame-Con, Smolder-Con, Duff-Con, Litter-Fuel, Litter-Con, 1Hr-Fuel, 1Hr-Con, 10Hr-Fuel, 10Hr-Con, 100Hr-Fuel, 100Hr-Con, 1kHr-Fuel, 1kHr-Con");


/* Put the Emission Component Codes to the top of File */
   for ( i = 0; i < 100000; i++ ) {
     if ( !stricmp (sr_EF[i].cr_Code,"") ) 
       break; 
     fprintf (fh, " ,%s", sr_EF[i].cr_Code); } 
   fprintf (fh, "\n"); 
 
/* Put 2cd line of column headings to file - Unit Abreviations */ 
   if ( !stricmp (cr_EU,"Ton")){
     strcpy (cr_Unit, e_TonAc);         /* emis unit */
     strcpy (cr_FUnit, e_TonAc);}       /* fuel load unit */ 
   else if ( !stricmp (cr_EU,"Pound")){
     strcpy (cr_Unit, e_lbAc); 
     strcpy (cr_FUnit, e_TonAc);}
   else {
     strcpy (cr_Unit, e_GrSqM );	
     strcpy (cr_FUnit, e_TonAc);}

/*  Build a string of units ex: "T/ac, T/ac, T/ac ..." */ 
   strcpy (cr_Head,"");        
   for ( i = 0; i < eC_Tot; i++ ){ 
     strcat (cr_Head, ",");
     if ( i < 13 )  
       strcat (cr_Head, cr_FUnit);   /* Fuel Component unit */ 
     else 
       strcat (cr_Head, cr_Unit); }  /* Emission Unit */ 

  fprintf (fh,"Seconds,%s %s\n", e_KwSqM, cr_Head);    /* time = seconds, Fire Intensty = Kilowatt sq mt */

/*------------------------------------------------------------*/
/* Put out fuel loads & emission amounts for each time step */
   for ( i = 0; i < eC_SGV; i++ ) {       /* For each Time Step */
     if ( CO.sr_SGV[i].f_Sec < 0 )      /* All done */ 
        break;
     /* Time, Intensity, fuel amounts */
     fprintf (fh, "%4.0f, %4.3f", CO.sr_SGV[i].f_Sec,  CO.sr_SGV[i].f_Inten);  

     SGV_FlaSmoDuf (&CO.sr_SGV[i], &f_Fla, &f_Smo, &f_Duf);

    /* Fuel Consumed - Flame, Smolder, Duff */
    _PutFuel (fh,f_Fla,cr_EU);
    _PutFuel (fh,f_Smo,cr_EU);
    _PutFuel (fh,f_Duf,cr_EU);
     fr_Tot[0] += f_Fla; 
     fr_Tot[1] += f_Smo;
     fr_Tot[2] += f_Duf;

     /* Consumed & Remaining Fuel Loads */
    _PutFuel (fh, CO.sr_SGV[i].f_rm_Lit,  cr_EU);
    _PutFuel (fh, CO.sr_SGV[i].f_cn_Lit,  cr_EU);
    _PutFuel (fh, CO.sr_SGV[i].f_rm_1Hr,  cr_EU);
    _PutFuel (fh, CO.sr_SGV[i].f_cn_1Hr,  cr_EU);
    _PutFuel (fh, CO.sr_SGV[i].f_rm_10Hr, cr_EU); 
    _PutFuel (fh, CO.sr_SGV[i].f_cn_10Hr, cr_EU);
    _PutFuel (fh, CO.sr_SGV[i].f_rm_100Hr,cr_EU);
    _PutFuel (fh, CO.sr_SGV[i].f_cn_100Hr,cr_EU);
    _PutFuel (fh, CO.sr_SGV[i].f_rm_1kHr, cr_EU);
    _PutFuel (fh, CO.sr_SGV[i].f_cn_1kHr, cr_EU);
 
    /* Final Totals */ 
     fr_Tot[4]  += CO.sr_SGV[i].f_cn_Lit;  
     fr_Tot[6]  += CO.sr_SGV[i].f_cn_1Hr;
     fr_Tot[8]  += CO.sr_SGV[i].f_cn_10Hr;
     fr_Tot[10] += CO.sr_SGV[i].f_cn_100Hr;
     fr_Tot[12] += CO.sr_SGV[i].f_cn_1kHr;

     /* Each Emission Compoents */ 
     for ( j = 0; j < 100; j++ ) {
       if ( !strcmp (sr_EF[j].cr_Code,"" ) ) 
          break; 

       f = _GetDefEmi (&CO.sr_SGV[i], sr_EF[j].cr_Code);  
       f_Emi = _CnvEmis (f,cr_EU);           
      _GetFmt (f_Emi, cr_Fmt,cr_EU);
       fprintf (fh,cr_Fmt,f_Emi);          /* output emission */
       fr_Tot[eC_Emi + j] += f; 
     } /* for j */
   
     fprintf (fh, "\n"); 

  } /* for i */ 

/* The remaining (un-consumed) fuel load amounts will go on */
/*  last line of file along with the consumed and emis totals */
  i--; 
  fr_Tot[3]  = CO.sr_SGV[i].f_rm_Lit;    /* remaining/unconsumed fuel loads */
  fr_Tot[5]  = CO.sr_SGV[i].f_rm_1Hr;    /* are in the last time step */ 
  fr_Tot[7]  = CO.sr_SGV[i].f_rm_10Hr;   
  fr_Tot[9]  = CO.sr_SGV[i].f_rm_100Hr; 
  fr_Tot[11] = CO.sr_SGV[i].f_rm_1kHr; 

/* Put out last line of file - totals and remain fuel loads */ 
  fprintf (fh, "Total,"); 
  for ( i = 0; i < eC_Tot; i++ ) {
    if ( i >= 13 ) {
      f = _CnvEmis (fr_Tot[i],cr_EU);   /* Convert emis */
     _GetFmt (f, cr_Fmt,cr_EU); }       /* Get format statement */
    else {
      f = _CnvFuel (fr_Tot[i],"Ton");   /* Fuel Load */ 
     _GetFmt (f, cr_Fmt,"Ton"); }       /* Always output as T/ac */
    
   fprintf (fh,  cr_Fmt,f); 
  } 

  fprintf (fh, "\n"); 
  fclose (fh); 

  return 1; 
}

/********************************************************
* Put a single fuel conponent value out to the file
********************************************************/
void _PutFuel (FILE *fh, float f_Fuel, char cr_EU[])
{
float f; 
char cr_Fmt[100]; 
  f = _CnvFuel (f_Fuel, "Ton");  /* Convert to requested unit */
 _GetFmt (f, cr_Fmt, "Ton");  /* Get format with desired decimal precision */
  fprintf (fh,cr_Fmt,f); 
}

/***********************************************************
* Get an fprintf format statment that will give us enough
*  decimal precision for the value, for example we
*  want more precision for Tons than pounds.   
***********************************************************/
void  _GetFmt (float f, char cr[], char cr_Unit[])
{
int i,j;
char A[20]; 

 /* asssume g/m2 grams square meter */ 
   if ( f < 1.0 ) 
     strcpy (cr,",%4.3f"); 
   else if ( f < 10.0 )
     strcpy (cr, ",%4.2f");
   else 
     strcpy (cr, ",%4.1f"); 

/* See if it's pounds or tons */ 
   if ( !stricmp (cr_Unit,"Pound") ||  !stricmp (cr_Unit,"Pounds") ) {
     if ( f < 1.0 ) 
       strcpy (cr,",%4.2f"); 
     else if ( f < 10.0 )
       strcpy (cr, ",%4.1f");
     else 
       strcpy (cr, ",%4.0f");
     return;  
   }  /* if */ 
  
   else if ( !stricmp (cr_Unit,"Tons" ) || !stricmp (cr_Unit,"Ton" )    ) {
     if ( f < 1.0 ) 
       strcpy (cr,",%4.3f"); 
     else if ( f < 10.0 )
       strcpy (cr, ",%4.2f");
     else 
       strcpy (cr, ",%4.2f"); 
     return; 
   }

   return ; 

}

/**************************************************************
*
*
**************************************************************/
float _GetDefEmi (d_SGV *a, char cr_Code[])
{
float f,g;
    f = 0; 
    if ( !stricmp (cr_Code, e_PM10 ) ) 
      f = a->f_PM10;
    if ( !stricmp (cr_Code, e_PM25 ) )
      f = a->f_PM2_5; 
    if ( !stricmp (cr_Code, e_CH4 ) ) 
      f = a->f_CH4; 
    if ( !stricmp (cr_Code, e_CO  ) )
      f = a->f_CO;  
    if ( !stricmp (cr_Code, e_CO2 ) )
      f = a->f_CO2;  
    if ( !stricmp (cr_Code, e_NOX ) )
      f = a->f_NOX;  
    if ( !stricmp (cr_Code, e_SO2 ) )
      f = a->f_SO2;
    return f; 
}

/******************************************************
* Name _CnvEmis 
* Desc: converts emission from grams per squear meter
*       to Pounds or Tons
******************************************************/
float _CnvEmis (float f, char cr_EU[])
{
float g; 
/* Emission amounts are in grams per square meter */
   if ( !stricmp (cr_EU, "Pound") ) 
     g = GramSqMt_To_Pounds(f); 
   else if ( !stricmp (cr_EU, "Ton") ) {
     g = GramSqMt_To_Pounds(f); 
     g = g / 2000.0; }
   else 
     g = f; 

    return g; 
}



#ifdef OLDOOOO
/*****************************************************************
* Name: Emission_CSVSum_DefFac
* Desc: CSV file Summary for the default emissions factors
*       this it the counter part of the Expanded Factor .csv
* Note-1: round these off the flame and smold totals to match
*         GUI smoke report, don't have to round the duff because
*         it's added to the smoldering 
*
******************************************************************/
int  Wnd_Mai::Emission_CSVSum_DefFac  (char cr_FN[], char cr_ErrMes[])
{
int i,j,k,ix; 
float f,fN_Tot; 
//float f_Flame, f_Smolder, f_Duff;
float f_ConFla, f_ConSmo, f_ConDuf; 
float f_FacFla, f_FacSmo, f_FacDuf; 
double d_FacFla, d_FacSmo;  
float fN_EmiFla, fN_EmiSmo, fN_EmiDuf, Rnd; 

// float f_Fla,f_Smo,f_Duf;
float f_EmiFla, f_EmiSmo, f_EmiDuf; 
float f_Fla, f_Smo, f_Duf;

char cr_EU[100], cr_Fmt[300]; 
FILE  *fh;
d_CO CO,  *a_CO;
d_CI CI,  *a_CI;
d_SI SI,  *a_SI;
d_SO SO,  *a_SO;
d_FLI  s_FLI; 

  fN_EmiFla = fN_EmiSmo = fN_EmiDuf = fN_Tot = 0; 
  Rnd = 0;  // 0.5; 

/* Get the output Emission Unit from the Settings Window */
  this->set_frm->EU_Unit(cr_EU); 
 
  strcpy (cr_ErrMes,""); 
  fh = fopen (cr_FN, "w" ) ;
  if ( fh == NULL ) {   
	sprintf (cr_ErrMes, "Can't open file: %s", cr_FN); 
    return 0; }

  a_CO = &CO; 
  a_CI = &CI; 
  a_SI = &SI;
  a_SO = &SO; 

  CI_Init (&CI);
  CO_Init (&CO); 
  SI_Init (&SI);
  SO_Init (&SO);  

/* Get Inputs and Run FOFEM */
  if ( !Get_Inputs(&s_FLI)) {             /* Get inputs from GUI */
     myMessageBox(s_FLI.cr_ErrMes);
     return 0; }

/* Run FOFEM calcs */
  i = CalcSub(&s_FLI, &CI, &CO, &SI, &SO, "", cr_ErrMes);
  if ( i == 0 )                         
    return 0;                          
  
// test - try
float f_CO2F; 
   f_CO2F  = _RndEmiPnd(a_CO->f_CO2F);  
// test - try'
/* Go thru every time step in SGV table to get total fuel consumed totals */
  f_Fla = f_Smo = f_Duf = 0; 
  for ( i = 0; i < CO.iC_SGV; i++ ) {    /* for each time step */
    if ( CO.sr_SGV[i].f_Sec < 0 )
	  break;
    SGV_FlaSmoDuf (&CO.sr_SGV[i], &f_Fla, &f_Smo, &f_Duf);
    f_ConFla  +=f_Fla;   /* Flame consumed fuels */
	f_ConSmo  +=f_Smo;  /* Smoldering consumed fuels */
    f_ConDuf  +=f_Duf;    /* duff consumed fuel */
  }

/* Output file column headings */ 
//   fprintf (fh, "Emis-Code, Name, Flame-Emis, Coarse-Emis, Duff-Emis, Total-Emis, Flame-Factor, Coarse-Factor, Duff-Factor\n");
   fprintf (fh, "Emis-Code,Name,STFS-Emis,CWDRSC-Emis,DuffRSC-Emis,Total-Emis,STFS-EF,CWDRSC-EF,DuffRSC-EF\n");
 
   if ( !stricmp (cr_EU, "Pound") ) 
  fprintf (fh, ",,%s,%s,%s,%s,%s,%s,%s\n",e_lbAc, e_lbAc, e_lbAc, e_lbAc, e_lbTon, e_lbTon, e_lbTon );
      else if ( !stricmp (cr_EU, "Ton") ) 
     fprintf (fh, ",,%s,%s,%s,%s,%s,%s,%s\n",e_TonAc,e_TonAc,e_TonAc,e_TonAc,e_lbTon, e_lbTon, e_lbTon );
   else 
     fprintf (fh, ",,%s,%s,%s,%s,%s,%s,%s\n",e_GrSqM,e_GrSqM,e_GrSqM,e_GrSqM, e_GrKg, e_GrKg, e_GrKg); 


/* Output each component one row at a time */
   for ( i = 0; i < 100; i++ ) {               /* Each component in table */
      if ( !strcmp ( sr_EF[i].cr_Code, "") )    /* No more */ 
       break; 
 
      ES_GetEmi (sr_EF[i].cr_Code,  &d_FacFla,  &d_FacSmo); 
     
      f_FacFla = (float) d_FacFla;              /* emission factor */
      f_FacSmo = (float) d_FacSmo;   

	  f_EmiFla = f_ConFla * f_FacFla;          /* Tot Consmd * Emis Compnt Factr */
      f_EmiSmo = f_ConSmo * f_FacSmo;
	  f_EmiDuf = f_ConDuf * f_FacSmo; 

      /* default - for metric g/m2 */
      strcpy (cr_Fmt, "%s, %s, %6.3f, %6.3f, %6.3f, %6.3f, %5.3f, %5.3f, %5.3f\n");

	  if ( !stricmp (cr_EU, "Pound") ) {      
        f_EmiFla = GramSqMt_To_Pounds (f_EmiFla); 
        f_EmiSmo = GramSqMt_To_Pounds (f_EmiSmo); 
		f_EmiDuf = GramSqMt_To_Pounds (f_EmiDuf);
        f_FacFla =  f_FacFla * e_gkTolb;     /* To output emiss factors as lb/T pounds per ton */
        f_FacSmo =  f_FacSmo * e_gkTolb;
        f_FacDuf =  f_FacSmo * e_gkTolb;
        f_EmiFla += Rnd;        /* See Note-1 above */ 
        f_EmiSmo += Rnd;  
        strcpy (cr_Fmt, "%s, %s, %6.0f, %6.0f, %6.0f, %6.0f, %5.3f, %5.3f, %5.3f\n"); } 

	  if ( !stricmp (cr_EU, "Ton")) {
        f_EmiFla = GramSqMt_To_Pounds (f_EmiFla); 
        f_EmiSmo = GramSqMt_To_Pounds (f_EmiSmo); 
		f_EmiDuf = GramSqMt_To_Pounds (f_EmiDuf);   
        f_EmiFla = f_EmiFla / 2000.0;           /* Get it to Tons */
        f_EmiSmo = f_EmiSmo / 2000.0;
        f_EmiDuf = f_EmiDuf / 2000.0; 
        f_FacFla =  f_FacFla * e_gkTolb;   /* To output emiss factors as lb/T pounds per ton */
        f_FacSmo =  f_FacSmo * e_gkTolb;
        f_FacDuf =  f_FacDuf * e_gkTolb;
        strcpy (cr_Fmt, "%s, %s, %6.3f, %6.3f, %6.3f, %6.3f, %5.3f, %5.3f, %5.3f\n"); }


      fN_EmiFla += f_EmiFla;              /* Output totals */ 
      fN_EmiSmo += f_EmiSmo;
      fN_EmiDuf += f_EmiDuf;
      f = (f_EmiFla + f_EmiSmo + f_EmiDuf); 
	  fN_Tot += f;

	 fprintf (fh, cr_Fmt, 
         sr_EF[i].cr_Code, sr_EF[i].cr_Name, 
         f_EmiFla, f_EmiSmo, f_EmiDuf,
         f, 
         f_FacFla,f_FacSmo,f_FacSmo);  
   } 

/* Output the totals */ 
/* if English, we already convert to pounds above, so just get to tons per acre */
   if ( !stricmp (cr_EU, "Ton"))
      fprintf (fh, "Total, %s , %6.4f, %6.4f, %6.4f,%6.4f, , , \n",e_TonAc,fN_EmiFla,fN_EmiSmo,fN_EmiDuf, fN_Tot);
  
   else if ( !stricmp (cr_EU, "Pound"))
      fprintf (fh, "Total, %s , %6.0f, %6.0f, %6.0f, %6.0f, , , \n",e_lbAc,fN_EmiFla,fN_EmiSmo,fN_EmiDuf, fN_Tot);

   else 
      fprintf (fh, "Total, %s , %6.4f, %6.4f, %6.4f, %6.4f, , , \n",e_GrSqM,fN_EmiFla,fN_EmiSmo,fN_EmiDuf,fN_Tot);


  fclose (fh); 
  return 1; 
}
#endif  

//888888888888888888888888888888888888888888888888888888888888888888888

/*****************************************************************
* Name: Emission_CSVSum_DefFac
* Desc: CSV file Summary for the default emissions factors
*       this it the counter part of the Expanded Factor .csv
* Note-1: round these off the flame and smold totals to match
*         GUI smoke report, don't have to round the duff because
*         it's added to the smoldering 
*
******************************************************************/
int  Wnd_Mai::Emission_CSVSum_DefFac  (char cr_FN[], char cr_ErrMes[])
{
int i,j,k,ix; 
float f,fN_Tot; 
//float f_Flame, f_Smolder, f_Duff;
float f_ConFla, f_ConSmo, f_ConDuf; 
float f_FacFla, f_FacSmo, f_FacDuf; 
double d_FacFla, d_FacSmo;  
float fN_EmiFla, fN_EmiSmo, fN_EmiDuf, Rnd; 
float  fc; 

float  f_PM10F, f_PM25F, f_CH4F, f_COF, f_CO2F, f_NOXF, f_SO2F; 
float   f_PM10S, f_PM25S, f_CH4S, f_COS, f_CO2S, f_NOXS, f_SO2S; 
float   f_PM10S_Duff, f_PM25S_Duff, f_CH4S_Duff, f_COS_Duff, f_CO2S_Duff, f_NOXS_Duff, f_SO2S_Duff; 
float smo, tot;  


// float f_Fla,f_Smo,f_Duf;
float f_EmiFla, f_EmiSmo, f_EmiDuf; 
float f_Fla, f_Smo, f_Duf;

char cr_EU[100], cr_Fmt[500]; 
FILE  *fh;
d_CO CO,  *a_CO;
d_CI CI,  *a_CI;
d_SI SI,  *a_SI;
d_SO SO,  *a_SO;
d_FLI  s_FLI; 

  fN_EmiFla = fN_EmiSmo = fN_EmiDuf = fN_Tot = 0; 
  Rnd = 0;  // 0.5; 

/* Get the output Emission Unit from the Settings Window */
  this->set_frm->EU_Unit(cr_EU); 
 
  strcpy (cr_ErrMes,""); 
  fh = fopen (cr_FN, "w" ) ;
  if ( fh == NULL ) {   
	sprintf (cr_ErrMes, "Can't open file: %s", cr_FN); 
    return 0; }

  a_CO = &CO; 
  a_CI = &CI; 
  a_SI = &SI;
  a_SO = &SO; 

  CI_Init (&CI);
  CO_Init (&CO); 
  SI_Init (&SI);
  SO_Init (&SO);  

/* Get Inputs and Run FOFEM */
  if ( !Get_Inputs(&s_FLI)) {             /* Get inputs from GUI */
     myMessageBox(s_FLI.cr_ErrMes);
     return 0; }

/* Run FOFEM calcs */
  i = CalcSub(&s_FLI, &CI, &CO, &SI, &SO, "", cr_ErrMes);
  if ( i == 0 )                         
    return 0;                          
  
   if ( !stricmp (cr_EU,"Pound") ) {
	 f_PM10F = _RndEmiPnd(a_CO->f_PM10F);
     f_PM25F = _RndEmiPnd(a_CO->f_PM25F);
     f_CH4F  = _RndEmiPnd(a_CO->f_CH4F ); 
     f_COF   = _RndEmiPnd(a_CO->f_COF );  
     f_CO2F  = _RndEmiPnd(a_CO->f_CO2F);  
     f_NOXF =  _RndEmiPnd(a_CO->f_NOXF ); 
     f_SO2F =  _RndEmiPnd(a_CO->f_SO2F);  

     f_PM10S = _RndEmiPnd(a_CO->f_PM10S);
     f_PM25S = _RndEmiPnd(a_CO->f_PM25S);
     f_CH4S  = _RndEmiPnd(a_CO->f_CH4S); 
     f_COS   = _RndEmiPnd(a_CO->f_COS ); 
     f_CO2S  = _RndEmiPnd(a_CO->f_CO2S); 
     f_NOXS =  _RndEmiPnd(a_CO->f_NOXS);    /* no NOX are created during smoldering */ 
     f_SO2S =  _RndEmiPnd(a_CO->f_SO2S); 

     f_PM10S_Duff  = _RndEmiPnd(a_CO->f_PM10S_Duff );
     f_PM25S_Duff  = _RndEmiPnd(a_CO->f_PM25S_Duff );
     f_CH4S_Duff   = _RndEmiPnd(a_CO->f_CH4S_Duff ); 
     f_COS_Duff    = _RndEmiPnd(a_CO->f_COS_Duff  ); 
     f_CO2S_Duff   = _RndEmiPnd(a_CO->f_CO2S_Duff ); 
     f_NOXS_Duff   =  _RndEmiPnd(a_CO->f_NOXS_Duff );    /* no NOX are created during smoldering */ 
     f_SO2S_Duff   =  _RndEmiPnd(a_CO->f_SO2S_Duff ); 
     fc = e_gkTolb ;  /* Emis factor convert */
     strcpy (cr_Fmt, "%s, %s, %6.0f, %6.0f, %6.0f, %6.0f, %5.0f, %5.0f, %5.0f\n"); }

   else if ( !stricmp (cr_EU, "Ton") ) {
     f_PM10F = a_CO->f_PM10F / 2000.0 ;
     f_PM25F = a_CO->f_PM25F / 2000.0 ;
     f_CH4F  = a_CO->f_CH4F  / 2000.0 ;
     f_COF   = a_CO->f_COF   / 2000.0 ;
     f_CO2F  = a_CO->f_CO2F  / 2000.0 ;
     f_NOXF =  a_CO->f_NOXF  / 2000.0 ;
     f_SO2F =  a_CO->f_SO2F  / 2000.0 ;

     f_PM10S = a_CO->f_PM10S / 2000.0 ;
     f_PM25S = a_CO->f_PM25S / 2000.0 ;
     f_CH4S  = a_CO->f_CH4S  / 2000.0 ;
     f_COS   = a_CO->f_COS   / 2000.0 ;
     f_CO2S  = a_CO->f_CO2S  / 2000.0 ;
     f_NOXS =  a_CO->f_NOXS  / 2000.0 ;
     f_SO2S =  a_CO->f_SO2S  / 2000.0 ; 

     f_PM10S_Duff = a_CO->f_PM10S_Duff / 2000.0 ;
     f_PM25S_Duff = a_CO->f_PM25S_Duff / 2000.0 ;
     f_CH4S_Duff  = a_CO->f_CH4S_Duff  / 2000.0 ;
     f_COS_Duff   = a_CO->f_COS_Duff   / 2000.0 ;
     f_CO2S_Duff  = a_CO->f_CO2S_Duff  / 2000.0 ;
     f_NOXS_Duff =  a_CO->f_NOXS_Duff  / 2000.0 ;
     f_SO2S_Duff =  a_CO->f_SO2S_Duff  / 2000.0 ; 
     fc = e_gkTolb ;  /* Emis factor convert */
     strcpy (cr_Fmt, "%s, %s, %6.3f, %6.3f, %6.3f, %6.3f, %5.3f, %5.3f, %5.3f\n");}

   else {
     f_PM10F = PndAcre_To_GramSqMt (a_CO->f_PM10F);
     f_PM25F = PndAcre_To_GramSqMt (a_CO->f_PM25F);
     f_CH4F  = PndAcre_To_GramSqMt (a_CO->f_CH4F );
     f_COF   = PndAcre_To_GramSqMt (a_CO->f_COF  );
     f_CO2F  = PndAcre_To_GramSqMt (a_CO->f_CO2F );
     f_NOXF =  PndAcre_To_GramSqMt (a_CO->f_NOXF );
     f_SO2F =  PndAcre_To_GramSqMt (a_CO->f_SO2F );

     f_PM10S = PndAcre_To_GramSqMt (a_CO->f_PM10S);
     f_PM25S = PndAcre_To_GramSqMt (a_CO->f_PM25S);
     f_CH4S  = PndAcre_To_GramSqMt (a_CO->f_CH4S );
     f_COS   = PndAcre_To_GramSqMt (a_CO->f_COS  );
     f_CO2S  = PndAcre_To_GramSqMt (a_CO->f_CO2S );
     f_NOXS =  PndAcre_To_GramSqMt (a_CO->f_NOXS );
     f_SO2S =  PndAcre_To_GramSqMt (a_CO->f_SO2S );

     f_PM10S_Duff = PndAcre_To_GramSqMt (a_CO->f_PM10S_Duff);
     f_PM25S_Duff = PndAcre_To_GramSqMt (a_CO->f_PM25S_Duff);
     f_CH4S_Duff  = PndAcre_To_GramSqMt (a_CO->f_CH4S_Duff );
     f_COS_Duff   = PndAcre_To_GramSqMt (a_CO->f_COS_Duff  );
     f_CO2S_Duff  = PndAcre_To_GramSqMt (a_CO->f_CO2S_Duff );
     f_NOXS_Duff =  PndAcre_To_GramSqMt (a_CO->f_NOXS_Duff );
     f_SO2S_Duff =  PndAcre_To_GramSqMt (a_CO->f_SO2S_Duff );
     fc = 1.0 ;  /* Emis factor convert, none needed for metric  */
     strcpy (cr_Fmt, "%s, %s, %6.3f, %6.3f, %6.3f, %6.3f, %5.3f, %5.3f, %5.3f\n");}


/* Output file column headings */ 
   fprintf (fh, "Emis-Code, Name, Flame-Emis, Smolder-Emis, Duff-Emis, Total-Emis, Flame-Factor, Smolder-Factor, Duff-Factor\n");
//   fprintf (fh, "Emis-Code,Name,STFS-Emis,CWDRSC-Emis,DuffRSC-Emis,Total-Emis,STFS-EF,CWDRSC-EF,DuffRSC-EF\n");
 
   if ( !stricmp (cr_EU, "Pound") ) 
     fprintf (fh, ",,%s,%s,%s,%s,%s,%s,%s\n",e_lbAc, e_lbAc, e_lbAc, e_lbAc, e_lbTon, e_lbTon, e_lbTon );
   else if ( !stricmp (cr_EU, "Ton") ) 
     fprintf (fh, ",,%s,%s,%s,%s,%s,%s,%s\n",e_TonAc,e_TonAc,e_TonAc,e_TonAc,e_lbTon, e_lbTon, e_lbTon );
   else 
     fprintf (fh, ",,%s,%s,%s,%s,%s,%s,%s\n",e_GrSqM,e_GrSqM,e_GrSqM,e_GrSqM, e_GrKg, e_GrKg, e_GrKg); 
        

                                                                                                                                                                      
   ES_GetEmi ("co2",  &d_FacFla,  &d_FacSmo); 
  _EmisConv (&d_FacFla, &d_FacSmo, fc );
  _MakeTot (f_CO2F, f_CO2S,f_CO2S_Duff, &smo, &tot );
   fprintf (fh, cr_Fmt, "CO 2","Carbon Dioxide",   f_CO2F,  smo,  f_CO2S_Duff, tot, d_FacFla, d_FacSmo, d_FacSmo); 

   ES_GetEmi ("co",  &d_FacFla,  &d_FacSmo); 
  _EmisConv (&d_FacFla, &d_FacSmo, fc );
  _MakeTot (f_COF,f_COS,f_COS_Duff, &smo, &tot) ;
   fprintf (fh, cr_Fmt, "CO","Carbon Monoxide",    f_COF,   smo,  f_COS_Duff,  tot,  d_FacFla,d_FacSmo,d_FacSmo); 

   ES_GetEmi ("ch4",  &d_FacFla,  &d_FacSmo); 
  _EmisConv (&d_FacFla, &d_FacSmo, fc );
  _MakeTot (f_CH4F, f_CH4S, f_CH4S_Duff,&smo, &tot) ; 
   fprintf (fh, cr_Fmt, "CH4","Methane",           f_CH4F,  smo,  f_CH4S_Duff, tot,  d_FacFla,d_FacSmo,d_FacSmo); 

   ES_GetEmi ("nox",  &d_FacFla,  &d_FacSmo);
  _EmisConv (&d_FacFla, &d_FacSmo, fc );
  _MakeTot (f_NOXF, f_NOXS,  f_NOXS_Duff,&smo, &tot) ;
   fprintf (fh, cr_Fmt, "NOX","Nitrogen Oxides",   f_NOXF,  smo,  f_NOXS_Duff,  tot,  d_FacFla,d_FacSmo,d_FacSmo); 

   ES_GetEmi ("so2",  &d_FacFla,  &d_FacSmo); 
  _EmisConv (&d_FacFla, &d_FacSmo, fc );
  _MakeTot (f_SO2F, f_SO2S,f_SO2S_Duff,&smo, &tot) ;
   fprintf (fh, cr_Fmt, "SO2","Sulfor Oxides",     f_SO2F,  smo,   f_SO2S_Duff, tot,  d_FacFla,d_FacSmo,d_FacSmo); 

   ES_GetEmi ("pm25",  &d_FacFla,  &d_FacSmo);
   _EmisConv (&d_FacFla, &d_FacSmo, fc );
  _MakeTot ( f_PM25F, f_PM25S, f_PM25S_Duff,&smo, &tot) ; 
   fprintf (fh, cr_Fmt, "PM2.5","PM2.5",           f_PM25F, smo,   f_PM25S_Duff, tot, d_FacFla,d_FacSmo,d_FacSmo); 

   ES_GetEmi ("pm10",  &d_FacFla,  &d_FacSmo); 
  _EmisConv (&d_FacFla, &d_FacSmo, fc );
  _MakeTot ( f_PM10F, f_PM10S, f_PM10S_Duff,&smo, &tot) ; 
   fprintf (fh, cr_Fmt, "PM10","PM10",             f_PM10F, smo,   f_PM10S_Duff, tot, d_FacFla,d_FacSmo,d_FacSmo); 


   fN_EmiFla =  f_PM10F + f_PM25F + f_CH4F + f_COF  + f_CO2F + f_NOXF + f_SO2F; 
   fN_EmiDuf =  f_PM10S_Duff + f_PM25S_Duff + f_CH4S_Duff + f_COS_Duff  + f_CO2S_Duff + f_NOXS_Duff + f_SO2S_Duff; 
   fN_EmiSmo =  f_PM10S + f_PM25S + f_CH4S + f_COS  + f_CO2S + f_NOXS + f_SO2S; 
  _MakeTot (fN_EmiFla, fN_EmiSmo, fN_EmiDuf,&smo, &tot) ; 


/* Output the totals */ 
/* if English, we already convert to pounds above, so just get to tons per acre */
   if ( !stricmp (cr_EU, "Ton"))
      fprintf (fh, "Total, %s , %6.4f, %6.4f, %6.4f,%6.4f, , , \n",e_TonAc,fN_EmiFla,smo,fN_EmiDuf,tot);
  
   else if ( !stricmp (cr_EU, "Pound"))
      fprintf (fh, "Total, %s , %6.0f, %6.0f, %6.0f, %6.0f, , , \n",e_lbAc,fN_EmiFla,smo,fN_EmiDuf, tot);

   else 
      fprintf (fh, "Total, %s , %6.4f, %6.4f, %6.4f, %6.4f, , , \n",e_GrSqM,fN_EmiFla,smo,fN_EmiDuf,tot);


  fclose (fh); 
  return 1; 
}

/*****************************************************************************************/
void _EmisConv (double *ad_FacFla, double *ad_FacSmo, float fc )
{
  *ad_FacFla = *ad_FacFla * fc;
  *ad_FacSmo = *ad_FacSmo * fc;
}


/****************************************************************************************/
void  _MakeTot (float f_fla, float f_Smo, float f_SmoDuff, float *af_smo, float *af_tot )
{
float tot, smo;

   smo = f_Smo - f_SmoDuff;
   tot = f_fla + f_Smo;

   *af_smo = smo;
   *af_tot = tot; 
}



//88888888888888888888888888888888888888888888888888888888888888888888




/**********************************************************************
* Name: Emission_CSVSum
* Desc: CSV Summary file for the Expanded emission factors 
*       Creates one row for each emission component with columns
*        for emssion amounts (flame,coarse,smolder) and emission
*        factors.
**********************************************************************/
int Wnd_Mai::Emission_CSVSumExpFac (char cr_FN[], char cr_ErrMes[]) 
{
int i,j,k,ix; 
float f,a,b,c,d,g, f_Percent, f_Factor, fN_Tot;
float f_ConFla,f_ConSmo,f_ConDuf;
float f_FacFla,f_FacSmo, f_FacDuf;
float f_EmiFla, f_EmiSmo, f_EmiDuf; 
float fN_EmiFla, fN_EmiSmo, fN_EmiDuf; 

char cr_GrpNum[100], cr_CovTyp[300], cr_Name[500], cr_Code[100];
char cr_EmiFla[20], cr_EmiCoa[20], cr_EmiDuf[20]; 
char cr_EU[100], cr_Fmt[200]; 

d_CO CO,  *a_CO;
d_CI CI,  *a_CI;
d_SI SI,  *a_SI;
d_SO SO,  *a_SO;
d_FLI  s_FLI; 
d_ST ST;
d_STT STT; 
d_STS STS; 
FILE  *fh;

/* See Note-2 above */
#define e_Crit 8
#define e_NH3  4

/* Get the output Emission Unit from the Settings Window */
  this->set_frm->EU_Unit(cr_EU); 
 
  strcpy (cr_ErrMes,""); 
  fh = fopen (cr_FN, "w" ) ;
  if ( fh == NULL ) {   
	sprintf (cr_ErrMes, "Can't open file: %s", cr_FN); 
    return 0; }

  a_CO = &CO;  /* just doing this to not re-write old code that uses a_CO-> */
  a_CI = &CI; 
  a_SI = &SI;
  a_SO = &SO; 

  CI_Init (&CI);
  CO_Init (&CO); 
  SI_Init (&SI);
  SO_Init (&SO);  

/* Get Inputs and Run FOFEM */
  if ( !Get_Inputs(&s_FLI)) {             /* Get inputs from GUI */
     myMessageBox(s_FLI.cr_ErrMes);
     return 0; }
  i = CalcSub(&s_FLI, &CI, &CO, &SI, &SO, "", cr_ErrMes);
  if ( i == 0 )                         
    return 0;                          
  
/* Get the 3 Emission Group Numbers from GUI dropdowns */
  _GetEmiGrp (s_FLI.cr_EmisFlame, cr_EmiFla); 
  _GetEmiGrp (s_FLI.cr_EmisSmold, cr_EmiCoa);
  _GetEmiGrp (s_FLI.cr_EmisDuff,  cr_EmiDuf); 

  f_ConFla = 0;
  f_ConSmo = 0;
  f_ConDuf = 0; 

/* Get Total Consumed Fuel Amounts, */
/* Coarse = smoldering 1k wood where fire intensity < 15 kW */
  for ( i = 0; i < CO.iC_SGV; i++ ) {
    if ( CO.sr_SGV[i].f_Sec < 0 )
	  break;
    f_ConFla += CO.sr_SGV[i].f_FlaCon;
	f_ConSmo += CO.sr_SGV[i].f_SmoCon;
    f_ConDuf += CO.sr_SGV[i].f_DuffCon; 
  }

/* Take the Duff out of the Smolder */
  f_ConSmo = f_ConSmo - f_ConDuf; 

/* Tested OK, it adds up to Total consumed in Fuel Consump Report */
a = KgSq_To_TPA(f_ConFla);
b = KgSq_To_TPA(f_ConSmo);
c = KgSq_To_TPA(f_ConDuf); 
f = a + b + c;

   fN_EmiFla = 0;   /*Emis Totals for testing */ 
   fN_EmiSmo = 0;
   fN_EmiDuf = 0;
   fN_Tot = 0; 

// fprintf (fh, "Emis-Code, Name, Flame-Emis, Coarse-Emis, Duff-Emis, Total-Emis, Flame-Factor, Coarse-Factor, Duff-Factor\n");
    
   fprintf (fh, "Emis-Code,Name,STFS-Emis,CWDRSC-Emis,DuffRSC-Emis,Total-Emis,STFS-EF,CWDRSC-EF,DuffRSC-EF\n");
 
   if ( !stricmp (cr_EU, "Pound") ) 
     fprintf (fh, ",,%s,%s,%s,%s,%s,%s,%s\n",e_lbAc, e_lbAc, e_lbAc, e_lbAc, e_lbTon, e_lbTon, e_lbTon );
   else if ( !stricmp (cr_EU, "Ton") ) 
     fprintf (fh, ",,%s,%s,%s,%s,%s,%s,%s\n",e_TonAc,e_TonAc,e_TonAc,e_TonAc, e_lbTon, e_lbTon, e_lbTon  );
   else 
     fprintf (fh, ",,%s,%s,%s,%s,%s,%s,%s\n",e_GrSqM,e_GrSqM,e_GrSqM,e_GrSqM, e_GrKg, e_GrKg, e_GrKg); 

/* Put a Row for Each Emission Component */
   for ( j = 0; j < 100000; j++ ) {
      k = NES_Get_Factor(cr_EmiFla, j, cr_CovTyp, &f_FacFla, cr_Name, cr_Code);
 	  if ( k == 0 )         /* No more */ 
		break; 

      NES_Get_Factor(cr_EmiCoa, j, cr_CovTyp, &f_FacSmo, cr_Name, cr_Code);
      NES_Get_Factor(cr_EmiDuf, j, cr_CovTyp, &f_FacDuf, cr_Name, cr_Code);

      StrRepChr (cr_Name, ',', ';');  /* Get rid of any commas */ 
      StrRepChr (cr_Code, ',', ';'); 

	  f_EmiFla = f_ConFla * f_FacFla;               /* Tot Consmd * Emis Compnt Factr */
      f_EmiSmo = f_ConSmo * f_FacSmo;
	  f_EmiDuf = f_ConDuf * f_FacDuf; 

	  if ( !stricmp (cr_EU, "Pound") ) {
        f_EmiFla = GramSqMt_To_Pounds (f_EmiFla); 
        f_EmiSmo = GramSqMt_To_Pounds (f_EmiSmo); 
		f_EmiDuf = GramSqMt_To_Pounds (f_EmiDuf);
        f_FacFla =  f_FacFla * e_gkTolb;    /* emission factors to lb/T - pounds per ton */ 
        f_FacSmo =  f_FacSmo * e_gkTolb;
        f_FacDuf =  f_FacDuf * e_gkTolb; } 

	  if ( !stricmp (cr_EU, "Ton")) {
        f_EmiFla = GramSqMt_To_Pounds (f_EmiFla); 
        f_EmiSmo = GramSqMt_To_Pounds (f_EmiSmo); 
		f_EmiDuf = GramSqMt_To_Pounds (f_EmiDuf);   
        f_EmiFla = f_EmiFla / 2000.0;
        f_EmiSmo = f_EmiSmo / 2000.0;
        f_EmiDuf = f_EmiDuf / 2000.0; 
        f_FacFla =  f_FacFla * e_gkTolb;  /* emission factors to lb/T - pounds per ton */ 
        f_FacSmo =  f_FacSmo * e_gkTolb;
        f_FacDuf =  f_FacDuf * e_gkTolb;}

	  f = f_EmiFla + f_EmiSmo + f_EmiDuf; 
      fN_Tot += f; 
	  fprintf (fh, "%s, %s", cr_Code,cr_Name); 
			  
      _GetFmt (f_EmiFla,cr_Fmt, cr_EU); fprintf (fh,cr_Fmt,f_EmiFla);  /* Emission amounts */
      _GetFmt (f_EmiSmo,cr_Fmt, cr_EU); fprintf (fh,cr_Fmt,f_EmiSmo);
      _GetFmt (f_EmiDuf,cr_Fmt, cr_EU); fprintf (fh,cr_Fmt,f_EmiDuf);
      _GetFmt (f,       cr_Fmt, cr_EU); fprintf (fh,cr_Fmt,f);         /* Total */ 

      _GetFmt (f_FacFla,cr_Fmt,cr_EU); fprintf (fh,cr_Fmt,f_FacFla);   /* Emission Factors */
      _GetFmt (f_FacSmo,cr_Fmt,cr_EU); fprintf (fh,cr_Fmt,f_FacSmo);
      _GetFmt (f_FacDuf,cr_Fmt,cr_EU); fprintf (fh,cr_Fmt,f_FacDuf);

      fprintf (fh, "\n"); 

      fN_EmiFla += f_EmiFla; /* Emis Grand Totals */
      fN_EmiSmo += f_EmiSmo;
      fN_EmiDuf += f_EmiDuf;  

     } /* for j - each emis component */

/* if English, we already convert to pounds above, so just get to tons per acre */
   if ( !stricmp (cr_EU, "Ton"))
      fprintf (fh, "Total, %s , %6.4f, %6.4f, %6.4f,%6.4f, , , \n",e_TonAc,fN_EmiFla,fN_EmiSmo,fN_EmiDuf, fN_Tot);
  
   else if ( !stricmp (cr_EU, "Pound"))
      fprintf (fh, "Total, %s , %6.4f, %6.4f, %6.4f,%6.4f, , , \n",e_lbAc,fN_EmiFla,fN_EmiSmo,fN_EmiDuf, fN_Tot);

   else 
      fprintf (fh, "Total, %s , %6.4f, %6.4f, %6.4f, %6.4f, , , \n",e_GrSqM,fN_EmiFla,fN_EmiSmo,fN_EmiDuf,fN_Tot);

  fclose (fh); 
  return 1; 

}

/***************************************************************************
* Name: Consumed_FlaSmo  
* Desc: Report out the total consumed flaming and smoldering amounts
*
***************************************************************************/
int  Wnd_Mai::Consumed_FlaSmo ()
{
int i; 
float  f_FI;
float  f_Fla, f_Smo, fN_Fla, fN_Smo; 
char  cr[1000]; 
char  AA[10][20];
char  BB[10][20];

   fN_Fla = fN_Smo = 0; 

   strcpy (AA[0], "1kHr"); 
   strcpy (AA[1], "100Hr"); 
   strcpy (AA[2], "10Hr"); 
   strcpy (AA[3], "1Hr");
   strcpy (AA[4], "Litter");
   strcpy (AA[5], "Duff");
   strcpy (AA[6], "HSFB");
   strcpy (AA[7], "");

   strcpy (BB[0], "  3+"); 
   strcpy (BB[1], "1-3"); 
   strcpy (BB[2], "1/4-1"); 
   strcpy (BB[3], "0-1/4");
   strcpy (BB[4], "Litter");
   strcpy (BB[5], "Duff");
   strcpy (BB[6], "HSFB");
   strcpy (BB[7], "");

   _rep ("\n\n\n");
   _rep ("       Comsumed Fuel Component \n");
   _rep ("   Component   Flaming    Smoldering \n");
   _rep ("     inch       T/ac        T/ac  \n");


   for ( i = 0; i < 100; i++ ) {
     if ( !stricmp (AA[i], "" ) ) 
        break; 
     SGV_GetFS (AA[i], &f_Fla, &f_Smo); 
     sprintf (cr, " %8s    %7.2f     %7.2f       %7.2f  \n", BB[i], f_Fla, f_Smo, (f_Fla+f_Smo)); 
     _rep (cr); 
     fN_Fla += f_Fla;
     fN_Smo += f_Smo; 
   }

     sprintf (cr, "    Total    %7.2f     %7.2f       %7.2f \n",  fN_Fla, fN_Smo, (fN_Fla+fN_Smo)); 
     _rep (cr); 


   return 1; 
}

/************************************************************************
* Name: SoilReport_Massman
* Desc: The Massman Soil Report
*       The report will be created for the last simulation that was 
*       run. 
*       Unlike the Campbell soil report which runs the simulation automatically
*       when the report button is clicked, the Massman simulation has to be
*       run by the user going to the Massman soil window and running 
*       it then returning to the main window to run the report. 
*       
*************************************************************************/
int  Wnd_Mai::SoilReport_Massman()
{
int i,L,T, i_Lay, i_Max1, i_Max2;
float f_Heat, f_Moist, f_psin, f_Time; 
float f_MaxHea, f_MaxTim; 
char cr[400], cr_Date[100]; 
float fr_Heat[30], fr_Time[30]; 

  i_Max1 = i_Max2 = -1; 

  i_Lay = 14;

  for ( L = 1; L <= i_Lay ; L++ ) {     /* For Each Soil Layer */
     fr_Heat[L] = fr_Time[L] = 0; 
     for ( T = 0; T < 200000; T++  ) {  /* Time - seconds */ 
      i = HTA_Get (L, T, &f_Heat, &f_Moist, &f_psin, &f_Time);   /* Get data */
      if ( i == 0 ) 
        break;  
      if ( f_Heat > fr_Heat[L] ) {
        fr_Heat[L] = f_Heat;
        fr_Time[L] = f_Time; } 

      if ( f_Heat > (float)e_Max1 )      /* Deepest layer reaching Max (60,270) degrees */
        i_Max1 = L;
      if ( f_Heat > (float) e_Max2 )
        i_Max2 = L;

    } /* For T */ 
  }  /* For L */ 



   _rep ("\n\n");
   GetDate (cr_Date); 
   sprintf (cr, "           TITLE: Results of FOFEM model execution on date: %s\n",cr_Date);
   _rep (cr); 
   _rep ("                           Soil Heat Report - Massman\n\n");
   _rep ("                                     Soil Layer Maximum Temperature\n"); 

   _rep ("Depth (cm)    "); 
   for ( L = 1; L <= i_Lay ; L++ ){  
     sprintf (cr,"%2d   ",L-1);
    _rep (cr); } 
 
  _rep ("\nTemp (C)    "); 
   for ( L = 1; L <= i_Lay ; L++ ) { 
      sprintf (cr,"%4.0f ",fr_Heat[L]); 
     _rep(cr);  }

   _rep ("\nTime (min)  "); 
   for ( L = 1; L <= i_Lay ; L++ ) { 
      sprintf (cr,"%4.0f ",fr_Time[L]); 
     _rep(cr);  }

     _rep ("\n\n"); 

/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
/* Do Max Temp at Layer, put to report and save for needed totals later      */

   if ( i_Max1 == -1 ){
      sprintf (cr, "Max Depth Having %d degrees: - None - \n", e_Max1);
     _rep (cr); }
   else {
     sprintf (cr, "Max Depth Having %d degrees: %d\n", e_Max1, i_Max1 - 1);
     _rep (cr); }

   if ( i_Max2 == -1 ) {
     sprintf (cr,"Max Depth Having %d degrees: - None - \n", e_Max2);
     _rep (cr); }
   else {
     sprintf (cr,"Max Depth Having %d degrees: %d\n", e_Max2, i_Max2 - 1 );
     _rep (cr); }





  return 1; 
}