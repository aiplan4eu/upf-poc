package eu.aiplan4eu.jplanner.grpc;

import eu.aiplan4eu.jplanner.Action;
import eu.aiplan4eu.jplanner.IHeuristic;
import eu.aiplan4eu.jplanner.Problem;
import eu.aiplan4eu.jplanner.Solver;
import io.grpc.Server;
import io.grpc.ServerBuilder;
import io.grpc.stub.StreamObserver;

import java.io.IOException;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.ReentrantLock;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class GRPCServer {

    private Server server;

    private void start() throws IOException {
        /* The port on which the server should run */
        int port = 50051;
        server = ServerBuilder.forPort(port)
                .addService(new GreeterImpl())
                .build()
                .start();
        System.out.println("Server started, listening on " + port);
        Runtime.getRuntime().addShutdownHook(new Thread() {
            @Override
            public void run() {
                // Use stderr here since the logger may have been reset by its JVM shutdown hook.
                System.err.println("*** shutting down gRPC server since JVM is shutting down");
                try {
                    GRPCServer.this.stop();
                } catch (InterruptedException e) {
                    e.printStackTrace(System.err);
                }
                System.err.println("*** server shut down");
            }
        });
    }

    private void stop() throws InterruptedException {
        if (server != null) {
            server.shutdown().awaitTermination(30, TimeUnit.SECONDS);
        }
    }

    /**
     * Await termination on the main thread since the grpc library uses daemon threads.
     */
    private void blockUntilShutdown() throws InterruptedException {
        if (server != null) {
            server.awaitTermination();
        }
    }

    /**
     * Main launches the server from the command line.
     */
    public static void main(String[] args) throws IOException, InterruptedException {
        final GRPCServer server = new GRPCServer();
        server.start();
        server.blockUntilShutdown();
    }

    static class GreeterImpl extends JPlannerUPFGrpc.JPlannerUPFImplBase {
        @Override
        public void solve(ProblemMessage request, StreamObserver<PlanMessage> responseObserver) {
            Solver solver = new Solver();
            List<String> plan = solver.solve(convertToProblem(request));
            PlanMessage reply = PlanMessage.newBuilder().addAllActions(plan).build();
            responseObserver.onNext(reply);
            responseObserver.onCompleted();
        }

        private static Problem convertToProblem(ProblemMessage pm) {
            Stream<Action> actions = pm.getActionsList().stream().map(GreeterImpl::convertToAction);
            return new Problem(new HashSet<>(actions.collect(Collectors.toList())), new HashSet<>(pm.getInitList()), new HashSet<>(pm.getGoalList()));
        }

        private static Action convertToAction(ActionMessage am) {
            return new Action(am.getName(), new HashSet<>(am.getPreconditionList()), new HashSet<>(am.getPositiveEffectList()), new HashSet<>(am.getNegativeEffectList()));
        }


        private static class GRPCStreamingHeuristic implements IHeuristic {
            private double hValue;
            private StreamObserver<PlanOrHRequest> responseObserver;

            public GRPCStreamingHeuristic(StreamObserver<PlanOrHRequest> responseObserver) {
                this.hValue = 0;
                this.responseObserver = responseObserver;
            }

            public synchronized void setHValue(double hValue) {
                this.hValue = hValue;
                notify();
            }

            @Override
            public synchronized double evaluate(Set<String> state) {
                State stateMsg = State.newBuilder().addAllState(state).build();
                PlanOrHRequest reply = PlanOrHRequest.newBuilder().setStateToEvaluate(stateMsg).build();
                responseObserver.onNext(reply);
                try {
                    wait();
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }
                return hValue;
            }
        }

        private static class SolveWithHeuristicStreamObserver implements StreamObserver<ProblemOrHAnswer> {
            private final GRPCStreamingHeuristic heuristic;
            private final Solver solver;
            private final StreamObserver<PlanOrHRequest> responseObserver;
            private Thread thread;

            public SolveWithHeuristicStreamObserver(StreamObserver<PlanOrHRequest> responseObserver) {
                this.responseObserver = responseObserver;
                this.heuristic = new GRPCStreamingHeuristic(responseObserver);
                this.solver = new Solver(this.heuristic);
                this.thread = null;
            }

            @Override
            public void onNext(final ProblemOrHAnswer value) {
                switch (value.getOneof1Case()) {
                    case PROBLEM:
                        if (thread != null) {
                            throw new RuntimeException("Double problem?");
                        }
                        thread = new Thread(() -> {
                            List<String> plan = solver.solve(convertToProblem(value.getProblem()));
                            PlanMessage reply = PlanMessage.newBuilder().addAllActions(plan).build();
                            responseObserver.onNext(PlanOrHRequest.newBuilder().setPlan(reply).build());
                            responseObserver.onCompleted();
                        });
                        thread.start();
                        break;
                    case STATEEVALUATION:
                        this.heuristic.setHValue(value.getStateEvaluation());
                        break;
                    default:
                        throw new RuntimeException("Invalid request!");
                }
            }

            @Override
            public void onError(Throwable t) {
                System.err.println(t.getMessage());
            }

            @Override
            public void onCompleted() {
                responseObserver.onCompleted();
            }
        }

        @Override
        public StreamObserver<ProblemOrHAnswer> solveWithHeuristic(StreamObserver<PlanOrHRequest> responseObserver) {
            return new SolveWithHeuristicStreamObserver(responseObserver);
        }
    }
}
