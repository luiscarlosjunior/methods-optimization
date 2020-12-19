# -*- coding: utf-8 -*-
"""
Created on Tue Aug 11 11:33:44 2020

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

#hyperparameter (user inputted parameters)
#the temperature (temp.)
T0 = 1000 
temp_for_plot = T0

M = 300 # how many times you will decrease the temp.
N = 15 #how many times you want to search your neighborhood
alpha = 0.85 # by how much do you want to decrease the temp
            # when you increase m by 1 (m small)

k = 0.1 #helps reduce the step-size, for example, if your random number
        # was 0.9 and your x value was 1, to make your step size in
        # the search space smaller, instead of making your new x value
        # 1.9 (1 + .9), you can multiply the random number (.9) with a small
        # number to ensure you will take a smaller step.. so, if k = 0.1, then
        # 0.9 * .1 = .09, wich will make your new x value 1.09 (1 + .09).. this
        # helps you search the search space more efficiently by being more
        # through insted of jumping around and taking a large step that may
        # lead you to pass the optimal or near-optimal solution
        
temp = [] #to plot the temperature
obj_val = [] #to plot the obj val reached at the end of each (m small M)

for i in range(M): #how many times to decrease the temp
        for j in range(N): # for each m, how many neighborhood searches

                # for the decision variable x -->
                rand_num_x_1 = np.random.rand() # increase or decrease x value
                rand_num_x_2 = np.random.rand() # by how much

                if rand_num_x_1 >= 0.5: # greater than .5, we increase
                        step_size_x = k * rand_num_x_2 # make sure we take a smaller
                else:
                        step_size_x = -k * rand_num_x_2 #less than 0.5, we decrease

                # for the decision variable y -->
                rand_num_y_1 = np.random.rand() #increase or decrease y value
                rand_num_y_2 = np.random.rand()     

                if rand_num_y_1 >= 0.5: # greater than .5, we increase
                        step_size_y = k * rand_num_y_2 # make sure we take a smaller
                else:
                        step_size_y = -k * rand_num_y_2 #less than 0.5, we decrease

                # temporary because we still need to know if we should take this
                # new solution or stay where we are and look again
                x_temporary = x + step_size_x # add or subtract the x value
                y_temporary = y + step_size_y # add or subtract the y value

                # the possible new move with the temporary values
                # need to see if better or worse than where we currently are
                obj_val_possible = ((x_temporary**2) + y_temporary - 11)**2 + (x_temporary+(y_temporary**2)-7)**2

                # where we are currently
                obj_val_current = ((x**2) + y - 11)**2 + (x+(y**2)-7)**2

                # in case our possible new solution is worse, we need to figure out
                # if we should take it or not
                rand_num = np.random.rand()

                # If the possible solution is worse, with the help of the random 
                # number above, we will let a formula decide if we take it or
                # stay where we are
                # the closer we get to the end of our search (the lower the temp. value)
                # it will be less likely to take worse solutions because the radom number will 
                # most likely be greater
                formula = 1/(np.exp((obj_val_possible - obj_val_current)/T0))

                # Do we change our current solution?
                # is the new potencial solution or worse?

                if obj_val_possible <= obj_val_current: #if is better, then we
                                                        # take it and make it our
                                                        #new solution
                        x = x_temporary
                        y = y_temporary
                elif rand_num <= formula: #if it's better, then we take
                        x = x_temporary
                        y = y_temporary    
                else: #if the random number was greater than the formula, it means
                        x = x
                        y = y

        temp.append(T0) # append the temp, to [temp] for plotting reasons
        obj_val.append(obj_val_current) # append the temp, to [temp] for plotting reasons

        T0 = alpha * T0 # since we are done with a certain m (M small), before going to the next m, 
                        # we need to decrease the temp

print()
print("x is: %0.3f" % x)
print("y is: %0.3f" % y)
print("obj val is: %0.3f" % obj_val_current)
print("----------------------------------------")

plt.figure()
plt.plot(temp, obj_val)
plt.title("Z at Temperature Values", fontsize=20, fontweight='bold')
plt.xlabel("Temperature", fontsize=18, fontweight='bold')
plt.ylabel("Z", fontsize=18, fontweight='bold')

plt.xlim(temp_for_plot, 0)
plt.xticks(np.arange(min(temp), max(temp), 100), fontweight='bold')
plt.yticks(fontweight='bold')

plt.show()