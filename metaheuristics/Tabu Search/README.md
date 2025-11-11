# Tabu Search

## Overview

Tabu Search is an advanced local search metaheuristic that uses memory structures to guide the search process beyond local optimality. Unlike simple hill-climbing methods, Tabu Search strategically explores the solution space by maintaining a "tabu list" of recently visited solutions or moves, preventing cycling and encouraging exploration of new regions.

## Historical Background

Tabu Search was developed by **Fred Glover** at the University of Colorado Boulder in the mid-1980s. His pioneering work established TS as one of the most powerful optimization techniques for combinatorial problems.

### Key Contributors:
- **Fred Glover** (1937-) - Creator of Tabu Search
- **Manuel Laguna** - Co-developer and theoretical extensions
- **Rafael Martí** - Applications and adaptive mechanisms
- **Michel Gendreau** - Parallel implementations

### Timeline:
- **1986**: First publication by Glover
- **1989-1990**: Major theoretical developments
- **1997**: Comprehensive book "Tabu Search" by Glover & Laguna
- **2000s-Present**: Hybrid methods and industrial applications

## Theoretical Foundation

### Core Concepts

#### 1. **Memory Structures**
Tabu Search uses three types of memory:

**Short-term Memory (Tabu List)**:
- Records recently visited solutions or moves
- Prevents immediate cycling
- Length: typically 7-20 iterations

**Medium-term Memory (Intensification)**:
- Focuses search on promising regions
- Exploits good solution characteristics
- Frequency-based mechanisms

**Long-term Memory (Diversification)**:
- Drives search to unexplored regions
- Prevents stagnation
- Penalty-based or restart mechanisms

#### 2. **Aspiration Criteria**
Override tabu status when:
- Move leads to best solution found so far
- Move improves upon aspiration level
- Tabu restriction becomes ineffective

#### 3. **Neighborhood Structure**
Define N(x) as the set of solutions reachable from x:
```
N(x) = {x' : x' can be obtained from x by a move m}
```

### Mathematical Formulation

#### Basic Algorithm
```
Input: Initial solution x₀
Initialize: x* = x₀ (best solution)
           T = ∅ (tabu list)
           k = 0 (iteration counter)

While stopping criterion not met:
    1. Generate candidate set: C = N(x) \ T
    2. Select best move: x' = argmin{f(y) : y ∈ C}
    3. If aspiration criterion met:
          Allow tabu moves
    4. Update solution: x = x'
    5. Update best: if f(x) < f(x*): x* = x
    6. Update tabu list: T = T ∪ {attributes of x}
    7. k = k + 1

Return x*
```

#### Tabu Tenure Function
The tabu tenure (lifetime) can be:

**Static**: Fixed value τ
```
T(m) = constant (e.g., 7, 10, 20)
```

**Dynamic**: Adapts based on search progress
```
T(m) = τ₀ + random(0, r)
where τ₀ is base tenure, r is random component
```

**Adaptive**: Based on solution quality
```
T(m) = f(iteration_count, improvement_rate, problem_size)
```

## Key Parameters

| Parameter | Symbol | Typical Range | Description |
|-----------|--------|---------------|-------------|
| Tabu List Size | τ | 7-20 | Number of iterations a move stays tabu |
| Max Iterations | K | 100-10000 | Stopping criterion |
| Aspiration Level | A | Dynamic | Threshold for overriding tabu |
| Neighborhood Size | \|N(x)\| | Problem-dependent | Number of candidate moves |
| Intensification Frequency | f_int | Every 10-50 iter | How often to intensify |
| Diversification Frequency | f_div | Every 50-200 iter | How often to diversify |

## Algorithm Variants

### 1. **Reactive Tabu Search**
- Automatically adjusts tabu tenure based on search behavior
- Detects cycling through solution repetition
- **Reference**: Battiti, R., & Tecchiolli, G. (1994)

### 2. **Robust Tabu Search**
- Uses multiple diversification strategies
- Adaptive parameter adjustment
- **Reference**: Taillard, É. (1991)

### 3. **Granular Tabu Search**
- Restricts neighborhood based on problem structure
- Efficient for large-scale problems
- **Reference**: Toth, P., & Vigo, D. (2003)

