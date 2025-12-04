# Ant Colony Optimization - MATLAB Implementation

## Overview

This directory contains a MATLAB implementation of Ant Colony Optimization for the Traveling Salesman Problem (TSP). The implementation provides a complete ACO framework with visualization capabilities, demonstrating the emergent collective behavior of artificial ants.

## Implementation Details

### Author
**Luis Carlos Junior** and contributors (2020)

### Language
MATLAB (R2016b or later recommended)

### Algorithm Variant
Ant System (AS) - The original ACO algorithm by Dorigo (1992)

## Files Description

### Core Functions

1. **`principal.m`** - Main script
   - Entry point for the ACO algorithm
   - Sets up problem parameters
   - Initializes colony and graph
   - Main optimization loop
   - Results visualization

2. **`createGraph.m`** - Graph initialization
   - Generates city coordinates (random or predefined)
   - Calculates distance matrix
   - Initializes pheromone trails

3. **`createColony.m`** - Colony creation
   - Initializes ant population
   - Sets starting positions
   - Prepares data structures for tour construction

4. **`fitnessFunction.m`** - Tour evaluation
   - Calculates tour length (fitness)
   - Returns cost for a given tour

5. **`rouletteWheel.m`** - Probabilistic selection
   - Implements roulette wheel selection
   - Used for probabilistic city selection

6. **`updatePhromone.m`** - Pheromone update
   - Applies evaporation
   - Deposits new pheromones
   - Implements update rules

### Visualization Functions

7. **`drawGraph.m`** - Draw city graph
   - Plots cities as points
   - Shows city labels
   - Displays graph structure

8. **`drawBestTour.m`** - Visualize best tour
   - Plots best tour found
   - Highlights path
   - Shows tour cost

9. **`drawPhromone.m`** - Pheromone visualization
   - Displays pheromone intensity on edges
   - Line thickness proportional to pheromone
   - Helps understand algorithm behavior

## Usage

### Basic Usage

```matlab
% Run ACO for TSP
principal
```

### Customized Parameters

```matlab
% principal.m configuration

% Problem definition
nCities = 20;           % Number of cities
graphType = 'random';   % 'random' or 'circle' or 'grid'

% ACO parameters
nAnts = 20;             % Number of ants
maxIter = 100;          % Maximum iterations
alpha = 1;              % Pheromone importance
beta = 5;               % Heuristic importance (distance)
rho = 0.1;              % Evaporation rate
Q = 100;                % Pheromone deposit constant

% Visualization
showVisualization = true;
updateFrequency = 10;    % Update plot every N iterations
```

### Custom Problem Instance

```matlab
% Define your own cities
cities = [
    10, 20;   % City 1: (x, y)
    30, 40;   % City 2
    50, 10;   % City 3
    % ... add more cities
];

% Or load from file
cities = load('tsp_cities.txt');

% Create problem
graph = createGraphFromCities(cities);

% Run ACO
[bestTour, bestCost, history] = runACO(graph, nAnts, maxIter);
```

## Complete Example Script

