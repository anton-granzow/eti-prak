#!/bin/bash
#SBATCH --ntasks=10
# #SBATCH --nodes=1
# #SBATCH --cpus-per-task=1
#SBATCH --output=test.out
#SBATCH --error=test.err
#SBATCH --time=00:10:00

#Module
#module load release/23.04  GCC/12.2.0
#module load OpenMPI/4.1.4


output_file="./data/aufgabe6.csv"
input_file="./src/mpi_matmul_measure.c"

#compiling
bin_file="./bin/mpi_matmul_measure"
# mpicc -march=native -O3 $input_file -o $bin_file

#10 mal Ausführung
loops=1
# for nodes in 1 2
# do
for tasks in 1 2 4 #8 16 32 48 64 80 96 104
do
	output_file="./data/test/aufgabe6_nodes_1_tasks_${tasks}"
	touch $output_file
	echo "size,time" >> $output_file
	for size in 1024 2048 4096
	do
	    echo "SIZE: $size"
	    for i in {1..10}
	    do
		    echo "iteration: $i, size: $size, tasks: $tasks"
		    srun --exclusive --ntasks=$tasks $bin_file -s $size -l  $loops >> $output_file &
	    done
	done
done
wait
# done

