#!/bin/bash
#SBATCH --nodes=2
#SBATCH --ntasks=96
#SBATCH --cpus-per-task=1
#SBATCH --job-name=eti2-t_48,96
#SBATCH --output=prak2-tasks_48,96.out
#SBATCH --error=prak2-tasks_48,9.err
#SBATCH --time=10:00:00


bin_file="./bin/mpi_matmul_measure"

loops=1
for nodes in 1 2
do
    for tasks in 48 96
    do
        output_file="./data/aufgabe6_nodes_${nodes}_tasks_${tasks}"
        touch $output_file
        echo "size,time" >> $output_file
        for size in 4096 8192 16384
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


