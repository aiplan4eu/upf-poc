import grpc
import threading
from contextlib import contextmanager
from time import time


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


@dataclass
class StateContainer():
    state: Iterable[str]

def HIterator(problem_message, lock, state_container, heuristic):
    try:
        yield ProblemOrHAnswer(problem=problem_message)
        while True:
            lock.acquire()
            state = state_container.state
            if state is None:
                break
            res = heuristic(frozenset(state))
            #print(res)
            yield ProblemOrHAnswer(stateEvaluation=res)
        return
    except Exception as e:
        print(e)


def _do_solve_with_h(stub, problem_message, heuristic):
    lock = threading.Lock()
    lock.acquire();
    state_container = StateContainer(None)
    it = HIterator(problem_message, lock, state_container, heuristic)
    responses = stub.solveWithHeuristic(it)
    for response in responses:
        if response.HasField('stateToEvaluate'):
            state_container.state = response.stateToEvaluate.state
            #print(state_container.state)
            lock.release()
        else:
            state_container.state = None
            lock.release()
            return response.plan



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

def solve_with_heuristic(problem, h):
    with grpc.insecure_channel('localhost:50051') as channel:
        stub = JPlannerUPFStub(channel)
        return list(_do_solve_with_h(stub, _upf_problem_to_grpc(problem), h).actions)
