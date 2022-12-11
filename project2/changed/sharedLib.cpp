#ifndef __SHARED_LIB_CPP__
#define __SHARED_LIB_CPP__

#include "sharedLib.h"


void printPolygon(Polygon_2 points)
{
    for (Point_2 p : points)
        std::cout << p << std::endl;
}

int findPolygonPoint(Polygon_2 polygon, Point_2 point)
{
    int index = -1;
    for (int i = 0; i < polygon.size(); i++)
    {
        if (polygon[i] == point)
        {
            index = i;
            return index;
        }
    }
    return index;
}

std::vector<std::string> split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

void writeToOutput(Polygon_2 A, double initial_area, double initial_ratio, std::string polygonization, int constructionTime, std::string outputFile, std::string algorithm)
{
    std::ofstream output(outputFile);
    if (!output)
    {
        std::cout << "Could not open file " << outputFile << std::endl;
        perror("Error on opening output file");
        exit(ERROR);
    }
    // Write to output file
    output << "Optimal Area Polygonization" << std::endl;

    // Write the vertices of the polygon
    for (auto it = A.vertices_begin(); it != A.vertices_end(); it++)
    {
        output << *it << std::endl;
    }

    // Write the edges of the polygon
    for (auto it = A.edges_begin(); it != A.edges_end(); it++)
    {
        output << *it << std::endl;
    }

    //Compute the area of the convexHull of the point set given
    Polygon_2 convexHull;
    CGAL::convex_hull_2(A.begin(), A.end(), std::back_inserter(convexHull));

    double final_area = std::abs(A.area());
    double final_ratio = std::abs(A.area()) / std::abs(convexHull.area());

    // Write the algorithm used
    output << "Algorithm: " << algorithm << "_" << polygonization << std::endl;
    if (algorithm == "local_search" || algorithm == "simulated_annealing" )
        output << "area_initial " << initial_area << std::endl;
    else
        output << std::endl;
    // Write the area of the polygon
    output << "area: " << final_area << std::endl;

    if (algorithm == "local_search" || algorithm == "simulated_annealing" )
        // Write the initial ratio of the polygon
        output << "ratio_initial " << initial_ratio << std::endl;
     else
        output << std::endl;
    // Write the ratio of the polygon
    output << "ratio: " << final_ratio << std::endl;

    // Write the construction time of the polygon
    output << "construction time: " << constructionTime << std::endl;
}

#endif // __SHARED_LIB_CPP__