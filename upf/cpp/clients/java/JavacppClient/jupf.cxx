#include "jupf.hxx"

#include "upf.hxx"
#include <iostream>

namespace jupf
{

  std::optional<std::vector<std::string>> solve(std::string planner, upf::Problem problem, Heuristic& h)
  {
    std::function<double(std::set<std::string>)> heuristic(h);
    std::cout << h({}) << "  " << heuristic({}) << std::endl;
    return upf::solve(planner, problem, h);
  }

  std::optional<std::vector<std::string>> solve(std::string planner, upf::Problem problem)
  {
    return upf::solve(planner, problem);
  }

}
