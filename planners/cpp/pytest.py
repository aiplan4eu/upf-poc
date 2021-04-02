from cppplanner_upf import *
from typing import Set
from contextlib import contextmanager
from time import time

@contextmanager
def timing(description: str) -> None:
    start = time()
    yield
    elapsed_time = (time() - start) * 1000

    print(f"{description}: {elapsed_time}ms")

def main():
    actions = [Action('a', set(['x']), set(['y']), set(['x'])),
               Action('b', set(['y']), set(['z']), set(['y'])),
               Action('c', set(['y']), set(['w']), set(['y'])),
               Action('d', set([]), set(['x']), set([]))]
    problem = Problem(actions, set(['x']), set(['z', 'w']))

    with timing("Without Heuristic"):
        plan = solve(problem)
    print(plan)

    def heuristic(state: Set[str]) -> float:
        if 'z' in state:
            return 1.0
        if 'w' in state:
            return 1.0
        return 10.0

    with timing("With Heuristic"):
        plan = solve(problem, heuristic)
    print(plan)

if __name__ == '__main__':
    main()
