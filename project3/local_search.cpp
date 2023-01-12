#include "local_search.h"

void findPaths(Polygon_2 A, std::vector<Polygon_2> &paths, int k, Segment_2 edge)
{
    // for every vertex of A
    for (auto v = A.vertices_begin(); v != A.vertices_end(); v++)
    {
        auto saveV = v;
        for (int i = 1; i <= k; i++)
        {
            Polygon_2 path;
            for (int j = 1; j <= i; j++)
            {
                path.push_back(*v);
                v++;
                if (v == A.vertices_end())
                    v = A.vertices_begin();
            }

            v = saveV;

            //  if the path includes the edge, ignore it
            if (isEdgePartOfPath(path, edge))
                continue;

            paths.push_back(path);
        }
    }
}

bool isEdgePartOfPath(Polygon_2 path, Segment_2 edge)
{
    for (auto p = path.vertices_begin(); p != path.vertices_end(); p++)
    {
        if (*p == edge.source() || *p == edge.target())
            return true;
    }
    return false;
}

void printPaths(std::vector<Polygon_2> paths)
{
    int i = 1;
    for (auto path : paths)
    {

        std::cout << "Path " << i << ":" << std::endl;
        for (auto point : path)
        {
            std::cout << point << std::endl;
        }
        i++;
    }
}


