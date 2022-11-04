#ifndef __CONVEX_HULL_ALGORITHM_H__
#define __CONVEX_HULL_ALGORITHM_H__

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


Polygon_2 findInternalPoints(Polygon_2 polygon, Polygon_2 A);
std::vector<EdgeDistance> findMinEdgeDistances(Polygon_2 A, Polygon_2 internalPoints);
EdgeDistance findMinTriangleArea(Polygon_2 A, std::vector<EdgeDistance> edgeDistances, std::vector<TriangleArea> &triangleAreas);
Polygon_2 convexHullAlgorithm(Polygon_2 polygon, int edgeSelection, int &constructionTime);

#endif // __CONVEX_HULL_ALGORITHM_H__