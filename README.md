# Metaheuristic Optimization Methods Repository

## Introduction

This repository provides comprehensive implementations of state-of-the-art metaheuristic optimization algorithms, with a focus on academic research and scientific applications. Metaheuristics are high-level problem-solving frameworks that guide the search process to find near-optimal solutions for complex optimization problems that are often NP-hard in nature.

The algorithms implemented here are based on seminal research papers and have been extensively studied in the optimization literature. These methods are particularly valuable when traditional optimization techniques fail to provide satisfactory solutions due to problem complexity, multiple local optima, or computational constraints.

## Theoretical Background

Metaheuristic algorithms draw inspiration from various natural phenomena and mathematical concepts to solve optimization problems. Unlike exact algorithms, metaheuristics do not guarantee optimal solutions but provide high-quality approximations in reasonable computational time. This trade-off makes them invaluable for real-world applications where finding the global optimum is computationally intractable.

The general optimization problem addressed by these algorithms can be formulated as:

```
minimize f(x)
subject to: x ∈ S
```

Where `f(x)` is the objective function and `S` is the feasible solution space.

## Implemented Algorithms

### 1. Genetic Algorithms (GA)

**Theoretical Foundation:**
Genetic Algorithms, pioneered by John Holland (1975) and further developed by David Goldberg (1989), are evolutionary computation techniques inspired by natural selection and genetics. GAs operate on a population of candidate solutions, applying genetic operators such as selection, crossover, and mutation to evolve better solutions over generations.

**Mathematical Formulation:**
- **Selection:** Probability of selection based on fitness: `P(xi) = f(xi) / Σf(xj)`
- **Crossover:** Single-point crossover at position k: `offspring1 = parent1[1:k] + parent2[k+1:n]`
- **Mutation:** Bit-flip mutation with probability `pm`

**Key References:**
- Holland, J. H. (1992). Adaptation in Natural and Artificial Systems. MIT Press.
- Goldberg, D. E. (1989). Genetic Algorithms in Search, Optimization, and Machine Learning. Addison-Wesley.

**Implementation Details:**
- **Languages:** Python, C#, MATLAB
- **Features:** Tournament selection, single-point crossover, Gaussian mutation
- **Applications:** Function optimization, combinatorial problems

### 2. Simulated Annealing (SA)

**Theoretical Foundation:**
Simulated Annealing, introduced by Kirkpatrick et al. (1983), is inspired by the annealing process in metallurgy. The algorithm allows occasional acceptance of worse solutions to escape local optima, with the probability of acceptance decreasing over time according to a cooling schedule.

**Mathematical Formulation:**
- **Acceptance Probability:** `P(accept) = exp(-Δf / T)` for Δf > 0
- **Cooling Schedule:** `T(k) = T0 * α^k` (geometric cooling)
- **Metropolis Criterion:** Accept if `Δf ≤ 0` or `random() < P(accept)`

**Key References:**
- Kirkpatrick, S., Gelatt, C. D., & Vecchi, M. P. (1983). Optimization by simulated annealing. Science, 220(4598), 671-680.
- Černý, V. (1985). Thermodynamical approach to the traveling salesman problem. Journal of Optimization Theory and Applications, 45(1), 41-51.

**Implementation Details:**
- **Languages:** Python, C++
- **Features:** Multiple cooling schedules, continuous and discrete optimization
- **Applications:** Traveling Salesman Problem, Quadratic Assignment Problem, Himmelblau Function

### 3. Tabu Search (TS)

**Theoretical Foundation:**
Tabu Search, developed by Fred Glover (1986), is a metaheuristic that guides a local search procedure to explore the solution space beyond local optimality. It uses adaptive memory structures (tabu lists) to avoid cycling and encourage exploration of unvisited regions.

**Mathematical Formulation:**
- **Tabu List:** T(k) = {moves forbidden at iteration k}
- **Aspiration Criteria:** Override tabu status if move leads to best known solution
- **Intensification/Diversification:** Balance between exploiting good regions and exploring new areas

