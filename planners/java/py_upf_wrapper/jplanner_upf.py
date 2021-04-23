import os
filepath = os.path.dirname(os.path.abspath(__file__))

import jnius_config
jnius_config.set_classpath(os.path.join(filepath, '../JPlanner/build/classes/java/main'))

from jnius import autoclass, PythonJavaClass, java_method

Problem = autoclass('eu.aiplan4eu.jplanner.Problem')
Action = autoclass('eu.aiplan4eu.jplanner.Action')
Solver = autoclass('eu.aiplan4eu.jplanner.Solver')
IHeuristic = autoclass('eu.aiplan4eu.jplanner.IHeuristic')
HashSet = autoclass('java.util.HashSet')


def convert_action(action):
    precondition = HashSet()
    for p in action.precondition:
        precondition.add(p)
    positive_effect = HashSet()
    for p in action.positive_effect:
        positive_effect.add(p)
    negative_effect = HashSet()
    for p in action.negative_effect:
        negative_effect.add(p)
    return Action(action.name, precondition, positive_effect, negative_effect)


def convert_problem(problem):
    init = HashSet()
    for i in problem.init:
        init.add(i)
    goal = HashSet()
    for g in problem.goal:
        goal.add(g)
    actions = HashSet()
    for a in problem.actions:
        action = convert_action(a)
        actions.add(action)
    return Problem(actions, init, goal)


class Heuristic(PythonJavaClass):
    __javainterfaces__ = ['eu/aiplan4eu/jplanner/IHeuristic']

    def __init__(self, heuristic):
        self.heuristic = heuristic

    @java_method('(Ljava/util/Set;)D')
    def evaluate(self, state):
        s = set([i for i in state.iterator()])
        return self.heuristic(s)


def solve(problem):
    plan = Solver().solve(convert_problem(problem))
    res = []
    if plan is not None:
        for i in range(plan.size()):
            res.append(plan.get(i))
    return res


def solve_with_heuristic(problem, heuristic):
    h = Heuristic(heuristic)
    plan = Solver(h).solve(convert_problem(problem))
    res = []
    if plan is not None:
        for i in range(plan.size()):
            res.append(plan.get(i))
    return res
