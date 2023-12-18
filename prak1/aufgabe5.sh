#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --job-name=eti-prak1
#SBATCH --output=Lab1.out
#SBATCH --error=Lab1.err
#SBATCH --time=10:00:00

#Module
module purge
module load GCC/12.2.0


output_file="./data/aufgabe5.csv"
input_file="./src/matrix_mul_einfach.c"

#compiling
bin_file="./bin/einfach"
gcc $input_file -o $bin_file

#10 mal Ausführung
echo "size,min,max,avg" >> $output_file
loops=1
for size in 128 256 512 1024 2048
do
	echo "SIZE: $size"
	for i in {1..10}
	do
		echo "iteration: $i"
		case $size in
			128) loops=200 ;;
			256) loops=30 ;;
			512) loops=3 ;;
			1024) loops=2 ;;
			2048) loops=1 ;;
			*) loops=100 ;;
		esac
		srun $bin_file -s $size -l  $loops >> $output_file
	done
done
