#include "simulated_annealing.h"

// swaps the position of two polygon points
int swap_polygon_points(Polygon_2 &polygon, int indexA, int indexB, bool last_point)
{
    if (indexA > polygon.size() - 1 || indexB > polygon.size() - 1)
    {
        return ERROR;
    }
    if (last_point)
    {                                           // A is the last point of the polygon
        Point_2 first_vertex = polygon[indexB]; // indexB = 0
        polygon.insert(polygon.vertices_begin() + indexB, polygon[indexA]);
        polygon.insert(polygon.vertices_end(), first_vertex);
        polygon.erase(polygon.vertices_begin() + indexA + 1);
        polygon.erase(polygon.vertices_begin() + 1);
        return 0;
    }
    polygon.insert(polygon.vertices_begin() + indexB + 1, polygon[indexA]);
    polygon.insert(polygon.vertices_begin() + indexA + 1, polygon[indexB]);
    polygon.erase(polygon.vertices_begin() + indexA);
    polygon.erase(polygon.vertices_begin() + indexB);
    return 0;
}

// local transition step: swaps the position of two adjacent polygon points
int local_transition_step(Polygon_2 &polygon, int indexA)
{
    if (indexA == polygon.size() - 1)
    {
        if (!swap_polygon_points(polygon, indexA, 0, true))
        { // if A is the last value, swap it with polygon[0]
            // check validity of polygon
            // Construct a kd-Tree at initialization; all points inserted (CGAL class)
            return 0;
        }
        return ERROR;
    }
    if (!swap_polygon_points(polygon, indexA, indexA + 1, false))
    {
        // check validity of polygon
        // Construct a kd-Tree at initialization; all points inserted (CGAL class)
        return 0;
    }
    else
    {
        return ERROR;
    }
}
// local transition step: swaps the position of two randomply selected polygon points
bool globalStep(Polygon_2 &A)
{
    // Choose q and s randomly
    int q = 0;
    int s = 0;
    q = rand() % (A.size());
    while (1)
    {
        s = rand() % (A.size());
        if (q != s)
        {
            break;
        }
    }

    // Vertex p is the previous vertex of q, if q is the first vertex of A, p is the last vertex of A
    int p = (q == 0) ? A.size() - 1 : q - 1;

    // Vertex r is the next vertex of q
    int r = (q + 1) % A.size();

    // Vertex t is the next vertex of s
    int t = (s + 1) % A.size();

    // The two orders are: pqr and st
    
    Polygon_2 saveA = A;

    // Insert vertex q after vertex s
    A.insert(A.vertices_begin() + s + 1, A[q]);
    // Erase point q from polygon(initial place)
    A.erase(A.vertices_begin() + q);

    // if(!validityCheckGlobal(A, saveA[p], saveA[q], saveA[r], saveA[s], saveA[t]))
    // // Validity check
    // {
    //     A = saveA;
    //     return false;
    // }
    
    // If polygon is not simple, revert to the initial polygon
    if (!A.is_simple())
    {
        A = saveA;
        return false;
    }

    return true;
}

double Energy_of_State_P(Polygon_2 polygon, Polygon_2 convex_hull, bool is_Maximization)
{
    if (!is_Maximization)
    {
        return (double)(polygon.size() * ((std::abs(polygon.area())) / std::abs(convex_hull.area())));
    }
    else
    {
        return (double)(polygon.size() * (1 - (std::abs(polygon.area()) / std::abs(convex_hull.area()))));
    }
}

double compute_deltaEnergy(double finalEnergy, double initialEnergy)
{
    return (double)finalEnergy - initialEnergy;
}

bool Metropolis_criterion(double deltaEnergy, double T)
{
    if (T == 0.0)
    {
        std::cout << "T is zero. Metropolis is not defined" << std::endl;
        return false;
    }
    double R = (double)rand() / (RAND_MAX + 1.);
    if (R >= 1.0)
    {
        std::cout << "ERROR_____________________R___________________________: " << R << std::endl;
        exit(1);
    }

    double exp = std::exp(-deltaEnergy / T);
    if (exp > R)
    {
        return true;
    }
    return false;
}

