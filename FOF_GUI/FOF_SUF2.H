
  This file is obsolete
  when we implemented the new version of the soil input files

/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_suf2.h
* Desc: See comments below
* Date:  4/7/05
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{**/



/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/*                             Soil Input Struct                             */
/* Contains actual switch text to be matched up with input file switches     */
/*  read in values are then stored in the f_Val field                        */
typedef struct {
   char cr_Sw[50];
   float f_Val; }
 d_USI ;

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* Duff Sim Switches                                                         */
/* The text is the actual text for the swithes that are in the user input    */
/*  Duff files, as each switch is read in its' values is stored in the       */
/*  f_Val field                                                              */
/*                                                                           */
d_USI sr_USID[] = {
//    {   "midburn",               -1 },
//    {   "burn-time",             -1 },
//    {   "stop-time",             -1 },
   
    {   "duff-weight",           -1 },
    {   "duff-depth",            -1 },
    {   "duff-consumed",         -1 },
    {   "duff-moisture",         -1 },


    {   "soil-bulk-density",     -1 },
    {   "soil-particle-density", -1 },
    {   "extrapolated-water",    -1 },
    {   "thermal-conductivity",  -1 },
    {   "de-Vries-shape-factor", -1 },
    {   "water-content",         -1 },
    {   "recirculation",         -1 },
    {   "time-step",             -1 },
    {   "start-soil-water",      -1 },
    {   "start-soil-temp",       -1 },
    {   "",                      -1 } } ;


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/* Exp Heat Switches                                                         */
/*  See the notes above for the Duff struct                                  */

d_USI sr_USIE[] = {
  {"start-time",      -1}, /* starting time                                  */
  {"burn-time",       -1}, /* burntime-time from sim start to fire off - min */
  {"cool-time",       -1}, /* cooltime-time from start to end of simulation  */
  {"maxrabs",         -1}, /* maxrabs-maximum radiant heat input to surface  */
  {"heatconst-time",  -1}, /* heatconst-time constant for heating, minutes   */
  {"coolconst-time",  -1}, /* coolconst-time constant for cooling, minutes   */
  {"bulk-density",    -1}, /* bd-soil bulk density - g/m3                    */
  {"particle-density",-1}, /* pd-soil particle density - g/m3                */
  {"extrap-water",    -1}, /* xo-extrapolated water cont. at -1 J/kg         */
  {"thermal-conduct", -1}, /* ls-thermal conductivity of mineral fraction    */
  {"Vries-shape",     -1}, /* ga-de Vries shape factor                       */
  {"water-content",   -1}, /* xwo-water content for liquid recirculation     */
  {"cop-power",       -1}, /* cop-power for recirculation function           */
  {"dt-time-step",    -1}, /* dt-time step - s                               */
  {"start-water",     -1}, /* startwc-starting soil water content - m3/m3    */
  {"start-temp",      -1}, /* starttemp-starting soil temperatue - C         */
  {"",                -1}} ;
