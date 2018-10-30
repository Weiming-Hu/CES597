#!/bin/bash
cd output-profile-optimization
times=10

# Whether this script is run in the correct directory
FILE="CMakeLists.txt"
if [ -f $FILE ]; then
   echo "Correct directory: the script is run in the same directory with CMakeLists.txt."
else
   echo "Wrong directory: the script should be run in the same directory with CMakeLists.txt."
   exit 1
fi

rm profile_time_gauss_random.csv
echo "50,100,250,500,750,1000,1100,1200,1300" >> profile_time_gauss_random.csv

counter=1
while [ $counter -le $times ]
do
    echo $counter

    echo -n `./iterativeSolver.O2 G ../data/A_50.csv ../data/b_50.csv 100 2 0 | grep method | awk '{print $7}'` >> profile_time_gauss_random.csv
    echo -n ',' >> profile_time_gauss_random.csv
    echo -n `./iterativeSolver.O2 G ../data/A_100.csv ../data/b_100.csv 100 2 0 | grep method | awk '{print $7}'` >> profile_time_gauss_random.csv
    echo -n ',' >> profile_time_gauss_random.csv
    echo -n `./iterativeSolver.O2 G ../data/A_250.csv ../data/b_250.csv 100 2 0 | grep method | awk '{print $7}'` >> profile_time_gauss_random.csv
    echo -n ',' >> profile_time_gauss_random.csv
    echo -n `./iterativeSolver.O2 G ../data/A_500.csv ../data/b_500.csv 100 2 0 | grep method | awk '{print $7}'` >> profile_time_gauss_random.csv
    echo -n ',' >> profile_time_gauss_random.csv
    echo -n `./iterativeSolver.O2 G ../data/A_750.csv ../data/b_750.csv 100 2 0 | grep method | awk '{print $7}'` >> profile_time_gauss_random.csv
    echo -n ',' >> profile_time_gauss_random.csv
    echo -n `./iterativeSolver.O2 G ../data/A_1000.csv ../data/b_1000.csv 100 2 0 | grep method | awk '{print $7}'` >> profile_time_gauss_random.csv
    echo -n ',' >> profile_time_gauss_random.csv
    echo -n `./iterativeSolver.O2 G ../data/A_1100.csv ../data/b_1100.csv 100 2 0 | grep method | awk '{print $7}'` >> profile_time_gauss_random.csv
    echo -n ',' >> profile_time_gauss_random.csv
    echo -n `./iterativeSolver.O2 G ../data/A_1200.csv ../data/b_1200.csv 100 2 0 | grep method | awk '{print $7}'` >> profile_time_gauss_random.csv
    echo -n ',' >> profile_time_gauss_random.csv
    echo `./iterativeSolver.O2 G ../data/A_1300.csv ../data/b_1300.csv 100 2 0 | grep method | awk '{print $7}'` >> profile_time_gauss_random.csv

    ((counter++))
done

rm profile_time_gauss_all1s.csv
echo "50,100,250,500,750,1000,1100,1200,1300" >> profile_time_gauss_all1s.csv

counter=1
while [ $counter -le $times ]
do
    echo $counter

    echo -n `./iterativeSolver.O2 G ../data/A_50.csv ../data/b_50.csv 100 1 0 | grep method | awk '{print $7}'` >> profile_time_gauss_all1s.csv
    echo -n ',' >> profile_time_gauss_all1s.csv
    echo -n `./iterativeSolver.O2 G ../data/A_100.csv ../data/b_100.csv 100 1 0 | grep method | awk '{print $7}'` >> profile_time_gauss_all1s.csv
    echo -n ',' >> profile_time_gauss_all1s.csv
    echo -n `./iterativeSolver.O2 G ../data/A_250.csv ../data/b_250.csv 100 1 0 | grep method | awk '{print $7}'` >> profile_time_gauss_all1s.csv
    echo -n ',' >> profile_time_gauss_all1s.csv
    echo -n `./iterativeSolver.O2 G ../data/A_500.csv ../data/b_500.csv 100 1 0 | grep method | awk '{print $7}'` >> profile_time_gauss_all1s.csv
    echo -n ',' >> profile_time_gauss_all1s.csv
    echo -n `./iterativeSolver.O2 G ../data/A_750.csv ../data/b_750.csv 100 1 0 | grep method | awk '{print $7}'` >> profile_time_gauss_all1s.csv
    echo -n ',' >> profile_time_gauss_all1s.csv
    echo -n `./iterativeSolver.O2 G ../data/A_1000.csv ../data/b_1000.csv 100 1 0 | grep method | awk '{print $7}'` >> profile_time_gauss_all1s.csv
    echo -n ',' >> profile_time_gauss_all1s.csv
    echo -n `./iterativeSolver.O2 G ../data/A_1100.csv ../data/b_1100.csv 100 1 0 | grep method | awk '{print $7}'` >> profile_time_gauss_all1s.csv
    echo -n ',' >> profile_time_gauss_all1s.csv
    echo -n `./iterativeSolver.O2 G ../data/A_1200.csv ../data/b_1200.csv 100 1 0 | grep method | awk '{print $7}'` >> profile_time_gauss_all1s.csv
    echo -n ',' >> profile_time_gauss_all1s.csv
    echo `./iterativeSolver.O2 G ../data/A_1300.csv ../data/b_1300.csv 100 1 0 | grep method | awk '{print $7}'` >> profile_time_gauss_all1s.csv

    ((counter++))
done
