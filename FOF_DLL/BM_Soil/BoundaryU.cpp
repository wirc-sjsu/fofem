/******************************************************************
* Name: BoundaryU.cpp  - 
*  ------->  file contains  BM's Aug 22 2017 version 
* Desc: See Matlab code at end of this file 
*******************************************************************/
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "BMsoil.h"

double js    [eC_nsteps];
double expt  [eC_nsteps];
double bcQ   [eC_nsteps];
double bcva  [eC_nsteps]; 
double bcta  [eC_nsteps]; 
double eta4  [eC_nsteps]; 
double bcra  [eC_nsteps]; 
double forIR  [eC_nsteps]; 
