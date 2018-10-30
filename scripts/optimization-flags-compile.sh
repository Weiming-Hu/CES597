#!/bin/bash

# Whether this script is run in the correct directory
FILE="CMakeLists.txt"
if [ -f $FILE ]; then
   echo "Correct directory: the script is run in the same directory with CMakeLists.txt."
else
   echo "Wrong directory: the script should be run in the same directory with CMakeLists.txt."
   exit 1
fi

rm -rf output output-profile-optimization build*
mkdir output-profile-optimization

mkdir build-O0
cd build-O0
cmake -DCMAKE_CXX_FLAGS="-O0" -DEXE_SUFFIX=".O0" -DPROFILE_TIME=ON ..
make -j VERBOSE=1
cd ..
cp output/bin/* output-profile-optimization

mkdir build-O1
cd build-O1
cmake -DCMAKE_CXX_FLAGS="-O1" -DEXE_SUFFIX=".O1" -DPROFILE_TIME=ON ..
make -j VERBOSE=1
cd ..
cp output/bin/* output-profile-optimization

mkdir build-O2
cd build-O2
cmake -DCMAKE_CXX_FLAGS="-O2" -DEXE_SUFFIX=".O2" -DPROFILE_TIME=ON ..
make -j VERBOSE=1
cd ..
cp output/bin/* output-profile-optimization

mkdir build-O3
cd build-O3
cmake -DCMAKE_CXX_FLAGS="-O3" -DEXE_SUFFIX=".O3" -DPROFILE_TIME=ON ..
make -j VERBOSE=1
cd ..
cp output/bin/* output-profile-optimization

mkdir build-Ofast
cd build-Ofast
cmake -DCMAKE_CXX_FLAGS="-Ofast" -DEXE_SUFFIX=".Ofast" -DPROFILE_TIME=ON ..
make -j VERBOSE=1
cd ..
cp output/bin/* output-profile-optimization

cd output-profile-optimization
rm test*
