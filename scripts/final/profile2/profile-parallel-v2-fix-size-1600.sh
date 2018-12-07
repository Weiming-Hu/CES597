#!/bin/bash
#PBS -l nodes=4:ppn=16
#PBS -l walltime=40:00:00

module load gcc/5.3.1 
module load openmpi/1.10.1 
module load netcdf/4.4.1 

#rm log-fix-size-prog.txt || true
#rm log-fix-size-time.txt || true

cd /storage/home/w/wuh20/github/LinearSystemSolvers/scripts/final/profile2
{ time mpirun -np 1 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 1/log-fix-size-prog.txt; } 2>> 1/log-fix-size-time.txt
{ time mpirun -np 2 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 1/log-fix-size-prog.txt; } 2>> 1/log-fix-size-time.txt
{ time mpirun -np 4 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 1/log-fix-size-prog.txt; } 2>> 1/log-fix-size-time.txt
{ time mpirun -np 8 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 1/log-fix-size-prog.txt; } 2>> 1/log-fix-size-time.txt
{ time mpirun -np 16 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 1/log-fix-size-prog.txt; } 2>> 1/log-fix-size-time.txt
{ time mpirun -np 20 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 1/log-fix-size-prog.txt; } 2>> 1/log-fix-size-time.txt
{ time mpirun -np 24 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 1/log-fix-size-prog.txt; } 2>> 1/log-fix-size-time.txt
{ time mpirun -np 28 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 1/log-fix-size-prog.txt; } 2>> 1/log-fix-size-time.txt
{ time mpirun -np 32 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 1/log-fix-size-prog.txt; } 2>> 1/log-fix-size-time.txt
{ time mpirun -np 38 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 1/log-fix-size-prog.txt; } 2>> 1/log-fix-size-time.txt
{ time mpirun -np 44 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 1/log-fix-size-prog.txt; } 2>> 1/log-fix-size-time.txt
{ time mpirun -np 50 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 1/log-fix-size-prog.txt; } 2>> 1/log-fix-size-time.txt
{ time mpirun -np 55 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 1/log-fix-size-prog.txt; } 2>> 1/log-fix-size-time.txt
{ time mpirun -np 64 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 1/log-fix-size-prog.txt; } 2>> 1/log-fix-size-time.txt

{ time mpirun -np 1 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 2/log-fix-size-prog.txt; } 2>> 2/log-fix-size-time.txt
{ time mpirun -np 2 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 2/log-fix-size-prog.txt; } 2>> 2/log-fix-size-time.txt
{ time mpirun -np 4 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 2/log-fix-size-prog.txt; } 2>> 2/log-fix-size-time.txt
{ time mpirun -np 8 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 2/log-fix-size-prog.txt; } 2>> 2/log-fix-size-time.txt
{ time mpirun -np 16 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 2/log-fix-size-prog.txt; } 2>> 2/log-fix-size-time.txt
{ time mpirun -np 20 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 2/log-fix-size-prog.txt; } 2>> 2/log-fix-size-time.txt
{ time mpirun -np 24 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 2/log-fix-size-prog.txt; } 2>> 2/log-fix-size-time.txt
{ time mpirun -np 28 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 2/log-fix-size-prog.txt; } 2>> 2/log-fix-size-time.txt
{ time mpirun -np 32 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 2/log-fix-size-prog.txt; } 2>> 2/log-fix-size-time.txt
{ time mpirun -np 38 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 2/log-fix-size-prog.txt; } 2>> 2/log-fix-size-time.txt
{ time mpirun -np 44 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 2/log-fix-size-prog.txt; } 2>> 2/log-fix-size-time.txt
{ time mpirun -np 50 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 2/log-fix-size-prog.txt; } 2>> 2/log-fix-size-time.txt
{ time mpirun -np 55 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 2/log-fix-size-prog.txt; } 2>> 2/log-fix-size-time.txt
{ time mpirun -np 64 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 2/log-fix-size-prog.txt; } 2>> 2/log-fix-size-time.txt

{ time mpirun -np 1 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 3/log-fix-size-prog.txt; } 2>> 3/log-fix-size-time.txt
{ time mpirun -np 2 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 3/log-fix-size-prog.txt; } 2>> 3/log-fix-size-time.txt
{ time mpirun -np 4 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 3/log-fix-size-prog.txt; } 2>> 3/log-fix-size-time.txt
{ time mpirun -np 8 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 3/log-fix-size-prog.txt; } 2>> 3/log-fix-size-time.txt
{ time mpirun -np 16 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 3/log-fix-size-prog.txt; } 2>> 3/log-fix-size-time.txt
{ time mpirun -np 20 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 3/log-fix-size-prog.txt; } 2>> 3/log-fix-size-time.txt
{ time mpirun -np 24 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 3/log-fix-size-prog.txt; } 2>> 3/log-fix-size-time.txt
{ time mpirun -np 28 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 3/log-fix-size-prog.txt; } 2>> 3/log-fix-size-time.txt
{ time mpirun -np 32 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 3/log-fix-size-prog.txt; } 2>> 3/log-fix-size-time.txt
{ time mpirun -np 38 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 3/log-fix-size-prog.txt; } 2>> 3/log-fix-size-time.txt
{ time mpirun -np 44 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 3/log-fix-size-prog.txt; } 2>> 3/log-fix-size-time.txt
{ time mpirun -np 50 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 3/log-fix-size-prog.txt; } 2>> 3/log-fix-size-time.txt
{ time mpirun -np 55 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 3/log-fix-size-prog.txt; } 2>> 3/log-fix-size-time.txt
{ time mpirun -np 64 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 900000 3 1 >> 3/log-fix-size-prog.txt; } 2>> 3/log-fix-size-time.txt
