from concurrent import futures
import grpc
import logging
import threading
import queue
import upf

from JClientUPF_pb2 import PlanMessage, PlanOrHRequest, State
import JClientUPF_pb2_grpc


class JClientUPFServicer(JClientUPF_pb2_grpc.JClientUPFServicer):

    def __init__(self):
        JClientUPF_pb2_grpc.JClientUPFServicer.__init__(self)
        self.lock = threading.Lock()
        self.lock.acquire() # Start acquired!
        self.state_evaluation = None
        self.queue = queue.Queue()

    def solve(self, request, context):
        actions = []
        for a in request.actions:
            actions.append(upf.Action(a.name, set(a.precondition), set(a.positiveEffect), set(a.negativeEffect)))
        problem = upf.Problem(actions, set(request.init), set(request.goal))
        plan = []
        with upf.Planner(request.planner) as p:
            plan = p.solve(problem)
        return PlanMessage(actions=plan)

    def solveWithHeuristic(self, request_iterator, context):
        threading.Thread(target=self._response_thread, args=(request_iterator,)).start()
        while True:
            x = self.queue.get()
            if x is None:
                break
            #print("Send: %s" % x)
            yield x

    def heuristic(self, state):
        self.queue.put(PlanOrHRequest(stateToEvaluate=State(state=state)))
        self.lock.acquire()
        return self.state_evaluation

    def _response_thread(self, request_iterator):
        for request in request_iterator:
            #print("Recv: %s" % request)
            if request.HasField("problem"):
                pm = request.problem
                actions = []
                for a in pm.actions:
                    actions.append(upf.Action(a.name, set(a.precondition), set(a.positiveEffect), set(a.negativeEffect)))
                problem = upf.Problem(actions, set(pm.init), set(pm.goal))
                threading.Thread(target=self._planner_thread, args=(pm.planner, problem)).start()
            else:
                assert request.HasField("stateEvaluation")
                self.state_evaluation = request.stateEvaluation
                self.lock.release()

    def _planner_thread(self, planner, problem):
        with upf.Planner(planner) as p:
            plan = p.solve(problem, self.heuristic)
            self.queue.put(PlanOrHRequest(plan=PlanMessage(actions=plan)))
            self.queue.put(None)


def serve():
  server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
  JClientUPF_pb2_grpc.add_JClientUPFServicer_to_server(
      JClientUPFServicer(), server)
  server.add_insecure_port('[::]:50052')
  server.start()
  server.wait_for_termination()

if __name__ == '__main__':
    logging.basicConfig()
    serve()
