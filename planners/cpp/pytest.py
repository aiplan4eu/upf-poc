from cppplanner_upf import *
from typing import Set

actions = [Action('a', set(['x']), set(['y']), set(['x'])),
           Action('b', set(['y']), set(['z']), set(['y'])),
           Action('c', set(['y']), set(['w']), set(['y'])),
           Action('d', set([]), set(['x']), set([]))]
problem = Problem(actions, set(['x']), set(['z', 'w']))

plan = solve(problem)
print(plan)

def heuristic(state: Set[str]) -> float:
    if 'z' in state:
        return 1.0
    if 'w' in state:
        return 1.0
    return 10.0

plan = solve(problem, heuristic)
print(plan)
