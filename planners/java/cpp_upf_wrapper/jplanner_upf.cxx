#include "jplanner_upf.hxx"

#include <iostream>
#include <cstdlib>
#include "jniPlannerWrapper.h"


class NewHeuristic : public Heuristic {
public:
  NewHeuristic(std::function<double(std::set<std::string>)>& heuristic) :
    heuristic_(heuristic) {}

  double operator()(const std::set<std::string>& s) override
  {
    return heuristic_(s);
  }

private:
  std::function<double(std::set<std::string>)> heuristic_;
};

void init() {
  if (std::getenv("CLASSPATH")) {
    auto classpath = std::string("-Djava.class.path=")+std::getenv("CLASSPATH");
    const char* a[1] = {classpath.c_str()};
    JavaCPP_init(1, a);
  }
  else {
    JavaCPP_init(0, NULL);
  }
}

void uninit() {
  JavaCPP_uninit();
}

std::optional<std::vector<std::string>> solve_with_heuristic(upf::Problem& problem, std::function<double(std::set<std::string>)> heuristic)
{
  NewHeuristic h(heuristic);
  auto res = jsolve_with_heuristic(problem, h);
  return res;
}

std::optional<std::vector<std::string>> solve(upf::Problem& problem)
{
  auto res = jsolve(problem);
  return res;
}
