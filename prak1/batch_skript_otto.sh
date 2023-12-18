#!/bin/bash

#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --job-name=ETI-Lab1
#SBATCH --output=Lab1.out
#SBATCH --error=Lab1.err
#SBATCH --time=10:00:00

#Module stuff
module purge
module load GCC/12.2.0


#Compiler Stuff

echo "Naive Matrix Multiplikation \n"
gcc ./Code/Naive.c -O0    -o ./bin/n0
gcc ./Code/Naive.c -O1    -o ./bin/n1
gcc ./Code/Naive.c -O2    -o ./bin/n2
gcc ./Code/Naive.c -O3    -o ./bin/n3
gcc ./Code/Naive.c -Ofast -o ./bin/n4
gcc ./Code/Naive.c -Ofast -o ./bin/n5 -march=native

echo "\n Matrixmultiplikation mit Schleifenvertauschung \n"
gcc ./Code/Vertauscht.c -O0    -o ./bin/v0
gcc ./Code/Vertauscht.c -O1    -o ./bin/v1
gcc ./Code/Vertauscht.c -O2    -o ./bin/v2
gcc ./Code/Vertauscht.c -O3    -o ./bin/v3
gcc ./Code/Vertauscht.c -Ofast -o ./bin/v4
gcc ./Code/Vertauscht.c -Ofast -o ./bin/v5  -march=native

echo "\n Matrixmultiplikation mit Loopunrolling \n"
gcc ./Code/Unroll.c -O0    -o ./bin/u0
gcc ./Code/Unroll.c -O1    -o ./bin/u1
gcc ./Code/Unroll.c -O2    -o ./bin/u2
gcc ./Code/Unroll.c -O3    -o ./bin/u3
gcc ./Code/Unroll.c -Ofast -o ./bin/u4
gcc ./Code/Unroll.c -Ofast -o ./bin/u5 -march=native

echo "\n Matrixmultiplikation mit Tiling \n"
gcc ./code/Tiling.c -O0    -o ./bin/b0
gcc ./code/Tiling.c -O1    -o ./bin/b1
gcc ./code/Tiling.c -O2    -o ./bin/b2
gcc ./code/Tiling.c -O3    -o ./bin/b3
gcc ./code/Tiling.c -Ofast -o ./bin/b4
gcc ./code/Tiling.c -Ofast -o ./bin/b5 -march=native


#Ausführung der unoptimierten Matrixmultiplikationen
echo "Aufgabe 5,7"

echo "\n Naive Matrix Multiplikation \n"
srun ./bin/n0  128 300
srun ./bin/n0  256 100
srun ./bin/n0  512  25
srun ./bin/n0 1024   1
srun ./bin/n0 2048   1

echo "\n Matrixmultiplikation mit Schleifenvertauschung \n"
srun ./bin/v0  128 300
srun ./bin/v0  256 100
srun ./bin/v0  512  25
srun ./bin/v0 1024   1
srun ./bin/v0 2048   1

echo "\n Matrixmultiplikation mit Loopunrolling \n"
srun ./bin/u0  128 300
srun ./bin/u0  256 100
srun ./bin/u0  512  25
srun ./bin/u0 1024   1
srun ./bin/u0 2048   1

echo "\n Matrixmultiplikation mit Tiling \n"
srun ./bin/b0  128 300
srun ./bin/b0  256 100
srun ./bin/b0  512  25
srun ./bin/b0 1024   1
srun ./bin/b0 2048   1


#Ausführen der Benchmarks in verschiedenen Optimierungsstufen
echo "Aufgabe 8"

echo "\n Naive Matrixmultiplikation \n"
srun ./bin/n0 2048 1
srun ./bin/n1 2048 5
srun ./bin/n2 2048 1000
srun ./bin/n3 2048 1000
srun ./bin/n4 2048 1000
srun ./bin/n5 2048 1000

echo "\n Matrixmultiplikation mit Schleifenvertauschung \n"
srun ./bin/v0 2048 1
srun ./bin/v1 2048 5
srun ./bin/v2 2048 1000
srun ./bin/v3 2048 1000
srun ./bin/v4 2048 1000
srun ./bin/v5 2048 1000

echo "\n Matrixmultiplikation mit Loopunrolling \n"
srun ./bin/u0 2048 1
srun ./bin/u1 2048 5
srun ./bin/u2 2048 1000
srun ./bin/u3 2048 1000
srun ./bin/u4 2048 1000
srun ./bin/u5 2048 1000

echo "\n Matrixmultiplikation mit Tiling \n"
srun ./bin/b0 2048 1
srun ./bin/b1 2048 5
srun ./bin/b2 2048 1000
srun ./bin/b3 2048 1000
srun ./bin/b4 2048 1000
srun ./bin/b5 2048 1000