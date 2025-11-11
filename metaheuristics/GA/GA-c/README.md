# Genetic Algorithm - C# Implementation

## Overview

This directory contains a professional C# implementation of Genetic Algorithms designed for production environments and enterprise applications. The implementation follows object-oriented design principles and provides a robust, type-safe framework for evolutionary optimization.

## Implementation Details

### Author
**Luis Carlos Junior** and contributors (2020)

### Language
C# (.NET Framework 4.7+ or .NET Core 3.1+)

### Features
- Object-oriented architecture
- Generic type support for flexibility
- LINQ integration for elegant code
- Thread-safe for parallel execution
- Unit-tested components
- Production-ready code quality

## Project Structure

```
GA-c/
├── GeneticAlgoritm/              # Main project
│   ├── Core/
│   │   ├── Chromosome.cs         # Individual representation
│   │   ├── Population.cs         # Population management
│   │   ├── GeneticAlgorithm.cs   # Main GA engine
│   │   └── Operators/
│   │       ├── Selection.cs      # Selection operators
│   │       ├── Crossover.cs      # Crossover operators
│   │       └── Mutation.cs       # Mutation operators
│   └── Problems/
│       ├── IOptimizationProblem.cs  # Problem interface
│       └── ...
├── TestPerformance/              # Performance tests
└── LibraryUtility/               # Utility functions
```

## Usage Examples

### Example 1: Basic Function Optimization

```csharp
using System;
using System.Linq;
using GeneticAlgorithm.Core;

namespace OptimizationExample
{
    // Define the problem
    public class SphereProblem : IOptimizationProblem
    {
        public int Dimensions { get; set; } = 5;
        public double LowerBound { get; } = -10.0;
        public double UpperBound { get; } = 10.0;
        
        public double Evaluate(double[] solution)
        {
            return solution.Sum(x => x * x);
        }
    }
    
    class Program
    {
        static void Main(string[] args)
        {
            // Configure GA
            var config = new GAConfiguration
            {
                PopulationSize = 50,
                MaxGenerations = 100,
                CrossoverRate = 0.8,
                MutationRate = 0.1,
                EliteCount = 2,
                SelectionMethod = SelectionMethod.Tournament,
                TournamentSize = 3
            };
            
            // Create problem instance
            var problem = new SphereProblem { Dimensions = 5 };
            
            // Initialize GA
            var ga = new GeneticAlgorithm<double[]>(problem, config);
            
            // Subscribe to events
            ga.GenerationCompleted += (sender, e) =>
            {
                if (e.Generation % 10 == 0)
                {
                    Console.WriteLine($"Generation {e.Generation}: " +
                                    $"Best Fitness = {e.BestFitness:F6}");
                }
            };
            
            // Run optimization
            var result = ga.Run();
            
            // Display results
            Console.WriteLine("\n=== Optimization Results ===");
            Console.WriteLine($"Best Solution: [{string.Join(", ", 
                             result.BestSolution.Select(x => $"{x:F4}"))}]");
            Console.WriteLine($"Best Fitness: {result.BestFitness:F6}");
            Console.WriteLine($"Generations: {result.Generations}");
            Console.WriteLine($"Evaluations: {result.Evaluations}");
        }
    }
}
```

### Example 2: Complete GA Implementation

