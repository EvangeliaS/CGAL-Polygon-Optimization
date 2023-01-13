#ifndef __INCREMENTAL_ALGORITHM_CPP__
#define __INCREMENTAL_ALGORITHM_CPP__

#include "incrementalAlgorithm.h"


void findVisibleEdges(Polygon_2 polygon, Point_2 point, ColoredEdge redEdge, std::vector<EdgeArea> &visibleEdges)
{
    // Find indexes of redEdge.source() and redEdge.target() in polygon
    int indexSource = findPolygonPoint(polygon, redEdge.edge.source());
    int indexTarget = findPolygonPoint(polygon, redEdge.edge.target());

    // If the edge of the CH is on the polygon -- Consecutive edges of the polygon
    if (indexSource == indexTarget - 1 || (indexSource == polygon.size() - 1 && indexTarget == 0))
    {
        // The edge is visible
        Triangle_2 triangle(polygon[indexSource], polygon[indexTarget], point);
        visibleEdges.push_back(EdgeArea(redEdge.edge, std::abs(triangle.area()), false));
    }
    // The consecutive edges of the polygon, but in reverse order
    else if (indexTarget == indexSource - 1 || (indexTarget == polygon.size() - 1 && indexSource == 0))
    {
        // The edge is visible but must be flipped
        Triangle_2 triangle(polygon[indexSource], polygon[indexTarget], point);
        visibleEdges.push_back(EdgeArea(redEdge.edge, std::abs(triangle.area()), true));
    }
    else
    {
        // Traverse the polygon from redEdge.source() to redEdge.target()
        for (int i = indexSource; i != indexTarget; i = (i + 1) % polygon.size())
        {
            Triangle_2 triangle(polygon[i], polygon[(i + 1) % polygon.size()], point);

            // Candidate edge to get added to visibleEdges
            Polygon_2::Segment_2 candidateEdge(polygon[i], polygon[(i + 1) % polygon.size()]);

            // Check if the polygon intersects the triangle
            bool isIntersecting = false;

            // Check every edge of the polygon
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
                            isIntersecting = true;
                            break;
                        }
                    }
                    else
                    // If the intersection is a point then the polygon and the triangle
                    // share a vertex, therefore the edge in question can still be visible
                        const Point_2 *mutualPoint = boost::get<Point_2>(&*mutual);
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

            // Check if the polygon intersects the triangle
            bool isIntersecting = false;

            // Check every edge of the polygon
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
                            isIntersecting = true;
                            break;
                        }
                    }
                    else
                    // If the intersection is a point then the polygon and the triangle
                    // share a vertex, therefore the edge in question can still be visible
                        const Point_2 *mutualPoint = boost::get<Point_2>(&*mutual);
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