### 4. **Probabilistic Tabu Search**
- Introduces randomness in move selection
- Balances determinism and stochasticity
- **Reference**: Glover, F., & Laguna, M. (1997)

## Industrial Applications

### 1. **Vehicle Routing and Logistics**
- **Problem**: Optimize delivery routes for fleet of vehicles
- **Application**: Minimize distance, fuel cost, and time
- **Industry Impact**: UPS, FedEx, DHL use TS-based systems
- **Example**: 
  - Problem: 100 customers, 10 vehicles, capacity constraints
  - Results: 15-25% reduction in total distance
  - Computational time: Minutes for real-time decisions

### 2. **Production Scheduling**
- **Job Shop Scheduling Problem (JSSP)**: Sequence jobs on machines
- **Flow Shop Scheduling**: Optimize production line flow
- **Application**: Minimize makespan, tardiness penalties
- **Example**: 
  - Automotive assembly line with 50 operations
  - Reduced idle time by 30%
  - Improved on-time delivery from 85% to 97%

### 3. **Telecommunications Network Design**
- **Network Topology Optimization**: Minimize cost while ensuring connectivity
- **Bandwidth Allocation**: Distribute resources efficiently
- **Application**: Design fiber optic networks
- **Example**:
  - 100-node network design
  - 40% cost reduction compared to greedy methods
  - Maintains reliability requirements

### 4. **Nurse Scheduling**
- **Problem**: Assign nurses to shifts considering preferences and regulations
- **Constraints**: Coverage requirements, labor laws, skill levels
- **Application**: Hospital staff management
- **Example**:
  - 50 nurses, 3 shifts/day, 28-day period
  - Improved nurse satisfaction by 35%
  - Reduced overtime costs by 20%

### 5. **Exam Timetabling**
- **Problem**: Schedule exams avoiding conflicts
- **Constraints**: Room capacity, student conflicts, fairness
- **Application**: University examination scheduling
- **Example**:
  - 2000 students, 50 exams, 20 rooms
  - Zero conflicts achieved
  - Reduced exam period by 2 days

### 6. **Portfolio Selection**
- **Problem**: Select optimal subset of assets
- **Constraints**: Budget, risk limits, sector diversification
- **Application**: Investment fund management
- **Example**:
  - Universe of 500 stocks
  - Select 20-30 for portfolio
  - Improved Sharpe ratio from 1.2 to 1.8

### 7. **Warehouse Layout Optimization**
- **Problem**: Arrange products to minimize picking time
- **Application**: E-commerce fulfillment centers
- **Example**:
  - Amazon warehouse with 10,000+ SKUs
  - 25% reduction in average picking time
  - Significant impact on customer delivery times

## Advantages and Limitations

### Advantages:
✓ Highly effective for combinatorial problems  
✓ Flexible framework adaptable to many domains  
✓ Short-term memory prevents cycling  
✓ Long-term memory enables strategic search  
✓ Can escape deep local optima  
✓ Deterministic (reproducible results)  
✓ Good performance on structured problems  

### Limitations:
✗ Parameter tuning can be complex  
✗ Neighborhood definition is problem-specific  
✗ Memory management overhead  
✗ Less effective on continuous problems  
✗ Sequential nature limits parallelization  
✗ Requires domain knowledge for good neighborhoods  

## Implementation Strategies

### Efficient Data Structures

1. **Tabu List Implementation**:
   - Circular buffer (fixed size)
   - Hash table (for quick lookup)
   - Attribute-based (store move characteristics, not solutions)

2. **Neighborhood Evaluation**:
   - Incremental evaluation (avoid full recalculation)
   - Delta evaluation (compute only changes)
   - Candidate list strategies

### Example: Move-based Tabu List
```python
tabu_list = []
tabu_tenure = 10

def is_tabu(move):
    return move in tabu_list

def add_to_tabu(move):
    tabu_list.append(move)
    if len(tabu_list) > tabu_tenure:
        tabu_list.pop(0)  # Remove oldest
```

## Intensification and Diversification

