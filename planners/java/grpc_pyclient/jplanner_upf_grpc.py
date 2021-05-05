import grpc
import threading
from contextlib import contextmanager
from time import time
import queue

from dataclasses import dataclass
from typing import Iterable

from JPlannerUPF_pb2 import ActionMessage, ProblemMessage, ProblemOrHAnswer
from JPlannerUPF_pb2_grpc import JPlannerUPFStub

@contextmanager
def timing(description: str) -> None:
    start = time()
    yield
    elapsed_time = (time() - start) * 1000

    print(f"{description}: {elapsed_time}ms")


class StreamStreamWrapper():
    def __init__(self, method):
        self.request_queue = queue.SimpleQueue()
        self.responses = method(iter(self.request_queue.get, None))

    def write(self, x):
        self.request_queue.put(x)

    def close(self):
        self.request_queue.put(None)

    def read(self):
        return next(self.responses)


def _upf_action_to_grpc(action):
    return ActionMessage(name=action.name,
                         precondition=action.precondition,
                         positiveEffect=action.positive_effect,
                         negativeEffect=action.negative_effect)

def _upf_problem_to_grpc(problem):
    actions = [_upf_action_to_grpc(x) for x in problem.actions]
    return ProblemMessage(actions=actions, init=problem.init, goal=problem.goal)


def solve(problem):
    with grpc.insecure_channel('localhost:50051') as channel:
        stub = JPlannerUPFStub(channel)
        return list(stub.solve(_upf_problem_to_grpc(problem)).actions)


def solve_with_heuristic(problem, heuristic):
    with grpc.insecure_channel('localhost:50051') as channel:
        stub = JPlannerUPFStub(channel)
        ssw = StreamStreamWrapper(stub.solveWithHeuristic)
        ssw.write(ProblemOrHAnswer(problem=_upf_problem_to_grpc(problem)))
        while True:
            response = ssw.read()
            if response.HasField('stateToEvaluate'):
                state = frozenset(response.stateToEvaluate.state)
                ssw.write(ProblemOrHAnswer(stateEvaluation=heuristic(state)))
            else:
                ssw.close()
                return response.plan.actions
