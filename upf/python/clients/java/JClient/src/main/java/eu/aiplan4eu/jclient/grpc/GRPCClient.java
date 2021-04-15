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

    public List<String> solve(Problem problem, Heuristic heuristic, String planner) {
        final List<String> plan = new ArrayList<>();
        final Set<String> state = new HashSet<>();
        StreamObserver<ProblemOrHAnswer> streamObserver = asyncStub.solveWithHeuristic(new StreamObserver<PlanOrHRequest>() {
            @Override
            public void onNext(final PlanOrHRequest value) {
                switch (value.getOneof2Case()) {
                    case PLAN:
                        plan.addAll(value.getPlan().getActionsList());
                        break;
                    case STATETOEVALUATE:
                        ProtocolStringList stateList = value.getStateToEvaluate().getStateList();
                        state.clear();
                        for (String s : stateList) {
                            state.add(s);
                        }
                        break;
                    default:
                        throw new RuntimeException("Invalid request!");
                }
            }

            @Override
            public void onError(Throwable t) {

            }

            @Override
            public void onCompleted() {

            }
        });

        streamObserver.onNext(ProblemOrHAnswer.newBuilder().setProblem(encode(problem, planner)).build());
        while (plan.isEmpty()) {
            streamObserver.onNext(ProblemOrHAnswer.newBuilder().setStateEvaluation(heuristic.evaluate(state)).build());
        }

        return plan;
    }


}
