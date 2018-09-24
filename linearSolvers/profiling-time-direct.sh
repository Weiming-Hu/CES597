#!/bin/bash
cd output-profile-optimization
times=10

rm profile_time_direct.csv
echo "50,100,250,500,750,1000,1100,1200,1300" >> profile_time_direct.csv

counter=1
while [ $counter -le $times ]
do
    echo $counter

    echo -n `./directSolver.O2 ../data/A_50.csv ../data/b_50.csv 0 | grep method | awk '{print $7}'` >> profile_time_direct.csv
    echo -n ',' >> profile_time_direct.csv
    echo -n `./directSolver.O2 ../data/A_100.csv ../data/b_100.csv 0 | grep method | awk '{print $7}'` >> profile_time_direct.csv
    echo -n ',' >> profile_time_direct.csv
    echo -n `./directSolver.O2 ../data/A_250.csv ../data/b_250.csv 0 | grep method | awk '{print $7}'` >> profile_time_direct.csv
    echo -n ',' >> profile_time_direct.csv
    echo -n `./directSolver.O2 ../data/A_500.csv ../data/b_500.csv 0 | grep method | awk '{print $7}'` >> profile_time_direct.csv
    echo -n ',' >> profile_time_direct.csv
    echo -n `./directSolver.O2 ../data/A_750.csv ../data/b_750.csv 0 | grep method | awk '{print $7}'` >> profile_time_direct.csv
    echo -n ',' >> profile_time_direct.csv
    echo -n `./directSolver.O2 ../data/A_1000.csv ../data/b_1000.csv 0 | grep method | awk '{print $7}'` >> profile_time_direct.csv
    echo -n ',' >> profile_time_direct.csv
    echo -n `./directSolver.O2 ../data/A_1100.csv ../data/b_1100.csv 0 | grep method | awk '{print $7}'` >> profile_time_direct.csv
    echo -n ',' >> profile_time_direct.csv
    echo -n `./directSolver.O2 ../data/A_1200.csv ../data/b_1200.csv 0 | grep method | awk '{print $7}'` >> profile_time_direct.csv
    echo -n ',' >> profile_time_direct.csv
    echo `./directSolver.O2 ../data/A_1300.csv ../data/b_1300.csv 0 | grep method | awk '{print $7}'` >> profile_time_direct.csv


    ((counter++))
done
