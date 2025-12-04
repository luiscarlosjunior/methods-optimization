# Simulated Annealing (SA)

## Overview

Simulated Annealing is a probabilistic metaheuristic inspired by the annealing process in metallurgy, where controlled cooling of a material reduces defects and achieves a stable low-energy state. In optimization, SA explores the solution space by occasionally accepting worse solutions to escape local optima, with the probability of such moves decreasing over time according to a "cooling schedule."

## Historical Background

Simulated Annealing was independently developed by:
- **Scott Kirkpatrick**, **C. Daniel Gelatt Jr.**, and **Mario P. Vecchi** at IBM in 1983
- **Vladimír Černý** at Comenius University in 1985

The algorithm draws its theoretical foundation from the **Metropolis-Hastings algorithm** (1953), which was originally designed to simulate the behavior of particles in thermodynamic systems.

### Key Contributors:
- **Nicholas Metropolis** (1915-1999) - Metropolis criterion
- **Scott Kirkpatrick** - Optimization application
- **Vladimír Černý** - Independent discovery and TSP application
- **Emile Aarts** & **Jan Korst** - Theoretical analysis and practical guidelines

## Theoretical Foundation

### Physical Analogy

The algorithm mimics the physical process of annealing:
1. **Heating**: Start with high temperature (allows random moves)
2. **Slow Cooling**: Gradually decrease temperature (reduce randomness)
3. **Crystallization**: Reach low-energy state (optimal/near-optimal solution)

### Mathematical Formulation

#### Energy Function
In optimization, the objective function f(x) corresponds to the energy E:
```
E = f(x), where x ∈ S (solution space)
```

#### Acceptance Probability (Metropolis Criterion)
A candidate solution with energy difference ΔE is accepted with probability:
```
P(accept) = {
    1                    if ΔE ≤ 0 (improvement)
    exp(-ΔE/T)          if ΔE > 0 (worse solution)
}
```

Where:
- `ΔE = E_new - E_current` (energy difference)
- `T` = current temperature (control parameter)

#### Cooling Schedule
The temperature T is reduced according to a schedule:

1. **Geometric Cooling** (most common):
```
T(k) = α^k × T₀,  where 0.8 ≤ α ≤ 0.99
```

2. **Linear Cooling**:
```
T(k) = T₀ - k × δ
```

3. **Logarithmic Cooling** (slowest, guarantees convergence):
```
T(k) = T₀ / log(1 + k)
```

4. **Exponential Cooling**:
```
T(k) = T₀ × exp(-k/τ)
```

### Algorithm Pseudocode

```
1. Initialize:
   - x = random solution
   - T = T₀ (initial temperature)
   - best = x
   
2. While T > T_min:
   For i = 1 to L (iterations at current T):
       a. Generate neighbor: x' = neighbor(x)
       b. Calculate: ΔE = f(x') - f(x)
       c. If ΔE ≤ 0:
            x = x'  (accept improvement)
       d. Else:
            If random() < exp(-ΔE/T):
                x = x'  (accept with probability)
       e. If f(x) < f(best):
            best = x
   3. Cool down: T = cool(T)
   
4. Return best
```

## Key Parameters

| Parameter | Symbol | Typical Range | Description |
|-----------|--------|---------------|-------------|
| Initial Temperature | T₀ | 100-1000 | Starting temperature (problem-dependent) |
| Final Temperature | T_min | 0.001-0.1 | Stopping criterion |
| Cooling Rate | α | 0.80-0.99 | Temperature reduction factor |
| Iterations per Temp | L | 10-1000 | Equilibrium at each temperature |
| Neighborhood Size | - | - | Local search range |

## Cooling Schedule Comparison

| Schedule | Speed | Quality | Best For |
|----------|-------|---------|----------|
| Geometric | Fast | Good | Most applications |
| Linear | Medium | Good | Time-constrained problems |
| Logarithmic | Very Slow | Excellent | Theoretical guarantees needed |
| Exponential | Adjustable | Variable | Fine-tuning required |

## Industrial Applications

### 1. **VLSI Circuit Design**
- **Problem**: Placement and routing of electronic components
- **Application**: Minimize wire length and chip area
- **Industry Impact**: Used by major semiconductor companies (Intel, AMD)
- **Example**: Cell placement in integrated circuits with 10,000+ components

### 2. **Manufacturing and Scheduling**
- **Job Shop Scheduling**: Minimize makespan in production lines
- **Vehicle Routing**: Optimize delivery routes (similar to TSP)
- **Facility Layout**: Arrange machinery to minimize material handling
- **Example**: Toyota uses SA-based systems for production scheduling

### 3. **Logistics and Transportation**
- **Traveling Salesman Problem (TSP)**: Optimal route through cities
- **Vehicle Routing Problem (VRP)**: Multiple vehicle optimization
- **Airline Crew Scheduling**: Assign crews to flights efficiently
- **Example**: FedEx route optimization for package delivery

