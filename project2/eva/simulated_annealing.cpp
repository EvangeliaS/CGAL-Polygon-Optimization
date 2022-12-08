#include "simulated_annealing.h"

// swaps the position of two polygon points
int swap_polygon_points(Polygon_2 &polygon, int indexA, int indexB, bool last_point)
{
    if (indexA > polygon.size() - 1 || indexB > polygon.size() - 1)
    {
        // std::cout << "(indexA: " << indexA << "  , indexB: " << indexB << ") " << std::endl;
        // std::cout << "The indices are out of bound. Please enter different indexA and/or indexB" << std::endl;
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
int global_transition_step(Polygon_2 &polygon)
{
    // Choose q and s randomly
    int index_q = 0;
    int index_s = 0;
    index_q = rand() % (polygon.size() - 1);
    while (1)
    {
        index_s = rand() % (polygon.size() - 1);
        if (index_q != index_s)
        {
            break;
        }
    }
    // std::cout << "index_s " << index_s << " " << " index_q " << index_q << std::endl;

    // Insert vertex q after vertex s
    polygon.insert(polygon.vertices_begin() + index_s + 1, polygon[index_q]);
    // Erase point q from polygon(initial place)
    polygon.erase(polygon.vertices_begin() + index_q);
    return 0;
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

    // std::cout << "R: " << R  << std::endl;

    double exp = std::exp(-deltaEnergy / T);
    if (exp > R)
    {
        // std::cout <<"_____________________exp___________________________: " << exp  << std::endl;
        return true;
    }
    return false;
}

bool validityCheck(Polygon_2 &A, bool isLocal, Point_2 &p, Point_2 &q, Point_2 &r, Point_2 &s, CGAL::Kd_tree<CGAL::Search_traits_2<Kernel>> &kdTree)
{
    // if(isLocal)
    // {
    //     // A)  Check if pr and qs are intersecting
    //     Polygon_2::Segment_2 pr(p, r);
    //     Polygon_2::Segment_2 qs(q, s);

    //     // // Check if pr and qs are intersecting
    //     if (CGAL::do_intersect(pr, qs))
    //     {
    //         std::cout << "pr and qs are intersecting" << std::endl;
    //         return false;
    //     }

    //     // B)
    //     // A range-search query is performed, where the query is the rectangular
    //     // axis-parallel bounding box. For every point located in the range, test
    //     // whether one of the edges incident to the point intersects one of the edges pr or qs.

    //     //  we check whether either of the edges
    //     // pr or qs intersect any other segment in the polygonal chain as follows: we submit a range-search
    //     // query, where the query range is the rectangular axis-parallel bounding box of the points p, q, r,
    //     // and s. For every point located in the range, we test whether one of the two segments incident to
    //     // the point intersects one of pr or qs

    //     // Construct a rectangle with the points p, q, r, and s
    //     Polygon_2::Point_2 minPoint(std::min(p.x(), std::min(q.x(), std::min(r.x(), s.x()))),
    //                                 std::min(p.y(), std::min(q.y(), std::min(r.y(), s.y()))));
    //     Polygon_2::Point_2 maxPoint(std::max(p.x(), std::max(q.x(), std::max(r.x(), s.x()))),
    //                                 std::max(p.y(), std::max(q.y(), std::max(r.y(), s.y()))));

    //     // Construct the fuzzy box
    //     CGAL::Fuzzy_iso_box<CGAL::Search_traits_2<Kernel>> fuzzyBox(minPoint, maxPoint);

    //     // Perform the range search
    //     std::list<Point_2> result;
    //     kdTree.search(std::back_inserter(result), fuzzyBox);

    //     // Check if any of the edges incident to the point intersects one of the edges pr or qs
    //     for (auto it = result.begin(); it != result.end(); ++it)
    //     {
    //         // Find the position of it in the polygon
    //         int index = findPolygonPoint(A, *it);
    //         int indexNext = (index + 1) % A.size();
    //         int indexPrev = (index == 0) ? A.size() - 1 : index - 1;

    //         // If it is the point p, q, r, or s, continue
    //         // if (p == *it || q == *it || r == *it || s == *it)
    //         // {
    //         //     continue;
    //         // }
    //         // id A[indexPrev] == p,q,r,s or A[indexNext] == p,q,r,s, continue
    //         // if (A[indexPrev] == p || A[indexPrev] == q || A[indexPrev] == r || A[indexPrev] == s ||
    //         //     A[indexNext] == p || A[indexNext] == q || A[indexNext] == r || A[indexNext] == s)
    //         // {
    //         //     continue;
    //         // }

    //         // Construct the segments incident to the point
    //         Polygon_2::Segment_2 segment1(A[indexPrev], *it);
    //         Polygon_2::Segment_2 segment2(*it, A[indexNext]);

    //         // Ignore the edges pq and rs
    //         Polygon_2::Segment_2 pq(p, q);
    //         Polygon_2::Segment_2 rs(r, s);
    //         if (segment1 == pq || segment1 == rs || segment2 == pq || segment2 == rs)
    //         {
    //             std::cout << "Ignoring pq or rs" << std::endl;
    //             continue;
    //         }

    //         if(checkSegmentsIntersect(segment1, pr, true))
    //             return false;
    //         if(checkSegmentsIntersect(segment2, pr, true))
    //             return false;
    //         if(checkSegmentsIntersect(segment1, qs, false))
    //             return false;
    //         if(checkSegmentsIntersect(segment2, qs, false))
    //             return false;

    //         // Check if the segments intersect pr or qs
    //         // if (CGAL::do_intersect(segment1, pr) || CGAL::do_intersect(segment1, qs) ||
    //         //     CGAL::do_intersect(segment2, pr) || CGAL::do_intersect(segment2, qs))
    //         // {

    //         //     // std::cout << "pr and qs are intersecting" << std::endl;
    //         //     return false;
    //         // }
    //         // if (CGAL::do_intersect(segment1, pr))
    //         // {
    //         //     // Print segment1.source() and segment1.target(), pr.source() and pr.target()
    //         //     std::cout << "Intersection: " << segment1.source() << "  "  <<  segment1.target() << ", " << pr.source() << "   " << pr.target() << std::endl;
    //         //     return false;
    //         // }
    //         // if (CGAL::do_intersect(segment1, qs))
    //         // {
    //         //     // Print segment1.source() and segment1.target(), qs.source() and qs.target()
    //         //     std::cout << "Intersection: " << segment1.source() << "  "  <<  segment1.target() << ", " << qs.source() << "   " << qs.target() << std::endl;
    //         //     return false;
    //         // }
    //         // if (CGAL::do_intersect(segment2, pr))
    //         // {
    //         //     // Print segment2.source() and segment2.target(), pr.source() and pr.target()
    //         //     std::cout << "Intersection: " << segment2.source() << "  "  <<  segment2.target() << ", " << pr.source() << "   " << pr.target() << std::endl;
    //         //     return false;
    //         // }
    //         // if (CGAL::do_intersect(segment2, qs))
    //         // {
    //         //     // Print segment2.source() and segment2.target(), qs.source() and qs.target()
    //         //     std::cout << "Intersection: " << segment2.source() << "  "  <<  segment2.target() << ", " << qs.source() << "   " << qs.target() << std::endl;
    //         //     return false;
    //         // }

    //     }
    //     std::cout << "Here" << std::endl;

    // }
    // if(A.is_simple())
    // {
    //     // return true;
    // }
    // else
    // {
    //     std::cout << "A is not simple" << std::endl;
    //     std::cout << "---------A---------" << std::endl;
    //     printPolygon(A);
    //     std::cout << "-------------------" << std::endl;
    //     //print p, q, r, s
    //     std::cout << "p: " << p << std::endl;
    //     std::cout << "q: " << q << std::endl;
    //     std::cout << "r: " << r << std::endl;
    //     std::cout << "s: " << s << std::endl;

    // }
    // return true;
    if (A.is_simple())
    {
        return true;
    }
    return false;
}

void localStep(Polygon_2 &A, CGAL::Kd_tree<CGAL::Search_traits_2<Kernel>> &kdTree)
{
    // std::cout << "---------A---------" << std::endl;
    // printPolygon(A);
    // std::cout << "-------------------" << std::endl;
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
    // std::cout << "p: " << p << " q: " << q << " r: " << r << " s: " << s << std::endl;

    // Swap the vertices q and r
    std::swap(A[q], A[r]);

    if (!validityCheck(A, true, A[p], A[q], A[r], A[s], kdTree))
        // Swap them back
        std::swap(A[q], A[r]);
}

bool checkSegmentsIntersect(Polygon_2::Segment_2 segment, Polygon_2::Segment_2 pr, bool isFirst)
{
    const auto mutual = CGAL::intersection(segment, pr);
    if (mutual)
    {
        // If the intersection is a Segment
        if (const Polygon_2::Segment_2 *mutualSegment = boost::get<Polygon_2::Segment_2>(&*mutual))
        {
            // // Check if the mutualSegment is the same as the edge and ignore it
            // if (mutualSegment->source() == pr.source() && mutualSegment->target() == pr.target())
            // {
            //     return false;
            // }
            // else
            // {
            //     // Found an intersection Segment, therefore the new polygon cannot be valid
            //     return true;
            // }
            std::cout << "MUTUAL SEGMENT" << std::endl;
            return true;
        }
        else
        {
            // If the intersection is a Point
            const Point_2 *mutualPoint = boost::get<Point_2>(&*mutual);
            // Check if the mutualPoint is the same as p ignore it
            if (isFirst && mutualPoint->x() == pr.source().x() && mutualPoint->y() == pr.source().y())
            {
                std::cout << "isFirst" << std::endl;
                return false;
            }
            // // Check if the mutualPoint is the same as r ignore it
            else if (!isFirst && mutualPoint->x() == pr.target().x() && mutualPoint->y() == pr.target().y())
            {
                std::cout << "isSecond" << std::endl;
                return false;
            }
            else
            {
                // print the 2 segments and the mutualPoint
                std::cout << "segment: " << segment.source() << " " << segment.target() << ", pr: " << pr.source() << " " << pr.target() << ", mutualPoint: " << *mutualPoint << std::endl;
                return true;
            }
        }
    }
    return false;
}

int minimization_algorithm(Polygon_2 &polygon, bool isLocalStep, int L, bool is_Maximization)
{
    double T = 1.0;
    Polygon_2 save_polygon = polygon;
    Polygon_2 save_polygon_for_Metropolis = polygon; // if polygon is not accepted by metropolis criterion

    std::cout << " Initial area of polygon: " << std::abs(polygon.area()) << std::endl;
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
            localStep(polygon, kdTree);
            // int index = rand() % polygon.size();

            // if (!local_transition_step(polygon, index) && polygon.is_simple())
            // { // check validity with kd-tree
            //     // std::cout << " Local transision step is successful " << index << std::endl;
            // }
            // else
            // { // Polygon is not valid: (goto 4)
            //     // std::cout << "Not valid " << index << std::endl;
            //     polygon = save_polygon;
            //     continue;
            // }
        }
        else
        { // global transition step -- //check validity with intersect
            if (!global_transition_step(polygon) && polygon.is_simple())
            {
                // std::cout << " Global transision step is successful " << std::endl;
            }
            else
            { // Polygon is not valid: (goto 4)
                polygon = save_polygon;
                continue;
            }
        }

        Polygon_2 convex_hull;
        CGAL::convex_hull_2(polygon.begin(), polygon.end(), std::back_inserter(convex_hull));

        deltaEnergy = compute_deltaEnergy(Energy_of_State_P(polygon, convex_hull, is_Maximization), initialEnergy);
        // std::cout << deltaEnergy << "**************delta energy" << std::endl;
        if (deltaEnergy < 0.0 || Metropolis_criterion(deltaEnergy, T))
        { // the change is accepted
            Polygon_2 convex_hull;
            CGAL::convex_hull_2(polygon.begin(), polygon.end(), std::back_inserter(convex_hull));

            initialEnergy = Energy_of_State_P(polygon, convex_hull, is_Maximization); // update the energy
            // std::cout << initialEnergy << "**************final energy" << std::endl;
            save_polygon = polygon; // update saved polygon
        }
        else
        { // the change is not accepted, and we set the polygon to its previous state
            polygon = save_polygon;
        }
        T -= 1 / (double)L;
    }
    return 0;
}

