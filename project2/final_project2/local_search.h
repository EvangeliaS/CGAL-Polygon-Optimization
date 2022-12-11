#include "sharedLib.h"
#include "incrementalAlgorithm.h"
#include "convexHullAlgorithm.h"

typedef Polygon_2::Segment_2 Segment_2;

class PathEdge
{
public:
    Polygon_2 path;
    Segment_2 edge;
    int deltaArea;
};

void findPaths(Polygon_2 A, std::vector<Polygon_2> &paths, int k, Segment_2 edge);
void printPaths(std::vector<Polygon_2> paths);
bool isEdgePartOfPath(Polygon_2 path, Segment_2 edge);
bool local_search(Polygon_2 &A, int k, double threshold, int deltaArea, bool isMaximization, int& constructionTime);
