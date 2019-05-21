int _Get_DGFlo (DataGridView ^dg, int row, int col, float *af, char cr_Out[], char cr_Name[], char cr_ErrMes[]);

void GetDate(char cr[]);
 void _PutTextBox (TextBox ^ tb, char cr[]);
String ^  _FloToStr (float f, int i_Per);
int _SetComboBox (ComboBox ^cb, String ^Str);
String ^ _GetComboBox (ComboBox ^cb, char cr[]);
String ^  _CharToStr (char cr[]);
int  _StrToChar (String ^Str, char cr_To[]);


void  myMessageBox ( char cr[]);


void FloatToTextBox (TextBox^ a, float f, int i_Per);

int _Get_TBFlo (TextBox^ a, float *af, char cr_Out[], char cr_Name[], char cr_ErrMes[]);
int _GetTextBox (TextBox ^ tb, char cr[]);

int _MakeFileName (char cr_In[], char cr_Out[], char cr_Add[]);

float   _RndEmiPnd(float f);

void _CBI_Load_DGV (DataGridViewComboBoxColumn ^ cb, char A[], char B[], char C[], int iX);