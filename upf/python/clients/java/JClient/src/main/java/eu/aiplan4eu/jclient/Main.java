package eu.aiplan4eu.jclient;

import eu.aiplan4eu.jclient.grpc.GRPCClient;
import io.grpc.ManagedChannel;
import io.grpc.ManagedChannelBuilder;

import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.concurrent.TimeUnit;

public class Main {

    public static final <T> Set<T> newHashSet(T... obs) {
        Set<T> set = new HashSet<>();
        Collections.addAll(set, obs);
        return set;
    }

    public static void main(String[] args) throws InterruptedException {
        Action a = new Action("a", Collections.singleton("x"), Collections.singleton("y"), Collections.singleton("x"));
        Action b = new Action("b", Collections.singleton("y"), Collections.singleton("z"), Collections.singleton("y"));
        Action c = new Action("c", Collections.singleton("y"), Collections.singleton("w"), Collections.singleton("y"));
        Action d = new Action("d", Collections.emptySet(), Collections.singleton("x"), Collections.emptySet());

        Problem problem = new Problem(newHashSet(a, b, c, d), Collections.singleton("x"), newHashSet("z", "w"));

        List<String> planners = List.of("pyplanner_upf", "cppplanner_upf", "jplanner_upf");

        String target = "localhost:50052";
        ManagedChannel channel = ManagedChannelBuilder.forTarget(target).usePlaintext().build();
        try {
            GRPCClient client = new GRPCClient(channel);
            for (String planner : planners) {
                System.out.println(planner + " with no heuristic:");
                List<String> plan1 = client.solve(problem, planner);
                System.out.println(plan1);
                // System.out.println(planner + " with heuristic:");
                // List<String> plan2 = client.solve(problem, new Heuristic(), planner);
                // System.out.println(plan2);
                System.out.println();
            }
        } finally {
            channel.shutdownNow().awaitTermination(5, TimeUnit.SECONDS);
        }
    }

}
