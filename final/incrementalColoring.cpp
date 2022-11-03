#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/intersections.h>
#include <CGAL/Polygon_2.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <float.h>
#include <algorithm>
#include <random>

#include "coloring.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Triangle_2 Triangle_2;
typedef CGAL::Polygon_2<Kernel> Polygon_2;
typedef std::pair<Polygon_2::Segment_2, Point_2> EdgeDistance;
typedef std::pair<EdgeDistance,double> TriangleArea;


Polygon_2 findInternalPoints(Polygon_2 polygon, Polygon_2 A);
std::vector<EdgeDistance> findMinEdgeDistances(Polygon_2 A, Polygon_2 internalPoints);
EdgeDistance findMinTriangleArea(Polygon_2 A,std::vector<EdgeDistance> edgeDistances, std::vector<TriangleArea>& triangleAreas);


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
    std::cout << "----------------------------" << std::endl;
    printPolygon(polygon);
    std::cout << "----------------------------" << std::endl;

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

    if (algorithm == "incremental")
        incrementalAlgorithm(polygon, edgeSelection);
    else if (algorithm == "convex_hull")
    {
    }
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

    Polygon_2 internalPoints = findInternalPoints(polygon, A);

    std::cout << "---------internal Points---------" << std::endl;
    printPolygon(internalPoints);
    std::cout << "---------------------------------" << std::endl;

    int counter = 0;
    while(!internalPoints.is_empty())
    {
        std::vector<EdgeDistance> minEdgeDistances = findMinEdgeDistances(A, internalPoints);
        // Print minEdgeDistances
        // std::cout << "---------minEdgeDistances--------" << std::endl;
        // for (int i = 0; i < minEdgeDistances.size(); i++)
        // {
        //     std::cout << "Segment: " << minEdgeDistances[i].first << " Point: " << minEdgeDistances[i].second << std::endl;
        // }
        // std::cout << "---------------------------------" << std::endl;


        std::vector<TriangleArea> triangleAreas;
        EdgeDistance minEdgeAreaDistance = findMinTriangleArea(A, minEdgeDistances, triangleAreas);

        // Choose edge randomly
        if(edgeSelection == 1)
        {
            std::cout << "------------triangleAreas(BEFORE)------------" << std::endl;
            for (int i = 0; i < triangleAreas.size(); i++)
            {
                std::cout << "Triangle: " << triangleAreas[i].first.first << " " << triangleAreas[i].first.second << " Area: " << triangleAreas[i].second << std::endl;
            }
            std::cout << "---------------------------------" << std::endl;
            auto rng = std::default_random_engine {};
            std::shuffle(std::begin(triangleAreas), std::end(triangleAreas), rng);
        }
        // Add edge so that min area is added => max area is removed from A
        if(edgeSelection == 2)
        {
            // Sort triangleAreas by area largest to smallest
            std::sort(triangleAreas.begin(), triangleAreas.end(), [](TriangleArea t1, TriangleArea t2)
                { return t1.second > t2.second; });
        }
        // Add edge so that max area is added => min area is removed from A  
        else if(edgeSelection == 3)
        {
        // Sort triangleAreas by area smallest to largest
            std::sort(triangleAreas.begin(), triangleAreas.end(), [](TriangleArea t1, TriangleArea t2)
                { return t1.second < t2.second; });
        }

        // // Print minEdgeDistance
        // std::cout << "---------minEdgeAreaDistance---------" << std::endl;
        // std::cout << "Segment: " << minEdgeAreaDistance.first << " Point: " << minEdgeAreaDistance.second << std::endl;
        // std::cout << "---------------------------------" << std::endl;
        //Print edgeAreas
        std::cout << "------------triangleAreas------------" << std::endl;
        for (int i = 0; i < triangleAreas.size(); i++)
        {
            std::cout << "Triangle: " << triangleAreas[i].first.first << " " << triangleAreas[i].first.second << " Area: " << triangleAreas[i].second << std::endl;
        }
        std::cout << "---------------------------------" << std::endl;

        // For every triangle, see if it's edge on the polygon is visible
        // from the third point of the triangle
        for (int i = 0; i < triangleAreas.size(); i++)
        {
            // Make the triangle
            Triangle_2 triangle(triangleAreas[i].first.first.source(), triangleAreas[i].first.first.target(), triangleAreas[i].first.second);
            //print Triangle
            // std::cout << "-- (Triangle): " << triangle << std::endl;
            bool isIntersecting = false;
            
            // For every edge of the polygon check
            // check if it intersects the triangle
            for (int j = 0; j < A.size(); j++)
            {
                Polygon_2::Segment_2 edge = A.edge(j);
                const auto mutual = CGAL::intersection(triangle, edge);
                if (mutual)
                {
                    // If the intersection is a Segment
                    if (const Polygon_2::Segment_2 *mutualSegment = boost::get<Polygon_2::Segment_2>(&*mutual))
                    {
                        // Check if the mutualSegment is the same as the edge and ignore it
                        if (mutualSegment->source() == edge.source() && mutualSegment->target() == edge.target())
                            continue;
                        else
                        {
                            // Found an intersection Segment, therefore the edge in question,
                            // polygon[i] to polygon[(i + 1) % polygon.size()],  is not visible
                            // std::cout << "Intersecting Segment" << std::endl;
                            isIntersecting = true;
                            break;
                        }
                    }
                    else
                    // If the intersection is a point then the polygon and the triangle
                    // share a vertex, therefore the edge in question can still be visible
                    {
                        const Point_2 *mutualPoint = boost::get<Point_2>(&*mutual);
                        // std::cout << "Intersecting Point" << std::endl;
                    }
                }
            }
            if (!isIntersecting)
            {
                Polygon_2::Segment_2 edge(triangleAreas[i].first.first.source(), triangleAreas[i].first.first.target());
                Point_2 point = triangleAreas[i].first.second;

                int index = findPolygonPoint(A, edge.target());
                A.insert(A.begin() + index, point);
                internalPoints.erase(std::find(internalPoints.begin(), internalPoints.end(), point));
                break;
            }
                // The edge is visible
            //     visibleEdges.push_back(EdgeArea(candidateEdge, std::abs(triangle.area()), false));
            // break;
        }

        // Print A
        std::cout << "----------------A----------------" << std::endl;
        // printPolygon(A);
        const bool simpl = A.is_simple();
        ++counter;
        if (simpl)
            std::cout << counter << " --  Simple polygon" << std::endl;
        else
        {
            std::cout << counter << " -- Not simple" << std::endl;
        }
        std::cout << "---------------------------------" << std::endl;
    }
    //Print the size of the polygon
    std::cout << "Size: " << A.size() << std::endl;
    
    return 0;
}































