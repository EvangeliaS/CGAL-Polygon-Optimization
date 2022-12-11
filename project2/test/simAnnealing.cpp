#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Search_traits_2.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/intersections.h>
#include <CGAL/Polygon_2.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

#include "sharedLib.h"
#include "incrementalAlgorithm.h"
#include "convexHullAlgorithm.h"


#include <CGAL/Kd_tree.h>

bool validityCheck(Polygon_2 &A, bool isLocal, Point_2& p, Point_2& q, Point_2& r, Point_2& s, CGAL::Kd_tree<CGAL::Search_traits_2<Kernel>>& kdTree);
void localStep(Polygon_2& A, CGAL::Kd_tree<CGAL::Search_traits_2<Kernel>>& kdTree);

int main(int argc, char **argv)
{

    if (argc < 11)
    {
        std::cout << "Not enough arguments" << std::endl;
        return ERROR;
    }
    std::string inputFile = argv[2];
    std::string outputFile = argv[4];
    std::string algorithm = argv[6];
    int edgeSelection = atoi(argv[8]);
    if (edgeSelection != 1 && edgeSelection != 2 && edgeSelection != 3)
    {
        std::cout << "Invalid edge selection algorithm. Please choose 1, 2 or 3." << std::endl;
        return ERROR;
    }
    std::string initialization = argv[10];
    if (initialization != "1a" && initialization != "1b" && initialization != "2a" && initialization != "2b")
    {
        std::cout << "Invalid initialization algorithm. Please choose 1a, 1b, 2a or 2b." << std::endl;
        return ERROR;
    }
    int onionInitialization = atoi(argv[12]);

    srand(time(0));

    std::ifstream input(inputFile);
    if (!input)
    {
        std::cout << "Could not open file " << inputFile << std::endl;
        return ERROR;
    }

    // Ignore first two lines
    std::string line;
    std::getline(input, line);
    std::getline(input, line);

    Polygon_2 polygon;

    // Loop to read inputFile line by line
    while (std::getline(input, line))
    {
        std::vector<std::string> tokens = split(line, '\t');
        polygon.push_back(Point_2(std::stod(tokens[1]), std::stod(tokens[2])));
    }

    // Check the edge selection input if the incremental algorithm is chosen
    if (algorithm == "incremental")
    {
        if (initialization == "1a")
        {
            // Sort points by x coordinate largest to smallest
            std::sort(polygon.begin(), polygon.end(), [](Point_2 p1, Point_2 p2)
                      { return p1.x() > p2.x(); });
        }
        else if (initialization == "1b")
        {
            // Sort points by x coordinate smallest to largest
            std::sort(polygon.begin(), polygon.end(), [](Point_2 p1, Point_2 p2)
                      { return p1.x() < p2.x(); });
        }
        else if (initialization == "2a")
        {
            // Sort points by y coordinate largest to smallest
            std::sort(polygon.begin(), polygon.end(), [](Point_2 p1, Point_2 p2)
                      { return p1.y() > p2.y(); });
        }
        else if (initialization == "2b")
        {
            // Sort points by y coordinate smallest to largest
            std::sort(polygon.begin(), polygon.end(), [](Point_2 p1, Point_2 p2)
                      { return p1.y() < p2.y(); });
        }
    }

    int constructionTime = 0;
    // Create the polygon A according to the algorithm chosen by the user
    Polygon_2 A = convexHullAlgorithm(polygon, edgeSelection, constructionTime);

    // Optimize the polygon A for maximum area based on the algorithm of simulated annealing
    // Polygon_2 optimizedPolygon = simulatedAnnealing(A);
    

    // Construct a CGAL kd-Tree inserting all the points of the polygon A
    CGAL::Kd_tree<CGAL::Search_traits_2<Kernel>> kdTree(A.begin(), A.end());

    localStep(A, kdTree);

    return 0;
}

