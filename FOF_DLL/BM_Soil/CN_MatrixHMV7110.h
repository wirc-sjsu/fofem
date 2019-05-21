

// extern double *thpsini;  
extern double delt; 
double BounH, BDeltr,BDelt,BDelta,BounT; 

  for ( i = 1; i <= nlevel; i++ ) {
    denlv[i] =  denw[i] * lv[i];
    Gama[i]   =  denlv[i] * csi[i];
    ktcon[i]  =  difcoef * (0.5 * delz * denlv[i] * dKHdT[i]);
    lacon[i]  =  adtcon[i] + ktcon[i];
    denthi[i] =  denwi[i] * thpsini[i];
    dlvl[i]   =  0.25 * delvel * thpsini[i] * dKHdT[i]; 
    xai[i]    =  1.0 / xa[i];
    xaida[i]  =  0.5 * delt * xai[i] * dSvdr[i];

    BounH  =  difcoef * (1.0 + lacon[1] / hcon[1]) * csi[1] * 2.0 *delz;
    BDeltr =  difcoef * (1.0 + advdif[1] / hvdif[1]) * 2.0 * delz;
    BDelt  =  BDeltr * CE * rh[1]; 
    BDelta =  BDelt + BDeltr * (CU-1) * u[1];
    BounT  =  dlvl[1] * 2.0 * delz / tcon[1];
    RV[i]  =  delt * xai[i] * sourcev[i];
 }

double B011a,B011,B012,B013,B021,B022,B023a,B023b,B023,B031,B032,B033;
   B011a =  (hcon[1] + hcon[2]) * csi[1];
   B011  =  B011a + BounH * RH;
// % ++ B012  = -Gama(1)*(thpsin(1)-hKn(1)+hKn(2)+0.5*delvel*dKHdp(1));
   B012  = -Gama[1] * (thpsin[1]+hKn[1]+hKn[2]);
   B013  =  BounH * RE;
   B021  = -BounT * RH;
// % ++ B022  =  (hKn(2)-hKn(1))*thpsini(1) + 0.5*delvel*dKHdt(1);
   B022  =  (hKn[2]+hKn[1])*thpsini[1];
   B023a =  xa[1] * denthi[1];
   B023b =  (hvdif[1]+hvdif[2]-BDelta)*denthi[1];
   B023  =  B023a + B023b - BounT*RE;
   B031  = -0.5 * delt * xai[1] * dSvdT[1];
   B032  = -0.5*xai[1]*(2.0*rhov[1]*thpsin[1]+delt*dSvdp[1]);
   B033  =  xai[1]*(hvdif[1]+hvdif[2]-BDelta) - xaida[1];

double Bp011,Bp012, Bp013, Bp021, Bp022, Bp023, Bp031, Bp032, Bp033;
   Bp011 = -B011a;
// % ++ Bp012 = -Gama(1)*(thpsin(1)+hKn(1)-hKn(2)-0.5*delvel*dKHdp(1));
   Bp012 = -Gama[1] * (thpsin[1]-hKn[1]-hKn[2]);
   Bp013 =  0;
   Bp021 =  0;
   Bp022 = -B022;
   Bp023 =  B023a - B023b;
   Bp031 =  B031;
   Bp032 =  B032;
   Bp033 = -xai[1]*(hvdif[1]+hvdif[2]-BDelta) - xaida[1];

double C011, C012, C013,C021,C022,C023,C031,C032,C033;
double Cp011, Cp012, Cp013, Cp021, Cp022, Cp023, Cp031,Cp032,Cp033;

C011  = -B011a;
// % ++ C012  = -Gama(1)*(hKn(1)-hKn(2)-0.5*delvel*dKHdp(1));
C012  =  Gama[1]*(hKn[1]+hKn[2]);
C013  =  0;
C021  =  0;
C022  = -B022;
C023  = -(hvdif[1]+hvdif[2]+advdif[1]-advdif[2])*denthi[1];
C031  =  0;
C032  =  0;
C033  = -xai[1]*(hvdif[1]+hvdif[2]+advdif[1]-advdif[2]);

Cp011 = -C011;
Cp012 = -C012;
Cp013 =  0;
Cp021 =  0;
Cp022 = -C022;
Cp023 = -C023;
Cp031 =  0;
Cp032 =  0;
Cp033 = -C033;

double R01,R02,R03;
R01   =  BounH*(G0+QH);
R02   = -BounT * (G0+QH)-denthi[1] * BDelt * (bcra[jstep+1] + bcra[jstep]);
R03   =  RV[1]-xai[1]*BDelt*(bcra[jstep+1]+bcra[jstep]);