bool validityCheckLocal(Polygon_2 &A, Point_2 &p, Point_2 &q, Point_2 &r, Point_2 &s, CGAL::Kd_tree<CGAL::Search_traits_2<Kernel>> &kdTree)
{
    // A)  Check if pr and qs are intersecting

    //Construct the segments pr and qs
    Polygon_2::Segment_2 pr(p, r);
    Polygon_2::Segment_2 qs(q, s);

    // The validity check is false if the segments pr and qs intersect
    if (CGAL::do_intersect(pr, qs))
    {
        return false;
    }

    // B)
    // A range-search query is performed, where the query is the rectangular
    // axis-parallel bounding box. For every point located in the range, test
    // whether one of the edges incident to the point intersects one of the edges pr or qs.

    // we check whether either of the edges
    // pr or qs intersect any other segment in the polygonal chain as follows: we submit a range-search
    // query, where the query range is the rectangular axis-parallel bounding box of the points p, q, r,
    // and s. For every point located in the range, we test whether one of the two segments incident to
    // the point intersects one of pr or qs

    // Construct a rectangle with the points p, q, r, and s
    Polygon_2::Point_2 minPoint(std::min(p.x(), std::min(q.x(), std::min(r.x(), s.x()))),
                                std::min(p.y(), std::min(q.y(), std::min(r.y(), s.y()))));
    Polygon_2::Point_2 maxPoint(std::max(p.x(), std::max(q.x(), std::max(r.x(), s.x()))),
                                std::max(p.y(), std::max(q.y(), std::max(r.y(), s.y()))));

    // Construct the fuzzy box
    CGAL::Fuzzy_iso_box<CGAL::Search_traits_2<Kernel>> fuzzyBox(minPoint, maxPoint);

    // Perform the range search
    std::list<Point_2> result;
    kdTree.search(std::back_inserter(result), fuzzyBox);

    // Check if any of the edges incident to the point intersects one of the edges pr or qs
    for (auto it = result.begin(); it != result.end(); ++it)
    {
        for (auto it2 = A.edges_begin(); it2 != A.edges_end(); ++it2)
        {
            // If the edge is incident to the point
            if (it2->source() == *it || it2->target() == *it)
            {
                // If the edge intersects pr or qs
                if (checkSegmentsIntersect(*it2, pr) || checkSegmentsIntersect(*it2, qs))
                {
                    return false;
                }
            }
        }
    }


    if(A.is_simple())
    {
        // return true;
    }
    else
    {
        std::cout << "A is not simple" << std::endl;
        std::cout << "---------A---------" << std::endl;
        printPolygon(A);
        std::cout << "-------------------" << std::endl;
        //print p, q, r, s
        std::cout << "p: " << p << std::endl;
        std::cout << "q: " << q << std::endl;
        std::cout << "r: " << r << std::endl;
        std::cout << "s: " << s << std::endl;

    }
    return true;
}

bool localStep(Polygon_2 &A, CGAL::Kd_tree<CGAL::Search_traits_2<Kernel>> &kdTree)
{
    // Choose a random vertex q ∈ A
    int q = rand() % A.size();
    // Vertex r is the next vertex of q
    int r = (q + 1) % A.size();
    // Vertex p is the previous vertex of q, if q is the first vertex of A, p is the last vertex of A
    int p = (q == 0) ? A.size() - 1 : q - 1;
    // Vertex s is the next vertex of r
    int s = (r + 1) % A.size();

    // So the final order of the vertices is p, q, r, s

    // Check if swapping q and r is valid
    if (validityCheckLocal(A, A[p], A[q], A[r], A[s], kdTree))
    {
        // Swap them if it is valid
        std::swap(A[q], A[r]);
        return true;
    }
    return false;
}

bool checkSegmentsIntersect(Polygon_2::Segment_2 segment, Polygon_2::Segment_2 pr)
{
    const auto mutual = CGAL::intersection(segment, pr);
    if (mutual)
    {
        // If the intersection is a Segment
        if (const Polygon_2::Segment_2 *mutualSegment = boost::get<Polygon_2::Segment_2>(&*mutual))
        {
            // Check if the mutualSegment is the same as pr and ignore it
            if(*mutualSegment == pr)
            {
                return false;
            }
            return true;
        }
        else
        {
            // If the intersection is a Point
            const Point_2 *mutualPoint = boost::get<Point_2>(&*mutual);

            // If the mutualPoint is the same as p or r ignore it
            if(*mutualPoint == pr.source() || *mutualPoint == pr.target())
                return false;
            else
            // If a mutualPoint different from p and r is found, the new polygon cannot be valid
                return true;
        }
    }
    return false;
}

bool validityCheckGlobal(Polygon_2 &A, Point_2& p, Point_2& q, Point_2& r, Point_2& s, Point_2& t)
{
    // Construct the segments pr, sq, qt
    Polygon_2::Segment_2 pr(p, r);
    Polygon_2::Segment_2 sq(s, q);
    Polygon_2::Segment_2 qt(q, t);

    // Check if pr intersects sq or qt
    if (CGAL::do_intersect(pr, sq) || CGAL::do_intersect(pr, qt))
        return false;

    // Check if each of these new segments intersects any of the edges of A
    for (auto it = A.edges_begin(); it != A.edges_end(); ++it)
    {
        if (checkSegmentsIntersect(pr, *it) || checkSegmentsIntersect(sq, *it) || checkSegmentsIntersect(qt, *it))
            return false;
    } 

    return true;
}

