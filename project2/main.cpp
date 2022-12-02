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

typedef Polygon_2::Segment_2 Segment_2;

class PathEdge
{
public:
    Polygon_2 path;
    Segment_2 edge;
};


void findPaths(Polygon_2 A, std::vector<Polygon_2> &paths, int k);
void printPaths(std::vector<Polygon_2> paths);


int main(int argc, char** argv)
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


    // Length k ≤ 10 and threshold are user-defined parameters.
    int k = 3 ;
    int threshold = 10;
    int deltaArea = 100;

    // Initialize: Obtain (greedy) solution S; T ← ∅
    Polygon_2 A;
    int constructionTime = 0;

    // Create the polygon A according to the algorithm chosen by the user
    A = convexHullAlgorithm(polygon, edgeSelection, constructionTime);

    if (!A.is_simple())
        std::cout << "Polygon A is not simple" << std::endl;
    else
        std::cout << "Polygon A is  simple" << std::endl;


    // while ∆A ≥ threshold do
    // while (deltaArea >= threshold)
    // {
        int area = A.area();

        std::vector<PathEdge> T;
        // for every edge e ∈ S do
        for (auto e = A.edges_begin(); e != A.edges_end(); e++)
        {
            std::cout << "Here 1" << std::endl;
            std::vector<Polygon_2> paths;
            // for every path V of length ≤ k do
            findPaths(A, paths, k);         // TODO: EXCLUDE FROM PATH THE EDGE E
            for(auto path : paths)
            {
                Polygon_2 B = A;
                // if V moving to e increases area and retains simplicity then


                Polygon_2 addedArea = path;

                // Remove path from B
                std::cout << "Here 2" << std::endl;
                for (auto p = path.vertices_begin(); p != path.vertices_end(); p++)
                    B.erase(std::find(B.begin(), B.end(), *p));
                // Add path between e.begin() and e.end()
                int indexSource = findPolygonPoint(B, e->source());
                int indexTarget = findPolygonPoint(B, e->target());
                if (indexSource < indexTarget)
                {
                    for (auto p = path.vertices_begin(); p != path.vertices_end(); p++)
                        B.insert(B.vertices_begin() + indexTarget, *p);
                }
                else
                {
                    for (auto p = path.vertices_begin(); p != path.vertices_end(); p++)
                        B.insert(B.vertices_begin() + indexSource, *p);
                }
                std::cout << "Here 3" << std::endl;
                if (B.is_simple())
                {
                    T.push_back(PathEdge{path, *e});
                    std::cout << "Here 4" << std::endl;

                }// end if
            }// end for
            std::cout << "Here 5" << std::endl;
        }// end for

        // print vector T
        std::cout << "T: " << std::endl;
        for (auto t : T)
        {
            std::cout << "Path: " << std::endl;
            for (auto p = t.path.vertices_begin(); p != t.path.vertices_end(); p++)
                std::cout << "(" << p->x() << ", " << p->y() << ")" << std::endl;
            std::cout << "Edge: " << std::endl;
            std::cout << "(" << t.edge.source().x() << ", " << t.edge.source().y() << ")" << std::endl;
            std::cout << "(" << t.edge.target().x() << ", " << t.edge.target().y() << ")" << std::endl;
        }

        // Apply all changes in T to S
        if (!T.empty())
        {
            PathEdge chosenPEdge = T[0];
            // Remove path from A
            for (auto p = chosenPEdge.path.vertices_begin(); p != chosenPEdge.path.vertices_end(); p++)
                A.erase(std::find(A.begin(), A.end(), *p));
            
            // Add path between e.begin() and e.end()
            int indexSource = findPolygonPoint(A, chosenPEdge.edge.source());
            int indexTarget = findPolygonPoint(A, chosenPEdge.edge.target());

            if (indexSource < indexTarget)
            {
                for (auto p = chosenPEdge.path.vertices_begin(); p != chosenPEdge.path.vertices_end(); p++)
                    A.insert(A.vertices_begin() + indexTarget, *p);
            }
            else
            {
                for (auto p = chosenPEdge.path.vertices_begin(); p != chosenPEdge.path.vertices_end(); p++)
                    A.insert(A.vertices_begin() + indexSource, *p);
            }

        }
        deltaArea = A.area() - area;
        // Keep best solution S′; ∆A ← Area(S′) − Area(S)
    // }// end while





    return 0;
}

void findPaths(Polygon_2 A, std::vector<Polygon_2> &paths, int k)
{
    // for every vertex of A
    for (auto v = A.vertices_begin(); v != A.vertices_end(); v++)
    {
        auto saveV = v;
        for(int i = 1; i <= k; i++)
        {
            Polygon_2 path;
            for(int j = 1; j <= i; j++)
            {
                path.push_back(*v);
                v++;
                if(v == A.vertices_end())
                    v = A.vertices_begin();
            }

            v = saveV;
            paths.push_back(path);
        }
    }
}

void printPaths(std::vector<Polygon_2> paths)
{
    int i = 1;
    for (auto path : paths)
    {

        std::cout << "Path " << i  << ":" << std::endl;
        for (auto point : path)
        {
            std::cout << point << std::endl;
        }
        i++;
    }
}