
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_sgv.c     Smoke Graphing values
* Desc: Functions to deal with smoke graphing values
* Date:
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
#include   <stdlib.h>
#include   <string.h>
#include   <stdio.h>

#include  "fof_sgv.h"
#include  "fof_util.h"
#include  "fof_lem.h"\

#include  "fof_ansi.h"

int   iX_SGV;

d_SGV  sr_SGV[eC_SGV+1];

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SGV_Prepare
* Desc: Set the value that you want to use and convert it to pounds per Acre
*       This goes thru the table and for every entry sets into the 'amount'
*       field of that same entry the values you'll want to be graphing it as
*       converts it to Pounds PER ACRE, so you as a caller can deal with it
*       like that.
* NOTE: In order to graph smoke the burnup must have been run so that it
*       can load the SGV table, which this function will need.
*   In: cr_Name......USE defines in fof_sgv.h file
*  Out: af_MaxTim....largest second(time) value found
*       af_MaxAmt....largets amount that was found, in Pounds per ACRE
*  Ret: Total of Amount
*       -1 Error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
float SGV_Prepare (char cr_Name[], float *af_MaxTim, float *af_MaxAmt)
{
int i;
float f_Amt, f_Total;

  *af_MaxTim = 0;
  *af_MaxAmt = 0;
  f_Total = 0;

  for ( i = 0; i < eC_SGV; i++ ) {
     if ( sr_SGV[i].f_Sec == -1 )             /* Hit end of table             */
       break;
          if (!strcmp (cr_Name,e_SGV_PM2_5)) f_Amt = sr_SGV[i].f_PM2_5;
     else if (!strcmp (cr_Name,e_SGV_PM10 )) f_Amt = sr_SGV[i].f_PM10;
     else if (!strcmp (cr_Name,e_SGV_CH4  )) f_Amt = sr_SGV[i].f_CH4;
     else if (!strcmp (cr_Name,e_SGV_CO2  )) f_Amt = sr_SGV[i].f_CO2;
     else if (!strcmp (cr_Name,e_SGV_CO   )) f_Amt = sr_SGV[i].f_CO;

     else if (!strcmp (cr_Name,e_SGV_NOX  )) f_Amt = sr_SGV[i].f_NOX;
     else if (!strcmp (cr_Name,e_SGV_SO2  )) f_Amt = sr_SGV[i].f_SO2;

     else if (!strcmp (cr_Name,e_SGV_Inten)){ /* This is in kilowatts per  */
         sr_SGV[i].f_Amt = sr_SGV[i].f_Inten;  /* meter sqr, so no convert   */
         f_Total += sr_SGV[i].f_Amt;
         goto A;}
     else {
        return -1;
     /*  Took this out on for new version */
     /*   Error_Window ("Logic Error", "SGV_Prepare"); */
     /*   return 0; */
      }

     sr_SGV[i].f_Amt = GramSqMt_To_Pounds(f_Amt);

     f_Total += sr_SGV[i].f_Amt;      /* NEED TO TOTAL befor doing interval  */

     if ( i == 0 )                                      /* deal with burnup  */
       sr_SGV[i].f_Amt = sr_SGV[i].f_Amt / (float) 60;  /* time intervals    */
     else
       sr_SGV[i].f_Amt = sr_SGV[i].f_Amt / (float) 15;

A:
     if ( sr_SGV[i].f_Sec > *af_MaxTim )      /* find largest                */
       *af_MaxTim = sr_SGV[i].f_Sec;
     if ( sr_SGV[i].f_Amt > *af_MaxAmt )
       *af_MaxAmt = sr_SGV[i].f_Amt;

   }  /* for i */

  if ( i == 0 )       /* Nothing in table */
    return -1;

  return f_Total;

}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: GramSqMt_To_Pounds
* Desc: Convert Grams per Square Meter to Pounds Per Acre
*   In: f_Gram......grams per square meter
*  Ret: pounds per acre
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
float  GramSqMt_To_Pounds  (float f_Gram) 
{
float f, f_Pounds, f_Test;
   f = f_Gram * (float) 4046.86;   // 4047;              /* 4047 meters in an acre         */
   f_Pounds = f / (float) 453.592;          /* grams per pound                */


//  grams per (square meter) = 8.92179122 pound per acre
// This is here just for a test, and the numbers are matching up 
  f_Test = f_Gram * 8.92179122;

  return  f_Pounds; 

}



