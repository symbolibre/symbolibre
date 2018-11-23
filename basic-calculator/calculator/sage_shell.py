from __future__ import print_function
import sys
from sage.all import *
from sage.misc.functional import *

_SL_DEFAULT_APPROX = 10

sage_eval("42")

def my_approx(quantity):
	if (quantity == int(quantity)): return quantity
	else:
		approx = str(numerical_approx(res, digits=_SL_DEFAULT_APPROX))
		i = 1
		while (approx[-i] == '0'): i += 1
		return approx if i == 1 else approx[:-i+1]




while(1):
	ask = raw_input()
	if (ask == "quit"): exit(0)
	try:
		res = eval(preparse(str(ask)))
		print(res, "=", my_approx(res), file=sys.stdout)
	except ZeroDivisionError:
		print("Division by zero")
	except:
		print("Unknown Sage error")
	sys.stdout.flush()