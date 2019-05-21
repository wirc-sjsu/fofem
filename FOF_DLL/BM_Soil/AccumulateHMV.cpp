
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"
#include "SCN_Arrays.h"

int gi_LayStart, gi_LayStop; 
int gi_StepStart, gi_StepStop; 

extern double temp[], theta[]; 
extern double psin[], rhov[], Awa[], rhove[];
extern double rhcpv[], Concf[],tcon[],zlamda[],hcon[];  
extern double vdiff[], hvdif[], lv[], stef[];
extern double cs[],rh[],sourcev[],vapres[],xhiv[],u[],delu[],diseq[];
extern double adtcon[],advdif[],Kn[],KH[],Kd[],Km[],dKHdT[],dKHdp[],thpsin[];
extern double thtemp[],dAwadt[],dAwadp[];
extern double sdialc[],ddiaT[],stensn[],dstensdT[],KnF[],KHF[];

extern  double IR,LE,Heat,G0;

extern double difcoef;

extern double nsteps;  /* time step to run to */
extern double nlevel; /* num of layer increments */ 

/*--------------------------------------------------------------*/
typedef struct { 
           double *arr;         /* Address of array */
           char  cr_FN[1000];   /* file name */
           FILE  *fh;           
           double *val;         /* address of a single double */ 
     } d_AFT; 

d_AFT  AFT[] = { { temp,   "temp",  NULL, NULL },
                 { theta,  "theta", NULL, NULL  },
                 { psin,   "psin",  NULL, NULL  }, 
                 { rhov,   "rhov",  NULL, NULL  }, 
                 { Awa,    "Awa",   NULL, NULL  }, 
                 { rhove,  "rhove", NULL, NULL  }, 
                 { rhcpv,  "rhcpv", NULL, NULL  }, 
                 { Concf,  "Concf", NULL, NULL  }, 
                 { tcon,   "tcon",  NULL, NULL  }, 
                 { zlamda, "zlamda",NULL, NULL  }, 
                 { hcon,   "hcon",  NULL, &difcoef }, 
                 { vdiff,  "vdiff", NULL, NULL },    
                 { hvdif,  "hvdif", NULL, &difcoef }, 
                 { lv,     "lv",    NULL, NULL }, 
                 { stef,   "stef",  NULL, NULL }, 
                 { cs,      "cs",      NULL, NULL }, 
                 { rh,      "rh",      NULL, NULL }, 
                 { sourcev, "sourcev", NULL, NULL }, 
                 { vapres,  "vapres",  NULL, NULL }, 
                 { xhiv,    "xhiv",    NULL, NULL }, 
                 { u,       "u",       NULL, NULL }, 
                 { delu,    "delu",    NULL, NULL }, 
                 { diseq,   "diseq",   NULL, NULL }, 

                 { adtcon,    "adtcon", NULL, &difcoef },  
                 { advdif,    "advdif", NULL, &difcoef }, 
                 { Kn,        "Kn",     NULL, NULL },  
                 { KH,        "KH",     NULL, NULL }, 
                 { Kd,        "Kd",     NULL, NULL }, 
                 { Km,        "Km",     NULL, NULL }, 
                 { dKHdT,     "dKHdT",  NULL, NULL }, 
                 { dKHdp,     "dKHdp",  NULL, NULL }, 
                 { thpsin,    "thpsin", NULL, NULL }, 
                 { thtemp,    "thtemp", NULL, NULL }, 
                 { dAwadt,    "dAwadt", NULL, NULL }, 
                 { dAwadp,    "dAwadp", NULL, NULL },   
  
                 { sdialc,    "sdialc",   NULL, NULL },    
                 { ddiaT,     "ddiaT",    NULL, NULL },   
                 { stensn,    "stensn",   NULL, NULL },    
                 { dstensdT,  "dstensdT", NULL, NULL },   
                 { KnF,       "KnF",      NULL, NULL },   
                 { KHF,       "KHF",      NULL, NULL },   

/* Single Variables for output */ 
                 { NULL,        "IR",   NULL, &IR }, 
                 { NULL,        "LE",   NULL, &LE }, 
                 { NULL,      "Heat",   NULL, &Heat }, 
                 { NULL,        "G0",   NULL, &G0 }, 


                 { 0, 0, 0 } } ; 


bool AFT_End (int iX);

/******************************************************************
*
* Not Sure if I'll need i_TimStp 
******************************************************************/
int  AccumulateHMV (int i_TimStp )
{
int i,j;
double f,g; 

/* only want to output the steps in range user wants */ 
   if ( i_TimStp < gi_StepStart )
     return 1; 
   if ( i_TimStp > gi_StepStop ) 
     return 1; 
      

   for ( i = 0; i < 10000; i++ ) {
     if ( AFT_End(i) ) 
       break; 
     if ( AFT[i].fh == NULL ) 
       continue; 
     fprintf (AFT[i].fh, "%4d  ", i_TimStp); 
   
     if ( AFT[i].arr == NULL ) {  /* Non-array values */ 
       fprintf (AFT[i].fh, "%10.8f\n", *AFT[i].val); 
       continue; }

     for ( j = gi_LayStart; j <= gi_LayStop; j++ ) {  /* Array values */
       if ( AFT[i].val == NULL )  
         f =  AFT[i].arr[j];
       else {
         g = *AFT[i].val; 
         f =  AFT[i].arr[j] / g ;  }      
  
       fprintf (AFT[i].fh, "%12.8f ",f);
     } 
     fprintf (AFT[i].fh, "\n"); 
   } 

   return 1; 
} 

