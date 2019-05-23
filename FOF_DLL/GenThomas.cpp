#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <math.h> 
#include <malloc.h>
#include "BMsoil.h"
#include "Matrix.h" 


int  NewMat_Div (d_Mx *C, d_Mx *A,  d_Mx *B);

 int Mx_DivNew (d_M3 *C, d_Mx *A,  d_M3 *B);

#define eC_Inv 3

void MatrixInversion(double **A, int order, double **Y);
double CalcDeterminant( double **mat, int order);
int GetMinor(double **src, double **dest, int row, int col, int order);

int Mx_Div (d_M3 *Ans, d_Mx *Mx, d_M3 *M3); 
int Mx_Div_Mx (d_Mx *Ans, d_Mx *AMx, d_Mx *BMx) ;

double  a[3] = { 1,2,3 };
double  b[3] = { 4,5,6 };
double  c[3] = { 7,8,9 };

double  d[3] = { 11,12,13 };
double  e[3] = { 14,15,16 };
double  f[3] = { 17,18,19 };

double * InputMx[3]= { a, b, c }; 
double * InverseMx[3]= { d, e, f }; 

/***************************************************************/
d_Mx gam[500]; 

d_Mx gam2[500]; 


void GenThomas(d_M3 *uM3,  
               d_Mx *AMx,  d_Mx *BMx, d_Mx *CMx,  
               d_M3 *rM3,  double n)
{
int i, j; 
// d_Mx *gam = new d_Mx [n]; 
d_Mx bet; 
d_Mx Ans; 
d_M3 Tmp1M3, Tmp2M3;; 

d_M3 Test; 

   Mx_Copy (&bet, &BMx[1]);

//   u{1}    = bet \  r{1};
   Mx_Div (&uM3[1], &bet, &rM3[1]);
//   Mx_DivNew (&Test, &bet, &rM3[1]);



   for ( j = 2; j <= n; j++ ) {
if ( j == 68 ) 
    printf("");
      Mx_Div_Mx (&gam[j], &bet, &CMx[j-1]);  // gam{j} = bet \ C{j-1};
 //     NewMat_Div (&gam[j], &bet, &CMx[j-1]); 
 
      Mx_3x3 (&Ans, &AMx[j], &gam[j]);       //  bet = B{j} - A{j}*gam{j};
      Mx_Sub (&bet, &BMx[j], &Ans);     
 
//  u{j} = bet \ ( r{j} - A{j} * u{j-1} );  
      M3_3x1 (&Tmp1M3, &AMx[j], &uM3[j-1]);  //  A{j} * u{j-1}); 
      M3_Sub (&Tmp2M3, &rM3[j], &Tmp1M3); 
      Mx_Div (&uM3[j], &bet, &Tmp2M3);       //  u{j} = bet\(r{j} 
  } /* for j */ 


// for j   = n-1 : -1 : 1;
  for ( j = n - 1; j >= 1; j-- ) {
//  u{j}  = u{j} - gam{j+1} * u{j+1};
     M3_3x1 (&Tmp1M3, &gam[j+1],  &uM3[j+1]); 
     M3_Sub (&Tmp2M3, &uM3[j], &Tmp1M3); 
     M3_Copy (&uM3[j], &Tmp2M3); 
   }  /* for j */ 

//    delete gam; 
}

/****************************************************************
* Name: Mx_Div_Mx 
* Desc: 
*
**************************************************************/
int Mx_Div_Mx (d_Mx *AnsMx, d_Mx *AMx, d_Mx *BMx) 
{
int i, j; 
d_Mx InvMx; 


/* Get an input 3x3 Matrix, and put our input data into it */ 
//double **InputMx;
//  InputMx = new double*[eC_Inv];
//  for ( i = 0; i < eC_Inv; i++)
//    InputMx[i] = new double[eC_Inv];

  for ( i = 0; i < eC_Inv; i++ ) {
    for ( j = 0; j < eC_Inv; j++ ){ 
       InputMx[i][j] = AMx->M[i+1][j+1];  } }

/* Get a 3X3 matrix to hold the Inverse matrix we'll calculate */
//double **InverseMx;
//  InverseMx = new double*[eC_Inv];
//  for (i = 0; i < eC_Inv; i++)
 //   InverseMx[i] = new double[eC_Inv];

  for ( i = 0; i < eC_Inv; i++ ) {
    for ( j = 0; j < eC_Inv; j++ ){ 
       InverseMx[i][j] = 0;  } }

/* Calculate the Inverse matrix  */
  MatrixInversion(InputMx, eC_Inv, InverseMx);

  for ( i = 0; i < eC_Inv; i++ ) {
    for ( j = 0; j < eC_Inv; j++ ) { 
      InvMx.M[i+1][j+1] = InverseMx[i][j]; 
//      printf ("%d %d %f \n",i, j,  InverseMx[i][j]); 
     }
  }

   
/* Now multipy using the Inverse */  
   Mx_3x3 (AnsMx, &InvMx, BMx );


//  for ( i = 0; i < eC_Inv; i++) 
 //   delete InputMx[i]; 
 // delete InputMx;


 
 // for ( i = 0; i < eC_Inv; i++) 
//    delete [] InverseMx[i];  
 // delete InverseMx; 



    return 1; 
}  

