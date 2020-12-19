using System;
using System.Collections.Generic;

namespace GeneticAlgorithm
{
    public class GeneticAlgorithm<T>
    {
        public List<Chromosome<T>> Population { get; private set; }
        public int Generation { get; private set; }
        public float BestFitness { get; private set; }
        //public T[] BestGenes { get; private set; }
        public List<T> BestGenes { get; private set; }

        //public IReadOnlyCollection<T> BestGenes => BestGenes;

        public int Elitism { get => elitism; set => elitism = value; }

        private int elitism;
        private readonly float MutationRate;

        private List<Chromosome<T>> newPopulation;
        private readonly Random random;
        private float fitnessSum;
        private readonly int dnaSize;
        private readonly Func<T> getRandomGene;
        private readonly Func<int, float> fitnessFunction;

        public GeneticAlgorithm(int populationSize, int dnaSize, Random random, Func<T> getRandomGene, Func<int, float> fitnessFunction, int elitism, float mutationRate = 0.01f)
        {
            Generation = 1;
            Elitism = elitism;
            MutationRate = mutationRate;
            Population = new List<Chromosome<T>>(populationSize);
            newPopulation = new List<Chromosome<T>>(populationSize);

            this.random = random;
            this.dnaSize = dnaSize;
            this.getRandomGene = getRandomGene;
            this.fitnessFunction = fitnessFunction;

            BestGenes = new T[dnaSize];

            for (int i = 0; i < populationSize; i++)
            {
                Population.Add(new Chromosome<T>(dnaSize, random, getRandomGene, fitnessFunction, shouldInitGenes: true));
            }
        }

        public void NewGeneration(int numNewDNA = 0, bool crossoverNewDNA = false)
        {
            int finalCount = Population.Count + numNewDNA;

            if (finalCount <= 0)
            {
                return;
            }

            if (Population.Count > 0)
            {
                CalculateFitness();
                Population.Sort(CompareDNA);
            }

            newPopulation.Clear();

            for (int i = 0; i < Population.Count; i++)
            {
                if (i < Elitism && i < Population.Count)
                {
                    newPopulation.Add(Population[i]);
                }
                else if (i < Population.Count || crossoverNewDNA)
                {
                    Chromosome<T> parent1 = ChooseParent();
                    Chromosome<T> parent2 = ChooseParent();

                    Chromosome<T> child = parent1.Crossover(parent2);

                    child.Mutate(MutationRate);
                }
                else
                {
                    newPopulation.Add(new Chromosome<T>(dnaSize, random, getRandomGene, fitnessFunction, shouldInitGenes: true));
                }
            }

            List<Chromosome<T>> tmpList = Population;
            Population = newPopulation;
            newPopulation = tmpList;

            ++Generation;
        }

        private int CompareDNA(Chromosome<T> a, Chromosome<T> b)
        {
            if (a.Fitness > b.Fitness)
            {
                return -1;
            }
            else if (a.Fitness < b.Fitness)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }

        private void CalculateFitness()
        {
            fitnessSum = 0;
            Chromosome<T> best = Population[0];

            for (int i = 0; i < Population.Count; i++)
            {
                fitnessSum += Population[i].CalculateFitness(i);

                if (Population[i].Fitness > best.Fitness)
                {
                    best = Population[i];
                }
            }

            BestFitness = best.Fitness;
            best.Genes.CopyTo(BestGenes, 0);
        }

        private Chromosome<T> ChooseParent()
        {
            double randomNumber = random.NextDouble() * fitnessSum;

            for (int i = 0; i < Population.Count; i++)
            {
                if (randomNumber < Population[i].Fitness)
                {
                    return Population[i];
                }

                randomNumber -= Population[i].Fitness;
            }
            return null;
        }
    }
}