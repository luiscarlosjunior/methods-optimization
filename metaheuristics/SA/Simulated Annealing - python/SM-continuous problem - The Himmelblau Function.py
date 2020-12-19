# -*- coding: utf-8 -*-
"""
Created on Tue Aug 11 11:22:32 2020

@author: luisc
"""
import numpy as np
import matplotlib.pyplot as plt

#ignore the warning of zero division
# or np.seterr(divide="ignore", invalid="ignore")
np.seterr(divide="ignore") 

#initial solution you'd like to start at
x = 2
y = 1

#the himmelblau function
#objective function value
z = ((x**2) + y - 11)**2 + (x + (y**2)-7)**2

print()
print("initial x is: %0.3f" % x)
print("initial y is: %0.3f" % y)
print("initial z is: %0.3f" % z)