int spatialSubdivision(Polygon_2 &polygon, int m)
{
    /*    if(polygon.size()<1000){
            std::cout << "Please input a bigger polygon (>1000 points)" << std::endl;
            return ERROR;
        }*/
    // Sort in lexicographic order -x
    std::sort(polygon.begin(), polygon.end(), [](Point_2 p1, Point_2 p2)
              { return p1.x() < p2.x(); });
    printPolygon(polygon);

    // Number of subsets
    int k = std::ceil((polygon.size() - 1) / m - 1);
    std::cout << "K " << k << std::endl;

    // Perform global search on every subpolygon
    int i = 0;
    bool isFirst_subpolygon = true;
    Point_2 rightmost, leftmost;
    std::vector<Polygon_2> subsets;

    // For every point of the pointset
    while (i < polygon.size())
    {
        // create subpolygon
        for (auto it = polygon.begin() + i; it != polygon.end() && it != polygon.begin() + i + m; ++it)
        {
            if (!isFirst_subpolygon)
            {
                // check if the point between two subpolygons has y bigger than its prev and next points
                if (!(polygon[findPolygonPoint(polygon, rightmost)].y() > polygon[findPolygonPoint(polygon, rightmost) + 1].y() && polygon[findPolygonPoint(polygon, rightmost)].y() > polygon[findPolygonPoint(polygon, rightmost) - 1].y()))
                {
                    std::cout << " prev " << polygon[findPolygonPoint(polygon, rightmost) - 1] << " rightmost of s-1 " << rightmost << " next " << polygon[findPolygonPoint(polygon, rightmost) + 1] << std::endl;
                }
            }
            subsets[0].push_back(*it);
        }
        std::cout << "_____________________________________________" << std::endl;
        printPolygon(subsets[0]);

        if (isFirst_subpolygon)
        {
            // std::cout << "last point " << subpolygon[subpolygon.size() - 1] << " rightmost ";
            rightmost = *(right_vertex_2(subsets[0].begin(), subsets[0].end()));
        }
        else
        {
            leftmost = *(left_vertex_2(subsets[0].begin(), subsets[0].end()));
            rightmost = *(right_vertex_2(subsets[0].begin(), subsets[0].end()));
            // std::cout << "first point " << subpolygon[0] << " leftmost "  << *(left_vertex_2(subpolygon.begin(), subpolygon.end()));;
        }

        isFirst_subpolygon = false;

        i += m - 1;
    }
    return true;
}


void spDiv(Polygon_2 &pointset)
{
    std::vector<Polygon_2> subsets;
    splitIntoSubsets(pointset, subsets);

    // print subsets
    for(int i = 0; i < subsets.size(); i++)
    {
        std::cout << "----------------------- Subset " << i << "-----------------------" << std::endl;
        printPolygon(subsets[i]);
    }
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