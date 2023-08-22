#ifndef __INCREMENTAL_ALGORITHM_H__
#define __INCREMENTAL_ALGORITHM_H__

#include "sharedLib.h"

#define RED 1
#define BLUE 2
#define BURGUNDY 3
#define GREY 4


// Stores an edge and it's color
class ColoredEdge
{
public:
    Polygon_2::Segment_2 edge;
    int color;

    ColoredEdge(Polygon_2::Segment_2 edge, int color)
        : edge(edge), color(color)
    {
    }
};

// Stores an edge and the area of the triangle formed by the edge and a point
class EdgeArea
{
public:
    Polygon_2::Segment_2 edge;
    double area;
    bool invert;

    EdgeArea(Polygon_2::Segment_2 edge, double area, bool invert)
        : edge(edge), area(area), invert(invert)
    {
    }
};

// Finds the visible edges from a point to the polygon and stores them in visibleEdges
void findVisibleEdges(Polygon_2 polygon, Point_2 point, ColoredEdge redEdge, std::vector<EdgeArea> &visibleEdges);
// Returns the index of the edge that forms the smallest triangle by area
int findMinAreaEdge(std::vector<EdgeArea> visibleEdges);
// Returns the index of the edge that forms the largest triangle by area
int findMaxAreaEdge(std::vector<EdgeArea> visibleEdges);

int findSecondMaxAreaEdge(std::vector<EdgeArea> visibleEdges);

int findSecondMinAreaEdge(std::vector<EdgeArea> visibleEdges);

// Returns the polygon formed by following the incremental algorithm of the assignment. constructionTime gets updated.
Polygon_2 incrementalAlgorithm(Polygon_2 polygon, int edgeSelection, int &constructionTime, bool isSpatialSubdivision);

#endif // __INCREMENTAL_ALGORITHM_H__