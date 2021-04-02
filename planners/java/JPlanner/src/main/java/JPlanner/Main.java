package JPlanner;

import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

/**
 *
 * @author Andrea Micheli
 */
public class Main {
    
    public static final <T> Set<T> newHashSet(T... objs) {
        Set<T> set = new HashSet<>();
        Collections.addAll(set, objs);
        return set;
    }

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {        
        Action a = new Action("a", Collections.singleton("x"), Collections.singleton("y"), Collections.singleton("x"));
        Action b = new Action("b", Collections.singleton("y"), Collections.singleton("z"), Collections.singleton("y"));
        Action c = new Action("c", Collections.singleton("y"), Collections.singleton("w"), Collections.singleton("y"));
        Action d = new Action("d", Collections.emptySet(), Collections.singleton("x"), Collections.emptySet());

        Problem problem = new Problem(newHashSet(a, b, c, d), Collections.singleton("x"), newHashSet("z", "w"));

        Solver solver = new Solver((Set<String> state) -> {
            if (state.contains("z")) return 1;
            if (state.contains("w")) return 1;
            return 10;
        });
        List<String> plan = solver.solve(problem);
        System.out.println(plan);
    }
    
}
