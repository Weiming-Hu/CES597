#!/bin/bash

module load gcc/5.3.1 
module load openmpi/1.10.1 
module load netcdf/4.4.1 

rm log-fix-size.txt || true

echo 1 >> log-fix-size.txt
{ time mpirun -np 1 ../../output/bin/parallelJacobi2 ../../data/ncdf4/800.nc 90000 3 0 ; } >> log-fix-size.txt

echo 2 >> log-fix-size.txt
{ time mpirun -np 2 ../../output/bin/parallelJacobi2 ../../data/ncdf4/800.nc 90000 3 0 ; } >> log-fix-size.txt

echo 4 >> log-fix-size.txt
{ time mpirun -np 4 ../../output/bin/parallelJacobi2 ../../data/ncdf4/800.nc 90000 3 0 ; } >> log-fix-size.txt

echo 8 >> log-fix-size.txt
{ time mpirun -np 8 ../../output/bin/parallelJacobi2 ../../data/ncdf4/800.nc 90000 3 0 ; } >> log-fix-size.txt

echo 16 >> log-fix-size.txt
{ time mpirun -np 16 ../../output/bin/parallelJacobi2 ../../data/ncdf4/800.nc 90000 3 0 ; } >> log-fix-size.txt

echo 32 >> log-fix-size.txt
{ time mpirun -np 32 ../../output/bin/parallelJacobi2 ../../data/ncdf4/800.nc 90000 3 0 ; } >> log-fix-size.txt

echo 64 >> log-fix-size.txt
{ time mpirun -np 64 ../../output/bin/parallelJacobi2 ../../data/ncdf4/800.nc 90000 3 0 ; } >> log-fix-size.txt

echo 100 >> log-fix-size.txt
{ time mpirun -np 100 ../../output/bin/parallelJacobi2 ../../data/ncdf4/800.nc 90000 3 0 ; } >> log-fix-size.txt

