#ifndef __COLORING_H__
#define __COLORING_H__

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/intersections.h>
#include <CGAL/Polygon_2.h>



#define ERROR -1
#define RED 1
#define BLUE 2
#define BURGUNDY 3
#define GREY 4

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Triangle_2 Triangle_2;
typedef CGAL::Polygon_2<Kernel> Polygon_2;



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

int findPolygonPoint(Polygon_2 polygon, Point_2 point);
void findVisibleEdges(Polygon_2 polygon, Point_2 point, ColoredEdge redEdge, std::vector<EdgeArea> &visibleEdges);
int findMinAreaEdge(std::vector<EdgeArea> visibleEdges);
int findMaxAreaEdge(std::vector<EdgeArea> visibleEdges);
void printPolygon(Polygon_2 points);
std::vector<std::string> split(const std::string &s, char delimiter);
void incrementalAlgorithm(Polygon_2 polygon, int edgeSelection);


#endif // __COLORING_H__