int minimization_algorithm(Polygon_2 &polygon, bool isLocalStep, int L, bool is_Maximization, int &constructionTime)
{
    // Start the clock
    clock_t start = clock();
    clock_t end;

    double T = 1.0;
    Polygon_2 save_polygon = polygon;
    Polygon_2 convex_hull;
    CGAL::convex_hull_2(polygon.begin(), polygon.end(), std::back_inserter(convex_hull));

    double initialEnergy = Energy_of_State_P(polygon, convex_hull, is_Maximization);
    double deltaEnergy = 0.0;

    while (T > 0)
    {
        // Construct a CGAL kd-Tree inserting all the points of the polygon A
        CGAL::Kd_tree<CGAL::Search_traits_2<Kernel>> kdTree(polygon.begin(), polygon.end());
        if (isLocalStep)
        {
            if(!localStep(polygon, kdTree))
                continue;
        }
        else
        {
            if(!globalStep(polygon))
                continue;
        }

        Polygon_2 convex_hull;
        CGAL::convex_hull_2(polygon.begin(), polygon.end(), std::back_inserter(convex_hull));

        deltaEnergy = compute_deltaEnergy(Energy_of_State_P(polygon, convex_hull, is_Maximization), initialEnergy);
        if (deltaEnergy < 0.0 || Metropolis_criterion(deltaEnergy, T))
        { // the change is accepted
            Polygon_2 convex_hull;
            CGAL::convex_hull_2(polygon.begin(), polygon.end(), std::back_inserter(convex_hull));

            initialEnergy = Energy_of_State_P(polygon, convex_hull, is_Maximization); // update the energy
            save_polygon = polygon; // update saved polygon
        }
        else
        { // the change is not accepted, and we set the polygon to its previous state
            polygon = save_polygon;
        }
        T -= 1 / (double)L;
        // is polygon simple
        if (!polygon.is_simple())
        {
            std::cout << " Polygon is not simple " << std::endl;
        }
    }

    // Stop the clock
    end = clock();

    // Construction time of incremental algorithm in milliseconds
    constructionTime = (end - start) / (double)(CLOCKS_PER_SEC / 1000);
    
    return 0;
}


void spatialSubdivision(Polygon_2 &pointset, int &constructionTime)
{
    // Start the clock
    clock_t start = clock();
    clock_t end;
    std::vector<Polygon_2> subsets;
    splitIntoSubsets(pointset, subsets);


    // print subsets
    for(int i = 0; i < subsets.size(); i++)
    {
        std::cout << "----------------------- Subset " << i << "-----------------------" << std::endl;
        printPolygon(subsets[i]);
    }
   
    Polygon_2 A = incrementalAlgorithm(pointset, 3, constructionTime, true);

    // Stop the clock
    end = clock();

    // Construction time of incremental algorithm in milliseconds
    constructionTime = (end - start) / (double)(CLOCKS_PER_SEC / 1000);
}

void splitIntoSubsets(Polygon_2& pointset, std::vector<Polygon_2>& subsets)
{
    int m = 10;

    // Sort in lexicographic order -x
    std::sort(pointset.begin(), pointset.end(), [](Point_2 p1, Point_2 p2)
              { return p1.x() < p2.x(); });

    // Number of subsets
    int k = std::ceil((double)(pointset.size() - 1) / (m - 1));
    std::cout << "K " << k << std::endl;

    int lastInsertedIndex = 0;
    // for every subset, insert as close as possible to m points
    for(int i = 0; i < k-1; i++)
    {
        subsets.push_back(Polygon_2());

        // Insert the first point (same as the rightmost point of the previous subset)
        subsets[i].push_back(pointset[lastInsertedIndex]);

        // Are the Lower Hull conditions met?
        bool conditionsMetLH = true;

        //  While [(we haven't surpassed the preassigned number of points for the current subset or if we have and the conditions are not met)
        //        && (we haven't reached the end of the pointset) ]
        while( (lastInsertedIndex < m*(i+1) || !conditionsMetLH) && lastInsertedIndex < pointset.size()-1 )
        {
            // Insert the next point
            subsets[i].push_back(pointset[lastInsertedIndex + 1]);
            lastInsertedIndex++;

            // If we' ve reached or surpassed the preassigned number of points for the current subset
            if ( lastInsertedIndex >= m * (i+1))
            {
                // Check if the conditions are met
                if (!(pointset[lastInsertedIndex].y() > pointset[lastInsertedIndex + 1].y()
                && pointset[lastInsertedIndex].y() > pointset[lastInsertedIndex - 1].y()))
                    // If not, continue inserting points until the conditions are met
                    conditionsMetLH = false;
                else
                    // If they are, stop inserting points
                    conditionsMetLH = true;
            }
        }
    }
}