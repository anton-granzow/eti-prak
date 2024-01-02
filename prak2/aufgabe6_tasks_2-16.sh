#!/bin/bash
#SBATCH --nodes=2
#SBATCH --ntasks=20
#SBATCH --cpus-per-task=1
#SBATCH --job-name=eti2-t_2-16
#SBATCH --output=prak2-tasks_2-16.out
#SBATCH --error=prak2-tasks_2-16.err
#SBATCH --time=10:00:00

bin_file="./bin/mpi_matmul_measure"

loops=1
for nodes in 1 2
do
    for tasks in 2 4 8 16
    do
        output_file="./data/aufgabe6_nodes_${nodes}_tasks_${tasks}"
        touch $output_file
        echo "size,time" >> $output_file
        for size in 2048 4096 8192
        do
            echo "SIZE: $size"
            for i in {1..10}
            do
                srun --exclusive --ntasks=$tasks --nodes=$nodes $bin_file -s $size -l  $loops >> $output_file &
            done
            wait
        done
    done
done


