package eu.aiplan4eu.jclient;

import java.util.Set;

public class Problem {

    private Set<Action> actions;
    private Set<String> init;
    private Set<String> goal;

    public Problem(Set<Action> actions, Set<String> init, Set<String> goal) {
        this.actions = actions;
        this.init = init;
        this.goal = goal;
    }

    public Set<Action> getActions() {
        return actions;
    }

    public Set<String> getInit() {
        return init;
    }

    public Set<String> getGoal() {
        return goal;
    }

}
