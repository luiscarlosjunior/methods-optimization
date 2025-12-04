# Tabu Search - Python Implementation

## Overview

This directory contains Python implementations of Tabu Search for combinatorial optimization problems. The implementations demonstrate TS applications with various neighborhood structures and tabu list management strategies.

## Implementation Details

### Author
**Luis Carlos Junior** and contributors (2020)

### Language
Python 3.x with NumPy

## Files Description

### 1. `ts.py`
Main Tabu Search implementation with complete algorithm structure.

**Features**:
- Configurable tabu list size
- Multiple neighborhood operators
- Aspiration criteria
- Best solution tracking
- Iteration history

### 2. Supporting Files
Additional utility scripts for specific problem instances and testing.

## Core Algorithm Implementation

### Basic Tabu Search Structure

```python
import numpy as np
from collections import deque

class TabuSearch:
    def __init__(self, initial_solution, objective_func, 
                 neighbor_func, tabu_tenure=10, max_iterations=1000):
        """
        Initialize Tabu Search
        
        Parameters:
        -----------
        initial_solution : array-like
            Starting solution
        objective_func : callable
            Function to minimize f(solution)
        neighbor_func : callable
            Function to generate neighbors
        tabu_tenure : int
            Number of iterations a move stays tabu
        max_iterations : int
            Maximum number of iterations
        """
        self.current = initial_solution
        self.best = initial_solution.copy()
        self.objective_func = objective_func
        self.neighbor_func = neighbor_func
        self.tabu_list = deque(maxlen=tabu_tenure)
        self.max_iterations = max_iterations
        
        self.best_cost = objective_func(self.best)
        self.current_cost = self.best_cost
        
    def is_tabu(self, move):
        """Check if a move is in the tabu list"""
        return move in self.tabu_list
    
    def aspiration_criteria(self, solution, cost):
        """Check if solution overrides tabu status"""
        return cost < self.best_cost
    
    def search(self):
        """Execute Tabu Search"""
        history = []
        
        for iteration in range(self.max_iterations):
            # Generate candidate neighbors
            neighbors = self.neighbor_func(self.current)
            
            # Find best non-tabu neighbor
            best_neighbor = None
            best_neighbor_cost = float('inf')
            best_move = None
            
            for neighbor, move in neighbors:
                cost = self.objective_func(neighbor)
                
                # Accept if not tabu or meets aspiration criteria
                if not self.is_tabu(move) or self.aspiration_criteria(neighbor, cost):
                    if cost < best_neighbor_cost:
                        best_neighbor = neighbor
                        best_neighbor_cost = cost
                        best_move = move
            
            # Update current solution
            if best_neighbor is not None:
                self.current = best_neighbor
                self.current_cost = best_neighbor_cost
                self.tabu_list.append(best_move)
                
                # Update best solution
                if best_neighbor_cost < self.best_cost:
                    self.best = best_neighbor.copy()
                    self.best_cost = best_neighbor_cost
            
            # Record history
            history.append(self.best_cost)
            
            # Display progress
            if iteration % 50 == 0:
                print(f"Iteration {iteration}: Best Cost = {self.best_cost:.4f}")
        
        return self.best, self.best_cost, history
```

## Industrial Application Examples

### Example 1: Traveling Salesman Problem (TSP)

