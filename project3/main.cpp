#include <dirent.h>

#include "simulated_annealing.h"
#include "local_search.h"

// int main(int argc, char **argv)
// {
//     if (argc < 16)
//     {
//         std::cout << "Not enough arguments" << std::endl;
//         return ERROR;
//     }

//     std::string inputFile = argv[2];
//     std::string outputFile = argv[4];
//     std::string algorithm = argv[6];
//     std::string polygonization = argv[9];
//     std::string annealing = argv[13];

//     // incremental or convex_hull
//     std::string initialization_algorithm = argv[15];

//     int L = std::stoi(argv[8]);
//     double threshold = std::stod(argv[11]);

//     if (algorithm != "local_search" && algorithm != "simulated_annealing" && algorithm != "ant_colony")
//     {
//         std::cout << "Invalid algorithm. Choose local_search or simulated_annealing. " << std::endl;
//         return ERROR;
//     }

//     bool isMaximization;

//     if (polygonization == "-min")
//         isMaximization = false;
//     else if (polygonization == "-max")
//         isMaximization = true;
//     else
//     {
//         std::cout << "Please provide min or max as parameter " << std::endl;
//         return ERROR;
//     }

//     if (initialization_algorithm != "incremental" && initialization_algorithm != "convex_hull")
//     {
//         std::cout << "Invalid initialization algorithm. Choose incremental or convex_hull" << std::endl;
//         return ERROR;
//     }

//     if (annealing != "local" && annealing != "global" && annealing != "subdivision")
//     {
//         std::cout << "Invalid annealing parameter. Choose local, global or subdivision" << std::endl;
//         return ERROR;
//     }

//     srand(time(0));

//     std::ifstream input(inputFile);
//     if (!input)
//     {
//         std::cout << "Could not open file " << inputFile << std::endl;
//         return ERROR;
//     }

//     // Ignore first two lines
//     std::string line;
//     std::getline(input, line);
//     std::getline(input, line);

//     Polygon_2 polygon;

//     // Loop to read inputFile line by line
//     while (std::getline(input, line))
//     {
//         std::vector<std::string> tokens = split(line, '\t');
//         polygon.push_back(Point_2(std::stod(tokens[1]), std::stod(tokens[2])));
//     }

//     Polygon_2 A;
//     int constructionTime = 0;

//     if (initialization_algorithm == "incremental")
//     {
//         // Sort points by x coordinate smallest to largest
//         std::sort(polygon.begin(), polygon.end(), [](Point_2 p1, Point_2 p2)
//                   { return p1.x() < p2.x(); });

//         if (isMaximization)
//             A = incrementalAlgorithm(polygon, 3, constructionTime, false);
//         else
//             A = incrementalAlgorithm(polygon, 2, constructionTime, false);
//     }
//     else
//     {
//         if (isMaximization)
//             A = convexHullAlgorithm(polygon, 3, constructionTime);
//         else
//             A = convexHullAlgorithm(polygon, 2, constructionTime);
//     }

//     // check if the polygon created from initialization algorithm is valid (simple)
//     if (!A.is_simple())
//     {
//         std::cout << "Polygon after greedy is not simple. Please run the program using defferent input file. " << std::endl;
//         return ERROR;
//     }
//     else
//         std::cout << "Polygon after greedy is simple" << std::endl;

//     // Compute the area of the convexHull of the point set given
//     Polygon_2 convexHull;
//     CGAL::convex_hull_2(polygon.begin(), polygon.end(), std::back_inserter(convexHull));

//     double initial_area = std::abs(A.area());
//     double initial_ratio = std::abs(A.area()) / std::abs(convexHull.area());

//     // PROGRAM STARTS HERE

//     constructionTime = 0;

//     if (algorithm == "local_search")
//     {
//         int deltaArea = 100;                                                        ///?????????????????????????????????????????????????????
//         if(!isMaximization)
//             deltaArea = -100;
//         local_search(A, L, threshold, deltaArea, isMaximization, constructionTime,999999); // we use L(input) as k
//     }
//     else if (algorithm == "simulated_annealing")
//     {
//         bool isLocalStep;
//         if (annealing == "local")
//             isLocalStep = true;
//         else
//             isLocalStep = false;

//         if (annealing == "local" || annealing == "global")
//         {
//             minimization_algorithm(A, isLocalStep, L, isMaximization, constructionTime);
//         }
//         else
//         {
//             // Subdivision

//             // For the subdivision algorithm we couldn't make sure that the
//             // critical edges after the greedy algorithm were intact
//             // and therefore we couldn't use the subdivision algorithm.

