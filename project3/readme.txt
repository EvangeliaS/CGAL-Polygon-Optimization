- Efstathios Chatziloizos, 1115201800212
- Evangelia Steiropoulou, 1115201800186


The program is given an input directory, splits it into clusters of files with the same
amount of points. For each cluster of same size files it runs 6 distinct algorithms for
creating a polygon from the points, while maximizing the area of the polygon and the same
6 algorithms for minimizing the area of the polygon. Then the scores of the algorithms are
calculated for each cluster of same size files and they are written to an output file. If
at any point, for any algorithm, the program fails to create a simple polygon or algorithm
exceeds its time limit, the program attributes said algorithm a score of 0 for maximizing
and 1 for minimizing the area of the polygon.


INPUT FILE NAMES
----------------

The input files must end with -<7digits>.instance:
   e.g. -0000000.instance

The problem occurs with uniform input files so before adding them to the input directory
remove the -1 or -2.

    e.g. uniform-0000010-1.instance -> uniform-0000010.instance
         uniform-0000010-2.instance -> uniform-0000010.instance

FILES:
------

- readme.txt
- report.txt
    Includes an example of an output file.
- main.cpp
    Simple main that reads the points from the input file, creates the
    polygon based on the parameters specified by the user. After it optimizes
	it based on the selected algorithm provided by the user. Finally, it creates
	the output file.
- sharedLib.h:
    Header file that contains the declarations of the functions, the
    typedefs and defines used by both the Incremental and the Convex Hull
    algorithms or by main.
- sharedLib.cpp:
    Contains the source code for the aforementioned functions.
- incrementalAlgorithm.h:
    Header file that contains the declarations of the functions, the
    typedefs and defines used by the Incremental algorithm.
- incrementalAlgorithm.cpp:
    Contains the source code for the functions used by the Incremental
    algorithm.
- convexHullAlgorithm.h:
    Header file that contains the declarations of the functions, the
    typedefs and defines used by the Convex Hull algorithm.
- convexHullAlgorithm.cpp:
    Contains the source code for the functions used by the algorithm
    based on the Convex Hull.
- local_search.cpp:
	Contains the source code for the functions used by the local search
	algorithm.
- local_search.h:
	Header file that contains the declarations of the functions, the
	typedefs and defines used by the local search algorithm.
- simulated_annealing.cpp:
	Contains the source code for the functions used by the simulated annealing
	algorithm.
- simulated_annealing.h:
	Header file that contains the declarations of the functions, the
	typedefs and defines used by the simulated annealing algorithm.
- testDir:
    Contains the input files used for the output file.


COMPILE & RUN
-------------

The program is built using CMake. The following commands must be used
to build the program:

	cgal_create_CMakeLists -s evaluate && cmake -DCGAL_DIR=/usr/lib/CGAL -DCMAKE_BUILD_TYPE=Release && make

To run the program, the following command must be used:

	./evaluate –i testDir/ –ο output.txt


ALGORITHMS
----------

1) Incremental & Local Search

2) Incremental & Simulated Annealing (Local Step)

3) Incremental & Simulated Annealing (Global Step)

4) Convex Hull & Local Search

5) Convex Hull & Simulated Annealing (Local Step)

6) Convex Hull & Simulated Annealing (Global Step)


All algorithms are run twice for each file of the input directory. Once
to maximize the area of the polygon and once to minimize the area of the
polygon.

HYPERPARAMETERS (PREPROCESSING)
-------------------------------

The hyperparameters of each algorithm were chosen so as to simultaneously
create a polygon with the best possible score, while also being able to run
in a reasonable amount of time, namely in less than the cut-off point,
500 * n milliseconds, where n is the number of points of the file used.
More on that can be found in the report.