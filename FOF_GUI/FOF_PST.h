/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: FOF_PST.h  
* Desc: 
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/

/* Probability Size Class  Table */
typedef struct {
      float low;
      float up;
      float cut;
   } d_PST;  

#define eC_PST 3

void PST_Init();
float PST_GetProb (float dbh); 
int PST_Put (int ix, float low, float up, float cut); 
int PST_Check ();  