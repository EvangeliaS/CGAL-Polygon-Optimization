#ifndef __SHARED_LIB_CPP__
#define __SHARED_LIB_CPP__

#include "sharedLib.h"


void printPolygon(Polygon_2 points)
{
    for (Point_2 p : points)
        std::cout << p << std::endl;
}

int findPolygonPoint(Polygon_2 polygon, Point_2 point)
{
    int index = -1;
    for (int i = 0; i < polygon.size(); i++)
    {
        if (polygon[i] == point)
        {
            index = i;
            return index;
        }
    }
    return index;
}

std::vector<std::string> split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

void writeToOutput( std::string outputFile, std::vector<Score>& scores, int size, bool overwrite)
{
    if(overwrite)
    {
        std::ofstream output(outputFile);
       
        if (!output)
        {
            std::cout << "Could not open file " << outputFile << std::endl;
            perror("Error on opening output file");
            exit(ERROR);
        }

        // Write the following to output file
        //      || <Incremental & Local Search>                  || <Incremental & Sim. Annealing (Local Step)>   || <Incremental & Sim. Annealing (Global Step)>   || <Convex Hull & Local Search>                   || <Convex Hull & Sim. Annealing (Local Step)>    || <Convex Hull & Sim. Annealing (Global Step)>                                          
        // Size || min score | max score | min bound | max bound || min score | max score | min bound | max bound || min score | max score | min bound | max bound  || min score | max score | min bound | max bound  || min score | max score | min bound | max bound  || min score | max score | min bound | max bound
        output << "\t ||         <Incremental & Local Search>          ||  <Incremental & Sim. Annealing (Local Step)>  ||  <Incremental & Sim. Annealing (Global Step)>  ||          <Convex Hull & Local Search>          ||   <Convex Hull & Sim. Annealing (Local Step)>  ||  <Convex Hull & Sim. Annealing (Global Step)> " << std::endl;
        output << "Size || min score | max score | min bound | max bound || min score | max score | min bound | max bound || min score | max score | min bound | max bound  || min score | max score | min bound | max bound  || min score | max score | min bound | max bound  || min score | max score | min bound | max bound" << std::endl;
        // Close the output file
        output.close();
    }
    else
    {
        std::ofstream output(outputFile, std::ios::app);
        
        // std::ofstream output(outputFile);
        
        if (!output)
        {
            std::cout << "Could not open file " << outputFile << std::endl;
            perror("Error on opening output file");
            exit(ERROR);
        }
        
        output << std::fixed;
        output << std::setprecision(5);
        output << size << "\t ||  " << scores[0].scoreMin << "  |  " << scores[0].scoreMax << "  |  " << scores[0].boundMin << "  |  " << scores[0].boundMax << "  ||  " << scores[1].scoreMin << "  |  " << scores[1].scoreMax << "  |  " << scores[1].boundMin << "  |  " << scores[1].boundMax << "  ||  " << scores[2].scoreMin << "  |  " << scores[2].scoreMax << "  |  " << scores[2].boundMin << "  |  " << scores[2].boundMax << "   ||  " << scores[3].scoreMin << "  |  " << scores[3].scoreMax << "  |  " << scores[3].boundMin << "  |  " << scores[3].boundMax << "   ||  " << scores[4].scoreMin << "  |  " << scores[4].scoreMax << "  |  " << scores[4].boundMin << "  |  " << scores[4].boundMax << "   ||  " << scores[5].scoreMin << "  |  " << scores[5].scoreMax << "  |  " << scores[5].boundMin << "  |  " << scores[5].boundMax << std::endl;
        // Close the output file
        output.close();
    }
}

int cutOffmsecs(int numPoints)
{
    // f(n) = 500*n msec
    return  500 * numPoints;
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
    }

    return scores;
}

#endif // __SHARED_LIB_CPP__