
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_msr.c
* Desc: function to do Mortality Stand Report
*       When I changed this code over to use the new Mortality Calc code
*        (uses d_MI d_MO structs) I didn't use the output values in the
*        d_MO, I just used the 'probablity' retrun value. Using the d_MO
*        values would have meant really re-vamping this code so I left it
*        alone.
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "Wnd_Mai.h"
#include "wnd_util.h"
#include  "fof_util.h"
#include "cvt_cvt.h" 
#include  "fof_smt.h"
// #include  "fof_iss.h"
#include  "fof_mrt.h"
#include  "fof_slk.h"
#include  "fof_st.h"
#include  "fof_mt.h"
#include  "fof_mis.h"

#define  e_RepSep "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"

FILE *fh;                               /* Output Report File Handle         */

float  gf_ScoHgt;

void Load_SLK (void);
int  MI_ChkRelHum (char cr_ErrMes[]);
void Warning_Window (char a[], char b[]);
float  MRT_CalcMngr  (d_MIS *a_MIS, d_MO *a_MO, char cr_ErrMes[]);


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: Load_SLK
* Desc: Put the number of live and dead trees for each user selected
*        species into the SLK(Specie Live/Kill) Table, the SLK Table is
*        needed to put totals on the Mortality Report
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void Wnd_Mai::Load_SLK ()
{
int i,  i_Live, i_Killed;
float f_Sch,  f_Prob;
char  cr_ErrMes[300];
d_MIS s_MIS; 
d_MO s_MO;

   MO_Init(&s_MO);                   
   SLK_Init ();                           /* init Spe Live-Kill tot tbl   */

   for ( i = 0; i < 1000; i++ ) {         /* each spe selected by user    */
     if ( !MIS_Get (i, &s_MIS))
        break;
     f_Prob = MRT_CalcMngr  (&s_MIS, &s_MO, cr_ErrMes);
     if ( strcmp (cr_ErrMes,"") )
       myMessageBox (cr_ErrMes);

     i_Killed = (int) s_MO.f_Killed;   /* get # trees killed      */
     i_Live =  (int) s_MIS.f_Den - i_Killed;           /* # of live               */
     SLK_Put (s_MIS.cr_Spe, s_MIS.f_DBH,i_Live,i_Killed); /* Save totals      */
//     ISS_SetKill (i_Killed, i);
   }  /* for i */

}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: MSR_Avg_Mort
* Desc:  Do 'AVERAGE MORTALITY PROBS' section of report.
* Note-1:This function works for 2 foot Flame lengths of 10 foot Scorch
*       heights.
* Note-2: This array is used so that [2] gets totals for Flame len 2,
*         [4] for 4, and so, Scorch is [10] get Scorch Height 10,
*         [20] gets Height 20 totals, etc, so this array has more elements
*         than it needs but it make the logic easy.
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Wnd_Mai::MSR_Avg_Mort ()
{
int i,k,  i_Inc, i_Lst;
float f_Sch, f_Prob, f_Flame, f_AvgCnt, f, f_ToTDBH;
char cr_ErrMes[300], cr[300]; 
d_MO s_MO;
d_MIS s_MIS; 

#define eC_FlmLen 100                        /* see Note-2 above             */
#define eC_Avg eC_FlmLen
float  fr_AvgTot[eC_FlmLen+1];

   _rep (  "\n\n\n");
//   _rep (  "-------------------------------------------------------------------------------\n");


/***********************************************************************************/
/* Flame/Scorch Height type equations report  */
   i = MIS_EquTyp(e_CroSco);          /* are there any Crown Scorch equ typ */ 
   if ( i == 0 )                            /*  in GUI species grid */
     goto CrnDam;                                /* No */

   MO_Init (&s_MO);

   f_ToTDBH = 0;
   f_AvgCnt = 0;                             /* for taking averages          */
   for  ( i = 0; i <= eC_FlmLen; i++ )       /* init for Avg Totals          */
     fr_AvgTot[i] = 0;

   MIS_Get(0, &s_MIS);  /* get a record, tells us if GUI is 'Flame' or 'Scorch' is set */

   if ( !stricmp(s_MIS.cr_FS,"F")) {                   /* Fla Len was set by Usr      */
     i_Inc = 2;
     i_Lst = 20;
     _rep (  "     AVERAGE MORTALITY PROBS BY FLAME LENGTH BY SPECIES/DIAMETER ENTRY\n");
//     _rep (  "\n");
     _rep (  " Species   Tree                        Flame Length (feet)\n");
     _rep (  " Code      DBH        2     4     6     8    10    12    14    16    18    20\n"); }

   else {
     i_Inc = 10;
     i_Lst = 100;
     _rep (  "     AVERAGE MORTALITY PROBS BY SCORCH HEIGHT BY SPECIES/DIAMETER ENTRY\n");
 //    _rep (  "\n");
     _rep (  " Species   Tree                      Scorch Height (feet)\n");
     _rep (  " Code      DBH       10    20    30    40    50    60    70    80    90   100\n"); }
     _rep (  " _____________________________________________________________________________\n");

/*...........................................................................*/
   for ( i = 0; i < 1000; i++ ) {            /* each spe selected by user    */
     if ( !MIS_Get(i,&s_MIS) )
        break; 
     if ( stricmp (s_MIS.cr_EquTyp, e_CroSco)) /* if not a Crown Scorch typ equ */
        continue;                              /*  we don't want to use it   */
     sprintf (cr,  " %-7s %5.1f    ", s_MIS.cr_Spe, s_MIS.f_DBH);
     _rep (cr);  

     f_AvgCnt++;                             /* Have something to average    */
     f_ToTDBH += s_MIS.f_DBH;

     for (k = i_Inc; k <= i_Lst; k = k + i_Inc){ /* see Note-1 above       */ 
        s_MIS.f_FS = k; 
        f_Prob = MRT_CalcMngr (&s_MIS, &s_MO, cr_ErrMes); 
        if ( strcmp (cr_ErrMes,"") )
         Warning_Window (cr_ErrMes,"MSR_Avg_Mort");

         sprintf (cr,  "  %4.2f", f_Prob);
         _rep (cr); 
         fr_AvgTot[k] += f_Prob; }      /* Total for averaging        */
     _rep (  "\n");
    } /* for i */ 

/*...........................................................................*/
/* Do Total Average                                                          *//* Do Total Average                                                          */
    _rep (  " _____________________________________________________________________________\n");
    if ( f_AvgCnt != 0 )                     /* Dont wanna div by 0          */
      f =  f_ToTDBH  / f_AvgCnt;
    else
      f = 0;
    sprintf (cr,  " AVERAGES %4.1f     ", f);
    _rep (cr); 

    for ( k = i_Inc; k <= i_Lst; k = k + i_Inc ) {
      if ( f_AvgCnt != 0 )                   /* Dont wanna div by 0          */
         f = ( fr_AvgTot[k] / f_AvgCnt ) ;
      else
         f = 0;
      sprintf (cr,  " %4.2f ", f); 
      _rep(cr); }


/**********************************************************************************/
/*              Crown Scorch Damage equ types Report          */
CrnDam:
//float f_ToTDBH; 
char A[100]; 

   i = MIS_EquTyp(e_CroDam);          /* are there any Crown Damage equ types */ 
   if ( i == 0 )                            /*  in GUI species grid */
     goto BolCha;                           /* No */

   f_ToTDBH = 0;
   f_AvgCnt = 0;                            /* for taking averages          */
   for  ( i = 0; i <= eC_Avg; i++ )         /* init for Avg Totals          */
     fr_AvgTot[i] = 0;

   _rep  (  "\n\n");

   i_Inc = 10;
   i_Lst = 100;
   _rep  (  "\n     AVERAGE MORTALITY PROBS BY CROWN SCORCH PERCENT BY SPECIES/DIAMETER ENTRY\n");
//   _rep  (  "\n");
   _rep  (  " Species   DBH  CKR  Btle                  Crown Scorch Percent\n");
   _rep  (  " Code                Dam    10    20    30    40    50    60    70    80    90   100\n");
   _rep  (  " ____________________________________________________________________________________\n");

   for ( i = 0; i < 1000; i++ ) {            /* each spe selected by user    */
     if ( !MIS_Get(i,&s_MIS) ) 
       break;
     if ( stricmp (s_MIS.cr_EquTyp,e_CroDam) ) /* only want Crown Damge Equ Type records */
        continue; 

     if ( s_MIS.f_CKR == e_PFI_Miss )
       sprintf (A,"%s",e_NA);
     else
       sprintf (A,"%1.0f",s_MIS.f_CKR);

     sprintf (cr,  " %-6s    %4.1f  %s    %s  ", s_MIS.cr_Spe, s_MIS.f_DBH, A, s_MIS.cr_BeeDam);
     _rep (cr);

     f_AvgCnt++;                             /* Have something to average    */
     f_ToTDBH += (int)s_MIS.f_DBH;

     for (k = i_Inc; k <= i_Lst; k = k + i_Inc){
        s_MIS.f_CrnDam = (float) k;
        f = MRT_CalcMngr(&s_MIS,&s_MO,cr_ErrMes); 
        sprintf (cr,  "  %4.2f", s_MIS.f_Prop);
        _rep (cr);
        fr_AvgTot[k] += f;
     }
     _rep (  "\n");
   }


/* Crown Scorch Damage Total Average                         */
    _rep (  " ____________________________________________________________________________________\n");
    if ( f_AvgCnt != 0 )                     /* Dont wanna div by 0          */
      f =  f_ToTDBH  / f_AvgCnt;
    else
      f = 0;
    sprintf (cr,  " AVERAGES  %3.1f           ", f);
    _rep (cr); 

    for ( k = i_Inc; k <= i_Lst; k = k + i_Inc ) {
      if ( f_AvgCnt != 0 )                   /* Dont wanna div by 0          */
         f = ( fr_AvgTot[k] / f_AvgCnt ) ;
      else
         f = 0;
      sprintf (cr,  " %4.2f ", f);
      _rep (cr);  } 


/*************************************************************************************/
/*   Bole Char heights type equations  */
BolCha:
   i = MIS_EquTyp(e_BolCha);          /* are there any Crown Damage equ types */ 
   if ( i == 0 )                            /*  in GUI species grid */
     goto Done;                           /* No */
   
   f_ToTDBH = 0;
   f_AvgCnt = 0;                            /* for taking averages          */
   for  ( i = 0; i <= eC_Avg; i++ )         /* init for Avg Totals          */
     fr_AvgTot[i] = 0;

   _rep  (  "\n\n\n");
   i_Inc = 1;
   i_Lst = 12;
   _rep  (  "       AVERAGE MORTALITY PROBS BY BOLE CHAR HEIGHT BY SPECIES/DIAMETER ENTRY\n");
//   _rep  (  "\n");
   _rep  (  " Species   DBH                          Bole Char Height\n");
   _rep  (  " Code             1     2     3     4     5     6     7     8     9    10    11    12\n");
   _rep  (  " _____________________________________________________________________________________\n");

   for ( i = 0; i < 1000; i++ ) {            /* each spe selected by user    */
     if ( !MIS_Get(i,&s_MIS) ) 
       break;
     if ( stricmp (s_MIS.cr_EquTyp,e_BolCha) ) /* only want Bole Char  Equ Type records */
        continue; 
     sprintf (cr,  " %-6s   %4.1f", s_MIS.cr_Spe, s_MIS.f_DBH);
     _rep (cr);
     f_AvgCnt++;                             /* Have something to average    */
     f_ToTDBH += (int)s_MIS.f_DBH;

     for (k = i_Inc; k <= i_Lst; k = k + i_Inc){
        s_MIS.f_BolCha = (float) k;
        f = MRT_CalcMngr(&s_MIS,&s_MO,cr_ErrMes); 
        sprintf (cr,  "  %4.2f", s_MIS.f_Prop);
        _rep (cr);
        fr_AvgTot[k] += f;
     }
     _rep (  "\n");

// test
  } /* for i */


/* Bole Char - Do Total Average                                                          *//* Do Total Average                                                          */
    _rep (  " _____________________________________________________________________________________\n");
    if ( f_AvgCnt != 0 )                     /* Dont wanna div by 0          */
      f =  f_ToTDBH  / f_AvgCnt;
    else
      f = 0;
    sprintf (cr,  " AVERAGES %4.1f ", f);
    _rep (cr); 

    for ( k = i_Inc; k <= i_Lst; k = k + i_Inc ) {
      if ( f_AvgCnt != 0 )                   /* Dont wanna div by 0          */
         f = ( fr_AvgTot[k] / f_AvgCnt ) ;
      else
         f = 0;
      sprintf (cr,  " %4.2f ", f); 
      _rep(cr); 
    }  /* for k */ 
    _rep("\n"); 

//  }  /* for i */ 



Done:
   return;
}