**Key References:**
- Glover, F. (1986). Future paths for integer programming and links to artificial intelligence. Computers & Operations Research, 13(5), 533-549.
- Glover, F., & Laguna, M. (1997). Tabu Search. Kluwer Academic Publishers.

**Implementation Details:**
- **Languages:** Python
- **Features:** Dynamic tabu tenure, aspiration criteria
- **Applications:** Combinatorial optimization problems

### 4. Ant Colony Optimization (ACO)

**Theoretical Foundation:**
Ant Colony Optimization, introduced by Marco Dorigo (1992), mimics the foraging behavior of ants. Artificial ants construct solutions by moving through a graph representation of the problem, depositing pheromone trails that influence future decisions.

**Mathematical Formulation:**
- **Transition Probability:** `Pij^k = (τij^α * ηij^β) / Σ(τil^α * ηil^β)`
- **Pheromone Update:** `τij(t+1) = (1-ρ) * τij(t) + Σ Δτij^k`
- **Heuristic Information:** `ηij = 1/dij` (for TSP)

Where:
- `τij`: pheromone intensity
- `ηij`: heuristic information
- `α, β`: control parameters
- `ρ`: evaporation rate

**Key References:**
- Dorigo, M., & Gambardella, L. M. (1997). Ant colony system: a cooperative learning approach to the traveling salesman problem. IEEE Transactions on Evolutionary Computation, 1(1), 53-66.
- Stützle, T., & Hoos, H. H. (2000). MAX–MIN ant system. Future Generation Computer Systems, 16(8), 889-914.

**Implementation Details:**
- **Languages:** C++, MATLAB
- **Features:** Elitist strategy, pheromone bounds
- **Applications:** Traveling Salesman Problem, routing problems

### 5. Differential Evolution (DE)

**Theoretical Foundation:**
Differential Evolution, proposed by Storn and Price (1997), is a population-based optimization algorithm that uses vector differences to create new candidate solutions. DE is particularly effective for continuous optimization problems.

**Mathematical Formulation:**
- **Mutation:** `Vi = Xr1 + F * (Xr2 - Xr3)`
- **Crossover:** `Ui,j = Vi,j if rand() ≤ CR or j = jrand, else Xi,j`
- **Selection:** `Xi(t+1) = Ui if f(Ui) ≤ f(Xi), else Xi`

Where:
- `F`: mutation factor
- `CR`: crossover probability
- `r1, r2, r3`: random indices

**Key References:**
- Storn, R., & Price, K. (1997). Differential evolution–a simple and efficient heuristic for global optimization over continuous spaces. Journal of Global Optimization, 11(4), 341-359.
- Price, K., Storn, R. M., & Lampinen, J. A. (2005). Differential Evolution: A Practical Approach to Global Optimization. Springer.

**Implementation Details:**
- **Languages:** C++
- **Features:** Multiple mutation strategies
- **Applications:** Continuous function optimization

## Algorithm Selection Guidelines

### Problem Characteristics vs. Algorithm Suitability

| Problem Type | GA | SA | TS | ACO | DE |
|-------------|----|----|----|----|-----|
| Continuous Optimization | ✓ | ✓✓ | ✗ | ✗ | ✓✓✓ |
| Discrete Optimization | ✓✓ | ✓✓ | ✓✓✓ | ✓✓✓ | ✗ |
| Multi-objective | ✓✓✓ | ✓ | ✓ | ✓ | ✓✓ |
| Large Search Spaces | ✓✓ | ✓ | ✓ | ✓✓ | ✓✓ |
| Real-time Applications | ✓ | ✓✓ | ✓✓ | ✓ | ✓✓ |

**Legend:** ✓✓✓ Excellent, ✓✓ Good, ✓ Fair, ✗ Not Suitable

## Usage Examples

### Example 1: Function Optimization with Genetic Algorithm

```python
# Example: Minimizing Himmelblau's function
# f(x,y) = (x² + y - 11)² + (x + y² - 7)²

import numpy as np
from ga import run

# Define problem
problem = structure()
problem.costfunc = lambda x: ((x[0]**2 + x[1] - 11)**2 + (x[0] + x[1]**2 - 7)**2)
problem.nvar = 2
problem.varmin = -5
problem.varmax = 5

# Set parameters
params = structure()
params.maxit = 100
params.npop = 50

# Run algorithm
result = run(problem, params)
```

