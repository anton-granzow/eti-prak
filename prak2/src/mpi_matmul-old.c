#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <mpi.h>

#define BLOCK_SIZE 32
#define SIZE 512
#define PROCESSES 1

const float max_float = 100.0;

int main(int argc, char *argv[]){
	
    /* 2D array matrix allocation
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
    */

   // 1D array Matrix allocation
    float *A = new float[SIZE*SIZE];
    float *B = new float[SIZE*SIZE];
    float *C = new float[SIZE*SIZE];

    for (int i = 0; i < SIZE*SIZE; i++) {
        A[i] = (float)rand()/(float)(RAND_MAX/max_float);
        B[i] = (float)rand()/(float)(RAND_MAX/max_float);
        C[i] = (float)rand()/(float)(RAND_MAX/max_float);
    }

	// Initializing elements of matrix mult to 0.
	for(int i = 0; i < SIZE*SIZE; ++i) 
        C[i] = 0;
	
	// do matrix mul couple of times
	int counter = 0;
    double timer = 0;

    // init MPI variables
    int rank_id;
    int total_ranks;
    int slave_proc, rows_per_slave;

    //clock_t start = clock() ;
    // Init MPI environment
    MPI_Init(&argc,&argv);

    // get id of current rank
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_id);

    // get number of total ranks
    MPI_Comm_size(MPI_COMM_WORLD, &total_ranks);

    slave_proc = total_ranks - 1;

    // associate a number of rows of A for each slave process
    rows_per_slave = floor(SIZE / slave_proc-1);

    // get amount of rows for last slave process (take the last slave out: -1)
    last_slave = SIZE - (rows_per_slave * (slave_proc-1));

    // allocate ressources for all processes except the last one
    if ( rank_id < slave_proc ) {
        float *rows_i = new float[SIZE*rows_per_slave];
        float *B_i = new float[SIZE*SIZE];
    }
    
    // allocate ressources for the last process
    if ( rank_id == slave_proc ) {
        float *rows_i = new float[SIZE*last_slave];
        float *B_i = new float[SIZE*SIZE];
    }

    // send data from root process (COLLECTIVE COMMUNICATION: only MPI_SEND can be performed once, BCast and Scatter have to be called by all processes)
    if (rank_id == 0) {
                
        // send last slave the remaining rows
        int last_slave_offset = rows_per_slave * SIZE * (slave_proc - 1);
        MPI_Send( A[last_slave_offset] , last_slave * SIZE , MPI_FLOAT , slave_proc , 1 , MPI_COMM_WORLD); // TODO: whats the use of the tag? (here: 1)

    }

    // scatter rows of A to the processes
    MPI_Scatter( A , rows_per_slave * SIZE , MPI_FLOAT , rows_i ,
                    rows_per_slave * SIZE , MPI_FLOAT , 0 , MPI_COMM_WORLD);

    // broadcast the second input matrix
    MPI_Bcast( B , SIZE*SIZE , MPI_FLOAT , 0 , MPI_COMM_WORLD);

    // calculate output matrix for all processes except the last one
    if ((rank_id != 0) && (rank_id < slave_proc)) {
        
        int offset = rank_id * rows_per_slave * SIZE;                   // the offset that we have to jump to for every process
        for (int i = 0; i < rows_per_slave; i++) {                      // rows of current process
            for (int j = 0; j < SIZE; j++) {                            // cols of B

                float sum = 0f;
                for (int k = 0; k < SIZE; k++) {        
                    sum += rows_i[i * SIZE + k] * B_i[k * SIZE + j];    // sum up mult. results
                }

                C[offset + (i * SIZE + j)] = sum;                       // put results in correct place of C

            }
        }

    }

    // finalize matrix C with last process
    if ( rank_id == slave_proc ) {

        int offset = (rank_id-1) * rows_per_slave * SIZE;               // the offset that we have to jump to for last process (rank_id)
        for (int i = 0; i < last_slave; i++) {                          // rows of current process
            for (int j = 0; j < SIZE; j++) {                            // cols of B

                float sum = 0f;
                for (int k = 0; k < SIZE; k++) {        
                    sum += rows_i[i * SIZE + k] * B_i[k * SIZE + j];    // sum up mult. results
                }

                C[offset + (i * SIZE + j)] = sum;                       // put results in correct place of C

            }
        }

    }



    MPI_Finalize();
    //clock_t end = clock() ;
    //timer = (end-start)/(double)CLOCKS_PER_SEC ;
	
	printf("Size: %d\n Processes: %d\n time: %f\n", SIZE, PROCESSES, time);
    return 0;
}