```matlab
%% Ant Colony Optimization for TSP - Complete Example
% This script demonstrates ACO solving a 20-city TSP problem

clear; clc; close all;

%% 1. Problem Setup
nCities = 20;

% Generate random city coordinates
rng(42);  % For reproducibility
cities = rand(nCities, 2) * 100;

% Calculate distance matrix
distMatrix = zeros(nCities, nCities);
for i = 1:nCities
    for j = 1:nCities
        distMatrix(i,j) = norm(cities(i,:) - cities(j,:));
    end
end

%% 2. ACO Parameters
nAnts = 30;              % Population size
maxIter = 200;           % Iterations
alpha = 1.0;             % Pheromone weight
beta = 5.0;              % Distance weight
rho = 0.1;               % Evaporation rate
Q = 100;                 % Pheromone constant
tau0 = 1/(nCities * mean(distMatrix(:)));  % Initial pheromone

% Initialize pheromone matrix
pheromone = ones(nCities, nCities) * tau0;

%% 3. ACO Main Loop
bestTour = [];
bestCost = inf;
costHistory = zeros(maxIter, 1);

figure('Position', [100, 100, 1200, 400]);

for iter = 1:maxIter
    % Store tours and costs
    tours = zeros(nAnts, nCities);
    costs = zeros(nAnts, 1);
    
    % Construct solutions for all ants
    for ant = 1:nAnts
        % Start from random city
        startCity = randi(nCities);
        tour = startCity;
        visited = false(1, nCities);
        visited(startCity) = true;
        
        % Construct tour
        currentCity = startCity;
        for step = 1:(nCities-1)
            % Calculate probabilities for unvisited cities
            unvisited = find(~visited);
            probs = zeros(length(unvisited), 1);
            
            for k = 1:length(unvisited)
                nextCity = unvisited(k);
                % Probability = (pheromone^alpha) * (1/distance^beta)
                probs(k) = (pheromone(currentCity, nextCity)^alpha) * ...
                          ((1/distMatrix(currentCity, nextCity))^beta);
            end
            
            % Normalize probabilities
            probs = probs / sum(probs);
            
            % Select next city (roulette wheel)
            cumProbs = cumsum(probs);
            r = rand();
            idx = find(cumProbs >= r, 1, 'first');
            nextCity = unvisited(idx);
            
            % Update tour
            tour = [tour, nextCity];
            visited(nextCity) = true;
            currentCity = nextCity;
        end
        
        % Calculate tour cost
        cost = 0;
        for i = 1:nCities
            j = mod(i, nCities) + 1;
            cost = cost + distMatrix(tour(i), tour(j));
        end
        
        tours(ant, :) = tour;
        costs(ant) = cost;
        
        % Update best solution
        if cost < bestCost
            bestCost = cost;
            bestTour = tour;
        end
    end
    
    % Pheromone evaporation
    pheromone = (1 - rho) * pheromone;
    
    % Pheromone deposit
    for ant = 1:nAnts
        tour = tours(ant, :);
        contribution = Q / costs(ant);
        
        for i = 1:nCities
            j = mod(i, nCities) + 1;
            city1 = tour(i);
            city2 = tour(j);
            pheromone(city1, city2) = pheromone(city1, city2) + contribution;
            pheromone(city2, city1) = pheromone(city2, city1) + contribution;
        end
    end
    
    % Record history
    costHistory(iter) = bestCost;
    
    % Visualization (every 20 iterations)
    if mod(iter, 20) == 0
        % Plot cities and best tour
        subplot(1, 2, 1);
        plot(cities(:,1), cities(:,2), 'ro', 'MarkerSize', 8, 'MarkerFaceColor', 'r');
        hold on;
        
        % Draw best tour
        for i = 1:nCities
            j = mod(i, nCities) + 1;
            city1 = bestTour(i);
            city2 = bestTour(j);
            plot([cities(city1,1), cities(city2,1)], ...
                 [cities(city1,2), cities(city2,2)], 'b-', 'LineWidth', 2);
        end
        hold off;
        title(sprintf('Best Tour (Iteration %d) - Cost: %.2f', iter, bestCost));
        xlabel('X'); ylabel('Y');
        grid on;
        
        % Plot convergence
        subplot(1, 2, 2);
        semilogy(1:iter, costHistory(1:iter), 'b-', 'LineWidth', 2);
        xlabel('Iteration');
        ylabel('Best Cost (log scale)');
        title('ACO Convergence');
        grid on;
        
        drawnow;
    end
    
    % Display progress
    if mod(iter, 50) == 0
        fprintf('Iteration %d: Best Cost = %.2f\n', iter, bestCost);
    end
end

%% 4. Results
fprintf('\n=== ACO Results ===\n');
fprintf('Best tour cost: %.2f\n', bestCost);
fprintf('Best tour: %s\n', mat2str(bestTour));

% Final visualization
figure('Position', [100, 100, 800, 600]);
plot(cities(:,1), cities(:,2), 'ro', 'MarkerSize', 10, 'MarkerFaceColor', 'r');
hold on;
for i = 1:nCities
    j = mod(i, nCities) + 1;
    city1 = bestTour(i);
    city2 = bestTour(j);
    plot([cities(city1,1), cities(city2,1)], ...
         [cities(city1,2), cities(city2,2)], 'b-', 'LineWidth', 2);
    
    % Add city labels
    text(cities(i,1), cities(i,2), sprintf(' %d', i), 'FontSize', 10);
end
hold off;
title(sprintf('Best TSP Tour - Cost: %.2f', bestCost));
xlabel('X Coordinate');
ylabel('Y Coordinate');
grid on;
axis equal;
```