// Polygon_2 simulatedAnnealing(Polygon_2 A)
// {
//     bool isLocal = false;
//     double T = 1;
//     // number of iterations
//     int L = 0;
//     while (T >= 0)
//     {
//         L++;
//         // 4: Perform transition step: local xor global according to parameter.
//         if(isLocal)
//         {
//             localStep(A);
//         }
//         else
//         {
//             globalStep(A);
//         }
//         // 5: Check validity. If not valid goto 4.
//         if (validityCheck(A, isLocal))
//         {

//         }
//         else
//         {
//             // Go back to previous state
//             continue;
//         }

//         // 6: If ∆E < 0 or Metropolis criterion holds, apply the transition.
//         if (deltaE < 0 || metropolisCriterion())
//         {
//             applyTransition(A);
//         }
//         else
//         {
//             // Go back to previous state
//             continue;
//         }
//         // 7: T = T − 1 / L  , where L is the number of iterations
//         T = T - (1 / L);
//     }//end while
// }

void localStep(Polygon_2& A, CGAL::Kd_tree<CGAL::Search_traits_2<Kernel>>& kdTree)
{
    std::cout << "---------A---------" << std::endl;
    printPolygon(A);
    std::cout << "-------------------" << std::endl;
    // Choose a random vertex q ∈ A
    int q = rand() % A.size();
    
    // Vertex r is the next vertex of q
    int r = (q + 1) % A.size();
    // Vertex p is the previous vertex of q, if q is the first vertex of A, p is the last vertex of A
    int p = (q == 0) ? A.size() - 1 : q - 1;
    // Vertex s is the next vertex of r
    int s = (r + 1) % A.size();

    // So the final order of the vertices is p, q, r, s

    // print the vertices' indices
    std::cout << "p: " << p << " q: " << q << " r: " << r << " s: " << s << std::endl;


    // Swap the vertices q and r
    std::swap(A[q], A[r]);


    validityCheck(A, true, A[p], A[q], A[r], A[s], kdTree);
    

}

bool validityCheck(Polygon_2 &A, bool isLocal, Point_2& p, Point_2& q, Point_2& r, Point_2& s, CGAL::Kd_tree<CGAL::Search_traits_2<Kernel>>& kdTree)
{
    if(isLocal)
    {
        // A)  Check if pr and qs are intersecting
        Polygon_2::Segment_2 pr(p, r);
        Polygon_2::Segment_2 qs(q, s);

        // Check if pr and qs are intersecting
        if (CGAL::do_intersect(pr, qs))
        {
            std::cout << "pr and qs are intersecting" << std::endl;
            return false;
        }

        // B)
        // A range-search query is performed, where the query is the rectangular
        // axis-parallel bounding box. For every point located in the range, test
        // whether one of the edges incident to the point intersects one of the edges pr or qs.

        // Construct the bounding box
        CGAL::Bbox_2 bbox = CGAL::Bbox_2(p.x(), p.y(), r.x(), r.y());
        // Construct the query rectangle
        CGAL::Kd_tree_rectangle<Kernel::FT> queryRectangle(bbox.xmin(), bbox.ymin(), bbox.xmax(), bbox.ymax());
        // Perform the range search
        std::vector<std::pair<Point_2, unsigned int>> result;
        kdTree.search(std::back_inserter(result), queryRectangle);


        // For every point located in the range, test whether one of the edges incident to the point intersects one of the edges pr or qs.
        for (auto it = result.begin(); it != result.end(); it++)
        {
            // Construct the edge incident to the point
            Polygon_2::Segment_2 edge(it->first, A[(it->second + 1) % A.size()]);
            // Check if the edge intersects one of the edges pr or qs
            if (CGAL::do_intersect(edge, pr) || CGAL::do_intersect(edge, qs))
            {
                std::cout << "The edge incident to the point intersects one of the edges pr or qs" << std::endl;
                return false;
            }
        }

    }

    return true;
}