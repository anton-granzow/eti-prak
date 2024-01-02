#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks=8
#SBATCH --cpus-per-task=1
#SBATCH --job-name=eti2-t_8_n1
#SBATCH --output=prak2-tasks_8_nodes_1.out
#SBATCH --error=prak2-tasks_8_nodes_1.err
#SBATCH --time=10:00:00


bin_file="./bin/mpi_matmul_measure"

loops=1
tasks=8
output_file="./data/run2/aufgabe6_nodes_1_tasks_${tasks}"
touch $output_file
echo "size,time" >> $output_file
for size in 2048 4096 8192
do
    echo "SIZE: $size"
    for i in {1..10}
    do
        srun --nodes=1 --tasks-per-node=8 $bin_file -s $size -l  $loops >> $output_file
    done
done