### Intensification (Exploitation)
**When**: After finding good regions
**How**:
- Return to best solutions
- Reduce tabu tenure temporarily
- Focus on promising neighborhoods
- Frequency-based: revisit frequently successful moves

### Diversification (Exploration)
**When**: Search stagnates
**How**:
- Penalize frequently used attributes
- Restart from different regions
- Increase tabu tenure
- Random perturbations

### Balance Strategy
```
If no improvement for k_int iterations:
    Trigger intensification
If no improvement for k_div iterations:
    Trigger diversification
```

## Benchmark Problems

### 1. **Traveling Salesman Problem (TSP)**
- Classic combinatorial optimization
- Neighborhood: 2-opt, 3-opt moves
- Best known solutions available for comparison

### 2. **Quadratic Assignment Problem (QAP)**
- Assign facilities to locations
- Minimize total assignment cost
- Very difficult problem class

### 3. **Graph Coloring**
- Color nodes such that adjacent nodes differ
- Minimize number of colors
- Applications in scheduling

### 4. **Max-Cut Problem**
- Partition graph to maximize cut edges
- NP-hard optimization problem
- Applications in VLSI design

## Parameter Tuning Guidelines

### Tabu Tenure (τ)
- **Small problems (n < 50)**: τ = 5-10
- **Medium problems (50 ≤ n ≤ 200)**: τ = 10-20
- **Large problems (n > 200)**: τ = 20-50
- **Rule of thumb**: τ ≈ √n

### Iteration Budget
- **Quick solutions**: K = 100 × n
- **Quality solutions**: K = 1000 × n
- **Thorough search**: K = 10000 × n

### Aspiration Criterion
- **Default**: Accept if better than best known
- **Aggressive**: Accept if better than current
- **Conservative**: Accept only if significantly better

## Academic References

### Foundational Papers:
1. Glover, F. (1986). "Future paths for integer programming and links to artificial intelligence". *Computers & Operations Research*, 13(5), 533-549.
2. Glover, F. (1989). "Tabu search—part I". *ORSA Journal on Computing*, 1(3), 190-206.
3. Glover, F. (1990). "Tabu search—part II". *ORSA Journal on Computing*, 2(1), 4-32.

### Comprehensive Texts:
4. Glover, F., & Laguna, M. (1997). *Tabu Search*. Kluwer Academic Publishers.
5. Gendreau, M., & Potvin, J. Y. (Eds.). (2010). *Handbook of Metaheuristics* (2nd ed.). Springer. (Chapter on Tabu Search)

### Theoretical Analysis:
6. Battiti, R., & Tecchiolli, G. (1994). "The reactive tabu search". *ORSA Journal on Computing*, 6(2), 126-140.
7. Taillard, É. D. (1991). "Robust taboo search for the quadratic assignment problem". *Parallel Computing*, 17(4-5), 443-455.

### Recent Surveys:
8. Gendreau, M. (2003). "An introduction to tabu search". *Handbook of Metaheuristics*, 37-54.
9. Bouhmala, N. (2021). "A survey of tabu search meta-heuristic". *International Journal of Applied Metaheuristic Computing*, 12(4), 1-29.

### Applications:
10. Dell'Amico, M., & Trubian, M. (1993). "Applying tabu search to the job-shop scheduling problem". *Annals of Operations Research*, 41(3), 231-252.

## Getting Started

Refer to the specific implementation folders for code examples:
- [Python Implementation - Version 1](Tabu%20Search%20-%20py/README.md)
- [Python Implementation - Version 2](Tabu%20Search%20-%20py2/README.md)

## Further Reading

- **Books**:
  - Hertz, A., Taillard, E., & de Werra, D. (1997). "Tabu search". In *Local Search in Combinatorial Optimization* (pp. 121-136).
  - Aarts, E., & Lenstra, J. K. (Eds.). (2003). *Local Search in Combinatorial Optimization*. Princeton University Press.

- **Online Resources**:
  - [OptimizationOnline.org](http://www.optimizationonline.org)
  - [INFORMS Journal on Computing](https://pubsonline.informs.org/journal/ijoc)

---

*For questions or contributions, please refer to the main repository documentation.*
