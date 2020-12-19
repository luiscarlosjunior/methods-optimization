// SimulatedAnnealing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ctime>

#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>

/*Knapsack problem:
 - given a set of items, each with a weight and a value
 - find what items you can fit into a knapsack, so that their total value is the highest
 - without exceeding the weight that the knapsack can carry
*/

class Item
{
public:
    Item(double value, double weight, bool isPresent);
    double getValue() const;
    double getWeight() const;
    bool isPresent() const;
    void setPresent(bool newValue);
    std::string toString() const;
private:
    double mValue, mWeight;
    bool mIsPresent;
};
Item::Item(double value, double weight, bool isPresent): mValue{value}, mWeight{weight}, mIsPresent{isPresent}
{
}

double Item::getValue() const
{
    return mValue;
}
double Item::getWeight() const
{
    return mWeight;
}

bool Item::isPresent() const
{
    return mIsPresent;
}

void Item::setPresent(bool newValue) { mIsPresent = newValue; }


std::string Item::toString() const
{
    std::ostringstream stream;
    stream << "w = " << mWeight << " v = " << mValue << " p = " << mIsPresent;
    return stream.str();
}

class Solution
{
public:
    Solution(std::vector<Item> items);
    std::string toString() const;
    double value() const;
    Solution randomWalk() const;
private:
    double getTotalWeight() const;
private: 
    std::vector<Item> mItems;
    static const unsigned kMaxWeight;//TODO: set it by user input 
};

const unsigned Solution::kMaxWeight = 100;


Solution::Solution(std::vector<Item> items): mItems{items}
{
}

std::string Solution::toString() const
{
    std::ostringstream stream;
    stream << "Items: ";
    for (Item const& i : mItems)
    {
        stream << i.toString() << " ";
    }
    stream << std::endl << " Total weight & value = " << getTotalWeight() << ' ' << value() << std::endl;

    return stream.str();
}


double Solution::getTotalWeight() const
{
    double weightSum{ 0 };
    for (Item const& i : mItems)
    {
        if(i.isPresent())
            weightSum += i.getWeight();
    }
    return weightSum;
}
double Solution::value() const
{
    double valueSum{ 0 };
    for (Item const& i : mItems)
    {
        if(i.isPresent())
            valueSum += i.getValue();
    }
    if (getTotalWeight() > kMaxWeight)
    {
        return -10; //invalid solution
    }
    return valueSum;
}
// 1. penalty
// 2. discard
// 3. repair

Solution Solution::randomWalk() const
{
    std::vector<Item> itemsCopy = this->mItems;
    unsigned index = rand() % itemsCopy.size();

    itemsCopy[index].setPresent(1 - itemsCopy[index].isPresent());

    Solution newSolution(itemsCopy);
    while (newSolution.value() < 0)
    {
        for (Item& i : itemsCopy)
        {
            if (i.isPresent())
            {
                i.setPresent(false);
                break;
            }
        }
        newSolution.mItems = itemsCopy;
    }

    return newSolution;
}

bool randomBool()
{
    if (rand() % 2)
    {
        return true;
    }
    return false;
}

void simulatedAnnealing()
{
    const double kInitialTemperature = 100;
    const unsigned kTimesToDecreaseTemperature = 100;//100

    const unsigned kNeighbours = 10;//10
    const double kDecreaseAmount = 0.85;

    unsigned yes = 0, no = 0;

    double probTemp = 0;

    srand(time(NULL));

    //generate random first solution
    std::vector<Item> items
    {
        {10, 9, randomBool()},
        {12, 29, randomBool()},
        {13, 2,  randomBool()},
        {20, 23, randomBool()},
        {5,  12, randomBool()},
        {22, 32, randomBool()},
        {4,  14, randomBool()}
    };
    Solution currentSolution(items);//random

    std::ofstream csv("results.csv", std::ofstream::out);
    double currentTemperature = kInitialTemperature;
    for (unsigned i = 0; i < kTimesToDecreaseTemperature; i++)
    {
        std::cout << std::endl << "-------------------------Temperature = " << currentTemperature << std::endl;
        std::cout << "Solution = " << currentSolution.toString() << std::endl;
        for (unsigned j = 0; j < kNeighbours; j++)
        {
            Solution candidate = currentSolution.randomWalk();
            std::cout << "Candidate = " << candidate.toString() << std::endl;
            if (candidate.value() > currentSolution.value())// notice inverted sign
            {
                std::cout << "Found a better solution" << std::endl;
                currentSolution = candidate;
            }
            else if(candidate.value()>0)
            {
                double frac =  - candidate.value() + currentSolution.value();//notice inverted sign
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
