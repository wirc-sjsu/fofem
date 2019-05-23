
 // if ( jstep >= 422 ) 
//   printf ("Test1: \n");

// #define eC_Mx 220  see BMSoil.h 
d_Mx Ai[eC_Mx];
d_Mx Bi[eC_Mx]; 
d_Mx Ci[eC_Mx];
d_Mx Ap[eC_Mx];
d_Mx Bp[eC_Mx];
d_Mx Cp[eC_Mx];
d_M3 Ri[eC_Mx];

// d_M3 xi[eC_Mx]; 

d_M3 di[eC_Mx]; 

//% Now populate the arrays 
//% Elements of (Ai, Bi, Ci) and (Ap, Bp, Cp) are 3x3 matrices 
//% Elements of Ri, xi, and di are 1x3 column vectors
//%
//%**************************************************************************
//%  [n = 1] Ai{n}, Bi{n}, Ci{n}, Ap{n}, Bp{n}, Cp{n}, Ri{n} 
//%**************************************************************************
  Mx_Set (&Ai[1], 0,0,0,  0,0,0,  0,0,0);
  Mx_Eye (&Bi[1], B011,B012,B013,  B021,B022,B023, B031,B032,B033);
  Mx_Set (&Ci[1], C011,C012,C013,  C021,C022,C023,  C031,C032,C033);
  Mx_Set (&Ap[1], 0,0,0,  0,0,0,  0,0,0);
  Mx_Eye (&Bp[1], Bp011,Bp012,Bp013,  Bp021,Bp022,Bp023,  Bp031,Bp032,Bp033);
  Mx_Set (&Cp[1], Cp011,Cp012,Cp013, Cp021,Cp022,Cp023, Cp031,Cp032,Cp033);
  Mx_3Ary (&Ri[1], R01,R02,R03);

//%**************************************************************************
//%  [n = 2:nl] Ai{n}, Bi{n}, Ci{n}, Ap{n}, Bp{n}, Cp{n}, Ri{n} 
//%**************************************************************************
  
//if ( jstep >= 1414 )
 //  printf ("Ai gets trashed \n"); 

 for (  nn = 2; nn <= nl; nn++ ) {
     Mx_Set (& Ai[nn], Ai11[nn], Ai12[nn], Ai13[nn], 
             Ai21[nn], Ai22[nn], Ai23[nn],  
             Ai31[nn], Ai32[nn], Ai33[nn]); 

     Mx_Eye (&Bi[nn], Bi11[nn], Bi12[nn], Bi13[nn], 
                      Bi21[nn], Bi22[nn], Bi23[nn],
                      Bi31[nn], Bi32[nn], Bi33[nn]);
       
     Mx_Set (&Ci[nn], Ci11[nn], Ci12[nn], Ci13[nn], 
                      Ci21[nn], Ci22[nn], Ci23[nn],
                      Ci31[nn], Ci32[nn], Ci33[nn]); 
       
     Mx_Set (&Ap[nn], Api11[nn], Api12[nn], Api13[nn], 
                      Api21[nn], Api22[nn], Api23[nn], 
                      Api31[nn], Api32[nn], Api33[nn]); 
       
     Mx_Eye (&Bp[nn], Bpi11[nn], Bpi12[nn], Bpi13[nn],
                      Bpi21[nn], Bpi22[nn], Bpi23[nn],
                      Bpi31[nn], Bpi32[nn], Bpi33[nn]);
       
     Mx_Set (&Cp[nn], Cpi11[nn], Cpi12[nn], Cpi13[nn],
                      Cpi21[nn], Cpi22[nn], Cpi23[nn],
                      Cpi31[nn], Cpi32[nn], Cpi33[nn]); 
       
     Mx_3Ary (&Ri[nn], Ri1[nn], Ri2[nn], Ri3[nn]);


   }  /* for nn */ 

//%**************************************************************************
//%  [n = nlevel] Ai{n}, Bi{n}, Ci{n}, Ap{n}, Bp{n}, Cp{n}, Ri{n} 
//%**************************************************************************
   Mx_Set (&Ai[(int)nlevel], AM11,AM12,AM13,  AM21,AM22,AM23, AM31,AM32,AM33);
   Mx_Eye (&Bi[(int)nlevel], BM11,BM12,BM13,  BM21,BM22,BM23, BM31,BM32,BM33);
   Mx_Set (&Ci[(int)nlevel], 0,0,0,  0,0,0, 0,0,0);
   Mx_Set (&Ap[(int)nlevel], ApM11,ApM12,ApM13, ApM21,ApM22,ApM23, ApM31,ApM32,ApM33);
   Mx_Eye (&Bp[(int)nlevel], BpM11,BpM12,BpM13, BpM21,BpM22,BpM23, BpM31,BpM32,BpM33);
   Mx_Set (&Cp[(int)nlevel], 0,0,0, 0,0,0, 0,0,0);
   Mx_3Ary(&Ri[(int)nlevel], RM1, RM2, RM3);

