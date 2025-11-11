# Differential Evolution (DE)

## Overview

Differential Evolution is a population-based stochastic optimization algorithm particularly effective for continuous optimization problems. DE uses vector differences to perturb the population and create new candidate solutions, making it simple yet powerful for global optimization in multidimensional search spaces.

## Historical Background

Differential Evolution was proposed by **Rainer Storn** and **Kenneth Price** in 1995 at the International Computer Science Institute, Berkeley. The algorithm emerged from Storn's attempts to solve the Chebyshev polynomial fitting problem and has since become one of the most popular evolutionary algorithms for continuous optimization.

### Key Contributors:
- **Rainer Storn** (1950-) - Co-creator of DE
- **Kenneth Price** - Co-creator and author of seminal papers
- **Jouni Lampinen** - Extensions and applications
- **P. N. Suganthan** - Parameter adaptation strategies
- **Janez Brest** - Self-adaptive DE variants

### Timeline:
- **1995**: First publication at ICEC'95
- **1997**: Comprehensive paper in Journal of Global Optimization
- **2005**: Book "Differential Evolution: A Practical Approach"
- **2008**: First DE competition at CEC'08
- **2010s-Present**: Adaptive and hybrid variants

## Theoretical Foundation

### Core Concept

DE operates on a population of D-dimensional real-valued vectors through three main operations:
1. **Mutation**: Create donor vector using difference of population members
2. **Crossover**: Combine donor with target to create trial vector
3. **Selection**: Greedy selection between trial and target

### Mathematical Formulation

#### Population
At generation t, the population consists of NP vectors:
```
P(t) = {X₁(t), X₂(t), ..., Xₙₚ(t)}
where Xᵢ(t) = [x₁,ᵢ(t), x₂,ᵢ(t), ..., xᴰ,ᵢ(t)]
```

#### Mutation (Differential Mutation)

**DE/rand/1** (most common):
```
Vᵢ(t) = Xᵣ₁(t) + F · (Xᵣ₂(t) - Xᵣ₃(t))
```

Where:
- **Vᵢ(t)**: Donor (mutant) vector
- **r1, r2, r3**: Random distinct indices ∈ [1, NP], r1 ≠ r2 ≠ r3 ≠ i
- **F**: Mutation factor (scaling factor), typically F ∈ [0.4, 1.0]

**Other Mutation Strategies**:

1. **DE/best/1**:
   ```
   Vᵢ(t) = Xᵦₑₛₜ(t) + F · (Xᵣ₁(t) - Xᵣ₂(t))
   ```

2. **DE/rand/2**:
   ```
   Vᵢ(t) = Xᵣ₁(t) + F · (Xᵣ₂(t) - Xᵣ₃(t)) + F · (Xᵣ₄(t) - Xᵣ₅(t))
   ```

3. **DE/best/2**:
   ```
   Vᵢ(t) = Xᵦₑₛₜ(t) + F · (Xᵣ₁(t) - Xᵣ₂(t)) + F · (Xᵣ₃(t) - Xᵣ₄(t))
   ```

4. **DE/current-to-best/1**:
   ```
   Vᵢ(t) = Xᵢ(t) + F · (Xᵦₑₛₜ(t) - Xᵢ(t)) + F · (Xᵣ₁(t) - Xᵣ₂(t))
   ```

#### Crossover (Recombination)

**Binomial Crossover** (most common):
```
uⱼ,ᵢ(t) = {
    vⱼ,ᵢ(t)  if rand(0,1) ≤ CR or j = jᵣₐₙᵈ
    xⱼ,ᵢ(t)  otherwise
}
```

Where:
- **Uᵢ(t)**: Trial vector
- **CR**: Crossover rate ∈ [0, 1], typically CR ∈ [0.5, 0.9]
- **jᵣₐₙᵈ**: Random index to ensure at least one component from donor

**Exponential Crossover**:
```
Start at random j, copy from donor while rand() ≤ CR,
then copy remaining from target
```

#### Selection

**Greedy Selection**:
```
Xᵢ(t+1) = {
    Uᵢ(t)   if f(Uᵢ(t)) ≤ f(Xᵢ(t))  (minimization)
    Xᵢ(t)   otherwise
}
```

Only better or equal solutions survive to next generation.

### DE Algorithm Structure

