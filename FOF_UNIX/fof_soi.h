extern "C" {
/* #define e_mplus1 14     */



void soiltemp_initconsts (float r_bdi, float r_pdi, float r_lsi, float r_gai,
                     float r_xwoi, float r_copi, float r_xoi,
                     float rr_Lay[] );


void   soiltemp_initprofile (float rr_wi[], float rr_ti[]);

float  watercontent (float r_p, float r_xo, float *ar_dwdp);
float  humidity (float r_p, float r_t, float *ar_dhdp);
int   soiltemp_step (float r_Rabs, float r_dt,  int  *ai_success);
float  vaporpressure (float r_tn);
float  tcond (float r_t, float r_xw, float r_xs, float r_ls, float r_ga,
              float r_xwo, float r_cop, float r_p, float r_s, float *ar_enh);
float  Kvap (float r_t, float r_p);
float  Hv (float t);
float  POW (float x, float y);
float  sqr  (float  r);
float  abs_Real (float r);
float  slope  (float r_tn, float r_psat);

void soiltemp_getwater ( float rr_wi[]);
void soiltemp_gettemps ( float rr_ti[]);
void soiltemp_getdepths ( float rr_zi[]);


void  Copy_Array ( float rr_to[], float rr_from[]);
void Display_Array ( float rr[]);

/* void  App_Ext ( char cr_FN[], char  cr_Ext[] ); */

void  TmpFil_Heading (FILE *fh_Out); 
}