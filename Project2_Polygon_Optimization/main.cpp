#include "simulated_annealing.h"
#include "local_search.h"

int main(int argc, char **argv)
{
    if (argc < 16)
    {
        std::cout << "Not enough arguments" << std::endl;
        return ERROR;
    }

    std::string inputFile = argv[2];
    std::string outputFile = argv[4];
    std::string algorithm = argv[6];
    std::string polygonization = argv[9];
    std::string annealing = argv[13];

    // incremental or convex_hull
    std::string initialization_algorithm = argv[15];

    int L = std::stoi(argv[8]);
    double threshold = std::stod(argv[11]);

    if (algorithm != "local_search" && algorithm != "simulated_annealing" && algorithm != "ant_colony")
    {
        std::cout << "Invalid algorithm. Choose local_search or simulated_annealing. " << std::endl;
        return ERROR;
    }

    bool isMaximization;

    if (polygonization == "-min")
        isMaximization = false;
    else if (polygonization == "-max")
        isMaximization = true;
    else
    {
        std::cout << "Please provide min or max as parameter " << std::endl;
        return ERROR;
    }

    if (initialization_algorithm != "incremental" && initialization_algorithm != "convex_hull")
    {
        std::cout << "Invalid initialization algorithm. Choose incremental or convex_hull" << std::endl;
        return ERROR;
    }

    if (annealing != "local" && annealing != "global" && annealing != "subdivision")
    {
        std::cout << "Invalid annealing parameter. Choose local, global or subdivision" << std::endl;
        return ERROR;
    }

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

    Polygon_2 A;
    int constructionTime = 0;

    if (initialization_algorithm == "incremental")
    {
        // Sort points by x coordinate smallest to largest
        std::sort(polygon.begin(), polygon.end(), [](Point_2 p1, Point_2 p2)
                  { return p1.x() < p2.x(); });

        if (isMaximization)
            A = incrementalAlgorithm(polygon, 3, constructionTime, false);
        else
            A = incrementalAlgorithm(polygon, 2, constructionTime, false);
    }
    else
    {
        if (isMaximization)
            A = convexHullAlgorithm(polygon, 3, constructionTime);
        else
            A = convexHullAlgorithm(polygon, 2, constructionTime);
    }

    // check if the polygon created from initialization algorithm is valid (simple)
    if (!A.is_simple())
    {
        std::cout << "Polygon after gready is not simple. Please run the program using defferent input file. " << std::endl;
        return ERROR;
    }
    else
        std::cout << "Polygon after gready is simple" << std::endl;

    // Compute the area of the convexHull of the point set given
    Polygon_2 convexHull;
    CGAL::convex_hull_2(polygon.begin(), polygon.end(), std::back_inserter(convexHull));

    double initial_area = std::abs(A.area());
    double initial_ratio = std::abs(A.area()) / std::abs(convexHull.area());

    // PROGRAM STARTS HERE

    constructionTime = 0;

    if (algorithm == "local_search")
    {
        int deltaArea = 100;                                                        ///?????????????????????????????????????????????????????
        if(!isMaximization)
            deltaArea = -100;
        local_search(A, L, threshold, deltaArea, isMaximization, constructionTime); // we use L(input) as k
    }
    else if (algorithm == "simulated_annealing")
    {
        bool isLocalStep;
        if (annealing == "local")
            isLocalStep = true;
        else
            isLocalStep = false;

        if (annealing == "local" || annealing == "global")
            minimization_algorithm(A, isLocalStep, L, isMaximization, constructionTime);
        else
        {
            // Subdivision

            // For the subdivision algorithm we couldn't make sure that the
            // critical edges after the greedy algorithm were intact
            // and therefore we couldn't use the subdivision algorithm.

            // However, the algorithm's helper functions are implemented
            // For example, void splitIntoSubsets(Polygon_2& pointset, std::vector<Polygon_2>& subsets)
            // works as intended and can be used to split the initial pointset into subsets
            // keeping the monotonicity of the leftmost and rightmost points of each subset.
        }
    }
    else if (algorithm == "ant_colony")
    {
        std::cout << "Ant colony algorithm hasn't been implemented. Choose local_search or simulated_annealing. " << std::endl;
        return ERROR;
    }

    if (!A.is_simple())
        std::cout << "Polygon after optimization is not simple" << std::endl;
    else
        std::cout << "Polygon affter optimization is  simple" << std::endl;

    // Write the necessary information to the output file
    writeToOutput(A, initial_area, initial_ratio, polygonization, constructionTime, outputFile, algorithm);
    std::cout << "Done!\nOutput written to " << outputFile << std::endl;

    return 0;
}