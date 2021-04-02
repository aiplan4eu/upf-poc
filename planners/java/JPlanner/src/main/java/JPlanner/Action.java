package JPlanner;

import java.util.Set;

/**
 *
 * @author Andrea Micheli
 */
public class Action {
    private String name;
    private Set<String> precondition;
    private Set<String> positiveEffect;
    private Set<String> negativeEffect;

    public Action(String name, Set<String> precondition, Set<String> positive_effect, Set<String> negative_effect) {
        this.name = name;
        this.precondition = precondition;
        this.positiveEffect = positive_effect;
        this.negativeEffect = negative_effect;
    }

    public String getName() {
        return name;
    }

    public Set<String> getPrecondition() {
        return precondition;
    }

    public Set<String> getPositiveEffect() {
        return positiveEffect;
    }

    public Set<String> getNegativeEffect() {
        return negativeEffect;
    }
}
