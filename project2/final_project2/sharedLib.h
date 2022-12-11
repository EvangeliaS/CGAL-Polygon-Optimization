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

// Prints the Polygon_2
void printPolygon(Polygon_2 points);
// Returns the index of a point in the polygon. If the point is not in the polygon, returns -1.
int findPolygonPoint(Polygon_2 polygon, Point_2 point);
// Splits a string into a vector of strings, using the delimiter.
std::vector<std::string> split(const std::string &s, char delimiter);
// Writes the output to the output file.
void writeToOutput(Polygon_2 A, double initial_area, double initial_ratio, std::string polygonization, int constructionTime, std::string outputFile, std::string algorithm);

#endif // __SHARED_LIB_H__