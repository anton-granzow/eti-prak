#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks=10
#SBATCH --cpus-per-task=1
#SBATCH --job-name=eti2-t_1
#SBATCH --output=prak2-tasks_1.out
#SBATCH --error=prak2-tasks_1.err
#SBATCH --time=10:00:00

bin_file="./bin/mpi_matmul_measure"

loops=1

output_file="./data/aufgabe6_nodes_1_tasks_1"
touch $output_file
echo "size,time" >> $output_file
size=8192
#for size in 2048 4096 8192
#do
echo "SIZE: $size"
for i in {1..10}
do
    echo "SIZE: $size"
    for i in {1..10}
    do
        srun --exclusive --ntasks=1 $bin_file -s $size -l  $loops >> $output_file &
    done
    wait
done
wait
#done



