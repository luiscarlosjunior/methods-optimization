# Genetic Algorithm - MATLAB Implementation

## Overview

This directory contains MATLAB implementations of Genetic Algorithms for continuous and discrete optimization problems. MATLAB's rich numerical computing environment and visualization capabilities make it ideal for research, prototyping, and educational purposes in evolutionary computation.

## Implementation Details

### Author
**Luis Carlos Junior** and contributors (2020)

### Language
MATLAB (R2016b or later recommended)

### Features
- Clear, readable code for educational purposes
- Built-in visualization tools
- Integration with MATLAB Optimization Toolbox
- Vectorized operations for performance

## Implementation Approach

### Real-Valued Genetic Algorithm

For continuous optimization problems, this implementation uses:
- **Encoding**: Real-valued vectors (no binary encoding needed)
- **Selection**: Tournament selection or fitness-proportionate
- **Crossover**: Arithmetic crossover (blend-α)
- **Mutation**: Gaussian mutation
- **Replacement**: Elitist or generational

## Usage Examples

### Example 1: Basic Function Optimization

```matlab
%% Genetic Algorithm - Minimizing Sphere Function
clear; clc; close all;

%% Problem Definition
% Sphere function: f(x) = sum(x.^2)
fitnessFcn = @(x) sum(x.^2);
nvars = 5;              % Number of variables
lb = -10 * ones(1, nvars);  % Lower bounds
ub = 10 * ones(1, nvars);   % Upper bounds

%% GA Options
options = optimoptions('ga', ...
    'PopulationSize', 50, ...
    'MaxGenerations', 100, ...
    'CrossoverFraction', 0.8, ...
    'MutationFcn', {@mutationadaptfeasible, 0.1}, ...
    'SelectionFcn', @selectiontournament, ...
    'EliteCount', 2, ...
    'Display', 'iter', ...
    'PlotFcn', {@gaplotbestf, @gaplotdistance});

%% Run GA
[x, fval, exitflag, output] = ga(fitnessFcn, nvars, [], [], [], [], ...
                                 lb, ub, [], options);

%% Display Results
fprintf('\n=== Optimization Results ===\n');
fprintf('Best solution: [');
fprintf('%.4f ', x);
fprintf(']\n');
fprintf('Best fitness: %.6f\n', fval);
fprintf('Generations: %d\n', output.generations);
fprintf('Function evaluations: %d\n', output.funccount);
```

### Example 2: Custom GA Implementation

```matlab
function [bestSolution, bestFitness, history] = customGA(fitnessFcn, nvars, options)
%CUSTOMGA Custom Genetic Algorithm implementation
%   Demonstrates core GA concepts with clear, educational code

    % Extract options
    popSize = options.PopulationSize;
    maxGen = options.MaxGenerations;
    crossoverRate = options.CrossoverFraction;
    mutationRate = options.MutationRate;
    eliteCount = options.EliteCount;
    lb = options.LowerBound;
    ub = options.UpperBound;
    
    % Initialize population (random uniform)
    population = repmat(lb, popSize, 1) + ...
                 rand(popSize, nvars) .* repmat(ub - lb, popSize, 1);
    
    % Evaluate initial population
    fitness = zeros(popSize, 1);
    for i = 1:popSize
        fitness(i) = fitnessFcn(population(i, :));
    end
    
    % Track best solution
    [bestFitness, bestIdx] = min(fitness);
    bestSolution = population(bestIdx, :);
    history = zeros(maxGen, 1);
    
    % Main loop
    for gen = 1:maxGen
        % Sort by fitness
        [fitness, sortIdx] = sort(fitness);
        population = population(sortIdx, :);
        
        % Elite preservation
        newPopulation = population(1:eliteCount, :);
        
        % Generate offspring
        while size(newPopulation, 1) < popSize
            % Selection (tournament)
            parent1 = tournamentSelection(population, fitness, 3);
            parent2 = tournamentSelection(population, fitness, 3);
            
            % Crossover
            if rand() < crossoverRate
                [child1, child2] = arithmeticCrossover(parent1, parent2, 0.5);
            else
                child1 = parent1;
                child2 = parent2;
            end
            
            % Mutation
            child1 = gaussianMutation(child1, mutationRate, lb, ub);
            child2 = gaussianMutation(child2, mutationRate, lb, ub);
            
            % Add to new population
            newPopulation = [newPopulation; child1; child2];
        end
        
        % Trim to population size
        newPopulation = newPopulation(1:popSize, :);
        population = newPopulation;
        
        % Evaluate new population
        for i = 1:popSize
            fitness(i) = fitnessFcn(population(i, :));
        end
        
        % Update best
        [minFit, minIdx] = min(fitness);
        if minFit < bestFitness
            bestFitness = minFit;
            bestSolution = population(minIdx, :);
        end
        
        % Record history
        history(gen) = bestFitness;
        
        % Display progress
        if mod(gen, 10) == 0
            fprintf('Generation %d: Best Fitness = %.6f\n', gen, bestFitness);
        end
    end
end

function parent = tournamentSelection(population, fitness, tournamentSize)
    % Select random individuals
    popSize = size(population, 1);
    candidates = randperm(popSize, tournamentSize);
    
    % Find best among candidates
    [~, bestIdx] = min(fitness(candidates));
    parent = population(candidates(bestIdx), :);
end

function [child1, child2] = arithmeticCrossover(parent1, parent2, alpha)
    % Arithmetic crossover
    child1 = alpha * parent1 + (1 - alpha) * parent2;
    child2 = (1 - alpha) * parent1 + alpha * parent2;
end

function child = gaussianMutation(parent, mutationRate, lb, ub)
    % Gaussian mutation
    child = parent;
    mutate = rand(size(parent)) < mutationRate;
    child(mutate) = child(mutate) + randn(sum(mutate), 1) * 0.1 * (ub - lb)';
    
    % Apply bounds
    child = max(lb, min(ub, child));
end
```

