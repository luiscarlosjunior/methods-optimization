// DifferentialEvolution.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <set>
#include <algorithm>
#include <functional>
#include <cassert>
#include <cmath>


double randomDouble(double low, double high)
{
	double d = (double)rand() / RAND_MAX;
	return low + d * (high - low);
}

std::set<int> randomDistinctNumbers(int upperLimit, size_t howManyNumbers, int except)
{
	std::set<int> numbers;
	while (numbers.size() < howManyNumbers)
	{
		int r = rand() % upperLimit;
		if (r != except)
			numbers.insert(r);
	}
	return numbers;
}
//----

class Solution
{
public:
	Solution() = default;
	Solution(size_t vectorSize, double low, double high);
	Solution(std::vector<double> v);
	std::string toString() const;
	double value() const;
	std::vector<double> getVector() const { return mVector; }
private:
	std::vector<double> mVector;
};
Solution::Solution(size_t vectorSize, double low, double high)
{
	mVector.resize(vectorSize); //vector can't be empty
	std::generate_n(mVector.begin(), vectorSize, [low, high]() {return randomDouble(low, high); });
}
Solution::Solution(std::vector<double> v) : mVector{ v }
{
}
std::string Solution::toString() const
{
	std::ostringstream stream;
	stream << '[';
	for (double d : mVector)
	{
		stream << d << ' ';
	}
	stream << "] v = " << value() << std::endl;
	return stream.str();
}

double Solution::value() const
{
	double x1 = mVector[0];
	double x2 = mVector[1];
	double x3 = mVector[2];

	return 100 * pow(x2 - x1 * x1, 2) + pow(1 - x1, 2) + 100 * pow(x3 - x3 * x2, 2) + pow(1 - x2, 2);
}


std::vector<double> getDonorVector(Solution const& s1, Solution const& s2, Solution const& s3, double F)
{
	std::vector<double> dv;
	size_t size = s1.getVector().size();
	for (size_t i = 0; i < size; i++)
	{
		dv.push_back(s1.getVector()[i] + F * (s2.getVector()[i] - s3.getVector()[i]));
	}
	return dv;
}

std::vector<double> getTrialVector(double crossoverProbability, 
	std::vector<double> const& original, 
	std::vector<double> const& donor, 
	double low, 
	double high)
{
	std::vector<double> trial;
	int stable = rand() % original.size();
	for (size_t i = 0; i < original.size(); i++)
	{
		if (i == stable)
		{
			trial.push_back(donor[i]);
		}
		else
		{
			double r = rand() / RAND_MAX;
			if (r < crossoverProbability)
			{
				trial.push_back(donor[i]);
			}
			else
			{
				trial.push_back(original[i]);
			}
		}
	}

	std::replace_if(trial.begin(), trial.end(), std::bind(std::less<double>(), std::placeholders::_1, low), low);
	std::replace_if(trial.begin(), trial.end(), std::bind(std::greater<double>(), std::placeholders::_1, high), high);
	return trial;
}

void differentialEvolution(size_t numberOfSolutions, 
	size_t elementsInVector, 
	double lowBound, 
	double highBound, 
	double F, 
	int iterations, 
	double crossoverProbability)
{
	assert(numberOfSolutions >= 4);
	std::vector<Solution> solutions;
	solutions.resize(numberOfSolutions);
	std::generate_n(solutions.begin(), numberOfSolutions, [elementsInVector, lowBound, highBound]() {
		Solution s(elementsInVector, lowBound, highBound);
		return s;});
	for (int it = 0; it < iterations; it++)
	{
		std::cout << "---------" << std::endl;
		std::vector<Solution> trialSolutions;
		for (size_t i = 0; i < numberOfSolutions; i++)
		{
			//get donor vector:
			std::set<int> indexes = randomDistinctNumbers(numberOfSolutions, 3, i);
			std::vector<Solution> donorComponents;
			for (int index : indexes)
			{
				donorComponents.push_back(solutions[index]);
			}

			std::vector<double> donorVector = getDonorVector(donorComponents[0], donorComponents[1], donorComponents[2], F);

			//trial vector:
			std::vector<double> trialVector = getTrialVector(crossoverProbability, solutions[i].getVector(), donorVector, lowBound, highBound);
			Solution trialSolution(trialVector);
			trialSolutions.push_back(trialSolution);
		}

		for (size_t i = 0; i < numberOfSolutions; i++)
		{
			if (trialSolutions[i].value() < solutions[i].value())
			{
				solutions[i] = trialSolutions[i];
			}

			std::cout << solutions[i].toString() << std::endl;
		}
	}

}

int main()
{
	srand(time(NULL)); 
	
	const size_t kNumberOfSolutions = 15;
	const size_t kElementsInVector = 3;
	const double kLowBound = -2;
	const double kHighBound = 2;
	const double F = 0.98;
	const size_t kNumberOfIterations = 1000;
	const double kCrossoverProbability = 0.8;

	differentialEvolution(kNumberOfSolutions, 
		kElementsInVector, 
		kLowBound, 
		kHighBound, 
		F, 
		kNumberOfIterations, 
		kCrossoverProbability);

	std::vector<double> ones{ 1,1,1 };
	Solution s(ones);
	std::cout << "Minimal solution : " << s.toString() << std::endl;

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
