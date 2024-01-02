#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]){
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int len_name;
    MPI_Get_processor_name(processor_name, &len_name);

    int *nums = (int*)malloc(world_size*sizeof(int));
    int num;
    if(world_rank == 0){
        for(int i=0; i<world_size; i++){
            nums[i] = i;
        }
    }
    MPI_Scatter(nums, 1, MPI_INT, &num, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Gather(&num, 1, MPI_INT, nums, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("hello from %s, rank %d / %d \n", processor_name, world_rank, world_size);
    printf("%d\n", num);

    if(world_rank == 0){
        printf("nums: ");
        for(int i = 0; i<world_size; i++){
            printf("%d, ", nums[i]);
        }
    }

    MPI_Finalize();
}