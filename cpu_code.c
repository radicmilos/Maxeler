#include <stdlib.h>
#include "DFE.h" 			// Includes .max files
//#include "DFE2.h"
#include <MaxSLiCInterface.h>
#include <stdio.h>
#include <math.h>


void intel_sum(int num_row, int num_col, int *matrix, int *sum_rows, int *sum_cols)
{
    int i,j; 
    for (i = 0; i < num_row; ++i)
    {
      sum_rows[i] = 0;     
      for (j = 0; j < num_col; ++j)
      {
          sum_rows[i] += matrix[i * num_col + j];
      }
    }
    for (i = 0; i < num_col; ++i)
    {
        sum_cols[i] = 0;
        for (j = 0; j< num_row; ++j)
        {
            sum_cols[i] += matrix[j * num_col + i];
        }
    }
}

int main()
{
   int num_row, num_col, *matrix, *matrix2, *sum_rows, *sum_cols, i, nni, nnj, df, *sum_cols_copy, *chisq_sum;
   int minij, j;
   long sum = 0;
   float chisq, expctd, temp, cramrv, ccc;
   time_t time1, time2;
   num_row = 160;
   num_col = 160;
   nni = 0;
   nnj = 0;
   matrix = (int *)malloc(num_row * num_col * sizeof(int));
   matrix2 = (int *)malloc(num_row * num_col * sizeof(int));
   sum_rows = (int *)malloc(num_row * sizeof(int));
   sum_cols = (int *)malloc(num_col * sizeof(int));
   //sum_cols_copy = (int *)malloc(num_row * num_col * sizeof(int));
   //chisq_sum = (int *)malloc(num_col * sizeof(int));
  
   for (int i = 0; i < num_row; ++i)
     for (int j = 0; j < num_col; ++j)
       matrix[i * num_row + j] = matrix2[j * num_col + i] = 2;
       
   time1 = time(0);
   intel_sum(num_row, num_col, matrix, sum_rows, sum_cols);
   time1 = time(0) - time1;
   time2 = time(0);
   DFE_sum(num_row, num_col, matrix, matrix2, sum_rows, sum_cols);
   time2 = time(0) - time2;

   for (i = 0; i < num_row; ++i)
   {
	   if (sum_rows[i] != 0)
		   ++nni;
	   sum += sum_rows[i];
   } 
   for (i = 0; i < num_col; ++i)
   {
	   if (sum_cols[i] != 0)
		 ++nnj;
   }
   //for (i = 0; i < num_row; ++i)
	   //memcpy(sum_cols_copy + (i * num_col), sum_cols, num_col);
   
   df=nni*nnj-nni-nnj+1;
   chisq=0.0;
      
    for (i=0;i<num_row;i++) { 
      for (j=0;j<num_col;j++) {
           expctd=(float)(sum_cols[j]*sum_rows[i])/(float)sum;
           temp=matrix[i * num_col + j]-expctd;
           chisq += temp*temp/(expctd); 
           }
      }
    minij = nni < nnj ? nni-1 : nnj-1;
    cramrv=sqrt(chisq/(sum*minij));
    ccc=sqrt(chisq/(chisq+sum));   
    
    printf("%d %d\n", time1, time2);
   
   //DFE2_calc(sum, num_row, num_col, matrix, sum_rows, sum_cols_copy, chisq_sum);
   return 0;
}