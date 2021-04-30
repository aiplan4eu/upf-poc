#pragma once

#include <string>
#include <set>
#include <vector>
#include <optional>

#include "problem.hxx"

namespace jupf
{

  class Heuristic {
  public:
    virtual double operator()(const std::set<std::string>& s)
    {
      return 0;
    }

    virtual ~Heuristic() {}
  };

  std::optional<std::vector<std::string>> solve(std::string planner, upf::Problem problem, Heuristic& h);

  std::optional<std::vector<std::string>> solve(std::string planner, upf::Problem problem);

  bool set_contains(const std::set<std::string>& s, const std::string& x);

}
