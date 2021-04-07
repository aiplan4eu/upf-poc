import importlib

from dataclasses import dataclass
from typing import FrozenSet, Callable, List

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
    def solve(problem: Problem, heuristic: Callable[[FrozenSet[str]], float] = None) -> List[str]:
        raise NotImplementedError

    def destroy():
        raise NotImplementedError


class LinkedSolver(Solver):
    def __init__(self, module_name):
        self.module = importlib.import_module(module_name)
        print('Creating planner %s' % module_name)

    def solve(self, problem: Problem, heuristic: Callable[[FrozenSet[str]], float] = None) -> List[str]:
        if heuristic is None:
            plan = self.module.solve(problem)
        else:
            plan = self.module.solve_with_heuristic(problem, heuristic)
        return plan

    def destroy(self):
        print("Linked Solver destroyed")

from contextlib import contextmanager

@contextmanager
def Planner(module_name):
    a = LinkedSolver(module_name)
    try:
        yield a
    finally:
        a.destroy()


if __name__ == '__main__':

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

    size = 10
    def goal_counter(state: FrozenSet[str]) -> float:
        return size - len(state & problem.goal)

    problem = generate_problem(size)

    planners = ['cppplanner_upf', 'pyplanner_upf', 'jplanner_upf']
    for name in planners:
        with Planner(name) as p:
            plan = p.solve(problem)
            print(plan)

            plan = p.solve(problem, goal_counter)
            print(plan)
