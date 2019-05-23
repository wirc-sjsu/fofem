/**********************************************************
* Name: Brn_SFI.h  
* Desc: save fire intensity for doing soil model 
*
***********************************************************/
int  HSBI_FirstMinute (float f_HS, float f_WLfi, double *af_Con, double  *af_FI);

int HSBI_Next (float f_LWfi, double *af_Con, double *af_FI);