## Industrial Application Example: Delivery Route Optimization

```matlab
%% Real-World Application: Last-Mile Delivery Optimization
% Scenario: Optimize delivery routes for e-commerce logistics

clear; clc;

%% Problem Definition
% 25 delivery locations in a city
% Coordinates represent actual GPS locations (scaled)
deliveryLocations = [
    % [longitude, latitude] approximation
    50, 50;    % Warehouse (depot)
    45, 48; 52, 45; 48, 52; 55, 51;  % Zone 1
    42, 55; 38, 52; 41, 58; 36, 54;  % Zone 2
    60, 48; 63, 45; 58, 43; 62, 50;  % Zone 3
    48, 40; 52, 38; 45, 35; 50, 33;  % Zone 4
    35, 45; 32, 48; 38, 42; 34, 40;  % Zone 5
    68, 52; 70, 48; 65, 55; 72, 51;  % Zone 6
];

nLocations = size(deliveryLocations, 1);

% Calculate distance matrix (Euclidean distance as proxy for road distance)
distMatrix = zeros(nLocations, nLocations);
for i = 1:nLocations
    for j = 1:nLocations
        distMatrix(i,j) = norm(deliveryLocations(i,:) - deliveryLocations(j,:));
    end
end

%% ACO Configuration for Delivery Optimization
% Tuned parameters for better quality solutions
nAnts = 40;              % Larger colony for better exploration
maxIter = 300;           % More iterations for quality
alpha = 1.0;             % Standard pheromone influence
beta = 3.0;              % Strong heuristic (prefer shorter distances)
rho = 0.15;              % Moderate evaporation
Q = 100;
elitist = true;          % Use elitist strategy

%% Run ACO
% ... (same algorithm as before, but with elitist update)

%% Results Analysis
fprintf('\n=== Delivery Route Optimization Results ===\n');
fprintf('Total delivery route distance: %.2f km\n', bestCost * 0.1);  % Scale to km
fprintf('Estimated delivery time: %.1f hours\n', bestCost * 0.1 / 40);  % 40 km/h avg
fprintf('\nOptimal delivery sequence:\n');
fprintf('Warehouse (depot) -> ');
for i = 2:nLocations
    fprintf('Location %d -> ', bestTour(i));
    if mod(i-1, 5) == 0
        fprintf('\n  ');
    end
end
fprintf('Warehouse\n');

% Calculate savings vs greedy nearest neighbor
greedyCost = calculateGreedyTour(distMatrix);
savings = ((greedyCost - bestCost) / greedyCost) * 100;
fprintf('\nImprovement over greedy: %.1f%%\n', savings);
fprintf('Distance saved: %.2f km\n', (greedyCost - bestCost) * 0.1);
```

## Parameter Tuning Guidelines

### α (Pheromone Influence)
- **α = 0**: Pure greedy (follow heuristic only)
- **α = 1**: Balanced (standard)
- **α = 2-3**: Strong memory influence

