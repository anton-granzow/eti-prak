#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#ifdef _OPENMP
#include <omp.h>
#endif

int SIZE = 128;

const float max_float = 100.0;

int matrix_mul(float **firstMatrix, float **secondMatrix, float **mult, int rowFirst, int columnFirst, int rowSecond, int columnSecond);
int matrix_mul2(float **firstMatrix, float **secondMatrix, float **mult, int rowFirst, int columnFirst, int rowSecond, int columnSecond);

int main(int argc, char *argv[]){
	int loops = 1;

	int opt;

	while((opt = getopt(argc, argv, "s:l:")) != EOF){
		switch(opt){
		case 's':
			SIZE = atoi(optarg);
			break;
		case 'l':
			loops = atoi(optarg);
			break;
		default:
			fprintf(stderr, "Usage: %s [-s] SIZE [-l] loop-iterations\n",
                           argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	
	float **matrix1 = (float **)malloc(SIZE*sizeof(float *));
	float **matrix2 = (float **)malloc(SIZE*sizeof(float *));
	float **matrixresult = (float **)malloc(SIZE*sizeof(float *));
	// float **matrixresult2 = (float **)malloc(SIZE*sizeof(float *));

	// float inhalt = 112.2;
	// float result = inhalt * inhalt;
	// for (int k = 0; k < SIZE; k++){
	// 	result += inhalt * inhalt;
	// }

	for(int i = 0; i<SIZE; i++){
		matrix1[i] = (float *)malloc(SIZE*sizeof(float));
		matrix2[i] = (float *)malloc(SIZE*sizeof(float));
		matrixresult[i] = (float *)malloc(SIZE*sizeof(float));
		// matrixresult2[i] = (float *)malloc(SIZE*sizeof(float));

	    for(int j = 0; j<SIZE; j++){
			matrix1[i][j] = (float)rand()/(float)(RAND_MAX/max_float);
			matrix2[i][j] = (float)rand()/(float)(RAND_MAX/max_float);
			// matrix1[i][j] = inhalt;
			// matrix2[i][j] = inhalt;
			matrixresult[i][j] = 0;
			// matrixresult2[i][j] = 0;


	    }
	}

	// do matrix mul couple of times
	int counter = 0;
	int mistakes = 0;
	float check;
	double elapsed_time[loops];
	// for(int i = 0; i < loops; i++){
	int i = 0;

	clock_t start = clock() ;
	matrix_mul(matrix1, matrix2, matrixresult, SIZE, SIZE, SIZE, SIZE);
	clock_t end = clock() ;
	elapsed_time[i] = (end-start)/(double)CLOCKS_PER_SEC ;
	// matrix_mul2(matrix1, matrix2, matrixresult2, SIZE, SIZE, SIZE, SIZE);
	// for(int k = 0; i<SIZE; i++){
	// 	for(int j = 0; j<SIZE; j++){
	// 		check = matrixresult2[i][j] - matrixresult[i][j];
	// 		if(check != 0) mistakes++;
	// 	}
	// }
	// }



	double min_time = elapsed_time[0];
	double max_time= elapsed_time[0];
	double avg = 0;
	for(int i = 0; i<loops; i++){
		if (elapsed_time[i] > max_time) max_time = elapsed_time[i];
		if (elapsed_time[i] <  min_time) min_time = elapsed_time[i];
		avg += elapsed_time[i];
	}
	//printf("min,max,avg\n");
	printf("%d,%f,%f,%f\n", SIZE, min_time, max_time, avg/(double)loops);
	// printf("mistakes: %d", mistakes);
}


int matrix_mul(float **firstMatrix, float **secondMatrix, float **mult, int rowFirst, int columnFirst, int rowSecond, int columnSecond){
    int i, j, k;

	#pragma omp target data map(from:mult[:SIZE][:SIZE]) map(to: firstMatrix[:SIZE][:SIZE]) map(to: secondMatrix[:SIZE][:SIZE]) \
								map(to: rowFirst) map(to: columnSecond) map(to: columnSecond)
	{
		// Multiplying matrix firstMatrix and secondMatrix and storing in array mult.
		#pragma omp target teams distribute parallel for collapse(2)
		for(i = 0; i < rowFirst; ++i)
		{
			for(j = 0; j < columnSecond; ++j)
			{
				mult[i][j] = firstMatrix[i][0] * secondMatrix[0][j];
				for(k=1; k<columnFirst; ++k)
				{
					mult[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
				}
			}
		}
	}
}

int matrix_mul2(float **firstMatrix, float **secondMatrix, float **mult, int rowFirst, int columnFirst, int rowSecond, int columnSecond){
    int i, j, k;

	// Multiplying matrix firstMatrix and secondMatrix and storing in array mult.
	for(i = 0; i < rowFirst; ++i)
	{
		for(j = 0; j < columnSecond; ++j)
		{
			mult[i][j] = firstMatrix[i][0] * secondMatrix[0][j];
			for(k=1; k<columnFirst; ++k)
			{
				mult[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
			}
		}
	}
}