```python
import numpy as np

def tsp_distance(tour, distance_matrix):
    """Calculate total tour distance"""
    total = 0
    for i in range(len(tour)):
        j = (i + 1) % len(tour)
        total += distance_matrix[tour[i]][tour[j]]
    return total

def tsp_neighbors_2opt(tour):
    """Generate neighbors using 2-opt swaps"""
    neighbors = []
    n = len(tour)
    
    for i in range(n - 1):
        for j in range(i + 2, n):
            # Create neighbor by reversing segment [i+1, j]
            neighbor = tour.copy()
            neighbor[i+1:j+1] = reversed(neighbor[i+1:j+1])
            
            # Move representation: (i, j) swap
            move = (i, j)
            neighbors.append((neighbor, move))
    
    return neighbors

def solve_tsp_tabu(distance_matrix, initial_tour=None, 
                   tabu_tenure=20, max_iterations=1000):
    """
    Solve TSP using Tabu Search
    
    Example: Vehicle routing for 20-city delivery problem
    """
    n = len(distance_matrix)
    
    # Initial solution: random tour
    if initial_tour is None:
        initial_tour = list(range(n))
        np.random.shuffle(initial_tour)
    
    # Objective function
    def objective(tour):
        return tsp_distance(tour, distance_matrix)
    
    # Initialize TS
    ts = TabuSearch(
        initial_solution=initial_tour,
        objective_func=objective,
        neighbor_func=tsp_neighbors_2opt,
        tabu_tenure=tabu_tenure,
        max_iterations=max_iterations
    )
    
    # Solve
    best_tour, best_distance, history = ts.search()
    
    return best_tour, best_distance, history

# Example: 10-city problem
np.random.seed(42)
n_cities = 10

# Random city coordinates
cities = np.random.rand(n_cities, 2) * 100

# Calculate distance matrix
distance_matrix = np.zeros((n_cities, n_cities))
for i in range(n_cities):
    for j in range(n_cities):
        distance_matrix[i][j] = np.linalg.norm(cities[i] - cities[j])

# Solve
best_tour, best_distance, history = solve_tsp_tabu(distance_matrix)

print(f"\nBest tour: {best_tour}")
print(f"Best distance: {best_distance:.2f}")

# Plot convergence
import matplotlib.pyplot as plt
plt.figure(figsize=(10, 6))
plt.plot(history)
plt.xlabel('Iteration')
plt.ylabel('Best Tour Distance')
plt.title('Tabu Search Convergence - TSP')
plt.grid(True)
plt.show()
```

### Example 2: Job Shop Scheduling

```python
def job_shop_makespan(schedule, processing_times, machine_order):
    """
    Calculate makespan for job shop schedule
    
    schedule: list of job sequences per machine
    processing_times: dict of (job, machine) -> time
    machine_order: dict of job -> [machines in order]
    """
    n_jobs = len(machine_order)
    n_machines = len(schedule)
    
    # Track completion time for each job on each machine
    job_completion = {j: [0] * len(machine_order[j]) for j in range(n_jobs)}
    machine_available = [0] * n_machines
    
    # Simulate schedule
    for machine in range(n_machines):
        for job in schedule[machine]:
            # Find which operation this is for the job
            machine_idx = machine_order[job].index(machine)
            
            # Start time is max of:
            # 1. Machine available time
            # 2. Previous operation completion time
            start_time = machine_available[machine]
            if machine_idx > 0:
                prev_machine = machine_order[job][machine_idx - 1]
                start_time = max(start_time, job_completion[job][machine_idx - 1])
            
            # Process
            process_time = processing_times[(job, machine)]
            completion_time = start_time + process_time
            
            # Update
            job_completion[job][machine_idx] = completion_time
            machine_available[machine] = completion_time
    
    # Makespan is maximum completion time
    makespan = max(machine_available)
    return makespan

def job_shop_neighbors(schedule):
    """Generate neighbors by swapping adjacent jobs"""
    neighbors = []
    
    for machine in range(len(schedule)):
        if len(schedule[machine]) < 2:
            continue
        
        for i in range(len(schedule[machine]) - 1):
            # Swap adjacent jobs
            neighbor = [seq.copy() for seq in schedule]
            neighbor[machine][i], neighbor[machine][i+1] = \
                neighbor[machine][i+1], neighbor[machine][i]
            
            move = (machine, i)  # Machine and position
            neighbors.append((neighbor, move))
    
    return neighbors

# Example: 5 jobs, 3 machines
# Each job must visit machines in specific order
processing_times = {
    (0, 0): 3, (0, 1): 2, (0, 2): 4,  # Job 0
    (1, 0): 2, (1, 1): 4, (1, 2): 3,  # Job 1
    (2, 0): 4, (2, 1): 3, (2, 2): 2,  # Job 2
    (3, 0): 3, (3, 1): 3, (3, 2): 3,  # Job 3
    (4, 0): 2, (4, 1): 2, (4, 2): 4,  # Job 4
}

machine_order = {
    0: [0, 1, 2],  # Job 0: M0 -> M1 -> M2
    1: [1, 0, 2],  # Job 1: M1 -> M0 -> M2
    2: [0, 2, 1],  # Job 2: M0 -> M2 -> M1
    3: [1, 2, 0],  # Job 3: M1 -> M2 -> M0
    4: [2, 0, 1],  # Job 4: M2 -> M0 -> M1
}

# Initial schedule (random)
initial_schedule = [
    [0, 2],     # Machine 0
    [1, 3, 4],  # Machine 1
    [2, 4],     # Machine 2
]

# Solve with Tabu Search
ts = TabuSearch(
    initial_solution=initial_schedule,
    objective_func=lambda s: job_shop_makespan(s, processing_times, machine_order),
    neighbor_func=job_shop_neighbors,
    tabu_tenure=7,
    max_iterations=500
)

best_schedule, best_makespan, history = ts.search()

print(f"\nBest schedule found:")
for machine, jobs in enumerate(best_schedule):
    print(f"Machine {machine}: {jobs}")
print(f"Makespan: {best_makespan}")
```

