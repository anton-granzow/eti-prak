#!/bin/bash

output_file="./data/aufgabe5-2.csv"
input_file="./src/mpi_matmul.c"

#compiling
bin_file="./bin/mul_mpi"
mpicc -march=native -O3 $input_file -o $bin_file

#10 mal Ausführung
loops=1
for nodes in 1 2
do
    for tasks in 1 2 4 8 16 32 #48 64 80 96 104
    do
        output_file="./data/aufgabe6_nodes_${nodes}_tasks_${tasks}"
        touch $output_file
        echo "size,min,max,avg" >> $output_file
        for size in 2048 4096 #8192 16384
        do
            echo "SIZE: $size"
            for i in {1..2}
            do
                if  [[ ! ( ($size -eq 2048 && $tasks -gt 16) || ($size -eq 16384 && $tasks -lt 32) )]]; then
                    echo "iteration: $i, size: $size, tasks: $tasks, nodes: $nodes"
                    echo "srun --ntasks=$tasks --nodes=$nodes $bin_file -s $size -l  $loops >> $output_file"
                fi
            done
        done
    done
done
