#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <iostream>

#define ERROR -1
#define RED 1
#define BLUE 2
#define BURGUNDY 3
#define NO_COLOR 0


typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_2 Point_2;
typedef CGAL::Polygon_2<Kernel> Polygon_2;

class ColoredVertex {
public:
    Point_2 point;
    int color;

    ColoredVertex(Point_2 point, int color)
     : point(point), color(color)
    {}
};

void printColoredVertices(std::vector<ColoredVertex> coloredVertices) {
    for (int i = 0; i < coloredVertices.size(); i++) {
        std::cout << coloredVertices[i].point << ", color: " << coloredVertices[i].color << std::endl;
    }
}

int findVertex(std::vector<ColoredVertex> coloredVertices, Point_2 point);
void vertexColoring(std::vector<ColoredVertex>& coloredVertices, int color, Point_2 point);


class ColoredEdge  {
public:
    Polygon_2::Segment_2 edge;
    int color;

    ColoredEdge(Polygon_2::Segment_2 edge, int color)
     : edge(edge), color(color)
    {}
};

void printPolygon(Polygon_2 points);


int main()
{
    Point_2 p1(1, 1), p2(10, 10), p3(5, 9), p4(2, 7), p5(3, 6), p6(3.5, 4.5), p7(5, 4), p8(6, 3), p9(9, 2);
    typedef CGAL::Polygon_2<Kernel> Polygon_2;

    Polygon_2 polygon;
    polygon.push_back(p1);
    polygon.push_back(p2);
    polygon.push_back(p3);
    polygon.push_back(p4);
    polygon.push_back(p5);
    polygon.push_back(p6);
    polygon.push_back(p7);
    polygon.push_back(p8);
    polygon.push_back(p9);

    // //Sort points by x-coordinate largest to smallest
    // std::sort(points.begin(), points.end(), [](Point_2 p1, Point_2 p2) { return p1.x() > p2.x(); });
    // printPointVector(points);

    // Sort points by x-coordinate smallest to largest
    std::sort(polygon.begin(), polygon.end(), [](Point_2 p1, Point_2 p2)
              { return p1.x() < p2.x(); });
    printPolygon(polygon);

    // Convex Hull empty set
    Polygon_2 convexHull;

    // Insert first three points
    convexHull.push_back(polygon[0]);
    convexHull.push_back(polygon[1]);
    convexHull.push_back(polygon[2]);

    for (int i = 3; i < polygon.size(); i++)
    {
        std::vector<ColoredEdge> coloredEdges;
        std::vector<ColoredVertex> coloredVertices;
        //for all the edges of the convex hull
        for (int j = 0; j < convexHull.size(); j++)
        {
            Polygon_2::Segment_2 edge = convexHull.edge(j);
            Point_2 source = edge.source();
            Point_2 target = edge.target();



            std::cout << "Point: " << polygon[i] << " -- Edge: " << source << " -- " << target << std::endl;
            // Balta sto .h
            // ---------------------------------------------------------------1----------------2------
            CGAL::Orientation testOrientation = CGAL::orientation(source, target, polygon[i]);
            // ---------------------------------------------------------------1----------------2----------------0----------
            CGAL::Orientation convHullOrientation = CGAL::orientation(source, target, convexHull[0]);

            if (edge.has_on(convexHull[0]))
            {
                for (auto point : convexHull)
                {
                    if(!edge.has_on(point))
                    {
                        convHullOrientation = CGAL::orientation(source, target, point);
                        break;
                    }
                }
            }

            int index = -1;
            // Blue Edge - same sign
            if (testOrientation == convHullOrientation && testOrientation != CGAL::COLLINEAR)
            {
                // Print that it's a blue edge
                std::cout << "Blue Edge" << std::endl;
                coloredEdges.push_back(ColoredEdge(edge, BLUE));
                vertexColoring(coloredVertices, BLUE, source);
                vertexColoring(coloredVertices, BLUE, target);
            }
            // Red Edge - different sign
            else if (testOrientation != convHullOrientation && testOrientation != CGAL::COLLINEAR)
            {
                // Print that it's a red edge
                std::cout << "Red Edge" << std::endl;
                coloredEdges.push_back(ColoredEdge(edge, RED));
                vertexColoring(coloredVertices, RED, source);
                vertexColoring(coloredVertices, RED, target);
            }
            else
            {
                // Print that it's a collinear edge
                std::cout << "Collinear Edge" << std::endl;
            }
        }

        printColoredVertices(coloredVertices);

        std::cout << std::endl;
        // break;
    }

    // convexHull.push_back(p1);

    return 0;
}








void printPolygon(Polygon_2 points)
{
    for (Point_2 p : points)
        std::cout << p << std::endl;
}

int findVertex(std::vector<ColoredVertex> coloredVertices, Point_2 point)
{
    int index = -1;
    for (int i = 0; i < coloredVertices.size(); i++)
    {
        if (coloredVertices[i].point == point)
        {
            index = i;
            return index;
        }
    }
    return index;
}

void vertexColoring(std::vector<ColoredVertex>& coloredVertices, int color, Point_2 point)
{
    int index = findVertex(coloredVertices, point);
    if (index == -1)
    {
        // If not found, add it
        coloredVertices.push_back(ColoredVertex(point, color));
    }
    else
    {
        // If it's not the same color, make it burgundy
        if (coloredVertices[index].color != color)
            coloredVertices[index].color = BURGUNDY;
    }
}