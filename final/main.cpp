#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/intersections.h>
#include <CGAL/Polygon_2.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

#include "sharedLib.h"
#include "incrementalAlgorithm.h"
#include "convexHullAlgorithm.h"


int main(int argc, char **argv)
{
    if (argc < 11)
    {
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
    if (initialization != "1a" && initialization != "1b" && initialization != "2a" && initialization != "2b")
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
 
    // Check the edge selection input if the incremental algorithm is chosen
    if (algorithm == "incremental")
    {
        if (initialization == "1a")
        {
            // Sort points by x coordinate largest to smallest
            std::sort(polygon.begin(), polygon.end(), [](Point_2 p1, Point_2 p2)
                      { return p1.x() > p2.x(); });
        }
        else if (initialization == "1b")
        {
            // Sort points by x coordinate smallest to largest
            std::sort(polygon.begin(), polygon.end(), [](Point_2 p1, Point_2 p2)
                      { return p1.x() < p2.x(); });
        }
        else if (initialization == "2a")
        {
            // Sort points by y coordinate largest to smallest
            std::sort(polygon.begin(), polygon.end(), [](Point_2 p1, Point_2 p2)
                      { return p1.y() > p2.y(); });
        }
        else if (initialization == "2b")
        {
            // Sort points by y coordinate smallest to largest
            std::sort(polygon.begin(), polygon.end(), [](Point_2 p1, Point_2 p2)
                      { return p1.y() < p2.y(); });
        }
    }

    Polygon_2 A;
    int constructionTime = 0;

    // Create the polygon A according to the algorithm chosen by the user
    if (algorithm == "incremental")
        A = incrementalAlgorithm(polygon, edgeSelection, constructionTime);
    else if (algorithm == "convex_hull")
        A = convexHullAlgorithm(polygon, edgeSelection, constructionTime);
    else if (algorithm == "onion")
    {
        std::cout << "Onion algorithm was not implemented. Choose incremental or convex hull." << std::endl;
        return ERROR;
    }
    else
    {
        std::cout << "Invalid algorithm. Choose incremental or convex_hull." << std::endl;
        return ERROR;
    }

    // Compute the area of the convexHull of the point set given
    Polygon_2 convexHull;
    CGAL::convex_hull_2(polygon.begin(), polygon.end(), std::back_inserter(convexHull));

    // Compute the area of the polygon A
    // Find the ratio of the area of A to the area of the convex hull
    // Write the necessary information to the output file
    writeToOutput(A, std::abs(A.area()), std::abs(A.area()) / std::abs(convexHull.area()), constructionTime, outputFile, algorithm, edgeSelection, initialization);
    std::cout << "Done!\nOutput written to " << outputFile << std::endl;

    return 0;
}
