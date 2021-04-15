package eu.aiplan4eu.jclient;

import com.google.common.collect.Sets;

import java.util.Set;

public class Heuristic {

    private Set<String> goals;

    public Heuristic(Set<String> goals) {
        this.goals = goals;
    }

    public double evaluate(Set<String> state) {
        return goals.size() - Sets.intersection(goals, state).size();
    }
}