/****************************************************************
* Name: Mx_Div 
* Desc: Divide a 3x3 matric by a 3x1 array 
*       Ans = Mx / M3 
*       this is done my inverting (Inverse) of Mx and then
*       mulitpling Inv(Mx) * M3
**************************************************************/
int Mx_Div (d_M3 *Ans, d_Mx *Mx, d_M3 *M3) 
{
int i, j; 
d_Mx InvMx; 

/* Get an input 3x3 Matrix, and put our input data into it */ 
//double **InputMx;
//  InputMx = new double*[eC_Inv];
//  for ( i = 0; i < eC_Inv; i++)
//    InputMx[i] = new double[eC_Inv];

  for ( i = 0; i < eC_Inv; i++ ) {
    for ( j = 0; j < eC_Inv; j++ ){ 
       InputMx[i][j] = Mx->M[i+1][j+1];  } }

/* Get a 3X3 matrix to hold the Inverse matrix we'll calculate */
//double **InverseMx;
//  InverseMx = new double*[eC_Inv];
//  for ( i = 0; i < eC_Inv; i++)
 //   InverseMx[i] = new double[eC_Inv];

  for ( i = 0; i < eC_Inv; i++ ) {
    for ( j = 0; j < eC_Inv; j++ ){ 
       InverseMx[i][j] = 0;  } }

/* Calculate the Inverse matrix  */
  MatrixInversion(InputMx, eC_Inv, InverseMx);

  for ( i = 0; i < eC_Inv; i++ ) 
    for ( j = 0; j < eC_Inv; j++ ) 
      InvMx.M[i+1][j+1] = InverseMx[i][j]; 
   
/* Now multipy using the Inverse */ 
double C[4]; 
    Mx_3x1 ( C, &InvMx, M3 );
    Ans->m[1] = C[1]; 
    Ans->m[2] = C[2]; 
    Ans->m[3] = C[3]; 

  //for ( i = 0; i < eC_Inv; i++) {
  //   delete InputMx[i]; 
  //   delete InverseMx[i];  }
  //delete InputMx;
 // delete InverseMx; 

    return 1; 
}     



/***************************************************************
* matrix inversion the result is put in Y
* I got this code off the internet 
*  https://chi3x10.wordpress.com/2008/05/28/calculate-matrix-inversion-in-c/
* 
*
**************************************************************/
void MatrixInversion(double **A, int order, double **Y)
{
int i, j; 
double det, d; 

    // get the determinant of a
     d = CalcDeterminant(A,order);
     if ( d != 0 ) 
       det = 1.0 / d; 
     else 
       det = 0; 

    // memory allocation
    double *temp = new double[(order-1)*(order-1)];
    double **minor = new double*[order-1];
    for( i=0;i<order-1;i++)
        minor[i] = temp+(i*(order-1));
 
    for( j=0;j<order;j++)
    {
        for( i=0;i<order;i++)
        {
            // get the co-factor (matrix) of A(j,i)
            GetMinor(A,minor,j,i,order);
            Y[i][j] = det*CalcDeterminant(minor,order-1);

            if( (i+j)%2 == 1)
                Y[i][j] = -Y[i][j];
        }
    }
 
    // release memory
    //delete [] minor[0];
    delete [] temp;
    delete [] minor;
}
 
// calculate the cofactor of element (row,col)
int GetMinor(double **src, double **dest, int row, int col, int order)
{
int i, j; 

    // indicate which col and row is being copied to dest
    int colCount=0,rowCount=0;
 
    for( i = 0; i < order; i++ )
    {
        if( i != row )
        {
            colCount = 0;
            for( j = 0; j < order; j++ )
            {
                // when j is not the element
                if( j != col )
                {
                    dest[rowCount][colCount] = src[i][j];
                    colCount++;
                }
            }
            rowCount++;
        }
    }
 
    return 1;
}
 
// Calculate the determinant recursively.
double CalcDeterminant( double **mat, int order)
{
int i, j;

    // order must be >= 0
    // stop the recursion when matrix is a single element
    if( order == 1 )
        return mat[0][0];
 
    // the determinant value
    double det = 0;
 
    // allocate the cofactor matrix
    double **minor;
    minor = new double*[order-1];
    if ( minor == NULL ) 
      printf ("Fuck"); 
    for( i=0;i<order-1;i++)
        minor[i] = new double[order-1];
 
    for( i = 0; i < order; i++ )
    {
        // get minor of element (0,i)
        GetMinor( mat, minor, 0, i , order);
        // the recusion is here!
 
 // Orig       det += (i%2==1?-1.0:1.0) * mat[0][i] * CalcDeterminant(minor,order-1);
       det += xpow( -1.0, i ) * mat[0][i] * CalcDeterminant( minor,order-1 );
    }
 
    // release memory
    for( i=0;i<order-1;i++)
        delete [] minor[i];
    delete [] minor;
 
    return det;
}



#ifdef wowowo
function [ u ] = GenThomas( A,B,C,r,n )  

% Generalized Thomas Algorithm for solving block tridiagonal matrices 
%
%--------------------------------------------------------------------------
%              Adapted from Mendes (2002,2004) by 
%              WJ Massman (July 30, 2010) @ CSIRO  
%--------------------------------------------------------------------------
%
% A, B, and C are Matlab cell arrays with NxN matirces as their elements 
% d is a cell array with an Nx1 matrix (or column vector} as its element
% Each cell array is length nlev = numeber of spatial points in the model 
%
% Here the number of coupled differential equations is N and the arrays
% are derived from the tridiagonal form of the finite difference
% representation of the PDE's 
%     
%--------------------------------------------------------------------------

gam     = cell(n,1);
u       = cell(n,1);

bet     = B{1};
u{1}    = bet\r{1};

for j   = 2:n;
 gam{j} = bet\C{j-1};
 bet    = B{j} - A{j}*gam{j};
 u{j}   = bet\(r{j}-A{j}*u{j-1});
end

for j   = n-1 : -1 : 1;
  u{j}  = u{j}-gam{j+1}*u{j+1};
end 

end
#endif 

 