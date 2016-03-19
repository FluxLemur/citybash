import numpy as np
import string

def array_print(xs):
    s = '{'
    s += string.join([str(x) for x in xs], ', ')
    s += '}'
    print s

upgrade_costs = np.power(2, range(1, 11))
upgrade_times = np.multiply(5, range(1, 11))
incomes = [int(np.power(1.5, i)) for i in range(1, 11)]

array_print(upgrade_costs)
array_print(upgrade_times)
array_print(incomes)
