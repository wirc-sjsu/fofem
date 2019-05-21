#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

int load (char cr[]);

void   LoadQuincy1()
{

load("QY1_05");
load("QY1_15");
load("QY1_25");
load("QY1_35");
load("QY1_65");
load("QY1_95");


} 

/****************************************************************/
int load (char cr[])
{

 return 1; 
}


#ifdef WWWWW
%--------------------------------------------------------------------------
%                          LoadQuincy1.m                               
%--------------------------------------------------------------------------

load('QY1_05')
load('QY1_15')
load('QY1_25')
load('QY1_35')
load('QY1_65')
load('QY1_95')

lzero = false(1,1);

tempx  = 500;
Tvect  = [0 100 200 300 400 500];
thetax = 0.20;
% 2012 Newton-Raphson paper thetax = 0.30;
Hvect  = [0 0.05 0.10 0.15 0.20];
hvect  = [-0.04 0 0.04 0.08 0.12 0.16 0.20 0.24];
Swmin  = 0.60;
Svect  = [0.60 0.65 0.70 0.75 0.80 0.85 0.90 0.95 1];
LEx    = 5;
Lvect  = [0 1 2 3 4 5];
rhovvx = 3;
rvect  = [0 0.5 1 1.5 2 2.5 3];
% rhovvx = 2;
% rvect  = [0 0.5 1 1.5 2];
% thetax = 0.30; ! 2012 2-variable Model 
presvx = 5;
pvect  = [0 0.5 1 1.5 2 2.5 3 3.5 4 4.5 5];
% presvx = 4;
% pvect  = [0 0.5 1 1.5 2 2.5 3 3.5 4];
thetam = -0.10;
templ  = 20;
tempu  = 180;
thetal = 0; 
thetau = 0.25;

Toff   = 0;
Time05 = QY1_05(:,1)/60;
Time15 = QY1_15(:,1)/60;
Time25 = QY1_25(:,1)/60;
Time35 = QY1_35(:,1)/60;
Time65 = QY1_65(:,1)/60;
Time95 = QY1_95(:,1)/60;

Temp05 = QY1_05(:,2);
Temp15 = QY1_15(:,2);
Temp25 = QY1_25(:,2);
Temp35 = QY1_35(:,2);
Temp65 = QY1_65(:,2);
Temp95 = QY1_95(:,2);

Thet05 = QY1_05(:,3);
Thet15 = QY1_15(:,3);
Thet25 = QY1_25(:,3);
Thet35 = QY1_35(:,3);
Thet65 = QY1_65(:,3);
Thet95 = QY1_95(:,3);

RescaleQuincy1

TitleTemp = 'Quincy Sand -- Temperature Comparisons';
TitleThet = 'Quincy Sand -- Soil Moisture Comparisons';

TitleMPhase = 'Quincy Sand -- Model';
TitleOPhase = 'Quincy Sand -- Observed';

#endif 
