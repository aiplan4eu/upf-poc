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
            res = heuristic(state)
            #print(res)
            yield ProblemOrHAnswer(stateEvaluation=res)
        return
    except Exception as e:
        print(e)


def solve_with_h(stub, problem_message, heuristic):
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
            return response.plan.actions


def main():
    with grpc.insecure_channel('localhost:50051') as channel:
        stub = JPlannerUPFStub(channel)

        action_messages = [ActionMessage(name='a', precondition=['x'], positiveEffect=['y'], negativeEffect=['x']),
                           ActionMessage(name='b', precondition=['y'], positiveEffect=['z'], negativeEffect=['y']),
                           ActionMessage(name='c', precondition=['y'], positiveEffect=['w'], negativeEffect=['y']),
                           ActionMessage(name='d', precondition=[],    positiveEffect=['x'], negativeEffect=[])]
        problem_message = ProblemMessage(actions=action_messages, init=['x'], goal=['z', 'w'])

        with timing("Without Heuristic"):
            plan_message = stub.solve(problem_message)
        print(plan_message.actions)


        ################################################################################
        ## With heuristic
        def heuristic(state):
            if 'z' in state:
                return 5.0
            if 'w' in state:
                return 1.0
            return 10.0
        with timing("With Heuristic"):
            plan = solve_with_h(stub, problem_message, heuristic)
        print(plan)


if __name__ == '__main__':
    main()