/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: MSR_PostFire_Killed
* Desc: Do the sections of report for:
*
*     see the code below, I changed this title
*    -  'STAND DENSITY (TREES/ACRE) OF TREES KILLED BY THE FIRE' trees killed
*
*    -  'POSTFIRE STAND DENSITY (TREES/ACRE)'  trees remaining alive
*   In: i_SizGrp.....1 for 2 -> 20,  2 for 22 -> 40
*       cr_Mode......"KILLED" do KILLED else do the other
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  Wnd_Mai::MSR_PostFire_Killed  (int i_SizGrp, char cr_Mode[])
{
int i,j, A, B, i_Amt;
char cr_Spe[20], cr[500];
#define  eC_Tot 41
int ir_Tot[eC_Tot];

   for ( i = 0; i < eC_Tot; i++ )
    ir_Tot[i] = 0;

   _rep (  "\n");
   _rep (  "\n\n");

   if ( !stricmp (cr_Mode,"KILLED") )
//    printf (  "              STAND DENSITY (TREES/ACRE) OF TREES KILLED BY THE FIRE\n");
// Changed 12/26/01
      _rep (  "                         TREES PER ACRE KILLED BY THE FIRE\n");
   else
      _rep (  "                         POSTFIRE STAND DENSITY (TREES/ACRE)\n");

   _rep (  "\n");
   _rep (  " Species               Midpoint Diameter classes (in)\n");
   if ( i_SizGrp == 1 ) {
      A = 1; B = 19;
      _rep (  " Code            1     3     5     7     9    11    13    15    17    19\n"); }
   else {
      A = 21; B = 39;
      _rep (  " Code           21    23    25    27    29    31    33    35    37   39+\n"); }

   _rep(  "__________________________________________________________________________\n");

   for ( i = 0; i < 1000; i++ ) {            /* each spe selected by user    */
     SLK_GetSpe (i, cr_Spe);                 /* get a struct                 */
     if ( !strcmp (cr_Spe,"" ) )             /* last one                     */
        break;                               /*  all done                    */
     sprintf (cr,  " %-7s    ", cr_Spe);           /* species                      */
     _rep (cr); 
     for ( j = A; j <= B; j += 2 ) {         /* each density                 */
       if ( !stricmp (cr_Mode,"KILLED") )
         i_Amt = SLK_GetLK (i,j,'K');

       if ( !stricmp (cr_Mode,"LIVE") )
         i_Amt = SLK_GetLK (i,j,'L');

        sprintf (cr,  "  %4d", i_Amt );
        _rep (cr); 
        ir_Tot[j] += i_Amt;

     } /* for j end */
     _rep (  "\n");
   } /* for i end */

   _rep (  "__________________________________________________________________________\n");
   _rep (  " TOTALS     ");
   for ( j = A; j <= B; j += 2 ) {          /* each density                 */
     sprintf (cr,   "  %4d", ir_Tot[j]);
     _rep (cr); 
     }
   _rep (  "\n\n");
   _rep (  " DBH classes (in):  1: 0-<2,  3: 2-<4,  5: 4-<6, ... 39: 38 and over\n");
   return 1;
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: MSR_Density
* Desc: Mortality Stand Report, do density section of report
*       Will do either Size Class Group
*   In: i_SizGrp.....1 for 2 -> 20,  2 for 22 -> 40
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Wnd_Mai::MSR_Density (int i_SizGrp)
{
int i, j,  A, B, i_Density;
#define eC_Tot 41
int ir_Tot[eC_Tot];
char cr_Spe[20], cr[500];

   for ( i = 0; i < eC_Tot; i++ )
    ir_Tot[i] = 0;
   _rep  (   "\n\n");
   _rep  (   "                      ORIGINAL STAND DENSITY AS INPUT TO FOFEM\n");
   _rep (   "\n");
   _rep (   " Species              Midpoint Diameter classes (in)\n");

   if ( i_SizGrp == 1 ) {
      A = 1; B = 19;
      _rep (  " Code            1     3     5     7     9    11    13    15    17    19\n"); }
   else {
      A = 21; B = 39;
      _rep (  " Code           21    23    25    27    29    31    33    35    37   39+\n"); }

   _rep (  " ________________________________________________________________________\n");

   for ( i = 0; i < 1000; i++ ) {            /* each spe selected by user    */
     SLK_GetSpe (i, cr_Spe);                 /* get a struct                 */
     if ( !stricmp (cr_Spe,"" ) )             /* last one                     */
        break;                               /*  all done                    */
     sprintf (cr,  " %-7s    ", cr_Spe);         /* species                      */
     _rep (cr); 
     for ( j = A; j <= B; j += 2 ) {         /* each density                 */
       i_Density = SLK_GetDen (i, j);
       sprintf (cr,  "  %4d", i_Density);
       _rep(cr); 
       ir_Tot[j] += i_Density;              /* get total orignal density    */ 
// ZZZ       gi_TotOrg += i_Density;
//       if ( j >= 4 )                        /* # Original trees over 4 inch DBH */
//         gi_TotOrg4 += i_Density;
     }           
     _rep (  "\n"); }

     _rep (  " ________________________________________________________________________\n");
     _rep (  " TOTALS     ");
     for ( j = A; j <= B; j += 2 ) {         /* each density                 */
       sprintf (cr,  "  %4d", ir_Tot[j]); 
       _rep (cr); 
     }
     
     _rep (  "\n");

   _rep (  "\n");
   _rep (  " DBH classes (in):  1: 0-<2,  3: 2-<4,  5: 4-<6, ... 39: 38 and over\n");

}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: MSR_Mort_Indexes
* Desc: Do the Mortality Indexs portions of Mortality Report
* Note-1: When we put the basal area to the report, these are totals
*         from all the trees user selected, we show total prefire area
*         and postfire area for what was consumed by fire and amount of
*         basal area remaining. We use that divide by 144 to get from
*         square feet to square inches.
*         We calculate the percent consumed using the totals, and then
*         subtract from 100 to get the remaining percent which is the
*         basal area that was not consumed.
 
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Wnd_Mai::MSR_Mort_Indexes()
{
int i,j,k, i_PerConBA, i_PerRemBA, i_TotKil;
float f,g,h,pc;
float f_Den, f_Live, f_Kill;
float f_CrCoTo, f_CrCoLi, f_CrCoKi;
char cr[400], A[40]; 
d_MT  s_MT;

  _rep (  "\n");
  _rep (  "\n");
  _rep (  "\n");
  _rep (  " STAND TREE MORTALITY\n");

   MIS_Totals (&f_Den, &f_Live, &f_Kill, 0 );
   sprintf (cr,   " Total prefire number of trees: %5.0f \n", f_Den );
  _rep (cr);

   sprintf (cr,  " Number of trees killed by the fire: %5.0f\n", f_Kill);
  _rep (cr); 

   pc = 0;
   if ( f_Den > 0 )                    /* should always have some tree density */ 
     pc = ( f_Kill / f_Den ) * 100.0; 
   sprintf (cr,  " Percent mortality: %3.0f \n",pc);
  _rep (cr); 

   s_MT.i_PerMor = (int) pc;                         /* will need to save tot later  */

   MIS_Totals (&f_Den, &f_Live, &f_Kill, 4.0 );       /* Trees over 4 inch DBH */
   if ( f_Den <= 0 ) 
     pc = 0;
   else 
     pc = ( f_Kill / f_Den ) * 100.0; 
   sprintf (cr,  " Percent mortality for trees 4+ in DBH: %3.0f \n", pc);
  _rep (cr); 

 //  f =  ISS_AvgDBHKill (); 
   f = MIS_AvgDBHKill (); 
   sprintf (cr,  " Average tree diameter (DBH) of fire killed trees:%6.1f \n\n",f);
  _rep (cr); 

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* Basal Area Totals                                                         */
/* see  Note-1 above in func heading                                         */
  _rep ("  Stand Basal Area:   sq/ft   Percent\n");
  MIS_BasalArea (&f_Den, &f_Live, &f_Kill); 
  sprintf (cr, "      Prefire Live:   %6.2f    %3d\n",f_Den,100);  /* always 100 %    */
  _rep (cr); 

  if ( f_Den <= 0 )
     pc = 0; 
  else 
     pc = ( f_Live / f_Den ) * 100.0; 

  sprintf (cr, "     Postfire Live:   %6.2f    %3.0f\n",f_Live, pc);
  _rep (cr); 

  if ( f_Den <= 0 ) 
     pc = 0; 
  else
    pc = ( f_Kill / f_Den ) * 100.0;
  sprintf (cr, "   Postfire Killed:   %6.2f    %3.0f\n",f_Kill, pc);
  _rep(cr); 


  s_MT.i_PerConBA = (int) (pc + 0.5); 

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/* Canopy/Crown Cover percents                                               */
  MIS_CrownCover (&f_CrCoTo, &f_CrCoLi, &f_CrCoKi, A );


  _rep ("\n\n");
  _rep ("   Stand Canopy Cover:    Percent ( only records containing height are used )\n");
  sprintf (cr, "    Prefire Live:           %3.0f   %s \n",f_CrCoTo,A);
  _rep (cr); 
  sprintf (cr, "   Postfire Live:           %3.0f   %s \n",f_CrCoLi,A);
  _rep (cr); 
  sprintf (cr, "          Change:           %3.0f      \n",f_CrCoKi);
  _rep (cr); 

  s_MT.i_PreLivCC = f_CrCoTo;                        /* Save for Totals Report       */
  s_MT.i_PosLivCC = f_CrCoLi;

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/* If Total Radio Button is checked, save it                                 */
float f_LH; 
char cr_ErrMes[300]; 
/* Shouldn't get an error, should have checked previously */
  if ( !_Get_TBFlo (this->_txFlaSco, &f_LH, cr, "Flame Len - Scorch Hgt", cr_ErrMes)) {
  //   myMessageBox ("Invalid Flame Length - Scorch Height"); 
     f_LH = 5.0; }  /* User something so we can continue */

//  if ( this->_chRepTot->Checked ) {              /* if Totals Radio But is Chkd  */
//    _GetTextBox(this->_txRepTot,s_MT.cr_TotNam); /* get the Total Name from Wnd  */

  _GetTextBox(this->_txRepTot,s_MT.cr_TotNam);
   if ( stricmp (s_MT.cr_TotNam,"") ) {      /* if we have a total name */
     if ( CCW_isFlaLen())                      /* User has it set to.......    */
       strcpy (s_MT.cr_LenHei,"fl");           /* Flame Length                 */
     else
       strcpy (s_MT.cr_LenHei,"sh");          /* Scorch Height                */
    s_MT.f_LenHei = f_LH;                     /* Get the Len/Hei              */
    if ( !MT_Save(&s_MT, e_RTpre))            /* Save 'em                     */
      myMessageBox ("Maximum number of Mortality Summaries Exceeded - MSR_Mort_Indexes() ");
   }

}

#ifdef wowowowwo
/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: _MSR_ChkLin
* Desc: Check a single line from the Species Window,
*       Parses the line and validates it, see code below for type
*        checks being done.
*   In: cr_Line....text line from command file
*  Out: cr_ErrMess....error message
*  Ret: 1 OK,   0 Error
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int Wnd_Mai::_MSR_ChkLin(char cr_Line[], char cr_ErrMess[])
{
int i_Reg, iX;
float f;
char cr_Sp[100],cr_tmp[100],cr_Den[30],cr_DBH[30],cr_Hgt[30],cr_CR[30];

   strcpy (cr_ErrMess,"");
   strcpy (cr_Sp,"");
   strcpy (cr_Den,"");
   strcpy (cr_DBH,"");
   strcpy (cr_Hgt,"");
   strcpy (cr_CR,"");
   strcpy (cr_DBH,"");
   sscanf (cr_Line, "%s %s %s %s %s", cr_Sp,cr_Den,cr_DBH,cr_Hgt,cr_CR);
   if ( !stricmp (cr_CR,"" ) ) {
     strcpy (cr_ErrMess,"Missing Value/Item.");
     return 0; }

/* Tree Species.............................                                 */
   strcpy (cr_tmp,cr_Sp);                     /* Validate Species            */
   strupr (cr_tmp);                           /* need to check as upper case */
   iX = SMT_GetIdx (cr_tmp);
   if ( iX == -1 ) {                          /* chk for valid species       */
     sprintf (cr_ErrMess, "Invalid Species--> %s",cr_Sp);
     return 0; }

   i_Reg = CCW_Get_RegionInt();               /* Get Region #                */
   if ( !SMT_ChkRegion (cr_Sp, i_Reg) ) {
     strcpy (cr_ErrMess, "Species is Not in Region.");
     return 0; }

   if ( !SMT_NotImp (iX) ) {
     strcpy (cr_ErrMess, "Species equations Not implemented yet.\n");
     strcat (cr_ErrMess, "Remove species from list before running report.");
     return 0; }

/* Tree Density.............................                                 */
   if ( Get_NumTyp (cr_Den) == 'X' ) {
     strcpy (cr_ErrMess, "Density is Invalid or Missing ");
      return 0; }
   f = atof (cr_Den);
   if ( f < 1 || f > 10000 ) {
     strcpy (cr_ErrMess, "Density is out of range, 1 -> 10000");
     return 0; }

/* Tree DBH.................................                                 */
   if ( Get_NumTyp (cr_DBH) == 'X' ) {
     strcpy (cr_ErrMess, "DBH is Invalid or Missing");
      return 0; }
   f = atof (cr_DBH);
   if ( f < 1 || f > 999 ) {
     strcpy (cr_ErrMess, "DBH is out of range, 1 -> 999");
     return 0; }

/* Tree Height..............................                                 */
   if ( Get_NumTyp (cr_Hgt) == 'X' ){        /* make sure it's a number      */
      strcpy (cr_ErrMess, "Height is Invalid");
      return 0; }
   f = atof (cr_Hgt);
   if ( f < 1 || f > 300 ) {
      strcpy (cr_ErrMess, "Tree Height is out of range. Range is 1 -> 300");
      return 0; }

/* Crown Ratio..............................                                 */
   if ( Get_NumTyp (cr_CR) == 'X' ) {
      strcpy (cr_ErrMess, "Crown Ratio is Invalid");
      return 0; }
   f = atof (cr_CR);                         /* Crown Ratio  not over 100    */
   if (  f > 10 || f < 1 ) {
       strcpy (cr_ErrMess, "Crown Ratio is out of range. Range is 1 > 10");
       return 0; }
   return 1;
}
#endif 


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: MSR_Summary
* Desc: Put the Mortality Summary to the Report.
*       There are some values that have been save with their id names
*       everytime a mort report was created, here we put all the save
*       names and values to the report.
* Note-1: This will help position the Caption over the Summary Id-Name
*         The 'eC_TotNam' define is the size of of the Id-Name and is
*         also used to in the printf formating
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Wnd_Mai::MSR_Summary()
{
int i,j,k;
char cr[500];
d_MT s_MT;

  j = eC_TotNam - 5;                          /* see Note-1                  */
  k = j / 2;
  _rep ("\n\n");
  _rep (" Report Totals:\n");
  sprintf (cr, "%*c                          Mortality Summary\n\n",j,' ');
  _rep (cr); 

  sprintf (cr, "%*cName    Flame Length    Percent   Stand Basal Area   Stand Canopy Cover\n",j,' ');
  _rep (cr); 
  sprintf (cr, "%*c Id     Scorch Height  Mortality   Percent Killed    Prefire   Postfire  \n",j,' ');
  _rep (cr); 

  for ( i = 0; i < eC_Totals; i++ ) {
    MT_Get (i, &s_MT,e_RTpre);
    if ( !stricmp (s_MT.cr_TotNam,"") )
       break;

    sprintf (cr, "%*s   %6.1f %2s         %3d           %3d           %3d        %3d\n",
      eC_TotNam, s_MT.cr_TotNam,
       s_MT.f_LenHei,
       s_MT.cr_LenHei,
       s_MT.i_PerMor,
       s_MT.i_PerConBA,
       s_MT.i_PreLivCC,
       s_MT.i_PosLivCC);
    _rep (cr); 
  }
}


/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: MI_SetSFCP
* Desc: Set the Scorch Height and Fire Severity
*       Set the Coastal Plain and Relative humidity
* NOTE: it MUST be Scorch height you send in here
* NOTE: Check the Coastal Plain and Relative humidity before come here
*         see MI_ChkRelHum ()
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
void  Wnd_Mai::MI_SetSFCP  (float f_Sch, d_MI *a_MI)
{
char cr[40];

  a_MI->f_FS = f_Sch;                   /* Scorch Height                */
  strcpy (a_MI->cr_FS,"S");             /* It's Scorch Height           */
  if ( this->_chLowFireSev->Checked )   /* Fire Severity                */
    strcpy (a_MI->cr_FirSev,"Low");
   else
    strcpy (a_MI->cr_FirSev,"");

/* Get the Coastal Plain and Rel Hum inputs from screen                      */
  strcpy (a_MI->cr_CP,"N");             /* Assuem Not a coastal plain        */
  a_MI->f_RelHum = e_RHNonen;           /* and no rel humidity               */

  if ( CCW_isCoaPlaY() ) {              /* Is Coastal Plain set              */
    strcpy (a_MI->cr_CP,"Y");           /*  yes                              */
// not doing coast plane so don't have or care about Relative humidty 
 //   CCW_GetWndTxt ("RelHum",cr);    /* Get the relative humidity         */
    if ( isBlank(cr) )                  /* Blank is ok means user doesn't    */
      a_MI->f_RelHum = e_RHNonen;       /*  have or know it                  */
    else
      a_MI->f_RelHum = atof(cr);  }     /* we'll use it if we have it        */
}

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: MI_ChkRelHum
* Desc: Check for a valid Relative Humidity value in user wnd box,
*        only check it if the Coastal Plain button is 'Y'
*  Ret: 1 ok, else 0
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/
int  Wnd_Mai::MI_ChkRelHum (char cr_ErrMes[])
{
float f;
char  c, cr[40];

  strcpy (cr_ErrMes,"");
  if ( !CCW_isCoaPlaY() )               /* if not Coastal Plain we don't     */
    return 1;                           /* care about Relative Humidity      */

// not doing Coastal Plane so don't have or care about this */
 // CCW_GetWndTxt ("RelHum",cr);      /* Get Rel Hum from screen           */

  if (  isBlank (cr))                   /* None/Blank is ok, means user      */
    goto Ret;                           /*  doesn't know what to use         */

  c =  Get_NumTyp (cr);                 /* Make sure it's a valid number     */
  if ( c == 'X' )
     goto Err;

  f = atof(cr);                         /* To float and chk limits           */
  if ( f < e_RHLow || f > e_RHHigh )
     goto Err;

Ret:
  return 1;

Err:
   sprintf (cr_ErrMes, "Invalid Relative Humidity: Use (%3.0f -> %3.0f)",
           e_RHLow,  e_RHHigh  );
   return 0;
}

/************************************************************
*
************************************************************/
void Warning_Window (char a[], char b[]) 
{ 
char cr[1000];
  sprintf (cr,"%s\n%s",a,b);
  myMessageBox (cr); 
}

/*********************************************************
* See if the Mortality Flame Length checkbox is checked
**********************************************************/
int   Wnd_Mai::CCW_isFlaLen()
{ 
 if ( this->_rbFlameLen->Checked )
   return 1;   /* Flame Len is checked */ 
return 0; 
}

/*******************************************************
* For Now - it will never be Coastal Plain
********************************************************/
int   Wnd_Mai::CCW_isCoaPlaY()
{ 
return 0; 
}



