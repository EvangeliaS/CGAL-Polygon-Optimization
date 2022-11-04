#ifndef __INCREMENTAL_ALGORITHM_H__
#define __INCREMENTAL_ALGORITHM_H__

#include "sharedLib.h"

#define RED 1
#define BLUE 2
#define BURGUNDY 3
#define GREY 4


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

void findVisibleEdges(Polygon_2 polygon, Point_2 point, ColoredEdge redEdge, std::vector<EdgeArea> &visibleEdges);
int findMinAreaEdge(std::vector<EdgeArea> visibleEdges);
int findMaxAreaEdge(std::vector<EdgeArea> visibleEdges);
Polygon_2 incrementalAlgorithm(Polygon_2 polygon, int edgeSelection, int &constructionTime);

#endif // __INCREMENTAL_ALGORITHM_H__