### Example 3: Exam Timetabling

```python
def exam_timetabling():
    """
    Schedule university exams to minimize conflicts
    
    Constraints:
    - Students can't have two exams in same timeslot
    - Room capacity limits
    - Preference to spread exams for same students
    """
    
    # Example data
    n_exams = 20
    n_timeslots = 10
    n_students = 100
    
    # Student enrollments (which exams each student is taking)
    enrollments = {}
    for student in range(n_students):
        # Each student takes 3-5 exams
        n_exams_student = np.random.randint(3, 6)
        enrollments[student] = set(np.random.choice(n_exams, n_exams_student, replace=False))
    
    def conflict_count(schedule):
        """Count student conflicts (same student, two exams, same slot)"""
        conflicts = 0
        
        for timeslot in range(n_timeslots):
            exams_in_slot = [e for e, t in enumerate(schedule) if t == timeslot]
            
            # Check each pair of exams in this slot
            for i in range(len(exams_in_slot)):
                for j in range(i+1, len(exams_in_slot)):
                    exam1, exam2 = exams_in_slot[i], exams_in_slot[j]
                    
                    # Count students taking both exams
                    for student in range(n_students):
                        if exam1 in enrollments[student] and exam2 in enrollments[student]:
                            conflicts += 1
        
        return conflicts
    
    def neighbors(schedule):
        """Generate neighbors by moving one exam to different timeslot"""
        neighbors = []
        
        for exam in range(n_exams):
            current_slot = schedule[exam]
            
            for new_slot in range(n_timeslots):
                if new_slot != current_slot:
                    neighbor = schedule.copy()
                    neighbor[exam] = new_slot
                    
                    move = (exam, current_slot, new_slot)
                    neighbors.append((neighbor, move))
        
        return neighbors
    
    # Initial solution: random assignment
    initial_schedule = np.random.randint(0, n_timeslots, n_exams).tolist()
    
    # Solve
    ts = TabuSearch(
        initial_solution=initial_schedule,
        objective_func=conflict_count,
        neighbor_func=neighbors,
        tabu_tenure=15,
        max_iterations=1000
    )
    
    best_schedule, conflicts, history = ts.search()
    
    print(f"\nExam Timetabling Results:")
    print(f"Number of conflicts: {conflicts}")
    print(f"\nSchedule (exam -> timeslot):")
    for exam, timeslot in enumerate(best_schedule):
        print(f"Exam {exam:2d} -> Timeslot {timeslot}")
    
    return best_schedule, conflicts, history

# Run
exam_timetabling()
```

## Parameter Tuning Guidelines

### Tabu Tenure
- **Small problems (n < 50)**: tenure = 5-10
- **Medium problems (50 ≤ n ≤ 200)**: tenure = 10-20
- **Large problems (n > 200)**: tenure = 20-50
- **Adaptive**: tenure = √n

### Max Iterations
- **Quick exploration**: 100-500
- **Quality solution**: 1000-5000
- **Thorough search**: 10000+

### Aspiration Criteria
Always accept if better than best known solution found so far.

## Advanced Features

### Intensification
```python
def intensify(ts, best_region_solution):
    """Return to best regions and search more thoroughly"""
    ts.current = best_region_solution
    ts.tabu_list.clear()
    # Reduce tabu tenure temporarily
    ts.tabu_list = deque(maxlen=5)
```

### Diversification
```python
def diversify(ts):
    """Escape current region by random restart"""
    # Penalize frequently used attributes
    # or restart from new random solution
    pass
```

### Adaptive Tabu Tenure
```python
def adaptive_tenure(iteration, improvement_count):
    """Adjust tabu tenure based on search progress"""
    if improvement_count < 10:
        # Increase exploration
        return min(30, tenure + 2)
    else:
        # Increase exploitation
        return max(5, tenure - 1)
```

## Performance Tips

