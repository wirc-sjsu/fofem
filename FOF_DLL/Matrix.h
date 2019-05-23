
#pragma once 

/*--------------------------------------------*/
typedef struct {
    double M [4][4]; 
   } d_Mx; 

typedef struct {
   double m[4];
   } d_M3;


void  Mx_Set (d_Mx *Mx, double a1, double a2, double a3,
double b1, double b2, double b3, 
double c1, double c2, double c3);


void  Mx_Eye (d_Mx *Mx, double a1, double a2, double a3,
double b1, double b2, double b3, 
double c1, double c2, double c3);

void Mx_3Ary (d_M3 *m3, double a, double b, double c);

int Mx_3x1 (double C[], d_Mx *Mx, d_M3 *m3);

int Mx_3x1_Div (d_M3 *To, d_Mx *mx, d_M3 *m3);


void  Mx_Copy ( d_Mx *To, d_Mx *Frm);
void M3_Copy (d_M3 *To, d_M3 *Frm);

void GenThomas(d_M3 *uM3,  
               d_Mx *AMx,  d_Mx *BMx, d_Mx *CMx,  
               d_M3 *rM3,  double n);

int Mx_3x3 (d_Mx *Ans, d_Mx *A, d_Mx *B);

int Mx_Sub (d_Mx *Ans, d_Mx *A, d_Mx *B);

int M3_3x1 (d_M3 *AnsM3, d_Mx *mx, d_M3 *m3);

int M3_Sub (d_M3 *Ans, d_M3 *A, d_M3 *B);