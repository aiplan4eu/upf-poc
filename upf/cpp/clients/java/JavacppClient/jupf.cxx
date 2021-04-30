#include "jupf.hxx"

#include "upf.hxx"
#include <iostream>

namespace jupf
{

  std::optional<std::vector<std::string>> solve(std::string planner, upf::Problem problem, Heuristic& h)
  {
    return upf::solve(planner, problem, [&](std::set<std::string> x){return h(x);});
  }

  bool set_contains(const std::set<std::string>& s, const std::string& x) {
    return (s.find(x) != s.end());
  }
}
