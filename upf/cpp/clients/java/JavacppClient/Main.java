import java.nio.*;
import org.bytedeco.javacpp.*;
import org.bytedeco.javacpp.annotation.*;

import java.util.*;

class Main {

    public static UPF.Problem generateProblem(int size) {
        UPF.ActionVector actions = new UPF.ActionVector();
        {
            UPF.StringSet pre = new UPF.StringSet();
            pre.insert(new BytePointer("x"));
            UPF.StringSet pos = new UPF.StringSet();
            pos.insert(new BytePointer("y"));
            UPF.StringSet neg = new UPF.StringSet();
            neg.insert(new BytePointer("x"));
            UPF.Action a = new UPF.Action("mk_y", pre, pos, neg);
            actions.push_back(a);
        }

        {
            UPF.StringSet pre = new UPF.StringSet();
            UPF.StringSet pos = new UPF.StringSet();
            pos.insert(new BytePointer("x"));
            UPF.StringSet neg = new UPF.StringSet();
            UPF.Action a = new UPF.Action("reset_x", pre, pos, neg);
            actions.push_back(a);
        }

        UPF.StringSet init = new UPF.StringSet();
        init.insert(new BytePointer("x"));
        UPF.StringSet goal = new UPF.StringSet();
        for (int i = 0; i < size; i++) {
            String name = "v"+i;
            goal.insert(new BytePointer(name));
            UPF.StringSet pre = new UPF.StringSet();
            pre.insert(new BytePointer("y"));
            UPF.StringSet pos = new UPF.StringSet();
            pos.insert(new BytePointer(name));
            UPF.StringSet neg = new UPF.StringSet();
            neg.insert(new BytePointer("y"));
            UPF.Action a = new UPF.Action("mk_"+name, pre, pos, neg);
            actions.push_back(a);
        }
        UPF.Problem p = new UPF.Problem(actions, init, goal);
        return p;
    }

    public static class GoalCounter extends UPF.Heuristic {

        public GoalCounter(UPF.StringSet goals) {
            this.goals = new HashSet<>();
            for (UPF.StringSet.Iterator it = goals.begin(); !it.equals(goals.end()); it.increment()) {
                this.goals.add(it.get());
            }
        }

        @Override
        public double apply(@Const @ByRef UPF.StringSet state) {
            double res = goals.size();
            for (BytePointer x : goals) {
                if (UPF.set_contains(state, x)) {
                    res -= 1;
                }
            }
            return res;
        }

        private Set<BytePointer> goals;

    }

    public static void main(String[] args) {
        UPF.Problem p = generateProblem(14);

        GoalCounter h = new GoalCounter(p.goal());

        List<String> planners = List.of("cppplanner_upf.so", "pyplanner_upf.so", "jplanner_upf.so", "jplanner_upf_grpc.so");

        for (String planner : planners) {
            UPF.init(planner);
            long begin = System.currentTimeMillis();
            UPF.StringVector res1 = UPF.solve(planner, p).get();
            long end = System.currentTimeMillis();
            List<String> plan1 = new ArrayList<>();
            for (int i=0; i<res1.size(); i++) {
                plan1.add(res1.get(i).getString());
            }
            System.out.println(plan1);
            System.out.println(planner + " with no heuristic: " + (end - begin) + "ms");

            begin = System.currentTimeMillis();
            UPF.StringVector res2 = UPF.solve(planner, p, h).get();
            end = System.currentTimeMillis();
            List<String> plan2 = new ArrayList<>();
            for (int i=0; i<res2.size(); i++) {
                plan2.add(res2.get(i).getString());
            }
            System.out.println(plan2);
            System.out.println(planner + " with heuristic: " + (end - begin) + "ms");
            UPF.uninit(planner);
        }

    }

}
