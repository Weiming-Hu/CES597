#!/bin/bash
#PBS -l nodes=1:ppn=1
#PBS -l walltime=10:00:00

module load gcc/5.3.1 
module load openmpi/1.10.1 
module load netcdf/4.4.1 

rm log-fix-np-prog.txt || true
rm log-fix-np-time.txt || true

cd /storage/home/w/wuh20/github/LinearSystemSolvers/scripts/final
{ time mpirun -np 1 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/10.nc 90000 3 0   >> log-fix-np-prog.txt ; } 2>> log-fix-np-time.txt
{ time mpirun -np 1 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/100.nc 90000 3 0  >> log-fix-np-prog.txt ; } 2>> log-fix-np-time.txt
{ time mpirun -np 1 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/200.nc 90000 3 0  >> log-fix-np-prog.txt ; } 2>> log-fix-np-time.txt
{ time mpirun -np 1 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/400.nc 90000 3 0  >> log-fix-np-prog.txt ; } 2>> log-fix-np-time.txt
{ time mpirun -np 1 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/800.nc 90000 3 0  >> log-fix-np-prog.txt ; } 2>> log-fix-np-time.txt
{ time mpirun -np 1 ../../../output/bin/parallelJacobi2 ../../../data/ncdf4/1600.nc 90000 3 0 >> log-fix-np-prog.txt ; } 2>> log-fix-np-time.txt
