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

    double elapsed_time;
    clock_t start;
    clock_t end;
    if(rank == root){

        // init Matrices with random values in root process
        A = (float*) malloc(SIZE*SIZE*sizeof(float));
        C = (float*) malloc(SIZE*SIZE*sizeof(float));
        for(int i = 0; i<SIZE*SIZE; i++){
            A[i] = (float)rand()/(float)(RAND_MAX/max_float);
            B[i] = (float)rand()/(float)(RAND_MAX/max_float);
        }

        start = clock();
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
            C_sub[index] = A[row] * B[j];
            for(int k = 1; k<SIZE; k++){
                C_sub[index] += A[row + k] * B[j + k*SIZE];
            }
        }
    }

    MPI_Gatherv(C_sub, sendcounts[rank], MPI_FLOAT, C, sendcounts, displs, MPI_FLOAT, root, MPI_COMM_WORLD);


    if(rank == root){
        end = clock();
        elapsed_time = (end-start)/(double)CLOCKS_PER_SEC;
        printf("%d,%f \n", SIZE, elapsed_time);
    }

    MPI_Finalize();
}