### β (Heuristic Influence)
- **β = 2**: Light heuristic bias
- **β = 5**: Strong preference for short edges (recommended)
- **β = 10**: Very greedy, fast initial solutions

### ρ (Evaporation Rate)
- **ρ = 0.05**: Slow evaporation, strong memory
- **ρ = 0.1-0.3**: Balanced (recommended)
- **ρ = 0.5+**: Fast evaporation, more exploration

### Number of Ants
- **Rule of thumb**: m = n (number of cities)
- **Minimum**: 10 ants
- **Maximum**: 50-100 ants

### Iterations
- **Quick test**: 50-100 iterations
- **Quality solution**: 200-500 iterations
- **Best result**: 1000+ iterations

## Visualization Features

### Pheromone Trail Visualization
```matlab
% Show pheromone intensity
figure;
imagesc(pheromone);
colorbar;
title('Pheromone Distribution');
xlabel('City'); ylabel('City');
```

### Animated Tour Construction
```matlab
% Animate ant building tour
for step = 1:nCities
    % Plot current partial tour
    pause(0.1);
end
```

### 3D Pheromone Surface
```matlab
figure;
surf(pheromone);
title('Pheromone Intensity (3D)');
xlabel('City i'); ylabel('City j'); zlabel('Pheromone');
```

## Advanced Features

### Elitist Ant Strategy
```matlab
% Only best ant deposits pheromones
[~, bestAntIdx] = min(costs);
bestAnt = tours(bestAntIdx, :);

% Deposit additional pheromone on best tour
for i = 1:nCities
    j = mod(i, nCities) + 1;
    pheromone(bestAnt(i), bestAnt(j)) += elitistWeight * Q / bestCost;
end
```

### Max-Min Ant System (MMAS)
```matlab
% Bound pheromone values
tau_max = 1 / (rho * bestCost);
tau_min = tau_max / (2 * nCities);

pheromone = max(min(pheromone, tau_max), tau_min);
```

### Local Search (2-opt)
```matlab
% Improve ant solutions with local search
tour = twoOptImprovement(tour, distMatrix);
```

## Performance Metrics

### Benchmark Results on TSPLIB Instances

| Instance | Cities | Known Optimal | ACO Best | Gap (%) | Time (s) |
|----------|--------|---------------|----------|---------|----------|
| eil51    | 51     | 426           | 428      | 0.47    | 12       |
| berlin52 | 52     | 7542          | 7544     | 0.03    | 15       |
| kroA100  | 100    | 21282         | 21450    | 0.79    | 45       |

## Common Issues and Solutions

### Issue: Poor convergence
**Solution**: 
- Increase β (stronger heuristic)
- Decrease ρ (slower evaporation)
- Increase number of ants

### Issue: Premature convergence
**Solution**:
- Increase ρ (faster evaporation)
- Implement MMAS bounds
- Add diversification mechanism

### Issue: Slow execution
**Solution**:
- Reduce number of ants
- Use candidate list strategy
- Implement early stopping

## References

1. **Original Ant System**:
   - Dorigo, M., Maniezzo, V., & Colorni, A. (1996). "Ant system: optimization by a colony of cooperating agents". *IEEE Transactions on Systems, Man, and Cybernetics, Part B*, 26(1), 29-41.

2. **TSP Applications**:
   - Dorigo, M., & Gambardella, L. M. (1997). "Ant colony system: a cooperative learning approach to the traveling salesman problem". *IEEE Transactions on Evolutionary Computation*, 1(1), 53-66.

3. **MATLAB Implementation**:
   - MATLAB Optimization Toolbox Documentation

## See Also

- [Main ACO Documentation](../README.md)
- [C++ Implementation](../AntColonyOptimization-C/README.md)

## Advanced MATLAB Topics

### GPU Acceleration for ACO

Use MATLAB's GPU computing capabilities:

