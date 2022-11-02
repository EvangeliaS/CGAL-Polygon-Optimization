#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/intersections.h>
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
typedef Kernel::Triangle_2 Triangle_2;
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

class EdgeAreaTuple {
public:
    Polygon_2::Segment_2 edge;
    double area;

    EdgeAreaTuple(Polygon_2::Segment_2 edge, double area)
     : edge(edge), area(area)
    {}
};


int findVertex(std::vector<ColoredVertex> coloredVertices, Point_2 point);
int findPolygonPoint(Polygon_2 polygon, Point_2 point);
void vertexColoring(std::vector<ColoredVertex>& coloredVertices, int color, Point_2 point);
void findVisibleEdges(Polygon_2 polygon, Point_2 point, ColoredEdge redEdge, std::vector<EdgeAreaTuple>& visibleEdges);
int findMinAreaEdge(std::vector<EdgeAreaTuple> visibleEdges);
int findMaxAreaEdge(std::vector<EdgeAreaTuple> visibleEdges);





int main(int argc, char** argv)
{
    // if(argc < 3) {
    //     std::cout << "Usage: " << argv[0] << " -i <point set input file>" << std::endl;
    //     return ERROR;
    // }
    std::string inputFile = argv[2];
    inputFile ="../instances/data/images/us-night-0000100.instance";

    std::ifstream input(inputFile);
    if (!input) {
        std::cout << "Could not open file " << inputFile << std::endl;
        return ERROR;
    }

    //Ignore first two lines
    std::string line;
    std::getline(input, line);
    std::getline(input, line);


    Polygon_2 polygon, A;

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


    // // Insert first three points
    // convexHull.push_back(polygon[0]);
    // convexHull.push_back(polygon[1]);
    // convexHull.push_back(polygon[2]);

    // Insert the first three points to polygon A
    A.push_back(polygon[0]);
    A.push_back(polygon[1]);
    A.push_back(polygon[2]);

    for (int i = 3; i < polygon.size(); i++)
    {
        // Convex Hull empty set
        Polygon_2 convexHull;

        std::vector<ColoredEdge> coloredEdges;
        std::vector<ColoredVertex> coloredVertices;

        // Calculate convex hull of polygon A up to the i-th point
        CGAL::convex_hull_2(A.begin(), A.end(), std::back_inserter(convexHull));       


        //for all the edges of the convex hull
        for (int j = 0; j < convexHull.size(); j++)
        {
            Polygon_2::Segment_2 edge = convexHull.edge(j);
            Point_2 source = edge.source();
            Point_2 target = edge.target();


            // std::cout << "Point: " << polygon[i] << " -- Edge: " << source << " -- " << target << std::endl;
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
                // std::cout << "Blue Edge" << std::endl;
                coloredEdges.push_back(ColoredEdge(edge, BLUE));
                vertexColoring(coloredVertices, BLUE, source);
                vertexColoring(coloredVertices, BLUE, target);
            }
            // Red Edge - different sign
            else if (testOrientation != convHullOrientation && testOrientation != CGAL::COLLINEAR)
            {
                // Print that it's a red edge
                // std::cout << "Red Edge" << std::endl;
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
        // Print Colored Edges
        std::cout << "Colored Edges:" << "Point: " << polygon[i] << std::endl;
        for (int j = 0; j < coloredEdges.size(); j++)
        {
            std::cout << coloredEdges[j].edge << ", color: " << coloredEdges[j].color << std::endl;
        }

        std::vector<EdgeAreaTuple> visibleEdges;

        // For every red edge
        for (int j = 0; j < coloredEdges.size(); j++)
        {
            if(coloredEdges[j].color == RED)
            {
                // // If convexHull is the same as polygon A
                // if (convexHull == A)
                // {
                //     // Insert the new point to A inbetween edge.source() and edge.target()

                //     // Find the position of target in polygon A
                //     int index = findPolygonPoint(A, coloredEdges[j].edge.target());
                //     // Insert it before target
                //     A.insert(A.begin() + index, polygon[i]);
                //     break;
                //     printPolygon(A);
                // }

                // Find visible edges
                findVisibleEdges(A, polygon[i], coloredEdges[j], visibleEdges);
                // print visible edges


                // Choose the edge randomly or depending on the area of the triangles created
                // Polygon_2::Segment_2 edge = visibleEdges[0];
                // // Insert the new point to A inbetween the chosen edges' source and target
                // int index = findPolygonPoint(A, edge.target()); 
                // A.insert(A.begin() + index, polygon[i]);
                // std::cout << "------------ A ------------" << std::endl;
                // printPolygon(A);
                // std::cout << "---------------------------" << std::endl;
                // break;

                    // else
                    // {
                    //     // If this edge is on the convexHull and not in polygon A
                    //         // Find index of coloredEdges[j].edge.source() in polygon A  
                    //         int indexSource = findPolygonPoint(A, coloredEdges[j].edge.source());
                    //         int indexTarget = findPolygonPoint(A, coloredEdges[j].edge.target());
                    //         if(!(indexSource == indexTarget -1 || (indexSource == polygon.size() - 1 && indexTarget == 0)))
                    //             continue;
                    //         else
                    //         {
                    //             // Insert the new point to A inbetween edge.source() and edge.target()
                    //             A.insert(A.begin() + indexTarget, polygon[i]);
                    //             break;
                    //         }
                    // }
            }

        }
        std::cout << "Visible Edges:" << std::endl;
        for (int k = 0; k < visibleEdges.size(); k++)
        {
            std::cout << visibleEdges[k].edge << "  -- Area: " << visibleEdges[k].area << std::endl;
        }

        // Chose the visible edge
        int index = -1;
        index = findMinAreaEdge(visibleEdges);
        Polygon_2::Segment_2 edge = visibleEdges[index].edge;
        std::cout << "Chosen Edge: " << edge << std::endl;
        
        
        // Insert the new point to A inbetween the chosen edges' source and target
        index = findPolygonPoint(A, edge.target()); 
        A.insert(A.begin() + index, polygon[i]);
        std::cout << "------------ A ------------" << std::endl;
        printPolygon(A);
        const bool simpl = A.is_simple();
        if (simpl)
            std::cout << "Simple polygon" << std::endl;
        else
        {
            std::cout << "Not simple" << std::endl;
        }
        std::cout << "---------------------------" << std::endl;
        continue;





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


        // const bool simpl = convexHull.is_simple();
        // if (simpl)
        //     std::cout << "Simple polygon" << std::endl;
        // else
        // {
        //     std::cout << "Not simple" << std::endl;
        //     //Swap last and second to last points
        //     std::swap(convexHull[convexHull.size() - 1], convexHull[convexHull.size() - 2]);
        //     const bool simpl = convexHull.is_simple();
        //     if (simpl)
        //         std::cout << "Made it a Simple polygon" << std::endl;
        //     else
        //         std::cout << "Still not simple" << std::endl;
        // }


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

void findVisibleEdges(Polygon_2 polygon, Point_2 point, ColoredEdge redEdge, std::vector<EdgeAreaTuple>& visibleEdges)
{
    // Find index of redEdge.source() in polygon  
    int indexSource = findPolygonPoint(polygon, redEdge.edge.source());
    int indexTarget = findPolygonPoint(polygon, redEdge.edge.target());
    // If edge is on polygon
    if(indexSource == indexTarget -1 || (indexSource == polygon.size() - 1 && indexTarget == 0)
     ||indexTarget == indexSource -1 || (indexTarget == polygon.size() - 1 && indexSource == 0))
    {
        // The edge is visible
        Triangle_2 triangle(polygon[indexSource], polygon[indexTarget], point);        
        visibleEdges.push_back(EdgeAreaTuple(redEdge.edge, std::abs(triangle.area())));
    }
    else
    {
        // Traverse the polygon from redEdge.source() to redEdge.target()
        for(int i = indexSource; i != indexTarget; i = (i + 1) % polygon.size())
        {
            Triangle_2 triangle(polygon[i], polygon[(i + 1) % polygon.size()], point);

            Polygon_2::Segment_2 candidateEdge(polygon[i], polygon[(i + 1) % polygon.size()]);

            // CHECK IF THE POLYGON INTERSECTS THE TRIANGLE
            bool isIntersecting = false;
            

            for( int j = 0; j < polygon.size(); j++)
            {
                Polygon_2::Segment_2 edge = polygon.edge(j);

                const auto mutual = CGAL::intersection(triangle, edge);
                if (mutual)
                {
                    // If the intersection is a Segment
                    if (const Polygon_2::Segment_2* mutualSegment = boost::get<Polygon_2::Segment_2>(&*mutual))
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
                        const Point_2* mutualPoint = boost::get<Point_2 >(&*mutual);
                        // std::cout << "Intersecting Point" << std::endl;
                    }
                }
            }
            if (!isIntersecting)
                // The edge is visible
                visibleEdges.push_back(EdgeAreaTuple(candidateEdge, std::abs(triangle.area())));
        }

    }
}

int findMinAreaEdge(std::vector<EdgeAreaTuple> visibleEdges)
{
    int index = 0;
    double minArea = visibleEdges[0].area;
    for (int i = 1; i < visibleEdges.size(); i++)
    {
        if (visibleEdges[i].area < minArea)
        {
            minArea = visibleEdges[i].area;
            index = i;
        }
    }
    return index;
}

int findMaxAreaEdge(std::vector<EdgeAreaTuple> visibleEdges)
{
    int index = 0;
    double maxArea = visibleEdges[0].area;
    for (int i = 1; i < visibleEdges.size(); i++)
    {
        if (visibleEdges[i].area > maxArea)
        {
            maxArea = visibleEdges[i].area;
            index = i;
        }
    }
    return index;
}