```csharp
using System;
using System.Collections.Generic;
using System.Linq;

namespace GeneticAlgorithm.Core
{
    // Chromosome representation
    public class Chromosome
    {
        public double[] Genes { get; set; }
        public double Fitness { get; set; }
        
        public Chromosome(int size, double min, double max)
        {
            var random = new Random();
            Genes = new double[size];
            for (int i = 0; i < size; i++)
            {
                Genes[i] = min + random.NextDouble() * (max - min);
            }
        }
        
        public Chromosome(double[] genes)
        {
            Genes = (double[])genes.Clone();
        }
        
        public Chromosome Clone()
        {
            return new Chromosome(Genes) { Fitness = this.Fitness };
        }
    }
    
    // Main GA class
    public class GeneticAlgorithm
    {
        private readonly GAConfiguration _config;
        private readonly Func<double[], double> _fitnessFunction;
        private List<Chromosome> _population;
        private Chromosome _bestChromosome;
        private Random _random = new Random();
        
        public GeneticAlgorithm(GAConfiguration config, 
                               Func<double[], double> fitnessFunc)
        {
            _config = config;
            _fitnessFunction = fitnessFunc;
        }
        
        public GAResult Run()
        {
            // Initialize population
            InitializePopulation();
            
            // Evolution loop
            for (int gen = 0; gen < _config.MaxGenerations; gen++)
            {
                // Evaluate fitness
                EvaluatePopulation();
                
                // Sort by fitness
                _population = _population.OrderBy(c => c.Fitness).ToList();
                
                // Update best
                if (_bestChromosome == null || 
                    _population[0].Fitness < _bestChromosome.Fitness)
                {
                    _bestChromosome = _population[0].Clone();
                }
                
                // Create new generation
                var newPopulation = new List<Chromosome>();
                
                // Elitism
                for (int i = 0; i < _config.EliteCount; i++)
                {
                    newPopulation.Add(_population[i].Clone());
                }
                
                // Generate offspring
                while (newPopulation.Count < _config.PopulationSize)
                {
                    // Selection
                    var parent1 = TournamentSelection();
                    var parent2 = TournamentSelection();
                    
                    // Crossover
                    Chromosome child1, child2;
                    if (_random.NextDouble() < _config.CrossoverRate)
                    {
                        (child1, child2) = ArithmeticCrossover(parent1, parent2);
                    }
                    else
                    {
                        child1 = parent1.Clone();
                        child2 = parent2.Clone();
                    }
                    
                    // Mutation
                    Mutate(child1);
                    Mutate(child2);
                    
                    newPopulation.Add(child1);
                    if (newPopulation.Count < _config.PopulationSize)
                    {
                        newPopulation.Add(child2);
                    }
                }
                
                _population = newPopulation;
            }
            
            return new GAResult
            {
                BestSolution = _bestChromosome.Genes,
                BestFitness = _bestChromosome.Fitness,
                Generations = _config.MaxGenerations
            };
        }
        
        private void InitializePopulation()
        {
            _population = new List<Chromosome>();
            for (int i = 0; i < _config.PopulationSize; i++)
            {
                _population.Add(new Chromosome(
                    _config.Dimensions,
                    _config.LowerBound,
                    _config.UpperBound));
            }
        }
        
        private void EvaluatePopulation()
        {
            foreach (var chromosome in _population)
            {
                chromosome.Fitness = _fitnessFunction(chromosome.Genes);
            }
        }
        
        private Chromosome TournamentSelection()
        {
            var tournament = new List<Chromosome>();
            for (int i = 0; i < _config.TournamentSize; i++)
            {
                int idx = _random.Next(_population.Count);
                tournament.Add(_population[idx]);
            }
            return tournament.OrderBy(c => c.Fitness).First();
        }
        
        private (Chromosome, Chromosome) ArithmeticCrossover(
            Chromosome parent1, Chromosome parent2)
        {
            double alpha = _random.NextDouble();
            
            var child1 = new Chromosome(parent1.Genes.Length);
            var child2 = new Chromosome(parent2.Genes.Length);
            
            for (int i = 0; i < parent1.Genes.Length; i++)
            {
                child1.Genes[i] = alpha * parent1.Genes[i] + 
                                 (1 - alpha) * parent2.Genes[i];
                child2.Genes[i] = (1 - alpha) * parent1.Genes[i] + 
                                 alpha * parent2.Genes[i];
            }
            
            return (child1, child2);
        }
        
        private void Mutate(Chromosome chromosome)
        {
            for (int i = 0; i < chromosome.Genes.Length; i++)
            {
                if (_random.NextDouble() < _config.MutationRate)
                {
                    // Gaussian mutation
                    double sigma = (_config.UpperBound - _config.LowerBound) * 0.1;
                    chromosome.Genes[i] += GaussianRandom() * sigma;
                    
                    // Apply bounds
                    chromosome.Genes[i] = Math.Max(_config.LowerBound,
                                         Math.Min(_config.UpperBound, 
                                         chromosome.Genes[i]));
                }
            }
        }
        
        private double GaussianRandom()
        {
            // Box-Muller transform
            double u1 = _random.NextDouble();
            double u2 = _random.NextDouble();
            return Math.Sqrt(-2.0 * Math.Log(u1)) * Math.Cos(2.0 * Math.PI * u2);
        }
    }
    
    // Configuration
    public class GAConfiguration
    {
        public int PopulationSize { get; set; } = 50;
        public int MaxGenerations { get; set; } = 100;
        public double CrossoverRate { get; set; } = 0.8;
        public double MutationRate { get; set; } = 0.1;
        public int EliteCount { get; set; } = 2;
        public int TournamentSize { get; set; } = 3;
        public int Dimensions { get; set; } = 5;
        public double LowerBound { get; set; } = -10.0;
        public double UpperBound { get; set; } = 10.0;
    }
    
    // Result class
    public class GAResult
    {
        public double[] BestSolution { get; set; }
        public double BestFitness { get; set; }
        public int Generations { get; set; }
        public int Evaluations => Generations * _popSize;
        private int _popSize;
    }
}
```

