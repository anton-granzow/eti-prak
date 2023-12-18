#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const float max_float = 100.0;
int matrix_mul(float **firstMatrix, float **secondMatrix, float **mult, int rowFirst, int columnFirst, int rowSecond, int columnSecond){
    int i, j, k;
    int counter = 0;

	// Initializing elements of matrix mult to 0.
	for(i = 0; i < rowFirst; ++i)
	{
		for(j = 0; j < columnSecond; ++j)
		{
			mult[i][j] = 0;
		}
	}

	// Multiplying matrix firstMatrix and secondMatrix and storing in array mult.
	for(i = 0; i < rowFirst; ++i)
	{
		for(j = 0; j < columnSecond; ++j)
		{
			mult[i][j] = firstMatrix[i][0] * secondMatrix[0][j];
			counter ++;
			for(k=1; k<columnFirst; ++k)
			{
				mult[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
				counter += 2;
			}
		}
	}
	return counter;
}

int main(int argc, char *argv[]){
	int SIZE=128;
	//for(int i; i<argc; i++){
	//    if(argv[i]== "-SIZE"){
	//	if(i+1 == argc) return 1;
	//	else SIZE = atoi(argv[i+1]);
	//    }
	//}
	//if(SIZE == 0) return 1;
	// float matrix1[SIZE][SIZE];
	// float matrix2[SIZE][SIZE];
	// float matrixresult[SIZE][SIZE];
	float **matrix1 = (float **)malloc(SIZE*sizeof(float));
	float **matrix2 = (float **)malloc(SIZE*sizeof(float));
	float **matrixresult = (float **)malloc(SIZE*sizeof(float));
	for(int i = 0; i<SIZE; i++){
	    for(int j = 0; j<SIZE; j++){
		matrix1[i] = (float *)malloc(SIZE*sizeof(float));
		matrix2[i] = (float *)malloc(SIZE*sizeof(float));
		matrixresult[i] = (float *)malloc(SIZE*sizeof(float));

		matrix1[i][j] = (float)rand()/(float)(RAND_MAX/max_float);
		matrix2[i][j] = (float)rand()/(float)(RAND_MAX/max_float);
		matrixresult[i][j] = 0;
		// matrix1[i][j] = 2.0;
		// matrix2[i][j] = 2.0;


	    }
	}

	int counter = 0;
	double elapsed_time[200];
	for(int i = 0; i < 200; i++){
	    clock_t start = clock() ;
	    matrix_mul(matrix1, matrix2, matrixresult, SIZE, SIZE, SIZE, SIZE);
	    clock_t end = clock() ;
	    elapsed_time[i] = (end-start)/(double)CLOCKS_PER_SEC ;
	}
	//printf("FP-Operationen: %d", counter);
	double min_time = elapsed_time[0];
	double max_time= elapsed_time[0];
	double avg = 0;
	for(int i = 0; i<200; i++){
		//printf("%f, ", elapsed_time[i]);
		if (elapsed_time[i] > max_time) max_time = elapsed_time[i];
		if (elapsed_time[i] <  min_time) min_time = elapsed_time[i];
		avg += elapsed_time[i];
	}
	printf("min,max,avg\n");
	printf("%f,%f,%f\n", min_time, max_time, avg/200);
//   printf("matrix 1: \n");
//   for(int i = 0; i<SIZE; i++){
//       printf("\n");
//       for(int j =0; j<SIZE; j++){
//           printf("%f ", matrix1[i][j]);
//       }
//   }
//   printf("\nmatrix 1: \n");
//   for(int i = 0; i<SIZE; i++){
//       printf("\n");
//       for(int j =0; j<SIZE; j++){
//           printf("%f ", matrix2[i][j]);
//       }
//   }
//   printf("\nmatrix1 * matrix2 = \n");
//   for(int i = 0; i<SIZE; i++){
//       printf("\n");
//       for(int j =0; j<SIZE; j++){
//           printf("%f ", matrixresult[i][j]);
//       }
//   }
    

}
