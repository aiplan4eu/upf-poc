#include "cppplanner_upf.hxx"

#include "planner.hxx"


std::optional<std::vector<std::string>> solve(upf::Problem& problem, std::function<double(std::set<std::string>)> heuristic)
{
  std::vector<cppplanner::Action> actions;
  for (auto& a : problem.actions()) {
    actions.emplace_back(a.name(), a.precondition(), a.positive_effect(), a.negative_effect());
  }
  cppplanner::Problem planner_problem(actions, problem.init(), problem.goal());
  return cppplanner::solve(planner_problem, heuristic);
}