//             // However, the algorithm's helper functions are implemented
//             // For example, void splitIntoSubsets(Polygon_2& pointset, std::vector<Polygon_2>& subsets)
//             // works as intended and can be used to split the initial pointset into subsets
//             // keeping the monotonicity of the leftmost and rightmost points of each subset.
//         }
//     }
//     else if (algorithm == "ant_colony")
//     {
//         std::cout << "Ant colony algorithm hasn't been implemented. Choose local_search or simulated_annealing. " << std::endl;
//         return ERROR;
//     }

//     if (!A.is_simple())
//         std::cout << "Polygon after optimization is not simple" << std::endl;
//     else
//         std::cout << "Polygon affter optimization is  simple" << std::endl;

//     // Write the necessary information to the output file
//     writeToOutput(A, initial_area, initial_ratio, polygonization, constructionTime, outputFile, algorithm, false);
//     std::cout << "Done!\nOutput written to " << outputFile << std::endl;

//     return 0;
// }

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


std::vector<Score> calculateScores(std::vector<std::vector<double>> scoresOfSize);

int main(int argc, char **argv)
{
    std::string inputDirectory = argv[2];
    std::string inputFile;
    std::string outputFile = argv[4];

    // Read the input directory's files
    std::vector<File> files;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(inputDirectory.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_name[0] != '.')
            {
                File file;
                file.name = ent->d_name;
                files.push_back(file);

            }
        }
        closedir(dir);
    }
    else
    {
        std::cout << "Could not open directory " << inputDirectory << std::endl;
        return ERROR;
    }


    // Sort the files by number
    std::sort(files.begin(), files.end(), [](File& a, File& b) {
        std::string sizeA = a.name;
        sizeA.erase(0, sizeA.find_last_of("-") + 1);
        sizeA.erase(sizeA.find("."), sizeA.length());

        std::string sizeB = b.name;
        sizeB.erase(0, sizeB.find_last_of("-") + 1);
        sizeB.erase(sizeB.find("."), sizeB.length());

        a.size = std::stoi(sizeA);
        b.size = std::stoi(sizeB);

        return a.size < b.size;
    });

    // Keep all files of the same size in a vector
    std::vector<std::vector<File>> filesBySize;
    std::vector<File> temp;
    int size = files[0].size;
    for(auto file : files)
    {
        if(file.size == size)
        {
            temp.push_back(file);
        }
        else
        {
            filesBySize.push_back(temp);
            temp.clear();
            temp.push_back(file);
            size = file.size;
        }
    }
    // Add the last vector of files
    filesBySize.push_back(temp);

    // Print the files by size
    for(auto files : filesBySize)
    {
        std::cout << "\t" << files[0].size << std::endl;
        for(auto file : files)
        {
            file.print();
        }
        std::cout << std::endl;
    }


    // Read the first file in the directory

    for( auto files : filesBySize)
    {
        std::vector<std::vector<double>> scoresOfSize;


        for(auto file : files)
        {
            inputFile = inputDirectory + "/" + file.name;
            std::cout << "Reading file " << inputFile << std::endl;

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

            // Sort points by x coordinate smallest to largest
            std::sort(polygon.begin(), polygon.end(), [](Point_2 p1, Point_2 p2)
                        { return p1.x() < p2.x(); });

            // Find the convex hull of the point set given
            Polygon_2 convexHull;
            CGAL::convex_hull_2(polygon.begin(), polygon.end(), std::back_inserter(convexHull));
            double ratio;

            Polygon_2 A;
            int constructionTime = 0;
            int cutOff = cutOffmsecs(polygon.size());
            int remainingTime;
            std::vector<double> scoresOfCurrFile;

            // For all files of the same size
            // for()

            // Algorithm 1: Incremental & Local Search

            // Maximize the area
            remainingTime = cutOff;
            A = incrementalAlgorithm(polygon, 3, constructionTime, false);
            remainingTime -= constructionTime;
            if(local_search(A, 3, 10, 100, true, constructionTime, remainingTime) == false)
            {
                std::cout << "Time limit reached: Local Search max" << std::endl;
            }
            ratio = std::abs(A.area()) / std::abs(convexHull.area());
            scoresOfCurrFile.push_back(ratio);
            writeToOutput(A, 5, ratio, "polygonization", constructionTime, outputFile, "algorithm", false);



            // Minimize the area
            remainingTime = cutOff;
            A = incrementalAlgorithm(polygon, 2, constructionTime, false);
            remainingTime -= constructionTime;
            if(local_search(A, 3, -10, -100, false, constructionTime, remainingTime) == false)
            {
                std::cout << "Time limit reached: Local Search min" << std::endl;
            }
            ratio = std::abs(A.area()) / std::abs(convexHull.area());
            scoresOfCurrFile.push_back(ratio);
            writeToOutput(A, 5, ratio, "polygonization", constructionTime, outputFile, "algorithm", false);




            // Algorithm 2: Incremental & Simulated Annealing (Local Step)

            // Maximize the area
            remainingTime = cutOff;
            A = incrementalAlgorithm(polygon, 3, constructionTime, false);
            remainingTime -= constructionTime;
            if(minimization_algorithm(A, true, 8000, true, constructionTime, remainingTime) == ERROR)
            {
                std::cout << "Time limit reached: Simulated Annealing max" << std::endl;
            }
            ratio = std::abs(A.area()) / std::abs(convexHull.area());
            scoresOfCurrFile.push_back(ratio);
            writeToOutput(A, 5, ratio, "polygonization", constructionTime, outputFile, "algorithm", false);



            // Minimize the area
            remainingTime = cutOff;
            A = incrementalAlgorithm(polygon, 2, constructionTime, false);
            remainingTime -= constructionTime;
            if(minimization_algorithm(A, true, 8000, false, constructionTime, remainingTime) == ERROR)
            {
                std::cout << "Time limit reached: Simulated Annealing min" << std::endl;
            }
            ratio = std::abs(A.area()) / std::abs(convexHull.area());
            scoresOfCurrFile.push_back(ratio);
            writeToOutput(A, 5, ratio, "polygonization", constructionTime, outputFile, "algorithm", false);




            // Algorithm 3: Incremental & Simulated Annealing (Global Step)

            // Maximize the area
            remainingTime = cutOff;
            A = incrementalAlgorithm(polygon, 3, constructionTime, false);
            remainingTime -= constructionTime;
            if(minimization_algorithm(A, false, 5000, true, constructionTime, remainingTime) == ERROR)
            {
                std::cout << "Time limit reached: Simulated Annealing max" << std::endl;
            }
            ratio = std::abs(A.area()) / std::abs(convexHull.area());
            scoresOfCurrFile.push_back(ratio);
            writeToOutput(A, 5, ratio, "polygonization", constructionTime, outputFile, "algorithm", false);



            // Minimize the area
            remainingTime = cutOff;
            A = incrementalAlgorithm(polygon, 2, constructionTime, false);
            remainingTime -= constructionTime;
            if(minimization_algorithm(A, false, 5000, false, constructionTime, remainingTime) == ERROR)
            {
                std::cout << "Time limit reached: Simulated Annealing min" << std::endl;
            }
            ratio = std::abs(A.area()) / std::abs(convexHull.area());
            scoresOfCurrFile.push_back(ratio);
            writeToOutput(A, 5, ratio, "polygonization", constructionTime, outputFile, "algorithm", false);




            // Algorithm 4: Convex Hull & Local Search

            // Maximize the area
            remainingTime = cutOff;
            A = convexHullAlgorithm(polygon, 3, constructionTime);
            remainingTime -= constructionTime;
            if(local_search(A, 3, 10, 100, true, constructionTime, remainingTime) == false)
            {
                std::cout << "Time limit reached: Local Search max" << std::endl;
            }
            ratio = std::abs(A.area()) / std::abs(convexHull.area());
            scoresOfCurrFile.push_back(ratio);
            writeToOutput(A, 5, ratio, "polygonization", constructionTime, outputFile, "algorithm", false);



            // Minimize the area
            remainingTime = cutOff;
            A = convexHullAlgorithm(polygon, 2, constructionTime);
            remainingTime -= constructionTime;
            if(local_search(A, 3, -10, -100, false, constructionTime, remainingTime) == false)
            {
                std::cout << "Time limit reached: Local Search min" << std::endl;
            }
            ratio = std::abs(A.area()) / std::abs(convexHull.area());
            scoresOfCurrFile.push_back(ratio);
            writeToOutput(A, 5, ratio, "polygonization", constructionTime, outputFile, "algorithm", false);




            // Algorithm 5: Convex Hull & Simulated Annealing (Local Step)

            // Maximize the area
            remainingTime = cutOff;
            A = convexHullAlgorithm(polygon, 3, constructionTime);
            remainingTime -= constructionTime;
            if(minimization_algorithm(A, true, 8000, true, constructionTime, remainingTime) == ERROR)
            {
                std::cout << "Time limit reached: Simulated Annealing max" << std::endl;
            }
            ratio = std::abs(A.area()) / std::abs(convexHull.area());
            scoresOfCurrFile.push_back(ratio);
            writeToOutput(A, 5, ratio, "polygonization", constructionTime, outputFile, "algorithm", false);



            // Minimize the area
            remainingTime = cutOff;
            A = convexHullAlgorithm(polygon, 2, constructionTime);
            remainingTime -= constructionTime;
            if(minimization_algorithm(A, true, 8000, false, constructionTime, remainingTime) == ERROR)
            {
                std::cout << "Time limit reached: Simulated Annealing min" << std::endl;
            }
            ratio = std::abs(A.area()) / std::abs(convexHull.area());
            scoresOfCurrFile.push_back(ratio);
            writeToOutput(A, 5, ratio, "polygonization", constructionTime, outputFile, "algorithm", false);




            // Algorithm 6: Convex Hull & Simulated Annealing (Global Step)

            // Maximize the area
            remainingTime = cutOff;
            A = convexHullAlgorithm(polygon, 3, constructionTime);
            remainingTime -= constructionTime;
            if(minimization_algorithm(A, false, 5000, true, constructionTime, remainingTime) == ERROR)
            {
                std::cout << "Time limit reached: Simulated Annealing max" << std::endl;
            }
            ratio = std::abs(A.area()) / std::abs(convexHull.area());
            scoresOfCurrFile.push_back(ratio);
            writeToOutput(A, 5, ratio, "polygonization", constructionTime, outputFile, "algorithm", false);



            // Minimize the area
            remainingTime = cutOff;
            A = convexHullAlgorithm(polygon, 2, constructionTime);
            remainingTime -= constructionTime;
            if(minimization_algorithm(A, false, 5000, false, constructionTime, remainingTime) == ERROR)
            {
                std::cout << "Time limit reached: Simulated Annealing min" << std::endl;
            }
            ratio = std::abs(A.area()) / std::abs(convexHull.area());
            scoresOfCurrFile.push_back(ratio);
            writeToOutput(A, 5, ratio, "polygonization", constructionTime, outputFile, "algorithm", false);
    
            scoresOfSize.push_back(scoresOfCurrFile);

            for(auto score : scoresOfCurrFile)
            {
                std::cout << "Score: " << score << std::endl;
            }
        }
        // Calculate metrics
        std::vector<Score> scores = calculateScores(scoresOfSize);
        for (auto score : scores)
        {
            std::cout << "--------------------------" << std::endl;
            score.print();
            std::cout << "--------------------------" << std::endl;
        }
        
        // Write results of the current size to the output file
    }


    return 0;
}

