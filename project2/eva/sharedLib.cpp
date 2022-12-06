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

void writeToOutput(Polygon_2 A, double area, double ratio, int constructionTime, std::string outputFile, std::string algorithm, int edgeSelection, std::string initialization)
{
    std::ofstream output(outputFile);
    if (!output)
    {
        std::cout << "Could not open file " << outputFile << std::endl;
        perror("Error on opening output file");
        exit(ERROR);
    }
    // Write to output file
    output << "Polygonization" << std::endl;

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

    // Write the algorithm used
    output << "Algorithm: " << algorithm << ", edge_selection " << edgeSelection;
    if (algorithm == "incremental")
        output << ", initialization " << initialization << std::endl;
    else
        output << std::endl;
    // Write the area of the polygon
    output << "area: " << area << std::endl;

    // Write the ratio of the polygon
    output << "ratio: " << ratio << std::endl;

    // Write the construction time of the polygon
    output << "construction time: " << constructionTime << std::endl;
}

#endif // __SHARED_LIB_CPP__