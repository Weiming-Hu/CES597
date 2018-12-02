#!/bin/bash

module load gcc/5.3.1 
module load openmpi/1.10.1 
module load netcdf/4.4.1 

rm log-fix-np.txt || true

echo 10 >> log-fix-np.txt
time mpirun -np 1 ../../output/bin/parallelJacobi2 ../../data/ncdf4/10.nc 90000 3 0 >> log-fix-np.txt

echo 100 >> log-fix-np.txt
time mpirun -np 1 ../../output/bin/parallelJacobi2 ../../data/ncdf4/100.nc 90000 3 0 >> log-fix-np.txt

echo 200 >> log-fix-np.txt
time mpirun -np 1 ../../output/bin/parallelJacobi2 ../../data/ncdf4/200.nc 90000 3 0 >> log-fix-np.txt

echo 400 >> log-fix-np.txt
time mpirun -np 1 ../../output/bin/parallelJacobi2 ../../data/ncdf4/400.nc 90000 3 0 >> log-fix-np.txt

echo 800 >> log-fix-np.txt
time mpirun -np 1 ../../output/bin/parallelJacobi2 ../../data/ncdf4/800.nc 90000 3 0 >> log-fix-np.txt

echo 1600 >> log-fix-np.txt
time mpirun -np 1 ../../output/bin/parallelJacobi2 ../../data/ncdf4/1600.nc 90000 3 0 >> log-fix-np.txt