1. **Efficient Neighbor Generation**: Don't generate all neighbors if not needed
2. **Incremental Evaluation**: Reuse computation from previous evaluation
3. **Candidate List Strategy**: Evaluate only promising subset of neighbors
4. **Hash-based Tabu List**: For O(1) lookup instead of O(n)

## References

1. **Tabu Search Fundamentals**:
   - Glover, F., & Laguna, M. (1997). *Tabu Search*. Kluwer Academic Publishers.

2. **TSP with Tabu Search**:
   - Knox, J. (1994). "Tabu search performance on the symmetric traveling salesman problem". *Computers & Operations Research*, 21(8), 867-876.

3. **Job Shop Scheduling**:
   - Dell'Amico, M., & Trubian, M. (1993). "Applying tabu search to the job-shop scheduling problem". *Annals of Operations Research*, 41(3), 231-252.

4. **Exam Timetabling**:
   - Burke, E. K., et al. (1996). "A memetic algorithm for university exam timetabling". *PATAT*, 241-250.

## See Also

- [Main Tabu Search Documentation](../README.md)
- [Alternative Python Implementation](../Tabu%20Search%20-%20py2/README.md)

## Advanced Implementation Topics

### Efficient Data Structures

Use appropriate data structures for performance:

```python
from collections import OrderedDict

class EfficientTabuList:
    """
    Hash-based tabu list with O(1) lookup and automatic expiration.
    """
    
    def __init__(self, tenure):
        self.tenure = tenure
        self.tabu_dict = OrderedDict()
        self.current_iteration = 0
    
    def add(self, move):
        """Add move with expiration iteration."""
        self.tabu_dict[move] = self.current_iteration + self.tenure
        self._cleanup()
    
    def is_tabu(self, move):
        """O(1) lookup."""
        if move not in self.tabu_dict:
            return False
        return self.tabu_dict[move] > self.current_iteration
    
    def _cleanup(self):
        """Remove expired entries."""
        expired = [k for k, v in self.tabu_dict.items() 
                   if v <= self.current_iteration]
        for k in expired:
            del self.tabu_dict[k]
    
    def step(self):
        """Advance iteration counter."""
        self.current_iteration += 1
```

### Multi-Threaded Neighborhood Evaluation

```python
from concurrent.futures import ThreadPoolExecutor
import numpy as np

class ParallelTabuSearch:
    """
    Tabu Search with parallel neighborhood evaluation.
    """
    
    def __init__(self, objective_func, neighbor_func, 
                 tabu_tenure=10, n_workers=4):
        self.objective = objective_func
        self.neighbor_func = neighbor_func
        self.tabu = EfficientTabuList(tabu_tenure)
        self.n_workers = n_workers
    
    def evaluate_neighbors_parallel(self, current):
        """Evaluate all neighbors in parallel."""
        neighbors = self.neighbor_func(current)
        
        with ThreadPoolExecutor(max_workers=self.n_workers) as executor:
            # Submit all evaluations
            futures = {
                executor.submit(self.objective, n): (n, m) 
                for n, m in neighbors
            }
            
            # Collect results
            results = []
            for future in futures:
                neighbor, move = futures[future]
                cost = future.result()
                results.append((neighbor, move, cost))
        
        return results
    
    def search(self, initial, max_iterations=1000):
        current = initial
        best = initial.copy()
        best_cost = self.objective(initial)
        
        for _ in range(max_iterations):
            # Evaluate neighbors in parallel
            evaluated = self.evaluate_neighbors_parallel(current)
            
            # Find best non-tabu neighbor
            candidates = [
                (n, m, c) for n, m, c in evaluated
                if not self.tabu.is_tabu(m) or c < best_cost  # Aspiration
            ]
            
            if candidates:
                best_neighbor, best_move, neighbor_cost = min(
                    candidates, key=lambda x: x[2]
                )
                
                current = best_neighbor
                self.tabu.add(best_move)
                
                if neighbor_cost < best_cost:
                    best = best_neighbor.copy()
                    best_cost = neighbor_cost
            
            self.tabu.step()
        
        return best, best_cost
```

### Visualization Utilities

