#pragma once

#include <optional>
#include <functional>
#include <string>
#include <vector>

#include "problem.hxx"

extern "C"
{

  void init();

  void uninit();

  std::optional<std::vector<std::string>> solve_with_heuristic(upf::Problem& problem, std::function<double(std::set<std::string>)> heuristic);

  std::optional<std::vector<std::string>> solve(upf::Problem& problem);

}