/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SGV_GetTimTP
* Desc:
*  Ret: 1 OK,  0 No more
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int SGV_GetTimPPA (int iX, float *af_Time,  float *af_Amt)
{
  *af_Time = sr_SGV[iX].f_Sec;
  *af_Amt  = sr_SGV[iX].f_Amt;
  if ( *af_Time == -1 )
    return 0;
  return 1;
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SGV_Init
* Desc: Init the table, gotta do this before you can load it with anything
*  Ret: 1 OK
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int SGV_Init  ()
{
int i;
  for ( i = 0; i <= eC_SGV; i++ ) {
     sr_SGV[i].f_Sec = -1;
     sr_SGV[i].f_Amt = 0;
     sr_SGV[i].f_PM2_5 = 0;
     sr_SGV[i].f_PM10 = 0;
     sr_SGV[i].f_CH4 = 0;
     sr_SGV[i].f_CO2 = 0;
     sr_SGV[i].f_CO = 0;

     sr_SGV[i].f_NOX = 0;
     sr_SGV[i].f_SO2 = 0;

     sr_SGV[i].f_Inten = 0;

     sr_SGV[i].f_FlaCon = 0;
     sr_SGV[i].f_SmoCon = 0; 
     sr_SGV[i].f_DuffCon = 0;
     sr_SGV[i].f_HSFBCon = 0;  


     sr_SGV[i].f_rm_Lit   = 0;
     sr_SGV[i].f_rm_1Hr   = 0;
     sr_SGV[i].f_rm_10Hr  = 0;
     sr_SGV[i].f_rm_100Hr = 0;
     sr_SGV[i].f_rm_1kHr    = 0;

     sr_SGV[i].f_cn_Lit   = 0;
     sr_SGV[i].f_cn_1Hr   = 0;
     sr_SGV[i].f_cn_10Hr  = 0;
     sr_SGV[i].f_cn_100Hr = 0;
     sr_SGV[i].f_cn_1kHr    = 0;

     sr_SGV[i].f_cn_1kHrFla = 0; 
     sr_SGV[i].f_cn_1kHrSmo = 0; 


     sr_SGV[i].f_cn_LitSmoPerCent = 0; 
     sr_SGV[i].f_cn_1HrSmoPerCent   = 0;
     sr_SGV[i].f_cn_10HrSmoPerCent  = 0;
     sr_SGV[i].f_cn_100HrSmoPerCent = 0;
     sr_SGV[i].f_cn_1kSmoPerCent    = 0;

   }

   return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: SGV_Set
* Desc: Put an entry into the table.
* NOTE: If the table gets full it doesn't matter becasue this holding
*        values for smoke graph so we'll just have less numbers to graph
*   In: a_SGV......stuff you want loaded
*                  NOTE, you don't set the f_Amt field
*  Ret: 1 OK,  0 Table is full
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/
int SGV_Set  (d_SGV  *a_SGV)
{
int i;
  for ( i = 0; i < eC_SGV; i++ ) {
    if ( sr_SGV[i].f_Sec == -1 ) {
      sr_SGV[i].f_Sec    = a_SGV->f_Sec ;
      sr_SGV[i].f_PM2_5  = a_SGV->f_PM2_5;
      sr_SGV[i].f_PM10   = a_SGV->f_PM10;
      sr_SGV[i].f_CH4    = a_SGV->f_CH4;
      sr_SGV[i].f_CO2    = a_SGV->f_CO2;
      sr_SGV[i].f_CO     = a_SGV->f_CO;

      sr_SGV[i].f_NOX    = a_SGV->f_NOX;
      sr_SGV[i].f_SO2    = a_SGV->f_SO2;

      sr_SGV[i].f_Inten  = a_SGV->f_Inten;
      
	  sr_SGV[i].f_FlaCon  = a_SGV->f_FlaCon; 
      sr_SGV[i].f_SmoCon = a_SGV->f_SmoCon;  
      sr_SGV[i].f_DuffCon   = a_SGV->f_DuffCon; 
      sr_SGV[i].f_HSFBCon   = a_SGV->f_HSFBCon; 

      sr_SGV[i].f_rm_Lit   = a_SGV->f_rm_Lit  ;
      sr_SGV[i].f_rm_1Hr   = a_SGV->f_rm_1Hr  ;
      sr_SGV[i].f_rm_10Hr  = a_SGV->f_rm_10Hr ;
      sr_SGV[i].f_rm_100Hr = a_SGV->f_rm_100Hr;
      sr_SGV[i].f_rm_1kHr  = a_SGV->f_rm_1kHr   ;
							
      sr_SGV[i].f_cn_Lit   = a_SGV->f_cn_Lit  ;
      sr_SGV[i].f_cn_1Hr   = a_SGV->f_cn_1Hr  ;
      sr_SGV[i].f_cn_10Hr  = a_SGV->f_cn_10Hr ;
      sr_SGV[i].f_cn_100Hr = a_SGV->f_cn_100Hr;
      sr_SGV[i].f_cn_1kHr  = a_SGV->f_cn_1kHr   ;

      sr_SGV[i].f_cn_1kHrFla = a_SGV->f_cn_1kHrFla; 
      sr_SGV[i].f_cn_1kHrSmo = a_SGV->f_cn_1kHrSmo; 

      sr_SGV[i].f_cn_LitSmoPerCent   = a_SGV->f_cn_LitSmoPerCent  ;
      sr_SGV[i].f_cn_1HrSmoPerCent   = a_SGV->f_cn_1HrSmoPerCent  ;
      sr_SGV[i].f_cn_10HrSmoPerCent  = a_SGV->f_cn_10HrSmoPerCent ;
      sr_SGV[i].f_cn_100HrSmoPerCent = a_SGV->f_cn_100HrSmoPerCent;
      sr_SGV[i].f_cn_1kSmoPerCent  = a_SGV->f_cn_1kSmoPerCent   ;


    return 1; } }

   return 0;
}

/****************************************************************/
int SGV_Get (int iX, d_SGV *a_SGV)
{
  if ( iX >= eC_SGV )
    return 0; 

  memcpy (a_SGV, &sr_SGV[iX], sizeof (d_SGV)); 
  return 1; 
  
}

/***********************************************************************
* Name: SGV_BurnOutTime
* Desc: Determine when the specified fuel component has stopped
*       burning
* Note-1:  When there is no 1 hour fuels and the user does an Emission 
*          report it shows a burnout time (75 sec) for 1 hour fuels and 
*          that is because we always send a tiny bit of 1 hour fuels
*          into Burnup. Rather than going into the burnup code to 
*          try and deal with this I decided to just check here and if
*          it's a tiny bit of consumed than just consider it 0, it seems
*          to have solved this particular problem, hopefull it won't cause
*          another.
*          Burnup - I can't remember why we always send a tiny bit of 
*          1 Hr fuel into burnup, but it solved some problem of the fire
*          not igniting, crashing or something, and it was done early on
*          in developement.
************************************************************************/
float  SGV_BurnOutTime (char cr_Cmp[])
{
int i, n;
float f_Con, f_Sec, f_SmoPercent; 

   for ( i = 0; i < eC_SGV; i++ ) {   /* Find end of list */ 
     if ( sr_SGV[i].f_Sec < 0 ) 
	   break; }
   n = i - 1;

   n--;
   for ( i = n; i >= 0; i-- ) {
     f_Con = _Con (&sr_SGV[i], cr_Cmp, &f_SmoPercent);

/* Change 11-30-2015 See Note-1 above ......*/
     if ( !xstrcmpi (cr_Cmp, "1Hr") )
       if ( f_Con < 0.00001 )
         f_Con = 0; 
/* ........................................ */

     if ( f_Con == 0  ) 
       continue; 
     f_Sec = sr_SGV[i].f_Sec;
     break; }

   return f_Sec; 

}


/**********************************************************
* Get the Consumed amount for the specifed
*  fuel component
**********************************************************/
float _Con (d_SGV *a, char cr_Cmp[], float *af_SmoPercent )
{
float f;

  if ( !xstrcmpi (cr_Cmp,"Litter") ) {
    f  = a->f_cn_Lit;
    *af_SmoPercent = a->f_cn_LitSmoPerCent; }
 
  else if ( !xstrcmpi (cr_Cmp,"1Hr") ) {
    f  = a->f_cn_1Hr;
   *af_SmoPercent = a->f_cn_1HrSmoPerCent;}

  else if ( !xstrcmpi (cr_Cmp,"10Hr") ) {
    f  = a->f_cn_10Hr;
   *af_SmoPercent = a->f_cn_10HrSmoPerCent;}

  else if ( !xstrcmpi (cr_Cmp, "100Hr") ){
    f = a->f_cn_100Hr;
   *af_SmoPercent = a->f_cn_100HrSmoPerCent; }

  else if ( !xstrcmpi (cr_Cmp, "1kHr") ){
    f = a->f_cn_1kHr;
   *af_SmoPercent = a->f_cn_1kSmoPerCent;}

  else if ( !xstrcmpi (cr_Cmp, "Duff") ){
    f = a->f_DuffCon;
   *af_SmoPercent = 1.0; }

  else if ( !xstrcmpi (cr_Cmp, "HSFB") ){
    f = a->f_HSFBCon;
   *af_SmoPercent = 1.0; }

  else 
    f = - 1.0;    /* logic error */
    
  return f; 
}

/*********************************************************************
* Name: SGV_GetFS
* Desc: Get the Flaming and Smoldering consumed amounts for the 
*       specified fuel component.
*       The SGV Table contains the amounts consumed for each
*        component at each burnup time step,
*   In: cr_Cmp... fuel component, see the _Con() function above
*       f_FI..... 

*********************************************************************/
int SGV_GetFS(char cr_Cmp[], float *af_Fla, float *af_Smo)
{
int i; 
float f,g, s, f_Out, fN_Fla, fN_Smo, f_SmoPercent; 



  fN_Fla = fN_Smo = 0; 
  for ( i = 0; i < eC_SGV; i++ ) {      /* Find end of list */ 
    if ( sr_SGV[i].f_Sec < 0 ) 
	   break;                           /* end of table */
    f = _Con (&sr_SGV[i],cr_Cmp, &f_SmoPercent);     /* consumed amount */
    if ( !xstrcmpi (cr_Cmp, "Duff") ) { /* Duff always smolders */ 
      fN_Smo += f;
      continue; }
    if ( !xstrcmpi (cr_Cmp, "HSFB") ) {  /* Herb Shr Fol Bra always flames */ 
      fN_Fla += f;
      continue; }
    g = f * f_SmoPercent;                /* How much went up in smoldering */ 
    fN_Smo += g;            
    fN_Fla +=  f - g;     
  }  /* for i */ 

  f = KgSq_To_TPA  (fN_Fla);    /* Kilograms sq/mt to Ton per Acre */
  s = KgSq_To_TPA  (fN_Smo); 

   *af_Fla = f;
   *af_Smo = s;

  return 1; 
}

/**************************************************************
* Name: SGV_FlaSmoDuf
* Desc: For a single time step get the consumed fuel loads 
*       for flaming, smolder and duff
*       the consumed fuel and the percent smoldering are 
*       stored for every time step in burnups emission 
*       functions  
****************************************************************/
int SGV_FlaSmoDuf ( d_SGV *a, float *af_Fla, float *af_Smo, float *af_Duf)
{
float f, f_SmoPC; 
float fN_Smo, fN_Fla, fN_Duf;

  fN_Smo = fN_Fla = fN_Duf = 0; 

  f =  _Con (a,"Litter",&f_SmoPC); 
  fN_Smo += f * f_SmoPC;
  fN_Fla += f * ( 1.0 - f_SmoPC ); 

  f =  _Con (a,"1Hr",&f_SmoPC); 
  fN_Smo += f * f_SmoPC;
  fN_Fla += f * ( 1.0 - f_SmoPC ); 

  f =  _Con (a,"10Hr",&f_SmoPC); 
  fN_Smo += f * f_SmoPC;
  fN_Fla += f * ( 1.0 - f_SmoPC ); 

  f =  _Con (a,"100Hr",&f_SmoPC); 
  fN_Smo += f * f_SmoPC;
  fN_Fla += f * ( 1.0 - f_SmoPC ); 

  f =  _Con (a,"1kHr",&f_SmoPC); 
  fN_Smo += f * f_SmoPC;
  fN_Fla += f * ( 1.0 - f_SmoPC ); 

  *af_Smo = fN_Smo; 
  *af_Fla = fN_Fla + a->f_HSFBCon ; /* Herb,shr,fol,bra always go up flaming */
  *af_Duf = a->f_DuffCon;           /* duff is always smolder */

  return 1; 
}