function [nextNode] = rouletteWheel(P)
    cumSumP = cumsum(P);
    
    r = rand();
    
    nextNode = find(r <= cumSumP);
    nextNode = nextNode(1);
    
end