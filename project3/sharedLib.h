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

class Score
{
public:
    double scoreMin;
    double scoreMax;
    double boundMin;
    double boundMax;

    void print()
    {
        std::cout << "Score Min: " << scoreMin << std::endl;
        std::cout << "Score Max: " << scoreMax << std::endl;
        std::cout << "Bound Min: " << boundMin << std::endl;
        std::cout << "Bound Max: " << boundMax << std::endl;
    }
};


class File{
public:
    std::string name;
    int size;

    void print(){
        std::cout << name << " ,  " << size << std::endl;
    }
};



// Prints the Polygon_2
void printPolygon(Polygon_2 points);
// Returns the index of a point in the polygon. If the point is not in the polygon, returns -1.
int findPolygonPoint(Polygon_2 polygon, Point_2 point);
// Splits a string into a vector of strings, using the delimiter.
std::vector<std::string> split(const std::string &s, char delimiter);
// Writes the output to the output file.
void writeToOutput( std::string outputFile, std::vector<Score>& scores, int size, bool overwrite);
// Finds the cut-off point.
int cutOffmsecs(int numPoints);
// Calculates the scores for all the files of the same size.
std::vector<Score> calculateScores(std::vector<std::vector<double>> scoresOfSize);

#endif // __SHARED_LIB_H__