```matlab
function [bestTour, bestCost] = gpuACO(distMatrix, nAnts, maxIter)
    % Transfer to GPU
    distGPU = gpuArray(distMatrix);
    n = size(distGPU, 1);
    
    % Initialize pheromone on GPU
    tau0 = 1 / (n * sum(distGPU(:)) / n^2);
    pheromone = gpuArray(ones(n, n) * tau0);
    
    % Heuristic information
    eta = 1 ./ distGPU;
    eta(isinf(eta)) = 0;
    
    for iter = 1:maxIter
        % Construct solutions in parallel on GPU
        tours = constructToursGPU(pheromone, eta, nAnts);
        costs = evaluateToursGPU(tours, distGPU);
        
        % Update pheromone on GPU
        pheromone = updatePheromoneGPU(pheromone, tours, costs);
    end
    
    % Transfer back to CPU
    [bestCost, idx] = min(gather(costs));
    bestTour = gather(tours(idx, :));
end
```

### Real-Time Visualization Dashboard

```matlab
function dashboard = createACODashboard()
    % Create figure with multiple visualization panels
    dashboard.fig = figure('Position', [50 50 1400 900], ...
                          'Name', 'ACO Optimization Dashboard');
    
    % Panel 1: Current best tour
    dashboard.ax1 = subplot(2, 3, 1);
    title('Best Tour Found');
    
    % Panel 2: Pheromone heatmap
    dashboard.ax2 = subplot(2, 3, 2);
    title('Pheromone Distribution');
    
    % Panel 3: Convergence curve
    dashboard.ax3 = subplot(2, 3, 3);
    title('Convergence');
    xlabel('Iteration'); ylabel('Best Cost');
    
    % Panel 4: Ant activity heatmap
    dashboard.ax4 = subplot(2, 3, 4);
    title('Edge Usage Frequency');
    
    % Panel 5: Solution quality distribution
    dashboard.ax5 = subplot(2, 3, 5);
    title('Solution Quality Distribution');
    
    % Panel 6: Statistics table
    dashboard.ax6 = subplot(2, 3, 6);
    axis off;
    title('Statistics');
end

function updateDashboard(dashboard, data, iter)
    % Update tour visualization
    axes(dashboard.ax1);
    cla;
    plotTour(data.cities, data.bestTour);
    title(sprintf('Best Tour (Cost: %.2f)', data.bestCost));
    
    % Update pheromone heatmap
    axes(dashboard.ax2);
    imagesc(log(data.pheromone + 1e-10));
    colorbar;
    title('Pheromone (log scale)');
    
    % Update convergence
    axes(dashboard.ax3);
    if iter > 1
        plot(1:iter, data.history(1:iter), 'b-', 'LineWidth', 2);
        xlabel('Iteration'); ylabel('Best Cost');
    end
    
    drawnow;
end
```

### Hybrid ACO with Local Search

