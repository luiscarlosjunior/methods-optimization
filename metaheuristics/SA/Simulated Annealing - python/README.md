# Simulated Annealing - Python Implementation

## Overview

This directory contains Python implementations of Simulated Annealing for both continuous and discrete optimization problems. The implementations demonstrate SA application to classic benchmark problems including the Himmelblau function and the Quadratic Assignment Problem (QAP).

## Implementation Details

### Author
**Luis Carlos Junior** (2020)

### Language
Python 3.x with NumPy and Matplotlib

## Files Description

### 1. `SM-continuous problem - The Himmelblau Function.py`
Basic example demonstrating SA setup for the Himmelblau function.

**Himmelblau Function**:
```
f(x,y) = (x² + y - 11)² + (x + y² - 7)²
```

**Global Minima** (all equal to 0):
- f(3.0, 2.0) = 0
- f(-2.805118, 3.131312) = 0
- f(-3.779310, -3.283186) = 0
- f(3.584428, -1.848126) = 0

**Features**:
- 2-dimensional continuous optimization
- Multiple global minima (4 locations)
- Common benchmark for metaheuristics
- Non-convex, multimodal landscape

### 2. `SM-continuous problem - The Himmelblau Function - Set parameters.py`
Complete SA implementation with configurable parameters for the Himmelblau function.

**Parameters**:
- Initial temperature (T₀)
- Cooling rate (α)
- Iterations per temperature
- Stopping criteria

**Outputs**:
- Best solution found
- Convergence plot
- Temperature schedule visualization

### 3. `QAP,+SA.py`
Simulated Annealing for the Quadratic Assignment Problem.

**Quadratic Assignment Problem (QAP)**:
- Assign n facilities to n locations
- Minimize total assignment cost
- Cost = flow between facilities × distance between locations
- NP-hard combinatorial problem

**Formulation**:
```
minimize: Σᵢ Σⱼ fᵢⱼ × dₚ(ᵢ)ₚ(ⱼ)
where:
  fᵢⱼ = flow from facility i to facility j
  dₖₗ = distance from location k to location l
  p(i) = location assigned to facility i
```

**Features**:
- Discrete optimization
- Permutation-based solution representation
- Swap-based neighborhood structure
- Classic SA application

### 4. `SM-continuous problem - QAP.py`
Alternative QAP implementation with continuous relaxation approach.

## Dependencies

```bash
pip install numpy matplotlib
```

Or using conda:
```bash
conda install numpy matplotlib
```

## Usage Examples

### Example 1: Himmelblau Function Optimization

```python
import numpy as np
import matplotlib.pyplot as plt

# Himmelblau function
def himmelblau(x, y):
    return ((x**2) + y - 11)**2 + (x + (y**2) - 7)**2

# Neighbor generation (small perturbation)
def neighbor(x, y, step_size=0.5):
    x_new = x + np.random.uniform(-step_size, step_size)
    y_new = y + np.random.uniform(-step_size, step_size)
    return x_new, y_new

# Simulated Annealing
def simulated_annealing_himmelblau():
    # Initial solution
    x, y = 0.0, 0.0
    current_cost = himmelblau(x, y)
    best_x, best_y = x, y
    best_cost = current_cost
    
    # SA parameters
    T = 100.0           # Initial temperature
    T_min = 0.01        # Minimum temperature
    alpha = 0.95        # Cooling rate
    iterations = 100    # Iterations per temperature
    
    # History for plotting
    history = []
    
    # Main loop
    while T > T_min:
        for _ in range(iterations):
            # Generate neighbor
            x_new, y_new = neighbor(x, y)
            new_cost = himmelblau(x_new, y_new)
            
            # Calculate change
            delta = new_cost - current_cost
            
            # Acceptance criterion
            if delta < 0 or np.random.random() < np.exp(-delta / T):
                x, y = x_new, y_new
                current_cost = new_cost
                
                # Update best
                if current_cost < best_cost:
                    best_x, best_y = x, y
                    best_cost = current_cost
        
        # Record and cool
        history.append(best_cost)
        T *= alpha
    
    # Results
    print(f"Best solution: x = {best_x:.6f}, y = {best_y:.6f}")
    print(f"Best cost: {best_cost:.6f}")
    
    # Plot convergence
    plt.figure(figsize=(10, 6))
    plt.semilogy(history)
    plt.xlabel('Temperature Step')
    plt.ylabel('Best Cost (log scale)')
    plt.title('SA Convergence - Himmelblau Function')
    plt.grid(True)
    plt.show()
    
    return best_x, best_y, best_cost

# Run
best_x, best_y, best_cost = simulated_annealing_himmelblau()
```

