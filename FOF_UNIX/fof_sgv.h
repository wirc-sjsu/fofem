extern "C" {
#pragma once 

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/* NOTE 1500 should be more than enough, this table gets loaded from calls   */
/*  in the burnup code, entry is made for each 15 second time, so lets the   */
/*  table hold enough entries to do (1500 / 15 sec) minutes which should be  */
/*  more than enough, if not the will just tell call there are no more       */
/*  so it will end gracefully                                                */
#define   eC_SGV 1500
        
float  GramSqMt_To_Pounds  (float f_Gram);

typedef   struct {
    float f_Sec;        /* Emission are in g/sq m, Gram per Square meter */
    float f_Amt;
    float f_PM2_5;
    float f_PM10;
    float f_CH4;
    float f_CO2;
    float f_CO;
    float f_NOX;
    float f_SO2;
    float f_Inten;       /* Kilowatts per square meter */

/* Consumed amounts, that correspond with Emis Factor Groups */ 
    float f_FlaCon;        /* Fuel Amounts are Kg/Sq m, Killograms per square meter */
    float f_SmoCon;
    float f_DuffCon;
    float f_HSFBCon;         /* Herb Shr Fol Bra */

/* Remaining fuel load at time step */
    float f_rm_Lit;         /* Kg/Sq m */
    float f_rm_1Hr;
    float f_rm_10Hr;
    float f_rm_100Hr;
    float f_rm_1kHr;

/* Consumed fuel load at time step */
    float f_cn_Lit;
    float f_cn_1Hr;
    float f_cn_10Hr;
    float f_cn_100Hr;
    float f_cn_1kHr;   /* combined flaming and smoldering */

/* Consumed amounts of 1000 hr for flaming and smoldering - see f_cn_1kHr above */
    float f_cn_1kHrFla;
    float f_cn_1kHrSmo; 

/* percent of fuel consumed in smoldering */
    float f_pcSmo; 


/* percent of the above Consumed Fuel for this time step that was smodering */
/* Lit, 1Hr, 10Hr, 100Hr  will always be 100 percent smolder or 0 for flame */
/* 1kHr can be 0 -> 100 because 1kHr has sub compoents 3,6,9,20 - Snd * Rot */
/*   so some may go up in flame and other in smolder  */
   float f_cn_LitSmoPerCent;
   float f_cn_1HrSmoPerCent;
   float f_cn_10HrSmoPerCent;
   float f_cn_100HrSmoPerCent;   
   float f_cn_1kSmoPerCent; 


    } d_SGV ;



#define e_SGV_PM2_5  "PM 2.5"
#define e_SGV_PM10   "PM 10"
#define e_SGV_CH4    "CH 4"
#define e_SGV_CO2    "CO 2"
#define e_SGV_CO     "CO"

#define e_SGV_NOX    "NOX"
#define e_SGV_SO2    "SO2"

#define e_SGV_Inten  "Intensity"

int   SGV_Init  (void);
int   SGV_GetTimPPA (int iX, float *af_Time,  float *af_Amt);
int   SGV_Set  (d_SGV  *a_SGV);
float SGV_Prepare (char cr_Name[], float *af_MaxTim, float *af_MaxAmt);
int SGV_Get (int iX, d_SGV *a_SGV);

float  SGV_BurnOutTime (char cr_Cmp[]);

float _Con (d_SGV *a, char cr_Cmp[], float *af_SmoPercent);


int SGV_GetFS(char cr_Cmp[], float *af_Fla, float *af_Smo);

int SGV_FlaSmoDuf ( d_SGV *a, float *af_Fla, float *af_Smo, float *af_Duf);
}