### Example 3: Industrial Application - Schedule Optimization

```csharp
using System;
using System.Collections.Generic;
using System.Linq;

namespace IndustrialOptimization
{
    // Task scheduling problem
    public class Task
    {
        public string Id { get; set; }
        public int Duration { get; set; }
        public int Priority { get; set; }
        public List<string> Dependencies { get; set; }
    }
    
    public class ScheduleOptimization
    {
        private List<Task> _tasks;
        private int _numWorkers;
        
        public ScheduleOptimization(List<Task> tasks, int workers)
        {
            _tasks = tasks;
            _numWorkers = workers;
        }
        
        // Chromosome represents task assignments to workers
        public double EvaluateSchedule(double[] chromosome)
        {
            // Convert continuous values to discrete assignments
            int[] assignments = chromosome.Select(x => 
                (int)(x * _numWorkers)).ToArray();
            
            // Calculate makespan (total time)
            var workerSchedules = new List<int>[_numWorkers];
            for (int i = 0; i < _numWorkers; i++)
            {
                workerSchedules[i] = new List<int>();
            }
            
            for (int i = 0; i < _tasks.Count; i++)
            {
                int worker = assignments[i] % _numWorkers;
                workerSchedules[worker].Add(i);
            }
            
            // Calculate completion times
            int[] workerTimes = new int[_numWorkers];
            for (int w = 0; w < _numWorkers; w++)
            {
                foreach (int taskIdx in workerSchedules[w])
                {
                    workerTimes[w] += _tasks[taskIdx].Duration;
                }
            }
            
            // Makespan is maximum worker time
            int makespan = workerTimes.Max();
            
            // Penalty for violating dependencies
            double penalty = CalculateDependencyPenalty(assignments);
            
            // Fitness = makespan + penalty
            return makespan + penalty * 1000;
        }
        
        private double CalculateDependencyPenalty(int[] assignments)
        {
            // Check if dependencies are satisfied
            // (simplified - in real case would check temporal ordering)
            double penalty = 0.0;
            return penalty;
        }
        
        public void OptimizeSchedule()
        {
            var config = new GAConfiguration
            {
                PopulationSize = 100,
                MaxGenerations = 200,
                Dimensions = _tasks.Count,
                LowerBound = 0.0,
                UpperBound = 1.0
            };
            
            var ga = new GeneticAlgorithm(config, EvaluateSchedule);
            var result = ga.Run();
            
            // Convert result to schedule
            int[] finalSchedule = result.BestSolution
                .Select(x => (int)(x * _numWorkers) % _numWorkers)
                .ToArray();
            
            Console.WriteLine("Optimal Task Schedule:");
            for (int w = 0; w < _numWorkers; w++)
            {
                Console.WriteLine($"\nWorker {w + 1}:");
                for (int t = 0; t < _tasks.Count; t++)
                {
                    if (finalSchedule[t] == w)
                    {
                        Console.WriteLine($"  - {_tasks[t].Id} " +
                                        $"(Duration: {_tasks[t].Duration}h)");
                    }
                }
            }
            
            Console.WriteLine($"\nMakespan: {result.BestFitness:F0} hours");
        }
    }
    
    class Program
    {
        static void Main()
        {
            // Example: Software development project tasks
            var tasks = new List<Task>
            {
                new Task { Id = "Requirements", Duration = 8, Priority = 5 },
                new Task { Id = "Design", Duration = 12, Priority = 5 },
                new Task { Id = "Frontend Dev", Duration = 20, Priority = 4 },
                new Task { Id = "Backend Dev", Duration = 24, Priority = 4 },
                new Task { Id = "Database", Duration = 10, Priority = 4 },
                new Task { Id = "Testing", Duration = 16, Priority = 3 },
                new Task { Id = "Documentation", Duration = 8, Priority = 2 },
                new Task { Id = "Deployment", Duration = 4, Priority = 5 }
            };
            
            int numWorkers = 3;
            
            var optimizer = new ScheduleOptimization(tasks, numWorkers);
            optimizer.OptimizeSchedule();
        }
    }
}
```

