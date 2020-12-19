# -*- coding: utf-8 -*-
"""
Created on Fri Aug 28 13:43:41 2020

@author: luisc
"""
import ga
import numpy as np
import matplotlib.pyplot as plt
from ypstruct import structure

# Sphere teste function
def sphere(x):
    return sum(x**2)

# problem Definition
problem = structure()
problem.costfunc = sphere
problem.nvar = 5
problem.varmin = -10
problem.varmax = 10

# Run GA
params = structure()
params.maxit = 100
params.npop = 100
params.beta = 1
params.pc = 1
params.gamma = 0.1
params.mu = 0.2
params.sigma = 0.1

# Run GA

out = ga.run(problem, params)

# Results
plt.plot(out.bestcost)
plt.xlim(0, params.maxit)
plt.xlabel('Iterations')
plt.ylabel('Best Cost')
plt.title('Genetic Algorithm')
plt.grid(True)
plt.show()

print(0)

# Results