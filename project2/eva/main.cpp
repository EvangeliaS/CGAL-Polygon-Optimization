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
#include "simulated_annealing.h"

//command: 
//./optimal_polygon –i instances/data/images/euro-night-0000010.instance –ο output.txt –algorithm convex_hull -edge_selection 3 -initialization 1b -onion_initialization 2

typedef Polygon_2::Segment_2 Segment_2;

class PathEdge
{
public:
    Polygon_2 path;
    Segment_2 edge;
    int deltaArea;
};

void findPaths(Polygon_2 A, std::vector<Polygon_2> &paths, int k, Segment_2 edge);
void printPaths(std::vector<Polygon_2> paths);
bool isEdgePartOfPath(Polygon_2 path, Segment_2 edge);

int main(int argc, char **argv)
{

/*
Polygon_2 B;

B.push_back(Point_2(0, 0));
B.push_back(Point_2(0, 100));
B.push_back(Point_2(100, 100));
B.push_back(Point_2(100, 0));
B.push_back(Point_2(0, 200));
B.push_back(Point_2(200, 300));
B.push_back(Point_2(300, 500));
B.push_back(Point_2(500, 0));

printPolygon(B);

swap_polygon_points(B, 0, 7);

swap_polygon_points(B, 5, 3);

swap_polygon_points(B, 0, 8);

std::cout << "______________________" << std::endl;
printPolygon(B);
*/

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

    int constructionTime = 0;
    Polygon_2 A;
    A = convexHullAlgorithm(polygon, edgeSelection, constructionTime);
    

    if (!A.is_simple())
    {
        std::cout << "Polygon A is not simple" << std::endl;
        return ERROR;
    }
    else
        std::cout << "Polygon A is  simple" << std::endl;

    bool isMaximization = true;
    bool isLocalStep = true;

    printPolygon(A);
    //Polygon_2 convex_hull;
    
    minimization_algorithm(A, isLocalStep, 10000,isMaximization);

    //local_transition_step(temp, a);
    //std::cout << "a = " << a << std::endl;
/*    Polygon_2 temp = A;
    int i = 0;
    int a = A.size()-1;

    std::cout << "Size " << " a " << A.size() << std::endl;
    local_transition_step(A, a) && A.is_simple();
    std::cout << " a " << a << std::endl;*/
/*
    while(i<10){


        if(!local_transition_step(temp, a) && temp.is_simple()){
            A = temp;

            std::cout << "Simple " << " a " << a << std::endl;
            i++;
            //break;
        }
        else{
            temp = A; 
            //std::cout << "Not simple " << std::endl;
        }

        a = rand()%A.size();
    }

*/

    std::cout << " Final area of polygon: " << std::abs(A.area()) << std::endl;
    printPolygon(A);


    return 0;
}