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


def generate_problem(size: int) -> Problem:
    actions = [Action('mk_y', set(['x']), set(['y']), set(['x'])),
               Action('reset_x', set([]), set(['x']), set([]))]
    goal = []
    for i in range(size):
        name = f"v{i}"
        goal.append(name)
        actions.append(Action(f'mk_{name}', set(['y']), set([name]), set(['y'])),)
    init = set(['x'])
    return Problem(actions, init, set(goal))


def main():
    size = 15
    problem = generate_problem(size)

    with timing("Without Heuristic"):
        plan = solve(problem)
    print(plan)

    def heuristic(state: Set[str]) -> float:
        return size - len(state & problem.goal)

    with timing("With Heuristic"):
        plan = solve(problem, heuristic)
    print(plan)

if __name__ == '__main__':
    main()