### 4. **Telecommunications**
- **Network Design**: Optimal topology for minimum cost
- **Frequency Assignment**: Channel allocation in cellular networks
- **Bandwidth Allocation**: Optimize resource distribution
- **Example**: AT&T network capacity planning

### 5. **Computer Vision and Image Processing**
- **Image Segmentation**: Partition images into regions
- **Object Recognition**: Feature matching optimization
- **Image Registration**: Align multiple images
- **Example**: Medical imaging alignment in CT/MRI scans

### 6. **Financial Engineering**
- **Portfolio Optimization**: Asset allocation strategies
- **Option Pricing**: Calibrate financial models
- **Risk Management**: Optimize hedging strategies
- **Example**: Goldman Sachs model calibration

### 7. **Energy Systems**
- **Power Plant Scheduling**: Optimize generation dispatch
- **Energy Trading**: Bidding strategy optimization
- **Smart Grid Management**: Load balancing
- **Example**: Grid operators use SA for unit commitment problems

## Advantages and Limitations

### Advantages:
✓ Simple to implement and understand  
✓ Few parameters to tune  
✓ Can escape local optima effectively  
✓ Works well for discrete and continuous problems  
✓ Theoretical convergence guarantees (with proper cooling)  
✓ Good for problems with many local optima  

### Limitations:
✗ Slow convergence compared to other metaheuristics  
✗ Performance highly dependent on cooling schedule  
✗ No population-based learning  
✗ Difficult to determine optimal temperature settings  
✗ May require many iterations for complex problems  
✗ Sequential nature limits parallelization  

## Theoretical Guarantees

**Convergence Theorem** (Hajek, 1988):
If the cooling schedule is:
```
T(k) ≥ d / log(1 + k)
```
where d is a problem-dependent constant, then SA converges to the global optimum with probability 1 as k → ∞.

**Practical Implication**: Logarithmic cooling guarantees optimality but is too slow for real applications. Geometric cooling provides good practical performance.

## Implementation Notes

This repository contains two implementations:

