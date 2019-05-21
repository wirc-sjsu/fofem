
#define   e_TPALit  1
#define   e_TPAW0   2
#define   e_TPAW01  3
#define   e_TPAW13  4
#define   e_TPAW3   5

#define   e_TPAHer  6
#define   e_TPAShr  7
#define   e_TPAFol  8
#define   e_TPASmB  9


#define eC_CFLCo  50 

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* The Default Duff Amount to Depth Ration,                                  */
/*  When a user changes the Depth the program will re-calc new Amount based  */
/*   on a ratio it gets from the Cover Type Record, if however there is no   */
/*   duff amount and/or depth we need to use something as a default          */
/* This ratio is saying for every 15 tons of duff there is 1 inch of duff    */ 
#define  e_DufDefRat   10.0

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* To adjust Duff to Light or Heavy in the Duff Amoun Window                 */
#define e_AdjDufLit  (float) 0.5        /* this use to be 0.6                */
#define e_AdjDufHvy  (float) 1.5        /* this use to be 1.4                */


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/*                           Live Adjustments                                */
/* Herb, Shrub, Foliage, Branch - Sparse & Abundent                          */
#define  e_LA_NA   "NA"            /* Not Applicable Code                    */
typedef struct {
   int i_IdAmt;                    /* CCW Id, herb,shrub,foliage,branch      */
   char cr_Code[eC_CFLCo];         /* Its code from Fuel .dat file           */
   float f_SpaAdj;                 /* Sparse                                 */
   float f_AbuAdj;                 /* Abundent                               */
} d_LA;

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/*                           Dead Adjustments                                */
/* Litter, Down Wood, 1/4, 1, 3, 3+                                          */
typedef struct {
   int i_IdAmt;                     /* CCW Id,                              */
   float f_Light;
   float f_Heavy;
} d_DA;

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/*                         Harvesting Debris Loading                         */
#define e_HD_NA  "NA"              /* Not Applicable Code                    */
typedef struct {
   int i_IdAmt;                    /* CCW Id, Lit,Down Wood                  */
   char cr_Code[eC_CFLCo];         /* Its code from Fuel .dat file           */
   float f_Mean;                   /* Mean                                   */
   float f_Heav;                   /* Heavy                                  */
   float f_Ligh;                   /* Light                                  */
} d_HD;

float DA_AdjLight    (int i_IdAmt);
float DA_AdjHeavy    (int i_IdAmt);

float FWF_HDAdj (int i_IdAmt, char cr_Code[], char c_MHL);
int  FWF_LAValid (int i_IdAmt, char cr_Code[]);
float FWF_LAadj (int i_IdAmt,  char cr_Code[], char c_LA );
int  FWF_HDValid (char cr_Code[]);
