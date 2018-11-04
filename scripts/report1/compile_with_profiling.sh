#!/bin/bash

# Exit with nonzero exit code if anything fails
set -e

# Whether this script is run in the correct directory
FILE="CMakeLists.txt"
if [ -f $FILE ]; then
   echo "Correct directory: the script is run in the same directory with CMakeLists.txt."
else
   echo "Wrong directory: the script should be run in the same directory with CMakeLists.txt."
   exit 1
fi

rm -rf exes_profiling || true
mkdir exes_profiling

# Load modules
# module load gcc/5.3.1 tau/2.27 cmake/3.9.1 

# Load Adam's pre-built
module use /storage/work/a/awl5173/toShare/tauPdt/tau 
module load adamsTau_2.27 cmake/3.9.1 

# Build exes with tau
mkdir build_tau
cd build_tau

CC=tau_cc.sh CXX=tau_cxx.sh cmake -DCMAKE_BUILD_TYPE=Debug -DEXE_SUFFIX=.tau .. > ../exes_profiling/cmake.tau
make VERBOSE=1 > ../exes_profiling/make.tau
cp ../output/bin/* ../exes_profiling

cd ..
rm -rf build_tau

# Build exes with -pg
mkdir build_pg
cd build_pg

cmake -DCMAKE_BUILD_TYPE=Debug -DEXE_SUFFIX=.pg -DCMAKE_CXX_FLAGS=-pg .. > ../exes_profiling/cmake.pg
make VERBOSE=1 > ../exes_profiling/make.pg
cp ../output/bin/* ../exes_profiling

cd ..
rm -rf build_pg

echo "Done! Please find the executables in folder exes_profiling/"