bool local_search(Polygon_2 &A, int k, double threshold, int deltaArea, bool isMaximization, int& constructionTime, int remainingTime){
    // Start the clock
    clock_t start = clock();
    clock_t elapsed;
    clock_t end;


    // while ∆A ≥ threshold do
    while (1)
    {
        bool acceptableSolutionFound = false;
        if(isMaximization)
        {
            if(deltaArea < threshold)
                break;
        }
        else
        {
            if(deltaArea > threshold)
                break;
        }
        
        int area = std::abs(A.area());

        std::vector<PathEdge> T;
        bool flag = false;
        // for every edge e ∈ S do
        for (auto e = A.edges_begin(); e != A.edges_end(); e++)
        {
            std::vector<Polygon_2> paths;
            // for every path V of length ≤ k do
            findPaths(A, paths, k, *e);
            for (auto path : paths)
            {
                // B is our testing polygon
                Polygon_2 B = A;

                // if V moving to e increases area and retains simplicity
               
                // Remove path from B
                for (auto p = path.vertices_begin(); p != path.vertices_end(); p++)
                    B.erase(std::find(B.begin(), B.end(), *p));

                // Add path between e.begin() and e.end()
                int indexSource = findPolygonPoint(B, e->source());
                int indexTarget = findPolygonPoint(B, e->target());
                if (indexSource < indexTarget)
                {
                    for (auto p = path.vertices_begin(); p != path.vertices_end(); p++)
                        B.insert(B.vertices_begin() + indexTarget, *p);
                }
                else
                {
                    for (auto p = path.vertices_begin(); p != path.vertices_end(); p++)
                        B.insert(B.vertices_begin() + indexSource, *p);
                }

                // If it retains simplicity then we add it to T
                if (B.is_simple())
                {
                    // Polygon_2 addedPolygon;

                    // int indexPreviousVertex = findPolygonPoint(A, path[0]);
                    // if (indexPreviousVertex == 0)
                    //     indexPreviousVertex = A.size() - 1;
                    // else
                    //     indexPreviousVertex--;

                    // int indexNextVertex = findPolygonPoint(A, path[path.size() - 1]);

                    // if (indexNextVertex == A.size() - 1)
                    //     indexNextVertex = 0;
                    // else
                    //     indexNextVertex++;

                    // // is indexPreviousVertex a point of the path
                    // bool isInPath = false;
                    // for (auto point : path)
                    // {
                    //     if (point == A[indexPreviousVertex])
                    //     {
                    //         isInPath = true;
                    //         std::cout << "Previous vertex is in path" << std::endl;
                    //         break;
                    //     }

                    //     if(point == A[indexNextVertex])
                    //     {
                    //         isInPath = true;
                    //         std::cout << "Next vertex is in path" << std::endl;
                    //         break;
                    //     }
                    // }

                    // // Add previous vertex, path and next vertex to the addedPolygon
                    // addedPolygon.push_back(A[indexPreviousVertex]);
                    // for (auto point : path)
                    //     addedPolygon.push_back(point);
                    // addedPolygon.push_back(A[indexNextVertex]);
                    int Barea = std::abs(B.area());
                    if(isMaximization)
                    {
                        if (Barea - area > threshold)
                        {
                            acceptableSolutionFound = true;
                            T.push_back(PathEdge{path, *e, Barea - area});
                            // std::cout << "Acceptable solution found" << std::endl;
                        }
                    }
                    else
                    {
                        if (Barea - area < threshold)
                        {
                            acceptableSolutionFound = true;
                            T.push_back(PathEdge{path, *e, Barea - area});

                            // std::cout << "Acceptable solution found" << std::endl;
                        }
                    }
                    // // Calculate the addedPolygon area
                    // int addedArea = std::abs(addedPolygon.area());

                    // Polygon_2 removedPolygon;
                    // // Add e.source(), the path and e.target() to removedArea
                    // removedPolygon.push_back(e->source());
                    // for (auto p = path.vertices_begin(); p != path.vertices_end(); p++)
                    //     removedPolygon.push_back(*p);
                    // removedPolygon.push_back(e->target());

                    // // Calculate the removedPolygon area
                    // int removedPolygonArea = std::abs(removedPolygon.area());

                    // // Calculate the difference between the added and removed polygons
                    // int delta = addedArea - removedPolygonArea;


                    T.push_back(PathEdge{path, *e, Barea - area});
                    if(acceptableSolutionFound && T.size() > 300)
                    {
                        flag = true;
                        break;
                    }
                } // end if
                if (flag)
                    break;
            }     // end for
        } // end for


        // Sort T by delta in descending order
        std::sort(T.begin(), T.end(), [](PathEdge p1, PathEdge p2)
                  { return p1.deltaArea > p2.deltaArea; });


        // Apply all changes in T to S
        if (!T.empty())
        {
            // Choose the path with the highest delta
            PathEdge chosenPEdge = T[0];
            
            // Choose the path with the lowest delta if we are minimizing
            if(!isMaximization)
            {
                chosenPEdge = T[T.size() - 1];
            }
            
            // Remove path from A
            for (auto p = chosenPEdge.path.vertices_begin(); p != chosenPEdge.path.vertices_end(); p++)
                A.erase(std::find(A.begin(), A.end(), *p));

            // Add path between e.begin() and e.end()
            int indexSource = findPolygonPoint(A, chosenPEdge.edge.source());
            int indexTarget = findPolygonPoint(A, chosenPEdge.edge.target());

            if (indexSource < indexTarget)
            {
                for (auto p = chosenPEdge.path.vertices_begin(); p != chosenPEdge.path.vertices_end(); p++)
                    A.insert(A.vertices_begin() + indexTarget, *p);
            }
            else
            {
                for (auto p = chosenPEdge.path.vertices_begin(); p != chosenPEdge.path.vertices_end(); p++)
                    A.insert(A.vertices_begin() + indexSource, *p);
            }

            // Keep best solution S′; ∆A ← Area(S′) − Area(S)
            // Update area - Area already calculated when we added the path to T
            deltaArea = chosenPEdge.deltaArea;
        }
        // If no path is found then we stop
        else
            break;
        // std::cout << "deltaArea: " << deltaArea << ", Number of paths: " << T.size() << std::endl;
        elapsed = (clock() - start) / (double)(CLOCKS_PER_SEC / 1000);
        if(elapsed > remainingTime)
            return false;
    }// end while


    // Stop the clock
    end = clock();

    // Construction time of incremental algorithm in milliseconds
    constructionTime = (end - start) / (double)(CLOCKS_PER_SEC / 1000);
    return true;
}