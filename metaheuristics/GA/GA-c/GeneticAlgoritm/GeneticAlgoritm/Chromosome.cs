using System;

namespace GeneticAlgorithm
{
    public class Chromosome<T>
    {
        public T[] Genes { get; private set; }

        public float Fitness { get; private set; }

        private readonly Random random;
        private readonly Func<T> getRandomGene;
        private readonly Func<int, float> fitnessFunction;

        public Chromosome(int size, Random random, Func<T> getRandomGene, Func<int, float> fitnessFunction, bool shouldInitGenes = true)
        {
            Genes = new T[size];
            this.random = random;
            this.getRandomGene = getRandomGene;
            this.fitnessFunction = fitnessFunction;

            if (shouldInitGenes)
            {
                for (int i = 0; i < Genes.Length; i++)
                {
                    Genes[i] = getRandomGene();
                }
            }
        }

        public float CalculateFitness(int index)
        {
            Fitness = fitnessFunction(index);
            return Fitness;
        }

        public Chromosome<T> Crossover(Chromosome<T> otherParent)
        {
            Chromosome<T> child = new Chromosome<T>(Genes.Length, random, getRandomGene, fitnessFunction, shouldInitGenes: false);

            for (int i = 0; i < Genes.Length; i++)
            {
                child.Genes[i] = random.NextDouble() < 0.5 ? Genes[i] : otherParent.Genes[i];
            }

            return child;
        }

        public void Mutate(float mutationRate)
        {
            for (int i = 0; i < Genes.Length; ++i)
            {
                if (random.NextDouble() < mutationRate)
                {
                    Genes[i] = getRandomGene();
                }
            }
        }
    }
}