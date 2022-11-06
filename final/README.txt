Assignment 1 - Polygonization of point sets
-------------------------------------------

- Efstathios Chatziloizos, 1115201800212
- Evangelia Steiropoulou, 1115201800186


The program is given a set of points in 2D space and creates a simple
polygon out of them. The polygon is created according to either the
Incremental algorithm or the algorithm based on the Convex Hull. The
program is written in C++ and uses the CGAL library. Extensive comments
have been used throught the source code, explaining in detail every
step of the implementaion.



Files
-----

- README.txt
- Makefile
- main.cpp:
    Simple main that reads the points from the input file, creates the
    polygon based on the parameters specified by the user, using the
    Incremental or the Convex Hull algorithm. Finally, it creates the
    output file.  
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

COMPILE & RUN
-------------

The program is built using CMake. The following commands must be used
to build the program:
    
        $cgal_create_CMakeLists -s to_polygon
        $cmake -DCGAL_DIR=/usr/lib/CGAL -DCMAKE_BUILD_TYPE=Release
        $make

To run the program, the following command must be used:

        $./to_polygon –i <point set input file> –ο <output file>
         –algorithm <incremental or convex_hull>
         -edge_selection <1 or 2 or 3>
         -initialization <1a or 1b or 2a or 2b | for incremental>
         -onion_initialization <1 to 5>

The onion_initialization parameter can be omitted altogether becasue
the onion algorithm hasn't been implemented. An example of a run
command can be:

        $./to_polygon -i input_file.instance -o output_file.txt
         -algorithm incremental -edge_selection 3 -initialization 2b