```
1. Initialize:
   For i = 1 to NP:
       Xᵢ = random vector in [xₘᵢₙ, xₘₐₓ]
   
2. While stopping criterion not met:
   For i = 1 to NP:
       a. Mutation:
          Select random r1, r2, r3 (distinct from i)
          Vᵢ = Xᵣ₁ + F × (Xᵣ₂ - Xᵣ₃)
       
       b. Crossover:
          For j = 1 to D:
              If rand() ≤ CR or j = jᵣₐₙᵈ:
                  uⱼ,ᵢ = vⱼ,ᵢ
              Else:
                  uⱼ,ᵢ = xⱼ,ᵢ
       
       c. Boundary handling:
          If uⱼ,ᵢ < xₘᵢₙ or uⱼ,ᵢ > xₘₐₓ:
              Apply boundary constraint
       
       d. Selection:
          If f(Uᵢ) ≤ f(Xᵢ):
              Xᵢ = Uᵢ
   
3. Return best solution
```

## Key Parameters

| Parameter | Symbol | Typical Range | Description |
|-----------|--------|---------------|-------------|
| Population Size | NP | 5D - 10D | D is problem dimension |
| Mutation Factor | F | 0.4 - 1.0 | Scaling of difference vectors |
| Crossover Rate | CR | 0.5 - 0.9 | Probability of crossover |
| Max Generations | G | 100 - 10000 | Stopping criterion |
| Dimensions | D | Problem-dependent | Search space dimensionality |

## Strategy Notation: DE/x/y/z

- **x**: Vector to be perturbed (rand, best, current-to-best)
- **y**: Number of difference vectors
- **z**: Crossover scheme (bin=binomial, exp=exponential)

**Examples**:
- **DE/rand/1/bin**: Random base, 1 difference, binomial crossover
- **DE/best/2/bin**: Best base, 2 differences, binomial crossover

## Advanced Variants

### 1. **Self-Adaptive DE (jDE)** - Brest et al. (2006)
- F and CR adapt during evolution
- Each individual has its own F and CR
- Parameters self-tune for the problem

### 2. **JADE** - Zhang & Sanderson (2009)
- Adaptive F and CR using historical data
- Archive of recent successful parameters
- Current-to-pbest mutation strategy

### 3. **SaDE** - Qin & Suganthan (2009)
- Strategy adaptation pool
- Learning period to select best strategies
- Adaptive parameter control

### 4. **CoDE** - Wang et al. (2011)
- Composite of three strategies
- Randomly combines different F, CR, and strategies
- Robust across problem types

### 5. **SHADE** - Tanabe & Fukunaga (2013)
- Success-History based Adaptation
- Linear population size reduction
- Won multiple competitions

## Industrial Applications

### 1. **Neural Network Training**
- **Problem**: Optimize network weights and biases
- **Application**: Deep learning hyperparameter tuning
- **Example**:
  - Train feedforward network with 100+ parameters
  - Achieves comparable accuracy to backpropagation
  - Better for non-differentiable activation functions
  - Used when gradient computation is expensive

### 2. **Control System Design**
- **Problem**: Tune PID controller parameters
- **Application**: Industrial process control
- **Example**:
  - Chemical reactor temperature control
  - Optimize Kp, Ki, Kd for minimal settling time
  - Improved stability margins by 40%
  - Reduced overshoot from 25% to 8%

### 3. **Chemical Engineering**
- **Problem**: Parameter estimation in kinetic models
- **Application**: Reaction rate constant determination
- **Example**:
  - Fit 10+ parameters to experimental data
  - Better fits than least-squares methods
  - Handles noisy measurements
  - Applied in pharmaceutical manufacturing

### 4. **Aerospace Engineering**
- **Problem**: Wing shape optimization
- **Application**: Aerodynamic design
- **Example**:
  - Optimize 20-30 shape parameters
  - Maximize lift-to-drag ratio
  - Used in NASA research programs
  - 8% improvement in fuel efficiency

### 5. **Power Systems**
- **Problem**: Optimal power flow
- **Application**: Economic dispatch of generators
- **Example**:
  - 50-generator system optimization
  - Minimize fuel costs while meeting demand
  - Consider transmission losses and constraints
  - Annual savings of millions of dollars

