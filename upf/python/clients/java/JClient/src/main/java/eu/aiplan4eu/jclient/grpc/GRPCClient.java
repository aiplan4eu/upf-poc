package eu.aiplan4eu.jclient.grpc;

import com.google.protobuf.ProtocolStringList;
import eu.aiplan4eu.jclient.Action;
import eu.aiplan4eu.jclient.Heuristic;
import eu.aiplan4eu.jclient.Problem;
import io.grpc.Channel;
import io.grpc.stub.StreamObserver;

import java.util.*;
import java.util.logging.Logger;

import static eu.aiplan4eu.jclient.grpc.PlanOrHRequest.Oneof2Case.STATETOEVALUATE;

public class GRPCClient {

    private static final Logger logger = Logger.getLogger(GRPCClient.class.getName());

    private final JClientUPFGrpc.JClientUPFBlockingStub blockingStub;
    private final JClientUPFGrpc.JClientUPFStub asyncStub;

    public GRPCClient(Channel channel) {
        blockingStub = JClientUPFGrpc.newBlockingStub(channel);
        asyncStub = JClientUPFGrpc.newStub(channel);
    }

    private ProblemMessage encode(Problem problem, String planner) {
        Set<String> init = problem.getInit();
        Set<String> goals = problem.getGoal();
        Set<ActionMessage> actions = new HashSet<>();
        for (Action a : problem.getActions()) {
            actions.add(ActionMessage.newBuilder().setName(a.getName()).addAllPrecondition(a.getPrecondition()).addAllPositiveEffect(a.getPositiveEffect()).addAllNegativeEffect(a.getNegativeEffect()).build());
        }
        ProblemMessage problemMessage = ProblemMessage.newBuilder().setPlanner(planner).addAllActions(actions).addAllInit(init).addAllGoal(goals).build();
        return problemMessage;
    }

    public List<String> solve(Problem problem, String planner) {
        PlanMessage plan = blockingStub.solve(encode(problem, planner));
        return plan.getActionsList();
    }

    static private class RequestStreamObserver implements StreamObserver<PlanOrHRequest> {

        private StreamObserver<ProblemOrHAnswer> answerChannel;
        private List<String> plan;
        private Heuristic heuristic;
        private boolean ready;

        public RequestStreamObserver(Heuristic heuristic) {
            this.heuristic = heuristic;
            plan = new ArrayList<>();
            answerChannel = null;
            ready = false;
        }

        @Override
        public void onNext(PlanOrHRequest value) {
            //System.out.println("Recv: " + value);
            switch (value.getOneof2Case()) {
                case PLAN:
                    //System.out.println("Got plan " + value.getPlan());
                    plan.addAll(value.getPlan().getActionsList());
                    synchronized (this) {
                        this.notify();
                    }
                    ready = true;
                    break;
                case STATETOEVALUATE:
                    ProtocolStringList stateList = value.getStateToEvaluate().getStateList();
                    final double hval = heuristic.evaluate(new HashSet<>(stateList));
                    answerChannel.onNext(ProblemOrHAnswer.newBuilder().setStateEvaluation(hval).build());
                    break;
                default:
                    throw new RuntimeException("Invalid request!");
            }
        }

        @Override
        public void onError(Throwable t) {
            t.printStackTrace();
        }

        @Override
        public void onCompleted() {
            //System.out.println("Completed!");
        }

        public void setAnswerChannel(StreamObserver<ProblemOrHAnswer> answerChannel) {
            this.answerChannel = answerChannel;
        }

        public List<String> getPlan() {
            return plan;
        }

        public synchronized boolean isReady() {
            return ready;
        }
    }

    public List<String> solve(Problem problem, Heuristic heuristic, String planner) {
        RequestStreamObserver requestStreamObserver = new RequestStreamObserver(heuristic);
        StreamObserver<ProblemOrHAnswer> responseStreamObserver = asyncStub.solveWithHeuristic(requestStreamObserver);
        requestStreamObserver.setAnswerChannel(responseStreamObserver);

        //System.out.println("Send: problem");
        responseStreamObserver.onNext(ProblemOrHAnswer.newBuilder().setProblem(encode(problem, planner)).build());

//        while (!requestStreamObserver.isReady()) {
//            try {
//                Thread.sleep(1);
//            } catch (InterruptedException e) {
//                e.printStackTrace();
//            }
//        }

        synchronized (requestStreamObserver) {
            try {
                requestStreamObserver.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        List<String> res = requestStreamObserver.getPlan();
        responseStreamObserver.onCompleted();
        return res;
    }


}