### Example 3: Multi-Objective Optimization

```matlab
%% Multi-Objective GA using gamultiobj
clear; clc; close all;

%% Problem: Portfolio Optimization
% Minimize risk and maximize return

% Mock data: 10 assets
nAssets = 10;
expectedReturns = 0.05 + 0.15 * rand(nAssets, 1);  % 5-20% return
riskLevels = 0.1 + 0.3 * rand(nAssets, 1);         % 10-40% risk

% Covariance matrix (simplified)
covMatrix = diag(riskLevels.^2);

% Objective functions
objectiveFcn = @(w) portfolioObjective(w, expectedReturns, covMatrix);

% Constraints: weights sum to 1, non-negative
Aeq = ones(1, nAssets);
beq = 1;
lb = zeros(nAssets, 1);
ub = ones(nAssets, 1);

%% GA Options
options = optimoptions('gamultiobj', ...
    'PopulationSize', 100, ...
    'MaxGenerations', 200, ...
    'ParetoFraction', 0.35, ...
    'Display', 'iter', ...
    'PlotFcn', @gaplotpareto);

%% Run Multi-Objective GA
[x, fval] = gamultiobj(objectiveFcn, nAssets, [], [], Aeq, beq, ...
                       lb, ub, options);

%% Analyze Pareto Front
fprintf('\n=== Pareto Optimal Solutions ===\n');
fprintf('Found %d solutions on Pareto front\n', size(x, 1));

% Plot Pareto front
figure;
scatter(fval(:,1), -fval(:,2), 50, 'filled');
xlabel('Portfolio Risk (Std Dev)');
ylabel('Portfolio Return');
title('Pareto Front: Risk vs Return');
grid on;

% Show a few solutions
fprintf('\nSample solutions:\n');
for i = 1:min(3, size(x, 1))
    fprintf('Solution %d: Risk=%.4f, Return=%.4f\n', ...
            i, fval(i,1), -fval(i,2));
    fprintf('  Weights: [');
    fprintf('%.2f ', x(i,:));
    fprintf(']\n');
end

function obj = portfolioObjective(weights, returns, covMatrix)
    % Objective 1: Minimize risk (portfolio variance)
    portfolioRisk = sqrt(weights' * covMatrix * weights);
    
    % Objective 2: Maximize return (minimize negative return)
    portfolioReturn = -sum(weights .* returns);
    
    obj = [portfolioRisk, portfolioReturn];
end
```

## Industrial Application Example: Process Optimization

