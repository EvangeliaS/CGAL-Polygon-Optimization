#ifndef __COLORING_CPP__
#define __COLORING_CPP__

#include "coloring.h"

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

void findVisibleEdges(Polygon_2 polygon, Point_2 point, ColoredEdge redEdge, std::vector<EdgeArea> &visibleEdges)
{
    // Find index of redEdge.source() in polygon
    int indexSource = findPolygonPoint(polygon, redEdge.edge.source());
    int indexTarget = findPolygonPoint(polygon, redEdge.edge.target());
    // If edge is on polygon
    if (indexSource == indexTarget - 1 || (indexSource == polygon.size() - 1 && indexTarget == 0))
    {
        // The edge is visible
        Triangle_2 triangle(polygon[indexSource], polygon[indexTarget], point);
        visibleEdges.push_back(EdgeArea(redEdge.edge, std::abs(triangle.area()), false));
    }
    else if (indexTarget == indexSource - 1 || (indexTarget == polygon.size() - 1 && indexSource == 0))
    {
        // The edge is visible, must be flipped
        Triangle_2 triangle(polygon[indexSource], polygon[indexTarget], point);
        visibleEdges.push_back(EdgeArea(redEdge.edge, std::abs(triangle.area()), true));
    }
    else
    {
        // Traverse the polygon from redEdge.source() to redEdge.target()
        for (int i = indexSource; i != indexTarget; i = (i + 1) % polygon.size())
        {
            Triangle_2 triangle(polygon[i], polygon[(i + 1) % polygon.size()], point);

            Polygon_2::Segment_2 candidateEdge(polygon[i], polygon[(i + 1) % polygon.size()]);

            // CHECK IF THE POLYGON INTERSECTS THE TRIANGLE
            bool isIntersecting = false;

            for (int j = 0; j < polygon.size(); j++)
            {
                Polygon_2::Segment_2 edge = polygon.edge(j);

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
                // The edge is visible
                visibleEdges.push_back(EdgeArea(candidateEdge, std::abs(triangle.area()), false));
        }
        // Traverse the polygon from redEdge.target() to redEdge.source()
        for (int i = indexTarget; i != indexSource; i = (i + 1) % polygon.size())
        {
            Triangle_2 triangle(polygon[i], polygon[(i + 1) % polygon.size()], point);

            Polygon_2::Segment_2 candidateEdge(polygon[i], polygon[(i + 1) % polygon.size()]);

            // CHECK IF THE POLYGON INTERSECTS THE TRIANGLE
            bool isIntersecting = false;

            for (int j = 0; j < polygon.size(); j++)
            {
                Polygon_2::Segment_2 edge = polygon.edge(j);

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
                // The edge is visible
                visibleEdges.push_back(EdgeArea(candidateEdge, std::abs(triangle.area()), false));
        }
    }
}

int findMinAreaEdge(std::vector<EdgeArea> visibleEdges)
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

int findMaxAreaEdge(std::vector<EdgeArea> visibleEdges)
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


void incrementalAlgorithm(Polygon_2 polygon,int edgeSelection)
{   
    Polygon_2 A;
    
    printPolygon(polygon);

    // Insert the first three points to polygon A
    A.push_back(polygon[0]);
    A.push_back(polygon[1]);
    A.push_back(polygon[2]);

    for (int i = 3; i < polygon.size(); i++)
    {
        // Convex Hull empty set
        Polygon_2 convexHull;

        std::vector<ColoredEdge> coloredEdges;

        // Calculate convex hull of polygon A up to the i-th point
        CGAL::convex_hull_2(A.begin(), A.end(), std::back_inserter(convexHull));

        // for all the edges of the convex hull
        for (int j = 0; j < convexHull.size(); j++)
        {
            Polygon_2::Segment_2 edge = convexHull.edge(j);
            Point_2 source = edge.source();
            Point_2 target = edge.target();


            CGAL::Orientation testOrientation = CGAL::orientation(source, target, polygon[i]);
            CGAL::Orientation convHullOrientation = CGAL::orientation(source, target, convexHull[0]);

            if (edge.has_on(convexHull[0]))
            {
                for (auto point : convexHull)
                {
                    if (!edge.has_on(point))
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
            }
            // Red Edge - different sign
            else if (testOrientation != convHullOrientation && testOrientation != CGAL::COLLINEAR)
            {
                // Print that it's a red edge
                // std::cout << "Red Edge" << std::endl;
                coloredEdges.push_back(ColoredEdge(edge, RED));
            }
            else
            {
                // Print that it's a collinear edge
                std::cout << "Collinear Edge -- " << edge << std::endl;
                coloredEdges.push_back(ColoredEdge(edge, GREY));
            }
        }
        // Print Colored Edges
        std::cout << "Colored Edges:"
                  << "Point: " << polygon[i] << std::endl;
        for (int j = 0; j < coloredEdges.size(); j++)
        {
            std::cout << coloredEdges[j].edge << ", color: " << coloredEdges[j].color << std::endl;
        }

        std::vector<EdgeArea> visibleEdges;

        // For every red edge
        for (int j = 0; j < coloredEdges.size(); j++)
        {
            if (coloredEdges[j].color == RED)
            {
                findVisibleEdges(A, polygon[i], coloredEdges[j], visibleEdges);
            }
        }
        if (visibleEdges.empty())
        {
            std::cout << "CHECKING GREY EDGES" << std::endl;
            // For every grey edge
            for (int j = 0; j < coloredEdges.size(); j++)
            {
                if (coloredEdges[j].color == GREY)
                {
                    findVisibleEdges(A, polygon[i], coloredEdges[j], visibleEdges);
                }
            }
        }
        std::cout << "Visible Edges:" << std::endl;
        for (int k = 0; k < visibleEdges.size(); k++)
        {
            std::cout << visibleEdges[k].edge << "  -- Area: " << visibleEdges[k].area << " -- Invert: " << visibleEdges[k].invert << std::endl;
        }

        // Chose the visible edge
        int index = -1;

        // Chose index randomly
        if (edgeSelection == 1)
            index = rand() % visibleEdges.size();
        else if (edgeSelection == 2)
            index = findMinAreaEdge(visibleEdges);
        else if (edgeSelection == 3)
            index = findMaxAreaEdge(visibleEdges);

        Polygon_2::Segment_2 edge = visibleEdges[index].edge;
        std::cout << "Chosen Edge: " << edge << std::endl;

        // Insert the new point to A inbetween the chosen edges' source and target

        if (!visibleEdges[index].invert)
            index = findPolygonPoint(A, edge.target());
        else
            index = findPolygonPoint(A, edge.source());
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
    }
}

#endif // __COLORING_CPP__