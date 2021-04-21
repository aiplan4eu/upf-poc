package eu.aiplan4eu.jclient;

import eu.aiplan4eu.jclient.grpc.GRPCClient;
import io.grpc.ManagedChannel;
import io.grpc.ManagedChannelBuilder;

import java.util.*;
import java.util.concurrent.TimeUnit;

public class Main {

    public static Problem generateProblem(int size) {
        Set<Action> actions = new HashSet<>();
        Action a1 = new Action("mk_y", Collections.singleton("x"), Collections.singleton("y"), Collections.singleton("x"));
        actions.add(a1);
        Action a2 = new Action("reset_x", Collections.emptySet(), Collections.singleton("x"), Collections.emptySet());
        actions.add(a2);
        Set<String> goal = new HashSet<>();
        for (int i = 0; i < size; i++) {
            String name = "v"+i;
            goal.add(name);
            Action a = new Action("mk_"+name, Collections.singleton("y"), Collections.singleton(name), Collections.singleton("y"));
            actions.add(a);
        }
        Problem problem = new Problem(actions, Collections.singleton("x"), goal);
        return problem;
    }

    public static void main(String[] args) throws InterruptedException {
        Problem problem = generateProblem(15);

        List<String> planners = List.of("/home/alvalentini/aiplan4eu/upf-poc/planners/cpp/cppplanner_cppupf.so",
                                        "/home/alvalentini/aiplan4eu/upf-poc/planners/python/pyplanner_cppupf.so",
                                        "/home/alvalentini/aiplan4eu/upf-poc/planners/java/JPlanner/grpc_cpp_client_test/jplanner_cppupf.so");

        String target = "localhost:50052";
        ManagedChannel channel = ManagedChannelBuilder.forTarget(target).usePlaintext().build();
        try {
            GRPCClient client = new GRPCClient(channel);
            for (String planner : planners) {
                System.out.println(planner + " with no heuristic:");
                List<String> plan1 = client.solve(problem, planner);
                System.out.println(plan1);
                // System.out.println(planner + " with heuristic:");
                // List<String> plan2 = client.solve(problem, new Heuristic(problem.getGoal()), planner);
                // System.out.println(plan2);
                // System.out.println();
            }
        } finally {
            channel.shutdownNow().awaitTermination(5, TimeUnit.SECONDS);
        }
    }

}
