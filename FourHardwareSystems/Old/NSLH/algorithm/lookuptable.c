#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lookuptable.h"

/**************************** Table  Global Variables *******************************/
float RowScale[4][ROWMAX] = {0};
float ColumnScale[4][COLMAX] = {0};
float TableData[4][ROWMAX][COLMAX] = {{0,0,0}};
int RowNum[4] = {0};		//Actual row numember user inputs
int ColNum[4] = {0};		//Actual column numember user inputs

int indexOfRow(int port,float rowdata);
int indexOfColumn(int port,float columndata);
/************************************************************************************/


////////////////////////////////////////////////////////////////////////////////////////////////
///<Function Descriptions:> According to input row value, column value and specified table,
///                         return a proper value after looking up table;
///<Parameter1:> specified ith table you will use.
///<Parameter1:> input row value.
///<Parameter1:>  input column value.
////////////////////////////////////////////////////////////////////////////////////////////////
float LookupTable(int port,float rowdata,float columndata)
{
	float result = 0;
	float temp[4];       //store 4 block-surrouding data whose indexes are (i,j),(i+1,j),(i,j+1),(i+1,j+),respectively.
	float percent[4];   //store 4 percentage value according to the differences between inputed row/column data and Row/Column Scale
	int rowth = indexOfRow(port,rowdata);
	int columnth = indexOfColumn(port,columndata);
	if(rowth>=0 && columnth>=0)
		{
			temp[0] = TableData[port][rowth][columnth];
	    temp[1] = TableData[port][rowth+1][columnth];
	    temp[2] = TableData[port][rowth][columnth+1];
	    temp[3] = TableData[port][rowth+1][columnth+1];

	    percent[0] = (rowdata-RowScale[port][rowth])/(RowScale[port][rowth+1]-RowScale[port][rowth]);
	    percent[1] = (RowScale[port][rowth+1]-rowdata)/(RowScale[port][rowth+1]-RowScale[port][rowth]);
	    percent[2] = (columndata-ColumnScale[port][columnth])/(ColumnScale[port][columnth+1]-ColumnScale[port][columnth]);
	    percent[3] = (ColumnScale[port][columnth+1]-columndata)/(ColumnScale[port][columnth+1]-ColumnScale[port][columnth]);

	    result = percent[0]*percent[2]*temp[0] + percent[1]*percent[2]*temp[1] + percent[0]*percent[3]*temp[2] + percent[1]*percent[3]*temp[3];
  	}
	return	result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
///<Function Descriptions:> Fill the data of the specified table to be used in LUT Library Function.
///<Parameter1:> the ith table to be filled.
///<Parameter2:> the actual row number in the table.
///<Parameter3:> the actual column number in the table.
///<Parameter4:> the pointer pointing the memory which stores RowScale of this table.
///<Parameter5:> the pointer pointing the memory which stores ColumnScale of this table.
///<Parameter6:>
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void FillTableData(int port,int rownum,int colnum,float* rowScale,float* columnScale,float** tableData)
{
	int i,j;
  port = port - 1; 
	RowNum[port] = rownum;
  ColNum[port] = colnum;
	
	for(i=0;i<rownum;i++)
	{
		RowScale[port][i] = rowScale[i];
	}
	for(j=0;j<colnum;j++)
	{
		ColumnScale[port][j] = columnScale[j];
	}
	for(i=0;i<rownum;i++)
	{
		for(j=0;j<colnum;j++)
		{
			TableData[port][i][j] = tableData[i][j];
	  }
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
///<Function Description:> According to the input value and given scales, find out the position 
///						   of input value in the choosen table -- ith row, jth column;
///<Parameter1:>  the ith table you choose to use
///<Parameter2:>  the input row or column value
///////////////////////////////////////////////////////////////////////////////////////////////
int indexOfRow(int port,float rowdata)
{
	int i = 0;
  if(rowdata < RowScale[port][RowNum[port]-1] && rowdata >= RowScale[port][0])
		{
			for(i=0;i<RowNum[port]-1;i++)
      {
				if(rowdata >= RowScale[port][i] && rowdata < RowScale[port][i+1])
					return i;
	    }
   }
  return -1;
}

int indexOfColumn(int port,float columndata)
{
 // if( B[j] <= columndata < B[j+1])  return j;
	int i = 0;
 
  if(columndata < ColumnScale[port][ColNum[port]-1] && columndata >= ColumnScale[port][0])
		{
			for(i=0;i<ColNum[port]-1;i++)
      {
				if(columndata >= ColumnScale[port][i] && columndata < ColumnScale[port][i+1])
					return i;
			}
   }
  return -1;
}
