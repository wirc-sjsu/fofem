/***************************************************************************
* Name: fof_se2.h
* Desc: Non Duff Soil Model initial values
* NOTE: not all of the values get used any more, I should gut out the 
*        unused stuff at some point. 
***************************************************************************/

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/*                         EID - Exp Input Data                              */
d_SE  sr_SE[] = {
/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
 {   e_FinSil,    /* Fine Silt  */
    1.3e6,            /* soil bulk density - g/m3                     */
    2.35e6,           /* soil particle density - g/m3                 */
    0.207,            /* extrapolated water cont. at -1 J/kg          */
    2.31,             /* thermal conductivity of mineral fraction     */
    0.071,            /* de Vries shape factor                        */
    0.148,            /* water content for liquid recirculation       */
    4.14,             /* power for recirculation function             */
    10,               /* time step - s                                */
    -1,               /* starting soil water content - m3/m3          */
    e_StaSoiTem },    /* starting soil temperatue - C                 */

/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
 {   e_LoaSke,    /* Loamy-Skeletal   */
    0.8e6,
    2.13e6,
    0.321,
    1.03,
    0.13,
    0.133,
    6.08,
    10,
    -1,
    e_StaSoiTem },

/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
 {   e_Fin,       /* Fine       */
    1.15e6,
    2.35e6,
    0.202,
    2.21,
    0.084,
    0.152,
    4.63,
    10,
    -1,
    e_StaSoiTem },

/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
 {   e_CoaSil,    /* Coarse Silt      */
    1.23e6,
    2.35e6,
    0.157,
    2.53,
    0.103,
    0.218,
    3.43,
    10,
    -1,
    e_StaSoiTem },

/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
 {   e_CoaLoa,    /* Coarse Loam  */
    1.3e6,
    2.35e6,
    0.102,
    2.57,
    0.106,
    0.127,
    2.93,
    10,
    -1,
    e_StaSoiTem },

/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
 {  "", -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 } };
