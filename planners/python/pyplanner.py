from dataclasses import dataclass
from typing import FrozenSet, Callable, List
import heapq

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
        open_list = [(self.heuristic(problem.init), problem.init, [])]
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
                            child_f = len(path) + 1 + self.heuristic(child)
                            heapq.heappush(open_list, (child_f, child, path+[act.name]))


def main():
    actions = [Action('a', frozenset(['x']), frozenset(['y']), frozenset(['x'])),
               Action('b', frozenset(['y']), frozenset(['z']), frozenset(['y'])),
               Action('c', frozenset(['y']), frozenset(['w']), frozenset(['y'])),
               Action('d', frozenset([]), frozenset(['x']), frozenset([]))]
    problem = Problem(actions, frozenset(['x']), frozenset(['z', 'w']))

    def heuristic(state: FrozenSet[str]) -> float:
        if 'z' in state:
            return 1.0
        if 'w' in state:
            return 1.0
        return 10.0

    solver = Solver(heuristic=heuristic)
    plan = solver.solve(problem)
    print(plan)

if __name__ == '__main__':
    main()