//%**************************************************************************
//%  n = 2:nl
//%**************************************************************************
int nv; 
  
  for ( nv = 2; nv <= nl; nv++ ) {
   Ai11[nv]  = -(hcon[nv]+lacon[nv])*csi[nv];
   Ai12[nv]  =  Gama[nv]*(hKn[nv]-0.25*delvel*dKHdp[nv]);
   Ai13[nv]  =  0;
   Ai21[nv]  =  dlvl[nv];
   Ai22[nv]  = -hKn[nv]*thpsini[nv]+0.25*delvel*dKHdt[nv];
   Ai23[nv]  = -(hvdif[nv]+advdif[nv-1])*denthi[nv];
   Ai31[nv]  =  0;
   Ai32[nv]  =  0;
   Ai33[nv]  = -xai[nv]*(hvdif[nv]+advdif[nv-1]);
 
   Api11[nv] = -Ai11[nv];
   Api12[nv] = -Ai12[nv];
   Api13[nv] =  0;
   Api21[nv] = -Ai21[nv];
   Api22[nv] = -Ai22[nv];
   Api23[nv] = -Ai23[nv];
   Api31[nv] =  0;
   Api32[nv] =  0;
   Api33[nv] = -Ai33[nv];

Bi11[nv]  =  (hcon[nv]+hcon[nv+1])*csi[nv];
Bi12[nv]  = -Gama[nv]*(thpsin[nv]+hKn[nv]+hKn[nv+1]);
Bi13[nv]  =  0;
Bi21[nv]  =  0;
Bi22[nv]  =  (hKn[nv]+hKn[nv+1])*thpsini[nv];
Bi23[nv]  =  (xa[nv]+hvdif[nv]+hvdif[nv+1])*denthi[nv];
Bi31[nv]  = -0.5*delt*xai[nv]*dSvdT[nv];
Bi32[nv]  = -0.5*xai[nv]*(2.0*rhov[nv]*thpsin[nv]+delt*dSvdp[nv]);
Bi33[nv]  =  xai[nv]*(hvdif[nv]+hvdif[nv+1])-xaida[nv];

Bpi11[nv] = -Bi11[nv];
Bpi12[nv] = -Gama[nv]*(thpsin[nv]-hKn[nv]-hKn[nv+1]);
Bpi13[nv] =  0;
Bpi21[nv] =  0;
Bpi22[nv] = -Bi22[nv];
Bpi23[nv] =  (xa[nv]-hvdif[nv]-hvdif[nv+1])*denthi[nv];
Bpi31[nv] =  Bi31[nv];
Bpi32[nv] =  Bi32[nv];
Bpi33[nv] = -xai[nv]*(hvdif[nv]+hvdif[nv+1])-xaida[nv];

 Ci11[nv]  = -(hcon[nv+1]-lacon[nv])*csi[nv];
 Ci12[nv]  =  Gama[nv]*(hKn[nv+1]+0.25*delvel*dKHdp[nv]);
 Ci13[nv]  =  0;
 Ci21[nv]  = -Ai21[nv];
 Ci22[nv]  = -hKn[nv+1]*thpsini[nv]-0.25*delvel*dKHdt[nv];
 Ci23[nv]  = -(hvdif[nv+1]-advdif[nv+1])*denthi[nv];
 Ci31[nv]  =  0;
 Ci32[nv]  =  0;
 Ci33[nv]  = -xai[nv]*(hvdif[nv+1]-advdif[nv+1]); 
 
 Cpi11[nv] = -Ci11[nv];
 Cpi12[nv] = -Ci12[nv];
 Cpi13[nv] =  0;
 Cpi21[nv] = -Ci21[nv];
 Cpi22[nv] = -Ci22[nv];
 Cpi23[nv] = -Ci23[nv];
 Cpi31[nv] =  0;
 Cpi32[nv] =  0;
 Cpi33[nv] = -Ci33[nv];
 
 Ri1[nv]   =  0;
 Ri2[nv]   =  0;
 Ri3[nv]   =  RV[nv];
 } /* for n */

 

  
//%**************************************************************************
//%  n = nlevel
//%**************************************************************************

double AM11,AM12,AM13,AM21,AM22,AM23,AM31,AM32,AM33;
double ApM11,ApM12,ApM13,ApM21,ApM22,ApM23,ApM31,ApM32,ApM33;
double BM11,BM12,BM13,BM21,BM22,BM23,BM31,BM32,BM33;
double BpM11,BpM12,BpM13,BpM21,BpM22,BpM23,BpM31,BpM32,BpM33;
double RM1, RM2, RM3; 
double BM12a; 

