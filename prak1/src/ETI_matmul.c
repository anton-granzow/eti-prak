#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>

#define BLOCK_SIZE 32
int SIZE = 512;

const float max_float = 100.0;

int matrix_mul(float **firstMatrix, float **secondMatrix, float **mult, int rowFirst, int columnFirst, int rowSecond, int columnSecond);
int matrix_mul_unroll(float **firstMatrix, float **secondMatrix, float **mult, int rowFirst, int columnFirst, int rowSecond, int columnSecond);
int matrix_mul_tiling(float **firstMatrix, float **secondMatrix, float **mult, int rowFirst, int columnFirst, int rowSecond, int columnSecond);

int main(int argc, char *argv[]){
	int loops = 100;

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
	for(int i = 0; i<SIZE; i++){
		matrix1[i] = (float *)malloc(SIZE*sizeof(float));
		matrix2[i] = (float *)malloc(SIZE*sizeof(float));
		matrixresult[i] = (float *)malloc(SIZE*sizeof(float));

	    for(int j = 0; j<SIZE; j++){
		matrix1[i][j] = (float)rand()/(float)(RAND_MAX/max_float);
		matrix2[i][j] = (float)rand()/(float)(RAND_MAX/max_float);
		matrixresult[i][j] = 0;


	    }
	}

	// Initializing elements of matrix mult to 0.
	for(int i = 0; i < SIZE; ++i)
	{
		for(int j = 0; j < SIZE; ++j)
		{
			matrixresult[i][j] = 0;
		}
	}
	
	// do matrix mul couple of times
	int counter = 0;
	double elapsed_time[loops];
	for(int i = 0; i < loops; i++){
	    clock_t start = clock() ;
	    matrix_mul_tiling(matrix1, matrix2, matrixresult, SIZE, SIZE, SIZE, SIZE);
	    clock_t end = clock() ;
	    elapsed_time[i] = (end-start)/(double)CLOCKS_PER_SEC ;
	}

	double min_time = elapsed_time[0];
	double max_time= elapsed_time[0];
	double avg = 0;
	for(int i = 0; i<loops; i++){
		if (elapsed_time[i] > max_time) max_time = elapsed_time[i];
		if (elapsed_time[i] <  min_time) min_time = elapsed_time[i];
		avg += elapsed_time[i];
	}
	//printf("min,max,avg\n");
	printf("SIZE: %d\n min_time: %f\n max_time: %f\n avg_time: %f\n", SIZE, min_time, max_time, avg/(double)loops);
}

