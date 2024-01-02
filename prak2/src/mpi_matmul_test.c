#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <getopt.h>
#include <time.h>

int main(int argc, char *argv[]){
    int SIZE = 4;
    int root = 0;
    float max_float = 1000.0;

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

    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    int min_rows = (int)SIZE/world_size;
    int min_count = min_rows*SIZE;
    int *sendcounts = (int*)malloc(world_size*sizeof(int));
    int *displs = (int*)malloc(world_size*sizeof(int));
    int *sendrows = (int*)malloc(world_size*sizeof(int));
    
    // determine rows of each process
    for(int i = 0; i<world_size; i++){
        sendrows[i] = min_rows;
    }
    int totalrows = min_rows * world_size;
    int count = 0;
    while(totalrows < SIZE){
        count = count % world_size;
        sendrows[count]++;
        count++;
        totalrows++;
    }

    // Process Sendcounts and dipls for Scatterv
    sendcounts[0] = sendrows[0]*SIZE;
    displs[0] = 0;
    for(int i = 1; i<world_size; i++){
        sendcounts[i] = sendrows[i]*SIZE;
        displs[i] = displs[i-1] + sendrows[i-1]*SIZE;
    }


    // init Matrix Sizes
    float *A;
    float *B = (float*) malloc(SIZE*SIZE*sizeof(float));
    float *C_sub = (float*) malloc(SIZE*sendrows[rank]*sizeof(float));
    float *C = NULL;
    float *C_check = (float*) malloc(SIZE*SIZE*sizeof(float));

    if(rank == root){

        // init Matrices with random values in root process
        A = (float*) malloc(SIZE*SIZE*sizeof(float));
        C = (float*) malloc(SIZE*SIZE*sizeof(float));
        for(int i = 0; i<SIZE*SIZE; i++){
            A[i] = (float)rand()/(float)(RAND_MAX/max_float);
            B[i] = (float)rand()/(float)(RAND_MAX/max_float);
        }

        // Check Matrix
        for(int i = 0; i<SIZE; i++){
            int row = i*SIZE;
            for(int j = 0; j<SIZE; j++){
                int index = row + j;
                C_check[index] = A[row] * B[j];
                for(int k = 1; k<SIZE; k++){
			C_check[index] += A[row+k] * B[j+k*SIZE];
                }
            }
        }


    }
    else{
        //allocate A for slave processes
        A = (float*) malloc(sendcounts[rank]*sizeof(float));
    }

    MPI_Scatterv(A, sendcounts, displs, MPI_FLOAT, A, sendcounts[rank], MPI_FLOAT, root, MPI_COMM_WORLD);

    MPI_Bcast(B, SIZE*SIZE, MPI_FLOAT, root, MPI_COMM_WORLD);

    // Matrix Mul for each subprocess
    for(int i = 0; i<sendrows[rank]; i++){
        int row = i*SIZE;
        for(int j=0; j<SIZE; j++){
            int index = row + j;
	    C_sub[index]=0;
            for(int k = 0; k<SIZE; k+=32){
			C_sub[index] += A[row + k] * B[j + k*SIZE];
			C_sub[index] += A[row + k+1] * B[j + (k+1)*SIZE];
			C_sub[index] += A[row + k+2] * B[j + (k+2)*SIZE];
			C_sub[index] += A[row + k+3] * B[j + (k+3)*SIZE];
			C_sub[index] += A[row + k+4] * B[j + (k+4)*SIZE];
			C_sub[index] += A[row + k+5] * B[j + (k+5)*SIZE];
			C_sub[index] += A[row + k+6] * B[j + (k+6)*SIZE];
			C_sub[index] += A[row + k+7] * B[j + (k+7)*SIZE];

			C_sub[index] += A[row + k+8] * B[j + (k+8)*SIZE];
			C_sub[index] += A[row + k+9] * B[j + (k+9)*SIZE];
			C_sub[index] += A[row + k+10] * B[j + (k+10)*SIZE];
			C_sub[index] += A[row + k+11] * B[j + (k+11)*SIZE];
			C_sub[index] += A[row + k+12] * B[j + (k+12)*SIZE];
			C_sub[index] += A[row + k+13] * B[j + (k+13)*SIZE];
			C_sub[index] += A[row + k+14] * B[j + (k+14)*SIZE];
			C_sub[index] += A[row + k+15] * B[j + (k+15)*SIZE];

			C_sub[index] += A[row + k+16] * B[j + (k+16)*SIZE];
			C_sub[index] += A[row + k+17] * B[j + (k+17)*SIZE];
			C_sub[index] += A[row + k+18] * B[j + (k+18)*SIZE];
			C_sub[index] += A[row + k+19] * B[j + (k+19)*SIZE];
			C_sub[index] += A[row + k+20] * B[j + (k+20)*SIZE];
			C_sub[index] += A[row + k+21] * B[j + (k+21)*SIZE];
			C_sub[index] += A[row + k+22] * B[j + (k+22)*SIZE];
			C_sub[index] += A[row + k+23] * B[j + (k+23)*SIZE];

			C_sub[index] += A[row + k+24] * B[j + (k+24)*SIZE];
			C_sub[index] += A[row + k+25] * B[j + (k+25)*SIZE];
			C_sub[index] += A[row + k+26] * B[j + (k+26)*SIZE];
			C_sub[index] += A[row + k+27] * B[j + (k+27)*SIZE];
			C_sub[index] += A[row + k+28] * B[j + (k+28)*SIZE];
			C_sub[index] += A[row + k+29] * B[j + (k+29)*SIZE];
			C_sub[index] += A[row + k+30] * B[j + (k+30)*SIZE];
			C_sub[index] += A[row + k+31] * B[j + (k+31)*SIZE];
            }
        }
    }

    MPI_Gatherv(C_sub, sendcounts[rank], MPI_FLOAT, C, sendcounts, displs, MPI_FLOAT, root, MPI_COMM_WORLD);

    if(rank == root){
        int mistakes = 0;
        for(int i = 0; i<SIZE; i++){
            int index = i*SIZE;
            for(int j = 0; j<SIZE; j++){
                if(C[index+j] != C_check[index+j]) mistakes++;
            //    printf("%f, ", C[index+j]);
            }
            //printf("\n");
        }
        printf("mistakes: %d\n", mistakes);
    }

    MPI_Finalize();
}
