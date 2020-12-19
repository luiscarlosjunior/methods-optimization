// SimulatedAnnealing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ctime>

#include <fstream>
#include <string>
#include <sstream>
#include <cmath>

class Solution
{
public:
    Solution();
    Solution(double _x, double _y);
    std::string toString() const;
    double value() const;
    Solution randomWalk(double maxStep) const;
private: 
    double x, y;
};

Solution::Solution()
{
    x = rand() % 10 - 5;
    y = rand() % 10 - 5;
}

Solution::Solution(double _x, double _y)
{
    x = _x;
    y = _y;
}

std::string Solution::toString() const
{
    std::ostringstream stream;
    stream << "x = " << x << " y = " << y << " z = " << value() << std::endl;
    return stream.str();
}

double Solution::value() const
{
    //Himmelblau
    //pow(a,b) = a^b 
    return pow((x * x) + y - 11, 2) + pow(x + (y * y) - 7, 2);
}

Solution Solution::randomWalk(double maxStep) const
{
    std::cout << "random walk " << this->toString() << std::endl;
    double step = rand() % (2 * (int)maxStep * 1000) / (double)1000;
    step = step - maxStep;
    double newX = x + step;

    step = rand() % (2 * (int)maxStep * 1000) / (double)1000;
    step = step - maxStep;
    double newY = y + step;

    Solution newSolution(newX, newY);
    return newSolution;
}


void simulatedAnnealing()
{
    const double kInitialTemperature = 100;
    const unsigned kTimesToDecreaseTemperature = 100;//100

    const double kStep = 1;
    const unsigned kNeighbours = 10;//10
    const double kDecreaseAmount = 0.85;

    unsigned yes = 0, no = 0;

    double probTemp = 0;

    srand(time(NULL));
    Solution currentSolution;//random

    std::ofstream csv("results.csv", std::ofstream::out);
    double currentTemperature = kInitialTemperature;
    for (unsigned i = 0; i < kTimesToDecreaseTemperature; i++)
    {
        std::cout << "--------------------Temperature = " << currentTemperature << std::endl;
        std::cout << "Solution = " << currentSolution.toString() << std::endl;
        for (unsigned j = 0; j < kNeighbours; j++)
        {
            Solution candidate = currentSolution.randomWalk(kStep);
            std::cout << "Candidate = " << candidate.toString() << std::endl;
            if (candidate.value() < currentSolution.value())
            {
                std::cout << "Found a better solution" << std::endl;
                currentSolution = candidate;
            }
            else
            {
                double frac = candidate.value() - currentSolution.value();
                double probability = 1 / exp(frac/currentTemperature);

                std::cout << "Probability = " << probability << std::endl;
                probTemp = probability;

                double r = (double)(rand() % 1000) / 1000;

                if (r < probability)
                {
                    std::cout << "Taking the worse solution" << std::endl;
                    currentSolution = candidate;
                    yes++;
                }
                else
                {
                    std::cout << "NOT taking the worse solution" << std::endl;
                    no++;
                }
            }
        }
        std::cout << " Worse solution? Yes " << yes << " No " << no << std::endl;
        currentTemperature = currentTemperature * kDecreaseAmount;
        csv << currentSolution.value() << "\t" << yes << "\t" << no << "\t" << probTemp << '\n';
    }
    std::cout << "Finally, solution = " << currentSolution.toString() << std::endl;
    csv.close();
}

int main()
{
    std::cout << "Hello World!\n";
    simulatedAnnealing();
    return 0;
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