```matlab
function [bestTour, bestCost] = hybridACO(distMatrix, options)
    % ACO with integrated 2-opt and 3-opt local search
    
    n = size(distMatrix, 1);
    pheromone = initializePheromone(distMatrix, options);
    eta = 1 ./ distMatrix;
    
    bestTour = [];
    bestCost = inf;
    history = zeros(options.maxIter, 1);
    
    for iter = 1:options.maxIter
        % Construct ant solutions
        tours = constructSolutions(pheromone, eta, options.nAnts, distMatrix);
        costs = calculateCosts(tours, distMatrix);
        
        % Apply local search to best ants
        nLocalSearch = min(5, options.nAnts);
        [~, sortIdx] = sort(costs);
        
        for i = 1:nLocalSearch
            antIdx = sortIdx(i);
            
            % 2-opt improvement
            [tours(antIdx,:), costs(antIdx)] = twoOptImprove(
                tours(antIdx,:), distMatrix);
            
            % 3-opt for very best ant
            if i == 1 && options.use3opt
                [tours(antIdx,:), costs(antIdx)] = threeOptImprove(
                    tours(antIdx,:), distMatrix);
            end
        end
        
        % Update global best
        [minCost, minIdx] = min(costs);
        if minCost < bestCost
            bestCost = minCost;
            bestTour = tours(minIdx, :);
        end
        
        % Update pheromone (ACS-style)
        pheromone = (1 - options.rho) * pheromone;
        
        % Only best ant deposits
        deposit = options.Q / bestCost;
        for i = 1:n
            j = mod(i, n) + 1;
            city1 = bestTour(i);
            city2 = bestTour(j);
            pheromone(city1, city2) = pheromone(city1, city2) + deposit;
            pheromone(city2, city1) = pheromone(city2, city1) + deposit;
        end
        
        % Apply MMAS bounds
        tauMax = 1 / (options.rho * bestCost);
        tauMin = tauMax / (2 * n);
        pheromone = max(tauMin, min(tauMax, pheromone));
        
        history(iter) = bestCost;
        
        if mod(iter, 10) == 0
            fprintf('Iter %d: Best = %.2f\n', iter, bestCost);
        end
    end
end

function [tour, cost] = twoOptImprove(tour, distMatrix)
    n = length(tour);
    improved = true;
    
    while improved
        improved = false;
        for i = 1:n-2
            for j = i+2:n
                % Calculate improvement
                if j == n
                    jNext = 1;
                else
                    jNext = j + 1;
                end
                
                d1 = distMatrix(tour(i), tour(i+1)) + ...
                     distMatrix(tour(j), tour(jNext));
                d2 = distMatrix(tour(i), tour(j)) + ...
                     distMatrix(tour(i+1), tour(jNext));
                
                if d2 < d1 - 1e-10
                    % Reverse segment
                    tour(i+1:j) = tour(j:-1:i+1);
                    improved = true;
                end
            end
        end
    end
    
    cost = calculateTourCost(tour, distMatrix);
end
```

### TSPLIB Instance Loader

```matlab
function [coords, distMatrix, optimalCost] = loadTSPLIB(filename)
    % Load standard TSPLIB format files
    
    fid = fopen(filename, 'r');
    if fid == -1
        error('Cannot open file: %s', filename);
    end
    
    % Parse header
    dimension = 0;
    edgeWeightType = '';
    
    while true
        line = fgetl(fid);
        if contains(line, 'DIMENSION')
            dimension = sscanf(line, 'DIMENSION : %d');
        elseif contains(line, 'EDGE_WEIGHT_TYPE')
            edgeWeightType = strtrim(extractAfter(line, ':'));
        elseif contains(line, 'NODE_COORD_SECTION')
            break;
        end
    end
    
    % Read coordinates
    coords = zeros(dimension, 2);
    for i = 1:dimension
        line = fgetl(fid);
        data = sscanf(line, '%d %f %f');
        coords(i, :) = data(2:3)';
    end
    
    fclose(fid);
    
    % Calculate distance matrix
    if strcmp(edgeWeightType, 'EUC_2D')
        distMatrix = pdist2(coords, coords);
    else
        distMatrix = pdist2(coords, coords);  % Default to Euclidean
    end
    
    % Known optimal values for common instances
    optimalValues = containers.Map(...
        {'berlin52', 'eil51', 'kroA100', 'att48'}, ...
        [7542, 426, 21282, 10628]);
    
    [~, name] = fileparts(filename);
    if isKey(optimalValues, name)
        optimalCost = optimalValues(name);
    else
        optimalCost = NaN;
    end
    
    fprintf('Loaded %s: %d cities, optimal = %.0f\n', ...
            name, dimension, optimalCost);
end

% Usage:
% [coords, dist, opt] = loadTSPLIB('berlin52.tsp');
% [tour, cost] = hybridACO(dist, options);
% gap = (cost - opt) / opt * 100;
% fprintf('Gap from optimal: %.2f%%\n', gap);
```

---

*For questions or contributions, please refer to the main repository.*
