#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks=10
#SBATCH --cpus-per-task=1
#SBATCH --job-name=eti-prak2
#SBATCH --output=prak2.out
#SBATCH --error=prak2.err
#SBATCH --time=02:00:00

#Module
module purge
module load GCC/12.2.0
module load OpenMPI/4.1.4


output_file="./data/aufgabe6.csv"
input_file="./src/mpi_matmul_measure.c"

#compiling
bin_file="./bin/mpi_matmul_measure"
mpicc -march=native -O3 $input_file -o $bin_file

#10 mal Ausführung
loops=1

output_file="./data/aufgabe6_nodes_1_tasks_1"
touch $output_file
echo "size,time" >> $output_file
for size in 2048 4096 8192
do
    echo "SIZE: $size"
    for i in {1..10}
    do
        # echo "iteration: $i, size: $size, tasks: $tasks, nodes: $nodes"
        srun --exclusive --ntasks=$tasks --nodes=$nodes $bin_file -s $size -l  $loops >> $output_file &
    done
    wait
done



