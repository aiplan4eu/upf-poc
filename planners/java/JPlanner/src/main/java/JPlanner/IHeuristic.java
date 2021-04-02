package JPlanner;

import java.util.Set;

/**
 *
 * @author Andrea Micheli
 */
public interface IHeuristic {
    
    public double evaluate(Set<String> state);
    
}
