#ifndef __CONVEX_HULL_ALGORITHM_H__
#define __CONVEX_HULL_ALGORITHM_H__

#include <chrono> 
#include <float.h>
#include <algorithm>
#include <random>

#include "sharedLib.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Triangle_2 Triangle_2;
typedef CGAL::Polygon_2<Kernel> Polygon_2;
typedef std::pair<Polygon_2::Segment_2, Point_2> EdgeDistance;
typedef std::pair<EdgeDistance, double> TriangleArea;

// Returns a Polygon_2 of the internal points of polygon (points in polygon but not in A).
Polygon_2 findInternalPoints(Polygon_2 polygon, Polygon_2 A);
// Returns a vector of a pair (edge, point) of the minimum distance from every edge of A to its closest point in internalPoints.
std::vector<EdgeDistance> findMinEdgeDistances(Polygon_2 A, Polygon_2 internalPoints);
// Returns the pair (edge, point), whose triangle's area is the smallest. It also fills triangleAreas vector with all the triangle areas.
EdgeDistance findMinTriangleArea(Polygon_2 A, std::vector<EdgeDistance> edgeDistances, std::vector<TriangleArea> &triangleAreas);
// Returns the polygon formed by following the CH algorithm of the assignment. constructionTime gets updated.
Polygon_2 convexHullAlgorithm(Polygon_2 polygon, int edgeSelection, int &constructionTime);

#endif // __CONVEX_HULL_ALGORITHM_H__