package JPlanner;

import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.PriorityQueue;
import java.util.Set;

/**
 *
 * @author Andrea Micheli
 */
public class Solver {
    
    private final IHeuristic heuristic;

    public Solver(IHeuristic heuristic) {
        this.heuristic = heuristic;
    }

    public Solver() {
        this.heuristic = (Set<String> state) -> 0;
    }
    
    static class E implements Comparable<E> {
        public Set<String> state;
        public List<String> path;
        public double f;

        public E(Set<String> state, List<String> path, double f) {
            this.state = state;
            this.path = path;
            this.f = f;
        }

        @Override
        public int compareTo(E oth) {
            if (f - oth.f > 0) {
                return 1;
            }
            if (f - oth.f < 0) {
                return -1;
            }
            return 0;
        }
        
    } 
    
    public List<String> solve(Problem problem) {
        final PriorityQueue<E> queue = new PriorityQueue<>();
        final Set<Set<String>> visited = new HashSet<>();
        queue.add(new E(problem.getInit(), new LinkedList(), heuristic.evaluate(problem.getInit())));
        
        while(!queue.isEmpty()) {
            final E current = queue.poll();
            if (!visited.contains(current.state)) {
                visited.add(current.state);
                if (current.state.containsAll(problem.getGoal())) {
                    return current.path;
                }
                else {
                    for(Action a : problem.getActions()) {
                        if (current.state.containsAll(a.getPrecondition())) {
                            Set<String> child = new HashSet<>(current.state);
                            child.removeAll(a.getNegativeEffect());
                            child.addAll(a.getPositiveEffect());
                            double f = current.path.size() + 1 + heuristic.evaluate(child);
                            List<String> child_path = new LinkedList<>(current.path);
                            child_path.add(a.getName());
                            queue.add(new E(child, child_path, f));
                        }
                    }
                }
            }
        }
        return null;
    }
    
}
