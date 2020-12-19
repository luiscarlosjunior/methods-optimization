// AntColonyOptimization.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <set>
#include <numeric>
#include <vector>
#include <time.h>
#include <cmath>

const int kCities = 6;
const int kInfinite = INT_MAX;

void addEdge(int graph[][kCities], int from, int to, int cost)
{
    if (to == from)
    {
        throw std::runtime_error("Source and destination city must be the same");
    }
    graph[from][to] = graph[to][from] = cost;
}

template<class T>
void printMatrix(T graph[][kCities])
{
    for (int i = 0; i < kCities; i++)
    {
        for (int j = 0; j < kCities; j++)
        {
            std::cout << graph[i][j] << '\t';
        }
        std::cout << '\n';
    }
}

int selectNext(int currentNode,
    int graph[][kCities],
    double pheromone[][kCities],
    std::vector<int> const& travelledNodes,
    double alpha,
    double beta)
{
    std::cout << "Current node = " << currentNode << '\n';
    std::vector<int> possibleNodes;
    std::vector<double> probabilities;
    for (int i = 0; i < kCities; i++)
    {
        if (std::find(travelledNodes.begin(), travelledNodes.end(), i) == travelledNodes.end())
        {
            possibleNodes.push_back(i);
            probabilities.push_back(pow(pheromone[currentNode][i], alpha) *
                pow(1 / (double)graph[currentNode][i], beta));
        }
    }
    double denominator = 0;
    for (double p : probabilities)
    {
        denominator += p;
    }

    for (int i = 0; i < probabilities.size(); i++)
    {
        probabilities[i] /= denominator;
        std::cout << "Probability to go to city" << possibleNodes[i] << " = " << probabilities[i] << '\n';
    }

    double r = (double)rand() / RAND_MAX;
    int selectedNode = -1;

    double probabilitySum = 0;
    std::cout << "r = " << r << '\n';
    for (int i = 0; i < probabilities.size(); i++)
    {
        double p = probabilities[i];
        std::cout << "Probability sum = " << probabilitySum << " p = " << p << '\n';
        if (r >= probabilitySum && r <= probabilitySum + p)
        {
            selectedNode = possibleNodes[i];
            break;
        }
        probabilitySum += p;
    }
    return selectedNode;
}

int main()
{
    srand(time(NULL));
    //----parameters
    const int kAnts = 100;
    const double alpha = 1, beta = 1, vaporization = 0.75;
    const int startNode = 0;
    //----

    int graph[kCities][kCities];
    double pheromone[kCities][kCities];

    for (int i = 0; i < kCities; i++)
        for (int j = 0; j < kCities; j++)
        {
            if (i == j)
                graph[i][j] = 0; //the distance from a city to itself is always 0
            else
                graph[i][j] = kInfinite;
        }

    addEdge(graph, 0, 1, 14);
    addEdge(graph, 0, 2, 32);
    addEdge(graph, 0, 3, 17);
    addEdge(graph, 0, 4, 52);
    addEdge(graph, 0, 5, 42);

    addEdge(graph, 1, 2, 36);
    addEdge(graph, 1, 3, 14);
    addEdge(graph, 1, 4, 25);
    addEdge(graph, 1, 5, 43);

    addEdge(graph, 2, 3, 40);
    addEdge(graph, 2, 4, 20);
    addEdge(graph, 2, 5, 23);

    addEdge(graph, 3, 4, 25);
    addEdge(graph, 3, 5, 14);

    addEdge(graph, 4, 5, 24);
    std::cout << "Cities graph:\n";
    printMatrix<int>(graph);

    std::vector<int> bestPath;
    int bestLength = INT_MAX;

    for (int i = 0; i < kAnts; i++)
    {
        std::cout << "Ant " << i << '\n';
        std::vector<int> travelledNodes;
        int currentNode = startNode;
        int totalLength = 0;

        while (travelledNodes.size() < kCities)
        {
            travelledNodes.push_back(currentNode);
            int nextNode = selectNext(currentNode, graph, pheromone, travelledNodes, alpha, beta);
            std::cout << "Going to node " << nextNode << '\n';

            if (-1 == nextNode)
            {
                break;
            }
            totalLength += graph[currentNode][nextNode];

            //update pheromone matrix:
            double extraPheromone = 1 / (graph[currentNode][nextNode]);
            for (int iVapor = 0; iVapor < kCities; iVapor++)
                for (int jVapor = 0; jVapor < kCities; jVapor++)
                {
                    pheromone[iVapor][jVapor] *= vaporization;
                }
            pheromone[currentNode][nextNode] += extraPheromone;
            pheromone[nextNode][currentNode] += extraPheromone;

            currentNode = nextNode;


        }

        std::cout << "Path found by the ant:\n";
        for (int node : travelledNodes)
        {
            std::cout << node << ' ';
        }
        std::cout << " length = " << totalLength << '\n';
        if (totalLength < bestLength)
        {
            bestLength = totalLength;
            bestPath = travelledNodes;
        }
        //std::cout << "Pheromone\n";
        //printMatrix<double>(pheromone);
    }

    std::cout << "Best path found by the ants:\n";
    for (int node : bestPath)
    {
        std::cout << node << ' ';
    }
    std::cout << " length = " << bestLength << '\n';
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