int xlevel = nlevel; /* Compile wants an int for indexing arrays */
/* NOTE xlevel+1, hcon[] and a few other arrays are actually x/nlevel */
/*      plus 2 in size, so that index won't go out of bounds */ 

AM11  = -(hcon[xlevel]-hcon[xlevel+1]+2.0*ktcon[xlevel]) * csi[xlevel];
AM12  =  Gama[xlevel]*(hKn[xlevel]-hKn[xlevel+1]-0.5*delvel*dKHdp[xlevel]);
AM13  =  0;
AM21  =  2.0*dlvl[xlevel];
AM22  = -(hKn[xlevel]-hKn[xlevel+1])*thpsini[xlevel]+0.5*delvel*dKHdt[xlevel];
AM23  = -(hvdif[xlevel]-hvdif[xlevel+1]+advdif[(int)nl])*denthi[xlevel];
AM31  =  0;
AM32  =  0;
AM33  = -xai[xlevel] * (hvdif[xlevel]-hvdif[xlevel+1] + advdif[(int)nl] );

ApM11 = -AM11;
ApM12 = -AM12;
ApM13 =  0;
ApM21 = -AM21;
ApM22 = -AM22;
ApM23 = -AM23;
ApM31 =  0;
ApM32 =  0;
ApM33 = -AM33;

BM11  = -AM11;
BM12a =  hKn[xlevel]-hKn[xlevel+1]-0.5*delvel*dKHdp[xlevel];
BM12  = -Gama[xlevel]*(thpsin[xlevel]+BM12a);
BM13  =  0;
BM21  = -AM21; 
BM22  = -AM22;
BM23  =  (xa[xlevel]+hvdif[xlevel]-hvdif[xlevel+1])*denthi[xlevel];
BM31  = -0.5*delt*xai[xlevel]*dSvdT[xlevel];
BM32  = -0.5*xai[xlevel]*(2.0*rhov[xlevel]*thpsin[xlevel]+delt*dSvdp[xlevel]);
BM33  =  xai[xlevel]*(hvdif[xlevel]-hvdif[xlevel+1])-xaida[xlevel];

BpM11 =  AM11;
BpM12 = -Gama[xlevel]*(thpsin[xlevel]-BM12a);
BpM13 =  0;
BpM21 =  AM21; 
BpM22 =  AM22;
BpM23 =  (xa[xlevel]-hvdif[xlevel]+hvdif[xlevel+1])*denthi[xlevel];
BpM31 =  BM31;
BpM32 =  BM32;
BpM33 = -xai[xlevel]*(hvdif[xlevel]-hvdif[xlevel+1])-xaida[xlevel];

RM1   =  0;
RM2   =  0;
RM3   =  RV[xlevel];


#ifdef wowow
%--------------------------------------------------------------------------
% CN_MatrixHMV7110.m
%--------------------------------------------------------------------------
%
% This script file sets up and solves the matrix inversion for the 
%  tridiagonal Crank-Nicolson system 
%
%__________________________________________________________________________
%
% Begin with some preliminary computations to reduce model run time 
%__________________________________________________________________________

denlv  =  denw.*lv;
Gama   =  denlv.*csi;
ktcon  =  difcoef*(0.5*delz*denlv.*dKHdT);
lacon  =  adtcon + ktcon;
denthi =  denwi.*thpsini;
dlvl   =  0.25*delvel*thpsini.*dKHdT;
xai    =  1./xa;
xaida  =  0.5*delt*xai.*dSvdr;

BounH  =  difcoef*(1+lacon(1)/hcon(1))*csi(1)*2*delz;
BDeltr =  difcoef*(1+advdif(1)/hvdif(1))*2*delz;
BDelt  =  BDeltr*CE*rh(1); 
BDelta =  BDelt + BDeltr*(CU-1)*u(1);
BounT  =  dlvl(1)*2*delz/tcon(1);

RV     =  delt*xai.*sourcev;

%__________________________________________________________________________
%
% Calculate the array elements - from the top downward 
%__________________________________________________________________________

%**************************************************************************
%  n = 1
%**************************************************************************