int matrix_mul(float **firstMatrix, float **secondMatrix, float **mult, int rowFirst, int columnFirst, int rowSecond, int columnSecond){
    int i, j, k;
    int counter = 0;

	// Multiplying matrix firstMatrix and secondMatrix and storing in array mult.
	for(i = 0; i < rowFirst; ++i)
	{
		for(j = 0; j < columnSecond; ++j)
		{
			for(k=0; k<columnFirst; ++k)
			{
				mult[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
			}
		}
	}
	return counter;
}


// optimized matrix mult (loop unrolling)
int matrix_mul_unroll(float **firstMatrix, float **secondMatrix, float **mult, int rowFirst, int columnFirst, int rowSecond, int columnSecond){
    int i, j, k;
    int counter = 0;

	// Multiplying matrix firstMatrix and secondMatrix and storing in array mult.
	for(i = 0; i < rowFirst; ++i)
	{
		for(j = 0; j < columnSecond; ++j)
		{
			for(k=0; k<columnFirst; k+=32)
			{
				// unroll 32x
				mult[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
				mult[i][j] += firstMatrix[i][k+1] * secondMatrix[k+1][j];
				mult[i][j] += firstMatrix[i][k+2] * secondMatrix[k+2][j];
				mult[i][j] += firstMatrix[i][k+3] * secondMatrix[k+3][j];
				mult[i][j] += firstMatrix[i][k+4] * secondMatrix[k+4][j];
				mult[i][j] += firstMatrix[i][k+5] * secondMatrix[k+5][j];
				mult[i][j] += firstMatrix[i][k+6] * secondMatrix[k+6][j];
				mult[i][j] += firstMatrix[i][k+7] * secondMatrix[k+7][j];

				mult[i][j] += firstMatrix[i][k+8] * secondMatrix[k+8][j];
				mult[i][j] += firstMatrix[i][k+9] * secondMatrix[k+9][j];
				mult[i][j] += firstMatrix[i][k+10] * secondMatrix[k+10][j];
				mult[i][j] += firstMatrix[i][k+11] * secondMatrix[k+11][j];
				mult[i][j] += firstMatrix[i][k+12] * secondMatrix[k+12][j];
				mult[i][j] += firstMatrix[i][k+13] * secondMatrix[k+13][j];
				mult[i][j] += firstMatrix[i][k+14] * secondMatrix[k+14][j];
				mult[i][j] += firstMatrix[i][k+15] * secondMatrix[k+15][j];

				mult[i][j] += firstMatrix[i][k+16] * secondMatrix[k+16][j];
				mult[i][j] += firstMatrix[i][k+17] * secondMatrix[k+17][j];
				mult[i][j] += firstMatrix[i][k+18] * secondMatrix[k+18][j];
				mult[i][j] += firstMatrix[i][k+19] * secondMatrix[k+19][j];
				mult[i][j] += firstMatrix[i][k+20] * secondMatrix[k+20][j];
				mult[i][j] += firstMatrix[i][k+21] * secondMatrix[k+21][j];
				mult[i][j] += firstMatrix[i][k+22] * secondMatrix[k+22][j];
				mult[i][j] += firstMatrix[i][k+23] * secondMatrix[k+23][j];

				mult[i][j] += firstMatrix[i][k+24] * secondMatrix[k+24][j];
				mult[i][j] += firstMatrix[i][k+25] * secondMatrix[k+25][j];
				mult[i][j] += firstMatrix[i][k+26] * secondMatrix[k+26][j];
				mult[i][j] += firstMatrix[i][k+27] * secondMatrix[k+27][j];
				mult[i][j] += firstMatrix[i][k+28] * secondMatrix[k+28][j];
				mult[i][j] += firstMatrix[i][k+29] * secondMatrix[k+29][j];
				mult[i][j] += firstMatrix[i][k+30] * secondMatrix[k+30][j];
				mult[i][j] += firstMatrix[i][k+31] * secondMatrix[k+31][j];
				//counter += 2;
			}
		}
	}
	return counter;
}

int matrix_mul_tiling(float **firstMatrix, float **secondMatrix, float **mult, int rowFirst, int columnFirst, int rowSecond, int columnSecond){
    int i, j, k;
    int counter = 0;

	// iterate through the blocks
	for (int i = 0; i < SIZE; i += BLOCK_SIZE) {
		for (int j = 0; j < SIZE; j += BLOCK_SIZE) {
			for (int k = 0; k < SIZE; k+= BLOCK_SIZE) {


				// do matrix multiplication on block
				for (int ii = i; ii < i + BLOCK_SIZE; ++ii) {
					for (int jj = j; jj < j + BLOCK_SIZE; ++jj) {
						for (int kk = k; kk < k + BLOCK_SIZE; ++kk) {
							mult[ii][jj] += firstMatrix[ii][kk] * secondMatrix[kk][jj];
							mult[ii][jj] += firstMatrix[ii][kk+1] * secondMatrix[kk+1][jj];
							mult[ii][jj] += firstMatrix[ii][kk+2] * secondMatrix[kk+2][jj];
							mult[ii][jj] += firstMatrix[ii][kk+3] * secondMatrix[kk+3][jj];
							mult[ii][jj] += firstMatrix[ii][kk+4] * secondMatrix[kk+4][jj];
							mult[ii][jj] += firstMatrix[ii][kk+5] * secondMatrix[kk+5][jj];
							mult[ii][jj] += firstMatrix[ii][kk+6] * secondMatrix[kk+6][jj];
							mult[ii][jj] += firstMatrix[ii][kk+7] * secondMatrix[kk+7][jj];

							mult[ii][jj] += firstMatrix[ii][kk+8] * secondMatrix[kk+8][jj];
							mult[ii][jj] += firstMatrix[ii][kk+9] * secondMatrix[kk+9][jj];
							mult[ii][jj] += firstMatrix[ii][kk+10] * secondMatrix[kk+10][jj];
							mult[ii][jj] += firstMatrix[ii][kk+11] * secondMatrix[kk+11][jj];
							mult[ii][jj] += firstMatrix[ii][kk+12] * secondMatrix[kk+12][jj];
							mult[ii][jj] += firstMatrix[ii][kk+13] * secondMatrix[kk+13][jj];
							mult[ii][jj] += firstMatrix[ii][kk+14] * secondMatrix[kk+14][jj];
							mult[ii][jj] += firstMatrix[ii][kk+15] * secondMatrix[kk+15][jj];

							mult[ii][jj] += firstMatrix[ii][kk+16] * secondMatrix[kk+16][jj];
							mult[ii][jj] += firstMatrix[ii][kk+17] * secondMatrix[kk+17][jj];
							mult[ii][jj] += firstMatrix[ii][kk+18] * secondMatrix[kk+18][jj];
							mult[ii][jj] += firstMatrix[ii][kk+19] * secondMatrix[kk+19][jj];
							mult[ii][jj] += firstMatrix[ii][kk+20] * secondMatrix[kk+20][jj];
							mult[ii][jj] += firstMatrix[ii][kk+21] * secondMatrix[kk+21][jj];
							mult[ii][jj] += firstMatrix[ii][kk+22] * secondMatrix[kk+22][jj];
							mult[ii][jj] += firstMatrix[ii][kk+23] * secondMatrix[kk+23][jj];

							mult[ii][jj] += firstMatrix[ii][kk+24] * secondMatrix[kk+24][jj];
							mult[ii][jj] += firstMatrix[ii][kk+25] * secondMatrix[kk+25][jj];
							mult[ii][jj] += firstMatrix[ii][kk+26] * secondMatrix[kk+26][jj];
							mult[ii][jj] += firstMatrix[ii][kk+27] * secondMatrix[kk+27][jj];
							mult[ii][jj] += firstMatrix[ii][kk+28] * secondMatrix[kk+28][jj];
							mult[ii][jj] += firstMatrix[ii][kk+29] * secondMatrix[kk+29][jj];
							mult[ii][jj] += firstMatrix[ii][kk+30] * secondMatrix[kk+30][jj];
							mult[ii][jj] += firstMatrix[ii][kk+31] * secondMatrix[kk+31][jj];
						}
					}
				}


			}
		}
	}
				
	return counter;
}