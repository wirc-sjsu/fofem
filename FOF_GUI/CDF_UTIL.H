/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: cdf_util.h        Comma Delimited File
* Desc:
*
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/


#define eC_ErrMes 3000


/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
#define  e_Cha   'c'
#define  e_Str    's'
#define  e_Int    'i'
#define  e_Lon    'l'
#define  e_Flo    'f'
#define  e_End    'x'


#define  e_Cha_Init    ' '
#define  e_Str_Init     ""
#define  e_Int_Init    -1
#define  e_Lon_Init    -1
#define  e_Flo_Init    -1


typedef struct  {
   char cr_Name[75];                    /* Name of field                     */
   char c_Type;                         /* type                              */
   int  i_Size;                         /* size only needed for strings      */
   void *adr;                           /* pointer to record field           */
}  d_CDF;



int  CDF_Get (d_CDF *a_CDF, FILE *fh, char cr_ErrMes[]);

void CDF_Test (void);
