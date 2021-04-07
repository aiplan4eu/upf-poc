import pyplanner as pp

def _upf_action_to_pp(action):
    return pp.Action(action.name,
                     frozenset(action.precondition),
                     frozenset(action.positive_effect),
                     frozenset(action.negative_effect))

def _upf_problem_to_pp(problem):
    actions = [_upf_action_to_pp(x) for x in problem.actions]
    return pp.Problem(actions, frozenset(problem.init), frozenset(problem.goal))

def solve(problem):
    s = pp.Solver()
    return s.solve(_upf_problem_to_pp(problem))

def solve_with_heuristic(problem, h):
    s = pp.Solver(h)
    return s.solve(_upf_problem_to_pp(problem))
