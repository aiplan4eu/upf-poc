import _pywrapper_upf as upf
from time import time
from contextlib import contextmanager


@contextmanager
def Timing(description: str) -> None:
    start = time()
    yield
    elapsed_time = (time() - start) * 1000

    print(f"{description}: {elapsed_time}ms")



def generate_problem(size: int) -> upf.Problem:
    actions = [upf.Action('mk_y', set(['x']), set(['y']), set(['x'])),
               upf.Action('reset_x', set([]), set(['x']), set([]))]
    goal = []
    for i in range(size):
        name = f"v{i}"
        goal.append(name)
        actions.append(upf.Action(f'mk_{name}', set(['y']), set([name]), set(['y'])),)
    init = set(['x'])
    return upf.Problem(actions, init, set(goal))


class GoalCounterHeuristic:
    def __init__(self, size, problem):
        self.size = size
        self.problem = problem

    def __call__(self, state):
        return self.size - len(state & self.problem.goal)


def main():
    size = 14

    problem = generate_problem(size)

    planners = ["cppplanner_upf.so", "pyplanner_upf.so", "jplanner_upf.so", "jplanner_upf_grpc.so"]
    for name in planners:
        upf.init(name)
        with Timing("%s with no heuristic" % name):
            plan = upf.solve(name, problem)
            print(plan)
        with Timing("%s with heuristic" % name):
            plan = upf.solve_with_heuristic(name, problem, GoalCounterHeuristic(size, problem))
            print(plan)
        upf.uninit(name)


if __name__ == '__main__':
    main()
