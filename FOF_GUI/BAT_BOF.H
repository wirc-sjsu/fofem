


void  BOF_Close  (void);
int   BOF_Open (char cr_OutFN[]);
int   BOF_Put (char cr_StdId[], d_CO *a_CO, d_CI *a_CI, int iS_Soil, d_SO *a_SO);

void  BOF_PutMort(char cr_StdId[]);

int  BOF_PutHeading (int iS_Soil, char cr_EmiFla[], char cr_EmiFull[], char cr_UseEqu[]);
int  BOF_MortHeading (void);
