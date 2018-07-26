#ifndef _LOOKUPTABLE
#define _LOOKUPTABLE

#define ROWMAX 25
#define COLMAX 25
#define DATASIZE  ROWMAX*COLMAX

extern float RowScale[4][ROWMAX];
extern float ColumnScale[4][COLMAX];
extern float TableData[4][ROWMAX][COLMAX];
extern int RowNum[4];		//Actual row numember user inputs
extern int ColNum[4];		//Actual column numember user inputs
float LookupTable(int port,float rowdata,float columndata);
void FillTableData(int port,int rownum,int colnum,float* rowScale,float* columnScale,float** tableData);

#endif /*_LOOKUPTABLE_*/
