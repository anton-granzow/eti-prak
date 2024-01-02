#!/bin/bash
#SBATCH --nodes=2
#SBATCH --ntasks=104
#SBATCH --cpus-per-task=1
#SBATCH --job-name=eti-prak2
#SBATCH --output=prak2.out
#SBATCH --error=prak2.err
#SBATCH --time=10:00:00

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
for nodes in 1 2
do
    for tasks in 1 2 4 8 16 32 48 64 80 96 104
    do
        output_file="./data/aufgabe6_nodes_${nodes}_tasks_${tasks}"
        touch $output_file
        echo "size,time" >> $output_file
        for size in 2048 4096 8192 16384
        do
            echo "SIZE: $size"
            for i in {1..10}
            do
                if  [[ ! ( ($size -eq 2048 && $tasks -gt 16) || ($size -eq 16384 && $tasks -lt 32) )]]; then
                    echo "iteration: $i, size: $size, tasks: $tasks, nodes: $nodes"
                    srun --ntasks=$tasks --nodes=$nodes $bin_file -s $size -l  $loops >> $output_file
                fi
            done
        done
    done
done