//%**************************************************************************
//%  [n = 1:nlevel] xi(n)
//%**************************************************************************
  for (   ni = 1; ni <= nlevel; ni++ ) 
     Mx_3Ary ( &xi[ni], temp[ni], psin[ni], rhov[ni]);


//%**************************************************************************
//%  [n = 1:nlevel] di(n)
//%**************************************************************************
double ap[4], bp[4],cp[4]; 
int nx; 

//   di{1}      = Bp{1}*xi{1} + Cp{1}*xi{2} + Ri{1};
  Mx_3x1 (bp, &Bp[1], &xi[1]);
  Mx_3x1 (cp, &Cp[1], &xi[2]);
  di[1].m[1] = bp[1] + cp[1] + Ri[1].m[1]; 
  di[1].m[2] = bp[2] + cp[2] + Ri[1].m[2]; 
  di[1].m[3] = bp[3] + cp[3] + Ri[1].m[3]; 


  for (  nx = 2; nx <= nl; nx++ ) {
//  di{nx}   = Ap{nx}*xi{nx-1} + Bp{nx}*xi{nx} + Cp{nx}*xi{nx+1} + Ri{nx};
    Mx_3x1 (ap, &Ap[nx], &xi[nx-1]);
    Mx_3x1 (bp, &Bp[nx], &xi[nx]);
    Mx_3x1 (cp, &Cp[nx], &xi[nx+1]);
    di[nx].m[1] =  ap[1] + bp[1] + cp[1] + Ri[nx].m[1]; 
    di[nx].m[2] =  ap[2] + bp[2] + cp[2] + Ri[nx].m[2]; 
    di[nx].m[3] =  ap[3] + bp[3] + cp[3] + Ri[nx].m[3]; 
  } 

//  di{nlevel} = Ap{nlevel}*xi{nl} + Bp{nlevel}*xi{nlevel} + Ri{nlevel};
  Mx_3x1 (ap, &Ap[(int)nlevel], &xi[(int)nl]);
  Mx_3x1 (bp, &Bp[(int)nlevel], &xi[(int)nlevel]);
  di[(int)nlevel].m[1] = ap[1] + bp[1] + Ri[(int)nlevel].m[1]; 
  di[(int)nlevel].m[2] = ap[2] + bp[2] + Ri[(int)nlevel].m[2]; 
  di[(int)nlevel].m[3] = ap[3] + bp[3] + Ri[(int)nlevel].m[3]; 


//%__________________________________________________________________________
//%
//% Solve for [temp psin rhov]' as a function of depth using the generalized 
//%   Thomas algorithm - Output Cell = xi{nlevel} = [temp psin rhov]' 
//%   at the j+1 time step
//%__________________________________________________________________________

//xi = GenThomas(Ai,Bi,Ci,di,nlevel);

//  if ( jstep == 2088 ) 
//    printf ("xi trashed \n"); 

  GenThomas(xi,Ai,Bi,Ci,di,nlevel);

 

#ifdef wwewerew
%--------------------------------------------------------------------------
% CN_Matrix_Pop.m
%--------------------------------------------------------------------------
%
% Now populate the arrays 
% Elements of (Ai, Bi, Ci) and (Ap, Bp, Cp) are 3x3 matrices 
% Elements of Ri, xi, and di are 1x3 column vectors
%
%**************************************************************************
%  [n = 1] Ai{n}, Bi{n}, Ci{n}, Ap{n}, Bp{n}, Cp{n}, Ri{n} 
%**************************************************************************

Ai{1} = [0 0 0 ; 0 0 0 ; 0 0 0];
Bi{1} = eye(3) + [B011 B012 B013 ; B021 B022 B023 ; B031 B032 B033];
Ci{1} = [C011 C012 C013 ; C021 C022 C023 ; C031 C032 C033];
Ap{1} = [0 0 0 ; 0 0 0 ; 0 0 0];
Bp{1} = eye(3) + [Bp011 Bp012 Bp013 ; Bp021 Bp022 Bp023 ; Bp031 Bp032 Bp033];
Cp{1} = [Cp011 Cp012 Cp013 ; Cp021 Cp022 Cp023 ; Cp031 Cp032 Cp033];
Ri{1} = [R01 ; R02 ; R03];