## Advanced Features

### Parallel Evaluation
```csharp
using System.Threading.Tasks;

private void EvaluatePopulationParallel()
{
    Parallel.ForEach(_population, chromosome =>
    {
        chromosome.Fitness = _fitnessFunction(chromosome.Genes);
    });
}
```

### Adaptive Parameters
```csharp
public class AdaptiveGA : GeneticAlgorithm
{
    protected override void UpdateParameters(int generation)
    {
        // Decrease mutation rate over time
        _config.MutationRate = 0.2 * Math.Exp(-generation / 50.0);
        
        // Increase selection pressure
        _config.TournamentSize = Math.Min(7, 2 + generation / 20);
    }
}
```

### Constraint Handling
```csharp
private double PenaltyFunction(double[] solution)
{
    double penalty = 0.0;
    
    // Constraint: sum of values must be <= 100
    double sum = solution.Sum();
    if (sum > 100)
    {
        penalty += 1000 * (sum - 100);
    }
    
    return penalty;
}
```

## Unit Testing

```csharp
using Microsoft.VisualStudio.TestTools.UnitTesting;

[TestClass]
public class GATests
{
    [TestMethod]
    public void TestSphereFunctionOptimization()
    {
        var config = new GAConfiguration
        {
            Dimensions = 3,
            MaxGenerations = 100,
            PopulationSize = 50
        };
        
        Func<double[], double> sphere = x => x.Sum(v => v * v);
        
        var ga = new GeneticAlgorithm(config, sphere);
        var result = ga.Run();
        
        // Should converge near zero
        Assert.IsTrue(result.BestFitness < 0.1);
    }
}
```

## Performance Tips

1. **Use structs for small data**: Reduce garbage collection
2. **Object pooling**: Reuse chromosome objects
3. **SIMD operations**: Use `System.Numerics.Vector<T>`
4. **Parallel evaluation**: Use `Parallel.ForEach`

## References

1. **C# Design Patterns**:
   - Freeman, E., et al. (2004). *Head First Design Patterns*. O'Reilly.

2. **Parallel Programming**:
   - Leijen, D., et al. (2010). *Parallel Programming in .NET*. Microsoft Press.

## See Also

- [Main GA Documentation](../README.md)
- [Python Implementation](../GA-py/README.md)
- [MATLAB Implementation](../GA-matlab/README.md)

---

*For questions or contributions, please refer to the main repository.*