std::vector<Score> calculateScores(std::vector<std::vector<double>> scoresOfSize)
{
    std::vector<Score> scores;
    // Push back 6 scores
    for(int i = 0; i < scoresOfSize[0].size()/2; i++)
    {
        scores.push_back(Score());
    }

    // Create a vector sum of doubles with inital value 0
    double sum[scoresOfSize[0].size()];
    double boundMin[scoresOfSize[0].size()];
    double boundMax[scoresOfSize[0].size()];
    for(int i = 0; i < scoresOfSize[0].size(); i++)
    {
        sum[i] = 0.0;
        boundMin[i] = 0.0;
        boundMax[i] = 1.0;
    }

    // std::cout << "--------------------------boundMax:--------------- " << std::endl;
    // for(int i = 0; i < scoresOfSize[0].size(); i++)
    // {
    //     std::cout << sum[i] << " ";
    // }
    // std::cout << "\n---------------------------------------------------" <<std::endl;

    for(int i = 0; i < scoresOfSize.size(); i++)
    {
        // For Max Results
        for(int j = 0; j < scoresOfSize[i].size(); j+=2)
        {
            sum[j] += scoresOfSize[i][j];
            if(scoresOfSize[i][j] < boundMax[j])
            {
                boundMax[j] = scoresOfSize[i][j];
            }
        }

        // For Min Results
        for(int j = 1; j < scoresOfSize[i].size(); j+=2)
        {
            sum[j] += scoresOfSize[i][j];
            if(scoresOfSize[i][j] > boundMin[j])
            {
                boundMin[j] = scoresOfSize[i][j];
            }
        }
    }

    for(int i = 0; i < scores.size(); i++)
    {
        scores[i].scoreMax = sum[i*2];
        scores[i].scoreMin = sum[i*2+1];

        scores[i].boundMax = boundMax[i*2];
        scores[i].boundMin = boundMin[i*2+1];
        scores[i].print();
    }
    
    // print boundMax array
    // std::cout << "--------------------------boundMax:--------------- " << std::endl;
    // for(int i = 0; i < scoresOfSize[0].size(); i++)
    // {
    //     std::cout << boundMax[i] << " ";
    // }
    // std::cout << "\n---------------------------------------------------" <<std::endl;
    return scores;
}