%**************************************************************************
%  [n = 2:nl] Ai{n}, Bi{n}, Ci{n}, Ap{n}, Bp{n}, Cp{n}, Ri{n} 
%**************************************************************************

for nn  = 2:nl
 Ai{nn} = [Ai11(nn) Ai12(nn) Ai13(nn) ; ... 
           Ai21(nn) Ai22(nn) Ai23(nn) ; ... 
           Ai31(nn) Ai32(nn) Ai33(nn)]; 
       
 Bi{nn} = eye(3) + [Bi11(nn) Bi12(nn) Bi13(nn) ; ... 
                    Bi21(nn) Bi22(nn) Bi23(nn) ; ...
                    Bi31(nn) Bi32(nn) Bi33(nn)];
       
 Ci{nn} = [Ci11(nn) Ci12(nn) Ci13(nn) ; ... 
           Ci21(nn) Ci22(nn) Ci23(nn) ; ...
           Ci31(nn) Ci32(nn) Ci33(nn)]; 
       
 Ap{nn} = [Api11(nn) Api12(nn) Api13(nn) ; ... 
           Api21(nn) Api22(nn) Api23(nn) ; ... 
           Api31(nn) Api32(nn) Api33(nn)]; 
       
 Bp{nn} = eye(3) + [Bpi11(nn) Bpi12(nn) Bpi13(nn) ; ... 
                    Bpi21(nn) Bpi22(nn) Bpi23(nn) ; ...
                    Bpi31(nn) Bpi32(nn) Bpi33(nn)];
       
 Cp{nn} = [Cpi11(nn) Cpi12(nn) Cpi13(nn) ; ... 
           Cpi21(nn) Cpi22(nn) Cpi23(nn) ; ...
           Cpi31(nn) Cpi32(nn) Cpi33(nn)]; 
       
 Ri{nn} = [Ri1(nn) ; Ri2(nn) ; Ri3(nn)];
end
 
%**************************************************************************
%  [n = nlevel] Ai{n}, Bi{n}, Ci{n}, Ap{n}, Bp{n}, Cp{n}, Ri{n} 
%**************************************************************************

Ai{nlevel} = [AM11 AM12 AM13 ; AM21 AM22 AM23 ; AM31 AM32 AM33];
Bi{nlevel} = eye(3) + [BM11 BM12 BM13 ; BM21 BM22 BM23 ; BM31 BM32 BM33];
Ci{nlevel} = [0 0 0 ; 0 0 0 ; 0 0 0];
Ap{nlevel} = [ApM11 ApM12 ApM13 ; ApM21 ApM22 ApM23 ; ApM31 ApM32 ApM33];
Bp{nlevel} = eye(3) + [BpM11 BpM12 BpM13 ; ... 
                       BpM21 BpM22 BpM23 ; ... 
                       BpM31 BpM32 BpM33];
Cp{nlevel} = [0 0 0 ; 0 0 0 ; 0 0 0];
Ri{nlevel} = [RM1 ; RM2 ; RM3];

%**************************************************************************
%  [n = 1:nlevel] xi(n)
%**************************************************************************

for ni     = 1:nlevel
  xi{ni}   = [temp(ni) ; psin(ni) ; rhov(ni)];
end

%**************************************************************************
%  [n = 1:nlevel] di(n)
%**************************************************************************

di{1}      = Bp{1}*xi{1} + Cp{1}*xi{2} + Ri{1};
for nx     = 2:nl
  di{nx}   = Ap{nx}*xi{nx-1} + Bp{nx}*xi{nx} + Cp{nx}*xi{nx+1} + Ri{nx};
end
di{nlevel} = Ap{nlevel}*xi{nl} + Bp{nlevel}*xi{nlevel} + Ri{nlevel};

%__________________________________________________________________________
%
% Solve for [temp psin rhov]' as a function of depth using the generalized 
%   Thomas algorithm - Output Cell = xi{nlevel} = [temp psin rhov]' 
%   at the j+1 time step
%__________________________________________________________________________

xi = GenThomas(Ai,Bi,Ci,di,nlevel);


#endif 