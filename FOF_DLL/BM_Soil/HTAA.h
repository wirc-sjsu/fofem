/*************************************************
* Name: HTAA.h  Heat Time Arrays 
*
*************************************************/

/* Will Save simulation values every # many seconds */
#define e_SecSav (int) 30 

#define e_HTA_Init  -999
#define eC_HTA  150000    /* Size of array - max values that can be saved */
#define e_BM_Lay 21       /* Max Number of soil layers (1 cm) that cam be saved */


int HTA_Layers ();
void HTA_Init ();
int HTA_Put (int i_Lay, float temp, float moist, float psin, float t) ;
int HTA_Get (int i_Lay, int iX, float *af_Heat, float *af_Moist, float *af_psin, float *af_Time);
int  HTA_Count () ;