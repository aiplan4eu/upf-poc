import _cppplanner_pyupf as cpp

def _upf_action_to_cpp(action):
    return cpp.Action(action.name, action.precondition, action.positive_effect, action.negative_effect)

def _upf_problem_to_cpp(problem):
    actions = [_upf_action_to_cpp(x) for x in problem.actions]
    return cpp.Problem(actions, problem.init, problem.goal)

def solve(problem):
    return cpp.solve(_upf_problem_to_cpp(problem))

def solve_with_heuristic(problem, h):
    return cpp.solve_with_heuristic(_upf_problem_to_cpp(problem), h)
