#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"
#include "SCN_Arrays.h"
#include "Matrix.h" 

int  vxx[10] = { 1,2,3,4,5 }; 

#include "Crank.h" 
#include "HTAA.h"   /* Store Temps for graphing */ 

double  IR, G0, LE, Heat;

int  gi_iN; 

/********************************************************************
* Name: CrankNicolson
* Desc: Do a single time step 
*   In: jstep....time step
*       bmi......not using this for anything yet, I put it in incase
*                i need to send something in or out
*  Ret: 1 ok, 0 error  
**********************************************************************/
int  CrankNicolson (int jstep, char cr_ErrMes[]) 
{
int i, ii,  j, n, nn, ni, Lay; 
double Radiant;  
double emis0, CHp, RH, IRl, CEp, RE;
double f; 

   if ( jstep == 1 )
     gi_iN = 0;  /* init on first step, see how used at end of function */

   strcpy (cr_ErrMes,""); 

   calcsHMV (csx, NULL,     csd, den, temp, theta, nlevel);
 
   caldryvis   (mud,     tempk,nlevel);
 
   calmulaHMV  (TempR, temR, muv, muvT, zlamv, zlavT,       tempk, tempki, nlevel);
 
   calden2HMV  (LGL, denw, denwi, denwdT, denwN,       TempR, tempk, nlevel, jstep );

   calmulaWHMV (muw, muwT, dmTm, zlamw, lawT,     tempki,LGL,denwN,temR, muv,muvT, zlamv,zlavT,denwi,denwdT,nlevel);
   
   calxhiv1(xhiv, dxhivdT,    dxhivdr,vapres,tempki,rhov, nlevel);

   calcondry(zlamdd, NULL,   tempk, tempki, nlevel);  

   calgascomb( zlamda,    zlamv, zlamdd, muv, mud, xhiv, nlevel);

   densatHMV( esat, ss, csat, dcsatdT,          tempk,tempki,nlevel);

   calstefan1NR(stef, stefpT, stefpr,     vapres, tempki, rhov, nlevel );
                                          
   caldiffHMVNRa ( diffa, diffH, diffs, difsdT, difsdr,
                    xhiv,dxhivdT,dxhivdr,tempki,tempr,stef,stefpT,stefpr,nlevel);

   calconHMVll  ( tcona, xa, fw, hv, dhvdT, ka, zlamv,
                TempR, temR, tempk, tempki, temp, 
                diffs,  zlamda, zlamw, ss,
                xm, poros, theta,  psin, nlevel );

   if ( CONRD == 1 ) 
      calconRAD (tcon, tconR,     poros, theta, tempk, tcona, nlevel);  
   else {
     for ( i = 1; i < nlevel; i++ ) 
       tcon[i] = tcona[i]; 
   } /* else */  
 

   if ( ENHEV == 1 )                                                      
      vapdiffEHMV(enh, vdiff, lv,   diffs,xa,poros,fw,ka,hv,nlevel);
   else {
      strcpy (cr_ErrMes, "Not implemented yet -  vapdiffHMV(vdiff, lv,diffs,xa,poros,hv); ");
      return 0; }
      
   vaporTempdiff( DiffT,  vdiff,tempki,nlevel);

   calcsHMVnT(denwnT, cs, csi,    csx, denw, lv, thtemp, nlevel);

// this 'if' doesn't happen 
//   if ( SORCE != 1 )  
//    calgascomb(mug,muv,mud,muv,mud,xhiv,nlevel);

   calepssurfHMV(sdialc, ddiaT, stensn, dstensdT,    tempk,tempki,TempR,nlevel);

   calhydrauKF(KnF, KHF, dKnFdT, dKnFdp, dKHFdT, dKHFdp,
                 poros,tempk,tempki,psin,denw,muw,dmTm,sdialc,ddiaT, 
                 stensn,dstensdT,nlevel); 
 
   ACS_Check (dKnFdp, "dKnFdp"); 

   calSw (Sw, LGLth,  theta,poros,nlevel);

   calcpaHMVNR(rhcpv, rhcpa, Dummy1, Dummy2,     tempki,tempr,rhov,nlevel );

   calhydrauVA ( Kn, KH, dKHdT, dKHdt, dKHdp,  Rcoef,muw,dmTm,denw,Sw,poros,thpsin,nlevel);

 //  if ( WRCHY == 1 ) 
 //     calhydrauWRC (Kn,KH,dKHdT,dKHdt,dKHdp,    psin,muw,dmTm,denw, nlevel); 

   for ( i = 1; i <= nlevel; i++ ) 
     dKHdT[i] = dKHdT[i] + dKHdt[i] * thtemp[i];

   calsurfdT(Kd, Km, tempr,theta,Sw,poros,thpsin,thtemp,nlevel);
   for ( i = 1; i <= nlevel; i++ ) 
     Kn[i] = Kn[i]+ Kd[i];

   calrhev (rh, rhove, drevdp, drevdT,  tempki,csat,dcsatdT,psin, nlevel);
 
   if ( BRUSS == 1 ) 
      calAwa (Awa, dAwadt, dAwadp,  Sw, poros,thpsin,LGLth,nlevel);
   else 
// % *** Parabolic Model for volume-averaged soil moisture surface area ------   
//  [Awa dAwadt dAwadp] = calAwaP(Sw,poros,thpsin,LGLth);
   calAwaP (Awa, dAwadt, dAwadp,  Sw, poros,thpsin,LGLth,nlevel);

   calConCoef5(Concf, dCondT, dCondp, psin,tempki,nlevel);

   DRYSV = 1;  
                 
   calVsourceGNRa1 (sourcev, dSvdT, dSvdp, dSvdr, Srhov,
                   Awa,dAwadt,dAwadp,thtemp,rhove,
                  rhov,Concf,dCondT,dCondp,drevdT,
                  drevdp,tempk,tempki,nlevel);
  
   caluHMV (u, delu, advdif, adtcon,    xa,difcoef,delz,sourcev,rhov,rhcpa,nlevel,nl);  

   harmean( hcon, difcoef, tcon,nlevel);
   harmean( hvdif, difcoef, vdiff,nlevel);
   harmean( hKn,   difcoef, Kn,nlevel);
   harmean( hKm,   difcoef, Km,nlevel);
   harmean( hDifT, difcoef, DiffT,nlevel);


// %__________________________________________________________________________
// % Calculate the surface emissivity, the outgoing IR, and 
// %     the surface energy balance coeffients 
extern double forIR[]; 

  EBcoef2(&emis0, &IR, &CHp, &RH, &IRl, &CEp, &RE, 
          theta[1], bcra[jstep], bcta[jstep], 
          Htrans, temp[1], tempk[1], tempki[1], forIR[jstep], eta4[1], rhov[1], CE, 
          rh[1], CU, u[1], lv[1]);

   calEBCN( &Radiant, &Heat, &LE, &G0,
            bcQ[(int)jstep], bcta[(int)jstep], bcra[(int)jstep], 
            emis0, CHp, CEp,
            RE, IR, rhov[1],
            temp[1]);  

   caldiseq(diseq,rhove,Srhov,nlevel);
 
   AccumulateHMV  (jstep); 

   if ( jstep >= nsteps )
    return 1; 

 int   js1 = jstep + 1;  
double QH; 
   calQHCN(&QH, bcQ[js1], bcta[js1],bcra[js1],emis0,CHp,CEp,IR,IRl);

// %__________________________________________________________________________
// % Fill in the arrays and the martix and solve the tridiagonal 
// % Crank-Nicolson system: Version 7.110 of the [3-variable] Model
// % or Version 7.110T, the temperature dependent WRC 

d_M3 xi[eC_Mx];  /* Needs to be here because those #includes below */

#include  "CN_MatrixHMV7110.h" 
#include  "CN_MatrixPop.h"

   for ( ni = 1; ni <= nlevel; ni++ ) {  // for ni      = 1:nlevel;
     temp1[ni] = xi[ni].m[1];   //xi{ni}(1);
     psin1[ni] = xi[ni].m[2]; 
     rhov1[ni] = xi[ni].m[3];
   } /* for ni */    

/*.................................................................... */
   calConCoef2_Ary (Rcoef, dRcdT, dRcdp, psin1, temp1, nlevel );

 
//NOTE NOTE NOTE: 
// There's a bunch of 'if' 'else' checking these FYWRC, etc swithces in the 
//  Matlab code that i didn't put  in here, SO if the 'else' error message 
//  goes off you'll need to check the Matlab code to fix this 

  if ( FYWRC == 1  )  { 
    if ( TMPWRC == 0 ) 
        calthetaFYr (theta1, thpsin, thpsini, thtemp,     Rcoef,dRcdT,dRcdp,psin1,poros, nlevel );
    else {
      strcpy (cr_ErrMes,"CrankNicolson() - Fix This. FYWRC "); 
      return 0; }
  } /* if FYWRD */ 
 

  if ( CSWRC == 1 ) {
     if ( TMPWRC == 0 ) { 
       calthetaCSr(theta1, thpsin, thpsini, thtemp,  Rcoef,dRcdT,dRcdp,psin1,nlevel); }
     else {
      strcpy (cr_ErrMes,"CrankNicolson() - Fix This. CSWRC "); 
      return 0; }
  } /* if CSWRC == 1 */

//%__________________________________________________________________________
//%__________________________________________________________________________
//% Save the previous value of theta (theto)
//% Reset the solution for the next time step of the integration 

   for ( i = 1; i <= nlevel; i++ ) {
     theto[i] = theta[i];
     temp[i]  = temp1[i];
     psin[i]  = psin1[i];
     theta[i] = theta1[i];
     rhov[i]  = rhov1[i];  }
 
   caltempkHMV (tempk, tempki, tempr,   temp, nlevel );
   calvaporHMV (vapres, rhov,tempk, nlevel);

  
/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-*/
int i_Lays, i_Inc, i_Min, i_Sec; 
float dep,inc,f_Seconds, f_Minutes, f_Rem, f_Min; 

/* Determine where the 1,2,3... centimeter values will be found in the */
/*  temp, mosist... array */ 
   dep = e_Depth * 100;      /* number of centimeters in depth */
   inc = ( e_Depth / delz ); /* number of 'slices' in dep */ 
   f  = inc / dep;           /* so each centimeter is this far apart in arrays*/
   i_Inc = (int) f; 

   f_Seconds = (float) jstep * delt;  /* time step * seconds per time step */
   f_Minutes = f_Seconds / 60.0;
   i_Lays = HTA_Layers();             /* number of layers, surface is Layer 1  */ 
   j = 1;    

/* Save every so many values, don't save for every time step */ 
   gi_iN++;
   n = (int) delt * 10; /* single time step (seconds) times 10 */
   if ( gi_iN <  n ) 
    goto Ret;

   gi_iN = 0; 
    
   for ( Lay = 1; Lay <= i_Lays; Lay++ ) {
      ii = HTA_Put (Lay,temp[j],theta[j], psin[j], f_Minutes); 
      if ( ii == 0 ) { 
        strcpy ( cr_ErrMes, "CrankNicolson() - Array overflow"); 
        return 0; }
      j = j + i_Inc;  /* Get to next layer, layers are 1 cm thick */
      if ( j > nlevel ) {  
         strcpy ( cr_ErrMes, "Logic Error Saving Temp/Moist CrankNicolson()"); 
         return 0; }
     }
Ret:
   return 1; 
}