### 6. **Robotics**
- **Problem**: Inverse kinematics
- **Application**: Robot arm positioning
- **Example**:
  - 6-DOF manipulator configuration
  - Find joint angles for desired end-effector position
  - Real-time capable for certain applications
  - Avoids singularities better than analytical methods

### 7. **Financial Engineering**
- **Problem**: Option pricing model calibration
- **Application**: Risk-neutral volatility surface fitting
- **Example**:
  - Calibrate Heston model (5 parameters)
  - Fit to market option prices
  - More accurate than traditional methods
  - Used by quantitative trading firms

### 8. **Image Processing**
- **Problem**: Image enhancement parameter optimization
- **Application**: Automatic image quality improvement
- **Example**:
  - Optimize contrast, brightness, sharpness
  - Maximize quality metrics (SSIM, PSNR)
  - Used in smartphone camera processing
  - Adobe Lightroom-like adjustments

## Advantages and Limitations

### Advantages:
✓ Very simple to implement (< 100 lines of code)  
✓ Few control parameters (F, CR, NP)  
✓ Highly effective for continuous problems  
✓ Good global search capability  
✓ Handles non-differentiable and noisy functions  
✓ Naturally handles constraints via penalty methods  
✓ Parallel-friendly (fitness evaluations independent)  
✓ Robust across diverse problem types  

### Limitations:
✗ Can be slow for high-dimensional problems (D > 100)  
✗ Performance sensitive to F and CR settings  
✗ Not well-suited for discrete problems  
✗ May stagnate if population diversity is lost  
✗ No explicit mechanism for constraint handling  
✗ Requires many function evaluations  

## Parameter Tuning Guidelines

### Population Size (NP)
- **Rule of thumb**: NP = 10 × D
- **Minimum**: NP = 4 (to ensure distinct r1, r2, r3)
- **Small problems (D < 10)**: NP = 20-50
- **Large problems (D > 50)**: NP = 5D to 10D

### Mutation Factor (F)
- **Exploration (early)**: F = 0.8 - 1.0
- **Exploitation (late)**: F = 0.4 - 0.6
- **Adaptive**: Start high, reduce over time
- **Typical fixed value**: F = 0.5

### Crossover Rate (CR)
- **Low dimensional (D < 10)**: CR = 0.1 - 0.3
- **High dimensional (D > 50)**: CR = 0.8 - 0.95
- **Typical value**: CR = 0.9
- **Separable problems**: Higher CR
- **Non-separable problems**: Lower CR

### Strategy Selection
- **Unknown problem**: Start with DE/rand/1/bin
- **Fast convergence needed**: DE/best/1/bin
- **Multimodal landscape**: DE/rand/2/bin
- **Robust choice**: DE/current-to-best/1/bin

## Boundary Constraint Handling

**Methods**:
1. **Reflection**: Reflect back from boundary
2. **Wrapping**: Wrap around to opposite bound
3. **Resampling**: Generate new random value
4. **Clamping**: Set to boundary value (most common)

```cpp
// Clamping example
for (int j = 0; j < D; j++) {
    if (trial[j] < lower[j]) trial[j] = lower[j];
    if (trial[j] > upper[j]) trial[j] = upper[j];
}
```

## Benchmark Functions

### Standard Test Suite

1. **Sphere Function** (Unimodal, Separable):
   ```
   f(x) = Σᵢ₌₁ᴰ xᵢ²
   Global min: f(0,...,0) = 0
   ```

2. **Rosenbrock Function** (Unimodal, Non-separable):
   ```
   f(x) = Σᵢ₌₁ᴰ⁻¹ [100(xᵢ₊₁ - xᵢ²)² + (1 - xᵢ)²]
   Global min: f(1,...,1) = 0
   ```

3. **Rastrigin Function** (Multimodal, Separable):
   ```
   f(x) = 10D + Σᵢ₌₁ᴰ [xᵢ² - 10cos(2πxᵢ)]
   Global min: f(0,...,0) = 0
   ```

4. **Griewank Function** (Multimodal, Non-separable):
   ```
   f(x) = 1 + (1/4000)Σᵢ₌₁ᴰ xᵢ² - Πᵢ₌₁ᴰ cos(xᵢ/√i)
   Global min: f(0,...,0) = 0
   ```

5. **Ackley Function** (Multimodal):
   ```
   f(x) = -20exp(-0.2√((1/D)Σxᵢ²)) - exp((1/D)Σcos(2πxᵢ)) + 20 + e
   Global min: f(0,...,0) = 0
   ```