```matlab
%% Chemical Process Optimization
% Optimize reactor parameters to maximize yield and minimize cost

clear; clc; close all;

%% Process Model
function [yield, cost] = chemicalProcess(params)
    % params = [temperature, pressure, flowRate, catalystAmount]
    T = params(1);      % Temperature (°C)
    P = params(2);      % Pressure (bar)
    F = params(3);      % Flow rate (L/min)
    C = params(4);      % Catalyst (g)
    
    % Empirical model (simplified)
    % Optimal around T=350, P=10, F=5, C=50
    yield = 100 * exp(-((T-350)/50)^2 - ((P-10)/3)^2 - ...
                      ((F-5)/2)^2 - ((C-50)/20)^2);
    
    % Cost model
    energyCost = 0.1 * T + 0.05 * P;  % Energy costs
    catalystCost = 2.0 * C;           % Catalyst cost
    operatingCost = 5.0 * F;          % Operating cost
    cost = energyCost + catalystCost + operatingCost;
end

%% Problem Setup
% Decision variables: [T, P, F, C]
nvars = 4;
lb = [250, 5, 1, 10];    % Lower bounds
ub = [450, 20, 10, 100]; % Upper bounds

% Multi-objective: maximize yield, minimize cost
fitnessFcn = @(x) processObjective(x);

function obj = processObjective(params)
    [yield, cost] = chemicalProcess(params);
    obj = [-yield, cost];  % Minimize negative yield and cost
end

%% Run GA
options = optimoptions('gamultiobj', ...
    'PopulationSize', 80, ...
    'MaxGenerations', 150, ...
    'Display', 'iter');

[x, fval] = gamultiobj(fitnessFcn, nvars, [], [], [], [], ...
                       lb, ub, [], options);

%% Results Analysis
fprintf('\n=== Process Optimization Results ===\n');
fprintf('Found %d Pareto-optimal operating points\n', size(x, 1));

% Find solution with best yield
[~, bestYieldIdx] = max(-fval(:,1));
optParams = x(bestYieldIdx, :);
[optYield, optCost] = chemicalProcess(optParams);

fprintf('\nBest Yield Solution:\n');
fprintf('Temperature: %.1f °C\n', optParams(1));
fprintf('Pressure: %.1f bar\n', optParams(2));
fprintf('Flow Rate: %.2f L/min\n', optParams(3));
fprintf('Catalyst: %.1f g\n', optParams(4));
fprintf('Yield: %.2f%%\n', optYield);
fprintf('Cost: $%.2f/hour\n', optCost);

% Plot Pareto front
figure('Position', [100, 100, 1000, 400]);
subplot(1, 2, 1);
scatter(-fval(:,1), fval(:,2), 50, 'filled');
xlabel('Yield (%)');
ylabel('Cost ($/hour)');
title('Pareto Front: Yield vs Cost');
grid on;

% Visualize parameter distributions
subplot(1, 2, 2);
paramNames = {'Temperature', 'Pressure', 'Flow Rate', 'Catalyst'};
boxplot(x, 'Labels', paramNames);
ylabel('Parameter Value');
title('Parameter Distributions in Pareto Set');
grid on;
```

## Visualization Tools

### Convergence Plot
```matlab
% Plot fitness over generations
figure;
semilogy(history);
xlabel('Generation');
ylabel('Best Fitness (log scale)');
title('GA Convergence');
grid on;
```

### Population Distribution
```matlab
% Visualize population diversity
figure;
for i = 1:nvars
    subplot(2, ceil(nvars/2), i);
    histogram(population(:, i), 20);
    xlabel(sprintf('Variable %d', i));
    ylabel('Frequency');
    title(sprintf('Distribution of Variable %d', i));
end
```

### Fitness Landscape (2D)
```matlab
% For 2D problems, visualize landscape
[X, Y] = meshgrid(linspace(lb(1), ub(1), 50), ...
                  linspace(lb(2), ub(2), 50));
Z = zeros(size(X));
for i = 1:size(X, 1)
    for j = 1:size(X, 2)
        Z(i,j) = fitnessFcn([X(i,j), Y(i,j)]);
    end
end

figure;
contourf(X, Y, Z, 20);
hold on;
scatter(population(:,1), population(:,2), 30, 'r', 'filled');
scatter(bestSolution(1), bestSolution(2), 100, 'k', 'filled', 'MarkerEdgeColor', 'w');
colorbar;
xlabel('Variable 1');
ylabel('Variable 2');
title('Fitness Landscape with Population');
hold off;
```

## Parameter Tuning Guidelines

### Population Size
```matlab
popSize = 10 * nvars;  % Rule of thumb
```

### Crossover Rate
```matlab
crossoverRate = 0.8;  % 80% is typical
```

### Mutation Rate
```matlab
mutationRate = 1 / nvars;  % Adaptive to problem size
```

