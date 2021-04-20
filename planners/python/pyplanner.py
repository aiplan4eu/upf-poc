from dataclasses import dataclass
from typing import FrozenSet, Callable, List
import heapq
from contextlib import contextmanager
from time import time

@contextmanager
def timing(description: str) -> None:
    start = time()
    yield
    elapsed_time = (time() - start) * 1000

    print(f"{description}: {elapsed_time}ms")


@dataclass
class Action:
    name: str
    precondition: FrozenSet[str]
    positive_effect: FrozenSet[str]
    negative_effect: FrozenSet[str]


@dataclass
class Problem:
    actions: List[Action]
    init: FrozenSet[str]
    goal: FrozenSet[str]


class Solver():
    def __init__(self, heuristic: Callable[[FrozenSet[str]], float] = None):
        self.heuristic = heuristic
        if heuristic is None:
            self.heuristic = lambda x: 0

    def solve(self, problem: Problem) -> List[str]:
        open_list = [(self.heuristic(set(problem.init)), problem.init, [])]
        closed_list = set()
        while open_list:
            _, current, path = heapq.heappop(open_list)
            if current not in closed_list:
                closed_list.add(current)
                if problem.goal.issubset(current):
                    return path

                for act in problem.actions:
                    if act.precondition.issubset(current):
                        child = current.difference(act.negative_effect).union(act.positive_effect)
                        if child not in closed_list:
                            child_f = len(path) + 1 + self.heuristic(set(child))
                            heapq.heappush(open_list, (child_f, child, path+[act.name]))


def generate_problem(size: int) -> Problem:
    actions = [Action('mk_y', frozenset(['x']), frozenset(['y']), frozenset(['x'])),
               Action('reset_x', frozenset([]), frozenset(['x']), frozenset([]))]
    goal = []
    for i in range(size):
        name = f"v{i}"
        goal.append(name)
        actions.append(Action(f'mk_{name}', frozenset(['y']), frozenset([name]), frozenset(['y'])),)
    init = frozenset(['x'])
    return Problem(actions, init, frozenset(goal))


def main():
    size = 15
    problem = generate_problem(size)

    def heuristic(state: FrozenSet[str]) -> float:
        return size - len(state & problem.goal)

    with timing("Without Heuristic"):
        solver = Solver(heuristic=None)
        plan = solver.solve(problem)
    print(plan)

    with timing("With Heuristic"):
        solver = Solver(heuristic=heuristic)
        plan = solver.solve(problem)
    print(plan)

if __name__ == '__main__':
    main()