/**************************************************************
*
*
*  Ret: 1 ok, 0 error message is in bmi
**************************************************************/
int AFT_Init (d_BMI *bmi) 
{
#ifdef wowowow
int i; 
   char cr_FN[1000]; 
   strcpy (bmi->cr_ErrMes,""); 

/* range of time steps the user wants */
   gi_StepStart = (int) bmi->f_StepStart;
   gi_StepStop  = (int) bmi->f_StepStop; 
   if ( gi_StepStart <= 0 )       /* See Note-1 above */ 
     gi_StepStart = 1; 
   if ( gi_StepStop <= 0 )        
     gi_StepStop = (int) nsteps; 

/* range of layers the user wants */ 
   gi_LayStart = (int) bmi->f_LayStart; 
   gi_LayStop  = (int) bmi->f_LayStop; 
   if ( gi_LayStart <= 0 ) 
     gi_LayStart = 1; 
   if ( gi_LayStop <= 0 ) 
     gi_LayStop = (int) nlevel; 

   for ( i = 0; i < 10000; i++ ) {
     if (  AFT_End (i) ) 
       break; 
     sprintf (cr_FN, "%s-%s.txt", bmi->cr_FNPrefix, AFT[i].cr_FN); 
     AFT[i].fh = fopen (cr_FN,"w"); 
     if ( AFT[i].fh == NULL ) {
       sprintf (bmi->cr_ErrMes, "Can't open %s\n, Is file already open ? \n Does Folder exist ? \n AFT_Init()",cr_FN); 
       AFT_Close(); 
       return 0; }
        
   }
#endif 
  return 1; 
}

/***************************************************************************
* Close the files 
******************************************************************************/
int AFT_Close()
{

int i; 
   
   for ( i = 0; i < 10000; i++ ) {
     if ( AFT_End (i) ) 
       break; 
     if ( AFT[i].fh == NULL ) 
       continue; 
     fclose (AFT[i].fh);
     AFT[i].fh = NULL;  
   }

  return 1; 
} 

/*************************************************************/
bool AFT_End (int iX)
{
  if ( !strcmp ( AFT[iX].cr_FN,"") ) 
    return true;
  return false; 
}

#ifdef wowow
%--------------------------------------------------------------------------
% AccumulateHMV.m
%--------------------------------------------------------------------------
%__________________________________________________________________________
% The first four matrices store the solution after each time step 
% The first row of tempfeN is the initial soil temp profile
% The first row of thetaf is the initial soil moisture profile
% The first row of psinf is the initial profile of soil water potential
% The first row of rhovf id the initial profile of sol vapor density
   
   tempfeN(jstep,:) = temp;
   thetaf(jstep,:)  = theta;
   psinf(jstep,:)   = psin;
   rhovf(jstep,:)   = rhov;
   Awaf(jstep,:)    = Awa;
   rhovef(jstep,:)  = rhove;
   rhcpvf(jstep,:)  = rhcpv;
   Concff(jstep,:)  = Concf;
   tconf(jstep,:)   = tcon;
   zlamdf(jstep,:)  = zlamda;
   hconf(jstep,:)   = hcon/difcoef;
   vdifa(jstep,:)   = vdiff;
   hvdifa(jstep,:)  = hvdif/difcoef;
   lvf(jstep,:)     = lv;
   steff(jstep,:)   = stef;
   if ENHEV == 1;
   enhf(jstep,:)    = enh;
   end
   csf(jstep,:)     = cs;
   rhf(jstep,:)     = rh;
   sourcef(jstep,:) = sourcev;
   vapresf(jstep,:) = vapres;
   xhivf(jstep,:)   = xhiv;
   uf(jstep,:)      = u;
   deluf(jstep,:)   = delu;
   diseqf(jstep,:)  = diseq;
   adtconf(jstep,:) = adtcon/difcoef; 
   advdiff(jstep,:) = advdif/difcoef;
   Knf(jstep,:)     = Kn; 
   KHf(jstep,:)     = KH;
   Kdf(jstep,:)     = Kd;
   Kmf(jstep,:)     = Km;
   dKHTf(jstep,:)   = dKHdT;
   dKHpf(jstep,:)   = dKHdp;
   thpsf(jstep,:)   = thpsin;
   thtmf(jstep,:)   = thtemp;
   dAwadtf(jstep,:) = dAwadt;
   dAwadpf(jstep,:) = dAwadp;  
   IRf(jstep)       = IR;
   LEf(jstep)       = LE;
   Heatf(jstep)     = Heat;
   G0f(jstep)       = G0;
   
   sdialf(jstep,:)  = sdialc; 
   ddiaTf(jstep,:)  = ddiaT;
   stensnf(jstep,:) = stensn; 
   dstenTf(jstep,:) = dstensdT;
   KnFf(jstep,:)    = KnF;
   KHFf(jstep,:)    = KHF;
#endif 