B011a =  (hcon(1)+hcon(2))*csi(1);
B011  =  B011a + BounH*RH;
% ++ B012  = -Gama(1)*(thpsin(1)-hKn(1)+hKn(2)+0.5*delvel*dKHdp(1));
B012  = -Gama(1)*(thpsin(1)+hKn(1)+hKn(2));
B013  =  BounH*RE;
B021  = -BounT*RH;
% ++ B022  =  (hKn(2)-hKn(1))*thpsini(1) + 0.5*delvel*dKHdt(1);
B022  =  (hKn(2)+hKn(1))*thpsini(1);
B023a =  xa(1)*denthi(1);
B023b =  (hvdif(1)+hvdif(2)-BDelta)*denthi(1);
B023  =  B023a + B023b - BounT*RE;
B031  = -0.5*delt*xai(1)*dSvdT(1);
B032  = -0.5*xai(1)*(2*rhov(1)*thpsin(1)+delt*dSvdp(1));
B033  =  xai(1)*(hvdif(1)+hvdif(2)-BDelta) - xaida(1);

Bp011 = -B011a;
% ++ Bp012 = -Gama(1)*(thpsin(1)+hKn(1)-hKn(2)-0.5*delvel*dKHdp(1));
Bp012 = -Gama(1)*(thpsin(1)-hKn(1)-hKn(2));
Bp013 =  0;
Bp021 =  0;
Bp022 = -B022;
Bp023 =  B023a - B023b;
Bp031 =  B031;
Bp032 =  B032;
Bp033 = -xai(1)*(hvdif(1)+hvdif(2)-BDelta) - xaida(1);

C011  = -B011a;
% ++ C012  = -Gama(1)*(hKn(1)-hKn(2)-0.5*delvel*dKHdp(1));
C012  =  Gama(1)*(hKn(1)+hKn(2));
C013  =  0;
C021  =  0;
C022  = -B022;
C023  = -(hvdif(1)+hvdif(2)+advdif(1)-advdif(2))*denthi(1);
C031  =  0;
C032  =  0;
C033  = -xai(1)*(hvdif(1)+hvdif(2)+advdif(1)-advdif(2));

Cp011 = -C011;
Cp012 = -C012;
Cp013 =  0;
Cp021 =  0;
Cp022 = -C022;
Cp023 = -C023;
Cp031 =  0;
Cp032 =  0;
Cp033 = -C033;

R01   =  BounH*(G0+QH);
R02   = -BounT*(G0+QH)-denthi(1)*BDelt*(bcra(jstep+1)+bcra(jstep));
R03   =  RV(1)-xai(1)*BDelt*(bcra(jstep+1)+bcra(jstep));

%**************************************************************************
%  n = 2:nl
%**************************************************************************

for nv     =  2:nl
 Ai11(nv)  = -(hcon(nv)+lacon(nv))*csi(nv);
 Ai12(nv)  =  Gama(nv)*(hKn(nv)-0.25*delvel*dKHdp(nv));
 Ai13(nv)  =  0;
 Ai21(nv)  =  dlvl(nv);
 Ai22(nv)  = -hKn(nv)*thpsini(nv)+0.25*delvel*dKHdt(nv);
 Ai23(nv)  = -(hvdif(nv)+advdif(nv-1))*denthi(nv);
 Ai31(nv)  =  0;
 Ai32(nv)  =  0;
 Ai33(nv)  = -xai(nv)*(hvdif(nv)+advdif(nv-1));
 
 Api11(nv) = -Ai11(nv);
 Api12(nv) = -Ai12(nv);
 Api13(nv) =  0;
 Api21(nv) = -Ai21(nv);
 Api22(nv) = -Ai22(nv);
 Api23(nv) = -Ai23(nv);
 Api31(nv) =  0;
 Api32(nv) =  0;
 Api33(nv) = -Ai33(nv);
 
 Bi11(nv)  =  (hcon(nv)+hcon(nv+1))*csi(nv);
 Bi12(nv)  = -Gama(nv)*(thpsin(nv)+hKn(nv)+hKn(nv+1));
 Bi13(nv)  =  0;
 Bi21(nv)  =  0;
 Bi22(nv)  =  (hKn(nv)+hKn(nv+1))*thpsini(nv);
 Bi23(nv)  =  (xa(nv)+hvdif(nv)+hvdif(nv+1))*denthi(nv);
 Bi31(nv)  = -0.5*delt*xai(nv)*dSvdT(nv);
 Bi32(nv)  = -0.5*xai(nv)*(2*rhov(nv)*thpsin(nv)+delt*dSvdp(nv));
 Bi33(nv)  =  xai(nv)*(hvdif(nv)+hvdif(nv+1))-xaida(nv);
 
 Bpi11(nv) = -Bi11(nv);
 Bpi12(nv) = -Gama(nv)*(thpsin(nv)-hKn(nv)-hKn(nv+1));
 Bpi13(nv) =  0;
 Bpi21(nv) =  0;
 Bpi22(nv) = -Bi22(nv);
 Bpi23(nv) =  (xa(nv)-hvdif(nv)-hvdif(nv+1))*denthi(nv);
 Bpi31(nv) =  Bi31(nv);
 Bpi32(nv) =  Bi32(nv);
 Bpi33(nv) = -xai(nv)*(hvdif(nv)+hvdif(nv+1))-xaida(nv);
 
 Ci11(nv)  = -(hcon(nv+1)-lacon(nv))*csi(nv);
 Ci12(nv)  =  Gama(nv)*(hKn(nv+1)+0.25*delvel*dKHdp(nv));
 Ci13(nv)  =  0;
 Ci21(nv)  = -Ai21(nv);
 Ci22(nv)  = -hKn(nv+1)*thpsini(nv)-0.25*delvel*dKHdt(nv);
 Ci23(nv)  = -(hvdif(nv+1)-advdif(nv+1))*denthi(nv);
 Ci31(nv)  =  0;
 Ci32(nv)  =  0;
 Ci33(nv)  = -xai(nv)*(hvdif(nv+1)-advdif(nv+1)); 
 
 Cpi11(nv) = -Ci11(nv);
 Cpi12(nv) = -Ci12(nv);
 Cpi13(nv) =  0;
 Cpi21(nv) = -Ci21(nv);
 Cpi22(nv) = -Ci22(nv);
 Cpi23(nv) = -Ci23(nv);
 Cpi31(nv) =  0;
 Cpi32(nv) =  0;
 Cpi33(nv) = -Ci33(nv);
 
 Ri1(nv)   =  0;
 Ri2(nv)   =  0;
 Ri3(nv)   =  RV(nv);
