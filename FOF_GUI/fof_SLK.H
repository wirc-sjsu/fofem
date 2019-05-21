
/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/*                     Species Live Killed Struct                        */
/* Table will hold one entry ror each user entered Mortality Species,  */
/* for ir_Live, ir_Kill, by DBH,  [0] = 0->0.99,  [1] = 1.0->1.99,     */
/*                               [39] = 39.0 -> all above              */
typedef struct  {
#define  eC_SLK_SPe 10
   char cr_Spe[eC_SLK_SPe+1];


#define eC_LivKil 40               /* 0...39   */
   int ir_Live [eC_LivKil];        /* See notes above */
   int ir_Kill [eC_LivKil];

}  d_SLK;



void SLK_Init (void);
void SLK_Zero (void);
void SLK_Display (void);
int  SLK_Put (char cr_Spe[], float f_DBH, int i_Live, int i_Kill);
int   SLK_ifGroup (int i_Grp);
void SLK_GetSpe (int iX, char cr_Spe[]);
int   SLK_GetDen (int iX, int i_SizCls);
int   SLK_GetLK  (int iX, int i_SizCls, char c);

int  SLK_Total (float *af_Live, float *af_Kill, int X);
