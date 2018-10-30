#!/bin/bash

# Whether this script is run in the correct directory
FILE="CMakeLists.txt"
if [ -f $FILE ]; then
   echo "Correct directory: the script is run in the same directory with CMakeLists.txt."
else
   echo "Wrong directory: the script should be run in the same directory with CMakeLists.txt."
   exit 1
fi

cd output-profile-optimization
times=10

rm time_direct.csv
echo "-O0,-O1,-O2,-O3,-Ofast" >> time_direct.csv

counter=1
while [ $counter -le $times ]
do
    echo $counter

    echo -n `./directSolver.O0 ../data/A_500.csv ../data/b_500.csv 0 | grep method | awk '{print $7}'` >> time_direct.csv
    echo -n ',' >> time_direct.csv
    echo -n `./directSolver.O1 ../data/A_500.csv ../data/b_500.csv 0 | grep method | awk '{print $7}'` >> time_direct.csv
    echo -n ',' >> time_direct.csv
    echo -n `./directSolver.O2 ../data/A_500.csv ../data/b_500.csv 0 | grep method | awk '{print $7}'` >> time_direct.csv
    echo -n ',' >> time_direct.csv
    echo -n `./directSolver.O3 ../data/A_500.csv ../data/b_500.csv 0 | grep method | awk '{print $7}'` >> time_direct.csv
    echo -n ',' >> time_direct.csv
    echo `./directSolver.Ofast ../data/A_500.csv ../data/b_500.csv 0 | grep method | awk '{print $7}'` >> time_direct.csv

    ((counter++))
done

cd ..
