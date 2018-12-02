#!/bin/bash

time OMP_NUM_THREADS=1 mpirun -np 1 parallelJacobi2 ../../data/ncdf4/1600.nc 10000 3 0 ;
} 2>> log1.txt