### Example 2: Quadratic Assignment Problem

```python
import numpy as np

def qap_cost(assignment, flow, distance):
    """
    Calculate QAP objective value
    assignment: permutation of facilities
    flow: flow matrix (n x n)
    distance: distance matrix (n x n)
    """
    n = len(assignment)
    cost = 0
    for i in range(n):
        for j in range(n):
            cost += flow[i][j] * distance[assignment[i]][assignment[j]]
    return cost

def swap_neighbor(assignment):
    """Generate neighbor by swapping two random positions"""
    neighbor = assignment.copy()
    i, j = np.random.choice(len(assignment), 2, replace=False)
    neighbor[i], neighbor[j] = neighbor[j], neighbor[i]
    return neighbor

def simulated_annealing_qap(flow, distance):
    n = len(flow)
    
    # Initial random assignment
    current = np.random.permutation(n)
    current_cost = qap_cost(current, flow, distance)
    best = current.copy()
    best_cost = current_cost
    
    # SA parameters
    T = 100.0
    T_min = 0.1
    alpha = 0.98
    iterations = 50
    
    while T > T_min:
        for _ in range(iterations):
            # Generate and evaluate neighbor
            candidate = swap_neighbor(current)
            candidate_cost = qap_cost(candidate, flow, distance)
            
            # Acceptance decision
            delta = candidate_cost - current_cost
            if delta < 0 or np.random.random() < np.exp(-delta / T):
                current = candidate
                current_cost = candidate_cost
                
                if current_cost < best_cost:
                    best = current.copy()
                    best_cost = current_cost
        
        T *= alpha
    
    return best, best_cost

# Example: 5x5 QAP instance
n = 5

# Flow matrix (facility interaction)
flow = np.array([
    [0, 5, 2, 4, 1],
    [5, 0, 3, 0, 2],
    [2, 3, 0, 0, 0],
    [4, 0, 0, 0, 5],
    [1, 2, 0, 5, 0]
])

# Distance matrix (location separation)
distance = np.array([
    [0, 1, 2, 3, 1],
    [1, 0, 1, 2, 2],
    [2, 1, 0, 1, 3],
    [3, 2, 1, 0, 4],
    [1, 2, 3, 4, 0]
])

# Solve
best_assignment, best_cost = simulated_annealing_qap(flow, distance)
print(f"Best assignment: {best_assignment}")
print(f"Best cost: {best_cost}")
```

## Industrial Application Example: Warehouse Layout Optimization

```python
import numpy as np

def warehouse_layout_optimization():
    """
    Optimize warehouse storage locations to minimize picking time.
    
    Problem: Assign 20 products to 20 storage locations
    Objective: Minimize total distance × frequency
    """
    
    # Product picking frequency (how often each product is picked)
    frequency = np.random.randint(1, 100, size=20)
    
    # Distance matrix (distances between locations)
    # Locations arranged in a grid: 4 rows × 5 columns
    def calculate_distance_matrix(rows=4, cols=5):
        n = rows * cols
        dist = np.zeros((n, n))
        for i in range(n):
            row_i, col_i = i // cols, i % cols
            for j in range(n):
                row_j, col_j = j // cols, j % cols
                # Manhattan distance
                dist[i][j] = abs(row_i - row_j) + abs(col_i - col_j)
        return dist
    
    distance = calculate_distance_matrix()
    
    # Create flow matrix (products picked together frequently)
    # Higher values = picked together more often
    flow = np.outer(frequency, frequency)
    np.fill_diagonal(flow, 0)
    
    # Solve using SA
    best_layout, cost = simulated_annealing_qap(flow, distance)
    
    # Analyze results
    print("Warehouse Layout Optimization Results")
    print("=" * 50)
    print(f"Total picking distance-frequency cost: {cost:.0f}")
    print("\nOptimal Layout (product -> location):")
    for product_id, location_id in enumerate(best_layout):
        row = location_id // 5
        col = location_id % 5
        freq = frequency[product_id]
        print(f"Product {product_id:2d} (freq={freq:3d}) -> " +
              f"Location {location_id:2d} (Row {row}, Col {col})")
    
    # High-frequency products should be near entrance (location 0)
    high_freq_products = np.argsort(frequency)[-5:]  # Top 5
    avg_distance_high_freq = np.mean([distance[0][best_layout[p]] 
                                       for p in high_freq_products])
    print(f"\nAverage distance of top 5 products from entrance: " +
          f"{avg_distance_high_freq:.2f}")
    
    return best_layout, cost

# Run warehouse optimization
warehouse_layout_optimization()
```