## Performance Metrics

1. **Solution Quality**: Best fitness achieved
2. **Convergence Speed**: Generations to target fitness
3. **Success Rate**: % of runs reaching target
4. **Reliability**: Std. deviation across runs
5. **Efficiency**: Function evaluations needed

## Practical Implementation Tips

### 1. **Initialization**
```cpp
// Latin Hypercube Sampling for better coverage
for (int i = 0; i < NP; i++) {
    for (int j = 0; j < D; j++) {
        population[i][j] = lower[j] + 
            (upper[j] - lower[j]) * latin_hypercube(i, j, NP);
    }
}
```

### 2. **Vectorization**
Use SIMD operations or matrix libraries for speed:
```cpp
// Vector operations for mutation
donor = pop[r1] + F * (pop[r2] - pop[r3]);
```

### 3. **Parallelization**
```cpp
#pragma omp parallel for
for (int i = 0; i < NP; i++) {
    // Mutation, crossover, selection for individual i
    // Fitness evaluations are independent
}
```

### 4. **Early Stopping**
```cpp
if (no_improvement_count > patience) {
    break;  // Stop if stagnated
}
```

## Academic References

### Foundational Papers:
1. Storn, R., & Price, K. (1997). "Differential evolution–a simple and efficient heuristic for global optimization over continuous spaces". *Journal of Global Optimization*, 11(4), 341-359.
2. Price, K. V., Storn, R. M., & Lampinen, J. A. (2005). *Differential Evolution: A Practical Approach to Global Optimization*. Springer.

### Adaptive Variants:
3. Brest, J., Greiner, S., Boskovic, B., Mernik, M., & Zumer, V. (2006). "Self-adapting control parameters in differential evolution". *IEEE Transactions on Evolutionary Computation*, 10(6), 646-657.
4. Zhang, J., & Sanderson, A. C. (2009). "JADE: adaptive differential evolution with optional external archive". *IEEE Transactions on Evolutionary Computation*, 13(5), 945-958.
5. Tanabe, R., & Fukunaga, A. (2013). "Success-history based parameter adaptation for differential evolution". *IEEE Congress on Evolutionary Computation*, 71-78.

### Theoretical Analysis:
6. Zaharie, D. (2009). "Influence of crossover on the behavior of differential evolution algorithms". *Applied Soft Computing*, 9(3), 1126-1138.
7. Gämperle, R., Müller, S. D., & Koumoutsakos, P. (2002). "A parameter study for differential evolution". *Advances in Intelligent Systems, Fuzzy Systems, Evolutionary Computation*, 293-298.

### Recent Surveys:
8. Das, S., & Suganthan, P. N. (2011). "Differential evolution: A survey of the state-of-the-art". *IEEE Transactions on Evolutionary Computation*, 15(1), 4-31.
9. Neri, F., & Tirronen, V. (2010). "Recent advances in differential evolution: a survey and experimental analysis". *Artificial Intelligence Review*, 33(1-2), 61-106.

### Applications:
10. Feoktistov, V. (2006). *Differential Evolution: In Search of Solutions*. Springer. (Applications in various domains)

## Getting Started

See the C++ implementation in this folder for a complete working example with benchmark functions.

## Further Reading

- **Books**:
  - Chakraborty, U. K. (Ed.). (2008). *Advances in Differential Evolution*. Springer.
  - Zelinka, I., Skanderova, L., & Snášel, V. (2019). *Handbook of Differential Evolution*. Springer.

- **Online Resources**:
  - [Differential Evolution Homepage](http://www.icsi.berkeley.edu/~storn/code.html)
  - [CEC Competitions](https://www.ntu.edu.sg/home/epnsugan/)

## Comparison with Other Algorithms

| Aspect | DE | GA | PSO | CMA-ES |
|--------|----|----|-----|--------|
| Convergence | Fast | Medium | Fast | Very Fast |
| Simplicity | Very High | Medium | High | Low |
| Parameters | 3 | 5-8 | 4-5 | Many |
| Robustness | High | Medium | Medium | Very High |
| Continuous Opt | Excellent | Good | Excellent | Excellent |
| Discrete Opt | Poor | Excellent | Poor | Poor |

---

*For questions or contributions, please refer to the main repository documentation.*