int findSecondMinAreaEdge(std::vector<EdgeArea> visibleEdges)
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
    index = -1;
    double secondMinArea = visibleEdges[0].area;
    for (int i = 1; i < visibleEdges.size(); i++)
    {
        if (visibleEdges[i].area < secondMinArea && visibleEdges[i].area != minArea)
        {
            secondMinArea = visibleEdges[i].area;
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

int findSecondMaxAreaEdge(std::vector<EdgeArea> visibleEdges)
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
    index = -1;
    double secondMaxArea = visibleEdges[0].area;
    for (int i = 1; i < visibleEdges.size(); i++)
    {
        if (visibleEdges[i].area > secondMaxArea && visibleEdges[i].area != maxArea)
        {
            secondMaxArea = visibleEdges[i].area;
            index = i;
        }
    }
    return index;
}

Polygon_2 incrementalAlgorithm(Polygon_2 polygon, int edgeSelection, int &constructionTime, bool isSpatialSubdivision)
{
    // Start the clock
    clock_t start = clock();
    clock_t end;

    Polygon_2 A;

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

            // If a point is chosen twice for the orientation calculations, choose the next point
            if (edge.has_on(convexHull[0]))
            {
                for (auto point : convexHull)
                {
                    // If the point is not on the edge, calculate the orientation
                    // of the internal point in the convex hull
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
                coloredEdges.push_back(ColoredEdge(edge, BLUE));
            // Red Edge - different sign
            else if (testOrientation != convHullOrientation && testOrientation != CGAL::COLLINEAR)
                coloredEdges.push_back(ColoredEdge(edge, RED));
            // Grey Edge - collinear
            else
                coloredEdges.push_back(ColoredEdge(edge, GREY));
        }

        // Vector of visible edges
        std::vector<EdgeArea> visibleEdges;

        // For every Red edge of the CH find the visible edges of the polygon
        for (int j = 0; j < coloredEdges.size(); j++)
        {
            if (coloredEdges[j].color == RED)
            {
                findVisibleEdges(A, polygon[i], coloredEdges[j], visibleEdges);
            }
        }
        // If there are no visible edges, check the Grey edges
        if (visibleEdges.empty())
        {
            // For every grey edge
            for (int j = 0; j < coloredEdges.size(); j++)
            {
                if (coloredEdges[j].color == GREY)
                {
                    findVisibleEdges(A, polygon[i], coloredEdges[j], visibleEdges);
                }
            }
        }

        int index = -1;

        // Choose a visible edge randomly
        if (edgeSelection == 1)
            index = rand() % visibleEdges.size();
        // Add a visible edge so that min area is added to A
        else if (edgeSelection == 2)
            index = findMinAreaEdge(visibleEdges);
        // Add a visible edge so that max area is added to A
        else if (edgeSelection == 3)
            index = findMaxAreaEdge(visibleEdges);

        Polygon_2::Segment_2 edge = visibleEdges[index].edge;
        
        if(isSpatialSubdivision)
        {
            // If edge is comprised of the first and second points of the polygon or the last and second to last points of the polygon
            // during spatial subdivision, choose the second best edge
            if ((edge.has_on(polygon[0]) && edge.has_on(polygon[1]) && edgeSelection == 2)
            || (edge.has_on(polygon[polygon.size() - 1]) && edge.has_on(polygon[polygon.size() - 2]) && edgeSelection == 2))
            {
                std::cout << "Spatial Subdivision - Second Best Edge" << std::endl;
                index = findSecondMinAreaEdge(visibleEdges);
                if (index == -1)
                {
                    std::cout << "Error: During polygonization with spatial subdivision, the second best edge could not be found. (So as to meet the LH conditions." << std::endl;
                    index = findMinAreaEdge(visibleEdges);
                }
                edge = visibleEdges[index].edge;
            }

            if((edge.has_on(polygon[0]) && edge.has_on(polygon[1]) && edgeSelection == 3)
            || (edge.has_on(polygon[polygon.size() - 1]) && edge.has_on(polygon[polygon.size() - 2]) && edgeSelection == 3))
            {
                std::cout << "Spatial Subdivision - Second Best Edge" << std::endl;
                index = findSecondMaxAreaEdge(visibleEdges);
                if (index == -1)
                {
                    std::cout << "Error: During polygonization with spatial subdivision, the second best edge could not be found. (So as to meet the LH conditions." << std::endl;
                    index = findMaxAreaEdge(visibleEdges);
                }
                edge = visibleEdges[index].edge;
            }
        }

        // Insert the new point to A inbetween the chosen edges' source and target
        if (!visibleEdges[index].invert)
            index = findPolygonPoint(A, edge.target());
        // If the edge is inverted, insert the point before the source
        else
            index = findPolygonPoint(A, edge.source());
        
        A.insert(A.begin() + index, polygon[i]);
    }

    // Stop the clock
    end = clock();

    // Construction time of incremental algorithm in milliseconds
    constructionTime = (end - start) / (double)(CLOCKS_PER_SEC / 1000);

    return A;
}

#endif // __INCREMENTAL_ALGORITHM_H__