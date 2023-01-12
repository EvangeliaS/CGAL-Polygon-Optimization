#include <dirent.h>

#include "simulated_annealing.h"
#include "local_search.h"




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


    // Write the header of the output file
    std::vector<Score> sc;
    writeToOutput(outputFile, sc , 0, true);

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


            // Algorithm 1: Incremental & Local Search

            // Maximize the area
            remainingTime = cutOff;
            A = incrementalAlgorithm(polygon, 3, constructionTime, false);
            remainingTime -= constructionTime;
            if(local_search(A, 2, 10, 100, true, constructionTime, remainingTime) == false)
            {
                std::cout << "Time limit reached: Local Search max" << std::endl;
                scoresOfCurrFile.push_back(0.0);
            }
            else
            {
                ratio = std::abs(A.area()) / std::abs(convexHull.area());
                scoresOfCurrFile.push_back(ratio);
            }

            // Minimize the area
            remainingTime = cutOff;
            A = incrementalAlgorithm(polygon, 2, constructionTime, false);
            remainingTime -= constructionTime;
            if(local_search(A, 2, -10, -100, false, constructionTime, remainingTime) == false)
            {
                std::cout << "Time limit reached: Local Search min" << std::endl;
                scoresOfCurrFile.push_back(1.0);
            }
            else
            {
                ratio = std::abs(A.area()) / std::abs(convexHull.area());
                scoresOfCurrFile.push_back(ratio);
            }


            // Algorithm 2: Incremental & Simulated Annealing (Local Step)

            // Maximize the area
            remainingTime = cutOff;
            A = incrementalAlgorithm(polygon, 3, constructionTime, false);
            remainingTime -= constructionTime;
            if(minimization_algorithm(A, true, 8000, true, constructionTime, remainingTime) == ERROR)
            {
                std::cout << "Time limit reached: Simulated Annealing max" << std::endl;
                scoresOfCurrFile.push_back(0.0);
            }
            else
            {
                ratio = std::abs(A.area()) / std::abs(convexHull.area());
                scoresOfCurrFile.push_back(ratio);
            }

            // Minimize the area
            remainingTime = cutOff;
            A = incrementalAlgorithm(polygon, 2, constructionTime, false);
            remainingTime -= constructionTime;
            if(minimization_algorithm(A, true, 8000, false, constructionTime, remainingTime) == ERROR)
            {
                std::cout << "Time limit reached: Simulated Annealing min" << std::endl;
                scoresOfCurrFile.push_back(1.0);
            }
            else
            {
                 ratio = std::abs(A.area()) / std::abs(convexHull.area());
                scoresOfCurrFile.push_back(ratio);
            }


            // Algorithm 3: Incremental & Simulated Annealing (Global Step)

            // Maximize the area
            remainingTime = cutOff;
            A = incrementalAlgorithm(polygon, 3, constructionTime, false);
            remainingTime -= constructionTime;
            if(minimization_algorithm(A, false, 5000, true, constructionTime, remainingTime) == ERROR)
            {
                std::cout << "Time limit reached: Simulated Annealing max" << std::endl;
                scoresOfCurrFile.push_back(0.0);
            }
            else
            {
                ratio = std::abs(A.area()) / std::abs(convexHull.area());
                scoresOfCurrFile.push_back(ratio);
            }

            // Minimize the area
            remainingTime = cutOff;
            A = incrementalAlgorithm(polygon, 2, constructionTime, false);
            remainingTime -= constructionTime;
            if(minimization_algorithm(A, false, 5000, false, constructionTime, remainingTime) == ERROR)
            {
                std::cout << "Time limit reached: Simulated Annealing min" << std::endl;
                scoresOfCurrFile.push_back(1.0);
            }
            else
            {
                ratio = std::abs(A.area()) / std::abs(convexHull.area());
                scoresOfCurrFile.push_back(ratio);
            }


            // Algorithm 4: Convex Hull & Local Search

            // Maximize the area
            remainingTime = cutOff;
            A = convexHullAlgorithm(polygon, 3, constructionTime);
            remainingTime -= constructionTime;
            if(local_search(A, 2, 10, 100, true, constructionTime, remainingTime) == false)
            {
                std::cout << "Time limit reached: Local Search max" << std::endl;
                scoresOfCurrFile.push_back(0.0);
            }
            else
            {
                ratio = std::abs(A.area()) / std::abs(convexHull.area());
                scoresOfCurrFile.push_back(ratio);
            }

            // Minimize the area
            remainingTime = cutOff;
            A = convexHullAlgorithm(polygon, 2, constructionTime);
            remainingTime -= constructionTime;
            if(local_search(A, 2, -10, -100, false, constructionTime, remainingTime) == false)
            {
                std::cout << "Time limit reached: Local Search min" << std::endl;
                scoresOfCurrFile.push_back(1.0);
            }
            else
            {
                ratio = std::abs(A.area()) / std::abs(convexHull.area());
                scoresOfCurrFile.push_back(ratio);
            }


            // Algorithm 5: Convex Hull & Simulated Annealing (Local Step)

            // Maximize the area
            remainingTime = cutOff;
            A = convexHullAlgorithm(polygon, 3, constructionTime);
            remainingTime -= constructionTime;
            if(minimization_algorithm(A, true, 8000, true, constructionTime, remainingTime) == ERROR)
            {
                std::cout << "Time limit reached: Simulated Annealing max" << std::endl;
                scoresOfCurrFile.push_back(0.0);
            }
            else
            {
                ratio = std::abs(A.area()) / std::abs(convexHull.area());
                scoresOfCurrFile.push_back(ratio);
            }

            // Minimize the area
            remainingTime = cutOff;
            A = convexHullAlgorithm(polygon, 2, constructionTime);
            remainingTime -= constructionTime;
            if(minimization_algorithm(A, true, 8000, false, constructionTime, remainingTime) == ERROR)
            {
                std::cout << "Time limit reached: Simulated Annealing min" << std::endl;
                scoresOfCurrFile.push_back(1.0);
            }
            else
            {
                ratio = std::abs(A.area()) / std::abs(convexHull.area());
                scoresOfCurrFile.push_back(ratio);
            }



            // Algorithm 6: Convex Hull & Simulated Annealing (Global Step)

            // Maximize the area
            remainingTime = cutOff;
            A = convexHullAlgorithm(polygon, 3, constructionTime);
            remainingTime -= constructionTime;
            if(minimization_algorithm(A, false, 5000, true, constructionTime, remainingTime) == ERROR)
            {
                std::cout << "Time limit reached: Simulated Annealing max" << std::endl;
                scoresOfCurrFile.push_back(0.0);
            }
            else
            {
                ratio = std::abs(A.area()) / std::abs(convexHull.area());
                scoresOfCurrFile.push_back(ratio);
            }

            // Minimize the area
            remainingTime = cutOff;
            A = convexHullAlgorithm(polygon, 2, constructionTime);
            remainingTime -= constructionTime;
            if(minimization_algorithm(A, false, 5000, false, constructionTime, remainingTime) == ERROR)
            {
                std::cout << "Time limit reached: Simulated Annealing min" << std::endl;
                scoresOfCurrFile.push_back(1.0);
            }
            else
            {
                ratio = std::abs(A.area()) / std::abs(convexHull.area());
                scoresOfCurrFile.push_back(ratio);
            }



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
        writeToOutput(outputFile, scores, files[0].size, false);

    }


    return 0;
}
