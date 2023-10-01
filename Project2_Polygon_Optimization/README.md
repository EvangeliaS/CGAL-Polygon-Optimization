# Polygon Optimization Program

- Efstathios Chatziloizos
- Evangelia Steiropoulou

## Introduction

This C++ program is designed for polygon optimization using the CGAL library. It offers multiple optimization algorithms, including Incremental, Convex Hull, Local Search, and Simulated Annealing. This README provides an overview of the program's structure and usage.

## Files

- README.md: Description of the program.
- [report.txt](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/6ac0c71f9d437004321efb7a4b647f4c72d95320/Project2_Polygon_Optimization/report.txt): Detailed information about the program's algorithms and usage.
- [main.cpp](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/6ac0c71f9d437004321efb7a4b647f4c72d95320/Project2_Polygon_Optimization/main.cpp): Main program that processes input files, creates polygons, and performs optimization.
- [sharedLib.h](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/6ac0c71f9d437004321efb7a4b647f4c72d95320/Project2_Polygon_Optimization/sharedLib.h): Header file containing function declarations, typedefs, and defines shared across different algorithms.
- [sharedLib.cpp](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/6ac0c71f9d437004321efb7a4b647f4c72d95320/Project2_Polygon_Optimization/sharedLib.cpp): Source code for shared functions.
- [incrementalAlgorithm.h](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/6ac0c71f9d437004321efb7a4b647f4c72d95320/Project2_Polygon_Optimization/incrementalAlgorithm.h): Header file for the Incremental algorithm.
- [incrementalAlgorithm.cpp]([incrementalAlgorithm.cpp](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/6ac0c71f9d437004321efb7a4b647f4c72d95320/Project2_Polygon_Optimization/incrementalAlgorithm.cpp): Source code for the Incremental algorithm.
- [convexHullAlgorithm.h](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/6ac0c71f9d437004321efb7a4b647f4c72d95320/Project2_Polygon_Optimization/convexHullAlgorithm.h): Header file for the Convex Hull algorithm.
- [convexHullAlgorithm.cpp](convexHullAlgorithm.cpp): Source code for the Convex Hull algorithm.
- [local_search.h](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/6ac0c71f9d437004321efb7a4b647f4c72d95320/Project2_Polygon_Optimization/local_search.h): Header file for the Local Search algorithm.
- [local_search.cpp](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/6ac0c71f9d437004321efb7a4b647f4c72d95320/Project2_Polygon_Optimization/local_search.cpp): Source code for the Local Search algorithm.
- [simulated_annealing.h](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/6ac0c71f9d437004321efb7a4b647f4c72d95320/Project2_Polygon_Optimization/simulated_annealing.h): Header file for the Simulated Annealing algorithm.
- [simulated_annealing.cpp](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/6ac0c71f9d437004321efb7a4b647f4c72d95320/Project2_Polygon_Optimization/simulated_annealing.cpp): Source code for the Simulated Annealing algorithm.


## Compilation and Execution

To build the program using CMake, follow these commands:

  $ cgal_create_CMakeLists -s optimal_polygon && cmake -DCGAL_DIR=/usr/lib/CGAL -DCMAKE_BUILD_TYPE=Release && make

To run the program, use the following command:


  $ ./optimal_polygon –i instances/data/images/euro-night-0000100.instance –o output.txt –algorithm simulated_annealing -L 1000 -max -threshold 10.45 -annealing local -initialization_algorithm convex_hull

- Additional parameter for greedy algorithm: Use `-max` for maximization and `-min` for minimization.

## Local Search

- The Local Search algorithm is implemented with a parameter `-L` specifying the maximum number of vertices in the path.

## Simulated Annealing

- The Simulated Annealing algorithm is implemented for local steepness and global optimization.
- A Kd-Tree is used to avoid unnecessary calculations.
- For global optimization, custom validation functions can be enabled as described in the report.

## Subdivision Algorithm (Not implemented)

The subdivision algorithm is not implemented due to concerns about critical edges. However, helper functions like `splitIntoSubsets` are available for future use.
Feel free to explore the source code and report for more details and examples of program usage.
