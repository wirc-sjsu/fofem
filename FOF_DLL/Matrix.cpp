#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMSoil.h"
#include "Matrix.h" 

/************************************************************
*
************************************************************/
void M3_Copy (d_M3 *To, d_M3 *Frm)
{
  To->m[1] = Frm->m[1]; 
  To->m[2] = Frm->m[2]; 
  To->m[3] = Frm->m[3]; 
}

/************************************************************
*
************************************************************/
void  Mx_Copy ( d_Mx *To, d_Mx *Frm)
{
int i, j; 
   for ( i = 1; i <= 3; i++ ) 
     for ( j = 1; j <= 3; j++ )
       To->M[i][j] = Frm->M[i][j]; 
} 

/************************************************************
*
************************************************************/
void  Mx_Set (d_Mx *Mx, double a1, double a2, double a3,
double b1, double b2, double b3, 
double c1, double c2, double c3)
 
{
int i; 

 Mx->M[1][1] = a1; 
 Mx->M[1][2] = a2; 
 Mx->M[1][3] = a3;

 Mx->M[2][1] = b1; 
 Mx->M[2][2] = b2; 
 Mx->M[2][3] = b3;


 Mx->M[3][1] = c1; 
 Mx->M[3][2] = c2; 
 Mx->M[3][3] = c3;
}  

/*******************************************************
* mimic a Matlab function called eye()
********************************************************/
void  Mx_Eye (d_Mx *Mx, double a1, double a2, double a3,
               double b1, double b2, double b3, 
               double c1, double c2, double c3)
{
    Mx_Set (Mx,  a1, a2, a3, b1,  b2, b3, c1,  c2,  c3);
    Mx->M[1][1] += 1.0; 
    Mx->M[2][2] += 1.0; 
    Mx->M[3][3] += 1.0; 
}

/*********************************************************
*
**********************************************************/
void Mx_3Ary (d_M3 *m3, double a, double b, double c)
{
  m3->m[1] = a; 
  m3->m[2] = b; 
  m3->m[3] = c; 
}

/****************************************************
* Nane:  Mx_3x1
* Desc: Mulitpy a 3x3 array by a 1x3 array
* NOTE  THE ARRAYs are actually 4, but we use start
*       indexing at [1] to mimic MatLab
*       
*****************************************************/
int Mx_3x1 (double C[], d_Mx *mx, d_M3 *m3)
{
int i,j;
double f; 

   f = 0; 
   for ( i = 1; i <= 3; i++ ) { 
     f = 0; 
     for ( j = 1; j <= 3; j++ ) { 
         f += mx->M[i][j] * m3->m[j];
     } /* for j */
     C[i] = f; 
   } /* for i */ 


  return 1; 
}

/***************************************************
* Name: M3_3x1 
* Desc: Multiply a 3x3 * 3x1, and put it in an M3
*       
****************************************************/
int M3_3x1 (d_M3 *AnsM3, d_Mx *mx, d_M3 *m3)
{
double C[4]; 
   Mx_3x1 (C, mx, m3);
   AnsM3->m[1] = C[1];
   AnsM3->m[2] = C[2];
   AnsM3->m[3] = C[3];
   return 1; 
}

int M3_Sub (d_M3 *Ans, d_M3 *A, d_M3 *B)
{
  Ans->m[1] = A->m[1] - B->m[1]; 
  Ans->m[2] = A->m[2] - B->m[2]; 
  Ans->m[3] = A->m[3] - B->m[3]; 
  return 1; 
}

/****************************************************
* Nane:  Mx_3x3
* Desc: Mulitpy a 3x3 array by a 3x3 array
* NOTE  THE ARRAYs are actually 4, but we use start
*       indexing at [1] to mimic MatLab
*       
*****************************************************/
int Mx_3x3 (d_Mx *Ans, d_Mx *A, d_Mx *B)
{
int i,j,k;
double f; 


   f = 0; 
   for ( i = 1; i <= 3; i++ ) { 
     for ( j = 1; j <= 3; j++ ) {     
       for ( k = 1; k <= 3; k++ ) {
         f += A->M[i][k] * B->M[k][j]; }
     Ans->M[i][j] = f; 
     f = 0; } 
   } /* for i */ 

  return 1; 
}


/*********************************************
* Name: Mx_Sub
* Desc: Subtract Matrix B from A
*       C = A - B
*********************************************/
int Mx_Sub (d_Mx *Ans, d_Mx *A, d_Mx *B)
{
int i,j;    
  for ( i = 1; i <= 3; i++ ) 
    for ( j = 1; j <= 3; j++ ) 
      Ans->M[i][j] = A->M[i][j] - B->M[i][j]; 
   return 1; 
}


