#include <iostream>
#include <functional>
#include <chrono>
#include "jniPlannerWrapper.h"

#include "problem.hxx"
#include "heuristic.hxx"

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

class NewHeuristic : public Heuristic {
public:
  NewHeuristic(std::function<double(std::set<std::string>)> heuristic) :
    heuristic_(heuristic) {}

  double operator()(const std::set<std::string>& s) override
  {
    return heuristic_(s);
  }

private:
  std::function<double(std::set<std::string>)> heuristic_;
};


int main() {
  JavaCPP_init(0, NULL);
  try {
    size_t size = 14;
    auto p = generate_problem(size);
    {
      auto clock = std::chrono::steady_clock::now();
      auto v = jsolve(p);
      std::cout << "[";
      bool first = true;
      for (auto& s : v) {
        if (!first) {
          std::cout << ", ";
        }
        first = false;
        std::cout << s;
      }
      std::cout << "]" << std::endl;
      auto now = std::chrono::steady_clock::now();
      auto duration
        = std::chrono::duration_cast<std::chrono::milliseconds>(now - clock);
      std::cout << "with no heuristic: " << duration.count() << "ms" << std::endl;
    }

    {
      NewHeuristic h(heuristic(size, p.goal()));
      auto clock = std::chrono::steady_clock::now();
      auto v = jsolve_with_heuristic(p, h);
      std::cout << "[";
      bool first = true;
      for (auto& s : v) {
        if (!first) {
          std::cout << ", ";
        }
        first = false;
        std::cout << s;
      }
      std::cout << "]" << std::endl;
      auto now = std::chrono::steady_clock::now();
      auto duration
        = std::chrono::duration_cast<std::chrono::milliseconds>(now - clock);
      std::cout << "with heuristic: " << duration.count() << "ms" << std::endl;
    }
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  JavaCPP_uninit();
}
