/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: fof_se.h
* Desc: Soil Exp Simulation
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
/*                         SE  - Soil Exp Input Data (nonduff)              */
typedef struct {
  char cr_Name[50];   /* Soil Type Name                                      */

  float r_bd;          /* bd-soil bulk density - g/m3                     */
  float r_pd;          /* pd-soil particle density - g/m3                 */
  float r_xo;          /* xo-extrapolated water cont. at -1 J/kg          */
  float r_ls;          /* ls-thermal conductivity of mineral fraction     */
  float r_ga;          /* ga-de Vries shape factor                        */
  float r_xwo;         /* xwo-water content for liquid recirculation      */
  float r_cop;         /* cop-power for recirculation function            */
  int  i_dt;          /* dt-time step - s                                */
  float r_startwc;     /* startwc-starting soil water content - m3/m3     */
  float r_starttemp;   /* starttemp-starting soil temperatue - C          */

/* Layer and Display arrays,                                                 */
/* This get initalized before going into simulation, they tell the simulatn  */
/*  they define what the layers are and if it is to be outputed              */
/*  See where they get iniialized,                                           */
    float rr_z[e_mplus1+1];              /* Layer                             */
    float rr_node[e_mplus1+1];           /* 1 = Display, 0 = No Display       */

/* Soil Heating Fire Intensity Efficency percent - testing */
   float f_SoilWlEff;   /* Wood Litter, whole number percent  */
   float f_SoilHsEff;   /* Herb Shrub */
   float f_fiInt ;       /* seconds between fir intensity readings */

/* Fire Intensity arrays, used on nonduff user input  file */
#define e_FI 6000
   float frWL[e_FI+ 1];  /* Wood liter */
   float frHS[e_FI + 1];  /* Herb shrub */


} d_SE ;




int  SE_Init (d_SI *a_SI, d_SE *a_SE, char cr_ErrMes[]);
int  SE_Mngr (d_SE *a_SE, char cr_TmpFN[], char cr_ErrMes[]);
int  SE_Mngr_Array (d_SE *a_SE, float fr_FI[], float fr_FIhs[], int i_frInc, char cr_TmpFN[], char cr_ErrMes[]);

void SE_Disp (d_SE *a_SE);

float _Get_FirInt (int i_ClockSec, float fr[], int i_frInc);
