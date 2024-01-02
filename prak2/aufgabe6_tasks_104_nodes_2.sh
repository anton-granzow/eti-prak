#!/bin/bash
#SBATCH --nodes=2
#SBATCH --tasks-per-node=52
#SBATCH --cpus-per-task=1
#SBATCH --job-name=eti2-t_104_n2
#SBATCH --output=prak2-tasks_104_nodes_2.out
#SBATCH --error=prak2-tasks_104_nodes_2.err
#SBATCH --time=10:00:00


bin_file="./bin/mpi_matmul_measure"

loops=1
tasks=104
output_file="./data/run2/aufgabe6_nodes_2_tasks_${tasks}"
touch $output_file
echo "size,time" >> $output_file
for size in 4096 8192 16384
do
    echo "SIZE: $size"
    for i in {1..10}
    do
        srun --nodes=2 --tasks-per-node=52 $bin_file -s $size -l  $loops >> $output_file
    done
done


