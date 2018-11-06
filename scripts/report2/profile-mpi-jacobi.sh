#!/bin/bash
rm log1.txt || true
rm log2.txt || true
rm log3.txt || true
rm log4.txt || true
rm log5.txt || true

{ time OMP_NUM_THREADS=1 mpirun -np 1 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0 ; } 2>> log1.txt
{ time OMP_NUM_THREADS=1 mpirun -np 2 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0 ; } 2>> log1.txt
{ time OMP_NUM_THREADS=1 mpirun -np 4 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0 ; } 2>> log1.txt 
{ time OMP_NUM_THREADS=1 mpirun -np 8 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0 ; } 2>> log1.txt 
{ time OMP_NUM_THREADS=1 mpirun -np 16 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0; } 2>> log1.txt 
{ time OMP_NUM_THREADS=1 mpirun -np 32 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0; } 2>> log1.txt 

{ time OMP_NUM_THREADS=1 mpirun -np 1 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0 ; } 2>> log2.txt
{ time OMP_NUM_THREADS=1 mpirun -np 2 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0 ; } 2>> log2.txt
{ time OMP_NUM_THREADS=1 mpirun -np 4 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0 ; } 2>> log2.txt 
{ time OMP_NUM_THREADS=1 mpirun -np 8 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0 ; } 2>> log2.txt 
{ time OMP_NUM_THREADS=1 mpirun -np 16 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0; } 2>> log2.txt 
{ time OMP_NUM_THREADS=1 mpirun -np 32 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0; } 2>> log2.txt 

{ time OMP_NUM_THREADS=1 mpirun -np 1 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0 ; } 2>> log3.txt
{ time OMP_NUM_THREADS=1 mpirun -np 2 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0 ; } 2>> log3.txt
{ time OMP_NUM_THREADS=1 mpirun -np 4 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0 ; } 2>> log3.txt 
{ time OMP_NUM_THREADS=1 mpirun -np 8 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0 ; } 2>> log3.txt 
{ time OMP_NUM_THREADS=1 mpirun -np 16 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0; } 2>> log3.txt 
{ time OMP_NUM_THREADS=1 mpirun -np 32 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0; } 2>> log3.txt 

{ time OMP_NUM_THREADS=1 mpirun -np 1 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0 ; } 2>> log4.txt
{ time OMP_NUM_THREADS=1 mpirun -np 2 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0 ; } 2>> log4.txt
{ time OMP_NUM_THREADS=1 mpirun -np 4 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0 ; } 2>> log4.txt 
{ time OMP_NUM_THREADS=1 mpirun -np 8 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0 ; } 2>> log4.txt 
{ time OMP_NUM_THREADS=1 mpirun -np 16 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0; } 2>> log4.txt 
{ time OMP_NUM_THREADS=1 mpirun -np 32 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0; } 2>> log4.txt 

{ time OMP_NUM_THREADS=1 mpirun -np 1 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0 ; } 2>> log5.txt
{ time OMP_NUM_THREADS=1 mpirun -np 2 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0 ; } 2>> log5.txt
{ time OMP_NUM_THREADS=1 mpirun -np 4 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0 ; } 2>> log5.txt 
{ time OMP_NUM_THREADS=1 mpirun -np 8 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0 ; } 2>> log5.txt 
{ time OMP_NUM_THREADS=1 mpirun -np 16 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0; } 2>> log5.txt 
{ time OMP_NUM_THREADS=1 mpirun -np 32 parallelJacobi ../../data/A_500.csv ../../data/b_500.csv 10000 3 0; } 2>> log5.txt 
