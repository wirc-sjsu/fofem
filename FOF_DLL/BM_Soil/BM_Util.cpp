/*****************************************************************
* Name: BM_Util.cpp
* Desc: various utility and testing functions
*       There are 'x' math functions I wrote that program uses
*       so I can check and put break points and catch anything
*       going wrong.
*       In addition to these 'x' functions I got the debugger
*       to catch (popup window) divide by 0 and math function
*       exceptions. 
*****************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h> 
#include <setjmp.h> 

#include "BMSoil.h"
extern  double jstep; 
extern double nlevel; 

jmp_buf g_env;  // longjmp(); 

char gcr_FuncName[200] = ""; 
char gcr_JumpMess[500] = ""; 



/*************************************************
* mimic Matlab nthroot() function
*************************************************/
double xnthroot (double f, double to ) 
{
double g; 
char cr[50]; 

 g = nthroot (abs(f),to); 
 sprintf (cr,"%f",g);

 if ( !strncmp (cr, "1.#",3 ) ) {
   sprintf (gcr_JumpMess, "xnthroot (%f, %f) %s ", f, to, cr); 
   longjmp(g_env,e_JmpExp);  } 

 if ( !strncmp (cr, "-1.#",4 ) ) {
   sprintf (gcr_JumpMess, "xnthroot (%f, %f) %s ", f, to, cr); 
   longjmp(g_env,e_JmpExp);  } 

 return g; 
}

/*************************************************
* Those strncmp() are how I figured out to catch
* the invalid float points... ex:  1.#IND, 1.#INF, etc. 
* 
*************************************************/
double xsqrt (double f ) 
{
double g; 
char cr[50]; 

 g = sqrt (abs(f)); 
 sprintf (cr,"%f",g);

 if ( !strncmp (cr, "1.#",3 ) ) {
   sprintf (gcr_JumpMess, "xsqrt (%f) %s ", f, cr); 
   longjmp(g_env,e_JmpExp);  } 

 if ( !strncmp (cr, "-1.#",4 ) ) {
   sprintf (gcr_JumpMess, "xsqrt (%f) %s ", f, cr); 
   longjmp(g_env,e_JmpExp);  } 
 
return g; 
}

/*************************************************
*
**************************************************/
double xexp (double f)
{
double g; 
char cr[50]; 

 g = exp (f); 
 sprintf (cr,"%f",g);

 if ( g == HUGE_VAL ) { 
   sprintf (gcr_JumpMess, "xexp (%f) %s ", f, cr); 
   longjmp(g_env,e_JmpExp);  } 

 if ( !strncmp (cr, "1.#",3 ) ) {
   sprintf (gcr_JumpMess, "xexp (%f) %s ", f, cr); 
   longjmp(g_env,e_JmpExp);  }

 if ( !strncmp (cr, "-1.#",4 ) ) {
   sprintf (gcr_JumpMess, "xexp (%f) %s ", f, cr); 
   longjmp(g_env,e_JmpExp);  }

 return g; 
}

/****************************************************************
*
****************************************************************/
double xpow (double base, double to)
{
double f; 
char cr[50];

  f = pow (base,to);
  sprintf (cr,"%f",f); 
 
  if ( !strncmp (cr, "1.#", 3 ) ) {
   sprintf (gcr_JumpMess, "xpow (%f, %f) %s ", base, to , cr); 
   longjmp(g_env,e_JmpExp);  }

  if ( !strncmp (cr, "-1.#", 4 ) ) {
   sprintf (gcr_JumpMess, "xpow (%f, %f) %s ", base,to, cr); 
   longjmp(g_env,e_JmpExp);  }

  return f; 
}

/******************************************************************
*
*
*******************************************************************/
double  xlog (double f) 
{
double a,b,c;
char cr[100]; 

  if ( f >= 0 ) { 
    c = log (f); 
    goto Ret; }

  if ( f == 0 ) 
    return 0; 

  a = abs (f);
  b = log  (a);
  c = b + e_pi;

Ret: 
  sprintf (cr,"%f",c); 

  if ( !strncmp (cr, "1.#", 3 ) ) {
   sprintf (gcr_JumpMess, "xlog (%f) %s ", f, cr); 
   longjmp(g_env,e_JmpExp);  }

  if ( !strncmp (cr, "-1.#", 4 ) ) {
   sprintf (gcr_JumpMess, "xlog (%f) %s ", f, cr); 
   longjmp(g_env,e_JmpExp);  }

  return c; 
}

double xZero (char cr[])
{
  printf ("Divide by Zero: : %s \n",cr); 
  return 0; 
}

/*********************************************************************
* Name: nthroot
* Desc: the n-th square root
*    pow (27,  1/3 )
* cubic root Example; 3 = nthroot (27,3);
*                     3 = pow (27, 1/3 );          
*********************************************************************/
double nthroot (double f, double g)
{
double F,G;
   if ( g <= 0 ) { 
     printf ("nthroot() neg zero \n"); 
     return 0; } 
   F = 1.0 / g;
   G = xpow (f,F);
   return G; 
}

/********************************************************
*
*
********************************************************/
int Vec_Mult (double A[], double B[], double C[], double N)
{
int i;
   for ( i = 1; i <= N; i++ ){
     C[i] = A[i] * B[i];
   }
  return 1; 
}



/********************************************************************
*
*******************************************************************/
int ACS_Check (double fr[], char cr_Id[])
{
int i,N;
char cr[400]; 
   N = 0;  

   for ( i = 1; i <= nlevel; i++ ) {
   // can't do this when I use    _controlfp_s()  
    // if ( fr[i] == gf_DivErr ) { 
     //  printf ("ACS - Divide Error: Stuff a 1.0,   %s %d \n", cr_Id, i ); 
     //  fr[i] = 1.0;
     //  continue; }

//     if ( fr[i] > 10000000.0 ) {
 //      printf ("ACS - Huge Number: %s %d \n", cr_Id, i ); 
//       N++; }

     sprintf (cr,"%f",fr[i]);
     if ( !strncmp (cr, "1.#",3 ) ) {
        printf ("ACS - Trashed: %s, %d,  \n", cr_Id,i); 
        return 0; 
     }

     if ( !strncmp (cr, "-1.#",4 ) ) {
        printf ("ACS - Trashed: %s, %d,  \n", cr_Id,i); 
        return 0; 
     }

  

   }  /* for i */ 

  if ( N > 0 ) {
    printf ("ACS - ERRORS  %s  %d \n", cr_Id, N); 
    return 0; }
  return 1; 

}

