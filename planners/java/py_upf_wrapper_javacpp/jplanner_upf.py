import atexit
import _jplanner_upf as jp

jp.init()
atexit.register(jp.uninit)


def _upf_action_to_java(action):
    return jp.Action(action.name, action.precondition, action.positive_effect, action.negative_effect)

def _upf_problem_to_java(problem):
    actions = [_upf_action_to_java(x) for x in problem.actions]
    return jp.Problem(actions, problem.init, problem.goal)

def solve(problem):
    return jp.solve(_upf_problem_to_java(problem))

def solve_with_heuristic(problem, h):
    return jp.solve_with_heuristic(_upf_problem_to_java(problem), h)
