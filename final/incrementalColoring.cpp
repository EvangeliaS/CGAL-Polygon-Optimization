#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/intersections.h>
#include <CGAL/Polygon_2.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

#include "coloring.h"

int main(int argc, char **argv)
{
    if(argc < 11) {
        std::cout << "Not enough arguments" << std::endl;
        return ERROR;
    }
    std::string inputFile = argv[2];
    std::string outputFile = argv[4];
    std::string algorithm = argv[6];
    int edgeSelection = atoi(argv[8]);
    if (edgeSelection != 1 && edgeSelection != 2 && edgeSelection != 3)
    {
        std::cout << "Invalid edge selection algorithm. Please choose 1, 2 or 3." << std::endl;
        return ERROR;
    }
    std::string initialization = argv[10];
    if (initialization != "1a" && initialization != "1b"
        && initialization != "2a" && initialization != "2b")
    {
        std::cout << "Invalid initialization algorithm. Please choose 1a, 1b, 2a or 2b." << std::endl;
        return ERROR;
    }
    int onionInitialization = atoi(argv[12]);

    srand(time(0));

    std::ifstream input(inputFile);
    if (!input)
    {
        std::cout << "Could not open file " << inputFile << std::endl;
        return ERROR;
    }

    // Ignore first two lines
    std::string line;
    std::getline(input, line);
    std::getline(input, line);

    Polygon_2 polygon;

    // Loop to read inputFile line by line
    while (std::getline(input, line))
    {
        std::vector<std::string> tokens = split(line, '\t');
        polygon.push_back(Point_2(std::stod(tokens[1]), std::stod(tokens[2])));
    }
    std::cout << "----------------------------" << std::endl;
    printPolygon(polygon);
    std::cout << "----------------------------" << std::endl;


    if(initialization == "1a")
    {
        // Sort points by x coordinate largest to smallest
        std::sort(polygon.begin(), polygon.end(), [](Point_2 p1, Point_2 p2)
                { return p1.x() > p2.x(); });
    }
    else if(initialization == "1b")
    {
        // Sort points by x coordinate smallest to largest
        std::sort(polygon.begin(), polygon.end(), [](Point_2 p1, Point_2 p2)
                { return p1.x() < p2.x(); });
    }
    else if(initialization == "2a")
    {
        // Sort points by y coordinate largest to smallest
        std::sort(polygon.begin(), polygon.end(), [](Point_2 p1, Point_2 p2)
                { return p1.y() > p2.y(); });
    }
    else if(initialization == "2b")
    {
        // Sort points by y coordinate smallest to largest
        std::sort(polygon.begin(), polygon.end(), [](Point_2 p1, Point_2 p2)
                { return p1.y() < p2.y(); });
    }


    if(algorithm == "incremental")
        incrementalAlgorithm(polygon, edgeSelection);
    else if(algorithm == "convex_hull")
    {}
    else if(algorithm == "onion")
    {
        std::cout << "Onion algorithm was not implemented. Choose incremental or convex hull." << std::endl;
        return ERROR;
    }
    else
    {
        std::cout << "Invalid algorithm. Choose incremental or convex_hull." << std::endl;
        return ERROR;
    }

    std::cout << "-------------polygon-------------" << std::endl;
    printPolygon(polygon);
    std::cout << "---------------------------------" << std::endl;


    Polygon_2 A, convexHull;

     // Calculate convex hull of polygon
    CGAL::convex_hull_2(polygon.begin(), polygon.end(), std::back_inserter(convexHull));
    A = convexHull;

    std::cout << "----------------A----------------" << std::endl;
    printPolygon(A);
    std::cout << "---------------------------------" << std::endl;

    // find the points in polygon that are not in A
    // and add them to B
    // B = polygon - A
    Polygon_2 B;
    for (auto it = polygon.begin(); it != polygon.end(); ++it)
    {
        
    }
    std::cout << "----------------B----------------" << std::endl;
    printPolygon(A);
    std::cout << "---------------------------------" << std::endl;

    return 0;
}