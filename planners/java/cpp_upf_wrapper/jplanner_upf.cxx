#include "jplanner_upf.hxx"

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

std::optional<std::vector<std::string>> solve_with_heuristic(upf::Problem& problem, std::function<double(std::set<std::string>)> heuristic)
{
  JavaCPP_init(0, NULL);
  NewHeuristic h(heuristic);
  auto res = jsolve_with_heuristic(problem, h);
  JavaCPP_uninit();
  return res;
}

std::optional<std::vector<std::string>> solve(upf::Problem& problem)
{
  JavaCPP_init(0, NULL);
  auto res = jsolve(problem);
  JavaCPP_uninit();
  return res;
}
