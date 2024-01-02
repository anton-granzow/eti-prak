#!/bin/bash
#SBATCH --nodes=2
#SBATCH --tasks-per-node=1
#SBATCH --job-name=eti2-t_2_n2
#SBATCH --output=prak2-tasks_2_nodes_2.out
#SBATCH --error=prak2-tasks_2_nodes_2.err
#SBATCH --time=05:00:00


bin_file="./bin/mpi_matmul_measure"

loops=1
tasks=2
output_file="./data/run2/aufgabe6_nodes_2_tasks_${tasks}"
touch $output_file
echo "size,time" >> $output_file
for size in 2048 4096 8192
do
    echo "SIZE: $size"
    for i in {1..10}
    do
        srun --nodes=2 --tasks-per-node=1 $bin_file -s $size -l  $loops >> $output_file
    done
done


