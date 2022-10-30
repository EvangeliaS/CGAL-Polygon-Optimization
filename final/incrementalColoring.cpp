#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <iostream>
#include <fstream>
#include <string>


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

std::vector<std::string> split(const std::string& s, char delimiter)
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


int main(int argc, char** argv)
{
    if(argc < 3) {
        std::cout << "Usage: " << argv[0] << " -i <point set input file>" << std::endl;
        return ERROR;
    }
    std::string inputFile = argv[2];
    // inputFile ="../instances/data/images/euro-night-0000010.instance";

    std::ifstream input(inputFile);
    if (!input) {
        std::cout << "Could not open file " << inputFile << std::endl;
        return ERROR;
    }

    //Ignore first two lines
    std::string line;
    std::getline(input, line);
    std::getline(input, line);


    Polygon_2 polygon;

    //Loop to read inputFile line by line
    while(std::getline(input, line))
    {
        std::vector<std::string> tokens = split(line, '\t');
        polygon.push_back(Point_2(std::stod(tokens[1]), std::stod(tokens[2])));
    }
    std::cout << "----------------------------" << std::endl;
    printPolygon(polygon);
    std::cout << "----------------------------" << std::endl;



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

        // printColoredVertices(coloredVertices);
        // Remove all red vertices from the convexHull
        for (int j = 0; j < coloredVertices.size(); j++)
        {
            if (coloredVertices[j].color == RED)
            {
                convexHull.erase(std::find(convexHull.begin(), convexHull.end(), coloredVertices[j].point));
            }
        } 
        // Push back the new point to the convexHull
        convexHull.push_back(polygon[i]);


        const bool simpl = convexHull.is_simple();
        if (simpl)
            std::cout << "Simple polygon" << std::endl;
        else
        {
            std::cout << "Not simple" << std::endl;
            //Swap last and second to last points
            std::swap(convexHull[convexHull.size() - 1], convexHull[convexHull.size() - 2]);
            const bool simpl = convexHull.is_simple();
            if (simpl)
                std::cout << "Made it a Simple polygon" << std::endl;
            else
                std::cout << "Still not simple" << std::endl;
        }


        printPolygon(convexHull);
        std::cout << std::endl;
        // break;
    }

    // convexHull.push_back(p1);
    //print convexHull

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