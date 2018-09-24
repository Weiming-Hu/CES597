# CSE 597 Course Project Repository

* [Basic Information](#basic-information)
    * [Abstract](#abstract)
        * [Compilation instructions](#compilation-instructions)
                * [Code](#code)
                * [Tunable Options in CMake](#tunable-options-in-cmake)
        * [Write-Up](#write-up)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

# Basic Information

Author: [Weiming Hu](weiming.ddns.net)

This project aims to develop an efficient C++ solver for linear systems, or the Ax = b problems. The solver is applied to verify probabilistic forecasts generated from the Analog Ensemble technique.

Please see [LICENSE.txt](https://github.com/Weiming-Hu/CSE597/blob/master/LICENSE.txt) for licensing information.

An overview of the files is provided below.

- `R/` contains the R scripts for data analyses and visualization.
- `data/` contains the test data sets and the R script to generate them.
- `.gitignore` is the file specifying which files should be ignored in Git.
- `CMakeLists.txt` guides CMake to generate a make file. Please see README.md for detailed usage.
- `LICENSE.txt` is the MIT license.
- `Matrix.cpp` is the source file for Matrix library.
- `Matrix.h` is the header file for Matrix library.
- `README.md` contains basic information for the repository and detailed information for how to compile and reproduce the results.
- `directSolver.cpp` is the source file for the direct solver.
- `iterativeSolver.cpp` is the source file the iterative solver.
- `optimization-flags-compile.sh` is the bash script to compile solvers with different optimization flags and to organize them in a particular folder.
- `optimization-flags-test-direct.sh` is the bash script to test the direct solver with different optimization flags.
- `optimization-flags-test-gauss.sh` is the bash script to test the Gauss-Seidel iterative solver with different optimization flags.
- `profiling-time-direct.sh` is the bash script to profile the direct solver.
- `profiling-time-guass.sh` is the bash script to profile the iterative solver.
- `testMatrix.cpp` is the source file for testing Matrix library.

## Abstract

Analog Ensemble is a highly parallelizable and scalable technique to generate probabilistic forecasts using historical deterministic predictions and the corresponding observations. Since the year of 2013 when it is first brought up, it has been successfully applied to short-term wind and temperature forecasts and also to spatial and temporal down-scaling of other numeric weather prediction models, like the Global Forecast System. However, most of the studies focused on the application of the technique, rather than how to improve and complete the technique. Actually, problems have already emerged when the Analog Ensemble technique is applied to a gridded model output. Because of the intrinsic feature of the search algorithm, the produced forecasts loses the spatial continuity that is usually present in a realistic physical world. To get a better understanding of the problem, a deeper inspection into the ensemble is needed.

This study proposes a matrix approach to analyze the contribution of individual ensemble member to the overall forecast distribution by setting up and solving a linear system. The results can be used to gain better understanding of ensemble members at different locations. The study also has a computational focus that it looks at different solvers and implementation, and tries to optimize the problem solving process.

### Compilation instructions

##### Code

[CMake](https://cmake.org/) is needed to generate the make file for the project. This project follows the conventional out-of-source build process.

```
cd CSE597/
mkdir build
cd build
cmake ..
make
# The compiled progrom will be in CSE597/output/
```

##### Tunable Options in CMake

|       Option       |                               Description                               |       Default      |
|:------------------:|:-----------------------------------------------------------------------:|:------------------:|
|         CC         |                          The C compiler to use.                         | [System dependent] |
|         CXX        |                         The C++ compiler to use.                        | [System dependent] |
| CMAKE\_BUILD\_TYPE | "Release" for building release verbose; "Debug" for debugging purposes. |        None        |
| CMAKE\_CXX\_FLAGS  |        The compiler and linking flags to be used for C++ codes.         |        None        |
|     EXE\_SUFFIX    |                        The suffix of executables.                       |        None        |
|    PROFILE\_TIME   |          Set it to "ON" to have profiling information printed.          |         OFF        |


### Write-Up

Report #1 can be found at [Overleaf](https://v2.overleaf.com/project/5b8dbf5764a73d48a03b6064)

```
# "`-''-/").___..--''"`-._
#  (`6_ 6  )   `-.  (     ).`-.__.`)   WE ARE ...
#  (_Y_.)'  ._   )  `._ `. ``-..-'    PENN STATE!
#    _ ..`--'_..-_/  /--'_.' ,'
#  (il),-''  (li),'  ((!.-'
# 
# Author: Weiming Hu <weiming@psu.edu>
#         
# Geoinformatics and Earth Observation Laboratory (http://geolab.psu.edu)
# Department of Geography and Institute for CyberScience
# The Pennsylvania State University
```