Polygon_2 findInternalPoints(Polygon_2 polygon, Polygon_2 A)
{
    Polygon_2 internalPoints;

    // Loop through all points of polygon
    for (auto polygonPoint = polygon.begin(); polygonPoint != polygon.end(); polygonPoint++)
    {
        bool isInside = false;
        // Loop through all points of A
        for (auto APoint = A.begin(); APoint != A.end(); APoint++)
        {
            // If point is both in polygon and A
            if (*polygonPoint == *APoint)
            {
                isInside = true;
                break;
            }
        }
        if (!isInside)
            internalPoints.push_back(*polygonPoint);
    }

    return internalPoints;
}

std::vector<EdgeDistance> findMinEdgeDistances(Polygon_2 A, Polygon_2 internalPoints)
{
    std::vector<std::pair<Polygon_2::Segment_2, Point_2>> minEdgeDistances;

    // Loop through every edge of A
    for (int i = 0; i < A.size(); i++)
    {
        Polygon_2::Segment_2 edge = A.edge(i);
        // std::cout << "edge " << i << ": " << edge << std::endl;

        double minDistance = DBL_MAX;
        std::pair<Polygon_2::Segment_2, Point_2> minEdgeDist;

        // Find the distance from the edge to every point in internalPoints
        for (int j = 0; j < internalPoints.size(); j++)
        {
            Point_2 point = internalPoints[j];

            // Find the distance from the edge to the point
            double distance = CGAL::squared_distance(edge, point);
            // std::cout << "point " << j << ": " << point << ", distance: " << distance << std::endl;

            if (distance < minDistance)
            {
                minDistance = distance;
                minEdgeDist = std::make_pair(edge, point);
            }
        }
        // std::cout << "---------------------------------" << std::endl;
        minEdgeDistances.push_back(minEdgeDist);
    }
    return minEdgeDistances;
}

EdgeDistance findMinTriangleArea(Polygon_2 A,std::vector<EdgeDistance> edgeDistances, std::vector<TriangleArea>& triangleAreas)
{
    double minArea = DBL_MAX;
    EdgeDistance minTriangleAreaDistance;

    // For every minEdgeDistance, find the area of the triangle formed by the edge and the point
    for(int i = 0; i < edgeDistances.size(); i++)
    {
        Polygon_2::Segment_2 edge = edgeDistances[i].first;
        Point_2 point = edgeDistances[i].second;
        //Create a triangle with the edge and the point
        Triangle_2 triangle(edge.source(), edge.target(), point);
        //Find the area of the triangle
        double area = std::abs(triangle.area());

        //Insert the edge and the area into the vector
        TriangleArea triangleArea = std::make_pair(edgeDistances[i], area);
        triangleAreas.push_back(triangleArea);

        if(area < minArea)
        {
            minArea = area;
            // minTriangleAreaDistance = std::make_pair(edge, point);
            minTriangleAreaDistance = edgeDistances[i];
        }
    }

    return minTriangleAreaDistance;
}