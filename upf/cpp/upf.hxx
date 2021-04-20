#pragma once

#include <string>
#include <set>
#include <vector>
#include <optional>
#include <functional>

#include "problem.hxx"

namespace upf
{

  std::optional<std::vector<std::string>> solve(std::string planner, Problem problem, std::function<double(std::set<std::string>)> heuristic);

  std::optional<std::vector<std::string>> solve(std::string planner, Problem problem);

}
