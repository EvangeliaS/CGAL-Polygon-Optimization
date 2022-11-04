#ifndef __SHARED_LIB_H__
#define __SHARED_LIB_H__

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/intersections.h>
#include <CGAL/Polygon_2.h>
#include <fstream>

#define ERROR -1

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Triangle_2 Triangle_2;
typedef CGAL::Polygon_2<Kernel> Polygon_2;


void printPolygon(Polygon_2 points);
int findPolygonPoint(Polygon_2 polygon, Point_2 point);
std::vector<std::string> split(const std::string &s, char delimiter);
void writeToOutput(Polygon_2 A, double area, double ratio, int constructionTime, std::string outputFile, std::string algorithm, int edgeSelection, std::string initialization);


#endif // __SHARED_LIB_H__