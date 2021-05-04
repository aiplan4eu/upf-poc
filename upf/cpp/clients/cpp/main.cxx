#include "upf.hxx"

#include <iostream>
#include <chrono>

struct heuristic
{
  heuristic(size_t size_, std::set<std::string> goal_) : size(size_), goal(goal_) {}

  double operator()(const std::set<std::string>& state)
  {
    double res = size;
    for (auto& x : goal) {
      if (state.find(x) != state.end()) {
        res -= 1.0;
      }
    }
    return res;
  }

  size_t size;
  std::set<std::string> goal;
};

upf::Problem generate_problem(size_t size)
{
  std::vector<upf::Action> actions;
  std::set<std::string> init;
  std::set<std::string> goal;
  init.insert("x");
  upf::Action a1("mk_y", {"x"}, {"y"}, {"x"});
  actions.push_back(a1);
  upf::Action a2("reset_x", {}, {"x"}, {});
  actions.push_back(a2);
  for (size_t i=0; i<size; i++) {
    std::string name = "v" + std::to_string(i);
    upf::Action a("mk_"+name, {"y"}, {name}, {"y"});
    actions.push_back(a);
    goal.insert(name);
  }
  upf::Problem problem(actions, init, goal);
  return problem;
}

int main() {
  size_t size = 14;

  auto problem = generate_problem(size);

  std::vector<std::string>
    planners = {"cppplanner_upf.so", "pyplanner_upf.so", "jplanner_upf.so", "jplanner_upf_grpc.so"};

  for (auto& planner : planners) {
    upf::init(planner);
    {
      auto clock = std::chrono::steady_clock::now();
      if (auto plan = upf::solve(planner, problem)) {
        std::cout << "[";
        bool first = true;
        for (auto& s : *plan) {
          if (!first) {
            std::cout << ", ";
          }
          first = false;
          std::cout << s;
        }
        std::cout << "]" << std::endl;
      }
      auto now = std::chrono::steady_clock::now();
      auto duration
        = std::chrono::duration_cast<std::chrono::milliseconds>(now - clock);
      std::cout << planner << " with no heuristic: " << duration.count() << "ms" << std::endl;
    }

    {
      auto clock = std::chrono::steady_clock::now();
      if (auto plan = upf::solve(planner, problem, heuristic(size, problem.goal()))) {
        std::cout << "[";
        bool first = true;
        for (auto& s : *plan) {
          if (!first) {
            std::cout << ", ";
          }
          first = false;
          std::cout << s;
        }
        std::cout << "]" << std::endl;
      }
      auto now = std::chrono::steady_clock::now();
      auto duration
        = std::chrono::duration_cast<std::chrono::milliseconds>(now - clock);
      std::cout << planner << " with heuristic: " << duration.count() << "ms" << std::endl;
    }
    upf::uninit(planner);
  }

  return 0;
}