### Elite Count
```matlab
eliteCount = 2;  % Preserve 2 best solutions
```

## Performance Tips

1. **Vectorization**: Evaluate population in batches
```matlab
fitness = arrayfun(@(i) fitnessFcn(population(i,:)), 1:popSize);
```

2. **Parallel Evaluation**: Use `parfor` for expensive fitness
```matlab
parfor i = 1:popSize
    fitness(i) = fitnessFcn(population(i,:));
end
```

3. **Warm Start**: Initialize with good solutions
```matlab
population(1,:) = knownGoodSolution;
```

## References

1. **MATLAB GA Toolbox**:
   - MathWorks. (2021). *Global Optimization Toolbox User's Guide*.

2. **Genetic Algorithms**:
   - Goldberg, D. E. (1989). *Genetic Algorithms in Search, Optimization, and Machine Learning*. Addison-Wesley.

3. **Multi-Objective Optimization**:
   - Deb, K. (2001). *Multi-Objective Optimization Using Evolutionary Algorithms*. Wiley.

## See Also

- [Main GA Documentation](../README.md)
- [Python Implementation](../GA-py/README.md)
- [C# Implementation](../GA-c/README.md)

## Advanced MATLAB Topics

### Vectorized Operations for Speed

Maximize MATLAB's vectorization capabilities:

```matlab
% Vectorized fitness evaluation for population
function fitness = evaluatePopulationVectorized(population, fitnessFcn)
    % Evaluate entire population at once if possible
    [popSize, nvars] = size(population);
    
    % For simple functions, vectorize directly
    if strcmp(func2str(fitnessFcn), 'sphere')
        fitness = sum(population.^2, 2);
    else
        % Fallback to arrayfun for custom functions
        fitness = arrayfun(@(i) fitnessFcn(population(i,:)), 1:popSize)';
    end
end

% Vectorized crossover
function children = vectorizedCrossover(parents, alpha)
    n = size(parents, 1);
    shuffled = parents(randperm(n), :);
    
    % Blend crossover for all pairs simultaneously
    weights = rand(n/2, 1);
    children1 = weights .* parents(1:2:end, :) + (1-weights) .* parents(2:2:end, :);
    children2 = (1-weights) .* parents(1:2:end, :) + weights .* parents(2:2:end, :);
    
    children = [children1; children2];
end
```

### GPU Acceleration

Use MATLAB's GPU computing for large populations:

```matlab
function [bestSolution, bestFitness] = gpuGA(fitnessFcn, nvars, options)
    % Transfer population to GPU
    population = gpuArray(rand(options.PopulationSize, nvars));
    population = population .* (options.ub - options.lb) + options.lb;
    
    for gen = 1:options.MaxGenerations
        % Evaluate fitness on GPU
        fitness = arrayfun(fitnessFcn, population);
        
        % Selection, crossover, mutation on GPU
        [~, sortIdx] = sort(fitness);
        population = population(sortIdx, :);
        
        % Elite preservation
        newPop = population(1:options.EliteCount, :);
        
        % Generate offspring
        for i = options.EliteCount+1:2:options.PopulationSize
            % Tournament selection
            p1 = tournamentSelectGPU(population, fitness, options.TournamentSize);
            p2 = tournamentSelectGPU(population, fitness, options.TournamentSize);
            
            % Crossover and mutation
            [c1, c2] = crossoverGPU(p1, p2, options.CrossoverFraction);
            c1 = mutateGPU(c1, options.MutationRate, options.lb, options.ub);
            c2 = mutateGPU(c2, options.MutationRate, options.lb, options.ub);
            
            newPop = [newPop; c1; c2];
        end
        
        population = newPop(1:options.PopulationSize, :);
    end
    
    % Transfer result back to CPU
    bestSolution = gather(population(1, :));
    bestFitness = gather(fitness(1));
end
```

### Custom Visualization Dashboard

Create an interactive optimization dashboard:

```matlab
function fig = createGADashboard(gaData)
    fig = figure('Position', [100, 100, 1400, 800], 'Name', 'GA Dashboard');
    
    % Panel 1: Convergence plot
    subplot(2, 3, 1);
    semilogy(gaData.bestHistory, 'b-', 'LineWidth', 2);
    hold on;
    semilogy(gaData.meanHistory, 'r--', 'LineWidth', 1);
    hold off;
    xlabel('Generation');
    ylabel('Fitness (log)');
    title('Convergence');
    legend('Best', 'Mean', 'Location', 'northeast');
    grid on;
    
    % Panel 2: Population diversity
    subplot(2, 3, 2);
    plot(gaData.diversityHistory, 'g-', 'LineWidth', 2);
    xlabel('Generation');
    ylabel('Diversity');
    title('Population Diversity');
    grid on;
    
    % Panel 3: Best solution variables
    subplot(2, 3, 3);
    bar(gaData.bestSolution);
    xlabel('Variable Index');
    ylabel('Value');
    title('Best Solution');
    grid on;
    
    % Panel 4: Fitness distribution
    subplot(2, 3, 4);
    histogram(gaData.finalFitness, 20);
    xlabel('Fitness');
    ylabel('Count');
    title('Final Population Fitness Distribution');
    
    % Panel 5: Variable correlation heatmap
    subplot(2, 3, 5);
    imagesc(corr(gaData.finalPopulation));
    colorbar;
    xlabel('Variable');
    ylabel('Variable');
    title('Variable Correlation');
    
    % Panel 6: Performance metrics
    subplot(2, 3, 6);
    metrics = {'Best Fitness', 'Mean Fitness', 'Std Dev', 'Convergence Gen'};
    values = [gaData.bestFitness, mean(gaData.finalFitness), ...
              std(gaData.finalFitness), gaData.convergenceGen];
    bar(categorical(metrics), values);
    title('Performance Metrics');
    
    % Add overall title
    sgtitle(sprintf('GA Optimization Results - %d Generations', gaData.generations));
end
```

### Constraint Handling Techniques

Implement multiple constraint handling methods:

```matlab
% 1. Penalty Method
function fitness = penaltyMethod(x, objectiveFcn, constraintFcn, penalty)
    objValue = objectiveFcn(x);
    [c, ceq] = constraintFcn(x);
    
    % Inequality constraint violations
    violation = sum(max(0, c).^2);
    
    % Equality constraint violations
    if ~isempty(ceq)
        violation = violation + sum(ceq.^2);
    end
    
    fitness = objValue + penalty * violation;
end

% 2. Repair Method
function xRepaired = repairSolution(x, constraintFcn, bounds)
    maxIter = 100;
    stepSize = 0.1;
    
    xRepaired = x;
    for iter = 1:maxIter
        [c, ceq] = constraintFcn(xRepaired);
        
        if all(c <= 0) && all(abs(ceq) < 1e-6)
            break;  % Feasible
        end
        
        % Move towards feasibility
        gradient = estimateConstraintGradient(xRepaired, constraintFcn);
        xRepaired = xRepaired - stepSize * gradient;
        
        % Apply bounds
        xRepaired = max(bounds.lb, min(bounds.ub, xRepaired));
    end
end

% 3. Feasibility-Based Selection
function selected = feasibilitySelection(pop1, pop2, constraintFcn)
    feasible1 = isFeasible(pop1, constraintFcn);
    feasible2 = isFeasible(pop2, constraintFcn);
    
    if feasible1 && ~feasible2
        selected = pop1;
    elseif ~feasible1 && feasible2
        selected = pop2;
    elseif feasible1 && feasible2
        % Both feasible: select by fitness
        selected = selectByFitness(pop1, pop2);
    else
        % Both infeasible: select by constraint violation
        selected = selectByViolation(pop1, pop2, constraintFcn);
    end
end
```

### Saving and Loading GA State

Enable checkpoint/restart functionality:

```matlab
function saveGAState(ga, filename)
    state.population = ga.population;
    state.fitness = ga.fitness;
    state.bestSolution = ga.bestSolution;
    state.bestFitness = ga.bestFitness;
    state.generation = ga.generation;
    state.history = ga.history;
    state.options = ga.options;
    state.rngState = rng;  % Random number generator state
    
    save(filename, 'state');
    fprintf('GA state saved to %s\n', filename);
end

function ga = loadGAState(filename)
    load(filename, 'state');
    
    ga.population = state.population;
    ga.fitness = state.fitness;
    ga.bestSolution = state.bestSolution;
    ga.bestFitness = state.bestFitness;
    ga.generation = state.generation;
    ga.history = state.history;
    ga.options = state.options;
    
    rng(state.rngState);  % Restore RNG state
    fprintf('GA state loaded from %s (Generation %d)\n', filename, ga.generation);
end
```

---

*For questions or contributions, please refer to the main repository.*
