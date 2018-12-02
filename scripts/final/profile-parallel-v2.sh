#!/bin/bash

echo 1 >> log-fix-size.txt
time mpirun -np 1 parallelJacobi2 ../../data/ncdf4/1600.nc 90000 3 0 >> log-fix-size.txt

echo 2 >> log-fix-size.txt
time mpirun -np 2 parallelJacobi2 ../../data/ncdf4/1600.nc 90000 3 0 >> log-fix-size.txt

echo 4 >> log-fix-size.txt
time mpirun -np 4 parallelJacobi2 ../../data/ncdf4/1600.nc 90000 3 0 >> log-fix-size.txt

echo 8 >> log-fix-size.txt
time mpirun -np 8 parallelJacobi2 ../../data/ncdf4/1600.nc 90000 3 0 >> log-fix-size.txt

echo 16 >> log-fix-size.txt
time mpirun -np 16 parallelJacobi2 ../../data/ncdf4/1600.nc 90000 3 0 >> log-fix-size.txt

echo 32 >> log-fix-size.txt
time mpirun -np 32 parallelJacobi2 ../../data/ncdf4/1600.nc 90000 3 0 >> log-fix-size.txt

echo 64 >> log-fix-size.txt
time mpirun -np 64 parallelJacobi2 ../../data/ncdf4/1600.nc 90000 3 0 >> log-fix-size.txt

echo 100 >> log-fix-size.txt
time mpirun -np 100 parallelJacobi2 ../../data/ncdf4/1600.nc 90000 3 0 >> log-fix-size.txt