1. **Simulated Annealing - python/**: Python implementation
   - Easy to understand and modify
   - Multiple test problems included
   - Suitable for research and education

2. **Simulated Annealing - CPP/**: C++ implementation
   - High-performance execution
   - Suitable for large-scale industrial applications
   - Optimized for speed

## Benchmark Problems

### Standard Test Functions

1. **Quadratic Assignment Problem (QAP)**
   - Facility location optimization
   - n! possible solutions for n facilities
   - Classic SA application

2. **Himmelblau Function** (Continuous)
   ```
   f(x,y) = (x² + y - 11)² + (x + y² - 7)²
   Global minima: f(3, 2) = f(-2.805, 3.131) = 
                  f(-3.779, -3.283) = f(3.584, -1.848) = 0
   ```

3. **Traveling Salesman Problem (TSP)**
   - Find shortest route through n cities
   - Classic combinatorial optimization
   - Real-world logistics applications

## Parameter Tuning Guidelines

### Initial Temperature (T₀)
- **Rule of thumb**: Accept ~80-90% of worse moves initially
- **Formula**: T₀ ≈ -Δf_avg / ln(0.8), where Δf_avg is average increase in objective
- **Empirical**: Run test with very high T, reduce until acceptance rate ~80%

### Cooling Rate (α)
- **Slow cooling (α = 0.95-0.99)**: Better quality, longer runtime
- **Medium cooling (α = 0.90-0.95)**: Balanced approach
- **Fast cooling (α = 0.80-0.90)**: Faster, may miss optimum

### Iterations per Temperature (L)
- **Simple rule**: L = 100 (constant)
- **Adaptive**: L = n (problem size)
- **Thorough**: L = n² (for complex problems)

### Final Temperature (T_min)
- **Criterion**: T_min = T₀ × 10^-3 to 10^-5
- **Alternative**: Stop when no improvement for K consecutive temperatures

## Academic References

### Foundational Papers:
1. Kirkpatrick, S., Gelatt, C. D., & Vecchi, M. P. (1983). "Optimization by simulated annealing". *Science*, 220(4598), 671-680.
2. Černý, V. (1985). "Thermodynamical approach to the traveling salesman problem: An efficient simulation algorithm". *Journal of Optimization Theory and Applications*, 45(1), 41-51.
3. Metropolis, N., Rosenbluth, A. W., Rosenbluth, M. N., Teller, A. H., & Teller, E. (1953). "Equation of state calculations by fast computing machines". *The Journal of Chemical Physics*, 21(6), 1087-1092.

### Theoretical Analysis:
4. Hajek, B. (1988). "Cooling schedules for optimal annealing". *Mathematics of Operations Research*, 13(2), 311-329.
5. Aarts, E., & Korst, J. (1989). *Simulated Annealing and Boltzmann Machines*. John Wiley & Sons.
6. Van Laarhoven, P. J., & Aarts, E. H. (1987). *Simulated Annealing: Theory and Applications*. Springer.

### Recent Surveys:
7. Delahaye, D., Chaimatanan, S., & Mongeau, M. (2019). "Simulated annealing: From basics to applications". *Handbook of Metaheuristics*, 1-35.
8. Suman, B., & Kumar, P. (2006). "A survey of simulated annealing as a tool for single and multiobjective optimization". *Journal of the Operational Research Society*, 57(10), 1143-1160.

### Applications:
9. Johnson, D. S., Aragon, C. R., McGeoch, L. A., & Schevon, C. (1989). "Optimization by simulated annealing: An experimental evaluation; part I, graph partitioning". *Operations Research*, 37(6), 865-892.
10. Collins, N. E., Eglese, R. W., & Golden, B. L. (1988). "Simulated annealing—an annotated bibliography". *American Journal of Mathematical and Management Sciences*, 8(3-4), 209-307.

## Getting Started

Refer to the specific implementation folders for detailed usage:
- [Python Implementation](Simulated%20Annealing%20-%20python/README.md)
- [C++ Implementation](Simulated%20Annealing%20-%20CPP/README.md)

## Further Reading

- **Books**:
  - Aarts, E. H. L., & Lenstra, J. K. (Eds.). (2003). *Local Search in Combinatorial Optimization*. Princeton University Press.
  - Press, W. H., et al. (2007). *Numerical Recipes: The Art of Scientific Computing*. Cambridge University Press. (Chapter 10.9)

- **Online Resources**:
  - [MIT OCW: Simulated Annealing Lectures](https://ocw.mit.edu)
  - [Stanford Engineering: Optimization Methods](https://see.stanford.edu)

## Comparison with Other Methods

| Aspect | SA | GA | Tabu Search | ACO |
|--------|----|----|-------------|-----|
| Population | Single solution | Multiple | Single | Multiple |
| Memory | Current + best | Population | Tabu list | Pheromone trails |
| Parameters | Few (3-5) | Many (5-8) | Medium (3-6) | Many (4-7) |
| Convergence | Slow but sure | Medium | Fast | Medium |
| Parallelization | Limited | Excellent | Limited | Good |

## Troubleshooting Common Issues

### Problem: Algorithm Not Finding Good Solutions

**Diagnosis checklist:**
1. Is the initial temperature high enough to accept most moves initially?
2. Is the cooling rate slow enough to allow equilibrium at each temperature?
3. Is the neighborhood function generating diverse candidates?
4. Is the minimum temperature low enough to allow fine-tuning?

**Solutions:**
- Increase T₀ until initial acceptance rate is ~80%
- Decrease α closer to 0.99 for slow cooling
- Increase iterations per temperature (L)
- Implement adaptive temperature schedule

### Problem: Algorithm is Too Slow

**Possible causes:**
1. Too many iterations per temperature
2. Expensive fitness function evaluation
3. Very slow cooling schedule
4. Large neighborhood size

**Solutions:**
- Reduce L (iterations per temperature)
- Cache fitness values when possible
- Implement incremental fitness evaluation
- Use candidate list strategies for neighborhoods

### Problem: Solutions Vary Widely Across Runs

**Strategies to improve consistency:**
- Use multiple restarts and keep best solution
- Implement reheating when stuck
- Combine with local search for final refinement
- Increase the number of iterations

## Best Practices

### For Continuous Optimization
```
1. Use Gaussian perturbation for neighbor generation
2. Adaptive step size that decreases with temperature
3. Consider problem-specific neighbor operators
4. Apply bounds checking or reflection at boundaries
```

### For Combinatorial Optimization
```
1. Use swap, insert, or inversion moves
2. Consider multiple move types with varying intensities
3. Implement efficient incremental cost calculation
4. Use appropriate tabu-like mechanisms to avoid cycling
```

### For Real-Time Applications
```
1. Set strict time limits instead of iteration counts
2. Use geometric cooling with carefully chosen rate
3. Implement any-time behavior (return best so far)
4. Consider parallel independent runs
```

## Real-World Case Studies

### Case Study 1: VLSI Chip Placement (Intel)
**Problem**: Place 10,000+ gates on a chip minimizing wire length
**SA Configuration**: T₀=10000, α=0.95, L=10×n
**Results**: 
- 20% improvement over greedy placement
- Computation time: 2-4 hours for production chips
- Used in commercial EDA tools

### Case Study 2: Airline Crew Scheduling (Delta Airlines)
**Problem**: Assign 5,000 crew members to 3,000 flights monthly
**SA Configuration**: Custom cooling with restarts
**Results**:
- 8% reduction in crew costs
- Better work-life balance scores
- Compliance with FAA regulations

### Case Study 3: Protein Structure Prediction
**Problem**: Find minimum energy configuration of 100+ amino acids
**SA Configuration**: Very slow cooling (α=0.999), millions of iterations
**Results**:
- Accurate secondary structure prediction
- Complemented molecular dynamics simulations
- Published in computational biology journals

---

*For questions or contributions, please refer to the main repository documentation.*
