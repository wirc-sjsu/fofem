/*************************************************
* Name: HTA.h  Heat Time Arrays 
*
*************************************************/
int HTA_Layers ();
void HTA_Init ();
void HTA_Put (int i_Lay, float temp, float moist, float psin, float t) ;
int HTA_Get (int i_Lay, int iX, float *af_Heat, float *af_Moist, float *af_psin, float *af_Time);