### Example 2: Traveling Salesman Problem with ACO

```matlab
% TSP with Ant Colony Optimization
graph = createGraph();  % Create city graph
[bestTour, bestCost] = runACO(graph, 50, 500);  % 50 ants, 500 iterations
drawBestTour(graph, bestTour);
```

## Performance Benchmarks

### Standard Test Functions

The implementations have been tested on standard optimization benchmarks:

1. **Sphere Function:** `f(x) = Σxi²`
2. **Rosenbrock Function:** `f(x) = Σ[100(xi+1 - xi²)² + (1 - xi)²]`
3. **Himmelblau Function:** `f(x,y) = (x² + y - 11)² + (x + y² - 7)²`
4. **Ackley Function:** `f(x) = -20exp(-0.2√(Σxi²/n)) - exp(Σcos(2πxi)/n) + 20 + e`

## Repository Structure

```
metaheuristics/
├── GA/                     # Genetic Algorithms
│   ├── GA-py/             # Python implementation
│   ├── GA-c/              # C# implementation
│   └── GA-matlab/         # MATLAB implementation
├── SA/                     # Simulated Annealing
│   ├── Simulated Annealing - python/
│   └── Simulated Annealing - CPP/
├── Tabu Search/           # Tabu Search implementations
├── ACO/                   # Ant Colony Optimization
│   ├── AntColonyOptimization-C/
│   └── AntColonyOptimization-matlab/
└── Differential Evolution/ # Differential Evolution
```

## Contributing

We welcome contributions that maintain the academic rigor of this repository. Please ensure:

1. Implementations follow the original algorithmic specifications
2. Code is well-documented with theoretical background
3. Include relevant academic references
4. Provide test cases with known benchmarks

## Academic References

### Foundational Papers

1. **Genetic Algorithms:**
   - Holland, J. H. (1975). Adaptation in Natural and Artificial Systems. University of Michigan Press.
   - Goldberg, D. E. (1989). Genetic Algorithms in Search, Optimization, and Machine Learning. Addison-Wesley.

2. **Simulated Annealing:**
   - Kirkpatrick, S., Gelatt Jr, C. D., & Vecchi, M. P. (1983). Optimization by simulated annealing. Science, 220(4598), 671-680.
   - Metropolis, N., et al. (1953). Equation of state calculations by fast computing machines. The Journal of Chemical Physics, 21(6), 1087-1092.

3. **Tabu Search:**
   - Glover, F. (1986). Future paths for integer programming and links to artificial intelligence. Computers & Operations Research, 13(5), 533-549.
   - Glover, F., & Laguna, M. (1997). Tabu Search. Kluwer Academic Publishers.

4. **Ant Colony Optimization:**
   - Dorigo, M., Maniezzo, V., & Colorni, A. (1996). Ant system: optimization by a colony of cooperating agents. IEEE Transactions on Systems, Man, and Cybernetics, Part B, 26(1), 29-41.
   - Stützle, T., & Hoos, H. H. (2000). MAX–MIN ant system. Future Generation Computer Systems, 16(8), 889-914.

5. **Differential Evolution:**
   - Storn, R., & Price, K. (1997). Differential evolution–a simple and efficient heuristic for global optimization over continuous spaces. Journal of Global Optimization, 11(4), 341-359.

### Survey Papers

- Blum, C., & Roli, A. (2003). Metaheuristics in combinatorial optimization: Overview and conceptual comparison. ACM Computing Surveys, 35(3), 268-308.
- Gendreau, M., & Potvin, J. Y. (Eds.). (2010). Handbook of Metaheuristics. Springer.
- Talbi, E. G. (2009). Metaheuristics: From Design to Implementation. John Wiley & Sons.

## License

This repository is intended for educational and research purposes. Please cite appropriate academic sources when using these implementations in your research.

---

*This repository is continuously updated with new implementations and improvements. For questions or suggestions, please open an issue.*
