from concurrent import futures
import grpc
import logging
import threading
import upf

from JClientUPF_pb2 import PlanMessage, PlanOrHRequest, State
import JClientUPF_pb2_grpc


class JClientUPFServicer(JClientUPF_pb2_grpc.JClientUPFServicer):
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
        raise NotImplementedError('Method not implemented!')


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
