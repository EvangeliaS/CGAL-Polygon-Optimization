# Polygon Optimization Program

- Efstathios Chatziloizos
- Evangelia Steiropoulou

## Introduction

This C++ program is designed to optimize polygons using various algorithms. It operates on an input directory, grouping files with the same number of points. For each cluster of files with the same point count, the program runs six distinct algorithms for both maximizing and minimizing the polygon's area. The scores for each algorithm in each cluster are then calculated and written to an output file. If any algorithm fails to create a simple polygon or exceeds its time limit, it receives a score of 0 for maximization and 1 for minimization of the polygon's area.

## Input File Names

Input files must have names ending with -<7digits>.instance, for example, `-0000000.instance`. Ensure that uniform input files do not contain -1 or -2 before adding them to the input directory.

- `uniform-0000010-1.instance` -> `uniform-0000010.instance`
- `uniform-0000010-2.instance` -> `uniform-0000010.instance`

## Files

- [readme.txt](readme.txt): Description of the program.
- [report.txt](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/95715118d8b0ccf8cbd502c2c3cc14620c8dce38/Project3_Polygon_Evaluation/report.txt): Includes an example of an output file.
- [main.cpp](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/95715118d8b0ccf8cbd502c2c3cc14620c8dce38/Project3_Polygon_Evaluation/main.cpp): Main program that reads points from the input file, creates polygons, optimizes them based on user-specified parameters, and generates the output file.
- [sharedLib.h](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/95715118d8b0ccf8cbd502c2c3cc14620c8dce38/Project3_Polygon_Evaluation/sharedLib.h): Header file containing function declarations, typedefs, and defines shared between the Incremental and Convex Hull algorithms or used by the main program.
- [sharedLib.cpp](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/95715118d8b0ccf8cbd502c2c3cc14620c8dce38/Project3_Polygon_Evaluation/sharedLib.cpp): Source code for the aforementioned functions
- [incrementalAlgorithm.h](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/95715118d8b0ccf8cbd502c2c3cc14620c8dce38/Project3_Polygon_Evaluation/incrementalAlgorithm.h): Header file for the Incremental algorithm.
- [incrementalAlgorithm.cpp](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/95715118d8b0ccf8cbd502c2c3cc14620c8dce38/Project3_Polygon_Evaluation/incrementalAlgorithm.cpp): Source code for the Incremental algorithm
- [convexHullAlgorithm.h](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/95715118d8b0ccf8cbd502c2c3cc14620c8dce38/Project3_Polygon_Evaluation/convexHullAlgorithm.h): Header file for the Convex Hull algorithm.
- [convexHullAlgorithm.cpp](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/95715118d8b0ccf8cbd502c2c3cc14620c8dce38/Project3_Polygon_Evaluation/convexHullAlgorithm.cpp): Source code for the Convex Hull algorithm
- [local_search.cpp](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/95715118d8b0ccf8cbd502c2c3cc14620c8dce38/Project3_Polygon_Evaluation/local_search.cpp): Source code for the functions used by the local search algorithm
- [local_search.h](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/95715118d8b0ccf8cbd502c2c3cc14620c8dce38/Project3_Polygon_Evaluation/local_search.h): Header file for the local search algorithm.
- [simulated_annealing.cpp](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/95715118d8b0ccf8cbd502c2c3cc14620c8dce38/Project3_Polygon_Evaluation/simulated_annealing.cpp): Source code for the functions used by the simulated annealing algorithm
- [simulated_annealing.h](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/95715118d8b0ccf8cbd502c2c3cc14620c8dce38/Project3_Polygon_Evaluation/simulated_annealing.h): Header file for the simulated annealing algorithm.
- [testDir](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/tree/95715118d8b0ccf8cbd502c2c3cc14620c8dce38/Project3_Polygon_Evaluation/testDir): Contains input files used for the output file.

## Compilation and Execution

To build the program using CMake, follow these commands:

```shell
cgal_create_CMakeLists -s evaluate && cmake -DCGAL_DIR=/usr/lib/CGAL -DCMAKE_BUILD_TYPE=Release && make
```

To run the program, use the following command as an example:

```shell
./evaluate –i testDir –ο output.txt
```

Adjust the input directory and output file as needed for your specific use case.

## Algorithms

The program runs six distinct algorithms for both maximizing and minimizing the polygon's area:

1) Incremental & Local Search
2) Incremental & Simulated Annealing (Local Step)
3) Incremental & Simulated Annealing (Global Step)
4) Convex Hull & Local Search
5) Convex Hull & Simulated Annealing (Local Step)
6) Convex Hull & Simulated Annealing (Global Step)

All algorithms are run twice for each file in the input directory, once to maximize the polygon's area and once to minimize it.

## Hyperparameters (Preprocessing)

Hyperparameters for each algorithm are chosen to balance performance and runtime. The program includes a cutoff point to ensure execution within reasonable time limits, as discussed in the report.

## Further Details

### Compilation and Build

The program is built using CMake. To compile the program, follow these steps

:

1. Run `cgal_create_CMakeLists` to generate the CMakeLists file.
2. Use `cmake` to configure the build, specifying the location of the CGAL library with `-DCGAL_DIR`.
3. Build the program with `make`.

### Running the Program

To execute the program, use the following command as an example:

```shell
./evaluate –i testDir –ο output.txt
```

Adjust the input directory and output file as needed for your specific use case.

### Algorithm Options

The program employs a variety of optimization algorithms for polygon creation and area maximization/minimization. For details on these algorithms, refer to the report.

### Hyperparameters

Each algorithm is configured with specific hyperparameters to balance performance and runtime. The program includes a cutoff point to ensure execution within reasonable time limits, as discussed in the report.

## Conclusion

This program provides a comprehensive tool for optimizing polygons using multiple algorithms. Refer to the report for detailed insights and examples of program usage. Feel free to explore the source code and adapt it to your specific optimization needs.
```