end

%**************************************************************************
%  n = nlevel
%**************************************************************************

AM11  = -(hcon(nlevel)-hcon(nlevel+1)+2*ktcon(nlevel))*csi(nlevel);
AM12  =  Gama(nlevel)*(hKn(nlevel)-hKn(nlevel+1)-0.5*delvel*dKHdp(nlevel));
AM13  =  0;
AM21  =  2*dlvl(nlevel);
AM22  = -(hKn(nlevel)-hKn(nlevel+1))*thpsini(nlevel)+0.5*delvel*dKHdt(nlevel);
AM23  = -(hvdif(nlevel)-hvdif(nlevel+1)+advdif(nl))*denthi(nlevel);
AM31  =  0;
AM32  =  0;
AM33  = -xai(nlevel)*(hvdif(nlevel)-hvdif(nlevel+1)+advdif(nl));

ApM11 = -AM11;
ApM12 = -AM12;
ApM13 =  0;
ApM21 = -AM21;
ApM22 = -AM22;
ApM23 = -AM23;
ApM31 =  0;
ApM32 =  0;
ApM33 = -AM33;

BM11  = -AM11;
BM12a =  hKn(nlevel)-hKn(nlevel+1)-0.5*delvel*dKHdp(nlevel);
BM12  = -Gama(nlevel)*(thpsin(nlevel)+BM12a);
BM13  =  0;
BM21  = -AM21; 
BM22  = -AM22;
BM23  =  (xa(nlevel)+hvdif(nlevel)-hvdif(nlevel+1))*denthi(nlevel);
BM31  = -0.5*delt*xai(nlevel)*dSvdT(nlevel);
BM32  = -0.5*xai(nlevel)*(2*rhov(nlevel)*thpsin(nlevel)+delt*dSvdp(nlevel));
BM33  =  xai(nlevel)*(hvdif(nlevel)-hvdif(nlevel+1))-xaida(nlevel);

BpM11 =  AM11;
BpM12 = -Gama(nlevel)*(thpsin(nlevel)-BM12a);
BpM13 =  0;
BpM21 =  AM21; 
BpM22 =  AM22;
BpM23 =  (xa(nlevel)-hvdif(nlevel)+hvdif(nlevel+1))*denthi(nlevel);
BpM31 =  BM31;
BpM32 =  BM32;
BpM33 = -xai(nlevel)*(hvdif(nlevel)-hvdif(nlevel+1))-xaida(nlevel);

RM1   =  0;
RM2   =  0;
RM3   =  RV(nlevel);

%__________________________________________________________________________
%
% Now populate the arrays and solve using Generalized Thomas algorithm 
%__________________________________________________________________________

CN_Matrix_Pop
#endif 