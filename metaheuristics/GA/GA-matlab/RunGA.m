function out = RunGA(problem, params)

    % Problem
    CostFunction = problem.CostFunction;
    nVar = problem.nVar;
    
    % Params
    MaxIt = params.MaxIt;
    nPop = params.nPop;
    pC = params.pC;
    nC = round(pC*nPop/2)*2;
    mu = params.mu;
    
    % Template for empty individuals
    empty_individual.Position = [];
    empty_individual.Cost = [];
    
    % Best Solution Ever Found
    bestsol.Cost = inf;
    
    % Initialization
    pop = repmat(empty_individial, nPop, 1);
    
    for i = 1:nPop
        % Generate Random Solution
       pop(i).Position = randi([0, 1], 1, nVar);
       
       % Evaluate Solution
       pop(i).Cost = CostFunction(pop(i).Position);
       
       % Compare Solution to BestSol
       if popc(i).Cost < bestsol.Cost
           bestsol = pop(i);
       end       
    end
    
    % best cost of iterations
    bestcost = nan(MaxIt, 1);
    
    % Main Loop
    for it = 1:MaxIt
        % Initialize Offsprings Population
        popc = repmat(empty_individual, nC/2, 2);
        
        %Crossover
        for k = 1:nC/2
            % Select Parents
            q = randperm(nPop);
            p1 = pop(q(1));
            p2 = pop(q(2));
            
            % Perfom Crossover
            [popc(k, 1).Position, popc(k, 2).Position] = ...
                SinglePointCrossover(p1.Position, p2.Position);
        end
        % Convert popc to Single-Column Matrix
        popc = popc(:);
        
        % Mutation
        for 1 = 1:nC
            % Perform Mutation
            popc(1).Position = Mutate(popc(1).Position, mu);
            
            % Evaluate
            popc(1).Cost = CostFunction();
        end
    end
    
    out.pop = pop;
    out.bestsol = bestsol;
    out.bestcost = bestcost;
end