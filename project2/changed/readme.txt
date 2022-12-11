- Efstathios Chatziloizos, 1115201800212
- Evangelia Steiropoulou, 1115201800186


The program is given an input file and creates a  simple polygon out of them.
After that it area optimizes it according to the algorithm choice that is
provided by the user. The program is written in C++ and uses the CGAL library.
Extensive comments have been used throught the source code, explaining
in detail every step of the implementaion.

FILES:

- readme.txt
- report.txt
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


COMPILE & RUN
-------------

The program is built using CMake. The following commands must be used
to build the program:

	cgal_create_CMakeLists -s optimal_polygon && cmake -DCGAL_DIR=/usr/lib/CGAL -DCMAKE_BUILD_TYPE=Release && make

To run the program, the following command must be used:

	./optimal_polygon –i instances/data/images/euro-night-0000100.instance –ο output.txt –algorithm simulated_annealing -L 1000 -max -threshold 10.45 -annealing local -initialization_algorithm convex_hull


 - We added another parameter at the end of the command, which is the greedy algorithm
needed for the initialization(incremental/convex_hull)

For minimization and maximization algorithms, we choose edge selection 2 and 3 for the greedy algorithms respectively. Having the minimim or maximum polygon area, created from the greedy algorithm, we had even better results with the optimization.


Local Search
------------

- The local search algorithm is implemented and works as intended. 

- The L parameter when running the program is the value k of the algorithm (max number
  of vertices of the path).

- Examples of runs of the local search algorithm are given in the report.

Simulated Annealing
-------------------

- The simulated annealing algorithm is implemented and works as intended for the local
  steep. A Kd-Tree is used to avoid unnecessary calculations.

- For the global Step the validation is done by checking if the polygon is simple or not

Subdivision Algorithm (Not implemented)
---------------------------------------

For the subdivision algorithm we couldn't make sure that the critical edges after the
greedy algorithm were intact and therefore we couldn't use the subdivision algorithm.

However, the algorithm's helper functions are implemented .For example,
void splitIntoSubsets(Polygon_2& pointset, std::vector<Polygon_2>& subsets)
works as intended and can be used to split the initial pointset into subsets keeping
the monotonicity of the leftmost and rightmost points of each subset.