## Parameter Tuning Recommendations

### For Himmelblau Function (Continuous, Smooth)
```python
T_initial = 100.0      # High initial temperature
T_min = 0.01          # Low final temperature
alpha = 0.95          # Slow cooling (good quality)
iterations = 100      # Moderate iterations per temp
step_size = 0.5       # Neighbor generation range
```

### For QAP (Discrete, Combinatorial)
```python
T_initial = 100.0      # Start with high acceptance
T_min = 0.1           # Stop when very selective
alpha = 0.98          # Very slow cooling (best quality)
iterations = 50       # Fewer iterations per temp
swap_strategy = "random_swap"  # Neighborhood type
```

## Visualization

### 3D Surface Plot of Himmelblau Function
```python
from mpl_toolkits.mplot3d import Axes3D

# Create meshgrid
x = np.linspace(-5, 5, 200)
y = np.linspace(-5, 5, 200)
X, Y = np.meshgrid(x, y)
Z = himmelblau(X, Y)

# Plot
fig = plt.figure(figsize=(12, 8))
ax = fig.add_subplot(111, projection='3d')
surf = ax.plot_surface(X, Y, Z, cmap='viridis', alpha=0.8)
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('f(x,y)')
ax.set_title('Himmelblau Function')
fig.colorbar(surf)
plt.show()
```

### Contour Plot with SA Path
```python
# Track SA path
path_x, path_y = [], []

# Modified SA to record path
# ... (add path_x.append(x), path_y.append(y) in loop)

# Plot
plt.figure(figsize=(10, 8))
plt.contour(X, Y, Z, levels=50, cmap='coolwarm')
plt.plot(path_x, path_y, 'ko-', markersize=3, linewidth=0.5, 
         label='SA Path', alpha=0.6)
plt.plot(path_x[0], path_y[0], 'go', markersize=10, label='Start')
plt.plot(path_x[-1], path_y[-1], 'ro', markersize=10, label='End')
plt.xlabel('x')
plt.ylabel('y')
plt.title('SA Trajectory on Himmelblau Function')
plt.colorbar(label='f(x,y)')
plt.legend()
plt.grid(True, alpha=0.3)
plt.show()
```

## Performance Notes

- **Himmelblau Function**: SA typically finds a global minimum within 5000-10000 function evaluations
- **QAP**: Performance depends heavily on problem size; SA competitive for n < 30
- **Cooling Schedule**: Slow cooling (α = 0.95-0.99) gives better quality but takes longer
- **Initial Temperature**: Should accept ~80% of worse moves initially

## Common Issues and Solutions

### Issue: Not finding global optimum
**Solution**: 
- Increase initial temperature
- Decrease cooling rate (slower cooling)
- Increase iterations per temperature

### Issue: Converging too slowly
**Solution**:
- Increase cooling rate
- Decrease iterations per temperature
- Consider adaptive cooling schedule

### Issue: Solution quality varies widely
**Solution**:
- Run multiple times with different seeds
- Use restart strategy
- Implement adaptive temperature schedule

## References

1. **Himmelblau Function**:
   - Himmelblau, D. M. (1972). *Applied Nonlinear Programming*. McGraw-Hill.

2. **Quadratic Assignment Problem**:
   - Koopmans, T. C., & Beckmann, M. (1957). "Assignment problems and the location of economic activities". *Econometrica*, 53-76.
   - Burkard, R. E., Dell'Amico, M., & Martello, S. (2012). *Assignment Problems*. SIAM.

3. **SA for QAP**:
   - Connolly, D. T. (1990). "An improved annealing scheme for the QAP". *European Journal of Operational Research*, 46(1), 93-100.

## See Also

- [Main SA Documentation](../README.md)
- [C++ Implementation](../Simulated%20Annealing%20-%20CPP/README.md)

---

*For questions or bug reports, please refer to the main repository.*
