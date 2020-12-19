function [graph] = createGraph()
    % create the graph and calculate the distances between each nodes
    
    x = [0.09, 0.16, 0.84, 0.7];
    y = [0.17, 0.52, 0.92, 0.16];
    
    graph.n = length(x);
    
    for i = 1 : graph.n
        graph.node(i).x = x(i);
        graph.node(i).y = y(i);
    end
    
    graph.edges = zeros(graph.n, graph.n);
    
    for i = 1 : graph.n
        for j = 1 : graph.n
            x1 = graph.node(i).x;
            x2 = graph.node(j).x;
            y1 = graph.node(i).y;
            y2 = graph.node(j).y;
            % Distance hamiltanian
            graph.edges(i,j) = sqrt((x1 - x2)^2 + (y1 - y2)^2);
       end
    end
    
end