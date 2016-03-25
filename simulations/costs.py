import numpy as np
import string

def array_print(xs):
    s = '{'
    s += string.join([str(x) for x in xs], ', ')
    s += '}'
    print s + '\n'

LEVELS = 5

upgrade_costs = [int(10*np.power(3, i)) for i in range(1, LEVELS+1)]
upgrade_times = np.multiply(5, range(1, LEVELS+1))
incomes = [int(i + 0.1 * np.power(2, i)) for i in range(1, LEVELS+1)]
s_cost = 5 # soldier cost

print 'upgrade costs: '
array_print(upgrade_costs)
print 'upgrade times: '
array_print(upgrade_times)
print 'incomes: '
array_print(incomes)

print 'time to upgrade only by income: '
for income, cost in zip(incomes, upgrade_costs):
    print int(cost * 1.0 / income),
print ''

print '\ntime to get gold = soldier cost'
for income in incomes:
    print '{:.2f}'.format(s_cost * 1.0 / income),
