# Polygonization of Point Sets

- Efstathios Chatziloizos
- Evangelia Steiropoulou

This C++ program generates a simple polygon from a set of points in 2D space using either the Incremental algorithm or the Convex Hull algorithm. The program utilizes the CGAL library and provides detailed comments explaining each step of the implementation.

## Files

- README.md: This file.
- [Makefile](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/bd861eb4fc3a76064ee2aa1d38c3f7c36491eaac/Project1_Polygonization/Makefile): Makefile for building the program.
- [report.txt](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/9bbc53644100587e8cf089fde05c166fa6761338/Project1_Polygonization/report.txt): Comparison of results for both algorithms with different edge selections and initializations.
- [main.cpp](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/9bbc53644100587e8cf089fde05c166fa6761338/Project1_Polygonization/main.cpp): Main program that reads input points, creates a polygon based on user-specified parameters, and generates an output file.
- [sharedLib.h](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/9bbc53644100587e8cf089fde05c166fa6761338/Project1_Polygonization/sharedLib.h): Header file containing function declarations, typedefs, and defines used by both algorithms and the main program.
- [sharedLib.cpp](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/9bbc53644100587e8cf089fde05c166fa6761338/Project1_Polygonization/sharedLib.cpp): Source code for functions shared by both algorithms and the main program.
- [incrementalAlgorithm.h](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/9bbc53644100587e8cf089fde05c166fa6761338/Project1_Polygonization/incrementalAlgorithm.h): Header file for Incremental algorithm function declarations, typedefs, and defines.
- [incrementalAlgorithm.cpp](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/9bbc53644100587e8cf089fde05c166fa6761338/Project1_Polygonization/incrementalAlgorithm.cpp): Source code for functions used by the Incremental algorithm.
- [convexHullAlgorithm.h](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/9bbc53644100587e8cf089fde05c166fa6761338/Project1_Polygonization/convexHullAlgorithm.h): Header file for Convex Hull algorithm function declarations, typedefs, and defines.
- [convexHullAlgorithm.cpp](https://github.com/StathisChatziloizos/CGAL-Polygon-Optimization/blob/9bbc53644100587e8cf089fde05c166fa6761338/Project1_Polygonization/convexHullAlgorithm.cpp): Source code for functions used by the Convex Hull algorithm.

## Compile & Run

To build the program, follow these commands:

  $ cgal_create_CMakeLists -s to_polygon
  $ cmake -DCGAL_DIR=/usr/lib/CGAL -DCMAKE_BUILD_TYPE=Release
  $ make

To run the program, the following command must be used:

  $ ./to_polygon –i <point set input file> –ο <output file>
 –algorithm <incremental or convex_hull>
 -edge_selection <1 or 2 or 3>
 -initialization <1a or 1b or 2a or 2b | for incremental>
 -onion_initialization <1 to 5>

The onion_initialization parameter can be omitted altogether becasue
the onion algorithm hasn't been implemented. An example of a run
command can be:

  $ ./to_polygon -i input_file.instance -o output_file.txt
   -algorithm incremental -edge_selection 3 -initialization 2b cannot be edited in github