```python
import matplotlib.pyplot as plt
import numpy as np

def plot_search_trajectory(history, title="Tabu Search Progress"):
    """
    Plot the search trajectory showing cost over iterations.
    """
    fig, axes = plt.subplots(2, 2, figsize=(14, 10))
    
    # 1. Cost over iterations
    ax1 = axes[0, 0]
    ax1.plot(history['costs'], 'b-', linewidth=1, alpha=0.7, label='Current')
    ax1.plot(history['best_costs'], 'r-', linewidth=2, label='Best')
    ax1.set_xlabel('Iteration')
    ax1.set_ylabel('Cost')
    ax1.set_title('Cost Evolution')
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    # 2. Improvement events
    ax2 = axes[0, 1]
    improvements = np.diff(history['best_costs'])
    improvement_iters = np.where(improvements < 0)[0]
    ax2.stem(improvement_iters, -improvements[improvement_iters])
    ax2.set_xlabel('Iteration')
    ax2.set_ylabel('Improvement')
    ax2.set_title('Improvement Events')
    ax2.grid(True, alpha=0.3)
    
    # 3. Tabu list size
    ax3 = axes[1, 0]
    ax3.plot(history['tabu_sizes'], 'g-', linewidth=1)
    ax3.set_xlabel('Iteration')
    ax3.set_ylabel('Tabu List Size')
    ax3.set_title('Tabu List Size Over Time')
    ax3.grid(True, alpha=0.3)
    
    # 4. Move acceptance rate
    ax4 = axes[1, 1]
    window = 50
    acceptance = np.convolve(
        history['accepted'], 
        np.ones(window)/window, 
        mode='valid'
    )
    ax4.plot(acceptance, 'm-', linewidth=1)
    ax4.set_xlabel('Iteration')
    ax4.set_ylabel('Acceptance Rate (moving avg)')
    ax4.set_title(f'Move Acceptance Rate (window={window})')
    ax4.grid(True, alpha=0.3)
    
    plt.suptitle(title, fontsize=14, fontweight='bold')
    plt.tight_layout()
    return fig

def visualize_tsp_solution(cities, tour, title="TSP Solution"):
    """
    Visualize a TSP tour on a 2D plane.
    """
    plt.figure(figsize=(10, 8))
    
    # Plot cities
    x = [cities[i][0] for i in range(len(cities))]
    y = [cities[i][1] for i in range(len(cities))]
    plt.scatter(x, y, c='red', s=100, zorder=5)
    
    # Label cities
    for i, (xi, yi) in enumerate(cities):
        plt.annotate(str(i), (xi, yi), xytext=(5, 5), 
                    textcoords='offset points', fontsize=10)
    
    # Draw tour
    for i in range(len(tour)):
        j = (i + 1) % len(tour)
        city1, city2 = tour[i], tour[j]
        plt.plot([cities[city1][0], cities[city2][0]], 
                [cities[city1][1], cities[city2][1]], 
                'b-', linewidth=2, alpha=0.7)
    
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.title(title)
    plt.grid(True, alpha=0.3)
    plt.axis('equal')
    plt.show()
```

### Integration with Optimization Libraries

```python
from scipy.optimize import OptimizeResult

def tabu_search_scipy_compatible(fun, x0, args=(), 
                                  tabu_tenure=10, 
                                  max_iterations=1000,
                                  neighbor_size=0.5):
    """
    Tabu Search with scipy.optimize compatible interface.
    
    Parameters
    ----------
    fun : callable
        Objective function to minimize
    x0 : array-like
        Initial solution
    args : tuple
        Extra arguments passed to fun
    tabu_tenure : int
        Number of iterations a move stays tabu
    max_iterations : int
        Maximum iterations
    neighbor_size : float
        Size of neighborhood perturbation
    
    Returns
    -------
    result : OptimizeResult
        Object containing optimization results
    """
    x0 = np.asarray(x0)
    
    def objective(x):
        return fun(x, *args)
    
    def generate_neighbors(x):
        neighbors = []
        for i in range(len(x)):
            for delta in [-neighbor_size, neighbor_size]:
                neighbor = x.copy()
                neighbor[i] += delta
                move = (i, delta)
                neighbors.append((neighbor, move))
        return neighbors
    
    ts = ParallelTabuSearch(objective, generate_neighbors, tabu_tenure)
    best, best_cost = ts.search(x0, max_iterations)
    
    return OptimizeResult(
        x=best,
        fun=best_cost,
        success=True,
        message="Tabu Search completed",
        nit=max_iterations
    )

# Usage:
# from scipy.optimize import rosen
# result = tabu_search_scipy_compatible(rosen, [0, 0, 0])
# print(result.x, result.fun)
```

---

*For questions or contributions, please refer to the main repository.*
