#include <CGAL/Simple_cartesian.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;

void printPointVector(std::vector<Point_2> points);

class Edge {
public:
    Point_2 p1;
    Point_2 p2;

    Edge(Point_2 p1, Point_2 p2) {
        this->p1 = p1;
        this->p2 = p2;
    }

    bool isPointOfEdge(Point_2 p) {
        return (p1 == p || p2 == p);
    }
};


int main()
{
    Point_2 p1(1, 1), p2(10, 10), p3(5, 9), p4(2, 7), p5(3, 6), p6(3.5, 4.5), p7(5, 4), p8(6, 3), p9(9, 2);

    std::vector<Edge> edges;


    std::vector<Point_2> points;
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    points.push_back(p4);
    points.push_back(p5);
    points.push_back(p6);
    points.push_back(p7);
    points.push_back(p8);
    points.push_back(p9);

    // //Sort points by x-coordinate largest to smallest
    // std::sort(points.begin(), points.end(), [](Point_2 p1, Point_2 p2) { return p1.x() > p2.x(); });
    // printPointVector(points);

    // Sort points by x-coordinate smallest to largest
    std::sort(points.begin(), points.end(), [](Point_2 p1, Point_2 p2)
              { return p1.x() < p2.x(); });
    printPointVector(points);

    // Convex Hull empty set
    std::vector<Point_2> convexHull;

    // Insert first three points
    convexHull.push_back(points[0]);
    convexHull.push_back(points[1]);
    convexHull.push_back(points[2]);

    edges.push_back(Edge(points[0], points[1]));
    edges.push_back(Edge(points[1], points[2]));
    edges.push_back(Edge(points[2], points[0]));

    for (int i = 3; i < points.size(); i++)
    {
        for (auto edge : edges)
        {
            std::cout << "Point: " << points[i] << " -- Edge: " << edge.p1 << " -- " << edge.p2 << std::endl;
            // Balta sto .h
            // ---------------------------------------------------------------1----------------2------
            CGAL::Orientation testOrientation = CGAL::orientation(edge.p1, edge.p2, points[i]);
            // ---------------------------------------------------------------1----------------2----------------0----------
            CGAL::Orientation convHullOrientation = CGAL::orientation(edge.p1, edge.p2, convexHull[0]);

            if (edge.isPointOfEdge(convexHull[0]))
            {
                for (auto point : convexHull)
                {
                    if(!edge.isPointOfEdge(point))
                    {
                        convHullOrientation = CGAL::orientation(edge.p1, edge.p2, point);
                        break;
                    }
                }
            }

            // Blue Edge - same sign
            if (testOrientation == convHullOrientation && testOrientation != CGAL::COLLINEAR)
            {
                // Print that it's a blue edge
                std::cout << "Blue Edge" << std::endl;
            }
            // Red Edge - different sign
            else if (testOrientation != convHullOrientation && testOrientation != CGAL::COLLINEAR)
            {
                // Print that it's a red edge
                std::cout << "Red Edge" << std::endl;
            }
            else
            {
                // Print that it's a collinear edge
                std::cout << "Collinear Edge" << std::endl;
            }
        }
        std::cout << std::endl;
        // break;
    }

    // convexHull.push_back(p1);

    return 0;
}

void printPointVector(std::vector<Point_2> points)
{
    for (Point_2 p : points)
        std::cout << p << std::endl;
}